#include "StartScreen.h"

#include <string.h>

#include "ScreenManager.h"
#include "ObjectAtributes.h"

///Backgrounds
#include "ImgStartScreen.h"

///Sprites
#include "SprCursor.h"

StartScreen::StartScreen()
    : Screen(ScreenType::START_SCREEN), optionSelected(0){

    REG_DISPCNT ^= BG2_ON;
    REG_DISPCNT |= BG1_ON;

    REG_BG1CNT = BG_PRIORITY(0) | CHAR_BASE(2) | BG_16_COLOR | SCREEN_BASE(1) | BG_SIZE_0;

    ///Copy image palette, tile data and map
    memcpy(&BGPAL_MEMORY[16], ImgStartScreenPal, ImgStartScreenPalLen);
    memcpy(&TILE8_MEMORY[2][0], ImgStartScreenTiles, ImgStartScreenTilesLen);
    memcpy(&SCRBLOCK_MEMORY[1][0], ImgStartScreenMap, ImgStartScreenMapLen);

    inputManager = InputManager::getInstance();

    ///Get oamManager instance
    oamManager = OAMManager::getInstance();

    ///Copy sprite tiles and palette
    memcpy(&OBJPAL_MEMORY[0],SprCursorPal,SprCursorPalLen);

    memcpy(&TILE8_MEMORY[4][0], SprCursorTiles, SprCursorTilesLen);

    ///Set sprite attributes
    cursorAttr = oamManager->getAttrPtr(0);

    cursorAttr->attr0 = ATTR0_YPOS(63) | ATTR0_REG | ATTR0_8BPP | ATTR0_SQUARE;
    cursorAttr->attr1 = ATTR1_XPOS(81) | ATTR1_SIZE(1);
    cursorAttr->attr2 = ATTR2_BASE_TILE(0) | ATTR2_PRIORITY(0);

    oamManager->copyBuffer(1);
}

void StartScreen::update(){
    if(inputManager->keyWentDown(KEY_DOWN))
        optionSelected ++;
    if(inputManager->keyWentDown(KEY_UP))
        optionSelected --;
    optionSelected = (optionSelected <1)? 1 : ((optionSelected > 3)? 3 : optionSelected);

    if(inputManager->keyWentDown(KEY_A)){
        ScreenManager::getInstance()->setScreen((ScreenType)optionSelected);
    }

    cursorAttr->attr0 &= ~ATTR0_YPOS_MASK;
    cursorAttr->attr0 |= ATTR0_YPOS(38+optionSelected*25);

    oamManager->copyBuffer(1);
}
