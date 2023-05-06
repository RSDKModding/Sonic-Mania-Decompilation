// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HotaruHiWatt Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHotaruHiWatt *HotaruHiWatt;

void HotaruHiWatt_Update(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);
}

void HotaruHiWatt_LateUpdate(void) {}

void HotaruHiWatt_StaticUpdate(void) {}

void HotaruHiWatt_Draw(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
}

void HotaruHiWatt_Create(void *data)
{
    RSDK_THIS(HotaruHiWatt);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active        = ACTIVE_XBOUNDS;
            self->updateRange.x = 0xC00000;
            self->updateRange.y = 0xC00000;
            self->type          = VOID_TO_INT(data);
            self->drawGroup     = Zone->objectDrawGroup[0];

            switch (self->type) {
                case HHW_BOSS:
                    self->visible = false;
                    self->drawFX  = FX_FLIP;
                    self->health  = 6;
                    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 0, &self->headAnimator, true, 0);
                    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 1, &self->bulbAnimator, true, 0);

                    self->alpha         = 0x100;
                    self->state         = HotaruHiWatt_StateBoss_SetupArena;
                    self->hitbox.left   = -24;
                    self->hitbox.top    = -24;
                    self->hitbox.right  = 24;
                    self->hitbox.bottom = 24;
                    break;

                case HHW_SINGLE_HOTARU:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->mainAnimator, true, 3);
                    ++self->drawGroup;

                    self->inkEffect = INK_ADD;
                    self->visible   = true;
                    self->health    = 3;
                    self->position.x += RSDK.Rand(-0x500000, 0x500000);
                    self->position.y += RSDK.Rand(-0x800000, 0x800000);
                    self->state         = HotaruHiWatt_StateHotaru_DimScreen;
                    self->hitbox.left   = -6;
                    self->hitbox.top    = -6;
                    self->hitbox.right  = 6;
                    self->hitbox.bottom = 6;
                    break;

                case HHW_PAIR_HOTARU:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->mainAnimator, true, 3);

                    self->originPos             = self->position;
                    self->inkEffect             = INK_ADD;
                    self->visible               = true;
                    self->health                = 3;
                    self->formationCircleRadius = 0x800;
                    self->alpha                 = -0x200;
                    self->state                 = HotaruHiWatt_StateHotaruPair_DimScreen;
                    self->hitbox.left           = -6;
                    self->hitbox.top            = -6;
                    self->hitbox.right          = 6;
                    self->hitbox.bottom         = 6;
                    break;

                case HHW_SMALL_HOTARU:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->mainAnimator, true, 3);

                    self->targetPos = self->position;
                    self->inkEffect = INK_ADD;
                    self->visible   = true;
                    self->health    = 3;
                    self->alpha     = 0x100;
                    self->state     = HotaruHiWatt_StateHotaru_MoveToTarget;
                    break;

                case HHW_FLASH:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->mainAnimator, true, 0);

                    self->inkEffect = INK_ADD;
                    self->visible   = true;
                    self->health    = 3;
                    self->alpha     = 0x100;
                    self->state     = HotaruHiWatt_StateBoss_FlashFadeOut;
                    break;

                case HHW_UNUSED2: break;

                case HHW_MINILASER:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 2, &self->mainAnimator, true, 1);
                    self->state         = HotaruHiWatt_State_MiniLaser;
                    self->visible       = true;
                    self->hitbox.left   = -4;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 4;
                    self->hitbox.bottom = 8;
                    break;

                case HHW_ELECTRICORB:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 4, &self->mainAnimator, true, 0);
                    self->state         = HotaruHiWatt_State_SparkyOrb;
                    self->visible       = true;
                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void HotaruHiWatt_StageLoad(void)
{
    HotaruHiWatt->aniFrames    = RSDK.LoadSpriteAnimation("SSZ1/HotaruHiWatt.bin", SCOPE_STAGE);
    HotaruHiWatt->hotaruFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);

    HotaruHiWatt->spawnedLaserStrike = false;
    RSDK.CopyPalette(1, 128, 0, 128, 64);

    HotaruHiWatt->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    HotaruHiWatt->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    HotaruHiWatt->sfxHHWAppear = RSDK.GetSfx("SSZ1/HHWAppear.wav");
    HotaruHiWatt->sfxFlash     = RSDK.GetSfx("SSZ1/HHWFlash.wav");
    HotaruHiWatt->sfxLaser     = RSDK.GetSfx("SSZ1/HotaruLaser.wav");
    HotaruHiWatt->sfxAppear    = RSDK.GetSfx("SSZ1/HotaruAppear.wav");
    HotaruHiWatt->sfxFly       = RSDK.GetSfx("SSZ1/HotaruFly.wav");
    HotaruHiWatt->sfxFlyUp     = RSDK.GetSfx("SSZ1/HHWFlyUp.wav");
    HotaruHiWatt->sfxCharge    = RSDK.GetSfx("SSZ1/HHWCharge.wav");

    Soundboard_LoadSfx("Stage/Zap.wav", true, HotaruHiWatt_SfxCheck_Zap, StateMachine_None);
    Soundboard_LoadSfx("SSZ1/HHWLaser.wav", 32606, HotaruHiWatt_SfxCheck_HHWLaser, HotaruHiWatt_SfxUpdate_HHWLaser);
}

