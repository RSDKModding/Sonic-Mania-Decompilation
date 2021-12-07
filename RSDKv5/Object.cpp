#include "RetroEngine.hpp"

int32 objectCount = 0;
ObjectInfo objectList[OBJECT_COUNT];
int32 globalObjectCount = 0;
int32 globalObjectIDs[OBJECT_COUNT];
int32 stageObjectIDs[OBJECT_COUNT];

EntityBase objectEntityList[ENTITY_COUNT];

EditableVarInfo *editableVarList;
int32 editableVarCount = 0;

TypeGroupList typeGroups[TYPEGROUP_COUNT];

bool32 validDraw = false;

ForeachStackInfo foreachStackList[FOREACH_STACK_COUNT];
ForeachStackInfo *foreachStackPtr = NULL;

#if RETRO_USE_MOD_LOADER
void RegisterObject(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                    void (*staticUpdate)(void), void (*draw)(void), void (*create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                    void (*editorLoad)(void), void (*serialize)(void))
{
    return RegisterObject_STD(structPtr, name, entitySize, objectSize, update, lateUpdate, staticUpdate, draw, create, stageLoad, editorDraw, editorLoad, serialize);
}
void RegisterObject_STD(Object **structPtr, const char *name, uint entitySize, uint objectSize, std::function<void(void)> update,
                        std::function<void(void)> lateUpdate, std::function<void(void)> staticUpdate, std::function<void(void)> draw,
                        std::function<void(void *)> create, std::function<void(void)> stageLoad, std::function<void(void)> editorDraw,
                        std::function<void(void)> editorLoad, std::function<void(void)> serialize)
#else
void RegisterObject(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                    void (*staticUpdate)(void), void (*draw)(void), void (*create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                    void (*editorLoad)(void), void (*serialize)(void))
#endif
{
    if (objectCount < OBJECT_COUNT) {
        ObjectInfo *info = &objectList[objectCount];
        if (entitySize > sizeof(EntityBase))
            printf("Class exceeds max entity memory: %s \n", name);
        GEN_HASH(name, info->hash);
        info->type         = structPtr;
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
        ++objectCount;
    }
}

#if RETRO_REV02
void RegisterObjectContainer(Object **structPtr, const char *name, uint objectSize)
{
    uint hash[4];
    GEN_HASH(name, hash);
    AllocateStorage(objectSize, (void **)structPtr, DATASET_STG, true);
    LoadStaticObject((byte *)*structPtr, hash, 0);
}
#endif

void LoadStaticObject(byte *obj, uint *hash, int dataPos)
{
    FileInfo info;

    char buffer[0x40];
    const char *hexChars = "0123456789ABCDEF";

    char hashBuf[0x21];
    sprintf(hashBuf, "%s", "00000000000000000000000000000000");

    int strPos = 0;
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[0] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[1] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[2] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[3] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }

    bool loaded = false;
    //This is done so Static/ can be loaded first, ensuring smoother development
#if !RETRO_USE_ORIGINAL_CODE
    sprintf(buffer, "Static/%s.bin", hashBuf);

    InitFileInfo(&info);
    info.externalFile = true;
    loaded            = LoadFile(&info, buffer, FMODE_RB);

    if (!loaded) {
#endif
        sprintf(buffer, "Data/Objects/Static/%s.bin", hashBuf);
        InitFileInfo(&info);
        loaded = LoadFile(&info, buffer, FMODE_RB);
        if (loaded)
            printf("what");
#if !RETRO_USE_ORIGINAL_CODE
    }
#endif

    if (loaded) {
        uint sig = ReadInt32(&info, false);

        if (sig != 0x4A424F) {
            CloseFile(&info);
            return;
        }

        while (info.readPos < info.fileSize) {
            int dataType  = ReadInt8(&info);
            int arraySize = ReadInt32(&info, false);

            if (dataType & 0x80) {
                uint dataSize = ReadInt32(&info, false);
                dataType &= 0x7F;

                switch (dataType) {
                    case SVAR_UINT8:
                    case SVAR_INT8:
                        if (info.readPos + dataSize <= info.fileSize && &obj[dataPos]) {
                            for (int i = 0; i < dataSize * sizeof(byte); i += sizeof(byte)) ReadBytes(&info, &obj[dataPos + i], sizeof(byte));
                        }
                        else {
                            for (int i = 0; i < dataSize * sizeof(byte); ++i) ReadInt8(&info);
                        }
                        dataPos += dataSize * sizeof(byte);
                        break;
                    case SVAR_UINT16:
                    case SVAR_INT16: {
                        int tmp = (dataPos & -(int)sizeof(short)) + sizeof(short);
                        if ((dataPos & -(int)sizeof(short)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;
                        if (info.readPos + (dataSize * sizeof(short)) <= info.fileSize && &obj[dataPos]) {
                            for (int i = 0; i < dataSize * sizeof(short); i += sizeof(short)) ReadBytes(&info, &obj[dataPos + i], sizeof(short));
                        }
                        else {
                            info.readPos += (dataSize * sizeof(short));
                        }
                        dataPos = tmp + sizeof(short) * dataSize;
                        break;
                    }
                    case SVAR_UINT32:
                    case SVAR_INT32:
                    case SVAR_BOOL: {
                        int tmp = (dataPos & -(int)sizeof(int)) + sizeof(int);
                        if ((dataPos & -(int)sizeof(int)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;
                        if (info.readPos + (dataSize * sizeof(int)) <= info.fileSize && &obj[dataPos]) {
                            for (int i = 0; i < dataSize * sizeof(int); i += sizeof(int)) ReadBytes(&info, &obj[dataPos + i], sizeof(int));
                        }
                        else {
                            info.readPos += (dataSize * sizeof(int));
                        }
                        dataPos = tmp + sizeof(int) * dataSize;
                        break;
                    }
                }
            }
            else {
                int tmp = 0;
                switch (dataType) {
                    case SVAR_UINT8:
                    case SVAR_INT8: dataPos += sizeof(byte) * arraySize; break;
                    case SVAR_UINT16:
                    case SVAR_INT16:
                        tmp = (dataPos & -(int)sizeof(short)) + sizeof(short);
                        if ((dataPos & -(int)sizeof(short)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(short) * arraySize;
                        break;
                    case SVAR_UINT32:
                    case SVAR_INT32:
                    case SVAR_BOOL:
                        tmp = (dataPos & -(int)sizeof(int)) + sizeof(int);
                        if ((dataPos & -(int)sizeof(int)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(int) * arraySize;
                        break;
                    case SVAR_PTR:
                        tmp = (dataPos & -(int)sizeof(void *)) + sizeof(void *);
                        if ((dataPos & -(int)sizeof(void *)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(void *) * arraySize; // 4/8
                        break;
                    case SVAR_VEC2:
                        tmp = (dataPos & -(int)sizeof(int)) + sizeof(int);
                        if ((dataPos & -(int)sizeof(int)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(Vector2) * arraySize; // 8
                        break;
                    case SVAR_TEXT:
                        tmp = (dataPos & -(int)sizeof(void *)) + sizeof(void *);
                        if ((dataPos & -(int)sizeof(void *)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(TextInfo) * arraySize; // 8/16
                        break;
                    case SVAR_ANIMATOR: // Animator
                        tmp = (dataPos & -(int)sizeof(void *)) + sizeof(void *);
                        if ((dataPos & -(int)sizeof(void *)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(Animator) * arraySize; // 24/32
                        break;
                    case SVAR_HITBOX: // Hitbox
                        tmp = (dataPos & -(int)sizeof(short)) + sizeof(short);
                        if ((dataPos & -(int)sizeof(short)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(Hitbox) * arraySize; // 8
                        break;
                    case SVAR_UNKNOWN: //???
                        tmp = (dataPos & -(int)sizeof(short)) + sizeof(short);
                        if ((dataPos & -(int)sizeof(short)) >= dataPos)
                            tmp = dataPos;
                        sizeof(Matrix);
                        sizeof(SpriteFrame);
                        dataPos = tmp + sizeof(UnknownStruct) * arraySize; // 18 (2 * 9)
                        break;
                    default: break;
                }
            }
        }
        CloseFile(&info);
    }
}

void SetActiveVariable(int objectID, const char *name) {}
void AddEnumVariable(const char *name) {}

void InitObjects()
{
    sceneInfo.entitySlot = 0;
    sceneInfo.createSlot = ENTITY_COUNT - 0x100;
    cameraCount          = 0;

    for (int o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif
        if (objectList[stageObjectIDs[o]].stageLoad)
            objectList[stageObjectIDs[o]].stageLoad();
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_STAGELOAD, NULL);
#endif

    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entitySlot = e;
        sceneInfo.entity     = &objectEntityList[e];
        if (sceneInfo.entity->objectID) {
            if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].create) {
                sceneInfo.entity->interaction = true;
                objectList[stageObjectIDs[sceneInfo.entity->objectID]].create(NULL);
            }
        }
    }

    sceneInfo.state = ENGINESTATE_REGULAR;
    if (!cameraCount) {
        AddCamera(&screens[0].position, screens[0].center.x << 0x10, screens[0].center.x << 0x10, false);
    }
}
void ProcessObjects()
{
    for (int i = 0; i < DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entityCount = 0;
    }

    for (int o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif
        ObjectInfo *objInfo = &objectList[stageObjectIDs[o]];
        if ((*objInfo->type)->active == ACTIVE_ALWAYS || (*objInfo->type)->active == ACTIVE_NORMAL) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONSTATICUPDATE, intToVoid(ENGINESTATE_REGULAR));
#endif

    for (int s = 0; s < cameraCount; ++s) {
        CameraInfo *camera = &cameras[s];
        if (camera->targetPos) {
            if (camera->worldRelative) {
                camera->position.x = camera->targetPos->x;
                camera->position.y = camera->targetPos->y;
            }
            else {
                camera->position.x = camera->targetPos->x << 0x10;
                camera->position.y = camera->targetPos->y << 0x10;
            }
        }
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->objectID) {
            switch (sceneInfo.entity->active) {
                case ACTIVE_NEVER:
                case ACTIVE_PAUSED: sceneInfo.entity->inBounds = false; break;
                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;
                case ACTIVE_BOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);
                        int sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);
                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x
                            && sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);
                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);
                        if (sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sx = abs(sceneInfo.entity->position.x - cameras[s].position.x) >> 0x10;
                        int sy = abs(sceneInfo.entity->position.y - cameras[s].position.y) >> 0x10;

                        if (sx * sx + sy * sy <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
            }

            if (sceneInfo.entity->inBounds) {
                if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].update) {
                    objectList[stageObjectIDs[sceneInfo.entity->objectID]].update();
                }

                if (sceneInfo.entity->drawOrder < DRAWLAYER_COUNT)
                    drawLayers[sceneInfo.entity->drawOrder].entries[drawLayers[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
            }
        }
        else {
            sceneInfo.entity->inBounds = false;
        }
        sceneInfo.entitySlot++;
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONUPDATE, intToVoid(ENGINESTATE_REGULAR));
#endif

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].rowCount = 0;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].rowCount++]                                   = e; // All active objects
            typeGroups[sceneInfo.entity->objectID].entries[typeGroups[sceneInfo.entity->objectID].rowCount++] = e; // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].rowCount++] = e; // extra slots
            }
        }
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds) {
            if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate) {
                objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate();
            }
        }
        sceneInfo.entity->activeScreens = 0;
        sceneInfo.entitySlot++;
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONLATEUPDATE, intToVoid(ENGINESTATE_REGULAR));
#endif
}
void ProcessPausedObjects()
{
    for (int i = 0; i < DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entityCount = 0;
    }

    for (int o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif
        ObjectInfo *objInfo = &objectList[stageObjectIDs[o]];
        if ((*objInfo->type)->active == ACTIVE_ALWAYS || (*objInfo->type)->active == ACTIVE_PAUSED) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONSTATICUPDATE, intToVoid(ENGINESTATE_PAUSED));
#endif

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->objectID) {
            if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
                if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].update) {
                    objectList[stageObjectIDs[sceneInfo.entity->objectID]].update();
                }

                if (sceneInfo.entity->drawOrder < DRAWLAYER_COUNT)
                    drawLayers[sceneInfo.entity->drawOrder].entries[drawLayers[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
            }
        }
        else {
            sceneInfo.entity->inBounds = false;
        }
        sceneInfo.entitySlot++;
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONSTATICUPDATE, intToVoid(ENGINESTATE_PAUSED));
#endif

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].rowCount = 0;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].rowCount++]                                   = e; // All active objects
            typeGroups[sceneInfo.entity->objectID].entries[typeGroups[sceneInfo.entity->objectID].rowCount++] = e; // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].rowCount++] = e; // extra slots
            }
        }
        sceneInfo.entitySlot++;
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONUPDATE, intToVoid(ENGINESTATE_PAUSED));
#endif

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
            if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate) {
                objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate();
            }
        }
        sceneInfo.entity->activeScreens = 0;
        sceneInfo.entitySlot++;
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONLATEUPDATE, intToVoid(ENGINESTATE_PAUSED));
#endif
}
void ProcessFrozenObjects()
{
    for (int i = 0; i < DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entityCount = 0;
    }

    for (int o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif
        ObjectInfo *objInfo = &objectList[stageObjectIDs[o]];
        if ((*objInfo->type)->active == ACTIVE_ALWAYS || (*objInfo->type)->active == ACTIVE_PAUSED) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONSTATICUPDATE, intToVoid(ENGINESTATE_FROZEN));
#endif

    for (int s = 0; s < cameraCount; ++s) {
        CameraInfo *camera = &cameras[s];
        if (camera->targetPos) {
            if (camera->worldRelative) {
                camera->position.x = camera->targetPos->x;
                camera->position.y = camera->targetPos->y;
            }
            else {
                camera->position.x = camera->targetPos->x << 0x10;
                camera->position.y = camera->targetPos->y << 0x10;
            }
        }
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->objectID) {
            switch (sceneInfo.entity->active) {
                case ACTIVE_NEVER:
                case ACTIVE_PAUSED: sceneInfo.entity->inBounds = false; break;
                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;
                case ACTIVE_BOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);
                        int sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);
                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x
                            && sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);
                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);
                        if (sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < cameraCount; ++s) {
                        int sx = abs(sceneInfo.entity->position.x - cameras[s].position.x) >> 0x10;
                        int sy = abs(sceneInfo.entity->position.y - cameras[s].position.y) >> 0x10;

                        if (sx * sx + sy * sy <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
            }

            if (sceneInfo.entity->inBounds) {
                if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
                    if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].update) {
                        objectList[stageObjectIDs[sceneInfo.entity->objectID]].update();
                    }
                }

                if (sceneInfo.entity->drawOrder < DRAWLAYER_COUNT)
                    drawLayers[sceneInfo.entity->drawOrder].entries[drawLayers[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
            }
        }
        else {
            sceneInfo.entity->inBounds = false;
        }
        sceneInfo.entitySlot++;
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONUPDATE, intToVoid(ENGINESTATE_FROZEN));
#endif

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].rowCount = 0;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].rowCount++]                                   = e; // All active objects
            typeGroups[sceneInfo.entity->objectID].entries[typeGroups[sceneInfo.entity->objectID].rowCount++] = e; // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].rowCount++] = e; // extra slots
            }
        }
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds) {
            if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
                if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate) {
                    objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate();
                }
            }
        }
        sceneInfo.entity->activeScreens = 0;
        sceneInfo.entitySlot++;
    }
