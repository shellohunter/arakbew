#ifndef MODULE_MANAGER_HPP
#define MODULE_MANAGER_HPP

#include <vector>
#include <stack>
#include <string>

#include <QtGui>
#include <QtCore>

#include "shared.hpp"

using namespace std;

enum
{
    GUI_EVENT_LOGIN_STATUS,
};

typedef struct
{
    int type;
    union
    {
        bool loginStatus;    /* GUI_EVENT_LOGIN_STATUS */
    }data;
}GuiEvent;

#define GUI_MODULE_LOGIN                "login"
#define GUI_MODULE_CATEGORY             "category"
#define GUI_MODULE_SEARCHRESULT         "searchresult"
#define GUI_MODULE_PLAYLIST             "playlist"
#define GUI_MODULE_PLAYER               "player"
#define GUI_MODULE_SINGERLIST           "singerlist"



class GuiModule : public QObject
{
Q_OBJECT

public:
    GuiModule(string moduleName) : name(moduleName) {}
    virtual ~GuiModule()    {}
    void sendMessage(string name, int msg, void * data) { emit signalPrivateMsg(name, msg, data);}
    void sendMessage(int msg, void * data) { emit signalBroadcastMsg(msg, data);}
    void sendEvent(GuiEvent* event) { emit signalModuleEvent(event);}
    virtual int resume()    = 0;
    virtual int pause()     = 0;
    virtual int init()      = 0;
    virtual int exit()      = 0;
    virtual int processMessage(int msg, void * data) = 0;
    virtual void toString() { printf("<GuiModule> %s.\n", name.c_str());}
    string name;

signals:
    void signalPrivateMsg(string name, int msg, void* data);
    void signalBroadcastMsg(int msg, void* data);
    void signalModuleEvent(GuiEvent* event);
};


class GuiManager : public QObject
{
Q_OBJECT

public:
    GuiManager();
    ~GuiManager();

    static GuiManager * gm;

    int append(GuiModule * module);
    GuiModule * getActiveModule();
    GuiModule * getModule(string moduleName);
    int activate(string moduleName);
    int activate(GuiModule * module);
    int deactivate();
    void toString();
    int broadcast(int msg, void * data);

public slots:
    void slotPrivateMsg(string name, int msg, void * data);
    void slotBroadcastMsg(int msg, void * data);
    void slotModuleEvent(GuiEvent * event);

protected:
    int remove(GuiModule * module);
    int remove(string moduleName);

private:
    QStack <GuiModule*> guiStack;
    QVector<GuiModule*> guiModules;
};

#endif /* MODULE_MANAGER_HPP */

