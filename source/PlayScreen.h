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
        OBJ_ATTR* cursorAttr;

        u32 ballPos; /// Index of what cup the ball is

        u32 coins;
        u32 highscore;
        u32 multiplier;
        u32 bet;
        u32 minimumBet;
        u32 level;
        u32 winnedCoins;

        int addBet;

        u32 pOpSelected;

        TextSystem* textLayer;

        OAMManager* oamManager;
        InputManager* inputManager;

    public:
        PlayScreen();
        ~PlayScreen() override;

        void update() override;

    private:

        void updateCHB(); ///update Coins, High score and Bet values on screen

        void changeBet(int amount); ///Change the current bet by "amount" coins

        void updateMultiplier(); ///Update the number of cups visible and the value of the multiplier onScreen
};

#endif // PLAYSCREEN_H
