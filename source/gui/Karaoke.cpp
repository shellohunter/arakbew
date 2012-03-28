
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "Karaoke.h"
#include "log.h"
#include "ipc.h"
#include "cli.h"
#include "dbg.h"


static void IPCHandler(IPCNotify * notify)
{
    switch(notify->ipcType)
    {
        case E_IPC_UNDEFINED:
            break;

        default:
            LOG_INFO("ignore ipc type[%d].\n", notify->ipcType);
            break;
    }
}



Karaoke::Karaoke() : background()
{
    LOG_API();

    /* init IPC */
    ipc = new IPC(IPCHandler, "ipc");
    ipc->start();

    /* init CLI */
    cli = new Cli();
    cli->start();

    /* init log */
    log_init();
    dbg_init();

    /* create gui modules */
    login           = new Login(&background);
    guiManager.append(login);

    category        = new Category(&background);
    guiManager.append(category);

    player          = new Player(&background);
    guiManager.append(player);

    playlistview    = new PlayListView(&background);
    guiManager.append(playlistview);

    searchresult    = new SearchResult(&background);
    guiManager.append(searchresult);

    singerlist      = new SingerList(&background);
    guiManager.append(singerlist);

    menu            = new Menu(&background);
    guiManager.append(menu);

    dbg_init(); // reinstall signal handler.

}

Karaoke::~Karaoke()
{
    LOG_API();
    DELETE(ipc);
    DELETE(cli);
    DELETE(menu);
    DELETE(singerlist);
    DELETE(searchresult);
    DELETE(playlistview);
    DELETE(player);
    DELETE(login);

}

int Karaoke::resume()
{
    LOG_API();
    background.show();
    guiManager.activate(GUI_MODULE_LOGIN);
    return 0;
}


int Karaoke::pause()
{
    LOG_API();
    guiManager.deactivate();
    background.hide();
    return 0;
}

