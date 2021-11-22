#include "SonicMania.h"

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
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void PhantomKing_Create(void *data)
{
    RSDK_THIS(PhantomKing);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x1000000;
        self->drawOrder     = Zone->drawOrderLow;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->type          = voidToInt(data);

        switch (self->type) {
            case 0: // main
                self->hitbox.left   = -24;
                self->hitbox.top    = -24;
                self->hitbox.right  = 24;
                self->hitbox.bottom = 24;
                self->active        = ACTIVE_BOUNDS;
                self->visible       = false;
                self->direction     = FLIP_X;
                self->health        = 8;
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 0, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 1, &self->animator3, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 2, &self->animator4, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 7, &self->animator5, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &self->animatorRuby, true, 0);
                self->posUnknown = self->position;
                self->state      = PhantomKing_State_Unknown1;
                break;
            case 1: // body
            case 2: // body
                self->active  = ACTIVE_NORMAL;
                self->visible = true;
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 3, &self->animator8, true, 0);
                RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 4, &self->animator9, true, 0);
                if (self->type == 1) {
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 6, &self->animator10, true, 0);
                }
                else {
                    self->drawOrder = Zone->playerDrawLow;
                    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 5, &self->animator10, true, 0);
                }
                self->stateDraw = PhantomKing_StateDraw_Arm;
                self->state     = PhantomKing_StateArm1_Unknown2;
                break;
        }
    }
}

