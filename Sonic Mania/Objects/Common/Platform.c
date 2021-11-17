#include "SonicMania.h"

ObjectPlatform *Platform;

void Platform_Update(void)
{
    RSDK_THIS(Platform);
#if RETRO_USE_PLUS
    if (Zone->flag) {
        entity->pushPlayersL = false;
        entity->pushPlayersR = false;
    }
#endif
    entity->position.x = entity->drawPos.x & 0xFFFF0000;
    entity->position.y = entity->drawPos.y & 0xFFFF0000;

    if (entity->hasTension) {
        if (entity->stood && entity->stoodAngle < 64)
            entity->stoodAngle += 4;
        else if (!entity->stood && entity->stoodAngle > 0)
            entity->stoodAngle -= 4;
    }

    entity->collisionOffset.x = -entity->position.x;
    entity->collisionOffset.y = -entity->position.y;

    StateMachine_Run(entity->state);

    if (entity->objectID) {
        entity->stood = false;
        entity->collisionOffset.x += entity->drawPos.x & 0xFFFF0000;
        entity->collisionOffset.y += entity->drawPos.y & 0xFFFF0000;
        if (entity->state != Platform_State_Falling && entity->state != Platform_State_OffScreenReset) {
            StateMachine_Run(entity->stateCollide);
        }

        entity->position.x = entity->centerPos.x;
        entity->position.y = entity->centerPos.y;
        for (int32 s = RSDK_sceneInfo->entitySlot + 1, i = 0; i < entity->childCount; ++i) {
            Entity *child = RSDK.GetEntityByID(s++);
            if (child->objectID == ItemBox->objectID) {
                if (!child->scale.y) {
                    EntityItemBox *itemBox = (EntityItemBox *)child;
                    itemBox->parent        = (Entity *)entity;
                    itemBox->scale.x       = itemBox->position.x - entity->centerPos.x;
                    itemBox->scale.y       = itemBox->position.y - entity->centerPos.y;
                    itemBox->position.x    = itemBox->scale.x + entity->drawPos.x;
                    itemBox->position.y    = itemBox->scale.y + entity->drawPos.y;
                    itemBox->updateRange.x = entity->updateRange.x;
                    itemBox->updateRange.y = entity->updateRange.y;
                }
            }
            else {
                child->position.x += entity->collisionOffset.x;
                child->position.y += entity->collisionOffset.y;
                if (child->objectID == Spikes->objectID) {
                    EntitySpikes *spikes = (EntitySpikes *)child;
                    spikes->offset.x     = entity->collisionOffset.x;
                    spikes->offset.y     = entity->collisionOffset.y;
                }
                else if (child->objectID == Platform->objectID) {
                    EntityPlatform *platform = (EntityPlatform *)child;
                    platform->centerPos.x += entity->collisionOffset.x;
                    platform->centerPos.y += entity->collisionOffset.y;
                }
                else if (Ice && child->objectID == Ice->objectID) {
                    EntityIce *ice = (EntityIce *)child;
                    ice->playerPos = entity->collisionOffset;
                }
                else if (TurboTurtle && child->objectID == TurboTurtle->objectID) {
                    EntityTurboTurtle *turboTurtle = (EntityTurboTurtle *)child;
                    turboTurtle->startPos.x += entity->collisionOffset.x;
                    turboTurtle->startPos.y += entity->collisionOffset.y;
                }
            }
        }
        if (entity->animator.frameDelay)
            RSDK.ProcessAnimation(&entity->animator);
    }
}

void Platform_LateUpdate(void) {}

void Platform_StaticUpdate(void)
{
    for (int32 i = 0; i < 4; ++i) {
        Platform->stoodPos[i].x = 0;
        Platform->stoodPos[i].y = 0;
    }
}

void Platform_Draw(void)
{
    RSDK_THIS(Platform);
    Vector2 drawPos;

    if (entity->frameID >= 0) {
        if ((entity->state != Platform_State_Circular || !entity->hasTension)
            && (entity->state != Platform_State_Swing && entity->state != Platform_State_14 && entity->type != PLATFORM_12)) {
            if (Platform->aniFrames == 0xFFFF)
                RSDK.DrawRect(entity->drawPos.x - 0x200000, entity->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, 255, INK_NONE, false);
            else
                RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
        }
        else {
            int32 ang = 0;
            if (entity->state == Platform_State_Circular && entity->hasTension)
                ang = entity->speed * Zone->timer + 4 * entity->angle;
            else
                ang = entity->angle;
            int32 fxStore = entity->drawFX;
            entity->drawFX |= FX_FLIP | FX_ROTATE;
            int32 cnt                = (entity->amplitude.y >> 10) - 1;
            int32 angle              = 0x400;
            entity->direction        = FLIP_NONE;
            entity->animator.frameID = entity->frameID + 1;

            int32 rot = ang >> 1;
            for (int32 i = 0; i < cnt; ++i) {
                drawPos.x = angle * RSDK.Cos1024(ang) + entity->centerPos.x;
                drawPos.y = angle * RSDK.Sin1024(ang) + entity->centerPos.y;
                RSDK.DrawSprite(&entity->animator, &drawPos, false);
                angle += 0x400;
                entity->direction ^= FLIP_X;
                entity->rotation = rot;
            }

            entity->drawFX           = fxStore;
            entity->animator.frameID = entity->frameID + 2;
            RSDK.DrawSprite(&entity->animator, &entity->centerPos, false);
            entity->animator.frameID = entity->frameID;

            if (Platform->aniFrames == 0xFFFF)
                RSDK.DrawRect(entity->drawPos.x - 0x200000, entity->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, 255, INK_NONE, false);
            else
                RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
        }
    }
}

