
#include <string.h>
#include "keyboard.hpp"
#include "shared.hpp"
#include "log.hpp"



VKey::VKey(QWidget * parent)
    : QPushButton(parent)
{
}

void VKey::setChar(const char * charForMode1, const char * charForMode2)
{
    modeText[0] = charForMode1;
    modeText[0] = charForMode2;
}

const char * VKey::getChar()
{
    return this->modeText[0];
}

void VKey::setMode(KeyBoardMode mode)
{
    this->setText(modeText[mode]);
}

void VKey::setKeyType(KeyType type)
{
    this->type = type;
}

KeyType VKey::getKeyType()
{
    return this->type;
}


#if 0
char * keyset[5][11][2];
{
    {"@","@"},{",",","},{".","."},{"?","?"},{"!","!"},{"+","+"},{"-","-"},{"_","_"},{"=","="},{"\"","\""},{"Backspace","Backspace"},
    {"1","1"},{"2","2"},{"3","3"},{"4","4"},{"5","5"},{"6","6"},{"7","7"},{"8","8"},{"9","9"},{"0",  "0"},{"Mode","Mode"        },
    {"q","Q"},{"w","Q"},{"e","E"},{"r","R"},{"t","T"},{"y","Y"},{"u","U"},{"i","I"},{"o","O"},{"p",  "P"},{"Clear","Clear"        },
    {"a","A"},{"s","S"},{"d","D"},{"f","F"},{"g","G"},{"h","H"},{"j","J"},{"k","K"},{"l","L"},{"<-","<-"},{  "->"  ,  "->"        },
    {"z","Z"},{"x","X"},{"c","C"},{"v","V"},{"b","B"},{"Space" , "Space"},{"n","N"},{"m","M"},{"OK","OK"                          },
}

keyConfig keys_config[5][11]  /* KEY_TYPE_CHARACTER == 0 */
{
    {{"@","@", 0},{",",",", 0},{".",".", 0},{"?","?", 0},{"!","!", 0},{"+","+", 0},{"-","-", 0},{"_","_", 0},{"=","=", 0},{"\"","\"", 0},{"Backspace","Backspace", 1}},
    {{"1","1", 0},{"2","2", 0},{"3","3", 0},{"4","4", 0},{"5","5", 0},{"6","6", 0},{"7","7", 0},{"8","8", 0},{"9","9", 0},{"0",  "0", 0},{"Mode","Mode"          , 1}},
    {{"q","Q", 0},{"w","Q", 0},{"e","E", 0},{"r","R", 0},{"t","T", 0},{"y","Y", 0},{"u","U", 0},{"i","I", 0},{"o","O", 0},{"p",  "P", 0},{"Clear","Clear"        , 1}},
    {{"a","A", 0},{"s","S", 0},{"d","D", 0},{"f","F", 0},{"g","G", 0},{"h","H", 0},{"j","J", 0},{"k","K", 0},{"l","L", 0},{"<-","<-", 1},{  "->"  ,  "->"        , 1}},
    {{"z","Z", 0},{"x","X", 0},{"c","C", 0},{"v","V", 0},{"b","B", 0},{"Space",     "Space", 1},{"n","N", 0},{"m","M", 0},{"OK","OK"                             , 1}}
}

#endif

typedef struct {
    char*   charForMode1;
    char*   charForMode2;
    short   keyType;
}KeyConfig;

KeyConfig keys_config[5*11] = /* KEY_TYPE_CHARACTER == 0 */
{
    {"@","@", 0},{",",",", 0},{".",".", 0},{"?","?", 0},{"!","!", 0},{"+","+", 0},{"-","-", 0},{"_","_", 0},{"=","=", 0},{"\"","\"", 0},{"Backspace","Backspace", 1},
    {"1","1", 0},{"2","2", 0},{"3","3", 0},{"4","4", 0},{"5","5", 0},{"6","6", 0},{"7","7", 0},{"8","8", 0},{"9","9", 0},{"0",  "0", 0},{"Mode","Mode"          , 1},
    {"q","Q", 0},{"w","Q", 0},{"e","E", 0},{"r","R", 0},{"t","T", 0},{"y","Y", 0},{"u","U", 0},{"i","I", 0},{"o","O", 0},{"p",  "P", 0},{"Clear","Clear"        , 1},
    {"a","A", 0},{"s","S", 0},{"d","D", 0},{"f","F", 0},{"g","G", 0},{"h","H", 0},{"j","J", 0},{"k","K", 0},{"l","L", 0},{"<-","<-", 1},{  "->"  ,  "->"        , 1},
    {"z","Z", 0},{"x","X", 0},{"c","C", 0},{"v","V", 0},{"b","B", 0},{"Space",     "Space", 1},{"n","N", 0},{"m","M", 0},{"OK","OK"                             , 1}
};




