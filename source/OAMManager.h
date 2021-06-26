#ifndef OAMMANAGER_H
#define OAMMANAGER_H

#include "MemoryDefines.h"

class OAMManager{
    private:
        static OAMManager* instance;

        OBJ_ATTR objBuffer[128];

    public:
        OAMManager();
        ~OAMManager();

        static OAMManager* getInstance();

        OBJ_ATTR* getAttrPtr(u32 index);

        void copyBuffer(u32 noObjects);
};


#endif // OAMMANAGER_H
