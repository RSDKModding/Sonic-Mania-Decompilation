// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: KleptoMobile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectKleptoMobile *KleptoMobile;

void KleptoMobile_Update(void)
{
    RSDK_THIS(KleptoMobile);

    KleptoMobile_HandleAnimations();

    StateMachine_Run(self->state);
}

void KleptoMobile_LateUpdate(void) {}

void KleptoMobile_StaticUpdate(void) {}

void KleptoMobile_Draw(void)
{
    RSDK_THIS(KleptoMobile);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->basicAnimator, NULL, false);
    }
}

void KleptoMobile_Create(void *data)
{
    RSDK_THIS(KleptoMobile);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->updateRange.x   = 0x800000;
            self->updateRange.y   = 0x800000;
            self->visible         = true;
            self->drawGroup       = Zone->objectDrawGroup[0];
            self->drawFX          = FX_FLIP;
            self->explosionVolume = 0x200;

            self->type = VOID_TO_INT(data);
            switch (self->type) {
                case KLEPTOMOBILE_EGGMAN:
                    self->hitbox.left   = -22;
                    self->hitbox.top    = -22;
                    self->hitbox.right  = 22;
                    self->hitbox.bottom = 22;

                    self->active = ACTIVE_NORMAL;
                    self->health = 8;

                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->mobileTopAnimator, true, 1);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->eggmanAnimator, true, 0);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->mobileAnimator, true, 0);
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &self->rubyAnimator, true, 0);

                    KleptoMobile->defeated = false;

                    self->stateDraw = KleptoMobile_Draw_KleptoMobile;
                    self->state     = KleptoMobile_State_SetupArena;
                    break;

                case KLEPTOMOBILE_ARM_L:
                case KLEPTOMOBILE_ARM_R:
                    self->hitbox.left   = -32;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 32;
                    self->hitbox.bottom = 8;

                    self->active  = ACTIVE_NORMAL;
                    self->visible = true;

                    if (self->type == KLEPTOMOBILE_ARM_R) {
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 7, &self->orbAnimator, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 8, &self->handAnimator, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 9, &self->finger1Animator, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 10, &self->finger2Animator, true, 0);
                    }
                    else {
                        self->drawGroup = Zone->playerDrawGroup[0];
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 3, &self->orbAnimator, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 4, &self->handAnimator, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 5, &self->finger1Animator, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 6, &self->finger2Animator, true, 0);
                    }

                    self->stateDraw = KleptoMobile_Draw_Arm;
                    self->state     = KleptoMobile_StateArm_Cutscene;
                    break;

                case KLEPTOMOBILE_HAND:
                    self->active  = ACTIVE_NORMAL;
                    self->visible = true;

                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 12, &self->handAnimator, true, 5);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 17, &self->orbAnimator, true, 0);

                    self->stateDraw = KleptoMobile_Draw_Hand;
                    self->state     = KleptoMobile_StateHand_Cutscene;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void KleptoMobile_StageLoad(void)
{
    KleptoMobile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/KleptoMobile.bin", SCOPE_STAGE);

    KleptoMobile->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    KleptoMobile->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    KleptoMobile->sfxFlail     = RSDK.GetSfx("SSZ1/Flail.wav");
    KleptoMobile->sfxWhack     = RSDK.GetSfx("Stage/Whack.wav");
    KleptoMobile->sfxPowerUp   = RSDK.GetSfx("Stage/PowerUp.wav");
    KleptoMobile->sfxRocketJet = RSDK.GetSfx("Stage/RocketJet.wav");
}

void KleptoMobile_HandleAnimations(void)
{
    RSDK_THIS(KleptoMobile);

    bool32 laughing = false;
    RSDK.ProcessAnimation(&self->eggmanAnimator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->eggmanAnimator.animationID == 13) {
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hurt || player->state == Player_State_Death || player->state == ERZStart_State_PlayerRebound)
                laughing = true;
        }

        if (laughing)
            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 14, &self->eggmanAnimator, true, 0);
    }
    else if (self->eggmanAnimator.animationID == 14) {
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hurt || player->state == Player_State_Death || player->state == ERZStart_State_PlayerRebound)
                laughing = true;
        }

        if (self->eggmanAnimator.frameID >= 14) {
            if (laughing)
                RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 14, &self->eggmanAnimator, true, 7);
            else
                RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->eggmanAnimator, true, 0);
        }
    }
    else {
        if (self->eggmanAnimator.animationID == 15 && !self->invincibilityTimer)
            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->eggmanAnimator, true, 0);
    }
}

void KleptoMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(KleptoMobile);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            KleptoMobile_Hit();
        }
    }
}

void KleptoMobile_Hit(void)
{
    RSDK_THIS(KleptoMobile);

    if (--self->health <= 0) {
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 16, &self->eggmanAnimator, true, 0);

        self->state = KleptoMobile_State_Destroyed;
        self->timer = 0;

        if (PhantomKing->defeated)
            SceneInfo->timeEnabled = false;

        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 15, &self->eggmanAnimator, true, 0);
        RSDK.PlaySfx(KleptoMobile->sfxHit, false, 255);

        foreach_all(PhantomRuby, ruby)
        {
            ruby->position.x = self->rubyPos.x;
            ruby->position.y = self->rubyPos.y;
            ruby->velocity.x = -0x20000;
            ruby->velocity.y = -0x40000;
            ruby->state      = PhantomRuby_State_MoveGravity;

            EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, KleptoMobile);
            arm1->timer              = 0;
            arm1->state              = KleptoMobile_StateArm_Cutscene;

            EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, KleptoMobile);
            arm2->timer              = 0;
            arm2->state              = KleptoMobile_StateArm_Cutscene;
        }

        self->holdingRuby = false;
        self->state       = KleptoMobile_State_HitFall;
    }
}

void KleptoMobile_Explode(void)
{
    RSDK_THIS(KleptoMobile);

    if (!(Zone->timer % 3)) {
        int32 channel = RSDK.PlaySfx(KleptoMobile->sfxExplosion, false, 255);

        if (self->state == KleptoMobile_State_CutsceneExplode)
            RSDK.SetChannelAttributes(channel, self->explosionVolume * (1 / 512.0f), 0.0, 1.0);

        if (Zone->timer & 4) {
            int32 x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int32 y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void KleptoMobile_HandleFrames(void)
{
    RSDK_THIS(KleptoMobile);

    self->rotation = RSDK.Sin512(2 * Zone->timer) >> 6;
    self->armAngle   = (self->armAngle + 12) & 0x3FF;

    int32 moveX = 0x1C00 * RSDK.Sin512(-self->rotation) + self->position.x;
    int32 moveY = 0x1C00 * RSDK.Cos512(-self->rotation) + self->position.y;

    int32 angle = self->armAngle;

    for (int32 i = 0; i < 10; i += 2) {
        self->armPositions[i].x = moveX + 2 * RSDK.Cos1024(angle) * RSDK.Cos512(self->rotation);
        self->armPositions[i].y = moveY + 2 * RSDK.Cos1024(angle) * RSDK.Sin512(self->rotation);
        self->armAngles[i]    = angle & 0x3FF;
        angle += 0x200;

        self->armPositions[i + 1].x = moveX + 2 * RSDK.Cos1024(angle) * RSDK.Cos512(self->rotation);
        self->armPositions[i + 1].y = moveY + 2 * RSDK.Cos1024(angle) * RSDK.Sin512(self->rotation);
        self->armAngles[i + 1]      = angle & 0x3FF;

        moveX += RSDK.Sin512(-self->rotation) << 10;
        moveY += RSDK.Cos512(-self->rotation) << 10;
        angle += 0x240;
    }
}

void KleptoMobile_SwitchToKing(void)
{
    EntityPhantomKing *kingPtr = NULL;

    foreach_active(PhantomKing, king)
    {
        if (king->type == PHANTOMKING_KING) {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

            king->direction = FLIP_X;
            if (RSDK.Rand(0, 2))
                king->position.x = player1->position.x + 0x1000000;
            else
                king->position.x = player1->position.x - 0x1000000;

            king->position.y  = player1->position.y + 0x800000;
            king->originPos.y = player1->position.y + 0x800000;
            king->rubyPos     = king->position;
            king->rubyPos.x -= 0x1400 * RSDK.Sin512(-king->rotation);
            king->rubyPos.y -= 0x1400 * RSDK.Cos512(-king->rotation);
            king->velocity.x = 0;
            king->velocity.y = 0;
            king->drawRuby   = true;

            for (int32 i = 0; i < 0x3FC; i += 0xAA) {
                EntityPKingAttack *attack = CREATE_ENTITY(PKingAttack, INT_TO_VOID(1), king->rubyPos.x, king->rubyPos.y);
                attack->angle             = i;
                attack->target            = (Entity *)king;
            }

            king->state = PhantomKing_State_FlyAround;
            kingPtr     = king;

            foreach_break;
        }
    }

    // NOTE: this will crash if kingPtr is NULL, make sure there's a phantom king in the scene!!!
    foreach_active(PhantomKing, kingArm)
    {
        if (kingArm->type != PHANTOMKING_KING) {
            kingArm->position.x = kingPtr->position.x;
            kingArm->position.y = kingPtr->position.y;

            for (int32 i = 0; i < 10; ++i) kingArm->armPositions[i] = kingPtr->position;

            kingArm->velocity.x = 0;
            kingArm->velocity.y = 0;
            kingArm->state      = PhantomKing_StateArm_Idle;
        }
    }
}

void KleptoMobile_Draw_KleptoMobile(void)
{
    RSDK_THIS(KleptoMobile);

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 112, 0, 136, 8);
        RSDK.CopyPalette(2, 120, 0, 11, 1);
        RSDK.CopyPalette(2, 121, 0, 25, 1);
        RSDK.CopyPalette(2, 122, 0, 197, 6);

        RSDK.DrawSprite(&self->mobileTopAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);

        self->mobileAnimator.frameID = 2;
        RSDK.DrawSprite(&self->mobileAnimator, NULL, false);

        self->mobileAnimator.frameID = 0;
        RSDK.DrawSprite(&self->mobileAnimator, NULL, false);

        RSDK.CopyPalette(1, 136, 0, 136, 8);
        RSDK.CopyPalette(1, 11, 0, 11, 1);
        RSDK.CopyPalette(1, 25, 0, 25, 1);
        RSDK.CopyPalette(1, 197, 0, 197, 6);
    }
    else {
        self->basicAnimator.frameID = 1; // this sets basicAnimator to anim 0, frame 1... and then draws it using mobileTopAnimator instead
        RSDK.DrawSprite(&self->mobileTopAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);

        self->mobileAnimator.frameID = 2;
        RSDK.DrawSprite(&self->mobileAnimator, NULL, false);

        self->mobileAnimator.frameID = 0;
        RSDK.DrawSprite(&self->mobileAnimator, NULL, false);
    }

    RSDK.DrawCircle(self->rubyPos.x, self->rubyPos.y, self->circleRadius, 0x000000, 0xFF, INK_TINT, false);
}

void KleptoMobile_State_SetupArena(void)
{
    RSDK_THIS(KleptoMobile);

    self->active = ACTIVE_NORMAL;

    EntityKleptoMobile *hand = RSDK_GET_ENTITY(SceneInfo->entitySlot - 2, KleptoMobile);
    RSDK.ResetEntity(hand, KleptoMobile->classID, INT_TO_VOID(KLEPTOMOBILE_HAND));
    hand->position.x = self->position.x;
    hand->position.y = self->position.y;
    hand->parent     = self;

    EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, KleptoMobile);
    RSDK.ResetEntity(arm2, KleptoMobile->classID, INT_TO_VOID(KLEPTOMOBILE_ARM_R));
    arm2->position.x = self->position.x;
    arm2->position.y = self->position.y;
    arm2->parent     = self;

    EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, KleptoMobile);
    RSDK.ResetEntity(arm1, KleptoMobile->classID, INT_TO_VOID(KLEPTOMOBILE_ARM_L));
    arm1->position.x = self->position.x;
    arm1->position.y = self->position.y;
    arm1->parent     = self;

    self->originPos.x     = self->position.x;
    self->originPos.y     = self->position.y;
    KleptoMobile->boundsM = self->position.x;
    KleptoMobile->boundsL = KleptoMobile->boundsM - 0x800000;
    KleptoMobile->boundsR = KleptoMobile->boundsM + 0x800000;
    KleptoMobile->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
    KleptoMobile->boundsB = (Zone->cameraBoundsB[0] - 96) << 16;
    self->state           = StateMachine_None;
}

