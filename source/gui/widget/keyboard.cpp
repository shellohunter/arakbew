
#include <string.h>
#include "keyboard.h"
#include "shared.h"
#include "log.h"



VKey::VKey(QWidget * parent)
    : QPushButton(parent), mode(E_MODE_NORMAL)
{
    this->setStyleSheet("color:black; border:1px solid #AAAAAA");
    connect(this, SIGNAL(clicked()), this, SLOT(slot_clicked()));
    this->setFocusPolicy(Qt::NoFocus);
}

void VKey::setChar(const char * charForMode1, const char * charForMode2)
{
    modeText[0] = charForMode1;
    modeText[1] = charForMode2;
    this->setText(modeText[mode]);
}

const char * VKey::getChar()
{
    return this->modeText[mode];
}

void VKey::setMode(KeyBoardMode mode)
{
    this->setText(modeText[mode]);
}

void VKey::setKeyType(KeyType type)
{
    if(KEY_TYPE_FUNCTION == type)
    {
        QString css = this->styleSheet().replace("color:black;","color:red;");
        this->setStyleSheet(css);
    }
    this->type = type;
}

KeyType VKey::getKeyType()
{
    return this->type;
}

void VKey::getFocus()
{
    QString normalCSS = this->styleSheet();
    QString focusedCSS = this->styleSheet().replace("solid #AAAAAA","solid red");
    this->setStyleSheet(focusedCSS);
}


void VKey::loseFocus()
{
    QString focusedCSS = this->styleSheet();
    QString normalCSS = this->styleSheet().replace("solid red", "solid #AAAAAA");
    this->setStyleSheet(normalCSS);
}


void VKey::slot_clicked()
{
    emit signal_notify_clicked(this);
}


typedef struct {
    const char*   charForMode1;
    const char*   charForMode2;
    const short   keyType;
}KeyConfig;



/* 1. KEY_TYPE_CHARACTER == 0, KEY_TYPE_FUNCTION == 1.
   2. {NULL,NULL,0} means the key is the same as previous key.
*/

KeyConfig keys_config[KEYNUM] =
{
    {"@","@", 0},{",",",", 0},{".",".", 0},{"?","?", 0},{"!","!", 0},{"+","+", 0},{"-","-", 0},{"_","_", 0},{"=","=", 0},{"\"","\"", 0},{"Backspace","Backspace", 1},
    {"1","1", 0},{"2","2", 0},{"3","3", 0},{"4","4", 0},{"5","5", 0},{"6","6", 0},{"7","7", 0},{"8","8", 0},{"9","9", 0},{"0",  "0", 0},{"Mode","Mode", 1},
    {"q","Q", 0},{"w","Q", 0},{"e","E", 0},{"r","R", 0},{"t","T", 0},{"y","Y", 0},{"u","U", 0},{"i","I", 0},{"o","O", 0},{"p",  "P", 0},{"Clear","Clear", 1},
    {"a","A", 0},{"s","S", 0},{"d","D", 0},{"f","F", 0},{"g","G", 0},{"h","H", 0},{"j","J", 0},{"k","K", 0},{"l","L", 0},{"*",  "*", 0},{"?", "?", 0},
    {"z","Z", 0},{"x","X", 0},{"c","C", 0},{"v","V", 0},{"b","B", 0},{"n","N", 0},{"m","M", 0},{"Space","Space", 1},{"<-","<-", 1},{"->", "->", 1},{"OK","OK", 1},
};




KeyBoard::KeyBoard(QString titleText, QWidget * parent)
    : QWidget(parent), input((const char *)0), mode(E_MODE_NORMAL)
{
    LOG_INFO("KeyBoard.cpp, %s(%s, %p).\n", __FUNCTION__, qPrintable(titleText), parent);
    init(titleText, parent);
}


KeyBoard::KeyBoard(QWidget * parent)
    : QWidget(parent), input((const char *)0), mode(E_MODE_NORMAL)
{
    LOG_INFO("KeyBoard.cpp, %s(%p).\n", __FUNCTION__, parent);
    init(0, parent);
}


int KeyBoard::init(QString titleText, QWidget * parent)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setGeometry(10, 10, 800, 250);
    this->setWindowModality(Qt::WindowModal);

    QVBoxLayout * vlayout = new QVBoxLayout();

    title = new QLabel(titleText);
    vlayout->addWidget(title);

    editbox = new QLineEdit();
    editbox->installEventFilter(this);
    editbox->setFixedSize(800, 30);
    editbox->setFocusPolicy(Qt::NoFocus);
    vlayout->addWidget(editbox);

    for(int i=0; i<KEYNUM; i++)
    {
        if(!keys_config[i].charForMode1) //{NULL,NULL,0}
        {
            keys[i] = keys[i-1];
            //keys[i-1]->resize(210,30);
            keys[i-1]->resize(keys[i-1]->width()*2, keys[i-1]->height());
            continue;
        }
        else
        {
            keys[i] = new VKey(this);
            keys[i]->setFixedSize(60,30);
            keys[i]->setChar(keys_config[i].charForMode1, keys_config[i].charForMode2);
            keys[i]->setKeyType((KeyType)keys_config[i].keyType);
            connect(keys[i], SIGNAL(signal_notify_clicked(VKey *)), this, SLOT(slot_btn_press(VKey *)));

        }
    }

    for(int i=0; i<5; i++)
    {
        QHBoxLayout * hlayout = new QHBoxLayout();
        for(int j=0; j<11; j++)
        {
            hlayout->addWidget(keys[i*11+j]);
        }
        vlayout->addLayout(hlayout);
    }

    this->setLayout(vlayout);
    focus = NULL;
    return OK;
}


