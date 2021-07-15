#ifndef SAVE_H
#define SAVE_H

#include "MemoryDefines.h"

namespace Save{

    extern const u8 magicBytes[4];

    void readHighscore(u32* highscore);

    void saveHighScore(u32 highscore);
};

#endif // SAVE_H
