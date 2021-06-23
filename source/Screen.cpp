#include "Screen.h"

Screen::Screen(ScreenType type)
    :type(type) {}

ScreenType Screen::getType(){
    return type;
}
