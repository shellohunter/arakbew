
#include "shared.h"
#include "log.h"

#include "../GuiManager.h"
#include "../background/Background.h"
#include "Login.h"

Login::Login(QWidget * parent) : GuiModule(GUI_MODULE_LOGIN)
{
    parentWidget = parent;
    init();
}

Login::~Login()
{
    exit();
}

int Login::init()
{
    LOG_API();

    root = new QWidget(parentWidget);
    root->setWindowFlags(Qt::FramelessWindowHint);
    root->setAutoFillBackground(true);
    QPalette palette = root->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/dialog_background.png")));
    root->setPalette(palette);
    root->setGeometry(260, 120, 300, 150);
    root->setStyleSheet("QPushButton {background: lightgray; color: blue; font: 14px bold;}");

    kb = new KeyBoard("");
    label_title     = new QLabel(QObject::tr("Welcome to Karaoke!"), root);
    label_title->setStyleSheet("color: red; font: 24px bold;");
    label_title->setAlignment(Qt::AlignCenter);
    label_username  = new QLabel(QObject::tr("username"), root);
    label_username->setStyleSheet("padding-left: 10px;");
    label_password  = new QLabel(QObject::tr("password"), root);
    label_password->setStyleSheet("padding-left: 10px;");
    edit_username   = new InputLine(kb, root);
    edit_password   = new InputLine(kb, root);
    edit_password->echo(false);
    btn_login       = new Button(QObject::tr("Login"), root);
    //btn_login->setStyleSheet("{background: whitesmoke; barder:none; color: blue; font: 16px;}");
    btn_register    = new Button(QObject::tr("Register"), root);
    btn_exit        = new Button(QObject::tr("Exit"), root);


    QGridLayout  *layoutGrid = new QGridLayout();
    layoutGrid->addWidget(label_title, 0,0,1,3);
    layoutGrid->addWidget(label_username, 1,0);
    layoutGrid->addWidget(edit_username, 1,1,1,2);
    layoutGrid->addWidget(label_password, 2,0);
    layoutGrid->addWidget(edit_password, 2,1,1,2);
    layoutGrid->addWidget(btn_login, 3,0);
    layoutGrid->addWidget(btn_register, 3,1);
    layoutGrid->addWidget(btn_exit, 3,2);

    root->setLayout(layoutGrid);

    edit_username->setText("test");
    edit_password->setText("test");

    QObject::connect(btn_login, SIGNAL(clicked()), this, SLOT(slotLogin()));
    QObject::connect(btn_register, SIGNAL(clicked()), this, SLOT(slotRegister()));
    QObject::connect(btn_exit, SIGNAL(clicked()), this, SLOT(slotExit()));

    root->hide();

    return OK;
}


int Login::resume()
{
    LOG_API();
    btn_login->setFocus();
    root->show();
    return OK;
}


int Login::pause()
{
    LOG_API();
    root->hide();
    return OK;
}


int Login::exit()
{
    LOG_API();
    DELETE(root);
    return OK;
}

int Login::processMessage(int msg, void * data)
{
    LOG_API();
    return OK;
}


void Login::slotLogin()
{
    LOG_INFO("login button clicked!\n");
    if(edit_username->text() == "test"
    && edit_password->text() == "test")
    {
        GuiEvent event;
        event.type = GUI_EVENT_RESUME_PAGE;
        event.data.moduleName = (char *)GUI_MODULE_MENU;
        sendEvent(&event);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Invalid username or password!\nPlease input again!");
        msgBox.exec();
    }
}

void Login::slotRegister()
{
    LOG_INFO("register button clicked!\n");
    QMessageBox msgBox;
    msgBox.setText("Function not ready yet. please wait....");
    msgBox.exec();
}

void Login::slotExit()
{
    LOG_INFO("exit button clicked!\n");
    QMessageBox msgBox;
    msgBox.setInformativeText(QObject::tr("Are you sure to quit?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(QMessageBox::Yes == ret)
    {
        qApp->quit();
    }
}

