// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomKing Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPhantomKing *PhantomKing;

void PhantomKing_Update(void)
{
    RSDK_THIS(PhantomKing);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);
}

void PhantomKing_LateUpdate(void) {}

void PhantomKing_StaticUpdate(void) {}

void PhantomKing_Draw(void)
{
    RSDK_THIS(PhantomKing);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->basicAnimator, NULL, false);
    }
}

void PhantomKing_Create(void *data)
{
    RSDK_THIS(PhantomKing);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x1000000;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->type          = VOID_TO_INT(data);

        switch (self->type) {
            case PHANTOMKING_KING:
                self->hitbox.left   = -24;
                self->hitbox.top    = -24;
                self->hitbox.right  = 24;
                self->hitbox.bottom = 24;

                self->active    = ACTIVE_BOUNDS;
                self->visible   = false;
                self->direction = FLIP_X;
                self->health    = 8;

                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 0, &self->headAnimator, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 1, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 2, &self->beltAnimator, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 7, &self->particleAnimator, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &self->rubyAnimator, true, 0);

                self->originPos = self->position;
                self->state     = PhantomKing_State_Initialize;
                break;

            case PHANTOMKING_ARM_L:
            case PHANTOMKING_ARM_R:
                self->active  = ACTIVE_NORMAL;
                self->visible = true;

                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 3, &self->armAnimator, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 4, &self->cuffAnimator, true, 0);

                if (self->type == PHANTOMKING_ARM_L) {
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 6, &self->handAnimator, true, 0);
                }
                else {
                    self->drawGroup = Zone->playerDrawGroup[0];
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 5, &self->handAnimator, true, 0);
                }

                self->stateDraw = PhantomKing_Draw_Arm;
                self->state     = PhantomKing_StateArm_Idle;
                break;
        }
    }
}

void PhantomKing_StageLoad(void)
{
    PhantomKing->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);

    PhantomKing->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    PhantomKing->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
}

void PhantomKing_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomKing);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            PhantomKing_Hit();
        }
    }
}

void PhantomKing_Hit(void)
{
    RSDK_THIS(PhantomKing);

    foreach_all(PhantomRuby, ruby)
    {
        ruby->position.x = self->rubyPos.x;
        ruby->position.y = self->rubyPos.y;
        ruby->velocity.x = -0x20000;
        ruby->velocity.y = -0x40000;
        ruby->state      = PhantomRuby_State_MoveGravity;
    }

    self->drawRuby = false;
    if (--self->health <= 0) {
        self->originPos.x = self->position.x;
        self->originPos.y = self->position.y;
        self->state       = PhantomKing_State_Explode;
        self->velocity.y  = -0x10000;
        self->timer       = 0;

        foreach_active(PKingAttack, attack)
        {
            if (attack->state == PKingAttack_State_Orbiting)
                destroyEntity(attack);
        }

        if (KleptoMobile->defeated)
            SceneInfo->timeEnabled = false;

        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(PhantomKing->sfxHit, false, 255);
        self->state = PhantomKing_State_HitFall;
    }
}

