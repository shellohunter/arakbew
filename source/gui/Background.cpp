
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "Background.hpp"

Background::Background(QWidget * parent)
    : QWidget(parent)
{
    qDebug("%s, %d\n", __FUNCTION__, __LINE__);
#ifndef WINDOWS
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    this->setFixedSize(800,450);
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width()-this->width())/2, (desktop->height()-this->height())/2);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/background_normal.png")));
    this->setPalette(palette);

}

int Background::setBackground(QString & imagePath)
{
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(imagePath)));
    this->setPalette(palette);
    return 0;
}

int Background::clearBackground()
{
    this->setPalette(QPalette());
    return 0;
}
