#include "OAMManager.h"

#include "ObjectAtributes.h"

OAMManager* OAMManager::instance = nullptr;

OAMManager::OAMManager(){
    ///Init objBuffer hiding each object
    u32* objBuf = (u32*)objBuffer;
    for(u32 i=0; i<256; i+=2){
        objBuf[i] = ATTR0_HIDE;
        objBuf[i+1] = 0;
    }

    copyBuffer(128);
}

OAMManager::~OAMManager(){}

OAMManager* OAMManager::getInstance(){
    if(instance == nullptr){
        instance = new OAMManager();
    }

    return instance;
}

OBJ_ATTR* OAMManager::getAttrPtr(u32 index){
    return &objBuffer[index];
}

void OAMManager::copyBuffer(u32 noObjects){
    /*for(u32 i = 0; i<noObjects; i++){
        OBJ_ATTR_MEMORY[i] = objBuffer[i];
    }*/
    u32* oamMemory = (u32*) OBJ_ATTR_MEMORY;
    u32* objBuf = (u32*) objBuffer;
    while(noObjects--){
        *oamMemory++ = *objBuf++;
        *oamMemory++ = *objBuf++;
    }
    ///NOTE: if this generates problems use word-copy instead
}
