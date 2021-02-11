#include "RetroEngine.hpp"

byte tilesetGFXData[TILESET_SIZE];

TileLayer tileLayers[LAYER_COUNT];
CollisionMasks collisionMasks[2];

int activeSceneList   = 0;
int sceneListPosition = 0;

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

    if (StrComp(currentSceneFolder, sceneInfo.listData[sceneListPosition].folder)) {
        // Reload
        ClearUnusedStorage(DATASET_STG);
        sceneInfo.filter = sceneInfo.listData[sceneInfo.listPos].modeFilter;
        return;
    }

    // Unload Model data
    for (int m = 0; m < MODEL_MAX; ++m) {
        if (modelList[m].scope != SCOPE_GLOBAL) {
            modelList[m].scope = SCOPE_NONE;
            memset(modelList[m].hash, 0, 4 * sizeof(int));
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
            memset(spriteAnimationList[s].hash, 0, 4 * sizeof(int));
        }
    }

    // Unload surfaces
    for (int s = 0; s < SURFACE_MAX; ++s) {
        if (gfxSurface[s].scope != SCOPE_GLOBAL) {
            gfxSurface[s].scope = SCOPE_NONE;
            memset(gfxSurface[s].hash, 0, 4 * sizeof(int));
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

    SceneListEntry *sceneEntry = &sceneInfo.listData[sceneListPosition];
    StrCopy(currentSceneFolder, sceneEntry->folder);
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
            // LoadWAV(buffer, maxConcurrentPlays, SCOPE_STAGE);
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

    SceneListEntry *sceneEntry = &sceneInfo.listData[sceneListPosition];
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
                SetEditableVar(ATTRIBUTE_U8, "filter", objID, offsetof(Entity, filter));
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
                        case ATTRIBUTE_U8:
                        case ATTRIBUTE_S8:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(byte));
                            }
                            else {
                                ReadInt8(&info);
                            }
                            break;
                        case ATTRIBUTE_U16:
                        case ATTRIBUTE_S16:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(short));
                            }
                            else { 
                                ReadInt16(&info);
                            }
                            break;
                        case ATTRIBUTE_U32:
                        case ATTRIBUTE_S32:
                        case ATTRIBUTE_VAR: 
                        case ATTRIBUTE_BOOL: 
                        case ATTRIBUTE_COLOUR:
                            if (varList[v].active) {
                                byte *ptr = (byte *)entity;
                                ReadBytes(&info, &ptr[varList[v].offset], sizeof(int));
                            }
                            else {
                                ReadInt32(&info);
                            }
                            break;
                        case ATTRIBUTE_STRING:
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
                        case ATTRIBUTE_VECTOR2:
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
                        case ATTRIBUTE_UNKNOWN:
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

void ProcessParallaxAutoScroll() {}
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