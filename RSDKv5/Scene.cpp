#include "RetroEngine.hpp"

byte tilesetGFXData[TILESET_SIZE * 4];

ScanlineInfo *scanlines = NULL;
TileLayer tileLayers[LAYER_COUNT];
CollisionMask collisionMasks[CPATH_COUNT][TILE_COUNT * 4];

#if RETRO_USE_PLUS
bool32 hardResetFlag = false;
#endif
char currentSceneFolder[0x10];

SceneInfo sceneInfo;

void LoadScene()
{
    sceneInfo.timeCounter  = 0;
    sceneInfo.minutes      = 0;
    sceneInfo.seconds      = 0;
    sceneInfo.milliseconds = 0;

    for (int i = 0; i < DRAWLAYER_COUNT; ++i) {
        drawLayers[i].entityCount = 0;
        drawLayers[i].layerCount  = 0;
    }

    for (int i = 0; i < TYPEGROUP_COUNT; ++i) {
        typeGroups[i].entryCount = 0;
    }
#if RETRO_USE_PLUS
    debugValCnt = 0;
#endif
    lookUpBuffer = NULL;

#if RETRO_USE_PLUS
    if (StrComp(currentSceneFolder, sceneInfo.listData[sceneInfo.listPos].folder) && !hardResetFlag) {
        // Reload
        ClearUnusedStorage(DATASET_STG);
        sceneInfo.filter = sceneInfo.listData[sceneInfo.listPos].filter;
        if (sceneInfo.filter == 0x00)
            sceneInfo.filter = 0xFF;
        printLog(SEVERITY_NONE, "Reloading Scene \"%s - %s\" with filter %d", sceneInfo.listCategory[sceneInfo.activeCategory].name,
                 sceneInfo.listData[sceneInfo.listPos].name, sceneInfo.listData[sceneInfo.listPos].filter);
        return;
    }
#endif

#if !RETRO_USE_PLUS
    if (StrComp(currentSceneFolder, sceneInfo.listData[sceneInfo.listPos].folder)) {
        // Reload
        ClearUnusedStorage(DATASET_STG);
        printLog(SEVERITY_NONE, "Reloading Scene \"%s - %s\"", sceneInfo.listCategory[sceneInfo.activeCategory].name,
                 sceneInfo.listData[sceneInfo.listPos].name);
        return;
    }
#endif

    // Unload Model data
    for (int m = 0; m < MODEL_MAX; ++m) {
        if (modelList[m].scope != SCOPE_GLOBAL) {
            MEM_ZERO(modelList[m]);
        }
    }

    //Unload 3D Scenes
    for (int s = 0; s < SCENE3D_MAX; ++s) {
        if (scene3DList[s].scope != SCOPE_GLOBAL) {
            MEM_ZERO(scene3DList[s]);
        }
    }

    // Unload animations
    for (int s = 0; s < SPRFILE_COUNT; ++s) {
        if (spriteAnimationList[s].scope != SCOPE_GLOBAL) {
            MEM_ZERO(spriteAnimationList[s]);
        }
    }

    // Unload surfaces
    for (int s = 0; s < SURFACE_MAX; ++s) {
        if (gfxSurface[s].scope != SCOPE_GLOBAL) {
            MEM_ZERO(gfxSurface[s]);
        }
    }

    // Unload stage SFX
    for (int s = 0; s < SFX_COUNT; ++s) {
        if (sfxList[s].scope != SCOPE_GLOBAL) {
            MEM_ZERO(sfxList[s]);
        }
    }

    // Unload object data
    for (int o = 0; o < sceneInfo.classCount; ++o) {
        if (objectList[stageObjectIDs[o]].type) {
            *objectList[stageObjectIDs[o]].type = NULL;
        }
    }

    ClearUnusedStorage(DATASET_STG);
    ClearUnusedStorage(DATASET_SFX);

    for (int s = 0; s < SCREEN_MAX; ++s) {
        screens[s].position.x = 0;
        screens[s].position.y = 0;
    }

    SceneListEntry *sceneEntry = &sceneInfo.listData[sceneInfo.listPos];
    StrCopy(currentSceneFolder, sceneEntry->folder);

#if RETRO_USE_PLUS
    hardResetFlag = false;
    sceneInfo.filter = sceneEntry->filter;
    if (sceneInfo.filter == 0x00)
        sceneInfo.filter = 0xFF;

    printLog(SEVERITY_NONE, "Loading Scene \"%s - %s\" with filter %d", sceneInfo.listCategory[sceneInfo.activeCategory].name, sceneEntry->name, sceneEntry->filter);
#endif

#if !RETRO_USE_PLUS
    printLog(SEVERITY_NONE, "Loading Scene \"%s - %s\"", sceneInfo.listCategory[sceneInfo.activeCategory].name, sceneEntry->name);
#endif

    char buffer[0x40];
    StrCopy(buffer, "Data/Stages/");
    StrAdd(buffer, currentSceneFolder);
    StrAdd(buffer, "/TileConfig.bin");
    LoadTileConfig(buffer);

    StrCopy(buffer, "Data/Stages/");
    StrAdd(buffer, currentSceneFolder);
    StrAdd(buffer, "/StageConfig.bin");

    FileInfo info;
    MEM_ZERO(info);
    if (LoadFile(&info, buffer)) {
        char buffer[0x100];
        uint sig = ReadInt32(&info);

        if (sig != 0x474643) {
            CloseFile(&info);
            return;
        }

        sceneInfo.useGlobalScripts = ReadInt8(&info);
        if (sceneInfo.useGlobalScripts) {
            for (int o = 0; o < globalObjectCount; ++o) {
                stageObjectIDs[o] = globalObjectIDs[o];
            }
            sceneInfo.classCount = TYPE_DEFAULTCOUNT + globalObjectCount;
        }
        else {
            for (int o = 0; o < TYPE_DEFAULTCOUNT; ++o) {
                stageObjectIDs[o] = globalObjectIDs[o];
            }

            sceneInfo.classCount = TYPE_DEFAULTCOUNT;
        }

        byte objCnt = ReadInt8(&info);

        for (int o = 0; o < objCnt; ++o) {
            ReadString(&info, hashBuffer);

            uint hash[4];
            GenerateHash(hash, StrLength(hashBuffer));

            if (objectCount > 0) {
                int objID                            = 0;
                stageObjectIDs[sceneInfo.classCount] = 0;
                do {
                    if (HASH_MATCH(hash, objectList[objID].hash)) {
                        stageObjectIDs[sceneInfo.classCount++] = objID;
                    }
                    ++objID;
                } while (objID < objectCount);
            }
        }

        for (int o = 0; o < sceneInfo.classCount; ++o) {
            ObjectInfo *obj = &objectList[stageObjectIDs[o]];
            if (obj->type && !*obj->type) {
                AllocateStorage(obj->objectSize, (void**)obj->type, DATASET_STG, true);
                LoadStaticObject((byte *)*obj->type, obj->hash, sizeof(Object));
                (*obj->type)->objectID = o;
                if (o >= TYPE_DEFAULTCOUNT)
                    (*obj->type)->active = ACTIVE_NORMAL;
            }
        }

        for (int i = 0; i < PALETTE_COUNT; ++i) {
            for (int r = 0; r < 0x10; ++r) {
                if ((activeGlobalRows[i] >> r & 1)) {
                    for (int c = 0; c < 0x10; ++c) {
                        fullPalette[i][(r << 4) + c] = globalPalette[i][(r << 4) + c];
                    }
                }
            }

            activeStageRows[i] = ReadInt16(&info);
            for (int r = 0; r < 0x10; ++r) {
                if ((activeStageRows[i] >> r & 1)) {
                    for (int c = 0; c < 0x10; ++c) {
                        byte red                     = ReadInt8(&info);
                        byte green                   = ReadInt8(&info);
                        byte blue                    = ReadInt8(&info);
                        fullPalette[i][(r << 4) + c] = bIndexes[blue] | gIndexes[green] | rIndexes[red];
                    }
                }
            }
        }

        byte sfxCnt = ReadInt8(&info);
        for (int i = 0; i < sfxCnt; ++i) {
            ReadString(&info, buffer);
            byte maxConcurrentPlays = ReadInt8(&info);
            LoadSfx(buffer, maxConcurrentPlays, SCOPE_STAGE);
        }

        CloseFile(&info);
    }

    StrCopy(buffer, "Data/Stages/");
    StrAdd(buffer, currentSceneFolder);
    StrAdd(buffer, "/16x16Tiles.gif");
    LoadStageGIF(buffer);
}
void LoadSceneFile() {

    memset(objectEntityList, 0, ENTITY_COUNT * sizeof(EntityBase));

    SceneListEntry *sceneEntry = &sceneInfo.listData[sceneInfo.listPos];
    char buffer[0x40];
    StrCopy(buffer, "Data/Stages/");
    StrAdd(buffer, currentSceneFolder);
    StrAdd(buffer, "/Scene");
    StrAdd(buffer, sceneEntry->sceneID);
    StrAdd(buffer, ".bin");

    dataStorage[DATASET_TMP].usedStorage = 0;

    for (int s = 0; s < SCREEN_MAX; ++s) screens[s].waterDrawPos = screens[s].height;

    if (screens[0].height > 0) {
        memset(gfxLineBuffer, 0, screens[0].height * sizeof(byte));
    }
    
    memset(tileLayers, 0, LAYER_COUNT * sizeof(TileLayer));

    FileInfo info;
    MEM_ZERO(info);
    if (LoadFile(&info, buffer)) {
        uint sig = ReadInt32(&info);

        if (sig != 0x4E4353) {
            CloseFile(&info);
            return;
        }

        Seek_Cur(&info, 0x10);
        byte strLen = ReadInt8(&info);
        Seek_Cur(&info, strLen + 1);

        byte layerCount = ReadInt8(&info);
        for (int i = 0; i < layerCount; ++i) {
            TileLayer* layer = &tileLayers[i];

            ReadInt8(&info);
            ReadString(&info, hashBuffer);
            GenerateHash(layer->name, StrLength(hashBuffer));

            layer->behaviour = ReadInt8(&info);
            layer->drawLayer[0] = ReadInt8(&info);
            for (int s = 1; s < screenCount; ++s) layer->drawLayer[s] = layer->drawLayer[0];

            layer->width = ReadInt16(&info);
            int w        = layer->width;
            if (w > 1) {
                int ls = 0;
                while (w > 1) {
                    w >>= 1;
                    ++ls;
                }
                layer->widthShift = ls;
            }

            layer->height = ReadInt16(&info);
            int h         = layer->height;
            if (h > 1) {
                int ls = 0;
                while (h > 1) {
                    h >>= 1;
                    ++ls;
                }
                layer->heightShift = ls;
            }

            layer->parallaxFactor = ReadInt16(&info);
            layer->scrollSpeed    = ReadInt16(&info) << 8;
            layer->scrollPos      = 0;

            layer->layout = NULL;
            if (layer->width || layer->height) {
                AllocateStorage(sizeof(ushort) * layer->width * layer->height, (void **)&layer->layout, DATASET_STG, true);
                memset(layer->layout, 0xFF, sizeof(ushort) * layer->width * layer->height);
            }

            int size = layer->width;
            if (size <= layer->height)
                size = layer->height;
            AllocateStorage(TILE_SIZE * size, (void **)&layer->lineScroll, DATASET_STG, true);

            layer->scrollInfoCount = ReadInt16(&info);
            for (int s = 0; s < layer->scrollInfoCount; ++s) {
                layer->scrollInfo[s].parallaxFactor = ReadInt16(&info);
                layer->scrollInfo[s].scrollSpeed    = ReadInt16(&info) << 8;
                layer->scrollInfo[s].scrollPos      = 0;
                layer->scrollInfo[s].unknown        = ReadInt8(&info);
                layer->scrollInfo[s].deform         = ReadInt8(&info);
            }

            byte *scrollIndexes = NULL;
            ReadZLibRSDK(&info, (byte**)&scrollIndexes);
            memcpy(layer->lineScroll, scrollIndexes, TILE_SIZE * size * sizeof(byte));
            scrollIndexes = NULL;

            byte *tileLayout = NULL;
            ReadZLibRSDK(&info, (byte **)&tileLayout);

            int id = 0;
            for (int y = 0; y < layer->height; ++y) {
                for (int x = 0; x < layer->width; ++x) {
                    layer->layout[x + (y * layer->width)] = (tileLayout[id + 1] << 8) + tileLayout[id + 0];
                    id += 2;
                }
            }
            tileLayout = NULL;
        }

        byte objCount = ReadInt8(&info);
        AllocateStorage(sizeof(EditableVarInfo) * EDITABLEVAR_COUNT, (void **)&editableVarList, DATASET_TMP, false);

        EntityBase *entList = NULL;
        AllocateStorage(SCENEENTITY_COUNT * sizeof(EntityBase), (void **)&entList, DATASET_TMP, true);
        for (int i = 0; i < objCount; ++i) {
            uint hashBuf[4];
            ReadHash(&info, hashBuf);

            int objID = 0;
            for (int o = 0; o < sceneInfo.classCount; ++o) {
                if (HASH_MATCH(hashBuf, objectList[stageObjectIDs[o]].hash)) {
                    objID = o;
                    break;
                }
            }

            ObjectInfo *obj = &objectList[stageObjectIDs[objID]];
            byte varCnt     = ReadInt8(&info);
            EditableVarInfo *varList = NULL;
            AllocateStorage(sizeof(EditableVarInfo) * varCnt, (void **)&varList, DATASET_TMP, false);
            editableVarCount = 0;
            if (objID) {
#if RETRO_USE_PLUS
                SetEditableVar(VAR_UINT8, "filter", objID, offsetof(Entity, filter));
#endif
                if (obj->serialize)
                    obj->serialize();
            }

            for (int e = 1; e < varCnt; ++e) {
                ReadHash(&info, hashBuf);

                int varID = 0;
                MEM_ZERO(varList[e]);
                for (int v = 0; v < editableVarCount; ++v) {
                    if (HASH_MATCH(hashBuf, editableVarList[v].hash)) {
                        varID = v;
                        memcpy(varList[e].hash, editableVarList[v].hash, 4 * sizeof(uint));
                        varList[e].offset = editableVarList[v].offset;
                        varList[e].active = true;
                        break;
                    }
                }

                editableVarList[varID].type = varList[e].type = ReadInt8(&info);
            }

            ushort entCount = ReadInt16(&info);
            for (int e = 0; e < entCount; ++e) {
                ushort slotID = ReadInt16(&info);

                EntityBase *entity = NULL;
                if (slotID < SCENEENTITY_COUNT)
                    entity = &entList[slotID];
                else
                    entity = &entList[SCENEENTITY_COUNT - slotID];

                entity->objectID = objID;
#if RETRO_USE_PLUS
                entity->filter = 0xFF;
#endif
                entity->position.x = ReadInt32(&info);
                entity->position.y = ReadInt32(&info);

                for (int v = 1; v < varCnt; ++v) {
                    switch (varList[v].type) {
                        case VAR_UINT8:
                        case VAR_INT8:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(byte));
                            }
                            else {
                                ReadInt8(&info);
                            }
                            break;
                        case VAR_UINT16:
                        case VAR_INT16:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(short));
                            }
                            else { 
                                ReadInt16(&info);
                            }
                            break;
                        case VAR_UINT32:
                        case VAR_INT32:
                        case VAR_ENUM: 
                        case VAR_BOOL: 
                        case VAR_COLOUR:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(int));
                            }
                            else {
                                ReadInt32(&info);
                            }
                            break;
                        case VAR_STRING:
                            if (varList[v].active) {
                                byte *ptr      = (byte *)entity;
                                TextInfo *text = (TextInfo *)&ptr[varList[v].offset];
                                ushort len     = ReadInt16(&info);

                                SetText(text, textBuffer, len);
                                for (text->textLength = 0; text->textLength < len; ++text->textLength) {
                                    ushort c                     = ReadInt16(&info);
                                    text->text[text->textLength] = c;
                                }
                            }
                            else {
                                ushort cnt = ReadInt16(&info);
                                Seek_Cur(&info, cnt * sizeof(ushort));
                            }
                            break;
                        case VAR_VECTOR2:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(int));
                                ReadBytes(&info, &ptr[varList[v].offset + sizeof(int)], sizeof(int));
                            }
                            else {
                                ReadInt32(&info); //x
                                ReadInt32(&info); //y
                            }
                            break;
                        case VAR_UNKNOWN:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(int));
                            }
                            else {
                                ReadInt32(&info);
                            }
                            break;
                    }
                }
            }
        }