void PhantomKing_Explode(void)
{
    RSDK_THIS(PhantomKing);

    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomKing->sfxExplosion, false, 255);

        if (Zone->timer & 8) {
            int32 x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int32 y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void PhantomKing_HandleFrames(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->bodyAnimator);

    if (self->state != PhantomKing_State_WrestleEggman) {
        int32 max = self->velocity.x >> 15;

        if (self->rotation >= max) {
            if (self->rotation > max) {
                self->rotation--;
                if (self->rotation < max)
                    self->rotation = self->velocity.x >> 15;
            }
        }
        else {
            self->rotation++;
            if (self->rotation > max)
                self->rotation = max;
        }
    }

    int32 negAng    = -self->rotation;
    self->bodyAngle = (self->bodyAngle + 12) & 0x3FF;

    int32 x = 0x1C00 * RSDK.Sin512(negAng) + self->position.x;
    int32 y = 0x1C00 * RSDK.Cos512(negAng) + self->position.y;

    int32 angle = self->bodyAngle;

    for (int32 i = 0; i < 10; i += 2) {
        self->armPositions[i].x = x + 2 * RSDK.Cos512(self->rotation) * RSDK.Cos1024(angle);
        self->armPositions[i].y = y + 2 * RSDK.Sin512(self->rotation) * RSDK.Cos1024(angle);
        self->armAngles[i]      = angle & 0x3FF;

        angle += 512;

        self->armPositions[i + 1].x = x + 2 * RSDK.Cos512(self->rotation) * RSDK.Cos1024(angle);
        self->armPositions[i + 1].y = y + 2 * RSDK.Sin512(self->rotation) * RSDK.Cos1024(angle);
        self->armAngles[i + 1]      = angle & 0x3FF;

        x += RSDK.Sin512(negAng) << 10;
        y += RSDK.Cos512(negAng) << 10;
        angle += 0x240;
    }

    self->rubyPos.x = self->position.x - 0x1400 * RSDK.Sin512(negAng);
    self->rubyPos.y = self->position.y - 0x1400 * RSDK.Cos512(negAng);
    if (self->direction) {
        self->rubyPos.x -= 0x180 * RSDK.Cos512(negAng);
        self->rubyPos.y -= 0x180 * RSDK.Sin512(negAng);
    }
    else {
        self->rubyPos.x += 0x180 * RSDK.Cos512(negAng);
        self->rubyPos.y += 0x180 * RSDK.Sin512(negAng);
    }
}

void PhantomKing_HandleAttacks(void)
{
    RSDK_THIS(PhantomKing);

    EntityPlayer *player  = Player_GetNearestPlayer();
    bool32 launchedAttack = false;

    foreach_active(PKingAttack, attack)
    {
        if (attack->state == PKingAttack_State_Orbiting) {
            PhantomRuby_PlaySfx(RSDK.Rand(RUBYSFX_ATTACK1, RUBYSFX_REDCUBE));
            attack->type = PKINGATTACK_LAUNCHED;
            int32 angle  = RSDK.ATan2((player->position.x - attack->position.x) >> 16, (player->position.y - attack->position.y) >> 16);

            attack->targetVelocity.x = 0x600 * RSDK.Cos256(angle);
            attack->targetVelocity.y = 0x600 * RSDK.Sin256(angle);
            attack->drawGroup        = Zone->objectDrawGroup[0];
            attack->state            = PKingAttack_State_OrbitLaunched;
            launchedAttack           = true;
            foreach_break;
        }
    }

    if (!launchedAttack) {
        // No More Attacks, lets make more
        for (int32 i = 0; i < 0x3FC; i += 0xAA) {
            EntityPKingAttack *attackOrb = CREATE_ENTITY(PKingAttack, INT_TO_VOID(PKINGATTACK_ORBIT), self->position.x, self->position.y);
            attackOrb->angle             = i;
            attackOrb->target            = (Entity *)self;
        }
        self->timer = -90;
    }
}

void PhantomKing_SwitchToEggman(void)
{
    EntityKleptoMobile *eggmanPtr = NULL;

    foreach_active(KleptoMobile, eggman)
    {
        if (eggman->type == KLEPTOMOBILE_EGGMAN) {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

            eggman->direction = FLIP_X;
            if (RSDK.Rand(0, 2))
                eggman->position.x = player1->position.x + 0x1000000;
            else
                eggman->position.x = player1->position.x - 0x1000000;

            eggman->position.y  = player1->position.y + 0x800000;
            eggman->originPos.y = player1->position.y + 0x800000;
            eggman->holdingRuby = true;
            eggman->velocity.x  = 0;
            eggman->velocity.y  = 0;
            eggman->state       = KleptoMobile_State_MoveAround;

            eggmanPtr = eggman;
            foreach_break;
        }
    }

    foreach_active(KleptoMobile, eggmanArm)
    {
        if (eggmanArm->type != KLEPTOMOBILE_EGGMAN) {
            eggmanArm->position.x = eggmanPtr->position.x;
            eggmanArm->position.y = eggmanPtr->position.y;

            for (int32 i = 0; i < 10; ++i) eggmanArm->armPositions[i] = eggmanPtr->position;

            eggmanArm->velocity.x = 0;
            eggmanArm->velocity.y = 0;
        }
    }
}

