// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BSS_Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"
#include <time.h>

ObjectBSS_Setup *BSS_Setup;

void BSS_Setup_Update(void)
{
    RSDK_THIS(BSS_Setup);

    StateMachine_Run(self->state);

    ScreenInfo->position.x = 0x100 - ScreenInfo->center.x;

    if (self->palettePage) {
        RSDK.CopyPalette(2, 16 * self->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(1, 16 * self->paletteLine, 0, 144, 16);
    }
    else {
        RSDK.CopyPalette(1, 16 * self->paletteLine, 0, 128, 16);
        RSDK.CopyPalette(2, 16 * self->paletteLine, 0, 144, 16);
    }

#if MANIA_USE_PLUS
    EntityMenuParam *param = MenuParam_GetParam();
    if (param->bssSelection == BSS_SELECTION_NONE && !self->stopMovement && globals->gameMode < MODE_TIMEATTACK)
        ++SaveGame_GetSaveRAM()->zoneTimes[28];
#endif
}

void BSS_Setup_LateUpdate(void) {}

void BSS_Setup_StaticUpdate(void) {}

void BSS_Setup_Draw(void)
{
    RSDK_THIS(BSS_Setup);

    TileLayer *globe = RSDK.GetTileLayer(BSS_Setup->globeLayer);
    self->inkEffect  = INK_NONE;
    if (globe->drawGroup[0] == DRAWGROUP_COUNT)
        RSDK.DrawSprite(&self->globeSpinAnimator, NULL, false);

    Vector2 drawPos;
    self->inkEffect = INK_BLEND;
    drawPos.x       = self->position.x;
    drawPos.y       = TO_FIXED(158);
    RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);
}

void BSS_Setup_Create(void *data)
{
    RSDK_THIS(BSS_Setup);

    if (!SceneInfo->inEditor) {
        self->active          = ACTIVE_BOUNDS;
        self->visible         = true;
        self->drawGroup       = 2;
        self->drawFX          = FX_FLIP;
        self->position.x      = TO_FIXED(256);
        self->updateRange.x   = TO_FIXED(128);
        self->updateRange.y   = TO_FIXED(128);
        self->speedupInterval = 30 * 60; // speed up every 30 seconds
        self->stopMovement    = false;

        RSDK.SetSpriteAnimation(BSS_Setup->globeFrames, 0, &self->globeSpinAnimator, true, 0);
        RSDK.SetSpriteAnimation(BSS_Setup->globeFrames, 1, &self->shadowAnimator, true, 0);

        BSS_Setup_GetStartupInfo();

        self->state = BSS_Setup_State_GlobeMoveZ;
    }
}

