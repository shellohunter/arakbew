
#include "shared.hpp"
#include "log.hpp"
#include "cli.hpp"
#include "GuiManager.hpp"


int cli_gm_stt(int argc, const char ** argv)
{
    if(GuiManager::gm)
    {
        GuiManager::gm->toString();
    }
    return OK;
}


int cli_gm_resume(int argc, const char ** argv)
{
    if (argc < 2)
    {
        LOG_ERROR("<cli> usage: gui.r <module-name> \n");
        return FAIL;
    }
    
    if(GuiManager::gm)
    {
        LOG_INFO("<cli> %s(%s) \n", __FUNCTION__, argv[1]);
        GuiManager::gm->activate(argv[1]);
    }
    return OK;
}

int cli_gm_pause(int argc, const char ** argv)
{
    if(GuiManager::gm)
    {
        GuiManager::gm->deactivate();
    }
    return OK;
}


CliItem guiManagerCli[] = 
{
    {"gui.stt",         cli_gm_stt,      "print gui manager status", 0},
    {"gui.r",           cli_gm_resume,   "resume gui module",        0},
    {"gui.p",           cli_gm_pause,    "pause current module",     0},
};



GuiManager * GuiManager::gm = NULL;
GuiManager::GuiManager()
{
    LOG_API();
    if(gm)
    {
        LOG_ERROR("Detected multiple GuiManager! Only one instance is allowed!\n");
    }
    else
    {
        Cli::cli_append(guiManagerCli, sizeof(guiManagerCli)/sizeof(CliItem));
        gm = this;
    }
}


GuiManager::~GuiManager()
{
    LOG_API();
}


int GuiManager::append(GuiModule * module)
{
    LOG_API();
    if(module)
    {
        LOG_INFO("<GuiManager> append \"%s\".\n", module->name.c_str());
        QObject::connect(module, SIGNAL(signalPrivateMsg(string, int, void*)), this, SLOT(slotPrivateMsg(string, int, void*)));
        QObject::connect(module, SIGNAL(signalBroadcastMsg(int, void*)), this, SLOT(slotBroadcastMsg(int, void*)));
        QObject::connect(module, SIGNAL(signalModuleEvent(GuiEvent*)), this, SLOT(slotModuleEvent(GuiEvent*)));
        guiModules.push_back(module);
    }
    return OK;
}


int GuiManager::remove(GuiModule * module)
{
    LOG_API();
    for (int i = 0; i < guiModules.size(); ++i)
    {
        if (guiModules.at(i) == module)
        {
            LOG_INFO("<GuiManager> remove \"%s\".\n", module->name.c_str());
            QObject::disconnect(module, 0, 0, 0);
            QObject::disconnect(module, 0, 0, 0);
            QObject::disconnect(module, 0, 0, 0);
            guiModules.remove(i);
        }
        else
        {
            LOG_INFO("<GuiManager> remove \"%s\", not found!\n", module->name.c_str());
        }
    }

    return OK;
}


int GuiManager::remove(string moduleName)
{
    return remove(getModule(moduleName));
}


GuiModule * GuiManager::getActiveModule()
{
    if(guiStack.isEmpty())
        return NULL;
    else
        return guiStack.top();
}



GuiModule * GuiManager::getModule(string moduleName)
{
    for(int i=0; i<guiModules.size(); i++)
    {
        if(guiModules.at(i)->name == moduleName)
            return guiModules.at(i);
    }
    return NULL;
}



int GuiManager::activate(string moduleName)
{
    return activate(getModule(moduleName));
}



int GuiManager::activate(GuiModule * module)
{
    LOG_API();
    if(module)
    {
        GuiModule * current = getActiveModule();
        if(!current)
        {
            LOG_INFO("<GuiManager> activate \"%s\".\n", module->name.c_str());
            module->resume();
            guiStack.push(module);
        }
        else if(current != module)
        {
            current->pause();
            LOG_INFO("<GuiManager> activate \"%s\".\n", module->name.c_str());
            module->resume();
            guiStack.push(module);
        }
        else
        {
            LOG_INFO("<GuiManager> \"%s\" is already active.\n", module->name.c_str());
        }
    }
    else
    {
        LOG_WARNING("<GuiManager> Null Module!\n");
        return FAIL;
    }
    return OK;
}


int GuiManager::deactivate()
{
    LOG_API();
    GuiModule * module = guiStack.pop();
    if(module)
    {
        LOG_INFO("<GuiManager> deactivate \"%s\".\n", module->name.c_str());
        module->pause();
    }
    if(guiStack.top())
        guiStack.top()->resume();
    return OK;
}


void GuiManager::toString()
{

    printf("-------------------------\n");
    printf("    <GuiModule List>     \n");
    for(int i=0; i<guiModules.size(); i++)
    {
        printf("    [%d]:  ",i);guiModules.at(i)->toString();
    }
    printf("    <GuiModule Stack>     \n");
    for(int i=0; i<guiStack.size(); i++)
    {
        printf("    [%d]:  ",i);guiStack.at(i)->toString();
    }
    printf("-------------------------\n");
}


int GuiManager::broadcast(int msg, void * data)
{
    LOG_INFO("<GuiManager> broadcast(%d, %p).\n", msg, data);
    for(int i=0; i<guiModules.size(); i++)
    {
        guiModules.at(i)->processMessage(msg, data);
    }

    return OK;
}


void GuiManager::slotPrivateMsg(string moduleName, int msg, void * data)
{
    GuiModule * module = getModule(moduleName);
    if(module)
    {
        LOG_INFO("<GuiManager> send slotPrivateMsg(\"%s\", %d, %p).\n", module->name.c_str(), msg, data);
        module->processMessage(msg, data);
    }
    else
    {
        LOG_INFO("<GuiManager> receiver not found!. slotPrivateMsg(\"%s\", %d, %p).\n", moduleName.c_str(), msg, data);
    }
}


void GuiManager::slotBroadcastMsg(int msg, void * data)
{
    broadcast(msg, data);
}


void GuiManager::slotModuleEvent(GuiEvent * event)
{
    LOG_API();
    switch(event->type)
    {
        case GUI_EVENT_LOGIN_STATUS:
            if(event->data.loginStatus)
            {
                activate(GUI_MODULE_CATEGORY);
            }
            break;
        default:
            break;
    }
}



