

#include <stdlib.h>

#include "../GuiManager.hpp"
#include "shared.hpp"
#include "../background/Background.hpp"
#include "SingerList.hpp"





SingerIcon::SingerIcon(QWidget * parent)
    : QWidget(parent)
{
    this->setAutoFillBackground(true);
    this->setGeometry(0,0,170,200);
    label_picture = new QLabel(this);
    label_picture->setGeometry(10,10,150,150);
    label_picture->setAutoFillBackground(true);

    label_text = new QLabel(this);
    LOG_INFO("label_text x%d,y%d,w%d,h%d,\n",
        label_text->geometry().x(),
        label_text->geometry().y(),
        label_text->geometry().width(),
        label_text->geometry().height());
    label_text->setGeometry(10,170,150,30);
    LOG_INFO("label_text x%d,y%d,w%d,h%d,\n",
        label_text->geometry().x(),
        label_text->geometry().y(),
        label_text->geometry().width(),
        label_text->geometry().height());
    label_text->setAlignment(Qt::AlignCenter);
    this->highlight(false);
}


void SingerIcon::highlight(bool hlt)
{
    QPalette palette = this->palette();
    if(hlt)
        palette.setBrush(QPalette::Background, QBrush(QColor(255,255,255,255)));
    else
        palette.setBrush(QPalette::Background, QBrush(QColor(0,255,255,255)));
    this->setPalette(palette);

}


void SingerIcon::setPicture(QString path)
{
    QPalette palette = label_picture->palette();
    if(path.isEmpty())
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/unknown_avatar.png")));
    else
        palette.setBrush(QPalette::Background, QBrush(QPixmap(path)));
    label_picture->setPalette(palette);
}


void SingerIcon::setText(QString text)
{
    label_picture->setText(text);
}


void SingerIcon::setTag(void * tag)
{
    vtag = tag;
}


void * SingerIcon::tag()
{
    return vtag;
}


SingerList::SingerList(QWidget * parent) : GuiModule(GUI_MODULE_SINGERLIST)
{
    LOG_API();
    parentWidget = parent;
    this->init();
}

SingerList::~SingerList()
{
    LOG_API();
    this->exit();
}

int SingerList::init()
{
    LOG_API();
    root = new QWidget(parentWidget);

    root->setAutoFillBackground(true);
    root->setGeometry(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    root->setFocusPolicy(Qt::NoFocus);

    realFocus = new QPushButton(root);
    realFocus->setObjectName("singerlistfocus");
    realFocus->setGeometry(0,0,0,0);
    realFocus->installEventFilter(this);


    lbl_keyword  = new QLabel("Male Singers", root);
    lbl_keyword->setGeometry(10, 10, 600, 30);
    btn_return   = new QPushButton("Back", root);
    btn_return->setGeometry(10, 10, 50, 30);

    lbl_pagenum  = new QLabel("7/12", root);
    lbl_pagenum->setGeometry(500, 760, 50, 30);
    btn_prevPage = new QPushButton("Previous", root);
    btn_prevPage->setGeometry(600, 760, 50, 30);
    btn_nextPage = new QPushButton("Next", root);
    btn_nextPage->setGeometry(700, 760, 50, 30);

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            singerIcon[i*4+j] = new SingerIcon(root);
            singerIcon[i*4+j]->move(30+170*j,20+200*i);
            singerIcon[i*4+j]->setPicture(QString(":/images/unknown_avatar.png"));
            singerIcon[i*4+j]->setText("Some One");
        }
    }

    root->hide();

    return OK;
}

int SingerList::resume()
{
    LOG_API();
    singerIcon[0]->highlight(true);
    realFocus->setFocus(Qt::OtherFocusReason);
    root->show();
    return OK;
}


int SingerList::pause()
{
    LOG_API();
    root->hide();
    return OK;
}


int SingerList::exit()
{
    LOG_API();
    DELETE(root);
    return OK;
}

int SingerList::processMessage(int msg, void * data)
{
    LOG_API();
    return OK;
}



int SingerList::keyPressEvent(QObject * obj, QKeyEvent * event)
{
    LOG_API();
    static QWidget * fakeFocus = singerIcon[0];
    static bool      icon_hlted = true;

    if(icon_hlted)
    {
        static int cur_idx = 0;
        int        new_idx = cur_idx;
        LOG_VERBOSE("cur hlt idx %d.\n", cur_idx);
        switch(event->key())
        {
            case Qt::Key_Up:
                new_idx = abs(cur_idx-4)%8;
                break;
            case Qt::Key_Down:
                new_idx = (cur_idx+4)%8;
                break;
            case Qt::Key_Left:
                new_idx = abs(cur_idx-1)%8;
                break;
            case Qt::Key_Right:
                new_idx = (cur_idx+1)%8;
                break;
            default:
                break;
        }
        LOG_VERBOSE("new hlt idx %d.\n", new_idx);

        singerIcon[cur_idx]->highlight(false);
        singerIcon[new_idx]->highlight(true);
        fakeFocus = singerIcon[new_idx];
        cur_idx = new_idx;
    }
    else
    {

    }

    return OK;
}


bool SingerList::eventFilter(QObject * obj, QEvent * event)
{
    LOG_API();
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);
        LOG_INFO("(%p, %d)\n", obj, keyEvent->key());
        switch(keyEvent->key())
        {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::Key_Select:
            case Qt::Key_Return:
            case Qt::Key_Back:
            case Qt::Key_Escape:
                keyPressEvent(obj, keyEvent);
                event->accept();
                return true;
            default:
                break;
        }
    }
    return false;
}

void SingerList::slotSingerSelected()
{
    LOG_API();
}