bool32 HotaruHiWatt_SfxCheck_Zap(void)
{
    int32 count = 0;
    foreach_active(HotaruHiWatt, boss)
    {
        if (boss->state == HotaruHiWatt_StateHotaruPair_AttackDelay || boss->state == HotaruHiWatt_StateHotaruPair_AttackMovingDown
            || boss->state == HotaruHiWatt_StateHotaruPair_AttackMovingUp)
            ++count;
    }
    return count > 1;
}

bool32 HotaruHiWatt_SfxCheck_HHWLaser(void)
{
    int32 count = 0;
    foreach_active(HotaruHiWatt, boss)
    {
        if (boss->state == HotaruHiWatt_StateBoss_LaserAttack_Right || boss->state == HotaruHiWatt_StateBoss_LaserAttack_Left)
            ++count;
    }
    return count > 0;
}

void HotaruHiWatt_SfxUpdate_HHWLaser(int32 sfx)
{
    if (!(Soundboard->sfxPlayingTimer[sfx] % 6))
        Camera_ShakeScreen(0, 0, 2);
}

void HotaruHiWatt_Hit(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (--self->health <= 0) {
        self->stateDraw        = HotaruHiWatt_Draw_Boss;
        self->state            = HotaruHiWatt_StateBoss_Destroyed;
        self->timer            = 0;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(HotaruHiWatt->sfxHit, false, 255);
    }
}

void HotaruHiWatt_Explode(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(HotaruHiWatt->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int32 y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions_Hotaru(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBadnikBreak(player, self, false)) {
            if (self->type == HHW_SINGLE_HOTARU) {
                HotaruHiWatt_SpawnPairHotarus();
            }
            else {
                int32 hotaruPairCount = 0;
                foreach_active(HotaruHiWatt, boss)
                {
                    if (boss->type == HHW_PAIR_HOTARU)
                        hotaruPairCount++;
                }

                if (hotaruPairCount == 1)
                    HotaruHiWatt_SetupHHWReappear();
            }

            destroyEntity(self);
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions_Boss(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self))
            HotaruHiWatt_Hit();
    }
}

void HotaruHiWatt_CheckPlayerCollisions_MiniLaser(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                Player_Hurt(player, self);
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions_BossLaser(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        Hitbox hitbox = self->hitbox;
        if (!player->invincibleTimer && !player->blinkTimer)
            hitbox.bottom += 0x80;

        if (Player_CheckBadnikTouch(player, self, &hitbox)) {
            if (self->invincibilityTimer) {
                if (player->position.y > self->position.y)
                    Player_Hurt(player, self);
            }
            else if (player->position.y < self->position.y || player->invincibleTimer || player->blinkTimer) {
                if (Player_CheckBossHit(player, self))
                    HotaruHiWatt_Hit();
            }
            else {
                Player_Hurt(player, self);
            }
        }
    }
}

void HotaruHiWatt_Draw_Hotaru(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->inkEffect = INK_ADD;

    if (!(self->timer & 2))
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    int32 alpha = self->alpha;
    self->alpha = self->bulbAlpha;
    RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

    self->alpha = alpha;
}

