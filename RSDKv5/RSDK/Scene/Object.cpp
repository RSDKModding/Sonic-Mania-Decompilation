#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

ObjectClass RSDK::objectClassList[OBJECT_COUNT];
int32 RSDK::objectClassCount = 0;

int32 RSDK::globalObjectCount = 0;
int32 RSDK::globalObjectIDs[OBJECT_COUNT];
int32 RSDK::stageObjectIDs[OBJECT_COUNT];

EntityBase RSDK::objectEntityList[ENTITY_COUNT];

EditableVarInfo *RSDK::editableVarList;
int32 RSDK::editableVarCount = 0;

TypeGroupList RSDK::typeGroups[TYPEGROUP_COUNT];

bool32 RSDK::validDraw = false;

ForeachStackInfo RSDK::foreachStackList[FOREACH_STACK_COUNT];
ForeachStackInfo *RSDK::foreachStackPtr = NULL;

#if RETRO_USE_MOD_LOADER
void RSDK::RegisterObject(Object **staticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize, void (*update)(void),
                          void (*lateUpdate)(void), void (*staticUpdate)(void), void (*draw)(void), void (*create)(void *), void (*stageLoad)(void),
                          void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void))
{
    return RegisterObject_STD(staticVars, name, entityClassSize, staticClassSize, update, lateUpdate, staticUpdate, draw, create, stageLoad,
                              editorDraw, editorLoad, serialize);
}

void RSDK::RegisterObject_STD(Object **staticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize, std::function<void(void)> update,
                              std::function<void(void)> lateUpdate, std::function<void(void)> staticUpdate, std::function<void(void)> draw,
                              std::function<void(void *)> create, std::function<void(void)> stageLoad, std::function<void(void)> editorDraw,
                              std::function<void(void)> editorLoad, std::function<void(void)> serialize)
#else
void RSDK::RegisterObject(Object **staticVars, const char *name, uint32 entityClassSize, uint32 staticClassSize, void (*update)(void),
                          void (*lateUpdate)(void), void (*staticUpdate)(void), void (*draw)(void), void (*create)(void *), void (*stageLoad)(void),
                          void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void))
#endif
{
    if (objectClassCount < OBJECT_COUNT) {
        if (entityClassSize > sizeof(EntityBase))
            printf("Class exceeds max entity memory: %s \n", name);

        ObjectClass *info = &objectClassList[objectClassCount];
        GEN_HASH_MD5(name, info->hash);
        info->staticVars      = staticVars;
        info->entityClassSize = entityClassSize;
        info->staticClassSize = staticClassSize;
        info->update          = update;
        info->lateUpdate      = lateUpdate;
        info->staticUpdate    = staticUpdate;
        info->draw            = draw;
        info->create          = create;
        info->stageLoad       = stageLoad;
        info->editorDraw      = editorDraw;
        info->editorLoad      = editorLoad;
        info->serialize       = serialize;

        ++objectClassCount;
    }
}

#if RETRO_REV02
void RSDK::RegisterStaticVariables(void **staticVars, const char *name, uint32 classSize)
{
    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(name, hash);
    AllocateStorage(classSize, (void **)staticVars, DATASET_STG, true);
    LoadStaticVariables((uint8 *)*staticVars, hash, 0);
}
#endif

void RSDK::LoadStaticVariables(uint8 *classPtr, uint32 *hash, int32 readOffset)
{
    FileInfo info;

    char buffer[0x40];
    const char *hexChars = "0123456789ABCDEF";

    char hashBuf[0x21];
    sprintf(hashBuf, "%s", "00000000000000000000000000000000");

    int32 strPos = 0;
    for (int32 i = 0; i < 32; i += 4) {
        int32 charVal     = hash[0] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }

    for (int32 i = 0; i < 32; i += 4) {
        int32 charVal     = hash[1] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }

    for (int32 i = 0; i < 32; i += 4) {
        int32 charVal     = hash[2] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }

    for (int32 i = 0; i < 32; i += 4) {
        int32 charVal     = hash[3] >> i;
        hashBuf[strPos++] = hexChars[charVal & 0xF];
    }

    bool loaded = false;

    // TODO: remove
    // This is done so Static/ can be loaded first, ensuring smoother development
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
#if !RETRO_USE_ORIGINAL_CODE
    }
