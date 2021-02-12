#include "RetroEngine.hpp"

byte tilesetGFXData[TILESET_SIZE];

ScanlineInfo scanlines[SCREEN_YSIZE];
TileLayer tileLayers[LAYER_COUNT];
CollisionMasks collisionMasks[2];

bool hardResetFlag = false;
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

    debugValCnt = 0;
    lookUpBuffer = NULL;

    if (StrComp(currentSceneFolder, sceneInfo.listData[sceneInfo.listPos].folder) && !hardResetFlag) {
        // Reload
        ClearUnusedStorage(DATASET_STG);
        sceneInfo.filter = sceneInfo.listData[sceneInfo.listPos].modeFilter;
        return;
    }

    // Unload Model data
    for (int m = 0; m < MODEL_MAX; ++m) {
        if (modelList[m].scope != SCOPE_GLOBAL) {
            modelList[m].scope = SCOPE_NONE;
            memset(modelList[m].hash, 0, 4 * sizeof(uint));
        }
    }

    //Unload 3D Scenes
    for (int m = 0; m < SCENE3D_MAX; ++m) {
        if (scene3DList[m].scope != SCOPE_GLOBAL) {
            scene3DList[m].scope = SCOPE_NONE;
            memset(scene3DList[m].hash, 0, 4 * sizeof(int));
        }
    }

    // Unload animations
    for (int s = 0; s < SPRFILE_COUNT; ++s) {
        if (spriteAnimationList[s].scope != SCOPE_GLOBAL) {
            spriteAnimationList[s].scope = SCOPE_NONE;
            memset(spriteAnimationList[s].hash, 0, 4 * sizeof(uint));
        }
    }

    // Unload surfaces
    for (int s = 0; s < SURFACE_MAX; ++s) {
        if (gfxSurface[s].scope != SCOPE_GLOBAL) {
            gfxSurface[s].scope = SCOPE_NONE;
            memset(gfxSurface[s].hash, 0, 4 * sizeof(uint));
        }
    }

    // Unload stage SFX

    // Unload object data
    for (int o = 0; o < sceneInfo.classCount; ++o) {
        if (objectList[stageObjectIDs[o]].type) {
            objectList[stageObjectIDs[o]].type = NULL;
        }
    }

    ClearUnusedStorage(DATASET_STG);
    ClearUnusedStorage(DATASET_SFX);

    SceneListEntry *sceneEntry = &sceneInfo.listData[sceneInfo.listPos];
    StrCopy(currentSceneFolder, sceneEntry->folder);
    hardResetFlag = false;
    sceneInfo.filter = sceneEntry->modeFilter;

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
        }
        else {
            stageObjectIDs[0]    = globalObjectIDs[0];
            stageObjectIDs[1]    = globalObjectIDs[1];
            sceneInfo.classCount = 2;
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
                    if (hash[0] == objectList[objID].hash[0] && hash[1] == objectList[objID].hash[1] && hash[2] == objectList[objID].hash[2]
                        && hash[3] == objectList[objID].hash[3]) {
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
                LoadStaticObject((byte *)obj->type, obj->hash, 2);
                (*obj->type)->objectID = o;
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

    memset(objectEntityList, 0, ENTITY_COUNT * sizeof(EntityBase));
    userStorage[DATASET_TMP].usedStorage = 0;

    //if (dword_B82A94 > 0) {
    //    memset(gfxLineBuffer, 0, dword_B82A94);
    //}
    
    memset(tileLayers, 0, LAYER_COUNT * sizeof(TileLayer));

    FileInfo info;
    MEM_ZERO(info);
    if (LoadFile(&info, buffer)) {
        char buffer[0x100];
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

            byte unknown = ReadInt8(&info);
            ReadString(&info, hashBuffer);
            GenerateHash(layer->name, StrLength(hashBuffer));

            layer->behaviour = ReadInt8(&info);
            layer->drawLayer[0] = ReadInt8(&info);
            layer->drawLayer[1] = layer->drawLayer[0];
            layer->drawLayer[2] = layer->drawLayer[0];
            layer->drawLayer[3] = layer->drawLayer[0];

            layer->width = ReadInt16(&info);
            int w        = layer->width;
            if (w > 1) {
                int ls = 0;
                do {
                    w >>= 1;
                    ++ls;
                } while (w > 1);
                layer->widthShift = ls;
            }

            layer->height = ReadInt16(&info);
            int h         = layer->height;
            if (h > 1) {
                int ls = 0;
                do {
                    h >>= 1;
                    ++ls;
                } while (h > 1);
                layer->heightShift = ls;
            }

            layer->parallaxFactor = ReadInt16(&info);
            layer->scrollSpeed    = ReadInt16(&info) << 8;
            layer->scrollPos      = 0;

            AllocateStorage(sizeof(ushort) * (1 << layer->widthShift) * (1 << layer->heightShift), (void **)&layer->layout, DATASET_STG, true);
            memset(layer->layout, 0xFF, sizeof(ushort) * (1 << layer->widthShift) * (1 << layer->heightShift));

            int size           = layer->width;
            if (size <= layer->height)
                size = layer->height;
            AllocateStorage(TILE_SIZE * size, (void **)&layer->lineScroll, DATASET_STG, true);

            layer->scrollIndexCount = ReadInt16(&info);
            for (int s = 0; s < layer->scrollIndexCount; ++s) {
                layer->scrollInfo[i].parallaxFactor = ReadInt16(&info);
                layer->scrollInfo[i].scrollSpeed    = ReadInt16(&info) << 8;
                layer->scrollInfo[i].scrollPos      = 0;
                layer->scrollInfo[i].behaviour      = ReadInt8(&info);
                layer->scrollInfo[i].deform         = ReadInt8(&info);
            }

            byte *scrollIndexes = NULL;
            ReadZLib(&info, (byte**)&scrollIndexes);
            memcpy(layer->lineScroll, scrollIndexes, size * sizeof(byte));

            byte *tileLayout = NULL;
            ReadZLib(&info, (byte **)&tileLayout);

            int id = 0;
            for (int y = 0; y < layer->height; ++y) {
                for (int x = 0; x < layer->width; ++x) {
                    layer->layout[x + (y * layer->width)] = (tileLayout[id + 0] << 8) + tileLayout[id + 1];
                    id += 2;
                }
            }
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
                if (memcmp(hashBuf, objectList[stageObjectIDs[o]].hash, 4 * sizeof(int)) == 0) {
                    objID = o;
                    break;
                }
            }

            ObjectInfo *obj = &objectList[stageObjectIDs[objID]];
            byte varCnt     = ReadInt8(&info);
            EditableVarInfo *varList = NULL;
            AllocateStorage(sizeof(EditableVarInfo) * varCnt, (void **)&varList, DATASET_TMP, false);
            if (objID) {
                editableVarCount = 0;
                SetEditableVar(VAR_UINT8, "filter", objID, offsetof(Entity, filter));
                if (obj->serialize)
                    obj->serialize();
            }

            for (int e = 1; e < varCnt; ++e) {
                ReadHash(&info, hashBuf);

                int varID = 0;
                editableVarList[e].active = false;
                for (int v = 0; v < editableVarCount; ++v) {
                    if (memcmp(hashBuf, editableVarList[v].hash, 4 * sizeof(int)) == 0) {
                        varID = v;
                        memcpy(varList[e].hash, editableVarList[v].hash, 4 * sizeof(int));
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

                entity->type = objID;
                entity->filter = 0xFF;
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

        //handle filter and stuff
        int slot = RESERVE_ENTITY_COUNT;
        int activeSlot = RESERVE_ENTITY_COUNT;
        for (int e = 0; e < 0x800; ++e) {
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
        }

        slot = activeSlot;
        for (int e = 0; e < 0x800; ++e) {
            if (sceneInfo.filter & entList[e].filter) {
                memcpy(&objectEntityList[slot], &entList[e], sizeof(EntityBase));
            }
            slot++;
        }

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
        ReadZLib(&info, &buffer);

        int bufPos    = 0;
        for (int p = 0; p < CPATH_COUNT; ++p) {
            int tileIndex = 0;
            for (int t = 0; t < TILE_COUNT; ++t) {
                byte collision[0x10];
                byte hasCollision[0x10];

                memcpy(collision, buffer + bufPos, 0x10 * sizeof(byte));
                bufPos += 0x10;
                memcpy(hasCollision, buffer + bufPos, 0x10 * sizeof(byte));
                bufPos += 0x10;

                bool isCeiling              = buffer[bufPos++];
                collisionMasks[p].angles[t] = buffer[bufPos++] << 0;
                collisionMasks[p].angles[t] += buffer[bufPos++] << 8;
                collisionMasks[p].angles[t] += buffer[bufPos++] << 16;
                collisionMasks[p].angles[t] += buffer[bufPos++] << 24;
                collisionMasks[p].flags[t] = buffer[bufPos++];

                if (isCeiling) // Ceiling Tile
                {
                    for (int c = 0; c < TILE_SIZE; c++) {
                        if (hasCollision[c]) {
                            collisionMasks[p].roofMasks[c + tileIndex]  = collision[c];
                            collisionMasks[p].floorMasks[c + tileIndex] = 0;
                        }
                        else {
                            collisionMasks[p].floorMasks[c + tileIndex] = 0xFF;
                            collisionMasks[p].roofMasks[c + tileIndex]  = 0xFF;
                        }
                    }

                    // LWall rotations
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = 0;
                        while (h > -1) {
                            if (h == TILE_SIZE) {
                                collisionMasks[p].lWallMasks[c + tileIndex] = 0xFF;
                                h                                           = -1;
                            }
                            else if (c > collisionMasks[p].roofMasks[h + tileIndex]) {
                                ++h;
                            }
                            else {
                                collisionMasks[p].lWallMasks[c + tileIndex] = h;
                                h                                           = -1;
                            }
                        }
                    }

                    // RWall rotations
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = TILE_SIZE - 1;
                        while (h < TILE_SIZE) {
                            if (h == -1) {
                                collisionMasks[p].rWallMasks[c + tileIndex] = 0xFF;
                                h                                           = TILE_SIZE;
                            }
                            else if (c > collisionMasks[p].roofMasks[h + tileIndex]) {
                                --h;
                            }
                            else {
                                collisionMasks[p].rWallMasks[c + tileIndex] = h;
                                h                                           = TILE_SIZE;
                            }
                        }
                    }
                }
                else // Regular Tile
                {
                    //Collision heights
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        if (hasCollision[c]) {
                            collisionMasks[p].floorMasks[c + tileIndex] = collision[c];
                            collisionMasks[p].roofMasks[c + tileIndex]  = 0x0F;
                        }
                        else {
                            collisionMasks[p].floorMasks[c + tileIndex] = 0xFF;
                            collisionMasks[p].roofMasks[c + tileIndex]  = 0xFF;
                        }
                    }

                    // LWall rotations
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = 0;
                        while (h > -1) {
                            if (h == TILE_SIZE) {
                                collisionMasks[p].lWallMasks[c + tileIndex] = 0xFF;
                                h                                           = -1;
                            }
                            else if (c < collisionMasks[p].floorMasks[h + tileIndex]) {
                                ++h;
                            }
                            else {
                                collisionMasks[p].lWallMasks[c + tileIndex] = h;
                                h                                           = -1;
                            }
                        }
                    }

                    // RWall rotations
                    for (int c = 0; c < TILE_SIZE; ++c) {
                        int h = TILE_SIZE - 1;
                        while (h < TILE_SIZE) {
                            if (h == -1) {
                                collisionMasks[p].rWallMasks[c + tileIndex] = 0xFF;
                                h                                           = TILE_SIZE;
                            }
                            else if (c < collisionMasks[p].floorMasks[h + tileIndex]) {
                                --h;
                            }
                            else {
                                collisionMasks[p].rWallMasks[c + tileIndex] = h;
                                h                                           = TILE_SIZE;
                            }
                        }
                    }
                }
                tileIndex += 16;
            }
        }
        CloseFile(&info);
    }
}
void LoadStageGIF(char *filepath)
{
    Image tileset;
    MEM_ZERO(tileset);

    AllocateStorage(sizeof(GifDecoder), (void **)&tileset.decoder, DATASET_TMP, true);

    if (LoadGIF(&tileset, filepath, true) && tileset.width == TILE_SIZE && tileset.height <= 0x4000) {
        tileset.dataPtr = tilesetGFXData;
        LoadGIF(&tileset, 0, false);

        for (int r = 0; r < 0x10; ++r) {
            // only overwrite inactive rows
            if (!(activeStageRows[0] >> r & 1)) {
                for (int c = 0; c < 0x10; ++c) {
                    byte red                     = (tileset.palette[(r << 4) + c] >> 0x10);
                    byte green                   = (tileset.palette[(r << 4) + c] >> 0x08);
                    byte blue                    = (tileset.palette[(r << 4) + c] >> 0x00);
                    fullPalette[0][(r << 4) + c] = bIndexes[blue] | gIndexes[green] | rIndexes[red];
                }
            }
        }

        // TODO: theres more stuff here but its unreadable so make sure to look back at it eventually
    }
}

