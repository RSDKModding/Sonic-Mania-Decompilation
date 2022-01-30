// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WarpDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectWarpDoor *WarpDoor;

void WarpDoor_Update(void)
{
    RSDK_THIS(WarpDoor);

    if (self->fadeTimer > 0)
        self->fadeTimer -= 16;
    if (self->destinationTag >= 1 && self->destinationTag <= 256) {
        EntityWarpDoor *tag = (EntityWarpDoor *)WarpDoor->tags[self->destinationTag];
        if (tag) {
            bool32 flag = false;
            int32 boundID = -1;
            foreach_all(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    flag = true;
                    if (!self->hasWarped) {
                        EntityCamera *camera = player->camera;
                        int32 camRelPosX       = 0;
                        int32 camRelPosY       = 0;
                        int32 playerX          = player->position.x;
                        int32 playerY          = player->position.y;
                        if (camera) {
                            camRelPosX = camera->position.x - playerX;
                            camRelPosY = camera->position.y - playerY;
                        }
                        int32 relPosX = playerX - self->position.x;
                        int32 relPosY = playerY - self->position.y;
                        LogHelpers_Print("camRelPos = <%d, %d>", camRelPosX >> 16, camRelPosY >> 16);
                        LogHelpers_Print("relPos = <%d, %d>", relPosX >> 16, relPosY >> 16);

                        Vector2 posPtr;
                        posPtr.x = tag->position.x;
                        posPtr.y = tag->position.y;
                        if (!self->warpToCenter) {
                            posPtr.x += relPosY;
                            posPtr.y += relPosY;
                        }

                        if (self->forcePlayerState) {
                            player->nextAirState    = StateMachine_None;
                            player->nextGroundState = StateMachine_None;
                            player->onGround        = false;
                            player->state           = Player_State_Hit;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                        }

                        if (camera) {
                            int32 screenY = camRelPosY + posPtr.y;
                            int32 screenX = camRelPosX + posPtr.x;
                            LogHelpers_Print("newPos = <%d, %d>", posPtr.x >> 16, posPtr.y >> 16);
                            LogHelpers_Print("newCamPos = <%d, %d>", screenX >> 16, screenY >> 16);
                            camera->position.x                        = screenX;
                            camera->position.y                        = screenY;
                            ScreenInfo[camera->screenID].position.x = (camera->position.x >> 16);
                            ScreenInfo[camera->screenID].position.y = (camera->position.y >> 16);
                            camera->state                             = Camera_State_Follow;
                            player->scrollDelay                       = 0;
                            WarpDoor_SetupBoundaries(boundID, &posPtr);
                            int32 pID                   = RSDK.GetEntityID(player);
                            camera->boundsL           = Zone->cameraBoundsL[pID];
                            camera->boundsR           = Zone->cameraBoundsR[pID];
                            camera->boundsT           = Zone->cameraBoundsT[pID];
                            camera->boundsB           = Zone->cameraBoundsB[pID];
                            Zone->playerBoundsL[pID] = Zone->cameraBoundsL[pID] << 16;
                            Zone->playerBoundsR[pID] = Zone->cameraBoundsR[pID] << 16;
                            Zone->playerBoundsT[pID] = Zone->cameraBoundsT[pID] << 16;
                            Zone->playerBoundsB[pID] = Zone->cameraBoundsB[pID] << 16;
                            Zone->deathBoundary[pID]  = Zone->cameraBoundsB[pID] << 16;
                        }

                        if (tag->definesBounds)
                            boundID = tag->boundID;
                        tag->hasWarped      = true;
                        player->position.x = posPtr.x;
                        player->position.y = posPtr.y;
                        WarpDoor_CheckAllBounds();

                        if (!player->sidekick) {
                            if (RSDK.CheckStageFolder("TMZ2")) {
                                if (self->effect == TMZ2_WARPDOOR_EFFECT_MIST) {
                                    int32 sfx     = RSDK.Rand(0, 6);
                                    int32 channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackL[sfx], false, 0xFF);
                                    RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                                    channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackR[sfx], 0, 0xFF);
                                    RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                                    foreach_active(TMZBarrier, barrier)
                                    {
                                        if (barrier->warpTag == self->tag)
                                            barrier->cleared = true;
                                    }
                                    tag->fadeTimer = 512;
                                    if (self->destinationTag == 99 || self->destinationTag == 100)
                                       TMZ2_DrawDynTiles_Eggman();
                                    else
                                       TMZ2_DrawDynTiles_Ruby();
                                }
                            }
                            else if (RSDK.CheckStageFolder("OOZ2")) {
                                if (self->effect == OOZ_WARPDOOR_EFFECT_TO_SUB || self->effect == OOZ_WARPDOOR_EFFECT_FROM_SUB) {
                                    OOZSetup->useSmogEffect = self->effect == OOZ_WARPDOOR_EFFECT_FROM_SUB ? 1 : 0;
                                    destroyEntity(self);
                                    foreach_return;
                                }
                            }
                            else if (RSDK.CheckStageFolder("FBZ")) {
                                RSDK.PlaySfx(WarpDoor->sfxWarpDoor, false, 255);
                                flag = false;
                                if (self->go) {
                                    tag->fadeTimer = 256;
                                    tag->fadeOut   = true;
                                }
                                else {
                                    tag->fadeOut   = false;
                                    tag->fadeTimer = 320;
                                }
                            }
                        }
                    }
                }
            }
            if (flag)
                return;
            if (self->hasWarped)
                self->hasWarped = false;
        }
    }
}

