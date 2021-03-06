#ifndef MENU_HPP
#define MENU_HPP

#include <QtGui>
#include <QtCore>
#include "../GuiManager.h"
#include "../widget/button.h"

class Menu : public GuiModule
{
Q_OBJECT

public:
    Menu(QWidget * parent = NULL);
    virtual ~Menu();

    int init();
    int resume();
    int pause();
    int exit();
    int processMessage(int msg, void * data);

private:
    QWidget * root;
    Button * btn_to_player;
    Button * btn_to_playlist;
    Button * btn_to_category;
    Button * btn_to_logout;
    QWidget * parentWidget;

public slots:
    void slot2Player();
    void slot2Playlist();
    void slot2Category();
    void slot2Logout();
};

#endif /* MENU_HPP */
