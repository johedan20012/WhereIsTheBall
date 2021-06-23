#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "Screen.h"

class ScreenManager{
    private:
        static ScreenManager* instance;

        Screen* currentScreen;

        ScreenManager();
        ~ScreenManager();

    public:
        static ScreenManager* getInstance();
        static void release();

        void update();
        void setScreen(ScreenType screenType);
};


#endif // SCREENMANAGER_H
