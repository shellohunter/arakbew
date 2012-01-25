
#include <string>
#include "ptasking.hpp"
#include "log.hpp"
#include "cli.hpp"
#include "shared.hpp"
#include "playback.hpp"

using namespace std;


Playback::Playback(PbCallback cb)
{
    LOG_API();
}

Playback::~Playback()
{
    LOG_API();
}

int Playback::play()
{
    LOG_API();

    stop(true);

    LOG_INFO("playing %s.\n", currentFile.c_str());
    return RET_OK;
}

int Playback::play(string & filePath)
{
    LOG_API();

    if(filePath != currentFile)
    {
        stop(true);
        currentFile = filePath;
        LOG_INFO("playing %s.\n", currentFile.c_str());
    }

    return RET_OK;
}


int Playback::pause()
{
    LOG_API();

    return RET_OK;
}


int Playback::stop(bool sync)
{
    LOG_API();

    if(E_PBC_IDLE != currentState())
    {
        //xxx_stop(true);
        LOG_INFO("<playbakc> stop %s.\n", currentFile.c_str());
        currentFile.clear();
    }
    else
    {
        LOG_INFO("<playbakc> IDLE state, no need to stop.\n");
    }

    return RET_OK;
}


int Playback::mute()
{
    LOG_API();

    return RET_OK;
}

int Playback::vocal(bool enable)
{
    LOG_API();

    return RET_OK;
}

int Playback::volumeUp()
{
    LOG_API();

    return RET_OK;
}


int Playback::volumeDown()
{
    LOG_API();

    return RET_OK;
}


int Playback::setVideoWindow(int x, int y, int w, int h)
{
    LOG_API();

    return RET_OK;
}

string Playback::currentPath()
{
    LOG_API();

    return currentFile;
}

int Playback::getAudioTracks()
{
    LOG_API();

    return 2;
}

int Playback::setAudioTrack(int track)
{
    LOG_API();

    return RET_OK;
}

int Playback::setDebugLvl()
{
    return RET_OK;
}

int Playback::getDebugLvl()
{
    return RET_OK;
}

void Playback::toString()
{
    printf("<playback> pbstate == .%d. \n", currentState());
    printf("<playback> file == %s \n", currentFile.c_str());
}


