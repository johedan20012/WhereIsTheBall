#include "PlayScreen.h"

#include <string.h>
#include <stdlib.h>

#include <gba_video.h>

#include "ScreenManager.h"

///Sprites
#include "SprCup.h"
#include "SprBall.h"
#include "SprCursorPause.h"
#include "SprCursorSelect.h"

PlayScreen::PlayScreen()
    :Screen(ScreenType::PLAY_SCREEN),scrState(PlayScreenState::PLAYING),playState(PlayState::BET)
    ,cupsMoveFlags(0),showBall(0),cupSelected(2),coins(1000),highscore(200000),multiplier(2),bet(1),minimumBet(1),level(1),pOpSelected(0){

    srand(REG_VCOUNT);

    REG_DISPCNT ^= BG1_ON;
    REG_DISPCNT |= BG2_ON;

    inputManager = InputManager::getInstance();

    ///Get oamManager instance
    oamManager = OAMManager::getInstance();

    ///Copy sprite tiles and palette
    memcpy(&OBJPAL_MEMORY[0],SprCupPal,SprCupPalLen);
    memcpy(&TILE4_MEMORY[4][0], SprCupTiles, SprCupTilesLen);

    memcpy(&OBJPAL_MEMORY[16],SprBallPal,SprBallPalLen);
    memcpy(&TILE4_MEMORY[4][32], SprBallTiles,SprBallTilesLen);

    memcpy(&OBJPAL_MEMORY[32],SprCursorPausePal,SprCursorPausePalLen);
    memcpy(&TILE4_MEMORY[4][36],SprCursorPauseTiles,SprCursorPauseTilesLen);

    memcpy(&OBJPAL_MEMORY[48],SprCursorSelectPal,SprCursorSelectPalLen);
    memcpy(&TILE4_MEMORY[4][37],SprCursorSelectTiles,SprCursorSelectTilesLen);

    ///Set cups sprite attributes
    for(u32 i = 0; i<5; i++){
        cups[i] = new Cup(oamManager->getAttrPtr(i),30+i*36,62,((i>0 && i<4)? ATTR0_REG:ATTR0_HIDE));
    }

    ///Set ball sprite attributes
    ballAttr = oamManager->getAttrPtr(5);
    ballAttr->attr0 = ATTR0_YPOS(86) | ATTR0_HIDE | ATTR0_4BPP | ATTR0_SQUARE;
    ballAttr->attr1 = ATTR1_XPOS(32) | ATTR1_SIZE(1);
    ballAttr->attr2 = ATTR2_BASE_TILE(32) | ATTR2_PRIORITY(2) | ATTR2_PALBANK(1);

    ///Set cursor sprite attributes
    cursorAttr = oamManager->getAttrPtr(6);
    cursorAttr->attr0 = ATTR0_YPOS(72) | ATTR0_HIDE | ATTR0_4BPP | ATTR0_SQUARE;
    cursorAttr->attr1 = ATTR1_XPOS(80) | ATTR1_SIZE(0);
    cursorAttr->attr2 = ATTR2_BASE_TILE(36) | ATTR2_PRIORITY(0) | ATTR2_PALBANK(2);

    cursorSelectAttr = oamManager->getAttrPtr(7);
    cursorSelectAttr->attr0 = ATTR0_YPOS(32) | ATTR0_HIDE | ATTR0_4BPP | ATTR0_SQUARE;
    cursorSelectAttr->attr1 = ATTR1_XPOS(80) | ATTR1_SIZE(1);
    cursorSelectAttr->attr2 = ATTR2_BASE_TILE(37) | ATTR2_PRIORITY(1) | ATTR2_PALBANK(3);

    textLayer = new TextSystem(2,2,toncfontTiles,sizeof(toncfontTiles));
    textLayer->puts(1,0,"Multiplier");
    textLayer->puts(5,1,"x2");
    textLayer->puts(13,0,"Level");
    textLayer->puts(20,0,"Highscore");

    textLayer->puts(6,17,"Coins");
    textLayer->puts(20,17,"Bet");

    inputManager->setKeyRepeat(KeyIndex::KEY_UP,30);
    inputManager->setKeyRepeat(KeyIndex::KEY_DOWN,30);

    updateCHB();

    oamManager->copyBuffer(NUM_SPRITES);
}

