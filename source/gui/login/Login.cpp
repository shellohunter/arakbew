
#include "shared.hpp"
#include "log.hpp"

#include "../GuiManager.hpp"
#include "Login.hpp"

Login::Login(QWidget * parent) : GuiModule(GUI_MODULE_LOGIN)
{
    parentWidget = parent;
    init();
}


int Login::init()
{
    LOG_API();

    root = new QWidget(parentWidget);

    root->setAutoFillBackground(true);
    QPalette palette = root->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/dialog_background.png")));
    root->setPalette(palette);

    KeyBoard * kb = new KeyBoard("");
    label_title     = new QLabel(QObject::tr("Welcome to karaoke!"));
    label_username  = new QLabel(QObject::tr("username"));
    label_password  = new QLabel(QObject::tr("password"));
    edit_username   = new InputLine(kb);
    edit_password   = new InputLine(kb);
    edit_password->echo(false);
    btn_login       = new QPushButton(QObject::tr("Login"));
    btn_register    = new QPushButton(QObject::tr("Register"));
    btn_exit        = new QPushButton(QObject::tr("Exit"));

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
    qDebug("%dx%d %dx%d\n", root->width(), root->height(), parentWidget->width(), parentWidget->height());
    root->move((parentWidget->width()-root->width())/2, (parentWidget->height()-root->height())/2);

    edit_username->setText("test");
    edit_password->setText("test");
    btn_login->setFocus();

    QObject::connect(btn_login, SIGNAL(clicked()), this, SLOT(slotLogin()));
    QObject::connect(btn_register, SIGNAL(clicked()), this, SLOT(slotRegister()));
    QObject::connect(btn_exit, SIGNAL(clicked()), this, SLOT(slotExit()));

    return RET_OK;
}


int Login::resume()
{
    LOG_API();
    root->show();
    return RET_OK;
}


int Login::pause()
{
    LOG_API();
    root->hide();
    return RET_OK;
}


int Login::exit()
{
    LOG_API();
    delete root;
    return RET_OK;
}

int Login::processMessage(int msg, void * data)
{
    LOG_API();
    return RET_OK;
}


void Login::slotLogin()
{
    LOG_INFO("login button clicked!\n");
    if(edit_username->text() == "test"
    && edit_password->text() == "test")
    {
        GuiEvent event;
        event.type = GUI_EVENT_LOGIN_STATUS;
        event.data.loginStatus = true;
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

