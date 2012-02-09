
#ifndef INPUTLINE_HPP
#define INPUTLINE_HPP


#include <QtGui>
#include <QtCore>
#include "keyboard.hpp"


class InputLine : public QLineEdit
{
Q_OBJECT

public:
    InputLine(KeyBoard * keyboard = NULL, QWidget * parent = NULL);
    void echo(bool on);

private:
    KeyBoard * kb;

public slots:
    void slot_receive_input(QString text);
    bool eventFilter(QObject * obj, QEvent * event);    

};


#endif /* INPUTLINE_HPP */
