

#include "SongListView.hpp"

SongItemView::SongItemView(QWidget * parent) : QWidget(parent)
{
 
    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedSize(330,30);
#if 0
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/song_hili.png").scaled( // Ëõ·Å±³¾°Í¼.  
                            this->size(),  
                            Qt::IgnoreAspectRatio,  
                            Qt::SmoothTransformation)));
    this->setPalette(palette);
#endif
    this->song = Song();

    QHBoxLayout * hboxlayout = new QHBoxLayout();

 
    songname = new QLabel(QString("No Name"), this);
    //songname->setGeometry(0, 0, 200, 30);

    singername = new QLabel(QString("Unknown Artist"), this);    
    //singername->setGeometry(100, 0, 100, 30);

    status = new QLabel(QString("selected"), this);
#if 0
    status->setIcon(QPixmap(":/images/song_select.png"));
#elif 0
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/song_select.png")));
    status->setPalette(palette);
#else
    status->setPixmap(QPixmap(":/images/song_select.png"));
#endif
    //status->setGeometry(300, 0, 30, 30);

    hboxlayout->addWidget(songname);
    hboxlayout->addWidget(singername);
    hboxlayout->addWidget(status);
    this->setLayout(hboxlayout);
}
#if 0
SongItemView::SongItemView(Song& song, QWidget * parent) : QWidget(parent)
{
    this->setFixedSize(330, 30);
    this->song = song;
    songname = new QLabel(QString(song.name.c_str()), this);
    songname->setGeometry(0, 0, 200, 30);
    songname->setStyleSheet(QString("border:1 black solid"));

    singername = new QLabel(song.artistName.c_str(), this);    
    singername->setGeometry(100, 0, 100, 30);
    singername->setStyleSheet(QString("border:1 black solid"));

    status = new QLabel(this);
#if 0
    status->setIcon(QPixmap(":/images/song_select.png"));
#elif 0
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/song_select.png")));
    status->setPalette(palette);
#else
    status->setPixmap(QPixmap(":/images/song_select.png"));
#endif
    status->setGeometry(300, 0, 30, 30);
    status->setStyleSheet(QString("border:1 black solid"));
}
#endif

SongItemView::~SongItemView()
{
}

void SongItemView::focusInEvent ( QFocusEvent * event )
{
    printf("focus in!\n");
#if 0
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/images/song_hili.png").scaled( // Ëõ·Å±³¾°Í¼.  
                            this->size(),  
                            Qt::IgnoreAspectRatio,  
                            Qt::SmoothTransformation)));
    this->setPalette(palette);
#else
    this->setStyleSheet(QString("border-image: url(:/images/song_hili.png)"));
#endif
}

void SongItemView::focusOutEvent ( QFocusEvent * event )
{
    printf("focus out!\n");
#if 0
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("")));
    this->setPalette(palette);
#else
    this->setStyleSheet(QString(""));
#endif

}


Song& SongItemView::getSong()
{
    return song;
}