void KleptoMobile_State_CutsceneControlled(void)
{
    RSDK_THIS(KleptoMobile);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->originPos.x += self->velocity.x;
    self->originPos.y += self->velocity.y;

    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_MoveAround(void)
{
    RSDK_THIS(KleptoMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);
    KleptoMobile_CheckPlayerCollisions();

    int32 angle = RSDK.ATan2(self->position.x - player1->position.x, self->originPos.y - player1->position.y);
    int32 x     = (RSDK.Cos256(angle) << 15) + player1->position.x;
    int32 y     = (RSDK.Sin256(angle) << 15) + player1->position.y;

    if (self->circleRadius > 0)
        self->circleRadius -= 8;

    if (x <= self->position.x) {
        if (self->velocity.x > -0x20000)
            self->velocity.x -= 0x800;
    }
    else {
        if (self->velocity.x < 0x20000)
            self->velocity.x += 0x800;
    }

    int32 bottom   = Zone->cameraBoundsB[0] << 16;
    int32 boundary = bottom - 0x800000;

    if (y <= bottom - 0x800000) {
        boundary = bottom + 0x800000;
        if (y >= (Zone->cameraBoundsT[0] + 128) << 16)
            boundary = y;
    }

    if (boundary <= self->originPos.y) {
        if (self->velocity.y > -0x20000)
            self->velocity.y -= 0x800;
    }
    else {
        if (self->velocity.y < 0x20000)
            self->velocity.y += 0x800;
    }

    self->position.x += self->velocity.x;
    self->originPos.y += self->velocity.y;
    self->direction = player1->position.x > self->position.x;

    if (--self->bashArmDelay == 1) {
        self->bashArmDelay = 0;
        self->timer        = 0;
        RSDK.PlaySfx(KleptoMobile->sfxPowerUp, false, 255);

        EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, KleptoMobile);
        arm2->timer              = 0;
        arm2->state              = KleptoMobile_StateArm_ChargeAttack;

        EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, KleptoMobile);
        arm1->timer              = 0;
        arm1->state              = KleptoMobile_StateArm_ChargeAttack;
        self->state              = KleptoMobile_State_Hover;
    }
    else {
        if (self->canBashAttack) {
            if (self->bashArmDelay <= 0 && ++self->timer == 15) {
                self->timer = 0;
                RSDK.PlaySfx(KleptoMobile->sfxFlail, false, 0xFF);
                int32 armSlot = self->bashArmID ? SceneInfo->entitySlot + 1 : SceneInfo->entitySlot - 1;

                EntityKleptoMobile *arm = RSDK_GET_ENTITY(armSlot, KleptoMobile);

                x = self->position.x + (self->direction == FLIP_X ? -0x30000 : 0x30000);
                if (arm->type == KLEPTOMOBILE_ARM_R)
                    x += 0x180000;
                y = self->position.y + 0xD0000;

                angle                   = RSDK.ATan2(player1->position.x - x, player1->position.y - y);
                arm->bashArmTargetPos.x = x + (RSDK.Cos256(angle) << 15);
                arm->bashArmTargetPos.y = y + (RSDK.Sin256(angle) << 15);
                arm->timer              = 0;
                arm->state              = KleptoMobile_StateArm_BashAttack;
                self->bashArmID ^= 1;

                if (++self->attackCount >= 5) {
                    self->attackCount   = 0;
                    self->bashArmDelay  = 90;
                    self->canBashAttack = false;
                }
            }
        }
        else {
            if (abs(player1->position.x - self->position.x) < 0x1800000 && abs(player1->position.y - self->position.y) < 0x1800000)
                self->canBashAttack = true;
        }

        KleptoMobile_HandleFrames();
    }
}