PlayScreen::~PlayScreen(){
    ///Hide sprites
    for(u32 i = 0; i<5; i++){
        delete cups[i];
        cups[i] = nullptr;
    }
    ballAttr->attr0 = ATTR0_HIDE;
    cursorAttr->attr0 = ATTR0_HIDE;
    cursorSelectAttr->attr0 = ATTR0_HIDE;
    oamManager->copyBuffer(NUM_SPRITES);

    inputManager->setKeyRepeat(KeyIndex::KEY_UP,0);
    inputManager->setKeyRepeat(KeyIndex::KEY_DOWN,0);

    delete textLayer;
    textLayer = nullptr;
}

void PlayScreen::update(){
    if(scrState == PlayScreenState::PLAYING){
        if(inputManager->keyWentDown(KEY_START)){
            scrState = PlayScreenState::PAUSE;
            textLayer->puts(10,5,"GAME PAUSED");
            textLayer->puts(12,9,"Resume");
            textLayer->puts(12,11,"Quit");
            BF_SET(cursorAttr->attr0,0,ATTR0_MODE);
        }

        switch(playState){
            case PlayState::BET: ///Player can select the multiplier and the bet
                addBet = 0;
                if(inputManager->keyWentDown(KEY_L))
                    multiplier--;
                if(inputManager->keyWentDown(KEY_R))
                    multiplier++;
                if(inputManager->keyWentDown(KEY_UP) || inputManager->keyRepeated(KeyIndex::KEY_UP))
                    addBet++;
                if(inputManager->keyWentDown(KEY_DOWN) || inputManager->keyRepeated(KeyIndex::KEY_DOWN))
                    addBet--;
                if(inputManager->keyWentDown(KEY_A)){
                    playState = PlayState::SHUFFLE;
                    updateMultiplier();


                    ballPos = (rand()% (multiplier+1)) + ((multiplier>2)? 0:1);
                    BF_SET(ballAttr->attr0,0,ATTR0_MODE);
                    BF_SET(ballAttr->attr1,cups[ballPos]->getXPos()+8,ATTR1_XPOS);
                    showBall = 1;
                    cups[ballPos]->moveTo(cups[ballPos]->getXPos(),30,2,1);
                    ///Set # of shuffles and their data
                    generateRandomShuffles();
                }

                changeBet(addBet);
                updateMultiplier();

                oamManager->copyBuffer(NUM_SPRITES);
                updateCHB();
                break;
            case PlayState::SHUFFLE:
                ///Bet disable and shuffle of the cups
                switch(showBall){
                    case 1:
                        if(!cups[ballPos]->isMoving()){
                            cups[ballPos]->moveTo(cups[ballPos]->getXPos(),62,2,1);
                            showBall = 2;
                        }
                        updateCupsShuffle();
                        return;
                    case 2:
                        if(!cups[ballPos]->isMoving()){
                            BF_SET(ballAttr->attr0,2,ATTR0_MODE);
                            showBall = 0;
                        }
                        updateCupsShuffle();
                        return;
                }

                if(!cupsMoveFlags){
                    if(actShuffle >= shuffles){
                        playState = PlayState::SELECTION;
                        BF_SET(cursorSelectAttr->attr0,0,ATTR0_MODE);
                    }

                    u32 cup1,cup2,layer = (shufflesData[actShuffle] & SHUFFLE_LAYER_MASK)>>6;
                    u32 word,mode;
                    do{
                        if(actShuffle >= shuffles) break;
                        word = shufflesData[actShuffle];
                        cup1 = (word & SHUFFLE_CUP1_MASK);
                        cup2 = (word & SHUFFLE_CUP2_MASK)>>3;
                        layer = (word & SHUFFLE_LAYER_MASK)>>6;
                        mode = (word & SHUFFLE_MODE_MASK)>>7;
                        swapCups(cup1,cup2,mode);

                    }while(layer == ((shufflesData[++actShuffle] & SHUFFLE_LAYER_MASK)>>6));
                }else{
                    updateCupsShuffle();
                }
                break;
            case PlayState::SELECTION:
                ///Selection of the cup that may have the ball
                if(inputManager->keyWentDown(KEY_LEFT))
                    curSelectPos--;
                if(inputManager->keyWentDown(KEY_RIGHT))
                    curSelectPos++;
                updateSelectionCursor();
                if(inputManager->keyWentDown(KEY_A)){
                    BF_SET(cursorSelectAttr->attr0,2,ATTR0_MODE);
                    playState = PlayState::PRIZE;
                    BF_SET(ballAttr->attr0,0,ATTR0_MODE);
                    BF_SET(ballAttr->attr1,cups[ballPos]->getXPos()+8,ATTR1_XPOS);
                    cups[cupSelected]->moveTo(cups[cupSelected]->getXPos(),30,2,1);
                    showBall = 1;
                }

                oamManager->copyBuffer(NUM_SPRITES);
                break;
            case PlayState::PRIZE:
                ///Ball reveal and prize delivery in case of correct guessing
                switch(showBall){
                    case 1:
                        if(!cups[cupSelected]->isMoving()){
                            if(ballPos != cupSelected){
                                cups[ballPos]->moveTo(cups[ballPos]->getXPos(),30,2,1);
                                showBall = 2;
                            }else{
                                cups[ballPos]->moveTo(cups[ballPos]->getXPos(),62,2,1);
                                showBall = 3;
                            }
                        }
                        updateCupsShuffle();
                        return;
                    case 2:
                        if(!cups[ballPos]->isMoving()){
                            cups[cupSelected]->moveTo(cups[cupSelected]->getXPos(),62,2,1);
                            cups[ballPos]->moveTo(cups[ballPos]->getXPos(),62,2,1);
                            showBall = 3;
                        }
                        updateCupsShuffle();
                        return;
                    case 3:
                        if(!cups[ballPos]->isMoving() && !cups[cupSelected]->isMoving()){
                            showBall = 4;
                        }
                        updateCupsShuffle();
                        return;
                    case 4:
                        if(ballPos != cupSelected){
                            if(coins == 0)
                                scrState = PlayScreenState::GAMEOVER;
                            bet = 1;
                            coins --;
                        }else{
                            coins += bet*multiplier;
                            if(coins > highscore)
                                highscore = coins;
                            bet = 1;
                            coins --;
                        }
                        winnedCoins += bet*multiplier;
                        level = (winnedCoins /200) + 1;
                        if(level > 20) level = 20;
                        playState = PlayState::BET;
                        return;
                };
                break;
        }
    }else if(scrState == PlayScreenState::PAUSE){
        if(inputManager->keyWentDown(KEY_UP))
            pOpSelected = 0;
        if(inputManager->keyWentDown(KEY_DOWN))
            pOpSelected = 1;
        if(inputManager->keyWentDown(KEY_START)){
            if(pOpSelected)
                ScreenManager::getInstance()->setScreen(ScreenType::START_SCREEN);

            scrState = PlayScreenState::PLAYING;
            textLayer->clean(10,5,11);
            textLayer->clean(12,9,6);
            textLayer->clean(12,11,4);
            BF_SET(cursorAttr->attr0,2,ATTR0_MODE);
        }

        BF_SET(cursorAttr->attr0,72+pOpSelected*16,ATTR0_YPOS);

        oamManager->copyBuffer(NUM_SPRITES);
    }else{ ///Gameover state
        if(inputManager->keyWentDown(KEY_START))
           ScreenManager::getInstance()->setScreen(ScreenType::START_SCREEN);
    }
}

