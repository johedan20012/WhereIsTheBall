#include "TextSystem.h"

TextSystem::TextSystem(u32 screenBlock,u32 charBlock,u32 *font,u32 fontLength)
    : se0(&SCRBLOCK_MEMORY[screenBlock][0]){

    ///Enable BG3
    REG_DISPCNT |= BG3_ENABLE;

    ///Set bg2 control
    REG_BG3CNT = BG_PRIORITY(0) | CHAR_BASE(charBlock) | BG_16_COLOR | SCREEN_BASE(screenBlock) | BG_SIZE_0;

    ///Unpack and copy font data to charblock
    unpackFont4BPP(&TILE4_MEMORY[charBlock][0],font,fontLength);

    ///Set two colors of pallete number 3
    int base = 48;
    BGPAL_MEMORY[base] = 0;
    BGPAL_MEMORY[base+1] = 0xFFFF;

    for(u32 i=0; i<96; i++)
        characters[i+32]= i;

    ///Clean the map
    for(u32 x=0; x<30; x++){
        for(u32 y=0; y<20; y++){
            se0[y*32+x] = characters[' '];
        }
    }
}

TextSystem::~TextSystem(){
    ///Hide BG3
    REG_DISPCNT ^= BG3_ENABLE;
}

void TextSystem::puts(int x,int y,const char *str){
    int c;
    SCRBLOCK_ENTRY *dst= &se0[y*32+x];

    x=0;
    while((c=*str++) != 0){
        if(c == '\n'){
            dst += (x&~31) + 32;
            x=0;
        }else{
            dst[x++] = characters[c] | 0x3000; ///Set to palette number 3
        }
    }
}

void TextSystem::puts(int x,int y,u32 number,u32 nDigits){
    int digit;
    SCRBLOCK_ENTRY *dst = &se0[y*32+x];
    x = nDigits-1;
    do{
        digit = number % 10;
        number /= 10;
        dst[x--] = characters['0'+digit] | 0x3000;
    }while(nDigits-- && number !=0);
    while(nDigits--){
        dst[x--] = 0;
    }
}

void TextSystem::clean(int x,int y,u32 numSpaces){
    int c;
    SCRBLOCK_ENTRY *dst= &se0[y*32+x];

    x=0;
    while(numSpaces--){
        dst[x++] = characters[' '] | 0x3000; ///Set to palette number 3
    }
}

void TextSystem::unpackFont4BPP(void *destination, const void *source, u32 length){
    u32* src = (u32*)source;
    u32* dst = (u32*)destination;

    length = (length*4+3)>>2; /// # of words in destination

    u32 srcWord, srcShift = 32;
    u32 dstWord, dstShift;
    while(length--){
        if(srcShift >= 32){
            srcWord = *src++;
            srcShift = 0;
        }
        dstWord = 0;
        for(dstShift=0; dstShift<32; dstShift+=4){
            u32 word = (srcWord & 1);
            dstWord |= word<<dstShift;
            srcWord >>= 1;
            srcShift++;
        }
        *dst++ = dstWord;
    }
}
