// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SilverSonic Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSilverSonic *SilverSonic;

void SilverSonic_Update(void)
{
    RSDK_THIS(SilverSonic);

    RSDK.ProcessAnimation(&self->animator);

    self->outerBox = RSDK.GetHitbox(&self->animator, 0);
    self->innerBox = RSDK.GetHitbox(&self->animator, 1);

    if (self->position.x >= Zone->playerBoundsL[0] + 0x180000) {
        if (self->position.x > Zone->playerBoundsR[0] - 0x180000) {
            self->position.x = Zone->playerBoundsR[0] - 0x180000;
            if (self->state != SilverSonic_State_RollRebound) {
                self->direction  = FLIP_X;
                self->velocity.x = 0;
            }
        }
    }
    else {
        self->position.x = Zone->playerBoundsL[0] + 0x180000;
        if (self->state != SilverSonic_State_RollRebound) {
            self->direction  = FLIP_NONE;
            self->velocity.x = 0;
        }
    }

    RSDK.ProcessObjectMovement(self, self->outerBox, self->innerBox);

    StateMachine_Run(self->state);
}

void SilverSonic_LateUpdate(void) {}

void SilverSonic_StaticUpdate(void) {}

void SilverSonic_Draw(void)
{
    RSDK_THIS(SilverSonic);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SilverSonic_Create(void *data)
{
    RSDK_THIS(SilverSonic);

    if (!SceneInfo->inEditor) {
        self->active          = ACTIVE_BOUNDS;
        self->visible         = true;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x800000;
        self->collisionLayers = Zone->collisionLayers;
        self->velocity.y      = 0x10000;
        self->tileCollisions  = TILECOLLISION_DOWN;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_IDLE, &self->animator, true, 0);

        self->drawFX    = FX_FLIP;
        self->inkEffect = INK_ALPHA;
        self->state     = SilverSonic_State_Appear;

        self->attackType = RSDK.Rand(0, 3);
        if (self->attackType == 2)
            self->attackType = SSONIC_ATTACK_ROLL;
        self->attackRepeatCount = 1;

        self->drawGroup = Zone->objectDrawGroup[0];
    }
}

void SilverSonic_StageLoad(void)
{
    SilverSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SilverSonic.bin", SCOPE_STAGE);

    SilverSonic->sfxJump    = RSDK.GetSfx("SSZ2/SSJump.wav");
    SilverSonic->sfxDash    = RSDK.GetSfx("SSZ2/SSDash.wav");
    SilverSonic->sfxBoost   = RSDK.GetSfx("SSZ2/SSBoost.wav");
    SilverSonic->sfxRebound = RSDK.GetSfx("SSZ2/SSRebound.wav");
    SilverSonic->sfxArm     = RSDK.GetSfx("SSZ2/SSArm.wav");
}

void SilverSonic_HandleNextAttack(void)
{
    RSDK_THIS(SilverSonic);

    if (self->attackRepeatCount <= 0) {
        self->attackRepeatCount = RSDK.Rand(0, 2);
        self->attackType ^= 1;
    }

    switch (self->attackType) {
        default: break;

        case SSONIC_ATTACK_BOOST: // Arm Extend -> Boost
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_ARMATTACK, &self->animator, false, 0);
            self->state = SilverSonic_State_ArmAttack;
            RSDK.PlaySfx(SilverSonic->sfxArm, false, 255);
            break;

        case SSONIC_ATTACK_ROLL: // Spindash -> Roll/Jump
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_CROUCH, &self->animator, false, 0);
            self->state = SilverSonic_State_Crouch;
            break;
    }

    --self->attackRepeatCount;
}

void SilverSonic_CheckPlayerCollisions_Badnik(void)
{
    RSDK_THIS(SilverSonic);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, self->innerBox)) {
#if MANIA_USE_PLUS
            if (Player_CheckBadnikBreak(player, self, false)) {
                int32 x = self->position.x;
                int32 y = self->position.y;
                RSDK.ResetEntity(self, MSBomb->classID, NULL);

                EntityMSBomb *bomb = (EntityMSBomb *)self;
                bomb->position.x   = x;
                bomb->position.y   = y;

                bomb->velocity.y = -0x10000;
                bomb->timer      = 2;
                bomb->state      = MSBomb_State_SilverSonicExplode;
            }
#else
            Player_CheckBadnikBreak(player, self, true);
#endif
        }
    }
}

