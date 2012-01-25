#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <QtGui>
#include <QtCore>
#include "../GuiManager.hpp"

class Login : public GuiModule
{
Q_OBJECT
    
public:
    Login(QWidget * parent = NULL);

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
    QLineEdit * edit_username;
    QLineEdit * edit_password;
    QPushButton * btn_login;
    QPushButton * btn_register;
    QPushButton * btn_exit;
    QWidget * parentWidget;
    
public slots:
    void slotLogin();
    void slotRegister();
    void slotExit();
};

#endif /* LOGIN_HPP */
