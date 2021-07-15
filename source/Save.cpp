#include "Save.h"

namespace Save{
    const u8 magicBytes[4] = {'h','o','l','a'};

    IWRAM_CODE
    void readHighscore(u32* highscore){
        ///Check magic bytes
        for(int i =0; i<4; i++){
            if(GAMEPAK_SRAM_MEMORY[i] != magicBytes[i]){
                *highscore  = 10000;
                return;
            }
        }

        u32 highscr = 0,shift=0;
        for(int i =4; i<8; i++,shift++)
            highscr |= GAMEPAK_SRAM_MEMORY[i] << (shift*8);
        if(highscr < 10000)
            highscr = 10000;
        *highscore = highscr;
    }

    IWRAM_CODE
    void saveHighScore(u32 highscore){
        ///Write magic bytes
        for(int i =0; i<4; i++){
            GAMEPAK_SRAM_MEMORY[i] = magicBytes[i];
        }

        u8 wrByte;
        u32 mask = 0xFF,shift = 0;
        for(int  i=4; i<8; i++,shift++){
            wrByte = (highscore & (mask<<(shift*8))) >> (shift*8);
            GAMEPAK_SRAM_MEMORY[i] = wrByte;
        }
    }
};
