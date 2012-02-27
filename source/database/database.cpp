
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.hpp"

int Song::counter = 0; /* for dbg only */

void Song::toString()
{
    LOG_INFO("<Song> %02d. [%s], [%s], by [%s]. \n",
        index, 
        name.c_str(),
        url.c_str(),
        artistName.c_str());
}


bool Song::operator==(const Song& song)
{
    if(this->index == song.index && this->name ==  song.name)
        return true;
    else
        return false;
}

int Artist::counter = 0; /* for dbg only */

void Artist::toString()
{
    LOG_INFO("<Aritist> %02d. [%s], [%d], url [%s]. \n",
        index, 
        name.c_str(),
        gender,
        imgurl.c_str());
}

bool Artist::operator==(const Artist& artist)
{
    if(this->index == artist.index && this->name ==  artist.name)
        return true;
    else
        return false;
}


KaraokeDatabase::KaraokeDatabase()
{
    LOG_API();
} 

KaraokeDatabase::KaraokeDatabase(string filepath)
{
    LOG_API();
    open(filepath);
} 

KaraokeDatabase::~KaraokeDatabase()
{   
    LOG_API();
    close();
} 

int KaraokeDatabase::open(string filepath)
{
    //open the sqlite database
    sqlite3 *pDatabase = NULL;
    int rc = SQLITE_OK;

    LOG_API();
    rc = sqlite3_open(filepath.c_str(), &pDatabase);
    if( rc != SQLITE_OK ){
        LOG_ERROR("<SQlite> Can not open %s\n", filepath.c_str());
        LOG_ERROR("<SQlite> Error message is %s\n", sqlite3_errmsg(pDatabase));
        sqlite3_close(pDatabase);
        return E_SQLITE_FAIL;
    }
    else
    {
        LOG_INFO("<SQlite> Open %s sucessfully!!\n", filepath.c_str());
        this->pDatabase = pDatabase;
    }

    return OK;
}



int KaraokeDatabase::close()
{
    LOG_API();
    //close datebase
    sqlite3_close(this->pDatabase);
    this->pDatabase = NULL;
    return OK;
}


sqlite3* KaraokeDatabase::getDatabase()
{
    return this->pDatabase;
}


int KaraokeDatabase::execFindCount(string SQL)
{
    const char *pSqlMsg = SQL.c_str();
    int iCnt = 0;
    int rc = SQLITE_OK;
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;

    LOG_API();

    if (this->pDatabase == NULL)
    {
        LOG_ERROR("<SQlite> Database error!\n");
        return E_SQLITE_FAIL;
    }

    /* exec query */
    rc = sqlite3_get_table(this->pDatabase,
                           pSqlMsg,
                           &dbResult,
                           &nRow,
                           &nColumn,
                           &errmsg);
    if(rc == SQLITE_OK)
    {
        //query sucess
        iCnt = atoi(dbResult[nColumn]);
    }
    else
    {
        iCnt = 0;
        LOG_ERROR("<SQlite> Query Error: %s\n", errmsg);
    }

    //free dbResult
    sqlite3_free_table(dbResult);

    return iCnt;
}


int KaraokeDatabase::getSongNumber()
{
    return execFindCount("SELECT COUNT(*) AS count_song FROM song_list");
}

int KaraokeDatabase::getArtistNumber()
{
     return execFindCount("SELECT COUNT(*) AS artist_song FROM artist");
}

int KaraokeDatabase::getAlbumNumber()
{
    return 0;
}

int KaraokeDatabase::getSong(Song & song, int index)
{
    char *pSqlMsg = NULL;
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt;

    LOG_API();
    if(index >= this->getSongNumber())
    {
        return FAIL;
    }

    if (this->pDatabase == NULL)
    {
        LOG_ERROR("<SQlite> Database error!\n");
        return E_SQLITE_FAIL;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        LOG_ERROR("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
        return E_OUT_OF_MEMORY;
    }
    memset(pSqlMsg, 0,SQLMSG_LEN * sizeof(char));

    pMsg = "select song_list.[_id], song_list.[name],song_list.[url], artist.[artist_name],song_list.[artist_no], song_list.[category]"
            "from song_list inner join artist "
            "where song_list.[artist_no] = artist.[_id] and song_list.[_id]=? "
            "order by song_list.[_id] asc";
    strncpy(pSqlMsg, pMsg, strlen(pMsg));

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        LOG_ERROR("<SQlite> Query Error: SQLITE ID %d\n", rc);
        rc = E_SQLITE_FAIL;
    }
    else
    {
        /* fill the first parameter with index+1 */
        rc = sqlite3_bind_int(pStmt, 1, index + 1);
        rc = sqlite3_step(pStmt);
        if(rc == SQLITE_ROW)
        {
            iColumnCnt = sqlite3_column_count(pStmt);
            for(iColumnIdx =0; iColumnIdx < iColumnCnt; iColumnIdx++)
            {
                if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "name") == 0)
                {
                    song.name = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                }
                else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "url") == 0)
                {
                    song.url = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                }
                else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_name") == 0)
                {
                    song.artistName = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                }
                else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_no") == 0)
                {
                    song.artistIndex = sqlite3_column_int(pStmt, iColumnIdx);
                }
                else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "category") == 0)
                {
                    song.category = sqlite3_column_int(pStmt, iColumnIdx);
                }
            }
            song.albumName = "unknown";
            song.index = index;
        }
    }

    //free pStmt
    sqlite3_finalize(pStmt);
    FREE(pSqlMsg);

    return rc;
}


