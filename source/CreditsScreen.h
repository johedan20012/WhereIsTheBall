#ifndef CREDITSSCREEN_H
#define CREDITSSCREEN_H

#include "Screen.h"
#include "TextSystem.h"
#include "InputManager.h"

class CreditsScreen : public Screen{
    private:
        TextSystem* textLayer;
        InputManager* inputManager;

    public:
        CreditsScreen();
        ~CreditsScreen() override;

        void update() override;

};

#endif // CREDITSSCREEN_H
