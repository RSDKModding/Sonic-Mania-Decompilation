#include "../SonicMania.h"

ObjectPlatform *Platform;

void Platform_Update()
{
    RSDK_THIS(Platform);
    if (Zone->field_4724) {
        entity->pushPlayersL = 0;
        entity->pushPlayersR = 0;
    }
    entity->position.x = entity->drawPos.x >> 0x10 << 0x10;
    entity->position.y = entity->drawPos.y >> 0x10 << 0x10;
    if (entity->hasTension) {
        if (entity->stood && entity->stoodAngle < 64) {
            entity->stoodAngle += 4;
        }
        else if (entity->stoodAngle > 0) {
            entity->stoodAngle -= 4;
        }
    }

    entity->collisionOffset.x = -entity->position.x;
    entity->collisionOffset.y = -entity->position.y;

    if (entity->state)
        entity->state();

    if (entity->objectID) {
        entity->stood = false;
        entity->collisionOffset.x += entity->drawPos.x & 0xFFFF0000;
        entity->collisionOffset.y += entity->drawPos.y & 0xFFFF0000;
        if (entity->state != Platform_State_Falling && entity->state != Platform_State_OffScreenReset) {
            if (entity->stateCollide)
                entity->stateCollide();
        }

        entity->position.x = entity->centerPos.x;
        entity->position.y = entity->centerPos.y;
        for (int s = RSDK_sceneInfo->entitySlot + 1, i = 0; i < entity->childCount; ++i) {
            Entity *child = RSDK.GetEntityByID(s);
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
                entity->position.x += entity->collisionOffset.x;
                entity->position.y += entity->collisionOffset.y;
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
                    // EntityIce *ice  = (EntityIce *)child;
                    // ice[2].alpha    = entity->collisionOffset.x;
                    // ice[2].rotation = entity->collisionOffset.y;
                }
                else if (TurboTurtle && child->objectID == TurboTurtle->objectID) {
                    // EntityTurboTurtle *turboTurtle = (EntityTurboTurtle *)child;
                    // turboTurtle[1].position.y += entity->collisionOffset.x;
                    // turboTurtle[1].scale.x += entity->collisionOffset.y;
                }
            }
        }
        if (entity->animData.frameDelay)
            RSDK.ProcessAnimation(&entity->animData);
    }
}

void Platform_LateUpdate() {}

void Platform_StaticUpdate()
{
    Platform->stoodPos[0].x = 0;
    Platform->stoodPos[0].y = 0;
    Platform->stoodPos[1].x = 0;
    Platform->stoodPos[1].y = 0;
    Platform->stoodPos[2].x = 0;
    Platform->stoodPos[2].y = 0;
    Platform->stoodPos[3].x = 0;
    Platform->stoodPos[3].y = 0;
}

