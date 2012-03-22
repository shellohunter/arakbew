


#include "shared.h"
#include "PlayListItem.h"






PlayListItem::PlayListItem(QWidget *parent) : QLabel(parent)
{
    LOG_API();
    this->installEventFilter(this);
    this->setAutoFillBackground(true);
    this->setFixedSize(460,30);

    label_songName = new QLabel(this);
    label_songName->setGeometry(0,0,300,30);
    label_songName->setScaledContents(true);
    label_songName->setStyleSheet("border-right: 1px solid whitesmoke;");
    label_songName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    label_singerName = new QLabel(this);
    label_singerName->setGeometry(label_songName->x()+label_songName->frameGeometry().width(),0,100,30);
    label_singerName->setStyleSheet("border-right: 1px solid whitesmoke;");
    label_singerName->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

    label_status = new QLabel(this);
    label_status->setGeometry(label_singerName->x()+label_singerName->frameGeometry().width(),0,60,30);
    label_status->setScaledContents(true);
    label_status->setAlignment(Qt::AlignCenter);


#if 1
    this->focusOut();
#else
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background,
        QBrush(QPixmap(":/images/playlist_item_normal.png").scaled(this->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);
#endif
}




PlayListItem::~PlayListItem()
{
    LOG_API();
}


void PlayListItem::setContent(const char * songName, const char * singerName, bool playing)
{
    setContent(QString(songName), QString(singerName), playing);
}


void PlayListItem::setContent(QString & songName, QString & singerName, bool playing)
{
    LOG_API();
    label_songName->setText(songName);
    label_singerName->setText(singerName);

    if(playing)
        label_status->setPixmap(QPixmap(":/images/item-playing.png"));
    else
        label_status->setPixmap(QPixmap());

}


void PlayListItem::focusIn()
{
    LOG_API();
    #if 1
    this->setStyleSheet("background-color: black; color: white;");
    #else
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/playlist_item_hl.png")));
    this->setPalette(palette);
    #endif
}



void PlayListItem::focusOut()
{
    LOG_API();
    #if 1
    this->setStyleSheet("background-color: lightgreen; color: blue;");
    #else
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/playlist_item_normal.png")));
    this->setPalette(palette);
    #endif
}


bool PlayListItem::eventFilter(QObject * obj, QEvent * event)
{
    if((event->type() == QEvent::KeyPress))
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch(keyEvent->key())
        {
            case Qt::Key_Back:
            case Qt::Key_Escape:
                event->accept();
                return true;

            case Qt::Key_Select:
            case Qt::Key_Return:
                LOG_INFO(" enter....");
                event->accept();
                return true;

            case Qt::Key_Option:
            case Qt::Key_Left:
                LOG_INFO(" option....");
                event->accept();
                return true;

            default:
                break;
        }
    }

    return false;
}




