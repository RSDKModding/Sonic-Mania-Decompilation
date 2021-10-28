#include "SonicMania.h"

ObjectFoldingPlatform *FoldingPlatform;

void FoldingPlatform_Update(void)
{
    RSDK_THIS(FoldingPlatform);
    int32 timer = (Zone->timer + entity->intervalOffset) % entity->interval;

    if (timer >= entity->duration) {
        if (timer >= entity->duration + 8) {
            entity->animator.frameID = 0;
        }
        else {
            entity->active           = ACTIVE_NORMAL;
            entity->collapseDelay    = timer - entity->duration;
            entity->state            = FoldingPlatform_Unknown2;
            entity->animator.frameID = 4 - ((timer - entity->duration) >> 1);
        }
    }
    else {
        entity->active        = ACTIVE_NORMAL;
        entity->collapseDelay = timer;
        if (timer > 8)
            entity->animator.frameID = 4;
        else
            entity->animator.frameID = timer >> 1;
        entity->state = FoldingPlatform_Unknown1;
    }
    if (entity->animator.frameID == 4) {
        entity->stateCollide = Platform_CollisionState_TopSolid;
        entity->collision    = 0;
    }
    else {
        entity->stateCollide = Platform_CollisionState_None;
        entity->collision    = 4;
    }
    Platform_Update();
}

void FoldingPlatform_LateUpdate(void) {}

void FoldingPlatform_StaticUpdate(void) {}

void FoldingPlatform_Draw(void)
{
    RSDK_THIS(FoldingPlatform);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void FoldingPlatform_Create(void *data)
{
    RSDK_THIS(FoldingPlatform);
    if (!entity->interval)
        entity->interval = 480;
    if (!entity->duration)
        entity->duration = 240;
    entity->collision = PLATFORM_C_0;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->spriteIndex, 2, &entity->animator, true, 0);
    entity->drawFX  = FX_FLIP;
    entity->scale.x = 0x000;
    entity->scale.y = 0x200;
    entity->state   = Platform_State_Normal;
}

void FoldingPlatform_StageLoad(void) {}

void FoldingPlatform_Unknown1(void)
{
    RSDK_THIS(FoldingPlatform);
    if (!(++entity->collapseDelay & 1)) {
        if (entity->animator.frameID < 4)
            entity->animator.frameID++;
    }

    if (entity->collapseDelay == entity->duration)
        entity->state = FoldingPlatform_Unknown2;
}

void FoldingPlatform_Unknown2(void)
{
    RSDK_THIS(FoldingPlatform);
    --entity->collapseDelay;
    if (!(entity->collapseDelay & 1)) {
        if (entity->animator.frameID > 0) {
            entity->animator.frameID--;
        }
    }

    if (!entity->animator.frameID) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = Platform_State_Normal;
    }
}

#if RETRO_INCLUDE_EDITOR
void FoldingPlatform_EditorDraw(void) {}

void FoldingPlatform_EditorLoad(void) {}
#endif

void FoldingPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT16, duration);
}
