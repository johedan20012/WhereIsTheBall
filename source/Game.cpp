#include "Game.h"

#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

#include <string.h>

///Bg images
#include "ImgGameBG.h"
#include "ImgPlayBG.h"

///BG palette
Game::Game(){
    /// (MODE_0) Set video mode to mode 0, 4 backgrounds with no rotation or scaling
    /// (OBJ_1D_MAP) Set object mapping mode to 1D
    REG_DISPCNT = MODE_0 | OBJ_1D_MAP | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | OBJ_ENABLE;

    /// Set priority to 3, Character block to 1, 16 colors/ 16 palettes, Screeen block base to 0, and size 256x256 px (32x32 tiles)
    REG_BG0CNT = BG_PRIORITY(3) | CHAR_BASE(1) | BG_16_COLOR | SCREEN_BASE(0) | BG_SIZE_0;

    ///Copy image palette, tile data and map of the game background
    memcpy(&BGPAL_MEMORY[0], ImgGameBGPal, ImgGameBGPalLen);
    memcpy(&TILE8_MEMORY[1][0], ImgGameBGTiles, ImgGameBGTilesLen);
    memcpy(&SCRBLOCK_MEMORY[0][0], ImgGameBGMap, ImgGameBGMapLen);


    REG_BG2CNT = BG_PRIORITY(2) | CHAR_BASE(3) | BG_16_COLOR | SCREEN_BASE(5) | BG_SIZE_0;
    ///Copy image palette, tile data and map of the play background
    memcpy(&BGPAL_MEMORY[32], ImgPlayBGPal, ImgPlayBGPalLen);
    memcpy(&TILE8_MEMORY[3][0], ImgPlayBGTiles, ImgPlayBGTilesLen);
    memcpy(&SCRBLOCK_MEMORY[5][0], ImgPlayBGMap, ImgPlayBGMapLen);

    /// Enable interrupts
    irqInit();
	irqEnable(IRQ_VBLANK); /// Enable Vertical Blank interrupt

    oamManager = OAMManager::getInstance();
	screenManager = ScreenManager::getInstance();
	inputManager = InputManager::getInstance();
}

void Game::run(){
    while (1) {
		VBlankIntrWait();

		inputManager->update();

		screenManager->update();
	}
}