void PlayScreen::updateCHB(){
    textLayer->puts(20,1,highscore,9);
    textLayer->puts(4,18,coins,9);
    textLayer->puts(18,18,bet,7);
    textLayer->puts(15,1,level,2);
}

void PlayScreen::changeBet(int amount){
    u32 value = (amount < 0)? amount*-1 : amount;
    if(amount > 0 && value <= coins){
        coins -= value;
        bet += value;
    }else if(amount <0 && value <= bet){
        if((bet-value) < minimumBet) return;
        coins += value;
        bet -= value;
    }
}

void PlayScreen::updateMultiplier(){
    multiplier = (multiplier < 2)? 2 : (multiplier > 4)? 4 : multiplier;

    cups[0]->changeMode((multiplier == 2)? 2:0);
    cups[4]->changeMode((multiplier == 4)? 0:2);

    textLayer->puts(6,1,multiplier,1);
}

void PlayScreen::fillNotUsedCupStack(int cups){
    while(!cupsNotUsed.empty())
        cupsNotUsed.pop();
    for(int i = 0; i<5; i++){
        if(cups & (1<<i))
            cupsNotUsed.push(i);
    }
}

int PlayScreen::getRandomCup(){
    int offset = rand()%5;
    if(cupsNotUsed.empty())
        return -1;

    int ret;
    do{
        ret = cupsNotUsed.front();
        cupsNotUsed.pop();
        if(offset > 0)
            cupsNotUsed.push(ret);
    }while(offset--);

    return ret;
}