void SilverSonic_CheckPlayerCollisions_Ball(void)
{
    RSDK_THIS(SilverSonic);

    if (self->invincibilityTimer > 0) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, self->innerBox)) {
                if (Player_CheckAttackingNoInvTimer(player, self)) {
                    if (self->onGround) {
                        if (abs(player->velocity.x) <= 0x30000) {
                            player->groundVel  = self->groundVel;
                            player->velocity.x = self->groundVel;
                            self->groundVel    = (-0xC0 * self->groundVel) >> 8;
                        }
                        else {
                            self->onGround   = false;
                            self->velocity.x = (288 * player->velocity.x) >> 8;
                            self->velocity.y = -abs((0xC0 * player->velocity.x) >> 8);

                            player->velocity.x = (-0xC0 * player->velocity.x) >> 8;
                            player->groundVel  = player->velocity.x;

                            self->timer = 0;
                            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_JUMP, &self->animator, false, 0);
                            self->state = SilverSonic_State_RollRebound;
                        }
                    }
                    else {
                        if (abs(player->velocity.x) + abs(player->velocity.y) <= 0x40000) {
                            player->groundVel  = self->velocity.x;
                            player->velocity.x = self->velocity.x;
                            player->velocity.y = self->velocity.y;

                            int32 angle      = RSDK.ATan2(self->position.x - player->position.x, self->position.y - player->position.y);
                            self->velocity.x = RSDK.Cos256(angle) << 10;
                            self->velocity.y = RSDK.Sin256(angle) << 10;
                        }
                        else {
                            self->velocity.x = player->velocity.x;
                            self->velocity.y = -abs(player->velocity.y);

                            player->velocity.x = (-0xC0 * player->velocity.x) >> 8;
                            player->groundVel  = player->velocity.x;

                            self->timer = 0;
                            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_JUMP, &self->animator, false, 0);
                            self->state = SilverSonic_State_RollRebound;
                        }
                    }

                    if (player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_GLIDE) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_GLIDE_DROP, &player->animator, false, 0);
                        player->state = Player_State_KnuxGlideDrop;
                    }

                    self->invincibilityTimer = 8;
                    RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
                }
                else {
                    Player_Hurt(player, self);
                }
            }
        }
    }
}

void SilverSonic_CheckPlayerCollisions_Arm(void)
{
    RSDK_THIS(SilverSonic);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, self->innerBox)) {
#if MANIA_USE_PLUS
            if (self->direction) {
                if (player->position.x <= self->position.x) {
                    Player_Hurt(player, self);
                }
                else if (Player_CheckBadnikBreak(player, self, false)) {
                    int32 x = self->position.x;
                    int32 y = self->position.y;
                    RSDK.ResetEntity(self, MSBomb->classID, NULL);

                    EntityMSBomb *bomb = (EntityMSBomb *)self;
                    bomb->position.x   = x;
                    bomb->position.y   = y;

                    bomb->velocity.y = -0x10000;
                    bomb->timer      = 2;
                    bomb->state      = MSBomb_State_SilverSonicExplode;
                }
            }
            else {
                if (player->position.x >= self->position.x) {
                    Player_Hurt(player, self);
                }
                else if (Player_CheckBadnikBreak(player, self, false)) {
                    int32 x = self->position.x;
                    int32 y = self->position.y;
                    RSDK.ResetEntity(self, MSBomb->classID, NULL);

                    EntityMSBomb *bomb = (EntityMSBomb *)self;
                    bomb->position.x   = x;
                    bomb->position.y   = y;

                    bomb->velocity.y = -0x10000;
                    bomb->timer      = 2;
                    bomb->state      = MSBomb_State_SilverSonicExplode;
                }
            }
#else
            if (self->direction) {
                if (player->position.x <= self->position.x)
                    Player_Hurt(player, self);
                else
                    Player_CheckBadnikBreak(player, self, true);
            }
            else {
                if (player->position.x >= self->position.x)
                    Player_Hurt(player, self);
                else
                    Player_CheckBadnikBreak(player, self, true);
            }
#endif
        }
    }
}

