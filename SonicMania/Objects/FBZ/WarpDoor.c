// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WarpDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWarpDoor *WarpDoor;

void WarpDoor_Update(void)
{
    RSDK_THIS(WarpDoor);

    if (self->fadeTimer > 0)
        self->fadeTimer -= 16;

    if (self->destinationTag >= 1 && self->destinationTag <= 256) {
        EntityWarpDoor *tag = WarpDoor->tags[self->destinationTag];
        if (tag) {
            bool32 warped = false;
            int32 boundID = -1;
            foreach_all(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    warped = true;

                    if (!self->hasWarped) {
                        EntityCamera *camera = player->camera;
                        int32 camRelPosX     = 0;
                        int32 camRelPosY     = 0;
                        int32 playerX        = player->position.x;
                        int32 playerY        = player->position.y;
                        if (camera) {
                            camRelPosX = camera->position.x - playerX;
                            camRelPosY = camera->position.y - playerY;
                        }
                        int32 relPosX = playerX - self->position.x;
                        int32 relPosY = playerY - self->position.y;
                        LogHelpers_Print("camRelPos = <%d, %d>", camRelPosX >> 16, camRelPosY >> 16);
                        LogHelpers_Print("relPos = <%d, %d>", relPosX >> 16, relPosY >> 16);

                        Vector2 newPos;
                        newPos.x = tag->position.x;
                        newPos.y = tag->position.y;
                        if (!self->warpToCenter) {
                            newPos.x += relPosY;
                            newPos.y += relPosY;
                        }

                        if (self->forcePlayerState) {
                            player->nextAirState    = StateMachine_None;
                            player->nextGroundState = StateMachine_None;
                            player->onGround        = false;
                            player->state           = Player_State_Hurt;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                        }

                        if (camera) {
                            Vector2 newCamPos;
                            newCamPos.x = camRelPosX + newPos.x;
                            newCamPos.y = camRelPosY + newPos.y;
                            LogHelpers_Print("newPos = <%d, %d>", newPos.x >> 16, newPos.y >> 16);
                            LogHelpers_Print("newCamPos = <%d, %d>", newCamPos.x >> 16, newCamPos.y >> 16);

                            camera->position.x                      = newCamPos.x;
                            camera->position.y                      = newCamPos.y;
                            ScreenInfo[camera->screenID].position.x = (camera->position.x >> 16);
                            ScreenInfo[camera->screenID].position.y = (camera->position.y >> 16);
                            camera->state                           = Camera_State_FollowXY;
                            player->scrollDelay                     = 0;
                            WarpDoor_SetupBoundaries(boundID, &newPos);

                            int32 playerID                = RSDK.GetEntitySlot(player);
                            camera->boundsL               = Zone->cameraBoundsL[playerID];
                            camera->boundsR               = Zone->cameraBoundsR[playerID];
                            camera->boundsT               = Zone->cameraBoundsT[playerID];
                            camera->boundsB               = Zone->cameraBoundsB[playerID];
                            Zone->playerBoundsL[playerID] = Zone->cameraBoundsL[playerID] << 16;
                            Zone->playerBoundsR[playerID] = Zone->cameraBoundsR[playerID] << 16;
                            Zone->playerBoundsT[playerID] = Zone->cameraBoundsT[playerID] << 16;
                            Zone->playerBoundsB[playerID] = Zone->cameraBoundsB[playerID] << 16;
                            Zone->deathBoundary[playerID] = Zone->cameraBoundsB[playerID] << 16;
                        }

                        if (tag->definesBounds)
                            boundID = tag->boundID;
                        tag->hasWarped = true;

                        player->position.x = newPos.x;
                        player->position.y = newPos.y;
                        WarpDoor_CheckAllBounds();

                        if (!player->sidekick) {
                            if (RSDK.CheckSceneFolder("TMZ2")) {
                                if (self->effect == TMZ2_WARPDOOR_EFFECT_MIST) {
                                    int32 sfx     = RSDK.Rand(0, 6);
                                    int32 channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackL[sfx], false, 0xFF);
                                    RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                                    channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackR[sfx], false, 0xFF);
                                    RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                                    foreach_active(TMZBarrier, barrier)
                                    {
                                        if (barrier->warpTag == self->tag)
                                            barrier->cleared = true;
                                    }

                                    tag->fadeTimer = 512;
                                    if (self->destinationTag == 99 || self->destinationTag == 100)
                                        TMZ2Setup_DrawDynTiles_Eggman();
                                    else
                                        TMZ2Setup_DrawDynTiles_Ruby();
                                }
                            }
                            else if (RSDK.CheckSceneFolder("OOZ2")) {
                                if (self->effect == OOZ_WARPDOOR_EFFECT_TO_SUB || self->effect == OOZ_WARPDOOR_EFFECT_FROM_SUB) {
                                    OOZSetup->useSmogEffect = self->effect == OOZ_WARPDOOR_EFFECT_FROM_SUB ? 1 : 0;
                                    destroyEntity(self);
                                    foreach_return;
                                }
                            }
                            else if (RSDK.CheckSceneFolder("FBZ")) {
                                RSDK.PlaySfx(WarpDoor->sfxWarpDoor, false, 0xFF);
                                warped = false;
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

            if (!warped && self->hasWarped)
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
            if (warpDoor->tag >= 1)
                WarpDoor->tags[warpDoor->tag] = warpDoor;

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
        if (!WarpDoor->hasSetupStartBounds && WarpDoor_SetupBoundaries(-1, NULL)) {
            WarpDoor->hasSetupStartBounds = true;
            WarpDoor_SetupPlayerCamera();
            WarpDoor_CheckAllBounds();
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
        if (RSDK.CheckSceneFolder("FBZ")) {
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
        self->hasWarped     = false;
        self->drawFX        = FX_NONE;
        self->inkEffect     = INK_NONE;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
    }
}

void WarpDoor_StageLoad(void)
{
    WarpDoor->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    WarpDoor_SetStageBounds();
    WarpDoor->hasSetupStartBounds = false;
    WarpDoor->hasSetupTags        = 0;
    WarpDoor->boundCount          = 0;

    for (int32 i = 0; i < 0x100; ++i) WarpDoor->tags[i] = NULL;

    if (RSDK.CheckSceneFolder("FBZ"))
        WarpDoor->sfxWarpDoor = RSDK.GetSfx("FBZ/WarpDoor.wav");

    if (RSDK.CheckSceneFolder("TMZ2")) {
        WarpDoor->sfxRubyAttackL[RUBYSFX_ATTACK1 - 1] = RSDK.GetSfx("Ruby/Attack1_L.wav");
        WarpDoor->sfxRubyAttackR[RUBYSFX_ATTACK1 - 1] = RSDK.GetSfx("Ruby/Attack1_R.wav");
        WarpDoor->sfxRubyAttackL[RUBYSFX_ATTACK2 - 1] = RSDK.GetSfx("Ruby/Attack2_L.wav");
        WarpDoor->sfxRubyAttackR[RUBYSFX_ATTACK2 - 1] = RSDK.GetSfx("Ruby/Attack2_R.wav");
        WarpDoor->sfxRubyAttackL[RUBYSFX_ATTACK3 - 1] = RSDK.GetSfx("Ruby/Attack3_L.wav");
        WarpDoor->sfxRubyAttackR[RUBYSFX_ATTACK3 - 1] = RSDK.GetSfx("Ruby/Attack3_R.wav");
        WarpDoor->sfxRubyAttackL[RUBYSFX_ATTACK4 - 1] = RSDK.GetSfx("Ruby/Attack4_L.wav");
        WarpDoor->sfxRubyAttackR[RUBYSFX_ATTACK4 - 1] = RSDK.GetSfx("Ruby/Attack4_R.wav");
        WarpDoor->sfxRubyAttackL[RUBYSFX_ATTACK5 - 1] = RSDK.GetSfx("Ruby/Attack5_L.wav");
        WarpDoor->sfxRubyAttackR[RUBYSFX_ATTACK5 - 1] = RSDK.GetSfx("Ruby/Attack5_R.wav");
        WarpDoor->sfxRubyAttackL[RUBYSFX_ATTACK6 - 1] = RSDK.GetSfx("Ruby/Attack6_L.wav");
        WarpDoor->sfxRubyAttackR[RUBYSFX_ATTACK6 - 1] = RSDK.GetSfx("Ruby/Attack6_R.wav");
    }
}

void WarpDoor_SetupPlayerCamera(void)
{
    foreach_all(Player, player)
    {
        if (player->camera) {
            player->camera->state      = Camera_State_FollowXY;
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
    RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);

    Zone->cameraBoundsL[0] = 0;
    Zone->cameraBoundsR[0] = size.x;
    Zone->cameraBoundsT[0] = 0;
    Zone->cameraBoundsB[0] = size.y;

    Zone->playerBoundsL[0] = Zone->cameraBoundsL[0] << 16;
    Zone->playerBoundsR[0] = Zone->cameraBoundsR[0] << 16;
    Zone->playerBoundsT[0] = Zone->cameraBoundsT[0] << 16;
    Zone->playerBoundsB[0] = Zone->cameraBoundsB[0] << 16;
    Zone->deathBoundary[0] = Zone->cameraBoundsB[0] << 16;

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

    Zone->cameraBoundsT[0] = WarpDoor->boundaries[id].top;
    Zone->cameraBoundsR[0] = WarpDoor->boundaries[id].right;
    Zone->cameraBoundsB[0] = WarpDoor->boundaries[id].bottom;
    Zone->cameraBoundsL[0] = WarpDoor->boundaries[id].left;

    Zone->playerBoundsT[0] = WarpDoor->boundaries[id].top << 16;
    Zone->playerBoundsR[0] = WarpDoor->boundaries[id].right << 16;
    Zone->playerBoundsB[0] = WarpDoor->boundaries[id].bottom << 16;
    Zone->playerBoundsL[0] = WarpDoor->boundaries[id].left << 16;
    Zone->deathBoundary[0] = WarpDoor->boundaries[id].bottom << 16;

    WarpDoor->boundsTimer  = 16;
    WarpDoor->lastBoundsID = id;
}

void WarpDoor_CheckAllBounds(void)
{
    foreach_all(BoundsMarker, marker)
    {
        foreach_all(Player, player) { BoundsMarker_ApplyBounds(player, marker, true); }
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

        int32 targetDistance = -1;
        int32 targetBoundsID = -1;
        for (int32 b = 0; b < 0x100; ++b) {
            if (!WarpDoor->boundaries[b].left && !WarpDoor->boundaries[b].top && !WarpDoor->boundaries[b].right && !WarpDoor->boundaries[b].bottom)
                break;

            if (player->position.y >= WarpDoor->boundaries[b].top << 16 && player->position.x <= WarpDoor->boundaries[b].right << 16
                && player->position.y <= WarpDoor->boundaries[b].bottom << 16) {
                int32 left = WarpDoor->boundaries[b].left << 16;
                if (player->position.x >= left) {
                    int32 distance = (WarpDoor->boundaries[b].bottom - WarpDoor->boundaries[b].top)
                                     * (WarpDoor->boundaries[b].left - WarpDoor->boundaries[b].right);
                    if (distance < targetDistance) {
                        targetDistance = distance;
                        targetBoundsID = b;
                    }
                }
            }
        }

        player->position.x = storeX;
        player->position.y = storeY;
        if (targetBoundsID >= 0) {
            WarpDoor_SetWarpBounds(targetBoundsID);
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

#if GAME_INCLUDE_EDITOR
    if (SceneInfo->inEditor) {
        // added for RE2, it makes the scene a mess without this
        if (!showGizmos())
            return;
        RSDK_DRAWING_OVERLAY(true); // added for RE2

        int32 color = self->go ? 0xFF0000 : 0x0000FF;

        if (self->destinationTag >= 1 && self->destinationTag <= 0x100) {
            EntityWarpDoor *dest = WarpDoor->tags[self->destinationTag];
            if (dest) {
                DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitbox, FLIP_NONE, color);
                DrawHelpers_DrawArrowAdditive(self->position.x, self->position.y, dest->position.x, dest->position.y, 0x00FFFF);
            }
        }

        if (self->definesBounds) {
            Hitbox hitbox;
            hitbox.left   = self->xBoundaryPosL;
            hitbox.top    = self->yBoundaryPosT;
            hitbox.right  = self->xBoundaryPosR;
            hitbox.bottom = self->yBoundaryPosB;
            DrawHelpers_DrawHitboxOutline(0, 0, &hitbox, FLIP_NONE, color);

            RSDK.DrawLine(self->xBoundaryPosL << 16, self->yBoundaryPosT << 16, self->position.x, self->position.y, color, 0xFF, INK_NONE, false);
            RSDK.DrawLine(self->xBoundaryPosR << 16, self->yBoundaryPosT << 16, self->position.x, self->position.y, color, 0xFF, INK_NONE, false);
            RSDK.DrawLine(self->xBoundaryPosR << 16, self->yBoundaryPosB << 16, self->position.x, self->position.y, color, 0xFF, INK_NONE, false);
            RSDK.DrawLine(self->xBoundaryPosL << 16, self->yBoundaryPosB << 16, self->position.x, self->position.y, color, 0xFF, INK_NONE, false);
        }

        RSDK_DRAWING_OVERLAY(false); // added for RE2
    }
#endif
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

#if GAME_INCLUDE_EDITOR
void WarpDoor_EditorDraw(void)
{
    RSDK_THIS(WarpDoor);
    self->active = ACTIVE_ALWAYS;

    WarpDoor_SetupHitbox();

    if (self->tag >= 1)
        WarpDoor->tags[self->tag] = self;

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
