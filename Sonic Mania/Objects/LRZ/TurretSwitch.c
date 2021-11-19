#include "SonicMania.h"

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

    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (voidToInt(data) == 1) {
        RSDK.SetSpriteAnimation(TurretSwitch->aniFrames, 1, &self->animator, true, 0);
        self->state     = TurretSwitch_State_Projectile;
        self->drawOrder = Zone->drawOrderLow;
    }
    else {
        self->hitbox.left        = -12;
        self->hitbox.top         = -12;
        self->hitbox.right       = 12;
        self->hitbox.bottom      = 12;
        self->hitboxRange.left   = -12;
        self->hitboxRange.top    = -140;
        self->hitboxRange.right  = 140;
        self->hitboxRange.bottom = 140;
        self->state              = TurretSwitch_State_Setup;
        self->drawOrder          = Zone->drawOrderHigh;
    }
}

void TurretSwitch_StageLoad(void)
{
    TurretSwitch->aniFrames               = RSDK.LoadSpriteAnimation("LRZ1/TurretSwitch.bin", SCOPE_STAGE);
    TurretSwitch->hitboxProjectile.left   = -3;
    TurretSwitch->hitboxProjectile.top    = -3;
    TurretSwitch->hitboxProjectile.right  = 3;
    TurretSwitch->hitboxProjectile.bottom = 3;
    TurretSwitch->sfxShot                 = RSDK.GetSFX("Stage/Shot.wav");
}

void TurretSwitch_CheckPlayerCollisions(void)
{
    RSDK_THIS(TurretSwitch);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &ItemBox->hitbox)) {
            if (Player_CheckAttacking(player, self) || player->state == Ice_State_FrozenPlayer) {
                TurretSwitch_Break(self, player);
                foreach_break;
            }
            else {
#if RETRO_USE_PLUS
                if (player->characterID != ID_MIGHTY || player->animator.animationID != ANI_CROUCH)
#endif
                    Player_CheckHit(player, self);
                foreach_break;
            }
        }
    }
}

void TurretSwitch_Break(EntityTurretSwitch *self, EntityPlayer *player)
{
    player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
    CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
    RSDK.PlaySfx(ItemBox->sfxDestroy, false, 255);
    self->visible         = 0;
    self->state           = 0;
    self->currentlyActive = true;
    self->wasActivated    = true;
    self->down            = true;
    self->toggled         = true;
    self->activated       = true;
}

void TurretSwitch_State_Setup(void)
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
                EntityTurretSwitch *projectile = CREATE_ENTITY(TurretSwitch, intToVoid(1), self->position.x, self->position.y);
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
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, TurretSwitch->hitboxProjectile.bottom << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, TurretSwitch->hitboxProjectile.right << 13, 0, 4)
            || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, TurretSwitch->hitboxProjectile.top << 13, 4)
            || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, TurretSwitch->hitboxProjectile.left << 13, 0, 4)) {
            destroyEntity(self);
        }
        else {
            self->position.x += self->velocity.x;
            self->position.y += self->velocity.y;

            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &TurretSwitch->hitboxProjectile))
                    Player_CheckProjectileHit(player, self);
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
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
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void TurretSwitch_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurretSwitch, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(TurretSwitch, VAR_UINT8, direction);
}
