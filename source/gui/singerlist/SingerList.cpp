

#include <stdlib.h>

#include "../GuiManager.h"
#include "shared.h"
#include "../background/Background.h"
#include "SingerList.h"





SingerIcon::SingerIcon(QWidget * parent)
    : QWidget(parent)
{
    this->setAutoFillBackground(true);
    this->installEventFilter(this);
    this->setGeometry(0,0,150,180);
    label_picture = new QLabel(this);
    label_picture->setGeometry(10,10,130,130);
    label_picture->setAutoFillBackground(true);

    label_text = new QLabel(this);
    label_text->setGeometry(10,140,130,30);
    label_text->setStyleSheet("border: 1px solid whitesmoke;");
    label_text->setAlignment(Qt::AlignCenter);
    this->highlight(false);

}


bool SingerIcon::eventFilter(QObject * obj, QEvent * event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        emit clicked();
        return true;
    }
    else
        return false;
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
    label_text->setText(text);
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

    realFocus = new Button(root);
    realFocus->setObjectName("singerlistfocus");
    realFocus->setGeometry(0,0,0,0);
    realFocus->installEventFilter(this);


    lbl_keyword  = new QLabel("Singers", root);
    lbl_keyword->setStyleSheet("background-color: whitesmoke;");
    lbl_keyword->setGeometry(10, 10, 300, 30);

    lbl_pagenum  = new QLabel(root);
    lbl_pagenum->setStyleSheet("background-color: whitesmoke;");
    lbl_pagenum->setGeometry(310, 10, 300, 30);


    btn_prevPage = new Button("Previous", root);
    btn_prevPage->setGeometry(10, SCREEN_HEIGHT-30, 200, 20);
    btn_nextPage = new Button("Next", root);
    btn_nextPage->setGeometry(230, SCREEN_HEIGHT-30, 200, 20);
    btn_return   = new Button("Back", root);
    btn_return->setGeometry(SCREEN_WIDTH-210, SCREEN_HEIGHT-30, 200, 20);

    char* singers[] =
    {
        "Singers/avril.png",
        "Singers/beyond.png",
        "Singers/linkin-park.png",
        "Singers/luodayou.png",
        "Singers/mj.png",
        "Singers/yu-quan.png",
        "Singers/zhangxueyou.png",
        "Singers/zhoujielun.png",
    };


    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            singerIcon[i*4+j] = new SingerIcon(root);
            singerIcon[i*4+j]->move(70+150*j,50+180*i);
            //singerIcon[i*4+j]->setPicture(QString(":/images/unknown_avatar.png"));
            LOG_INFO(singers[i*4+j]);
            singerIcon[i*4+j]->setPicture(QString(singers[i*4+j]));
            singerIcon[i*4+j]->setText("Some One");
            connect(singerIcon[i*4+j], SIGNAL(clicked()), this, SLOT(slotSingerSelected()));
        }
    }

    cur_idx = 0;
    page_now = 1;
    page_total = 18;
    singerIcon[0]->highlight(true);
    lbl_pagenum->setText(QString("%1/%2").arg(page_now).arg(page_total));

    connect(btn_return, SIGNAL(clicked()), this, SLOT(slotReturnButton()));
    connect(btn_prevPage, SIGNAL(clicked()), this, SLOT(slotPagePrev()));
    connect(btn_nextPage, SIGNAL(clicked()), this, SLOT(slotPageNext()));

    root->hide();

    return OK;
}

int SingerList::resume()
{
    LOG_API();
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
    int        new_idx = cur_idx;
    LOG_VERBOSE("cur hlt idx %d.\n", cur_idx);
    switch(event->key())
    {
        case Qt::Key_Select:
        case Qt::Key_Return:
            slotSingerSelected();
            return OK;
        case Qt::Key_Back:
        case Qt::Key_Escape:
            slotReturnButton();
            return OK;
        case Qt::Key_MediaPrevious:
            slotPagePrev();
            return OK;
        case Qt::Key_MediaNext:
            slotPageNext();
            return OK;
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
    GuiEvent event;
    event.type = GUI_EVENT_RESUME_PAGE;
    event.data.moduleName = (char *)GUI_MODULE_SEARCHRESULT;
    sendEvent(&event);
}


void SingerList::slotReturnButton()
{
    LOG_API();
    GuiEvent event;
    event.type = GUI_EVENT_PAUSE_PAGE;
    sendEvent(&event);
}



void SingerList::slotPageNext()
{
    LOG_API();
    if(page_now < page_total)
    {
        page_now++;
        lbl_pagenum->setText(QString("%1/%2").arg(page_now).arg(page_total));
    }
}


void SingerList::slotPagePrev()
{
    LOG_API();
    if(page_now > 1)
    {
        page_now--;
        lbl_pagenum->setText(QString("%1/%2").arg(page_now).arg(page_total));
    }
}


