// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomMissile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPhantomMissile *PhantomMissile;

void PhantomMissile_Update(void) {}

void PhantomMissile_LateUpdate(void)
{
    RSDK_THIS(PhantomMissile);

    StateMachine_Run(self->state);
}

void PhantomMissile_StaticUpdate(void) {}

void PhantomMissile_Draw(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.DrawSprite(&self->missileAnimator, NULL, false);
    RSDK.DrawSprite(&self->targetOutsideAnimator, &self->targetPos, false);

    if (Zone->timer & 0x10)
        RSDK.DrawSprite(&self->targetNumbersAnimator, &self->targetPos, false);
    else
        RSDK.DrawSprite(&self->targetInsideAnimator, &self->targetPos, false);
}

void PhantomMissile_Create(void *data)
{
    RSDK_THIS(PhantomMissile);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->drawFX        = FX_ROTATE;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->state         = PhantomMissile_State_Attached;

        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &self->missileAnimator, true, 0);
    }
}

void PhantomMissile_StageLoad(void)
{
    PhantomMissile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EggMissile.bin", SCOPE_STAGE);

    PhantomMissile->hitbox.left   = -16;
    PhantomMissile->hitbox.top    = -12;
    PhantomMissile->hitbox.right  = 16;
    PhantomMissile->hitbox.bottom = 12;
}

void PhantomMissile_GetTargetPos(void)
{
    RSDK_THIS(PhantomMissile);

    int32 distance = 0x7FFFFFFF;

    for (int32 p = SLOT_PLAYER1; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

        if (player->sidekick == true && player->stateInput != Player_Input_P2_Player) {
            int32 storeX     = self->position.x;
            int32 storeY     = self->position.y;
            self->position.x = self->targetPos.x;
            self->position.y = self->targetPos.y;

            while (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, 16)) {
                self->position.y += 0x100000;
            }

            int32 y           = self->position.y;
            self->position.x  = storeX;
            self->position.y  = storeY;
            self->targetPos.y = y;
            foreach_break;
        }

        int32 rx   = (self->position.x - player->position.x) >> 16;
        int32 ry   = (self->position.y - player->position.y) >> 16;
        int32 dist = rx * rx + ry * ry;
        if (dist < distance) {
            distance          = dist;
            self->targetPos.x = player->position.x;
            self->targetPos.y = player->position.y;
        }
    }
}

void PhantomMissile_HandleExhaust(void)
{
    RSDK_THIS(PhantomMissile);

    if (!(Zone->timer & 3)) {
        int32 x          = self->position.x - (RSDK.Sin512(self->rotation) << 11);
        int32 y          = self->position.y + (RSDK.Cos512(self->rotation) << 11);
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, x, y);

        dust->state     = Dust_State_DustPuff;
        dust->drawGroup = Zone->objectDrawGroup[1];
        dust->inkEffect = INK_BLEND;
    }
}

void PhantomMissile_State_Attached(void)
{
    RSDK_THIS(PhantomMissile);

    if (self->parent) {
        self->position.x     = self->parent->position.x;
        self->position.y     = self->parent->position.y;
        self->oscillateAngle = (self->oscillateAngle + 6) & 0xFF;
        int32 off            = 8 * RSDK.Sin256(self->oscillateAngle) + 0x2000;

        self->position.x += off * RSDK.Sin256(self->angle);
        self->position.y -= off * RSDK.Cos256(self->angle);
        self->rotation = 2 * self->angle;

        if (self->timer > 0) {
            if (!--self->timer) {
                self->velocity.x = 0x300 * RSDK.Sin256(self->angle) + self->parent->velocity.x;
                self->velocity.y = -0x300 * RSDK.Cos256(self->angle) + self->parent->velocity.y;
                self->timer      = 8;
                self->state      = PhantomMissile_State_PrepareLaunch;
            }
        }
    }
}

void PhantomMissile_State_PrepareLaunch(void)
{
    RSDK_THIS(PhantomMissile);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!--self->timer) {
        self->targetRadius = 0x400;
        self->state        = PhantomMissile_State_Launched;
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);
    }
}

