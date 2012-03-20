
#ifndef SEARCHRESULT_HPP
#define SEARCHRESULT_HPP

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "shared.h"
#include "database.h"
#include "../songlist/SongListView.h"
#include "../GuiManager.h"
#include "../widget/button.h"


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
    Button * button_return;
    QLabel * label_current;
    SongListView * song_table;
    QLabel * label_songnumber;
    Button * button_prev;
    Button * button_next;
    QWidget * parentWidget;
    bool eventFilter(QObject * obj, QEvent * event);

private:
    DataSet<Song*> songs;
    KaraokeDatabase database;

private slots:
    void slotReturnButton();

};

#endif /* SEARCHRESULT_HPP */

