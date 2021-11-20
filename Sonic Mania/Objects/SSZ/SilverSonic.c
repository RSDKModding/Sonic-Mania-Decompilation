#include "SonicMania.h"

ObjectSilverSonic *SilverSonic;

void SilverSonic_Update(void)
{
    RSDK_THIS(SilverSonic);
    RSDK.ProcessAnimation(&self->animator);
    self->outerBox = RSDK.GetHitbox(&self->animator, 0);
    self->innerBox = RSDK.GetHitbox(&self->animator, 1);

    if (self->position.x >= Zone->screenBoundsL2[0] + 0x180000) {
        if (self->position.x > Zone->screenBoundsR2[0] - 0x180000) {
            self->position.x = Zone->screenBoundsR2[0] - 0x180000;
            if (self->state != SilverSonic_State2_Unknown6) {
                self->direction  = FLIP_X;
                self->velocity.x = 0;
            }
        }
    }
    else {
        self->position.x = Zone->screenBoundsL2[0] + 0x180000;
        if (self->state != SilverSonic_State2_Unknown6) {
            self->direction  = FLIP_NONE;
            self->velocity.x = 0;
        }
    }

    RSDK.ProcessTileCollisions(self, self->outerBox, self->innerBox);
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
        self->collisionLayers = Zone->fgLayers;
        self->velocity.y      = 0x10000;
        self->tileCollisions  = true;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &self->animator, true, 0);
        self->drawFX    = FX_FLIP;
        self->inkEffect = INK_ALPHA;
        self->state     = SilverSonic_State_Unknown1;
        self->field_60  = RSDK.Rand(0, 3);
        if (self->field_60 == 2)
            self->field_60 = 1;
        self->field_64  = 1;
        self->drawOrder = Zone->drawOrderLow;
    }
}

void SilverSonic_StageLoad(void)
{
    SilverSonic->aniFrames  = RSDK.LoadSpriteAnimation("SSZ2/SilverSonic.bin", SCOPE_STAGE);
    SilverSonic->sfxJump    = RSDK.GetSFX("SSZ2/SSJump.wav");
    SilverSonic->sfxDash    = RSDK.GetSFX("SSZ2/SSDash.wav");
    SilverSonic->sfxBoost   = RSDK.GetSFX("SSZ2/SSBoost.wav");
    SilverSonic->sfxRebound = RSDK.GetSFX("SSZ2/SSRebound.wav");
    SilverSonic->sfxArm     = RSDK.GetSFX("SSZ2/SSArm.wav");
}

void SilverSonic_HandleNextAttack(void)
{
    RSDK_THIS(SilverSonic);

    if (self->field_64 <= 0) {
        self->field_64 = RSDK.Rand(0, 2);
        self->field_60 ^= 1;
    }

    if (self->field_60) {
        if (!--self->field_60) {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 1, &self->animator, false, 0);
            self->state = SilverSonic_State2_Unknown1;
        }
    }
    else {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 4, &self->animator, false, 0);
        self->state = SilverSonic_State_Unknown2;
        RSDK.PlaySfx(SilverSonic->sfxArm, false, 255);
    }
    --self->field_64;
}