void WarpDoor_LateUpdate(void) {}

void WarpDoor_StaticUpdate(void)
{
    if (!WarpDoor->hasSetupTags) {
        foreach_all(WarpDoor, warpDoor)
        {
            if (warpDoor->tag >= 1) {
                WarpDoor->tags[warpDoor->tag] = (Entity *)warpDoor;
            }
            if (warpDoor->definesBounds) {
                WarpDoor->boundaries[WarpDoor->boundCount].top    = warpDoor->yBoundaryPosT;
                WarpDoor->boundaries[WarpDoor->boundCount].right  = warpDoor->xBoundaryPosR;
                WarpDoor->boundaries[WarpDoor->boundCount].bottom = warpDoor->yBoundaryPosB;
                WarpDoor->boundaries[WarpDoor->boundCount].left   = warpDoor->xBoundaryPosL;
                warpDoor->boundID                                 = WarpDoor->boundCount++;
            }
        }
        WarpDoor->hasSetupTags = true;
    }

    if (WarpDoor->boundCount) {
        if (!WarpDoor->hasSetupStartBounds) {
            if (WarpDoor_SetupBoundaries(-1, NULL)) {
                WarpDoor->hasSetupStartBounds = true;
                WarpDoor_SetupPlayerCamera();
                WarpDoor_CheckAllBounds();
            }
        }

        if (WarpDoor->boundsTimer > 0)
            WarpDoor->boundsTimer--;
    }
}

void WarpDoor_Draw(void)
{
    RSDK_THIS(WarpDoor);
    if (DebugMode->debugActive)
        WarpDoor_DrawDebug();
    if (self->fadeTimer > 0) {
        if (RSDK.CheckStageFolder("FBZ")) {
            if (self->fadeOut)
                RSDK.FillScreen(0x000000, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
            else
                RSDK.FillScreen(0x800080, self->fadeTimer, self->fadeTimer - 256, self->fadeTimer);
        }
        else {
            RSDK.FillScreen(0xFFF0F0, self->fadeTimer, self->fadeTimer - 256, self->fadeTimer - 256);
        }
    }
}

void WarpDoor_Create(void *data)
{
    RSDK_THIS(WarpDoor);
    RSDK.SetSpriteAnimation(WarpDoor->aniFrames, 0, &self->animator, true, 0);
    WarpDoor_SetupHitbox();
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_ALWAYS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->hasWarped      = false;
        self->drawFX        = FX_NONE;
        self->inkEffect     = INK_NONE;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderHigh;
    }
}

void WarpDoor_StageLoad(void)
{
    WarpDoor->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);
    WarpDoor_SetStageBounds();
    WarpDoor->hasSetupStartBounds = false;
    WarpDoor->hasSetupTags = 0;
    WarpDoor->boundCount = 0;
    for (int32 i = 0; i < 0x100; ++i) {
        WarpDoor->tags[i] = NULL;
    }
    if (RSDK.CheckStageFolder("FBZ"))
        WarpDoor->sfxWarpDoor = RSDK.GetSfx("FBZ/WarpDoor.wav");
    if (RSDK.CheckStageFolder("TMZ2")) {
        WarpDoor->sfxRubyAttackL[0] = RSDK.GetSfx("Ruby/Attack1_L.wav");
        WarpDoor->sfxRubyAttackR[0] = RSDK.GetSfx("Ruby/Attack1_R.wav");
        WarpDoor->sfxRubyAttackL[1] = RSDK.GetSfx("Ruby/Attack2_L.wav");
        WarpDoor->sfxRubyAttackR[1] = RSDK.GetSfx("Ruby/Attack2_R.wav");
        WarpDoor->sfxRubyAttackL[2] = RSDK.GetSfx("Ruby/Attack3_L.wav");
        WarpDoor->sfxRubyAttackR[2] = RSDK.GetSfx("Ruby/Attack3_R.wav");
        WarpDoor->sfxRubyAttackL[3] = RSDK.GetSfx("Ruby/Attack4_L.wav");
        WarpDoor->sfxRubyAttackR[3] = RSDK.GetSfx("Ruby/Attack4_R.wav");
        WarpDoor->sfxRubyAttackL[4] = RSDK.GetSfx("Ruby/Attack5_L.wav");
        WarpDoor->sfxRubyAttackR[4] = RSDK.GetSfx("Ruby/Attack5_R.wav");
        WarpDoor->sfxRubyAttackL[5] = RSDK.GetSfx("Ruby/Attack6_L.wav");
        WarpDoor->sfxRubyAttackR[5] = RSDK.GetSfx("Ruby/Attack6_R.wav");
    }
}