void Platform_Create(void *data)
{
    RSDK_THIS(Platform);
    entity->amplitude.x >>= 10;
    entity->amplitude.y >>= 10;
    entity->active      = ACTIVE_BOUNDS;
    entity->visible     = true;
    entity->drawOrder   = Zone->drawOrderLow + 1;
    entity->centerPos.x = entity->position.x;
    entity->centerPos.y = entity->position.y;
    entity->drawPos.x   = entity->position.x;
    entity->drawPos.y   = entity->position.y;
    RSDK.SetSpriteAnimation(Platform->aniFrames, 0, &entity->animator, true, 0);
    entity->animator.frameID = entity->frameID;
    if (!RSDK_sceneInfo->inEditor && RSDK.GetFrameID(&entity->animator) == 108)
        entity->drawOrder = Zone->drawOrderLow;

    switch (entity->type) {
        case PLATFORM_0:
        default:
            entity->state         = Platform_State_Normal;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case PLATFORM_1:
            entity->updateRange.x = 0x800000;
            entity->state         = Platform_State_Collapsing;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            break;
        case PLATFORM_2:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 10;
            entity->state         = Platform_State_Moving;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            entity->rotation      = entity->angle;
            entity->angle         = 0;
            break;
        case PLATFORM_3:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 10;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            entity->state         = Platform_State_Circular;
            break;
        case PLATFORM_5:
        case PLATFORM_11:
            entity->active = ACTIVE_BOUNDS;
            if (entity->type == PLATFORM_5)
                entity->state = Platform_State_Nothing;
            else
                entity->state = Platform_Unknown8;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case PLATFORM_6:
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            if (!RSDK_sceneInfo->inEditor) {
                entity->speed <<= 11;
                entity->position.x += 0x8000;
            }
            entity->state = Platform_State_StartPush;
            break;
        case PLATFORM_8:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);
            if (entity->speed < 0)
                entity->direction = FLIP_X;
            entity->state = Platform_State_Wait;
            break;
        case PLATFORM_9:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);
            if (entity->speed < 0)
                entity->direction = 1;
            entity->state = Platform_State_WaitBobbing;
            break;
        case PLATFORM_10:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);

            if (entity->speed < 0) {
                entity->direction = 1;
                entity->speed     = -entity->speed;
            }
            entity->state = Platform_State_PlayerActivated;
            break;
        case PLATFORM_12: entity->collapseDelay = 88;
        case PLATFORM_4:
            entity->updateRange.x = (abs(entity->amplitude.y) + 512) << 14;
            entity->updateRange.y = (abs(entity->amplitude.y) + 512) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &entity->animator, true, 0);
            entity->amplitude.y *= 16;
            entity->groundVel = 4 * entity->angle;
            entity->angle     = entity->groundVel + 256 + (entity->amplitude.x * RSDK.Sin1024(entity->speed * entity->collapseDelay) >> 14);
            entity->drawPos.x = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
            entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
            if (entity->type == PLATFORM_4) {
                entity->state = Platform_State_Swing;
            }
            else {
                if (entity->groundVel >= 0)
                    entity->drawPos.x -= 0x200000;
                else
                    entity->drawPos.x += 0x200000;
                entity->state = Platform_State_12;
            }
            break;
        case PLATFORM_13:
            if (entity->direction) {
                entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9);
                entity->drawPos.y = entity->centerPos.y + (entity->amplitude.y << 9);
            }
            else {
                entity->drawPos.x = entity->centerPos.x - (entity->amplitude.x << 9);
                entity->drawPos.y = entity->centerPos.y - (entity->amplitude.y << 9);
            }
        case PLATFORM_7:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x4000) << 9;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x4000) << 9;
            if (entity->speed < 0)
                entity->direction = FLIP_X;

            if (entity->type == PLATFORM_7)
                entity->state = Platform_State_MovingSpike;
            else
                entity->state = Platform_State_13;
            break;
        case PLATFORM_14:
            entity->updateRange.x = (abs(entity->amplitude.y) + 512) << 14;
            entity->updateRange.y = (abs(entity->amplitude.y) + 512) << 14;
            RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &entity->animator, true, 0);
            entity->amplitude.y *= 16;
            entity->groundVel = 4 * entity->angle;
            entity->angle     = entity->groundVel + 256 + (entity->amplitude.x * RSDK.Sin1024(entity->speed * entity->collapseDelay) >> 14);
            entity->drawPos.x = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
            entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
            entity->state     = Platform_State_14;
            break;
        case PLATFORM_15:
            entity->state         = Platform_State_15;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case PLATFORM_16:
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            if (!entity->speed)
                entity->speed = 1;
            entity->amplitude.y = entity->amplitude.y << 10;
            entity->velocity.y  = entity->speed << 16;
            entity->state       = Platform_State_16;
            break;
    }

    if (entity->frameID >= 0) {
        int32 f    = entity->frameID;
        int32 anim = 0;
        while (f >= entity->animator.frameCount) {
            if (!entity->animator.frameCount)
                break;
            f -= entity->animator.frameCount;
            RSDK.SetSpriteAnimation(Platform->aniFrames, ++anim, &entity->animator, true, 0);
        }
        entity->frameID          = f;
        entity->animator.frameID = f;
    }
    else {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator, true, 0);
    }

    if (!RSDK_sceneInfo->inEditor) {
        if (entity->collision != PLATFORM_C_4) {
            Hitbox *hitbox = RSDK.GetHitbox(&entity->animator, entity->collision != PLATFORM_C_0);
            if (Platform->aniFrames != 0xFFFF && hitbox) {
                entity->hitbox.left   = hitbox->left;
                entity->hitbox.top    = hitbox->top;
                entity->hitbox.right  = hitbox->right;
                entity->hitbox.bottom = hitbox->bottom;
            }
            else {
                entity->hitbox.left   = -32;
                entity->hitbox.top    = -16;
                entity->hitbox.right  = -8;
                entity->hitbox.bottom = 32;
            }
        }
        switch (entity->collision) {
            case PLATFORM_C_0:
                entity->stateCollide  = Platform_CollisionState_TopSolid;
                entity->hitbox.bottom = entity->hitbox.top + 8;
                break;
            case PLATFORM_C_1:
            default: entity->stateCollide = Platform_CollisionState_AllSolid; break;
            case PLATFORM_C_2: entity->stateCollide = Platform_CollisionState_Tiles; break;
            case PLATFORM_C_3: entity->stateCollide = Platform_CollisionState_AllHazard; break;
            case PLATFORM_C_4: entity->stateCollide = Platform_CollisionState_None; break;
            case PLATFORM_C_5: entity->stateCollide = Platform_CollisionState_LRHazard; break;
            case PLATFORM_C_6: entity->stateCollide = Platform_CollisionState_BottomHazard; break;
            case PLATFORM_C_7: entity->stateCollide = Platform_CollisionState_TopHazard; break;
            case PLATFORM_C_8: entity->stateCollide = Platform_CollisionState_Twister; break;
            case PLATFORM_C_9:
            case PLATFORM_C_10:
            case PLATFORM_C_11:
            case PLATFORM_C_12:
            case PLATFORM_C_13: entity->stateCollide = Platform_CollisionState_Sticky; break;
            case PLATFORM_C_14: entity->stateCollide = Platform_CollisionState_TurnTable; break;
            case PLATFORM_C_15: entity->stateCollide = Platform_CollisionState_AllSolid_NoCrush; break;
        }
        for (int32 i = 0; i < entity->childCount; ++i) {
            EntityPlatform *child = RSDK_GET_ENTITY((i + RSDK.GetEntityID(entity) + 1), Platform);
            child->tileCollisions = false;
            if (HangPoint && child->objectID == HangPoint->objectID) {
                EntityHangPoint *hang = (EntityHangPoint *)child;
                if (entity->updateRange.y < 0x800000 + abs(entity->position.y - (hang->length << 16) - hang->position.y))
                    entity->updateRange.y = 0x800000 + abs(entity->position.y - (hang->length << 16) - hang->position.y);
            }
            else {
                if (entity->updateRange.y < 0x800000 + abs(entity->position.y - child->position.y))
                    entity->updateRange.y = 0x800000 + abs(entity->position.y - child->position.y);
            }
        }
    }
}

