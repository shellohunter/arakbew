


#ifndef SINGERLIST_HPP
#define SINGERLIST_HPP

#include <QtGui>
#include <QtCore>
#include "../GuiManager.hpp"




class SingerIcon : public QWidget
{
public:
    SingerIcon(QWidget * parent = NULL);
    void setPicture(QString path);
    void setText(QString text);
    void setTag(void * tag);
    void * tag();
    void setFocus(bool focus);

private:
    QLabel label_picture;
    QLabel label_text;
    QLabel container;
    void * vtag;
};







class SingerList : public GuiModule
{
Q_OBJECT

public:
    SingerList(QWidget * parent = NULL);
    ~SingerList();

    int resume();
    int pause();
    int init();
    int exit();
    int processMessage(int msg, void * data);

private:
    QWidget * root;

#if 0
    QPushButton * btn_avatar[8];
    QPushButton * btn_name[8];
#else
    SingerIcon * singerIcon[8];
#endif
    QWidget * parentWidget;

protected:
    int keyPressEvent(QObject * obj, QKeyEvent * event);
    bool eventFilter(QObject * obj, QEvent * event);

public slots:
    void slotSingerSelected();

};

#endif /* SINGERLIST_HPP */



