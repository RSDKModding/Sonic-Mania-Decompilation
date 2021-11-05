#include "SonicMania.h"

ObjectWarpDoor *WarpDoor;

void WarpDoor_Update(void)
{
    RSDK_THIS(WarpDoor);

    if (entity->fadeTimer > 0)
        entity->fadeTimer -= 16;
    if (entity->destinationTag >= 1 && entity->destinationTag <= 256) {
        EntityWarpDoor *tag = (EntityWarpDoor *)WarpDoor->tags[entity->destinationTag];
        if (tag) {
            bool32 flag = false;
            int32 boundID = -1;
            foreach_all(Player, player)
            {
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    flag = true;
                    if (!entity->field_A4) {
                        EntityCamera *camera = player->camera;
                        int32 camRelPosX       = 0;
                        int32 camRelPosY       = 0;
                        int32 playerX          = player->position.x;
                        int32 playerY          = player->position.y;
                        if (camera) {
                            camRelPosX = camera->position.x - playerX;
                            camRelPosY = camera->position.y - playerY;
                        }
                        int32 relPosX = playerX - entity->position.x;
                        int32 relPosY = playerY - entity->position.y;
                        LogHelpers_Print("camRelPos = <%d, %d>", camRelPosX >> 16, camRelPosY >> 16);
                        LogHelpers_Print("relPos = <%d, %d>", relPosX >> 16, relPosY >> 16);

                        Vector2 posPtr;
                        posPtr.x = tag->position.x;
                        posPtr.y = tag->position.y;
                        if (!entity->warpToCenter) {
                            posPtr.x += relPosY;
                            posPtr.y += relPosY;
                        }

                        if (entity->forcePlayerState) {
                            player->nextAirState    = StateMachine_None;
                            player->nextGroundState = StateMachine_None;
                            player->onGround        = false;
                            player->state           = Player_State_Hit;
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
                        }

                        if (camera) {
                            int32 screenY = camRelPosY + posPtr.y;
                            int32 screenX = camRelPosX + posPtr.x;
                            LogHelpers_Print("newPos = <%d, %d>", posPtr.x >> 16, posPtr.y >> 16);
                            LogHelpers_Print("newCamPos = <%d, %d>", screenX >> 16, screenY >> 16);
                            camera->position.x                        = screenX;
                            camera->position.y                        = screenY;
                            RSDK_screens[camera->screenID].position.x = (camera->position.x >> 16);
                            RSDK_screens[camera->screenID].position.y = (camera->position.y >> 16);
                            camera->state                             = Camera_State_Follow;
                            player->scrollDelay                       = 0;
                            WarpDoor_Unknown5(boundID, &posPtr);
                            int32 pID                   = RSDK.GetEntityID(player);
                            camera->boundsL           = Zone->screenBoundsL1[pID];
                            camera->boundsR           = Zone->screenBoundsR1[pID];
                            camera->boundsT           = Zone->screenBoundsT1[pID];
                            camera->boundsB           = Zone->screenBoundsB1[pID];
                            Zone->screenBoundsL2[pID] = Zone->screenBoundsL1[pID] << 16;
                            Zone->screenBoundsR2[pID] = Zone->screenBoundsR1[pID] << 16;
                            Zone->screenBoundsT2[pID] = Zone->screenBoundsT1[pID] << 16;
                            Zone->screenBoundsB2[pID] = Zone->screenBoundsB1[pID] << 16;
                            Zone->deathBoundary[pID]  = Zone->screenBoundsB1[pID] << 16;
                        }

                        if (tag->definesBounds)
                            boundID = tag->boundID;
                        tag->field_A4      = true;
                        player->position.x = posPtr.x;
                        player->position.y = posPtr.y;
                        WarpDoor_Unknown4();

                        if (!player->sidekick) {
                            if (RSDK.CheckStageFolder("TMZ2")) {
                                if (entity->effect == 1) {
                                    int32 sfx     = RSDK.Rand(0, 6);
                                    int32 channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackL[sfx], false, 0xFF);
                                    RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                                    channel = RSDK.PlaySfx(WarpDoor->sfxRubyAttackR[sfx], 0, 0xFF);
                                    RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                                    foreach_active(TMZBarrier, barrier)
                                    {
                                        if (barrier->warpTag == entity->tag)
                                           barrier->field_5C = 1;
                                    }
                                    tag->fadeTimer = 512;
                                    if (entity->destinationTag == 99 || entity->destinationTag == 100)
                                       TMZ2_DrawDynTiles_Eggman();
                                    else
                                       TMZ2_DrawDynTiles_Ruby();
                                }
                            }
                            else if (RSDK.CheckStageFolder("OOZ2")) {
                                if (entity->effect == 1 || entity->effect == 2) {
                                    OOZSetup->flags = entity->effect == 2 ? 1 : 0;
                                    destroyEntity(entity);
                                    foreach_return;
                                }
                            }
                            else if (RSDK.CheckStageFolder("FBZ")) {
                                RSDK.PlaySfx(WarpDoor->sfxWarpDoor, 0, 255);
                                flag = false;
                                if (entity->go) {
                                    tag->fadeTimer = 256;
                                    tag->field_B4  = 1;
                                }
                                else {
                                    tag->field_B4  = 0;
                                    tag->fadeTimer = 320;
                                }
                            }
                        }
                    }
                }
            }
            if (flag)
                return;
            if (entity->field_A4)
                entity->field_A4 = false;
        }
    }
}

