// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZKing Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        RSDK.DrawSprite(&self->basicAnimator, NULL, false);
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
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->type          = VOID_TO_INT(data);

        switch (self->type) {
            case ERZKING_KING:
                self->hitbox.left   = -24;
                self->hitbox.top    = -24;
                self->hitbox.right  = 24;
                self->hitbox.bottom = 24;

                self->visible   = false;
                self->direction = FLIP_X;
                self->health    = 8;

                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 0, &self->headAnimator, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 1, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 2, &self->beltAnimator, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 7, &self->particleAnimator, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 8, &self->rubyAnimator, true, 0);

                self->originPos = self->position;
                self->state     = ERZKing_State_SetupArena;
                break;

            case ERZKING_ARM_L:
            case ERZKING_ARM_R:
                self->visible = true;

                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 3, &self->armAnimator, true, 0);
                RSDK.SetSpriteAnimation(ERZKing->aniFrames, 4, &self->cuffAnimator, true, 0);

                if (self->type == ERZKING_ARM_L) {
                    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 6, &self->handAnimator, true, 0);
                }
                else {
                    self->drawGroup = Zone->playerDrawGroup[0];
                    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 5, &self->handAnimator, true, 0);
                }

                self->stateDraw = ERZKing_Draw_Arm;
                self->state     = ERZKing_State_Arm;
                break;
        }
    }
}

void ERZKing_StageLoad(void)
{
    ERZKing->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);

    ERZKing->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    ERZKing->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
}

void ERZKing_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZKing);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            ERZKing_Hit();
        }
    }
}

void ERZKing_Hit(void)
{
    RSDK_THIS(ERZKing);

    if (--self->health <= 0) {
        self->originPos.x      = self->position.x;
        self->originPos.y      = self->position.y;
        self->state            = ERZKing_State_Explode;
        self->velocity.y       = -0x10000;
        self->timer            = 0;
        SceneInfo->timeEnabled = false;
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
    }
}

void ERZKing_Explode(void)
{
    RSDK_THIS(ERZKing);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(ERZKing->sfxExplosion2, false, 255);

        if (Zone->timer & 4) {
            int32 x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int32 y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void ERZKing_HandleFrames(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&self->bodyAnimator);

    self->rotation = RSDK.Sin512(2 * Zone->timer) >> 6;
    int32 negAng   = -self->rotation;

    self->bodyAngle = (self->bodyAngle + 12) & 0x3FF;

    int32 x = 0x1C00 * RSDK.Sin512(negAng) + self->position.x;
    int32 y = 0x1C00 * RSDK.Cos512(negAng) + self->position.y;

    int32 angle = self->bodyAngle;

    for (int32 i = 0; i < 10; i += 2) {
        self->armPositions[i].x = x + 2 * RSDK.Cos512(self->rotation) * RSDK.Cos1024(angle);
        self->armPositions[i].y = y + 2 * RSDK.Sin512(self->rotation) * RSDK.Cos1024(angle);
        self->armAngles[i]         = angle & 0x3FF;

        angle += 0x200;

        self->armPositions[i + 1].x = x + 2 * RSDK.Cos512(self->rotation) * RSDK.Cos1024(angle);
        self->armPositions[i + 1].y = y + 2 * RSDK.Sin512(self->rotation) * RSDK.Cos1024(angle);
        self->armAngles[i + 1]         = angle & 0x3FF;

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

void ERZKing_Draw_Body(void)
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

void ERZKing_Draw_Arm(void)
{
    RSDK_THIS(ERZKing);

    EntityERZKing *parent = self->parent;

    if (parent->typeChangeTimer > 0) {
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 0, 48);
        RSDK.SetLimitedFade(0, 1, 4, parent->typeChangeTimer, 128, 256);
    }

    for (int32 i = 0; i < 6; ++i) {
        RSDK.DrawSprite(&self->armAnimator, &self->armPositions[i], false);
    }

    RSDK.DrawSprite(&self->cuffAnimator, &self->armPositions[6], false);
    RSDK.DrawSprite(&self->handAnimator, &self->armPositions[6], false);

    if (parent->typeChangeTimer > 0) {
        RSDK.CopyPalette(1, 0, 0, 0, 48);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
}

void ERZKing_State_SetupArena(void)
{
    RSDK_THIS(ERZKing);

    if (++self->timer >= 8) {
        self->timer = 0;

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - 320;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + 320;
        Zone->cameraBoundsT[0]      = Zone->cameraBoundsB[0] - ScreenInfo->size.y;

        ERZKing->boundsL = (Zone->cameraBoundsL[0] + 64) << 16;
        ERZKing->boundsR = (Zone->cameraBoundsR[0] - 64) << 16;
        ERZKing->boundsM = self->position.x;
        ERZKing->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
        ERZKing->boundsB = (Zone->cameraBoundsB[0] - 96) << 16;

        self->position.y += 0x1000000;
        self->active = ACTIVE_NORMAL;
        self->state  = ERZKing_State_SetupBody;
    }
}

void ERZKing_State_SetupBody(void)
{
    RSDK_THIS(ERZKing);

    if (self->timer) {
        self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < self->position.x;

        if (++self->timer == 30) {
            EntityERZKing *leftArm = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, ERZKing);
            RSDK.ResetEntity(leftArm, ERZKing->classID, INT_TO_VOID(ERZKING_ARM_L));
            leftArm->position.x = self->position.x;
            leftArm->position.y = self->position.y;
            leftArm->parent     = self;

            EntityERZKing *rightArm = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, ERZKing);
            RSDK.ResetEntity(rightArm, ERZKing->classID, INT_TO_VOID(ERZKING_ARM_R));
            rightArm->position.x = self->position.x;
            rightArm->position.y = self->position.y;
            rightArm->parent     = self;

            self->timer     = 0;
            self->visible   = true;
            self->stateDraw = ERZKing_Draw_Body;
            self->state     = ERZKing_State_EnterKing;
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x)
            ++self->timer;
    }
}

void ERZKing_State_EnterKing(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->velocity.y -= 0x1800;

    if (self->position.y <= self->originPos.y - 0x200000) {
        self->originPos = self->position;
        self->state     = ERZKing_State_FlyAround;
    }
    else {
        self->position.y += self->velocity.y;
    }

    ERZKing_HandleFrames();
}

void ERZKing_State_FlyAround(void)
{
    RSDK_THIS(ERZKing);

    RSDK.ProcessAnimation(&self->beltAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

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
            self->timer       = 0;
            self->scale.x     = 0x200;
            self->scale.y     = 0x200;
            self->storeDrawFX = FX_SCALE;
            self->state       = ERZKing_State_ChangeHBH;

            CREATE_ENTITY(FXRuby, FXRuby_State_ShrinkAndDestroy, self->position.x, self->position.y)->radiusSpeed = 0x80000;
        }
    }
    ERZKing_HandleFrames();
}