void PlayScreen::generateRandomShuffles(){
    shuffles = 0;
    u32 numShuffleLayers = (level+15)/4,typeLayer;
    u32 layer = 0;
    int cup1,cup2,cup3,cup4,cup5;

    for(u32 i = 0; i<numShuffleLayers; i++){
        typeLayer = rand()%((multiplier-1)*2);
        fillNotUsedCupStack(14 | ((multiplier>2)?1:0) | ((multiplier>3)?16:0) );

        switch(typeLayer){
            case 0:
                cup1 = getRandomCup();
                cup2 = getRandomCup();

                shufflesData[shuffles++] = (layer << 6 | cup2 << 3 | cup1);
                break;
            case 1:
                cup1 = getRandomCup();
                cup2 = getRandomCup();
                cup3 = getRandomCup();

                shufflesData[shuffles++] = (0x80 | layer<<6 | cup2<<3 |cup1);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup3<<3 |cup2);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup1<<3 |cup3);
                break;
            case 2:
                cup1 = getRandomCup();
                cup2 = getRandomCup();
                cup3 = getRandomCup();
                cup4 = getRandomCup();

                shufflesData[shuffles++] = ( layer<<6 | cup2<<3 |cup1);
                shufflesData[shuffles++] = ( layer<<6 | cup4<<3 |cup3);
                break;
            case 3:
                cup1 = getRandomCup();
                cup2 = getRandomCup();
                cup3 = getRandomCup();
                cup4 = getRandomCup();

                shufflesData[shuffles++] = (0x80 | layer<<6 | cup3<<3 |cup1);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup1<<3 |cup2);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup4<<3 |cup3);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup2<<3 |cup4);
                break;
            case 4:
            case 5:
                cup1 = getRandomCup();
                cup2 = getRandomCup();
                cup3 = getRandomCup();
                cup4 = getRandomCup();
                cup5 = getRandomCup();

                shufflesData[shuffles++] = (       layer<<6 | cup2<<3 |cup1);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup4<<3 |cup3);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup5<<3 |cup4);
                shufflesData[shuffles++] = (0x80 | layer<<6 | cup3<<3 |cup5);
                break;
        };
        layer = (layer+1) %2;
    }
    actShuffle = 0;
}

 void PlayScreen::swapCups(int c1,int c2,int mode){
    cupsMoveFlags |= (1<<c1);

    cups[c1]->moveTo(cups[c2]->getXPos(),cups[c2]->getYPos(),0,level);
    if(mode == 0){
        cups[c2]->moveTo(cups[c1]->getXPos(),cups[c1]->getYPos(),0,level);
        cupsMoveFlags |=  (1<<c2);
    }
 }

 void PlayScreen::updateCupsShuffle(){
    for(int i = 0; i<5; i++){
        cups[i]->update();
        if(!cups[i]->isMoving())
            cupsMoveFlags &= ~(1<<i);
    }

    oamManager->copyBuffer(NUM_SPRITES);
 }

 void PlayScreen::updateSelectionCursor(){
    if(curSelectPos <=0)
        curSelectPos = (curSelectPos >2)? 0:1;
    if(curSelectPos >= 4)
        curSelectPos = (curSelectPos == 4)? 4:3;

    BF_SET(cursorSelectAttr->attr1,36+curSelectPos*36,ATTR1_XPOS);
    for(int i =0 ; i<5;i++){
        if(cups[i]->getXPos() == 30+curSelectPos*36){
            cupSelected = i;
            return;
        }
    }
 }

