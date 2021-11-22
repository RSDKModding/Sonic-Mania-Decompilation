#include "SonicMania.h"
#include <time.h>

ObjectBSS_Setup *BSS_Setup;

void BSS_Setup_Update(void)
{
    RSDK_THIS(BSS_Setup);

    StateMachine_Run(self->state);
    ScreenInfo->position.x = 0x100 - ScreenInfo->centerX;

    if (self->palettePage) {
        RSDK.CopyPalette(2, 16 * self->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(1, 16 * self->paletteLine, 0, 144, 16);
    }
    else {
        RSDK.CopyPalette(1, 16 * self->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(2, 16 * self->paletteLine, 0, 144, 16);
    }

#if RETRO_USE_PLUS
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    if (!param->selectionType && !self->stopMovement && globals->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM->zoneTimes[28];
#endif
}

void BSS_Setup_LateUpdate(void) {}

void BSS_Setup_StaticUpdate(void) {}

void BSS_Setup_Draw(void)
{
    RSDK_THIS(BSS_Setup);
    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    self->inkEffect  = INK_NONE;
    if (globe->drawLayer[0] == DRAWLAYER_COUNT)
        RSDK.DrawSprite(&self->globeSpinAnimator, NULL, false);

    Vector2 drawPos;
    self->inkEffect = INK_BLEND;
    drawPos.x       = self->position.x;
    drawPos.y       = 0x9E0000;
    RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);
}

void BSS_Setup_Create(void *data)
{
    RSDK_THIS(BSS_Setup);
    if (!SceneInfo->inEditor) {
        self->active          = ACTIVE_BOUNDS;
        self->visible         = true;
        self->drawOrder       = 2;
        self->drawFX          = FX_FLIP;
        self->position.x      = 0x1000000;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x800000;
        self->speedupInterval = 1800;
        self->stopMovement    = false;
        RSDK.SetSpriteAnimation(BSS_Setup->globeMappings, 0, &self->globeSpinAnimator, true, 0);
        RSDK.SetSpriteAnimation(BSS_Setup->globeMappings, 1, &self->shadowAnimator, true, 0);
        BSS_Setup_GetStartupInfo();
        self->state = BSS_Setup_State_HandleStage;
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

    memset(BSS_Setup->playField, 0, BSS_PLAYFIELD_W * BSS_PLAYFIELD_H * sizeof(uint16));
    memset(BSS_Setup->playField2, 0, BSS_PLAYFIELD_W * BSS_PLAYFIELD_H * sizeof(uint16));
    memset(BSS_Setup->playField3, 0, BSS_PLAYFIELD_W * BSS_PLAYFIELD_H * sizeof(uint16));

    if (playField->width <= BSS_PLAYFIELD_W) {
        for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
            for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                uint16 tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, x, y);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
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
        int32 max = (playField->width >> 4) * ((playField->height - 32) >> 4);
        int32 key = (int32)time(0);

        BSS_Setup->flags[0] = RSDK.RandSeeded(0, max, &key);
        BSS_Setup->flags[1] = RSDK.RandSeeded(0, max, &key);
        BSS_Setup->flags[2] = RSDK.RandSeeded(0, max, &key);
        BSS_Setup->flags[3] = RSDK.RandSeeded(0, max, &key);

        BSS_Palette->startColourID = 16 * (BSS_Setup->flags[1] & 0x0F);

        // Top Left Quadrant
        for (int32 y = 0, py = 0; y < BSS_PLAYFIELD_H / 2; ++y, ++py) {
            for (int32 x = 0, px = 0; x < BSS_PLAYFIELD_W / 2; ++x, ++px) {
                int32 tx = px + (0x10 * (BSS_Setup->flags[0] & 0x0F));
                int32 ty = py + BSS_PLAYFIELD_H + (BSS_Setup->flags[0] & 0xF0);

                uint16 tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
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
        for (int32 y = 0, py = 0; y < BSS_PLAYFIELD_H / 2; ++y, ++py) {
            for (int32 x = BSS_PLAYFIELD_W / 2, px = 0; x < BSS_PLAYFIELD_W; ++x, ++px) {
                int32 tx = 0x10 * (BSS_Setup->flags[1] & 0x0F) - px + 0x0F;
                int32 ty = py + BSS_PLAYFIELD_H + (BSS_Setup->flags[1] & 0xF0);

                uint16 tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
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
        for (int32 y = BSS_PLAYFIELD_H / 2, py = 0; y < BSS_PLAYFIELD_H; ++y, ++py) {
            for (int32 x = 0, px = 0; x < BSS_PLAYFIELD_W / 2; ++x, ++px) {
                int32 tx = px + (0x10 * (BSS_Setup->flags[2] & 0x0F));
                int32 ty = (BSS_Setup->flags[2] & 0xF0) - py + 0x2F;

                uint16 tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
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
        for (int32 y = BSS_PLAYFIELD_H / 2, py = 0; y < BSS_PLAYFIELD_H; ++y, ++py) {
            for (int32 x = BSS_PLAYFIELD_W / 2, px = 0; x < BSS_PLAYFIELD_W; ++x, ++px) {
                int32 tx = 0x10 * (BSS_Setup->flags[3] & 0x0F) - px + 0x0F;
                int32 ty = (BSS_Setup->flags[3] & 0xF0) - py + 0x2F;

                uint16 tile = RSDK.GetTileInfo(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
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
    BSS_Setup->sfxBlueSphere  = RSDK.GetSfx("Special/BlueSphere.wav");
    BSS_Setup->sfxSSExit      = RSDK.GetSfx("Special/SSExit.wav");
    BSS_Setup->sfxSSJettison  = RSDK.GetSfx("Special/SSJettison.wav");
    BSS_Setup->sfxEmerald     = RSDK.GetSfx("Special/Emerald.wav");
    BSS_Setup->sfxEvent       = RSDK.GetSfx("Special/Event.wav");
    BSS_Setup->sfxBumper      = RSDK.GetSfx("Stage/Bumper.wav");
    BSS_Setup->sfxSpring      = RSDK.GetSfx("Global/Spring.wav");
    BSS_Setup->sfxRing        = RSDK.GetSfx("Global/Ring.wav");
    BSS_Setup->sfxLoseRings   = RSDK.GetSfx("Global/LoseRings.wav");
    BSS_Setup->sfxMedal       = RSDK.GetSfx("Special/Medal.wav");
    BSS_Setup->sfxMedalCaught = RSDK.GetSfx("Special/MedalCaught.wav");
    BSS_Setup->sfxTeleport    = RSDK.GetSfx("Global/Teleport.wav");

    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    if (param->selectionType == 1) {
        TextInfo info;
        Localization_GetString(&info, STR_RPC_PLAYING);
        API_SetRichPresence(PRESENCE_GENERIC, &info);
    }
}

int32 BSS_Setup_GetStageID(void)
{
    int32 pos = SceneInfo->listPos;
    RSDK.SetScene("Blue Spheres", "");
    int32 id           = (pos - SceneInfo->listPos) % 32;
    SceneInfo->listPos = pos;
    return id;
}

void BSS_Setup_SetupPalette(void)
{
    for (int32 i = 0; i < 0x10; ++i) RSDK.SetPaletteEntry(0, 0x80 + i, RSDK.GetPaletteEntry(1, BSS_Palette->startColourID));
    for (int32 i = 0; i < 0x10; ++i) RSDK.SetPaletteEntry(0, 0x90 + i, RSDK.GetPaletteEntry(1, BSS_Palette->startColourID + 1));

    for (int32 i = 0; i < 3; ++i) RSDK.SetPaletteEntry(0, 0xA0 + i, RSDK.GetPaletteEntry(1, i + BSS_Palette->startColourID + 2));
    for (int32 i = 0; i < 4; ++i) RSDK.SetPaletteEntry(0, 0xC0 + i, RSDK.GetPaletteEntry(1, i + BSS_Palette->startColourID + 8));

    for (int32 i = 0; i < 0x100; i += 0x10) {
        RSDK.CopyPalette(0, 0x80, 1, i, 0x10);
        RSDK.RotatePalette(0, 0x80, 0x9F, true);
    }
    for (int32 i = 0; i < 0x100; i += 0x10) {
        RSDK.CopyPalette(0, 0x80, 2, i, 0x10);
        RSDK.RotatePalette(0, 0x80, 0x9F, true);
    }
}

void BSS_Setup_SetupFrustum(void)
{
    int32 offset = 0;
    int32 count  = 0;
    for (int32 f = 0; f < 2; ++f) {
        int32 frustumID = 0;
        if (f)
            frustumID = BSS_Setup->frustum2Layer;
        else
            frustumID = BSS_Setup->frustum1Layer;
        TileLayer *frustum = RSDK.GetSceneLayer(frustumID);

        count       = offset;
        int32 lastX = 0;
        int32 lastY = 0;

        for (int32 y = 0; y < frustum->height; ++y) {
            for (int32 x = 0; x < frustum->width; ++x) {
                uint16 id = (RSDK.GetTileInfo(frustumID, x, y) & 0x3FF);
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
        int32 *offsetRadiusTable    = &BSS_Setup->offsetRadiusTable[BSS_Setup->frustumOffset[f]];

        for (int32 i = 0; i < BSS_Setup->frustumCount[f]; ++i) {
            offsetTable[i].x -= lastX;
            offsetTable[i].y -= lastY;
            offsetRadiusTable[i] = offsetTable[i].x * offsetTable[i].x + offsetTable[i].y * offsetTable[i].y;
        }

        for (int32 o = 0; o < BSS_Setup->frustumCount[f]; ++o) {
            for (int32 i = BSS_Setup->frustumCount[f] - 1; i > o; --i) {
                int32 ox = offsetTable[i - 1].x;
                int32 oy = offsetTable[i - 1].y;
                int32 id = offsetRadiusTable[i - 1];
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

    for (int32 i = RESERVE_ENTITY_COUNT; i < RESERVE_ENTITY_COUNT + 0x60; ++i) {
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
            CREATE_ENTITY(BSS_Message, intToVoid(BSS_MESSAGE_PERFECT), self->position.x, self->position.y);
            RSDK.PlaySfx(BSS_Setup->sfxEvent, false, 255);
        }
    }

    if (BSS_Setup->ringPan) {
        int32 channel = RSDK.PlaySfx(BSS_Setup->sfxRing, false, 255);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        BSS_Setup->ringPan = 0;
    }
    else {
        int32 channel = RSDK.PlaySfx(BSS_Setup->sfxRing, false, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        BSS_Setup->ringPan = 1;
    }
}

void BSS_Setup_GetStartupInfo(void)
{
    RSDK_THIS(BSS_Setup);
    BSS_Setup->sphereCount     = 0;
    BSS_Setup->pinkSphereCount = 0;

    for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
            int32 playFieldPos = (x * BSS_PLAYFIELD_W) + y;
            switch (BSS_Setup->playField[playFieldPos]) {
                case BSS_SPHERE_BLUE:
                case BSS_SPHERE_GREEN: ++BSS_Setup->sphereCount; break;
                case BSS_SPHERE_PINK: ++BSS_Setup->pinkSphereCount; break;
                case BSS_SPAWN_UP:
                    self->angle                        = 0x00;
                    self->playerPos.x                  = x;
                    self->playerPos.y                  = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                case BSS_SPAWN_RIGHT:
                    self->angle                        = 0xC0;
                    self->playerPos.x                  = x;
                    self->playerPos.y                  = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                case BSS_SPAWN_DOWN:
                    self->angle                        = 0x80;
                    self->playerPos.x                  = x;
                    self->playerPos.y                  = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                case BSS_SPAWN_LEFT:
                    self->angle                        = 0x40;
                    self->playerPos.x                  = x;
                    self->playerPos.y                  = y;
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;
                    break;
                default: break;
            }
        }
    }

    RSDK.GetSceneLayer(BSS_Setup->bgLayer)->scrollInfo[0].scrollPos = self->angle << 18;
}

void BSS_Setup_Finished(void)
{
    RSDK_THIS(BSS_Setup);
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;
    self->globeTimer += self->globeSpeed;
    if (self->globeSpeed <= 0 && self->globeTimer < 0) {
        self->palettePage ^= 1;
        self->globeTimer += 0x100;
        self->playerPos.x -= RSDK.Sin256(self->angle) >> 8;
        self->playerPos.y += RSDK.Cos256(self->angle) >> 8;
    }
    else if (self->globeTimer >= 0x100) {
        self->palettePage ^= 1;
        self->globeTimer -= 0x100;
        self->playerPos.x += RSDK.Sin256(self->angle) >> 8;
        self->playerPos.y -= RSDK.Cos256(self->angle) >> 8;
    }
    self->playerPos.x &= 0x1F;
    self->playerPos.y &= 0x1F;

    self->paletteLine = (self->globeTimer >> 4) & 0xF;
    TileLayer *bg     = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollPos += self->globeSpeed << 14;
    self->stopMovement = true;
    BSS_Setup_HandleCollectableMovement();
    BSS_Setup_LaunchSpheres();

    if (++self->spinTimer == 128) {
        self->spinTimer  = 0;
        self->maxSpeed   = 8;
        self->globeSpeed = 8;
        BSS_Setup_StageFinishClear();
        EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        player->stateInput       = false;
        player->jumpPress        = false;
        self->state              = BSS_Setup_State_FinishWalk;
    }
}

void BSS_Setup_HandleSteppedObjects(void)
{
    RSDK_THIS(BSS_Setup);
    if (self->globeTimer < 32)
        self->bumperFlag = false;
    if (self->globeTimer > 224)
        self->bumperFlag = false;

    int32 fieldPos = self->playerPos.y + (BSS_PLAYFIELD_H * self->playerPos.x);
    switch (BSS_Setup->playField[fieldPos]) {
        case BSS_SPHERE_BLUE:
            if (self->globeTimer < 128) {
                self->lastSpherePos.x = self->playerPos.x;
                self->lastSpherePos.y = self->playerPos.y;
                BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;
                if (!self->ringLoopFlag) {
                    CREATE_ENTITY(BSS_Collected, intToVoid(BSS_COLLECTED_BLUE), self->playerPos.x, self->playerPos.y);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }

                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    self->state            = BSS_Setup_Finished;
                    RSDK.PlaySfx(BSS_Setup->sfxSSJettison, false, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
                }
            }
            break;
        case BSS_SPHERE_RED:
            if (self->state != BSS_Setup_State_Exit && self->globeTimer < 32) {
                self->state        = BSS_Setup_State_Exit;
                self->stopMovement = true;
                self->spinTimer    = 0;
                self->globeTimer   = 0;
                RSDK.PlaySfx(BSS_Setup->sfxSSExit, false, 255);
                Music_FadeOut(0.0125);
            }
            break;
        case BSS_SPHERE_BUMPER:
            if (!self->bumperFlag && self->globeTimer < 112) {
                if (self->globeTimer > 16) {
                    if (self->globeSpeed < 0) {
                        self->bumperFlag      = true;
                        self->globeSpeed      = -self->globeSpeed;
                        self->playerWasBumped = false;
                        RSDK.PlaySfx(BSS_Setup->sfxBumper, false, 255);
                    }
                }
                else if (!self->spinState) {
                    if (self->globeSpeed < 0) {
                        self->globeTimer      = 16;
                        self->bumperFlag      = true;
                        self->globeSpeed      = -self->globeSpeed;
                        self->playerWasBumped = false;
                        RSDK.PlaySfx(BSS_Setup->sfxBumper, false, 255);
                    }
                }
            }
            break;
        case BSS_SPHERE_YELLOW:
            if (self->globeTimer < 128) {
                EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
                player->velocity.y       = -0x180000;
                player->onGround         = 0;
                RSDK.SetSpriteAnimation(player->aniFrames, 3, &player->animator, false, 0);
                BSS_Player->unused1 = 4;
                self->globeSpeed *= 2;
                self->spinState     = 0;
                self->globeSpeedInc = 4;
                RSDK.PlaySfx(BSS_Setup->sfxSpring, false, 255);
            }
            break;
        case BSS_SPHERE_GREEN:
            if (self->globeTimer > 128) {
                CREATE_ENTITY(BSS_Collected, intToVoid(BSS_COLLECTED_GREEN), self->playerPos.x, self->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
            }
            break;
        case BSS_SPHERE_PINK:
            if (self->state != BSS_Setup_State_PinkSphereWarp && self->globeTimer < 64) {
                self->state      = BSS_Setup_State_PinkSphereWarp;
                self->spinTimer  = 0;
                self->globeTimer = 0;
                RSDK.PlaySfx(BSS_Setup->sfxTeleport, false, 255);
                EntityFXFade *fade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
                fade->speedIn      = 32;
                fade->speedOut     = 32;
                fade->wait         = 48;
            }
            break;
        case BSS_RING:
            if (self->globeTimer < 128) {
                CREATE_ENTITY(BSS_Collected, intToVoid(BSS_COLLECTED_RING), self->playerPos.x, self->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;
        default: break;
    }

    int32 posX      = (self->playerPos.x + (RSDK.Sin256(self->angle) >> 8)) & 0x1F;
    int32 posY      = (self->playerPos.y - (RSDK.Cos256(self->angle) >> 8)) & 0x1F;
    fieldPos        = posY + (BSS_PLAYFIELD_H * posX);
    int32 fieldData = BSS_Setup->playField[fieldPos];
    switch (fieldData) {
        case BSS_SPHERE_BLUE:
            if (self->globeTimer > 128) {
                self->lastSpherePos.x = posX;
                self->lastSpherePos.y = posY;
                BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;
                if (!self->ringLoopFlag) {
                    CREATE_ENTITY(BSS_Collected, intToVoid(BSS_COLLECTED_BLUE), posX, posY);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }
                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    RSDK.PlaySfx(BSS_Setup->sfxMedal, false, 255);
                    self->state = BSS_Setup_Finished;
                    RSDK.PlaySfx(BSS_Setup->sfxSSJettison, false, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
                }
            }
            break;
        case BSS_SPHERE_RED:
            if (self->state != BSS_Setup_State_Exit && self->globeTimer > 224) {
                self->palettePage ^= 1;
                self->state      = BSS_Setup_State_Exit;
                self->spinTimer  = 0;
                self->globeTimer = 0;
                self->playerPos.x += RSDK.Sin256(self->angle) >> 8;
                self->playerPos.x &= 0x1F;
                self->playerPos.y -= RSDK.Cos256(self->angle) >> 8;
                self->playerPos.y &= 0x1F;
                RSDK.PlaySfx(BSS_Setup->sfxSSExit, false, 255);
                Music_FadeOut(0.0125);
            }
            break;
        case BSS_SPHERE_BUMPER:
            if (!self->bumperFlag && self->globeTimer > 144) {
                if (self->globeTimer >= 240) {
                    if (!self->spinState) {
                        if (self->globeSpeed > 0) {
                            self->globeTimer      = 240;
                            self->bumperFlag      = true;
                            self->globeSpeed      = -self->globeSpeed;
                            self->playerWasBumped = true;
                            RSDK.PlaySfx(BSS_Setup->sfxBumper, false, 255);
                        }
                    }
                }
                else {
                    if (self->globeSpeed > 0) {
                        self->bumperFlag      = true;
                        self->globeSpeed      = -self->globeSpeed;
                        self->playerWasBumped = true;
                        RSDK.PlaySfx(BSS_Setup->sfxBumper, false, 255);
                    }
                }
            }
            break;
        case BSS_SPHERE_YELLOW:
            if (self->globeTimer > 128) {
                EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
                player->velocity.y       = -0x180000;
                player->onGround         = 0;
                RSDK.SetSpriteAnimation(player->aniFrames, 3, &player->animator, 0, 0);
                BSS_Player->unused1 = 4;
                self->globeSpeed *= 2;
                self->spinState     = 0;
                self->globeSpeedInc = 4;
                RSDK.PlaySfx(BSS_Setup->sfxSpring, false, 255);
            }
            break;
        case BSS_SPHERE_GREEN:
            if (self->globeTimer > 128) {
                CREATE_ENTITY(BSS_Collected, intToVoid(BSS_COLLECTED_GREEN), posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
            }
            break;
        case BSS_RING:
            if (self->globeTimer > 128) {
                CREATE_ENTITY(BSS_Collected, intToVoid(BSS_COLLECTED_RING), posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;
        case BSS_EMERALD_CHAOS:
        case BSS_EMERALD_SUPER:
        case BSS_MEDAL_SILVER:
        case BSS_MEDAL_GOLD:
            if (self->globeTimer > 240) {
                EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
                if (!param->selectionType && globals->gameMode < MODE_TIMEATTACK) {
                    int32 pos = BSS_Setup_GetStageID();
                    if (pos >= 0) {
                        EntityGameProgress *progress = GameProgress_GetGameProgress();
                        if (progress) {
                            uint8 medal = fieldData == BSS_MEDAL_SILVER ? 1 : fieldData == BSS_MEDAL_GOLD ? 2 : 0;

                            if (medal)
                                GameProgress_GiveMedal(pos, medal);

                            if (progress->allGoldMedals && progress->goldMedalCount == 31)
                                API_UnlockAchievement("ACH_GOLD_MEDAL");
                            if (progress->allSilverMedals && progress->silverMedalCount == 31)
                                API_UnlockAchievement("ACH_SILVER_MEDAL");
                        }
                    }
                }

                self->palettePage ^= 1;
                self->state      = BSS_Setup_State_Exit;
                self->spinTimer  = 0;
                self->globeTimer = 0;
                self->playerPos.x += RSDK.Sin256(self->angle) >> 8;
                self->playerPos.x &= 0x1F;
                self->playerPos.y -= RSDK.Cos256(self->angle) >> 8;
                self->playerPos.y &= 0x1F;
                globals->specialCleared = true;
                RSDK.PlaySfx(BSS_Setup->sfxSSExit, false, 255);
            }
            break;
        default: break;
    }
}

void BSS_Setup_HandleCollectableMovement(void)
{
    RSDK_THIS(BSS_Setup);
    self->offsetDir = self->angle >> 6;
    int32 off       = BSS_Setup->frustumOffset[(self->angle & 0x3F) != 0];
    int32 id        = BSS_Setup->frustumCount[(self->angle & 0x3F) != 0];
    Vector2 *offset = &BSS_Setup->offsetTable[off];

    int32 slot = RESERVE_ENTITY_COUNT;
    while (id > 0) {
        switch (self->offsetDir) {
            case FLIP_NONE:
                self->offset.x = offset->x;
                self->offset.y = offset->y;
                break;
            case FLIP_X:
                self->offset.x = -offset->y;
                self->offset.y = offset->x;
                break;
            case FLIP_Y:
                self->offset.x = offset->x;
                self->offset.y = -offset->y;
                break;
            case FLIP_XY:
                self->offset.x = offset->y;
                self->offset.y = offset->x;
                break;
            default: break;
        }

        uint16 tile =
            BSS_Setup->playField[((self->offset.y + self->playerPos.y) & 0x1F) + (BSS_PLAYFIELD_H * ((self->offset.x + self->playerPos.x) & 0x1F))];
        if (tile) {
            EntityBSS_Collectable *collectable = (EntityBSS_Collectable *)RSDK.GetEntityByID(slot);
            int32 x                            = (self->offset.x * RSDK.Cos256(self->angle) + self->offset.y * RSDK.Sin256(self->angle)) >> 4;
            int32 y                            = (self->offset.y * RSDK.Cos256(self->angle) - self->offset.x * RSDK.Sin256(self->angle)) >> 4;
            y                                  = -(y + self->paletteLine - 16);
            if (y < 0) {
                collectable->objectID = TYPE_BLANK;
            }
            else {
                collectable->objectID = BSS_Collectable->objectID;
                collectable->type     = tile & 0x3FF;
                if (y < 112) {
                    self->xMultiplier    = BSS_Setup->xMultiplierTable[y];
                    self->divisor        = BSS_Setup->divisorTable[y];
                    collectable->frameID = BSS_Setup->frameTable[y];
                    collectable->frameID -= abs(x) >> 5;
                    if (collectable->frameID < 0)
                        collectable->frameID = 0;

                    int32 finalX = self->xMultiplier * x;
                    int32 distX  = finalX * finalX >> 16;

                    int32 worldX = 0;
                    if (finalX <= 0)
                        worldX = (distX + finalX) >> 4;
                    else
                        worldX = (finalX - distX) >> 4;

                    collectable->position.x = (worldX + ScreenInfo->centerX) << 16;
                    collectable->position.y = (BSS_Setup->screenYTable[y] + worldX * worldX / self->divisor) << 16;
                    ++slot;
                }
            }
        }
        id--;
        ++offset;
    }

    while (slot < RESERVE_ENTITY_COUNT + 0x80) {
        Entity *entity = RSDK.GetEntityByID(slot++);
        entity->objectID = TYPE_BLANK;
    }
}

void BSS_Setup_State_FinishWalk(void)
{
    RSDK_THIS(BSS_Setup);
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;
    self->globeTimer += self->globeSpeed;
    if (++self->spinTimer == 120)
        RSDK.PlaySfx(BSS_Setup->sfxMedalCaught, false, 255);
    BSS_Setup_HandleSteppedObjects();

    if (self->globeSpeed <= 0 && self->globeTimer < 0) {
        self->palettePage ^= 1;
        self->globeTimer += 256;
        self->playerPos.x -= RSDK.Sin256(self->angle) >> 8;
        self->playerPos.y += RSDK.Cos256(self->angle) >> 8;
    }
    else if (self->globeTimer >= 0x100) {
        self->palettePage ^= 1;
        self->globeTimer -= 256;
        self->playerPos.x += RSDK.Sin256(self->angle) >> 8;
        self->playerPos.y -= RSDK.Cos256(self->angle) >> 8;
    }
    self->playerPos.x &= 0x1F;
    self->playerPos.y &= 0x1F;
    self->paletteLine = (self->globeTimer >> 4) & 0xF;
    TileLayer *bg     = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollPos += self->globeSpeed << 14;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_PinkSphereWarp(void)
{
    RSDK_THIS(BSS_Setup);
    self->alpha += 8;
    if (self->alpha == 320) {
        EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        EntityBSS_Player *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, BSS_Player);
        RSDK.SetSpriteAnimation(player1->aniFrames, 0, &player1->animator, true, 0);
        RSDK.SetSpriteAnimation(player2->aniFrames, 0, &player2->animator, true, 0);

        int32 count = BSS_Setup->pinkSphereCount;
        int32 val   = RSDK.Rand(0, count - 1);
        bool32 flag = false;
        for (; (count && val >= 0) && !flag; --count) {
            for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    uint16 tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
                    if ((tile & 0x7F) == BSS_SPHERE_PINK && (x != self->playerPos.x || y != self->playerPos.y) && --val < 0) {
                        self->playerPos.x = x;
                        self->playerPos.y = y;

                        x    = 0x20;
                        y    = 0x20;
                        flag = true;
                    }
                }
            }
        }

        val  = RSDK.Rand(0, 4);
        flag = false;
        for (int32 i = 0; i < 4; ++i) {
            int32 x = self->playerPos.x;
            int32 y = self->playerPos.y;
            switch (val) {
                case 0: y = (y - 1) & 0x1F; break;
                case 1: x = (x + 1) & 0x1F; break;
                case 2: y = (y + 1) & 0x1F; break;
                case 3: x = (x - 1) & 0x1F; break;
                default: break;
            }
            uint16 tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
            if (tile < BSS_SPHERE_RED || (tile > BSS_SPHERE_BUMPER && tile != BSS_SPHERE_PINK)) {
                flag = true;
                break;
            }

            val = (val + 1) & 3;
        }

        if (!flag) {
            for (int32 i = 0; i < 4; ++i) {
                int32 x = self->playerPos.x;
                int32 y = self->playerPos.y;
                switch (val) {
                    case 0: y = (y - 2) & 0x1F; break;
                    case 1: x = (x + 2) & 0x1F; break;
                    case 2: y = (y + 2) & 0x1F; break;
                    case 3: x = (x - 2) & 0x1F; break;
                    default: break;
                }

                uint16 tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
                if (tile < BSS_SPHERE_RED || (tile > BSS_SPHERE_BUMPER && tile != BSS_SPHERE_PINK)) {
                    flag = true;
                    break;
                }

                val = (val + 1) & 3;
            }
        }

        self->angle = val << 6;
        CREATE_ENTITY(BSS_Collected, intToVoid(BSS_COLLECTED_PINK), self->playerPos.x, self->playerPos.y);
        BSS_Setup->playField[self->playerPos.y + (BSS_PLAYFIELD_H * self->playerPos.x)] = BSS_SPHERE_PINK_STOOD;
        self->timer                                                                     = 100;
        self->state                                                                     = BSS_Setup_State_Unknown23;
    }
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_Exit(void)
{
    RSDK_THIS(BSS_Setup);

    PauseMenu->disableEvents = true;
    self->maxSpeed           = 0;
    if (self->spinTimer <= 0) {
        CREATE_ENTITY(BSS_Message, intToVoid(BSS_MESSAGE_2_FINISHED), self->position.x, self->position.y);
        foreach_active(BSS_Player, player) { player->stateInput = StateMachine_None; }
    }
    else {
        TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
        bg->scrollInfo[0].scrollPos -= 0x200000;
        self->angle -= 8;
        self->angle &= 0xFF;
    }

    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (self->spinTimer & 0xF) {
        globe->drawLayer[0]             = DRAWLAYER_COUNT;
        int32 timer                     = self->spinTimer & 0xF;
        self->globeSpinAnimator.frameID = BSS_Setup->globeFrameTable[timer - 1];
        self->direction                 = BSS_Setup->globeDirTableL[timer - 1];
    }
    else {
        if (self->spinTimer > 0) {
            self->palettePage ^= 1;
            globe->drawLayer[0] = 1;
        }
    }
    self->spinTimer += 2;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_HandleStage(void)
{
    RSDK_THIS(BSS_Setup);
    EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);

    if (self->maxSpeed < 32 && ++self->speedupTimer >= self->speedupInterval) {
        self->speedupTimer = 0;
        self->maxSpeed += 4;
    }
    RSDK.GetSceneLayer(BSS_Setup->globeLayer)->drawLayer[0] = 1;

    if (self->playerWasBumped) {
        if (!self->bumperFlag && player1->up)
            self->playerWasBumped = false;
    }
    else {
        if (self->globeSpeed < self->maxSpeed)
            self->globeSpeed += self->globeSpeedInc;
    }

    if (player1->onGround) {
        if (self->globeTimer > 0 && self->globeTimer < 256) {
            if (player1->left)
                self->spinState = 1;
            if (player1->right)
                self->spinState = 2;
        }
        self->globeTimer += self->globeSpeed;
        BSS_Setup_HandleSteppedObjects();
    }
    else {
        self->globeTimer += self->globeSpeed;
        self->spinState = 0;
    }

    if (self->state == BSS_Setup_State_HandleStage) {
        if (self->globeSpeed > 0) {
            if (self->globeTimer >= 0x100) {
                if (self->spinState == 1) {
                    self->state      = BSS_Setup_State_SpinLeft;
                    self->globeTimer = 0;
                }
                else {
                    if (self->spinState == 2) {
                        self->state      = BSS_Setup_State_SpinRight;
                        self->globeTimer = 0;
                    }
                    if (self->spinState == 0) {
                        self->globeTimer -= 256;
                    }
                }

                self->palettePage ^= 1;
                self->spinState = 0;

                self->playerPos.x += RSDK.Sin256(self->angle) >> 8;
                self->playerPos.x &= 0x1F;
                self->playerPos.y -= RSDK.Cos256(self->angle) >> 8;
                self->playerPos.y &= 0x1F;
            }
        }
        else if (self->globeTimer < 0) {
            if (self->spinState == 1) {
                self->state      = BSS_Setup_State_SpinLeft;
                self->globeTimer = 0;
                self->spinState  = 0;
            }
            else {
                if (self->spinState == 2) {
                    self->state      = BSS_Setup_State_SpinRight;
                    self->globeTimer = 0;
                    self->spinState  = 0;
                }
                else if (self->spinState == 0) {
                    self->globeTimer += 256;
                    self->palettePage ^= 1;

                    self->playerPos.x -= RSDK.Sin256(self->angle) >> 8;
                    self->playerPos.x &= 0x1F;
                    self->playerPos.y += RSDK.Cos256(self->angle) >> 8;
                    self->playerPos.y &= 0x1F;

                    self->spinState = 0;
                }
            }
        }

        TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
        bg->scrollPos += self->globeSpeed << 14;
    }

    self->paletteLine = (self->globeTimer >> 4) & 0xF;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_SpinLeft(void)
{
    RSDK_THIS(BSS_Setup);

    if (self->maxSpeed < 32 && ++self->speedupTimer >= self->speedupInterval) {
        self->speedupTimer = 0;
        self->maxSpeed += 4;
    }
    TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollInfo[0].scrollPos -= 0x100000;

    self->angle -= 4;
    self->angle &= 0xFF;
    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (self->spinTimer == 15) {
        self->spinTimer     = 0;
        globe->drawLayer[0] = 1;
        self->palettePage ^= 1;
        if (!self->timer)
            self->state = BSS_Setup_State_HandleStage;
        else
            self->state = BSS_Setup_State_Unknown23;
        BSS_Setup_HandleCollectableMovement();
    }
    else {
        globe->drawLayer[0] = DRAWLAYER_COUNT;

        self->globeSpinAnimator.frameID = BSS_Setup->globeFrameTable[self->spinTimer];
        self->direction                 = BSS_Setup->globeDirTableL[self->spinTimer];
        self->spinTimer++;
        if (self->timer > 1)
            self->timer--;
        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_SpinRight(void)
{
    RSDK_THIS(BSS_Setup);

    if (self->maxSpeed < 32 && ++self->speedupTimer >= self->speedupInterval) {
        self->speedupTimer = 0;
        self->maxSpeed += 4;
    }
    TileLayer *bg = RSDK.GetSceneLayer(BSS_Setup->bgLayer);
    bg->scrollInfo[0].scrollPos += 0x100000;

    self->angle += 4;
    self->angle &= 0xFF;
    TileLayer *globe = RSDK.GetSceneLayer(BSS_Setup->globeLayer);
    if (self->spinTimer == 15) {
        self->spinTimer     = 0;
        globe->drawLayer[0] = 1;
        if (!self->timer)
            self->state = BSS_Setup_State_HandleStage;
        else
            self->state = BSS_Setup_State_Unknown23;
        BSS_Setup_HandleCollectableMovement();
    }
    else {
        globe->drawLayer[0] = DRAWLAYER_COUNT;

        if (!self->spinTimer)
            self->palettePage ^= 1;
        self->globeSpinAnimator.frameID = BSS_Setup->globeFrameTable[self->spinTimer];
        self->direction                 = BSS_Setup->globeDirTableR[self->spinTimer];
        self->spinTimer++;
        if (self->timer > 1)
            self->timer--;
        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_Unknown23(void)
{
    RSDK_THIS(BSS_Setup);

    EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
    if (self->alpha <= 0) {
        if (player->up) {
            self->timer = 1;
        }
        else if (player->left) {
            self->state = BSS_Setup_State_SpinLeft;
        }
        else if (player->right) {
            self->state = BSS_Setup_State_SpinRight;
        }
    }
    else {
        self->alpha -= 8;
    }

    if (!--self->timer) {
        self->state = BSS_Setup_State_HandleStage;

        EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        if (player1->onGround)
            RSDK.SetSpriteAnimation(player1->aniFrames, 1, &player1->animator, 0, 0);

        EntityBSS_Player *player2 = (EntityBSS_Player *)RSDK.GetEntityByID(SLOT_PLAYER2);
        if (player2->onGround)
            RSDK.SetSpriteAnimation(player2->aniFrames, 1, &player2->animator, 0, 0);
    }
    BSS_Setup_HandleCollectableMovement();
}

bool32 BSS_Setup_CheckSphereValid(int32 x, int32 y)
{
    int32 x1 = BSS_PLAYFIELD_H * (((uint8)x - 1) & 0x1F);
    int32 y1 = ((uint8)y - 1) & 0x1F;
    int32 x2 = BSS_PLAYFIELD_H * (((uint8)x + 1) & 0x1F);
    int32 y2 = ((uint8)y + 1) & 0x1F;

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
    int32 x                = self->spinTimer + 0x100;
    int32 y                = self->spinTimer << 17;
    RSDKScreenInfo *screen = ScreenInfo;

    int32 i                            = RESERVE_ENTITY_COUNT;
    EntityBSS_Collectable *collectable = NULL;

    collectable = RSDK_GET_ENTITY(i++, BSS_Collectable);
    while (collectable->objectID != TYPE_BLANK) {
        int32 ix                = (collectable->position.x >> 16);
        collectable->position.x = ((x * (ix - screen->centerX) >> 8) + screen->centerX) << 16;
        collectable->position.y -= y;
        collectable = RSDK_GET_ENTITY(i++, BSS_Collectable);
    }
}

void BSS_Setup_StageFinishClear(void)
{
    RSDK_THIS(BSS_Setup);

    for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
            BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
        }
    }

    int32 fx       = (RSDK.Sin256(self->angle) >> 5) + self->playerPos.x;
    int32 fy       = (((uint8)self->playerPos.y - (uint8)(RSDK.Cos256(self->angle) >> 5)) & 0x1F);
    int32 fieldPos = fy + (BSS_PLAYFIELD_H * (fx & 0x1F));

    if (BSS_Setup->ringCount > 0)
        BSS_Setup->playField[fieldPos] = BSS_MEDAL_SILVER;
    else
        BSS_Setup->playField[fieldPos] = BSS_MEDAL_GOLD;
    RSDK_GET_ENTITY(RESERVE_ENTITY_COUNT, BSS_Collectable)->drawOrder     = 3;
    RSDK_GET_ENTITY(RESERVE_ENTITY_COUNT + 1, BSS_Collectable)->drawOrder = 3;
}

bool32 BSS_Setup_ScanSphereChain_Up(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);
    if (self->ringLoopFlag)
        return true;

    int32 px = BSS_PLAYFIELD_H * x;
    int32 id = 0;

    int32 y1 = y;
    while (true) {
        y1 = ((uint8)y1 - 1) & 0x1F;
        if ((BSS_Setup->playField[px + y1] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->playField2[px + y1] & 0x7F) == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_CheckSphereValid(x, y1))
            break;

        BSS_Setup->playField2[y1 + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y1 + px] = BSS_SPHERE_BLUE;
        if (x == self->lastSpherePos.x && y1 == self->lastSpherePos.y) {
            self->ringLoopFlag = true;
            return true;
        }
        bool32 flag = false;

        int32 fieldPos  = y1 + (BSS_PLAYFIELD_H * (((uint8)x + 1) & 0x1F));
        int32 fieldPos2 = y1 + (BSS_PLAYFIELD_H * (((uint8)x - 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Right(x, y1);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Left(x, y1);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (self->ringLoopFlag)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        y1                             = ((uint8)y1 + 1) & 0x1F;
        BSS_Setup->playField3[px + y1] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_ScanSphereChain_Down(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);
    if (self->ringLoopFlag)
        return true;

    int32 px = BSS_PLAYFIELD_H * x;
    int32 id = 0;
    int32 y1 = y;

    while (true) {
        y1 = ((uint8)y1 + 1) & 0x1F;
        if ((BSS_Setup->playField[px + y1] & 0x7F) != BSS_SPHERE_RED)
            break;

        if (BSS_Setup->playField2[px + y1] == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_CheckSphereValid(x, y1)) {
            break;
        }

        BSS_Setup->playField2[y1 + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y1 + px] = BSS_SPHERE_BLUE;
        if (x == self->lastSpherePos.x && y1 == self->lastSpherePos.y) {
            self->ringLoopFlag = true;
            return true;
        }

        bool32 flag = false;

        int32 fieldPos  = y1 + (BSS_PLAYFIELD_H * (((uint8)x - 1) & 0x1F));
        int32 fieldPos2 = y1 + (BSS_PLAYFIELD_H * (((uint8)x + 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Left(x, y1);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Right(x, y1);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (self->ringLoopFlag)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        y1                             = ((uint8)y1 - 1) & 0x1F;
        BSS_Setup->playField3[px + y1] = BSS_NONE;
    }
    return false;
}
bool32 BSS_Setup_ScanSphereChain_Left(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);
    if (self->ringLoopFlag)
        return true;

    int32 id = 0;

    int32 x1 = x;
    while (true) {
        x1       = ((uint8)x1 - 1) & 0x1F;
        int32 px = (BSS_PLAYFIELD_H * x1);
        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->playField2[px + y] & 0x7F) == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_CheckSphereValid(x1, y))
            break;

        BSS_Setup->playField2[y + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y + px] = BSS_SPHERE_BLUE;
        if (x1 == self->lastSpherePos.x && y == self->lastSpherePos.y) {
            self->ringLoopFlag = true;
            return true;
        }
        bool32 flag = false;

        int32 fieldPos  = (BSS_PLAYFIELD_H * x1) + (((uint8)y - 1) & 0x1F);
        int32 fieldPos2 = (BSS_PLAYFIELD_H * x1) + (((uint8)y + 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Up(x1, y);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Down(x1, y);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (self->ringLoopFlag)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        x1                            = ((uint8)x1 + 1) & 0x1F;
        int32 px                      = (BSS_PLAYFIELD_H * x1);
        BSS_Setup->playField3[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_ScanSphereChain_Right(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);
    if (self->ringLoopFlag)
        return true;

    int32 id = 0;

    int32 x1 = x;
    while (true) {
        x1       = ((uint8)x1 + 1) & 0x1F;
        int32 px = (BSS_PLAYFIELD_H * x1);
        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->playField2[px + y] & 0x7F) == BSS_SPHERE_BLUE)
            break;
        if (!BSS_Setup_CheckSphereValid(x1, y))
            break;

        BSS_Setup->playField2[y + px] = BSS_SPHERE_BLUE;
        BSS_Setup->playField3[y + px] = BSS_SPHERE_BLUE;
        if (x1 == self->lastSpherePos.x && y == self->lastSpherePos.y) {
            self->ringLoopFlag = true;
            return true;
        }
        bool32 flag = false;

        int32 fieldPos  = (BSS_PLAYFIELD_H * x1) + (((uint8)y + 1) & 0x1F);
        int32 fieldPos2 = (BSS_PLAYFIELD_H * x1) + (((uint8)y - 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPos] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Down(x1, y);
        }
        if ((BSS_Setup->playField[fieldPos2] & 0x7F) == BSS_SPHERE_RED) {
            flag |= BSS_Setup_ScanSphereChain_Up(x1, y);
        }

        if (!flag)
            id++;
        else
            id = 0;
        if (self->ringLoopFlag)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        x1                            = ((uint8)x1 - 1) & 0x1F;
        int32 px                      = (BSS_PLAYFIELD_H * x1);
        BSS_Setup->playField3[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_GetChainedSphereCount(uint8 x, uint8 y)
{
    int32 px = BSS_PLAYFIELD_H * x;

    int32 y1 = ((uint8)y - 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_H; ++i) {
        if (BSS_Setup->playField3[px + y1] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[px + y1] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            y1 = ((uint8)y1 - 1) & 0x1F;
        }
    }

    y1 = ((uint8)y + 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_H; ++i) {
        if (BSS_Setup->playField3[px + y1] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[px + y1] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            y1 = ((uint8)y1 + 1) & 0x1F;
        }
    }

    int32 x1 = ((uint8)x - 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_W; ++i) {
        if (BSS_Setup->playField3[y + (BSS_PLAYFIELD_W * x1)] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[y + (BSS_PLAYFIELD_W * x1)] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            x1 = ((uint8)x1 - 1) & 0x1F;
        }
    }

    x1 = ((uint8)x + 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_W; ++i) {
        if (BSS_Setup->playField3[y + (BSS_PLAYFIELD_W * x1)] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[y + (BSS_PLAYFIELD_W * x1)] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            x1 = ((uint8)x1 + 1) & 0x1F;
        }
    }

    BSS_Setup->playField3[px + y] = BSS_SPHERE_BLUE;
    return true;
}

void BSS_Setup_ProcessChain(void)
{
    RSDK_THIS(BSS_Setup);

    for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
            BSS_Setup->playField2[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
        }
    }
    BSS_Setup->playField[self->lastSpherePos.y + (BSS_PLAYFIELD_H * self->lastSpherePos.x)]  = BSS_SPHERE_RED;
    BSS_Setup->playField3[self->lastSpherePos.y + (BSS_PLAYFIELD_H * self->lastSpherePos.x)] = BSS_SPHERE_BLUE;

    self->ringLoopFlag = false;
    BSS_Setup_ScanSphereChain_Up(self->lastSpherePos.x, self->lastSpherePos.y);
    BSS_Setup_ScanSphereChain_Down(self->lastSpherePos.x, self->lastSpherePos.y);
    BSS_Setup_ScanSphereChain_Left(self->lastSpherePos.x, self->lastSpherePos.y);
    BSS_Setup_ScanSphereChain_Right(self->lastSpherePos.x, self->lastSpherePos.y);

    BSS_Setup->playField[self->lastSpherePos.y + (BSS_PLAYFIELD_H * self->lastSpherePos.x)] = BSS_SPHERE_BLUE;
    if (self->ringLoopFlag) {
        int32 spheresCollected = 0;

        for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
            for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                if ((BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] & 0x7F) == BSS_SPHERE_BLUE) {
                    spheresCollected += BSS_Setup_GetChainedSphereCount(x, y);
                }
            }
        }

        if (spheresCollected <= 0) {
            self->ringLoopFlag = false;
        }
        else {
            for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    int32 pitch = x * BSS_PLAYFIELD_H;

                    if (BSS_Setup->playField3[pitch + y] == BSS_SPHERE_BLUE) {
                        int32 y1 = ((uint8)y - 1) & 0x1F;
                        if ((BSS_Setup->playField[y1 + pitch] & 0x7F) != BSS_SPHERE_BLUE) {
                            int32 y2 = ((uint8)y + 1) & 0x1F;
                            if ((BSS_Setup->playField[y2 + pitch] & 0x7F) != BSS_SPHERE_BLUE) {
                                int32 x1 = BSS_PLAYFIELD_H * (((uint8)x - 1) & 0x1F);
                                if ((BSS_Setup->playField[x1 + y] & 0x7F) != BSS_SPHERE_BLUE) {
                                    int32 x2 = BSS_PLAYFIELD_H * (((uint8)x + 1) & 0x1F);
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

            for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    if (BSS_Setup->playField3[(x * BSS_PLAYFIELD_H) + y]) {
                        BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] = BSS_RING;
                    }
                }
            }

            BSS_Setup->sphereCount -= spheresCollected;
            RSDK.PlaySfx(BSS_Setup->sfxLoseRings, false, 0xFF);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void BSS_Setup_EditorDraw(void) {}

void BSS_Setup_EditorLoad(void) {}
#endif

void BSS_Setup_Serialize(void) { RSDK_EDITABLE_VAR(BSS_Setup, VAR_UINT8, paletteID); }
