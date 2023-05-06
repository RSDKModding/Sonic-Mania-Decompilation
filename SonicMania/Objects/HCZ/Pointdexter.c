// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Pointdexter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPointdexter *Pointdexter;

void Pointdexter_Update(void)
{
    RSDK_THIS(Pointdexter);

    StateMachine_Run(self->state);
}

void Pointdexter_LateUpdate(void) {}

void Pointdexter_StaticUpdate(void) {}

void Pointdexter_Draw(void)
{
    RSDK_THIS(Pointdexter);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Pointdexter_Create(void *data)
{
    RSDK_THIS(Pointdexter);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(Pointdexter->aniFrames, 0, &self->animator, true, 0);
    self->state = Pointdexter_State_Init;
}

void Pointdexter_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HCZ"))
        Pointdexter->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Pointdexter.bin", SCOPE_STAGE);

    Pointdexter->hitbox.left   = -16;
    Pointdexter->hitbox.top    = -6;
    Pointdexter->hitbox.right  = 16;
    Pointdexter->hitbox.bottom = 6;

    DEBUGMODE_ADD_OBJ(Pointdexter);
}

void Pointdexter_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Pointdexter, NULL, self->position.x, self->position.y);
}

void Pointdexter_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Pointdexter->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Pointdexter_CheckOffScreen(void)
{
    RSDK_THIS(Pointdexter);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Pointdexter_Create(NULL);
    }
}

void Pointdexter_CheckPlayerCollisions(void)
{
    RSDK_THIS(Pointdexter);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Pointdexter->hitbox)) {
            if (self->animator.frameID != 2 || player->invincibleTimer || player->blinkTimer
#if MANIA_USE_PLUS
                || player->state == Player_State_MightyHammerDrop
#endif
            ) {
                Player_CheckBadnikBreak(player, self, true);
            }
#if MANIA_USE_PLUS
            else if (!Player_CheckMightyUnspin(player, 0x200, 2, &player->uncurlTimer))
#else
            else
#endif
                Player_Hurt(player, self);
        }
    }
}

void Pointdexter_State_Init(void)
{
    RSDK_THIS(Pointdexter);

    if (self->position.y >= Water->waterLevel) {
        self->active     = ACTIVE_NORMAL;
        self->velocity.x = self->direction == FLIP_NONE ? -0x4000 : 0x4000;
        self->timer      = 0;

        self->state = Pointdexter_State_Swimming;
        Pointdexter_State_Swimming();
    }
    else {
        destroyEntity(self);
    }
}

void Pointdexter_State_Swimming(void)
{
    RSDK_THIS(Pointdexter);

    self->position.x += self->velocity.x;

    if (!--self->timer) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
        self->timer      = 0;
    }

    self->position.y += 32 * RSDK.Sin256(self->angle);
    self->angle = (self->angle + 2) & 0xFF;

    RSDK.ProcessAnimation(&self->animator);

    Pointdexter_CheckPlayerCollisions();
    Pointdexter_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Pointdexter_EditorDraw(void) { Pointdexter_Draw(); }

void Pointdexter_EditorLoad(void)
{
    Pointdexter->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Pointdexter.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Pointdexter, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Pointdexter_Serialize(void) { RSDK_EDITABLE_VAR(Pointdexter, VAR_UINT8, direction); }
