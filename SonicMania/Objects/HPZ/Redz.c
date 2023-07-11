// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Redz Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRedz *Redz = NULL;

void Redz_Update(void)
{
    RSDK_THIS(Redz);
    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);

    if (self->state != Redz_State_Init && self->state != Redz_Flame_State && self->state != Redz_Flame_Setup) {
        Redz_CheckPlayerCollisions();
        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->direction  = self->startDir;
            self->position.x = self->startPos.x;
            self->position.y = self->startPos.y;
            Redz_Create(NULL);
        }
    }
}

void Redz_LateUpdate(void) {}

void Redz_StaticUpdate(void) {}

void Redz_Draw(void)
{
    RSDK_THIS(Redz);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Redz_Create(void *data)
{
    RSDK_THIS(Redz);
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Redz_State_Init;
}

void Redz_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HPZ"))
        Redz->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Redz.bin", SCOPE_STAGE);

    Redz->hitboxBadnik.left   = -16;
    Redz->hitboxBadnik.top    = -16;
    Redz->hitboxBadnik.right  = 16;
    Redz->hitboxBadnik.bottom = 16;

    Redz->attackbox.left   = -64;
    Redz->attackbox.top    = -96;
    Redz->attackbox.right  = 0;
    Redz->attackbox.bottom = -8;

    Redz->hitboxFlame.left   = -7;
    Redz->hitboxFlame.top    = -7;
    Redz->hitboxFlame.right  = 7;
    Redz->hitboxFlame.bottom = -7;

    Redz->hitboxRange.left   = 0;
    Redz->hitboxRange.top    = 0;
    Redz->hitboxRange.right  = 0;
    Redz->hitboxRange.bottom = 0;

    Redz->sfxFlame = RSDK.GetSfx("Stage/Flame.wav");

    DEBUGMODE_ADD_OBJ(Redz);
}

void Redz_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityRedz *redz = CREATE_ENTITY(Redz, NULL, self->position.x, self->position.y);
    redz->direction  = self->direction;
    redz->startDir   = self->direction;
}

void Redz_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Redz_CheckPlayerCollisions(void)
{
    RSDK_THIS(Redz);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Redz->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Redz_State_Init(void)
{
    RSDK_THIS(Redz);

    self->active = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &self->animator, true, 0);
    self->animator.frameID = 0;
    self->animator.speed   = 1;
    self->state            = Redz_State_Walk;
    self->velocity.x       = (2 * (self->direction != FLIP_NONE) - 1) << 15;
    Redz_State_Walk();
}

void Redz_State_Walk(void)
{
    RSDK_THIS(Redz);

    self->position.x += self->velocity.x;
    if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, (2 * (self->direction != FLIP_NONE) - 1) << 19, 0x100000, 8)) {
        self->state = Redz_State_Turn;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &self->animator, true, 0);
        self->animator.frameID = 0;
        self->animator.speed   = 0;
    }

    if (self->attackDelay <= 0) {
        foreach_active(Player, player)
        {
            if (RSDK.CheckObjectCollisionTouchBox(player, &Redz->hitboxRange, self, &Redz->attackbox)) {
                self->state = Redz_State_PrepareAttack;
                RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &self->animator, true, 0);
                self->animator.frameID = 0;
                self->attackDelay      = 60;
                self->animator.speed   = 0;
            }
        }
    }
    else {
        self->attackDelay--;
    }
}

void Redz_State_Turn(void)
{
    RSDK_THIS(Redz);

    if (self->timer < 59) {
        self->timer++;
    }
    else {
        self->state = Redz_State_Walk;
        self->timer = 0;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &self->animator, true, 0);
        self->animator.frameID = 0;
        self->animator.speed   = 1;
        self->direction        = self->direction == FLIP_NONE;
        self->velocity.x       = -self->velocity.x;
    }
}

void Redz_State_PrepareAttack(void)
{
    RSDK_THIS(Redz);
    if (self->timer >= 30) {
        self->state = Redz_State_Attack;
        self->timer = 0;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 1, &self->animator, true, 0);
        RSDK.PlaySfx(Redz->sfxFlame, false, 255);
    }
    else {
        self->timer++;
    }
}

void Redz_State_Attack(void)
{
    RSDK_THIS(Redz);
    if (self->timer >= 90) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &self->animator, true, 0);
        self->animator.frameID = 0;
        self->animator.speed   = 1;
        self->state            = Redz_State_Walk;
    }

    if (!(self->timer & 3)) {
        EntityRedz *flame = CREATE_ENTITY(Redz, self, self->position.x, self->position.y);
        flame->state      = Redz_Flame_Setup;
        flame->position.y -= 0x40000;
        flame->position.x += (2 * (self->direction != FLIP_NONE) - 1) << 19;
        flame->velocity.x =
            (2 * (self->direction != FLIP_NONE) - 1) * (RSDK.Cos512(((RSDK.Sin512(8 * (self->timer & 0x3F)) >> 5) - 48) & 0x1FF) << 8);
        flame->velocity.y = RSDK.Sin512(((RSDK.Sin512(8 * (self->timer & 0x3F)) >> 5) - 48) & 0x1FF) << 8;
    }
    ++self->timer;
}

void Redz_Flame_Setup(void)
{
    RSDK_THIS(Redz);

    self->active = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(Redz->aniFrames, 2, &self->animator, true, 0);
    self->animator.frameID = 0;
    self->state            = Redz_Flame_State;
    self->animator.speed   = 1;
    Redz_Flame_State();
}

void Redz_Flame_State(void)
{
    RSDK_THIS(Redz);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Redz->hitboxFlame)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }

    if (++self->timer > 20)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Redz_EditorDraw(void)
{
    RSDK_THIS(Redz);
    RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &self->animator, true, 0);

    Redz_Draw();
}

void Redz_EditorLoad(void)
{
    Redz->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Redz.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Redz, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Redz_Serialize(void) { RSDK_EDITABLE_VAR(Redz, VAR_UINT8, direction); }