void PhantomKing_StageLoad(void)
{
    PhantomKing->aniFrames    = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);
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
        ruby->state      = PhantomRuby_State_FallOffScreen;
    }

    self->drawRuby = false;
    if (--self->health <= 0) {
        self->posUnknown.x = self->position.x;
        self->posUnknown.y = self->position.y;
        self->state        = PhantomKing_State_Explode;
        self->velocity.y   = -0x10000;
        self->timer        = 0;
        foreach_active(PKingAttack, attack)
        {
            if (attack->state == PKingAttack_Unknown3)
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

void PhantomKing_HandleFrames(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->animator3);

    if (self->state != PhantomKing_State_Unknown7) {
        int max = self->velocity.x >> 15;
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

    int negAng     = -self->rotation;
    self->angle2 = (self->angle2 + 12) & 0x3FF;

    int x = 0x1C00 * RSDK.Sin512(negAng) + self->position.x;
    int y = 0x1C00 * RSDK.Cos512(negAng) + self->position.y;

    int angle = self->angle2;

    for (int i = 0; i < 10; i += 2) {
        self->framePositions[i].x = x + 2 * RSDK.Cos512(self->rotation) * RSDK.Cos1024(angle);
        self->framePositions[i].y = y + 2 * RSDK.Sin512(self->rotation) * RSDK.Cos1024(angle);
        self->frameIDs[i]         = angle & 0x3FF;

        angle += 512;

        self->framePositions[i + 1].x = x + 2 * RSDK.Cos512(self->rotation) * RSDK.Cos1024(angle);
        self->framePositions[i + 1].y = y + 2 * RSDK.Sin512(self->rotation) * RSDK.Cos1024(angle);
        self->frameIDs[i + 1]         = angle & 0x3FF;

        x += RSDK.Sin512(negAng) << 10;
        y += RSDK.Cos512(negAng) << 10;
        angle += 0x240;
    }

    self->rubyPos.x = self->position.x;
    self->rubyPos.y = self->position.y;
    self->rubyPos.x -= 0x1400 * RSDK.Sin512(negAng);
    self->rubyPos.y -= 0x1400 * RSDK.Cos512(negAng);
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

    EntityPlayer *player = Player_GetNearestPlayer();
    bool32 flag          = false;

    foreach_active(PKingAttack, attack)
    {
        if (attack->state == PKingAttack_Unknown3) {
            PhantomRuby_PlaySFX(RSDK.Rand(RUBYSFX_ATTACK1, RUBYSFX_REDCUBE));
            attack->type = 2;
            int angle    = RSDK.ATan2((player->position.x - attack->position.x) >> 16, (player->position.y - attack->position.y) >> 16);

            attack->field_70.x = 0x600 * RSDK.Cos256(angle);
            attack->field_70.y = 0x600 * RSDK.Sin256(angle);
            attack->drawOrder  = Zone->drawOrderLow;
            attack->state      = PKingAttack_Unknown4;
            flag               = true;
            foreach_break;
        }
    }

    if (!flag) {
        for (int i = 0; i < 0x3FC; i += 0xAA) {
            EntityPKingAttack *attack = CREATE_ENTITY(PKingAttack, intToVoid(1), self->position.x, self->position.y);
            attack->angle             = i;
            attack->target            = (Entity *)self;
        }
        self->timer = -90;
    }
}

void PhantomKing_SwitchToEggman(void)
{
    RSDK_THIS(PhantomKing);

    EntityKleptoMobile *eggmanPtr = NULL;

    foreach_active(KleptoMobile, eggman)
    {
        if (!eggman->type) {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            eggman->direction     = FLIP_X;
            if (RSDK.Rand(0, 2))
                eggman->position.x = player1->position.x + 0x1000000;
            else
                eggman->position.x = player1->position.x - 0x1000000;
            eggman->position.y = player1->position.y + 0x800000;
            eggman->field_70.y = player1->position.y + 0x800000;
            eggman->field_120  = 1;
            eggman->velocity.x = 0;
            eggman->velocity.y = 0;
            eggman->state      = KleptoMobile_State_Unknown2;
            eggmanPtr          = eggman;
            foreach_break;
        }
    }

    foreach_active(KleptoMobile, eggmanArm)
    {
        if (eggmanArm->type) {
            eggmanArm->position.x = eggmanPtr->position.x;
            eggmanArm->position.y = eggmanPtr->position.y;
            for (int i = 0; i < 10; ++i) eggmanArm->framePositions[i] = eggmanPtr->position;
            eggmanArm->velocity.x = 0;
            eggmanArm->velocity.y = 0;
        }
    }
}

void PhantomKing_SetupKing(EntityPhantomKing *king)
{
    king->posUnknown     = king->position;
    king->velocity.x     = 0;
    king->drawRuby      = true;
    PhantomKing->boundsM = king->position.x;
    PhantomKing->boundsL = PhantomKing->boundsM - 0x800000;
    PhantomKing->boundsR = PhantomKing->boundsM + 0x800000;
    PhantomKing->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
    PhantomKing->boundsB = (Zone->cameraBoundsB[0] - 96) << 16;

    int slot                                      = RSDK.GetEntityID(king);
    RSDK_GET_ENTITY(slot - 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown2;
    RSDK_GET_ENTITY(slot + 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown2;
    king->state                                   = PhantomKing_State_Unknown8;
}

void PhantomKing_StateDraw_Body(void)
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
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);

    for (int i = 0; i < 10; ++i) {
        if (self->frameIDs[i] < 0x200) {
            self->animator5.frameID = self->frameIDs[i] / 42 % 6;
            RSDK.DrawSprite(&self->animator5, &self->framePositions[i], false);
        }
    }

    self->drawFX = self->storeDrawFX | FX_ROTATE;
    RSDK.DrawSprite(&self->animator4, NULL, false);
    self->drawFX = self->storeDrawFX | FX_ROTATE | FX_FLIP;

    for (int i = 0; i < 10; ++i) {
        if (self->frameIDs[i] >= 0x200) {
            self->animator5.frameID = self->frameIDs[i] / 42 % 6;
            RSDK.DrawSprite(&self->animator5, &self->framePositions[i], false);
        }
    }

    if (self->drawRuby)
        RSDK.DrawSprite(&self->animatorRuby, &self->rubyPos, false);

    if (self->typeChangeTimer <= 0) {
        if (self->invincibilityTimer & 1)
            RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomKing_StateDraw_Arm(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)self->parent;

    if (parent->typeChangeTimer > 0) {
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 128, 256);
    }

    for (int i = 0; i < 6; ++i) RSDK.DrawSprite(&self->animator8, &self->framePositions[i], false);
    RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);
    RSDK.DrawSprite(&self->animator10, &self->framePositions[6], false);

    if (parent->typeChangeTimer > 0) {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void PhantomKing_State_Unknown1(void)
{
    RSDK_THIS(PhantomKing);

    if (++self->timer >= 8) {
        self->position.y += 0x1000000;
        self->timer  = 0;
        self->active = ACTIVE_NORMAL;
        self->state  = StateMachine_None;
    }
}

void PhantomKing_State_Unknown2(void)
{
    RSDK_THIS(PhantomKing);

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < self->position.x;
    if (++self->timer == 30) {
        EntityPhantomKing *arm1 = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, PhantomKing);
        RSDK.ResetEntityPtr(arm1, PhantomKing->objectID, intToVoid(1));
        arm1->position.x = self->position.x;
        arm1->position.y = self->position.y;
        arm1->parent     = (Entity *)self;
        arm1->field_11C  = 0x2800;
        arm1->field_120  = 96;
        arm1->field_124  = 32;

        EntityPhantomKing *arm2 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, PhantomKing);
        RSDK.ResetEntityPtr(arm2, PhantomKing->objectID, intToVoid(2));
        arm2->position.x = self->position.x;
        arm2->position.y = self->position.y;
        arm2->parent     = (Entity *)self;
        arm2->field_11C  = 0x2800;
        arm2->field_120  = 96;
        arm2->field_124  = 32;

        self->timer     = 0;
        self->visible   = true;
        self->stateDraw = PhantomKing_StateDraw_Body;
        self->state     = PhantomKing_State_Unknown3;
    }
}

void PhantomKing_State_Unknown3(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->animator4);
    self->velocity.y -= 0x800;

    if (self->position.y <= self->posUnknown.y) {
        self->posUnknown.x                                                = self->position.x;
        self->posUnknown.y                                                = self->posUnknown.y;
        self->position.y                                                  = self->posUnknown.y;
        RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown1;
        self->velocity.y                                                  = 0;
        self->state                                                       = PhantomKing_State_Unknown4;
    }
    else {
        self->position.y += self->velocity.y;
    }
    PhantomKing_HandleFrames();
}