void KleptoMobile_State_Hover(void)
{
    RSDK_THIS(KleptoMobile);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);

    KleptoMobile_CheckPlayerCollisions();

    if (++self->timer < 180) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (abs(player1->position.x - self->position.x) < 0xC00000 && abs(player1->position.y - self->position.y) < 0xC00000) {
            RSDK.StopSfx(KleptoMobile->sfxPowerUp);

            self->timer         = 14;
            self->bashArmDelay  = 0;
            self->canBashAttack = true;
            self->state         = KleptoMobile_State_MoveAround;
        }
    }
    else {
        self->timer      = 0;
        self->velocity.x = self->direction == FLIP_NONE ? -0xA0000 : 0xA0000;

        PhantomRuby_PlaySfx(RUBYSFX_REDCUBE);
        RSDK.PlaySfx(KleptoMobile->sfxRocketJet, false, 255);
        self->state = KleptoMobile_State_FirstChargeAttack;
    }

    KleptoMobile_HandleFrames();
}

void KleptoMobile_HandleArmPositions(void)
{
    foreach_active(KleptoMobile, eggman)
    {
        if (eggman->type != KLEPTOMOBILE_EGGMAN) {
            EntityKleptoMobile *parent = eggman->parent;
            eggman->position.x         = parent->position.x;
            eggman->position.y         = parent->position.y;

            for (int32 i = 0; i < 10; ++i) eggman->armPositions[i] = parent->position;

            eggman->velocity.x = 0;
            eggman->velocity.y = 0;
        }
    }
}