void HotaruHiWatt_Draw_HotaruAttacking(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->bulbAnimator, NULL, false);
}

void HotaruHiWatt_Draw_Boss(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&self->headAnimator) == 'b')
        self->direction = FLIP_X;

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);

        RSDK.DrawSprite(&self->headAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&self->headAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);
    }
}

void HotaruHiWatt_Draw_BossLaserCharging(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&self->headAnimator) == 'b')
        self->direction = FLIP_X;

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&self->headAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&self->headAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);
    }

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->bulbFlashAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void HotaruHiWatt_Draw_BossLaser(void)
{
    RSDK_THIS(HotaruHiWatt);

    Vector2 drawPos;
    if (RSDK.GetFrameID(&self->headAnimator) == 'b')
        self->direction = FLIP_X;

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&self->headAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&self->headAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->bulbAnimator, NULL, false);
    }

    self->inkEffect                 = INK_ALPHA;
    self->bulbFlashAnimator.frameID = 0;
    RSDK.DrawSprite(&self->bulbFlashAnimator, NULL, false);

    self->bulbFlashAnimator.frameID = 1;
    RSDK.DrawSprite(&self->bulbFlashAnimator, NULL, false);

    drawPos = self->position;
    drawPos.y += 0x180000 + ((Zone->timer & 7) << 20);
    self->direction = (Zone->timer >> 2) & 1;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->laserImpactAnimator, &self->laserImpactPos, false);

    self->inkEffect = INK_NONE;
}

void HotaruHiWatt_Draw_FormingHHW(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.DrawCircle(self->position.x, self->position.y, self->formationCircleRadius >> 16, 0xF0F0F0, 128, INK_ADD, false);
    RSDK.DrawCircle(self->position.x, self->position.y, self->formationCircleRadius / 0xCCCC, 0xF0F0F0, 128, INK_ADD, false);
}

void HotaruHiWatt_Draw_FlashAppear(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&self->headAnimator) == 'b')
        self->direction = FLIP_X;

    RSDK.DrawSprite(&self->headAnimator, NULL, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

    RSDK.DrawCircle(self->position.x, self->position.y, self->formationCircleRadius >> 16, 0xF0F0F0, self->alpha >> 1, INK_ADD, false);
    RSDK.DrawCircle(self->position.x, self->position.y, self->formationCircleRadius / 0xCCCC, 0xF0F0F0, self->alpha >> 1, INK_ADD, false);
}

void HotaruHiWatt_SpawnPairHotarus(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    EntityHotaruHiWatt *boss = self->parents[0];

    if (boss->state != HotaruHiWatt_StateBoss_HiddenDimScreen)
        boss->state = HotaruHiWatt_StateBoss_HiddenDimScreen;

    EntityHotaruHiWatt *pairHotaru1 = CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_PAIR_HOTARU), player1->position.x, HotaruHiWatt->startY + 0x200000);
    pairHotaru1->isPermanent        = true;
    pairHotaru1->parents[0]         = boss;

    EntityHotaruHiWatt *pairHotaru2 = CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_PAIR_HOTARU), player1->position.x, HotaruHiWatt->startY + 0x200000);
    pairHotaru2->angle += 0x800000;
    pairHotaru2->isPermanent = true;
    pairHotaru2->parents[0]  = boss;
    pairHotaru1->parents[1]  = pairHotaru2;
}

void HotaruHiWatt_SetupHHWReappear(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *boss = self->parents[0];

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= HotaruHiWatt->boundsM)
        boss->position.x = HotaruHiWatt->boundsR;
    else
        boss->position.x = HotaruHiWatt->boundsL;

    boss->position.y = HotaruHiWatt->startY;

    boss->nextState = HotaruHiWatt_StateBoss_LaserAttackDelay;
    boss->state     = HotaruHiWatt_StateBoss_DimScreen;
}