void SilverSonic_CheckPlayerCollisions_Badnik(void)
{
    RSDK_THIS(SilverSonic);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, self->innerBox)) {
#if RETRO_USE_PLUS
            if (Player_CheckBadnikBreak(self, player, false)) {
                int x = self->position.x;
                int y = self->position.y;
                RSDK.ResetEntityPtr(self, MSBomb->objectID, NULL);
                self->position.x = x;
                self->position.y = y;
                self->velocity.y = -0x10000;
                self->field_60   = 2;
                self->state      = MSBomb_Unknown2;
            }
#else
            Player_CheckBadnikBreak(self, player, true);
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
                if (Player_CheckAttacking(player, self)) {
                    if (self->onGround) {
                        if (abs(player->velocity.x) <= 0x30000) {
                            player->groundVel  = self->groundVel;
                            player->velocity.x = self->groundVel;
                            self->groundVel  = (-0xC0 * self->groundVel) >> 8;
                        }
                        else {
                            self->onGround   = false;
                            self->velocity.x = (288 * player->velocity.x) >> 8;
                            self->velocity.y = -abs((0xC0 * player->velocity.x) >> 8);
                            player->velocity.x = (-0xC0 * player->velocity.x) >> 8;
                            player->groundVel  = player->velocity.x;
                            self->timer      = 0;
                            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 5, &self->animator, false, 0);
                            self->state = SilverSonic_State2_Unknown6;
                        }
                    }
                    else {
                        if (abs(player->velocity.x) + abs(player->velocity.y) <= 0x40000) {
                            player->groundVel  = self->velocity.x;
                            player->velocity.x = self->velocity.x;
                            player->velocity.y = self->velocity.y;
                            int angle          = RSDK.ATan2(self->position.x - player->position.x, self->position.y - player->position.y);
                            self->velocity.x = RSDK.Cos256(angle) << 10;
                            self->velocity.y = RSDK.Sin256(angle) << 10;
                        }
                        else {
                            self->velocity.x = player->velocity.x;
                            self->velocity.y = -abs(player->velocity.y);
                            player->velocity.x = (-0xC0 * player->velocity.x) >> 8;
                            player->groundVel  = player->velocity.x;
                            self->timer      = 0;
                            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 5, &self->animator, false, 0);
                            self->state = SilverSonic_State2_Unknown6;
                        }
                    }

                    if (player->characterID == ID_KNUCKLES && player->animator.animationID == 48) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLYTIRED, &player->animator, false, 0);
                        player->state = Player_State_KnuxGlideDrop;
                    }
                    self->invincibilityTimer = 8;
                    RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
                }
                else {
                    Player_CheckHit(player, self);
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
#if RETRO_USE_PLUS
            if (self->direction) {
                if (player->position.x <= self->position.x) {
                    Player_CheckHit(player, self);
                }
                else if (Player_CheckBadnikBreak(self, player, false)) {
                    int x = self->position.x;
                    int y = self->position.y;
                    RSDK.ResetEntityPtr(self, MSBomb->objectID, NULL);
                    self->position.x = x;
                    self->position.y = y;
                    self->velocity.y = -0x10000;
                    self->field_60   = 2;
                    self->state      = MSBomb_Unknown2;
                }
            }
            else {
                if (player->position.x >= self->position.x) {
                    Player_CheckHit(player, self);
                }
                else if (Player_CheckBadnikBreak(self, player, false)) {
                    int x = self->position.x;
                    int y = self->position.y;
                    RSDK.ResetEntityPtr(self, MSBomb->objectID, NULL);
                    self->position.x = x;
                    self->position.y = y;
                    self->velocity.y = -0x10000;
                    self->field_60   = 2;
                    self->state      = MSBomb_Unknown2;
                }
            }
#else
            if (self->direction) {
                if (player->position.x <= self->position.x)
                    Player_CheckHit(player, self);
                else
                    Player_CheckBadnikBreak(self, player, true);
            }
            else {
                if (player->position.x >= self->position.x)
                    Player_CheckHit(player, self);
                else
                    Player_CheckBadnikBreak(self, player, true);
            }
#endif
        }
    }
}

void SilverSonic_State_Unknown1(void)
{
    RSDK_THIS(SilverSonic);

    if (!self->alpha)
        self->direction = Player_GetNearestPlayerX()->position.x < self->position.x;

    if (self->alpha >= 256) {
        self->inkEffect = INK_NONE;
        SilverSonic_HandleNextAttack();
    }
    else {
        self->alpha += 8;
    }
}

void SilverSonic_State2_Unknown5(void)
{
    RSDK_THIS(SilverSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        SilverSonic_HandleNextAttack();
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown2(void)
{
    RSDK_THIS(SilverSonic);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 2, &self->animator, false, 0);
        self->state = SilverSonic_State_Unknown3;
    }
    SilverSonic_CheckPlayerCollisions_Arm();
}

void SilverSonic_State2_Unknown1(void)
{
    RSDK_THIS(SilverSonic);

    if (++self->timer == 30) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 6, &self->animator, false, 0);
        self->state = SilverSonic_State2_Unknown2;
        RSDK.PlaySfx(SilverSonic->sfxDash, false, 255);
    }
}

void SilverSonic_State2_Unknown2(void)
{
    RSDK_THIS(SilverSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 5, &self->animator, false, 0);
        if (self->direction)
            self->velocity.x = -0x80000;
        else
            self->velocity.x = 0x80000;
        if (Zone->timer & 2) {
            self->groundVel = self->velocity.x;
            self->state     = SilverSonic_State2_Unknown3;
            RSDK.PlaySfx(SilverSonic->sfxBoost, false, 255);
        }
        else {
            self->velocity.y = -0x68000;
            self->onGround   = false;
            self->state      = SilverSonic_State2_Unknown4;
            RSDK.PlaySfx(SilverSonic->sfxJump, false, 255);
        }
    }
    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State2_Unknown3(void)
{
    RSDK_THIS(SilverSonic);

    if (self->position.x < Zone->screenBoundsL2[0] + 0x180000) {
        self->velocity.x = 0x80000;
        self->direction  = FLIP_NONE;
        self->state      = SilverSonic_State2_Unknown4;
        self->onGround   = false;
        self->velocity.y = -0x68000;
        self->position.x = Zone->screenBoundsL2[0] + 0x180000;
    }
    else if (self->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        self->velocity.x = -0x80000;
        self->direction  = FLIP_X;
        self->state      = SilverSonic_State2_Unknown4;
        self->onGround   = false;
        self->velocity.y = -0x68000;
        self->position.x = Zone->screenBoundsR2[0] - 0x180000;
    }
    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State2_Unknown4(void)
{
    RSDK_THIS(SilverSonic);

    self->velocity.y += 0x3800;
    if (self->position.x < Zone->screenBoundsL2[0] + 0x180000) {
        self->velocity.x = 0;
        self->position.x = Zone->screenBoundsL2[0] + 0x180000;
    }
    else if (self->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        self->velocity.x = 0;
        self->position.x = Zone->screenBoundsR2[0] - 0x180000;
    }

    if (self->onGround) {
        self->direction ^= FLIP_X;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &self->animator, false, 0);
        self->state = SilverSonic_State2_Unknown5;
    }
    else {
        SilverSonic_CheckPlayerCollisions_Ball();
    }
}

