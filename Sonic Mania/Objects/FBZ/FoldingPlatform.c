#include "SonicMania.h"

ObjectFoldingPlatform *FoldingPlatform;

void FoldingPlatform_Update(void)
{
    RSDK_THIS(FoldingPlatform);
    int32 timer = (Zone->timer + self->intervalOffset) % self->interval;

    if (timer >= self->duration) {
        if (timer >= self->duration + 8) {
            self->animator.frameID = 0;
        }
        else {
            self->active           = ACTIVE_NORMAL;
            self->collapseDelay    = timer - self->duration;
            self->state            = FoldingPlatform_Unknown2;
            self->animator.frameID = 4 - ((timer - self->duration) >> 1);
        }
    }
    else {
        self->active        = ACTIVE_NORMAL;
        self->collapseDelay = timer;
        if (timer > 8)
            self->animator.frameID = 4;
        else
            self->animator.frameID = timer >> 1;
        self->state = FoldingPlatform_Unknown1;
    }
    if (self->animator.frameID == 4) {
        self->stateCollide = Platform_CollisionState_TopSolid;
        self->collision    = 0;
    }
    else {
        self->stateCollide = Platform_CollisionState_None;
        self->collision    = 4;
    }
    Platform_Update();
}

void FoldingPlatform_LateUpdate(void) {}

void FoldingPlatform_StaticUpdate(void) {}

void FoldingPlatform_Draw(void)
{
    RSDK_THIS(FoldingPlatform);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void FoldingPlatform_Create(void *data)
{
    RSDK_THIS(FoldingPlatform);
    if (!self->interval)
        self->interval = 480;
    if (!self->duration)
        self->duration = 240;
    self->collision = PLATFORM_C_0;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 0);
    self->drawFX  = FX_FLIP;
    self->scale.x = 0x000;
    self->scale.y = 0x200;
    self->state   = Platform_State_Normal;
}

void FoldingPlatform_StageLoad(void) {}

void FoldingPlatform_Unknown1(void)
{
    RSDK_THIS(FoldingPlatform);
    if (!(++self->collapseDelay & 1)) {
        if (self->animator.frameID < 4)
            self->animator.frameID++;
    }

    if (self->collapseDelay == self->duration)
        self->state = FoldingPlatform_Unknown2;
}

void FoldingPlatform_Unknown2(void)
{
    RSDK_THIS(FoldingPlatform);
    --self->collapseDelay;
    if (!(self->collapseDelay & 1)) {
        if (self->animator.frameID > 0) {
            self->animator.frameID--;
        }
    }

    if (!self->animator.frameID) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Platform_State_Normal;
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
