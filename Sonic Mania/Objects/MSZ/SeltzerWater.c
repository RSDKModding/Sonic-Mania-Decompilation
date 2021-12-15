// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SeltzerWater Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSeltzerWater *SeltzerWater;

void SeltzerWater_Update(void)
{
    RSDK_THIS(SeltzerWater);
    StateMachine_Run(self->state);

    if (self->scale.x < 512) {
        self->scale.x += 16;
        self->scale.y = self->scale.x;
        if (self->scale.x == 512)
            self->drawFX = FX_NONE;
    }
}

void SeltzerWater_LateUpdate(void) {}

void SeltzerWater_StaticUpdate(void) {}

void SeltzerWater_Draw(void)
{
    RSDK_THIS(SeltzerWater);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SeltzerWater_Create(void *data)
{
    RSDK_THIS(SeltzerWater);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SeltzerWater->aniFrames, 4, &self->animator, true, RSDK.Rand(0, 8));
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x100000;
        self->updateRange.y = 0x100000;
        self->visible       = 1;
        self->drawFX        = FX_SCALE;
        self->scale.x       = 0x80;
        self->scale.y       = 0x80;
        self->field_5C      = RSDK.Rand(0, 256);
        self->state         = SeltzerWater_Unknown1;
    }
}

void SeltzerWater_StageLoad(void) { SeltzerWater->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE); }

void SeltzerWater_Unknown1(void)
{
    RSDK_THIS(SeltzerWater);
    EntityPlatformNode *node = RSDK_GET_ENTITY(self->nodeSlot, PlatformNode);
    RSDK.ProcessAnimation(&self->animator);
    if (node->objectID == PlatformNode->objectID) {
        int32 x = (self->position.x - node->position.x) >> 16;
        int32 y = (self->position.y - node->position.y) >> 16;

        self->angle      = RSDK.ATan2(x, y);
        self->velocity.x = -(self->field_64 * RSDK.Cos256(self->angle));
        self->velocity.y = 16 * RSDK.Sin256(self->field_5C) - self->field_64 * RSDK.Sin256(self->angle);
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        if (x * x + y * y < 80)
            ++self->nodeSlot;
        self->field_5C++;
    }
    else {
        self->state           = SeltzerWater_Unknown2;
        self->gravityStrength = 0x3800;
    }
}

void SeltzerWater_Unknown2(void)
{
    RSDK_THIS(SeltzerWater);
    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += self->gravityStrength;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x60000, true)) {
        RSDK.SetSpriteAnimation(SeltzerWater->aniFrames, 5, &self->animator, true, 0);
        self->state = SeltzerWater_Unknown3;
    }
    else {
        if (!RSDK.CheckOnScreen(self, NULL))
            destroyEntity(self);
    }
}

void SeltzerWater_Unknown3(void)
{
    RSDK_THIS(SeltzerWater);
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void SeltzerWater_EditorDraw(void) {}

void SeltzerWater_EditorLoad(void) {}
#endif

void SeltzerWater_Serialize(void) {}
