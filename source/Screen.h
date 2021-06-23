#ifndef SCREEN_H
#define SCREEN_H

enum class ScreenType{
    START_SCREEN,
    PLAY_SCREEN
};

class Screen{
    private:
        ScreenType type;

    public:
        Screen(ScreenType type);

        ScreenType getType();

        virtual void update() = 0;
};

#endif // SCREEN_H
