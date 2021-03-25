#include "../SonicMania.h"
#include <time.h>

ObjectBSS_Setup *BSS_Setup;

void BSS_Setup_Update()
{
    RSDK_THIS(BSS_Setup);

    if (entity->state)
        entity->state();
    RSDK_screens->position.x = 0x100 - RSDK_screens->centerX;

    if (entity->palettePage) {
        RSDK.CopyPalette(2, 16 * entity->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(1, 16 * entity->paletteLine, 0, 144, 16);
    }
    else {
        RSDK.CopyPalette(1, 16 * entity->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(2, 16 * entity->paletteLine, 0, 144, 16);
    }

    if ((byte)(globals->menuParam[22] >> 8) != 1 && !entity->dwordC8 && globals->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM[62];
}

void BSS_Setup_LateUpdate() {}

void BSS_Setup_StaticUpdate() {}

void BSS_Setup_Draw()
{
    RSDK_THIS(BSS_Setup);
    TileLayer *globe  = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    entity->inkEffect = INK_NONE;
    if (globe->drawLayer[0] == 16)
        RSDK.DrawSprite(&entity->globeSpinData, 0, 0);

    Vector2 drawPos;
    entity->inkEffect = INK_BLEND;
    drawPos.x         = entity->position.x;
    drawPos.y         = 0x9E0000;
    RSDK.DrawSprite(&entity->shadowData, &drawPos, 0);
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
        entity->dwordC8         = 0;
        RSDK.SetSpriteAnimation(BSS_Setup->globeMappings, 0, &entity->globeSpinData, true, 0);
        RSDK.SetSpriteAnimation(BSS_Setup->globeMappings, 1, &entity->shadowData, true, 0);
        BSS_Setup_GetStartupInfo();
        entity->state = BSS_Setup_State_HandleStage;
    }
}

void BSS_Setup_StageLoad()
{
    BSS_Setup->globeMappings  = RSDK.LoadSpriteAnimation("SpecialBS/Globe.bin", SCOPE_STAGE);
    BSS_Setup->bgLayer        = RSDK.GetSceneLayerID("Background");
    BSS_Setup->globeLayer     = RSDK.GetSceneLayerID("Globe");
    BSS_Setup->frustrum1Layer = RSDK.GetSceneLayerID("Frustum 1");
    BSS_Setup->frustrum2Layer = RSDK.GetSceneLayerID("Frustum 2");
    BSS_Setup->playFieldLayer = RSDK.GetSceneLayerID("Playfield");
    BSS_Setup->ringCountLayer = RSDK.GetSceneLayerID("Ring Count");
    RSDK.ResetEntitySlot(SLOT_BSS_SETUP, BSS_Setup->objectID, 0);
    // BSS_Setup_SetupCollectables();
    BSS_Setup->ringCount = 0;
    TileLayer *playField = RSDK.GetSceneLayer(BSS_Setup->playFieldLayer);

    if (playField->width <= BSS_PLAYFIELD_W) {
        for (int y = 0; y < BSS_PLAYFIELD_H; ++y) {
            for (int x = 0; x < BSS_PLAYFIELD_W; ++x) {
                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, x, y);

                int playFieldPos                   = (x * BSS_PLAYFIELD_W) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = 0;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, x, y) & 0x3FF) == BSS_RING)
                    ++BSS_Setup->ringCount;
            }
        }
    }
    else {
        int max  = (playField->width >> 4) * ((playField->height - 32) >> 4);
        uint key = time(0);

        BSS_Setup->flags[0] = RSDK.Random(0, max, &key);
        BSS_Setup->flags[1] = RSDK.Random(0, max, &key);
        BSS_Setup->flags[2] = RSDK.Random(0, max, &key);
        BSS_Setup->flags[3] = RSDK.Random(0, max, &key);

        BSS_Palette->startColourID = 16 * (BSS_Setup->flags[0] & 0xF);

        for (int y = 0; y < BSS_PLAYFIELD_H; y += 2) {
            for (int x = 0; x < BSS_PLAYFIELD_W / 2; ++x) {
                int f  = (BSS_Setup->flags[0] & 0xF0);
                int tx = x + ((BSS_PLAYFIELD_W / 2) * f);
                int ty = y + BSS_PLAYFIELD_W + f;

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                   = (x * BSS_PLAYFIELD_W) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = 0;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        for (int y = 0; y < BSS_PLAYFIELD_H; y += 2) {
            for (int x = 0; x < 0x10; ++x) {
                int f  = (BSS_Setup->flags[1] & 0xF0);
                int tx = (BSS_PLAYFIELD_W / 2) * f - x + 0x0F;
                int ty = y + BSS_PLAYFIELD_W + f;

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                    = (x * BSS_PLAYFIELD_W) + y;
                BSS_Setup->playField2[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField2[playFieldPos] > 0x18) {
                    BSS_Setup->playField2[playFieldPos] = 0;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        for (int y = 1; y < BSS_PLAYFIELD_H; y += 2) {
            for (int x = 0; x < 0x10; ++x) {
                int f  = (BSS_Setup->flags[2] & 0xF0);
                int tx = x + ((BSS_PLAYFIELD_W / 2) * f);
                int ty = f - y + BSS_PLAYFIELD_H + 0xF;

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                   = (x * BSS_PLAYFIELD_W) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 0x18) {
                    BSS_Setup->playField[playFieldPos] = 0;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        for (int y = 1; y < BSS_PLAYFIELD_H; y += 2) {
            for (int x = 0; x < 0x10; ++x) {
                int f  = (BSS_Setup->flags[3] & 0xF0);
                int tx = (BSS_PLAYFIELD_W / 2) * f - x + 0x0F;
                int ty = f - y + BSS_PLAYFIELD_H + 0xF;

                ushort tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int playFieldPos                    = (x * BSS_PLAYFIELD_W) + y;
                BSS_Setup->playField2[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField2[playFieldPos] > 0x18) {
                    BSS_Setup->playField2[playFieldPos] = 0;
                }

                if ((RSDK.GetTileInfo(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING) {
                    ++BSS_Setup->ringCount;
                }
            }
        }

        // BSS_Setup->playField[515] = 9;
    }

    BSS_Setup_SetupPalette();
    globals->specialCleared    = false;
    BSS_Setup->sfx_BlueSphere  = RSDK.GetSFX("Special/BlueSphere.wav");
    BSS_Setup->sfx_SSExit      = RSDK.GetSFX("Special/SSExit.wav");
    BSS_Setup->sfx_SSJettison  = RSDK.GetSFX("Special/SSJettison.wav");
    BSS_Setup->sfx_Emerald     = RSDK.GetSFX("Special/Emerald.wav");
    BSS_Setup->sfx_Event       = RSDK.GetSFX("Special/Event.wav");
    BSS_Setup->sfx_Bumper      = RSDK.GetSFX("Stage/Bumper.wav");
    BSS_Setup->sfx_Spring      = RSDK.GetSFX("Global/Spring.wav");
    BSS_Setup->sfx_Ring        = RSDK.GetSFX("Global/Ring.wav");
    BSS_Setup->sfx_LoseRings   = RSDK.GetSFX("Global/LoseRings.wav");
    BSS_Setup->sfx_Medal       = RSDK.GetSFX("Special/Medal.wav");
    BSS_Setup->sfx_MedalCaught = RSDK.GetSFX("Special/MedalCaught.wav");
    BSS_Setup->sfx_Teleport    = RSDK.GetSFX("Global/Teleport.wav");
    if ((byte)(globals->menuParam[22] >> 8) == 1) {
        TextInfo info;
        Localization_GetString(&info, STR_RPC_PLAYING);
        User.SetRichPresence(PRESENCE_GENERIC, &info);
    }
}

int BSS_Setup_ReloadScene()
{
    int pos = RSDK_sceneInfo->listPos;
    RSDK.LoadScene("Blue Spheres", "");
    RSDK_sceneInfo->listPos = pos;
    return (pos - RSDK_sceneInfo->listPos) % 32;
}

void BSS_Setup_SetupPalette()
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

void BSS_Setup_CollectRing()
{
    RSDK_THIS(BSS_Setup);
    ++BSS_Setup->rings;

    if (BSS_Setup->ringCount > 0) {
        BSS_Setup->ringCount--;
        if (!BSS_Setup->ringCount) {
            RSDK.CreateEntity(BSS_Message->objectID, (void *)1, entity->position.x, entity->position.y);
            RSDK.PlaySFX(BSS_Setup->sfx_Event, 0, 255);
        }
    }

    if (BSS_Setup->ringPan) {
        int channel = RSDK.PlaySFX(BSS_Setup->sfx_Ring, 0, 255);
        RSDK.SetSoundAttributes(0, channel, 1.0, -1.0, 1.0);
        BSS_Setup->ringPan = 0;
    }
    else {
        int channel = RSDK.PlaySFX(BSS_Setup->sfx_Ring, 0, 255);
        RSDK.SetSoundAttributes(0, channel, 1.0, 1.0, 1.0);
        BSS_Setup->ringPan = 1;
    }
}

void BSS_Setup_GetStartupInfo()
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

void BSS_Setup_Finished()
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
    entity->dwordC8 = 1;
    BSS_Setup_HandleCollectableMovement();
    BSS_Setup_Unknown12();

    if (++entity->spinTimer == 128) {
        entity->spinTimer  = 0;
        entity->maxSpeed   = 8;
        entity->globeSpeed = 8;
        // BSS_Setup_Unknown13();
        EntityBSS_Player *player = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER1);
        player->inputState       = false;
        player->jumpPress        = false;
        entity->state            = BSS_Setup_State_FinishWalk;
    }
}

void BSS_Setup_HandleSteppedObjects()
{
    RSDK_THIS(BSS_Setup);
    if (entity->globeTimer < 32)
        entity->field_90 = 0;
    if (entity->globeTimer > 224)
        entity->field_90 = 0;

    int fieldPos = entity->playerPos.y + (BSS_PLAYFIELD_W * entity->playerPos.x);
    switch (BSS_Setup->playField[fieldPos]) {
        case BSS_SPHERE_BLUE:
            if (entity->globeTimer < 128) {
                entity->field_B4 = entity->playerPos.x;
                entity->field_B8 = entity->playerPos.y;
                // BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;
                if (!entity->field_C0) {
                    RSDK.CreateEntity(BSS_Collected->objectID, (void *)1, entity->playerPos.x, entity->playerPos.y);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }

                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    entity->state          = BSS_Setup_Finished;
                    RSDK.PlaySFX(BSS_Setup->sfx_SSJettison, 0, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySFX(BSS_Setup->sfx_BlueSphere, 0, 255);
                }
            }
            break;
        case BSS_SPHERE_RED:
            if (entity->state != BSS_Setup_State_Exit && entity->globeTimer < 32) {
                entity->state      = BSS_Setup_State_Exit;
                entity->dwordC8    = 1;
                entity->spinTimer  = 0;
                entity->globeTimer = 0;
                RSDK.PlaySFX(BSS_Setup->sfx_SSExit, 0, 255);
                Music_FadeOut(0.0125);
            }
            break;
        case BSS_SPHERE_BUMPER:
            if (entity->field_90 && entity->globeTimer < 112) {
                if (entity->globeTimer > 16) {
                    if (entity->globeSpeed < 0) {
                        entity->field_90   = 1;
                        entity->globeSpeed = -entity->globeSpeed;
                        entity->field_88   = 0;
                        RSDK.PlaySFX(BSS_Setup->sfx_Bumper, 0, 255);
                    }
                }
                else if (!entity->spinState) {
                    if (entity->globeSpeed < 0) {
                        entity->globeTimer = 16;
                        entity->field_90   = 1;
                        entity->globeSpeed = -entity->globeSpeed;
                        entity->field_88   = 0;
                        RSDK.PlaySFX(BSS_Setup->sfx_Bumper, 0, 255);
                    }
                }
            }
            break;
        case BSS_SPHERE_YELLOW:
            if (entity->globeTimer < 128) {
                EntityBSS_Player *player = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER1);
                player->velocity.y       = -0x180000;
                player->onGround         = 0;
                RSDK.SetSpriteAnimation(player->spriteIndex, 3, &player->playerData, 0, 0);
                BSS_Player->field_6 = 4;
                entity->globeSpeed *= 2;
                entity->spinState = 0;
                entity->field_8C  = 4;
                RSDK.PlaySFX(BSS_Setup->sfx_Spring, 0, 255);
            }
            break;
        case BSS_SPHERE_GREEN:
            if (entity->globeTimer > 128) {
                RSDK.CreateEntity(BSS_Collected->objectID, (void *)3, entity->playerPos.x, entity->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySFX(BSS_Setup->sfx_BlueSphere, 0, 255);
            }
            break;
        case BSS_SPHERE_PINK:
            if (entity->state != BSS_Setup_State_PinkSphereWarp && entity->globeTimer < 64) {
                entity->state      = BSS_Setup_State_PinkSphereWarp;
                entity->spinTimer  = 0;
                entity->globeTimer = 0;
                RSDK.PlaySFX(BSS_Setup->sfx_Teleport, 0, 255);
                EntityFXFade *fade = (EntityFXFade *)RSDK.CreateEntity(FXFade->objectID, (void *)0xF0F0F0, entity->position.x, entity->position.y);
                fade->speedIn      = 32;
                fade->speedOut     = 32;
                fade->wait         = 48;
            }
            break;
        case BSS_RING:
            if (entity->globeTimer < 128) {
                RSDK.CreateEntity(BSS_Collected->objectID, 0, entity->playerPos.x, entity->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;
        default: break;
    }

    int posX      = (entity->playerPos.x - (RSDK.Sin256(entity->angle) >> 8)) & 0x1F;
    int posY      = (entity->playerPos.y - (RSDK.Cos256(entity->angle) >> 8)) & 0x1F;
    fieldPos      = posY + (BSS_PLAYFIELD_W * posX);
    int fieldData = BSS_Setup->playField[fieldPos];
    switch (fieldData) {
        case BSS_SPHERE_BLUE:
            if (entity->globeTimer > 128) {
                entity->field_B4 = posX;
                entity->field_B8 = posY;
                // BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;
                if (!entity->field_C0) {
                    RSDK.CreateEntity(BSS_Collected->objectID, (void *)1, posX, posY);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }
                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    RSDK.PlaySFX(BSS_Setup->sfx_Medal, 0, 255);
                    entity->state = BSS_Setup_Finished;
                    RSDK.PlaySFX(BSS_Setup->sfx_SSJettison, 0, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySFX(BSS_Setup->sfx_BlueSphere, 0, 255);
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
                entity->playerPos.y += RSDK.Cos256(entity->angle) >> 8;
                entity->playerPos.y &= 0x1F;
                RSDK.PlaySFX(BSS_Setup->sfx_SSExit, 0, 255);
                Music_FadeOut(0.0125);
            }
            break;
        case BSS_SPHERE_BUMPER:
            if (entity->field_90 && entity->globeTimer > 144) {
                if (entity->globeTimer >= 240) {
                    if (!entity->spinState) {
                        if (entity->globeSpeed > 0) {
                            entity->globeTimer = 240;
                            entity->field_90   = 1;
                            entity->globeSpeed = -entity->globeSpeed;
                            entity->field_88   = 1;
                            RSDK.PlaySFX(BSS_Setup->sfx_Bumper, 0, 255);
                        }
                    }
                }
                else {
                    if (entity->globeSpeed > 0) {
                        entity->field_90   = 1;
                        entity->globeSpeed = -entity->globeSpeed;
                        entity->field_88   = 1;
                        RSDK.PlaySFX(BSS_Setup->sfx_Bumper, 0, 255);
                    }
                }
            }
            break;
        case BSS_SPHERE_YELLOW:
            if (entity->globeTimer > 128) {
                EntityBSS_Player *player = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER1);
                player->velocity.y       = -0x180000;
                player->onGround         = 0;
                RSDK.SetSpriteAnimation(player->spriteIndex, 3, &player->playerData, 0, 0);
                BSS_Player->field_6 = 4;
                entity->globeSpeed *= 2;
                entity->spinState = 0;
                entity->field_8C  = 4;
                RSDK.PlaySFX(BSS_Setup->sfx_Spring, 0, 255);
            }
            break;
        case BSS_SPHERE_GREEN:
            if (entity->globeTimer > 128) {
                RSDK.CreateEntity(BSS_Collected->objectID, (void *)3, posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySFX(BSS_Setup->sfx_BlueSphere, 0, 255);
            }
            break;
        case BSS_RING:
            if (entity->globeTimer > 128) {
                RSDK.CreateEntity(BSS_Collected->objectID, 0, posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;
        case BSS_EMERALD_CHAOS:
        case BSS_EMERALD_SUPER:
        case BSS_MEDAL_SILVER:
        case BSS_MEDAL_GOLD:
            if (entity->globeTimer > 240) {
                if ((byte)(globals->menuParam[22] >> 8) != 1 && globals->gameMode < MODE_TIMEATTACK) {
                    int pos = BSS_Setup_ReloadScene();
                    if (pos >= 0) {
                        int *saveRAM = SaveGame_GetGlobalData();
                        if (saveRAM) {
                            byte medalType = 0;
                            if (fieldData == BSS_MEDAL_SILVER) {
                                medalType = 1;
                            }
                            else if (fieldData == BSS_MEDAL_GOLD) {
                                medalType = 2;
                            }
                            if (medalType)
                                SaveGame_GetMedal(pos, medalType);

                            if (saveRAM[30] && saveRAM[71] == 0x1F)
                                User.UnlockAchievement("ACH_GOLD_MEDAL");
                            if (saveRAM[31] && saveRAM[72] == 0x1F)
                                User.UnlockAchievement("ACH_SILVER_MEDAL");
                        }
                    }
                }

                entity->palettePage ^= 1;
                entity->state      = BSS_Setup_State_Exit;
                entity->spinTimer  = 0;
                entity->globeTimer = 0;
                entity->playerPos.x += RSDK.Sin256(entity->angle) >> 8;
                entity->playerPos.x &= 0x1F;
                entity->playerPos.y += RSDK.Cos256(entity->angle) >> 8;
                entity->playerPos.y &= 0x1F;
                globals->specialCleared = true;
                RSDK.PlaySFX(BSS_Setup->sfx_SSExit, 0, 255);
            }
            break;
        default: break;
    }
}

void BSS_Setup_HandleCollectableMovement()
{
    RSDK_THIS(BSS_Setup);
    entity->field_9C = entity->angle >> 6;
    int id           = BSS_Setup->field_142C[(entity->angle & 0x3F) != 0];
    Vector2 *offset  = &BSS_Setup->offsetTable[id];

    int slot = 0x40;
    if (id > 0) {
        for (; slot < 0xC0; ++slot) {
            switch (entity->field_9C) {
                case 0:
                    entity->offset.x = offset->x;
                    entity->offset.y = offset->y;
                    break;
                case 1:
                    entity->offset.x = -offset->y;
                    entity->offset.y = offset->x;
                    break;
                case 2:
                    entity->offset.x = offset->x;
                    entity->offset.y = -offset->y;
                    break;
                case 3:
                    entity->offset.x = offset->y;
                    entity->offset.y = offset->x;
                    break;
                default: break;
            }

            ushort tile = BSS_Setup->playField[((entity->offset.y + entity->playerPos.y) & 0x1F)
                                               + (BSS_PLAYFIELD_W * ((entity->offset.x + entity->playerPos.x) & 0x1F))];
            if (tile) {
                EntityBSS_Collectable *collectable = (EntityBSS_Collectable *)RSDK.GetEntityByID(slot);
                collectable->position.x = ((entity->offset.x * RSDK.Cos256(entity->angle)) + entity->offset.y * RSDK.Sin256(entity->angle)) >> 4;
                collectable->position.y = ((entity->offset.y * RSDK.Cos256(entity->angle)) - entity->offset.x * RSDK.Sin256(entity->angle)) >> 4;
                collectable->position.y = -(collectable->position.y + entity->paletteLine - 16);
                if (collectable->position.y < 0) {
                    collectable->objectID = 0;
                }
                else {
                    collectable->objectID = BSS_Collectable->objectID;
                    collectable->type     = tile & 0x3FF;
                    if (collectable->position.y < 112) {
                        entity->xMultiplier  = BSS_Setup->xMultiplierTable[collectable->position.y];
                        entity->divisor      = BSS_Setup->divisorTable[collectable->position.y];
                        collectable->frameID = BSS_Setup->frameTable[collectable->position.y];
                        collectable->frameID -= (abs(collectable->position.x) >> 5);
                        if (collectable->frameID < 0)
                            collectable->frameID = 0;

                        int finalX              = entity->xMultiplier * collectable->position.x;
                        collectable->position.x = finalX;
                        int xDif                = finalX * finalX >> 16;
                        collectable->position.x = abs(finalX - xDif);
                        collectable->position.x >>= 4;
                        collectable->position.y =
                            BSS_Setup->screenYTable[collectable->position.y] + collectable->position.x * collectable->position.x / entity->divisor;

                        collectable->position.y <<= 16;
                        collectable->position.x = (collectable->position.x + RSDK_screens->centerX) << 16;
                        ++slot;
                    }
                }
            }
            id--;
            ++offset;
        }
    }

    for (; slot < 0xC0; ++slot) {
        Entity *entity   = RSDK.GetEntityByID(slot);
        entity->objectID = TYPE_BLANK;
    }
}

void BSS_Setup_State_FinishWalk()
{
    RSDK_THIS(BSS_Setup);
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;
    entity->globeTimer += entity->globeSpeed;
    if (++entity->spinTimer == 120)
        RSDK.PlaySFX(BSS_Setup->sfx_MedalCaught, 0, 255);
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

void BSS_Setup_State_PinkSphereWarp() { RSDK_THIS(BSS_Setup); }

void BSS_Setup_State_Exit()
{
    RSDK_THIS(BSS_Setup);

    // PauseMenu->dword8 = 1;
    entity->maxSpeed = 0;
    if (entity->spinTimer <= 0) {
        RSDK.CreateEntity(BSS_Message->objectID, (void *)2, entity->position.x, entity->position.y);
        EntityBSS_Player *player = NULL;
        while (RSDK.GetActiveEntities(BSS_Player->objectID, (Entity **)&player) == 1) {
            player->inputState = NULL;
        }
    }
    else {
        TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
        bg->scrollInfo[0].scrollPos -= 0x200000;
        entity->angle -= 8;
        entity->angle &= 0xFF;
    }

    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (entity->spinTimer & 0xF) {
        globe->drawLayer[0]           = 16;
        int timer                     = entity->spinTimer & 0xF;
        entity->globeSpinData.frameID = BSS_Setup->globeFrameTable[timer];
        entity->spinTimer += 2;
        entity->direction = BSS_Setup->globeDirTableL[timer];
        BSS_Setup_HandleCollectableMovement();
    }
    else {
        if (entity->spinTimer > 0) {
            entity->palettePage ^= 1;
            globe->drawLayer[0] = 1;
        }
        entity->spinTimer += 2;
        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_HandleStage()
{
    RSDK_THIS(BSS_Setup);
    EntityBSS_Player *player1 = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER1);

    if (entity->maxSpeed < 32 && ++entity->speedupTimer >= entity->speedupInterval) {
        entity->speedupTimer = 0;
        entity->maxSpeed += 4;
    }
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;

    if (entity->field_88) {
        if (!entity->field_90 && player1->up)
            entity->field_88 = 0;
    }
    else {
        if (entity->globeSpeed < entity->maxSpeed)
            entity->globeSpeed += entity->field_8C;
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
                entity->playerPos.y += RSDK.Cos256(entity->angle) >> 8;
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
                }

                entity->playerPos.x -= RSDK.Sin256(entity->angle) >> 8;
                entity->playerPos.x &= 0x1F;
                entity->playerPos.y -= RSDK.Cos256(entity->angle) >> 8;
                entity->playerPos.x &= 0x1F;

                entity->spinState = 0;
            }
        }

        TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
        bg->scrollPos += entity->globeSpeed << 14;
    }

    entity->paletteLine = (entity->globeTimer >> 4) & 0xF;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_SpinLeft()
{
    RSDK_THIS(BSS_Setup);

    if (entity->maxSpeed < 32 && ++entity->speedupTimer >= entity->speedupInterval) {
        entity->speedupTimer = 0;
        entity->maxSpeed += 4;
    }
    TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollInfo[0].scrollPos -= 0x100000;

    entity->angle += 4;
    entity->angle &= 0xFF;
    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (entity->spinTimer == 15) {
        entity->spinTimer   = 0;
        globe->drawLayer[0] = 1;
        entity->palettePage ^= 1;
        if (!entity->gap68)
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
        if (entity->gap68 > 1)
            entity->gap68--;
        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_SpinRight()
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
        if (!entity->gap68)
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
        if (entity->gap68 > 1)
            entity->gap68--;
        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_Unknown23()
{
    RSDK_THIS(BSS_Setup);

    EntityBSS_Player *player = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER1);
    if (entity->alpha <= 0) {
        if (player->up) {
            entity->gap68 = 1;
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

    if (!--entity->gap68) {
        entity->state = BSS_Setup_State_HandleStage;

        EntityBSS_Player *player1 = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER1);
        if (player1->onGround)
            RSDK.SetSpriteAnimation(player1->spriteIndex, 1, &player1->playerData, 0, 0);

        EntityBSS_Player *player2 = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER2);
        if (player2->onGround)
            RSDK.SetSpriteAnimation(player2->spriteIndex, 1, &player2->playerData, 0, 0);
    }
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_Unknown12()
{
    RSDK_THIS(BSS_Setup);

    ScreenInfo *screen = RSDK_screens;
    int spinA          = entity->spinTimer << 17;
    int spinB          = entity->spinTimer + 256;

    EntityBSS_Collectable *collectable = (EntityBSS_Collectable *)RSDK.GetEntityByID(64);
    for (int slot = 0x40; collectable->objectID; ++slot) {
        collectable             = (EntityBSS_Collectable *)RSDK.GetEntityByID(slot);
        int x                   = (collectable->position.x >> 0x10);
        collectable->position.x = x;
        collectable->position.x = spinB * (x - screen->centerX) >> 8;
        collectable->position.x = (collectable->position.x + screen->centerX) << 16;
        collectable->position.y -= spinA;
        collectable->drawOrder = 5;
    }
}

void BSS_Setup_EditorDraw() {}

void BSS_Setup_EditorLoad() {}

void BSS_Setup_Serialize() { RSDK_EDITABLE_VAR(BSS_Setup, VAR_UINT8, paletteID); }
