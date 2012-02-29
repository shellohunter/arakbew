
#include "shared.h"
#include "log.h"
#include "cli.h"
#include "GuiManager.h"


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



GuiModule::GuiModule(const char * moduleName)
{
    LOG_INFO("<GuiModule> %s->%s() \n", moduleName, __FUNCTION__);

    strncpy(name, moduleName, GUI_MODULE_NAME_LENGTH);
    
    connect(this, SIGNAL(signalResume()), this, SLOT(resume()));
    connect(this, SIGNAL(signalPause()), this, SLOT(pause()));
    connect(this, SIGNAL(signalInit()), this, SLOT(init()));
    connect(this, SIGNAL(signalExit()), this, SLOT(exit()));
}


GuiModule::~GuiModule()
{
    LOG_INFO("<GuiModule> %s->%s() \n", name, __FUNCTION__);
}


void GuiModule::sendMessage(const char * name, int msg, void * data)
{
    emit signalPrivateMsg(name, msg, data);
}


void GuiModule::sendMessage(int msg, void * data)
{
    emit signalBroadcastMsg(msg, data);
}



void GuiModule::sendEvent(GuiEvent* event)
{
    emit signalModuleEvent(event);
}


void GuiModule::toString()
{
    printf("<GuiModule> %s.\n", name);
}


void GuiModule::requestResume()
{
    emit signalResume();
}


void GuiModule::requestPause()
{
    emit signalPause();
}


void GuiModule::requestInit()
{
    emit signalInit();
}


void GuiModule::requestExit()
{
    emit signalExit();
}










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
        LOG_INFO("<GuiManager> append \"%s\".\n", module->name);
        QObject::connect(module, SIGNAL(signalPrivateMsg(const char *, int, void*)), this, SLOT(slotPrivateMsg(const char *, int, void*)));
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
            LOG_INFO("<GuiManager> remove \"%s\".\n", module->name);
            QObject::disconnect(module, 0, 0, 0);
            QObject::disconnect(module, 0, 0, 0);
            QObject::disconnect(module, 0, 0, 0);
            guiModules.remove(i);
        }
        else
        {
            LOG_INFO("<GuiManager> remove \"%s\", not found!\n", module->name);
        }
    }

    return OK;
}


int GuiManager::remove(const char * moduleName)
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



GuiModule * GuiManager::getModule(const char * moduleName)
{
    for(int i=0; i<guiModules.size(); i++)
    {
        if(0 == strcmp(guiModules.at(i)->name, moduleName))
            return guiModules.at(i);
    }
    return NULL;
}



int GuiManager::activate(const char * moduleName)
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
            LOG_INFO("<GuiManager> activate \"%s\".\n", module->name);
            module->requestResume();
            guiStack.push(module);
        }
        else if(current != module)
        {
            current->requestPause();
            LOG_INFO("<GuiManager> activate \"%s\".\n", module->name);
            module->requestResume();
            guiStack.push(module);
        }
        else
        {
            LOG_INFO("<GuiManager> \"%s\" is already active.\n", module->name);
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
        LOG_INFO("<GuiManager> deactivate \"%s\".\n", module->name);
        module->requestPause();
    }
    if(!guiStack.isEmpty())
        guiStack.top()->requestResume();
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


void GuiManager::slotPrivateMsg(const char * moduleName, int msg, void * data)
{
    GuiModule * module = getModule(moduleName);
    if(module)
    {
        LOG_INFO("<GuiManager> send slotPrivateMsg(\"%s\", %d, %p).\n", module->name, msg, data);
        module->processMessage(msg, data);
    }
    else
    {
        LOG_INFO("<GuiManager> receiver not found!. slotPrivateMsg(\"%s\", %d, %p).\n", moduleName, msg, data);
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
        case GUI_EVENT_RESUME_PAGE:
            activate(event->data.moduleName);
            break;
        case GUI_EVENT_PAUSE_PAGE:
            deactivate();
            break;
        default:
            break;
    }
}