void KleptoMobile_HandleChargeFinish(void)
{
    RSDK_THIS(KleptoMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->timer = 0;

    PhantomRuby_PlaySfx(RSDK.Rand(RUBYSFX_ATTACK1, RUBYSFX_REDCUBE));
    if (!RSDK.Rand(0, 2))
        self->position.x = player1->position.x - 0x1800000;
    else
        self->position.x = player1->position.x + 0x1800000;

    self->originPos.y = (RSDK.Rand(-2, 3) << 21) + player1->position.y;
    while (self->originPos.y > (Zone->cameraBoundsB[0] - 64) << 16 && self->originPos.y < (Zone->cameraBoundsT[0] + 64) << 16) {
        self->originPos.y = (RSDK.Rand(-2, 3) << 21) + player1->position.y;
    }

    self->circleRadius = 128;

    if (++self->attackCount == 4) {
        self->attackCount = 0;
        self->velocity.x  = 0;
        self->velocity.y  = 0;
        self->state       = KleptoMobile_State_MoveAround;
    }
    else if (player1->position.x >= self->position.x) {
        self->velocity.x = 0xA0000;
        self->direction  = FLIP_X;
    }
    else {
        self->velocity.x = -0xA0000;
        self->direction  = FLIP_NONE;
    }

    KleptoMobile_HandleArmPositions();
}

void KleptoMobile_State_FirstChargeAttack(void)
{
    RSDK_THIS(KleptoMobile);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);

    if (self->circleRadius < 128)
        self->circleRadius += 8;

    if (++self->timer >= 180) {
        KleptoMobile_HandleChargeFinish();
        self->state = KleptoMobile_State_NextChargeAttacks;
    }

    self->position.x += self->velocity.x;
    self->originPos.y += self->velocity.y;

    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_NextChargeAttacks(void)
{
    RSDK_THIS(KleptoMobile);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 9);

    KleptoMobile_CheckPlayerCollisions();

    if (self->onScreen) {
        if (self->circleRadius > 0)
            self->circleRadius -= 8;
    }

    if (++self->timer >= 180)
        KleptoMobile_HandleChargeFinish();

    self->position.x += self->velocity.x;
    self->originPos.y += self->velocity.y;

    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_Switch(void)
{
    RSDK_THIS(KleptoMobile);

    if (++self->timer >= 120) {
        self->timer = 0;
        KleptoMobile_SwitchToKing();
        self->state = StateMachine_None;
    }
}

void KleptoMobile_State_HitFall(void)
{
    RSDK_THIS(KleptoMobile);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    if (self->position.y < -0x1000000) {
        self->canBashAttack = false;
        self->bashArmDelay  = 0;
        self->velocity.y    = 0;
        self->timer         = 0;
        self->state         = KleptoMobile_State_Switch;
    }

    KleptoMobile_HandleFrames();
}

void KleptoMobile_StateHand_Cutscene(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = self->parent;

    int32 parentX = parent->position.x;
    int32 parentY = parent->position.y;

    int32 moveX = 0;
    int32 moveY = -0x200000 - (RSDK.Sin256((((self->type << 7) - 0x80) ^ 0x80) + 2 * Zone->timer) << 12);

    self->direction = parent->direction;
    int32 x         = parent->position.x;
    int32 y         = parent->position.y - 0x180000;

    int32 x2 = 0;
    int32 y2 = 0;
    if (self->direction == FLIP_X) {
        moveX = parentX + 0x200000;
        x2    = ((self->position.x + parent->position.x) >> 1) + 0x80000;
    }
    else {
        moveX = parentX - 0x200000;
        x2    = ((self->position.x + parent->position.x) >> 1) - 0x80000;
    }

    y2 = ((parent->position.y - 0x180000 + self->position.y) >> 1) - 0x300000;

    self->velocity.x += ((moveX - self->position.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += (((moveY + parentY) - self->position.y) >> 5) - (self->velocity.y >> 3);
    self->position.y += self->velocity.y;
    self->position.x += self->velocity.x;

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    parent->rubyPos.x = self->armPositions[6].x;
    parent->rubyPos.y = self->armPositions[6].y;

    if (self->direction)
        parent->rubyPos.x += 0x80000;
    else
        parent->rubyPos.x -= 0x80000;

    parent->rubyPos.y += 0x80000;
}

void KleptoMobile_StateHand_Boss(void)
{
    RSDK_THIS(KleptoMobile);

    EntityKleptoMobile *parent = self->parent;

    self->direction = parent->direction;
    int32 parentX   = parent->position.x;
    int32 parentY   = parent->position.y - 0x180000;

    int32 x = 0;
    if (self->direction == FLIP_X)
        x = ((parent->position.x + self->position.x) >> 1) + 0x80000;
    else
        x = ((parent->position.x + self->position.x) >> 1) - 0x80000;
    int32 y = ((parentY + self->position.y) >> 1) - 0x300000;

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, parentX, parentY, x, y, x, y, self->position.x, self->position.y);
        percent += 0x2000;
    }

    parent->rubyPos.x = self->armPositions[6].x;
    parent->rubyPos.y = self->armPositions[6].y;

    parent->rubyPos.x += self->direction ? 0x80000 : -0x80000;
    parent->rubyPos.y += 0x80000;
}

void KleptoMobile_Draw_Hand(void)
{
    RSDK_THIS(KleptoMobile);

    EntityKleptoMobile *parent = self->parent;

    if (parent->holdingRuby)
        RSDK.DrawSprite(&parent->rubyAnimator, &parent->rubyPos, false);

    for (int32 i = 0; i < 6; ++i) RSDK.DrawSprite(&self->orbAnimator, &self->armPositions[i], false);

    RSDK.DrawSprite(&self->handAnimator, &self->armPositions[6], false);
}

void KleptoMobile_CheckPlayerCollisions_Arm(void)
{
    RSDK_THIS(KleptoMobile);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            if (player->superState == SUPERSTATE_SUPER) {
                if (!player->blinkTimer) {
                    RSDK.PlaySfx(KleptoMobile->sfxWhack, false, 255);
                    player->blinkTimer = 120;

                    if (self->state == KleptoMobile_StateArm_ChargeAttack) {
                        player->velocity.x = self->parent->velocity.x >> 2;
                        player->groundVel  = player->velocity.x;

                        if (player->position.y <= self->position.y || player->onGround) {
                            player->onGround   = false;
                            player->velocity.y = -0xA0000;
                        }
                        else {
                            player->velocity.y = 0xA0000;
                        }
                    }
                    else {
                        player->velocity.x = self->velocity.x >> 2;
                        player->velocity.y = self->velocity.y >> 2;
                        player->groundVel  = player->velocity.x;
                    }

                    player->rotation = 0;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 0);
                    player->state = ERZStart_State_PlayerRebound;
                }
            }
            else {
                Player_Hurt(player, self);
            }
        }
    }
}

