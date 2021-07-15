#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "StartScreen.h"

class ScreenManager{
    private:
        static ScreenManager* instance;

        Screen* currentScreen;
        ScreenType nextScreen;
        bool changeScreen;

        ScreenManager();
        ~ScreenManager();

    public:
        static ScreenManager* getInstance();
        static void release();

        void update();
        void setScreen(ScreenType screenType);
};


#endif // SCREENMANAGER_H
