#include "Game.h"

#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

#include <string.h>

///Bg images
#include "ImgGameBG.h"

///BG palette
Game::Game(){
    /// (MODE_0) Set video mode to mode 0, 4 backgrounds with no rotation or scaling
    /// (OBJ_1D_MAP) Set object mapping mode to 1D
    REG_DISPCNT = MODE_0 | OBJ_1D_MAP | BG0_ENABLE | BG1_ENABLE | OBJ_ENABLE;

    /// Set priority to 1, Character block to 1, 16 colors/ 16 palettes, Screeen block base to 0, and size 256x256 px (32x32 tiles)
    REG_BG1CNT = BG_PRIORITY(1) | CHAR_BASE(2) | BG_16_COLOR | SCREEN_BASE(4) | BG_SIZE_0;

    ///Copy image palette, tile data and map
    memcpy(&BGPAL_MEMORY[0], ImgGameBGPal, ImgGameBGPalLen);

    memcpy(&TILE8_MEMORY[2][0], ImgGameBGTiles, ImgGameBGTilesLen);

    memcpy(&SCRBLOCK_MEMORY[4][0], ImgGameBGMap, ImgGameBGMapLen);

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