void KleptoMobile_StateArm_Cutscene(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = self->parent;

    int32 parentX = parent->position.x;
    int32 parentY = parent->position.y;

    int32 moveX     = 0;
    int32 moveY     = ((RSDK.Sin256((((self->type << 7) - 128) ^ 0x80) + 2 * Zone->timer) + 512) << 12) + parentY;
    self->direction = parent->direction;

    int32 x = 0, y = 0;
    int32 x2 = 0, y2 = 0;

    if (self->direction == FLIP_X) {
        moveX = parentX + 0x300000;
        x     = parentX - 0x30000;
        y     = parentY + 0xD0000;

        if (self->type == KLEPTOMOBILE_ARM_R) {
            x += 0x180000;
            moveX = parentX + 0x600000;
        }

        x2 = ((self->position.x + x) >> 1) - 0x100000;
        y2 = ((self->position.y + y) >> 1) + 0x100000;
    }
    else {
        moveX = parentX - 0x300000;
        x     = parentX + 0x30000;
        y     = parentY + 0xD0000;

        if (self->type == KLEPTOMOBILE_ARM_R) {
            x -= 0x180000;
            moveX = parentX - 0x600000;
        }

        x2 = ((self->position.x + x) >> 1) + 0x200000;
        y2 = ((self->position.y + y) >> 1) + 0x200000;
    }

    self->velocity.x += ((moveX - self->position.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += ((moveY - self->position.y) >> 5) - (self->velocity.y >> 3);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->finger1Animator);
    RSDK.ProcessAnimation(&self->finger2Animator);
}

void KleptoMobile_StateArm_Idle(void)
{
    RSDK_THIS(KleptoMobile);

    EntityKleptoMobile *parent = self->parent;

    self->direction = parent->direction;

    int32 x = 0;
    int32 y = parent->position.y + 0x60000;
    if (self->direction == FLIP_X) {
        x = parent->position.x - 0x90000;
        if (self->type == KLEPTOMOBILE_ARM_R)
            x += 0x180000;
    }
    else {
        x = parent->position.x + 0x90000;
        if (self->type == KLEPTOMOBILE_ARM_R)
            x -= 0x180000;
    }

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, self->armBezierPos.x, self->armBezierPos.y, self->armBezierPos.x,
                                                             self->armBezierPos.y, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->finger1Animator);
    RSDK.ProcessAnimation(&self->finger2Animator);
}