void Platform_StageLoad(void)
{
    Platform->useClack = false;
    if (RSDK.CheckStageFolder("GHZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("GHZCutscene")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("GHZCutscene/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Platform.bin", SCOPE_STAGE);
        Platform->sfxClack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("PSZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE);
        Platform->sfxClack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("MSZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MMZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("AIZ")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Blueprint")) {
        Platform->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/Platform.bin", SCOPE_STAGE);
    }

    Platform->sfxClacker = RSDK.GetSFX("Stage/Clacker.wav");
    Platform->sfxClang   = RSDK.GetSFX("Stage/Clang.wav");
    Platform->sfxPush    = RSDK.GetSFX("Global/Push.wav");
    if (Platform->playingPushSFX) {
        RSDK.StopSFX(Platform->sfxPush);
        Platform->playingPushSFX = 0;
    }
}

void Platform_State_Nothing(void)
{
    RSDK_THIS(Platform);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
}

void Platform_State_Normal(void)
{
    RSDK_THIS(Platform);
    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y;
    entity->drawPos.y += RSDK.Sin256(entity->stoodAngle) << 10;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
}

void Platform_State_Moving(void)
{
    RSDK_THIS(Platform);
    int32 drawX       = -entity->drawPos.x;
    int32 drawY       = -entity->drawPos.y;
    entity->drawPos.x = entity->amplitude.x * RSDK.Sin1024(entity->speed * (entity->rotation + Zone->timer)) + entity->centerPos.x;
    entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * (entity->rotation + Zone->timer)) + entity->centerPos.y;
    entity->drawPos.y += RSDK.Sin256(entity->stoodAngle) << 10;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_Swing(void)
{
    RSDK_THIS(Platform);
    int32 drawX        = -entity->drawPos.x;
    int32 drawY        = -entity->drawPos.y;
    entity->angle      = entity->groundVel + 0x100 + ((entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + 0x200) >> 14);
    entity->drawPos.x  = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
    entity->drawPos.y  = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_Wait(void)
{
    RSDK_THIS(Platform);
    if (entity->stood) {
        if (entity->hasTension)
            entity->collapseDelay = 120;
        entity->rotation     = 0;
        entity->tileOrigin.x = 0;
        entity->tileOrigin.y = 0;
        entity->active       = ACTIVE_NORMAL;
        entity->state        = Platform_State_PlayerMove;
    }
    entity->velocity.y = 0;
    entity->velocity.x = 0;
}

void Platform_State_WaitBobbing(void)
{
    RSDK_THIS(Platform);
    entity->rotation += 4;
    entity->drawPos.y = entity->centerPos.y + (RSDK.Sin1024(entity->rotation) << 9);
    if (entity->stood) {
        if (entity->hasTension)
            entity->collapseDelay = 120;
        entity->tileOrigin.x = entity->drawPos.x - entity->centerPos.x;
        entity->centerPos.x  = entity->drawPos.x;
        entity->tileOrigin.y = RSDK.Sin1024(entity->rotation) << 9;
        entity->centerPos.y  = entity->drawPos.y;
        entity->active       = ACTIVE_NORMAL;
        entity->state        = Platform_State_PlayerMove;
    }
    entity->velocity.y = 0;
    entity->velocity.x = 0;
}

void Platform_State_StartPush(void)
{
    RSDK_THIS(Platform);
    entity->active        = ACTIVE_NORMAL;
    entity->updateRange.x = 0x2000000;
    entity->updateRange.y = 0x2000000;
    entity->state         = Platform_State_Pushable;
    entity->velocity.x    = 0;
    entity->velocity.y    = 0;
}

void Platform_State_StartFalling(void)
{
    RSDK_THIS(Platform);
    if (--entity->collapseDelay <= 0) {
        entity->collapseDelay = 0;
        entity->state         = Platform_State_Falling;
        foreach_active(Player, player)
        {
            if ((1 << RSDK.GetEntityID(player)) & entity->stoodPlayers)
                player->velocity.y = entity->velocity.y - 0x10000;
        }
    }

    entity->drawPos.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->velocity.x = 0;
}

void Platform_State_PlayerActivated(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            int32 angle = (uint8)((uint8)(entity->angle) + -0x80);
            int32 xOff  = (player->position.x - entity->centerPos.x) >> 8;
            int32 yOff  = (player->position.y - entity->centerPos.y) >> 8;
            RSDK.Sin256(angle);
            RSDK.Cos256(angle);

            int32 cy = (yOff * RSDK.Cos256(angle)) - xOff * RSDK.Sin256(angle);

            // int32 center = cy + entity->centerPos.y;
            if (abs(player->position.x - entity->centerPos.x) <= 0x4000000) {
                if (abs(player->position.y - entity->centerPos.y) <= 0x4000000 && (cy + entity->centerPos.y) < entity->centerPos.y) {
                    if (entity->centerPos.y - (cy + entity->centerPos.y) < 0x1000000)
                        flag = true;
                }
            }
        }
    }

    if (!flag) {
        if (entity->amplitude.y > 0) {
            entity->amplitude.y = entity->amplitude.y - (entity->speed << 16);
            if (entity->amplitude.y <= 0) {
                entity->amplitude.y = 0;
                if (entity->activeScreens && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, 0, 255);
            }
        }
    }
    else {
        if (entity->amplitude.y < entity->amplitude.x) {
            entity->amplitude.y = entity->amplitude.y + (entity->speed << 16);
            if (entity->amplitude.y >= entity->amplitude.x) {
                entity->amplitude.y = entity->amplitude.x;
                if (entity->activeScreens && Platform->useClack)
                    RSDK.PlaySfx(Platform->sfxClack, 0, 255);
            }
        }
    }

    if (entity->direction) {
        entity->drawPos.x = (-entity->amplitude.y >> 8) * RSDK.Cos256(entity->angle) + entity->centerPos.x;
        entity->drawPos.y = (-entity->amplitude.y >> 8) * RSDK.Sin256(entity->angle) + entity->centerPos.y;
    }
    else {
        entity->drawPos.x = (entity->amplitude.y >> 8) * RSDK.Cos256(entity->angle) + entity->centerPos.x;
        entity->drawPos.y = (entity->amplitude.y >> 8) * RSDK.Sin256(entity->angle) + entity->centerPos.y;
    }

    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_PlayerMove(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;
    entity->amplitude.y += entity->groundVel;

    entity->drawPos.x = (entity->amplitude.y >> 8) * RSDK.Cos256(entity->angle) + entity->centerPos.x;
    entity->drawPos.y = (entity->amplitude.y >> 8) * RSDK.Sin256(entity->angle) + entity->centerPos.y;

    int32 speed16 = entity->speed << 16;
    if (entity->groundVel == speed16) {
        if (entity->amplitude.y >= entity->amplitude.x) {
            entity->amplitude.y = entity->amplitude.x;
            entity->groundVel   = entity->groundVel - (entity->speed << 11);
            entity->state       = Platform_Unknown5;
        }
    }
    else {
        entity->groundVel += (entity->speed << 10);
        if (entity->groundVel >= speed16) {
            entity->groundVel = speed16;
            entity->centerPos.x += ((entity->amplitude.y + entity->groundVel) >> 8) * RSDK.Cos256(entity->angle);
            entity->centerPos.y += ((entity->groundVel + entity->amplitude.y) >> 8) * RSDK.Sin256(entity->angle);
            entity->amplitude.y = -entity->groundVel;
        }
    }
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_Pushable(void)
{
    RSDK_THIS(Platform);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    foreach_active(Player, playerLoop)
    {
        uint8 id = 1 << RSDK.GetEntityID(playerLoop);
        if (id & entity->pushPlayersL)
            entity->velocity.x += entity->speed;
        if (id & entity->pushPlayersR)
            entity->velocity.x -= entity->speed;
    }

    if (entity->velocity.x > 0 || entity->velocity.x < 0) {
        if (!Platform->playingPushSFX) {
            RSDK.PlaySfx(Platform->sfxPush, true, 255);
            Platform->playingPushSFX = true;
        }
    }

    if (entity->velocity.x) {
        if (entity->collapseDelay > 0) {
            entity->collapseDelay--;
            entity->velocity.x = 0;
        }
    }
    else {
        if (Platform->playingPushSFX) {
            RSDK.StopSFX(Platform->sfxPush);
            Platform->playingPushSFX = false;
        }
        if (entity->collapseDelay < 4) {
            entity->collapseDelay++;
        }
    }

    entity->drawPos.x += entity->velocity.x;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 bitID          = 1 << RSDK.GetEntityID(&player);
        if (bitID & entity->pushPlayersL)
            player->position.x = entity->drawPos.x + ((entity->hitbox.left - playerHitbox->right) << 16);
        if (bitID & entity->pushPlayersR)
            player->position.x = entity->drawPos.x + ((entity->hitbox.right - playerHitbox->left - 1) << 16);
    }

    bool32 collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, (entity->hitbox.left + 16) << 16, entity->hitbox.bottom << 16, 4);
    int32 y         = entity->position.y;

    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0, entity->hitbox.bottom << 16, 4);
    if (entity->position.y < y)
        y = entity->position.y;

    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, (entity->hitbox.right - 16) << 16, entity->hitbox.bottom << 16, 4);
    if (entity->position.y < y)
        y = entity->position.y;

    entity->position.y = y;
    entity->drawPos.y  = y;
    if (!collided) {
        int32 vel          = 2 * entity->velocity.x;
        entity->velocity.x = vel;
        if (!vel) {
            entity->state = Platform_Unknown3;
        }
        else {
            if (vel > 0)
                entity->state = Platform_Unknown1;
            else
                entity->state = Platform_Unknown2;
        }
    }
    RSDK.ObjectTileCollision(entity, Zone->fgLayers, 3, 0, entity->hitbox.left << 16, (entity->hitbox.bottom - 8) << 16, 1);
    RSDK.ObjectTileCollision(entity, Zone->fgLayers, 1, 0, entity->hitbox.right << 16, (entity->hitbox.bottom - 8) << 16, 1);
    RSDK.ObjectTileCollision(entity, Zone->fgLayers, 3, 0, entity->hitbox.left << 16, (entity->hitbox.top + 8) << 16, 1);
    RSDK.ObjectTileCollision(entity, Zone->fgLayers, 1, 0, entity->hitbox.right << 16, (entity->hitbox.top + 8) << 16, 1);

    entity->drawPos.x = entity->position.x;
    entity->drawPos.y = entity->position.y;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->speed >>= 11;
        entity->drawPos.x  = entity->centerPos.x;
        entity->drawPos.y  = entity->centerPos.y;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->visible    = false;
        if (Platform->playingPushSFX) {
            RSDK.StopSFX(Platform->sfxPush);
            Platform->playingPushSFX = false;
        }
        entity->state = Platform_State_OffScreenReset;
    }
}

