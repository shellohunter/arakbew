
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QtGui>
#include <QtCore>
#include "../GuiManager.h"
#ifdef WINDOWS
#include "win32/playback.h"
#else
#include "playback.h"
#endif /* WINDOWS */


class Player : public GuiModule
{
Q_OBJECT
    
public:
    Player(QWidget * parent = NULL);
    ~Player();

    int resume();
    int pause();
    int init();
    int exit();
    int processMessage(int msg, void * data);

private:
    Playback * player;

    QWidget * root;

    QPushButton * btn_prev;
    QPushButton * btn_play_pause;
    QPushButton * btn_next;
    QWidget * parentWidget;

    bool eventFilter(QObject * obj, QEvent * event);

public slots:
    void slotStop();
    void slotNext();
    void slotPrev();
    void slotPlayPause();
};

#endif /* PLAYER_HPP */

