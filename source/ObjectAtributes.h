#ifndef OBJECTATTRIBUTES_H
#define OBJECTATTRIBUTES_H

///Defines for attribute zero

#define ATTR0_YPOS_MASK 0x00FF
#define ATTR0_YPOS(n) (n)
#define ATTR0_REG    0x0000
#define ATTR0_HIDE   0x0200
#define ATTR0_8BPP   0x2000
#define ATTR0_SQUARE 0x0000

///Defines for attribute one

#define ATTR1_XPOS_MASK 0x01FF
#define ATTR1_XPOS(n) (n)
#define ATTR1_SIZE_SHIFT  14
#define ATTR1_SIZE(n)    ((n)<<ATTR1_SIZE_SHIFT)

///Defines for attribute two

#define ATTR2_BASE_TILE(n) (n)
#define ATTR2_PRIORITY_SHIFT 10
#define ATTR2_PIORITY(n) ((n)<<ATTR2_PRIORITY_SHIFT)

#endif // OBJECTATTRIBUTES_H
