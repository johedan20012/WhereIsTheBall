#include "InputManager.h"

#include <gba_input.h>

InputManager* InputManager::instance = nullptr;

InputManager::InputManager(){
    currentState = 0;
    previousState = 0;
}

InputManager::~InputManager(){}

InputManager* InputManager::getInstance(){
    if(instance == nullptr){
        instance = new InputManager();
    }

    return instance;
}

void InputManager::update(){
    previousState = currentState;
    currentState = ~REG_KEYINPUT & KEY_MASK;
}

u32 InputManager::key(u32 key){
    return currentState & key;
}

u32 InputManager::keyWentDown(u32 key){
    return (currentState & ~previousState) &key;
}

u32 InputManager::keyWentUp(u32 key){
    return (previousState & ~currentState) &key;
}
