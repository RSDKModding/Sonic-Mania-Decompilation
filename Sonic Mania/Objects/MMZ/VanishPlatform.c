// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: VanishPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectVanishPlatform *VanishPlatform;

void VanishPlatform_Update(void)
{
    RSDK_THIS(VanishPlatform);
    if (!((Zone->timer + self->intervalOffset) % self->interval) && self->state == Platform_State_Fixed) {
        self->active        = ACTIVE_NORMAL;
        self->timer = self->duration;
        self->state         = VanishPlatform_State_Appear;
    }

    if (self->scale.x <= 336) {
        self->stateCollide = Platform_Collision_None;
        self->collision    = PLATFORM_C_SOLID_NONE;
    }
    else {
        self->stateCollide = Platform_Collision_TopSolid;
        self->collision    = PLATFORM_C_SOLID_TOP;
    }
    Platform_Update();
}

void VanishPlatform_LateUpdate(void) {}

void VanishPlatform_StaticUpdate(void) {}

void VanishPlatform_Draw(void)
{
    RSDK_THIS(VanishPlatform);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void VanishPlatform_Create(void *data)
{
    RSDK_THIS(VanishPlatform);
    self->collision = PLATFORM_C_SOLID_TOP;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator, true, 0);
    self->drawFX  = FX_SCALE;
    self->scale.x = 0;
    self->scale.y = 0x200;
    self->state   = Platform_State_Fixed;
}

void VanishPlatform_StageLoad(void) {}

void VanishPlatform_State_Appear(void)
{
    RSDK_THIS(VanishPlatform);

    if (self->scale.x < 0x200)
        self->scale.x += 22;

    if (--self->timer <= 0)
        self->state = VanishPlatform_State_Disappear;
}

void VanishPlatform_State_Disappear(void)
{
    RSDK_THIS(VanishPlatform);

    if (self->scale.x <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Platform_State_Fixed;
    }
    else {
        self->scale.x -= 22;
    }
}

#if RETRO_INCLUDE_EDITOR
void VanishPlatform_EditorDraw(void)
{
    RSDK_THIS(VanishPlatform);
    self->drawPos = self->position;
    self->drawFX  = FX_NONE;

    VanishPlatform_Draw();
}

void VanishPlatform_EditorLoad(void) {}
#endif

void VanishPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(VanishPlatform, VAR_UINT16, duration);
}
