
#include "shared.h"
#include "../GuiManager.h"
#include "../background/Background.h"

#include "playlist.h"
#include "../player/player.h"
#include "PlaylistItem.h"
#include "PlayListView.h"


PlayList playlist;

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


static const char * btn_css = "background-color: lightgray; color: blue; font: 12pt;";
static const char * btn_css_hl = "background-color: black; color: white; font: 12pt;";

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

    listview = new QWidget(root);
    listview->setGeometry(110, 80, 600, 300);
    listview->setStyleSheet("background-color: whitesmoke;");

    promptLabel = new QLabel(root);
    promptLabel->setGeometry(110, 80, 600, 300);
    promptLabel->setAlignment(Qt::AlignCenter);
    promptLabel->setStyleSheet("background-color: whitesmoke; color: blue; font: 24px bold;");

    QVBoxLayout * layout = new QVBoxLayout();
    for(int i = 0; i<PLAYLISTVIEW_ITEM_NUM; i++)
    {
        items[i] = new PlayListItem(listview);
        items[i]->setObjectName(QString("PlayListItem%1").arg(i));
        layout->addWidget(items[i]);
    }
    listview->setLayout(layout);

#if 0
    PlayListItem * item1 = new PlayListItem(listview);
    item1->setContent("hello1", "adfafasdfds");

    PlayListItem * item2 = new PlayListItem(listview);
    item2->setContent("hello2", "adfafasdfds");

    PlayListItem * item3 = new PlayListItem(listview);
    item3->setContent("hello3", "adfafasdfds", true);

    PlayListItem * item4 = new PlayListItem(listview);
    item4->setContent("hello4", "adfafasdfds");

    PlayListItem * item5 = new PlayListItem(listview);
    item5->setContent("hello5", "adfafasdfds");

    PlayListItem * item6 = new PlayListItem(listview);
    item6->setContent("hello6", "adfafasdfds");

    PlayListItem * item7 = new PlayListItem(listview);
    item7->setContent("hello7", "adfafasdfds");

    PlayListItem * item8 = new PlayListItem(listview);
    item8->setContent("hello8", "adfafasdfds");


    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(item1);
    layout->addSpacing(5);
    layout->addWidget(item2);
    layout->addSpacing(5);
    layout->addWidget(item3);
    layout->addSpacing(5);
    layout->addWidget(item4);
    layout->addSpacing(5);
    layout->addWidget(item5);
    layout->addSpacing(5);
    layout->addWidget(item6);
    layout->addSpacing(5);
    layout->addWidget(item7);
    layout->addSpacing(5);
    layout->addWidget(item8);
    layout->addSpacing(5);
    listview->setLayout(layout);
#endif
    btn_prev = new Button(root);
    btn_prev->setText(tr("Previous"));
    btn_prev->setObjectName("Previous");
    btn_prev->setGeometry(QRect(100, 400, 75, 23));
    btn_prev->setStyleSheet(btn_css);

    btn_next = new Button(root);
    btn_next->setText(tr("Next"));
    btn_next->setObjectName("Next");
    btn_next->setGeometry(QRect(300, 400, 75, 23));
    btn_next->setStyleSheet(btn_css);

    btn_return = new Button(root);
    btn_return->setText(tr("Back"));
    btn_return->setObjectName("Back");
    btn_return->setGeometry(QRect(550, 400, 75, 23));
    btn_return->setStyleSheet(btn_css);

    eventAgent = new Button(root);
    eventAgent->setGeometry(0,0,0,0);
    eventAgent->installEventFilter(this);

    initNavigation();
    connect(btn_return, SIGNAL(clicked()), this, SLOT(slotReturnButton()));
#if 0
    ui = new Ui();
    ui->setupUi(root);

    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(slotReturnButton()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(slotContextMenu()));
#endif

    root->hide();
    return OK;
}



int PlayListView::resume()
{
    int i = 0;
    LOG_API();


    for(i=0;i<PLAYLISTVIEW_ITEM_NUM;i++)
    {
        if(i>=playlist.size())
        {
            items[i]->hide();
        }
        else
        {
            Song song = playlist.at(i);
            items[i]->setContent(song.name.c_str(), song.artistName.c_str());
            items[i]->show();
        }

    }

    if(playlist.size() == 0)
    {
        promptLabel->setText(QObject::tr("Play list is empty. \n"));
        promptLabel->show();
        listview->hide();
        btn_return->setStyleSheet(btn_css_hl);
        currentFocus = btn_return;
    }
    else
    {
        promptLabel->hide();
        listview->show();
        items[0]->focusIn();
        currentFocus = items[0];
    }

    eventAgent->setFocus();  // this agent handles all key events.
    root->show();
    return OK;
}