void WarpDoor_LateUpdate(void) {}

void WarpDoor_StaticUpdate(void)
{
    if (!WarpDoor->field_1408) {
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
        WarpDoor->field_1408 = true;
    }

    if (WarpDoor->boundCount) {
        if (!WarpDoor->field_140C) {
            if (WarpDoor_Unknown5(-1, 0)) {
                WarpDoor->field_140C = 1;
                WarpDoor_Unknown1();
                WarpDoor_Unknown4();
            }
        }

        if (WarpDoor->field_1406 > 0)
            WarpDoor->field_1406--;
    }
}

void WarpDoor_Draw(void)
{
    RSDK_THIS(WarpDoor);
    if (DebugMode->debugActive)
        WarpDoor_DrawDebug();
    if (entity->fadeTimer > 0) {
        if (RSDK.CheckStageFolder("FBZ")) {
            if (entity->field_B4)
                RSDK.FillScreen(0x000000, entity->fadeTimer, entity->fadeTimer - 128, entity->fadeTimer - 256);
            else
                RSDK.FillScreen(0x800080, entity->fadeTimer, entity->fadeTimer - 256, entity->fadeTimer);
        }
        else {
            RSDK.FillScreen(0xFFF0F0, entity->fadeTimer, entity->fadeTimer - 256, entity->fadeTimer - 256);
        }
    }
}

void WarpDoor_Create(void *data)
{
    RSDK_THIS(WarpDoor);
    RSDK.SetSpriteAnimation(WarpDoor->aniFrames, 0, &entity->animator, true, 0);
    WarpDoor_Unknown7();
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_ALWAYS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->field_A4      = false;
        entity->drawFX        = FX_NONE;
        entity->inkEffect     = INK_NONE;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
    }
}

void WarpDoor_StageLoad(void)
{
    WarpDoor->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);
    WarpDoor_SetStageBounds();
    WarpDoor->field_140C = 0;
    WarpDoor->field_1408 = 0;
    WarpDoor->boundCount = 0;
    for (int32 i = 0; i < 0x100; ++i) {
        WarpDoor->tags[i] = NULL;
    }
    if (RSDK.CheckStageFolder("FBZ"))
        WarpDoor->sfxWarpDoor = RSDK.GetSFX("FBZ/WarpDoor.wav");
    if (RSDK.CheckStageFolder("TMZ2")) {
        WarpDoor->sfxRubyAttackL[0] = RSDK.GetSFX("Ruby/Attack1_L.wav");
        WarpDoor->sfxRubyAttackR[0] = RSDK.GetSFX("Ruby/Attack1_R.wav");
        WarpDoor->sfxRubyAttackL[1] = RSDK.GetSFX("Ruby/Attack2_L.wav");
        WarpDoor->sfxRubyAttackR[1] = RSDK.GetSFX("Ruby/Attack2_R.wav");
        WarpDoor->sfxRubyAttackL[2] = RSDK.GetSFX("Ruby/Attack3_L.wav");
        WarpDoor->sfxRubyAttackR[2] = RSDK.GetSFX("Ruby/Attack3_R.wav");
        WarpDoor->sfxRubyAttackL[3] = RSDK.GetSFX("Ruby/Attack4_L.wav");
        WarpDoor->sfxRubyAttackR[3] = RSDK.GetSFX("Ruby/Attack4_R.wav");
        WarpDoor->sfxRubyAttackL[4] = RSDK.GetSFX("Ruby/Attack5_L.wav");
        WarpDoor->sfxRubyAttackR[4] = RSDK.GetSFX("Ruby/Attack5_R.wav");
        WarpDoor->sfxRubyAttackL[5] = RSDK.GetSFX("Ruby/Attack6_L.wav");
        WarpDoor->sfxRubyAttackR[5] = RSDK.GetSFX("Ruby/Attack6_R.wav");
    }
}

void WarpDoor_Unknown1(void)
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
    Zone->screenBoundsL1[0]     = 0;
    Zone->screenBoundsR1[0]     = size.x;
    Zone->screenBoundsT1[0]     = 0;
    Zone->screenBoundsB1[0]     = size.y;
    Zone->screenBoundsL2[0]     = Zone->screenBoundsL1[0] << 16;
    Zone->screenBoundsR2[0]     = Zone->screenBoundsR1[0] << 16;
    Zone->screenBoundsT2[0]     = Zone->screenBoundsT1[0] << 16;
    Zone->screenBoundsB2[0]     = Zone->screenBoundsB1[0] << 16;
    Zone->deathBoundary[0]      = Zone->screenBoundsB1[0] << 16;
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
    Zone->screenBoundsT1[0]     = WarpDoor->boundaries[id].top;
    Zone->screenBoundsR1[0]     = WarpDoor->boundaries[id].right;
    Zone->screenBoundsB1[0]     = WarpDoor->boundaries[id].bottom;
    Zone->screenBoundsL1[0]     = WarpDoor->boundaries[id].left;
    Zone->screenBoundsT2[0]     = WarpDoor->boundaries[id].top << 16;
    Zone->screenBoundsR2[0]     = WarpDoor->boundaries[id].right << 16;
    Zone->screenBoundsB2[0]     = WarpDoor->boundaries[id].bottom << 16;
    Zone->screenBoundsL2[0]     = WarpDoor->boundaries[id].left << 16;
    Zone->deathBoundary[0]      = WarpDoor->boundaries[id].bottom << 16;
    WarpDoor->field_1406        = 16;
    WarpDoor->field_1405        = id;
}

