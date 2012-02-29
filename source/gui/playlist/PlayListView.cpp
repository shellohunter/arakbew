
#include "shared.h"
#include "log.h"

#include "../GuiManager.h"
#include "../background/Background.h"

#include "PlayListView.h"






PlayListView::PlayListView(QWidget * parent) : GuiModule(GUI_MODULE_PLAYLIST)
{
    LOG_API();
    parentWidget = parent;
    init();

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