void PhantomKing_SetupKing(EntityPhantomKing *king)
{
    king->originPos  = king->position;
    king->velocity.x = 0;
    king->drawRuby   = true;

    PhantomKing->boundsM = king->position.x;
    PhantomKing->boundsL = PhantomKing->boundsM - 0x800000;
    PhantomKing->boundsR = PhantomKing->boundsM + 0x800000;
    PhantomKing->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
    PhantomKing->boundsB = (Zone->cameraBoundsB[0] - 96) << 16;

    int32 slot                                    = RSDK.GetEntitySlot(king);
    RSDK_GET_ENTITY(slot - 1, PhantomKing)->state = PhantomKing_StateArm_Idle;
    RSDK_GET_ENTITY(slot + 1, PhantomKing)->state = PhantomKing_StateArm_Idle;
    king->state                                   = PhantomKing_State_FlyAround;
}

void PhantomKing_Draw_Body(void)
{
    RSDK_THIS(PhantomKing);

    if (self->typeChangeTimer <= 0) {
        if (self->invincibilityTimer & 1)
            RSDK.CopyPalette(2, 128, 0, 128, 128);
    }
    else {
        RSDK.SetLimitedFade(0, 1, 4, self->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, self->typeChangeTimer, 128, 256);
    }

    RSDK.DrawSprite(&self->headAnimator, NULL, false);
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    for (int32 i = 0; i < 10; ++i) {
        if (self->armAngles[i] < 0x200) {
            self->particleAnimator.frameID = self->armAngles[i] / 42 % 6;
            RSDK.DrawSprite(&self->particleAnimator, &self->armPositions[i], false);
        }
    }

    self->drawFX = self->storeDrawFX | FX_ROTATE;
    RSDK.DrawSprite(&self->beltAnimator, NULL, false);
    self->drawFX = self->storeDrawFX | FX_ROTATE | FX_FLIP;

    for (int32 i = 0; i < 10; ++i) {
        if (self->armAngles[i] >= 0x200) {
            self->particleAnimator.frameID = self->armAngles[i] / 42 % 6;
            RSDK.DrawSprite(&self->particleAnimator, &self->armPositions[i], false);
        }
    }

    if (self->drawRuby)
        RSDK.DrawSprite(&self->rubyAnimator, &self->rubyPos, false);

    if (self->typeChangeTimer <= 0) {
        if (self->invincibilityTimer & 1)
            RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomKing_Draw_Arm(void)
{
    RSDK_THIS(PhantomKing);

    EntityPhantomKing *parent = self->parent;

    if (parent->typeChangeTimer > 0) {
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 128, 256);
    }

    for (int32 i = 0; i < 6; ++i) RSDK.DrawSprite(&self->armAnimator, &self->armPositions[i], false);

    RSDK.DrawSprite(&self->cuffAnimator, &self->armPositions[6], false);
    RSDK.DrawSprite(&self->handAnimator, &self->armPositions[6], false);

    if (parent->typeChangeTimer > 0) {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomKing_State_Initialize(void)
{
    RSDK_THIS(PhantomKing);

    if (++self->timer >= 8) {
        self->position.y += 0x1000000;
        self->timer  = 0;
        self->active = ACTIVE_NORMAL;
        self->state  = StateMachine_None;
    }
}

void PhantomKing_State_SetupArms(void)
{
    RSDK_THIS(PhantomKing);

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < self->position.x;

    if (++self->timer == 30) {
        EntityPhantomKing *armL = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, PhantomKing);
        RSDK.ResetEntity(armL, PhantomKing->classID, INT_TO_VOID(PHANTOMKING_ARM_L));
        armL->position.x     = self->position.x;
        armL->position.y     = self->position.y;
        armL->parent         = self;
        armL->armRadius      = 0x2800;
        armL->armAngle       = 96;
        armL->armAngleOffset = 32;

        EntityPhantomKing *armR = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, PhantomKing);
        RSDK.ResetEntity(armR, PhantomKing->classID, INT_TO_VOID(PHANTOMKING_ARM_R));
        armR->position.x     = self->position.x;
        armR->position.y     = self->position.y;
        armR->parent         = self;
        armR->armRadius      = 0x2800;
        armR->armAngle       = 96;
        armR->armAngleOffset = 32;

        self->timer     = 0;
        self->visible   = true;
        self->stateDraw = PhantomKing_Draw_Body;
        self->state     = PhantomKing_State_EnterKing;
    }
}

void PhantomKing_State_EnterKing(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->velocity.y -= 0x800;

    if (self->position.y <= self->originPos.y) {
        self->originPos.x                                              = self->position.x;
        self->originPos.y                                              = self->originPos.y;
        self->position.y                                               = self->originPos.y;
        RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, PhantomKing)->state = PhantomKing_StateArm_PullBack;
        self->velocity.y                                               = 0;
        self->state                                                    = PhantomKing_State_InitialHover;
    }
    else {
        self->position.y += self->velocity.y;
    }
    PhantomKing_HandleFrames();
}

