

#include "../GuiManager.h"
#include "shared.h"
#include "player.h"
#include "../background/Background.h"


static void player_callback(PB_NOTIFY_T *tNotifyInf)
{
    LOG_INFO("<player> %s(%d)\n",__FUNCTION__,tNotifyInf->eNfyTp);
    switch(tNotifyInf->eNfyTp)
    {
        case E_UOP_RET:
            printf("<player> UOP_RET. id %d, ret %d.\n",
                tNotifyInf->u.tUopInf.eUop,
                tNotifyInf->u.tUopInf.eRet);
            break;
        case E_PBC_NOTIFY:
            printf("<player> PBC_STT. %d.\n",
                tNotifyInf->u.ePbcStt);
            break;
        case E_TIME_UPDATE:
            printf("<player> PB_TIME. (%u/%u).\n",
                (unsigned)tNotifyInf->u.tTmInf.u4CurTime,
                (unsigned)tNotifyInf->u.tTmInf.u4TtTime);
            break;
        default:
            printf("<player> Unknown Notification.\n");
            break;
    }
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
    root->setWindowFlags(Qt::FramelessWindowHint);
    root->setGeometry((SCREEN_WIDTH-400)/2,SCREEN_HEIGHT-100,400,80);

    root->installEventFilter(this);

    QPixmap mypixmap;

    btn_prev = new Button(root);
    //btn_prev->setStyleSheet(QString("background:url(:/images/prev.png);"));
    btn_prev->setFixedSize(64,64);
    mypixmap.load(":/images/prev.png");  
    btn_prev->clearMask();  
    btn_prev->setMask(mypixmap.createHeuristicMask());  
    btn_prev->setIcon(mypixmap);  
    btn_prev->setIconSize(QSize(64,64)); 
    
    btn_pause = new Button(root);
    btn_pause->setFixedSize(64,64);
    //btn_pause->setStyleSheet(QString("background-image:url(:/images/pause.png);"));
    mypixmap.load(":/images/pause.png");  
    btn_pause->clearMask();  
    btn_pause->setMask(mypixmap.createHeuristicMask());  
    btn_pause->setIcon(mypixmap);  
    btn_pause->setIconSize(QSize(64,64)); 

    btn_play = new Button(root);
    //btn_play->setStyleSheet(QString("border-image:url(:/images/play.png);"));
    btn_play->setFixedSize(64,64);
    mypixmap.load(":/images/play.png");  
    btn_play->clearMask();  
    btn_play->setMask(mypixmap.createHeuristicMask());  
    btn_play->setIcon(mypixmap);  
    btn_play->setIconSize(QSize(64,64)); 

    btn_stop = new Button(root);
    //btn_stop->setStyleSheet(QString("border-image:url(:/images/stop.png);"));
    btn_stop->setFixedSize(64,64);
    mypixmap.load(":/images/stop.png");  
    btn_stop->clearMask();  
    btn_stop->setMask(mypixmap.createHeuristicMask());  
    btn_stop->setIcon(mypixmap);  
    btn_stop->setIconSize(QSize(64,64)); 

    btn_next = new Button(root);
    //btn_next->setStyleSheet(QString("border-image:url(:/images/next.png);"));
    btn_next->setFixedSize(64,64);
    mypixmap.load(":/images/next.png");  
    btn_next->clearMask();  
    btn_next->setMask(mypixmap.createHeuristicMask());  
    btn_next->setIcon(mypixmap);  
    btn_next->setIconSize(QSize(64,64)); 

    QHBoxLayout * layout = new QHBoxLayout();

    layout->addWidget(btn_prev);
    layout->addSpacing(10);
    layout->addWidget(btn_pause);
    layout->addSpacing(10);
    layout->addWidget(btn_play);
    layout->addSpacing(10);
    layout->addWidget(btn_stop);
    layout->addSpacing(10);
    layout->addWidget(btn_next);

    root->setLayout(layout);

    connect(btn_prev, SIGNAL(clicked()), this, SLOT(slotPrev()));
    connect(btn_pause, SIGNAL(clicked()), this, SLOT(slotPause()));
    connect(btn_play, SIGNAL(clicked()), this, SLOT(slotPlay()));
    connect(btn_next, SIGNAL(clicked()), this, SLOT(slotNext()));
    connect(btn_stop, SIGNAL(clicked()), this, SLOT(slotStop()));

    root->hide();

    return OK;
}

int Player::resume()
{
    LOG_API();
    btn_play->setFocus();
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



void Player::slotPause()
{
    LOG_API();
    player->mrPause();
}


void Player::slotPlay()
{
    LOG_API();
    player->mrPlay();
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


