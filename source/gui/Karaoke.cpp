
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

    /* create gui modules */
    guiManager.append(new Login(&background));
    guiManager.append(new Category(&background));
    guiManager.append(new PlayListView(&background));
    guiManager.append(new SearchResult(&background));
    guiManager.append(new Player(&background));
    guiManager.append(new SingerList(&background));
    guiManager.append(new Menu(&background));

    dbg_init();

}

Karaoke::~Karaoke()
{
    LOG_API();
    DELETE(ipc);
    DELETE(cli);
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