void Platform_State_OffScreenReset(void)
{
    RSDK_THIS(Platform);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        Platform_Create(NULL);
    entity->velocity.y = 0;
    entity->velocity.x = 0;
}

void Platform_State_MovingSpike(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;
    int32 move  = Zone->timer * (entity->speed << 7);

    int32 y1, y2;
    if (((move >> 16) & 1) == entity->direction) {
        entity->drawPos.x = entity->centerPos.x + ((uint16)move * entity->amplitude.x >> 6) - (entity->amplitude.x << 9);
        y1                = (uint16)move * entity->amplitude.y >> 6;
        y2                = entity->amplitude.y << 9;
    }
    else {
        entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9) - ((uint16)move * entity->amplitude.x >> 6);
        y2                = (uint16)move * entity->amplitude.y >> 6;
        y1                = entity->amplitude.y << 9;
    }
    entity->drawPos.y = entity->centerPos.y + y1 - y2;
    if (((move >> 16) & 1) != entity->hasTension) {
        if (entity->activeScreens)
            RSDK.PlaySfx(Platform->sfxClang, 0, 255);
        entity->hasTension = (move >> 16) & 1;
    }
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_Falling(void)
{
    RSDK_THIS(Platform);
    entity->drawPos.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->position.x = entity->centerPos.x;
        entity->position.y = entity->centerPos.y;
        entity->velocity.x = 0;
    }
    else {
        if (entity->amplitude.x) {
            RSDK.ResetEntityPtr(entity, 0, 0);
        }
        else {
            entity->drawPos.x  = entity->centerPos.x;
            entity->drawPos.y  = entity->centerPos.y;
            entity->velocity.y = 0;
            entity->visible    = 0;
            entity->state      = Platform_State_OffScreenReset;
        }
        entity->velocity.x = 0;
    }
}

void Platform_State_Collapsing(void)
{
    RSDK_THIS(Platform);

    if (entity->collapseDelay) {
        if (!--entity->collapseDelay) {
            entity->active        = ACTIVE_NORMAL;
            entity->state         = Platform_State_StartFalling;
            entity->collapseDelay = 30;
        }
    }

    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y;
    entity->drawPos.y += RSDK.Sin256(entity->stoodAngle) << 10;
    entity->velocity.x = 0;
}

void Platform_State_Circular(void)
{
    RSDK_THIS(Platform);
    int32 drawX        = -entity->drawPos.x;
    int32 drawY        = -entity->drawPos.y;
    entity->drawPos.x  = entity->amplitude.x * RSDK.Cos1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->centerPos.x;
    entity->drawPos.y  = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_12(void)
{
    RSDK_THIS(Platform);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    if (entity->stood)
        entity->state = Platform_Unknown9;
}

void Platform_State_13(void)
{
    RSDK_THIS(Platform);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    if (entity->stood)
        entity->state = Platform_Unknown12;
}

void Platform_State_14(void)
{
    RSDK_THIS(Platform);
    int32 oldAngle = entity->angle;
    int32 drawX    = -entity->drawPos.x;
    int32 drawY    = -entity->drawPos.y;
    int32 angle    = entity->groundVel + 0x100;
    entity->angle  = entity->groundVel + ((entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + 512) >> 14) + 0x100;
    if (!entity->hasTension) {
        if (entity->angle >= angle) {
            if (!entity->activeScreens || oldAngle >= angle) {
                entity->angle = entity->groundVel + 0x100;
            }
            else {
                RSDK.PlaySfx(Platform->sfxClacker, 0, 255);
                entity->angle = entity->groundVel + 0x100;
            }
        }
    }
    else if (entity->angle <= angle) {
        if (!entity->activeScreens || oldAngle <= angle) {
            entity->angle = entity->groundVel + 0x100;
        }
        else {
            RSDK.PlaySfx(Platform->sfxClacker, 0, 255);
            entity->angle = entity->groundVel + 0x100;
        }
    }

    entity->drawPos.x  = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
    entity->drawPos.y  = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_15(void)
{
    RSDK_THIS(Platform);
    int32 drawX        = -entity->drawPos.x;
    int32 drawY        = -entity->drawPos.y;
    entity->drawPos.x  = entity->centerPos.x;
    entity->drawPos.y  = entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_16(void)
{
    RSDK_THIS(Platform);

    if (entity->stood) {
        entity->drawPos.y += entity->velocity.y;
        if (entity->drawPos.y > entity->centerPos.y + entity->amplitude.y)
            entity->drawPos.y = entity->centerPos.y + entity->amplitude.y;
    }
    else {
        entity->drawPos.y -= entity->velocity.y;
        if (entity->drawPos.y < entity->centerPos.y)
            entity->drawPos.y = entity->centerPos.y;
    }
}

void Platform_Unknown1(void)
{
    RSDK_THIS(Platform);
    entity->drawPos.x += entity->velocity.x;
    int32 x = entity->position.x;
    int32 y = entity->position.y;

    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, entity->hitbox.left << 16, entity->hitbox.bottom << 16, 0))
        entity->state = Platform_Unknown3;
    entity->position.x = x;
    entity->position.y = y;
}

void Platform_Unknown2(void)
{
    RSDK_THIS(Platform);
    entity->drawPos.x += entity->velocity.x;
    int32 x = entity->position.x;
    int32 y = entity->position.y;

    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, entity->hitbox.right << 16, entity->hitbox.bottom << 16, 0))
        entity->state = Platform_Unknown3;
    entity->position.x = x;
    entity->position.y = y;
}

