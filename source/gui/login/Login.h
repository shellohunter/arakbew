#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <QtGui>
#include <QtCore>
#include "../GuiManager.h"
#include "../widget/inputline.h"
#include "../widget/keyboard.h"
#include "../widget/button.h"

class Login : public GuiModule
{
Q_OBJECT

public:
    Login(QWidget * parent = NULL);
    ~Login();

    int resume();
    int pause();
    int init();
    int exit();
    int processMessage(int msg, void * data);

private:
    QWidget * root;
    QLabel * label_title;
    QLabel * label_username;
    QLabel * label_password;
    InputLine * edit_username;
    InputLine * edit_password;
    Button * btn_login;
    Button * btn_register;
    Button * btn_exit;
    QWidget * parentWidget;
    KeyBoard * kb;

public slots:
    void slotLogin();
    void slotRegister();
    void slotExit();
};

#endif /* LOGIN_HPP */
