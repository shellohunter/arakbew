


#ifndef SINGERLIST_HPP
#define SINGERLIST_HPP

#include <QtGui>
#include <QtCore>
#include "../GuiManager.h"
#include "../widget/button.h"


class SingerIcon : public QWidget
{
Q_OBJECT

public:
    SingerIcon(QWidget * parent = NULL);
    void setPicture(QString path);
    void setText(QString text);
    void setTag(void * tag);
    void * tag();
    void highlight(bool hlt);

protected:
    bool eventFilter(QObject * obj, QEvent * event);

signals:
    void clicked();

private:
    QLabel * label_picture;
    QLabel * label_text;
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

    SingerIcon  * singerIcon[8];
    int           cur_idx;
    int           page_now;
    int           page_total;

    QLabel      * lbl_keyword;
    QLabel      * lbl_pagenum;
    Button      * btn_return;
    Button      * btn_prevPage;
    Button      * btn_nextPage;

    Button      * realFocus;
    QWidget     * parentWidget;

protected:
    int keyPressEvent(QObject * obj, QKeyEvent * event);
    bool eventFilter(QObject * obj, QEvent * event);

public slots:
    void slotSingerSelected();
    void slotReturnButton();
    void slotPagePrev();
    void slotPageNext();

};

#endif /* SINGERLIST_HPP */



