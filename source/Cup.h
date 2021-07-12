#ifndef CUP_H
#define CUP_H

#include "OAMManager.h"

class Cup{
    private:
        OBJ_ATTR* objAttr;
        u32 timerVel;
        u32 contTimerVel;
        u32 velocity;
        u32 xPos;
        u32 yPos;
        u32 newXPos;
        u32 newYPos;

    public:
        Cup(OBJ_ATTR* objAttr,u32 x,u32 y,u32 mode);
        ~Cup();

        void changeMode(u32 mode);

        void moveTo(u32 x,u32 y,u32 vel,u32 steps); ///Move the cup to the specified coords step by step
        u32 isMoving();

        void translate(u32 x,u32 y); ///Instantaneous move of the cup

        void update();

        u32 getXPos();
        u32 getYPos();
};

#endif // CUP_H