#endif

    if (loaded) {
        uint32 sig = ReadInt32(&info, false);

        if (sig != RSDK_SIGNATURE_OBJ) {
            CloseFile(&info);
            return;
        }

        int32 dataPos = readOffset;
        while (info.readPos < info.fileSize) {
            int32 dataType  = ReadInt8(&info);
            int32 arraySize = ReadInt32(&info, false);

            // bit 7 == "hasValues"
            if (dataType & 0x80) {
                uint32 dataSize = ReadInt32(&info, false);
                dataType &= 0x7F;

                switch (dataType) {
                    case SVAR_UINT8:
                    case SVAR_INT8:
                        if (info.readPos + dataSize <= info.fileSize && &classPtr[dataPos]) {
                            for (int32 i = 0; i < dataSize * sizeof(uint8); i += sizeof(uint8))
                                ReadBytes(&info, &classPtr[dataPos + i], sizeof(uint8));
                        }
                        else {
                            for (int32 i = 0; i < dataSize * sizeof(uint8); ++i) ReadInt8(&info);
                        }

                        dataPos += dataSize * sizeof(uint8);
                        break;

                    case SVAR_UINT16:
                    case SVAR_INT16: {
                        int32 tmp = (dataPos & -(int32)sizeof(int16)) + sizeof(int16);
                        if ((dataPos & -(int32)sizeof(int16)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;

                        if (info.readPos + (dataSize * sizeof(int16)) <= info.fileSize && &classPtr[dataPos]) {
                            for (int32 i = 0; i < dataSize * sizeof(int16); i += sizeof(int16))
                                ReadBytes(&info, &classPtr[dataPos + i], sizeof(int16));
                        }
                        else {
                            info.readPos += (dataSize * sizeof(int16));
                        }

                        dataPos = tmp + sizeof(int16) * dataSize;
                        break;
                    }

                    case SVAR_UINT32:
                    case SVAR_INT32: {
                        int32 tmp = (dataPos & -(int32)sizeof(int32)) + sizeof(int32);
                        if ((dataPos & -(int32)sizeof(int32)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;

                        if (info.readPos + (dataSize * sizeof(int32)) <= info.fileSize && &classPtr[dataPos]) {
                            for (int32 i = 0; i < dataSize * sizeof(int32); i += sizeof(int32))
                                ReadBytes(&info, &classPtr[dataPos + i], sizeof(int32));
                        }
                        else {
                            info.readPos += (dataSize * sizeof(int32));
                        }

                        dataPos = tmp + sizeof(int32) * dataSize;
                        break;
                    }

                    case SVAR_BOOL: {
                        int32 tmp = (dataPos & -(int32)sizeof(bool32)) + sizeof(int32);
                        if ((dataPos & -(int32)sizeof(bool32)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;

                        if (info.readPos + (dataSize * sizeof(bool32)) <= info.fileSize && &classPtr[dataPos]) {
                            for (int32 i = 0; i < dataSize * sizeof(bool32); i += sizeof(bool32))
                                ReadBytes(&info, &classPtr[dataPos + i], sizeof(int32));
                        }
                        else {
                            info.readPos += (dataSize * sizeof(bool32));
                        }

                        dataPos = tmp + sizeof(bool32) * dataSize;
                        break;
                    }
                }
            }
            else {
                int32 tmp = 0;
                switch (dataType) {
                    case SVAR_UINT8:
                    case SVAR_INT8: dataPos += sizeof(uint8) * arraySize; break;

                    case SVAR_UINT16:
                    case SVAR_INT16:
                        tmp = (dataPos & -(int32)sizeof(int16)) + sizeof(int16);
                        if ((dataPos & -(int32)sizeof(int16)) >= dataPos)
                            tmp = dataPos;

                        dataPos = tmp + sizeof(int16) * arraySize;
                        break;

                    case SVAR_UINT32:
                    case SVAR_INT32:
                        tmp = (dataPos & -(int32)sizeof(int32)) + sizeof(int32);
                        if ((dataPos & -(int32)sizeof(int32)) >= dataPos)
                            tmp = dataPos;

                        dataPos = tmp + sizeof(int32) * arraySize;
                        break;

                    case SVAR_BOOL:
                        tmp = (dataPos & -(int32)sizeof(bool32)) + sizeof(bool32);
                        if ((dataPos & -(int32)sizeof(bool32)) >= dataPos)
                            tmp = dataPos;

                        dataPos = tmp + sizeof(bool32) * arraySize;
                        break;

                    case SVAR_POINTER:
                        tmp = (dataPos & -(int32)sizeof(void *)) + sizeof(void *);
                        if ((dataPos & -(int32)sizeof(void *)) >= dataPos)
                            tmp = dataPos;

                        dataPos = tmp + sizeof(void *) * arraySize; // 4/8
                        break;

                    case SVAR_VECTOR2:
                        tmp = (dataPos & -(int32)sizeof(int32)) + sizeof(int32);
                        if ((dataPos & -(int32)sizeof(int32)) >= dataPos)
                            tmp = dataPos;

                        dataPos = tmp + sizeof(Vector2) * arraySize; // 8
                        break;

                    case SVAR_STRING:
                        tmp = (dataPos & -(int32)sizeof(void *)) + sizeof(void *);
                        if ((dataPos & -(int32)sizeof(void *)) >= dataPos)
                            tmp = dataPos;

                        dataPos = tmp + sizeof(String) * arraySize; // 8/16
                        break;

                    case SVAR_ANIMATOR:
                        tmp = (dataPos & -(int32)sizeof(void *)) + sizeof(void *);
                        if ((dataPos & -(int32)sizeof(void *)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(Animator) * arraySize; // 24/32
                        break;

                    case SVAR_HITBOX:
                        tmp = (dataPos & -(int32)sizeof(int16)) + sizeof(int16);
                        if ((dataPos & -(int32)sizeof(int16)) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(Hitbox) * arraySize; // 8
                        break;

                    case SVAR_UNKNOWN: // ???
                        tmp = (dataPos & -(int32)sizeof(int16)) + sizeof(int16);
                        if ((dataPos & -(int32)sizeof(int16)) >= dataPos)
                            tmp = dataPos;

                        dataPos = tmp + sizeof(UnknownStruct) * arraySize; // 18 (0x12) (2 * 9)
                        break;

                    default: break;
                }
            }
        }

        CloseFile(&info);
    }
}

void RSDK::InitObjects()
{
    sceneInfo.entitySlot = 0;
    sceneInfo.createSlot = ENTITY_COUNT - 0x100;
    cameraCount          = 0;

    for (int32 o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif

        if (objectClassList[stageObjectIDs[o]].stageLoad)
            objectClassList[stageObjectIDs[o]].stageLoad();
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_STAGELOAD, NULL);
#endif

    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entitySlot = e;
        sceneInfo.entity     = &objectEntityList[e];

        if (sceneInfo.entity->classID) {
            if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].create) {
                sceneInfo.entity->interaction = true;
                objectClassList[stageObjectIDs[sceneInfo.entity->classID]].create(NULL);
            }
        }
    }

    sceneInfo.state = ENGINESTATE_REGULAR;

    if (!cameraCount)
        AddCamera(&screens[0].position, screens[0].center.x << 0x10, screens[0].center.x << 0x10, false);
}
void RSDK::ProcessObjects()
{
    for (int32 i = 0; i < DRAWGROUP_COUNT; ++i) drawGroups[i].entityCount = 0;

    for (int32 o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif

        ObjectClass *objInfo = &objectClassList[stageObjectIDs[o]];
        if ((*objInfo->staticVars)->active == ACTIVE_ALWAYS || (*objInfo->staticVars)->active == ACTIVE_NORMAL) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONSTATICUPDATE, intToVoid(ENGINESTATE_REGULAR));
#endif

    for (int32 s = 0; s < cameraCount; ++s) {
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
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->classID) {
            switch (sceneInfo.entity->active) {
                default: break;

                case ACTIVE_NEVER:
                case ACTIVE_PAUSED: sceneInfo.entity->inBounds = false; break;

                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;

                case ACTIVE_BOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);
                        int32 sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);

                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x
                            && sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;

                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);

                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;

                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);

                        if (sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;

                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sx = abs(sceneInfo.entity->position.x - cameras[s].position.x) >> 0x10;
                        int32 sy = abs(sceneInfo.entity->position.y - cameras[s].position.y) >> 0x10;

                        if (sx * sx + sy * sy <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
            }

            if (sceneInfo.entity->inBounds) {
                if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].update)
                    objectClassList[stageObjectIDs[sceneInfo.entity->classID]].update();

                if (sceneInfo.entity->drawOrder < DRAWGROUP_COUNT)
                    drawGroups[sceneInfo.entity->drawOrder].entries[drawGroups[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
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

    for (int32 i = 0; i < TYPEGROUP_COUNT; ++i) typeGroups[i].entryCount = 0;

    sceneInfo.entitySlot = 0;
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];

        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++] = e; // All active objects

            typeGroups[sceneInfo.entity->classID].entries[typeGroups[sceneInfo.entity->classID].entryCount++] = e; // class-based groups

            if (sceneInfo.entity->group >= TYPE_COUNT)
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; // extra groups
        }

        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];

        if (sceneInfo.entity->inBounds) {
            if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].lateUpdate)
                objectClassList[stageObjectIDs[sceneInfo.entity->classID]].lateUpdate();
        }

        sceneInfo.entity->activeScreens = 0;
        sceneInfo.entitySlot++;
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONLATEUPDATE, intToVoid(ENGINESTATE_REGULAR));
#endif
}
void RSDK::ProcessPausedObjects()
{
    for (int32 i = 0; i < DRAWGROUP_COUNT; ++i) drawGroups[i].entityCount = 0;

    for (int32 o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif

        ObjectClass *object = &objectClassList[stageObjectIDs[o]];
        if ((*object->staticVars)->active == ACTIVE_ALWAYS || (*object->staticVars)->active == ACTIVE_PAUSED) {
            if (object->staticUpdate)
                object->staticUpdate();
        }
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONSTATICUPDATE, intToVoid(ENGINESTATE_PAUSED));
#endif

    sceneInfo.entitySlot = 0;
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];

        if (sceneInfo.entity->classID) {
            if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
                if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].update)
                    objectClassList[stageObjectIDs[sceneInfo.entity->classID]].update();

                if (sceneInfo.entity->drawOrder < DRAWGROUP_COUNT)
                    drawGroups[sceneInfo.entity->drawOrder].entries[drawGroups[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
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

    for (int32 i = 0; i < TYPEGROUP_COUNT; ++i) typeGroups[i].entryCount = 0;

    sceneInfo.entitySlot = 0;
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];

        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++] = e; // All active entities

            typeGroups[sceneInfo.entity->classID].entries[typeGroups[sceneInfo.entity->classID].entryCount++] = e; // type-based groups

            if (sceneInfo.entity->group >= TYPE_COUNT)
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; // extra groups
        }

        sceneInfo.entitySlot++;
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONUPDATE, intToVoid(ENGINESTATE_PAUSED));
#endif

    sceneInfo.entitySlot = 0;
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];

        if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
            if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].lateUpdate)
                objectClassList[stageObjectIDs[sceneInfo.entity->classID]].lateUpdate();
        }

        sceneInfo.entity->activeScreens = 0;
        sceneInfo.entitySlot++;
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONLATEUPDATE, intToVoid(ENGINESTATE_PAUSED));
#endif
}
void RSDK::ProcessFrozenObjects()
{
    for (int32 i = 0; i < DRAWGROUP_COUNT; ++i) drawGroups[i].entityCount = 0;

    for (int32 o = 0; o < sceneInfo.classCount; ++o) {
#if RETRO_USE_MOD_LOADER
        currentObjectID = o;
#endif

        ObjectClass *object = &objectClassList[stageObjectIDs[o]];
        if ((*object->staticVars)->active == ACTIVE_ALWAYS || (*object->staticVars)->active == ACTIVE_PAUSED) {
            if (object->staticUpdate)
                object->staticUpdate();
        }
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONSTATICUPDATE, intToVoid(ENGINESTATE_FROZEN));
#endif

    for (int32 s = 0; s < cameraCount; ++s) {
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
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];

        if (sceneInfo.entity->classID) {
            switch (sceneInfo.entity->active) {
                default: break;

                case ACTIVE_NEVER:
                case ACTIVE_PAUSED: sceneInfo.entity->inBounds = false; break;

                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;

                case ACTIVE_BOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);
                        int32 sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);

                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x
                            && sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;

                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sx = abs(sceneInfo.entity->position.x - cameras[s].position.x);

                        if (sx <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;

                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sy = abs(sceneInfo.entity->position.y - cameras[s].position.y);

                        if (sy <= sceneInfo.entity->updateRange.y + cameras[s].offset.y) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;

                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;

                    for (int32 s = 0; s < cameraCount; ++s) {
                        int32 sx = abs(sceneInfo.entity->position.x - cameras[s].position.x) >> 0x10;
                        int32 sy = abs(sceneInfo.entity->position.y - cameras[s].position.y) >> 0x10;

                        if (sx * sx + sy * sy <= sceneInfo.entity->updateRange.x + cameras[s].offset.x) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
            }

            if (sceneInfo.entity->inBounds) {
                if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
                    if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].update)
                        objectClassList[stageObjectIDs[sceneInfo.entity->classID]].update();
                }

                if (sceneInfo.entity->drawOrder < DRAWGROUP_COUNT)
                    drawGroups[sceneInfo.entity->drawOrder].entries[drawGroups[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
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

    for (int32 i = 0; i < TYPEGROUP_COUNT; ++i) typeGroups[i].entryCount = 0;

    sceneInfo.entitySlot = 0;
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++] = e; // All active entities

            typeGroups[sceneInfo.entity->classID].entries[typeGroups[sceneInfo.entity->classID].entryCount++] = e; // type-based groups

            if (sceneInfo.entity->group >= TYPE_COUNT)
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; // extra groups
        }
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    for (int32 e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];

        if (sceneInfo.entity->inBounds) {
            if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
                if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].lateUpdate)
                    objectClassList[stageObjectIDs[sceneInfo.entity->classID]].lateUpdate();
            }
        }

        sceneInfo.entity->activeScreens = 0;
        sceneInfo.entitySlot++;
    }

