// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PKingAttack Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPKingAttack *PKingAttack;

// NOTE:
// This object is likely highly based off LRZ/KingAttack
// hence why there's a bunch of "unused" stuff that resembles that object

void PKingAttack_Update(void)
{
    RSDK_THIS(PKingAttack);

    StateMachine_Run(self->state);
}

void PKingAttack_LateUpdate(void) {}

void PKingAttack_StaticUpdate(void) {}

void PKingAttack_Draw(void)
{
    RSDK_THIS(PKingAttack);

    if (self->type == PKINGATTACK_LAUNCHED)
        RSDK.DrawCircle(self->position.x, self->position.y, 0x20, 0x00, 0xFF, INK_TINT, false);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PKingAttack_Create(void *data)
{
    RSDK_THIS(PKingAttack);

    if (!SceneInfo->inEditor) {
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->type          = VOID_TO_INT(data);
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        switch (self->type) {
            default: break;
            case PKINGATTACK_LASER: break;

            case PKINGATTACK_ORBIT:
                self->drawFX  = FX_SCALE;
                self->visible = true;
                RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 10, &self->animator, true, RSDK.Rand(0, 6));
                self->state = PKingAttack_State_OrbitAppear;

                self->hitbox.left   = -10;
                self->hitbox.top    = -10;
                self->hitbox.right  = 10;
                self->hitbox.bottom = 10;
                break;

            case PKINGATTACK_TRAIL:
                self->drawGroup = Zone->objectDrawGroup[0];
                self->visible   = true;
                self->inkEffect = INK_ADD;
                self->alpha     = 0xC0;
                RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 12, &self->animator, true, 0);
                self->state = PKingAttack_State_Trail;
                break;

            case PKINGATTACK_LARGEBULLET: break;
            case PKINGATTACK_ENERGYLINE: break;

            case PKINGATTACK_SMALLBULLET:
                self->drawGroup = Zone->objectDrawGroup[0];
                self->drawFX    = FX_ROTATE;
                self->visible   = true;
                RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 11, &self->animator, true, 0);
                self->state = PKingAttack_State_SmallBullet;

                self->hitbox.left   = -4;
                self->hitbox.top    = -4;
                self->hitbox.right  = 4;
                self->hitbox.bottom = 4;
                break;
        }
    }
}

void PKingAttack_StageLoad(void)
{
    PKingAttack->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);

    PKingAttack->sfxPulse = RSDK.GetSfx("Stage/ElecPulse.wav");
}

void PKingAttack_CheckPlayerCollisions(void)
{
    RSDK_THIS(PKingAttack);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            if (player->superState == SUPERSTATE_SUPER) {
                if (!player->blinkTimer) {
                    int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

                    if (self->state == PKingAttack_State_OrbitLaunched) {
                        player->blinkTimer = 120;
                        Ring_LoseRings(player, MIN(player->rings, 8), player->collisionPlane);
                        player->rings -= MIN(player->rings, 8);
                        RSDK.PlaySfx(Player->sfxLoseRings, false, 255);
                    }
                    else {
                        RSDK.PlaySfx(PKingAttack->sfxPulse, false, 255);
                        destroyEntity(self);
                    }

                    player->rotation   = 0;
                    player->velocity.x = RSDK.Cos256(angle) << 10;
                    player->velocity.y = RSDK.Sin256(angle) << 10;
                    player->groundVel  = player->velocity.x;
                    player->state      = ERZStart_State_PlayerRebound;
                }
            }
            else {
                Player_Hurt(player, self);
            }
        }
    }
}