void HotaruHiWatt_StateBoss_SetupArena(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer >= 8) {
        self->timer = 0;

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;

        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsT[0] = (self->position.y >> 16) - ScreenInfo->size.y;
        Zone->cameraBoundsB[0] = (self->position.y >> 16);

        HotaruHiWatt->boundsL = (Zone->cameraBoundsL[0] + 64) << 16;
        HotaruHiWatt->boundsR = (Zone->cameraBoundsR[0] - 64) << 16;
        HotaruHiWatt->boundsM = self->position.x;
        HotaruHiWatt->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
        HotaruHiWatt->boundsB = (Zone->cameraBoundsB[0] - 8) << 16;

        foreach_active(Fireflies, fireflies)
        {
            if (fireflies->state == Fireflies_State_Spawner)
                destroyEntity(fireflies);
        }

        self->state = HotaruHiWatt_StateBoss_AwaitPlayer;
    }
}

void HotaruHiWatt_StateBoss_AwaitPlayer(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        self->position.y += -0x100000 - (ScreenInfo->center.y << 16);
        HotaruHiWatt->startY = self->position.y;

        self->state     = HotaruHiWatt_StateBoss_DimScreen;
        self->nextState = HotaruHiWatt_StateBoss_Appear;
    }
}

void HotaruHiWatt_StateBoss_Appear(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->headAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxFlyUp, false, 255);
        self->state = HotaruHiWatt_StateBoss_FlyUp;
    }

    HotaruHiWatt_CheckPlayerCollisions_Boss();
}

void HotaruHiWatt_StateBoss_FlyUp(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->velocity.y -= 0x3800;
    self->originPos.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->headAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    HotaruHiWatt_CheckPlayerCollisions_Boss();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->position.x = HotaruHiWatt->boundsM;
        self->position.y = HotaruHiWatt->startY;
        self->state      = self->nextState;
        self->visible    = false;

        EntityHotaruHiWatt *hotaru = CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_SINGLE_HOTARU), self->position.x, self->position.y);
        hotaru->isPermanent        = true;
        hotaru->parents[0]         = self;
    }
}

void HotaruHiWatt_StateHotaru_MoveToTarget(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!(Zone->timer & 3))
        CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_FLASH), self->position.x, self->position.y);

    if (!((self->position.x ^ self->targetPos.x) & 0xFFF80000)) {
        if (!((self->position.y ^ self->targetPos.y) & 0xFFF80000)) {
            self->position.x = self->targetPos.x;
            self->position.y = self->targetPos.y;
            self->velocity.x = 0;
            self->velocity.y = 0;

            if (!self->nextState) {
                destroyEntity(self);
            }
            else {
                RSDK.PlaySfx(HotaruHiWatt->sfxAppear, false, 0xFF);
                self->state = self->nextState;
            }
        }
    }
}

void HotaruHiWatt_StateBoss_FlashFadeOut(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->alpha -= 8;
    if (self->alpha <= 0)
        destroyEntity(self);
}

void HotaruHiWatt_StateBoss_HiddenDimScreen(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->timer >= 192)
        self->state = StateMachine_None;
    else
        self->timer += 8;

    RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);
}

void HotaruHiWatt_StateBoss_HiddenUndimScreen(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->timer <= 0)
        self->state = StateMachine_None;
    else
        self->timer -= 8;

    RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);
}

void HotaruHiWatt_StateHotaru_DimScreen(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *boss = self->parents[0];

    if (boss->state != HotaruHiWatt_StateBoss_HiddenDimScreen)
        boss->state = HotaruHiWatt_StateBoss_HiddenDimScreen;

    if (self->bulbAlpha >= 256) {
        self->targetPos.x = HotaruHiWatt->boundsM + RSDK.Rand(-0x800000, 0x800000);
        self->targetPos.y = HotaruHiWatt->startY + RSDK.Rand(-0x500000, 0x500000);
        self->stateDraw   = HotaruHiWatt_Draw_Hotaru;
        self->velocity.x  = (self->targetPos.x - self->position.x) / 48;
        self->velocity.y  = (self->targetPos.y - self->position.y) / 48;

        RSDK.PlaySfx(HotaruHiWatt->sfxFly, false, 255);

        self->nextState = HotaruHiWatt_StateHotaru_Charging;
        self->state     = HotaruHiWatt_StateHotaru_MoveToTarget;
    }
    else {
        self->bulbAlpha += 16;
    }
}

