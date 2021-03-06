
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <deque>
#include "database.h"
#include "shared.h"

using namespace std;

class PlayList : public deque<Song>
{
public:
    void clear()
    {
        LOG_INFO("<playlist> all selected songs are removed.\n");
        deque<Song>::clear();
    }

    void remove(const Song &val)
    {
        deque<Song>::iterator iter;
        for(iter = this->begin(); iter != this->end(); ++iter)
        {
            if(*iter ==  val)
            {
                this->erase(iter);
                LOG_INFO("<playlist> [%s] is removed from playlist. now total %d.\n", val.name.c_str(), this->size());
                return;
            }
        }
        LOG_INFO("<playlist> [%s] not in playlist. can not remove.\n", val.name.c_str());
    }

    void append(const Song &val)
    {
        this->push_back(val);
        LOG_INFO("<playlist> [%s] is added to playlist.  now total %d.\n", val.name.c_str(), this->size());
    }


    Song pop()
    {
        if(0 == this->size())
        {
            LOG_ERROR("<playlist> no element to pop! in %s.\n", __FUNCTION_NAME__);
            return Song();
        }
        LOG_VERBOSE("<playlist> pop, \"%s\", bf pop size == %d.\n",this->at(0).name.c_str(), this->size());

        this->remove(this->at(0));
        return this->at(0);
    }

    Song top()
    {
        return this->at(0);
    }

    bool has(Song& song)
    {
        deque<Song>::iterator iter = this->begin();
        deque<Song>::iterator iter_end = this->end();
        for(;iter != iter_end;++iter)
        {
            if (*iter == song) return true;
        }
        return false;
    }

    int find(const Song& song)
    {
        for(unsigned idx = 0;idx<this->size();idx++)
        {
            if(this->at(idx) == song)
                return idx;
        }
        LOG_INFO("<playlist> can not find \"%s\".\n", song.name.c_str());
        return -1;
    }


    void moveForward(const Song& song, int step = 1)
    {
        unsigned idx = this->find(song);
        LOG_INFO("<playlist> moveForward(\"%s\", %d).\n",song.name.c_str(), step);
        if((idx-step) < 0 || idx >= this->size())
        {
            LOG_ERROR("<playlist> can not move, (idx, step) == (%d, %d).\n", idx, step);
            return;
        }

        Song s = this->at(idx);
        this->at(idx) = this->at(idx-step);
        this->at(idx-step) = s;
    }

    void moveBackward(const Song& song, int step = 1)
    {
        int idx = this->find(song);
        LOG_INFO("<playlist> %s(\"%s\", %d).\n",__FUNCTION_NAME__, song.name.c_str(), step);
        if((idx+step) >= this->size() || idx < 0)
        {
            LOG_ERROR("<playlist> can not move, (idx, step) == (%d, %d).\n", idx, step);
            return;
        }
        Song s(this->at(idx));
        this->at(idx) = this->at(idx+step);
        this->at(idx+step) = s;
    }

    void moveTop(Song& song)
    {
        int i = this->find(song);
        if(i<0) return;

        Song tmp(this->at(i));
        this->remove(this->at(i));
        this->push_front(tmp);
    }

    void moveBottom(Song& song)
    {
        int i = this->find(song);
        if(i<0) return;

        Song tmp(this->at(i));
        this->remove(this->at(i));
        this->push_back(tmp);
    }

    void saveToFile() {}
    void loadFromFile() {}

    void toString()
    {
        deque<Song>::iterator iter = this->begin();
        deque<Song>::iterator iter_end = this->end();
        printf("----------------------------------\n");
        for(;iter != iter_end;++iter)
        {
            (*iter).toString();
        }
        printf("----------------------------------\n");
    }
};




extern PlayList playlist; /* defined in playlistview.cpp */





#endif /* PLAYLIST_H */



