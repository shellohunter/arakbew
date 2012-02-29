#ifndef KARAOKE_HPP
#define KARAOKE_HPP


#include <vector>
#include <stack>
#include <string>

#include "shared.h"
#include "GuiManager.h"
#include "background/Background.h"
#include "login/Login.h"
#include "search/SearchResult.h"
#include "singerlist/SingerList.h"
#include "player/player.h"
#include "category/category.h"
#include "playlist/PlayListView.h"
#include "menu/menu.h"


class Karaoke
{
public:
    Karaoke();
    ~Karaoke()                  {}

    int init()                  {return OK;}
    int pause();
    int resume();
    int exit()                  {return OK;}

private:
    GuiManager guiManager;
    Background  background;
};


#endif /* KARAOKE_HPP */
