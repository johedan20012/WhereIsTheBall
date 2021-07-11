#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#define KEY_MASK 0x03FF

#include <gba_types.h>
#include <gba_input.h>

enum class KeyIndex{
    KEY_A = 0,
    KEY_B = 1,
    KEY_SELECT = 2,
    KEY_START = 3,
    KEY_RIGHT = 4,
    KEY_LEFT = 5,
    KEY_UP = 6,
    KEY_DOWN = 7,
    KEY_R = 8,
    KEY_L = 9,
};

class InputManager{
    private:
            static InputManager* instance;

            u16 currentState, previousState;
            u32 repeatFrames[10],currRepeatFrames[10]; ///Indicates the frames pressing a especific key to be in repeated state

            InputManager();
            ~InputManager();
    public:
            static InputManager* getInstance();
            void update();

            void setKeyRepeat(KeyIndex keyIndex,u32 repeatFrm);

            u32 key(u32 key);
            u32 keyWentDown(u32 key);
            u32 keyWentUp(u32 key);
            u32 keyRepeated(KeyIndex keyIndex);
};

#endif // INPUTMANAGER_H
