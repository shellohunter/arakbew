
#include "database.hpp"
#include "stdio.h"
#include <stdlib.h>
#include "sqlite3.h"
#include "string.h"

typedef struct
{
    char * name; /* mandatory */
    char * url; /* mandatory */
    char * artistName;
}SONG;



typedef struct
{
    char * artistName;
    eGender gender; /* male, female, band, unknown...*/
    char * imgurl; /* male, female, band, unknown...*/
}ARTIST;



void Song::toString()
{
    printf("[%02d]. [%s], by [%s], url [%s]. \n",
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

void Artist::toString()
{
    printf("[%02d]. [%s], [%d], url [%s]. \n",
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
    API();
} 

KaraokeDatabase::KaraokeDatabase(string filepath)
{
    API();
    open(filepath);
} 

KaraokeDatabase::~KaraokeDatabase()
{   
    API();

    //close datebase
    sqlite3_close(this->pDatabase);
    this->pDatabase = NULL;
} 

int KaraokeDatabase::open(string filepath)
{
    //open the sqlite database
    sqlite3 *pDatabase = NULL;
    int rc = SQLITE_OK;

    API();
    rc = sqlite3_open(filepath.c_str(), &pDatabase);
    if( rc != SQLITE_OK ){
        printf("<SQlite> Can not open %s\n", filepath.c_str());
        printf("<SQlite> Error message is %s\n", sqlite3_errmsg(pDatabase));
        sqlite3_close(pDatabase);
        return E_SQLITE_FAIL;
    }
    else
    {
        printf("<SQlite> Database %s is open sucessful!!\n", filepath.c_str());
        this->pDatabase = pDatabase;
    }

    return E_OK;
}


sqlite3* KaraokeDatabase::getDatabase()
{
    if (this->pDatabase != NULL)
    {
        return this->pDatabase;
    }
    else
    {
        return NULL;
    }
}

int KaraokeDatabase::close()
{
    API();
    return E_OK;
}

int KaraokeDatabase::execFindCount(string SQL)
{
    const char *pSqlMsg = SQL.c_str();
    int iCnt = 0;
    int rc = SQLITE_OK;
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;

    API();

    if (this->pDatabase == NULL)
    {
        printf("<SQlite> Database error!\n");
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
        printf("Query Error: %s\n", errmsg);
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

    API();
    if(index >= this->getSongNumber())
    {
        return E_FAIL;
    }

    if (this->pDatabase == NULL)
    {
        printf("Database error!\n");
        return E_SQLITE_FAIL;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        printf("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
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
        printf("Query Error: SQLITE ID %d\n", rc);
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
    if (pStmt != NULL)
    {
        sqlite3_finalize(pStmt);
        pStmt = NULL;
    }
    if (pSqlMsg != NULL)
    {
        free(pSqlMsg);
        pSqlMsg =NULL;
    }

    return rc;
}


DataSet<Song*>& KaraokeDatabase::getSongs(DataSet<Song*>& songs, string songName)
{
    char *pSqlMsg = NULL, pPara[SQLMSG_PARA_LEN] = {0};
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt;

    API();

    if (this->pDatabase == NULL)
    {
        printf("Database error!\n");
        //return E_SQLITE_FAIL;
        return songs;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        printf("Cannot malloc memory in %s\n", __FUNCTION__);
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
    printf("<SQlite> Input parameter is %s\n", pPara);

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        printf("Query Error: SQLITE ID %d\n", rc);
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

                songs.data.push_back(song);

                //exec next
                rc = sqlite3_step(pStmt);
            }while(rc == SQLITE_ROW);
        }
    }

    //free pStmt
    if (pStmt != NULL)
    {
        sqlite3_finalize(pStmt);
        pStmt = NULL;
    }
    if (pSqlMsg != NULL)
    {
        free(pSqlMsg);
        pSqlMsg =NULL;
    }

    return songs;
}

DataSet<Song*>& KaraokeDatabase::getSongs(DataSet<Song*>& songs, int startIndex, int num)
{
    char *pSqlMsg = NULL;
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt;

    API();

    if (this->pDatabase == NULL)
    {
        printf("Database error!\n");
        //return E_SQLITE_FAIL;
        return songs;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        printf("Cannot malloc memory in %s\n", __FUNCTION__);
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
        printf("Query Error: SQLITE ID %d\n", rc);
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

                songs.data.push_back(song);
                //exec next
                rc = sqlite3_step(pStmt);

            }while(rc == SQLITE_ROW);
        }
    }

    //free pStmt
    if (pStmt != NULL)
    {
        sqlite3_finalize(pStmt);
        pStmt = NULL;
    }
    if (pSqlMsg != NULL)
    {
        free(pSqlMsg);
        pSqlMsg =NULL;
    }

    return songs;

}

DataSet<Song*>& KaraokeDatabase::getSongsByArtist(DataSet<Song*>& songs, int artistIndex)
{
    Artist artist;
    if(E_OK == getArtist(artist, artistIndex))
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

    API();

    if (this->pDatabase == NULL)
    {
        printf("Database error!\n");
        //return E_SQLITE_FAIL;
        return songs;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        printf("Cannot malloc memory in %s\n", __FUNCTION__);
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
    printf("<SQlite> Input parameter is %s\n", pPara);

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        printf("Query Error: SQLITE ID %d\n", rc);
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

                songs.data.push_back(song);

                //exec next
                rc = sqlite3_step(pStmt);
            }while(rc == SQLITE_ROW);
        }
    }

    //free pStmt
    if (pStmt != NULL)
    {
        sqlite3_finalize(pStmt);
        pStmt = NULL;
    }
    if (pSqlMsg != NULL)
    {
        free(pSqlMsg);
        pSqlMsg =NULL;
    }

    return songs;
}