void PhantomKing_State_Unknown4(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->animator4);

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown.y;

    PhantomKing_HandleFrames();
    if (++self->timer >= 120) {
        self->timer = 0;
        self->state = PhantomKing_State_Unknown5;
    }
}

void PhantomKing_State_Unknown5(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->animator4);

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown.y;

    if (self->velocity.x < 0x40000)
        self->velocity.x += 0x1800;
    self->position.x += self->velocity.x;

    PhantomKing_HandleFrames();
    if (++self->timer >= 180) {
        RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, PhantomKing)->state = PhantomKing_StateArm1_Unknown2;
        self->velocity.x                                                  = 0;
        self->velocity.y                                                  = 0;
        self->state                                                       = PhantomKing_State_Unknown6;
        self->field_224                                                   = true;
    }
}

void PhantomKing_State_Unknown6(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->animator4);

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown.y;

    PhantomKing_HandleFrames();
}

void PhantomKing_State_Unknown7(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->animator4);

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown.y;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    PhantomKing_HandleFrames();
}

void PhantomKing_State_Unknown8(void)
{
    RSDK_THIS(PhantomKing);

    RSDK.ProcessAnimation(&self->animator4);

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown.y;

    PhantomKing_CheckPlayerCollisions();
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    int angle = RSDK.ATan2(self->position.x - player1->position.x, self->posUnknown.y - player1->position.y);
    int x     = (RSDK.Cos256(angle) << 15) + player1->position.x;
    int y     = (RSDK.Sin256(angle) << 15) + player1->position.y;

    if (x <= self->position.x) {
        if (self->velocity.x > -0x20000)
            self->velocity.x -= 0x800;
    }
    else {
        if (self->velocity.x < 0x20000)
            self->velocity.x += 0x800;
    }

    int bottom = Zone->cameraBoundsB[0] << 16;
    if (y > bottom - 0x400000)
        y = bottom - 0x800000;

    if (y <= self->posUnknown.y) {
        if (self->velocity.y > -0x20000)
            self->velocity.y -= 0x800;
    }
    else {
        if (self->velocity.y < 0x20000)
            self->velocity.y += 0x800;
    }

    self->posUnknown.y += self->velocity.y;
    self->position.x += self->velocity.x;
    self->direction = player1->position.x <= self->position.x;

    if (self->field_114) {
        if (++self->timer >= 60) {
            self->timer = 0;
            PhantomKing_HandleAttacks();
        }
    }
    else if (self->activeScreens == 1) {
        self->field_114 = true;
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

    RSDK.ProcessAnimation(&self->animator4);
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    if (self->position.y < -0x1000000) {
        foreach_active(PKingAttack, attack)
        {
            if (attack->state == PKingAttack_Unknown3)
                destroyEntity(attack);
        }
        self->field_114  = false;
        self->velocity.y = 0;
        self->timer      = 0;
        self->state      = PhantomKing_State_Switch;
    }
    PhantomKing_HandleFrames();
}

void PhantomKing_StateArm1_Unknown2(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)self->parent;

    int parentX = parent->position.x;
    int parentY = parent->position.y;

    int moveX = 0;
    int moveY = ((RSDK.Sin256(2 * (Zone->timer + (self->type << 6)) - 128) + 512) << 12) + parentY;

    self->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0;
    int y = 0;

    int x2 = 0;
    int y2 = 0;

    if (self->direction) {
        moveX = parentX - 0x300000;
        x     = 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle) + parent->position.x;
        y     = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);
        if (self->type == 1) {
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
        if (self->type == 1) {
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

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->animator9);
    RSDK.ProcessAnimation(&self->animator10);
}

void PhantomKing_StateArm1_Unknown3(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)self->parent;

    self->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0;
    int y = 0;

    if (self->direction) {
        x = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);

        if (self->type == 1) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
        }
    }
    else {
        x = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;

        if (self->type == 1) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
        }
    }

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, self->field_F8, self->field_FC, self->field_F8, self->field_FC,
                                                         self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->animator9);
    RSDK.ProcessAnimation(&self->animator10);
}

