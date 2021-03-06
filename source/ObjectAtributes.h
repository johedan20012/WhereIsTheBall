#ifndef OBJECTATTRIBUTES_H
#define OBJECTATTRIBUTES_H

///Defines for attribute zero

#define ATTR0_YPOS_MASK 0x00FF
#define ATTR0_YPOS_SHIFT 0
#define ATTR0_YPOS(n) (n)
#define ATTR0_REG    0x0000
#define ATTR0_HIDE   0x0200
#define ATTR0_MODE_MASK 0x300
#define ATTR0_MODE_SHIFT 8
#define ATRR0_MODE(n) ((n)<<ATTR0_MODE_SHIFT)
#define ATTR0_4BPP   0x0000
#define ATTR0_8BPP   0x2000
#define ATTR0_SQUARE 0x0000
#define ATTR0_WIDE   0X4000
#define ATTR0_TALL   0x8000

///Defines for attribute one

#define ATTR1_XPOS_MASK 0x01FF
#define ATTR1_XPOS_SHIFT 0
#define ATTR1_XPOS(n) (n)
#define ATTR1_SIZE_SHIFT  14
#define ATTR1_SIZE(n)    ((n)<<ATTR1_SIZE_SHIFT)

///Defines for attribute two

#define ATTR2_BASE_TILE(n) (n)
#define ATTR2_PRIORITY_SHIFT 10
#define ATTR2_PRIORITY(n) ((n)<<ATTR2_PRIORITY_SHIFT)
#define ATTR2_PALBANK_MASK 0xF000
#define ATTR2_PALBANK_SHIFT 12
#define ATTR2_PALBANK(n) ((n)<<ATTR2_PALBANK_SHIFT)

///Defines for bit set
#define BF_PREP(x, name)         ( ((x)<<name##_SHIFT)& name##_MASK  )
#define BF_SET(y, x, name)       (y = ((y)&~name##_MASK) | BF_PREP(x,name) )

#endif // OBJECTATTRIBUTES_H
