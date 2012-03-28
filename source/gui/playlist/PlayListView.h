
#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "shared.h"
#include "../GuiManager.h"
#include "database.h"
#include "../widget/button.h"
#include "PlayListItem.h"

#define PLAYLISTVIEW_ITEM_NUM (8)

class PlayListView : public GuiModule
{
    Q_OBJECT


    typedef struct
    {
        QWidget * focus;
        QWidget * btn_up;
        QWidget * btn_down;
        QWidget * btn_left;
        QWidget * btn_right;
    } Navigation;


public:
    PlayListView(QWidget * parent = NULL);
    ~PlayListView();

    int resume();
    int pause();
    int init();
    int exit();
    int processMessage(int msg, void * data);

    void getCurrentItem() {};


protected:
    bool eventFilter(QObject * obj, QEvent * event);
    int CursorKeyEvent(int key);
    void initNavigation();

private:
    QWidget * root;
    QWidget * parentWidget;
    PlayListItem * items[PLAYLISTVIEW_ITEM_NUM];
    Button * btn_return;
    Navigation * navi;
    Button * btn_prev;

    Button * btn_next;
    Button * eventAgent;
    QWidget * currentFocus;
    QWidget * listview;
    QLabel * promptLabel;

protected slots:
    void slotReturnButton();
    void slotContextMenu();
    void slotPageUp();
    void slotPageDown();

    void slotSongAdd(const Song &song);
    void slotSongDel(const Song &song);
    void slotSongStart();

};


#endif // PLAYLISTVIEW_H