void SilverSonic_State_Appear(void)
{
    RSDK_THIS(SilverSonic);

    if (!self->alpha)
        self->direction = Player_GetNearestPlayerX()->position.x < self->position.x;

    if (self->alpha >= 0x100) {
        self->inkEffect = INK_NONE;
        SilverSonic_HandleNextAttack();
    }
    else {
        self->alpha += 8;
    }
}

void SilverSonic_State_FinishedAttack(void)
{
    RSDK_THIS(SilverSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        SilverSonic_HandleNextAttack();
    }

    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_ArmAttack(void)
{
    RSDK_THIS(SilverSonic);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_BOOSTREADY, &self->animator, false, 0);
        self->state = SilverSonic_State_BoostReady;
    }

    SilverSonic_CheckPlayerCollisions_Arm();
}

void SilverSonic_State_Crouch(void)
{
    RSDK_THIS(SilverSonic);

    if (++self->timer == 30) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_SPINDASH, &self->animator, false, 0);

        self->state = SilverSonic_State_Spindash;
        RSDK.PlaySfx(SilverSonic->sfxDash, false, 255);
    }
}

void SilverSonic_State_Spindash(void)
{
    RSDK_THIS(SilverSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_JUMP, &self->animator, false, 0);
        self->velocity.x = self->direction ? -0x80000 : 0x80000;

        if (Zone->timer & 2) {
            self->groundVel = self->velocity.x;
            self->state     = SilverSonic_State_Roll;
            RSDK.PlaySfx(SilverSonic->sfxBoost, false, 255);
        }
        else {
            self->velocity.y = -0x68000;
            self->onGround   = false;
            self->state      = SilverSonic_State_RollJump;
            RSDK.PlaySfx(SilverSonic->sfxJump, false, 255);
        }
    }

    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State_Roll(void)
{
    RSDK_THIS(SilverSonic);

    if (self->position.x < Zone->playerBoundsL[0] + 0x180000) {
        self->velocity.x = 0x80000;
        self->direction  = FLIP_NONE;
        self->state      = SilverSonic_State_RollJump;
        self->onGround   = false;
        self->velocity.y = -0x68000;
        self->position.x = Zone->playerBoundsL[0] + 0x180000;
    }
    else if (self->position.x > Zone->playerBoundsR[0] - 0x180000) {
        self->velocity.x = -0x80000;
        self->direction  = FLIP_X;
        self->state      = SilverSonic_State_RollJump;
        self->onGround   = false;
        self->velocity.y = -0x68000;
        self->position.x = Zone->playerBoundsR[0] - 0x180000;
    }

    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State_RollJump(void)
{
    RSDK_THIS(SilverSonic);

    self->velocity.y += 0x3800;

    if (self->position.x < Zone->playerBoundsL[0] + 0x180000) {
        self->velocity.x = 0;
        self->position.x = Zone->playerBoundsL[0] + 0x180000;
    }
    else if (self->position.x > Zone->playerBoundsR[0] - 0x180000) {
        self->velocity.x = 0;
        self->position.x = Zone->playerBoundsR[0] - 0x180000;
    }

    if (self->onGround) {
        self->direction ^= FLIP_X;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_IDLE, &self->animator, false, 0);
        self->state = SilverSonic_State_FinishedAttack;
    }
    else {
        SilverSonic_CheckPlayerCollisions_Ball();
    }
}

