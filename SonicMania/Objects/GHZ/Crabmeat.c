// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Crabmeat Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCrabmeat *Crabmeat;

void Crabmeat_Update(void)
{
    RSDK_THIS(Crabmeat);
    StateMachine_Run(self->state);
}

void Crabmeat_LateUpdate(void) {}

void Crabmeat_StaticUpdate(void) {}

void Crabmeat_Draw(void)
{
    RSDK_THIS(Crabmeat);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Crabmeat_Create(void *data)
{
    RSDK_THIS(Crabmeat);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (data) {
        self->inkEffect     = INK_ADD;
        self->alpha         = 0xC0;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 3, &self->animator, true, 0);
        self->state = Crabmeat_State_Projectile;
    }
    else {
        self->drawFX |= FX_FLIP;
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->startDir      = self->direction;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 0, &self->animator, true, 0);
        self->state = Crabmeat_State_Init;
    }
}

void Crabmeat_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Crabmeat->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Crabmeat.bin", SCOPE_STAGE);

    Crabmeat->hitboxBadnik.left   = -14;
    Crabmeat->hitboxBadnik.top    = -14;
    Crabmeat->hitboxBadnik.right  = 14;
    Crabmeat->hitboxBadnik.bottom = 14;

    Crabmeat->hitboxProjectile.left   = -6;
    Crabmeat->hitboxProjectile.top    = -6;
    Crabmeat->hitboxProjectile.right  = 6;
    Crabmeat->hitboxProjectile.bottom = 6;

    DEBUGMODE_ADD_OBJ(Crabmeat);
}

void Crabmeat_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}
void Crabmeat_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Crabmeat, NULL, self->position.x, self->position.y);
}

void Crabmeat_CheckOffScreen(void)
{
    RSDK_THIS(Crabmeat);

    if (self->tileCollisions != TILECOLLISION_NONE && !RSDK.CheckOnScreen(self, NULL)
        && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        self->timer      = 0;
        Crabmeat_Create(NULL);
    }
}

void Crabmeat_CheckPlayerCollisions(void)
{
    RSDK_THIS(Crabmeat);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Crabmeat->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Crabmeat_State_Init(void)
{
    RSDK_THIS(Crabmeat);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x8000;
    RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 1, &self->animator, true, 0);
    self->state = Crabmeat_State_Moving;
    Crabmeat_State_Moving();
}

void Crabmeat_State_Moving(void)
{
    RSDK_THIS(Crabmeat);

    self->position.x += self->velocity.x;

    if (self->timer >= 128
        || (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0xE0000 * (self->velocity.x > 0 ? 1 : -1), 0xF0000, 8))) {
        self->timer = 0;
        if (self->shootState == 0)
            RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 0, &self->animator, true, 0);
        else
            RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 2, &self->animator, true, 0);
        self->state = Crabmeat_State_Shoot;
    }
    else {
        self->timer++;
    }

    RSDK.ProcessAnimation(&self->animator);

    Crabmeat_CheckPlayerCollisions();
    Crabmeat_CheckOffScreen();
}

void Crabmeat_State_Shoot(void)
{
    RSDK_THIS(Crabmeat);

    if (++self->timer >= 60) {
        switch (self->shootState) {
            default:
            case 2:
                if (self->animator.frameID != self->animator.frameCount - 1)
                    break;

            case 0:
                self->shootState = 1;
                RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 1, &self->animator, true, 0);
                self->direction ^= FLIP_X;
                self->velocity.x = -self->velocity.x;
                self->timer      = 0;
                self->state      = Crabmeat_State_Moving;
                break;

            case 1:
                self->shootState = 2;

                EntityCrabmeat *projectile = CREATE_ENTITY(Crabmeat, INT_TO_VOID(true), self->position.x - 0x100000, self->position.y);
                projectile->velocity.x     = -0x10000;
                projectile->velocity.y     = -0x40000;

                projectile             = CREATE_ENTITY(Crabmeat, INT_TO_VOID(true), self->position.x + 0x100000, self->position.y);
                projectile->velocity.x = 0x10000;
                projectile->velocity.y = -0x40000;
                break;
        }
    }

    RSDK.ProcessAnimation(&self->animator);
    Crabmeat_CheckPlayerCollisions();
    Crabmeat_CheckOffScreen();
}

void Crabmeat_State_Projectile(void)
{
    RSDK_THIS(Crabmeat);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }
    else {
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Crabmeat->hitboxProjectile))
                Player_ProjectileHurt(player, self);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Crabmeat_EditorDraw(void) { Crabmeat_Draw(); }

void Crabmeat_EditorLoad(void) { Crabmeat->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Crabmeat.bin", SCOPE_STAGE); }
#endif

void Crabmeat_Serialize(void) {}
