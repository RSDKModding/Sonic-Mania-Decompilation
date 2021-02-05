#include "RetroEngine.hpp"

byte tilesetGFXData[TILESET_SIZE];

SceneListInfo sceneLists[0x40];
SceneListEntry sceneListEntries[0x400];
int sceneListCount = 0;
int sceneCount = 0;

SceneInfo sceneInfo;