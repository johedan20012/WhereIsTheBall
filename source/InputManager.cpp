#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager(){
    currentState = 0;
    previousState = 0;

    for(u32 i = 0; i<10; i++){
        repeatFrames[i] = 0;
        currRepeatFrames[i] = 0;
    }
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

    for(u32 i = 0; i<10; i++){
        if(currentState & (1<<i)){
            currRepeatFrames[i] = (currRepeatFrames[i] > 0)? currRepeatFrames[i]-1 : currRepeatFrames[i];
        }else{
            currRepeatFrames[i] = repeatFrames[i];
        }
    }
}

void InputManager::setKeyRepeat(KeyIndex keyIndex,u32 repeatFrm){
    repeatFrames[static_cast<int>(keyIndex)] = repeatFrm;
    currRepeatFrames[static_cast<int>(keyIndex)] = repeatFrm;
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

u32 InputManager::keyRepeated(KeyIndex keyIndex){
    return (currRepeatFrames[static_cast<int>(keyIndex)] == 0 && repeatFrames[static_cast<int>(keyIndex)] > 0)? 1 : 0;
}