DataSet<Song*>& KaraokeDatabase::getSongsByAlbumIndex(DataSet<Song*>& songs, int albumIndex)
{
    API();
    return songs;
}

int KaraokeDatabase::getArtist(Artist & artist, int index)
{
    char *pSqlMsg = NULL;
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt, iTmp = 0;

    API();
    if(index >= this->getSongNumber())
    {
        return E_FAIL;
    }

    if (this->pDatabase == NULL)
    {
        printf("Database error!\n");
        return E_SQLITE_FAIL;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        printf("<SQlite> Cannot malloc memory in %s\n", __FUNCTION__);
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
        printf("Query Error: SQLITE ID %d\n", rc);
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
    if (pStmt != NULL)
    {
        sqlite3_finalize(pStmt);
        pStmt = NULL;
    }
    if (pSqlMsg != NULL)
    {
        free(pSqlMsg);
        pSqlMsg =NULL;
    }

    return rc;
}
    
DataSet<Artist*>& KaraokeDatabase::getArtists(DataSet<Artist*>& artists, string artistName)
{
    char *pSqlMsg = NULL, pPara[SQLMSG_PARA_LEN] = {0};
    const char *pMsg = NULL;
    int rc = SQLITE_OK;
    sqlite3_stmt *pStmt = NULL;
    int iColumnIdx, iColumnCnt, iTmp = 0;

    API();

    if (this->pDatabase == NULL)
    {
        printf("Database error!\n");
        //return E_SQLITE_FAIL;
        return artists;
    }

    /* malloc for psqlmsg*/
    pSqlMsg = (char*)malloc(SQLMSG_LEN * sizeof(char));
    if (NULL == pSqlMsg)
    {
        printf("Cannot malloc memory in %s\n", __FUNCTION__);
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
    printf("<SQlite> Input parameter is %s\n", pPara);

    /* exec query */
    rc = sqlite3_prepare(
                this->pDatabase,
                pSqlMsg,
                SQLMSG_AUTO_LEN,
                &pStmt,
                0);
    if( rc != SQLITE_OK )
    {
        printf("Query Error: SQLITE ID %d\n", rc);
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

                artists.data.push_back(artist);

                //exec next
                rc = sqlite3_step(pStmt);
            }while(rc == SQLITE_ROW);
        }
    }

    //free pStmt
    if (pStmt != NULL)
    {
        sqlite3_finalize(pStmt);
        pStmt = NULL;
    }
    if (pSqlMsg != NULL)
    {
        free(pSqlMsg);
        pSqlMsg =NULL;
    }

    return artists;

}