KeyBoard::KeyBoard(QString & prompt, QWidget * parent)
    : QWidget(parent)
{
    parent->setGeometry(0, 0, 100, 100);

    QVBoxLayout * vlayout = new QVBoxLayout();

    title = new QLabel(prompt);
    vlayout->addWidget(title);

    editbox = new QLineEdit();
    vlayout->addWidget(editbox);

#if 0
    for(int i=0; i<5; i++)
    {
        for(int j=0; j<11; j++)
        {
            pkeys[i][j] = new VKey();
            keys[i][j]->setChar(keys_config[i][j].charForMode1, keys_config[i][j].charForMode2);
            keys[i][j]->setKeyType(keys_config[i][j].keyType);
            if(KEY_TYPE_CHARACTER == keys_config[i][j].keyType)
                connect(keys[i][j], SIGNAL(clicked()), keys[i][j], SLOT(slotSendValue()));
            line[i]->addWidget(keys[i][j]);
        }
        line[i]->setLayout(layout);
        vlayout->addWidget(line[i]);
    }
#else
    for(int i=0; i<55; i++)
    {
        if(50 == i || 54 == i)
        {
            keys[i] = keys[i-1];
            keys[i-1]->resize(100, 100);
            continue;
        }
        else
        {
            keys[i] = new VKey(this);
            keys[i]->setChar(keys_config[i].charForMode1, keys_config[i].charForMode2);
            keys[i]->setKeyType((KeyType)keys_config[i].keyType);
            if(KEY_TYPE_CHARACTER == (KeyType)keys_config[i].keyType)
                connect(keys[i], SIGNAL(clicked()), this, SLOT(slot_key_char()));
        }
    }
    for(int i=0; i<5; i++)
    {
        QHBoxLayout * hlayout = new QHBoxLayout();
        for(int j=0; j<11; j++)
        {
            hlayout->addWidget(keys[i*11+j]);
        }
        //keyLine[i]->setLayout(hlayout);
        vlayout->addLayout(hlayout);
    }

#endif
    this->setLayout(vlayout);

    /* function keys */
    VKey * p = NULL;

    p = getKey("Backspace");
    p->setKeyType(KEY_TYPE_FUNCTION); connect(p, SIGNAL(clicked()), this, SLOT(slot_key_backspace()));
    p = getKey("Mode");
    p->setKeyType(KEY_TYPE_FUNCTION); connect(p, SIGNAL(clicked()), this, SLOT(slot_key_mode()));
    p = getKey("Clear");
    p->setKeyType(KEY_TYPE_FUNCTION); connect(p, SIGNAL(clicked()), this, SLOT(slot_key_clear()));
    p = getKey("<-");
    p->setKeyType(KEY_TYPE_FUNCTION); connect(p, SIGNAL(clicked()), this, SLOT(slot_key_left()));
    p = getKey("->");
    p->setKeyType(KEY_TYPE_FUNCTION); connect(p, SIGNAL(clicked()), this, SLOT(slot_key_right()));
    p = getKey("Space");
    p->setKeyType(KEY_TYPE_FUNCTION); connect(p, SIGNAL(clicked()), this, SLOT(slot_key_space()));
    p = getKey("OK");
    p->setKeyType(KEY_TYPE_FUNCTION); connect(p, SIGNAL(clicked()), this, SLOT(slot_key_ok()));
}


KeyBoard::~KeyBoard()
{
}

int KeyBoard::open()
{
    this->show();
    this->keys[0]->setFocus();
    return RET_OK;
}

int KeyBoard::open(QString& str)
{
    this->editbox->setText(str);
    input = str;
    return RET_OK;
}

int KeyBoard::close()
{
    this->hide();
    this->editbox->setText("");
    emit signalClose(this->input);
    return RET_OK;
}



KeyBoardMode KeyBoard::getMode()
{
    return this->mode;
}


int KeyBoard::setMode(KeyBoardMode mode)
{
    for(int i=0; i<sizeof(keys)/sizeof(VKey*); i++)
    {
        keys[i]->setMode(mode);
    }
    return RET_OK;
}



VKey * KeyBoard::getKey(const char * text)
{
    for(int i=0; i<55; i++)
        if(text == keys[i]->text())
            return keys[i];
    return NULL;
}

void KeyBoard::slot_key_char()
{
    editbox->setText(editbox->text()+keys[focusIndex]->text());
}


bool KeyBoard::eventFilter(QObject * obj, QEvent * event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);
        LOG_INFO("%s(%p, %d)\n", obj, keyEvent->key());
        switch(keyEvent->key())
        {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Left:
            case Qt::Key_Right:
                keyPressEvent(obj, keyEvent);
                break;
            default:
                return false;
                break;
        }
    }
    
    return QWidget::eventFilter(obj, event);
}

int KeyBoard::keyPressEvent(QObject * obj, QKeyEvent * event)
{
    switch(event->key())
    {    
        case Qt::Key_Up:
            if(focusIndex > 10)
                focusIndex = focusIndex - 11;
            else
                focusIndex = focusIndex + 44;
            keys[focusIndex]->setFocus();
            break;
        case Qt::Key_Down:
            if(focusIndex < 44)
                focusIndex = focusIndex + 11;
            else
                focusIndex = focusIndex - 44;
            keys[focusIndex]->setFocus();
            break;
        case Qt::Key_Left:
        {
            VKey * now  = keys[focusIndex];
            VKey * next = keys[focusIndex--];
            while(now == next && focusIndex > 0)
                next = keys[focusIndex--];
            next->setFocus();
            break;
        }
        case Qt::Key_Right:
        {
            VKey * now  = keys[focusIndex];
            VKey * next = keys[focusIndex++];
            while(now == next && focusIndex < 53)
                next = keys[focusIndex++];
            next->setFocus();
            break;
        }
        default:
            return false;
            break;    
    }
    return RET_OK;
}


void KeyBoard::slot_key_backspace()
{
    LOG_API();
}


void KeyBoard::slot_key_mode()
{
    LOG_API();
}

void KeyBoard::slot_key_clear()
{
    LOG_API();
}

void KeyBoard::slot_key_left()
{
    LOG_API();
}

void KeyBoard::slot_key_right()
{
    LOG_API();
}

void KeyBoard::slot_key_space()
{
    LOG_API();
}

void KeyBoard::slot_key_ok()
{
    LOG_API();
}




