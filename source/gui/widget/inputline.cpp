
#include "inputline.h"



InputLine::InputLine(KeyBoard * keyboard, QWidget * parent)
    : QLineEdit(parent), kb(keyboard)
{
    connect(kb, SIGNAL(signal_close(QString)), this, SLOT(slot_receive_input(QString)));
    installEventFilter(this);
}


void InputLine::echo(bool on)
{
    if(on) this->setEchoMode(QLineEdit::Normal);
    else   this->setEchoMode(QLineEdit::Password);
}


void InputLine::slot_receive_input(QString text)
{
    this->setText(text);
}


bool InputLine::eventFilter(QObject * obj, QEvent * event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);
        switch(keyEvent->key())
        {
            case Qt::Key_Select:
            case Qt::Key_Return:
                if(kb)
                {
                    disconnect(kb, SIGNAL(signal_close(QString)), 0, 0);
                    connect(kb, SIGNAL(signal_close(QString)), this, SLOT(slot_receive_input(QString)));
                    if(QLineEdit::Password == this->echoMode())
                        kb->echo(false);
                    else
                        kb->echo(true);
                    kb->open(this->text());
                }
                event->accept();
                return false;
                break;
            default:
                break;
        }
    }
    return QLineEdit::eventFilter(obj, event);
}


