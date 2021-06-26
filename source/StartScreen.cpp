#include "StartScreen.h"

#include <string.h>

#include "InputManager.h"
#include "ObjectAtributes.h"

///Backgrounds
#include "ImgStartScreen.h"

///Sprites
#include "SprCursor.h"

StartScreen::StartScreen()
    : Screen(ScreenType::START_SCREEN), optionSelected(0){

    ///Copy image palette, tile data and map
    memcpy(&BGPAL_MEMORY[0], ImgStartScreenPal, ImgStartScreenPalLen);

    memcpy(&TILE8_MEMORY[1][0], ImgStartScreenTiles, ImgStartScreenTilesLen);

    memcpy(&SCRBLOCK_MEMORY[0][0], ImgStartScreenMap, ImgStartScreenMapLen);

    ///Get oamManager instance
    oamManager = OAMManager::getInstance();

    ///Copy sprite tiles and palette
    memcpy(&OBJPAL_MEMORY[0],SprCursorPal,SprCursorPalLen);

    memcpy(&TILE8_MEMORY[4][0], SprCursorTiles, SprCursorTilesLen);

    ///Set sprite attributes
    cursorAttr = oamManager->getAttrPtr(0);

    cursorAttr->attr0 = ATTR0_YPOS(62) | ATTR0_REG | ATTR0_8BPP | ATTR0_SQUARE;
    cursorAttr->attr1 = ATTR1_XPOS(81) | ATTR1_SIZE(1);
    cursorAttr->attr2 = ATTR2_BASE_TILE(0) | ATTR2_PIORITY(0);

    oamManager->copyBuffer(1);
}

void StartScreen::update(){
    if(InputManager::getInstance()->keyWentDown(KEY_DOWN)){
        optionSelected = 1;
    }
    if(InputManager::getInstance()->keyWentDown(KEY_UP)){
        optionSelected = 0;
    }

    cursorAttr->attr0 &= ~ATTR0_YPOS_MASK;
    cursorAttr->attr0 |= ATTR0_YPOS(62+optionSelected*34);

    oamManager->copyBuffer(1);
}
