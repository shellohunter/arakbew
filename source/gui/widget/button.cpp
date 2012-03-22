
#include "button.h"


Button::Button(QWidget * parent) : QPushButton(parent)
{
}


Button::Button(const QString & text, QWidget * parent)
    : QPushButton(text, parent)
{
}


Button::Button(const QIcon & icon, const QString & text, QWidget * parent)
    : QPushButton(icon, text, parent)
{
}


Button::~Button()
{
}



void Button::keyPressEvent(QKeyEvent * event)
{

    switch(event->key())
    {
    case Qt::Key_Select:
    case Qt::Key_Return:
       /*  Keyboard::Enter ==> DIKS_RETURN == DIKS_ENTER,
           IR::Select ==> DIKS_SELECT
           So, Qt::Key_Return == Qt::Key_Select!
          */
        this->click();
        event->accept();
        break;

    default:
        QPushButton::keyPressEvent(event);
   }
}















