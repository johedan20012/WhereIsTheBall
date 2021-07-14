#ifndef PLAYSCREEN_H
#define PLAYSCREEN_H

#include <queue>

#include "Screen.h"
#include "OAMManager.h"
#include "InputManager.h"
#include "ObjectAtributes.h"
#include "TextSystem.h"
#include "Cup.h"

#define MAX_SCORE 999999999
#define MAX_BET   999999

#define SHUFFLE_MODE_MASK   0x80
#define SHUFFLE_LAYER_MASK  0x40
#define SHUFFLE_CUP2_MASK   0x38
#define SHUFFLE_CUP1_MASK   0x7

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
        static const int NUM_SPRITES = 8;

        PlayScreenState scrState;
        PlayState playState;

        Cup* cups[5];
        OBJ_ATTR* ballAttr;
        OBJ_ATTR* cursorAttr;
        OBJ_ATTR* cursorSelectAttr;

        u32 shufflesData[40];
        u32 shuffles;
        std::queue<int> cupsNotUsed;
        u32 actShuffle;
        u32 cupsMoveFlags;
        u32 showBall;
        u32 ballPos; /// Index of what cup the ball is

        int curSelectPos;
        u32 cupSelected;

        u32 coins;
        u32 highscore;
        u32 multiplier;
        u32 bet;
        u32 minimumBet;
        u32 level;

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
        void setMinimumBetAndLevel();

        void updateMultiplier(); ///Update the number of cups visible and the value of the multiplier onScreen

        void fillNotUsedCupStack(int cups);
        int getRandomCup();
        void generateRandomShuffles(); ///Generates the data for the SHUFFLE play state

        void swapCups(int c1,int c2,int mode);
        void updateCupsShuffle();

        void updateSelectionCursor();
};

#endif // PLAYSCREEN_H