DataSet<Song*>& KaraokeDatabase::getSongs(DataSet<Song*>& songs, string songName)
{
    char *pSqlMsg = NULL, pPara[SQLMSG_PARA_LEN] = {0};
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt;

    LOG_API();

    if (this->pDatabase == NULL)
    {
        LOG_ERROR("<SQlite> Database error!\n");
        //return E_SQLITE_FAIL;
        return songs;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        LOG_ERROR("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
        //return E_OUT_OF_MEMORY;
        return songs;
    }
    memset(pSqlMsg, 0,SQLMSG_LEN * sizeof(char));

    pMsg = "select song_list.[_id], song_list.[name],song_list.[url], artist.[artist_name],song_list.[artist_no], song_list.[category] "
            "from song_list inner join artist "
            "where song_list.[artist_no] = artist.[_id] and song_list.[name] like ? "
            "order by song_list.[_id] asc";;
    strncpy(pSqlMsg, pMsg, strlen(pMsg));

    /* malloc for query parameter */
    strncpy(pPara, "%", 1);
    strncat(pPara, songName.c_str(), strlen(songName.c_str()));
    strncat(pPara, "%", 1);
    LOG_INFO("<SQlite> Input parameter is %s\n", pPara);

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        LOG_ERROR("<SQlite> Query Error: SQLITE ID %d\n", rc);
        rc = E_SQLITE_FAIL;
    }
    else
    {
        /* fill the first parameter with index+1 */
        sqlite3_bind_text(pStmt, 1, pPara, strlen(pPara), NULL);
        rc = sqlite3_step(pStmt);
        if(rc == SQLITE_ROW)
        {
            iColumnCnt = sqlite3_column_count(pStmt);
            do
            {
                Song * song = new Song();
                for(iColumnIdx = 0; iColumnIdx < iColumnCnt; iColumnIdx++)
                {
                    if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "name") == 0)
                    {
                        song->name = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "url") == 0)
                    {
                        song->url = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_name") == 0)
                    {
                        song->artistName = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "_id") == 0)
                    {
                        song->index = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_no") == 0)
                    {
                        song->artistIndex = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "category") == 0)
                    {
                        song->category = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                }
                song->albumName = "unknown";

                songs.push_back(song);

                //exec next
                rc = sqlite3_step(pStmt);
            }while(rc == SQLITE_ROW);
        }
    }

    //free pStmt
    sqlite3_finalize(pStmt);
    FREE(pSqlMsg);

    return songs;
}

DataSet<Song*>& KaraokeDatabase::getSongs(DataSet<Song*>& songs, int startIndex, int num)
{
    char *pSqlMsg = NULL;
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt;

    LOG_API();

    if (this->pDatabase == NULL)
    {
        LOG_ERROR("<SQlite> Database error!\n");
        //return E_SQLITE_FAIL;
        return songs;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        LOG_ERROR("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
        //return E_OUT_OF_MEMORY;
        return songs;
    }
    memset(pSqlMsg, 0,SQLMSG_LEN * sizeof(char));

    pMsg = "select song_list.[_id], song_list.[name],song_list.[url], artist.[artist_name],song_list.[artist_no], song_list.[category] "
            "from song_list inner join artist "
            "where song_list.[artist_no] = artist.[_id] and song_list.[_id] >= ? and song_list.[_id] <= ? "
            "order by song_list.[_id] asc";;
    strncpy(pSqlMsg, pMsg, strlen(pMsg));

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        LOG_ERROR("<SQlite> Query Error: SQLITE ID %d\n", rc);
        rc = E_SQLITE_FAIL;
    }
    else
    {
        /* fill the first parameter with index+1 */
        rc = sqlite3_bind_int(pStmt, 1, startIndex + 1);
        rc = sqlite3_bind_int(pStmt, 2, startIndex + num);
        rc = sqlite3_step(pStmt);
        if(rc == SQLITE_ROW)
        {
            iColumnCnt = sqlite3_column_count(pStmt);
            do
            {
                Song * song = new Song();
                for(iColumnIdx = 0; iColumnIdx < iColumnCnt; iColumnIdx++)
                {
                    if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "name") == 0)
                    {
                        song->name = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "url") == 0)
                    {
                        song->url = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_name") == 0)
                    {
                        song->artistName = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "_id") == 0)
                    {
                        song->index = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_no") == 0)
                    {
                        song->artistIndex = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "category") == 0)
                    {
                        song->category = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                }
                song->albumName = "unknown";

                songs.push_back(song);
                //exec next
                rc = sqlite3_step(pStmt);

            }while(rc == SQLITE_ROW);
        }
    }


    //free pStmt
    sqlite3_finalize(pStmt);
    FREE(pSqlMsg);

    return songs;

}

