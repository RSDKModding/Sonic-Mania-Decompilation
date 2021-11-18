#include "SonicMania.h"

ObjectConveyorPlatform *ConveyorPlatform;

void ConveyorPlatform_Update(void)
{
    RSDK_THIS(ConveyorPlatform);
    if (self->state == Platform_Unknown4) {
        if (self->collapseDelay || self->timer) {
            if (++self->timer == 24) {
                self->stateCollide = Platform_CollisionState_AllSolid;
                self->collision    = PLATFORM_C_1;
                self->timer        = 0;
            }
            else {
                self->stateCollide = Platform_CollisionState_None;
                self->collision    = PLATFORM_C_4;
            }
            self->animator.frameID = ConveyorPlatform->frameIDs[self->timer];
            self->direction        = ConveyorPlatform->directionIDs[self->timer];
        }
    }
    else {
        if (self->timer) {
            self->timer++;
            if (self->timer >= self->flipCount) {
                self->stateCollide = Platform_CollisionState_AllSolid;
                self->collision    = PLATFORM_C_1;
                self->timer        = 0;
            }
            self->animator.frameID = ConveyorPlatform->frameIDs[self->timer % 24];
            self->direction        = ConveyorPlatform->directionIDs[self->timer % 24];
        }
        if (!((Zone->timer + self->intervalOffset) % self->interval) && !self->timer) {
            self->stateCollide = Platform_CollisionState_None;
            self->collision    = PLATFORM_C_4;
            self->timer        = 1;
        }
    }
    Platform_Update();
}

void ConveyorPlatform_LateUpdate(void) {}

void ConveyorPlatform_StaticUpdate(void) {}

void ConveyorPlatform_Draw(void)
{
    RSDK_THIS(ConveyorPlatform);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void ConveyorPlatform_Create(void *data)
{
    RSDK_THIS(ConveyorPlatform);
    if (self->type)
        self->type = PLATFORM_5;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 0);
    self->drawFX |= FX_FLIP;
    self->stateCollide = Platform_CollisionState_AllSolid;
    self->collision    = PLATFORM_C_1;
    self->timer        = 0;
    if (!SceneInfo->inEditor)
        self->flipCount *= 12;
}

void ConveyorPlatform_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void ConveyorPlatform_EditorDraw(void)
{
    RSDK_THIS(ConveyorPlatform);

    self->drawPos = self->position;
    ConveyorPlatform_Draw();
}

void ConveyorPlatform_EditorLoad(void) {}
#endif

void ConveyorPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_ENUM, node);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_UINT16, flipCount);
}