#if RETRO_USE_PLUS
        //handle filter and stuff
        int slot = RESERVE_ENTITY_COUNT;
        int activeSlot = RESERVE_ENTITY_COUNT;
        /*for (int e = 0; e < SCENEENTITY_COUNT; ++e) {
            if (sceneInfo.filter & entList[e].filter) {
                if (activeSlot != slot) {
                    memcpy(&objectEntityList[slot], &objectEntityList[e], sizeof(EntityBase));
                    memset(&objectEntityList[e], 0, sizeof(EntityBase));
                }
                activeSlot++;
            }
            else {
                memset(&objectEntityList[slot], 0, sizeof(EntityBase));
            }
            slot++;
        }*/

        slot = activeSlot;
        for (int e = 0; e < SCENEENTITY_COUNT; ++e) {
            if (sceneInfo.filter & entList[e].filter) {
                memcpy(&objectEntityList[e + RESERVE_ENTITY_COUNT], &entList[e], sizeof(EntityBase));
            }
            slot++;
        }
#endif

#if !RETRO_USE_PLUS
        for (int e = 0; e < SCENEENTITY_COUNT; ++e) {
            memcpy(&objectEntityList[e], &entList[e], sizeof(EntityBase));
        }
#endif
        entList = NULL;

        CloseFile(&info);
    }
}
void LoadTileConfig(char *filepath)
{
    FileInfo info;
    MEM_ZERO(info);
    if (LoadFile(&info, filepath)) {
        uint sig = ReadInt32(&info);
        if (sig != 0x4C4954) {
            CloseFile(&info);
            return;
        }

        byte *buffer = NULL;
        ReadZLibRSDK(&info, &buffer);

        int bufPos    = 0;
        for (int p = 0; p < CPATH_COUNT; ++p) {
            //No Flip/Stored in file
            for (int t = 0; t < TILE_COUNT; ++t) {
                byte collision[0x10];
                byte hasCollision[0x10];

                memcpy(collision, buffer + bufPos, 0x10 * sizeof(byte));
                bufPos += 0x10;
                memcpy(hasCollision, buffer + bufPos, 0x10 * sizeof(byte));
                bufPos += 0x10;

                bool32 isCeiling                = buffer[bufPos++];
                collisionMasks[p][t].floorAngle = buffer[bufPos++];
                collisionMasks[p][t].rWallAngle = buffer[bufPos++];
                collisionMasks[p][t].lWallAngle = buffer[bufPos++];
                collisionMasks[p][t].roofAngle = buffer[bufPos++];
                collisionMasks[p][t].flag = buffer[bufPos++];

                if (isCeiling) // Ceiling Tile
                {
                    for (int c = 0; c < TILE_SIZE; c++) {
                        if (hasCollision[c]) {
                            collisionMasks[p][t].roofMasks[c]  = collision[c];
                            collisionMasks[p][t].floorMasks[c] = 0xFF;
                        }
                        else {
                            collisionMasks[p][t].floorMasks[c] = 0xFF;
                            collisionMasks[p][t].roofMasks[c]  = 0xFF;
                        }
                    }

                    // LWall rotations
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = 0;
                        while (h > -1) {
                            if (h == TILE_SIZE) {
                                collisionMasks[p][t].lWallMasks[c] = 0xFF;
                                h                                  = -1;
                            }
                            else if (c > collisionMasks[p][t].roofMasks[h]) {
                                ++h;
                            }
                            else {
                                collisionMasks[p][t].lWallMasks[c] = h;
                                h                                  = -1;
                            }
                        }
                    }

                    // RWall rotations
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = TILE_SIZE - 1;
                        while (h < TILE_SIZE) {
                            if (h == -1) {
                                collisionMasks[p][t].rWallMasks[c] = 0xFF;
                                h                                  = TILE_SIZE;
                            }
                            else if (c > collisionMasks[p][t].roofMasks[h]) {
                                --h;
                            }
                            else {
                                collisionMasks[p][t].rWallMasks[c] = h;
                                h                                  = TILE_SIZE;
                            }
                        }
                    }
                }
                else // Regular Tile
                {
                    // Collision heights
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        if (hasCollision[c]) {
                            collisionMasks[p][t].floorMasks[c] = collision[c];
                            collisionMasks[p][t].roofMasks[c]  = 0xFF;
                        }
                        else {
                            collisionMasks[p][t].floorMasks[c] = 0xFF;
                            collisionMasks[p][t].roofMasks[c]  = 0xFF;
                        }
                    }

                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = 0;
                        while (h > -1) {
                            if (h == TILE_SIZE) {
                                collisionMasks[p][t].lWallMasks[c] = 0xFF;
                                h                                  = -1;
                            }
                            else if (c < collisionMasks[p][t].floorMasks[h]) {
                                ++h;
                            }
                            else {
                                collisionMasks[p][t].lWallMasks[c] = h;
                                h                                  = -1;
                            }
                        }
                    }

                    // RWall rotations
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = TILE_SIZE - 1;
                        while (h < TILE_SIZE) {
                            if (h == -1) {
                                collisionMasks[p][t].rWallMasks[c] = 0xFF;
                                h                                  = TILE_SIZE;
                            }
                            else if (c < collisionMasks[p][t].floorMasks[h]) {
                                --h;
                            }
                            else {
                                collisionMasks[p][t].rWallMasks[c] = h;
                                h                                  = TILE_SIZE;
                            }
                        }
                    }
                }
            }

            //FlipX
            for (int t = 0; t < TILE_COUNT; ++t) {
                int off                               = (FLIP_X * TILE_COUNT);
                collisionMasks[p][t + off].flag       = collisionMasks[p][t].flag;
                collisionMasks[p][t + off].floorAngle = -collisionMasks[p][t].floorAngle;
                collisionMasks[p][t + off].lWallAngle = -collisionMasks[p][t].lWallAngle;
                collisionMasks[p][t + off].rWallAngle = -collisionMasks[p][t].rWallAngle;
                collisionMasks[p][t + off].roofAngle  = -collisionMasks[p][t].roofAngle;

                for (int c = 0; c < TILE_SIZE; ++c) {
                    collisionMasks[p][t + off].lWallMasks[0xF - c] = collisionMasks[p][t].rWallMasks[c];
                    collisionMasks[p][t + off].rWallMasks[0xF - c] = collisionMasks[p][t].lWallMasks[c];

                    if (collisionMasks[p][t].floorMasks[c] >= 0xFF)
                        collisionMasks[p][t + off].floorMasks[0xF - c] = 0xFF;
                    else
                        collisionMasks[p][t + off].floorMasks[0xF - c] = collisionMasks[p][t].floorMasks[c];

                    if (collisionMasks[p][t].roofMasks[c] >= 0xFF)
                        collisionMasks[p][t + off].roofMasks[0xF - c] = 0xFF;
                    else
                        collisionMasks[p][t + off].roofMasks[0xF - c] = collisionMasks[p][t].roofMasks[c];
                }
            }

            // FlipY
            for (int t = 0; t < TILE_COUNT; ++t) {
                int off                               = (FLIP_Y * TILE_COUNT);
                collisionMasks[p][t + off].flag       = collisionMasks[p][t].flag;
                collisionMasks[p][t + off].floorAngle = -0x80 - collisionMasks[p][t].floorAngle;
                collisionMasks[p][t + off].lWallAngle = -0x80 - collisionMasks[p][t].lWallAngle;
                collisionMasks[p][t + off].roofAngle  = -0x80 - collisionMasks[p][t].roofAngle;
                collisionMasks[p][t + off].rWallAngle = -0x80 - collisionMasks[p][t].rWallAngle;

                for (int c = 0; c < TILE_SIZE; ++c) {
                    collisionMasks[p][t + off].floorMasks[0xF - c] = collisionMasks[p][t].roofMasks[c];
                    collisionMasks[p][t + off].roofMasks[0xF - c]  = collisionMasks[p][t].floorMasks[c];

                    if (collisionMasks[p][t].lWallMasks[c] >= 0xFF)
                        collisionMasks[p][t + off].lWallMasks[0xF - c] = 0xFF;
                    else
                        collisionMasks[p][t + off].lWallMasks[0xF - c] = collisionMasks[p][t].lWallMasks[c];

                    if (collisionMasks[p][t].rWallMasks[c] >= 0xFF)
                        collisionMasks[p][t + off].rWallMasks[0xF - c] = 0xFF;
                    else
                        collisionMasks[p][t + off].rWallMasks[0xF - c] = collisionMasks[p][t].rWallMasks[c];
                }
            }

            // FlipXY
            for (int t = 0; t < TILE_COUNT; ++t) {
                int off                                                  = (FLIP_XY * TILE_COUNT);
                int offX                                                  = (FLIP_X * TILE_COUNT);
                collisionMasks[p][t + off].flag                           = collisionMasks[p][t + offX].flag;
                collisionMasks[p][t + off].floorAngle                    = -collisionMasks[p][t + offX].floorAngle;
                collisionMasks[p][t + off].lWallAngle                    = -collisionMasks[p][t + offX].lWallAngle;
                collisionMasks[p][t + off].rWallAngle                    = -collisionMasks[p][t + offX].rWallAngle;
                collisionMasks[p][t + off].roofAngle                      = -collisionMasks[p][t + offX].roofAngle;

                for (int c = 0; c < TILE_SIZE; ++c) {
                    collisionMasks[p][t + off].floorMasks[c] = collisionMasks[p][t + offX].roofMasks[c];
                    collisionMasks[p][t + off].roofMasks[c]  = collisionMasks[p][t + offX].floorMasks[c];

                    if (collisionMasks[p][t + offX].lWallMasks[c] >= 0xFF)
                        collisionMasks[p][t + off].lWallMasks[0xF - c] = 0xFF;
                    else
                        collisionMasks[p][t + off].lWallMasks[0xF - c] = collisionMasks[p][t + offX].lWallMasks[c];

                    if (collisionMasks[p][t + offX].rWallMasks[c] >= 0xFF)
                        collisionMasks[p][t + off].rWallMasks[0xF - c] = 0xFF;
                    else
                        collisionMasks[p][t + off].rWallMasks[0xF - c] = collisionMasks[p][t + offX].rWallMasks[c];
                }
            }
        }
        CloseFile(&info);
    }
}
void LoadStageGIF(char *filepath)
{
    ImageGIF tileset;
    MEM_ZERO(tileset);

    AllocateStorage(sizeof(GifDecoder), (void **)&tileset.decoder, DATASET_TMP, true);

    if (LoadGIF(&tileset, filepath, true) && tileset.width == TILE_SIZE && tileset.height <= 0x4000) {
        tileset.dataPtr = tilesetGFXData;
        LoadGIF(&tileset, 0, false);

        for (int r = 0; r < 0x10; ++r) {
            // only overwrite inactive rows
            if (!(activeStageRows[0] >> r & 1) && !(activeGlobalRows[0] >> r & 1)) {
                for (int c = 0; c < 0x10; ++c) {
                    byte red                     = (tileset.palette[(r << 4) + c] >> 0x10);
                    byte green                   = (tileset.palette[(r << 4) + c] >> 0x08);
                    byte blue                    = (tileset.palette[(r << 4) + c] >> 0x00);
                    fullPalette[0][(r << 4) + c] = bIndexes[blue] | gIndexes[green] | rIndexes[red];
                }
            }
        }

        tileset.palette = NULL;
        tileset.decoder = NULL;
        tileset.dataPtr = NULL;

        // Flip X
        for (int t = 0; t < TILE_COUNT; ++t) {
            for (int y = 0; y < TILE_SIZE; ++y) {
                for (int x = 0; x < TILE_SIZE; ++x) {
                    int dst             = (TILESET_SIZE * FLIP_X) + (t * TILE_DATASIZE) + (y * TILE_SIZE) + x;
                    int src             = (t * TILE_DATASIZE) + (y * TILE_SIZE) + ((TILE_SIZE - 1) - x);
                    tilesetGFXData[dst] = tilesetGFXData[src];
                }
            }
        }

        // Flip Y
        for (int t = 0; t < TILE_COUNT; ++t) {
            for (int y = 0; y < TILE_SIZE; ++y) {
                for (int x = 0; x < TILE_SIZE; ++x) {
                    int dst             = (TILESET_SIZE * FLIP_Y) + (t * TILE_DATASIZE) + (y * TILE_SIZE) + x;
                    int src             = (t * TILE_DATASIZE) + (((TILE_SIZE - 1) - y) * TILE_SIZE) + x;
                    tilesetGFXData[dst] = tilesetGFXData[src];
                }
            }
        }

        // Flip XY
        for (int t = 0; t < TILE_COUNT; ++t) {
            for (int y = 0; y < TILE_SIZE; ++y) {
                for (int x = 0; x < TILE_SIZE; ++x) {
                    int dst             = (TILESET_SIZE * FLIP_XY) + (t * TILE_DATASIZE) + (y * TILE_SIZE) + x;
                    int src             = (t * TILE_DATASIZE) + (((TILE_SIZE - 1) - y) * TILE_SIZE) + ((TILE_SIZE - 1) - x);
                    tilesetGFXData[dst] = tilesetGFXData[src];
                }
            }
        }
    }
}

