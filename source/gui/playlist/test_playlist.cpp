

#include "database.hpp"
#include "playlist.hpp"
#include "stdio.h"

int main()
{
    printf("playlist test!\n");   
    log_init();
    
    KaraokeDatabase database;
    database.open("./wKaraOK.db");


    DataSet<Song*> songs;
    PlayList playlist;

    database.getSongs(songs,0,5);
    songs.toString();
    for(int i=0;i<songs.size();i++)
    {
        songs.at(i)->toString();
        playlist.append(*songs.at(i));
    }
    songs.clear();

    playlist.toString();

    Song a;
    a.index = 3;
    a.name = "poker face";
    printf("has \"poker face\"? %s\n", playlist.has(a)?"yes":"no");

    playlist.moveForward(a);
    playlist.toString();

    playlist.moveTop(a);
    playlist.toString();

    playlist.moveBottom(a);
    playlist.toString();

    Song b;
    b.index = 3;
    b.name = "no such song";
    printf("has \"no such song\"? %s\n", playlist.has(b)?"yes":"no");

    {
        Song d = playlist.pop();

        /* pop() returns a copy of the object. so it's ok to delete original object now. */
        playlist.clear();
        playlist.toString();


        d.toString();
        d.index = 99;
        d.toString();
        /* c++ is smart. after this . song "d" will be deconstructed.*/
    }

    printf("soga!\n");

    return 0;
}
