void HotaruHiWatt_StateHotaru_Charging(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *boss = self->parents[0];

    if (boss->state != HotaruHiWatt_StateBoss_HiddenUndimScreen)
        boss->state = HotaruHiWatt_StateBoss_HiddenUndimScreen;

    RSDK.ProcessAnimation(&self->bulbAnimator);

    if (!self->timer) {
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->bulbAnimator, true, 0);
    }

    self->alpha = 8 * ++self->timer;

    if (self->timer == 32) {
        self->timer     = 0;
        self->state     = HotaruHiWatt_StateHotaru_Attacking;
        self->stateDraw = HotaruHiWatt_Draw_HotaruAttacking;

        RSDK.PlaySfx(HotaruHiWatt->sfxLaser, false, 255);
    }

    HotaruHiWatt_CheckPlayerCollisions_Hotaru();
}

void HotaruHiWatt_StateHotaru_Attacking(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->bulbAnimator);

    ++self->timer;
    if (!(self->timer & 1))
        CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_MINILASER), self->position.x, self->position.y + 0x40000);

    if (self->timer == 24) {
        self->timer     = 0;
        self->stateDraw = HotaruHiWatt_Draw_Hotaru;
        self->state     = HotaruHiWatt_StateHotaru_FinishedAttack;
    }

    HotaruHiWatt_CheckPlayerCollisions_Hotaru();
}

void HotaruHiWatt_StateHotaru_FinishedAttack(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *boss = self->parents[0];

    if (boss->state != HotaruHiWatt_StateBoss_HiddenDimScreen)
        boss->state = HotaruHiWatt_StateBoss_HiddenDimScreen;

    ++self->timer;

    if (self->alpha <= 0) {
        // health prolly should be named "remainingAttacks" here
        if (self->health >= 1) {
            self->timer = 0;
            self->health--;
            RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->mainAnimator, true, 3);
            self->stateDraw = StateMachine_None;
            self->state     = HotaruHiWatt_StateHotaru_DimScreen;
        }
        else {
            self->state = HotaruHiWatt_StateHotaru_EndAttackSequence;
        }
    }
    else {
        self->alpha -= 8;
    }

    HotaruHiWatt_CheckPlayerCollisions_Hotaru();
}

void HotaruHiWatt_StateHotaru_EndAttackSequence(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->bulbAlpha <= 0) {
        HotaruHiWatt_SpawnPairHotarus();
        destroyEntity(self);
    }
    else {
        self->bulbAlpha -= 32;
        HotaruHiWatt_CheckPlayerCollisions_Hotaru();
    }
}

void HotaruHiWatt_State_MiniLaser(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->position.y += 0x80000;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        if (HotaruHiWatt->spawnedLaserStrike) {
            destroyEntity(self);
        }
        else {
            RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 3, &self->mainAnimator, true, 0);
            self->position.y += 0x80000;
            self->drawGroup                  = Zone->objectDrawGroup[1];
            self->state                      = HotaruHiWatt_State_MiniLaserStrike;
            HotaruHiWatt->spawnedLaserStrike = true;
        }
    }

    if (self->classID == HotaruHiWatt->classID)
        HotaruHiWatt_CheckPlayerCollisions_MiniLaser();
}

void HotaruHiWatt_State_MiniLaserStrike(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID == 2)
        HotaruHiWatt->spawnedLaserStrike = false;

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
        destroyEntity(self);
}

void HotaruHiWatt_StateHotaruPair_DimScreen(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *boss = self->parents[0];

    if (boss->state != HotaruHiWatt_StateBoss_HiddenDimScreen)
        boss->state = HotaruHiWatt_StateBoss_HiddenDimScreen;

    self->position.x = self->formationCircleRadius * RSDK.Cos1024(self->angle >> 14) + self->originPos.x;
    self->position.y = self->formationCircleRadius * RSDK.Sin1024(self->angle >> 14) + self->originPos.y;

    if (self->alpha >= 0x100)
        self->state = HotaruHiWatt_StateHotaruPair_PrepareAttack;
    else
        self->alpha += 0x10;
}