int PlayListView::pause()
{
    LOG_API();

    if(currentFocus->objectName().startsWith(QString("PlayListItem")))
        ((PlayListItem *)currentFocus)->focusOut();
    else
        ((Button *)currentFocus)->setStyleSheet(btn_css);
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



void PlayListView::initNavigation()
{
    static Navigation _navi_[] =
    {
        {this->items[0], btn_next,       this->items[1], NULL, NULL},
        {this->items[1], this->items[0], this->items[2], NULL, NULL},
        {this->items[2], this->items[1], this->items[3], NULL, NULL},
        {this->items[3], this->items[2], this->items[4], NULL, NULL},
        {this->items[4], this->items[3], this->items[5], NULL, NULL},
        {this->items[5], this->items[4], this->items[6], NULL, NULL},
        {this->items[6], this->items[5], this->items[7], NULL, NULL},
        {this->items[7], this->items[6], this->btn_prev, NULL, NULL},
        {this->btn_prev, this->items[7], NULL, NULL, this->btn_next},
        {this->btn_next, this->items[7], NULL, this->btn_prev, this->btn_return},
        {this->btn_return, this->items[7], NULL, this->btn_next, NULL},

        {NULL, NULL, NULL,NULL, NULL},
    };

    this->navi = _navi_;
}


void PlayListView::slotPageUp()
{
    LOG_API();
}


void PlayListView::slotPageDown()
{
    LOG_API();
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

    QPoint point = listview->mapToParent(currentFocus->pos());
    point.setX(currentFocus->x() + 20 + currentFocus->width());
    point.setY(currentFocus->y() + currentFocus->height());

    contextMenu.exec(point);
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


void PlayListView::slotSongStart()
{
    LOG_API();
    Song song = playlist.top();
    if(!song.url.empty())
    {
        Player * player = (Player *)GuiManager::gm->getModule(GUI_MODULE_PLAYER);
        player->slotPlay(song.url.c_str());
    }
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
            case Qt::Key_Option:
            case Qt::Key_F12:
                if(currentFocus->objectName().startsWith(QString("PlayListItem")))
                    slotContextMenu();
                return true;
            case Qt::Key_Left:
            case Qt::Key_Right:
            case Qt::Key_Up:
            case Qt::Key_Down:
                CursorKeyEvent(keyEvent->key());
                event->accept();
                return true;
            case Qt::Key_Select:
            case Qt::Key_Return:
                if(currentFocus->objectName().startsWith(QString("PlayListItem")))
                    slotContextMenu();
                else
                    ((Button *)currentFocus)->click();
                event->accept();
                return true;
            default:
                break;
        }
    }

    return false;
}



int PlayListView::CursorKeyEvent(int key)
{
    int i = 0;
    QWidget * temp = currentFocus;

    LOG_API();

    do
    {
        for(i=0;this->navi[i].focus != NULL;i++)
        {
            if(temp == this->navi[i].focus)
            {
                switch(key)
                {
                    case Qt::Key_Up:
                        temp =  this->navi[i].btn_up;
                        break;
                    case Qt::Key_Down:
                        temp =  this->navi[i].btn_down;
                        break;
                    case Qt::Key_Left:
                        temp =  this->navi[i].btn_left;
                        break;
                    case Qt::Key_Right:
                        temp =  this->navi[i].btn_right;
                        break;
                    default:
                        LOG_INFO("invalid move 1, break.\n");
                        break;
                }
                break;
            }
        }

        LOG_API();


        if(!temp)
        {
            LOG_INFO("invalid move 2, break.\n");
            break;
        }
        else if(!temp->isVisible())
        {
            LOG_INFO("invisible widget, skip.\n");
            continue;
        }
        else if(temp == currentFocus)
        {
            LOG_INFO("loop end. break.\n");
            break;
        }
        else
        {
            LOG_INFO("found the right focus. break.\n");
            break;
        }
    }while(1);

    if(temp)
    {
        if(currentFocus->objectName().startsWith(QString("PlayListItem")))
            ((PlayListItem *)currentFocus)->focusOut();
        else
            ((Button *)currentFocus)->setStyleSheet(btn_css);

        LOG_INFO("focus : %s -> %s\n", qPrintable(currentFocus->objectName()), qPrintable(temp->objectName()));
        currentFocus = temp;

        if(currentFocus->objectName().startsWith(QString("PlayListItem")))
            ((PlayListItem *)currentFocus)->focusIn();
        else
            ((Button *)currentFocus)->setStyleSheet(btn_css_hl);
    }

#if 0
    if(currentFocus != eventAgent
    || currentFocus != btn_next
    || currentFocus != btn_prev
    || currentFocus != btn_return
    )
    {

    }

    if(currentFocus == eventAgent)
    {

    }


    switch(key)
    {
        case Qt::Key_Select:
        case Qt::Key_Return:
            LOG_INFO("<playlistview> (Key_Return)\n");
            currentFocus->click();
            return OK;
        case Qt::Key_Back:
        case Qt::Key_Escape:
            LOG_INFO("<playlistview> (Key_Escape)\n");
            slotReturnButton();
            return OK;
        case Qt::Key_Up:
            btn_return->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);"));
            break;
        case Qt::Key_Down:
            break;
        case Qt::Key_Left:
            break;
        case Qt::Key_Right:
            break;

        default:
            break;
    }

    LOG_INFO("new hlt idx %d.\n", new_idx);

#endif
    return OK;
}