#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONLATEUPDATE, intToVoid(ENGINESTATE_FROZEN));
#endif
}
void ProcessObjectDrawLists()
{
    if (sceneInfo.state && sceneInfo.state != (ENGINESTATE_LOAD | ENGINESTATE_STEPOVER)) {
        for (int s = 0; s < engine.screenCount; ++s) {
            currentScreen             = &screens[s];
            sceneInfo.currentScreenID = s;
            for (int l = 0; l < DRAWLAYER_COUNT; ++l) {
                drawLayers[l].layerCount = 0;
            }

            for (int t = 0; t < LAYER_COUNT; ++t) {
                byte drawOrder = tileLayers[t].drawLayer[s];
                if (drawOrder < DRAWLAYER_COUNT)
                    drawLayers[drawOrder].layerDrawList[drawLayers[drawOrder].layerCount++] = t;
            }

            sceneInfo.currentDrawGroup = 0;
            for (int l = 0; l < DRAWLAYER_COUNT; ++l) {
                if (engine.drawLayerVisible[l]) {

                    DrawList *list = &drawLayers[l];

                    if (list->callback)
                        list->callback();

                    if (list->sorted) {
                        for (int e = 0; e < list->entityCount; ++e) {
                            for (int e2 = list->entityCount - 1; e2 > e; --e2) {
                                int slotA = list->entries[e2 - 1];
                                int slotB = list->entries[e2];
                                if (objectEntityList[slotB].depth > objectEntityList[slotA].depth) {
                                    list->entries[e2 - 1] = slotB;
                                    list->entries[e2]     = slotA;
                                }
                            }
                        }
                    }

                    for (int i = 0; i < list->entityCount; ++i) {
                        sceneInfo.entitySlot = list->entries[i];
                        validDraw            = false;
                        sceneInfo.entity     = &objectEntityList[list->entries[i]];
                        if (sceneInfo.entity->visible) {
                            if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].draw) {
                                objectList[stageObjectIDs[sceneInfo.entity->objectID]].draw();
                            }

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                            if (i == list->entityCount - 1) {
                                DrawAchievements();
                            }
#endif

                            sceneInfo.entity->activeScreens |= validDraw << sceneInfo.currentScreenID;
                        }
                    }

                    for (int i = 0; i < list->layerCount; ++i) {
                        TileLayer *layer = &tileLayers[list->layerDrawList[i]];

                        if (layer->scanlineCallback)
                            layer->scanlineCallback(scanlines);
                        else
                            ProcessParallax(layer);
                        switch (layer->type) {
                            case LAYER_HSCROLL: DrawLayerHScroll(layer); break;
                            case LAYER_VSCROLL: DrawLayerVScroll(layer); break;
                            case LAYER_ROTOZOOM: DrawLayerRotozoom(layer); break;
                            case LAYER_BASIC: DrawLayerBasic(layer); break;
                            default: break;
                        }
                    }

#if RETRO_USE_MOD_LOADER
                    RunModCallbacks(MODCB_ONDRAW, intToVoid(l));
#endif

                    if (currentScreen->clipBound_X1 > 0)
                        currentScreen->clipBound_X1 = 0;

                    if (currentScreen->clipBound_Y1 > 0)
                        currentScreen->clipBound_Y1 = 0;

                    if (currentScreen->size.x >= 0) {
                        if (currentScreen->clipBound_X2 < currentScreen->size.x)
                            currentScreen->clipBound_X2 = currentScreen->size.x;
                    }
                    else {
                        currentScreen->clipBound_X2 = 0;
                    }

                    if (currentScreen->size.y >= 0) {
                        if (currentScreen->clipBound_Y2 < currentScreen->size.y)
                            currentScreen->clipBound_Y2 = currentScreen->size.y;
                    }
                    else {
                        currentScreen->clipBound_Y2 = 0;
                    }
                }

                sceneInfo.currentDrawGroup++;
            }

#if !RETRO_USE_ORIGINAL_CODE
            if (engine.showPaletteOverlay) {
                for (int p = 0; p < PALETTE_COUNT; ++p) {
                    int x = (pixWidth - (0x10 << 3));
                    int y = (SCREEN_YSIZE - (0x10 << 2));
                    for (int c = 0; c < PALETTE_SIZE; ++c) {
                        uint clr = GetPaletteEntry(p, c);

                        DrawRectangle(x + ((c & 0xF) << 1) + ((p % (PALETTE_COUNT / 2)) * (2 * 16)),
                                      y + ((c >> 4) << 1) + ((p / (PALETTE_COUNT / 2)) * (2 * 16)), 2, 2, clr, 0xFF, INK_NONE, true);
                    }
                }
            }

            if (showHitboxes) {
                for (int i = 0; i < debugHitboxCount; ++i) {
                    DebugHitboxInfo *info = &debugHitboxList[i];
                    int x                 = info->pos.x + (info->hitbox.left << 16);
                    int y                 = info->pos.y + (info->hitbox.top << 16);
                    int w                 = abs((info->pos.x + (info->hitbox.right << 16)) - x);
                    int h                 = abs((info->pos.y + (info->hitbox.bottom << 16)) - y);

                    switch (info->type) {
                        case H_TYPE_TOUCH: DrawRectangle(x, y, w, h, info->collision ? 0x808000 : 0xFF0000, 0x60, INK_ALPHA, false); break;
                        case H_TYPE_CIRCLE:
                            DrawCircle(info->pos.x, info->pos.y, info->hitbox.left, info->collision ? 0x808000 : 0xFF0000, 0x60, INK_ALPHA, false);
                            break;
                        case H_TYPE_BOX:
                            DrawRectangle(x, y, w, h, 0x0000FF, 0x60, INK_ALPHA, false);
                            if (info->collision & 1) //top
                                DrawRectangle(x, y, w, 1 << 16, 0xFFFF00, 0xC0, INK_ALPHA, false);
                            if (info->collision & 8) //bottom
                                DrawRectangle(x, y + h, w, 1 << 16, 0xFFFF00, 0xC0, INK_ALPHA, false);
                            if (info->collision & 2) { //left
                                int sy = y;
                                int sh = h;
                                if (info->collision & 1) {
                                    sy += 1 << 16;
                                    sh -= 1 << 16;
                                }
                                if (info->collision & 8)
                                    sh -= 1 << 16;
                                DrawRectangle(x, sy, 1 << 16, sh, 0xFFFF00, 0xC0, INK_ALPHA, false);
                            }
                            if (info->collision & 4) { //right
                                int sy = y;
                                int sh = h;
                                if (info->collision & 1) {
                                    sy += 1 << 16;
                                    sh -= 1 << 16;
                                }
                                if (info->collision & 8)
                                    sh -= 1 << 16;
                                DrawRectangle(x + w, sy, 1 << 16, sh, 0xFFFF00, 0xC0, INK_ALPHA, false);
                            }
                            break;
                        case H_TYPE_PLAT:
                            DrawRectangle(x, y, w, h, 0x00FF00, 0x60, INK_ALPHA, false);
                            if (info->collision & 1) // top
                                DrawRectangle(x, y, w, 1 << 16, 0xFFFF00, 0xC0, INK_ALPHA, false);
                            if (info->collision & 8) // bottom
                                DrawRectangle(x, y + h, w, 1 << 16, 0xFFFF00, 0xC0, INK_ALPHA, false);
                            break;
                    }
                }
            }
#endif

            currentScreen++;
            sceneInfo.currentScreenID++;
        }
    }
}

