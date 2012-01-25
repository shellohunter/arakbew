#ifndef KARAOKE_HPP
#define KARAOKE_HPP


#include <vector>
#include <stack>
#include <string>

#include "shared.hpp"
#include "GuiManager.hpp"
#include "Background.hpp"
#include "login/Login.hpp"


class Karaoke
{
public:
    Karaoke();
    ~Karaoke()                  {};

    int init()                  {return RET_OK;}
    int pause();
    int resume();
    int exit()                  {return RET_OK;}

private:
    GuiManager guiManager;
    Background  background;
};


#endif /* KARAOKE_HPP */

