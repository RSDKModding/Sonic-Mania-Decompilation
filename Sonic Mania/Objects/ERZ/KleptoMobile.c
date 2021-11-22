#include "SonicMania.h"

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
        RSDK.DrawSprite(&self->animator1, NULL, false);
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
            self->drawOrder       = Zone->drawOrderLow;
            self->drawFX          = FX_FLIP;
            self->explosionVolume = 0x200;
            self->type            = voidToInt(data);
            switch (self->type) {
                case 0:
                    self->hitbox.left   = -22;
                    self->hitbox.top    = -22;
                    self->hitbox.right  = 0x16;
                    self->hitbox.bottom = 0x16;
                    self->active        = ACTIVE_NORMAL;
                    self->health        = 8;
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->animator2, true, 1);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->animator3, true, 0);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->animator4, true, 0);
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &self->animator11, true, 0);
                    KleptoMobile->defeated = false;
                    self->stateDraw      = KleptoMobile_StateDraw_Unknown1;
                    self->state          = KleptoMobile_State_SetupArena;
                    break;
                case 1:
                case 2:
                    self->hitbox.left   = -32;
                    self->hitbox.top    = -8;
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->hitbox.right  = 32;
                    self->hitbox.bottom = 8;
                    if (self->type == 2) {
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 7, &self->animator7, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 8, &self->animator8, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 9, &self->animator9, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 10, &self->animator10, true, 0);
                    }
                    else {
                        self->drawOrder = Zone->playerDrawLow;
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 3, &self->animator7, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 4, &self->animator8, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 5, &self->animator9, true, 0);
                        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 6, &self->animator10, true, 0);
                    }
                    self->stateDraw = KleptoMobile_StateDraw1_Unknown1;
                    self->state     = KleptoMobile_State1_Unknown1;
                    break;
                case 3:
                    self->active  = ACTIVE_NORMAL;
                    self->visible = true;
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 12, &self->animator8, true, 5);
                    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 17, &self->animator7, true, 0);
                    self->stateDraw = KleptoMobile_StateDraw3_Unknown1;
                    self->state     = KleptoMobile_State3_Unknown1;
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
    KleptoMobile->aniFrames    = RSDK.LoadSpriteAnimation("Phantom/KleptoMobile.bin", SCOPE_STAGE);
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

    bool32 flag = false;
    RSDK.ProcessAnimation(&self->animator3);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->animator3.animationID == 13) {
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == ERZStart_State_PlayerRebound)
                flag = true;
        }
        if (flag == true)
            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 14, &self->animator3, true, 0);
    }
    else if (self->animator3.animationID == 14) {
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == ERZStart_State_PlayerRebound)
                flag = true;
        }

        if (self->animator3.frameID >= 14) {
            if (flag)
                RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 14, &self->animator3, true, 7);
            else
                RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->animator3, true, 0);
        }
    }
    else {
        if (self->animator3.animationID == 15 && !self->invincibilityTimer)
            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->animator3, true, 0);
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
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 16, &self->animator3, true, 0);
        self->state = KleptoMobile_State_Destroyed;
        self->timer = 0;
        if (PhantomKing->defeated)
            SceneInfo->timeEnabled = false;

        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 15, &self->animator3, true, 0);
        RSDK.PlaySfx(KleptoMobile->sfxHit, false, 255);

        foreach_all(PhantomRuby, ruby)
        {
            ruby->position.x = self->circlePos.x;
            ruby->position.y = self->circlePos.y;
            ruby->velocity.x = -0x20000;
            ruby->velocity.y = -0x40000;
            ruby->state      = PhantomRuby_State_FallOffScreen;

            EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, KleptoMobile);
            arm1->timer              = 0;
            arm1->state              = KleptoMobile_State1_Unknown1;

            EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, KleptoMobile);
            arm2->timer              = 0;
            arm2->state              = KleptoMobile_State1_Unknown1;
        }

        self->field_120 = false;
        self->state     = KleptoMobile_State_HitFall;
    }
}

