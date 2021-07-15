#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "Screen.h"
#include "MemoryDefines.h"
#include "OAMManager.h"
#include "InputManager.h"

class StartScreen : public Screen{
    private:
        u32 optionSelected;

        OAMManager* oamManager;
        InputManager* inputManager;

        OBJ_ATTR* cursorAttr;
    public:
        StartScreen();
        ~StartScreen() override;

        void update() override;
};

#endif // STARTSCREEN_H
