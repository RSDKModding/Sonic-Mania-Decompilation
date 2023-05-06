// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Iwamodoki Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectIwamodoki *Iwamodoki;

void Iwamodoki_Update(void)
{
    RSDK_THIS(Iwamodoki);

    if (LRZConvItem && self->lrzConvPhys) {
        self->preMovePos.x = self->position.x;
        self->preMovePos.y = self->position.y;
        self->moveOffset   = LRZConvItem_HandleLRZConvPhys(self);
    }
    else {
        self->preMovePos.x = self->position.x;
        self->preMovePos.y = self->position.y;
        self->moveOffset.x = 0;
        self->moveOffset.y = 0;
    }

    self->postMovePos.x = self->position.x;
    self->postMovePos.y = self->position.y;

    StateMachine_Run(self->state);
}

void Iwamodoki_LateUpdate(void) {}

void Iwamodoki_StaticUpdate(void) {}

void Iwamodoki_Draw(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Iwamodoki_Create(void *data)
{
    RSDK_THIS(Iwamodoki);

    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawGroup     = Zone->objectDrawGroup[0];

    if (data) {
        self->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, VOID_TO_INT(data) + 2, &self->animator, true, 0);
        self->state = Iwamodoki_State_Debris;
    }
    else {
        RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 0, &self->animator, true, 0);
        self->state = Iwamodoki_State_Init;
    }
}

void Iwamodoki_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Iwamodoki.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Iwamodoki.bin", SCOPE_STAGE);

    Iwamodoki->hitboxBadnik.left   = -12;
    Iwamodoki->hitboxBadnik.top    = -11;
    Iwamodoki->hitboxBadnik.right  = 12;
    Iwamodoki->hitboxBadnik.bottom = 11;

    Iwamodoki->hitboxRange.left   = -64;
    Iwamodoki->hitboxRange.top    = -64;
    Iwamodoki->hitboxRange.right  = 64;
    Iwamodoki->hitboxRange.bottom = 64;

    Iwamodoki->hitboxProjectile.left   = -4;
    Iwamodoki->hitboxProjectile.top    = -4;
    Iwamodoki->hitboxProjectile.right  = 4;
    Iwamodoki->hitboxProjectile.bottom = 4;

    DEBUGMODE_ADD_OBJ(Iwamodoki);
}

void Iwamodoki_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Iwamodoki, NULL, self->position.x, self->position.y);
}

void Iwamodoki_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Iwamodoki_HandlePlayerCollisions(void)
{
    RSDK_THIS(Iwamodoki);

    self->position.x = self->preMovePos.x;
    self->position.y = self->preMovePos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &Iwamodoki->hitboxBadnik) == C_TOP) {
            player->position.x += self->moveOffset.x;
            player->position.y += self->moveOffset.y;
        }
    }

    self->position.x = self->postMovePos.x;
    self->position.y = self->postMovePos.y;
}

void Iwamodoki_CheckOffScreen(void)
{
    RSDK_THIS(Iwamodoki);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Iwamodoki_Create(NULL);
    }
}

void Iwamodoki_State_Init(void)
{
    RSDK_THIS(Iwamodoki);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;

    self->state = Iwamodoki_State_AwaitPlayer;
    Iwamodoki_State_AwaitPlayer();
}

void Iwamodoki_State_AwaitPlayer(void)
{
    RSDK_THIS(Iwamodoki);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Iwamodoki->hitboxRange)) {
            self->state = Iwamodoki_State_Appear;
        }
    }

    Iwamodoki_HandlePlayerCollisions();
    Iwamodoki_CheckOffScreen();
}

void Iwamodoki_State_Appear(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == 6) {
        RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 1, &self->animator, true, 0);
        self->chargeCount = 15;
        self->timer       = 15;
        self->state       = Iwamodoki_State_Charging;
    }

    Iwamodoki_HandlePlayerCollisions();
    Iwamodoki_CheckOffScreen();
}

void Iwamodoki_State_Charging(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&self->animator);

    if (!--self->timer) {
        if (!--self->chargeCount) {
            // Explode Anim
            self->timer = 32;
            RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 2, &self->animator, true, 0);
            self->state = Iwamodoki_State_Explode;
        }
        else {
            // Charging Anim
            RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 1, &self->animator, true, 0);
            self->timer = self->chargeCount;
        }
    }

    Iwamodoki_HandlePlayerCollisions();
    Iwamodoki_CheckOffScreen();
}

void Iwamodoki_State_Explode(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&self->animator);

    if (!--self->timer) {
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];

        if (self->onScreen == 1)
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

        EntityIwamodoki *debris = CREATE_ENTITY(Iwamodoki, INT_TO_VOID(1), self->position.x, self->position.y);
        debris->velocity.x      = -0x20000;
        debris->velocity.y      = -0x40000;

        debris             = CREATE_ENTITY(Iwamodoki, INT_TO_VOID(1), self->position.x, self->position.y);
        debris->velocity.x = 0x20000;
        debris->velocity.y = -0x40000;

        debris             = CREATE_ENTITY(Iwamodoki, INT_TO_VOID(2), self->position.x, self->position.y);
        debris->velocity.x = -0x40000;
        debris->velocity.y = -0x20000;

        debris             = CREATE_ENTITY(Iwamodoki, INT_TO_VOID(2), self->position.x, self->position.y);
        debris->velocity.x = 0x40000;
        debris->velocity.y = -0x20000;

        destroyEntity(self);
    }
    else {
        Iwamodoki_HandlePlayerCollisions();
        Iwamodoki_CheckOffScreen();
    }
}

void Iwamodoki_State_Debris(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Iwamodoki->hitboxProjectile)) {
            Player_ProjectileHurt(player, self);
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Iwamodoki_EditorDraw(void) { Iwamodoki_Draw(); }

void Iwamodoki_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Iwamodoki.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Iwamodoki.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Iwamodoki, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void Iwamodoki_Serialize(void)
{
    RSDK_EDITABLE_VAR(Iwamodoki, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Iwamodoki, VAR_BOOL, lrzConvPhys);
}
