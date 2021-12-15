// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZEggman Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPSZEggman *PSZEggman;

void PSZEggman_Update(void)
{
    RSDK_THIS(PSZEggman);
    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
}

void PSZEggman_LateUpdate(void) {}

void PSZEggman_StaticUpdate(void) {}

void PSZEggman_Draw(void)
{
    RSDK_THIS(PSZEggman);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void PSZEggman_Create(void *data)
{
    RSDK_THIS(PSZEggman);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->drawPos.x     = self->position.x - 0x180000;
        self->drawPos.y     = self->position.y + 0x10000;
        self->alpha         = 64;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 1, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 2, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(PSZEggman->aniFrames, 0, &self->animator, true, 0);
    }
}

void PSZEggman_StageLoad(void)
{
    PSZEggman->controlFrames = RSDK.LoadSpriteAnimation("PSZ2/ControlPanel.bin", SCOPE_STAGE);
    PSZEggman->aniFrames     = RSDK.LoadSpriteAnimation("Eggman/EggmanPSZ.bin", SCOPE_STAGE);
}

void PSZEggman_Unknown1(void)
{
    RSDK_THIS(PSZEggman);
    if (self->animator.timer == 1) {
        if (self->ruby) {
            self->ruby->position.x += 0x10000;
            ++self->timer;
        }
        else if (self->animator.frameID == 7) {
            self->ruby = CREATE_ENTITY(PhantomRuby, NULL, self->drawPos.x - 0x100000, self->drawPos.y - 0x20000);
        }
    }
    if (self->timer == 6) {
        self->ruby->startPos.x = self->ruby->position.x;
        self->ruby->startPos.y = self->ruby->position.y;
        self->ruby->state      = PhantomRuby_State_Oscillate;
        self->state            = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void PSZEggman_EditorDraw(void)
{
    RSDK_THIS(PSZEggman);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->drawPos.x     = self->position.x - 0x180000;
    self->drawPos.y     = self->position.y + 0x10000;
    self->alpha         = 0x40;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 1, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(PSZEggman->controlFrames, 2, &self->animator4, true, 0);
    RSDK.SetSpriteAnimation(PSZEggman->aniFrames, 0, &self->animator, true, 0);

    PSZEggman_Draw();
}

void PSZEggman_EditorLoad(void)
{
    PSZEggman->controlFrames = RSDK.LoadSpriteAnimation("PSZ2/ControlPanel.bin", SCOPE_STAGE);
    PSZEggman->aniFrames     = RSDK.LoadSpriteAnimation("Eggman/EggmanPSZ.bin", SCOPE_STAGE);
}
#endif

void PSZEggman_Serialize(void) {}