///Tonc font
u32 PlayScreen::toncfontTiles[192] = {0x00000000, 0x00000000, 0x18181818, 0x00180018, 0x00003636, 0x00000000, 0x367F3636, 0x0036367F,
    0x3C067C18, 0x00183E60, 0x1B356600, 0x0033566C, 0x6E16361C, 0x00DE733B, 0x000C1818, 0x00000000,
    0x0C0C1830, 0x0030180C, 0x3030180C, 0x000C1830, 0xFF3C6600, 0x0000663C, 0x7E181800, 0x00001818,
    0x00000000, 0x0C181800, 0x7E000000, 0x00000000, 0x00000000, 0x00181800, 0x183060C0, 0x0003060C,
    0x7E76663C, 0x003C666E, 0x181E1C18, 0x00181818, 0x3060663C, 0x007E0C18, 0x3860663C, 0x003C6660,
    0x33363C38, 0x0030307F, 0x603E067E, 0x003C6660, 0x3E060C38, 0x003C6666, 0x3060607E, 0x00181818,
    0x3C66663C, 0x003C6666, 0x7C66663C, 0x001C3060, 0x00181800, 0x00181800, 0x00181800, 0x0C181800,
    0x06186000, 0x00006018, 0x007E0000, 0x0000007E, 0x60180600, 0x00000618, 0x3060663C, 0x00180018,

    0x5A5A663C, 0x003C067A, 0x7E66663C, 0x00666666, 0x3E66663E, 0x003E6666, 0x06060C78, 0x00780C06,
    0x6666361E, 0x001E3666, 0x1E06067E, 0x007E0606, 0x1E06067E, 0x00060606, 0x7606663C, 0x007C6666,
    0x7E666666, 0x00666666, 0x1818183C, 0x003C1818, 0x60606060, 0x003C6660, 0x0F1B3363, 0x0063331B,
    0x06060606, 0x007E0606, 0x6B7F7763, 0x00636363, 0x7B6F6763, 0x00636373, 0x6666663C, 0x003C6666,
    0x3E66663E, 0x00060606, 0x3333331E, 0x007E3B33, 0x3E66663E, 0x00666636, 0x3C0E663C, 0x003C6670,
    0x1818187E, 0x00181818, 0x66666666, 0x003C6666, 0x66666666, 0x00183C3C, 0x6B636363, 0x0063777F,
    0x183C66C3, 0x00C3663C, 0x183C66C3, 0x00181818, 0x0C18307F, 0x007F0306, 0x0C0C0C3C, 0x003C0C0C,
    0x180C0603, 0x00C06030, 0x3030303C, 0x003C3030, 0x00663C18, 0x00000000, 0x00000000, 0x003F0000,

    0x00301818, 0x00000000, 0x603C0000, 0x007C667C, 0x663E0606, 0x003E6666, 0x063C0000, 0x003C0606,
    0x667C6060, 0x007C6666, 0x663C0000, 0x003C067E, 0x0C3E0C38, 0x000C0C0C, 0x667C0000, 0x3C607C66,
    0x663E0606, 0x00666666, 0x18180018, 0x00301818, 0x30300030, 0x1E303030, 0x36660606, 0x0066361E,
    0x18181818, 0x00301818, 0x7F370000, 0x0063636B, 0x663E0000, 0x00666666, 0x663C0000, 0x003C6666,
    0x663E0000, 0x06063E66, 0x667C0000, 0x60607C66, 0x663E0000, 0x00060606, 0x063C0000, 0x003E603C,
    0x0C3E0C0C, 0x00380C0C, 0x66660000, 0x007C6666, 0x66660000, 0x00183C66, 0x63630000, 0x00367F6B,
    0x36630000, 0x0063361C, 0x66660000, 0x0C183C66, 0x307E0000, 0x007E0C18, 0x0C181830, 0x00301818,
    0x18181818, 0x00181818, 0x3018180C, 0x000C1818, 0x003B6E00, 0x00000000, 0x00000000, 0x00000000,};
