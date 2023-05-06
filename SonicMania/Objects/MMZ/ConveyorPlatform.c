// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ConveyorPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectConveyorPlatform *ConveyorPlatform;

void ConveyorPlatform_Update(void)
{
    RSDK_THIS(ConveyorPlatform);

    if (self->state == Platform_State_Path) {
        if (self->timer || self->flipTimer) {
            if (++self->flipTimer == 24) {
                self->stateCollide = Platform_Collision_Solid;
                self->collision    = PLATFORM_C_SOLID;
                self->flipTimer    = 0;
            }
            else {
                self->stateCollide = Platform_Collision_None;
                self->collision    = PLATFORM_C_NONE;
            }

            self->animator.frameID = ConveyorPlatform->frameIDs[self->flipTimer];
            self->direction        = ConveyorPlatform->directionIDs[self->flipTimer];
        }
    }
    else {
        if (self->flipTimer) {
            if (++self->flipTimer >= self->flipCount) {
                self->stateCollide = Platform_Collision_Solid;
                self->collision    = PLATFORM_C_SOLID;
                self->flipTimer    = 0;
            }

            self->animator.frameID = ConveyorPlatform->frameIDs[self->flipTimer % 24];
            self->direction        = ConveyorPlatform->directionIDs[self->flipTimer % 24];
        }

        if (!((Zone->persistentTimer + self->intervalOffset) % self->interval) && !self->flipTimer) {
            self->stateCollide = Platform_Collision_None;
            self->collision    = PLATFORM_C_NONE;
            self->flipTimer    = 1;
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
        self->type = PLATFORM_PATH;

    Platform_Create(NULL);

    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 0);

    self->drawFX |= FX_FLIP;
    self->stateCollide = Platform_Collision_Solid;
    self->collision    = PLATFORM_C_SOLID;
    self->flipTimer    = 0;

    if (!SceneInfo->inEditor)
        self->flipCount *= 12;
}

void ConveyorPlatform_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void ConveyorPlatform_EditorDraw(void)
{
    RSDK_THIS(ConveyorPlatform);

    ConveyorPlatform_Create(NULL);
    self->drawPos = self->position;

    ConveyorPlatform_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void ConveyorPlatform_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(ConveyorPlatform, type);
    RSDK_ENUM_VAR("Fixed", CONVEYORPLATFORM_FIXED);
    RSDK_ENUM_VAR("Moving", CONVEYORPLATFORM_MOVING);
}
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
