// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PopcornKernel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPopcornKernel *PopcornKernel;

void PopcornKernel_Update(void)
{
    RSDK_THIS(PopcornKernel);

    StateMachine_Run(self->state);

    self->angle += self->angleVel;
    self->rotation = (self->angle >> 15) & 0x1FF;
}

void PopcornKernel_LateUpdate(void) {}

void PopcornKernel_StaticUpdate(void) {}

void PopcornKernel_Draw(void)
{
    RSDK_THIS(PopcornKernel);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PopcornKernel_Create(void *data)
{
    RSDK_THIS(PopcornKernel);

    if (!SceneInfo->inEditor) {
        self->drawFX = FX_ROTATE;
        self->state  = PopcornKernel_State_BounceAround;

        RSDK.SetSpriteAnimation(PopcornKernel->aniFrames, 1, &self->animator, true, RSDK.Rand(0, 7));
        if (self->animator.frameID >= 0 && (self->animator.frameID <= 1 || self->animator.frameID == 5))
            self->drawGroup = Zone->objectDrawGroup[0] - 1;
        else
            self->drawGroup = Zone->objectDrawGroup[0];

        self->active          = ACTIVE_NORMAL;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x2000000;
        self->gravityStrength = 0x4000;
        self->scale.x         = 0x200;
        self->scale.y         = 0x200;
        self->visible         = true;
    }
}

void PopcornKernel_StageLoad(void) { PopcornKernel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/PopcornMachine.bin", SCOPE_STAGE); }

void PopcornKernel_State_BounceAround(void)
{
    RSDK_THIS(PopcornKernel);

    self->velocity.y += self->gravityStrength;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->velocity.y >= 0) {
        foreach_active(PopcornKernel, kernel)
        {
            if (kernel != self) {
                int32 rx = (self->position.x - kernel->position.x) >> 16;
                int32 ry = (self->position.y - kernel->position.y) >> 16;
                if (rx * rx + ry * ry < 0x100) {
                    int32 angle      = RSDK.ATan2(rx, ry);
                    self->velocity.x = RSDK.Cos256(angle) << 17 >> 8;
                    self->velocity.y = RSDK.Sin256(angle) << 17 >> 8;
                }
            }
        }
    }

    if (self->position.x < self->bounds.x - 0x680000) {
        self->position.x = self->bounds.x - 0x680000;
        self->velocity.x = -self->velocity.x;
    }
    else if (self->position.x > self->bounds.x + 0x680000) {
        self->position.x = self->bounds.x + 0x680000;
        self->velocity.x = -self->velocity.x;
    }

    if (self->position.y > self->bounds.y - 0x380000) {
        self->position.y = self->bounds.y - 0x380000;
        self->velocity.y = -self->velocity.y >> 1;
    }
}

void PopcornKernel_State_RiseUp(void)
{
    RSDK_THIS(PopcornKernel);

    self->velocity.y += 0x800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y < self->maxY) {
        self->state = PopcornKernel_State_FallDown;
        //???
        // Dunno why this is here but removing it would change the internal randSeed
        RSDK.Rand(-0x80000, 0x80000);
    }
}

void PopcornKernel_State_FallDown(void)
{
    RSDK_THIS(PopcornKernel);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void PopcornKernel_EditorDraw(void)
{
    RSDK_THIS(PopcornKernel);
    RSDK.SetSpriteAnimation(PopcornKernel->aniFrames, 1, &self->animator, true, 0);

    PopcornKernel_Draw();
}

void PopcornKernel_EditorLoad(void) { PopcornKernel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/PopcornMachine.bin", SCOPE_STAGE); }
#endif

void PopcornKernel_Serialize(void) {}
