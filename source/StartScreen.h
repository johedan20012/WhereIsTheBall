#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "Screen.h"
#include "MemoryDefines.h"
#include "OAMManager.h"

class StartScreen : public Screen{
    private:
        u32 optionSelected;

        OAMManager* oamManager;

        OBJ_ATTR* cursorAttr;
    public:
        StartScreen();

        void update() override;
};

#endif // STARTSCREEN_H
