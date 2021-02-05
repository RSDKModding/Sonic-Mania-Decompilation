#include "RetroEngine.hpp"

int objectCount;
ObjectInfo objectList[OBJECT_COUNT];
int globalObjectCount = 0;
int globalObjectIDs[OBJECT_COUNT];
int stageObjectCount = 0;
int stageObjectIDs[OBJECT_COUNT];

EntityBase objectEntityList[ENTITY_COUNT];

void CreateObject(Object *structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void),
                  void (*lateUpdate)(void), void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void),
                  void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void))
{
    if (objectCount < OBJECT_COUNT) {
        ObjectInfo *info = &objectList[objectCount];
        if (entitySize > sizeof(EntityBase))
            printf("Class exceeds max entity memory: %s \n", name);
        info->type = (Object*)structPtr;
        memset(hashBuffer, 0, 0x400);
        int nameLen = 0;
        while (name[nameLen]) ++nameLen;

        if (nameLen < 0x400) {
            memcpy(hashBuffer, name, nameLen);
            GenerateHash(info->hash, nameLen);
        }
        ++objectCount;
        info->entitySize   = entitySize;
        info->objectSize   = objectSize;
        info->update       = update;
        info->lateUpdate   = lateUpdate;
        info->staticUpdate = staticUpdate;
        info->draw         = draw;
        info->create       = create;
        info->stageLoad    = stageLoad;
        info->editorDraw   = editorDraw;
        info->editorLoad   = editorLoad;
        info->serialize    = serialize;
    }
}

void CreateObjectContainer(Object *structPtr, const char *name, uint objectSize)
{
    memset(hashBuffer, 0, 0x400);
    int nameLen = 0;
    while (name[nameLen]) ++nameLen;

    if (nameLen < 0x400) {
        memcpy(hashBuffer, name, nameLen);
        //GenerateHash(info->hash, nameLen);
    }
    ClearStruct(objectSize, structPtr, 0, true);
    //LoadStaticObject(structPtr, info->hash, 0);
}