void KleptoMobile_HandleFrames(void)
{
    RSDK_THIS(KleptoMobile);

    self->rotation = RSDK.Sin512(2 * Zone->timer) >> 6;
    self->angle2   = (self->angle2 + 12) & 0x3FF;

    int moveX = 0x1C00 * RSDK.Sin512(-self->rotation) + self->position.x;
    int moveY = 0x1C00 * RSDK.Cos512(-self->rotation) + self->position.y;

    int angle = self->angle2;

    for (int i = 0; i < 10; i += 2) {
        self->framePositions[i].x = moveX + 2 * RSDK.Cos1024(angle) * RSDK.Cos512(self->rotation);
        self->framePositions[i].y = moveY + 2 * RSDK.Cos1024(angle) * RSDK.Sin512(self->rotation);
        self->frameIDs[i]         = angle & 0x3FF;
        angle += 0x200;

        self->framePositions[i + 1].x = moveX + 2 * RSDK.Cos1024(angle) * RSDK.Cos512(self->rotation);
        self->framePositions[i + 1].y = moveY + 2 * RSDK.Cos1024(angle) * RSDK.Sin512(self->rotation);
        self->frameIDs[i + 1]         = angle & 0x3FF;

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
        if (!king->type) {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            king->direction       = FLIP_X;
            if (RSDK.Rand(0, 2))
                king->position.x = player1->position.x + 0x1000000;
            else
                king->position.x = player1->position.x - 0x1000000;
            king->position.y   = player1->position.y + 0x800000;
            king->posUnknown.y = player1->position.y + 0x800000;
            king->rubyPos      = king->position;
            king->rubyPos.x -= 0x1400 * RSDK.Sin512(-king->rotation);
            king->rubyPos.y -= 0x1400 * RSDK.Cos512(-king->rotation);
            king->velocity.x = 0;
            king->velocity.y = 0;
            king->drawRuby  = true;

            for (int i = 0; i < 0x3FC; i += 0xAA) {
                EntityPKingAttack *attack = CREATE_ENTITY(PKingAttack, intToVoid(1), king->rubyPos.x, king->rubyPos.y);
                attack->angle             = i;
                attack->target            = (Entity *)king;
            }

            king->state = PhantomKing_State_Unknown8;
            kingPtr     = king;
            foreach_break;
        }
    }

    foreach_active(PhantomKing, kingArm)
    {
        if (kingArm->type) {
            kingArm->position.x = kingPtr->position.x;
            kingArm->position.y = kingPtr->position.y;
            for (int i = 0; i < 10; ++i) kingArm->framePositions[i] = kingPtr->position;
            kingArm->velocity.x = 0;
            kingArm->velocity.y = 0;
            kingArm->state      = PhantomKing_StateArm1_Unknown2;
        }
    }
}

void KleptoMobile_StateDraw_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 112, 0, 136, 8);
        RSDK.CopyPalette(2, 120, 0, 11, 1);
        RSDK.CopyPalette(2, 121, 0, 25, 1);
        RSDK.CopyPalette(2, 122, 0, 197, 6);

        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);

        self->animator4.frameID = 2;
        RSDK.DrawSprite(&self->animator4, NULL, false);

        self->animator4.frameID = 0;
        RSDK.DrawSprite(&self->animator4, NULL, false);

        RSDK.CopyPalette(1, 136, 0, 136, 8);
        RSDK.CopyPalette(1, 11, 0, 11, 1);
        RSDK.CopyPalette(1, 25, 0, 25, 1);
        RSDK.CopyPalette(1, 197, 0, 197, 6);
    }
    else {
        self->animator1.frameID = 1;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator3, NULL, false);

        self->animator4.frameID = 2;
        RSDK.DrawSprite(&self->animator4, NULL, false);

        self->animator4.frameID = 0;
        RSDK.DrawSprite(&self->animator4, NULL, false);
    }

    RSDK.DrawCircle(self->circlePos.x, self->circlePos.y, self->circleRadius, 0, 255, INK_LOOKUP, false);
}

void KleptoMobile_State_SetupArena(void)
{
    RSDK_THIS(KleptoMobile);

    self->active = ACTIVE_NORMAL;

    EntityKleptoMobile *hand = RSDK_GET_ENTITY(SceneInfo->entitySlot - 2, KleptoMobile);
    RSDK.ResetEntityPtr(hand, KleptoMobile->objectID, intToVoid(3));
    hand->position.x = self->position.x;
    hand->position.y = self->position.y;
    hand->parent     = (Entity *)self;

    EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, KleptoMobile);
    RSDK.ResetEntityPtr(arm2, KleptoMobile->objectID, intToVoid(2));
    arm2->position.x = self->position.x;
    arm2->position.y = self->position.y;
    arm2->parent     = (Entity *)self;

    EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, KleptoMobile);
    RSDK.ResetEntityPtr(arm1, KleptoMobile->objectID, intToVoid(1));
    arm1->position.x = self->position.x;
    arm1->position.y = self->position.y;
    arm1->parent     = (Entity *)self;

    self->field_70.x    = self->position.x;
    self->field_70.y    = self->position.y;
    KleptoMobile->boundsM = self->position.x;
    KleptoMobile->boundsL = KleptoMobile->boundsM - 0x800000;
    KleptoMobile->boundsR = KleptoMobile->boundsM + 0x800000;
    KleptoMobile->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
    KleptoMobile->boundsB = (Zone->cameraBoundsB[0] - 96) << 16;
    self->state         = StateMachine_None;
}

