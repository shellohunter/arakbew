
#include "shared.h"
#include "../GuiManager.h"
#include "../background/Background.h"

#include "PlayListView.h"
#include "playlist.h"


static PlayList playlist;

int cli_playlist_dump(int argc, const char ** argv)
{
    playlist.toString();
    return OK;
}

int cli_playlist_del(int argc, const char ** argv)
{
    if(argc < 3)
    {
        printf("Usage: pl.del <song name>\n");
    }
    printf("Usage: not implemented yet!\n");
    return OK;
}

int cli_playlist_add(int argc, const char ** argv)
{
    if(argc < 3)
    {
        printf("Usage: pl.add <song name> <singer name>\n");
    }
    Song a;
    a.index = -1;
    a.name = argv[1];
    a.artistName = argv[2];
    a.artistIndex = -1;
    a.toString();
    playlist.append(a);
    return OK;
}


CliItem playListCli[] =
{
    {"pl.stt",         cli_playlist_dump,   "print playlist",   0},
    {"pl.add",         cli_playlist_add,    "add new song",     0},
    {"pl.rm",          cli_playlist_del,    "delete song",      0},
};




PlayListView::PlayListView(QWidget * parent) : GuiModule(GUI_MODULE_PLAYLIST)
{
    LOG_API();
    parentWidget = parent;
    init();
    Cli::cli_append(playListCli, sizeof(playListCli)/sizeof(CliItem));

}


PlayListView::~PlayListView()
{
    LOG_API();
    exit();
}

int PlayListView::init()
{
    LOG_API();
    root = new QWidget(parentWidget);
    root->installEventFilter(this);
    root->setWindowFlags(Qt::FramelessWindowHint);
    root->setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    ui = new Ui();
    ui->setupUi(root);

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slotReturnButton()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotContextMenu()));
    root->hide();
    return OK;
}



int PlayListView::resume()
{
    LOG_API();
    this->ui->pushButton_2->setFocus();
    root->show();
    return OK;
}


int PlayListView::pause()
{
    LOG_API();
    root->hide();
    return OK;
}


int PlayListView::exit()
{
    LOG_API();
    DELETE(ui);
    DELETE(root);
    return OK;
}

int PlayListView::processMessage(int msg, void * data)
{
    LOG_API();
    return OK;
}


void PlayListView::slotReturnButton()
{
    LOG_API();
    GuiEvent event;
    event.type = GUI_EVENT_PAUSE_PAGE;
    sendEvent(&event);
}


void PlayListView::slotContextMenu()
{
    LOG_API();
    QMenu contextMenu(root);

    contextMenu.addAction("Cut");
    contextMenu.addAction("Copy");
    contextMenu.addAction("Paste");

    contextMenu.exec(root->mapToGlobal(QPoint(0,0)));
}


void PlayListView::slotSongAdd(const Song &song)
{
    LOG_API();
    playlist.append(song);
}

void PlayListView::slotSongDel(const Song &song)
{
    LOG_API();
    playlist.remove(song);
}

bool PlayListView::eventFilter(QObject * obj, QEvent * event)
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

