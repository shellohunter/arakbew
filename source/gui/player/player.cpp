

#include "../GuiManager.h"
#include "shared.h"
#include "player.h"
#include "../background/Background.h"


static void player_callback(PB_NOTIFY_T *tNotifyInf)
{
    LOG_INFO("<player> %s(%d)\n",__FUNCTION__,tNotifyInf->eNfyTp);
}


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

    player = new Playback("player", player_callback);

    root = new QWidget(parentWidget);
    root->setGeometry(SCREEN_WIDTH/2-140,SCREEN_HEIGHT-40,200,50);

    root->installEventFilter(this);

    QHBoxLayout * layout = new QHBoxLayout(); 

    btn_prev = new QPushButton(QObject::tr("Previous"), root);
    //btn_prev->setGeometry(0,0,30,30);
    btn_play_pause = new QPushButton(QObject::tr("Play"), root);
    //btn_play_pause->setGeometry(50,0,30,30);
    btn_next = new QPushButton(QObject::tr("Next"), root);
    //btn_next->setGeometry(100,0,30,30);

    layout->addWidget(btn_prev);
    layout->addWidget(btn_play_pause);
    layout->addWidget(btn_next);

    root->setLayout(layout);

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
    DELETE(player);
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

    player->mrStop();
}



void Player::slotNext()
{
    LOG_API();

    player->mrPlay();

}


void Player::slotPrev()
{
    LOG_API();

    player->mrPlay();
}



void Player::slotPlayPause()
{
    LOG_API();
    static bool current_play = true;

    if(current_play)
    {
        current_play = false;
        player->mrPause();
    }
    else
    {
        current_play = false;
        player->mrPlay();
    }

}


bool Player::eventFilter(QObject * obj, QEvent * event)
{
    if((event->type() == QEvent::KeyPress))
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch(keyEvent->key())
        {
            case Qt::Key_Back:
            case Qt::Key_Escape:
                GuiEvent aaevent;
                aaevent.type = GUI_EVENT_PAUSE_PAGE;
                sendEvent(&aaevent);
                event->accept();
                return true;
            default:
                break;
        }
    }

    return false;
}