void BSS_Setup_StageLoad(void)
{
    BSS_Setup->globeFrames = RSDK.LoadSpriteAnimation("SpecialBS/Globe.bin", SCOPE_STAGE);

    BSS_Setup->bgLayer        = RSDK.GetTileLayerID("Background");
    BSS_Setup->globeLayer     = RSDK.GetTileLayerID("Globe");
    BSS_Setup->frustum1Layer  = RSDK.GetTileLayerID("Frustum 1");
    BSS_Setup->frustum2Layer  = RSDK.GetTileLayerID("Frustum 2");
    BSS_Setup->playFieldLayer = RSDK.GetTileLayerID("Playfield");
    BSS_Setup->ringCountLayer = RSDK.GetTileLayerID("Ring Count");

    BSS_Setup_SetupFrustum();
    BSS_Setup->ringCount = 0;

    TileLayer *playField = RSDK.GetTileLayer(BSS_Setup->playFieldLayer);

    memset(BSS_Setup->playField, 0, sizeof(BSS_Setup->playField));
    memset(BSS_Setup->sphereChainTable, 0, sizeof(BSS_Setup->sphereChainTable));
    memset(BSS_Setup->sphereCollectedTable, 0, sizeof(BSS_Setup->sphereCollectedTable));

    if (playField->width <= BSS_PLAYFIELD_W) {
        for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
            for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                uint16 tile = RSDK.GetTile(BSS_Setup->playFieldLayer, x, y);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 24)
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;

                if ((RSDK.GetTile(BSS_Setup->ringCountLayer, x, y) & 0x3FF) == BSS_RING)
                    ++BSS_Setup->ringCount;
            }
        }
    }
    else {
        int32 max = (playField->width >> 4) * ((playField->height - 32) >> 4);
        int32 key = (int32)time(0);

        // Randomly generate a random number (same format as the random numbers from the S3 blue spheres game)
        BSS_Setup->randomNumbers[0] = RSDK.RandSeeded(0, max, &key);
        BSS_Setup->randomNumbers[1] = RSDK.RandSeeded(0, max, &key);
        BSS_Setup->randomNumbers[2] = RSDK.RandSeeded(0, max, &key);
        BSS_Setup->randomNumbers[3] = RSDK.RandSeeded(0, max, &key);

        BSS_Palette->startColorID = 16 * (BSS_Setup->randomNumbers[1] & 0x0F);

        // Top Left Quadrant
        for (int32 y = 0, py = 0; y < BSS_PLAYFIELD_H / 2; ++y, ++py) {
            for (int32 x = 0, px = 0; x < BSS_PLAYFIELD_W / 2; ++x, ++px) {
                int32 tx = px + (0x10 * (BSS_Setup->randomNumbers[0] & 0x0F));
                int32 ty = py + BSS_PLAYFIELD_H + (BSS_Setup->randomNumbers[0] & 0xF0);

                uint16 tile = RSDK.GetTile(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 24)
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;

                if ((RSDK.GetTile(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING)
                    ++BSS_Setup->ringCount;
            }
        }

        // Top Right Quadrant
        for (int32 y = 0, py = 0; y < BSS_PLAYFIELD_H / 2; ++y, ++py) {
            for (int32 x = BSS_PLAYFIELD_W / 2, px = 0; x < BSS_PLAYFIELD_W; ++x, ++px) {
                int32 tx = 0x10 * (BSS_Setup->randomNumbers[1] & 0x0F) - px + 0x0F;
                int32 ty = py + BSS_PLAYFIELD_H + (BSS_Setup->randomNumbers[1] & 0xF0);

                uint16 tile = RSDK.GetTile(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 24)
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;

                if ((RSDK.GetTile(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING)
                    ++BSS_Setup->ringCount;
            }
        }

        // Bottom Left Quadrant
        for (int32 y = BSS_PLAYFIELD_H / 2, py = 0; y < BSS_PLAYFIELD_H; ++y, ++py) {
            for (int32 x = 0, px = 0; x < BSS_PLAYFIELD_W / 2; ++x, ++px) {
                int32 tx = px + (0x10 * (BSS_Setup->randomNumbers[2] & 0x0F));
                int32 ty = (BSS_Setup->randomNumbers[2] & 0xF0) - py + 0x2F;

                uint16 tile = RSDK.GetTile(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 24)
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;

                if ((RSDK.GetTile(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING)
                    ++BSS_Setup->ringCount;
            }
        }

        // Bottom Right Quadrant
        for (int32 y = BSS_PLAYFIELD_H / 2, py = 0; y < BSS_PLAYFIELD_H; ++y, ++py) {
            for (int32 x = BSS_PLAYFIELD_W / 2, px = 0; x < BSS_PLAYFIELD_W; ++x, ++px) {
                int32 tx = 0x10 * (BSS_Setup->randomNumbers[3] & 0x0F) - px + 0x0F;
                int32 ty = (BSS_Setup->randomNumbers[3] & 0xF0) - py + 0x2F;

                uint16 tile = RSDK.GetTile(BSS_Setup->playFieldLayer, tx, ty);

                int32 playFieldPos                 = (x * BSS_PLAYFIELD_H) + y;
                BSS_Setup->playField[playFieldPos] = tile & 0x3FF;
                if (BSS_Setup->playField[playFieldPos] > 24)
                    BSS_Setup->playField[playFieldPos] = BSS_NONE;

                if ((RSDK.GetTile(BSS_Setup->ringCountLayer, tx, ty) & 0x3FF) == BSS_RING)
                    ++BSS_Setup->ringCount;
            }
        }

        BSS_Setup->playField[(16 * BSS_PLAYFIELD_H) + 3] = BSS_SPAWN_RIGHT;
    }

    RSDK.ResetEntitySlot(SLOT_BSS_SETUP, BSS_Setup->classID, NULL);

    BSS_Setup_SetupPalette();

    globals->specialCleared = false;

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

    EntityMenuParam *param = MenuParam_GetParam();
    if (param->bssSelection == BSS_SELECTION_EXTRAS) {
        String string;
        Localization_GetString(&string, STR_RPC_PLAYING);
        API_SetRichPresence(PRESENCE_GENERIC, &string);
    }
}

int32 BSS_Setup_GetStageID(void)
{
    int32 pos = SceneInfo->listPos;
    RSDK.SetScene("Blue Spheres", "");
    int32 id = (pos - SceneInfo->listPos) % 32;

    SceneInfo->listPos = pos;
    return id;
}

void BSS_Setup_SetupPalette(void)
{
    // Globe Colour 1 & 2
    for (int32 i = 0; i < 0x10; ++i) RSDK.SetPaletteEntry(0, 0x80 + i, RSDK.GetPaletteEntry(1, BSS_Palette->startColorID));
    for (int32 i = 0; i < 0x10; ++i) RSDK.SetPaletteEntry(0, 0x90 + i, RSDK.GetPaletteEntry(1, BSS_Palette->startColorID + 1));

    // Sky Colours
    for (int32 i = 0; i < 3; ++i) RSDK.SetPaletteEntry(0, 0xA0 + i, RSDK.GetPaletteEntry(1, i + BSS_Palette->startColorID + 2));

    // Emerald Colours (Unused in mania)
    for (int32 i = 0; i < 4; ++i) RSDK.SetPaletteEntry(0, 0xC0 + i, RSDK.GetPaletteEntry(1, i + BSS_Palette->startColorID + 8));

    // Alt Globe Palettes
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
        int32 frustumID    = f ? BSS_Setup->frustum2Layer : BSS_Setup->frustum1Layer;
        TileLayer *frustum = RSDK.GetTileLayer(frustumID);

        count       = offset;
        int32 lastX = 0;
        int32 lastY = 0;

        for (int32 y = 0; y < frustum->height; ++y) {
            for (int32 x = 0; x < frustum->width; ++x) {
                uint16 id = (RSDK.GetTile(frustumID, x, y) & 0x3FF);
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

                    offsetTable[i].y     = oy;
                    offsetTable[i].x     = ox;
                    offsetRadiusTable[i] = id;
                }
            }
        }

        offset += BSS_Setup->frustumCount[f];
    }

    for (int32 i = RESERVE_ENTITY_COUNT; i < RESERVE_ENTITY_COUNT + 0x60; ++i) {
        RSDK.ResetEntitySlot(i, BSS_Collectable->classID, NULL);
    }
}

void BSS_Setup_CollectRing(void)
{
    RSDK_THIS(BSS_Setup);

    ++BSS_Setup->rings;

    if (BSS_Setup->ringCount > 0) {
        BSS_Setup->ringCount--;

        if (!BSS_Setup->ringCount) {
            CREATE_ENTITY(BSS_Message, INT_TO_VOID(BSS_MESSAGE_PERFECT), self->position.x, self->position.y);
            RSDK.PlaySfx(BSS_Setup->sfxEvent, false, 0xFF);
        }
    }

    if (BSS_Setup->ringPan) {
        int32 channel = RSDK.PlaySfx(BSS_Setup->sfxRing, false, 0xFF);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        BSS_Setup->ringPan = 0;
    }
    else {
        int32 channel = RSDK.PlaySfx(BSS_Setup->sfxRing, false, 0xFF);
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

    RSDK.GetTileLayer(BSS_Setup->bgLayer)->scrollInfo[0].scrollPos = self->angle << 18;
}

void BSS_Setup_State_GlobeJettison(void)
{
    RSDK_THIS(BSS_Setup);

    RSDK.GetTileLayer(BSS_Setup->globeLayer)->drawGroup[0] = 1;

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

    TileLayer *background = RSDK.GetTileLayer(BSS_Setup->bgLayer);
    background->scrollPos += self->globeSpeed << 14;

    self->stopMovement = true;
    BSS_Setup_HandleCollectableMovement();
    BSS_Setup_LaunchSpheres();

    if (++self->spinTimer == 128) {
        self->spinTimer    = 0;
        self->speedupLevel = 8;
        self->globeSpeed   = 8;
        BSS_Setup_SetupFinishSequence();

        EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        player->stateInput       = StateMachine_None;
        player->jumpPress        = false;

        self->state = BSS_Setup_State_GlobeEmerald;
    }
}

void BSS_Setup_HandleSteppedObjects(void)
{
    RSDK_THIS(BSS_Setup);

    if (self->globeTimer < 32)
        self->disableBumpers = false;

    if (self->globeTimer > 224)
        self->disableBumpers = false;

    int32 fieldPos = self->playerPos.y + (BSS_PLAYFIELD_H * self->playerPos.x);
    switch (BSS_Setup->playField[fieldPos]) {
        case BSS_SPHERE_BLUE:
            if (self->globeTimer < 128) {
                self->lastSpherePos.x = self->playerPos.x;
                self->lastSpherePos.y = self->playerPos.y;

                BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;

                if (!self->completedRingLoop) {
                    CREATE_ENTITY(BSS_Collected, INT_TO_VOID(BSS_COLLECTED_BLUE), self->playerPos.x, self->playerPos.y);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }

                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    self->state            = BSS_Setup_State_GlobeJettison;
                    RSDK.PlaySfx(BSS_Setup->sfxSSJettison, false, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
                }
            }
            break;

        case BSS_SPHERE_RED:
            if (self->state != BSS_Setup_State_GlobeExit && self->globeTimer < 32) {
                self->state        = BSS_Setup_State_GlobeExit;
                self->stopMovement = true;
                self->spinTimer    = 0;
                self->globeTimer   = 0;
                RSDK.PlaySfx(BSS_Setup->sfxSSExit, false, 255);
                Music_FadeOut(0.0125);
            }
            break;

        case BSS_SPHERE_BUMPER:
            if (!self->disableBumpers && self->globeTimer < 112) {
                if (self->globeTimer > 16) {
                    if (self->globeSpeed < 0) {
                        self->disableBumpers  = true;
                        self->globeSpeed      = -self->globeSpeed;
                        self->playerWasBumped = false;
                        RSDK.PlaySfx(BSS_Setup->sfxBumper, false, 255);
                    }
                }
                else if (!self->spinState) {
                    if (self->globeSpeed < 0) {
                        self->globeTimer      = 16;
                        self->disableBumpers  = true;
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
                player->velocity.y       = -TO_FIXED(24);
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
                CREATE_ENTITY(BSS_Collected, INT_TO_VOID(BSS_COLLECTED_GREEN), self->playerPos.x, self->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
            }
            break;

        case BSS_SPHERE_PINK:
            if (self->state != BSS_Setup_State_StartGlobeTeleport && self->globeTimer < 64) {
                self->state      = BSS_Setup_State_StartGlobeTeleport;
                self->spinTimer  = 0;
                self->globeTimer = 0;
                RSDK.PlaySfx(BSS_Setup->sfxTeleport, false, 255);

                EntityFXFade *fade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
                fade->speedIn      = 32;
                fade->speedOut     = 32;
                fade->wait         = 48;
            }
            break;

        case BSS_RING:
            if (self->globeTimer < 128) {
                CREATE_ENTITY(BSS_Collected, INT_TO_VOID(BSS_COLLECTED_RING), self->playerPos.x, self->playerPos.y);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;

        default: break;
    }

    int32 posX = (self->playerPos.x + (RSDK.Sin256(self->angle) >> 8)) & 0x1F;
    int32 posY = (self->playerPos.y - (RSDK.Cos256(self->angle) >> 8)) & 0x1F;
    fieldPos   = posY + (BSS_PLAYFIELD_H * posX);

    switch (BSS_Setup->playField[fieldPos]) {
        case BSS_SPHERE_BLUE:
            if (self->globeTimer > 128) {
                self->lastSpherePos.x = posX;
                self->lastSpherePos.y = posY;

                BSS_Setup_ProcessChain();
                --BSS_Setup->sphereCount;

                if (!self->completedRingLoop) {
                    CREATE_ENTITY(BSS_Collected, INT_TO_VOID(BSS_COLLECTED_BLUE), posX, posY);
                    BSS_Setup->playField[fieldPos] = BSS_BLUE_STOOD;
                }

                if (BSS_Setup->sphereCount <= 0) {
                    BSS_Setup->sphereCount = 0;
                    RSDK.PlaySfx(BSS_Setup->sfxMedal, false, 255);
                    self->state = BSS_Setup_State_GlobeJettison;
                    RSDK.PlaySfx(BSS_Setup->sfxSSJettison, false, 255);
                    Music_FadeOut(0.0125);
                }
                else {
                    RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
                }
            }
            break;

        case BSS_SPHERE_RED:
            if (self->state != BSS_Setup_State_GlobeExit && self->globeTimer > 224) {
                self->palettePage ^= 1;
                self->state      = BSS_Setup_State_GlobeExit;
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
            if (!self->disableBumpers && self->globeTimer > 144) {
                if (self->globeTimer >= 240) {
                    if (!self->spinState) {
                        if (self->globeSpeed > 0) {
                            self->globeTimer      = 240;
                            self->disableBumpers  = true;
                            self->globeSpeed      = -self->globeSpeed;
                            self->playerWasBumped = true;
                            RSDK.PlaySfx(BSS_Setup->sfxBumper, false, 255);
                        }
                    }
                }
                else {
                    if (self->globeSpeed > 0) {
                        self->disableBumpers  = true;
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
                player->velocity.y       = -TO_FIXED(24);
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
                CREATE_ENTITY(BSS_Collected, INT_TO_VOID(BSS_COLLECTED_GREEN), posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_SPHERE_GREEN_STOOD;
                RSDK.PlaySfx(BSS_Setup->sfxBlueSphere, false, 255);
            }
            break;

        case BSS_RING:
            if (self->globeTimer > 128) {
                CREATE_ENTITY(BSS_Collected, INT_TO_VOID(BSS_COLLECTED_RING), posX, posY);
                BSS_Setup->playField[fieldPos] = BSS_RING_SPARKLE;
                BSS_Setup_CollectRing();
            }
            break;

        case BSS_EMERALD_CHAOS:
        case BSS_EMERALD_SUPER:
        case BSS_MEDAL_SILVER:
        case BSS_MEDAL_GOLD:
            if (self->globeTimer > 240) {
                EntityMenuParam *param = MenuParam_GetParam();
                if (param->bssSelection == BSS_SELECTION_NONE && globals->gameMode < MODE_TIMEATTACK) {
                    int32 pos = BSS_Setup_GetStageID();
                    if (pos >= 0) {
                        ProgressRAM *progress = GameProgress_GetProgressRAM();
                        if (progress) {
                            uint8 medal = BSS_Setup->playField[fieldPos] == BSS_MEDAL_SILVER ? 1
                                          : BSS_Setup->playField[fieldPos] == BSS_MEDAL_GOLD ? 2
                                                                                             : 0;

                            if (medal)
                                GameProgress_GiveMedal(pos, medal);

                            if (progress->allGoldMedals && progress->goldMedalCount == 31) {
                                API_UnlockAchievement(&achievementList[ACH_GOLD_MEDAL]);
                            }

                            if (progress->allSilverMedals && progress->silverMedalCount == 31) {
                                API_UnlockAchievement(&achievementList[ACH_SILVER_MEDAL]);
                            }
                        }
                    }
                }

                self->palettePage ^= 1;
                self->state      = BSS_Setup_State_GlobeExit;
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
            EntityBSS_Collectable *collectable = RSDK_GET_ENTITY(slot, BSS_Collectable);
            int32 x                            = (self->offset.x * RSDK.Cos256(self->angle) + self->offset.y * RSDK.Sin256(self->angle)) >> 4;
            int32 y                            = (self->offset.y * RSDK.Cos256(self->angle) - self->offset.x * RSDK.Sin256(self->angle)) >> 4;
            y                                  = -(y + self->paletteLine - 16);

            if (y < 0) {
                collectable->classID = TYPE_BLANK;
            }
            else {
                collectable->classID = BSS_Collectable->classID;
                collectable->type    = tile & 0x3FF;
                if (y < 112) {
                    self->xMultiplier             = BSS_Setup->xMultiplierTable[y];
                    self->divisor                 = BSS_Setup->divisorTable[y];
                    collectable->animator.frameID = BSS_Setup->frameTable[y];
                    collectable->animator.frameID -= abs(x) >> 5;
                    if (collectable->animator.frameID < 0)
                        collectable->animator.frameID = 0;

                    int32 finalX = self->xMultiplier * x;
                    int32 distX  = finalX * finalX >> 16;

                    int32 worldX = (finalX <= 0 ? (finalX + distX) : (finalX - distX)) >> 4;

                    collectable->position.x = (worldX + ScreenInfo->center.x) << 16;
                    collectable->position.y = (BSS_Setup->screenYTable[y] + worldX * worldX / self->divisor) << 16;

                    ++slot;
                }
            }
        }

        --id;
        ++offset;
    }

    while (slot < RESERVE_ENTITY_COUNT + 0x80) {
        RSDK_GET_ENTITY(slot++, BSS_Collectable)->classID = TYPE_BLANK;
    }
}

void BSS_Setup_State_GlobeEmerald(void)
{
    RSDK_THIS(BSS_Setup);

    RSDK.GetTileLayer(BSS_Setup->globeLayer)->drawGroup[0] = 1;

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

    TileLayer *background = RSDK.GetTileLayer(BSS_Setup->bgLayer);
    background->scrollPos += self->globeSpeed << 14;

    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_StartGlobeTeleport(void)
{
    RSDK_THIS(BSS_Setup);

    self->alpha += 8;
    if (self->alpha == 320) {
        EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        EntityBSS_Player *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, BSS_Player);

        RSDK.SetSpriteAnimation(player1->aniFrames, 0, &player1->animator, true, 0);
        RSDK.SetSpriteAnimation(player2->aniFrames, 0, &player2->animator, true, 0);

        int32 count                = BSS_Setup->pinkSphereCount;
        int32 dir                  = RSDK.Rand(0, count - 1);
        bool32 foundValidPlayerPos = false;

        for (; (count && dir >= 0) && !foundValidPlayerPos; --count) {
            for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    uint16 tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
                    if ((tile & 0x7F) == BSS_SPHERE_PINK && (x != self->playerPos.x || y != self->playerPos.y) && --dir < 0) {
                        self->playerPos.x = x;
                        self->playerPos.y = y;

                        x                   = 0x20;
                        y                   = 0x20;
                        foundValidPlayerPos = true;
                    }
                }
            }
        }

        dir                       = RSDK.Rand(0, 4);
        bool32 foundValidSpawnDir = false;
        for (int32 i = 0; i < 4; ++i) {
            int32 x = self->playerPos.x;
            int32 y = self->playerPos.y;
            switch (dir) {
                case 0: y = (y - 1) & 0x1F; break;
                case 1: x = (x + 1) & 0x1F; break;
                case 2: y = (y + 1) & 0x1F; break;
                case 3: x = (x - 1) & 0x1F; break;
                default: break;
            }

            uint16 tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
            if (tile < BSS_SPHERE_RED || (tile > BSS_SPHERE_BUMPER && tile != BSS_SPHERE_PINK)) {
                foundValidSpawnDir = true;
                break;
            }

            dir = (dir + 1) & 3;
        }

        if (!foundValidSpawnDir) {
            for (int32 i = 0; i < 4; ++i) {
                int32 x = self->playerPos.x;
                int32 y = self->playerPos.y;
                switch (dir) {
                    case 0: y = (y - 2) & 0x1F; break;
                    case 1: x = (x + 2) & 0x1F; break;
                    case 2: y = (y + 2) & 0x1F; break;
                    case 3: x = (x - 2) & 0x1F; break;
                    default: break;
                }

                uint16 tile = BSS_Setup->playField[y + (BSS_PLAYFIELD_H * x)];
                if (tile < BSS_SPHERE_RED || (tile > BSS_SPHERE_BUMPER && tile != BSS_SPHERE_PINK)) {
                    foundValidSpawnDir = true;
                    break;
                }

                dir = (dir + 1) & 3;
            }
        }

        self->angle = dir << 6;

        CREATE_ENTITY(BSS_Collected, INT_TO_VOID(BSS_COLLECTED_PINK), self->playerPos.x, self->playerPos.y);
        BSS_Setup->playField[self->playerPos.y + (BSS_PLAYFIELD_H * self->playerPos.x)] = BSS_SPHERE_PINK_STOOD;

        self->timer = 100;
        self->state = BSS_Setup_State_FinishGlobeTeleport;
    }
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_GlobeExit(void)
{
    RSDK_THIS(BSS_Setup);

    PauseMenu->disableEvents = true;
    self->speedupLevel       = 0;

    if (self->spinTimer <= 0) {
        CREATE_ENTITY(BSS_Message, INT_TO_VOID(BSS_MESSAGE_FINISHED), self->position.x, self->position.y);
        foreach_active(BSS_Player, player) { player->stateInput = StateMachine_None; }
    }
    else {
        TileLayer *background = RSDK.GetTileLayer(BSS_Setup->bgLayer);
        background->scrollInfo[0].scrollPos -= TO_FIXED(32);

        self->angle -= 8;
        self->angle &= 0xFF;
    }

    TileLayer *globe = RSDK.GetTileLayer(BSS_Setup->globeLayer);
    if (self->spinTimer & 0xF) {
        globe->drawGroup[0] = DRAWGROUP_COUNT;

        int32 timer                     = self->spinTimer & 0xF;
        self->globeSpinAnimator.frameID = BSS_Setup->globeFrameTable[timer - 1];
        self->direction                 = BSS_Setup->globeDirTableL[timer - 1];
    }
    else {
        if (self->spinTimer > 0) {
            self->palettePage ^= 1;
            globe->drawGroup[0] = 1;
        }
    }

    self->spinTimer += 2;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_GlobeMoveZ(void)
{
    RSDK_THIS(BSS_Setup);

    EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);

    if (self->speedupLevel < 32 && ++self->speedupTimer >= self->speedupInterval) {
        self->speedupTimer = 0;
        self->speedupLevel += 4;
    }

    RSDK.GetTileLayer(BSS_Setup->globeLayer)->drawGroup[0] = 1;

    if (self->playerWasBumped) {
        if (!self->disableBumpers && player1->up)
            self->playerWasBumped = false;
    }
    else {
        if (self->globeSpeed < self->speedupLevel)
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

    if (self->state == BSS_Setup_State_GlobeMoveZ) {
        if (self->globeSpeed > 0) {
            if (self->globeTimer >= 0x100) {
                switch (self->spinState) {
                    case 0: self->globeTimer -= 256; break;

                    case 1:
                        self->state      = BSS_Setup_State_GlobeTurnLeft;
                        self->globeTimer = 0;
                        break;

                    case 2:
                        self->state      = BSS_Setup_State_GlobeTurnRight;
                        self->globeTimer = 0;
                        break;
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
            switch (self->spinState) {
                case 0:
                    self->globeTimer += 256;
                    self->palettePage ^= 1;

                    self->playerPos.x -= RSDK.Sin256(self->angle) >> 8;
                    self->playerPos.x &= 0x1F;
                    self->playerPos.y += RSDK.Cos256(self->angle) >> 8;
                    self->playerPos.y &= 0x1F;

                    break;

                case 1:
                    self->state      = BSS_Setup_State_GlobeTurnLeft;
                    self->globeTimer = 0;
                    break;

                case 2:
                    self->state      = BSS_Setup_State_GlobeTurnRight;
                    self->globeTimer = 0;
                    break;
            }

            self->spinState = 0;
        }

        TileLayer *background = RSDK.GetTileLayer(BSS_Setup->bgLayer);
        background->scrollPos += self->globeSpeed << 14;
    }

    self->paletteLine = (self->globeTimer >> 4) & 0xF;
    BSS_Setup_HandleCollectableMovement();
}

void BSS_Setup_State_GlobeTurnLeft(void)
{
    RSDK_THIS(BSS_Setup);

    if (self->speedupLevel < 32 && ++self->speedupTimer >= self->speedupInterval) {
        self->speedupTimer = 0;
        self->speedupLevel += 4;
    }

    TileLayer *background = RSDK.GetTileLayer(BSS_Setup->bgLayer);
    background->scrollInfo[0].scrollPos -= TO_FIXED(16);

    self->angle -= 4;
    self->angle &= 0xFF;

    TileLayer *globe = RSDK.GetTileLayer(BSS_Setup->globeLayer);
    if (self->spinTimer == 15) {
        globe->drawGroup[0] = 1;

        self->spinTimer = 0;
        self->palettePage ^= 1;
        if (!self->timer)
            self->state = BSS_Setup_State_GlobeMoveZ;
        else
            self->state = BSS_Setup_State_FinishGlobeTeleport;

        BSS_Setup_HandleCollectableMovement();
    }
    else {
        globe->drawGroup[0] = DRAWGROUP_COUNT;

        self->globeSpinAnimator.frameID = BSS_Setup->globeFrameTable[self->spinTimer];
        self->direction                 = BSS_Setup->globeDirTableL[self->spinTimer];

        self->spinTimer++;
        if (self->timer > 1)
            self->timer--;

        BSS_Setup_HandleCollectableMovement();
    }
}

void BSS_Setup_State_GlobeTurnRight(void)
{
    RSDK_THIS(BSS_Setup);

    if (self->speedupLevel < 32 && ++self->speedupTimer >= self->speedupInterval) {
        self->speedupTimer = 0;
        self->speedupLevel += 4;
    }
    TileLayer *background = RSDK.GetTileLayer(BSS_Setup->bgLayer);
    background->scrollInfo[0].scrollPos += TO_FIXED(16);

    self->angle += 4;
    self->angle &= 0xFF;

    TileLayer *globe = RSDK.GetTileLayer(BSS_Setup->globeLayer);
    if (self->spinTimer == 15) {
        globe->drawGroup[0] = 1;

        self->spinTimer = 0;
        if (!self->timer)
            self->state = BSS_Setup_State_GlobeMoveZ;
        else
            self->state = BSS_Setup_State_FinishGlobeTeleport;

        BSS_Setup_HandleCollectableMovement();
    }
    else {
        globe->drawGroup[0] = DRAWGROUP_COUNT;

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

void BSS_Setup_State_FinishGlobeTeleport(void)
{
    RSDK_THIS(BSS_Setup);

    EntityBSS_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);

    if (self->alpha <= 0) {
        if (player->up)
            self->timer = 1;
        else if (player->left)
            self->state = BSS_Setup_State_GlobeTurnLeft;
        else if (player->right)
            self->state = BSS_Setup_State_GlobeTurnRight;
    }
    else {
        self->alpha -= 8;
    }

    if (!--self->timer) {
        self->state = BSS_Setup_State_GlobeMoveZ;

        EntityBSS_Player *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, BSS_Player);
        if (player1->onGround)
            RSDK.SetSpriteAnimation(player1->aniFrames, 1, &player1->animator, false, 0);

        EntityBSS_Player *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, BSS_Player);
        if (player2->onGround)
            RSDK.SetSpriteAnimation(player2->aniFrames, 1, &player2->animator, false, 0);
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

    if ((BSS_Setup->playField[x1 + y2] & 0x7F) != BSS_SPHERE_BLUE && (BSS_Setup->playField[x2 + y2] & 0x7F) != BSS_SPHERE_BLUE
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

    int32 slot                         = RESERVE_ENTITY_COUNT;
    EntityBSS_Collectable *collectable = NULL;

    collectable = RSDK_GET_ENTITY(slot++, BSS_Collectable);
    while (collectable->classID != TYPE_BLANK) {
        int32 ix                = (collectable->position.x >> 16);
        collectable->position.x = ((x * (ix - screen->center.x) >> 8) + screen->center.x) << 16;
        collectable->position.y -= y;
        collectable = RSDK_GET_ENTITY(slot++, BSS_Collectable);
    }
}

void BSS_Setup_SetupFinishSequence(void)
{
    RSDK_THIS(BSS_Setup);

    for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
    }

    int32 fx       = (RSDK.Sin256(self->angle) >> 5) + self->playerPos.x;
    int32 fy       = (((uint8)self->playerPos.y - (uint8)(RSDK.Cos256(self->angle) >> 5)) & 0x1F);
    int32 fieldPos = fy + (BSS_PLAYFIELD_H * (fx & 0x1F));

    if (BSS_Setup->ringCount > 0)
        BSS_Setup->playField[fieldPos] = BSS_MEDAL_SILVER;
    else
        BSS_Setup->playField[fieldPos] = BSS_MEDAL_GOLD;

    RSDK_GET_ENTITY(RESERVE_ENTITY_COUNT, BSS_Collectable)->drawGroup     = 3;
    RSDK_GET_ENTITY(RESERVE_ENTITY_COUNT + 1, BSS_Collectable)->drawGroup = 3;
}

bool32 BSS_Setup_ScanSphereChain_Up(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);

    if (self->completedRingLoop)
        return true;

    int32 px = BSS_PLAYFIELD_H * x;
    int32 id = 0;
    while (true) {
        y = (y - 1) & 0x1F;

        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->sphereChainTable[px + y] & 0x7F) == BSS_SPHERE_BLUE)
            break;

        if (!BSS_Setup_CheckSphereValid(x, y))
            break;

        BSS_Setup->sphereChainTable[y + px]     = BSS_SPHERE_BLUE;
        BSS_Setup->sphereCollectedTable[y + px] = BSS_SPHERE_BLUE;

        if (x == self->lastSpherePos.x && y == self->lastSpherePos.y) {
            self->completedRingLoop = true;
            return true;
        }

        bool32 foundLoop = false;

        int32 fieldPosRight = y + (BSS_PLAYFIELD_H * ((x + 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPosRight] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Right(x, y);

        int32 fieldPosLeft = y + (BSS_PLAYFIELD_H * ((x - 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPosLeft] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Left(x, y);

        if (!foundLoop)
            id++;
        else
            id = 0;

        if (self->completedRingLoop)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        y                                       = (y + 1) & 0x1F;
        BSS_Setup->sphereCollectedTable[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_ScanSphereChain_Down(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);

    if (self->completedRingLoop)
        return true;

    int32 px = BSS_PLAYFIELD_H * x;
    int32 id = 0;
    while (true) {
        y = (y + 1) & 0x1F;

        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if (BSS_Setup->sphereChainTable[px + y] == BSS_SPHERE_BLUE)
            break;

        if (!BSS_Setup_CheckSphereValid(x, y))
            break;

        BSS_Setup->sphereChainTable[y + px]     = BSS_SPHERE_BLUE;
        BSS_Setup->sphereCollectedTable[y + px] = BSS_SPHERE_BLUE;
        if (x == self->lastSpherePos.x && y == self->lastSpherePos.y) {
            self->completedRingLoop = true;
            return true;
        }

        bool32 foundLoop = false;

        int32 fieldPosLeft = y + (BSS_PLAYFIELD_H * ((x - 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPosLeft] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Left(x, y);

        int32 fieldPosRight = y + (BSS_PLAYFIELD_H * ((x + 1) & 0x1F));
        if ((BSS_Setup->playField[fieldPosRight] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Right(x, y);

        if (!foundLoop)
            id++;
        else
            id = 0;

        if (self->completedRingLoop)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        y                                       = (y - 1) & 0x1F;
        BSS_Setup->sphereCollectedTable[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_ScanSphereChain_Left(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);

    if (self->completedRingLoop)
        return true;

    int32 id = 0;
    while (true) {
        x        = (x - 1) & 0x1F;
        int32 px = (BSS_PLAYFIELD_H * x);

        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->sphereChainTable[px + y] & 0x7F) == BSS_SPHERE_BLUE)
            break;

        if (!BSS_Setup_CheckSphereValid(x, y))
            break;

        BSS_Setup->sphereChainTable[y + px]     = BSS_SPHERE_BLUE;
        BSS_Setup->sphereCollectedTable[y + px] = BSS_SPHERE_BLUE;
        if (x == self->lastSpherePos.x && y == self->lastSpherePos.y) {
            self->completedRingLoop = true;
            return true;
        }

        bool32 foundLoop = false;

        int32 fieldPosUp = px + ((y - 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPosUp] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Up(x, y);

        int32 fieldPosDown = px + ((y + 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPosDown] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Down(x, y);

        if (!foundLoop)
            id++;
        else
            id = 0;

        if (self->completedRingLoop)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        x                                       = (x + 1) & 0x1F;
        int32 px                                = (BSS_PLAYFIELD_H * x);
        BSS_Setup->sphereCollectedTable[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_ScanSphereChain_Right(uint8 x, uint8 y)
{
    RSDK_THIS(BSS_Setup);

    if (self->completedRingLoop)
        return true;

    int32 id = 0;
    while (true) {
        x        = (x + 1) & 0x1F;
        int32 px = (BSS_PLAYFIELD_H * x);

        if ((BSS_Setup->playField[px + y] & 0x7F) != BSS_SPHERE_RED)
            break;

        if ((BSS_Setup->sphereChainTable[px + y] & 0x7F) == BSS_SPHERE_BLUE)
            break;

        if (!BSS_Setup_CheckSphereValid(x, y))
            break;

        BSS_Setup->sphereChainTable[y + px]     = BSS_SPHERE_BLUE;
        BSS_Setup->sphereCollectedTable[y + px] = BSS_SPHERE_BLUE;
        if (x == self->lastSpherePos.x && y == self->lastSpherePos.y) {
            self->completedRingLoop = true;
            return true;
        }

        bool32 foundLoop = false;

        int32 fieldPosDown = px + ((y + 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPosDown] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Down(x, y);

        int32 fieldPosUp = px + ((y - 1) & 0x1F);
        if ((BSS_Setup->playField[fieldPosUp] & 0x7F) == BSS_SPHERE_RED)
            foundLoop |= BSS_Setup_ScanSphereChain_Up(x, y);

        if (!foundLoop)
            id++;
        else
            id = 0;

        if (self->completedRingLoop)
            return true;
    }

    for (int32 i = id; i > 0; --i) {
        x                                       = (x - 1) & 0x1F;
        int32 px                                = (BSS_PLAYFIELD_H * x);
        BSS_Setup->sphereCollectedTable[px + y] = BSS_NONE;
    }

    return false;
}
bool32 BSS_Setup_GetChainedSphereCount(uint8 x, uint8 y)
{
    int32 px = BSS_PLAYFIELD_H * x;

    uint8 y1 = (y - 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_H; ++i) {
        if (BSS_Setup->sphereCollectedTable[px + y1] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[px + y1] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            y1 = (y1 - 1) & 0x1F;
        }
    }

    y1 = (y + 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_H; ++i) {
        if (BSS_Setup->sphereCollectedTable[px + y1] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[px + y1] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            y1 = (y1 + 1) & 0x1F;
        }
    }

    uint8 x1 = (x - 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_W; ++i) {
        if (BSS_Setup->sphereCollectedTable[y + (BSS_PLAYFIELD_W * x1)] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[y + (BSS_PLAYFIELD_W * x1)] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            x1 = (x1 - 1) & 0x1F;
        }
    }

    x1 = (x + 1) & 0x1F;
    for (int32 i = 0; i < BSS_PLAYFIELD_W; ++i) {
        if (BSS_Setup->sphereCollectedTable[y + (BSS_PLAYFIELD_W * x1)] == BSS_SPHERE_BLUE) {
            break;
        }
        else {
            int32 id = BSS_Setup->playField[y + (BSS_PLAYFIELD_W * x1)] & 0x7F;
            if (id == BSS_NONE || id == BSS_SPHERE_RED)
                return false;
            x1 = (x1 + 1) & 0x1F;
        }
    }

    BSS_Setup->sphereCollectedTable[px + y] = BSS_SPHERE_BLUE;
    return true;
}

void BSS_Setup_ProcessChain(void)
{
    RSDK_THIS(BSS_Setup);

    for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
        for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
            BSS_Setup->sphereChainTable[(x * BSS_PLAYFIELD_H) + y]     = BSS_NONE;
            BSS_Setup->sphereCollectedTable[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
        }
    }

    BSS_Setup->playField[self->lastSpherePos.y + (BSS_PLAYFIELD_H * self->lastSpherePos.x)]            = BSS_SPHERE_RED;
    BSS_Setup->sphereCollectedTable[self->lastSpherePos.y + (BSS_PLAYFIELD_H * self->lastSpherePos.x)] = BSS_SPHERE_BLUE;

    self->completedRingLoop = false;
    BSS_Setup_ScanSphereChain_Up(self->lastSpherePos.x, self->lastSpherePos.y);
    BSS_Setup_ScanSphereChain_Down(self->lastSpherePos.x, self->lastSpherePos.y);
    BSS_Setup_ScanSphereChain_Left(self->lastSpherePos.x, self->lastSpherePos.y);
    BSS_Setup_ScanSphereChain_Right(self->lastSpherePos.x, self->lastSpherePos.y);

    BSS_Setup->playField[self->lastSpherePos.y + (BSS_PLAYFIELD_H * self->lastSpherePos.x)] = BSS_SPHERE_BLUE;

    if (self->completedRingLoop) {
        int32 spheresCollected = 0;

        for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
            for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                if ((BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] & 0x7F) == BSS_SPHERE_BLUE)
                    spheresCollected += BSS_Setup_GetChainedSphereCount(x, y);
            }
        }

        if (spheresCollected <= 0) {
            self->completedRingLoop = false;
        }
        else {
            for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    int32 p = x * BSS_PLAYFIELD_H;

                    // The hell pit
                    uint32 y1 = (y - 1) & 0x1F;
                    uint32 y2 = (y + 1) & 0x1F;
                    uint32 x1 = BSS_PLAYFIELD_H * ((x - 1) & 0x1F);
                    uint32 x2 = BSS_PLAYFIELD_H * ((x + 1) & 0x1F);

                    if (BSS_Setup->sphereCollectedTable[p + y] == BSS_SPHERE_BLUE) {
                        if ((BSS_Setup->playField[p + y1] & 0x7F) != BSS_SPHERE_BLUE) {
                            if ((BSS_Setup->playField[p + y2] & 0x7F) != BSS_SPHERE_BLUE) {
                                if ((BSS_Setup->playField[x1 + y] & 0x7F) != BSS_SPHERE_BLUE) {
                                    if ((BSS_Setup->playField[x2 + y] & 0x7F) != BSS_SPHERE_BLUE
                                        && (BSS_Setup->playField[x1 + y1] & 0x7F) != BSS_SPHERE_BLUE
                                        && (BSS_Setup->playField[x2 + y1] & 0x7F) != BSS_SPHERE_BLUE
                                        && (BSS_Setup->playField[x1 + y2] & 0x7F) != BSS_SPHERE_BLUE) {
                                        if ((BSS_Setup->playField[x2 + y2] & 0x7F) != BSS_SPHERE_BLUE)
                                            BSS_Setup->sphereCollectedTable[(x * BSS_PLAYFIELD_H) + y] = BSS_NONE;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int32 y = 0; y < BSS_PLAYFIELD_H; ++y) {
                for (int32 x = 0; x < BSS_PLAYFIELD_W; ++x) {
                    if (BSS_Setup->sphereCollectedTable[(x * BSS_PLAYFIELD_H) + y])
                        BSS_Setup->playField[(x * BSS_PLAYFIELD_H) + y] = BSS_RING;
                }
            }

            BSS_Setup->sphereCount -= spheresCollected;
            RSDK.PlaySfx(BSS_Setup->sfxLoseRings, false, 0xFF);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void BSS_Setup_EditorDraw(void) {}

void BSS_Setup_EditorLoad(void)
{

    // functionality was likely replaced by BSS_Palette
    RSDK_ACTIVE_VAR(BSS_Setup, paletteID);
    RSDK_ENUM_VAR("(Unused)", 0);
}
#endif

void BSS_Setup_Serialize(void) { RSDK_EDITABLE_VAR(BSS_Setup, VAR_UINT8, paletteID); }
