#include "Game.h"

#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>

Game::Game(){
    /// (MODE_0) Set video mode to mode 0, 4 backgrounds with no rotation or scaling
    /// (OBJ_1D_MAP) Set object mapping mode to 1D
    REG_DISPCNT = MODE_0 | OBJ_1D_MAP | BG0_ENABLE | OBJ_ENABLE;

    /// Set max priority to BG 0, Character block to 1, 256 colors/ 1 palette, Screeen block base to 0, and size 256x256 px (32x32 tiles)
    REG_BG0CNT = BG_PRIORITY(0) | CHAR_BASE(1) | BG_256_COLOR | SCREEN_BASE(0) | BG_SIZE_0;

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
