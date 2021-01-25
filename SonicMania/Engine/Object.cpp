#include "RetroEngine.hpp"

int objectCount;
ObjectInfo objectList[OBJECT_COUNT];

EntityBase objectEntityList[ENTITY_COUNT];

void CreateObject(Object *structPtr, const char *name, unsigned int entitySize, unsigned int objectSize, void (*update)(void),
                  void (*lateUpdate)(void), void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void),
                  void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void))
{
    if (objectCount < 0x400u) {
        ObjectInfo *info = &objectList[objectCount];
        if (entitySize > 0x458)
            printf("Class exceeds max entity memory: %s \n", name);
        info->type = (Object*)structPtr;
        // memset(HashBuffer, 0, 0x400u);
        int nameLen = 0;
        while (name[nameLen]) ++nameLen;

        if (nameLen < 0x400) {
            // memcpy(HashBuffer, name, nameLen);
            // GenerateHash(info->hash, nameLen);
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