ushort GetObjectByName(const char *name)
{
    uint hash[4];
    GEN_HASH(name, hash);

    for (int o = 0; o < sceneInfo.classCount; ++o) {
        if (HASH_MATCH(hash, objectList[stageObjectIDs[o]].hash))
            return o;
    }
    return 0;
}

int GetEntityCount(ushort type, bool32 isActive)
{
    if (type >= TYPE_COUNT)
        return 0;
    if (isActive)
        return typeGroups[type].rowCount;

    int cnt = 0;
    for (int i = 0; i < ENTITY_COUNT; ++i) {
        if (objectEntityList[i].objectID == type)
            cnt++;
    }
    return cnt;
}

void ResetEntityPtr(Entity *entity, ushort type, void *data)
{
    if (entity) {
        ObjectInfo *info = &objectList[stageObjectIDs[type]];
        memset(entity, 0, info->entitySize);
        if (info->create) {
            Entity *curEnt                = sceneInfo.entity;
            sceneInfo.entity              = entity;
            sceneInfo.entity->interaction = true;
            info->create(data);
            sceneInfo.entity->objectID = type;
            sceneInfo.entity           = curEnt;
        }
        entity->objectID = type;
    }
}

void ResetEntitySlot(ushort slotID, ushort type, void *data)
{
    short slot          = ENTITY_COUNT - 1;
    ObjectInfo *objInfo = &objectList[stageObjectIDs[type]];
    if (slotID < ENTITY_COUNT)
        slot = slotID;
    Entity *entityPtr = &objectEntityList[slot];
    memset(&objectEntityList[slot], 0, objInfo->entitySize);
    if (objInfo->create) {
        Entity *curEnt         = sceneInfo.entity;
        sceneInfo.entity       = entityPtr;
        entityPtr->interaction = true;
        objInfo->create(data);
        sceneInfo.entity    = curEnt;
        entityPtr->objectID = type;
    }
    else {
        entityPtr->objectID = type;
    }
}

