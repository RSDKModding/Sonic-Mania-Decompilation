#include "../SonicMania.h"

ObjectPlatform *Platform;

void Platform_Update() {}

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
    EntityPlatform *entity = (EntityPlatform *)RSDK_sceneInfo->entity;
    if (entity->frameID >= 0) {
        if (/*entity->state != Platform_State_Circular
            || !entity->hasTension && entity->state != Platform_State_Swing && entity->state != Platform_State_14 && entity->type != 12*/
            true) {
            if (Platform->spriteIndex == -1)
                RSDK.DrawRect(entity->drawPos.x - 0x200000, entity->drawPos.y - 0x100000, 0x400000, 0x200000, 0x8080A0, 255, 0, 0);
            else
                RSDK.DrawSprite(&entity->animData, &entity->drawPos, 0);
        }
        else {
            int ang     = entity->speed * Zone->timer + 4 * entity->angle;
            int fxStore = entity->drawFX;
            entity->drawFX |= FX_FLIP | FX_ROTATE;
            int cnt                  = (entity->amplitude.y >> 10) - 1;
            int angle                = 1024;
            entity->direction        = 0;
            entity->animData.frameID = entity->frameID + 1;

            int rot = ang >> 1;
            for (int i = 0; i < cnt; ++i) {
                drawPos.x = angle * RSDK.Sin1024(ang) + entity->centerPos.x;
                drawPos.y = angle * RSDK.Cos1024(ang) + entity->centerPos.y;
                RSDK.DrawSprite(&entity->animData, &drawPos, 0);
                angle += 1024;
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
    EntityPlatform *entity = (EntityPlatform *)RSDK_sceneInfo->entity;
    entity->amplitude.x >>= 10;
    entity->amplitude.y >>= 10;
    entity->active      = 4;
    entity->visible     = 1;
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
            // entity->state         = Platform_State_Collapsing;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            break;
        case 2:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 10;
            // entity->state              = Platform_State_Moving;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            entity->rotation      = entity->angle;
            entity->angle         = 0;
            break;
        case 3:
            entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 10;
            entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 10;
            // entity->state              = Platform_State_Circular;
            break;
        case 5:
        case 11:
            entity->active = ACTIVE_BOUNDS;
            // if (entity->type == 5)
            //    entity->state = Platform_State_Nothing;
            // else
            //    entity->state = Platform_Unknown8;
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
            // entity->state = Platform_State_StartPush;
            break;
        case 8:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);
            if (entity->speed < 0)
                entity->direction = FLIP_X;
            // entity->state = Platform_State_Wait;
            break;
        case 9:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);
            if (entity->speed < 0)
                entity->direction = 1;
            // entity->state = Platform_State_WaitBobbing;
            break;
        case 10:
            entity->amplitude.x <<= 10;
            entity->updateRange.x = 0x800000 + abs(entity->amplitude.x);
            entity->updateRange.y = 0x800000 + abs(entity->amplitude.x);

            if (entity->speed < 0) {
                entity->direction = 1;
                entity->speed     = -entity->speed;
            }
            // entity->state = Platform_State_PlayerActivated;
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
                // entity->state = Platform_State_Swing;
            }
            else {
                if (entity->groundVel >= 0)
                    entity->drawPos.x -= 0x200000;
                else
                    entity->drawPos.x += 0x200000;
                // entity->state = Platform_State_12;
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

            // if (entity->type == 7)
            //    entity->state = Platform_State_MovingSpike;
            // else
            //    entity->state = Platform_State_13;
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
            // entity->state          = Platform_State_14;
            break;
        case 15:
            // entity->state         = Platform_State_15;
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
            // entity->state           = Platform_State_16;
            break;
        default:
            // entity->state = Platform_State_Normal;
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

void Platform_State_Falling() {}
void Platform_State_OffScreenReset() {}

void Platform_StageLoad()
{
    Platform->useClack = false;
    if (RSDK.CheckStageFolder("GHZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("GHZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("GHZCutscene")) {
        Platform->spriteIndex = RSDK.LoadAnimation("GHZCutscene/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("OOZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ1")) {
        Platform->spriteIndex = RSDK.LoadAnimation("SPZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ2")) {
        Platform->spriteIndex = RSDK.LoadAnimation("SPZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("FBZ/Platform.bin", SCOPE_STAGE);
        Platform->sfx_Clack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Platform->spriteIndex = RSDK.LoadAnimation("PSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("PSZ2")) {
        Platform->spriteIndex = RSDK.LoadAnimation("PSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1")) {
        Platform->spriteIndex = RSDK.LoadAnimation("SSZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ2")) {
        Platform->spriteIndex = RSDK.LoadAnimation("SSZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("HCZ/Platform.bin", SCOPE_STAGE);
        Platform->sfx_Clack   = RSDK.GetSFX("Stage/Clack2.wav");
        Platform->useClack    = true;
    }
    else if (RSDK.CheckStageFolder("MSZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("MSZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("OOZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("OOZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ1")) {
        Platform->spriteIndex = RSDK.LoadAnimation("LRZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3")) {
        Platform->spriteIndex = RSDK.LoadAnimation("LRZ2/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MMZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("MMZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
        Platform->spriteIndex = RSDK.LoadAnimation("TMZ1/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("AIZ")) {
        Platform->spriteIndex = RSDK.LoadAnimation("AIZ/Platform.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Blueprint")) {
        Platform->spriteIndex = RSDK.LoadAnimation("Blueprint/Platform.bin", SCOPE_STAGE);
    }

    Platform->sfx_Clacker = RSDK.GetSFX("Stage/Clacker.wav");
    Platform->sfx_Clang   = RSDK.GetSFX("Stage/Clang.wav");
    Platform->sfx_Push    = RSDK.GetSFX("Global/Push.wav");
    if (Platform->playingPushSFX) {
        RSDK.StopSFX(Platform->sfx_Push);
        Platform->playingPushSFX = 0;
    }
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