void WarpDoor_SetupPlayerCamera(void)
{
    foreach_all(Player, player)
    {
        if (player->camera) {
            player->camera->state      = Camera_State_Follow;
            player->camera->position.x = player->position.x;
            player->camera->position.y = player->position.y;
            player->camera->offset.x   = 0;
            player->camera->offset.y   = 0;
        }
    }
}

void WarpDoor_SetStageBounds(void)
{
    Vector2 size;

    RSDK.GetLayerSize(Zone->fgLow, &size, true);
    Zone->cameraBoundsL[0]     = 0;
    Zone->cameraBoundsR[0]     = size.x;
    Zone->cameraBoundsT[0]     = 0;
    Zone->cameraBoundsB[0]     = size.y;
    Zone->playerBoundsL[0]     = Zone->cameraBoundsL[0] << 16;
    Zone->playerBoundsR[0]     = Zone->cameraBoundsR[0] << 16;
    Zone->playerBoundsT[0]     = Zone->cameraBoundsT[0] << 16;
    Zone->playerBoundsB[0]     = Zone->cameraBoundsB[0] << 16;
    Zone->deathBoundary[0]      = Zone->cameraBoundsB[0] << 16;
    Zone->playerBoundActiveL[0] = true;
    Zone->playerBoundActiveT[0] = false;
    Zone->playerBoundActiveR[0] = false;
    Zone->playerBoundActiveB[0] = false;
}

void WarpDoor_SetWarpBounds(uint8 id)
{
    Zone->playerBoundActiveT[0] = false;
    Zone->playerBoundActiveR[0] = false;
    Zone->playerBoundActiveB[0] = false;
    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsT[0]     = WarpDoor->boundaries[id].top;
    Zone->cameraBoundsR[0]     = WarpDoor->boundaries[id].right;
    Zone->cameraBoundsB[0]     = WarpDoor->boundaries[id].bottom;
    Zone->cameraBoundsL[0]     = WarpDoor->boundaries[id].left;
    Zone->playerBoundsT[0]     = WarpDoor->boundaries[id].top << 16;
    Zone->playerBoundsR[0]     = WarpDoor->boundaries[id].right << 16;
    Zone->playerBoundsB[0]     = WarpDoor->boundaries[id].bottom << 16;
    Zone->playerBoundsL[0]     = WarpDoor->boundaries[id].left << 16;
    Zone->deathBoundary[0]      = WarpDoor->boundaries[id].bottom << 16;
    WarpDoor->boundsTimer        = 16;
    WarpDoor->lastBoundsID        = id;
}

void WarpDoor_CheckAllBounds(void)
{
    foreach_all(BoundsMarker, marker)
    {
        foreach_all(Player, player) { BoundsMarker_CheckBounds(player, marker, true); }
    }
}

bool32 WarpDoor_SetupBoundaries(int16 boundsID, Vector2 *posPtr)
{
    if (!WarpDoor->boundCount)
        return false;
    if (Zone->timer < 1)
        return false;
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (!player || !player->camera)
        return false;
    if (boundsID > -1) {
        WarpDoor_SetWarpBounds(boundsID);
    }
    else {
        int32 storeX = player->position.x;
        int32 storeY = player->position.y;
        if (posPtr) {
            player->position.x = posPtr->x;
            player->position.y = posPtr->y;
        }

        int32 distStore = -1;
        int32 id        = -1;
        for (int32 i = 0; i < 0x100; ++i) {
            if (!WarpDoor->boundaries[i].left && !WarpDoor->boundaries[i].top && !WarpDoor->boundaries[i].right && !WarpDoor->boundaries[i].bottom)
                break;

            if (player->position.y < WarpDoor->boundaries[i].top << 16 || player->position.x > WarpDoor->boundaries[i].right << 16
                || player->position.y > WarpDoor->boundaries[i].bottom << 16) {
            }
            else {
                int32 left = WarpDoor->boundaries[i].left << 16;
                if (player->position.x >= left) {
                    int32 dist = (WarpDoor->boundaries[i].bottom - WarpDoor->boundaries[i].top)
                                 * (WarpDoor->boundaries[i].left - WarpDoor->boundaries[i].right);
                    if (dist < distStore) {
                        distStore = dist;
                        id        = i;
                    }
                }
            }
        }

        player->position.x = storeX;
        player->position.y = storeY;
        if (id >= 0) {
            WarpDoor_SetWarpBounds(id);
            return true;
        }
    }
    return false;
}