void PKingAttack_State_OrbitAppear(void)
{
    RSDK_THIS(PKingAttack);

    RSDK.ProcessAnimation(&self->animator);

    self->angle = (self->angle + 12) & 0x3FF;

    if (self->scale.x < 0x200) {
        self->scale.x += 0x20;
        self->scale.y = self->scale.x;
    }

    self->velocity.x = self->position.x;
    self->velocity.y = self->position.y;

    self->position.x = (RSDK.Cos1024(self->angle) << 12) + self->target->position.x;
    self->position.y = (RSDK.Sin1024(self->angle) << 12) + self->target->position.y;

    self->velocity.x = self->position.x - self->velocity.x;
    self->velocity.y = self->position.y - self->velocity.y;

    PKingAttack_CheckPlayerCollisions();

    if (++self->timer == 120) {
        self->timer       = 0;
        self->targetPos.x = self->target->position.x;
        self->targetPos.y = self->target->position.y;
        self->state       = PKingAttack_State_Orbiting;
    }
}

void PKingAttack_State_Orbiting(void)
{
    RSDK_THIS(PKingAttack);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->target->position.x - self->targetPos.x;
    self->position.y += self->target->position.y - self->targetPos.y;

    if (self->position.x <= self->target->position.x) {
        if (self->position.x < self->target->position.x)
            self->velocity.x += 0x4000;
    }
    else {
        self->velocity.x -= 0x4000;
    }

    if (self->position.y <= self->target->position.y) {
        if (self->position.y < self->target->position.y)
            self->velocity.y += 0x3800;
    }
    else {
        self->velocity.y -= 0x3800;
    }

    self->velocity.x = CLAMP(self->velocity.x, -0x50000, 0x50000);
    self->velocity.y = CLAMP(self->velocity.y, -0x50000, 0x50000);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->targetPos.x = self->target->position.x;
    self->targetPos.y = self->target->position.y;

    self->angle = (self->angle + 12) & 0x3FF;
    int32 inc   = (384 - self->scale.x - (RSDK.Sin1024(self->angle) >> 3)) >> 3;

    self->scale.x += inc;
    self->scale.y = self->scale.x;
    PKingAttack_CheckPlayerCollisions();

    if (self->angle < 0x200)
        self->drawGroup = Zone->objectDrawGroup[0] - 1;
    else
        self->drawGroup = Zone->objectDrawGroup[0];
}

void PKingAttack_State_OrbitLaunched(void)
{
    RSDK_THIS(PKingAttack);

    RSDK.ProcessAnimation(&self->animator);

    ++self->timer;
    if (!(self->timer & 3))
        CREATE_ENTITY(PKingAttack, INT_TO_VOID(PKINGATTACK_TRAIL), self->position.x, self->position.y);

    if (self->scale.x < 512) {
        self->scale.x += 0x20;
        self->scale.y = self->scale.x;
    }

    self->velocity.x = self->velocity.x + ((self->targetVelocity.x - self->velocity.x) >> 3);
    self->velocity.y = self->velocity.y + ((self->targetVelocity.y - self->velocity.y) >> 3);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    PKingAttack_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void PKingAttack_State_Trail(void)
{
    RSDK_THIS(PKingAttack);

    if (self->timer >= 8) {
        self->alpha -= 8;
        if (self->alpha <= 0)
            destroyEntity(self);
    }
    else {
        self->timer++;
    }
}

void PKingAttack_State_SmallBullet(void)
{
    RSDK_THIS(PKingAttack);

    RSDK.ProcessAnimation(&self->animator);

    self->rotation = (self->rotation + 16) & 0x1FF;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    PKingAttack_CheckPlayerCollisions();

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        self->position.y += 0x40000;
        destroyEntity(self);
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void PKingAttack_EditorDraw(void)
{
    RSDK_THIS(PKingAttack);

    self->type      = PKINGATTACK_ORBIT;
    self->inkEffect = INK_ADD;
    self->alpha     = 0xC0;
    RSDK.SetSpriteAnimation(PKingAttack->aniFrames, 12, &self->animator, true, 0);

    PKingAttack_Draw();
}

void PKingAttack_EditorLoad(void) { PKingAttack->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE); }
#endif

void PKingAttack_Serialize(void) {}
