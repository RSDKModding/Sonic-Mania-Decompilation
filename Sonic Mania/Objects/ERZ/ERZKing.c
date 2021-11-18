#include "SonicMania.h"

ObjectERZKing *ERZKing;

void ERZKing_Update(void)
{
    RSDK_THIS(ERZKing);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);
}

void ERZKing_LateUpdate(void) {}

void ERZKing_StaticUpdate(void) {}

void ERZKing_Draw(void)
{
    RSDK_THIS(ERZKing);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void ERZKing_Create(void *data)
{
    RSDK_THIS(ERZKing);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
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
                self->visible       = false;
                self->direction     = FLIP_X;
                self->health        = 8;
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 0, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 1, &self->animator3, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 2, &self->animator4, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 7, &self->animator5, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 8, &self->animatorRuby, true, 0);
                self->posUnknown = self->position;
                self->state      = ERZKing_State_Unknown1;
                break;
            case 1: // body
            case 2: // body
                self->visible = true;
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 3, &self->animator8, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 4, &self->animator9, true, 0);
                if (self->type == 1) {
                    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 6, &self->animator10, true, 0);
                }
                else {
                    self->drawOrder = Zone->playerDrawLow;
                    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 5, &self->animator10, true, 0);
                }
                self->stateDraw = ERZKing_StateDraw_Arm;
                self->state     = ERZKing_State_Arm;
                break;
        }
    }
}

void ERZKing_StageLoad(void)
{
    ERZKing->aniFrames     = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);
    ERZKing->sfxHit        = RSDK.GetSFX("Stage/BossHit.wav");
    ERZKing->sfxExplosion2 = RSDK.GetSFX("Stage/Explosion2.wav");
}

void ERZKing_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZKing);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            if (--self->health <= 0) {
                self->posUnknown.x        = self->position.x;
                self->posUnknown.y        = self->position.y;
                self->state               = ERZKing_State_Explode;
                self->velocity.y          = -0x10000;
                self->timer               = 0;
                SceneInfo->timeEnabled = false;
            }
            else {
                self->invincibilityTimer = 48;
                RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            }
        }
    }
}

void ERZKing_HandleFrames(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&self->animator3);

    self->rotation = RSDK.Sin512(2 * Zone->timer) >> 6;
    int negAng       = -self->rotation;

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

void ERZKing_StateDraw_Body(void)
{
    RSDK_THIS(ERZKing);

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

void ERZKing_StateDraw_Arm(void)
{
    RSDK_THIS(ERZKing);
    EntityERZKing *parent = (EntityERZKing *)self->parent;

    if (parent->typeChangeTimer > 0) {
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 128, 256);
    }

    for (int i = 0; i < 6; ++i) {
        RSDK.DrawSprite(&self->animator8, &self->framePositions[i], false);
    }

    RSDK.DrawSprite(&self->animator9, &self->framePositions[6], false);
    RSDK.DrawSprite(&self->animator10, &self->framePositions[6], false);

    if (parent->typeChangeTimer > 0) {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void ERZKing_State_Unknown1(void)
{
    RSDK_THIS(ERZKing);

    if (++self->timer >= 8) {
        self->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (self->position.x >> 16) - 320;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (self->position.x >> 16) + 320;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - ScreenInfo->height;
        ERZKing->boundsL            = (Zone->screenBoundsL1[0] + 64) << 16;
        ERZKing->boundsR            = (Zone->screenBoundsR1[0] - 64) << 16;
        ERZKing->boundsM            = self->position.x;
        ERZKing->boundsT            = (Zone->screenBoundsT1[0] + 48) << 16;
        ERZKing->boundsB            = (Zone->screenBoundsB1[0] - 96) << 16;
        self->position.y += 0x1000000;
        self->active = ACTIVE_NORMAL;
        self->state  = ERZKing_State_Unknown2;
    }
}

void ERZKing_State_Unknown2(void)
{
    RSDK_THIS(ERZKing);

    if (self->timer) {
        self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < self->position.x;
        if (++self->timer == 30) {
            EntityERZKing *leftArm = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, ERZKing);
            RSDK.ResetEntityPtr(leftArm, ERZKing->objectID, intToVoid(1));
            leftArm->position.x = self->position.x;
            leftArm->position.y = self->position.y;
            leftArm->parent     = (Entity *)self;

            EntityERZKing *rightArm = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, ERZKing);
            RSDK.ResetEntityPtr(rightArm, ERZKing->objectID, intToVoid(1));
            rightArm->position.x = self->position.x;
            rightArm->position.y = self->position.y;
            rightArm->parent     = (Entity *)self;

            self->timer     = 0;
            self->visible   = true;
            self->stateDraw = ERZKing_StateDraw_Body;
            self->state     = ERZKing_State_Unknown3;
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x)
            ++self->timer;
    }
}