void Platform_Draw()
{
    Vector2 drawPos;
    RSDK_THIS(Platform);
    if (entity->frameID >= 0) {
        if ((entity->state != Platform_State_Circular
            || !entity->hasTension) && (entity->state != Platform_State_Swing && entity->state != Platform_State_14 && entity->type != 12)) {
            if (Platform->spriteIndex == -1)
                RSDK.DrawRect(entity->drawPos.x - 0x200000, entity->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, 255, 0, 0);
            else
                RSDK.DrawSprite(&entity->animData, &entity->drawPos, 0);
        }
        else {
            int ang = 0;
            if (entity->state == Platform_State_Circular && entity->hasTension)
                ang = entity->speed * Zone->timer + 4 * entity->angle;
            else
                ang = entity->angle;
            int fxStore = entity->drawFX;
            entity->drawFX |= FX_FLIP | FX_ROTATE;
            int cnt                  = (entity->amplitude.y >> 10) - 1;
            int angle                = 0x400;
            entity->direction        = FLIP_NONE;
            entity->animData.frameID = entity->frameID + 1;

            int rot = ang >> 1;
            for (int i = 0; i < cnt; ++i) {
                drawPos.x = angle * RSDK.Cos1024(ang) + entity->centerPos.x;
                drawPos.y = angle * RSDK.Sin1024(ang) + entity->centerPos.y;
                RSDK.DrawSprite(&entity->animData, &drawPos, 0);
                angle += 0x400;
                entity->direction ^= FLIP_X;
                entity->rotation = rot;
            }

            entity->drawFX           = fxStore;
            entity->animData.frameID = entity->frameID + 2;
            RSDK.DrawSprite(&entity->animData, &entity->centerPos, 0);
            entity->animData.frameID = entity->frameID;

            if (Platform->spriteIndex == -1)
                RSDK.DrawRect(entity->drawPos.x - 0x200000, entity->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, 255, 0, 0);
            else
                RSDK.DrawSprite(&entity->animData, &entity->drawPos, 0);
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
    RSDK.SetSpriteAnimation(Platform->spriteIndex, 0, &entity->animData, true, 0);
    entity->animData.frameID = entity->frameID;
    if (!RSDK_sceneInfo->inEditor && RSDK.GetFrameID(&entity->animData) == 108)
        entity->drawOrder = Zone->drawOrderLow;

    switch (entity->type) {
        case 1:
            entity->updateRange.x = 0x800000;
            entity->state         = Platform_State_Collapsing;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            break;
        case 2:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 10;
            entity->state         = Platform_State_Moving;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            entity->rotation      = entity->angle;
            entity->angle         = 0;
            break;
        case 3:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 10;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            entity->state         = Platform_State_Circular;
            break;
        case 5:
        case 11:
            entity->active = ACTIVE_BOUNDS;
            if (entity->type == 5)
                entity->state = Platform_State_Nothing;
            else
                entity->state = Platform_Unknown8;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case 6:
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            if (!RSDK_sceneInfo->inEditor) {
                entity->speed <<= 11;
                entity->position.x += 0x8000;
            }
            entity->state = Platform_State_StartPush;
            break;
        case 8:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);
            if (entity->speed < 0)
                entity->direction = FLIP_X;
            entity->state = Platform_State_Wait;
            break;
        case 9:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);
            if (entity->speed < 0)
                entity->direction = 1;
            entity->state = Platform_State_WaitBobbing;
            break;
        case 10:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);

            if (entity->speed < 0) {
                entity->direction = 1;
                entity->speed     = -entity->speed;
            }
            entity->state = Platform_State_PlayerActivated;
            break;
        case 12: entity->collapseDelay = 88;
        case 4:
            entity->updateRange.x = (abs(entity->amplitude.y) + 512) << 14;
            entity->updateRange.y = (abs(entity->amplitude.y) + 512) << 14;
            RSDK.SetSpriteAnimation(Platform->spriteIndex, 1, &entity->animData, true, 0);
            entity->amplitude.y *= 16;
            entity->groundVel = 4 * entity->angle;
            entity->angle     = entity->groundVel + 256 + (entity->amplitude.x * RSDK.Sin1024(entity->speed * entity->collapseDelay) >> 14);
            entity->drawPos.x = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
            entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
            if (entity->type == 4) {
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
        case 13:
            if (entity->direction) {
                entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9);
                entity->drawPos.y = entity->centerPos.y + (entity->amplitude.y << 9);
            }
            else {
                entity->drawPos.x = entity->centerPos.x - (entity->amplitude.x << 9);
                entity->drawPos.y = entity->centerPos.y - (entity->amplitude.y << 9);
            }
        case 7:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x4000) << 9;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x4000) << 9;
            if (entity->speed < 0)
                entity->direction = FLIP_X;

            if (entity->type == 7)
                entity->state = Platform_State_MovingSpike;
            else
                entity->state = Platform_State_13;
            break;
        case 14:
            entity->updateRange.x = (abs(entity->amplitude.y) + 512) << 14;
            entity->updateRange.y = (abs(entity->amplitude.y) + 512) << 14;
            RSDK.SetSpriteAnimation(Platform->spriteIndex, 1, &entity->animData, true, 0);
            entity->amplitude.y *= 16;
            entity->groundVel = 4 * entity->angle;
            entity->angle     = entity->groundVel + 256 + (entity->amplitude.x * RSDK.Sin1024(entity->speed * entity->collapseDelay) >> 14);
            entity->drawPos.x = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
            entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
            entity->state     = Platform_State_14;
            break;
        case 15:
            entity->state         = Platform_State_15;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
        case 16:
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            if (!entity->speed)
                entity->speed = 1;
            entity->amplitude.y = entity->amplitude.y << 10;
            entity->velocity.y  = entity->speed << 16;
            entity->state       = Platform_State_16;
            break;
        default:
            entity->state         = Platform_State_Normal;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            break;
    }

    if (entity->frameID >= 0) {
        int f    = entity->frameID;
        int anim = 0;
        while (f >= entity->animData.frameCount) {
            if (!entity->animData.frameCount)
                break;
            f -= entity->animData.frameCount;
            RSDK.SetSpriteAnimation(Platform->spriteIndex, ++anim, &entity->animData, true, 0);
        }
        entity->frameID          = f;
        entity->animData.frameID = f;
    }
    else {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animData, true, 0);
    }

    if (!RSDK_sceneInfo->inEditor) {
        if (entity->collision != 4) {
            Hitbox *hitbox = RSDK.GetHitbox(&entity->animData, entity->collision != 0);
            if (Platform->spriteIndex != -1 && hitbox) {
                entity->hitbox.left   = hitbox->left;
                entity->hitbox.top    = hitbox->top;
                entity->hitbox.right  = hitbox->right;
                entity->hitbox.bottom = hitbox->bottom;
            }
            else {
                entity->hitbox.left   = -0x20;
                entity->hitbox.top    = -0xF0;
                entity->hitbox.right  = 0x7FF;
                entity->hitbox.bottom = 0xE0;
            }
        }
        // switch (entity->collision) {
        //    case 0u:
        //        entity->stateCollide  = Platform_CollisionState_TopSolid;
        //        entity->hitbox.bottom = entity->hitbox.top + 8;
        //        break;
        //    case 2u: entity->stateCollide = Platform_CollisionState_None; break;
        //    case 3u: entity->stateCollide = Platform_CollisionState_AllHazard; break;
        //    case 4u: entity->stateCollide = Platform_CollisionState_4; break;
        //    case 5u: entity->stateCollide = Platform_CollisionState_LRHazard; break;
        //    case 6u: entity->stateCollide = Platform_CollisionState_BottomHazard; break;
        //    case 7u: entity->stateCollide = Platform_CollisionState_TopHazard; break;
        //    case 8u: entity->stateCollide = Platform_CollisionState_Twister; break;
        //    case 9u:
        //    case 0xAu:
        //    case 0xBu:
        //    case 0xCu:
        //    case 0xDu: entity->stateCollide = Platform_CollisionState_Sticky; break;
        //    case 0xEu: entity->stateCollide = Platform_CollisionState_TurnTable; break;
        //    case 0xFu: entity->stateCollide = Platform_CollisionState_15; break;
        //    default: entity->stateCollide = Platform_CollisionState_AllSolid; break;
        //}
        for (int i = 0; i < entity->childCount; ++i) {
            EntityPlatform *ent = (EntityPlatform *)RSDK.GetEntityByID((i + RSDK.GetEntityID(entity) + 1));
            ent->tileCollisions = false;
            if (HangPoint && ent->objectID == HangPoint->objectID) {
                EntityHangPoint *hang = (EntityHangPoint *)ent;
                // v53 = hang[1].groundVel;
                // int posY = hang->position.y;
                // if (ent->position.y - (v53 << 16) - posY >= 0)
                //    v54 = ent->position.y - (v53 << 16) - posY;
                // else
                //    v54 = posY + (v53 << 16) - ent->position.y;
                if (entity->updateRange.y < 0x800000 + abs(entity->position.y - ent->position.y))
                    entity->updateRange.y = 0x800000 + abs(entity->position.y - ent->position.y);
            }
            else {
                if (entity->updateRange.y < 0x800000 + abs(entity->position.y - ent->position.y))
                    entity->updateRange.y = 0x800000 + abs(entity->position.y - ent->position.y);
            }
        }
    }
}

