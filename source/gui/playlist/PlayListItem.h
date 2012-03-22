
#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H


#include <QtGui/QtGui>
#include <QtCore/QtCore>





class PlayListItem : public QLabel
{
Q_OBJECT

public:
    PlayListItem(QWidget *parent = NULL);
    void setContent(const char * songName, const char * singerName, bool playing = false);
    void setContent(QString & songName, QString & singerName, bool playing = false);
    void focusIn();
    void focusOut();
    virtual ~PlayListItem();

protected:
    bool eventFilter(QObject * obj, QEvent * event);

private:
    QLabel * label_songName;
    QLabel * label_singerName;
    QLabel * label_status;

};


#endif /* PLAYLISTITEM_H */