void Platform_Unknown3(void)
{
    RSDK_THIS(Platform);
    int32 x = entity->position.x;
    int32 y = entity->position.y;

    entity->drawPos.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    bool32 collided    = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, (entity->hitbox.left + 16) << 16, entity->hitbox.bottom << 16, 4);
    int32 ypos         = entity->position.y;

    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0, entity->hitbox.bottom << 16, 4);
    if (entity->position.y < ypos)
        ypos = entity->position.y;

    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, (entity->hitbox.right - 16) << 16, entity->hitbox.bottom << 16, 4);
    if (entity->position.y < ypos)
        ypos = entity->position.y;
    if (collided) {
        entity->velocity.y = 0;
        entity->state      = Platform_State_Pushable;
    }
    else {
        entity->position.y = ypos;
        entity->drawPos.y  = ypos;
    }
    entity->position.x = x;
    entity->position.y = y;
    entity->velocity.x = 0;
}

void Platform_Unknown4(void)
{
    RSDK_THIS(Platform);
    entity->drawPos.x += entity->velocity.x;
    entity->drawPos.y += entity->velocity.y;
    EntityPlatformNode *node = RSDK_GET_ENTITY(entity->speed, PlatformNode);

    if (entity->velocity.y <= 0) {
        if (entity->drawPos.y < node->position.y)
            entity->drawPos.y = node->position.y;
    }
    else {
        if (entity->drawPos.y > node->position.y)
            entity->drawPos.y = node->position.y;
    }

    if (entity->velocity.x <= 0) {
        if (entity->drawPos.x < node->position.x)
            entity->drawPos.x = node->position.x;
    }
    else {
        if (entity->drawPos.x > node->position.x)
            entity->drawPos.x = node->position.x;
    }
}

void Platform_Unknown5(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;

    entity->amplitude.y += entity->groundVel;
    entity->drawPos.x = (entity->amplitude.y >> 8) * RSDK.Cos256(entity->angle) + entity->centerPos.x;
    entity->drawPos.y = (entity->amplitude.y >> 8) * RSDK.Sin256(entity->angle) + entity->centerPos.y;

    if (entity->groundVel <= 0) {
        entity->centerPos.x = entity->drawPos.x - entity->tileOrigin.x;
        entity->centerPos.y = entity->drawPos.y - entity->tileOrigin.y;
        entity->groundVel   = 0;
        entity->amplitude.y = 0;
        entity->angle       = -entity->angle;

        if (entity->collapseDelay == -1) {
            entity->collapseDelay = 0;
            if (entity->type == PLATFORM_9)
                entity->state = Platform_State_WaitBobbing;
            else
                entity->state = Platform_State_Wait;
        }
        else {
            if (entity->hasTension) {
                entity->state = Platform_Unknown7;
            }
            else {
                entity->active = ACTIVE_BOUNDS;
                if (entity->type == PLATFORM_9)
                    entity->state = Platform_Unknown6;
                else
                    entity->state = Platform_State_Normal;
            }
        }
    }
    else {
        entity->groundVel -= entity->speed << 10;
    }

    entity->velocity.x = drawX + entity->drawPos.x;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void Platform_Unknown6(void)
{
    RSDK_THIS(Platform);
    entity->rotation += 4;
    entity->velocity.x = 0;
    entity->drawPos.y  = (RSDK.Sin1024(entity->rotation) << 9) + entity->centerPos.y;
    entity->velocity.y = 0;
}

void Platform_Unknown7(void)
{
    RSDK_THIS(Platform);
    if (entity->type == PLATFORM_9) {
        entity->rotation += 4;
        entity->drawPos.y = (RSDK.Sin1024(entity->rotation) << 9) + entity->centerPos.y;
    }
    if (entity->stood) {
        entity->collapseDelay = 120;
        entity->velocity.x    = 0;
        entity->velocity.y    = 0;
    }
    else {
        if (--entity->collapseDelay) {
            entity->tileOrigin.x  = entity->drawPos.x - entity->centerPos.x;
            entity->tileOrigin.y  = entity->drawPos.y - entity->centerPos.y;
            entity->centerPos.x   = entity->drawPos.x;
            entity->collapseDelay = -1;
            entity->centerPos.y   = entity->drawPos.y;
            entity->active        = ACTIVE_NORMAL;
            entity->state         = Platform_State_PlayerMove;
        }
        entity->velocity.x = 0;
        entity->velocity.y = 0;
    }
}

void Platform_Unknown8(void)
{
    RSDK_THIS(Platform);
    if (!entity->stood) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
    }
    else {
        int32 slot                        = RSDK_sceneInfo->entitySlot - 1;
        EntityPlatformControl *controller = RSDK_GET_ENTITY(slot, PlatformControl);
        if (controller->objectID == PlatformControl->objectID) {
            controller->setActive = true;
            entity->state         = Platform_Unknown4;
            entity->velocity.x    = 0;
            entity->velocity.y    = 0;
        }
        else {
            while (controller->objectID == Platform->objectID || controller->objectID == PlatformNode->objectID) {
                controller = RSDK_GET_ENTITY(slot--, PlatformControl);
                if (controller->objectID == PlatformControl->objectID) {
                    controller->setActive = true;
                    entity->state         = Platform_Unknown4;
                    entity->velocity.x    = 0;
                    entity->velocity.y    = 0;
                    break;
                }
            }
        }
    }
}

