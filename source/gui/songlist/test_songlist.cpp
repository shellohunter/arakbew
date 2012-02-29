
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "database.h"
#include "SongListView.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KaraokeDatabase database;
    database.open("./wKaraOK.db");

    DataSet<Song*> songs;
    database.getSongs(songs, 0, 10);


    QWidget * root = new QWidget();
    root->setGeometry(10, 10, 800,300);

    QPushButton * button_back = new QPushButton("Back", root);
    button_back->setGeometry(10, 10, 80, 20);

    QLabel * label_current = new QLabel("American & Ueropean", root);
    label_current->setGeometry(70, 10, 400, 20);

    SongListView * song_table = new SongListView(songs, root);
    song_table->setGeometry(10, 40, 780, 200);

    QLabel * label_songnumber = new QLabel("121/11324", root);
    label_songnumber->setGeometry(10, 270, 80, 20);

    QPushButton * button_prev = new QPushButton("Page Up", root);
    button_prev->setGeometry(70, 270, 80, 20);

    QPushButton * button_next = new QPushButton("Page Down", root);
    button_next->setGeometry(150, 270, 80, 20);

    QHBoxLayout * header = new QHBoxLayout();
    header->addWidget(button_back);
    header->addWidget(label_current);

    QHBoxLayout * footer = new QHBoxLayout();
    footer->addWidget(label_songnumber);
    footer->addWidget(button_prev);
    footer->addWidget(button_next);

    QVBoxLayout * vlayout = new QVBoxLayout();
    vlayout->addLayout(header);
    vlayout->addWidget(song_table);
    vlayout->addLayout(footer);

    song_table->setFocus();
    root->setLayout(vlayout);
    root->show();

    return app.exec();
}


