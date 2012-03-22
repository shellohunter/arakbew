
#include "SearchResult.h"
#include "../Karaoke.h"
#include "../background/Background.h"


SearchResult::SearchResult(QWidget * parent) : GuiModule(GUI_MODULE_SEARCHRESULT)
{
    LOG_API();
    this->parentWidget = parent;
    init();
}


SearchResult::~SearchResult()
{
    LOG_API();
    exit();
}


int SearchResult::init()
{
    LOG_API();
    database.open("./wKaraOK.db");
    database.getSongs(songs, 0, 10);

    root = new QWidget(parentWidget);
    root->setWindowFlags(Qt::FramelessWindowHint);
    root->setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    label_current = new QLabel("American & European", root);
    label_current->setGeometry(10, 10, 200, 20);

    label_songnumber = new QLabel("121/11324", root);
    label_songnumber->setGeometry(220, 10, 80, 20);

    song_table = new SongListView(songs, root);
    song_table->setGeometry(10, 40, 700, 300);

    button_prev = new Button("Page Up", root);
    button_prev->setGeometry(10, 420, 80, 20);

    button_next = new Button("Page Down", root);
    button_next->setGeometry(100, 420, 80, 20);

    button_return = new Button("Back", root);
    button_return->setGeometry(800, 420, 40, 20);

    QHBoxLayout * header = new QHBoxLayout();
    header->addWidget(label_current);
    header->addWidget(label_songnumber);

    QGridLayout * footer = new QGridLayout();
    footer->addWidget(button_prev,0,0);
    footer->addWidget(button_next,0,1);
    footer->addWidget(button_return,0,3);

    QVBoxLayout * vlayout = new QVBoxLayout();
    vlayout->addLayout(header);
    vlayout->addWidget(song_table);
    vlayout->addLayout(footer);

    song_table->setFocus();
    root->setLayout(vlayout);
    this->installEventFilter(this);

    connect(button_return, SIGNAL(clicked()), this, SLOT(slotReturnButton()));
    printf("player %p\n", GuiManager::gm->getModule(GUI_MODULE_PLAYLIST));
#if 1
    connect(song_table, SIGNAL(signalSongAdd(Song)),
        GuiManager::gm->getModule(GUI_MODULE_PLAYLIST), SLOT(slotSongAdd(Song)));
    connect(song_table, SIGNAL(signalSongDel(Song)),
        GuiManager::gm->getModule(GUI_MODULE_PLAYLIST), SLOT(slotSongDel(Song)));
#else
    connect(song_table, SIGNAL(signalSongAdd(Song)),
        Karaoke::getInstance()->player, SLOT(slotSongAdd(Song)));
    connect(song_table, SIGNAL(signalSongDel(Song)),
        Karaoke::getInstance()->player, SLOT(slotSongDel(Song)));
#endif
    root->hide();

    return OK;
}




int SearchResult::resume()
{
    LOG_API();

    song_table->setFocus();
    root->show();
    return OK;
}


int SearchResult::pause()
{
    database.close();
    LOG_API();
    root->hide();
    return OK;
}


int SearchResult::exit()
{
    LOG_API();
    DELETE(root);
    return OK;
}

int SearchResult::processMessage(int msg, void * data)
{
    LOG_API();
    return OK;
}


int SearchResult::loadDataSet(DataSet<Song*>& songs)
{
    LOG_API();
    this->songs = songs;

    ASSERT(song_table);
    song_table->loadDataSet(songs);
    return OK;
}



bool SearchResult::eventFilter(QObject * obj, QEvent * event)
{
    if((event->type() == QEvent::KeyPress))
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch(keyEvent->key())
        {
            case Qt::Key_Back:
            case Qt::Key_Escape:
                slotReturnButton();
                event->accept();
                return true;
            default:
                break;
        }
    }

    return false;
}


void SearchResult::slotReturnButton()
{
    LOG_API();
    GuiEvent event;
    event.type = GUI_EVENT_PAUSE_PAGE;
    sendEvent(&event);
}


