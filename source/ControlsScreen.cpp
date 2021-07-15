#include "ControlsScreen.h"

#include <string.h>
#include <gba_video.h>

#include "ScreenManager.h"
#include "ObjectAtributes.h"

///Sprite
#include "SprPad.h"
#include "SprLBtn.h"
#include "SprRBtn.h"

///Font
#include "FontsData.h"

ControlsScreen::ControlsScreen()
    :Screen(ScreenType::CONTROLS_SCREEN){
        REG_DISPCNT ^= BG1_ON;

        inputManager = InputManager::getInstance();
        oamManager = OAMManager::getInstance();

        textLayer = new TextSystem(2,2,toncfontTiles,toncfontTilesLen);
        textLayer->puts(5,2,"Change\n  bet");
        textLayer->puts(7,5,"+1");
        textLayer->puts(10,7,"+100");
        textLayer->puts(7,9,"-1");
        textLayer->puts(2,7,"-100");
        textLayer->puts(17,2,"  Change\nmultiplier");
        textLayer->puts(19,6,"-1  +1");
        textLayer->puts(8,17,"Press B to return");

        ///Copy spr data
        memcpy(&OBJPAL_MEMORY[0],SprPadPal,SprPadPalLen);
        memcpy(&TILE4_MEMORY[4][0], SprPadTiles, SprPadTilesLen);

        memcpy(&TILE4_MEMORY[4][16], SprLBtnTiles, SprLBtnTilesLen);
        memcpy(&TILE4_MEMORY[4][18], SprRBtnTiles, SprRBtnTilesLen);

        padAttr = oamManager->getAttrPtr(0);
        padAttr->attr0 = ATTR0_YPOS(48) | ATTR0_REG | ATTR0_4BPP | ATTR0_SQUARE;
        padAttr->attr1 = ATTR1_XPOS(52) | ATTR1_SIZE(2);
        padAttr->attr2 = ATTR2_BASE_TILE(0) | ATTR2_PRIORITY(3) | ATTR2_PALBANK(0);

        rBtnAttr = oamManager->getAttrPtr(1);
        rBtnAttr->attr0 = ATTR0_YPOS(56) | ATTR0_REG | ATTR0_4BPP | ATTR0_WIDE;
        rBtnAttr->attr1 = ATTR1_XPOS(152) | ATTR1_SIZE(0);
        rBtnAttr->attr2 = ATTR2_BASE_TILE(16) | ATTR2_PRIORITY(3) | ATTR2_PALBANK(0);

        lBtnAttr = oamManager->getAttrPtr(2);
        lBtnAttr->attr0 = ATTR0_YPOS(56) | ATTR0_REG | ATTR0_4BPP | ATTR0_WIDE;
        lBtnAttr->attr1 = ATTR1_XPOS(184) | ATTR1_SIZE(0);
        lBtnAttr->attr2 = ATTR2_BASE_TILE(18) | ATTR2_PRIORITY(3) | ATTR2_PALBANK(0);

        oamManager->copyBuffer(3);
}

ControlsScreen::~ControlsScreen(){
    padAttr->attr0 = ATTR0_HIDE;
    rBtnAttr->attr0 = ATTR0_HIDE;
    lBtnAttr->attr0 = ATTR0_HIDE;
    oamManager->copyBuffer(3);

    delete textLayer;
    textLayer = nullptr;
}

void ControlsScreen::update(){
    if(inputManager->keyWentDown(KEY_B) || inputManager->keyRepeated(KeyIndex::KEY_DOWN))
        ScreenManager::getInstance()->setScreen(ScreenType::START_SCREEN);
}
