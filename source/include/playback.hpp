#ifndef PLAYBACK_HPP
#define PLAYBACK_HPP

#include "ptasking.hpp"


typedef enum
{
    E_PBC_IDLE = 0,
    E_PBC_PLAY,
    E_PBC_PAUSE,
    E_PBC_BUFFERING,
} PBState;


typedef enum
{
    E_NTFY_PBSTATE = 0,
    E_NTFY_AUDIOINFO,
    E_NTFY_VIDEOINFO,

}eNotifyType;


typedef enum
{
    E_NOTIFY = 0,
    E_UOPRET,

}PbMsgType;

typedef struct
{
    eNotifyType type;
    union
    {
        PBState state;
        int error;
    }u;

} PbNotify;



typedef int (*PbCallback)(int msg, void * data);


class Playback : public PThread
{
public:
    Playback(PbCallback function);
    virtual ~Playback();
 
    int play();
    int play(string & path); // path == (localpath || httpurl)
    int pause();
    int stop(bool sync = false);
    int mute();
    int vocal(bool enable);

    int volumeUp();
    int volumeDown();    
    int setVideoWindow(int x, int y, int w, int h);
    string currentPath();
    PBState currentState();
    int getAudioTracks();
    int setAudioTrack(int track);
 
    int setDebugLvl();
    int getDebugLvl();
    void toString();

private:
    string currentFile;
    /* internal data 
    message_queue, cmd parser,....which i don't care....
    */
};






#endif /* PLAYBACK_HPP */