void Platform_Unknown9(void)
{
    RSDK_THIS(Platform);
    int32 drawX   = -entity->drawPos.x;
    int32 drawY   = -entity->drawPos.y;
    entity->angle = entity->groundVel + 0x100 + ((entity->amplitude.x * RSDK.Sin1024(entity->collapseDelay * entity->speed) + 0x200) >> 14);

    if (entity->collapseDelay == 256 && entity->hasTension) {
        entity->collapseDelay = 119;
        entity->state         = Platform_Unknown10;
    }
    ++entity->collapseDelay;
    entity->drawPos.x = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
    entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;

    if (entity->groundVel < 0)
        entity->drawPos.x += 0x200000;
    else
        entity->drawPos.x -= 0x200000;
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void Platform_Unknown10(void)
{
    RSDK_THIS(Platform);
    if (entity->stood) {
        entity->collapseDelay = 120;
        entity->velocity.x    = 0;
        entity->velocity.y    = 0;
    }
    else {
        if (!--entity->collapseDelay) {
            entity->collapseDelay = 257;
            entity->active        = ACTIVE_NORMAL;
            entity->state         = Platform_Unknown11;
        }
        entity->velocity.x = 0;
        entity->velocity.y = 0;
    }
}

void Platform_Unknown11(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;
    int32 amp   = entity->amplitude.x * RSDK.Sin1024(entity->collapseDelay * entity->speed);
    --entity->collapseDelay;
    entity->angle = entity->groundVel + 0x100 + ((amp + 0x200) >> 14);

    if (entity->collapseDelay == 88)
        entity->state = Platform_State_12;

    entity->drawPos.x = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
    entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
    if (entity->groundVel < 0)
        entity->drawPos.x += 0x200000;
    else
        entity->drawPos.x -= 0x200000;
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void Platform_Unknown12(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;
    int32 speed = entity->angle * (entity->speed << 7);
    entity->angle++;

    if (entity->hasTension) {
        if (speed < 0x10000) {
            if (speed <= 0)
                speed = 0;
        }
        else {
            speed                 = 0x10000;
            entity->collapseDelay = 120;
            entity->state         = Platform_Unknown13;
        }
    }

    int32 y1, y2;
    if (((speed >> 0x10) & 1) == entity->direction) {
        entity->drawPos.x = entity->centerPos.x + ((uint16)speed * entity->amplitude.x >> 6) - (entity->amplitude.x << 9);
        y1                = (uint16)speed * entity->amplitude.y >> 6;
        y2                = entity->amplitude.y << 9;
    }
    else {
        entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9) - ((uint16)speed * entity->amplitude.x >> 6);
        y2                = (uint16)speed * entity->amplitude.y >> 6;
        y1                = entity->amplitude.y << 9;
    }
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->drawPos.y  = entity->centerPos.y + y1 - y2;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void Platform_Unknown13(void)
{
    RSDK_THIS(Platform);
    if (entity->stoodPlayers) {
        entity->collapseDelay = 120;
        entity->velocity.x    = 0;
        entity->velocity.y    = 0;
    }
    else {
        if (!--entity->collapseDelay) {
            entity->active = ACTIVE_NORMAL;
            entity->state  = Platform_Unknown14;
        }
        entity->velocity.x = 0;
        entity->velocity.y = 0;
    }
}

void Platform_Unknown14(void)
{
    RSDK_THIS(Platform);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;
    int32 speed = entity->angle * (entity->speed << 7);
    entity->angle--;

    if (speed > 0) {
        if (speed >= 0x10000)
            speed = 0x10000;
    }
    else {
        speed         = 0;
        entity->state = Platform_State_13;
    }

    int32 y1, y2;
    if (((speed >> 0x10) & 1) == entity->direction) {
        entity->drawPos.x = entity->centerPos.x + ((uint16)speed * entity->amplitude.x >> 6) - (entity->amplitude.x << 9);
        y1                = (uint16)speed * entity->amplitude.y >> 6;
        y2                = entity->amplitude.y << 9;
    }
    else {
        entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9) - ((uint16)speed * entity->amplitude.x >> 6);
        y2                = (uint16)speed * entity->amplitude.y >> 6;
        y1                = entity->amplitude.y << 9;
    }
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->drawPos.y  = entity->centerPos.y + y1 - y2;
    entity->velocity.y = drawY + entity->drawPos.y;
}

// Collision States
void Platform_CollisionState_AllSolid(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&entity->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&entity->animator, 0);
    int32 stoodPlayers     = entity->stoodPlayers;
    entity->stoodPlayers   = 0;
    entity->pushPlayersL   = 0;
    entity->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, entity, platformHitbox);
        switch (Player_CheckCollisionBox(player, entity, solidHitbox)) {
            case C_TOP:
                entity->stood = true;
                if (!((1 << pid) & stoodPlayers) && !player->sidekick && entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        entity->collapseDelay = 1;
                    else
#endif
                        entity->collapseDelay = 30;
                }
                entity->stoodPlayers |= 1 << pid;

                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                if (entity->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    entity->pushPlayersL |= 1 << pid;
                }

                if (entity->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    entity->pushPlayersR |= 1 << pid;
                }

                if (entity->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (entity->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_AllHazard(void)
{
    RSDK_THIS(Platform);
    if (entity->collapseDelay)
        entity->collapseDelay--;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(1024, player, entity->type == PLATFORM_3, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, &entity);
        }
    }
}
void Platform_CollisionState_BottomHazard(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&entity->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&entity->animator, 0);
    entity->stoodPlayers   = 0;
    entity->pushPlayersL   = 0;
    entity->pushPlayersR   = 0;

    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, entity, platformHitbox);
        switch (Player_CheckCollisionBox(player, entity, solidHitbox)) {
            case C_TOP:
                entity->stood = true;
                entity->stoodPlayers |= (1 << pid);
                if (!player->sidekick && entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        entity->collapseDelay = 1;
                    else
#endif
                        entity->collapseDelay = 30;
                }
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                if (entity->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    entity->pushPlayersL |= 1 << pid;
                }

                if (entity->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    entity->pushPlayersR |= 1 << pid;
                }

                if (entity->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (entity->velocity.y >= 0)
                    player->collisionFlagV |= 2;
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, entity);
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_LRHazard(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox  = RSDK.GetHitbox(&entity->animator, 1);
    entity->stoodPlayers = 0;
    entity->pushPlayersL = 0;
    entity->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        switch (Player_CheckCollisionBox(player, entity, solidHitbox)) {
            case C_TOP:
                entity->stood = true;
                entity->stoodPlayers |= (1 << pid);
                if (!player->sidekick && entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        entity->collapseDelay = 1;
                    else
#endif
                        entity->collapseDelay = 30;
                }
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                if (entity->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (
#if RETRO_USE_PLUS
                    Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer) ||
#endif
                    Player_CheckHit(player, entity)) {
                    player->velocity.x += entity->velocity.x;
                }

                if (!player->blinkTimer || player->velocity.x >= entity->velocity.x) {
                    if (player->onGround && player->right) {
                        entity->pushPlayersL |= 1 << pid;
                    }

                    if (entity->velocity.x < 0 && player->velocity.x >= entity->velocity.x && player->left) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                }
                if (entity->velocity.x <= 0)
                    player->collisionFlagH |= 1;
                break;
            case C_RIGHT:
                if (
#if RETRO_USE_PLUS
                    Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer) ||
#endif
                    Player_CheckHit(player, entity)) {
                    player->velocity.x += entity->velocity.x;
                }

                if (!player->blinkTimer || player->velocity.x <= entity->velocity.x) {
                    if (player->onGround && player->left) {
                        entity->pushPlayersR |= 1 << pid;
                    }

                    if (entity->velocity.x > 0 && player->velocity.x <= entity->velocity.x && player->right) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                }
                if (entity->velocity.x >= 0)
                    player->collisionFlagH |= 2;
                break;
            case C_BOTTOM:
                if (entity->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: continue;
        }
    }
}
void Platform_CollisionState_Tiles(void)
{
    RSDK_THIS(Platform);
    entity->stoodPlayers = 0;
    entity->pushPlayersL = 0;
    entity->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);

        Hitbox hitbox;
        hitbox.left   = entity->hitbox.left;
        hitbox.top    = entity->hitbox.top;
        hitbox.right  = entity->hitbox.right;
        hitbox.bottom = entity->hitbox.bottom;
        hitbox.left -= 16;
        hitbox.right += 16;
        hitbox.bottom += 16;
        hitbox.top -= 16;

        if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
            player->collisionLayers |= Zone->moveID;
            player->moveOffset.x = entity->tileOrigin.x - entity->drawPos.x;
            player->moveOffset.y = entity->tileOrigin.y - entity->drawPos.y;
            if (player->playerAnimator.animationID == ANI_PUSH && player->onGround) {
                if (player->right)
                    entity->pushPlayersL |= 1 << pid;
                if (player->onGround && player->left)
                    entity->pushPlayersR |= 1 << pid;
            }

            bool32 climbFlag = false;
            if (player->state == Player_State_KnuxWallClimb || player->state == Player_State_KnuxLedgePullUp) {
                climbFlag = true;
                if (player->state == Player_State_KnuxLedgePullUp)
                    hitbox.top -= 16;
                if (player->position.x >= entity->position.x) {
                    hitbox.top += 16;
                    hitbox.left += 16;
                }
                else {
                    hitbox.top += 16;
                    hitbox.right -= 16;
                }
                hitbox.bottom -= 16;
            }
            else {
                switch (player->collisionMode) {
                    case CMODE_FLOOR:
                        hitbox.right -= 16;
                        hitbox.left += 16;
                        hitbox.bottom -= 16;
                        break;
                    case CMODE_LWALL:
                        hitbox.top += 16;
                        hitbox.right -= 16;
                        hitbox.bottom -= 16;
                        break;
                    case CMODE_ROOF:
                        hitbox.top += 16;
                        hitbox.left += 16;
                        hitbox.right -= 16;
                        break;
                    case CMODE_RWALL:
                        hitbox.top += 16;
                        hitbox.left += 16;
                        hitbox.bottom -= 16;
                        break;
                    default: break;
                }
            }

            if (Player_CheckCollisionTouch(player, entity, &hitbox) && (player->onGround || climbFlag)) {
                entity->stoodPlayers |= 1 << pid;
                if (!player->sidekick) {
                    entity->stood = true;
                    if (entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            entity->collapseDelay = 1;
                        else
#endif
                            entity->collapseDelay = 30;
                    }
                }
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }
    }
}
void Platform_CollisionState_Sticky(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&entity->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&entity->animator, 0);
    entity->pushPlayersL   = 0;
    entity->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, entity, platformHitbox);
        int32 side = Player_CheckCollisionBox(player, entity, solidHitbox);

        bool32 flag = false;
        if ((entity->collision != PLATFORM_C_9 && entity->collision != side + PLATFORM_C_9) || !side)
            flag = true;
        if (player->state != Player_State_None && !flag) {
            player->state                         = Player_State_None;
            player->nextGroundState               = StateMachine_None;
            player->nextAirState                  = StateMachine_None;
            player->velocity.x                    = 0;
            player->velocity.y                    = 0;
            player->groundVel                     = 0;
            player->playerAnimator.animationSpeed = 0;
            switch (side) {
                case C_TOP: player->angle = 0x00; break;
                case C_LEFT:
                    player->angle    = 0xC0;
                    player->onGround = false;
                    break;
                case C_RIGHT: player->angle = 0x40; break;
                case C_BOTTOM: player->angle = 0x80; break;
                default: break;
            }

            player->tileCollisions = false;
            if (!player->sidekick) {
                entity->stood = true;
                if (entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        entity->collapseDelay = 1;
                    else
#endif
                        entity->collapseDelay = 30;
                }
            }
            entity->stoodPlayers |= 1 << pid;
        }
        else {
            if ((1 << pid) & entity->stoodPlayers) {
                if (player->state == Player_State_None) {
                    if (Platform->stoodPos[pid].x) {
                        player->position.x = Platform->stoodPos[pid].x;
                        player->position.y = Platform->stoodPos[pid].y;
                    }
                    else {
                        Platform->stoodPos[pid].x = player->position.x;
                        Platform->stoodPos[pid].y = player->position.y;
                    }
                    player->position.x += entity->collisionOffset.x;
                    player->position.y += entity->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                    if (player->jumpPress) {
                        player->tileCollisions = true;
                        Player_StartJump(player);
                    }
                }
                else {
                    entity->stoodPlayers = entity->stoodPlayers & ~(1 << pid);
                }
            }
            else if (side == C_TOP) {
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
            }
        }
    }
}
void Platform_CollisionState_TopHazard(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox  = RSDK.GetHitbox(&entity->animator, 1);
    entity->stoodPlayers = 0;
    entity->pushPlayersL = 0;
    entity->pushPlayersR = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        switch (Player_CheckCollisionBox(player, entity, solidHitbox)) {
            case C_TOP:
                entity->stood = true;
                entity->stoodPlayers |= (1 << pid);
                if (!player->sidekick && entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        entity->collapseDelay = 1;
                    else
#endif
                        entity->collapseDelay = 30;
                }

                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;

#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(1024, player, 0, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, entity);
                if (entity->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;
            case C_LEFT:
                if (player->onGround && player->right)
                    entity->pushPlayersL |= 1 << pid;

                if (entity->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left)
                    entity->pushPlayersR |= 1 << pid;

                if (entity->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (entity->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_TopSolid(void)
{
    RSDK_THIS(Platform);
    int32 stoodPlayers     = entity->stoodPlayers;
    entity->stoodPlayers   = 0;
    Hitbox *platformHitbox = RSDK.GetHitbox(&entity->animator, 0);
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        int32 yVel = player->velocity.y;
        if (entity->collisionOffset.y < 0)
            player->velocity.y -= entity->collisionOffset.y;
        if (Player_CheckCollisionPlatform(player, entity, platformHitbox)) {
            entity->stood = true;
            if (!((1 << pid) & stoodPlayers) && !player->sidekick && entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                    entity->collapseDelay = 1;
                else
#endif
                    entity->collapseDelay = 30;
            }
            entity->stoodPlayers |= 1 << pid;

            if (Platform->stoodPos[pid].x) {
                player->position.x = Platform->stoodPos[pid].x;
                player->position.y = Platform->stoodPos[pid].y;
            }
            else {
                Platform->stoodPos[pid].x = player->position.x;
                Platform->stoodPos[pid].y = player->position.y;
            }
            player->position.x += entity->collisionOffset.x;
            player->position.y += entity->collisionOffset.y;
            player->position.y &= 0xFFFF0000;
        }
        else {
            player->velocity.y = yVel;
        }
    }
}
void Platform_CollisionState_TurnTable(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&entity->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&entity->animator, 0);
    int32 stoodPlayers     = entity->stoodPlayers;
    entity->stoodPlayers   = 0;
    entity->pushPlayersL   = 0;
    entity->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, entity, platformHitbox);
        switch (Player_CheckCollisionBox(player, entity, solidHitbox)) {
            case C_TOP: entity->stood = true;
#if RETRO_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->state != Player_State_MightyHammerDrop) {
#endif
                    if (!((1 << pid) & stoodPlayers)) {
                        player->state           = Player_State_None;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        player->groundVel       = 0;
                        if (entity->objectID == Platform->objectID)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TURNTABLE, &player->playerAnimator, false, 0);
                        player->playerAnimator.animationSpeed = 64;
                        player->direction                     = FLIP_NONE;
                        if (!player->sidekick && entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                            if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                entity->collapseDelay = 1;
                            else
#endif
                                entity->collapseDelay = 30;
                        }
                    }
                    entity->stoodPlayers |= 1 << pid;
                    if (Platform->stoodPos[pid].x) {
                        player->position.x = Platform->stoodPos[pid].x;
                        player->position.y = Platform->stoodPos[pid].y;
                    }
                    else {
                        Platform->stoodPos[pid].x = player->position.x;
                        Platform->stoodPos[pid].y = player->position.y;
                    }

                    if ((player->position.x ^ entity->position.x) & 0xFFFF0000) {
                        if (player->position.x >= entity->position.x)
                            player->position.x -= 0x10000;
                        else
                            player->position.x += 0x10000;
                    }
                    player->position.x += entity->collisionOffset.x;
                    player->position.y += entity->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                    if (player->jumpPress) {
                        Player_StartJump(player);
                    }
                    else if (entity->velocity.y <= 0) {
                        player->collisionFlagV |= 1;
                    }
#if RETRO_USE_PLUS
                }
#endif
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    entity->pushPlayersL |= 1 << pid;
                }

                if (entity->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    entity->pushPlayersR |= 1 << pid;
                }

                if (entity->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (entity->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_Twister(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&entity->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&entity->animator, 0);
    int32 stoodPlayers     = entity->stoodPlayers;
    entity->stoodPlayers   = 0;
    entity->pushPlayersL   = 0;
    entity->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, entity, platformHitbox);
        switch (Player_CheckCollisionBox(player, entity, solidHitbox)) {
            case C_NONE:
                if ((1 << pid) & stoodPlayers) {
                    if (player->state == Player_State_None) {
                        player->state = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_AIRWALK, &player->playerAnimator, false, 0);
                    }
                }
                break;
            case C_TOP:
#if RETRO_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->state != Player_State_MightyHammerDrop) {
#endif
                    if (!((1 << pid) & stoodPlayers)) {
                        player->state           = Player_State_None;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        player->groundVel       = 0;
                        if (entity->objectID == Platform->objectID)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTER, &player->playerAnimator, false, 0);
                        player->playerAnimator.animationSpeed = 64;
                        player->direction                     = FLIP_X;
                        if (!player->sidekick) {
                            entity->stood = true;
                            if (entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                                if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                                    entity->collapseDelay = 1;
                                else
#endif
                                    entity->collapseDelay = 30;
                            }
                        }
                    }
                    entity->stoodPlayers |= 1 << pid;
                    if (Platform->stoodPos[pid].x) {
                        player->position.x = Platform->stoodPos[pid].x;
                        player->position.y = Platform->stoodPos[pid].y;
                    }
                    else {
                        Platform->stoodPos[pid].x = player->position.x;
                        Platform->stoodPos[pid].y = player->position.y;
                    }

                    if ((player->position.x ^ entity->position.x) & 0xFFFF0000) {
                        if (player->position.x >= entity->position.x)
                            player->position.x -= 0x10000;
                        else
                            player->position.x += 0x10000;
                    }
                    player->position.x += entity->collisionOffset.x;
                    player->position.y += entity->collisionOffset.y;
                    player->position.y &= 0xFFFF0000;
                    if (player->jumpPress) {
                        Player_StartJump(player);
                    }
                    else if (entity->velocity.y <= 0) {
                        player->collisionFlagV |= 1;
                    }
#if RETRO_USE_PLUS
                }
