#include "RetroEngine.hpp"

int objectCount;
ObjectInfo objectList[OBJECT_COUNT];
int globalObjectCount = 0;
int globalObjectIDs[OBJECT_COUNT];
int stageObjectIDs[OBJECT_COUNT];

EntityBase objectEntityList[ENTITY_COUNT];

EditableVarInfo editableVarList[EDITABLEVAR_COUNT];
int editableVarCount = 0;

ForeachStackInfo foreachStackList[0x20];
ForeachStackInfo *foreachStackPtr = NULL;

TypeGroupList typeGroups[TYPEGROUP_COUNT];

bool32 validDraw = false;

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
    sceneInfo.entitySlot = 0;
    sceneInfo.createSlot = ENTITY_COUNT - 0x100;
    for (int o = 0; o < sceneInfo.classCount; ++o) {
        if (objectList[stageObjectIDs[o]].stageLoad)
            objectList[stageObjectIDs[o]].stageLoad();
    }

    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entitySlot = e;
        Entity *entity  = &objectEntityList[e];
        if (sceneInfo.entity->type) {
            if (objectList[stageObjectIDs[sceneInfo.entity->type]].create) {
                sceneInfo.entity = entity;
                objectList[stageObjectIDs[sceneInfo.entity->type]].create(NULL);
            }
        }
    }

    sceneInfo.state = ENGINESTATE_REGULAR;
    if (!screenCount) {
        screens[0].position.x = 0;
        screens[0].position.y = 0;
        screens[0].width      = SCREEN_XSIZE;
        screens[0].height     = SCREEN_YSIZE;
        screenCount           = 1;
    }
}
void ProcessObjects()
{
    for (int i = 0; i < DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entityCount = 0;
    }

    for (int o = 0; o < sceneInfo.classCount; ++o) {
        ObjectInfo *objInfo = &objectList[stageObjectIDs[o]];
        if (objInfo->type->priority == ACTIVE_ALWAYS || objInfo->type->priority == ACTIVE_NORMAL) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }

    for (int s = 0; s < screenCount; ++s) {
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity    = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->type) {
            switch (sceneInfo.entity->priority) {
                case ACTIVE_NEVER:
                case ACTIVE_PAUSED: sceneInfo.entity->inBounds = false; break;
                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;
                case ACTIVE_BOUNDS: 
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = sceneInfo.entity->position.x - screens[s].position.x;
                        int sy = sceneInfo.entity->position.y - screens[s].position.y;
                        if (sx >= 0 && sy >= 0 && sx >= sceneInfo.entity->updateRange.x + screens[s].width && sy >= sceneInfo.entity->updateRange.y + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = sceneInfo.entity->position.x - screens[s].position.x;
                        if (sx >= 0 && sx >= sceneInfo.entity->updateRange.x + screens[s].width) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sy = sceneInfo.entity->position.y - screens[s].position.y;
                        if (sy >= 0 && sy >= sceneInfo.entity->updateRange.y + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = sceneInfo.entity->position.x - screens[s].position.x >= 0 ? sceneInfo.entity->position.x - screens[s].position.x
                                                                                 : screens[s].position.x - sceneInfo.entity->position.x;
                        sx >>= 16;
                        int sy = sceneInfo.entity->position.y - screens[s].position.y >= 0 ? sceneInfo.entity->position.y - screens[s].position.y
                                                                                 : screens[s].position.y - sceneInfo.entity->position.y;
                        sy >>= 16;
                        if (sx * sx + sy * sy <= sceneInfo.entity->updateRange.x + screens[s].width) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
            }

            if (sceneInfo.entity->inBounds) {
                if (objectList[stageObjectIDs[sceneInfo.entity->type]].update) {
                    objectList[stageObjectIDs[sceneInfo.entity->type]].update();
                }

                if (sceneInfo.entity->drawOrder < DRAWLAYER_COUNT)
                    drawLayers[sceneInfo.entity->drawOrder].entries[drawLayers[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
            }
        }
        else {
            sceneInfo.entity->inBounds = false;
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entryCount = 0;
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                           = e; // All active objects
            typeGroups[sceneInfo.entity->type].entries[typeGroups[sceneInfo.entity->type].entryCount++] = e; //type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; //extra slots
            }
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->inBounds) {
            if (objectList[stageObjectIDs[sceneInfo.entity->type]].lateUpdate) {
                objectList[stageObjectIDs[sceneInfo.entity->type]].lateUpdate();
            }
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }
}
void ProcessPausedObjects()
{
    for (int i = 0; i < DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entityCount = 0;
    }

    for (int o = 0; o < sceneInfo.classCount; ++o) {
        ObjectInfo *objInfo = &objectList[stageObjectIDs[o]];
        if (objInfo->type->priority == ACTIVE_ALWAYS || objInfo->type->priority == ACTIVE_PAUSED) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }

    for (int s = 0; s < screenCount; ++s) {
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->type) {
            if (sceneInfo.entity->priority == ACTIVE_ALWAYS || sceneInfo.entity->priority == ACTIVE_PAUSED) {
                if (objectList[stageObjectIDs[sceneInfo.entity->type]].update) {
                    objectList[stageObjectIDs[sceneInfo.entity->type]].update();
                }

                if (sceneInfo.entity->drawOrder < DRAWLAYER_COUNT)
                    drawLayers[sceneInfo.entity->drawOrder].entries[drawLayers[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
            }
        }
        else {
            sceneInfo.entity->inBounds = false;
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entryCount = 0;
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                           = e; // All active objects
            typeGroups[sceneInfo.entity->type].entries[typeGroups[sceneInfo.entity->type].entryCount++] = e; // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; // extra slots
            }
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->priority == ACTIVE_ALWAYS || sceneInfo.entity->priority == ACTIVE_PAUSED) {
            if (objectList[stageObjectIDs[sceneInfo.entity->type]].lateUpdate) {
                objectList[stageObjectIDs[sceneInfo.entity->type]].lateUpdate();
            }
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }
}
void ProcessFrozenObjects()
{
    for (int i = 0; i < DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entityCount = 0;
    }

    for (int o = 0; o < sceneInfo.classCount; ++o) {
        ObjectInfo *objInfo = &objectList[stageObjectIDs[o]];
        if (objInfo->type->priority == ACTIVE_ALWAYS || objInfo->type->priority == ACTIVE_PAUSED) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }

    for (int s = 0; s < screenCount; ++s) {
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->type) {
            switch (sceneInfo.entity->priority) {
                case ACTIVE_NEVER:
                case ACTIVE_PAUSED: sceneInfo.entity->inBounds = false; break;
                case ACTIVE_ALWAYS:
                case ACTIVE_NORMAL: sceneInfo.entity->inBounds = true; break;
                case ACTIVE_BOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = sceneInfo.entity->position.x - screens[s].position.x;
                        int sy = sceneInfo.entity->position.y - screens[s].position.y;
                        if (sx >= 0 && sy >= 0 && sx >= sceneInfo.entity->updateRange.x + screens[s].width
                            && sy >= sceneInfo.entity->updateRange.y + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = sceneInfo.entity->position.x - screens[s].position.x;
                        if (sx >= 0 && sx >= sceneInfo.entity->updateRange.x + screens[s].width) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sy = sceneInfo.entity->position.y - screens[s].position.y;
                        if (sy >= 0 && sy >= sceneInfo.entity->updateRange.y + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = sceneInfo.entity->position.x - screens[s].position.x >= 0 ? sceneInfo.entity->position.x - screens[s].position.x
                                                                                           : screens[s].position.x - sceneInfo.entity->position.x;
                        sx >>= 16;
                        int sy = sceneInfo.entity->position.y - screens[s].position.y >= 0 ? sceneInfo.entity->position.y - screens[s].position.y
                                                                                           : screens[s].position.y - sceneInfo.entity->position.y;
                        sy >>= 16;
                        if (sx * sx + sy * sy <= sceneInfo.entity->updateRange.x + screens[s].width) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
            }

            if (sceneInfo.entity->inBounds) {
                if (sceneInfo.entity->priority == ACTIVE_ALWAYS || sceneInfo.entity->priority == ACTIVE_PAUSED) {
                    if (objectList[stageObjectIDs[sceneInfo.entity->type]].update) {
                        objectList[stageObjectIDs[sceneInfo.entity->type]].update();
                    }
                }

                if (sceneInfo.entity->drawOrder < DRAWLAYER_COUNT)
                    drawLayers[sceneInfo.entity->drawOrder].entries[drawLayers[sceneInfo.entity->drawOrder].entityCount++] = sceneInfo.entitySlot;
            }
        }
        else {
            sceneInfo.entity->inBounds = false;
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entryCount = 0;
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                           = e; // All active objects
            typeGroups[sceneInfo.entity->type].entries[typeGroups[sceneInfo.entity->type].entryCount++] = e; // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; // extra slots
            }
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    sceneInfo.entity     = &objectEntityList[0];
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        if (sceneInfo.entity->inBounds) {
            if (sceneInfo.entity->priority == ACTIVE_ALWAYS || sceneInfo.entity->priority == ACTIVE_PAUSED) {
                if (objectList[stageObjectIDs[sceneInfo.entity->type]].lateUpdate) {
                    objectList[stageObjectIDs[sceneInfo.entity->type]].lateUpdate();
                }
            }
        }
        sceneInfo.entity++;
        sceneInfo.entitySlot++;
    }
}
void ProcessObjectDrawLists()
{
    if (sceneInfo.state && sceneInfo.state != ENGINESTATE_LOAD_STEPOVER) {
        ScreenInfo *currentScreen = &screens[0];
        sceneInfo.currentScreenID = 0;
        for (int s = 0; s < screenCount; ++s) {
            for (int l = 0; l < DRAWLAYER_COUNT; ++l) {
                drawLayers[l].layerCount = 0;
            }

            for (int t = 0; t < LAYER_COUNT; ++t) {
                byte drawOrder = 0; // tileLayers[t].drawLayer[l];
                if (drawOrder < LAYER_COUNT)
                    drawLayers[t].layerDrawList[drawLayers[t].layerCount++] = drawOrder;
            }

            sceneInfo.currentDrawGroup = 0;
            for (int l = 0; l < DRAWLAYER_COUNT; ++l) {
                DrawList *list = &drawLayers[l];
                
                if (list->initDrawPtr)
                    list->initDrawPtr();

                if (list->visible) {
                    for (int e = 0; e < list->entityCount; ++e) {
                        for (int e2 = list->entityCount - 1; e2 > e; --e2) {
                            int slotA = list->entries[e2 - 1];
                            int slotB = list->entries[e2];
                            if (objectEntityList[slotA].depth > objectEntityList[slotB].depth) {
                                list->entries[e2 - 1] = slotB;
                                list->entries[e2]     = slotA;
                            }
                        }
                    }

                    for (int i = 0; i < list->entityCount; ++i) {
                        sceneInfo.entitySlot = list->entries[i];
                        validDraw            = false;
                        sceneInfo.entity     = &objectEntityList[list->entries[i]];
                        if (sceneInfo.entity->visible) {
                            if (objectList[stageObjectIDs[sceneInfo.entity->type]].draw) {
                                objectList[stageObjectIDs[sceneInfo.entity->type]].draw();
                            }
                            sceneInfo.entity->activeScreens |= validDraw << sceneInfo.currentScreenID;
                        }
                    }

                    for (int i = 0; i < list->layerCount; ++i) {
                        //TileLayer *layer       = &TileLayers[list->layerDrawList[i]];
                        //
                        //if (layer->ParallaxPtr)
                        //    layer->ParallaxPtr(linePositions);
                        //else
                        //    ProcessParallaxScroll(layer);
                        //switch (layer->Behaviour) {
                        //    case 0: DrawLayerHScroll(layer); break;
                        //    case 1: DrawLayerVScroll(layer); break;
                        //    case 2: DrawLayer3(layer); break;
                        //    case 3: DrawLayer4(layer); break;
                        //    default: break;
                        //}
                    }
                }

                sceneInfo.currentDrawGroup++;
            }
            currentScreen++;
            sceneInfo.currentScreenID++;
        }
    }
}

ushort GetObjectByName(const char *name)
{
    StrCopy(hashBuffer, name);
    uint hash[4];
    GenerateHash(hash, StrLength(hashBuffer));

    for (int o = 0; o < sceneInfo.classCount; ++o) {
        if (memcmp(hash, objectList[stageObjectIDs[o]].hash, 4 * sizeof(int)) == 0)
            return o;
    }
    return 0xFFFF;
}

int GetEntityCount(ushort type, bool32 isActive)
{
    if (type >= TYPE_COUNT)
        return 0;
    if (isActive)
        return typeGroups[type].entryCount;

    int cnt = 0;
    for (int i = 0; i < ENTITY_COUNT; ++i) {
        if (objectEntityList[i].type == type)
            cnt++;
    }
    return cnt;
}

void DestroyEntity(Entity *entity, ushort type, void *data)
{
    if (entity) {
        ObjectInfo *info = &objectList[stageObjectIDs[type]];
        memset(entity, 0, info->entitySize);
        if (info->create) {
            Entity *curEnt      = sceneInfo.entity;
            sceneInfo.entity    = entity;
            sceneInfo.entity->interaction = 1;
            info->create(data);
            sceneInfo.entity = curEnt;
        }
        sceneInfo.entity->type = type;
    }
}

void ResetEntity(ushort slotID, ushort type, void *data)
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
        sceneInfo.entity = curEnt;
        entityPtr->type  = type;
    }
    else {
        entityPtr->type = type;
    }
}

void SpawnEntity(ushort type, void *data, int x, int y)
{
    if (sceneInfo.createSlot >= ENTITY_COUNT)
        sceneInfo.createSlot -= 0x100;

    ObjectInfo *objInfo = &objectList[stageObjectIDs[type]];
    Entity *entityPtr   = &objectEntityList[sceneInfo.createSlot++];

    memset(entityPtr, 0, objInfo->entitySize);
    entityPtr->position.x  = x;
    entityPtr->position.y  = y;
    entityPtr->interaction = true;

    if (objInfo->create) {
        Entity *curEnt   = sceneInfo.entity;
        sceneInfo.entity = entityPtr;
        objInfo->create(data);
        sceneInfo.entity    = curEnt;
        entityPtr->type = type;
    }
    else {
        entityPtr->type = type;
        entityPtr->priority = ACTIVE_NORMAL;
        entityPtr->visible  = true;
    }
}

bool32 GetActiveObjects(ushort group, Entity *entity) {
    if (group > 0x103u)
        return false;
    if (!entity)
        return false;
    ForeachStackInfo *stackPtr   = foreachStackPtr;
    Entity* nextEntity = 0;
    bool flag       = 0;
    while (true) {
        if (nextEntity && nextEntity->type == group) {
            entity = nextEntity;
            return true;
        }
        if (flag || entity) {
            ++stackPtr->id;
        }
        else {
            foreachStackPtr++;
            stackPtr->id = 0;
        }
        if (stackPtr->id >= typeGroups[group].entryCount)
            break;
        flag       = true;
        nextEntity = &objectEntityList[typeGroups[group].entries[stackPtr->id]];
    }
    foreachStackPtr--;
    return false;
}
bool32 GetObjects(ushort type, Entity* entity) {
    if (type > 0xFFu)
        return 0;
    if (!entity)
        return 0;

    ForeachStackInfo *stackPtr = foreachStackPtr;
    if (entity) {
        ++foreachStackPtr->id;
    }
    else {
        foreachStackPtr++;
        foreachStackPtr->id = 0;
    }
    
    Entity* nextEnt = &objectEntityList[stackPtr->id];
    if (stackPtr->id >= ENTITY_COUNT) {
        foreachStackPtr = stackPtr - 1;
        return false;
    }
    while (nextEnt->type != type) {
        ++stackPtr->id;
        ++nextEnt;
        if (stackPtr->id >= ENTITY_COUNT) {
            foreachStackPtr = stackPtr - 1;
            return false;
        }
    }

    entity = nextEnt;
    return true;
}