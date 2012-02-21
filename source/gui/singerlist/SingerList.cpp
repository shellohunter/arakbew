


#include "../GuiManager.hpp"
#include "shared.hpp"
#include "../background/Background.hpp"
#include "SingerList.hpp"





SingerIcon::SingerIcon(QWidget * parent)
    : QWidget(parent), label_picture(this), label_text(this)
{
    this->setGeometry(0,0,170,200);
    label_picture.setGeometry(10,10,150,150);
    label_text.setGeometry(10,160,150,30);
    this->setFocus(false);
}


void SingerIcon::setFocus(bool focus)
{
    QPalette palette = this->palette();
    if(focus)
        palette.setBrush(QPalette::Background, QBrush(QColor(255,255,255,255)));
    else
        palette.setBrush(QPalette::Background, QBrush(QColor(0,255,255,255)));
    this->setPalette(palette);

}


void SingerIcon::setPicture(QString path)
{
    QPalette palette = label_picture.palette();
    if(path.isEmpty())
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/unknown_avatar.png")));
    else
        palette.setBrush(QPalette::Background, QBrush(QPixmap(path)));
    label_picture.setPalette(palette);
}


void SingerIcon::setText(QString text)
{
    label_picture.setText(text);
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
    root->setFocus(Qt::OtherFocusReason);

    for(int i=0; i<2; i++)
    {
        for(int j=0; j<4; j++)
        {
            singerIcon[i*4+j] = new SingerIcon(root);
            singerIcon[i*4+j]->move(30+170*j,20+200*i);
            singerIcon[i*4+j]->setPicture(QString(":/images/unknown-avatar.png"));
            singerIcon[i*4+j]->setText("Some One");
#if 0
            btn_avatar[i*4+j] = new QPushButton(root);
            btn_name[i*4+j]   = new QPushButton("Some One",root);

            btn_avatar[i*4+j]->setGeometry(120+170*j,25+200*i,150,150);
            btn_name[i*4+j]->setGeometry(120+170*j,25+200*i+150,150,30);

            QPalette palette = btn_avatar[i*4+j]->palette();
            palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/unknown-avatar.jpg")));
            btn_avatar[i*4+j]->setPalette(palette);
            connect(btn_avatar[i*4+j], SIGNAL(clicked()), this, SLOT(slotSingerSelected()));
#endif
        }
    }

    root->hide();

    return OK;
}

int SingerList::resume()
{
    LOG_API();
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

