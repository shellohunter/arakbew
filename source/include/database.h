#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <vector>
#include "sqlite3.h"
#include "shared.h"

using namespace std;


#define SQLMSG_AUTO_LEN (-1)
#define SQLMSG_LEN (1024)
#define SQLMSG_PARA_LEN (128)

#define DBG_MEMLEAK (1)

typedef enum
{
    E_MALE,
    E_FEMALE,
    E_BAND,
    E_UNKNOWN
}eGender;

typedef enum
{
    E_SELECT_COUNT,
    E_SELECT_ITEM,
    E_UPDATE
    //... Please put your error code here.

}E_OPERATE_TYPE;

class Song
{
public:
    int index; /* mandatory */
    string name; /* mandatory */
    string url; /* mandatory */
    int category;
    int artistIndex;
    string artistName;
    string albumName;

    void toString();
    bool operator==(const Song & song);    

#if DBG_MEMLEAK
public:
    Song() {counter++; LOG_INFO("+Song. %d\n",counter);}
    Song(const Song& song)
    {
        index = song.index;
        name = song.name;
        url = song.url;
        category = song.category;
        artistIndex = song.artistIndex;
        artistName = song.artistName;
        albumName = song.albumName;
        counter++; LOG_INFO("+Song*. %d\n",counter);
    }

    ~Song() {counter--; LOG_INFO("-Song. %d\n",counter);}

private:
    static int counter; /* for dbg only */

#endif
};

class Artist
{
public:

    int index; /* mandatory */
    string name; /* mandatory */
    eGender gender;
    string imgurl;

    void toString();
    bool operator==(const Artist & song);    

#if DBG_MEMLEAK
public:
    Artist() {counter++; LOG_INFO("+Artist. %d\n",counter);}
    ~Artist() {counter--; LOG_INFO("-Artist. %d\n",counter);}

private:
    static int counter; /* for dbg only */
#endif
};


/* DataSet only accept pointers which are dynamic allocated. */
template <typename T>
class DataSet : public vector<T>
{
public:
    virtual ~DataSet()
    {
        this->clear();
    }
    void clear()
    {
        for(unsigned i=0; i<this->size(); i++)
        {
            delete this->at(i);
        }
        vector<T>::clear();
    }
    void toString()
    {
        printf("<database> %d records in all.\n", this->size());
    }
};

class KaraokeDatabase
{
public:
    KaraokeDatabase();
    KaraokeDatabase(string filepath);
    ~KaraokeDatabase();

    int open(string filepath);
    int close();

    int getSongNumber();
    int getArtistNumber();
    int getAlbumNumber();

    /* song */
    int getSong(Song & song, int index);
    DataSet<Song*>& getSongs(DataSet<Song*>& songs, string songName);
    DataSet<Song*>& getSongs(DataSet<Song*>& songs, int startIndex = 0, int num = 10);
    DataSet<Song*>& getSongsByArtist(DataSet<Song*>& songs, int artistIndex);
    DataSet<Song*>& getSongsByArtist(DataSet<Song*>& songs, string artistName);
    DataSet<Song*>& getSongsByAlbumIndex(DataSet<Song*>& songs, int albumIndex);

    /* artist */
    int getArtist(Artist& artist, int index);
    DataSet<Artist*>& getArtists(DataSet<Artist*>& artists, string artistName);

protected:
    int execFindCount(string SQL);

private:
    sqlite3 *pDatabase;
    sqlite3* getDatabase();

};

#endif /* DATABASE_HPP */