void ProcessParallaxAutoScroll()
{
    for (int l = 0; l < LAYER_COUNT; ++l) {
        TileLayer *layer = &tileLayers[l];
        if (layer->layout) {
            layer->scrollPos += layer->scrollSpeed;
            for (int s = 0; s < layer->scrollInfoCount; ++s) {
                layer->scrollInfo[s].scrollPos += layer->scrollInfo[s].scrollSpeed;
            }
        }
    }
}
void ProcessParallax(TileLayer *layer)
{
    if (!layer->width || !layer->height)
        return;
    int pixelWidth            = TILE_SIZE * layer->width;
    int pixelHeight           = TILE_SIZE * layer->height;
    ScanlineInfo *scanlinePtr = scanlines;
    ScrollInfo *scrollInfo    = layer->scrollInfo;

    switch (layer->behaviour) {
        default: break;
        case LAYER_HSCROLL: {
            for (int i = 0; i < layer->scrollInfoCount; ++i) {
                scrollInfo->unknown = scrollInfo->scrollPos + (currentScreen->position.x * scrollInfo->parallaxFactor << 8);
                short pos           = (scrollInfo->unknown >> 16) % pixelWidth;
                if (pos < 0)
                    pos += pixelWidth;
                scrollInfo->unknown = pos << 0x10;
                ++scrollInfo;
            }

            short posY = (((layer->scrollPos + (layer->parallaxFactor * currentScreen->position.y << 8)) & 0xFFFF0000) >> 16) % pixelHeight;
            if (posY < 0)
                posY += pixelHeight;

            byte *lineScrollPtr = &layer->lineScroll[posY];

            //Above water
            int *deformationData = &layer->deformationData[((posY >> 16) + (ushort)layer->deformationOffset) & 0x1FF];
            for (int i = 0; i < currentScreen->waterDrawPos; ++i) {
                scanlinePtr->position.x = layer->scrollInfo[*lineScrollPtr].unknown;
                if (layer->scrollInfo[*lineScrollPtr].deform) {
                    scanlinePtr->position.x += *deformationData;
                }
                deformationData++;
                scanlinePtr->position.y = posY++ << 0x10;

                if (posY == pixelHeight) {
                    lineScrollPtr = layer->lineScroll;
                    posY          = 0;
                }
                else {
                    ++lineScrollPtr;
                }
                scanlinePtr++;
            }

            //Under water
            deformationData = &layer->deformationDataW[((posY >> 16) + (ushort)layer->deformationOffsetW) & 0x1FF];
            for (int i = currentScreen->waterDrawPos; i < currentScreen->height; ++i) {
                scanlinePtr->position.x = layer->scrollInfo[*lineScrollPtr].unknown;
                if (layer->scrollInfo[*lineScrollPtr].deform) {
                    scanlinePtr->position.x += *deformationData;
                }
                deformationData++;
                scanlinePtr->position.y = posY++;
                scanlinePtr->position.y <<= 0x10;
                scanlinePtr->deform.x = 0x10000;
                scanlinePtr->deform.y = 0x00000;

                if (posY == pixelHeight) {
                    lineScrollPtr = layer->lineScroll;
                    posY          = 0;
                }
                else {
                    ++lineScrollPtr;
                }
                scanlinePtr++;
            }
            break;
        }
        case LAYER_VSCROLL: {
            for (int i = 0; i < layer->scrollInfoCount; ++i) {
                scrollInfo->unknown  = scrollInfo->scrollPos + (currentScreen->position.y * scrollInfo->parallaxFactor << 8);
                scrollInfo->unknown = (scrollInfo->unknown >> 16) % pixelHeight;
                scrollInfo->unknown <<= 0x10;
                ++scrollInfo;
            }

            short posX = (((layer->scrollPos + (layer->parallaxFactor * currentScreen->position.x << 8)) & 0xFFFF0000) >> 16) % pixelWidth;
            if (posX < 0)
                posX += pixelWidth;

            byte *lineScrollPtr = &layer->lineScroll[posX];

            // Above water
            for (int i = 0; i < currentScreen->width; ++i) {
                scanlinePtr->position.x = posX++ << 0x10;
                scanlinePtr->position.y = layer->scrollInfo[*lineScrollPtr].unknown;
                scanlinePtr->deform.x   = 0x10000;
                scanlinePtr->deform.y   = 0x00000;

                if (posX == pixelWidth) {
                    lineScrollPtr = layer->lineScroll;
                    posX          = 0;
                }
                else {
                    ++lineScrollPtr;
                }
                scanlinePtr++;
            }
            break;
        }
        case LAYER_ROTOZOOM: {
            short posX = (((layer->scrollPos + (layer->parallaxFactor * currentScreen->position.x << 8)) & 0xFFFF0000) >> 0x10) % pixelWidth;
            if (posX < 0)
                posX += pixelWidth;

            short posY = (((layer->scrollPos + (layer->parallaxFactor * currentScreen->position.y << 8)) & 0xFFFF0000) >> 0x10) % pixelHeight;
            if (posY < 0)
                posY += pixelHeight;

            for (int i = 0; i < currentScreen->height; ++i) {
                scanlinePtr->position.x = posX << 0x10;
                scanlinePtr->position.y = posY++ << 0x10;
                scanlinePtr->deform.x   = 0x10000;
                scanlinePtr->deform.y   = 0x00000;
                scanlinePtr++;
            }
            break;
        }
        case LAYER_BASIC: {
            for (int i = 0; i < layer->scrollInfoCount; ++i) {
                scrollInfo->unknown = scrollInfo->scrollPos + (currentScreen->position.x * scrollInfo->parallaxFactor << 8);
                short pos           = (scrollInfo->unknown >> 16) % pixelWidth;
                if (pos < 0)
                    pos += pixelWidth;
                scrollInfo->unknown = pos << 0x10;
                ++scrollInfo;
            }

            short posY = (((layer->scrollPos + (layer->parallaxFactor * currentScreen->position.y << 8)) & 0xFFFF0000) >> 16) % pixelHeight;
            if (posY < 0)
                posY += pixelHeight;

            byte *lineScrollPtr = &layer->lineScroll[posY];

            // Above water
            int *deformationData = &layer->deformationData[((posY >> 16) + (ushort)layer->deformationOffset) & 0x1FF];
            for (int i = 0; i < currentScreen->waterDrawPos; ++i) {
                scanlinePtr->position.x = layer->scrollInfo[*lineScrollPtr].unknown;
                if (layer->scrollInfo[*lineScrollPtr].deform) {
                    scanlinePtr->position.x += *deformationData;
                }
                deformationData++;
                scanlinePtr->position.y = posY++ << 0x10;
                scanlinePtr->deform.x   = 0x10000;
                scanlinePtr->deform.y   = 0x00000;

                if (posY == pixelHeight) {
                    lineScrollPtr = layer->lineScroll;
                    posY          = 0;
                }
                else {
                    ++lineScrollPtr;
                }
                scanlinePtr++;
            }

            // Under water
            deformationData = &layer->deformationDataW[((posY >> 16) + (ushort)layer->deformationOffsetW) & 0x1FF];
            for (int i = currentScreen->waterDrawPos; i < currentScreen->height; ++i) {
                scanlinePtr->position.x = layer->scrollInfo[*lineScrollPtr].unknown;
                if (layer->scrollInfo[*lineScrollPtr].deform) {
                    scanlinePtr->position.x += *deformationData;
                }
                deformationData++;
                scanlinePtr->position.y = posY++ << 0x10;

                if (posY == pixelHeight) {
                    lineScrollPtr = layer->lineScroll;
                    posY          = 0;
                }
                else {
                    ++lineScrollPtr;
                }
                scanlinePtr++;
            }
            break;
        }
    }
}