void ERZKing_State_ChangeHBH(void)
{
    RSDK_THIS(ERZKing);

    self->typeChangeTimer += 16;

    self->scale.x -= self->scale.x >> 4;
    self->scale.y = self->scale.x;

    if (self->typeChangeTimer == 0x400) {
        self->typeChangeTimer = 0;

        foreach_all(ERZKing, king) { king->active = ACTIVE_NEVER; }

        switch (self->nextType) {
            case ERZKING_HEAVY_GUNNER:
                CREATE_ENTITY(ERZGunner, NULL, self->position.x, self->position.y);

                self->storeDrawFX = FX_NONE;
                self->state       = ERZKing_State_FlyAround;
                self->nextType--;
                self->nextType &= 1;
                break;

            case ERZKING_HEAVY_MYSTIC:
                CREATE_ENTITY(ERZMystic, NULL, self->position.x, self->position.y);
                self->storeDrawFX = FX_NONE;
                self->state       = ERZKing_State_FlyAround;
                self->nextType--;
                self->nextType &= 1;
                break;

                // Shinobi & Rider never got completed... RIP
        }
    }
}

void ERZKing_State_Arm(void)
{
    RSDK_THIS(ERZKing);

    EntityERZKing *parent = self->parent;

    int32 moveX = 0;
    int32 moveY = ((RSDK.Sin256(2 * (Zone->timer + (self->type << 6)) - 128) + 512) << 12) + parent->position.y;

    self->direction = parent->direction;
    int32 negAngle  = -parent->rotation;

    int32 x = 0, y = 0;
    int32 x2 = 0, y2 = 0;
    if (parent->direction) {
        moveX = parent->position.x - 0x300000;
        x     = parent->position.x + 0xD00 * RSDK.Cos512(negAngle) + 0x300 * RSDK.Sin512(negAngle);
        y     = parent->position.y - 0xD00 * RSDK.Sin512(negAngle) + 0x300 * RSDK.Cos512(negAngle);

        if (self->type == ERZKING_ARM_L) {
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

        if (self->type == ERZKING_ARM_L) {
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

    int32 percent = 0x1800;
    for (int32 i = 0; i < 7; ++i) {
        self->armPositions[i] = MathHelpers_GetBezierPoint(percent, x, y, x2, y2, x2, y2, self->position.x, self->position.y);
        percent += 0x2000;
    }

    RSDK.ProcessAnimation(&self->cuffAnimator);
    RSDK.ProcessAnimation(&self->handAnimator);
}

void ERZKing_State_Explode(void)
{
    RSDK_THIS(ERZKing);

    self->velocity.y += 0x2800;
    self->position.y += self->velocity.y;

    ERZKing_Explode();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        // This boss made it far enough to get the player to the ending... neat!
        GameProgress_GiveEnding(GAMEPROGRESS_ENDING_GOOD);
        API_UnlockAchievement(&achievementList[ACH_GAME_CLEARED]);

        // It is interesting that the boss doesn't show ending videos, it just takes you to the credits... perhaps they weren't finished yet?
        RSDK.SetScene("Presentation", "Credits");
        Zone_StartFadeOut(10, 0x000000);
        Music_FadeOut(0.025);

        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void ERZKing_EditorDraw(void)
{
    RSDK_THIS(ERZKing);

    self->originPos = self->position;
    self->bodyAngle = 0;
    ERZKing_HandleFrames();

    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 0, &self->headAnimator, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 1, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 2, &self->beltAnimator, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 7, &self->particleAnimator, true, 0);
    RSDK.SetSpriteAnimation(ERZKing->aniFrames, 8, &self->rubyAnimator, true, 0);

    ERZKing_Draw_Body();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-320, -SCREEN_YSIZE, 320, 0, 1 | 2 | 4 | 0, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void ERZKing_EditorLoad(void)
{
    ERZKing->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomKing.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(ERZKing, type);
    RSDK_ENUM_VAR("King", ERZKING_KING);
}
#endif

void ERZKing_Serialize(void) { RSDK_EDITABLE_VAR(ERZKing, VAR_ENUM, type); }