void WarpDoor_Unknown4(void)
{
    foreach_all(BoundsMarker, marker)
    {
        foreach_all(Player, player) { BoundsMarker_CheckBounds(player, marker, true); }
    }
}

bool32 WarpDoor_Unknown5(int16 a1, Vector2 *posPtr)
{
    if (!WarpDoor->boundCount)
        return false;
    if (Zone->timer < 1)
        return false;
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (!player || !player->camera)
        return false;
    if (a1 > -1) {
        WarpDoor_SetWarpBounds(a1);
        return false;
    }

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
                int32 dist =
                    (WarpDoor->boundaries[i].bottom - WarpDoor->boundaries[i].top) * (WarpDoor->boundaries[i].left - WarpDoor->boundaries[i].right);
                if (dist < distStore) {
                    distStore = dist;
                    id        = i;
                }
            }
        }
    }

    player->position.x = storeX;
    player->position.y = storeY;
    if (id < 0)
        return 0;
    WarpDoor_SetWarpBounds(id);
    return true;
}

void WarpDoor_DrawDebug(void)
{
    RSDK_THIS(WarpDoor);
    int32 x = (entity->hitbox.left << 16) + entity->position.x + 0x80000;
    int32 y = (entity->hitbox.top << 16) + entity->position.y + 0x80000;

    entity->animator.frameID = entity->go ? 2 : 0;

    int32 xOff = 0;
    int32 yOff = 0;
    for (int32 i = 0; i < entity->width * entity->height; ++i) {
        Vector2 drawPos;
        drawPos.y = y + yOff;
        drawPos.x = x + (xOff << 20);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        if (++xOff == entity->width) {
            yOff += 0x100000;
            xOff = 0;
        }
    }

    if (RSDK_sceneInfo->inEditor) {
        int32 colour = 0xFF0000;
        if (!entity->go)
            colour = 0x0000FF;

        if (entity->destinationTag >= 1 && entity->destinationTag <= 256) {
            EntityWarpDoor *dest = (EntityWarpDoor *)WarpDoor->tags[entity->destinationTag];
            if (dest) {
                DrawHelpers_DrawHitboxOutline(colour, FLIP_NONE, entity->position.x, entity->position.y, &entity->hitbox);
                DrawHelpers_DrawArrow(0x00FFFF, entity->position.x, dest->position.x, entity->position.y, dest->position.y);
            }
        }

        if (entity->definesBounds) {
            Hitbox hitbox;
            hitbox.left   = entity->xBoundaryPosL;
            hitbox.top    = entity->yBoundaryPosT;
            hitbox.right  = entity->xBoundaryPosR;
            hitbox.bottom = entity->yBoundaryPosB;
            DrawHelpers_DrawHitboxOutline(colour, 0, 0, 0, &hitbox);
            RSDK.DrawLine(entity->xBoundaryPosL << 16, entity->yBoundaryPosT << 16, entity->position.x, entity->position.y, colour, 0xFF, INK_NONE, false);
            RSDK.DrawLine(entity->xBoundaryPosR << 16, entity->yBoundaryPosT << 16, entity->position.x, entity->position.y, colour, 0xFF, INK_NONE, false);
            RSDK.DrawLine(entity->xBoundaryPosR << 16, entity->yBoundaryPosB << 16, entity->position.x, entity->position.y, colour, 0xFF, INK_NONE, false);
            RSDK.DrawLine(entity->xBoundaryPosL << 16, entity->yBoundaryPosB << 16, entity->position.x, entity->position.y, colour, 0xFF, INK_NONE, false);
        }
    }
}

void WarpDoor_Unknown7(void)
{
    RSDK_THIS(WarpDoor);
    if (!entity->width)
        entity->width = 2;
    if (!entity->height)
        entity->height = 4;
    entity->hitbox.left   = -((16 * entity->width) >> 1);
    entity->hitbox.right  = (16 * entity->width) >> 1;
    entity->hitbox.top    = -((16 * entity->height) >> 1);
    entity->hitbox.bottom = (16 * entity->height) >> 1;
}

void WarpDoor_EditorDraw(void)
{
    RSDK_THIS(WarpDoor);
    entity->active = ACTIVE_ALWAYS;
    WarpDoor_DrawDebug();
}

void WarpDoor_EditorLoad(void) { WarpDoor->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

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