void ProcessSceneTimer()
{
    if (sceneInfo.timeEnabled) {
        sceneInfo.timeCounter += 100;
        if (sceneInfo.timeCounter >= 6000) {
            sceneInfo.timeCounter -= 6025;
            if (++sceneInfo.seconds > 59) {
                sceneInfo.seconds = 0;
                sceneInfo.minutes++;
                if (sceneInfo.minutes > 59)
                    sceneInfo.minutes = 0;
            }
        }
        sceneInfo.milliseconds = sceneInfo.timeCounter / engine.refreshRate;
    }
}

void SetScene(const char *categoryName, const char *sceneName)
{
    uint hash[4];
    GEN_HASH(categoryName, hash);

    for (int i = 0; i < sceneInfo.categoryCount; ++i) {
        if (HASH_MATCH(sceneInfo.listCategory[i].hash, hash)) {
            sceneInfo.activeCategory = i;
            sceneInfo.listPos        = 0;
            GEN_HASH(sceneName, hash);

            for (int s = 0; s < sceneInfo.listCategory[i].sceneCount; ++s) {
                if (HASH_MATCH(sceneInfo.listData[sceneInfo.listCategory[i].sceneOffsetStart + s].hash, hash)) {
                    sceneInfo.listPos = s;
                    break;
                }
            }
            break;
        }
    }
}

