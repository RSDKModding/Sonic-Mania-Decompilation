// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZEggman Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPSZEggman *PSZEggman;

void PSZEggman_Update(void)
{
    RSDK_THIS(PSZEggman);

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->displayAnimator);
    RSDK.ProcessAnimation(&self->scanlinesAnimator);
}

void PSZEggman_LateUpdate(void) {}

void PSZEggman_StaticUpdate(void) {}

void PSZEggman_Draw(void)
{
    RSDK_THIS(PSZEggman);

    RSDK.DrawSprite(&self->terminalAnimator, NULL, false);
    RSDK.DrawSprite(&self->displayAnimator, NULL, false);

    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->scanlinesAnimator, NULL, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->eggmanAnimator, &self->eggmanPos, false);
}

void PSZEggman_Create(void *data)
{
    RSDK_THIS(PSZEggman);

    if (!SceneInfo->inEditor) {
        self->visible     = true;
        self->drawGroup   = Zone->objectDrawGroup[0];
        self->eggmanPos.x = self->position.x - 0x180000;
        self->eggmanPos.y = self->position.y + 0x10000;

        self->alpha         = 64;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 0, &self->terminalAnimator, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 1, &self->displayAnimator, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 2, &self->scanlinesAnimator, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->aniFrames, 0, &self->eggmanAnimator, true, 0);
    }
}

void PSZEggman_StageLoad(void)
{
    PSZEggman->controlFrames = RSDK.LoadSpriteAnimation("PSZ2/ControlPanel.bin", SCOPE_STAGE);
    PSZEggman->aniFrames     = RSDK.LoadSpriteAnimation("Eggman/EggmanPSZ.bin", SCOPE_STAGE);
}

void PSZEggman_State_TurnRound(void)
{
    RSDK_THIS(PSZEggman);

    if (self->eggmanAnimator.timer == 1) {
        if (self->ruby) {
            self->ruby->position.x += 0x10000;
            ++self->timer;
        }
        else if (self->eggmanAnimator.frameID == 7) {
            self->ruby = CREATE_ENTITY(PhantomRuby, NULL, self->eggmanPos.x - 0x100000, self->eggmanPos.y - 0x20000);
        }
    }

    if (self->timer == 6) {
        self->ruby->startPos.x = self->ruby->position.x;
        self->ruby->startPos.y = self->ruby->position.y;
        self->ruby->state      = PhantomRuby_State_Oscillate;
        self->state            = StateMachine_None;
    }
}

#if GAME_INCLUDE_EDITOR
void PSZEggman_EditorDraw(void)
{
    RSDK_THIS(PSZEggman);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->eggmanPos.x   = self->position.x - 0x180000;
    self->eggmanPos.y   = self->position.y + 0x10000;
    self->alpha         = 0x40;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 0, &self->terminalAnimator, true, 0);
    RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 1, &self->displayAnimator, true, 0);
    RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 2, &self->scanlinesAnimator, true, 0);
    RSDK.SetSpriteAnimation(PSZEggman->aniFrames, 0, &self->eggmanAnimator, true, 0);

    PSZEggman_Draw();
}

void PSZEggman_EditorLoad(void)
{
    PSZEggman->controlFrames = RSDK.LoadSpriteAnimation("PSZ2/ControlPanel.bin", SCOPE_STAGE);
    PSZEggman->aniFrames     = RSDK.LoadSpriteAnimation("Eggman/EggmanPSZ.bin", SCOPE_STAGE);
}
#endif

void PSZEggman_Serialize(void) {}
