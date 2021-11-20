#include "SonicMania.h"

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
    self->drawOrder = Zone->drawOrderLow;
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
        self->state = Crabmeat_State_Setup;
    }
}

void Crabmeat_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Crabmeat->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Crabmeat.bin", SCOPE_STAGE);
    Crabmeat->hitboxBadnik.left             = -14;
    Crabmeat->hitboxBadnik.top              = -14;
    Crabmeat->hitboxBadnik.right            = 14;
    Crabmeat->hitboxBadnik.bottom           = 14;
    Crabmeat->projectileHitbox.left   = -6;
    Crabmeat->projectileHitbox.top    = -6;
    Crabmeat->projectileHitbox.right  = 6;
    Crabmeat->projectileHitbox.bottom = 6;

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

void Crabmeat_CheckOnScreen(void)
{
    RSDK_THIS(Crabmeat);
    if (self->tileCollisions && !RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
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
            Player_CheckBadnikBreak((Entity *)self, player, true);
    }
}

void Crabmeat_State_Setup(void)
{
    RSDK_THIS(Crabmeat);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x8000;
    RSDK.SetSpriteAnimation(Crabmeat->aniFrames, 1, &self->animator, true, 0);
    self->state = Crabmeat_State_Main;
    Crabmeat_State_Main();
}

void Crabmeat_State_Main(void)
{
    RSDK_THIS(Crabmeat);

    self->position.x += self->velocity.x;
    if (self->timer >= 128
        || (!RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, 0, ((self->velocity.x >> 31) & 0xFFE40000) + 0xE0000, 0xF0000, 8))) {
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
    Crabmeat_CheckOnScreen();
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
                self->state      = Crabmeat_State_Main;
                break;

            case 1:
                self->shootState = 2;

                EntityCrabmeat *projectile = CREATE_ENTITY(Crabmeat, intToVoid(true), self->position.x - 0x100000, self->position.y);
                projectile->velocity.x = -0x10000;
                projectile->velocity.y = -0x40000;

                projectile = CREATE_ENTITY(Crabmeat, intToVoid(true), self->position.x + 0x100000, self->position.y);
                projectile->velocity.x = 0x10000;
                projectile->velocity.y = -0x40000;
                break;
        }
    }

    RSDK.ProcessAnimation(&self->animator);
    Crabmeat_CheckPlayerCollisions();
    Crabmeat_CheckOnScreen();
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
            if (Player_CheckCollisionTouch(player, self, &Crabmeat->projectileHitbox))
                Player_CheckProjectileHit(player, self);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Crabmeat_EditorDraw(void) { Crabmeat_Draw(); }

void Crabmeat_EditorLoad(void) { Crabmeat->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Crabmeat.bin", SCOPE_STAGE); }
#endif

void Crabmeat_Serialize(void) {}