void KleptoMobile_State_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->field_70.y) & 0xFFFF0000;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->field_70.x += self->velocity.x;
    self->field_70.y += self->velocity.y;
    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_Unknown2(void)
{
    RSDK_THIS(KleptoMobile);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->field_70.y) & 0xFFFF0000;
    KleptoMobile_CheckPlayerCollisions();

    int angle = RSDK.ATan2(self->position.x - player1->position.x, self->field_70.y - player1->position.y);
    int x     = (RSDK.Cos256(angle) << 15) + player1->position.x;
    int y     = (RSDK.Sin256(angle) << 15) + player1->position.y;

    if (self->circleRadius > 0)
        self->circleRadius -= 8;

    if (x <= self->position.x) {
        if (self->velocity.x > -0x20000) {
            self->velocity.x -= 0x800;
        }
    }
    else if (self->velocity.x < 0x20000) {
        self->velocity.x += 0x800;
    }

    int bottom   = Zone->cameraBoundsB[0] << 16;
    int boundary = bottom - 0x800000;
    if (y <= bottom - 0x800000) {
        boundary = bottom + 0x800000;
        if (y >= (Zone->cameraBoundsT[0] + 128) << 16)
            boundary = y;
    }

    if (boundary <= self->field_70.y) {
        if (self->velocity.y > -0x20000)
            self->velocity.y -= 0x800;
    }
    else {
        if (self->velocity.y < 0x20000)
            self->velocity.y += 0x800;
    }

    self->position.x += self->velocity.x;
    self->field_70.y += self->velocity.y;
    self->direction = player1->position.x > self->position.x;
    if (--self->field_10C == 1) {
        self->field_10C = 0;
        self->timer     = 0;
        RSDK.PlaySfx(KleptoMobile->sfxPowerUp, false, 255);
        EntityKleptoMobile *arm2 = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, KleptoMobile);
        arm2->timer              = 0;
        arm2->state              = KleptoMobile_State1_Unknown4;

        EntityKleptoMobile *arm1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, KleptoMobile);
        arm1->timer              = 0;
        arm1->state              = KleptoMobile_State1_Unknown4;
        self->state            = KleptoMobile_State_Unknown3;
    }
    else {
        if (self->field_114) {
            if (self->field_10C <= 0 && ++self->timer == 15) {
                self->timer = 0;
                RSDK.PlaySfx(KleptoMobile->sfxFlail, false, 0xFF);
                int armSlot = self->field_110 ? SceneInfo->entitySlot + 1 : SceneInfo->entitySlot - 1;

                EntityKleptoMobile *arm = RSDK_GET_ENTITY(armSlot, KleptoMobile);

                x = self->direction == FLIP_X ? self->position.x - 0x30000 : self->position.x + 0x30000;
                if (arm->type == 2) {
                    x += 0x180000;
                }
                y = self->position.y + 0xD0000;

                angle           = RSDK.ATan2(player1->position.x - x, player1->position.y - y);
                arm->field_F8.x = x + (RSDK.Cos256(angle) << 15);
                arm->field_F8.y = y + (RSDK.Sin256(angle) << 15);
                arm->timer      = 0;
                arm->state      = KleptoMobile_State1_Unknown3;
                self->field_110 ^= 1;
                if (++self->field_108 >= 5) {
                    self->field_108 = 0;
                    self->field_10C = 90;
                    self->field_114 = false;
                }
            }
        }
        else {
            if (abs(player1->position.x - self->position.x) < 0x1800000 && abs(player1->position.y - self->position.y) < 0x1800000)
                self->field_114 = true;
        }
        KleptoMobile_HandleFrames();
    }
}