void PhantomKing_State_InitialHover(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    PhantomKing_HandleFrames();

    if (++self->timer >= 120) {
        self->timer = 0;
        self->state = PhantomKing_State_TakeRubyAway;
    }
}

void PhantomKing_State_TakeRubyAway(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    if (self->velocity.x < 0x40000)
        self->velocity.x += 0x1800;

    self->position.x += self->velocity.x;

    PhantomKing_HandleFrames();

    if (++self->timer >= 180) {
        RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, PhantomKing)->state = PhantomKing_StateArm_Idle;
        self->velocity.x                                               = 0;
        self->velocity.y                                               = 0;
        self->state                                                    = PhantomKing_State_RubyHoldHover;
        self->finishedMovingRuby                                       = true;
    }
}

void PhantomKing_State_RubyHoldHover(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    PhantomKing_HandleFrames();
}

void PhantomKing_State_WrestleEggman(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    PhantomKing_HandleFrames();
}

void PhantomKing_State_FlyAround(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    PhantomKing_CheckPlayerCollisions();

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    int32 angle = RSDK.ATan2(self->position.x - player1->position.x, self->originPos.y - player1->position.y);
    int32 x     = (RSDK.Cos256(angle) << 15) + player1->position.x;
    int32 y     = (RSDK.Sin256(angle) << 15) + player1->position.y;

    if (x <= self->position.x) {
        if (self->velocity.x > -0x20000)
            self->velocity.x -= 0x800;
    }
    else {
        if (self->velocity.x < 0x20000)
            self->velocity.x += 0x800;
    }

    int32 bottom = Zone->cameraBoundsB[0] << 16;
    if (y > bottom - 0x400000)
        y = bottom - 0x800000;

    if (y <= self->originPos.y) {
        if (self->velocity.y > -0x20000)
            self->velocity.y -= 0x800;
    }
    else {
        if (self->velocity.y < 0x20000)
            self->velocity.y += 0x800;
    }

    self->originPos.y += self->velocity.y;
    self->position.x += self->velocity.x;
    self->direction = player1->position.x <= self->position.x;

    if (self->canLaunchAttacks) {
        if (++self->timer >= 60) {
            self->timer = 0;
            PhantomKing_HandleAttacks();
        }
    }
    else if (self->onScreen == 1) {
        self->canLaunchAttacks = true;
    }

    PhantomKing_HandleFrames();
}