void KleptoMobile_StateArm_BashAttack(void)
{
    RSDK_THIS(KleptoMobile);

    EntityKleptoMobile *parent = self->parent;

    if (!self->timer) {
        self->bashArmStartPos.x = self->position.x;
        self->bashArmStartPos.y = self->position.y;
    }

    self->direction = parent->direction;

    int32 x = 0;
    int32 y = parent->position.y + 0xD0000;

    int32 x2 = 0, y2 = 0;
    int32 x3 = 0, y3 = 0;

    if (self->direction == FLIP_X) {
        x = parent->position.x - 0x30000;
        if (self->type == KLEPTOMOBILE_ARM_R)
            x += 0x180000;
    }
    else {
        x = parent->position.x + 0x30000;
        if (self->type == KLEPTOMOBILE_ARM_R)
            x -= 0x180000;
    }

    if (self->timer >= 4) {
        x2               = x;
        y2               = y;
        x3               = self->position.x;
        y3               = self->position.y;
        self->position.x = self->bashArmTargetPos.x;
        self->position.y = self->bashArmTargetPos.y;
    }
    else {
        self->velocity.x = (self->bashArmTargetPos.x - self->bashArmStartPos.x) / 4;
        self->velocity.y = (self->bashArmTargetPos.y - self->bashArmStartPos.y) / 4;

        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        int32 distX = self->position.x - x;
        int32 distY = self->position.y - y;

        int32 distValX = -distX;
        int32 distValY = distY;
        if (distX > 0) {
            distValX = distX;
            distValY = -distY;
        }

        int32 timerVal = 4 - self->timer;
        x2             = x + ((x + (distX >> 1) + distValY) - x) * timerVal / 4;
        y2             = y + (((distY >> 1) + distValX) * timerVal) / 4;
        x3             = self->position.x + ((x + (distX >> 1) + distValY) - self->position.x) * timerVal / 4;
        y3             = self->position.y + (((distY >> 1) + distValX - self->position.y) * timerVal / 4);
    }

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, x2, y2, x3, y3, self->position.x, self->position.y);
        percent += 0x2000;
    }

    if (++self->timer > 15) {
        self->timer = 0;
        self->state = KleptoMobile_StateArm_Cutscene;
    }

    RSDK.ProcessAnimation(&self->finger1Animator);
    RSDK.ProcessAnimation(&self->finger2Animator);

    if (parent->health > 0)
        KleptoMobile_CheckPlayerCollisions_Arm();
}

