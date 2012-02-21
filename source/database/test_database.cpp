#include "database.hpp"
#include "stdio.h"

int main()
{
    printf("sqlite wrapper test!\n");   
    
    KaraokeDatabase database;
    database.open("./wKaraOK.db");

    int songTotal = database.getSongNumber();
    printf("total song:     %d\n", songTotal);
    printf("total artist:   %d\n", database.getArtistNumber());
    printf("total album:    %d\n", database.getAlbumNumber());

    printf("----------------------------------\n");   
    for(int i=0;i<songTotal;i++)
    {
        Song song;
        database.getSong(song, i);
        song.toString();
    }
    printf("----------------------------------\n");   

    DataSet<Song*> songs;

    database.getSongs(songs, "A");
    songs.toString();
    for(int i=0;i<songs.size();i++)
    {
        songs.at(i)->toString();
    }
    songs.clear();
    printf("----------------------------------\n");
    
    database.getSongs(songs,0,2);
    songs.toString();
    for(int i=0;i<songs.size();i++)
    {
        songs.at(i)->toString();
    }
    songs.clear();
    printf("----------------------------------\n");   

    database.getSongsByArtist(songs,"lady gaga");
    songs.toString();
    for(int i=0;i<songs.size();i++)
    {
        songs.at(i)->toString();
    }
    songs.clear();
    printf("----------------------------------\n");   

    DataSet<Artist*> artists;
    database.getArtists(artists,"lady gaga");
    songs.toString();
    for(int i=0;i<artists.size();i++)
    {
        artists.at(i)->toString();
    }
    artists.clear();
    printf("----------------------------------\n");   

    database.close();

    return 0;
}
