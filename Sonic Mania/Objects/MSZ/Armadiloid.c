// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Armadiloid Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectArmadiloid *Armadiloid;

void Armadiloid_Update(void)
{
    RSDK_THIS(Armadiloid);
    StateMachine_Run(self->state);
}

void Armadiloid_LateUpdate(void) {}

void Armadiloid_StaticUpdate(void) {}

void Armadiloid_Draw(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Armadiloid_Create(void *data)
{
    RSDK_THIS(Armadiloid);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (data)
        self->type = 1;
    if (self->type) {
        self->hitbox.left   = -10;
        self->hitbox.top    = -40;
        self->hitbox.right  = 10;
        self->hitbox.bottom = -20;
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 4, &self->animator1, true, 0);
    }
    else {
        self->hitbox.left   = -24;
        self->hitbox.top    = -15;
        self->hitbox.right  = 24;
        self->hitbox.bottom = -7;
        self->velocity.x    = 0x8000;
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 3, &self->animator3, true, 0);
    }
    self->state = Armadiloid_State_Setup;
}

void Armadiloid_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        Armadiloid->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Armadiloid.bin", SCOPE_STAGE);

    DEBUGMODE_ADD_OBJ(Armadiloid);

    Armadiloid->sfxShot = RSDK.GetSfx("Stage/Shot.wav");
}
void Armadiloid_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Armadiloid, NULL, self->position.x, self->position.y);
}

void Armadiloid_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Armadiloid_State_Setup(void)
{
    RSDK_THIS(Armadiloid);

    foreach_active(Armadiloid, armadiloid)
    {
        if (armadiloid != self) {
            if (abs(self->position.x - armadiloid->position.x) < 0x300000) {
                if (abs(self->position.y - armadiloid->position.y) < 0x300000) {
                    if (self->type) {
                        if (armadiloid->type != self->type) {
                            self->child = (Entity*)armadiloid;
                            self->state = Armadiloid_State_Rider;
                            foreach_break;
                        }
                    }
                    else if (armadiloid->type) {
                        self->parent = (Entity *)armadiloid;
                        self->state  = Armadiloid_State_PlatformFlying;
                        foreach_break;
                    }
                }
            }
        }
    }
}

void Armadiloid_State_PlatformFlying(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&self->animator3);
    self->position.x += self->velocity.x;
    if (self->parent) {
        EntityPlayer *player = Player_GetNearestPlayerX();
        if (abs(self->position.x - player->position.x) < 0x800000) {
            RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 2, &self->animator2, true, 0);
            self->state = Armadiloid_PlatformShootDelay;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, &self->hitbox))
            player->position.x += self->velocity.x;
    }
}

void Armadiloid_PlatformShootDelay(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&self->animator3);
    self->position.x += self->velocity.x;

    if (self->parent) {
        if (++self->timer == 12) {
            self->timer = 0;
            self->state = Armadiloid_State_PlatformShoot;
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionPlatform(player, self, &self->hitbox))
                player->position.x += self->velocity.x;
        }
    }
    else {
        self->timer = 0;
        self->state = Armadiloid_State_PlatformFlying;
    }
}

void Armadiloid_State_PlatformShoot(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator2);
    self->position.x += self->velocity.x;

    if (self->animator2.frameID != 4 || self->timer) {
        if (self->animator2.frameID == self->animator2.frameCount - 1) {
            self->timer = 0;
            RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 2, &self->animator2, true, 0);
            self->state = Armadiloid_State_PlatformFlying;
        }
    }
    else {
        self->timer = 1;
        RSDK.PlaySfx(Armadiloid->sfxShot, false, 255);
        EntityProjectile *projectile = CREATE_ENTITY(Projectile, Projectile_State_Move, self->position.x - 0x120000, self->position.y + 0x90000);
        projectile->velocity.x       = -0x18000;
        projectile->drawOrder        = Zone->drawOrderLow;
        projectile->hitbox.left      = -4;
        projectile->hitbox.top       = -4;
        projectile->hitbox.right     = 4;
        projectile->hitbox.bottom    = 4;
        projectile->type             = PROJECTILE_BASIC2;
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 5, &projectile->animator, true, 0);
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, &self->hitbox))
            player->position.x += self->velocity.x;
    }
}

void Armadiloid_State_Rider(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&self->animator1);

    EntityArmadiloid *child = (EntityArmadiloid *)self->child;
    if (child) {
        self->position.x = child->position.x;
        self->position.y = child->position.y;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBadnikBreak(self, player, true)) {
            child->parent = NULL;
        }
    }
}

void Armadiloid_EditorDraw(void)
{
    RSDK_THIS(Armadiloid);

    if (self->type) {
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 4, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 3, &self->animator3, true, 0);
    }
    else {
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 3, &self->animator3, true, 0);
    }

    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Armadiloid_EditorLoad(void) { Armadiloid->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Armadiloid.bin", SCOPE_STAGE); }

void Armadiloid_Serialize(void) { RSDK_EDITABLE_VAR(Armadiloid, VAR_ENUM, type); }