void KleptoMobile_StateArm_ChargeAttack(void)
{
    RSDK_THIS(KleptoMobile);

    EntityKleptoMobile *parent = self->parent;

    int32 parentX = parent->position.x;
    int32 parentY = parent->position.y;

    int32 moveX = 0;
    int32 moveY = parentY + 0x180000;
    if (self->type == KLEPTOMOBILE_ARM_R)
        moveY = parentY - 0x180000;

    moveX = parentX - 0x400000;
    if (self->direction == FLIP_X)
        moveX = parentX + 0x400000;

    self->direction = parent->direction;

    int32 x = 0;
    int32 y = parent->position.y + 0xD0000;
    if (self->direction == FLIP_X) {
        moveX += 0x300000;
        x = parent->position.x - 0x30000;

        if (self->type == KLEPTOMOBILE_ARM_R) {
            x += 0x180000;
            moveX += 0x300000;
        }
    }
    else {
        moveX -= 0x300000;
        x = parent->position.x + 0x30000;

        if (self->type == KLEPTOMOBILE_ARM_R) {
            x -= 0x180000;
            moveX -= 0x300000;
        }
    }

    int32 x2 = (x + self->position.x) >> 1;
    int32 y2 = (y + self->position.y) >> 1;

    self->velocity.x += ((moveX - self->position.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += ((moveY - self->position.y) >> 5) - (self->velocity.y >> 3);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->finger1Animator);
    RSDK.ProcessAnimation(&self->finger2Animator);

    if (parent->health > 0)
        KleptoMobile_CheckPlayerCollisions_Arm();
}

void KleptoMobile_Draw_Arm(void)
{
    RSDK_THIS(KleptoMobile);

    for (int32 i = 0; i < 6; ++i) RSDK.DrawSprite(&self->orbAnimator, &self->armPositions[i], false);

    RSDK.DrawSprite(&self->handAnimator, &self->armPositions[6], false);

    if (self->direction) {
        if (self->type == KLEPTOMOBILE_ARM_R) {
            self->armPositions[6].x += 0x280000;
            self->armPositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].x -= 0x40000;
            self->armPositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->finger2Animator, &self->armPositions[6], false);
        }
        else {
            self->armPositions[6].x += 0x280000;
            RSDK.DrawSprite(&self->finger2Animator, &self->armPositions[6], false);

            self->armPositions[6].x -= 0x40000;
            self->armPositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].y -= 0x80000;
        }
    }
    else {
        if (self->type == KLEPTOMOBILE_ARM_R) {
            self->armPositions[6].x -= 0x280000;
            self->armPositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].x += 0x40000;
            self->armPositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->finger2Animator, &self->armPositions[6], false);
        }
        else {
            self->armPositions[6].x -= 0x280000;
            RSDK.DrawSprite(&self->finger2Animator, &self->armPositions[6], false);

            self->armPositions[6].x += 0x40000;
            self->armPositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->finger1Animator, &self->armPositions[6], false);

            self->armPositions[6].y -= 0x80000;
        }
    }
}

void KleptoMobile_State_Destroyed(void)
{
    RSDK_THIS(KleptoMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    KleptoMobile_Explode();

    if (++self->timer == 96) {
        Debris_CreateFromEntries(KleptoMobile->aniFrames, KleptoMobile->debrisInfo, 18);
    }
    else if (self->timer == 144) {
        KleptoMobile->defeated = true;
        if (PhantomKing->defeated) {
            self->position.y += 0x1000000;
            foreach_all(ERZOutro, outro)
            {
                outro->active = ACTIVE_NORMAL;
                foreach_break;
            }
        }
        self->state = KleptoMobile_State_Explode;
    }
}

void KleptoMobile_State_Explode(void)
{
    RSDK_THIS(KleptoMobile);

    self->velocity.y += 0x2800;
    self->position.y += self->velocity.y;

    KleptoMobile_Explode();

    if (self->position.y >= 0x2800000) {
        if (!PhantomKing->defeated) {
            KleptoMobile_SwitchToKing();
            self->state = StateMachine_None;
        }
    }
}

void KleptoMobile_State_CutsceneExplode(void)
{
    RSDK_THIS(KleptoMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    self->originPos.x += self->velocity.x;
    self->originPos.y += self->velocity.y;
    self->position.x = self->originPos.x;
    self->position.y = self->originPos.y;

    if (self->explosionVolume > 0) {
        KleptoMobile_Explode();

        if (self->timer >= 60)
            self->explosionVolume -= 2;
        self->timer++;
    }
}

#if GAME_INCLUDE_EDITOR
void KleptoMobile_EditorDraw(void)
{
    RSDK_THIS(KleptoMobile);
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->mobileTopAnimator, false, 1);
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->eggmanAnimator, false, 0);
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->mobileAnimator, false, 0);

    KleptoMobile_Draw_KleptoMobile();
}

void KleptoMobile_EditorLoad(void)
{
    KleptoMobile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/KleptoMobile.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(KleptoMobile, type);
    RSDK_ENUM_VAR("Eggman", KLEPTOMOBILE_EGGMAN);
}
#endif

void KleptoMobile_Serialize(void) { RSDK_EDITABLE_VAR(KleptoMobile, VAR_ENUM, type); }