void PhantomKing_State_Switch(void)
{
    RSDK_THIS(PhantomKing);

    if (++self->timer >= 120) {
        self->timer = 0;
        PhantomKing_SwitchToEggman();
        self->state = StateMachine_None;
    }
}

void PhantomKing_State_HitFall(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    if (self->position.y < -0x1000000) {
        foreach_active(PKingAttack, attack)
        {
            if (attack->state == PKingAttack_State_Orbiting)
                destroyEntity(attack);
        }

        self->canLaunchAttacks = false;
        self->velocity.y       = 0;
        self->timer            = 0;
        self->state            = PhantomKing_State_Switch;
    }

    PhantomKing_HandleFrames();
}

void PhantomKing_StateArm_Idle(void)
{
    RSDK_THIS(PhantomKing);

    EntityPhantomKing *parent = self->parent;

    int32 parentX = parent->position.x;
    int32 parentY = parent->position.y;

    int32 moveX = 0;
    int32 moveY = ((RSDK.Sin256(2 * (Zone->timer + (self->type << 6)) - 128) + 512) << 12) + parentY;

    self->direction = parent->direction;
    int32 negAngle  = -parent->rotation;

    int32 x = 0;
    int32 y = 0;

    int32 x2 = 0;
    int32 y2 = 0;

    if (self->direction) {
        moveX = parentX - 0x300000;
        x     = 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle) + parent->position.x;
        y     = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);

        if (self->type == PHANTOMKING_ARM_L) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
            moveX -= 0x300000;
        }

        x2 = ((self->position.x + x) >> 1) + 0x200000;
        y2 = ((self->position.y + y) >> 1) + 0x200000;
    }
    else {
        moveX = parentX + 0x300000;
        x     = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y     = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;

        if (self->type == PHANTOMKING_ARM_L) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
            moveX += 0x300000;
        }

        x2 = ((self->position.x + x) >> 1) - 0x100000;
        y2 = ((self->position.y + y) >> 1) + 0x100000;
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

    RSDK.ProcessAnimation(&self->cuffAnimator);
    RSDK.ProcessAnimation(&self->handAnimator);
}

void PhantomKing_StateArm_WrestleEggman(void)
{
    RSDK_THIS(PhantomKing);

    EntityPhantomKing *parent = self->parent;

    self->direction = parent->direction;
    int32 negAngle  = -parent->rotation;

    int32 x = 0;
    int32 y = 0;

    if (self->direction) {
        x = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);

        if (self->type == PHANTOMKING_ARM_L) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
        }
    }
    else {
        x = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;

        if (self->type == PHANTOMKING_ARM_L) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
        }
    }

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, self->armBezierPos.x, self->armBezierPos.y, self->armBezierPos.x,
                                                           self->armBezierPos.y, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->cuffAnimator);
    RSDK.ProcessAnimation(&self->handAnimator);
}

void PhantomKing_HandleArmMovement(void)
{
    RSDK_THIS(PhantomKing);

    EntityPhantomKing *parent = self->parent;

    RSDK.Sin256(2 * (Zone->timer + (self->type << 6)) - 128);
    ++self->timer;
    self->direction = parent->direction;
    int32 negAngle  = -parent->rotation;

    int32 x = 0;
    int32 y = 0;

    int32 x2 = 0;
    int32 y2 = 0;

    int32 x3 = 0;
    int32 y3 = 0;

    if (self->direction) {
        x = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);

        if (self->type == PHANTOMKING_ARM_L) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
        }

        x2 = x + self->armRadius * RSDK.Cos256(self->armAngle);
        y2 = y + self->armRadius * RSDK.Sin256(self->armAngle);
        x3 = x2 + self->armRadius * RSDK.Cos256(self->armAngleOffset + self->armAngle);
        y3 = y2 + self->armRadius * RSDK.Sin256(self->armAngleOffset + self->armAngle);
    }
    else {
        x = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;

        if (self->type == PHANTOMKING_ARM_L) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
        }

        x2 = x + self->armRadius * RSDK.Cos256(self->armAngle);
        y2 = y + self->armRadius * RSDK.Sin256(self->armAngle);
        x3 = x2 + self->armRadius * RSDK.Cos256(self->armAngleOffset + self->armAngle);
        y3 = y2 + self->armRadius * RSDK.Sin256(self->armAngleOffset + self->armAngle);
    }

    self->velocity.x += ((x3 - self->position.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += ((y3 - self->position.y) >> 5) - (self->velocity.y >> 3);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, x2, y2, x2, y2, x3, y3);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->cuffAnimator);
    RSDK.ProcessAnimation(&self->handAnimator);
}