#endif
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    entity->pushPlayersL |= 1 << pid;
                }

                if (entity->velocity.x <= 0) {
                    if (player->left) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x -= player->acceleration;
                    }
                    player->collisionFlagH |= 1;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left) {
                    entity->pushPlayersR |= 1 << pid;
                }

                if (entity->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                    player->collisionFlagH |= 2;
                }
                break;
            case C_BOTTOM:
                if (entity->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                break;
            default: break;
        }
    }
}
void Platform_CollisionState_None(void) {}
void Platform_CollisionState_AllSolid_NoCrush(void)
{
    RSDK_THIS(Platform);
    Hitbox *solidHitbox    = RSDK.GetHitbox(&entity->animator, 1);
    Hitbox *platformHitbox = RSDK.GetHitbox(&entity->animator, 0);
    int32 stoodPlayers     = entity->stoodPlayers;
    entity->stoodPlayers   = 0;
    entity->pushPlayersL   = 0;
    entity->pushPlayersR   = 0;
    foreach_active(Player, player)
    {
        uint16 pid = RSDK.GetEntityID(player);
        Player_CheckCollisionPlatform(player, entity, platformHitbox);

        switch (Player_CheckCollisionBox(player, entity, solidHitbox)) {
            default: break;
            case C_TOP:
                entity->stood = true;
                if (!((1 << pid) & stoodPlayers) && !player->sidekick && entity->state == Platform_State_Collapsing && !entity->collapseDelay) {
#if RETRO_USE_PLUS
                    if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                        entity->collapseDelay = 1;
                    else
#endif
                        entity->collapseDelay = 30;
                }
                entity->stoodPlayers |= 1 << pid;
                if (Platform->stoodPos[pid].x) {
                    player->position.x = Platform->stoodPos[pid].x;
                    player->position.y = Platform->stoodPos[pid].y;
                }
                else {
                    Platform->stoodPos[pid].x = player->position.x;
                    Platform->stoodPos[pid].y = player->position.y;
                }
                player->position.x += entity->collisionOffset.x;
                player->position.y += entity->collisionOffset.y;
                player->position.y &= 0xFFFF0000;
                break;
            case C_LEFT:
                if (player->onGround && player->right) {
                    entity->pushPlayersL |= 1 << pid;
                }

                if (entity->velocity.x <= 0 && player->left) {
                    player->groundVel  = entity->velocity.x;
                    player->velocity.x = player->groundVel;
                    player->velocity.x -= player->acceleration;
                }
                break;
            case C_RIGHT:
                if (player->onGround && player->left)
                    entity->pushPlayersR |= 1 << pid;

                if (entity->velocity.x >= 0) {
                    if (player->right) {
                        player->groundVel  = entity->velocity.x;
                        player->velocity.x = player->groundVel;
                        player->velocity.x += player->acceleration;
                    }
                }
                break;
            case C_BOTTOM: break;
        }
    }
}