void ERZKing_State_Unknown3(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&self->animator4);
    self->velocity.y -= 0x1800;
    if (self->position.y <= self->posUnknown.y - 0x200000) {
        self->posUnknown.x = self->position.x;
        self->posUnknown.y = self->position.y;
        self->state        = ERZKing_State_Unknown4;
    }
    else {
        self->position.y += self->velocity.y;
    }
    ERZKing_HandleFrames();
}

void ERZKing_State_Unknown4(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&self->animator4);

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown.y;
    ERZKing_CheckPlayerCollisions();

    if (self->direction) {
        if (self->velocity.x > -0x20000)
            self->velocity.x -= 0x800;
        if (self->position.x < ERZKing->boundsL)
            self->direction = FLIP_NONE;
    }
    else {
        if (self->velocity.x < 0x20000)
            self->velocity.x += 0x800;
        if (self->position.x > ERZKing->boundsR)
            self->direction = FLIP_X;
    }
    self->position.x += self->velocity.x;
    ++self->timer;

    if (self->timer > 240) {
        if (abs(self->position.x - ERZKing->boundsM) < 0x200000) {
            self->timer                                                                               = 0;
            self->scale.x                                                                             = 0x200;
            self->scale.y                                                                             = 0x200;
            self->storeDrawFX                                                                         = FX_SCALE;
            self->state                                                                               = ERZKing_State_ChangeHBH;
            CREATE_ENTITY(FXRuby, FXRuby_Unknown9, self->position.x, self->position.y)->radiusSpeed = 0x80000;
        }
    }
    ERZKing_HandleFrames();
}

void ERZKing_State_ChangeHBH(void)
{
    RSDK_THIS(ERZKing);

    self->typeChangeTimer += 16;
    self->scale.x -= (self->scale.x >> 4);
    self->scale.y = self->scale.x;
    if (self->typeChangeTimer == 0x400) {
        self->typeChangeTimer = 0;
        foreach_all(ERZKing, king) { king->active = ACTIVE_NEVER; }

        switch (self->nextType) {
            case 0:
                CREATE_ENTITY(ERZGunner, NULL, self->position.x, self->position.y);

                self->storeDrawFX = FX_NONE;
                self->state       = ERZKing_State_Unknown4;
                self->nextType--;
                self->nextType &= 1;
                break;
            case 1:
                CREATE_ENTITY(ERZMystic, NULL, self->position.x, self->position.y);
                self->storeDrawFX = FX_NONE;
                self->state       = ERZKing_State_Unknown4;
                self->nextType--;
                self->nextType &= 1;
                break;
        }
    }
}

void ERZKing_State_Arm(void)
{
    RSDK_THIS(ERZKing);

    EntityERZKing *parent = (EntityERZKing *)self->parent;

    int moveX = 0;
    int moveY = ((RSDK.Sin256(2 * (Zone->timer + (self->type << 6)) - 128) + 512) << 12) + parent->position.y;

    self->direction = parent->direction;
    int negAngle      = -parent->rotation;

    int x = 0, y = 0;
    int x2 = 0, y2 = 0;
    if (parent->direction) {
        moveX = parent->position.x - 0x300000;
        x     = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y     = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);
        if (self->type == 1) {
            x += -0x1800 * RSDK.Cos512(parent->rotation);
            y += 0x1800 * RSDK.Sin512(parent->rotation);
            moveX -= 0x300000;
        }
        x2 = ((self->position.x + x) >> 1) + 0x200000;
        y2 = ((self->position.y + y) >> 1) + 0x200000;
    }
    else {
        moveX = parent->position.x + 0x300000;
        x     = 0x300 * RSDK.Sin512(negAngle) - 0xD00 * RSDK.Cos512(negAngle) + parent->position.x;
        y     = 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle) + parent->position.y;
        if (self->type == 1) {
            x += 0x1800 * RSDK.Cos512(parent->rotation);
            y += -0x1800 * RSDK.Sin512(parent->rotation);
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

void ERZKing_State_Explode(void)
{
    RSDK_THIS(ERZKing);

    self->velocity.y += 0x2800;
    self->position.y += self->velocity.y;
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(ERZKing->sfxExplosion2, false, 255);
        if (Zone->timer & 4) {
            int x = RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16;
            int y = RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16;
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x + self->position.x, y + self->position.y)
                ->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        GameProgress_GiveEnding(2);
        API_UnlockAchievement("ACH_GAME_CLEARED");
        RSDK.SetScene("Presentation", "Credits");
        Zone_StartFadeOut(10, 0x000000);
        Music_FadeOut(0.025);
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void ERZKing_EditorDraw(void)
{
    RSDK_THIS(ERZKing);

    self->posUnknown = self->position;
    self->angle2     = 0;
    ERZKing_HandleFrames();

    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 1, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 2, &self->animator4, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 7, &self->animator5, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 8, &self->animatorRuby, true, 0);

    ERZKing_StateDraw_Body();
}

void ERZKing_EditorLoad(void) { ERZKing->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE); }
#endif

void ERZKing_Serialize(void) { RSDK_EDITABLE_VAR(ERZKing, VAR_ENUM, type); }