void PhantomKing_StateArm_PullBack(void)
{
    RSDK_THIS(PhantomKing);

    PhantomKing_HandleArmMovement();

    uint8 armFinished = 0;

    if (self->armAngle >= 0xC0)
        armFinished = 1;
    else
        self->armAngle += 3;

    if (self->armAngleOffset >= 0x60)
        ++armFinished;
    else
        self->armAngleOffset += 3;

    if (armFinished == 2)
        self->state = PhantomKing_StateArm_Point;
}

void PhantomKing_StateArm_Point(void)
{
    RSDK_THIS(PhantomKing);

    PhantomKing_HandleArmMovement();

    if (self->armAngle <= 0x80)
        self->armAngle = 0x80;
    else
        self->armAngle -= 0x10;

    if (self->armAngleOffset <= 0x00)
        self->armAngleOffset = 0x00;
    else
        self->armAngleOffset -= 0x10;
}

void PhantomKing_DestroyEntity(void)
{
    RSDK_THIS(PhantomKing);

    int32 slot              = RSDK.GetEntitySlot(self);
    EntityPhantomKing *armL = RSDK_GET_ENTITY(slot - 1, PhantomKing);
    EntityPhantomKing *armR = RSDK_GET_ENTITY(slot + 1, PhantomKing);

    foreach_all(PKingAttack, attack) { destroyEntity(attack); }

    destroyEntity(armL);
    destroyEntity(armR);
    destroyEntity(self);
}

void PhantomKing_State_Explode(void)
{
    RSDK_THIS(PhantomKing);

    PhantomKing_Explode();

    if (++self->timer == 60)
        self->state = PhantomKing_State_Destroyed;
}

void PhantomKing_State_Destroyed(void)
{
    RSDK_THIS(PhantomKing);

    self->velocity.y += 0x2800;
    self->position.y += self->velocity.y;

    PhantomKing_Explode();

    if (self->position.y >= 0x2800000) {
        PhantomKing->defeated = true;

        if (!KleptoMobile->defeated) {
            PhantomKing_SwitchToEggman();
        }
        else {
            foreach_all(ERZOutro, outro)
            {
                outro->active = ACTIVE_NORMAL;
                foreach_break;
            }
        }

        PhantomKing_DestroyEntity();
    }

    PhantomKing_HandleFrames();
}

#if RETRO_INCLUDE_EDITOR
void PhantomKing_EditorDraw(void)
{
    RSDK_THIS(PhantomKing);

    self->originPos          = self->position;
    self->rotation           = 0;
    self->bodyAngle          = 0;
    self->bodyAnimator.speed = 0;
    PhantomKing_HandleFrames();

    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 0, &self->headAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 1, &self->bodyAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 2, &self->beltAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 7, &self->particleAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &self->rubyAnimator, false, 0);

    PhantomKing_Draw_Body();
}

void PhantomKing_EditorLoad(void)
{
    PhantomKing->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PhantomKing, type);
    RSDK_ENUM_VAR("King", PHANTOMKING_KING);
}
#endif

void PhantomKing_Serialize(void) { RSDK_EDITABLE_VAR(PhantomKing, VAR_ENUM, type); }