#if RETRO_USE_MOD_LOADER
    RunModCallbacks(MODCB_ONLATEUPDATE, intToVoid(ENGINESTATE_FROZEN));
#endif
}
void RSDK::ProcessObjectDrawLists()
{
    if (sceneInfo.state && sceneInfo.state != (ENGINESTATE_LOAD | ENGINESTATE_STEPOVER)) {
        for (int32 s = 0; s < videoSettings.screenCount; ++s) {
            currentScreen             = &screens[s];
            sceneInfo.currentScreenID = s;

            for (int32 l = 0; l < DRAWGROUP_COUNT; ++l) drawGroups[l].layerCount = 0;

            for (int32 t = 0; t < LAYER_COUNT; ++t) {
                uint8 drawOrder = tileLayers[t].drawLayer[s];

                if (drawOrder < DRAWGROUP_COUNT)
                    drawGroups[drawOrder].layerDrawList[drawGroups[drawOrder].layerCount++] = t;
            }

            sceneInfo.currentDrawGroup = 0;
            for (int32 l = 0; l < DRAWGROUP_COUNT; ++l) {
                if (engine.drawLayerVisible[l]) {

                    DrawList *list = &drawGroups[l];

                    if (list->callback)
                        list->callback();

                    if (list->sorted) {
                        for (int32 e = 0; e < list->entityCount; ++e) {
                            for (int32 i = list->entityCount - 1; i > e; --i) {
                                int32 slot1 = list->entries[i - 1];
                                int32 slot2 = list->entries[i];
                                if (objectEntityList[slot2].depth > objectEntityList[slot1].depth) {
                                    list->entries[i - 1] = slot2;
                                    list->entries[i]     = slot1;
                                }
                            }
                        }
                    }

                    for (int32 i = 0; i < list->entityCount; ++i) {
                        sceneInfo.entitySlot = list->entries[i];
                        validDraw            = false;
                        sceneInfo.entity     = &objectEntityList[list->entries[i]];

                        if (sceneInfo.entity->visible) {
                            if (objectClassList[stageObjectIDs[sceneInfo.entity->classID]].draw)
                                objectClassList[stageObjectIDs[sceneInfo.entity->classID]].draw();

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
                            if (i == list->entityCount - 1)
                                SKU::DrawAchievements();
#endif

                            sceneInfo.entity->activeScreens |= validDraw << sceneInfo.currentScreenID;
                        }
                    }

                    for (int32 i = 0; i < list->layerCount; ++i) {
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
            if (showHitboxes) {
                for (int32 i = 0; i < debugHitboxCount; ++i) {
                    DebugHitboxInfo *info = &debugHitboxList[i];
                    int32 x               = info->pos.x + (info->hitbox.left << 16);
                    int32 y               = info->pos.y + (info->hitbox.top << 16);
                    int32 w               = abs((info->pos.x + (info->hitbox.right << 16)) - x);
                    int32 h               = abs((info->pos.y + (info->hitbox.bottom << 16)) - y);

                    switch (info->type) {
                        case H_TYPE_TOUCH: DrawRectangle(x, y, w, h, info->collision ? 0x808000 : 0xFF0000, 0x60, INK_ALPHA, false); break;

                        case H_TYPE_CIRCLE:
                            DrawCircle(info->pos.x, info->pos.y, info->hitbox.left, info->collision ? 0x808000 : 0xFF0000, 0x60, INK_ALPHA, false);
                            break;

                        case H_TYPE_BOX:
                            DrawRectangle(x, y, w, h, 0x0000FF, 0x60, INK_ALPHA, false);

                            if (info->collision & 1) // top
                                DrawRectangle(x, y, w, 1 << 16, 0xFFFF00, 0xC0, INK_ALPHA, false);

                            if (info->collision & 8) // bottom
                                DrawRectangle(x, y + h, w, 1 << 16, 0xFFFF00, 0xC0, INK_ALPHA, false);

                            if (info->collision & 2) { // left
                                int32 sy = y;
                                int32 sh = h;

                                if (info->collision & 1) {
                                    sy += 1 << 16;
                                    sh -= 1 << 16;
                                }

                                if (info->collision & 8)
                                    sh -= 1 << 16;

                                DrawRectangle(x, sy, 1 << 16, sh, 0xFFFF00, 0xC0, INK_ALPHA, false);
                            }

                            if (info->collision & 4) { // right
                                int32 sy = y;
                                int32 sh = h;

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

            if (engine.showPaletteOverlay) {
                for (int32 p = 0; p < PALETTE_BANK_COUNT; ++p) {
                    int32 x = (videoSettings.pixWidth - (0x10 << 3));
                    int32 y = (SCREEN_YSIZE - (0x10 << 2));

                    for (int32 c = 0; c < PALETTE_BANK_SIZE; ++c) {
                        uint32 clr = GetPaletteEntry(p, c);

                        DrawRectangle(x + ((c & 0xF) << 1) + ((p % (PALETTE_BANK_COUNT / 2)) * (2 * 16)),
                                      y + ((c >> 4) << 1) + ((p / (PALETTE_BANK_COUNT / 2)) * (2 * 16)), 2, 2, clr, 0xFF, INK_NONE, true);
                    }
                }
            }

#endif

            currentScreen++;
            sceneInfo.currentScreenID++;
        }
    }
}

uint16 RSDK::GetObjectByName(const char *name)
{
    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(name, hash);

    for (int32 o = 0; o < sceneInfo.classCount; ++o) {
        if (HASH_MATCH_MD5(hash, objectClassList[stageObjectIDs[o]].hash))
            return o;
    }

    return 0;
}

int32 RSDK::GetEntityCount(uint16 classID, bool32 isActive)
{
    if (classID >= TYPE_COUNT)
        return 0;
    if (isActive)
        return typeGroups[classID].entryCount;

    int32 entityCount = 0;
    for (int32 i = 0; i < ENTITY_COUNT; ++i) {
        if (objectEntityList[i].classID == classID)
            entityCount++;
    }

    return entityCount;
}

void RSDK::ResetEntityPtr(Entity *entity, uint16 classID, void *data)
{
    if (entity) {
        ObjectClass *info = &objectClassList[stageObjectIDs[classID]];
        memset(entity, 0, info->entityClassSize);

        if (info->create) {
            Entity *curEnt = sceneInfo.entity;

            sceneInfo.entity              = entity;
            sceneInfo.entity->interaction = true;
            info->create(data);
            sceneInfo.entity->classID = classID;

            sceneInfo.entity = curEnt;
        }

        entity->classID = classID;
    }
}

void RSDK::ResetEntitySlot(uint16 slot, uint16 classID, void *data)
{
    ObjectClass *object = &objectClassList[stageObjectIDs[classID]];
    slot                = slot < ENTITY_COUNT ? slot : (ENTITY_COUNT - 1);

    Entity *entity = &objectEntityList[slot];
    memset(&objectEntityList[slot], 0, object->entityClassSize);

    if (object->create) {
        Entity *curEnt = sceneInfo.entity;

        sceneInfo.entity    = entity;
        entity->interaction = true;
        object->create(data);
        entity->classID = classID;

        sceneInfo.entity = curEnt;
    }
    else {
        entity->classID = classID;
    }
}

Entity *RSDK::CreateEntity(uint16 classID, void *data, int32 x, int32 y)
{
    ObjectClass *object = &objectClassList[stageObjectIDs[classID]];
    Entity *entity      = &objectEntityList[sceneInfo.createSlot];

    int32 permCnt = 0, loopCnt = 0;
    while (entity->classID) {
        // after 16 loops, the game says fuck it and will start overwriting non-temp objects
        if (!entity->isPermanent && loopCnt >= 16)
            break;

        if (entity->isPermanent)
            ++permCnt;

        sceneInfo.createSlot++;
        if (sceneInfo.createSlot == ENTITY_COUNT) {
            sceneInfo.createSlot = TEMPENTITY_START;
            entity               = &objectEntityList[sceneInfo.createSlot];
        }
        else {
            entity = &objectEntityList[sceneInfo.createSlot];
        }

        if (permCnt >= TEMPENTITY_COUNT)
            break;

        ++loopCnt;
    }

    memset(entity, 0, object->entityClassSize);
    entity->position.x  = x;
    entity->position.y  = y;
    entity->interaction = true;

    if (object->create) {
        Entity *curEnt = sceneInfo.entity;

        sceneInfo.entity = entity;
        object->create(data);
        entity->classID = classID;

        sceneInfo.entity = curEnt;
    }
    else {
        entity->classID = classID;
        entity->active  = ACTIVE_NORMAL;
        entity->visible = true;
    }

    return entity;
}

bool32 RSDK::GetActiveEntities(uint16 group, Entity **entity)
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

    for (Entity *nextEntity = &objectEntityList[typeGroups[group].entries[foreachStackPtr->id]]; foreachStackPtr->id < typeGroups[group].entryCount;
         ++foreachStackPtr->id, nextEntity = &objectEntityList[typeGroups[group].entries[foreachStackPtr->id]]) {
        if (nextEntity->classID == group) {
            *entity = nextEntity;
            return true;
        }
    }

    foreachStackPtr--;

    return false;
}
bool32 RSDK::GetEntities(uint16 classID, Entity **entity)
{
    if (classID >= OBJECT_COUNT)
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

    for (Entity *nextEntity = &objectEntityList[foreachStackPtr->id]; foreachStackPtr->id < ENTITY_COUNT;
         ++foreachStackPtr->id, nextEntity = &objectEntityList[foreachStackPtr->id]) {
        if (nextEntity->classID == classID) {
            *entity = nextEntity;
            return true;
        }
    }

    foreachStackPtr--;

    return false;
}

bool32 RSDK::CheckOnScreen(Entity *entity, Vector2 *range)
{
    if (!entity)
        return false;

    if (!range)
        range = &entity->updateRange;

    return CheckPosOnScreen(&entity->position, range);
}
bool32 RSDK::CheckPosOnScreen(Vector2 *position, Vector2 *range)
{
    if (!position || !range)
        return false;

    for (int32 s = 0; s < cameraCount; ++s) {
        int32 sx = abs(position->x - cameras[s].position.x);
        int32 sy = abs(position->y - cameras[s].position.y);

        if (sx <= range->x + cameras[s].offset.x && sy <= range->y + cameras[s].offset.y)
            return true;
    }

    return false;
}

void RSDK::ClearStageObjects()
{
    // Unload static object classes
    for (int32 o = 0; o < sceneInfo.classCount; ++o) {
        if (objectClassList[stageObjectIDs[o]].staticVars) {
            *objectClassList[stageObjectIDs[o]].staticVars = NULL;
        }
    }
}