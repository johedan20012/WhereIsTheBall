#include "ScreenManager.h"

#include "PlayScreen.h"
#include "StartScreen.h"

ScreenManager* ScreenManager::instance = nullptr;

ScreenManager::ScreenManager()
    :currentScreen(new StartScreen()){}

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
    currentScreen->update();
}

void ScreenManager::setScreen(ScreenType screenType){
    delete currentScreen;
    if(screenType == ScreenType::START_SCREEN){
        currentScreen = new StartScreen();
        return;
    }
    currentScreen = new PlayScreen();
}
