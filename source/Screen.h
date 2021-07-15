#ifndef SCREEN_H
#define SCREEN_H

enum class ScreenType{
    START_SCREEN = 0,
    PLAY_SCREEN,
    CONTROLS_SCREEN,
};

class Screen{
    private:
        ScreenType type;

    public:
        Screen(ScreenType type);
        virtual ~Screen();

        ScreenType getType();

        virtual void update() = 0;
};

#endif // SCREEN_H
