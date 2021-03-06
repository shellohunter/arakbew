
#ifndef PLAYER_H
#define PLAYER_H

#include <QtGui>
#include <QtCore>
#include "../GuiManager.h"
#ifdef WINDOWS
#include "win32/playback.h"
#else
#include "playback.h"
#endif /* WINDOWS */

#include "../widget/button.h"

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

    Playback * player;

private:

    QWidget * root;

    Button * btn_prev;
    Button * btn_pause;
    Button * btn_play;
    Button * btn_stop;
    Button * btn_next;
    QWidget * parentWidget;

    bool eventFilter(QObject * obj, QEvent * event);

public slots:
    void slotStop();
    void slotNext();
    void slotPlay();
    void slotPlay(const char * path);
    void slotPause();
    void slotPrev();

signals:
    void signalPlayStart();
    void signalPlayStop();

};






#endif /* PLAYER_H */

