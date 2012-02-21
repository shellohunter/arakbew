
#ifndef SEARCHRESULT_HPP
#define SEARCHRESULT_HPP

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "shared.hpp"
#include "database.hpp"
#include "../songlist/SongListView.hpp"
#include "../GuiManager.hpp"


class SearchResult : public GuiModule
{
Q_OBJECT

public:
    SearchResult(QWidget * parent = NULL);
    ~SearchResult();

    int resume();
    int pause();
    int init();
    int exit();
    int processMessage(int msg, void * data);


    int loadDataSet(DataSet<Song*>& songs);

private:
    QWidget * root;
    QPushButton * button_back;
    QLabel * label_current;
    SongListView * song_table;
    QLabel * label_songnumber;
    QPushButton * button_prev;
    QPushButton * button_next;
    QWidget * parentWidget;

private:
    DataSet<Song*> songs;
};

#endif /* SEARCHRESULT_HPP */