void SilverSonic_State2_Unknown6(void)
{
    RSDK_THIS(SilverSonic);

    self->velocity.y += 0x1800;
    if (self->position.x < Zone->screenBoundsL2[0] + 0x180000) {
        if (self->velocity.x < 0) {
            self->direction  = FLIP_NONE;
            self->velocity.x = -self->velocity.x;
            Camera_ShakeScreen(0, 2, 0);
            RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
        }
    }
    else if (self->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        if (self->velocity.x > 0) {
            self->direction  = FLIP_X;
            self->velocity.x = -self->velocity.x;
            Camera_ShakeScreen(0, 2, 0);
            RSDK.PlaySfx(SilverSonic->sfxRebound, false, 255);
        }
    }

    if (self->onGround) {
        if (abs(self->groundVel) <= 0x40000) {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &self->animator, false, 0);
            self->groundVel = 0;
            self->state     = SilverSonic_State2_Unknown5;
        }
        else {
            RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 3, &self->animator, false, 0);
            self->state = SilverSonic_State_Unknown5;
        }
    }

    foreach_active(MetalSonic, metal)
    {
        if (RSDK.CheckObjectCollisionTouchBox(metal, metal->outerBox, self, self->outerBox)) {
            self->velocity.x  = 0;
            self->velocity.y  = 0;
            self->state       = SilverSonic_State2_Explode;
            MetalSonic->field_8 = 32;
            Camera_ShakeScreen(0, -4, 1);
            metal->health -= RETRO_USE_PLUS ? 2 : 1;
            if (metal->health <= 0) {
                metal->timer = 0;
                metal->state = MetalSonic_State_PanelExplosion;
            }
        }
    }
    SilverSonic_CheckPlayerCollisions_Ball();
}

void SilverSonic_State_Unknown3(void)
{
    RSDK_THIS(SilverSonic);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 3, &self->animator, false, 0);
        self->velocity.y = -0x30000;
        self->onGround   = false;
        self->state      = SilverSonic_State_Unknown4;
        RSDK.PlaySfx(SilverSonic->sfxJump, false, 255);
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown4(void)
{
    RSDK_THIS(SilverSonic);

    if (self->onGround) {
        self->state = SilverSonic_State_Unknown5;
        if (self->direction == FLIP_NONE)
            self->groundVel = 0x60000;
        else
            self->groundVel = -0x60000;
        RSDK.PlaySfx(SilverSonic->sfxBoost, false, 255);
    }
    else {
        self->velocity.y += 0x2800;
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown5(void)
{
    RSDK_THIS(SilverSonic);

    if (self->position.x < Zone->screenBoundsL2[0] + 0x180000 || self->position.x > Zone->screenBoundsR2[0] - 0x180000) {
        if (self->position.x < Zone->screenBoundsL2[0] + 0x180000)
            self->position.x = Zone->screenBoundsL2[0] + 0x180000;
        else
            self->position.x = Zone->screenBoundsR2[0] - 0x180000;

        self->velocity.x = 0;
        self->velocity.y = -0x30000;
        self->onGround   = false;
        RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &self->animator, false, 0);
        self->state = SilverSonic_State_Unknown6;
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State_Unknown6(void)
{
    RSDK_THIS(SilverSonic);

    if (self->onGround) {
        self->state = SilverSonic_State2_Unknown5;
    }
    else {
        int vel = self->velocity.y;
        self->velocity.y += 0x2800;
        if (self->velocity.y > 0 && vel <= 0)
            self->direction = self->position.x > Zone->screenBoundsL2[0] + 0x800000;
    }
    SilverSonic_CheckPlayerCollisions_Badnik();
}

void SilverSonic_State2_Explode(void)
{
    RSDK_THIS(SilverSonic);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);
        if (Zone->timer & 4) {
            int x = self->position.x + RSDK.Rand(-0x100000, 0x100000);
            int y = self->position.y + RSDK.Rand(-0x100000, 0x100000);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
    self->visible = Zone->timer & 1;
    if (++self->timer == 16)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void SilverSonic_EditorDraw(void)
{
    RSDK_THIS(SilverSonic);
    RSDK.SetSpriteAnimation(SilverSonic->aniFrames, 0, &self->animator, false, 0);
    SilverSonic_Draw();
}

void SilverSonic_EditorLoad(void) { SilverSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/SilverSonic.bin", SCOPE_STAGE); }
#endif

void SilverSonic_Serialize(void) {}
