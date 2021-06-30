#include "PlayScreen.h"

#include <string.h>

#include <gba_video.h>

#include "ScreenManager.h"

///Sprites
#include "SprCup.h"
#include "SprBall.h"

PlayScreen::PlayScreen()
    :Screen(ScreenType::PLAY_SCREEN),scrState(PlayScreenState::PLAYING),playState(PlayState::BET),coins(0){

    REG_DISPCNT ^= BG0_ON;

    inputManager = InputManager::getInstance();

    ///Get oamManager instance
    oamManager = OAMManager::getInstance();

    ///Copy sprite tiles and palette
    memcpy(&OBJPAL_MEMORY[0],SprCupPal,SprCupPalLen);
    memcpy(&TILE4_MEMORY[4][0], SprCupTiles, SprCupTilesLen);

    memcpy(&OBJPAL_MEMORY[16],SprBallPal,SprBallPalLen);
    memcpy(&TILE4_MEMORY[4][32], SprBallTiles,SprBallTilesLen);

    ///Set cups sprite attributes
    for(u32 i = 0; i<5; i++){
        cupsAttr[i] = oamManager->getAttrPtr(i);
        cupsAttr[i]->attr0 = ATTR0_YPOS(62) | ATTR0_REG | ATTR0_4BPP | ATTR0_TALL;
        cupsAttr[i]->attr1 = ATTR1_XPOS(30+i*36) | ATTR1_SIZE(3);
        cupsAttr[i]->attr2 = ATTR2_BASE_TILE(0) | ATTR2_PRIORITY(0);
    }

    ///Set ball sprite attributes
    ballAttr = oamManager->getAttrPtr(5);
    ballAttr->attr0 = ATTR0_YPOS(32) | ATTR0_REG | ATTR0_4BPP | ATTR0_SQUARE;
    ballAttr->attr1 = ATTR1_XPOS(32) | ATTR1_SIZE(1);
    ballAttr->attr2 = ATTR2_BASE_TILE(32) | ATTR2_PRIORITY(1) | ATTR2_PALBANK(1);

    oamManager->copyBuffer(6);
}

PlayScreen::~PlayScreen(){
    ///Hide sprites
    for(u32 i = 0; i<5; i++){
        cupsAttr[i]->attr0 = ATTR0_HIDE;
    }
    ballAttr->attr0 = ATTR0_HIDE;
    oamManager->copyBuffer(6);
}

void PlayScreen::update(){
    handleInput();

    if(scrState == PlayScreenState::PLAYING){
        switch(playState){
            case PlayState::BET:
                ///Player can select the multiplier and the bet
                break;
            case PlayState::SHUFFLE:
                ///Bet disable and shuffle of the cups
                break;
            case PlayState::SELECTION:
                ///Selection of the cup that may have the ball
                break;
            case PlayState::PRIZE:
                ///Ball reveal and prize delivery in case of correct guessing
                break;
        }

        if(coins == 0){
            scrState = PlayScreenState::GAMEOVER;
        }
    }
}

void PlayScreen::handleInput(){
    if(scrState == PlayScreenState::PLAYING){
        if(inputManager->keyWentDown(KEY_START)){
            scrState = PlayScreenState::PAUSE;
            return;
        }


    }

    if(scrState == PlayScreenState::GAMEOVER && inputManager->keyWentUp(KEY_START)){
        ScreenManager::getInstance()->setScreen(ScreenType::START_SCREEN);
    }
}