void KleptoMobile_State_Unknown3(void)
{
    RSDK_THIS(KleptoMobile);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->field_70.y) & 0xFFFF0000;

    KleptoMobile_CheckPlayerCollisions();
    if (++self->timer < 180) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (abs(player1->position.x - self->position.x) < 0xC00000 && abs(player1->position.y - self->position.y) < 0xC00000) {
            RSDK.StopSfx(KleptoMobile->sfxPowerUp);
            self->timer     = 14;
            self->field_10C = 0;
            self->field_114 = true;
            self->state     = KleptoMobile_State_Unknown2;
        }
    }
    else {
        self->timer = 0;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0xA0000;
        else
            self->velocity.x = 0xA0000;
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
        RSDK.PlaySfx(KleptoMobile->sfxRocketJet, false, 255);
        self->state = KleptoMobile_State_Unknown5;
    }
    KleptoMobile_HandleFrames();
}

void KleptoMobile_HandleArmPositions(void)
{
    foreach_active(KleptoMobile, eggman)
    {
        if (eggman->type) {
            EntityKleptoMobile *parent = (EntityKleptoMobile *)eggman->parent;
            eggman->position.x         = parent->position.x;
            eggman->position.y         = parent->position.y;
            for (int i = 0; i < 10; ++i) eggman->framePositions[i] = parent->position;
            eggman->velocity.x = 0;
            eggman->velocity.y = 0;
        }
    }
}

void KleptoMobile_State_Unknown4(void)
{
    RSDK_THIS(KleptoMobile);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->timer = 0;

    PhantomRuby_PlaySFX(RSDK.Rand(RUBYSFX_ATTACK1, RUBYSFX_REDCUBE));
    if (!RSDK.Rand(0, 2))
        self->position.x = player1->position.x - 0x1800000;
    else
        self->position.x = player1->position.x + 0x1800000;

      do {
        do {
            self->field_70.y = (RSDK.Rand(-2, 3) << 21) + player1->position.y;
        } while (self->field_70.y > (Zone->cameraBoundsB[0] - 64) << 16);
      } while (self->field_70.y < (Zone->cameraBoundsT[0] + 64) << 16);

    self->circleRadius = 128;
    if (++self->field_108 == 4) {
        self->field_108  = 0;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = KleptoMobile_State_Unknown2;
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

void KleptoMobile_State_Unknown5(void)
{
    RSDK_THIS(KleptoMobile);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->field_70.y) & 0xFFFF0000;

    if (self->circleRadius < 128)
        self->circleRadius += 8;

    if (++self->timer >= 180) {
        KleptoMobile_State_Unknown4();
        self->state = KleptoMobile_State_Unknown6;
    }
    self->position.x += self->velocity.x;
    self->field_70.y += self->velocity.y;
    KleptoMobile_HandleFrames();
}

void KleptoMobile_State_Unknown6(void)
{
    RSDK_THIS(KleptoMobile);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 9) + self->field_70.y) & 0xFFFF0000;

    KleptoMobile_CheckPlayerCollisions();
    if (self->activeScreens) {
        if (self->circleRadius > 0)
            self->circleRadius -= 8;
    }
    if (++self->timer >= 180)
        KleptoMobile_State_Unknown4();
    self->position.x += self->velocity.x;
    self->field_70.y += self->velocity.y;
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
        self->field_114  = false;
        self->field_10C  = 0;
        self->velocity.y = 0;
        self->timer      = 0;
        self->state      = KleptoMobile_State_Switch;
    }
    KleptoMobile_HandleFrames();
}

void KleptoMobile_State3_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)self->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX = 0;
    int moveY = -0x200000 - (RSDK.Sin256((((self->type << 7) - 0x80) ^ 0x80) + 2 * Zone->timer) << 12);

    self->direction = parent->direction;
    int x             = parent->position.x;
    int y             = parent->position.y - 0x180000;

    int x2 = 0;
    int y2 = 0;
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

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    parent->circlePos.x = self->framePositions[6].x;
    parent->circlePos.y = self->framePositions[6].y;

    if (self->direction)
        parent->circlePos.x += 0x80000;
    else
        parent->circlePos.x -= 0x80000;
    parent->circlePos.y += 0x80000;
}

void KleptoMobile_State3_Unknown2(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)self->parent;

    self->direction = parent->direction;
    int parentX       = parent->position.x;
    int parentY       = parent->position.y - 0x180000;

    int x = 0;
    if (self->direction == FLIP_X)
        x = ((parent->position.x + self->position.x) >> 1) + 0x80000;
    else
        x = ((parent->position.x + self->position.x) >> 1) - 0x80000;
    int y = ((parentY + self->position.y) >> 1) - 0x300000;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, parentX, parentY, x, y, x, y, self->position.x, self->position.y);
        percent += 0x2000;
    }

    parent->circlePos.x = self->framePositions[6].x;
    parent->circlePos.y = self->framePositions[6].y;

    if (self->direction)
        parent->circlePos.x += 0x80000;
    else
        parent->circlePos.x -= 0x80000;
    parent->circlePos.y += 0x80000;
}

