
#include "shared.h"
#include "log.h"

#include "../GuiManager.h"
#include "../background/Background.h"
#include "menu.h"



Menu::Menu(QWidget * parent) : GuiModule(GUI_MODULE_MENU)
{
    LOG_API();
    parentWidget = parent;
    init();
}


Menu::~Menu()
{
    LOG_API();
    exit();
}

int Menu::init()
{
    LOG_API();

    root = new QWidget(parentWidget);
    root->setWindowFlags(Qt::FramelessWindowHint);
    root->setGeometry(260, 80, 300, 350);

    QFont font;
    font.setPointSize(16);
    font.setBold(true);

    btn_to_category = new Button(QObject::tr("Select Songs"), root);
    btn_to_category->setIcon(QPixmap(":/images/menu-goto-select.png"));  
    btn_to_category->setIconSize(QSize(64,64)); 
    btn_to_category->setFont(font);

    btn_to_playlist = new Button(QObject::tr("View Playlist"), root);
    btn_to_playlist->setIcon(QPixmap(":/images/menu-goto-playlist.png"));  
    btn_to_playlist->setIconSize(QSize(64,64)); 
    btn_to_playlist->setFont(font);

    btn_to_player   = new Button(QObject::tr("Goto Player"), root);
    btn_to_player->setIcon(QPixmap(":/images/menu-goto-player.png"));  
    btn_to_player->setIconSize(QSize(64,64)); 
    btn_to_player->setFont(font);

    btn_to_logout   = new Button(QObject::tr("Log Out"), root);
    btn_to_logout->setIcon(QPixmap(":/images/menu-goto-logout.png"));  
    btn_to_logout->setIconSize(QSize(64,64)); 
    btn_to_logout->setFont(font);

    QVBoxLayout  *layout = new QVBoxLayout();
    layout->addWidget(btn_to_category);
    layout->addSpacing(10);
    layout->addWidget(btn_to_playlist);
    layout->addSpacing(10);
    layout->addWidget(btn_to_player);
    layout->addSpacing(10);
    layout->addWidget(btn_to_logout);

    root->setLayout(layout);

    QObject::connect(btn_to_category, SIGNAL(clicked()), this, SLOT(slot2Category()));
    QObject::connect(btn_to_playlist, SIGNAL(clicked()), this, SLOT(slot2Playlist()));
    QObject::connect(btn_to_player, SIGNAL(clicked()), this, SLOT(slot2Player()));
    QObject::connect(btn_to_logout, SIGNAL(clicked()), this, SLOT(slot2Logout()));

    root->hide();

    return OK;
}


int Menu::resume()
{
    LOG_API();
    btn_to_category->setFocus();
    root->show();
    return OK;
}


int Menu::pause()
{
    LOG_API();
    root->hide();
    return OK;
}


int Menu::exit()
{
    LOG_API();
    DELETE(root);
    return OK;
}

int Menu::processMessage(int msg, void * data)
{
    LOG_API();
    return OK;
}


void Menu::slot2Player()
{
    LOG_API();
    GuiEvent event;
    event.type = GUI_EVENT_RESUME_PAGE;
    event.data.moduleName = (char *)GUI_MODULE_PLAYER;
    sendEvent(&event);
}


void Menu::slot2Playlist()
{
    LOG_API();
    GuiEvent event;
    event.type = GUI_EVENT_RESUME_PAGE;
    event.data.moduleName = (char *)GUI_MODULE_PLAYLIST;
    sendEvent(&event);
}


void Menu::slot2Category()
{
    LOG_API();
    GuiEvent event;
    event.type = GUI_EVENT_RESUME_PAGE;
    event.data.moduleName = (char *)GUI_MODULE_CATEGORY;
    sendEvent(&event);
}

void Menu::slot2Logout()
{
    LOG_API();
    QMessageBox msgBox;
    msgBox.setInformativeText(QObject::tr("Are you sure to log off?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if(QMessageBox::Yes == ret)
    {
        GuiEvent event;
        event.type = GUI_EVENT_RESUME_PAGE;
        event.data.moduleName = (char *)GUI_MODULE_LOGIN;
        sendEvent(&event);
    }
}


