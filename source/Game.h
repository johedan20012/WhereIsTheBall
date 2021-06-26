#ifndef GAME_H
#define GAME_H

#include "InputManager.h"
#include "ScreenManager.h"
#include "OAMManager.h"

class Game{
    private:
        ScreenManager* screenManager;
        InputManager* inputManager;
        OAMManager* oamManager;

    public:
        Game();

        void run();
};

#endif // GAME_H
