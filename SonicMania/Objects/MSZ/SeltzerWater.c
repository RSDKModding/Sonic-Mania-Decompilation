// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SeltzerWater Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSeltzerWater *SeltzerWater;

void SeltzerWater_Update(void)
{
    RSDK_THIS(SeltzerWater);

    StateMachine_Run(self->state);

    if (self->scale.x < 0x200) {
        self->scale.x += 0x10;
        self->scale.y = self->scale.x;

        if (self->scale.x == 0x200)
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
        self->visible       = true;
        self->drawFX        = FX_SCALE;
        self->scale.x       = 0x80;
        self->scale.y       = 0x80;
        self->offsetAngle   = RSDK.Rand(0, 256);
        self->state         = SeltzerWater_State_Sprayed;
    }
}

void SeltzerWater_StageLoad(void) { SeltzerWater->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE); }

void SeltzerWater_State_Sprayed(void)
{
    RSDK_THIS(SeltzerWater);

    RSDK.ProcessAnimation(&self->animator);

    EntityPlatformNode *node = RSDK_GET_ENTITY(self->nodeSlot, PlatformNode);
    if (node->classID == PlatformNode->classID) {
        int32 x = (self->position.x - node->position.x) >> 16;
        int32 y = (self->position.y - node->position.y) >> 16;

        self->angle      = RSDK.ATan2(x, y);
        self->velocity.x = -(self->oscillateRadius * RSDK.Cos256(self->angle));
        self->velocity.y = (RSDK.Sin256(self->offsetAngle) << 4) - self->oscillateRadius * RSDK.Sin256(self->angle);
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        if (x * x + y * y < 0x50)
            ++self->nodeSlot;

        self->offsetAngle++;
    }
    else {
        self->state           = SeltzerWater_State_Falling;
        self->gravityStrength = 0x3800;
    }
}

void SeltzerWater_State_Falling(void)
{
    RSDK_THIS(SeltzerWater);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += self->gravityStrength;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x60000, true)) {
        RSDK.SetSpriteAnimation(SeltzerWater->aniFrames, 5, &self->animator, true, 0);
        self->state = SeltzerWater_State_Splash;
    }
    else {
        if (!RSDK.CheckOnScreen(self, NULL))
            destroyEntity(self);
    }
}

void SeltzerWater_State_Splash(void)
{
    RSDK_THIS(SeltzerWater);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void SeltzerWater_EditorDraw(void)
{
    RSDK_THIS(SeltzerWater);

    RSDK.SetSpriteAnimation(SeltzerWater->aniFrames, 4, &self->animator, true, 3);
    SeltzerWater_Draw();
}

void SeltzerWater_EditorLoad(void) { SeltzerWater->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Seltzer.bin", SCOPE_STAGE); }
#endif

void SeltzerWater_Serialize(void) {}
