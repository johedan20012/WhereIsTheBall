#include "Game.h"

#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>

Game::Game(){
    ///Enable interrupts
    irqInit();
	irqEnable(IRQ_VBLANK); ///Enable Vertical Blank interrupt

	screenManager = ScreenManager::getInstance();
	inputManager = InputManager::getInstance();
}

void Game::run(){
    while (1) {
		VBlankIntrWait();
	}
}
