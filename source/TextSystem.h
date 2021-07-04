#ifndef TEXTSYSTEM_H
#define TEXTSYSTEM_H

#include <gba_types.h>

#include "MemoryDefines.h"

class TextSystem{
    private:
        u16 *se0;
        u8 characters[256];
        u16 flags;

    public:
        TextSystem(u32 screenBlock,u32 charBlock,u32 *font,u32 fontLength);
        ~TextSystem();

        void puts(int x,int y,const char *str);
    private:
        void unpackFont4BPP(void *destination,const void *source,u32 length);
};

#endif // TEXTSYSTEM_H
