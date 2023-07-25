// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TurretSwitch Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTurretSwitch *TurretSwitch;

void TurretSwitch_Update(void)
{
    RSDK_THIS(TurretSwitch);

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);
}

void TurretSwitch_LateUpdate(void) {}

void TurretSwitch_StaticUpdate(void) {}

void TurretSwitch_Draw(void)
{
    RSDK_THIS(TurretSwitch);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TurretSwitch_Create(void *data)
{
    RSDK_THIS(TurretSwitch);

    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (VOID_TO_INT(data) == true) {
        RSDK.SetSpriteAnimation(TurretSwitch->aniFrames, 1, &self->animator, true, 0);
        self->state     = TurretSwitch_State_Projectile;
        self->drawGroup = Zone->objectDrawGroup[0];
    }
    else {
        self->hitbox.left   = -12;
        self->hitbox.top    = -12;
        self->hitbox.right  = 12;
        self->hitbox.bottom = 12;

        self->hitboxRange.left   = -12;
        self->hitboxRange.top    = -140;
        self->hitboxRange.right  = 140;
        self->hitboxRange.bottom = 140;

        self->state     = TurretSwitch_State_Init;
        self->drawGroup = Zone->objectDrawGroup[1];
    }
}

void TurretSwitch_StageLoad(void)
{
    TurretSwitch->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/TurretSwitch.bin", SCOPE_STAGE);

    TurretSwitch->hitboxProjectile.left   = -3;
    TurretSwitch->hitboxProjectile.top    = -3;
    TurretSwitch->hitboxProjectile.right  = 3;
    TurretSwitch->hitboxProjectile.bottom = 3;

    TurretSwitch->sfxShot = RSDK.GetSfx("Stage/Shot.wav");
}

void TurretSwitch_CheckPlayerCollisions(void)
{
    RSDK_THIS(TurretSwitch);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &ItemBox->hitboxItemBox)) {
            if (Player_CheckAttackingNoInvTimer(player, self) || player->state == Ice_PlayerState_Frozen) {
                TurretSwitch_Break(self, player);
                foreach_break;
            }
            else {
#if MANIA_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->animator.animationID != ANI_CROUCH)
#endif
                    Player_Hurt(player, self);
                foreach_break;
            }
        }
    }
}

void TurretSwitch_Break(EntityTurretSwitch *turret, EntityPlayer *player)
{
    player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
    CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ITEMBOX), turret->position.x, turret->position.y)->drawGroup = Zone->objectDrawGroup[1];

    RSDK.PlaySfx(ItemBox->sfxDestroy, false, 255);

    turret->visible         = false;
    turret->state           = StateMachine_None;
    turret->currentlyActive = true;
    turret->wasActivated    = true;
    turret->down            = true;
    turret->toggled         = true;
    turret->activated       = true;
}

void TurretSwitch_State_Init(void)
{
    RSDK_THIS(TurretSwitch);

    RSDK.SetSpriteAnimation(TurretSwitch->aniFrames, 0, &self->animator, true, 0);
    self->state = TurretSwitch_State_Turret;
}

void TurretSwitch_State_Turret(void)
{
    RSDK_THIS(TurretSwitch);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
            if (!self->timer) {
                EntityTurretSwitch *projectile = CREATE_ENTITY(TurretSwitch, INT_TO_VOID(true), self->position.x, self->position.y);
                projectile->velocity.y         = 0x20000;

                if (self->direction == FLIP_NONE) {
                    projectile->position.x += 0x80000;
                    projectile->velocity.x = 0x20000;
                }
                else {
                    projectile->position.x -= 0x80000;
                    projectile->velocity.x = -0x20000;
                }

                projectile->position.y += 0x80000;
                RSDK.PlaySfx(TurretSwitch->sfxShot, false, 255);

                self->timer = 40;
            }
            foreach_break;
        }
    }

    if (self->timer > 0)
        self->timer--;

    TurretSwitch_CheckPlayerCollisions();
}

void TurretSwitch_State_Projectile(void)
{
    RSDK_THIS(TurretSwitch);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }
    else {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, TurretSwitch->hitboxProjectile.bottom << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, TurretSwitch->hitboxProjectile.right << 13, 0, 4)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, TurretSwitch->hitboxProjectile.top << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, TurretSwitch->hitboxProjectile.left << 13, 0, 4)) {
            destroyEntity(self);
        }
        else {
            self->position.x += self->velocity.x;
            self->position.y += self->velocity.y;

            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &TurretSwitch->hitboxProjectile))
                    Player_ProjectileHurt(player, self);
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void TurretSwitch_EditorDraw(void)
{
    RSDK_THIS(TurretSwitch);

    RSDK.SetSpriteAnimation(TurretSwitch->aniFrames, 0, &self->animator, false, 0);

    TurretSwitch_Draw();
}

void TurretSwitch_EditorLoad(void)
{
    TurretSwitch->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/TurretSwitch.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TurretSwitch, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void TurretSwitch_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurretSwitch, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(TurretSwitch, VAR_UINT8, direction);
}
