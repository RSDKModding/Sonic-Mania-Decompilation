// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomMissile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    RSDK.DrawSprite(&self->animator0, NULL, false);
    RSDK.DrawSprite(&self->animator1, &self->drawPos, false);

    if (Zone->timer & 0x10)
        RSDK.DrawSprite(&self->animator3, &self->drawPos, false);
    else
        RSDK.DrawSprite(&self->animator2, &self->drawPos, false);
}

void PhantomMissile_Create(void *data)
{
    RSDK_THIS(PhantomMissile);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->drawFX        = FX_ROTATE;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->state         = PhantomMissile_Unknown3;
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &self->animator0, true, 0);
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

void PhantomMissile_Unknown1(void)
{
    RSDK_THIS(PhantomMissile);

    int distance = 0x7FFFFFFF;
    for (int p = SLOT_PLAYER1; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (player->sidekick == true && player->stateInput != Player_ProcessP2Input_Player) {
            int storeX         = self->position.x;
            int storeY         = self->position.y;
            self->position.x = self->drawPos.x;
            self->position.y = self->drawPos.y;

            while (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 16)) {
                self->position.y += 0x100000;
            }

            int y              = self->position.y;
            self->position.x = storeX;
            self->position.y = storeY;
            self->drawPos.y  = y;
            foreach_break;
        }

        int rx   = (self->position.x - player->position.x) >> 16;
        int ry   = (self->position.y - player->position.y) >> 16;
        int dist = rx * rx + ry * ry;
        if (dist < distance) {
            distance          = dist;
            self->drawPos.x = player->position.x;
            self->drawPos.y = player->position.y;
        }
    }
}

void PhantomMissile_Unknown2(void)
{
    RSDK_THIS(PhantomMissile);

    if (!(Zone->timer & 3)) {
        int x            = RSDK.Sin512(self->rotation) << 11;
        int y            = RSDK.Cos512(self->rotation) << 11;
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, self->position.x - x, self->position.y + y);
        dust->state      = Dust_State_Move;
        dust->drawOrder  = Zone->drawOrderHigh;
        dust->inkEffect  = INK_BLEND;
    }
}

void PhantomMissile_Unknown3(void)
{
    RSDK_THIS(PhantomMissile);

    if (self->parent) {
        self->position.x = self->parent->position.x;
        self->position.y = self->parent->position.y;
        self->field_60   = (self->field_60 + 6) & 0xFF;
        int off            = 8 * RSDK.Sin256(self->field_60) + 0x2000;

        self->position.x += off * RSDK.Sin256(self->angle);
        self->position.y -= off * RSDK.Cos256(self->angle);
        self->rotation = 2 * self->angle;

        if (self->timer > 0) {
            if (!--self->timer) {
                self->velocity.x = 0x300 * RSDK.Sin256(self->angle) + self->parent->velocity.x;
                self->velocity.y = -0x300 * RSDK.Cos256(self->angle) + self->parent->velocity.y;
                self->timer      = 8;
                self->state      = PhantomMissile_Unknown4;
            }
        }
    }
}

void PhantomMissile_Unknown4(void)
{
    RSDK_THIS(PhantomMissile);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (!--self->timer) {
        self->field_64 = 0x400;
        self->state    = PhantomMissile_Unknown5;
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);
    }
}

void PhantomMissile_Unknown5(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.ProcessAnimation(&self->animator0);
    if (self->field_64 < 0x600)
        self->field_64 += 4;

    self->position.x += self->field_64 * RSDK.Sin256(self->angle);
    self->position.y -= self->field_64 * RSDK.Cos256(self->angle);

    if (++self->timer == 24) {
        self->timer = 32;
        self->field_64 >>= 1;
        PhantomMissile_Unknown1();
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 2, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 3, &self->animator3, true, self->id);
        self->drawOrder = Zone->drawOrderHigh;
        self->state     = PhantomMissile_Unknown6;
    }
    PhantomMissile_Unknown2();
}

void PhantomMissile_Unknown6(void)
{
    RSDK_THIS(PhantomMissile);

    RSDK.ProcessAnimation(&self->animator0);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->field_64 < 0x800)
        self->field_64 += 4;
    if (self->timer > 4)
        self->timer--;

    int shift = self->timer >> 2;
    int rx    = (self->drawPos.x - self->position.x) >> 16;
    int ry    = (self->drawPos.y - self->position.y) >> 16;
    int angle = RSDK.ATan2(-ry, rx);
    int rot   = 2 * angle - self->rotation;

    if (abs(rot) >= abs(rot - 0x200)) {
        if (abs(rot - 0x200) < abs(rot + 0x200)) {
            self->rotation += ((rot - 0x200) >> shift);
        }
        else {
            self->rotation += ((rot + 0x200) >> shift);
        }
    }
    else {
        if (abs(rot) < abs(rot + 0x200)) {
            self->rotation += (rot >> shift);
        }
        else {
            self->rotation += ((rot + 0x200) >> shift);
        }
    }

    self->rotation &= 0x1FF;
    self->position.x += self->field_64 * RSDK.Sin512(self->rotation);
    self->position.y -= self->field_64 * RSDK.Cos512(self->rotation);
    if (rx * rx + ry * ry < 64) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->drawPos.x, self->drawPos.y - 0x80000)->drawOrder = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &self->animator0, true, 0);
        RSDK.SetSpriteAnimation(-1, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->animator3, true, 0);
        self->timer     = 0;
        self->field_68  = 0;
        self->drawOrder = Zone->drawOrderLow;
        self->visible   = 0;
        self->state     = PhantomMissile_Unknown7;
        Camera_ShakeScreen(0, 0, 3);
        RSDK.PlaySfx(PhantomEgg->sfxMissile, false, 255);
    }
    PhantomMissile_Unknown2();
}

void PhantomMissile_Unknown7(void)
{
    RSDK_THIS(PhantomMissile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &PhantomMissile->hitbox)) {
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }
    if (++self->timer >= 16) {
        if (self->parent) {
            self->position.x = self->parent->position.x;
            self->position.y = self->parent->position.y;
            self->timer      = 0;
            self->visible    = true;
            self->state      = PhantomMissile_Unknown8;
        }
    }
}

void PhantomMissile_Unknown8(void)
{
    RSDK_THIS(PhantomMissile);

    if (self->parent) {
        self->position.x = self->parent->position.x;
        self->position.y = self->parent->position.y;

        if (self->field_68 >= 0x2000) {
            self->field_60 = 0;
            self->state    = PhantomMissile_Unknown3;
        }
        else {
            self->field_68 += 0x100;
        }

        self->position.x += self->field_68 * RSDK.Sin256(self->angle);
        self->position.y -= self->field_68 * RSDK.Cos256(self->angle);
        self->rotation = (2 * self->angle);
    }
}

void PhantomMissile_Unknown9(void)
{
    RSDK_THIS(PhantomMissile);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->rotation = (self->rotation + self->groundVel) & 0x1FF;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void PhantomMissile_EditorDraw(void)
{
    RSDK_THIS(PhantomMissile);
    RSDK.SetSpriteAnimation(PhantomMissile->aniFrames, 0, &self->animator0, false, 0);

    RSDK.DrawSprite(&self->animator0, NULL, false);
}

void PhantomMissile_EditorLoad(void) { PhantomMissile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/EggMissile.bin", SCOPE_STAGE); }
#endif

void PhantomMissile_Serialize(void) {}
