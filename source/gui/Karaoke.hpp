#ifndef KARAOKE_HPP
#define KARAOKE_HPP


#include <vector>
#include <stack>
#include <string>

#include "shared.hpp"
#include "GuiManager.hpp"
#include "background/Background.hpp"
#include "login/Login.hpp"
#include "search/SearchResult.hpp"
#include "singerlist/SingerList.hpp"
#include "player/player.hpp"


class Karaoke
{
public:
    Karaoke();
    ~Karaoke()                  {};

    int init()                  {return OK;}
    int pause();
    int resume();
    int exit()                  {return OK;}

private:
    GuiManager guiManager;
    Background  background;
};


#endif /* KARAOKE_HPP */