void CopyTileLayout(ushort dstLayerID, int startX1, int startY1, ushort srcLayerID, int startX2, int startY2, int countX, int countY)
{
    if (dstLayerID < LAYER_COUNT && srcLayerID < LAYER_COUNT) {
        TileLayer *dstLayer = &tileLayers[dstLayerID];
        TileLayer *srcLayer = &tileLayers[srcLayerID];

        if (startX1 >= 0 && startX1 < dstLayer->width) {
            if (startY1 >= 0 && startY1 < dstLayer->height) {
                if (startX2 >= 0 && startX2 < srcLayer->width) {
                    if (startY2 >= 0 && startY2 < srcLayer->height) {

                        if (startX1 + countX > dstLayer->width) {
                            countX = dstLayer->width - startX1;
                        }

                        if (startY1 + countY > dstLayer->height) {
                            countY = dstLayer->height - startY1;
                        }

                        if (startX2 + countX > srcLayer->width) {
                            countX = srcLayer->width - startX2;
                        }

                        if (startY2 + countY > srcLayer->height) {
                            countY = srcLayer->height - startY2;
                        }

                        for (int y = 0; y < countY; ++y) {
                            for (int x = 0; x < countX; ++x) {
                                ushort tile = srcLayer->layout[(x + startX2) + ((y + startY2) * srcLayer->width)];
                                dstLayer->layout[(x + startX1) + ((y + startY1) * dstLayer->width)] = tile;
                            }
                        }
                    }
                }
            }
        }
    }
}

