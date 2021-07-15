#ifndef CONTROLSSCREEN_H
#define CONTROLSSCREEN_H

#include "Screen.h"
#include "TextSystem.h"
#include "InputManager.h"
#include "OAMManager.h"

class ControlsScreen : public Screen{
    private:
        TextSystem* textLayer;
        InputManager* inputManager;
        OAMManager* oamManager;

        OBJ_ATTR* padAttr,*rBtnAttr,*lBtnAttr;
    public:
        ControlsScreen();
        ~ControlsScreen() override;

        void update() override;
};

#endif // CONTROLSSCREEN_H
