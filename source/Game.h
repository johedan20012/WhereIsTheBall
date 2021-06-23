#ifndef GAME_H
#define GAME_H

#include "InputManager.h"
#include "ScreenManager.h"

class Game{
    private:
        ScreenManager* screenManager;
        InputManager* inputManager;

    public:
        Game();

        void run();
};

#endif // GAME_H
