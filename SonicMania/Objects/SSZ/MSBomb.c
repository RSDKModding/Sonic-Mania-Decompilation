// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSBomb Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectMSBomb *MSBomb;

void MSBomb_Update(void)
{
    RSDK_THIS(MSBomb);

    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);
}

void MSBomb_LateUpdate(void) {}

void MSBomb_StaticUpdate(void) {}

void MSBomb_Draw(void)
{
    RSDK_THIS(MSBomb);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MSBomb_Create(void *data)
{
    RSDK_THIS(MSBomb);

    self->active    = ACTIVE_NORMAL;
    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (data) {
        RSDK.SetSpriteAnimation(MSBomb->aniFrames, 1, &self->animator, true, 0);

        self->hitbox.left   = -4;
        self->hitbox.top    = -4;
        self->hitbox.right  = 4;
        self->hitbox.bottom = 4;

        self->state = MSBomb_State_Projectile;
    }
    else {
        RSDK.SetSpriteAnimation(MSBomb->aniFrames, 0, &self->animator, true, 0);

        self->hitbox.left   = -8;
        self->hitbox.top    = -8;
        self->hitbox.right  = 8;
        self->hitbox.bottom = 8;

        self->drawFX  = FX_SCALE;
        self->scale.x = 0xC0;
        self->scale.y = 0xC0;
        self->state   = MSBomb_State_EnterBomb;
    }
}

void MSBomb_StageLoad(void)
{
    MSBomb->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSBomb.bin", SCOPE_STAGE);

    MSBomb->sfxExplosion = RSDK.GetSfx("Stage/Explosion4.wav");
}

void MSBomb_State_EnterBomb(void)
{
    RSDK_THIS(MSBomb);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->scale.x >= 0x200) {
        self->drawFX = FX_NONE;
        self->state  = MSBomb_State_Bouncing;
    }
    else {
        self->scale.x += 0x40;
        self->scale.y = self->scale.x;
    }
}

// Not sure why this is part of MSBomb but alright
void MSBomb_State_SilverSonicExplode(void)
{
    RSDK_THIS(SilverSonic);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->scale.x >= 0x200) {
        int32 yVel = 0x18000;
        int32 xVel = 0x30000;

        foreach_active(MetalSonic, metal)
        {
            xVel = MAX(abs(self->position.x - metal->position.x) >> 6, 0x20000);
            yVel = abs(self->position.y - metal->position.y) >> 6;
        }

        EntityMSBomb *bomb = CREATE_ENTITY(MSBomb, INT_TO_VOID(true), self->position.x, self->position.y);
        bomb->velocity.x   = -xVel;
        bomb->velocity.y   = -yVel;

        bomb             = CREATE_ENTITY(MSBomb, INT_TO_VOID(true), self->position.x, self->position.y);
        bomb->velocity.x = xVel;
        bomb->velocity.y = -yVel;

        bomb             = CREATE_ENTITY(MSBomb, INT_TO_VOID(true), self->position.x, self->position.y);
        yVel             = -(yVel >> 1);
        bomb->velocity.x = -xVel;
        bomb->velocity.y = yVel;

        bomb             = CREATE_ENTITY(MSBomb, INT_TO_VOID(true), self->position.x, self->position.y);
        bomb->velocity.x = xVel;
        bomb->velocity.y = yVel;

        RSDK.PlaySfx(MSBomb->sfxExplosion, false, 0xFF);
        RSDK.ResetEntity(self, Explosion->classID, INT_TO_VOID(EXPLOSION_BOSS));
        self->position.x = bomb->position.x;
        self->position.y = bomb->position.y;
    }
    else {
        self->scale.x += 0x40;
        self->scale.y = self->scale.x;
    }
}

void MSBomb_State_Bouncing(void)
{
    RSDK_THIS(MSBomb);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xA0000, true)) {
        // Bounce
        self->velocity.y = -0x40000;

        if (self->timer > 0) {
            if (!--self->timer) {
                int32 storeX = self->position.x;
                int32 storeY = self->position.y;
                RSDK.ResetEntity(self, Explosion->classID, INT_TO_VOID(EXPLOSION_BOSS));
                self->position.x = storeX;
                self->position.y = storeY;
                RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
            }
        }
    }
    else if (!self->timer && !RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }

    if (self->classID == MSBomb->classID) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                if (Player_Hurt(player, self))
                    player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;

                CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y);
                RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);
                destroyEntity(self);
            }
        }
    }
}

void MSBomb_State_Projectile(void)
{
    RSDK_THIS(MSBomb);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            Player_Hurt(player, self);
        }
    }

    foreach_active(MetalSonic, metal)
    {
        if (RSDK.CheckObjectCollisionTouchBox(metal, metal->outerBox, self, &self->hitbox)) {
            MetalSonic->invincibilityTimerPanel = 16;

            if (--metal->health <= 0) {
                metal->timer = 0;
                metal->state = MetalSonic_State_PanelExplosion;
            }

            RSDK.PlaySfx(MetalSonic->sfxHit, false, 255);
            destroyEntity(self);
            foreach_break;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void MSBomb_EditorDraw(void)
{
    RSDK_THIS(MSBomb);
    RSDK.SetSpriteAnimation(MSBomb->aniFrames, 0, &self->animator, true, 0);

    MSBomb_Draw();
}

void MSBomb_EditorLoad(void) { MSBomb->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MSBomb.bin", SCOPE_STAGE); }
#endif

void MSBomb_Serialize(void) {}
#endif
