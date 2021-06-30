#ifndef MEMORYDEFINES_H
#define MEMORYDEFINES_H

#include <gba_types.h>
#include <gba_video.h>

#define BGPAL_ADR  0x5000000
#define OBJPAL_ADR 0x5000200

#define OAM_ADR 0x7000000

struct TILE8{
    u32 data[16];
};
struct TILE4{
    u32 data[8];
};
typedef TILE8 CHARBLOCK8[256];
typedef TILE4 CHARBLOCK4[512];
#define TILE8_MEMORY ((CHARBLOCK8*)VRAM)
#define TILE4_MEMORY ((CHARBLOCK4*)VRAM)

typedef u16 SCRBLOCK_ENTRY;
typedef SCRBLOCK_ENTRY SCREENBLOCK[1024];
#define SCRBLOCK_MEMORY ((SCREENBLOCK*)VRAM)

typedef u16 PAL_ENTRY;
#define BGPAL_MEMORY  ((PAL_ENTRY*)BGPAL_ADR)
#define OBJPAL_MEMORY ((PAL_ENTRY*)OBJPAL_ADR)

typedef struct S_OBJ_ATTR{
    u16 attr0;
    u16 attr1;
    u16 attr2;
    s16 unused;
} OBJ_ATTR;
#define OBJ_ATTR_MEMORY ((OBJ_ATTR*)OAM_ADR)


#endif // MEMORYDEFINES_H
