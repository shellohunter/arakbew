
#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <QtGui>
#include <QtCore>


typedef enum
{
    E_MODE_NORMAL = 0,
    E_MODE_CAPITAL,
    E_MODE_SYMBOL,
}KeyBoardMode;

typedef enum
{
    KEY_TYPE_CHARACTER = 0,
    KEY_TYPE_FUNCTION,
}KeyType;

class VKey : public QPushButton
{
public:
    VKey(QWidget * parent = NULL);

    void    setChar(const char * charForMode1, const char * charForMode2 = NULL);
    const char  * getChar();
    void    setKeyType(KeyType type);
    KeyType getKeyType();
    void    setMode(KeyBoardMode mode);
private:
    const char *  modeText[2];
    KeyType type;
    KeyBoardMode mode; 
};




class KeyBoard : public QWidget
{
Q_OBJECT

public:
    KeyBoard(QString & title, QWidget * parent = NULL);
    ~KeyBoard();
public:
    int open();
    int open(QString& str);
    int close();
    KeyBoardMode getMode();
    int setMode(KeyBoardMode mode);
    VKey * getKey(const char * keyText);
    //VKey * getKey(int row, int col);

private:
    bool eventFilter(QObject * obj, QEvent * event);
    int keyPressEvent(QObject * obj, QKeyEvent * event);

    QString input;
    KeyBoardMode mode;
    //VKey* keys[5][11];
    int focusIndex; /* -1 == editbox, 0 == 1st char */


    QLabel      * title;
    QLineEdit   * editbox;
    //QWidget     * keyLine[5];
    VKey        * keys[5*11];


signals:
    void signalClose(QString& str);
    void signalInput(QChar x);

public slots:
    void slot_key_backspace();
    void slot_key_mode();
    void slot_key_clear();
    void slot_key_left();
    void slot_key_right();
    void slot_key_space();
    void slot_key_ok();
    void slot_key_char();

};



#endif /* KEYBOARD_HPP */

