#include "SonicMania.h"
#include <time.h>

ObjectBSS_Setup *BSS_Setup;

void BSS_Setup_Update(void)
{
    RSDK_THIS(BSS_Setup);

    StateMachine_Run(entity->state);
    RSDK_screens->position.x = 0x100 - RSDK_screens->centerX;

    if (entity->palettePage) {
        RSDK.CopyPalette(2, 16 * entity->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(1, 16 * entity->paletteLine, 0, 144, 16);
    }
    else {
        RSDK.CopyPalette(1, 16 * entity->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(2, 16 * entity->paletteLine, 0, 144, 16);
    }

#if RETRO_USE_PLUS
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    if (!param->field_59 && !entity->pauseFlag && globals->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM->zoneTimes[28];
#endif
}

void BSS_Setup_LateUpdate(void) {}

void BSS_Setup_StaticUpdate(void) {}

void BSS_Setup_Draw(void)
{
    RSDK_THIS(BSS_Setup);
    TileLayer *globe  = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    entity->inkEffect = INK_NONE;
    if (globe->drawLayer[0] == DRAWLAYER_COUNT)
        RSDK.DrawSprite(&entity->globeSpinData, NULL, false);

    Vector2 drawPos;
    entity->inkEffect = INK_BLEND;
    drawPos.x         = entity->position.x;
    drawPos.y         = 0x9E0000;
    RSDK.DrawSprite(&entity->shadowData, &drawPos, false);
}

void BSS_Setup_Create(void *data)
{
    RSDK_THIS(BSS_Setup);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active          = ACTIVE_BOUNDS;
        entity->visible         = true;
        entity->drawOrder       = 2;
        entity->drawFX          = FX_FLIP;
        entity->position.x      = 0x1000000;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x800000;
        entity->speedupInterval = 1800;
        entity->pauseFlag       = false;
        RSDK.SetSpriteAnimation(BSS_Setup->globeMappings, 0, &entity->globeSpinData, true, 0);
        RSDK.SetSpriteAnimation(BSS_Setup->globeMappings, 1, &entity->shadowData, true, 0);
        BSS_Setup_GetStartupInfo();
        entity->state = BSS_Setup_State_HandleStage;
    }
}

void BSS_Setup_StageLoad(void)
{
    BSS_Setup->globeMappings  = RSDK.LoadSpriteAnimation("SpecialBS/Globe.bin", SCOPE_STAGE);
    BSS_Setup->bgLayer        = RSDK.GetSceneLayerID("Background");
    BSS_Setup->globeLayer     = RSDK.GetSceneLayerID("Globe");
    BSS_Setup->frustum1Layer  = RSDK.GetSceneLayerID("Frustum 1");
    BSS_Setup->frustum2Layer  = RSDK.GetSceneLayerID("Frustum 2");
    BSS_Setup->playFieldLayer = RSDK.GetSceneLayerID("Playfield");
    BSS_Setup->ringCountLayer = RSDK.GetSceneLayerID("Ring Count");
    BSS_Setup_SetupFrustum();
    BSS_Setup->ringCount = 0;
    TileLayer *playField = RSDK.GetSceneLayer(BSS_Setup->playFieldLayer);

    memset(BSS_Setup->playField, 0, BSS_PLAYFIELD_W * BSS_PLAYFIELD_H * sizeof(ushort));
    memset(BSS_Setup->playField2, 0, BSS_PLAYFIELD_W * BSS_PLAYFIELD_H * sizeof(ushort));
    memset(BSS_Setup->playField3, 0, BSS_PLAYFIELD_W * BSS_PLAYFIELD_H * sizeof(ushort));

    if (playField->width <= BSS_PLAYFIELD_W) {
        for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
            for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, x, y);

                int playFieldPos                   = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, x, y) & 0x3FF) == BSS_RING)
                    ++BSS_Setup->ringCount;
            }
        }
    }
    else {
        int max = (playField->width >> 4) * ((playField->height - 32) >> 4);
        int key = (int)time(0);

        BSS_Setup->flags[0] = RSDK.Random(0, max, &key);
        BSS_Setup->flags[1] = RSDK.Random(0, max, &key);
        BSS_Setup->flags[2] = RSDK.Random(0, max, &key);
        BSS_Setup->flags[3] = RSDK.Random(0, max, &key);

        BSS_Palette->startColourID = 16 * (BSS_Setup->flags[1] & 0x0F);

        // Top Left Quadrant
        for (int y = 0, py = 0; y < BSS_PLAYFIELD_H / 2; ++y, ++py) {
            for (int x = 0, px = 0; x < BSS_PLAYFIELD_W / 2; ++x, ++px) {
                int tx = px + (0x10 * (BSS_Setup->flags[0] & 0x0F));
                int ty = py + BSS_PLAYFIELD_H + (BSS_Setup->flags[0] & 0xF0);

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                   = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        // Top Right Quadrant
        for (int y = 0, py = 0; y < BSS_PLAYFIELD_H / 2; ++y, ++py) {
            for (int x = BSS_PLAYFIELD_W / 2, px = 0; x < BSS_PLAYFIELD_W; ++x, ++px) {
                int tx = 0x10 * (BSS_Setup->flags[1] & 0x0F) - px + 0x0F;
                int ty = py + BSS_PLAYFIELD_H + (BSS_Setup->flags[1] & 0xF0);

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                   = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        // Bottom Left Quadrant
        for (int y = BSS_PLAYFIELD_H / 2, py = 0; y < BSS_PLAYFIELD_H; ++y, ++py) {
            for (int x = 0, px = 0; x < BSS_PLAYFIELD_W / 2; ++x, ++px) {
                int tx = px + (0x10 * (BSS_Setup->flags[2] & 0x0F));
                int ty = (BSS_Setup->flags[2] & 0xF0) - py + 0x2F;

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                   = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        // Bottom Right Quadrant
        for (int y = BSS_PLAYFIELD_H / 2, py = 0; y < BSS_PLAYFIELD_H; ++y, ++py) {
            for (int x = BSS_PLAYFIELD_W / 2, px = 0; x < BSS_PLAYFIELD_W; ++x, ++px) {
                int tx = 0x10 * (BSS_Setup->flags[3] & 0x0F) - px + 0x0F;
                int ty = (BSS_Setup->flags[3] & 0xF0) - py + 0x2F;

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                   = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        BSS_Setup->playField[(16 * BSS_PLAYFIELD_H) + 3] = BSS_SPAWN_RIGHT;
    }

    RSDK.ResetEntitySlot(SLOT_BSS_SETUP, BSS_Setup->objectID, NULL);
    BSS_Setup_SetupPalette();
    globals->specialCleared   = false;
    BSS_Setup->sfxBlueSphere  = RSDK.GetSFX("Special/BlueSphere.wav");
    BSS_Setup->sfxSSExit      = RSDK.GetSFX("Special/SSExit.wav");
    BSS_Setup->sfxSSJettison  = RSDK.GetSFX("Special/SSJettison.wav");
    BSS_Setup->sfxEmerald     = RSDK.GetSFX("Special/Emerald.wav");
    BSS_Setup->sfxEvent       = RSDK.GetSFX("Special/Event.wav");
    BSS_Setup->sfxBumper      = RSDK.GetSFX("Stage/Bumper.wav");
    BSS_Setup->sfxSpring      = RSDK.GetSFX("Global/Spring.wav");
    BSS_Setup->sfxRing        = RSDK.GetSFX("Global/Ring.wav");
    BSS_Setup->sfxLoseRings   = RSDK.GetSFX("Global/LoseRings.wav");
    BSS_Setup->sfxMedal       = RSDK.GetSFX("Special/Medal.wav");
    BSS_Setup->sfxMedalCaught = RSDK.GetSFX("Special/MedalCaught.wav");
    BSS_Setup->sfxTeleport    = RSDK.GetSFX("Global/Teleport.wav");

    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    if (param->field_59 == 1) {
        TextInfo info;
        Localization_GetString(&info, STR_RPC_PLAYING);
        API_SetRichPresence(PRESENCE_GENERIC, &info);
    }
}

int BSS_Setup_GetStageID(void)
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.SetScene("Blue Spheres", "");
    int id                  = (pos - RSDK_sceneInfo->listPos) % 32;
    RSDK_sceneInfo->listPos = pos;
    return id;
}

void BSS_Setup_SetupPalette(void)
{
    for (int i = 0; i < 0x10; ++i) RSDK.SetPaletteEntry(0, 0x80 + i, RSDK.GetPaletteEntry(1, BSS_Palette->startColourID));
    for (int i = 0; i < 0x10; ++i) RSDK.SetPaletteEntry(0, 0x90 + i, RSDK.GetPaletteEntry(1, BSS_Palette->startColourID + 1));

    for (int i = 0; i < 3; ++i) RSDK.SetPaletteEntry(0, 0xA0 + i, RSDK.GetPaletteEntry(1, i + BSS_Palette->startColourID + 2));
    for (int i = 0; i < 4; ++i) RSDK.SetPaletteEntry(0, 0xC0 + i, RSDK.GetPaletteEntry(1, i + BSS_Palette->startColourID + 8));

    for (int i = 0; i < 0x100; i += 0x10) {
        RSDK.CopyPalette(0, 0x80, 1, i, 0x10);
        RSDK.RotatePalette(0, 0x80, 0x9F, true);
    }
    for (int i = 0; i < 0x100; i += 0x10) {
        RSDK.CopyPalette(0, 0x80, 2, i, 0x10);
        RSDK.RotatePalette(0, 0x80, 0x9F, true);
    }
}

void BSS_Setup_SetupFrustum(void)
{
    int offset = 0;
    int count  = 0;
    for (int f = 0; f < 2; ++f) {
        int frustumID = 0;
        if (f)
            frustumID = BSS_Setup->frustum2Layer;
        else
            frustumID = BSS_Setup->frustum1Layer;
        TileLayer *frustum = RSDK.GetSceneLayer(frustumID);

        count     = offset;
        int lastX = 0;
        int lastY = 0;

        for (int y = 0; y < frustum->height; ++y) {
            for (int x = 0; x < frustum->width; ++x) {
                ushort id = (RSDK.GetTileInfo(frustumID, x, y) & 0x3FF);
                if (id == BSS_SPHERE_BLUE || id == BSS_SPAWN_UP) {
                    BSS_Setup->offsetTable[count].x = x;
                    BSS_Setup->offsetTable[count].y = y;
                    count++;

                    if (id == BSS_SPAWN_UP) {
                        lastX = x;
                        lastY = y;
                    }
                }
            }
        }

        BSS_Setup->frustumCount[f]  = count - offset;
        BSS_Setup->frustumOffset[f] = offset;
        Vector2 *offsetTable        = &BSS_Setup->offsetTable[BSS_Setup->frustumOffset[f]];
        int *offsetRadiusTable      = &BSS_Setup->offsetRadiusTable[BSS_Setup->frustumOffset[f]];

        for (int i = 0; i < BSS_Setup->frustumCount[f]; ++i) {
            offsetTable[i].x -= lastX;
            offsetTable[i].y -= lastY;
            offsetRadiusTable[i] = offsetTable[i].x * offsetTable[i].x + offsetTable[i].y * offsetTable[i].y;
        }

        for (int o = 0; o < BSS_Setup->frustumCount[f]; ++o) {
            for (int i = BSS_Setup->frustumCount[f] - 1; i > o; --i) {
                int ox = offsetTable[i - 1].x;
                int oy = offsetTable[i - 1].y;
                int id = offsetRadiusTable[i - 1];
                if (offsetRadiusTable[i] > offsetRadiusTable[i - 1]) {
                    offsetTable[i - 1].x     = offsetTable[i].x;
                    offsetTable[i - 1].y     = offsetTable[i].y;
                    offsetRadiusTable[i - 1] = offsetRadiusTable[i];
                    offsetTable[i].y         = oy;
                    offsetTable[i].x         = ox;
                    offsetRadiusTable[i]     = id;
                }
            }
        }

        offset += BSS_Setup->frustumCount[f];
    }

    for (int i = RESERVE_ENTITY_COUNT; i < RESERVE_ENTITY_COUNT + 0x60; ++i) {
        RSDK.ResetEntitySlot(i, BSS_Collectable->objectID, NULL);
    }
}

void BSS_Setup_CollectRing(void)
{
    RSDK_THIS(BSS_Setup);
    ++BSS_Setup->rings;

    if (BSS_Setup->ringCount > 0) {
        BSS_Setup->ringCount--;
        if (!BSS_Setup->ringCount) {
            RSDK.CreateEntity(BSS_Message->objectID, intToVoid(1), entity->position.x, entity->position.y);
            RSDK.PlaySFX(BSS_Setup->sfxEvent, 0, 255);
        }
    }

    if (BSS_Setup->ringPan) {
        int channel = RSDK.PlaySFX(BSS_Setup->sfxRing, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        BSS_Setup->ringPan = 0;
    }
    else {
        int channel = RSDK.PlaySFX(BSS_Setup->sfxRing, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        BSS_Setup->ringPan = 1;
    }
}

void BSS_Setup_GetStartupInfo(void)
{
    RSDK_THIS(BSS_Setup);
    BSS_Setup->sphereCount     = 0;
    BSS_Setup->pinkSphereCount = 0;

    for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
            int playFieldPos = (x * BSS_PLAYFIELD_W) + y;
            switch (BSS_Setup->playField[playFieldPos]) {
                case BSS_SPHERE_BLUE:
                case BSS_SPHERE_GREEN: ++BSS_Setup->sphereCount; break;
                case BSS_SPHERE_PINK: ++BSS_Setup->pinkSphereCount; break;
                case BSS_SPAWN_UP:
                    entity->angle                      = 0x00;
                    entity->playerPos.x                = x;
                    entity->playerPos.y                = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                case BSS_SPAWN_RIGHT:
                    entity->angle                      = 0xC0;
                    entity->playerPos.x                = x;
                    entity->playerPos.y                = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                case BSS_SPAWN_DOWN:
                    entity->angle                      = 0x80;
                    entity->playerPos.x                = x;
                    entity->playerPos.y                = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                case BSS_SPAWN_LEFT:
                    entity->angle                      = 0x40;
                    entity->playerPos.x                = x;
                    entity->playerPos.y                = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                default: break;
            }
        }
    }

    RSDK.GetSceneLayer(BSS_Setup->bgLayer)->scrollInfo[0].scrollPos = entity->angle << 18;
}

void BSS_Setup_Finished(void)
{
    RSDK_THIS(BSS_Setup);
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;
    entity->globeTimer += entity->globeSpeed;
    if (entity->globeSpeed <= 0 && entity->globeTimer < 0) {
        entity->palettePage ^= 1;
        entity->globeTimer += 0x100;
        entity->playerPos.x -= RSDK.Sin256(entity->angle) >> 8;
        entity->playerPos.y += RSDK.Cos256(entity->angle) >> 8;
    }
    else if (entity->globeTimer >= 0x100) {
        entity->palettePage ^= 1;
        entity->globeTimer -= 0x100;
        entity->playerPos.x += RSDK.Sin256(entity->angle) >> 8;
        entity->playerPos.y -= RSDK.Cos256(entity->angle) >> 8;
    }
    entity->playerPos.x &= 0x1F;
    entity->playerPos.y &= 0x1F;

    entity->paletteLine = (entity->globeTimer >> 4) & 0xF;
    TileLayer *bg       = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollPos += entity->globeSpeed << 14;
    entity->pauseFlag = true;
    BSS_Setup_HandleCollectableMovement();
    BSS_Setup_LaunchSpheres();

    if (++entity->spinTimer == 128) {
        entity->spinTimer  = 0;
        entity->maxSpeed   = 8;
        entity->globeSpeed = 8;
        BSS_Setup_StageFinishClear();
        EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        player->stateInput       = false;
        player->jumpPress        = false;
        entity->state            = BSS_Setup_State_FinishWalk;
    }
}

void BSS_Setup_HandleSteppedObjects(void)
{
    RSDK_THIS(BSS_Setup);
    if (entity->globeTimer < 32)
        entity->bumperFlag = false;
    if (entity->globeTimer > 224)
        entity->bumperFlag = false;

    int fieldPos = entity->playerPos.y + (BSS_PLAYFIELD_H * entity->playerPos.x);
    switch (BSS_Setup->playField[fieldPos]) {
        case BSS_SPHERE_BLUE:
            if (entity->globeTimer < 128) {
                entity->lastSpherePos.x = entity->playerPos.x;
                entity->lastSpherePos.y = entity->playerPos.y;
                BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;
                if (!entity->ringLoopFlag) {
                    CREATE_ENTITY(BSS_Collected, intToVoid(1), entity->playerPos.x, entity->playerPos.y);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }

                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    entity->state          = BSS_Setup_Finished;
                    RSDK.PlaySFX(BSS_Setup->sfxSSJettison, 0, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySFX(BSS_Setup->sfxBlueSphere, 0, 255);
                }
            }
            break;
        case BSS_SPHERE_RED:
            if (entity->state != BSS_Setup_State_Exit && entity->globeTimer < 32) {
                entity->state      = BSS_Setup_State_Exit;
                entity->pauseFlag  = true;
                entity->spinTimer  = 0;
                entity->globeTimer = 0;
                RSDK.PlaySFX(BSS_Setup->sfxSSExit, 0, 255);
                Music_FadeOut(0.0125);
            }
            break;
        case BSS_SPHERE_BUMPER:
            if (!entity->bumperFlag && entity->globeTimer < 112) {
                if (entity->globeTimer > 16) {
                    if (entity->globeSpeed < 0) {
                        entity->bumperFlag  = true;
                        entity->globeSpeed  = -entity->globeSpeed;
                        entity->unknownFlag = false;
                        RSDK.PlaySFX(BSS_Setup->sfxBumper, 0, 255);
                    }
                }
                else if (!entity->spinState) {
                    if (entity->globeSpeed < 0) {
                        entity->globeTimer  = 16;
                        entity->bumperFlag  = true;
                        entity->globeSpeed  = -entity->globeSpeed;
                        entity->unknownFlag = false;
                        RSDK.PlaySFX(BSS_Setup->sfxBumper, 0, 255);
                    }
                }
            }
            break;
        case BSS_SPHERE_YELLOW:
            if (entity->globeTimer < 128) {
                EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
                player->velocity.y       = -0x180000;
                player->onGround         = 0;
                RSDK.SetSpriteAnimation(player->spriteIndex, 3, &player->playerAnimator, 0, 0);
                BSS_Player->field_6 = 4;
                entity->globeSpeed *= 2;
                entity->spinState     = 0;
                entity->globeSpeedInc = 4;
                RSDK.PlaySFX(BSS_Setup->sfxSpring, 0, 255);
            }
            break;
        case BSS_SPHERE_GREEN:
            if (entity->globeTimer > 128) {
                CREATE_ENTITY(BSS_Collected, intToVoid(3), entity->playerPos.x, entity->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySFX(BSS_Setup->sfxBlueSphere, 0, 255);
            }
            break;
        case BSS_SPHERE_PINK:
            if (entity->state != BSS_Setup_State_PinkSphereWarp && entity->globeTimer < 64) {
                entity->state      = BSS_Setup_State_PinkSphereWarp;
                entity->spinTimer  = 0;
                entity->globeTimer = 0;
                RSDK.PlaySFX(BSS_Setup->sfxTeleport, 0, 255);
                EntityFXFade *fade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
                fade->speedIn      = 32;
                fade->speedOut     = 32;
                fade->wait         = 48;
            }
            break;
        case BSS_RING:
            if (entity->globeTimer < 128) {
                CREATE_ENTITY(BSS_Collected, 0, entity->playerPos.x, entity->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;
        default: break;
    }

    int posX      = (entity->playerPos.x + (RSDK.Sin256(entity->angle) >> 8)) & 0x1F;
    int posY      = (entity->playerPos.y - (RSDK.Cos256(entity->angle) >> 8)) & 0x1F;
    fieldPos      = posY + (BSS_PLAYFIELD_H * posX);
    int fieldData = BSS_Setup->playField[fieldPos];
    switch (fieldData) {
        case BSS_SPHERE_BLUE:
            if (entity->globeTimer > 128) {
                entity->lastSpherePos.x = posX;
                entity->lastSpherePos.y = posY;
                BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;
                if (!entity->ringLoopFlag) {
                    CREATE_ENTITY(BSS_Collected, intToVoid(1), posX, posY);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }
                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    RSDK.PlaySFX(BSS_Setup->sfxMedal, 0, 255);
                    entity->state = BSS_Setup_Finished;
                    RSDK.PlaySFX(BSS_Setup->sfxSSJettison, 0, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySFX(BSS_Setup->sfxBlueSphere, 0, 255);
                }
            }
            break;
        case BSS_SPHERE_RED:
            if (entity->state != BSS_Setup_State_Exit && entity->globeTimer > 224) {
                entity->palettePage ^= 1;
                entity->state      = BSS_Setup_State_Exit;
                entity->spinTimer  = 0;
                entity->globeTimer = 0;
                entity->playerPos.x += RSDK.Sin256(entity->angle) >> 8;
                entity->playerPos.x &= 0x1F;
                entity->playerPos.y -= RSDK.Cos256(entity->angle) >> 8;
                entity->playerPos.y &= 0x1F;
                RSDK.PlaySFX(BSS_Setup->sfxSSExit, 0, 255);
                Music_FadeOut(0.0125);
            }
            break;
        case BSS_SPHERE_BUMPER:
            if (!entity->bumperFlag && entity->globeTimer > 144) {
                if (entity->globeTimer >= 240) {
                    if (!entity->spinState) {
                        if (entity->globeSpeed > 0) {
                            entity->globeTimer  = 240;
                            entity->bumperFlag  = true;
                            entity->globeSpeed  = -entity->globeSpeed;
                            entity->unknownFlag = true;
                            RSDK.PlaySFX(BSS_Setup->sfxBumper, 0, 255);
                        }
                    }
                }
                else {
                    if (entity->globeSpeed > 0) {
                        entity->bumperFlag  = true;
                        entity->globeSpeed  = -entity->globeSpeed;
                        entity->unknownFlag = true;
                        RSDK.PlaySFX(BSS_Setup->sfxBumper, 0, 255);
                    }
                }
            }
            break;
        case BSS_SPHERE_YELLOW:
            if (entity->globeTimer > 128) {
                EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
                player->velocity.y       = -0x180000;
                player->onGround         = 0;
                RSDK.SetSpriteAnimation(player->spriteIndex, 3, &player->playerAnimator, 0, 0);
                BSS_Player->field_6 = 4;
                entity->globeSpeed *= 2;
                entity->spinState     = 0;
                entity->globeSpeedInc = 4;
                RSDK.PlaySFX(BSS_Setup->sfxSpring, 0, 255);
            }
            break;
        case BSS_SPHERE_GREEN:
            if (entity->globeTimer > 128) {
                CREATE_ENTITY(BSS_Collected, intToVoid(3), posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySFX(BSS_Setup->sfxBlueSphere, 0, 255);
            }
            break;
        case BSS_RING:
            if (entity->globeTimer > 128) {
                CREATE_ENTITY(BSS_Collected, 0, posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;
        case BSS_EMERALD_CHAOS:
        case BSS_EMERALD_SUPER:
        case BSS_MEDAL_SILVER:
        case BSS_MEDAL_GOLD:
            if (entity->globeTimer > 240) {
                EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
                if (!param->field_59 && globals->gameMode < MODE_TIMEATTACK) {
                    int pos = BSS_Setup_GetStageID();
                    if (pos >= 0) {
                        EntityGameProgress *progress = GameProgress_GetGameProgress();
                        if (progress) {
                            byte medal = fieldData == BSS_MEDAL_SILVER ? 1 : fieldData == BSS_MEDAL_GOLD ? 2 : 0;

                            if (medal)
                                GameProgress_GiveMedal(pos, medal);

                            if (progress->allGoldMedals && progress->goldMedalCount == 31)
                                API_UnlockAchievement("ACH_GOLD_MEDAL");
                            if (progress->allSilverMedals && progress->silverMedalCount == 31)
                                API_UnlockAchievement("ACH_SILVER_MEDAL");
                        }
                    }
                }

                entity->palettePage ^= 1;
                entity->state      = BSS_Setup_State_Exit;
                entity->spinTimer  = 0;
                entity->globeTimer = 0;
                entity->playerPos.x += RSDK.Sin256(entity->angle) >> 8;
                entity->playerPos.x &= 0x1F;
                entity->playerPos.y -= RSDK.Cos256(entity->angle) >> 8;
                entity->playerPos.y &= 0x1F;
                globals->specialCleared = true;
                RSDK.PlaySFX(BSS_Setup->sfxSSExit, 0, 255);
            }
            break;
        default: break;
    }
}

void BSS_Setup_HandleCollectableMovement(void)
{
    RSDK_THIS(BSS_Setup);
    entity->offsetDir = entity->angle >> 6;
    int off           = BSS_Setup->frustumOffset[(entity->angle & 0x3F) != 0];
    int id            = BSS_Setup->frustumCount[(entity->angle & 0x3F) != 0];
    Vector2 *offset   = &BSS_Setup->offsetTable[off];

    int slot = RESERVE_ENTITY_COUNT;
    while (id > 0) {
        switch (entity->offsetDir) {
            case FLIP_NONE:
                entity->offset.x = offset->x;
                entity->offset.y = offset->y;
                break;
            case FLIP_X:
                entity->offset.x = -offset->y;
                entity->offset.y = offset->x;
                break;
            case FLIP_Y:
                entity->offset.x = offset->x;
                entity->offset.y = -offset->y;
                break;
            case FLIP_XY:
                entity->offset.x = offset->y;
                entity->offset.y = offset->x;
                break;
            default: break;
        }

        ushort tile = BSS_Setup->playField[((entity->offset.y + entity->playerPos.y) & 0x1F)
                                           + (BSS_PLAYFIELD_H * ((entity->offset.x + entity->playerPos.x) & 0x1F))];
        if (tile) {
            EntityBSS_Collectable *collectable = (EntityBSS_Collectable *)RSDK.GetEntityByID(slot);
            int x = (entity->offset.x * RSDK.Cos256(entity->angle) + entity->offset.y * RSDK.Sin256(entity->angle)) >> 4;
            int y = (entity->offset.y * RSDK.Cos256(entity->angle) - entity->offset.x * RSDK.Sin256(entity->angle)) >> 4;
            y     = -(y + entity->paletteLine - 16);
            if (y < 0) {
                collectable->objectID = TYPE_BLANK;
            }
            else {
                collectable->objectID = BSS_Collectable->objectID;
                collectable->type     = tile & 0x3FF;
                if (y < 112) {
                    entity->xMultiplier  = BSS_Setup->xMultiplierTable[y];
                    entity->divisor      = BSS_Setup->divisorTable[y];
                    collectable->frameID = BSS_Setup->frameTable[y];
                    collectable->frameID -= abs(x) >> 5;
                    if (collectable->frameID < 0)
                        collectable->frameID = 0;

                    int finalX = entity->xMultiplier * x;
                    int distX  = finalX * finalX >> 16;

                    int worldX = 0;
                    if (finalX <= 0)
                        worldX = (distX + finalX) >> 4;
                    else
                        worldX = (finalX - distX) >> 4;

                    collectable->position.x = (worldX + RSDK_screens->centerX) << 16;
                    collectable->position.y = (BSS_Setup->screenYTable[y] + worldX * worldX / entity->divisor) << 16;
                    ++slot;
                }
            }
        }
        id--;
        ++offset;
    }

    while (slot < RESERVE_ENTITY_COUNT + 0x80) {
        Entity *entity   = RSDK.GetEntityByID(slot++);
        entity->objectID = TYPE_BLANK;
    }
}

void BSS_Setup_State_FinishWalk(void)
{
    RSDK_THIS(BSS_Setup);
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;
    entity->globeTimer += entity->globeSpeed;
    if (++entity->spinTimer == 120)
        RSDK.PlaySFX(BSS_Setup->sfxMedalCaught, 0, 255);
    BSS_Setup_HandleSteppedObjects();

    if (entity->globeSpeed <= 0 && entity->globeTimer < 0) {
        entity->palettePage ^= 1;
        entity->globeTimer += 256;
        entity->playerPos.x -= RSDK.Sin256(entity->angle) >> 8;
        entity->playerPos.y += RSDK.Cos256(entity->angle) >> 8;
    }
    else if (entity->globeTimer >= 0x100) {
        entity->palettePage ^= 1;
        entity->globeTimer -= 256;
        entity->playerPos.x += RSDK.Sin256(entity->angle) >> 8;
        entity->playerPos.y -= RSDK.Cos256(entity->angle) >> 8;
    }
    entity->playerPos.x &= 0x1F;
    entity->playerPos.y &= 0x1F;
    entity->paletteLine = (entity->globeTimer >> 4) & 0xF;
    TileLayer *bg       = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollPos += entity->globeSpeed << 14;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_PinkSphereWarp(void)
{
    RSDK_THIS(BSS_Setup);
    entity->alpha += 8;
    if (entity->alpha == 320) {
        EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        EntityBSS_Player *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, BSS_Player);
        RSDK.SetSpriteAnimation(player1->spriteIndex, 0, &player1->playerAnimator, true, 0);
        RSDK.SetSpriteAnimation(player2->spriteIndex, 0, &player2->playerAnimator, true, 0);

        int count   = BSS_Setup->pinkSphereCount;
        int val     = RSDK.Rand(0, count - 1);
        bool32 flag = false;
        for (; (count && val >= 0) && !flag; --count) {
            for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    ushort tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
                    if ((tile & 0x7F) == BSS_SPHERE_PINK && (x != entity->playerPos.x || y != entity->playerPos.y) && --val < 0) {
                        entity->playerPos.x = x;
                        entity->playerPos.y = y;

                        x    = 0x20;
                        y    = 0x20;
                        flag = true;
                    }
                }
            }
        }

        val  = RSDK.Rand(0, 4);
        flag = false;
        for (int i = 0; i < 4; ++i) {
            int x = entity->playerPos.x;
            int y = entity->playerPos.y;
            switch (val) {
                case 0: y = (y - 1) & 0x1F; break;
                case 1: x = (x + 1) & 0x1F; break;
                case 2: y = (y + 1) & 0x1F; break;
                case 3: x = (x - 1) & 0x1F; break;
                default: break;
            }
            ushort tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
            if (tile < BSS_SPHERE_RED || (tile > BSS_SPHERE_BUMPER && tile != BSS_SPHERE_PINK)) {
                flag = true;
                break;
            }

            val = (val + 1) & 3;
        }

        if (!flag) {
            for (int i = 0; i < 4; ++i) {
                int x = entity->playerPos.x;
                int y = entity->playerPos.y;
                switch (val) {
                    case 0: y = (y - 2) & 0x1F; break;
                    case 1: x = (x + 2) & 0x1F; break;
                    case 2: y = (y + 2) & 0x1F; break;
                    case 3: x = (x - 2) & 0x1F; break;
                    default: break;
                }

                ushort tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
                if (tile < BSS_SPHERE_RED || (tile > BSS_SPHERE_BUMPER && tile != BSS_SPHERE_PINK)) {
                    flag = true;
                    break;
                }

                val = (val + 1) & 3;
            }
        }

        entity->angle = val << 6;
        CREATE_ENTITY(BSS_Collected, intToVoid(5), entity->playerPos.x, entity->playerPos.y);
        BSS_Setup->playField[entity->playerPos.y + (BSS_PLAYFIELD_H * entity->playerPos.x)] = BSS_SPHERE_PINK_STOOD;
        entity->timer                                                                       = 100;
        entity->state                                                                       = BSS_Setup_State_Unknown23;
    }
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_Exit(void)
{
    RSDK_THIS(BSS_Setup);

    PauseMenu->disableEvents = true;
    entity->maxSpeed         = 0;
    if (entity->spinTimer <= 0) {
        RSDK.CreateEntity(BSS_Message->objectID, intToVoid(2), entity->position.x, entity->position.y);
        foreach_active(BSS_Player, player) { player->stateInput = StateMachine_None; }
    }
    else {
        TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
        bg->scrollInfo[0].scrollPos -= 0x200000;
        entity->angle -= 8;
        entity->angle &= 0xFF;
    }

    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (entity->spinTimer & 0xF) {
        globe->drawLayer[0]           = DRAWLAYER_COUNT;
        int timer                     = entity->spinTimer & 0xF;
        entity->globeSpinData.frameID = BSS_Setup->globeFrameTable[timer - 1];
        entity->direction             = BSS_Setup->globeDirTableL[timer - 1];
    }
    else {
        if (entity->spinTimer > 0) {
            entity->palettePage ^= 1;
            globe->drawLayer[0] = 1;
        }
    }
    entity->spinTimer += 2;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_HandleStage(void)
{
    RSDK_THIS(BSS_Setup);
    EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);

    if (entity->maxSpeed < 32 && ++entity->speedupTimer >= entity->speedupInterval) {
        entity->speedupTimer = 0;
        entity->maxSpeed += 4;
    }
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;

    if (entity->unknownFlag) {
        if (!entity->bumperFlag && player1->up)
            entity->unknownFlag = false;
    }
    else {
        if (entity->globeSpeed < entity->maxSpeed)
            entity->globeSpeed += entity->globeSpeedInc;
    }

    if (player1->onGround) {
        if (entity->globeTimer > 0 && entity->globeTimer < 256) {
            if (player1->left)
                entity->spinState = 1;
            if (player1->right)
                entity->spinState = 2;
        }
        entity->globeTimer += entity->globeSpeed;
        BSS_Setup_HandleSteppedObjects();
    }
    else {
        entity->globeTimer += entity->globeSpeed;
        entity->spinState = 0;
    }

    if (entity->state == BSS_Setup_State_HandleStage) {
        if (entity->globeSpeed > 0) {
            if (entity->globeTimer >= 0x100) {
                if (entity->spinState == 1) {
                    entity->state      = BSS_Setup_State_SpinLeft;
                    entity->globeTimer = 0;
                }
                else {
                    if (entity->spinState == 2) {
                        entity->state      = BSS_Setup_State_SpinRight;
                        entity->globeTimer = 0;
                    }
                    if (entity->spinState == 0) {
                        entity->globeTimer -= 256;
                    }
                }

                entity->palettePage ^= 1;
                entity->spinState = 0;

                entity->playerPos.x += RSDK.Sin256(entity->angle) >> 8;
                entity->playerPos.x &= 0x1F;
                entity->playerPos.y -= RSDK.Cos256(entity->angle) >> 8;
                entity->playerPos.y &= 0x1F;
            }
        }
        else if (entity->globeTimer < 0) {
            if (entity->spinState == 1) {
                entity->state      = BSS_Setup_State_SpinLeft;
                entity->globeTimer = 0;
                entity->spinState  = 0;
            }
            else {
                if (entity->spinState == 2) {
                    entity->state      = BSS_Setup_State_SpinRight;
                    entity->globeTimer = 0;
                    entity->spinState  = 0;
                }
                else if (entity->spinState == 0) {
                    entity->globeTimer += 256;
                    entity->palettePage ^= 1;

                    entity->playerPos.x -= RSDK.Sin256(entity->angle) >> 8;
                    entity->playerPos.x &= 0x1F;
                    entity->playerPos.y += RSDK.Cos256(entity->angle) >> 8;
                    entity->playerPos.y &= 0x1F;

                    entity->spinState = 0;
                }
            }
        }

        TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
        bg->scrollPos += entity->globeSpeed << 14;
    }

    entity->paletteLine = (entity->globeTimer >> 4) & 0xF;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_SpinLeft(void)
{
    RSDK_THIS(BSS_Setup);

    if (entity->maxSpeed < 32 && ++entity->speedupTimer >= entity->speedupInterval) {
        entity->speedupTimer = 0;
        entity->maxSpeed += 4;
    }
    TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollInfo[0].scrollPos -= 0x100000;

    entity->angle -= 4;
    entity->angle &= 0xFF;
    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (entity->spinTimer == 15) {
        entity->spinTimer   = 0;
        globe->drawLayer[0] = 1;
        entity->palettePage ^= 1;
        if (!entity->timer)
            entity->state = BSS_Setup_State_HandleStage;
        else
            entity->state = BSS_Setup_State_Unknown23;
        BSS_Setup_HandleCollectableMovement();
    }
    else {
        globe->drawLayer[0] = DRAWLAYER_COUNT;

        entity->globeSpinData.frameID = BSS_Setup->globeFrameTable[entity->spinTimer];
        entity->direction             = BSS_Setup->globeDirTableL[entity->spinTimer];
        entity->spinTimer++;
        if (entity->timer > 1)
            entity->timer--;
        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_SpinRight(void)
{
    RSDK_THIS(BSS_Setup);

    if (entity->maxSpeed < 32 && ++entity->speedupTimer >= entity->speedupInterval) {
        entity->speedupTimer = 0;
        entity->maxSpeed += 4;
    }
    TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollInfo[0].scrollPos += 0x100000;

    entity->angle += 4;
    entity->angle &= 0xFF;
    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (entity->spinTimer == 15) {
        entity->spinTimer   = 0;
        globe->drawLayer[0] = 1;
        if (!entity->timer)
            entity->state = BSS_Setup_State_HandleStage;
        else
            entity->state = BSS_Setup_State_Unknown23;
        BSS_Setup_HandleCollectableMovement();
    }
    else {
        globe->drawLayer[0] = DRAWLAYER_COUNT;

        if (!entity->spinTimer)
            entity->palettePage ^= 1;
        entity->globeSpinData.frameID = BSS_Setup->globeFrameTable[entity->spinTimer];
        entity->direction             = BSS_Setup->globeDirTableR[entity->spinTimer];
        entity->spinTimer++;
        if (entity->timer > 1)
            entity->timer--;
        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_Unknown23(void)
{
    RSDK_THIS(BSS_Setup);

    EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
    if (entity->alpha <= 0) {
        if (player->up) {
            entity->timer = 1;
        }
        else if (player->left) {
            entity->state = BSS_Setup_State_SpinLeft;
        }
        else if (player->right) {
            entity->state = BSS_Setup_State_SpinRight;
        }
    }
    else {
        entity->alpha -= 8;
    }

    if (!--entity->timer) {
        entity->state = BSS_Setup_State_HandleStage;

        EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        if (player1->onGround)
            RSDK.SetSpriteAnimation(player1->spriteIndex, 1, &player1->playerAnimator, 0, 0);

        EntityBSS_Player *player2 = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER2);
        if (player2->onGround)
            RSDK.SetSpriteAnimation(player2->spriteIndex, 1, &player2->playerAnimator, 0, 0);
    }
    BSS_Setup_HandleCollectableMovement();
}

bool32 BSS_Setup_Unknown(int x, int y)
{
    int x1 = BSS_PLAYFIELD_H * (((byte)x - 1) & 0x1F);
    int y1 = ((byte)y - 1) & 0x1F;
    int x2 = BSS_PLAYFIELD_H * (((byte)x + 1) & 0x1F);
    int y2 = ((byte)y + 1) & 0x1F;

    if ((BSS_Setup->playField[x1 + y1] & 0x7F) == BSS_SPHERE_BLUE)
        return true;

    if ((BSS_Setup->playField[x2 + y1] & 0x7F) == BSS_SPHERE_BLUE || (BSS_Setup->playField[x1 + y] & 0x7F) == BSS_SPHERE_BLUE
        || (BSS_Setup->playField[x2 + y] & 0x7F) == BSS_SPHERE_BLUE) {
        return true;
    }

    if ((BSS_Setup->playField[y2 + x1] & 0x7F) != BSS_SPHERE_BLUE && (BSS_Setup->playField[y2 + x2] & 0x7F) != BSS_SPHERE_BLUE
        && (BSS_Setup->playField[(BSS_PLAYFIELD_H * x) + y1] & 0x7F) != BSS_SPHERE_BLUE
        && (BSS_Setup->playField[(BSS_PLAYFIELD_H * x) + y2] & 0x7F) != BSS_SPHERE_BLUE) {
        return false;
    }

    return true;
}

void BSS_Setup_LaunchSpheres(void)
{
    RSDK_THIS(BSS_Setup);
    int x              = entity->spinTimer + 0x100;
    int y              = entity->spinTimer << 17;
    ScreenInfo *screen = RSDK_screens;

    int i                              = RESERVE_ENTITY_COUNT;
    EntityBSS_Collectable *collectable = NULL;

    collectable = RSDK_GET_ENTITY(i++, BSS_Collectable);
    while (collectable->objectID != TYPE_BLANK) {
        int ix                  = (collectable->position.x >> 16);
        collectable->position.x = ((x * (ix - screen->centerX) >> 8) + screen->centerX) << 16;
        collectable->position.y -= y;
        collectable = RSDK_GET_ENTITY(i++, BSS_Collectable);
    }
}

void BSS_Setup_StageFinishClear(void)
{
    RSDK_THIS(BSS_Setup);

    for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
            BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
        }
    }

    int fx       = (RSDK.Sin256(entity->angle) >> 5) + entity->playerPos.x;
    int fy       = (((byte)entity->playerPos.y - (byte)(RSDK.Cos256(entity->angle) >> 5)) & 0x1F);
    int fieldPos = fy + (BSS_PLAYFIELD_H * (fx & 0x1F));

    if (BSS_Setup->ringCount > 0)
        BSS_Setup->playField[fieldPos] = BSS_MEDAL_SILVER;
    else
        BSS_Setup->playField[fieldPos] = BSS_MEDAL_GOLD;
    RSDK_GET_ENTITY(RESERVE_ENTITY_COUNT, BSS_Collectable)->drawOrder     = 3;
    RSDK_GET_ENTITY(RESERVE_ENTITY_COUNT + 1, BSS_Collectable)->drawOrder = 3;
}

bool32 BSS_Setup_Unknown2(byte x, byte y)
{
    RSDK_THIS(BSS_Setup);
    if (entity->ringLoopFlag)
        return true;

    int px = BSS_PLAYFIELD_H * x;
    int id = 0;

    int y1 = y;
    while (true) {
        y1 = ((byte)y1 - 1) & 0x1F;
        if ((BSS_Setup->playField[px + y1] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->playField2[px + y1] & 0x7F) == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_Unknown(x, y1))
            break;

        BSS_Setup->playField2[y1 + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y1 + px] = BSS_SPHERE_BLUE;
        if (x == entity->lastSpherePos.x && y1 == entity->lastSpherePos.y) {
            entity->ringLoopFlag = true;
            return true;
        }
        bool32 flag = false;

        int fieldPos  = y1 + (BSS_PLAYFIELD_H * (((byte)x + 1) & 0x1F));
        int fieldPos2 = y1 + (BSS_PLAYFIELD_H * (((byte)x - 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown5(x, y1);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown4(x, y1);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (entity->ringLoopFlag)
            return true;
    }

    for (int i = id; i > 0; --i) {
        y1                             = ((byte)y1 + 1) & 0x1F;
        BSS_Setup->playField3[px + y1] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_Unknown3(byte x, byte y)
{
    RSDK_THIS(BSS_Setup);
    if (entity->ringLoopFlag)
        return true;

    int px = BSS_PLAYFIELD_H * x;
    int id = 0;
    int y1 = y;

    while (true) {
        y1 = ((byte)y1 + 1) & 0x1F;
        if ((BSS_Setup->playField[px + y1] & 0x7F) != BSS_SPHERE_RED)
            break;

        if (BSS_Setup->playField2[px + y1] == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_Unknown(x, y1)) {
            break;
        }

        BSS_Setup->playField2[y1 + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y1 + px] = BSS_SPHERE_BLUE;
        if (x == entity->lastSpherePos.x && y1 == entity->lastSpherePos.y) {
            entity->ringLoopFlag = true;
            return true;
        }

        bool32 flag = false;

        int fieldPos  = y1 + (BSS_PLAYFIELD_H * (((byte)x - 1) & 0x1F));
        int fieldPos2 = y1 + (BSS_PLAYFIELD_H * (((byte)x + 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown4(x, y1);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown5(x, y1);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (entity->ringLoopFlag)
            return true;
    }

    for (int i = id; i > 0; --i) {
        y1                             = ((byte)y1 - 1) & 0x1F;
        BSS_Setup->playField3[px + y1] = BSS_NONE;
    }
    return false;
}
bool32 BSS_Setup_Unknown4(byte x, byte y)
{
    RSDK_THIS(BSS_Setup);
    if (entity->ringLoopFlag)
        return true;

    int id = 0;

    int x1 = x;
    while (true) {
        x1     = ((byte)x1 - 1) & 0x1F;
        int px = (BSS_PLAYFIELD_H * x1);
        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->playField2[px + y] & 0x7F) == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_Unknown(x1, y))
            break;

        BSS_Setup->playField2[y + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y + px] = BSS_SPHERE_BLUE;
        if (x1 == entity->lastSpherePos.x && y == entity->lastSpherePos.y) {
            entity->ringLoopFlag = true;
            return true;
        }
        bool32 flag = false;

        int fieldPos  = (BSS_PLAYFIELD_H * x1) + (((byte)y - 1) & 0x1F);
        int fieldPos2 = (BSS_PLAYFIELD_H * x1) + (((byte)y + 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown2(x1, y);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown3(x1, y);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (entity->ringLoopFlag)
            return true;
    }

    for (int i = id; i > 0; --i) {
        x1                            = ((byte)x1 + 1) & 0x1F;
        int px                        = (BSS_PLAYFIELD_H * x1);
        BSS_Setup->playField3[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_Unknown5(byte x, byte y)
{
    RSDK_THIS(BSS_Setup);
    if (entity->ringLoopFlag)
        return true;

    int id = 0;

    int x1 = x;
    while (true) {
        x1     = ((byte)x1 + 1) & 0x1F;
        int px = (BSS_PLAYFIELD_H * x1);
        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->playField2[px + y] & 0x7F) == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_Unknown(x1, y))
            break;

        BSS_Setup->playField2[y + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y + px] = BSS_SPHERE_BLUE;
        if (x1 == entity->lastSpherePos.x && y == entity->lastSpherePos.y) {
            entity->ringLoopFlag = true;
            return true;
        }
        bool32 flag = false;

        int fieldPos  = (BSS_PLAYFIELD_H * x1) + (((byte)y + 1) & 0x1F);
        int fieldPos2 = (BSS_PLAYFIELD_H * x1) + (((byte)y - 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown3(x1, y);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_Unknown2(x1, y);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (entity->ringLoopFlag)
            return true;
    }

    for (int i = id; i > 0; --i) {
        x1                            = ((byte)x1 - 1) & 0x1F;
        int px                        = (BSS_PLAYFIELD_H * x1);
        BSS_Setup->playField3[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_Unknown6(byte x, byte y)
{
    int px = BSS_PLAYFIELD_H * x;

    int y1 = ((byte)y - 1) & 0x1F;
    for (int i = 0; i < BSS_PLAYFIELD_H; ++i) {
        if (BSS_Setup->playField3[px + y1] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int id = BSS_Setup->playField[px + y1] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            y1 = ((byte)y1 - 1) & 0x1F;
        }
    }

    y1 = ((byte)y + 1) & 0x1F;
    for (int i = 0; i < BSS_PLAYFIELD_H; ++i) {
        if (BSS_Setup->playField3[px + y1] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int id = BSS_Setup->playField[px + y1] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            y1 = ((byte)y1 + 1) & 0x1F;
        }
    }

    int x1 = ((byte)x - 1) & 0x1F;
    for (int i = 0; i < BSS_PLAYFIELD_W; ++i) {
        if (BSS_Setup->playField3[y + (BSS_PLAYFIELD_W * x1)] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int id = BSS_Setup->playField[y + (BSS_PLAYFIELD_W * x1)] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            x1 = ((byte)x1 - 1) & 0x1F;
        }
    }

    x1 = ((byte)x + 1) & 0x1F;
    for (int i = 0; i < BSS_PLAYFIELD_W; ++i) {
        if (BSS_Setup->playField3[y + (BSS_PLAYFIELD_W * x1)] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int id = BSS_Setup->playField[y + (BSS_PLAYFIELD_W * x1)] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            x1 = ((byte)x1 + 1) & 0x1F;
        }
    }

    BSS_Setup->playField3[px + y] = BSS_SPHERE_BLUE;
    return true;
}

void BSS_Setup_ProcessChain(void)
{
    RSDK_THIS(BSS_Setup);

    for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
            BSS_Setup->playField2[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
        }
    }
    BSS_Setup->playField[entity->lastSpherePos.y + (BSS_PLAYFIELD_H * entity->lastSpherePos.x)]  = BSS_SPHERE_RED;
    BSS_Setup->playField3[entity->lastSpherePos.y + (BSS_PLAYFIELD_H * entity->lastSpherePos.x)] = BSS_SPHERE_BLUE;

    entity->ringLoopFlag = false;
    BSS_Setup_Unknown2(entity->lastSpherePos.x, entity->lastSpherePos.y);
    BSS_Setup_Unknown3(entity->lastSpherePos.x, entity->lastSpherePos.y);
    BSS_Setup_Unknown4(entity->lastSpherePos.x, entity->lastSpherePos.y);
    BSS_Setup_Unknown5(entity->lastSpherePos.x, entity->lastSpherePos.y);

    BSS_Setup->playField[entity->lastSpherePos.y + (BSS_PLAYFIELD_H * entity->lastSpherePos.x)] = BSS_SPHERE_BLUE;
    if (entity->ringLoopFlag) {
        int spheresCollected = 0;

        for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
            for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
                if ((BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] & 0x7F) == BSS_SPHERE_BLUE) {
                    spheresCollected += BSS_Setup_Unknown6(x, y);
                }
            }
        }

        if (spheresCollected <= 0) {
            entity->ringLoopFlag = false;
        }
        else {
            for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    int pitch = x * BSS_PLAYFIELD_H;

                    if (BSS_Setup->playField3[pitch + y] == BSS_SPHERE_BLUE) {
                        int y1 = ((byte)y - 1) & 0x1F;
                        if ((BSS_Setup->playField[y1 + pitch] & 0x7F) != BSS_SPHERE_BLUE) {
                            int y2 = ((byte)y + 1) & 0x1F;
                            if ((BSS_Setup->playField[y2 + pitch] & 0x7F) != BSS_SPHERE_BLUE) {
                                int x1 = BSS_PLAYFIELD_H * (((byte)x - 1) & 0x1F);
                                if ((BSS_Setup->playField[x1 + y] & 0x7F) != BSS_SPHERE_BLUE) {
                                    int x2 = BSS_PLAYFIELD_H * (((byte)x + 1) & 0x1F);
                                    if ((BSS_Setup->playField[x2 + y] & 0x7F) != BSS_SPHERE_BLUE
                                        && (BSS_Setup->playField[y1 + x1] & 0x7F) != BSS_SPHERE_BLUE
                                        && (BSS_Setup->playField[x2 + y1] & 0x7F) != BSS_SPHERE_BLUE
                                        && (BSS_Setup->playField[y2 + x1] & 0x7F) != BSS_SPHERE_BLUE
                                        && (BSS_Setup->playField[x2 + y2] & 0x7F) != BSS_SPHERE_BLUE) {
                                        BSS_Setup->playField3[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    if (BSS_Setup->playField3[(x * BSS_PLAYFIELD_H) + y]) {
                        BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] = BSS_RING;
                    }
                }
            }

            BSS_Setup->sphereCount -= spheresCollected;
            RSDK.PlaySFX(BSS_Setup->sfxLoseRings, 0, 0xFF);
        }
    }
}

void BSS_Setup_EditorDraw(void) {}

void BSS_Setup_EditorLoad(void) {}

void BSS_Setup_Serialize(void) { RSDK_EDITABLE_VAR(BSS_Setup, VAR_UINT8, paletteID); }
