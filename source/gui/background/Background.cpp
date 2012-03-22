
#include <QtGui/QtGui>
#include <QtCore/QtCore>
#include "shared.h"
#include "Background.h"

Background::Background(QWidget * parent)
    : QWidget(parent)
{

    LOG_API();

#ifndef WINDOWS
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    this->setFixedSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width()-this->width())/2, (desktop->height()-this->height())/2);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/background_normal.png")));
    this->setPalette(palette);

}

int Background::setBackground(QString & imagePath)
{
    LOG_API();

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/background_normal.png")));
    this->setPalette(palette);
    return 0;
}

int Background::clearBackground()
{
    LOG_API();
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(0,0,0,0));
    this->setPalette(palette);
    return 0;
}
