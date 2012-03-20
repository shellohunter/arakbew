

#ifndef BUTTON_H
#define BUTTON_H


#include <QtGui>
#include <QtCore>


class Button : public QPushButton
{
public:
    Button(QWidget * parent = NULL);
    Button(const QString & text, QWidget * parent = NULL);
    Button(const QIcon & icon, const QString & text, QWidget * parent = NULL);
    ~Button();
    void setStyleSheet(const QString& css);
protected:
    void keyPressEvent(QKeyEvent * event);
    QString style;
};




#endif /* BUTTON_H */