DataSet<Song*>& KaraokeDatabase::getSongsByArtist(DataSet<Song*>& songs, int artistIndex)
{
    Artist artist;
    if(OK == getArtist(artist, artistIndex))
    {
        return getSongsByArtist(songs, artist.name);
    }
    else
    {
        return songs;
    }
        
}

DataSet<Song*>& KaraokeDatabase::getSongsByArtist(DataSet<Song*>& songs, string artistName)
{
    char *pSqlMsg = NULL, pPara[SQLMSG_PARA_LEN] = {0};
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt;

    LOG_API();

    if (this->pDatabase == NULL)
    {
        LOG_ERROR("<SQlite> Database error!\n");
        //return E_SQLITE_FAIL;
        return songs;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        LOG_ERROR("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
        //return E_OUT_OF_MEMORY;
        return songs;
    }
    memset(pSqlMsg, 0,SQLMSG_LEN * sizeof(char));

    pMsg = "select song_list.[_id], song_list.[name],song_list.[url], artist.[artist_name], song_category.[type], song_list.[artist_no], song_list.[category] "
           "from song_list "
           "inner join artist, song_category "
           "where song_list.[artist_no] = artist.[_id] "
           "and song_list.[category] = song_category.[_id] "
           "and artist.[artist_name] like ? "
           "order by song_list.[_id] asc";
    strncpy(pSqlMsg, pMsg, strlen(pMsg));

    /* malloc for query parameter */
    strncpy(pPara, "%", 1);
    strncat(pPara, artistName.c_str(), strlen(artistName.c_str()));
    strncat(pPara, "%", 1);
    LOG_INFO("<SQlite> Input parameter is %s\n", pPara);

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        LOG_ERROR("<SQlite> Query Error: SQLITE ID %d\n", rc);
        rc = E_SQLITE_FAIL;
    }
    else
    {
        /* fill the first parameter with index+1 */
        sqlite3_bind_text(pStmt, 1, pPara, strlen(pPara), NULL);
        rc = sqlite3_step(pStmt);
        if(rc == SQLITE_ROW)
        {
            iColumnCnt = sqlite3_column_count(pStmt);
            do
            {
                Song * song = new Song();
                for(iColumnIdx = 0; iColumnIdx < iColumnCnt; iColumnIdx++)
                {
                    if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "name") == 0)
                    {
                        song->name = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "url") == 0)
                    {
                        song->url = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_name") == 0)
                    {
                        song->artistName = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "_id") == 0)
                    {
                        song->index = sqlite3_column_int(pStmt, iColumnIdx);
                    }//artist_no
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_no") == 0)
                    {
                        song->artistIndex = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "category") == 0)
                    {
                        song->category = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                }
                song->albumName = "unknown";

                songs.push_back(song);

                //exec next
                rc = sqlite3_step(pStmt);
            }while(rc == SQLITE_ROW);
        }
    }

    //free pStmt
    sqlite3_finalize(pStmt);
    FREE(pSqlMsg);

    return songs;
}

DataSet<Song*>& KaraokeDatabase::getSongsByAlbumIndex(DataSet<Song*>& songs, int albumIndex)
{
    LOG_API();
    return songs;
}

