#include "Cup.h"

#include <algorithm>

#include "ObjectAtributes.h"
#include "MemoryDefines.h"

Cup::Cup(OBJ_ATTR* objAttr,u32 x,u32 y,u32 mode)
    :objAttr(objAttr),timerVel(0),contTimerVel(0),velocity(0),xPos(x),yPos(y),newXPos(x),newYPos(y){
        this->objAttr->attr0 = ATTR0_YPOS(y) | mode | ATTR0_4BPP | ATTR0_TALL;
        this->objAttr->attr1 = ATTR1_XPOS(x) | ATTR1_SIZE(3);
        this->objAttr->attr2 = ATTR2_BASE_TILE(0) | ATTR2_PRIORITY(1);
}

Cup::~Cup(){
    objAttr->attr0 = ATTR0_HIDE;
}

void Cup::changeMode(u32 mode){
    BF_SET(objAttr->attr0,mode,ATTR0_MODE);
}

void Cup::moveTo(u32 x,u32 y,u32 vel,u32 steps){
    newXPos = x;
    newYPos = y;
    timerVel = vel;
    contTimerVel = 0;
    velocity = steps;
}

u32 Cup::isMoving(){
    return velocity;
}

void Cup::translate(u32 x,u32 y){
    xPos = x;
    yPos = y;

    BF_SET(objAttr->attr0,yPos,ATTR0_YPOS);
    BF_SET(objAttr->attr1,xPos,ATTR1_XPOS);
}

void Cup::update(){
    if(velocity == 0) return;
    contTimerVel++;
    if(contTimerVel < timerVel) return;
    contTimerVel = 0;

    if(xPos < newXPos)
        xPos += std::min(velocity,newXPos-xPos);
    else
        xPos -= std::min(velocity,xPos-newXPos);

    if(yPos < newYPos)
        yPos += std::min(velocity,newYPos-yPos);
    else
        yPos -= std::min(velocity,yPos-newYPos);

    BF_SET(objAttr->attr0,yPos,ATTR0_YPOS);
    BF_SET(objAttr->attr1,xPos,ATTR1_XPOS);

    if(xPos == newXPos && yPos == newYPos)
        velocity = 0;
}

u32 Cup::getXPos(){
    return xPos;
}

u32 Cup::getYPos(){
    return yPos;
}
