
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "database.hpp"
#include "SongListView.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KaraokeDatabase database;
    database.open("./wKaraOK.db");

    DataSet<Song*> songs;
    database.getSongs(songs, 0, 10);

    SongListView table(songs);
    table.show();

    return app.exec();
}


