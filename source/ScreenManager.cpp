#include "ScreenManager.h"

#include "PlayScreen.h"
#include "StartScreen.h"
#include "ControlsScreen.h"
#include "CreditsScreen.h"

ScreenManager* ScreenManager::instance = nullptr;

ScreenManager::ScreenManager()
    :currentScreen(new StartScreen()),changeScreen(false){}

ScreenManager::~ScreenManager(){}

ScreenManager* ScreenManager::getInstance(){
    if(instance == nullptr)
        instance = new ScreenManager();

    return instance;
}

void ScreenManager::release(){
    if(instance != nullptr){
        delete instance;
        instance = nullptr;
    }
}

void ScreenManager::update(){
    if(changeScreen){
        changeScreen = false;
        delete currentScreen;
        switch(nextScreen){
            case ScreenType::START_SCREEN:
                currentScreen = new StartScreen();
                break;
            case ScreenType::PLAY_SCREEN:
                currentScreen = new PlayScreen();
                break;
            case ScreenType::CONTROLS_SCREEN:
                currentScreen = new ControlsScreen();
                break;
            case ScreenType::CREDITS_SCREEN:
                currentScreen = new CreditsScreen();
        };
    }
    currentScreen->update();
}

void ScreenManager::setScreen(ScreenType screenType){
    nextScreen = screenType;
    changeScreen = true;
}