void PhantomMissile_State_Launched(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.ProcessAnimation(&self->missileAnimator);

    if (self->targetRadius < 0x600)
        self->targetRadius += 4;

    self->position.x += self->targetRadius * RSDK.Sin256(self->angle);
    self->position.y -= self->targetRadius * RSDK.Cos256(self->angle);

    if (++self->timer == 24) {
        self->timer = 32;
        self->targetRadius >>= 1;
        PhantomMissile_GetTargetPos();

        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 1, &self->targetOutsideAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 2, &self->targetInsideAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 3, &self->targetNumbersAnimator, true, self->id);

        self->drawGroup = Zone->objectDrawGroup[1];
        self->state     = PhantomMissile_State_Attacking;
    }

    PhantomMissile_HandleExhaust();
}

void PhantomMissile_State_Attacking(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.ProcessAnimation(&self->missileAnimator);
    RSDK.ProcessAnimation(&self->targetOutsideAnimator);
    RSDK.ProcessAnimation(&self->targetInsideAnimator);
    RSDK.ProcessAnimation(&self->targetNumbersAnimator);

    if (self->targetRadius < 0x800)
        self->targetRadius += 4;

    if (self->timer > 4)
        self->timer--;

    int32 shift = self->timer >> 2;
    int32 rx    = (self->targetPos.x - self->position.x) >> 16;
    int32 ry    = (self->targetPos.y - self->position.y) >> 16;
    int32 angle = RSDK.ATan2(-ry, rx);
    int32 rot   = 2 * angle - self->rotation;

    if (abs(rot) >= abs(rot - 0x200)) {
        if (abs(rot - 0x200) < abs(rot + 0x200))
            self->rotation += ((rot - 0x200) >> shift);
        else
            self->rotation += ((rot + 0x200) >> shift);
    }
    else {
        if (abs(rot) < abs(rot + 0x200))
            self->rotation += (rot >> shift);
        else
            self->rotation += ((rot + 0x200) >> shift);
    }

    self->rotation &= 0x1FF;

    self->position.x += self->targetRadius * RSDK.Sin512(self->rotation);
    self->position.y -= self->targetRadius * RSDK.Cos512(self->rotation);

    if (rx * rx + ry * ry < 64) {
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ITEMBOX), self->targetPos.x, self->targetPos.y - 0x80000)->drawGroup =
            Zone->objectDrawGroup[1];

        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &self->missileAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 1, &self->targetOutsideAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->targetInsideAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->targetNumbersAnimator, true, 0);

        self->timer          = 0;
        self->reattachRadius = 0;
        self->drawGroup      = Zone->objectDrawGroup[0];
        self->visible        = false;
        self->state          = PhantomMissile_State_Explode;

        Camera_ShakeScreen(0, 0, 3);
        RSDK.PlaySfx(PhantomEgg->sfxMissile, false, 255);
    }

    PhantomMissile_HandleExhaust();
}

void PhantomMissile_State_Explode(void)
{
    RSDK_THIS(PhantomMissile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &PhantomMissile->hitbox)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }

    if (++self->timer >= 16 && self->parent) {
        self->position.x = self->parent->position.x;
        self->position.y = self->parent->position.y;
        self->timer      = 0;
        self->visible    = true;
        self->state      = PhantomMissile_State_Reattach;
    }
}

void PhantomMissile_State_Reattach(void)
{
    RSDK_THIS(PhantomMissile);

    if (self->parent) {
        self->position.x = self->parent->position.x;
        self->position.y = self->parent->position.y;

        if (self->reattachRadius >= 0x2000) {
            self->oscillateAngle = 0;
            self->state          = PhantomMissile_State_Attached;
        }
        else {
            self->reattachRadius += 0x100;
        }

        self->position.x += self->reattachRadius * RSDK.Sin256(self->angle);
        self->position.y -= self->reattachRadius * RSDK.Cos256(self->angle);
        self->rotation = self->angle << 1;
    }
}

void PhantomMissile_State_Destroyed(void)
{
    RSDK_THIS(PhantomMissile);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->rotation = (self->rotation + self->groundVel) & 0x1FF;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void PhantomMissile_EditorDraw(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &self->missileAnimator, false, 0);

    RSDK.DrawSprite(&self->missileAnimator, NULL, false);
}

void PhantomMissile_EditorLoad(void) { PhantomMissile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EggMissile.bin", SCOPE_STAGE); }
#endif

void PhantomMissile_Serialize(void) {}