void ProcessParallaxAutoScroll()
{
    for (int l = 0; l < LAYER_COUNT; ++l) {
        TileLayer *layer = &tileLayers[l];
        if (layer->layout) {
            layer->scrollPos += layer->scrollSpeed;
            for (int s = 0; s < layer->scrollIndexCount; ++s) {
                layer->scrollInfo[s].scrollPos += layer->scrollInfo[s].scrollSpeed;
            }
        }
    }
}
void ProcessParallax(TileLayer *layer)
{
    int pixelWidth  = TILE_SIZE * layer->width;
    int pixelHeight = TILE_SIZE * layer->height;
    switch (layer->behaviour) {
        default: break;
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
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

void LoadSceneByName(const char *categoryName, const char *sceneName)
{
    uint hash[4];
    GEN_HASH(categoryName, hash);

    for (int i = 0; i < sceneInfo.categoryCount; ++i) {
        if (HASH_MATCH(sceneInfo.listCategory[i].hash, hash)) {
            sceneInfo.activeCategory = i;
            GEN_HASH(sceneName, hash);

            for (int s = 0; s < sceneInfo.listCategory[i].sceneCount; ++s) {
                if (HASH_MATCH(sceneInfo.listData[sceneInfo.listCategory[i].sceneOffsetStart + i].hash, hash)) {
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
                                ushort tile = srcLayer->layout[(x + startX2) + ((y + startY2) << srcLayer->widthShift)];
                                dstLayer->layout[(x + startX1) + ((y + startY1) << dstLayer->widthShift)] = tile;
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
    int screenwidth16            = (currentScreen->pitch >> 4) - 1;
    byte *lineBuffer             = &gfxLineBuffer[currentScreen->clipBound_Y1];
    ScanlineInfo *tilePosPtr = &scanlines[currentScreen->clipBound_Y1];
    ushort *frameBufferPtr       = &currentScreen->frameBuffer[currentScreen->pitch * currentScreen->clipBound_Y1];

    int cy = currentScreen->clipBound_Y1;
    if (cy < currentScreen->clipBound_Y2) {
        do {
            int x              = tilePosPtr->position.x;
            int y              = tilePosPtr->position.y;
            int tileX          = tilePosPtr->position.x >> 16;
            ushort *palettePtr = fullPalette[*lineBuffer++];
            if (tileX >= 0) {
                if (tileX >= 16 * layer->width)
                    x = (tileX - 16 * layer->width) << 16;
            }
            else {
                x = (tileX + 16 * layer->width) << 16;
            }
            int cnt  = 16 - (x >> 16) & 0xF;
            int cntX = (x >> 16) & 0xF;
            int cntY = 16 * ((y >> 16) & 0xF);

            int tx                = (x >> 20);
            ushort *tileLayoutPtr = &layer->layout[tx + (y >> 20 << layer->widthShift)];
            if (*tileLayoutPtr >= 0xFFFF) {
                frameBufferPtr += cnt;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntX + cntY]; cnt; ++frameBufferPtr) {
                    --cnt;
                    if (*i)
                        *frameBufferPtr = palettePtr[*i];
                    ++i;
                }
            }

            for (int i = screenwidth16; i; frameBufferPtr += TILE_SIZE, --i) {
                ++tileLayoutPtr;
                ++tx;

                if (tx == layer->width) {
                    tx = 0;
                    tileLayoutPtr -= layer->width;
                }
                if (*tileLayoutPtr < 0xFFFFu) {
                    byte *tilesetData = &tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntY];
                    if (*tilesetData)
                        *frameBufferPtr = palettePtr[tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntY]];

                    byte index = tilesetData[1];
                    if (index)
                        frameBufferPtr[1] = palettePtr[index];

                    index = tilesetData[2];
                    if (index)
                        frameBufferPtr[2] = palettePtr[index];

                    index = tilesetData[3];
                    if (index)
                        frameBufferPtr[3] = palettePtr[index];

                    index = tilesetData[4];
                    if (index)
                        frameBufferPtr[4] = palettePtr[index];

                    index = tilesetData[5];
                    if (index)
                        frameBufferPtr[5] = palettePtr[index];

                    index = tilesetData[6];
                    if (index)
                        frameBufferPtr[6] = palettePtr[index];

                    index = tilesetData[7];
                    if (index)
                        frameBufferPtr[7] = palettePtr[index];

                    index = tilesetData[8];
                    if (index)
                        frameBufferPtr[8] = palettePtr[index];

                    index = tilesetData[9];
                    if (index)
                        frameBufferPtr[9] = palettePtr[index];

                    index = tilesetData[10];
                    if (index)
                        frameBufferPtr[10] = palettePtr[index];

                    index = tilesetData[11];
                    if (index)
                        frameBufferPtr[11] = palettePtr[index];

                    index = tilesetData[12];
                    if (index)
                        frameBufferPtr[12] = palettePtr[index];

                    index = tilesetData[13];
                    if (index)
                        frameBufferPtr[13] = palettePtr[index];

                    index = tilesetData[14];
                    if (index)
                        frameBufferPtr[14] = palettePtr[index];

                    index = tilesetData[15];
                    if (index)
                        frameBufferPtr[15] = palettePtr[index];
                }
            }
            ++tileLayoutPtr;

            if (tx + 1 == layer->width)
                tileLayoutPtr -= layer->width;

            if (*tileLayoutPtr >= 0xFFFFu) {
                frameBufferPtr += cntX;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntY]; cntX; ++frameBufferPtr) {
                    --cntX;
                    if (*i)
                        *frameBufferPtr = palettePtr[*i];
                    ++i;
                }
            }

            ++tilePosPtr;
        } while (++cy < currentScreen->clipBound_Y2);
    }
}
void DrawLayerVScroll(TileLayer *layer)
{
    int width          = 1 << layer->widthShift;
    int screenHeight16 = (currentScreen->height >> 4) - 1;
    int clipX1         = currentScreen->clipBound_X1;
    ushort* frameBuffer         = &currentScreen->frameBuffer[clipX1];
    ScanlineInfo *scanLines      = &scanlines[currentScreen->clipBound_X1];
    ushort *palettePtr     = fullPalette[gfxLineBuffer[0]];
    if (clipX1 < currentScreen->clipBound_X2) {
        do {
            int x = scanLines->position.x;
            int y = scanLines->position.y;
            if (y >= 0) {
                if (y >= TILE_SIZE * layer->height)
                    y -= (TILE_SIZE * layer->height) << 0x10;
            }
            else {
                y += (TILE_SIZE * layer->height) << 0x10;
            }

            int cnt  = 16 - (y >> 16) & 0xF;
            int cntX = (y >> 16) & 0xF;
            int cntY = 16 * ((x >> 16) & 0xF);

            ushort *layout = &layer->layout[(x >> 20) + ((y >> 20) << layer->widthShift)];
            if (*layout >= 0xFFFFu) {
                frameBuffer += currentScreen->pitch * cnt;
            }
            else {
                for (byte *i = &tilesetGFXData[16 * (cntX + 16 * (*layout & 0xFFF)) + cntY]; cnt; frameBuffer += currentScreen->pitch) {
                    --cnt;
                    if (*i)
                        *frameBuffer = palettePtr[*i];
                    i += 16;
                }
            }
            
            int tileCnt = screenHeight16;
            int tileY   = y >> 20;
            if (screenHeight16) {
                do {
                    layout += width;
                    ++tileY;

                    if (tileY == layer->height) {
                        tileY = 0;
                        layout -= layer->height << layer->widthShift;
                    }
                    if (*layout >= 0xFFFFu) {
                        frameBuffer += TILE_SIZE * currentScreen->pitch;
                    }
                    else {
                        byte *gfxPtr = &tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntY];
                        if (*gfxPtr)
                            *frameBuffer = palettePtr[tilesetGFXData[TILE_DATASIZE * (*layout & 0xFFF) + cntY]];
                        ushort *fb = &frameBuffer[currentScreen->pitch];

                        if (gfxPtr[16])
                            *fb = palettePtr[gfxPtr[16]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[32])
                            *fb = palettePtr[gfxPtr[32]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[48])
                            *fb = palettePtr[gfxPtr[48]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[64])
                            *fb = palettePtr[gfxPtr[64]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[80])
                            *fb = palettePtr[gfxPtr[80]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[96])
                            *fb = palettePtr[gfxPtr[96]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[112])
                            *fb = palettePtr[gfxPtr[112]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[128])
                            *fb = palettePtr[gfxPtr[128]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[144])
                            *fb = palettePtr[gfxPtr[144]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[160])
                            *fb = palettePtr[gfxPtr[160]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[176])
                            *fb = palettePtr[gfxPtr[176]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[192])
                            *fb = palettePtr[gfxPtr[192]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[208])
                            *fb = palettePtr[gfxPtr[208]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[224])
                            *fb = palettePtr[gfxPtr[224]];

                        fb = &fb[currentScreen->pitch];
                        if (gfxPtr[240])
                            *fb = palettePtr[gfxPtr[240]];

                        frameBuffer = &fb[currentScreen->pitch];
                    }
                    tileCnt--;
                } while (tileCnt);
            }
            layout = &layout[width];

            if (tileY + 1 == layer->height)
                layout -= layer->height << layer->widthShift;
            ushort tile = *layout;
            if (*layout >= 0xFFFF) {
                frameBuffer += currentScreen->pitch * cntX;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (tile & 0xFFF) + cntY]; cntX; frameBuffer += currentScreen->pitch) {
                    --cntX;
                    if (*i)
                        *frameBuffer = palettePtr[*i];
                    i += 0x10;
                }
            }
            ++scanLines;
            ++clipX1;
            frameBuffer -= currentScreen->pitch * currentScreen->height - 2;
        } while (clipX1 < currentScreen->clipBound_X2);
    }
}
void DrawLayerRotozoom(TileLayer *layer)
{
    ushort* layout        = layer->layout;
    int clipX1        = currentScreen->clipBound_X1;
    int clipY1        = currentScreen->clipBound_Y1;
    int countX        = currentScreen->clipBound_X2 - clipX1;
    byte *lineBuffer = &gfxLineBuffer[clipY1];
    ScanlineInfo* scanlinePtr = &scanlines[clipY1];
    clipY1        = currentScreen->clipBound_Y1;
    scanlinePtr   = &scanlines[clipY1];
    ushort *frameBuffer   = &currentScreen->frameBuffer[clipX1 + clipY1 * currentScreen->pitch];
    int width         = (16 << layer->widthShift) - 1;
    int height        = (16 << layer->heightShift) - 1;

    if (clipY1 < currentScreen->clipBound_Y2) {
        do {
            int defX       = scanlinePtr->deform.x;
            int defY       = scanlinePtr->deform.y;
            ushort *palettePtr = fullPalette[*lineBuffer];
            int posX       = scanlinePtr->position.x;
            ++lineBuffer;
            for (int posY = scanlinePtr->position.y; countX; posY += defY) {
                --countX;
                int x1   = posX >> 20;
                int y1   = posY >> 20;
                int x2   = (posX >> 8) & 0xF;
                int y2   = (posY >> 8) & 0xF;
                byte idx =
                    tilesetGFXData[0x10 * (y2 + 0x10 * (layout[((width >> 4) & x1) + (((height >> 4) & y1) << layer->widthShift)] & 0xFFF)) + x2];
                if (idx)
                    *frameBuffer = palettePtr[idx];
                posX += defX;
                ++frameBuffer;
            }

            ++clipY1;
            frameBuffer += currentScreen->pitch - countX;
            ++scanlinePtr;
        } while (clipY1 < currentScreen->clipBound_Y2);
    }
}
void DrawLayerBasic(TileLayer *layer) {
    int screenwidth16        = (currentScreen->pitch >> 4) - 1;
    byte *lineBuffer         = &gfxLineBuffer[currentScreen->clipBound_Y1];
    ScanlineInfo *tilePosPtr = &scanlines[currentScreen->clipBound_Y1];
    ushort *frameBufferPtr   = &currentScreen->frameBuffer[currentScreen->pitch * currentScreen->clipBound_Y1];

    int cy = currentScreen->clipBound_Y1;
    if (cy < currentScreen->clipBound_Y2) {
        do {
            int x              = tilePosPtr->position.x;
            int y              = tilePosPtr->position.y;
            int tileX          = tilePosPtr->position.x >> 16;
            ushort *palettePtr = fullPalette[*lineBuffer++];
            if (tileX >= 0) {
                if (tileX >= 16 * layer->width)
                    x = (tileX - 16 * layer->width) << 16;
            }
            else {
                x = (tileX + 16 * layer->width) << 16;
            }
            int cnt  = 16 - (x >> 16) & 0xF;
            int cntX = (x >> 16) & 0xF;
            int cntY = 16 * ((y >> 16) & 0xF);

            int tx                = (x >> 20);
            ushort *tileLayoutPtr = &layer->layout[tx + (y >> 20 << layer->widthShift)];
            if (*tileLayoutPtr >= 0xFFFF) {
                frameBufferPtr += cnt;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntX + cntY]; cnt; ++frameBufferPtr) {
                    --cnt;
                    if (*i)
                        *frameBufferPtr = palettePtr[*i];
                    ++i;
                }
            }

            for (int i = screenwidth16; i; frameBufferPtr += TILE_SIZE, --i) {
                ++tileLayoutPtr;
                ++tx;

                if (tx == layer->width) {
                    tx = 0;
                    tileLayoutPtr -= layer->width;
                }
                if (*tileLayoutPtr < 0xFFFFu) {
                    byte *tilesetData = &tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntY];
                    if (*tilesetData)
                        *frameBufferPtr = palettePtr[tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntY]];

                    byte index = tilesetData[1];
                    if (index)
                        frameBufferPtr[1] = palettePtr[index];

                    index = tilesetData[2];
                    if (index)
                        frameBufferPtr[2] = palettePtr[index];

                    index = tilesetData[3];
                    if (index)
                        frameBufferPtr[3] = palettePtr[index];

                    index = tilesetData[4];
                    if (index)
                        frameBufferPtr[4] = palettePtr[index];

                    index = tilesetData[5];
                    if (index)
                        frameBufferPtr[5] = palettePtr[index];

                    index = tilesetData[6];
                    if (index)
                        frameBufferPtr[6] = palettePtr[index];

                    index = tilesetData[7];
                    if (index)
                        frameBufferPtr[7] = palettePtr[index];

                    index = tilesetData[8];
                    if (index)
                        frameBufferPtr[8] = palettePtr[index];

                    index = tilesetData[9];
                    if (index)
                        frameBufferPtr[9] = palettePtr[index];

                    index = tilesetData[10];
                    if (index)
                        frameBufferPtr[10] = palettePtr[index];

                    index = tilesetData[11];
                    if (index)
                        frameBufferPtr[11] = palettePtr[index];

                    index = tilesetData[12];
                    if (index)
                        frameBufferPtr[12] = palettePtr[index];

                    index = tilesetData[13];
                    if (index)
                        frameBufferPtr[13] = palettePtr[index];

                    index = tilesetData[14];
                    if (index)
                        frameBufferPtr[14] = palettePtr[index];

                    index = tilesetData[15];
                    if (index)
                        frameBufferPtr[15] = palettePtr[index];
                }
            }
            ++tileLayoutPtr;

            if (tx + 1 == layer->width)
                tileLayoutPtr -= layer->width;

            if (*tileLayoutPtr >= 0xFFFFu) {
                frameBufferPtr += cntX;
            }
            else {
                for (byte *i = &tilesetGFXData[TILE_DATASIZE * (*tileLayoutPtr & 0xFFF) + cntY]; cntX; ++frameBufferPtr) {
                    --cntX;
                    if (*i)
                        *frameBufferPtr = palettePtr[*i];
                    ++i;
                }
            }

            ++tilePosPtr;
        } while (++cy < currentScreen->clipBound_Y2);
    }
}