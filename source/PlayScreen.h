#ifndef PLAYSCREEN_H
#define PLAYSCREEN_H

#include "Screen.h"
#include "OAMManager.h"
#include "InputManager.h"
#include "ObjectAtributes.h"
#include "TextSystem.h"

enum class PlayState{
    BET = 0,
    SHUFFLE,
    SELECTION,
    PRIZE,
};

enum class PlayScreenState{
    PLAYING = 0,
    PAUSE,
    GAMEOVER,
};

class PlayScreen : public Screen{
    private:
        static u32 toncfontTiles[192];

        PlayScreenState scrState;
        PlayState playState;

        OBJ_ATTR* cupsAttr[5];
        OBJ_ATTR* ballAttr;

        u32 ballPos; /// Index of what cup the ball is

        u32 coins;

        TextSystem* textLayer;

        OAMManager* oamManager;
        InputManager* inputManager;

    public:
        PlayScreen();
        ~PlayScreen() override;

        void update() override;

    private:
        void handleInput();
};

#endif // PLAYSCREEN_H
