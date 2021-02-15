#include "RetroEngine.hpp"

int objectCount;
ObjectInfo objectList[OBJECT_COUNT];
int globalObjectCount = 0;
int globalObjectIDs[OBJECT_COUNT];
int stageObjectIDs[OBJECT_COUNT];

EntityBase objectEntityList[ENTITY_COUNT];

EditableVarInfo *editableVarList;
int editableVarCount = 0;

ForeachStackInfo foreachStackList[0x20];
ForeachStackInfo *foreachStackPtr = NULL;

TypeGroupList typeGroups[TYPEGROUP_COUNT];

bool32 validDraw = false;

void CreateObject(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void),
                  void (*lateUpdate)(void), void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void),
                  void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void))
{
    if (objectCount < OBJECT_COUNT) {
        ObjectInfo *info = &objectList[objectCount];
        if (entitySize > sizeof(EntityBase))
            printf("Class exceeds max entity memory: %s \n", name);
        info->type = structPtr;
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

void CreateObjectContainer(Object **structPtr, const char *name, uint objectSize)
{
    memset(hashBuffer, 0, 0x400);
    int len = StrLength(name);
    uint hash[4];

    memcpy(hashBuffer, name, len);
    GenerateHash(hash, len);
    AllocateStorage(objectSize, (void**)structPtr, DATASET_STG, true);
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
    MEM_ZERO(info);
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
                    case VAR_UINT8:
                    case VAR_INT8: 
                        if (info.readPos + dataSize <= info.fileSize) {
                            ReadBytes(&info, &obj[dataPos], sizeof(byte));
                        }
                        break;
                    case VAR_UINT16:
                    case VAR_INT16: {
                        int tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;
                        if (info.readPos + (dataSize * sizeof(short)) <= info.fileSize) {
                            ReadBytes(&info, &obj[dataPos], sizeof(short));
                        }
                        break;
                    }
                    case VAR_UINT32:
                    case VAR_INT32: 
                    case VAR_ENUM: {
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
        sceneInfo.entity     = &objectEntityList[e];
        if (sceneInfo.entity->objectID) {
            if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].create) {
                sceneInfo.entity->interaction = true;
                objectList[stageObjectIDs[sceneInfo.entity->objectID]].create(NULL);
            }
        }
    }

    SpawnEntity(TestObject->objectID, NULL, 0, 0);

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
        if ((*objInfo->type)->active == ACTIVE_ALWAYS || (*objInfo->type)->active == ACTIVE_NORMAL) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }

    for (int s = 0; s < screenCount; ++s) {
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

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entryCount = 0;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                           = e; // All active objects
            typeGroups[sceneInfo.entity->objectID].entries[typeGroups[sceneInfo.entity->objectID].entryCount++] = e; //type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; //extra slots
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
        if ((*objInfo->type)->active == ACTIVE_ALWAYS || (*objInfo->type)->active == ACTIVE_PAUSED) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }

    for (int s = 0; s < screenCount; ++s) {
    }

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

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entryCount = 0;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                           = e; // All active objects
            typeGroups[sceneInfo.entity->objectID].entries[typeGroups[sceneInfo.entity->objectID].entryCount++] = e; // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; // extra slots
            }
        }
        sceneInfo.entitySlot++;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->active == ACTIVE_ALWAYS || sceneInfo.entity->active == ACTIVE_PAUSED) {
            if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate) {
                objectList[stageObjectIDs[sceneInfo.entity->objectID]].lateUpdate();
            }
        }
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
        if ((*objInfo->type)->active == ACTIVE_ALWAYS || (*objInfo->type)->active == ACTIVE_PAUSED) {
            if (objInfo->staticUpdate)
                objInfo->staticUpdate();
        }
    }

    for (int s = 0; s < screenCount; ++s) {
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

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entryCount = 0;
    }

    sceneInfo.entitySlot = 0;
    for (int e = 0; e < ENTITY_COUNT; ++e) {
        sceneInfo.entity = &objectEntityList[e];
        if (sceneInfo.entity->inBounds && sceneInfo.entity->interaction) {
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                           = e; // All active objects
            typeGroups[sceneInfo.entity->objectID].entries[typeGroups[sceneInfo.entity->objectID].entryCount++] = e; // type-based slots
            if (sceneInfo.entity->group >= TYPE_COUNT) {
                typeGroups[sceneInfo.entity->group].entries[typeGroups[sceneInfo.entity->group].entryCount++] = e; // extra slots
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
        sceneInfo.entitySlot++;
    }
}
void ProcessObjectDrawLists()
{
    if (sceneInfo.state && sceneInfo.state != ENGINESTATE_LOAD_STEPOVER) {
        for (int s = 0; s < screenCount; ++s) {
            currentScreen             = &screens[s];
            sceneInfo.currentScreenID = s;
            for (int l = 0; l < DRAWLAYER_COUNT; ++l) {
                drawLayers[l].layerCount = 0;
            }

            for (int t = 0; t < LAYER_COUNT; ++t) {
                byte drawOrder = tileLayers[t].drawLayer[s];
                if (drawOrder < LAYER_COUNT)
                    drawLayers[drawOrder].layerDrawList[drawLayers[drawOrder].layerCount++] = t;
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
                            if (objectList[stageObjectIDs[sceneInfo.entity->objectID]].draw) {
                                objectList[stageObjectIDs[sceneInfo.entity->objectID]].draw();
                            }
                            sceneInfo.entity->activeScreens |= validDraw << sceneInfo.currentScreenID;
                        }
                    }

                    for (int i = 0; i < list->layerCount; ++i) {
                        TileLayer *layer       = &tileLayers[list->layerDrawList[i]];

                        if (layer->scanlineCallback)
                            layer->scanlineCallback(scanlines);
                        else
                            ProcessParallax(layer);
                        switch (layer->behaviour) {
                            case LAYER_HSCROLL: DrawLayerHScroll(layer); break;
                            case LAYER_VSCROLL: DrawLayerVScroll(layer); break;
                            case LAYER_ROTOZOOM: DrawLayerRotozoom(layer); break;
                            case LAYER_BASIC: DrawLayerBasic(layer); break;
                            default: break;
                        }
                    }
                }

                /*v29 = 0;
                if (ScreenWidth < 0)
                    v29 = ScreenWidth;
                ScreenClipX1 = v29;
                v30          = 0;
                if (ScreenHeight < 0)
                    v30 = ScreenHeight;
                ScreenClipY1 = v30;

                if (currentScreen->width >= 0) {
                    v31 = ScreenWidth;
                    if (currentScreen->width < ScreenWidth)
                        v31 = currentScreen->width;
                    ScreenClipX2 = v31;
                }
                else {
                    ScreenClipX2 = 0;
                }

                if (currentScreen->height >= 0) {
                    v32 = ScreenHeight;
                    if (currentScreen->height < ScreenHeight)
                        v32 = currentScreen->height;
                    ScreenClipY2 = v32;
                }
                else {
                    ScreenClipY2 = 0;
                }*/

                sceneInfo.currentDrawGroup++;
            }

#if !RETRO_USE_ORIGINAL_CODE
            if (engine.showPaletteOverlay) {
                for (int p = 0; p < PALETTE_COUNT; ++p) {
                    int x = (SCREEN_XSIZE - (0xF << 3));
                    int y = (SCREEN_YSIZE - (0xF << 2));
                    for (int c = 0; c < PALETTE_SIZE; ++c) {
                        uint clr = GetPaletteEntry(p, c);

                        DrawRectangle(x + ((c & 0xF) << 1) + ((p % (PALETTE_COUNT / 2)) * (2 * 16)),
                                      y + ((c >> 4) << 1) + ((p / (PALETTE_COUNT / 2)) * (2 * 16)), 2, 2, clr, 0xFF, INK_NONE, true);
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
    StrCopy(hashBuffer, name);
    uint hash[4];
    GenerateHash(hash, StrLength(hashBuffer));

    for (int o = 0; o < sceneInfo.classCount; ++o) {
        if (memcmp(hash, objectList[stageObjectIDs[o]].hash, 4 * sizeof(int)) == 0)
            return o;
    }
    return 0;
}

int GetEntityCount(ushort type, bool32 isActive)
{
    if (type >= TYPE_COUNT)
        return 0;
    if (isActive)
        return typeGroups[type].entryCount;

    int cnt = 0;
    for (int i = 0; i < ENTITY_COUNT; ++i) {
        if (objectEntityList[i].objectID == type)
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
        sceneInfo.entity->objectID = type;
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
        entityPtr->objectID  = type;
    }
    else {
        entityPtr->objectID = type;
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
        entityPtr->objectID = type;
    }
    else {
        entityPtr->objectID = type;
        entityPtr->active = ACTIVE_NORMAL;
        entityPtr->visible  = true;
    }
}

bool32 GetActiveObjects(ushort group, Entity **entity)
{
    if (group > 0x103u)
        return false;
    if (!entity)
        return false;
    ForeachStackInfo *stackPtr   = foreachStackPtr;
    Entity* nextEntity = 0;
    bool flag       = 0;
    while (true) {
        if (nextEntity && nextEntity->objectID == group) {
            *entity = nextEntity;
            return true;
        }
        if (flag || *entity) {
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
bool32 GetObjects(ushort type, Entity **entity)
{
    if (type > 0xFFu)
        return 0;
    if (!entity)
        return 0;

    ForeachStackInfo *stackPtr = foreachStackPtr;
    if (*entity) {
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
    while (nextEnt->objectID != type) {
        ++stackPtr->id;
        ++nextEnt;
        if (stackPtr->id >= ENTITY_COUNT) {
            foreachStackPtr = stackPtr - 1;
            return false;
        }
    }

    *entity = nextEnt;
    return true;
}


bool32 CheckOnScreen(Entity *entity, Vector2 *range)
{
    if (!entity)
        return false;

    if (range) {
        for (int s = 0; s < screenCount; ++s) {
            int sx = sceneInfo.entity->position.x - screens[s].position.x;
            int sy = sceneInfo.entity->position.y - screens[s].position.y;
            if (sx >= 0 && sy >= 0 && sx >= range->x + screens[s].width && sy >= range->y + screens[s].height) {
                return true;
            }
        }
    }
    else {
        for (int s = 0; s < screenCount; ++s) {
            int sx = sceneInfo.entity->position.x - screens[s].position.x;
            int sy = sceneInfo.entity->position.y - screens[s].position.y;
            if (sx >= 0 && sy >= 0 && sx >= sceneInfo.entity->updateRange.x + screens[s].width
                && sy >= sceneInfo.entity->updateRange.y + screens[s].height) {
                return true;
            }
        }
    }
    return false;
}
bool32 CheckPosOnScreen(Vector2 *position, Vector2 *range) {
    if (!position || !range)
        return false;

    for (int s = 0; s < screenCount; ++s) {
        int sx = position->x - screens[s].position.x;
        int sy = position->y - screens[s].position.y;
        if (sx >= 0 && sy >= 0 && sx >= range->x + screens[s].width && sy >= range->y + screens[s].height) {
            return true;
        }
    }

    return false; 
}