void DrawLayerHScroll(TileLayer *layer)
{
    if (!layer->width || !layer->height)
        return;
    int lineTileCount         = (currentScreen->pitch >> 4) - 1;
    byte *lineBuffer          = &gfxLineBuffer[currentScreen->clipBound_Y1];
    ScanlineInfo *scanlinePtr = &scanlines[currentScreen->clipBound_Y1];
    ushort *frameBuffer    = &currentScreen->frameBuffer[currentScreen->pitch * currentScreen->clipBound_Y1];

    for (int cy = currentScreen->clipBound_Y1; cy < currentScreen->clipBound_Y2; ++cy) {
        int x              = scanlinePtr->position.x;
        int y              = scanlinePtr->position.y;
        int tileX          = x >> 0x10;
        ushort *palette = fullPalette[*lineBuffer++];
        if (tileX >= TILE_SIZE * layer->width) {
            x = (tileX - TILE_SIZE * layer->width) << 0x10;
        }
        else if (tileX < 0) {
            x = (tileX + TILE_SIZE * layer->width) << 0x10;
        }
        int cnt  = TILE_SIZE - ((x >> 0x10) & 0xF);
        int cntX = (x >> 16) & 0xF;
        int cntY = TILE_SIZE * ((y >> 0x10) & 0xF);
        int lineRemain = currentScreen->width;

        int tx                = (x >> 20);
        ushort *layout = &layer->layout[tx + ((y >> 20) * layer->width)];

        lineRemain -= cnt;
        if (*layout >= 0xFFFF) {
            frameBuffer += cnt;
        }
        else {
            for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntX + cntY]; cnt; ++frameBuffer) {
                --cnt;
                if (*i)
                    *frameBuffer = palette[*i];
                ++i;
            }
        }

        for (int i = lineTileCount; i; frameBuffer += TILE_SIZE, lineRemain -= TILE_SIZE, --i) {
            ++layout;
            ++tx;

            if (tx == layer->width) {
                tx = 0;
                layout -= layer->width;
            }

            if (*layout < 0xFFFF) {
                byte *tilesetData = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntY];
                byte index        = *tilesetData;
                if (index)
                    *frameBuffer = palette[index];

                index = tilesetData[1];
                if (index)
                    frameBuffer[1] = palette[index];

                index = tilesetData[2];
                if (index)
                    frameBuffer[2] = palette[index];

                index = tilesetData[3];
                if (index)
                    frameBuffer[3] = palette[index];

                index = tilesetData[4];
                if (index)
                    frameBuffer[4] = palette[index];

                index = tilesetData[5];
                if (index)
                    frameBuffer[5] = palette[index];

                index = tilesetData[6];
                if (index)
                    frameBuffer[6] = palette[index];

                index = tilesetData[7];
                if (index)
                    frameBuffer[7] = palette[index];

                index = tilesetData[8];
                if (index)
                    frameBuffer[8] = palette[index];

                index = tilesetData[9];
                if (index)
                    frameBuffer[9] = palette[index];

                index = tilesetData[10];
                if (index)
                    frameBuffer[10] = palette[index];

                index = tilesetData[11];
                if (index)
                    frameBuffer[11] = palette[index];

                index = tilesetData[12];
                if (index)
                    frameBuffer[12] = palette[index];

                index = tilesetData[13];
                if (index)
                    frameBuffer[13] = palette[index];

                index = tilesetData[14];
                if (index)
                    frameBuffer[14] = palette[index];

                index = tilesetData[15];
                if (index)
                    frameBuffer[15] = palette[index];
            }
        }

        while (lineRemain > 0) {
            ++layout;
            ++tx;

            if (tx == layer->width) {
                tx = 0;
                layout -= layer->width;
            }

            int r = lineRemain >= TILE_SIZE ? TILE_SIZE : lineRemain;

            if (*layout >= 0xFFFF) {
                frameBuffer += r;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntY]; r; ++frameBuffer) {
                    --r;
                    if (*i)
                        *frameBuffer = palette[*i];
                    ++i;
                }
            }
            lineRemain -= TILE_SIZE;
        }

        ++scanlinePtr;
    }
}
void DrawLayerVScroll(TileLayer *layer)
{
    if (!layer->width || !layer->height)
        return;
    int lineTileCount       = (currentScreen->height >> 4) - 1;
    ushort *frameBuffer     = &currentScreen->frameBuffer[currentScreen->clipBound_X1];
    ScanlineInfo *scanLines = &scanlines[currentScreen->clipBound_X1];
    ushort *palettePtr      = fullPalette[gfxLineBuffer[0]];
    for (int cx = currentScreen->clipBound_X1; cx < currentScreen->clipBound_X2; ++cx) {
        int x       = scanLines->position.x;
        int y       = scanLines->position.y;
        int tileY   = y >> 0x10;
        if (tileY >= TILE_SIZE * layer->height) {
            y -= (TILE_SIZE * layer->height) << 0x10;
        }
        else if (tileY < 0) {
            y += (TILE_SIZE * layer->height) << 0x10;
        }

        int cnt        = TILE_SIZE - ((y >> 16) & 0xF);
        int cntY       = (y >> 16) & 0xF;
        int cntX       = (x >> 16) & 0xF;
        int lineRemain = currentScreen->height;

        ushort *layout = &layer->layout[(x >> 20) + ((y >> 20) * layer->width)];
        lineRemain -= cnt;
        if (*layout >= 0xFFFF) {
            frameBuffer += currentScreen->pitch * cnt;
        }
        else {
            for (byte *i = &tilesetGFXData[TILE_SIZE * (cntY + TILE_SIZE * (*layout & 0xFFF)) + cntX]; cnt; frameBuffer += currentScreen->pitch) {
                --cnt;
                if (*i)
                    *frameBuffer = palettePtr[*i];
                i += TILE_SIZE;
            }
        }

        tileY   = y >> 20;
        for (int i = 0; i < lineTileCount; ++i) {
            layout += layer->width;
            ++tileY;

            if (tileY == layer->height) {
                tileY = 0;
                layout -= layer->height * layer->width;
            }
            if (*layout >= 0xFFFF) {
                frameBuffer += TILE_SIZE * currentScreen->pitch;
            }
            else {
                byte *gfxPtr = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntX];
                if (*gfxPtr)
                    *frameBuffer = palettePtr[*gfxPtr];

                if (gfxPtr[0x10])
                    frameBuffer[currentScreen->pitch * 1] = palettePtr[gfxPtr[0x10]];

                if (gfxPtr[0x20])
                    frameBuffer[currentScreen->pitch * 2] = palettePtr[gfxPtr[0x20]];

                if (gfxPtr[0x30])
                    frameBuffer[currentScreen->pitch * 3] = palettePtr[gfxPtr[0x30]];

                if (gfxPtr[0x40])
                    frameBuffer[currentScreen->pitch * 4] = palettePtr[gfxPtr[0x40]];

                if (gfxPtr[0x50])
                    frameBuffer[currentScreen->pitch * 5] = palettePtr[gfxPtr[0x50]];

                if (gfxPtr[0x60])
                    frameBuffer[currentScreen->pitch * 6] = palettePtr[gfxPtr[0x60]];

                if (gfxPtr[0x70])
                    frameBuffer[currentScreen->pitch * 7] = palettePtr[gfxPtr[0x70]];

                if (gfxPtr[0x80])
                    frameBuffer[currentScreen->pitch * 8] = palettePtr[gfxPtr[0x80]];

                if (gfxPtr[0x90])
                    frameBuffer[currentScreen->pitch * 9] = palettePtr[gfxPtr[0x90]];

                if (gfxPtr[0xA0])
                    frameBuffer[currentScreen->pitch * 10] = palettePtr[gfxPtr[0xA0]];

                if (gfxPtr[0xB0])
                    frameBuffer[currentScreen->pitch * 11] = palettePtr[gfxPtr[0xB0]];

                if (gfxPtr[0xC0])
                    frameBuffer[currentScreen->pitch * 12] = palettePtr[gfxPtr[0xC0]];

                if (gfxPtr[0xD0])
                    frameBuffer[currentScreen->pitch * 13] = palettePtr[gfxPtr[0xD0]];

                if (gfxPtr[0xE0])
                    frameBuffer[currentScreen->pitch * 14] = palettePtr[gfxPtr[0xE0]];

                if (gfxPtr[0xF0])
                    frameBuffer[currentScreen->pitch * 15] = palettePtr[gfxPtr[0xF0]];

                frameBuffer += currentScreen->pitch * TILE_SIZE;
            }
            lineRemain -= TILE_SIZE;
        }

        while (lineRemain > 0) {
            layout += layer->width;
            ++tileY;

            if (tileY == layer->height) {
                tileY = 0;
                layout -= layer->height * layer->width;
            }

            int r = lineRemain >= TILE_SIZE ? TILE_SIZE : lineRemain;
            if (*layout >= 0xFFFF) {
                frameBuffer += currentScreen->pitch * cntY;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntX]; r; frameBuffer += currentScreen->pitch) {
                    --r;
                    if (*i)
                        *frameBuffer = palettePtr[*i];
                    i += 0x10;
                }
            }
            lineRemain -= TILE_SIZE;
        }

        ++scanLines;
        frameBuffer -= currentScreen->pitch * currentScreen->height;
        frameBuffer++;
    }
}
void DrawLayerRotozoom(TileLayer *layer)
{
    if (!layer->width || !layer->height)
        return;
    ushort *layout            = layer->layout;
    int countX                = currentScreen->clipBound_X2 - currentScreen->clipBound_X1;
    byte *lineBuffer          = &gfxLineBuffer[currentScreen->clipBound_Y1];
    ScanlineInfo *scanlinePtr = &scanlines[currentScreen->clipBound_Y1];
    ushort *frameBuffer       = &currentScreen->frameBuffer[currentScreen->clipBound_X1 + currentScreen->clipBound_Y1 * currentScreen->pitch];
    int width         = (TILE_SIZE * layer->width) - 1;
    int height        = (TILE_SIZE * layer->height) - 1;

    for (int cy = currentScreen->clipBound_Y1; cy < currentScreen->clipBound_Y2; ++cy) {
        int defX           = scanlinePtr->deform.x;
        int defY           = scanlinePtr->deform.y;
        int posX           = scanlinePtr->position.x;
        int posY           = scanlinePtr->position.y;

        ushort *palettePtr = fullPalette[*lineBuffer];
        ++lineBuffer;
        int fbOffset = currentScreen->pitch - countX;
        
        for (int cnt = countX; cnt; --cnt) {
            int tx   = posX >> 20;
            int ty   = posY >> 20;
            int x   = (posX >> 16) & 0xF;
            int y   = (posY >> 16) & 0xF;
            byte idx =
                tilesetGFXData[TILE_SIZE * (y + TILE_SIZE * (layout[((width >> 4) & tx) + (((height >> 4) & ty) * layer->width)] & 0xFFF)) + x];
            if (idx)
                *frameBuffer = palettePtr[idx];
            posX += defX;
            posY += defY;
            ++frameBuffer;
        }

        frameBuffer += fbOffset;
        ++scanlinePtr;
    }
}
void DrawLayerBasic(TileLayer *layer)
{
    if (!layer->width || !layer->height)
        return;
    int lineTileCount         = (currentScreen->pitch >> 4) - 1;
    byte *lineBuffer          = &gfxLineBuffer[currentScreen->clipBound_Y1];
    ScanlineInfo *scanlinePtr = &scanlines[currentScreen->clipBound_Y1];
    ushort *frameBuffer       = &currentScreen->frameBuffer[currentScreen->pitch * currentScreen->clipBound_Y1];

    for (int cy = currentScreen->clipBound_Y1; cy < currentScreen->clipBound_Y2; ++cy) {
        int x           = scanlinePtr->position.x;
        int y           = scanlinePtr->position.y;
        int tileX       = x >> 0x10;
        ushort *palette = fullPalette[*lineBuffer++];
        if (tileX >= TILE_SIZE * layer->width) {
            x = (tileX - TILE_SIZE * layer->width) << 0x10;
        }
        else if (tileX < 0) {
            x = (tileX + TILE_SIZE * layer->width) << 0x10;
        }
        int cnt        = TILE_SIZE - ((x >> 0x10) & 0xF);
        int cntX       = (x >> 16) & 0xF;
        int cntY       = TILE_SIZE * ((y >> 0x10) & 0xF);
        int lineRemain = currentScreen->width;

        int tx         = (x >> 20);
        ushort *layout = &layer->layout[tx + ((y >> 20) * layer->width)];

        lineRemain -= cnt;
        if (*layout >= 0xFFFF) {
            frameBuffer += cnt;
        }
        else {
            for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntX + cntY]; cnt; ++frameBuffer) {
                --cnt;
                if (*i)
                    *frameBuffer = palette[*i];
                ++i;
            }
        }

        for (int i = lineTileCount; i; frameBuffer += TILE_SIZE, lineRemain -= TILE_SIZE, --i) {
            ++layout;
            ++tx;

            if (tx == layer->width) {
                tx = 0;
                layout -= layer->width;
            }

            if (*layout < 0xFFFF) {
                byte *tilesetData = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntY];
                byte index        = *tilesetData;
                if (index)
                    *frameBuffer = palette[index];

                index = tilesetData[1];
                if (index)
                    frameBuffer[1] = palette[index];

                index = tilesetData[2];
                if (index)
                    frameBuffer[2] = palette[index];

                index = tilesetData[3];
                if (index)
                    frameBuffer[3] = palette[index];

                index = tilesetData[4];
                if (index)
                    frameBuffer[4] = palette[index];

                index = tilesetData[5];
                if (index)
                    frameBuffer[5] = palette[index];

                index = tilesetData[6];
                if (index)
                    frameBuffer[6] = palette[index];

                index = tilesetData[7];
                if (index)
                    frameBuffer[7] = palette[index];

                index = tilesetData[8];
                if (index)
                    frameBuffer[8] = palette[index];

                index = tilesetData[9];
                if (index)
                    frameBuffer[9] = palette[index];

                index = tilesetData[10];
                if (index)
                    frameBuffer[10] = palette[index];

                index = tilesetData[11];
                if (index)
                    frameBuffer[11] = palette[index];

                index = tilesetData[12];
                if (index)
                    frameBuffer[12] = palette[index];

                index = tilesetData[13];
                if (index)
                    frameBuffer[13] = palette[index];

                index = tilesetData[14];
                if (index)
                    frameBuffer[14] = palette[index];

                index = tilesetData[15];
                if (index)
                    frameBuffer[15] = palette[index];
            }
        }

        while (lineRemain > 0) {
            ++layout;
            ++tx;

            if (tx == layer->width) {
                tx = 0;
                layout -= layer->width;
            }

            int r = lineRemain >= TILE_SIZE ? TILE_SIZE : lineRemain;

            if (*layout >= 0xFFFF) {
                frameBuffer += r;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntY]; r; ++frameBuffer) {
                    --r;
                    if (*i)
                        *frameBuffer = palette[*i];
                    ++i;
                }
            }
            lineRemain -= TILE_SIZE;
        }

        ++scanlinePtr;
    }
}