void HotaruHiWatt_StateHotaruPair_PrepareAttack(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->timer >= 64) {
        if (self->velocity.x > 0)
            self->velocity.x -= 0x2000;
    }
    else {
        if (self->velocity.x < 0x80000)
            self->velocity.x += 0x2000;
    }

    if (self->originPos.x < HotaruHiWatt->boundsM - 0x600000)
        self->originPos.x += 0x20000;
    else if (self->originPos.x > HotaruHiWatt->boundsM + 0x600000)
        self->originPos.x -= 0x20000;

    self->originPos.y -= self->velocity.x >> 2;
    self->formationCircleRadius += self->velocity.x >> 13;

    if (!(self->timer % 30) && self->timer < 120)
        RSDK.PlaySfx(HotaruHiWatt->sfxFly, (self->timer % 30), 0xFF);

    self->angle += self->velocity.x;

    ++self->timer;
    self->position.x = self->formationCircleRadius * RSDK.Cos1024(self->angle >> 14) + self->originPos.x;
    self->position.y = self->formationCircleRadius * RSDK.Sin1024(self->angle >> 14) + self->originPos.y;

    if (!(Zone->timer & 3))
        CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_FLASH), self->position.x, self->position.y);

    if (self->timer == 128) {
        self->timer = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxAppear, false, 255);
        self->state = HotaruHiWatt_StateHotaruPair_Charging;
    }
}

void HotaruHiWatt_StateHotaruPair_Charging(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *boss = self->parents[0];

    if (boss->state != HotaruHiWatt_StateBoss_HiddenUndimScreen)
        boss->state = HotaruHiWatt_StateBoss_HiddenUndimScreen;

    RSDK.ProcessAnimation(&self->bulbAnimator);

    if (!self->timer) {
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->bulbAnimator, true, 0);
    }

    self->alpha = 8 * ++self->timer;
    if (self->timer == 32) {
        self->timer      = 0;
        self->velocity.y = 0;

        self->stateDraw = HotaruHiWatt_Draw_HotaruAttacking;
        self->state     = HotaruHiWatt_StateHotaruPair_AttackDelay;

        if (self->parents[1]) {
            for (int32 i = 30; i < 254; i += 28) {
                EntityHotaruHiWatt *orb = CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_ELECTRICORB), self->position.x, self->position.y);
                orb->scale.x            = i;
                orb->angle              = RSDK.Rand(0, 256);
                orb->parents[1]         = self;
                orb->parents[2]         = self->parents[1];
            }
        }
    }
}

void HotaruHiWatt_StateHotaruPair_AttackDelay(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer == 32) {
        self->timer = 0;
        self->state = HotaruHiWatt_StateHotaruPair_AttackMovingDown;
    }
}

void HotaruHiWatt_StateHotaruPair_AttackMovingDown(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->bulbAnimator);

    self->velocity.y += 0x800;

    if (self->velocity.y > 0x20000) {
        self->velocity.y = 0x20000;
        if (self->position.y > HotaruHiWatt->startY + 0x140000)
            self->state = HotaruHiWatt_StateHotaruPair_AttackMovingUp;
    }

    self->position.y += self->velocity.y;

    HotaruHiWatt_CheckPlayerCollisions_Hotaru();
}

void HotaruHiWatt_StateHotaruPair_AttackMovingUp(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->bulbAnimator);

    self->velocity.y -= 0x1000;
    if (self->velocity.y < -0x20000)
        self->velocity.y = -0x20000;

    self->position.y += self->velocity.y;

    if (self->position.y < HotaruHiWatt->startY - 0xC00000) {
        HotaruHiWatt_SetupHHWReappear();
        destroyEntity(self);
    }

    HotaruHiWatt_CheckPlayerCollisions_Hotaru();
}

void HotaruHiWatt_State_SparkyOrb(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->mainAnimator);

    EntityHotaruHiWatt *hotaruPair1 = self->parents[1];
    EntityHotaruHiWatt *hotaruPair2 = self->parents[2];

    if (hotaruPair1->classID == HotaruHiWatt->classID && hotaruPair2->classID == HotaruHiWatt->classID) {
        int32 scale = (RSDK.Sin256(self->angle + 4 * Zone->timer) >> 7) + self->scale.x;

        self->position.x = hotaruPair1->position.x + scale * ((hotaruPair2->position.x - hotaruPair1->position.x) >> 8);
        self->position.y = hotaruPair1->position.y + 0xC0000 + scale * ((hotaruPair2->position.y - hotaruPair1->position.y) >> 8);
        self->position.y += RSDK.Sin256(self->angle + 4 * Zone->timer) << 9;

        HotaruHiWatt_CheckPlayerCollisions_MiniLaser();
    }
    else {
        self->alpha     = 0x100;
        self->inkEffect = INK_ALPHA;
        self->state     = HotaruHiWatt_State_SparkyOrb_FadeOut;

        self->velocity.x = RSDK.Rand(-0x20000, 0x20000);
        self->velocity.y = RSDK.Rand(-0x20000, 0x20000);
    }
}

