// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Wisp Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWisp *Wisp = NULL;

void Wisp_Update(void)
{
    RSDK_THIS(Wisp);
    StateMachine_Run(self->state);
}

void Wisp_LateUpdate(void) {}

void Wisp_StaticUpdate(void) {}

void Wisp_Draw(void)
{
    RSDK_THIS(Wisp);

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->wingAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void Wisp_Create(void *data)
{
    RSDK_THIS(Wisp);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->direction     = FLIP_NONE;
    self->target        = NULL;
    self->alpha         = 0xC0;
    self->timer         = 16;
    self->buzzCount     = 4;
    RSDK.SetSpriteAnimation(Wisp->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Wisp->aniFrames, 1, &self->wingAnimator, true, 0);

    self->state = Wisp_State_Init;
}

void Wisp_StageLoad(void)
{
    Wisp->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/Wisp.bin", SCOPE_STAGE);

    Wisp->hitboxBadnik.left   = -8;
    Wisp->hitboxBadnik.top    = -8;
    Wisp->hitboxBadnik.right  = 8;
    Wisp->hitboxBadnik.bottom = 8;

    DEBUGMODE_ADD_OBJ(Wisp);
}

void Wisp_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Wisp, NULL, self->position.x, self->position.y);
}

void Wisp_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Wisp->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Wisp_HandlePlayerInteractions(void)
{
    RSDK_THIS(Wisp);

    foreach_active(Player, player)
    {
        EntityPlayer *target = self->target;
        if (target) {
            if (abs(player->position.x - self->position.x) < abs(target->position.x - self->position.x))
                self->target = player;
        }
        else {
            self->target = player;
        }

        if (Player_CheckBadnikTouch(player, self, &Wisp->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Wisp_CheckOffScreen(void)
{
    RSDK_THIS(Wisp);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        Wisp_Create(NULL);
    }
}

void Wisp_State_Init(void)
{
    RSDK_THIS(Wisp);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = 0;
    self->velocity.y = 0;

    self->state = Wisp_State_Idle;
    Wisp_State_Idle();
}

void Wisp_State_Idle(void)
{
    RSDK_THIS(Wisp);

    if (!--self->timer) {
        if (--self->buzzCount) {
            self->velocity.y = -0x10000;
            self->timer      = 96;
            self->state      = Wisp_State_FlyTowardTarget;
        }
        else {
            self->velocity.x = -0x20000;
            self->velocity.y = -0x20000;
            self->state      = Wisp_State_FlyAway;
        }
    }

    RSDK.ProcessAnimation(&self->wingAnimator);

    Wisp_HandlePlayerInteractions();
    Wisp_CheckOffScreen();
}

void Wisp_State_FlyTowardTarget(void)
{
    RSDK_THIS(Wisp);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityPlayer *target = self->target;
    if (target) {
        if (target->position.x >= self->position.x) {
            self->velocity.x += 0x1000;
            self->direction = FLIP_X;
            if (self->velocity.x > 0x20000)
                self->velocity.x = 0x20000;
        }
        else {
            self->velocity.x -= 0x1000;
            self->direction = FLIP_NONE;
            if (self->velocity.x < -0x20000)
                self->velocity.x = -0x20000;
        }

        if (target->position.y >= self->position.y) {
            self->velocity.y += 0x1000;
            if (self->velocity.y > 0x20000)
                self->velocity.y = 0x20000;
        }
        else {
            self->velocity.y -= 0x1000;
            if (self->velocity.y < -0x20000)
                self->velocity.y = -0x20000;
        }
    }

    if (!--self->timer) {
        self->timer      = RSDK.Rand(0, 32);
        self->state      = Wisp_State_Idle;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->direction  = FLIP_NONE;
    }
    RSDK.ProcessAnimation(&self->wingAnimator);

    Wisp_HandlePlayerInteractions();
    Wisp_CheckOffScreen();
}

void Wisp_State_FlyAway(void)
{
    RSDK_THIS(Wisp);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    RSDK.ProcessAnimation(&self->wingAnimator);

    Wisp_HandlePlayerInteractions();
    Wisp_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Wisp_EditorDraw(void) { Wisp_Draw(); }

void Wisp_EditorLoad(void)
{
    Wisp->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/Wisp.bin", SCOPE_STAGE);

    // despite being editable, this variable is set to FLIP_NONE on create, thereby making the editable var's value unused
    RSDK_ACTIVE_VAR(Wisp, direction);
    RSDK_ENUM_VAR("(Unused)", FLIP_NONE);
    // RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    // RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Wisp_Serialize(void) { RSDK_EDITABLE_VAR(Wisp, VAR_UINT8, direction); }
