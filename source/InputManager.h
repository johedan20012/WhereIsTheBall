#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#define KEY_MASK 0x03FF

#include <gba_types.h>
#include <gba_input.h>

class InputManager{
    private:
            static InputManager* instance;

            u16 currentState, previousState;

            InputManager();
            ~InputManager();
    public:
            static InputManager* getInstance();
            void update();

            u32 key(u32 key);
            u32 keyWentDown(u32 key);
            u32 keyWentUp(u32 key);
};

#endif // INPUTMANAGER_H