void HotaruHiWatt_State_SparkyOrb_FadeOut(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 8;
}

void HotaruHiWatt_StateBoss_DimScreen(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->onGround) {
        if (self->timer >= 192) {
            if (!self->playingBossTrack) {
                Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
                self->playingBossTrack = true;
            }
            self->state = HotaruHiWatt_StateBoss_CreateSmallHHWs;
            RSDK.PlaySfx(HotaruHiWatt->sfxHHWAppear, false, 255);
        }
        else {
            self->timer += 8;
        }

        RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);
    }
}

void HotaruHiWatt_StateBoss_CreateSmallHHWs(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer == 224) {
        for (int32 angle = 0x20; angle < 0x120; angle += 0x40) {
            EntityHotaruHiWatt *hotaru = CREATE_ENTITY(HotaruHiWatt, INT_TO_VOID(HHW_SMALL_HOTARU), self->position.x, self->position.y - 0xC0000);
            hotaru->position.x += RSDK.Cos256(angle) << 16;
            hotaru->position.y += RSDK.Sin256(angle) << 16;
            hotaru->velocity.x = (hotaru->targetPos.x - hotaru->position.x) >> 6;
            hotaru->velocity.y = (hotaru->targetPos.y - hotaru->position.y) >> 6;
        }

        self->timer      = 0;
        self->velocity.x = 0x50000;
        self->stateDraw  = HotaruHiWatt_Draw_FormingHHW;
        self->visible    = true;
        self->state      = HotaruHiWatt_StateBoss_FormingHHW;
    }
}

void HotaruHiWatt_StateBoss_FormingHHW(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer >= 60) {
        self->velocity.x -= 0x3800;
        self->formationCircleRadius += self->velocity.x;

        if (self->velocity.x <= 0) {
            self->originPos = self->position;
            self->timer     = 192;
            self->stateDraw = HotaruHiWatt_Draw_FlashAppear;
            self->state     = HotaruHiWatt_StateBoss_FlashAppear;

            RSDK.StopSfx(HotaruHiWatt->sfxHHWAppear);
            RSDK.PlaySfx(HotaruHiWatt->sfxFlash, false, 255);

            EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
            fxFade->speedIn      = 128;
            fxFade->speedOut     = 16;
        }
    }
}

void HotaruHiWatt_StateBoss_FlashAppear(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->headAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    self->velocity.x -= 0x3800;
    self->formationCircleRadius += self->velocity.x;

    if (self->timer > 0)
        self->timer -= 8;

    RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);

    if (self->alpha > 0) {
        self->alpha -= 8;
    }

    if (self->formationCircleRadius <= 0) {
        self->stateDraw = HotaruHiWatt_Draw_Boss;
        self->state     = self->nextState;
    }
}

void HotaruHiWatt_StateBoss_LaserAttackDelay(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->headAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    if (++self->timer >= 24) {
        self->timer     = 0;
        self->alpha     = 0;
        self->stateDraw = HotaruHiWatt_Draw_BossLaserCharging;
        RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 2, &self->bulbFlashAnimator, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 7, &self->mainAnimator, true, 0);

        RSDK.PlaySfx(HotaruHiWatt->sfxCharge, false, 255);
        self->state = HotaruHiWatt_StateBoss_ChargingLaser;
    }

    HotaruHiWatt_CheckPlayerCollisions_Boss();
}