void PhantomKing_Unknown21(void)
{
    RSDK_THIS(PhantomKing);
    EntityPhantomKing *parent = (EntityPhantomKing *)self->parent;

    RSDK.Sin256(2 * (Zone->timer + (self->type << 6)) - 128);
    ++self->timer;
    self->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0;
    int y = 0;

    int x2 = 0;
    int y2 = 0;

    int x3 = 0;
    int y3 = 0;

    if (self->direction) {
        x = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);
        if (self->type == 1) {
            x += -0x1800 * RSDK.Cos512(negAngle);
            y += 0x1800 * RSDK.Sin512(negAngle);
        }
        x2 = x + self->field_11C * RSDK.Cos256(self->field_120);
        y2 = y + self->field_11C * RSDK.Sin256(self->field_120);
        x3 = x2 + self->field_11C * RSDK.Cos256(self->field_124 + self->field_120);
        y3 = y2 + self->field_11C * RSDK.Sin256(self->field_124 + self->field_120);
    }
    else {
        x = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;
        if (self->type == 1) {
            x += 0x1800 * RSDK.Cos512(negAngle);
            y += -0x1800 * RSDK.Sin512(negAngle);
        }
        x2 = x + self->field_11C * RSDK.Cos256(self->field_120);
        y2 = y + self->field_11C * RSDK.Sin256(self->field_120);
        x3 = x2 + self->field_11C * RSDK.Cos256(self->field_124 + self->field_120);
        y3 = y2 + self->field_11C * RSDK.Sin256(self->field_124 + self->field_120);
    }

    self->velocity.x += ((x3 - self->position.x) >> 5) - (self->velocity.x >> 3);
    self->velocity.y += ((y3 - self->position.y) >> 5) - (self->velocity.y >> 3);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int percent = 0x1800;
    for (int i = 0; i < 7; ++i) {
        self->framePositions[i] = MathHelpers_Unknown5(percent, x, y, x2, y2, x2, y2, x3, y3);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->animator9);
    RSDK.ProcessAnimation(&self->animator10);
}

void PhantomKing_StateArm1_Unknown1(void)
{
    RSDK_THIS(PhantomKing);

    PhantomKing_Unknown21();

    uint8 flags = 0;

    if (self->field_120 >= 192)
        flags = 1;
    else
        self->field_120 += 3;

    if (self->field_124 >= 96)
        ++flags;
    else
        self->field_124 += 3;

    if (flags == 2)
        self->state = PhantomKing_StateArm1_Unknown4;
}

void PhantomKing_StateArm1_Unknown4(void)
{
    RSDK_THIS(PhantomKing);

    PhantomKing_Unknown21();

    if (self->field_120 <= 128)
        self->field_120 = 128;
    else
        self->field_120 -= 16;

    if (self->field_124 <= 0)
        self->field_124 = 0;
    else
        self->field_124 -= 16;
}

void PhantomKing_DestroyEntity(void)
{
    RSDK_THIS(PhantomKing);

    int slot                = RSDK.GetEntityID(self);
    EntityPhantomKing *arm1 = RSDK_GET_ENTITY(slot - 1, PhantomKing);
    EntityPhantomKing *arm2 = RSDK_GET_ENTITY(slot + 1, PhantomKing);

    foreach_all(PKingAttack, attack) { destroyEntity(attack); }
    destroyEntity(arm1);
    destroyEntity(arm2);
    destroyEntity(self);
}

void PhantomKing_State_Explode(void)
{
    RSDK_THIS(PhantomKing);

    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomKing->sfxExplosion, false, 255);
        if (Zone->timer & 8) {
            int x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }
    if (++self->timer == 60)
        self->state = PhantomKing_State_Destroyed;
}

void PhantomKing_State_Destroyed(void)
{
    RSDK_THIS(PhantomKing);

    self->velocity.y += 0x2800;
    self->position.y += self->velocity.y;
    if (!(Zone->timer % 7)) {
        RSDK.PlaySfx(PhantomKing->sfxExplosion, false, 255);
        if (Zone->timer & 8) {
            int x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

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

    self->posUnknown               = self->position;
    self->rotation                 = 0;
    self->angle2                   = 0;
    self->animator3.animationSpeed = 0;
    PhantomKing_HandleFrames();

    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 0, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 1, &self->animator3, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 2, &self->animator4, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 7, &self->animator5, false, 0);
    RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &self->animatorRuby, false, 0);

    PhantomKing_StateDraw_Body();
}

void PhantomKing_EditorLoad(void) { PhantomKing->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE); }
#endif

void PhantomKing_Serialize(void) { RSDK_EDITABLE_VAR(PhantomKing, VAR_ENUM, type); }