Entity *CreateEntity(ushort type, void *data, int x, int y)
{
    ObjectInfo *objInfo = &objectList[stageObjectIDs[type]];
    Entity *entityPtr   = &objectEntityList[sceneInfo.createSlot];

    int permCnt = 0, loopCnt = 0;
    while (entityPtr->objectID) {
        // after 16 loops, the game says fuck it and will start overwriting non-temp objects
        if (!entityPtr->isPermanent && loopCnt >= 16)
            break;
        if (entityPtr->isPermanent)
            ++permCnt;
        sceneInfo.createSlot++;
        if (sceneInfo.createSlot == ENTITY_COUNT) {
            sceneInfo.createSlot = TEMPENTITY_START;
            entityPtr            = &objectEntityList[sceneInfo.createSlot];
        }
        else {
            entityPtr = &objectEntityList[sceneInfo.createSlot];
        }
        if (permCnt >= TEMPENTITY_COUNT)
            break;
        ++loopCnt;
    }

    memset(entityPtr, 0, objInfo->entitySize);
    entityPtr->position.x  = x;
    entityPtr->position.y  = y;
    entityPtr->interaction = true;

    if (objInfo->create) {
        Entity *curEnt   = sceneInfo.entity;
        sceneInfo.entity = entityPtr;
        objInfo->create(data);
        sceneInfo.entity    = curEnt;
        entityPtr->objectID = type;
    }
    else {
        entityPtr->objectID = type;
        entityPtr->active   = ACTIVE_NORMAL;
        entityPtr->visible  = true;
    }
    return entityPtr;
}

