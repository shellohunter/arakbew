
#include <QtGui>
#include <QtCore>
#include "database.hpp"

/*

    1. Search song by song name.
    songname+singer img+singer+tickle
    
    2. Search song by singer.

*/



class SongItemView : public QWidget
{
Q_OBJECT
public:
    SongItemView(QWidget * parent = NULL);
    //SongItemView(Song& song, QWidget * parent = NULL);
    virtual ~SongItemView();

    Song& getSong();    

protected:
    QLabel * songname;
    QLabel * singername;
    QLabel * status;
    void focusInEvent ( QFocusEvent * event );
    void focusOutEvent ( QFocusEvent * event );

private:
    Song song;
};