void KleptoMobile_StateDraw3_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)self->parent;

    if (parent->field_120)
        RSDK.DrawSprite(&parent->animator11, &parent->circlePos, false);

    for (int i = 0; i < 6; ++i) RSDK.DrawSprite(&self->animator7, &self->framePositions[i], false);

    RSDK.DrawSprite(&self->animator8, &self->framePositions[6], false);
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
                    if (self->state == KleptoMobile_State1_Unknown4) {
                        player->velocity.x = self->parent->velocity.x >> 2;
                        player->groundVel  = player->velocity.x;
                        if (player->position.y <= self->position.y || player->onGround == true) {
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
                Player_CheckHit(player, self);
            }
        }
    }
}

void KleptoMobile_State1_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)self->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX         = 0;
    int moveY         = ((RSDK.Sin256((((self->type << 7) - 128) ^ 0x80) + 2 * Zone->timer) + 512) << 12) + parentY;
    self->direction = parent->direction;

    int x = 0, y = 0;
    int x2 = 0, y2 = 0;

    if (self->direction == FLIP_X) {
        moveX = parentX + 0x300000;
        x     = parentX - 0x30000;
        y     = parentY + 0xD0000;
        if (self->type == 2) {
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
        if (self->type == 2) {
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

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->animator9);
    RSDK.ProcessAnimation(&self->animator10);
}

void KleptoMobile_State1_Unknown2(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)self->parent;

    self->direction = parent->direction;

    int x = 0;
    int y = parent->position.y + 0x60000;
    if (self->direction == FLIP_X) {
        x = parent->position.x - 0x90000;
        if (self->type == 2)
            x += 0x180000;
    }
    else {
        x = parent->position.x + 0x90000;
        if (self->type == 2)
            x -= 0x180000;
    }

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, self->field_F0.x, self->field_F0.y, self->field_F0.x,
                                                         self->field_F0.y, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->animator9);
    RSDK.ProcessAnimation(&self->animator10);
}

void KleptoMobile_State1_Unknown3(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)self->parent;

    if (!self->timer) {
        self->field_100.x = self->position.x;
        self->field_100.y = self->position.y;
    }

    self->direction = parent->direction;

    int x = 0;
    int y = parent->position.y + 0xD0000;

    int x2 = 0, y2 = 0;
    int x3 = 0, y3 = 0;

    if (self->direction == FLIP_X) {
        x = parent->position.x - 0x30000;
        if (self->type == 2)
            x += 0x180000;
    }
    else {
        x = parent->position.x + 0x30000;
        if (self->type == 2)
            x -= 0x180000;
    }

    if (self->timer >= 4) {
        x2                 = x;
        y2                 = y;
        x3                 = self->position.x;
        y3                 = self->position.y;
        self->position.x = self->field_F8.x;
        self->position.y = self->field_F8.y;
    }
    else {
        self->velocity.x = (self->field_F8.x - self->field_100.x) / 4;
        self->velocity.y = (self->field_F8.y - self->field_100.y) / 4;

        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        int distX = self->position.x - x;
        int distY = self->position.y - y;

        int distValX = -distX;
        int distValY = distY;
        if (distX > 0) {
            distValX = distX;
            distValY = -distY;
        }

        int timerVal = 4 - self->timer;
        x2           = x + ((x + (distX >> 1) + distValY) - x) * timerVal / 4;
        y2           = y + (((distY >> 1) + distValX) * timerVal) / 4;
        x3           = self->position.x + ((x + (distX >> 1) + distValY) - self->position.x) * timerVal / 4;
        y3           = self->position.y + (((distY >> 1) + distValX - self->position.y) * timerVal / 4);
    }

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x3, y3, self->position.x, self->position.y);
        percent += 0x2000;
    }

    if (++self->timer > 15) {
        self->timer = 0;
        self->state = KleptoMobile_State1_Unknown1;
    }
    RSDK.ProcessAnimation(&self->animator9);
    RSDK.ProcessAnimation(&self->animator10);

    if (parent->health > 0)
        KleptoMobile_CheckPlayerCollisions_Arm();
}

