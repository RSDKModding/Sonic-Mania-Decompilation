#include "RetroEngine.hpp"

int objectCount;
ObjectInfo objectList[OBJECT_COUNT];
int globalObjectCount = 0;
int globalObjectIDs[OBJECT_COUNT];
int stageObjectIDs[OBJECT_COUNT];

EntityBase objectEntityList[ENTITY_COUNT];

EditableVarInfo *editableVarList;
int editableVarCount = 0;

TypeGroupList typeGroups[TYPEGROUP_COUNT];

bool32 validDraw = false;

ForeachStackInfo foreachStackList[0x20];
ForeachStackInfo *foreachStackPtr = NULL;

void CreateObject(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                  void (*staticUpdate)(void), void (*draw)(void), void (*create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                  void (*editorLoad)(void), void (*serialize)(void))
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

#if RETRO_USE_PLUS
void CreateObjectContainer(Object **structPtr, const char *name, uint objectSize)
{
    memset(hashBuffer, 0, 0x400);
    int len = StrLength(name);
    uint hash[4];

    memcpy(hashBuffer, name, len);
    GenerateHash(hash, len);
    AllocateStorage(objectSize, (void **)structPtr, DATASET_STG, true);
    LoadStaticObject((byte *)*structPtr, hash, 0);
}
#endif

void LoadStaticObject(byte *obj, uint *hash, int dataPos)
{
    char buffer[0x40];
    const char *hexChars = "0123456789ABCDEF";
    StrCopy(buffer, "Data/Objects/Static/00000000000000000000000000000000.bin");

    int strPos = 20;
    for (int i = 0; i < 32; i += 4) {
        int charVal      = hash[0] >> i;
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
                        if (info.readPos + dataSize <= info.fileSize && &obj[dataPos]) {
                            for (int i = 0; i < dataSize * sizeof(byte); i += sizeof(byte)) ReadBytes(&info, &obj[dataPos + i], sizeof(byte));
                        }
                        else {
                            for (int i = 0; i < dataSize * sizeof(byte); ++i) ReadInt8(&info);
                        }
                        dataPos += arraySize;
                        break;
                    case VAR_UINT16:
                    case VAR_INT16: {
                        int tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;
                        if (info.readPos + (dataSize * sizeof(short)) <= info.fileSize && &obj[dataPos]) {
                            for (int i = 0; i < dataSize * sizeof(short); i += sizeof(short)) ReadBytes(&info, &obj[dataPos + i], sizeof(short));
                        }
                        else {
                            for (int i = 0; i < dataSize * sizeof(short); ++i) ReadInt8(&info);
                        }
                        dataPos = tmp + sizeof(short) * arraySize;
                        break;
                    }
                    case VAR_UINT32:
                    case VAR_INT32:
                    case VAR_ENUM: {
                        int tmp = (dataPos & 0xFFFFFFFC) + 2;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp;
                        if (info.readPos + (dataSize * sizeof(int)) <= info.fileSize && &obj[dataPos]) {
                            for (int i = 0; i < dataSize * sizeof(int); i += sizeof(int)) ReadBytes(&info, &obj[dataPos + i], sizeof(int));
                        }
                        else {
                            for (int i = 0; i < dataSize * sizeof(int); ++i) ReadInt8(&info);
                        }
                        dataPos = tmp + sizeof(int) * arraySize;
                        break;
                    }
                }
            }
            else {
                int tmp = 0;
                switch (dataType) {
                    case VAR_UINT8:
                    case VAR_INT8: dataPos += sizeof(byte) * arraySize; break;
                    case VAR_UINT16:
                    case VAR_INT16:
                        tmp = (dataPos & 0xFFFFFFFE) + sizeof(short);
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(short) * arraySize;
                        break;
                    case VAR_UINT32:
                    case VAR_INT32:
                    case VAR_ENUM:
                        tmp = (dataPos & 0xFFFFFFFC) + sizeof(int);
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(int) * arraySize;
                        break;
                    case 7: //any pointer
                        tmp = (dataPos & 0xFFFFFFFC) + sizeof(int);
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(int *) * arraySize;
                        break;
                    case VAR_STRING: //textInfo
                        tmp = (dataPos & 0xFFFFFFFC) + 4;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(TextInfo) * arraySize; // 8
                        break;
                    case VAR_VECTOR2: //vector2
                        tmp = (dataPos & 0xFFFFFFFC) + 4;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(Vector2) * arraySize; // 8
                        break;
                    case 10: //EntityAnimationData
                        tmp = (dataPos & 0xFFFFFFFC) + 4;
                        if ((dataPos & 0xFFFFFFFC) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(EntityAnimationData) * arraySize; // 24
                        break;
                    case 11: //Hitbox
                        tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + sizeof(Hitbox) * arraySize; // 8
                        break;
                    case 12: //???
                        tmp = (dataPos & 0xFFFFFFFE) + 2;
                        if ((dataPos & 0xFFFFFFFE) >= dataPos)
                            tmp = dataPos;
                        dataPos = tmp + 18 * arraySize; // 18
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
    screenCount          = 0;

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

    // SpawnEntity(TestObject->objectID, NULL, 0, 0);

    sceneInfo.state = ENGINESTATE_REGULAR;
    if (!screenCount) {
        screenUnknown[0].targetPos     = &screens[0].position;
        screenUnknown[0].offset.x      = screens[0].centerX / 2;
        screenUnknown[0].offset.y      = screens[0].centerY / 2;
        screenUnknown[0].worldRelative = false;
        screenCount                    = 1;
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
        ScreenUnknown *screen = &screenUnknown[s];
        if (screen->targetPos) {
            if (screen->worldRelative) {
                screen->position.x = screen->targetPos->x;
                screen->position.y = screen->targetPos->y;
            }
            else {
                screen->position.x = screen->targetPos->x << 0x10;
                screen->position.y = screen->targetPos->y << 0x10;
            }
            screens[s].position.x = screen->position.x >> 0x10;
            screens[s].position.y = screen->position.y >> 0x10;
            screens[s].position.x -= screen->offset.x >> 0x10;
            screens[s].position.y -= screen->offset.y >> 0x10;
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
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = abs((sceneInfo.entity->position.x >> 0x10) - screens[s].position.x);
                        int sy = abs((sceneInfo.entity->position.y >> 0x10) - screens[s].position.y);
                        if (sx <= (sceneInfo.entity->updateRange.x >> 0x10) + screens[s].width
                            && sy <= (sceneInfo.entity->updateRange.y >> 0x10) + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = abs((sceneInfo.entity->position.x >> 0x10) - screens[s].position.x);
                        if (sx <= (sceneInfo.entity->updateRange.x >> 0x10) + screens[s].width) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sy = abs((sceneInfo.entity->position.y >> 0x10) - screens[s].position.y);
                        if (sy <= (sceneInfo.entity->updateRange.y >> 0x10) + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = abs((sceneInfo.entity->position.x >> 0x10) - screens[s].position.x);
                        int sy = abs((sceneInfo.entity->position.y >> 0x10) - screens[s].position.y);

                        if (sx * sx + sy * sy <= (sceneInfo.entity->updateRange.x >> 0x10) + screens[s].width) {
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
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                                   = e; // All active objects
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
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                                   = e; // All active objects
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
        ScreenUnknown *screen = &screenUnknown[s];
        if (screen->targetPos) {
            if (screen->worldRelative) {
                screen->position.x = screen->targetPos->x;
                screen->position.y = screen->targetPos->y;
            }
            else {
                screen->position.x = screen->targetPos->x << 0x10;
                screen->position.y = screen->targetPos->y << 0x10;
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
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = abs((sceneInfo.entity->position.x >> 0x10) - screens[s].position.x);
                        int sy = abs((sceneInfo.entity->position.y >> 0x10) - screens[s].position.y);
                        if (sx < sceneInfo.entity->updateRange.x + screens[s].width
                            && sy < sceneInfo.entity->updateRange.y + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_XBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = abs((sceneInfo.entity->position.x >> 0x10) - screens[s].position.x);
                        if (sx < sceneInfo.entity->updateRange.x + screens[s].width) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_YBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sy = abs((sceneInfo.entity->position.y >> 0x10) - screens[s].position.y);
                        if (sy < sceneInfo.entity->updateRange.y + screens[s].height) {
                            sceneInfo.entity->inBounds = true;
                            break;
                        }
                    }
                    break;
                case ACTIVE_RBOUNDS:
                    sceneInfo.entity->inBounds = false;
                    for (int s = 0; s < screenCount; ++s) {
                        int sx = abs((sceneInfo.entity->position.x >> 0x10) - screens[s].position.x);
                        int sy = abs((sceneInfo.entity->position.y >> 0x10) - screens[s].position.y);

                        if (sx * sx + sy * sy <= (sceneInfo.entity->updateRange.x >> 0x10) + screens[s].width) {
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
            typeGroups[GROUP_ALL].entries[typeGroups[GROUP_ALL].entryCount++]                                   = e; // All active objects
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
                        TileLayer *layer = &tileLayers[list->layerDrawList[i]];

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

                if (currentScreen->clipBound_X1 > 0)
                    currentScreen->clipBound_X1 = 0;

                if (currentScreen->clipBound_Y1 > 0)
                    currentScreen->clipBound_Y1 = 0;

                if (currentScreen->width >= 0) {
                    if (currentScreen->clipBound_X2 < currentScreen->width)
                        currentScreen->clipBound_X2 = currentScreen->width;
                }
                else {
                    currentScreen->clipBound_X2 = 0;
                }

                if (currentScreen->height >= 0) {
                    if (currentScreen->clipBound_Y2 < currentScreen->height)
                        currentScreen->clipBound_Y2 = currentScreen->height;
                }
                else {
                    currentScreen->clipBound_Y2 = 0;
                }

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

void CreateEntity(ushort type, void *data, int x, int y)
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
        entityPtr->active   = ACTIVE_NORMAL;
        entityPtr->visible  = true;
    }
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

    for (Entity *nextEnt = &objectEntityList[typeGroups[group].entries[foreachStackPtr->id]]; foreachStackPtr->id < typeGroups[group].entryCount;
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
        for (int s = 0; s < screenCount; ++s) {
            int sx = abs(sceneInfo.entity->position.x - screenUnknown[s].position.x);
            int sy = abs(sceneInfo.entity->position.y - screenUnknown[s].position.y);
            if (sx <= range->x + screenUnknown[s].offset.x && sy <= range->y + screenUnknown[s].offset.y) {
                return true;
            }
        }
    }
    else {
        for (int s = 0; s < screenCount; ++s) {
            int sx = abs(entity->position.x - screenUnknown[s].position.x);
            int sy = abs(entity->position.y - screenUnknown[s].position.y);
            if (sx <= entity->updateRange.x + screenUnknown[s].offset.x
                && sy <= entity->updateRange.y + screenUnknown[s].offset.y) {
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

    for (int s = 0; s < screenCount; ++s) {
        int sx = abs(position->x - screenUnknown[s].position.x);
        int sy = abs(position->y - screenUnknown[s].position.y);
        if (sx <= range->x + screenUnknown[s].offset.x && sy <= range->y + screenUnknown[s].offset.y) {
            return true;
        }
    }

    return false;
}
