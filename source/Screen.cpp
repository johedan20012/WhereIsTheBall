#include "Screen.h"

Screen::Screen(ScreenType type)
    :type(type) {}

Screen::~Screen(){}

ScreenType Screen::getType(){
    return type;
}
