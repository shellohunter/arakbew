
#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <QtGui>
#include <QtCore>


#define  KEYNUM  5*11

typedef enum
{
    E_MODE_NORMAL = 0,
    E_MODE_CAPITAL,
}KeyBoardMode;

typedef enum
{
    KEY_TYPE_CHARACTER = 0,
    KEY_TYPE_FUNCTION,
}KeyType;

class VKey : public QPushButton
{
Q_OBJECT
    
public:
    VKey(QWidget * parent = NULL);

    const char  * getChar();
    KeyType getKeyType();
    void    getFocus();
    void    loseFocus();
    void    setKeyType(KeyType type);
    void    setChar(const char * charForMode1, const char * charForMode2 = NULL);
    void    setMode(KeyBoardMode mode);

private:
    const char *  modeText[2];
    KeyType type;
    KeyBoardMode mode; 

signals:
    void signal_notify_clicked(VKey * p);

protected slots:
    void slot_clicked();
};




class KeyBoard : public QWidget
{
Q_OBJECT


public:
    KeyBoard(QString titleText, QWidget * parent = NULL);
    KeyBoard(QWidget * parent = NULL);
    ~KeyBoard();
public:
    int open();
    int open(QString str);
    int close();
    void echo(bool on);
    KeyBoardMode getMode();
    int setMode(KeyBoardMode mode);
    VKey * getKey(const char * keyText);
    void setFocus(VKey * key);

private:
    bool eventFilter(QObject * obj, QEvent * event);
    int keyPressEvent(QObject * obj, QKeyEvent * event);
    int init(QString titleText, QWidget * parent);

    QString input;
    KeyBoardMode mode;
    VKey * focus;


    QLabel      * title;
    QLineEdit   * editbox;
    VKey        * keys[KEYNUM];


signals:
    void signal_close(QString str);

public slots:
    void slot_open(QString str);
    void slot_btn_press(VKey * key);
    void slot_btn_backspace();
    void slot_btn_mode();
    void slot_btn_clear();
    void slot_btn_left();
    void slot_btn_right();
    void slot_btn_space();
    void slot_btn_ok();

};


#endif /* KEYBOARD_HPP */

