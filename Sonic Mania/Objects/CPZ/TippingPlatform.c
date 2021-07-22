#include "SonicMania.h"

ObjectTippingPlatform *TippingPlatform;

void TippingPlatform_Update(void)
{
    RSDK_THIS(TippingPlatform);

    if (entity->bossID) {
        if (entity->animator.frameID) {
            entity->stateCollide = Platform_CollisionState_Null;
            entity->collision    = 4;
        }
        else {
            entity->stateCollide = Platform_CollisionState_TopSolid;
            entity->collision    = 0;
        }
        if (!entity->stoodAngle && entity->bossID > 2 && entity->syringe->activated) {
            entity->stoodAngle    = 1;
            entity->collapseDelay = entity->intervalOffset + 1;
            RSDK.SetSpriteAnimation(Platform->spriteIndex, 1, &entity->animator, true, 0);
            entity->state = TippingPlatform_Unknown5;
        }
    }
    else {
        if (entity->state == Platform_State_Normal) {
            int timer = (Zone->timer + entity->intervalOffset) % entity->interval;
            if (timer >= entity->duration) {
                if (timer >= entity->duration + 22) {
                    RSDK.SetSpriteAnimation(Platform->spriteIndex, 2, &entity->animator, true, 6);
                }
                else {
                    entity->active = ACTIVE_NORMAL;
                    RSDK.SetSpriteAnimation(Platform->spriteIndex, 2, &entity->animator, true, (timer - entity->duration) >> 2);
                    entity->state = TippingPlatform_Unknown4;
                }
            }
            else {
                entity->active        = ACTIVE_NORMAL;
                entity->collapseDelay = entity->duration - timer;
                if (entity->duration - timer < entity->duration - 24) {
                    RSDK.SetSpriteAnimation(Platform->spriteIndex, 1, &entity->animator, true, 6);
                    entity->state = TippingPlatform_Unknown3;
                }
                else {
                    RSDK.SetSpriteAnimation(Platform->spriteIndex, 1, &entity->animator, true, timer >> 2);
                    entity->state                   = TippingPlatform_Unknown3;
                    entity->animator.animationTimer = (entity->duration - entity->collapseDelay) & 3;
                }
            }
        }

        bool32 flag = false;
        if (entity->animator.animationID == 1)
            flag = entity->animator.frameID == 0;
        else
            flag = entity->animator.frameID == 6;
        if (flag) {
            entity->stateCollide = Platform_CollisionState_TopSolid;
            entity->collision    = 0;
        }
        else {
            entity->stateCollide = Platform_CollisionState_Null;
            entity->collision    = 4;
        }
    }
    Platform_Update();
}

void TippingPlatform_LateUpdate(void) {}

void TippingPlatform_StaticUpdate(void) {}

void TippingPlatform_Draw(void)
{
    RSDK_THIS(TippingPlatform);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void TippingPlatform_Create(void *data)
{
    RSDK_THIS(TippingPlatform);
    if (!entity->interval)
        entity->interval = -16;
    if (!entity->duration)
        entity->duration = 120;
    entity->collision = 0;
    Platform_Create(NULL);

    if (entity->bossID) {
        RSDK.SetSpriteAnimation(Platform->spriteIndex, 1, &entity->animator, true, 0);
        entity->animator.animationSpeed = 0;
    }
    else {
        RSDK.SetSpriteAnimation(Platform->spriteIndex, 2, &entity->animator, true, 6);
    }

    entity->state = Platform_State_Normal;
    if (!RSDK_sceneInfo->inEditor) {
        if (entity->bossID > 1) {
            foreach_all(Syringe, syringe)
            {
                if (syringe->tag == entity->bossID) {
                    entity->syringe = syringe;
                    if (syringe) {
                        entity->updateRange.x = abs(entity->position.x - syringe->position.x) + 0x400000;
                        entity->updateRange.y = abs(entity->position.y - syringe->position.y) + 0x400000;
                    }
                    entity->stateCollide = Platform_CollisionState_AllSolid;
                    entity->collision    = 1;
                    foreach_break;
                }
            }
        }

        if (entity->bossID == 1) {
            entity->direction = FLIP_X;
            entity->drawFX    = FX_FLIP;
        }
    }
}

void TippingPlatform_StageLoad(void) {}

void TippingPlatform_Unknown1(void)
{
    RSDK_THIS(TippingPlatform);
    if (entity->animator.frameID < 6 && ++entity->animator.animationTimer == 4) {
        entity->animator.animationTimer = 0;
        entity->animator.frameID++;
    }

    if (--entity->collapseDelay <= 0) {
        entity->active = ACTIVE_BOUNDS;
        if (entity->bossID < 3)
            entity->state = TippingPlatform_Unknown2;
        else
            entity->state = Platform_State_Normal;
    }
}

void TippingPlatform_Unknown2(void)
{
    RSDK_THIS(TippingPlatform);
    if (entity->animator.frameID <= 0) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = Platform_State_Normal;
    }
    else if (++entity->animator.animationTimer == 4) {
        entity->animator.animationTimer = 0;
        entity->animator.frameID--;
    }
}

void TippingPlatform_Unknown3(void)
{
    RSDK_THIS(TippingPlatform);
    if (--entity->collapseDelay <= 0) {
        RSDK.SetSpriteAnimation(Platform->spriteIndex, 2, &entity->animator, true, 0);
        entity->state = TippingPlatform_Unknown2;
    }
}

void TippingPlatform_Unknown4(void)
{
    RSDK_THIS(TippingPlatform);
    if (entity->animator.frameID == 6) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = Platform_State_Normal;
    }
}

void TippingPlatform_Unknown5(void)
{
    RSDK_THIS(TippingPlatform);
    if (--entity->collapseDelay <= 0)
        entity->state = TippingPlatform_Unknown1;
}

void TippingPlatform_EditorDraw(void) {}

void TippingPlatform_EditorLoad(void) {}

void TippingPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, intervalOffset);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, duration);
    RSDK_EDITABLE_VAR(TippingPlatform, VAR_UINT8, bossID);
}
