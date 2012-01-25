
#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "Karaoke.hpp"
#include "log.hpp"
#include "ipc.hpp"
#include "cli.hpp"


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


Karaoke::Karaoke() 
    : background()
{
    LOG_API();

    /* init IPC */
    IPC * ipc = new IPC(IPCHandler, "ipc");
    ipc->start();

    /* init CLI */
    Cli * cli = new Cli();
    cli->start();

    /* init log */
    log_init();

    /* create gui modules */
    Login * login = new Login(&background);
    guiManager.append(login);
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