void Platform_StageLoad()
{
    Platform->useClack = false;
    if (RSDK.CheckStageFolder("GHZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("GHZCutscene")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("GHZCutscene/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("OOZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ1")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("SPZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ2")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("SPZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("FBZ/Platform.bin", SCOPE_STAGE);
        Platform->sfx_Clack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("PSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("PSZ2")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("PSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("SSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ2")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("SSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE);
        Platform->sfx_Clack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("MSZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("OOZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("OOZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("LRZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("LRZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MMZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("MMZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("TMZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("AIZ")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("AIZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Blueprint")) {
        Platform->spriteIndex = RSDK.LoadSpriteAnimation("Blueprint/Platform.bin", SCOPE_STAGE);
    }

    Platform->sfx_Clacker = RSDK.GetSFX("Stage/Clacker.wav");
    Platform->sfx_Clang   = RSDK.GetSFX("Stage/Clang.wav");
    Platform->sfx_Push    = RSDK.GetSFX("Global/Push.wav");
    if (Platform->playingPushSFX) {
        RSDK.StopSFX(Platform->sfx_Push);
        Platform->playingPushSFX = 0;
    }
}

void Platform_State_Nothing()
{
    RSDK_THIS(Platform);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
}

void Platform_State_Normal()
{
    RSDK_THIS(Platform);
    entity->drawPos.x = entity->centerPos.x;
    entity->drawPos.y = entity->centerPos.y;
    entity->drawPos.y += RSDK.Sin256(entity->stoodAngle) << 10;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
}

void Platform_State_Moving()
{
    RSDK_THIS(Platform);
    int drawX         = -entity->drawPos.x;
    int drawY         = -entity->drawPos.y;
    entity->drawPos.x = entity->amplitude.x * RSDK.Sin1024(entity->speed * (entity->rotation + Zone->timer)) + entity->centerPos.x;
    entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * (entity->rotation + Zone->timer)) + entity->centerPos.y;
    entity->drawPos.y += RSDK.Sin256(entity->stoodAngle) << 10;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_Swing()
{
    RSDK_THIS(Platform);
    int drawX          = -entity->drawPos.x;
    int drawY          = -entity->drawPos.y;
    entity->angle      = entity->groundVel + 0x100 + ((entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + 0x200) >> 14);
    entity->drawPos.x  = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
    entity->drawPos.y  = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_Wait()
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

void Platform_State_WaitBobbing()
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
        entity->centerPos.y  = RSDK.Sin1024(entity->rotation) << 9;
        entity->active       = 2;
        entity->state        = Platform_State_PlayerMove;
    }
    entity->velocity.y = 0;
    entity->velocity.x = 0;
}

void Platform_State_StartPush()
{
    RSDK_THIS(Platform);
    entity->active        = ACTIVE_NORMAL;
    entity->updateRange.x = 0x2000000;
    entity->updateRange.y = 0x2000000;
    entity->state         = Platform_State_Pushable;
    entity->velocity.x    = 0;
    entity->velocity.y    = 0;
}

void Platform_State_StartFalling()
{
    RSDK_THIS(Platform);
    if (--entity->collapseDelay <= 0) {
        entity->collapseDelay = 0;
        entity->state         = Platform_State_Falling;
        EntityPlayer *player  = NULL;
        while (RSDK.GetActiveEntities(Player->objectID, (Entity**)&player)) {
            if ((1 << RSDK.GetEntityID(player)) & entity->stoodPlayers)
                player->velocity.y = entity->velocity.y - 0x10000;
        }
    }

    entity->drawPos.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->velocity.x = 0;
}

void Platform_State_PlayerActivated()
{
    RSDK_THIS(Platform);
    int drawX            = -entity->drawPos.x;
    int drawY            = -entity->drawPos.y;
    EntityPlayer *player = NULL;

    bool32 flag = false;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        if (!player->sidekick) {
            int angle = (byte)((byte)(entity->angle) + -0x80);
            int xOff  = (player->position.x - entity->centerPos.x) >> 8;
            int yOff  = (player->position.y - entity->centerPos.y) >> 8;
            RSDK.Sin256(angle);
            RSDK.Cos256(angle);

            int cy = (yOff * RSDK.Cos256(angle)) - xOff * RSDK.Sin256(angle);

            int center = cy + entity->centerPos.y;
            if (abs(player->position.x - entity->centerPos.x) <= 0x4000000) {
                if (abs(player->position.y - entity->centerPos.y) <= 0x4000000 && cy < entity->centerPos.y) {
                    if (entity->centerPos.y - cy < 0x1000000)
                        flag = true;
                }
            }
        }
    }

    if (!flag) {
        if (entity->amplitude.y > 0) {
            entity->amplitude.y = entity->amplitude.y - (entity->speed << 16);
            if (entity->amplitude.y <= 0)
                entity->amplitude.y = 0;
        }
    }
    else {
        if (entity->amplitude.y < entity->amplitude.x) {
            entity->amplitude.y = entity->amplitude.y + (entity->speed << 16);
            if (entity->amplitude.y >= entity->amplitude.x)
                entity->amplitude.y = entity->amplitude.x;
        }
    }

    if (entity->activeScreens && Platform->useClack)
        RSDK.PlaySFX(Platform->sfx_Clack, 0, 255);

    if (entity->direction) {
        entity->drawPos.x = (-entity->amplitude.y >> 8) * RSDK.Cos256(entity->angle) + entity->centerPos.x;
        entity->drawPos.y = drawY + (-entity->amplitude.y >> 8) * RSDK.Sin256(entity->angle) + entity->centerPos.y;
    }
    else {
        entity->drawPos.x = (entity->amplitude.y >> 8) * RSDK.Cos256(entity->angle) + entity->centerPos.x;
        entity->drawPos.y = drawY + (entity->amplitude.y >> 8) * RSDK.Sin256(entity->angle) + entity->centerPos.y;
    }

    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_PlayerMove()
{
    RSDK_THIS(Platform);
    int drawX = -entity->drawPos.x;
    int drawY = -entity->drawPos.y;
    entity->amplitude.y += entity->groundVel;

    entity->drawPos.x = (entity->amplitude.y >> 8) * RSDK.Cos256(entity->angle) + entity->centerPos.x;
    entity->drawPos.y = (entity->amplitude.y >> 8) * RSDK.Sin256(entity->angle) + entity->centerPos.y;

    int speed16 = entity->speed << 16;
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

void Platform_State_Pushable()
{
    RSDK_THIS(Platform);
    entity->velocity.x   = 0;
    entity->velocity.y   = 0;
    EntityPlayer *player = NULL;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        byte id = 1 << RSDK.GetEntityID(player);
        if (id & entity->pushPlayersL)
            entity->velocity.x += entity->speed;
        if (id & entity->pushPlayersR)
            entity->velocity.x -= entity->speed;
    }

    if (entity->velocity.x > 0 || entity->velocity.x < 0) {
        if (!Platform->playingPushSFX) {
            RSDK.PlaySFX(Platform->sfx_Push, true, 255);
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
            RSDK.StopSFX(Platform->sfx_Push);
            Platform->playingPushSFX = false;
        }
        if (entity->collapseDelay < 4) {
            entity->collapseDelay++;
        }
    }

    entity->drawPos.x += entity->velocity.x;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    player             = NULL;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int bitID            = 1 << RSDK.GetEntityID(&player);
        if (bitID & entity->pushPlayersL)
            player->position.x = entity->drawPos.x + ((entity->hitbox.left - playerHitbox->right) << 16);
        if (bitID & entity->pushPlayersR)
            player->position.x = entity->drawPos.x + ((entity->hitbox.right - playerHitbox->left - 1) << 16);
    }

    bool32 collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, (entity->hitbox.left + 16) << 16, entity->hitbox.bottom << 16, 4);
    int y           = entity->position.y;

    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0, entity->hitbox.bottom << 16, 4);
    if (entity->position.y < y)
        y = entity->position.y;

    collided |= RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, (entity->hitbox.right - 16) << 16, entity->hitbox.bottom << 16, 4);
    if (entity->position.y < y)
        y = entity->position.y;

    entity->position.y = y;
    entity->drawPos.y  = y;
    if (!collided) {
        int vel            = 2 * entity->velocity.x;
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
            RSDK.StopSFX(Platform->sfx_Push);
            Platform->playingPushSFX = false;
        }
        entity->state = Platform_State_OffScreenReset;
    }
}

void Platform_State_OffScreenReset()
{
    RSDK_THIS(Platform);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, &entity->updateRange))
        Platform_Create(NULL);
    entity->velocity.y = 0;
    entity->velocity.x = 0;
}

