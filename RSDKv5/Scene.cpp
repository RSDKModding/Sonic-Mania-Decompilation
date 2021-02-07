#include "RetroEngine.hpp"

byte tilesetGFXData[TILESET_SIZE];

SceneListInfo sceneLists[0x40];
SceneListEntry sceneListEntries[0x400];
int sceneListCount = 0;
int sceneCount     = 0;

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

    // Clear Entity List

    if (StrComp(currentSceneFolder, sceneListEntries[sceneListPosition].folder)) {
        // Reload
        return;
    }

    // Unload Model data

    // Unload animations

    // Unload surfaces
    for (int s = 0; s < SURFACE_MAX; ++s) {
        gfxSurface[s].scope = 0;
        memset(gfxSurface[s].hash, 0, 4 * sizeof(int));
    }

    // Unload stage SFX

    for (int s = 0; s < sceneInfo.classCount; ++s) {
        // Clear stage objects
    }

    SceneListEntry *sceneEntry = &sceneListEntries[sceneListPosition];
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
            if (obj->type) {
                AllocateStorage(obj->objectSize, obj->type, DATASET_STG, true);
                LoadStaticObject((byte *)obj->type, obj->hash, 2);
            }
        }

        for (int i = 0; i < PALETTE_COUNT; ++i) {
            ushort activeRows = ReadInt16(&info);
            for (int r = 0; r < 0x10; ++r) {
                if ((activeRows >> r & 1)) {
                    for (int c = 0; c < 0x10; ++c) {
                        byte red                     = ReadInt8(&info);
                        byte green                   = ReadInt8(&info);
                        byte blue                    = ReadInt8(&info);
                        fullPalette[i][(r << 4) + c] = bIndexes[buffer[2]] | gIndexes[buffer[1]] | rIndexes[buffer[0]];
                    }
                }
                else {
                    for (int c = 0; c < 0x10; ++c) {
                        fullPalette[i][(r << 4) + c] = bIndexes[0x00] | gIndexes[0x00] | rIndexes[0x00];
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
void LoadSceneFile() {}
void LoadTileConfig(char *filepath) {}
void LoadStageGIF(char *filepath)
{
    Image tileset;
    MEM_ZERO(tileset);

    AllocateStorage(sizeof(GifDecoder), &tileset.decoder, DATASET_TMP, true);

    if (LoadGIF(&tileset, filepath, true) && tileset.width == TILE_SIZE && tileset.height <= 0x4000) {
        tileset.dataPtr = tilesetGFXData;
        LoadGIF(&tileset, 0, false);
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
        sceneInfo.milliseconds = sceneInfo.timeCounter / 60;
    }
}