KeyBoard::~KeyBoard()
{
    LOG_INFO("KeyBoard.cpp, %s().\n", __FUNCTION__);
}

int KeyBoard::open()
{
    LOG_API();
    editbox->setFocus();
    setFocus(keys[0]);
    this->show();
    return OK;
}


int KeyBoard::open(QString str)
{
    this->editbox->setText(str);
    input = str;
    open();
    return OK;
}

int KeyBoard::close()
{
    this->hide();
    this->editbox->setText("");
    emit signal_close(this->input);
    return OK;
}



KeyBoardMode KeyBoard::getMode()
{
    return this->mode;
}


int KeyBoard::setMode(KeyBoardMode mode)
{
    for(unsigned i=0; i<sizeof(keys)/sizeof(VKey*); i++)
    {
        keys[i]->setMode(mode);
    }
    this->mode = mode;
    return OK;
}

void KeyBoard::echo(bool on)
{
    if(on) editbox->setEchoMode(QLineEdit::Normal);
    else   editbox->setEchoMode(QLineEdit::Password);
}


void KeyBoard::setFocus(VKey * key)
{
    if(focus != key)
    {
        if(focus) focus->loseFocus();
        focus=key;
        focus->getFocus();
    }
}


VKey * KeyBoard::getKey(const char * text)
{
    LOG_INFO("%s\n",text);
    for(int i=0; i<KEYNUM; i++)
        if(0 == strcmp(text, keys[i]->getChar()))
            return keys[i];
    LOG_INFO("%s not found!\n",text);
    return NULL;
}


bool KeyBoard::eventFilter(QObject * obj, QEvent * event)
{
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

    return QWidget::eventFilter(obj, event);
}


int KeyBoard::keyPressEvent(QObject * obj, QKeyEvent * event)
{
    int idx = 0;
    while(idx < KEYNUM)
    {
        if(keys[idx] == focus)
            break;
        idx++;
    }

    LOG_INFO("key = %d, idx = %d \n", event->key(), idx);
    switch(event->key())
    {
        case Qt::Key_Up:

            if(idx > 10)
                idx = idx - 11;
            else
                idx = idx + 44;
            setFocus(keys[idx]);
            break;
        case Qt::Key_Down:
            if(idx < 44)
                idx = idx + 11;
            else
                idx = idx - 44;
            setFocus(keys[idx]);
            break;
        case Qt::Key_Left:
        {
            VKey * next = focus;
            if(idx==0)
                next = keys[54];
            else
                while(focus == next && idx > 0)
                    next = keys[--idx];
            setFocus(next);
            break;
        }
        case Qt::Key_Right:
        {
            VKey * next = focus;
            if(idx==54)
                next = keys[0];
            else
                while(focus == next && idx < 55)
                    next = keys[++idx];
            setFocus(next);
            break;
        }
        case Qt::Key_Select:
        case Qt::Key_Return:
        {
            slot_btn_press(focus);
            break;
        }
        default:
            return false;
            break;
    }
    return OK;
}


void KeyBoard::slot_open(QString str)
{
    open(str);
}


void KeyBoard::slot_btn_press(VKey * key)
{
    LOG_INFO("focus = %s idx %d \n", key->getChar(), key-keys[0]);
    setFocus(key);
    if(KEY_TYPE_CHARACTER == key->getKeyType())
    {
        editbox->insert(key->text());
        input = editbox->text();
    }
    else
    {
        if(0 == strcmp(key->getChar(),"Backspace"))
            slot_btn_backspace();
        else if(0 == strcmp(key->getChar(),"Mode"))
            slot_btn_mode();
        else if(0 == strcmp(key->getChar(),"Clear"))
            slot_btn_clear();
        else if(0 == strcmp(key->getChar(),"<-"))
            slot_btn_left();
        else if(0 == strcmp(key->getChar(),"->"))
            slot_btn_right();
        else if(0 == strcmp(key->getChar(),"Space"))
            slot_btn_space();
        else if(0 == strcmp(key->getChar(),"OK"))
            slot_btn_ok();
    }

}


void KeyBoard::slot_btn_backspace()
{
    LOG_API();
    editbox->backspace();
    input = editbox->text();
}


void KeyBoard::slot_btn_mode()
{
    LOG_API();
    this->setMode(E_MODE_NORMAL == mode? E_MODE_CAPITAL:E_MODE_NORMAL);
}

void KeyBoard::slot_btn_clear()
{
    LOG_API();
    editbox->setText("");
    input.clear();
}

void KeyBoard::slot_btn_left()
{
    LOG_API();
    editbox->cursorBackward(false, 1);
}

void KeyBoard::slot_btn_right()
{
    LOG_API();
    editbox->cursorForward(false, 1);
}

void KeyBoard::slot_btn_space()
{
    LOG_API();
    editbox->insert(" ");
    input = editbox->text();
}

void KeyBoard::slot_btn_ok()
{
    LOG_API();

    LOG_INFO("input==[%s] \n", qPrintable(input));
    this->close();
    emit signal_close(input);
}