void Platform_State_MovingSpike()
{
    RSDK_THIS(Platform);
    int drawX = -entity->drawPos.x;
    int drawY = -entity->drawPos.y;
    int move  = Zone->timer * (entity->speed << 7);

    int y1, y2;
    if ((move >> 16) & 1 == entity->direction) {
        entity->drawPos.x = entity->centerPos.x + ((ushort)move * entity->amplitude.x >> 6) - (entity->amplitude.x << 9);
        y1                = (ushort)move * entity->amplitude.y >> 6;
        y2                = entity->amplitude.y << 9;
    }
    else {
        entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9) - ((ushort)move * entity->amplitude.x >> 6);
        y2                = (ushort)move * entity->amplitude.y >> 6;
        y1                = entity->amplitude.y << 9;
    }
    entity->drawPos.y = entity->centerPos.y + y1 - y2;
    if ((move >> 16) & 1 != entity->hasTension) {
        if (entity->activeScreens)
            RSDK.PlaySFX(Platform->sfx_Clang, 0, 255);
        entity->hasTension = (move >> 16) & 1;
    }
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_Falling()
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

void Platform_State_Collapsing()
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

void Platform_State_Circular()
{
    RSDK_THIS(Platform);
    int drawX          = -entity->drawPos.x;
    int drawY          = -entity->drawPos.y;
    entity->drawPos.x  = entity->amplitude.x * RSDK.Cos1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->centerPos.x;
    entity->drawPos.y  = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_12()
{
    RSDK_THIS(Platform);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    if (entity->stood)
        entity->state = Platform_Unknown9;
}

void Platform_State_13()
{
    RSDK_THIS(Platform);
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    if (entity->stood)
        entity->state = Platform_Unknown12;
}

void Platform_State_14()
{
    RSDK_THIS(Platform);
    int oldAngle  = entity->angle;
    int drawX     = -entity->drawPos.x;
    int drawY     = -entity->drawPos.y;
    int angle     = entity->groundVel + 0x100;
    entity->angle = entity->groundVel + ((entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + 512) >> 14) + 0x100;
    if (!entity->hasTension) {
        if (entity->angle >= angle) {
            if (!entity->activeScreens || oldAngle >= angle) {
                entity->angle = entity->groundVel + 0x100;
            }
            else {
                RSDK.PlaySFX(Platform->sfx_Clacker, 0, 255);
                entity->angle = entity->groundVel + 0x100;
            }
        }
    }
    else if (entity->angle <= angle) {
        if (!entity->activeScreens || oldAngle <= angle) {
            entity->angle = entity->groundVel + 0x100;
        }
        else {
            RSDK.PlaySFX(Platform->sfx_Clacker, 0, 255);
            entity->angle = entity->groundVel + 0x100;
        }
    }

    entity->drawPos.x  = entity->amplitude.y * RSDK.Cos1024(entity->angle) + entity->centerPos.x;
    entity->drawPos.y  = entity->amplitude.y * RSDK.Sin1024(entity->angle) + entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_15()
{
    RSDK_THIS(Platform);
    int drawX          = -entity->drawPos.x;
    int drawY          = -entity->drawPos.y;
    entity->drawPos.x  = entity->centerPos.x;
    entity->drawPos.y  = entity->centerPos.y;
    entity->velocity.x = entity->drawPos.x + drawX;
    entity->velocity.y = entity->drawPos.y + drawY;
}

void Platform_State_16()
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

void Platform_Unknown1()
{
    RSDK_THIS(Platform);
    entity->drawPos.x += entity->velocity.x;
    int x = entity->position.x;
    int y = entity->position.y;

    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, entity->hitbox.left << 16, entity->hitbox.bottom << 16, 0))
        entity->state = Platform_Unknown3;
    entity->position.x = x;
    entity->position.y = y;
}

