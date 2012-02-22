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
    GuiModule(string moduleName);
    virtual ~GuiModule();

    /* send message to the specific module. */
    void sendMessage(string name, int msg, void * data);

    /* send message to all modules. */
    void sendMessage(int msg, void * data);

    /* send event to gui manager. */
    void sendEvent(GuiEvent* event);

    virtual int processMessage(int msg, void * data) = 0;
    virtual void toString();
    string name;

    /* in order to control gui in cli thread, we:
        1. add reqeust functions which emit signals.
        2. declare life cycle functions as slots. */
    void requestResume();
    void requestPause();
    void requestInit();
    void requestExit();

public slots:
    virtual int resume()    = 0;
    virtual int pause()     = 0;
    virtual int init()      = 0;
    virtual int exit()      = 0;

signals:
    void signalPrivateMsg(string name, int msg, void* data);
    void signalBroadcastMsg(int msg, void* data);
    void signalModuleEvent(GuiEvent* event);

    void signalResume();
    void signalPause();
    void signalInit();
    void signalExit();
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
    int activate(GuiModule * module);
    void toString();
    int broadcast(int msg, void * data);

public slots:
    void slotPrivateMsg(string name, int msg, void * data);
    void slotBroadcastMsg(int msg, void * data);
    void slotModuleEvent(GuiEvent * event);
    int activate(string moduleName);
    int deactivate();

protected:
    int remove(GuiModule * module);
    int remove(string moduleName);

private:
    QStack <GuiModule*> guiStack;
    QVector<GuiModule*> guiModules;
};

#endif /* MODULE_MANAGER_HPP */