void Platform_EditorDraw(void)
{
    RSDK_THIS(Platform);
    entity->drawPos = entity->position;
    entity->centerPos = entity->position;
    Platform_Draw();
}

void Platform_EditorLoad(void)
{
    Platform_StageLoad();

    RSDK_ACTIVE_VAR(Platform, type);
    RSDK_ENUM_VAR("Type 0", PLATFORM_0);
    RSDK_ENUM_VAR("Type 1", PLATFORM_1);
    RSDK_ENUM_VAR("Type 2", PLATFORM_2);
    RSDK_ENUM_VAR("Type 3", PLATFORM_3);
    RSDK_ENUM_VAR("Type 4", PLATFORM_4);
    RSDK_ENUM_VAR("Type 5", PLATFORM_5);
    RSDK_ENUM_VAR("Type 6", PLATFORM_6);
    RSDK_ENUM_VAR("Type 7", PLATFORM_7);
    RSDK_ENUM_VAR("Type 8", PLATFORM_8);
    RSDK_ENUM_VAR("Type 9", PLATFORM_9);
    RSDK_ENUM_VAR("Type 10", PLATFORM_10);
    RSDK_ENUM_VAR("Type 11", PLATFORM_11);
    RSDK_ENUM_VAR("Type 12", PLATFORM_12);
    RSDK_ENUM_VAR("Type 13", PLATFORM_13);
    RSDK_ENUM_VAR("Type 14", PLATFORM_14);
    RSDK_ENUM_VAR("Type 15", PLATFORM_15);
    RSDK_ENUM_VAR("Type 16", PLATFORM_16);

    RSDK_ACTIVE_VAR(Platform, collision);
    RSDK_ENUM_VAR("Type 0", PLATFORM_C_0);
    RSDK_ENUM_VAR("Type 1", PLATFORM_C_1);
    RSDK_ENUM_VAR("Type 2", PLATFORM_C_2);
    RSDK_ENUM_VAR("Type 3", PLATFORM_C_3);
    RSDK_ENUM_VAR("Type 4", PLATFORM_C_4);
    RSDK_ENUM_VAR("Type 5", PLATFORM_C_5);
    RSDK_ENUM_VAR("Type 6", PLATFORM_C_6);
    RSDK_ENUM_VAR("Type 7", PLATFORM_C_7);
    RSDK_ENUM_VAR("Type 8", PLATFORM_C_8);
    RSDK_ENUM_VAR("Type 9", PLATFORM_C_9);
    RSDK_ENUM_VAR("Type 10", PLATFORM_C_10);
    RSDK_ENUM_VAR("Type 11", PLATFORM_C_11);
    RSDK_ENUM_VAR("Type 12", PLATFORM_C_12);
    RSDK_ENUM_VAR("Type 13", PLATFORM_C_13);
    RSDK_ENUM_VAR("Type 14", PLATFORM_C_14);
    RSDK_ENUM_VAR("Type 15", PLATFORM_C_15);
}

void Platform_Serialize(void)
{
    RSDK_EDITABLE_VAR(Platform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Platform, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(Platform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Platform, VAR_BOOL, hasTension);
    RSDK_EDITABLE_VAR(Platform, VAR_INT8, frameID);
    RSDK_EDITABLE_VAR(Platform, VAR_UINT8, collision);
    RSDK_EDITABLE_VAR(Platform, VAR_VECTOR2, tileOrigin);
    RSDK_EDITABLE_VAR(Platform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(Platform, VAR_INT32, angle);
}