void Platform_Unknown2()
{
    RSDK_THIS(Platform);
    entity->drawPos.x += entity->velocity.x;
    int x = entity->position.x;
    int y = entity->position.y;

    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    if (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, 0, entity->hitbox.right << 16, entity->hitbox.bottom << 16, 0))
        entity->state = Platform_Unknown3;
    entity->position.x = x;
    entity->position.y = y;
}

void Platform_Unknown3()
{
    RSDK_THIS(Platform);
    int x = entity->position.x;
    int y = entity->position.y;

    entity->drawPos.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->position.x = entity->drawPos.x;
    entity->position.y = entity->drawPos.y;
    bool32 collided    = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, (entity->hitbox.left + 16) << 16, entity->hitbox.bottom << 16, 4);
    int ypos           = entity->position.y;

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

void Platform_Unknown4()
{
    RSDK_THIS(Platform);
    entity->drawPos.x += entity->velocity.x;
    entity->drawPos.y += entity->velocity.y;
    EntityPlatform *platform = (EntityPlatform *)RSDK.GetEntityByID(entity->speed);

    if (entity->velocity.y <= 0) {
        if (entity->drawPos.y < platform->position.y)
            entity->drawPos.y = platform->position.y;
    }
    else if (entity->drawPos.y > platform->position.y) {
        entity->drawPos.y = platform->position.y;
    }

    if (entity->velocity.x <= 0) {
        if (entity->drawPos.x < platform->position.x)
            entity->drawPos.x = platform->position.x;
    }
    else if (entity->drawPos.x > platform->position.x) {
        entity->drawPos.x = platform->position.x;
    }
}

