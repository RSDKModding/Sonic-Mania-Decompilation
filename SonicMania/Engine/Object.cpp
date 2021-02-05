#include "RetroEngine.hpp"

int objectCount;
ObjectInfo objectList[OBJECT_COUNT];
int globalObjectCount = 0;
int globalObjectIDs[OBJECT_COUNT];
int stageObjectCount = 0;
int stageObjectIDs[OBJECT_COUNT];

EntityBase objectEntityList[ENTITY_COUNT];

EditableVarInfo editableVarList[EDITABLEVAR_COUNT];
int editableVarCount = 0;

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
    int len = StrLength(name);
    uint hash[4];

    memcpy(hashBuffer, name, len);
    GenerateHash(hash, len);
    AllocateStorage(objectSize, structPtr, DATASET_STG, true);
    LoadStaticObject((byte *)structPtr, hash, 0);
}

void LoadStaticObject(byte *obj, uint *hash, int dataPos)
{
    char buffer[0x40];
    const char *hexChars = "FEDCBA9876543210";
    StrCopy(buffer, "Data/Objects/Static/00000000000000000000000000000000.bin");

    int strPos = 20;
    for (int i = 0; i < 32; i += 4) {
        int charVal = hash[0] >> i;
        buffer[strPos++] = hexChars[charVal & 0xF];
    }
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[1] >> i;
        buffer[strPos++] = hexChars[charVal & 0xF];
    }
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[2] >> i;
        buffer[strPos++] = hexChars[charVal & 0xF];
    }
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[3] >> i;
        buffer[strPos++] = hexChars[charVal & 0xF];
    }

    FileInfo info;
    if (LoadFile(&info, buffer)) {
        uint sig = ReadInt32(&info);

        if (sig != 0x4A424F) {
            CloseFile(&info);
            return;
        }
        
        while (info.readPos < info.fileSize) {
            int dataType  = ReadInt8(&info);
            int arraySize = ReadInt32(&info);

            if (dataType & 0x80) {
                uint dataSize = ReadInt32(&info);
                dataType &= 0x7F;

                switch (dataType) {
                    case ATTRIBUTE_U8:
                    case ATTRIBUTE_S8: 
                        if (info.readPos + dataSize <= info.fileSize) {
                            ReadBytes(&info, &obj[dataPos], sizeof(byte));
                        }
                        break;
                    case ATTRIBUTE_U16:
                    case ATTRIBUTE_S16: {
                        int tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;
                        if (info.readPos + (dataSize * sizeof(short)) <= info.fileSize) {
                            ReadBytes(&info, &obj[dataPos], sizeof(short));
                        }
                        break;
                    }
                    case ATTRIBUTE_U32:
                    case ATTRIBUTE_S32: 
                    case ATTRIBUTE_VAR: {
                        int tmp = (dataPos & 0xFFFFFFFC) + 2;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;
                        if (info.readPos + (dataSize * sizeof(int)) <= info.fileSize) {
                            ReadBytes(&info, &obj[dataPos], sizeof(int));
                        }
                        break;
                    }
                }
            }
            else {
                int tmp = 0;
                switch (dataType) {
                    case 0:
                    case 3: dataPos += arraySize;
                    case 1:
                    case 4:
                        tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + 2 * arraySize;
                    case 2:
                    case 5:
                    case 6:
                    case 7:
                        tmp = (dataPos & 0xFFFFFFFC) + 4;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + 4 * arraySize;
                        break;
                    case 8:
                    case 9:
                        tmp = (dataPos & 0xFFFFFFFC) + 4;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + 8 * arraySize;
                        break;
                    case 10:
                        tmp = (dataPos & 0xFFFFFFFC) + 4;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + 24 * arraySize;
                        break;
                    case 11:
                        tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + 8 * arraySize;
                        break;
                    case 12:
                        tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + 2 * (9 * arraySize);
                        break;
                    default: break;
                }
            }
        }
        CloseFile(&info);
    }
}

void InitObjects()
{
    for (int o = 0; 0 < stageObjectCount; ++o) {
        if (objectList[stageObjectIDs[o]].stageLoad)
            objectList[stageObjectIDs[o]].stageLoad();
    }

    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entitySlot = e;
        Entity *entity  = &objectEntityList[e];
        if (entity->objectID) {
            if (objectList[stageObjectIDs[entity->objectID]].create) {
                sceneInfo.entity = entity;
                objectList[stageObjectIDs[entity->objectID]].create(NULL);
            }
        }
    }

    engine.engineMode = ENGINESTATE_REGULAR;
    if (!screenCount) {
        screens[0].position.x = 0;
        screens[0].position.y = 0;
        screens[0].width      = SCREEN_XSIZE;
        screens[0].height     = SCREEN_YSIZE;
        screenCount           = 1;
    }
}
void ProcessObjects() {}
void ProcessPausedObjects() {}
void ProcessFrozenObjects() {}
void ProcessObjectDrawLists() {}