int KaraokeDatabase::getArtist(Artist & artist, int index)
{
    char *pSqlMsg = NULL;
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt, iTmp = 0;

    LOG_API();
    if(index >= this->getSongNumber())
    {
        return FAIL;
    }

    if (this->pDatabase == NULL)
    {
        LOG_ERROR("<SQlite> Database error!\n");
        return E_SQLITE_FAIL;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        LOG_INFO("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
        return E_OUT_OF_MEMORY;
    }
    memset(pSqlMsg, 0,SQLMSG_LEN * sizeof(char));

    pMsg = "select  artist.[_id], artist.[artist_name], artist.[img_url], artist.[idx_info],artist.[artist_category_no],artist_category.[name] from artist "
           "inner join artist_category "
           "where artist.[artist_category_no]= artist_category.[_id] "
           "and artist.[_id] = ? ";
    strncpy(pSqlMsg, pMsg, strlen(pMsg));

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        LOG_ERROR("<SQlite> Query Error: SQLITE ID %d\n", rc);
        rc = E_SQLITE_FAIL;
    }
    else
    {
        /* fill the first parameter with index+1 */
        rc = sqlite3_bind_int(pStmt, 1, index + 1);
        rc = sqlite3_step(pStmt);
        if(rc == SQLITE_ROW)
        {
            iColumnCnt = sqlite3_column_count(pStmt);
            for(iColumnIdx =0; iColumnIdx < iColumnCnt; iColumnIdx++)
            {
                if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_name") == 0)
                {
                    artist.name = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                }
                else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "url") == 0)
                {
                    artist.imgurl = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                }
                else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_category_no") == 0)
                {
                    iTmp = sqlite3_column_int(pStmt, iColumnIdx);
                    if (iTmp == 1)
                    {
                        artist.gender = E_MALE;
                    }
                    else if (iTmp == 2)
                    {
                        artist.gender = E_FEMALE;
                    }
                    else if (iTmp == 3)
                    {
                        artist.gender = E_BAND;
                    }
                    else
                    {
                        artist.gender = E_UNKNOWN;
                    }
                }
            }

            artist.index = index;
        }
    }

    //free pStmt
    sqlite3_finalize(pStmt);
    FREE(pSqlMsg);

    return rc;
}
    
DataSet<Artist*>& KaraokeDatabase::getArtists(DataSet<Artist*>& artists, string artistName)
{
    char *pSqlMsg = NULL, pPara[SQLMSG_PARA_LEN] = {0};
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt, iTmp = 0;

    LOG_API();

    if (this->pDatabase == NULL)
    {
        LOG_ERROR("<SQlite> Database error!\n");
        //return E_SQLITE_FAIL;
        return artists;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        LOG_ERROR("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
        //return E_OUT_OF_MEMORY;
        return artists;
    }
    memset(pSqlMsg, 0,SQLMSG_LEN * sizeof(char));

    pMsg = "select artist.[_id], artist.[artist_name], artist.[img_url], artist.[artist_category_no], artist.[idx_info],artist_category.[name] "
           "from artist "
           "inner join artist_category "
           "where artist.[artist_category_no]= artist_category.[_id] "
           "and artist.[artist_name] like ? "
           "order by artist.[_id] asc";
    strncpy(pSqlMsg, pMsg, strlen(pMsg));

    /* malloc for query parameter */
    strncpy(pPara, "%", 1);
    strncat(pPara, artistName.c_str(), strlen(artistName.c_str()));
    strncat(pPara, "%", 1);
    LOG_INFO("<SQlite> Input parameter is %s\n", pPara);

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        LOG_ERROR("<SQlite> Query Error: SQLITE ID %d\n", rc);
        rc = E_SQLITE_FAIL;
    }
    else
    {
        /* fill the first parameter with index+1 */
        sqlite3_bind_text(pStmt, 1, pPara, strlen(pPara), NULL);
        rc = sqlite3_step(pStmt);
        if(rc == SQLITE_ROW)
        {
            iColumnCnt = sqlite3_column_count(pStmt);
            do
            {
                Artist * artist = new Artist();
                for(iColumnIdx = 0; iColumnIdx < iColumnCnt; iColumnIdx++)
                {
                    if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "img_url") == 0)
                    {
                        artist->imgurl = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_name") == 0)
                    {
                        artist->name = (char*)sqlite3_column_text(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "_id") == 0)
                    {
                        artist->index = sqlite3_column_int(pStmt, iColumnIdx);
                    }
                    else if(strcmp(sqlite3_column_name(pStmt, iColumnIdx), "artist_category_no") == 0)
                    {
                        iTmp = sqlite3_column_int(pStmt, iColumnIdx);
                        if (iTmp == 1)
                        {
                            artist->gender = E_MALE;
                        }
                        else if (iTmp == 2)
                        {
                            artist->gender = E_FEMALE;
                        }
                        else if (iTmp == 3)
                        {
                            artist->gender = E_BAND;
                        }
                        else
                        {
                            artist->gender = E_UNKNOWN;
                        }
                    }
                }

                artists.push_back(artist);

                //exec next
                rc = sqlite3_step(pStmt);
            }while(rc == SQLITE_ROW);
        }
    }

    //free pStmt
    sqlite3_finalize(pStmt);
    FREE(pSqlMsg);

    return artists;

}