void Platform_Unknown5()
{
    RSDK_THIS(Platform);
    int drawX = -entity->drawPos.x;
    int drawY = -entity->drawPos.y;

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
            if (entity->type == 9)
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
                if (entity->type == 9)
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

void Platform_Unknown6()
{
    RSDK_THIS(Platform);
    entity->rotation += 4;
    entity->velocity.x = 0;
    entity->drawPos.y  = (RSDK.Sin1024(entity->rotation) << 9) + entity->centerPos.y;
    entity->velocity.y = 0;
}

void Platform_Unknown7()
{
    RSDK_THIS(Platform);
    if (entity->type == 9) {
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

void Platform_Unknown8()
{
    RSDK_THIS(Platform);
    if (!entity->stood) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
    }
    else {
        int slot                          = RSDK_sceneInfo->entitySlot - 1;
        EntityPlatformControl *controller = (EntityPlatformControl *)RSDK.GetEntityByID(slot);
        if (controller->objectID == PlatformControl->objectID) {
            // controller->gap64 = 1;
            entity->state      = Platform_Unknown4;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
        }
        else {
            while (controller->objectID == Platform->objectID || controller->objectID == PlatformNode->objectID) {
                controller = (EntityPlatformControl *)RSDK.GetEntityByID(slot--);
                if (controller->objectID == PlatformControl->objectID) {
                    // controller->gap64 = 1;
                    entity->state      = Platform_Unknown4;
                    entity->velocity.x = 0;
                    entity->velocity.y = 0;
                    break;
                }
            }
        }
    }
}

void Platform_Unknown9()
{
    RSDK_THIS(Platform);
    int drawX     = -entity->drawPos.x;
    int drawY     = -entity->drawPos.y;
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

void Platform_Unknown10()
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

void Platform_Unknown11()
{
    RSDK_THIS(Platform);
    int drawX = -entity->drawPos.x;
    int drawY = -entity->drawPos.y;
    int amp   = entity->amplitude.x * RSDK.Sin1024(entity->collapseDelay * entity->speed);
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

void Platform_Unknown12()
{
    RSDK_THIS(Platform);
    int drawX = -entity->drawPos.x;
    int drawY = -entity->drawPos.y;
    int speed = entity->angle * (entity->speed << 7);
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

    int y1, y2;
    if (((speed >> 0x10) & 1) == entity->direction) {
        entity->drawPos.x = entity->centerPos.x + ((ushort)speed * entity->amplitude.x >> 6) - (entity->amplitude.x << 9);
        y1                = (ushort)speed * entity->amplitude.y >> 6;
        y2                = entity->amplitude.y << 9;
    }
    else {
        entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9) - ((ushort)speed * entity->amplitude.x >> 6);
        y2                = (ushort)speed * entity->amplitude.y >> 6;
        y1                = entity->amplitude.y << 9;
    }
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->drawPos.y  = entity->centerPos.y + y1 - y2;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void Platform_Unknown13()
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

void Platform_Unknown14()
{
    RSDK_THIS(Platform);
    int drawX = -entity->drawPos.x;
    int drawY = -entity->drawPos.y;
    int speed = entity->angle * (entity->speed << 7);
    entity->angle--;

    if (speed > 0) {
        if (speed >= 0x10000)
            speed = 0x10000;
    }
    else {
        speed         = 0;
        entity->state = Platform_State_13;
    }

    int y1, y2;
    if (((speed >> 0x10) & 1) == entity->direction) {
        entity->drawPos.x = entity->centerPos.x + ((ushort)speed * entity->amplitude.x >> 6) - (entity->amplitude.x << 9);
        y1                = (ushort)speed * entity->amplitude.y >> 6;
        y2                = entity->amplitude.y << 9;
    }
    else {
        entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 9) - ((ushort)speed * entity->amplitude.x >> 6);
        y2                = (ushort)speed * entity->amplitude.y >> 6;
        y1                = entity->amplitude.y << 9;
    }
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->drawPos.y  = entity->centerPos.y + y1 - y2;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void Platform_EditorDraw() {}

void Platform_EditorLoad() {}

void Platform_Serialize()
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