void WarpDoor_DrawDebug(void)
{
    RSDK_THIS(WarpDoor);
    int32 x = (self->hitbox.left << 16) + self->position.x + 0x80000;
    int32 y = (self->hitbox.top << 16) + self->position.y + 0x80000;

    self->animator.frameID = self->go ? 2 : 0;

    int32 xOff = 0;
    int32 yOff = 0;
    for (int32 i = 0; i < self->width * self->height; ++i) {
        Vector2 drawPos;
        drawPos.y = y + yOff;
        drawPos.x = x + (xOff << 20);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        if (++xOff == self->width) {
            yOff += 0x100000;
            xOff = 0;
        }
    }

    if (SceneInfo->inEditor) {
        int32 colour = 0xFF0000;
        if (!self->go)
            colour = 0x0000FF;

        if (self->destinationTag >= 1 && self->destinationTag <= 256) {
            EntityWarpDoor *dest = (EntityWarpDoor *)WarpDoor->tags[self->destinationTag];
            if (dest) {
                DrawHelpers_DrawHitboxOutline(colour, FLIP_NONE, self->position.x, self->position.y, &self->hitbox);
                DrawHelpers_DrawArrow(0x00FFFF, self->position.x, dest->position.x, self->position.y, dest->position.y);
            }
        }

        if (self->definesBounds) {
            Hitbox hitbox;
            hitbox.left   = self->xBoundaryPosL;
            hitbox.top    = self->yBoundaryPosT;
            hitbox.right  = self->xBoundaryPosR;
            hitbox.bottom = self->yBoundaryPosB;
            DrawHelpers_DrawHitboxOutline(colour, 0, 0, 0, &hitbox);
            RSDK.DrawLine(self->xBoundaryPosL << 16, self->yBoundaryPosT << 16, self->position.x, self->position.y, colour, 0xFF, INK_NONE, false);
            RSDK.DrawLine(self->xBoundaryPosR << 16, self->yBoundaryPosT << 16, self->position.x, self->position.y, colour, 0xFF, INK_NONE, false);
            RSDK.DrawLine(self->xBoundaryPosR << 16, self->yBoundaryPosB << 16, self->position.x, self->position.y, colour, 0xFF, INK_NONE, false);
            RSDK.DrawLine(self->xBoundaryPosL << 16, self->yBoundaryPosB << 16, self->position.x, self->position.y, colour, 0xFF, INK_NONE, false);
        }
    }
}

void WarpDoor_SetupHitbox(void)
{
    RSDK_THIS(WarpDoor);
    if (!self->width)
        self->width = 2;
    if (!self->height)
        self->height = 4;
    self->hitbox.left   = -((16 * self->width) >> 1);
    self->hitbox.right  = (16 * self->width) >> 1;
    self->hitbox.top    = -((16 * self->height) >> 1);
    self->hitbox.bottom = (16 * self->height) >> 1;
}

#if RETRO_INCLUDE_EDITOR
void WarpDoor_EditorDraw(void)
{
    RSDK_THIS(WarpDoor);
    self->active = ACTIVE_ALWAYS;
    WarpDoor_DrawDebug();
}

void WarpDoor_EditorLoad(void)
{
    WarpDoor->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    // RSDK_ACTIVE_VAR(WarpDoor, effect);
    // RSDK_ENUM_VAR("None", WARPDOOR_EFFECT_NONE);

    RSDK_ACTIVE_VAR(WarpDoor, forcePlayerState);
    RSDK_ENUM_VAR("None", WARPDOOR_PLRSTATE_NONE);
    RSDK_ENUM_VAR("Hurt", WARPDOOR_PLRSTATE_HURT);
}
#endif

void WarpDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(WarpDoor, VAR_ENUM, go);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_UINT32, width);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_UINT32, height);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_ENUM, destinationTag);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_BOOL, definesBounds);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_ENUM, xBoundaryPosL);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_ENUM, xBoundaryPosR);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_ENUM, yBoundaryPosT);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_ENUM, yBoundaryPosB);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_UINT8, effect);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_BOOL, warpToCenter);
    RSDK_EDITABLE_VAR(WarpDoor, VAR_UINT8, forcePlayerState);
}