void SilverSonic_State_RollRebound(void)
{
    RSDK_THIS(SilverSonic);

    self->velocity.y += 0x1800;

    if (self->position.x < Zone->playerBoundsL[0] + 0x180000) {
        if (self->velocity.x < 0) {
            self->direction  = FLIP_NONE;
            self->velocity.x = -self->velocity.x;
            Camera_ShakeScreen(0, 2, 0);
            RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
        }
    }
    else if (self->position.x > Zone->playerBoundsR[0] - 0x180000) {
        if (self->velocity.x > 0) {
            self->direction  = FLIP_X;
            self->velocity.x = -self->velocity.x;
            Camera_ShakeScreen(0, 2, 0);
            RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
        }
    }

    if (self->onGround) {
        if (abs(self->groundVel) <= 0x40000) {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_IDLE, &self->animator, false, 0);
            self->groundVel = 0;
            self->state     = SilverSonic_State_FinishedAttack;
        }
        else {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_BOOST, &self->animator, false, 0);
            self->state = SilverSonic_State_Boost_Ground;
        }
    }

    foreach_active(MetalSonic, metal)
    {
        if (RSDK.CheckObjectCollisionTouchBox(metal, metal->outerBox, self, self->outerBox)) {
            self->velocity.x = 0;
            self->velocity.y = 0;
            self->state      = SilverSonic_State_Explode;

            MetalSonic->invincibilityTimerPanel = 32;
            Camera_ShakeScreen(0, -4, 1);
            metal->health -= MANIA_USE_PLUS ? 2 : 1;

            if (metal->health <= 0) {
                metal->timer = 0;
                metal->state = MetalSonic_State_PanelExplosion;
            }
        }
    }

    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State_BoostReady(void)
{
    RSDK_THIS(SilverSonic);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_BOOST, &self->animator, false, 0);

        self->velocity.y = -0x30000;
        self->onGround   = false;
        self->state      = SilverSonic_State_Boost_Air;

        RSDK.PlaySfx(SilverSonic->sfxJump, false, 255);
    }

    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Boost_Air(void)
{
    RSDK_THIS(SilverSonic);

    if (self->onGround) {
        self->state     = SilverSonic_State_Boost_Ground;
        self->groundVel = self->direction == FLIP_NONE ? 0x60000 : -0x60000;

        RSDK.PlaySfx(SilverSonic->sfxBoost, false, 255);
    }
    else {
        self->velocity.y += 0x2800;
    }

    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Boost_Ground(void)
{
    RSDK_THIS(SilverSonic);

    if (self->position.x < Zone->playerBoundsL[0] + 0x180000 || self->position.x > Zone->playerBoundsR[0] - 0x180000) {
        if (self->position.x < Zone->playerBoundsL[0] + 0x180000)
            self->position.x = Zone->playerBoundsL[0] + 0x180000;
        else
            self->position.x = Zone->playerBoundsR[0] - 0x180000;

        self->velocity.x = 0;
        self->velocity.y = -0x30000;
        self->onGround   = false;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_IDLE, &self->animator, false, 0);

        self->state = SilverSonic_State_FinishedBoost;
    }

    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_FinishedBoost(void)
{
    RSDK_THIS(SilverSonic);

    if (self->onGround) {
        self->state = SilverSonic_State_FinishedAttack;
    }
    else {
        int32 prevVelY = self->velocity.y;
        self->velocity.y += 0x2800;

        if (self->velocity.y > 0 && prevVelY <= 0)
            self->direction = self->position.x > Zone->playerBoundsL[0] + 0x800000;
    }

    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Explode(void)
{
    RSDK_THIS(SilverSonic);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);

        if (Zone->timer & 4) {
            int32 x = self->position.x + RSDK.Rand(-0x100000, 0x100000);
            int32 y = self->position.y + RSDK.Rand(-0x100000, 0x100000);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }

    self->visible = Zone->timer & 1;
    if (++self->timer == 16)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void SilverSonic_EditorDraw(void)
{
    RSDK_THIS(SilverSonic);
    RSDK.SetSpriteAnimation(SilverSonic->aniFrames, SSONIC_ANI_IDLE, &self->animator, false, 0);

    SilverSonic_Draw();
}

void SilverSonic_EditorLoad(void) { SilverSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SilverSonic.bin", SCOPE_STAGE); }
#endif

void SilverSonic_Serialize(void) {}