void HotaruHiWatt_StateBoss_ChargingLaser(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->headAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    HotaruHiWatt_CheckPlayerCollisions_Boss();

    if (self->state == HotaruHiWatt_StateBoss_ChargingLaser) {
        if (self->alpha < 0x100)
            self->alpha += 4;

        if (++self->timer >= 64) {
            self->timer      = 0;
            self->velocity.x = 0;
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 3, &self->bulbFlashAnimator, true, 0);
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 3, &self->mainAnimator, true, 2);
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 4, &self->laserImpactAnimator, true, 0);

            self->stateDraw = HotaruHiWatt_Draw_BossLaser;
            RSDK.StopSfx(HotaruHiWatt->sfxCharge);

            if (self->position.x < HotaruHiWatt->boundsM)
                self->state = HotaruHiWatt_StateBoss_LaserAttack_Left;
            else
                self->state = HotaruHiWatt_StateBoss_LaserAttack_Right;
        }
    }
}

// Right -> Left
void HotaruHiWatt_StateBoss_LaserAttack_Right(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->headAnimator);
    RSDK.ProcessAnimation(&self->laserImpactAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    self->velocity.x -= 0x1000;
    if (self->velocity.x < -0x20000)
        self->velocity.x = -0x20000;

    self->position.x += self->velocity.x;

    if (self->position.x <= HotaruHiWatt->boundsL) {
        self->position.x = HotaruHiWatt->boundsL;
        self->state      = HotaruHiWatt_StateBoss_FinishedLaserAttack;
    }

    int32 x = self->position.x;
    int32 y = self->position.y;
    self->position.y += 0x800000;
    RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0, 32);

    self->laserImpactPos.x = self->position.x;
    self->laserImpactPos.y = self->position.y;

    self->position.x = x;
    self->position.y = y;
    HotaruHiWatt_CheckPlayerCollisions_BossLaser();
}

// Left -> Right
void HotaruHiWatt_StateBoss_LaserAttack_Left(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->headAnimator);
    RSDK.ProcessAnimation(&self->laserImpactAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    self->velocity.x += 4096;
    if (self->velocity.x > 0x20000)
        self->velocity.x = 0x20000;
    self->position.x += self->velocity.x;

    if (self->position.x >= HotaruHiWatt->boundsR) {
        self->position.x = HotaruHiWatt->boundsR;
        self->state      = HotaruHiWatt_StateBoss_FinishedLaserAttack;
    }

    int32 x = self->position.x;
    int32 y = self->position.y;
    self->position.y += 0x800000;
    RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0, 32);

    self->laserImpactPos.x = self->position.x;
    self->laserImpactPos.y = self->position.y;

    self->position.x = x;
    self->position.y = y;
    HotaruHiWatt_CheckPlayerCollisions_BossLaser();
}

void HotaruHiWatt_StateBoss_FinishedLaserAttack(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->headAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    if (self->alpha > 0)
        self->alpha -= 4;

    if (++self->timer >= 64) {
        self->laserImpactPos.y += 0x800000;
        self->timer      = 0;
        self->stateDraw  = HotaruHiWatt_Draw_Boss;
        self->velocity.y = 0;

        RSDK.PlaySfx(HotaruHiWatt->sfxFlyUp, false, 255);
        self->state = HotaruHiWatt_StateBoss_FlyUp;
    }

    HotaruHiWatt_CheckPlayerCollisions_Boss();
}

void HotaruHiWatt_StateBoss_Destroyed(void)
{
    RSDK_THIS(HotaruHiWatt);

    HotaruHiWatt_Explode();

    if (++self->timer == 60) {
        Debris_CreateFromEntries(HotaruHiWatt->aniFrames, HotaruHiWatt->debrisInfo, 8);
        self->visible = false;
    }
    else if (self->timer == 90) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        self->timer   = 0;
        self->state   = HotaruHiWatt_StateBoss_Finished;
        self->visible = false;
    }
}

void HotaruHiWatt_StateBoss_Finished(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->active     = ACTIVE_NORMAL;
            signPost->state      = SignPost_State_Falling;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }

        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void HotaruHiWatt_EditorDraw(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 0, &self->headAnimator, false, 0);
    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 1, &self->bulbAnimator, false, 0);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->headAnimator, NULL, false);
    RSDK.DrawSprite(&self->bulbAnimator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 0, 1 | 2 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void HotaruHiWatt_EditorLoad(void)
{
    HotaruHiWatt->aniFrames    = RSDK.LoadSpriteAnimation("SSZ1/HotaruHiWatt.bin", SCOPE_STAGE);
    HotaruHiWatt->hotaruFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
}
#endif

void HotaruHiWatt_Serialize(void) {}
