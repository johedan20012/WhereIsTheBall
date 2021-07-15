#include "CreditsScreen.h"

#include "ScreenManager.h"

///Font
#include "FontsData.h"

CreditsScreen::CreditsScreen()
    :Screen(ScreenType::CREDITS_SCREEN){
    REG_DISPCNT ^= BG1_ON;

    inputManager = InputManager::getInstance();

    textLayer = new TextSystem(2,2,toncfontTiles,toncfontTilesLen);
    textLayer->puts(7,1,"Game programming");
    textLayer->puts(10,3,"kevin20012");
    textLayer->puts(12,6,"Assets");
    textLayer->puts(10,8,"kevin20012");
    textLayer->puts(1,17,"Made with devkitpro and\nfollowing Tonc tutorial.");
}

CreditsScreen::~CreditsScreen(){
    delete textLayer;
    textLayer = nullptr;
}

void CreditsScreen::update(){
    if(inputManager->keyWentDown(KEY_B))
        ScreenManager::getInstance()->setScreen(ScreenType::START_SCREEN);
}
