

#include "SearchResult.hpp"


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
    root = new QWidget(parentWidget);
    root->setGeometry(10, 10, 800,300);

    button_back = new QPushButton("Back", root);
    button_back->setGeometry(10, 10, 80, 20);

    label_current = new QLabel("American & Ueropean", root);
    label_current->setGeometry(70, 10, 400, 20);

    song_table = new SongListView(songs, root);
    song_table->setGeometry(10, 40, 780, 200);

    label_songnumber = new QLabel("121/11324", root);
    label_songnumber->setGeometry(10, 270, 80, 20);

    button_prev = new QPushButton("Page Up", root);
    button_prev->setGeometry(70, 270, 80, 20);

    button_next = new QPushButton("Page Down", root);
    button_next->setGeometry(150, 270, 80, 20);

    QHBoxLayout * header = new QHBoxLayout();
    header->addWidget(button_back);
    header->addWidget(label_current);

    QHBoxLayout * footer = new QHBoxLayout();
    footer->addWidget(label_songnumber);
    footer->addWidget(button_prev);
    footer->addWidget(button_next);

    QVBoxLayout * vlayout = new QVBoxLayout();
    vlayout->addLayout(header);
    vlayout->addWidget(song_table);
    vlayout->addLayout(footer);

    song_table->setFocus();
    root->setLayout(vlayout);

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