void KleptoMobile_State1_Unknown4(void)
{
    RSDK_THIS(KleptoMobile);
    EntityKleptoMobile *parent = (EntityKleptoMobile *)self->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX = 0;
    int moveY = parentY + 0x180000;
    if (self->type == 2)
        moveY = parentY - 0x180000;

    moveX = parentX - 0x400000;
    if (self->direction == FLIP_X)
        moveX = parentX + 0x400000;

    int x = 0;
    int y = parent->position.y + 0xD0000;
    if (self->direction == FLIP_X) {
        moveX += 0x300000;
        x = parent->position.x - 0x30000;
        if (self->type == 2) {
            x += 0x180000;
            moveX += 0x300000;
        }
    }
    else {
        moveX -= 0x300000;
        x = parent->position.x + 0x30000;
        if (self->type == 2) {
            x -= 0x180000;
            moveX -= 0x300000;
        }
    }

    int x2 = (x + self->position.x) >> 1;
    int y2 = (y + self->position.y) >> 1;

    self->velocity.x += ((moveX - self->position.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += ((moveY - self->position.y) >> 5) - (self->velocity.y >> 3);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->animator9);
    RSDK.ProcessAnimation(&self->animator10);

    if (parent->health > 0)
        KleptoMobile_CheckPlayerCollisions_Arm();
}

void KleptoMobile_StateDraw1_Unknown1(void)
{
    RSDK_THIS(KleptoMobile);

    for (int i = 0; i < 6; ++i) RSDK.DrawSprite(&self->animator7, &self->framePositions[i], false);

    RSDK.DrawSprite(&self->animator8, &self->framePositions[6], false);

    if (self->direction) {
        if (self->type == 2) {
            self->framePositions[6].x += 0x280000;
            self->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);

            self->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);

            self->framePositions[6].x -= 0x40000;
            self->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->animator10, &self->framePositions[6], false);
        }
        else {
            self->framePositions[6].x += 0x280000;
            RSDK.DrawSprite(&self->animator10, &self->framePositions[6], false);

            self->framePositions[6].x -= 0x40000;
            self->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);

            self->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);

            self->framePositions[6].y -= 0x80000;
        }
    }
    else {
        if (self->type == 2) {
            self->framePositions[6].x -= 0x280000;
            self->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);
            self->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);
            self->framePositions[6].x += 0x40000;
            self->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->animator10, &self->framePositions[6], false);
        }
        else {
            self->framePositions[6].x -= 0x280000;
            RSDK.DrawSprite(&self->animator10, &self->framePositions[6], false);

            self->framePositions[6].x += 0x40000;
            self->framePositions[6].y -= 0x80000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);

            self->framePositions[6].y += 0x100000;
            RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);

            self->framePositions[6].y -= 0x80000;
        }
    }
}

void KleptoMobile_State_Destroyed(void)
{
    RSDK_THIS(KleptoMobile);

    RSDK.ProcessAnimation(&self->animator3);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(KleptoMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++self->timer == 96) {
        Debris_FallFlickerAnimSetup(KleptoMobile->aniFrames, KleptoMobile->debrisInfo, 18);
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

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(KleptoMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
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

    RSDK.ProcessAnimation(&self->animator3);
    self->field_70.x += self->velocity.x;
    self->field_70.y += self->velocity.y;
    self->position.x = self->field_70.x;
    self->position.y = self->field_70.y;

    if (self->explosionVolume > 0) {
        if (!(Zone->timer % 3)) {
            int channel = RSDK.PlaySfx(KleptoMobile->sfxExplosion, false, 255);
            RSDK.SetChannelAttributes(channel, self->explosionVolume * (1 / 512.0f), 0.0, 1.0);
            if ((Zone->timer & 4) != 0) {
                int x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
                int y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
        if (self->timer >= 60)
            self->explosionVolume -= 2;
        self->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void KleptoMobile_EditorDraw(void)
{
    RSDK_THIS(KleptoMobile);
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->animator2, false, 1);
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &self->animator3, false, 0);
    RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 0, &self->animator4, false, 0);

    KleptoMobile_StateDraw_Unknown1();
}

void KleptoMobile_EditorLoad(void) { KleptoMobile->aniFrames = RSDK.LoadSpriteAnimation("Phantom/KleptoMobile.bin", SCOPE_STAGE); }
#endif

void KleptoMobile_Serialize(void) { RSDK_EDITABLE_VAR(KleptoMobile, VAR_ENUM, type); }
