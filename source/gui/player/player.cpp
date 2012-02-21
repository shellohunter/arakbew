

#include "../GuiManager.hpp"
#include "shared.hpp"
#include "player.hpp"
#include "../background/Background.hpp"



Player::Player(QWidget * parent) : GuiModule(GUI_MODULE_PLAYER)
{
    LOG_API();
    parentWidget = parent;
    this->init();
}

Player::~Player()
{
    LOG_API();
    this->exit();
}

int Player::init()
{
    LOG_API();
    root = new QWidget(parentWidget);

    root->setAutoFillBackground(true);
    QPalette palette = root->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/dialog_background.png")));
    root->setPalette(palette);
    root->setGeometry(SCREEN_WIDTH/2-140,SCREEN_HEIGHT-40,30,130);

    btn_prev = new QPushButton(QObject::tr("Previous"), root);
    btn_prev->setGeometry(0,0,30,30);
    btn_play_pause = new QPushButton(QObject::tr("Play"), root);
    btn_play_pause->setGeometry(50,0,30,30);
    btn_next = new QPushButton(QObject::tr("Next"), root);
    btn_next->setGeometry(100,0,30,30);


    connect(btn_prev, SIGNAL(clicked()), this, SLOT(slotPrev()));
    connect(btn_play_pause, SIGNAL(clicked()), this, SLOT(slotPlayPause()));
    connect(btn_next, SIGNAL(clicked()), this, SLOT(slotNext()));

    root->hide();

    return OK;
}

int Player::resume()
{
    LOG_API();
    btn_play_pause->setFocus();
    root->show();
    return OK;
}


int Player::pause()
{
    LOG_API();
    root->hide();
    return OK;
}


int Player::exit()
{
    LOG_API();
    DELETE(root);
    return OK;
}

int Player::processMessage(int msg, void * data)
{
    LOG_API();
    return OK;
}



void Player::slotStop()
{
    LOG_API();


}



void Player::slotNext()
{
    LOG_API();



}


void Player::slotPrev()
{
    LOG_API();


}



void Player::slotPlayPause()
{
    LOG_API();

}



