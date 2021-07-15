#include "PlayScreen.h"

#include <string.h>
#include <stdlib.h>

#include <gba_video.h>

#include "ScreenManager.h"

#include "MathFuntions.h"

///Sprites
#include "SprCup.h"
#include "SprBall.h"
#include "SprCursorPause.h"
#include "SprCursorSelect.h"

///Font
#include "FontsData.h"

PlayScreen::PlayScreen()
    :Screen(ScreenType::PLAY_SCREEN),scrState(PlayScreenState::PLAYING),playState(PlayState::BET)
    ,cupsMoveFlags(0),showBall(0),cupSelected(2),coins(10),highscore(100000),multiplier(2),bet(1),minimumBet(1),level(1),pOpSelected(0){

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

    textLayer = new TextSystem(2,2,toncfontTiles,toncfontTilesLen);
    textLayer->puts(1,0,"Multiplier");
    textLayer->puts(5,1,"x2");
    textLayer->puts(13,0,"Level");
    textLayer->puts(20,0,"Highscore");

    textLayer->puts(6,17,"Coins");
    textLayer->puts(20,17,"Bet");

    inputManager->setKeyRepeat(KeyIndex::KEY_UP,30);
    inputManager->setKeyRepeat(KeyIndex::KEY_DOWN,30);
    inputManager->setKeyRepeat(KeyIndex::KEY_RIGHT,30);
    inputManager->setKeyRepeat(KeyIndex::KEY_LEFT,30);
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
    inputManager->setKeyRepeat(KeyIndex::KEY_RIGHT,0);
    inputManager->setKeyRepeat(KeyIndex::KEY_LEFT,0);

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
                if(inputManager->keyWentDown(KEY_RIGHT))
                    addBet+=100;
                else if(inputManager->keyRepeated(KeyIndex::KEY_RIGHT))
                    addBet+=1000;
                if(inputManager->keyWentDown(KEY_LEFT))
                    addBet-=100;
                else if(inputManager->keyRepeated(KeyIndex::KEY_LEFT))
                    addBet-=1000;
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
                    if(actShuffle >= shuffles){ ///ALL the shuffles are finished
                        playState = PlayState::SELECTION;
                        curSelectPos = 2;
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
                    ///Hide the cursor
                    BF_SET(cursorSelectAttr->attr0,2,ATTR0_MODE);

                    ///Change to PRIZE state
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
                        for(int i = 0; i<5; i++){
                            cups[i]->translate(30+i*36,62);
                        }
                        showBall = 5;
                        if(ballPos != cupSelected){
                            textLayer->puts(12,10,"You lose");

                            if(coins == 0){
                                scrState = PlayScreenState::GAMEOVER;
                                textLayer->puts(12,12,"GAMEOVER");
                                return;
                            }
                        }else{
                            textLayer->puts(12,10,"You WIN");

                            coins += bet*multiplier;
                            if(coins > MAX_SCORE) coins = MAX_SCORE;
                            if(coins > highscore)
                                highscore = coins;
                        }
                        bet = 0;
                        setMinimumBetAndLevel();
                        changeBet(minimumBet);
                        updateCHB();
                        BF_SET(ballAttr->attr0,2,ATTR0_MODE);
                        return;
                    case 5:
                        if(inputManager->keyWentDown(KEY_A)){
                            textLayer->clean(12,10,8);
                            playState = PlayState::BET;
                        }
                        return;
                };
                break;
        }
    }else if(scrState == PlayScreenState::PAUSE){
        if(inputManager->keyWentDown(KEY_UP))
            pOpSelected = 0;
        if(inputManager->keyWentDown(KEY_DOWN))
            pOpSelected = 1;
        if(inputManager->keyWentDown(KEY_A)){
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
        if(inputManager->keyWentDown(KEY_A))
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
    if(amount > 0){
        value = std::min(value,std::min(coins,MAX_BET-bet));
        coins -= value;
        bet += value;
    }else if(amount <0){
        value = std::min(value,bet-minimumBet);
        coins += value;
        bet -= value;
    }
}

void PlayScreen::setMinimumBetAndLevel(){
    level = ((Math::logBase2(coins) + 1) / 2) +1;
    minimumBet = Math::pow(10,(Math::logBase2(coins)+1)/5);
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
    u32 numShuffleLayers = (level+3)/4,typeLayer;
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

    cups[c1]->moveTo(cups[c2]->getXPos(),cups[c2]->getYPos(),3,level);
    if(mode == 0){
        cups[c2]->moveTo(cups[c1]->getXPos(),cups[c1]->getYPos(),3,level);
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
        curSelectPos = (multiplier >2)? 0:1;
    if(curSelectPos >= 4)
        curSelectPos = (multiplier == 4)? 4:3;

    BF_SET(cursorSelectAttr->attr1,36+curSelectPos*36,ATTR1_XPOS);
    for(int i =0 ; i<5;i++){
        if(cups[i]->getXPos() == (u32)(30+curSelectPos*36)){
            cupSelected = i;
            return;
        }
    }
}