bool32 GetActiveEntities(ushort group, Entity **entity)
{
    if (group >= TYPEGROUP_COUNT)
        return false;
    if (!entity)
        return false;

    if (*entity) {
        ++foreachStackPtr->id;
    }
    else {
        foreachStackPtr++;
        foreachStackPtr->id = 0;
    }

    for (Entity *nextEnt = &objectEntityList[typeGroups[group].entries[foreachStackPtr->id]]; foreachStackPtr->id < typeGroups[group].rowCount;
         ++foreachStackPtr->id, nextEnt = &objectEntityList[typeGroups[group].entries[foreachStackPtr->id]]) {
        if (nextEnt->objectID == group) {
            *entity = nextEnt;
            return true;
        }
    }
    foreachStackPtr--;
    return false;
}
bool32 GetEntities(ushort type, Entity **entity)
{
    if (type >= OBJECT_COUNT)
        return false;
    if (!entity)
        return false;

    if (*entity) {
        ++foreachStackPtr->id;
    }
    else {
        foreachStackPtr++;
        foreachStackPtr->id = 0;
    }

    for (Entity *nextEnt = &objectEntityList[foreachStackPtr->id]; foreachStackPtr->id < ENTITY_COUNT;
         ++foreachStackPtr->id, nextEnt = &objectEntityList[foreachStackPtr->id]) {
        if (nextEnt->objectID == type) {
            *entity = nextEnt;
            return true;
        }
    }
    foreachStackPtr--;
    return false;
}

