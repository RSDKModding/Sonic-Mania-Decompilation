// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FoldingPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
            self->timer            = timer - self->duration;
            self->state            = FoldingPlatform_State_Disappear;
            self->animator.frameID = 4 - ((timer - self->duration) >> 1);
        }
    }
    else {
        self->active = ACTIVE_NORMAL;
        self->timer  = timer;
        if (timer > 8)
            self->animator.frameID = 4;
        else
            self->animator.frameID = timer >> 1;
        self->state = FoldingPlatform_State_Appear;
    }

    if (self->animator.frameID == 4) {
        self->stateCollide = Platform_Collision_Platform;
        self->collision    = PLATFORM_C_PLATFORM;
    }
    else {
        self->stateCollide = Platform_Collision_None;
        self->collision    = PLATFORM_C_NONE;
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

    self->collision = PLATFORM_C_PLATFORM;
    Platform_Create(NULL);

    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 0);
    self->drawFX  = FX_FLIP;
    self->scale.x = 0x000;
    self->scale.y = 0x200;
    self->state   = Platform_State_Fixed;
}

void FoldingPlatform_StageLoad(void) {}

void FoldingPlatform_State_Appear(void)
{
    RSDK_THIS(FoldingPlatform);

    if (!(++self->timer & 1)) {
        if (self->animator.frameID < 4)
            self->animator.frameID++;
    }

    if (self->timer == self->duration)
        self->state = FoldingPlatform_State_Disappear;
}

void FoldingPlatform_State_Disappear(void)
{
    RSDK_THIS(FoldingPlatform);

    --self->timer;
    if (!(self->timer & 1)) {
        if (self->animator.frameID > 0) {
            self->animator.frameID--;
        }
    }

    if (!self->animator.frameID) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Platform_State_Fixed;
    }
}

#if GAME_INCLUDE_EDITOR
void FoldingPlatform_EditorDraw(void)
{
    RSDK_THIS(FoldingPlatform);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 4);
    self->drawPos = self->position;

    FoldingPlatform_Draw();
}

void FoldingPlatform_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(FoldingPlatform, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void FoldingPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT16, duration);
}