bool32 CheckOnScreen(Entity *entity, Vector2 *range)
{
    if (!entity)
        return false;

    if (range) {
        for (int s = 0; s < cameraCount; ++s) {
            int sx = abs(entity->position.x - cameras[s].position.x);
            int sy = abs(entity->position.y - cameras[s].position.y);
            if (sx <= range->x + cameras[s].offset.x && sy <= range->y + cameras[s].offset.y) {
                return true;
            }
        }
    }
    else {
        for (int s = 0; s < cameraCount; ++s) {
            int sx = abs(entity->position.x - cameras[s].position.x);
            int sy = abs(entity->position.y - cameras[s].position.y);
            if (sx <= entity->updateRange.x + cameras[s].offset.x && sy <= entity->updateRange.y + cameras[s].offset.y) {
                return true;
            }
        }
    }
    return false;
}
bool32 CheckPosOnScreen(Vector2 *position, Vector2 *range)
{
    if (!position || !range)
        return false;

    for (int s = 0; s < cameraCount; ++s) {
        int sx = abs(position->x - cameras[s].position.x);
        int sy = abs(position->y - cameras[s].position.y);
        if (sx <= range->x + cameras[s].offset.x && sy <= range->y + cameras[s].offset.y) {
            return true;
        }
    }

    return false;
}
