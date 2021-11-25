#include "SonicMania.h"

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
        RSDK.DrawSprite(&self->animator1, NULL, false);
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
            self->type          = voidToInt(data);
            self->drawOrder     = Zone->drawOrderLow;
            switch (self->type) {
                case HHW_0:
                    self->visible = false;
                    self->drawFX  = FX_FLIP;
                    self->health  = 6;
                    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 0, &self->animator2, true, 0);
                    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 1, &self->animator3, true, 0);
                    self->alpha         = 256;
                    self->state         = HotaruHiWatt_State_SetupBounds;
                    self->hitbox.left   = -24;
                    self->hitbox.top    = -24;
                    self->hitbox.right  = 24;
                    self->hitbox.bottom = 24;
                    break;
                case HHW_1:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->animator1, true, 3);
                    ++self->drawOrder;
                    self->inkEffect = INK_ADD;
                    self->visible   = true;
                    self->health    = 3;
                    self->position.x += RSDK.Rand(-0x500000, 0x500000);
                    self->position.y += RSDK.Rand(-0x800000, 0x800000);
                    self->state         = HotaruHiWatt_State1_Unknown1;
                    self->hitbox.left   = -6;
                    self->hitbox.top    = -6;
                    self->hitbox.right  = 6;
                    self->hitbox.bottom = 6;
                    break;
                case HHW_2:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->animator1, true, 3);
                    self->field_80.x    = self->position.x;
                    self->field_80.y    = self->position.y;
                    self->inkEffect     = INK_ADD;
                    self->visible       = true;
                    self->health        = 3;
                    self->field_74      = 0x800;
                    self->alpha         = -0x200;
                    self->state         = HotaruHiWatt_State2_Unknown1;
                    self->hitbox.left   = -6;
                    self->hitbox.top    = -6;
                    self->hitbox.right  = 6;
                    self->hitbox.bottom = 6;
                    break;
                case HHW_4:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->animator1, true, 3);
                    self->field_88  = self->position;
                    self->inkEffect = INK_ADD;
                    self->visible  = true;
                    self->health   = 3;
                    self->alpha    = 0x100;
                    self->state    = HotaruHiWatt_State1_Unknown2;
                    break;
                case HHW_5:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->animator1, true, 0);
                    self->inkEffect = INK_ADD;
                    self->visible = true;
                    self->health  = 3;
                    self->alpha   = 0x100;
                    self->state   = HotaruHiWatt_State5_Unknown1;
                    break;
                case HHW_MINILASER:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 2, &self->animator1, true, 1);
                    self->state         = HotaruHiWatt_State7_Unknown1;
                    self->visible       = true;
                    self->hitbox.left   = -4;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 4;
                    self->hitbox.bottom = 8;
                    break;
                case HHW_ELECTRICORB:
                    RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 4, &self->animator1, true, 0);
                    self->state         = HotaruHiWatt_State8_Unknown1;
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
    HotaruHiWatt->value8       = false;
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

    Soundboard_LoadSFX("Stage/Zap.wav", true, HotaruHiWatt_ZapCheckCB, NULL);
    Soundboard_LoadSFX("SSZ1/HHWLaser.wav", 32606, HotaruHiWatt_LaserCheckCB, HotaruHiWatt_LaserUpdateCB);
}

bool32 HotaruHiWatt_ZapCheckCB(void)
{
    int count = 0;
    foreach_active(HotaruHiWatt, boss)
    {
        if (boss->state == HotaruHiWatt_State2_Unknown4 || boss->state == HotaruHiWatt_State2_Unknown5 || boss->state == HotaruHiWatt_State2_Unknown7)
            ++count;
    }
    return count > 1;
}

bool32 HotaruHiWatt_LaserCheckCB(void)
{
    int count = 0;
    foreach_active(HotaruHiWatt, boss)
    {
        if (boss->state == HotaruHiWatt_State_Unknown9 || boss->state == HotaruHiWatt_State_Unknown10)
            ++count;
    }
    return count > 0;
}

void HotaruHiWatt_LaserUpdateCB(int sfx)
{
    if (!(Soundboard->sfxPlayingTimer[sfx] % 6))
        Camera_ShakeScreen(0, 0, 2);
}

void HotaruHiWatt_CheckPlayerCollisions2(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBadnikBreak(self, player, false)) {
            if (self->type == HHW_1) {
                HotaruHiWatt_SpawnChildHotaru();
            }
            else {
                int count = 0;
                foreach_active(HotaruHiWatt, boss)
                {
                    if (boss->type == HHW_2)
                        count++;
                }
                if (count == 1)
                    HotaruHiWatt_Unknown16();
            }
            destroyEntity(self);
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            if (--self->health <= 0) {
                self->stateDraw           = HotaruHiWatt_StateDraw_Destroyed;
                self->state               = HotaruHiWatt_State_Destroyed;
                self->timer               = 0;
                SceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
            }
            else {
                self->invincibilityTimer = 48;
                RSDK.PlaySfx(HotaruHiWatt->sfxHit, false, 255);
            }
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions3(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                Player_CheckHit(player, self);
            }
        }
    }
}

void HotaruHiWatt_CheckPlayerCollisions4(void)
{
    RSDK_THIS(HotaruHiWatt);

    foreach_active(Player, player)
    {
        Hitbox hitbox = self->hitbox;
        if (!player->invincibleTimer && !player->blinkTimer)
            hitbox.bottom += 0x80;

        if (Player_CheckBadnikTouch(player, self, &hitbox)) {
            if (self->invincibilityTimer) {
                if (player->position.y > self->position.y) {
                    Player_CheckHit(player, self);
                }
            }
            else if (player->position.y < self->position.y || player->invincibleTimer || player->blinkTimer) {
                if (Player_CheckBossHit(player, self)) {
                    if (--self->health <= 0) {
                        self->stateDraw           = HotaruHiWatt_StateDraw_Destroyed;
                        self->state               = HotaruHiWatt_State_Destroyed;
                        self->timer               = 0;
                        SceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                    }
                    else {
                        self->invincibilityTimer = 48;
                        RSDK.PlaySfx(HotaruHiWatt->sfxHit, false, 255);
                    }
                }
            }
            else {
                Player_CheckHit(player, self);
            }
        }
    }
}

void HotaruHiWatt_StateDraw1_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->inkEffect = INK_ADD;
    if (!(self->timer & 2))
        RSDK.DrawSprite(&self->animator1, NULL, false);

    int alpha     = self->alpha;
    self->alpha = self->timer2;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->alpha = alpha;
}

void HotaruHiWatt_StateDraw1_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->inkEffect = INK_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void HotaruHiWatt_StateDraw_Destroyed(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&self->animator2) == 'b')
        self->direction = FLIP_X;

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&self->animator2, NULL, false);
        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
}

void HotaruHiWatt_StateDraw_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&self->animator2) == 'b')
        self->direction = FLIP_X;
    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    self->inkEffect = INK_NONE;
}

void HotaruHiWatt_StateDraw_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);
    Vector2 drawPos;

    if (RSDK.GetFrameID(&self->animator2) == 'b')
        self->direction = FLIP_X;

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 16);
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 16);
    }
    else {
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
    self->inkEffect         = INK_ALPHA;
    self->animator4.frameID = 0;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    self->animator4.frameID = 1;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    drawPos = self->position;
    drawPos.y += 0x180000 + ((Zone->timer & 7) << 20);
    self->direction = (Zone->timer >> 2) & 1;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator5, &self->drawPos, false);

    self->inkEffect = INK_NONE;
}

void HotaruHiWatt_StateDraw_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.DrawCircle(self->position.x, self->position.y, self->field_74 >> 16, 0xF0F0F0, 128, INK_ADD, false);
    RSDK.DrawCircle(self->position.x, self->position.y, self->field_74 / 0xCCCC, 0xF0F0F0, 128, INK_ADD, false);
}

void HotaruHiWatt_StateDraw_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK.GetFrameID(&self->animator2) == 'b')
        self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animator2, 0, false);
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawCircle(self->position.x, self->position.y, self->field_74 >> 16, 0xF0F0F0, self->alpha >> 1, INK_ADD, false);
    RSDK.DrawCircle(self->position.x, self->position.y, self->field_74 / 0xCCCC, 0xF0F0F0, self->alpha >> 1, INK_ADD, false);
}

void HotaruHiWatt_SpawnChildHotaru(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)self->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_2), player1->position.x, HotaruHiWatt->value7 + 0x200000);
    child->isPermanent        = true;
    child->parents[0]         = (Entity *)parent;

    EntityHotaruHiWatt *child2 = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_2), player1->position.x, HotaruHiWatt->value7 + 0x200000);
    child2->angle += 0x800000;
    child2->isPermanent = true;
    child2->parents[0]  = (Entity *)parent;
    child->parents[1]   = (Entity *)child2;
}

void HotaruHiWatt_Unknown16(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)self->parents[0];
    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= HotaruHiWatt->boundsM)
        parent->position.x = HotaruHiWatt->boundsR;
    else
        parent->position.x = HotaruHiWatt->boundsL;
    parent->position.y = HotaruHiWatt->value7;
    parent->nextState  = HotaruHiWatt_State_Unknown7;
    parent->state      = HotaruHiWatt_State_Unknown1;
}

void HotaruHiWatt_State_SetupBounds(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer >= 8) {
        self->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
        Zone->cameraBoundsR[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->cameraBoundsT[0]     = (self->position.y >> 16) - ScreenInfo->height;
        Zone->cameraBoundsB[0]     = (self->position.y >> 16);
        HotaruHiWatt->boundsL       = (Zone->cameraBoundsL[0] + 64) << 16;
        HotaruHiWatt->boundsR       = (Zone->cameraBoundsR[0] - 64) << 16;
        HotaruHiWatt->boundsM       = self->position.x;
        HotaruHiWatt->boundsT       = (Zone->cameraBoundsT[0] + 48) << 16;
        HotaruHiWatt->boundsB       = (Zone->cameraBoundsB[0] - 8) << 16;

        foreach_active(Fireflies, fireflies)
        {
            if (fireflies->state == Fireflies_State_Unknown1)
                destroyEntity(fireflies);
        }
        self->state = HotaruHiWatt_State_WaitForPlayer;
    }
}

void HotaruHiWatt_State_WaitForPlayer(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        self->position.y += -0x100000 - (ScreenInfo->centerY << 16);
        HotaruHiWatt->value7 = self->position.y;
        self->state        = HotaruHiWatt_State_Unknown1;
        self->nextState    = HotaruHiWatt_State_Unknown5;
    }
}

void HotaruHiWatt_State_Unknown5(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxFlyUp, false, 255);
        self->state = HotaruHiWatt_State_Unknown6;
    }
    HotaruHiWatt_CheckPlayerCollisions();
}

void HotaruHiWatt_State_Unknown6(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->velocity.y -= 0x3800;
    self->field_80.y += self->velocity.y;
    RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    HotaruHiWatt_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->position.x = HotaruHiWatt->boundsM;
        self->position.y = HotaruHiWatt->value7;
        self->state      = self->nextState;
        self->visible    = false;

        EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_1), self->position.x, self->position.y);
        child->isPermanent        = true;
        child->parents[0]         = (Entity *)self;
    }
}

void HotaruHiWatt_State1_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (!(Zone->timer & 3))
        CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_5), self->position.x, self->position.y);

    if (!((self->position.x ^ self->field_88.x) & 0xFFF80000)) {
        if (!((self->position.y ^ self->field_88.y) & 0xFFF80000)) {
            self->position.x = self->field_88.x;
            self->position.y = self->field_88.y;
            self->velocity.x = 0;
            self->velocity.y = 0;
            if (!self->nextState) {
                destroyEntity(self);
            }
            else {
                RSDK.PlaySfx(HotaruHiWatt->sfxAppear, false, 255);
                self->state = self->nextState;
            }
        }
    }
}

void HotaruHiWatt_State5_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator1);
    self->alpha -= 8;
    if (self->alpha <= 0)
        destroyEntity(self);
}

void HotaruHiWatt_StateUnknown_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->timer >= 192)
        self->state = StateMachine_None;
    else
        self->timer += 8;
    RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);
}

void HotaruHiWatt_StateUnknown_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->timer <= 0)
        self->state = StateMachine_None;
    else
        self->timer -= 8;
    RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);
}

void HotaruHiWatt_State1_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)self->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;

    if (self->timer2 >= 256) {
        self->field_88.x = HotaruHiWatt->boundsM + RSDK.Rand(-0x800000, 0x800000);
        self->field_88.y = HotaruHiWatt->value7 + RSDK.Rand(-0x500000, 0x500000);
        self->stateDraw  = HotaruHiWatt_StateDraw1_Unknown2;
        self->velocity.x = (self->field_88.x - self->position.x) / 48;
        self->velocity.y = (self->field_88.y - self->position.y) / 48;
        RSDK.PlaySfx(HotaruHiWatt->sfxFly, false, 255);
        self->nextState = HotaruHiWatt_State1_Unknown3;
        self->state     = HotaruHiWatt_State1_Unknown2;
    }
    else {
        self->timer2 += 16;
    }
}

void HotaruHiWatt_State1_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)self->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown2)
        parent->state = HotaruHiWatt_StateUnknown_Unknown2;
    RSDK.ProcessAnimation(&self->animator3);
    if (!self->timer) {
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->animator3, true, 0);
    }

    self->alpha = 8 * ++self->timer;
    if (self->timer == 32) {
        self->timer     = 0;
        self->stateDraw = HotaruHiWatt_StateDraw1_Unknown1;
        RSDK.PlaySfx(HotaruHiWatt->sfxLaser, false, 255);
        self->state = HotaruHiWatt_State1_Unknown4;
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State1_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator3);
    ++self->timer;
    if (!(self->timer & 1))
        CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_MINILASER), self->position.x, self->position.y + 0x40000);
    if (self->timer == 24) {
        self->timer     = 0;
        self->stateDraw = HotaruHiWatt_StateDraw1_Unknown2;
        self->state     = HotaruHiWatt_State1_Unknown5;
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State1_Unknown5(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)self->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;
    ++self->timer;

    if (self->alpha <= 0) {
        if (self->health >= 1) {
            self->timer = 0;
            self->health--;
            RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->animator1, true, 3);
            self->stateDraw = StateMachine_None;
            self->state     = HotaruHiWatt_State1_Unknown1;
        }
        else {
            self->state = HotaruHiWatt_State1_Unknown6;
        }
    }
    else {
        self->alpha -= 8;
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State1_Unknown6(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (self->timer2 <= 0) {
        HotaruHiWatt_SpawnChildHotaru();
        destroyEntity(self);
    }
    else {
        self->timer2 -= 32;
        HotaruHiWatt_CheckPlayerCollisions2();
    }
}

void HotaruHiWatt_State7_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    self->position.y += 0x80000;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        if (HotaruHiWatt->value8) {
            destroyEntity(self);
        }
        else {
            RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 3, &self->animator1, true, 0);
            self->position.y += 0x80000;
            self->drawOrder    = Zone->drawOrderHigh;
            self->state        = HotaruHiWatt_State7_Unknown2;
            HotaruHiWatt->value8 = true;
        }
    }

    if (self->objectID == HotaruHiWatt->objectID)
        HotaruHiWatt_CheckPlayerCollisions3();
}

void HotaruHiWatt_State7_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == 2)
        HotaruHiWatt->value8 = false;
    if (self->animator1.frameID == self->animator1.frameCount - 1)
        destroyEntity(self);
}

void HotaruHiWatt_State2_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)self->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown1)
        parent->state = HotaruHiWatt_StateUnknown_Unknown1;

    self->position.x = self->field_74 * RSDK.Cos1024(self->angle >> 14) + self->field_80.x;
    self->position.y = self->field_74 * RSDK.Sin1024(self->angle >> 14) + self->field_80.y;

    if (self->alpha >= 256)
        self->state = HotaruHiWatt_State2_Unknown2;
    else
        self->alpha += 16;
}

void HotaruHiWatt_State2_Unknown2(void)
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

    if (self->field_80.x < HotaruHiWatt->boundsM - 0x600000)
        self->field_80.x += 0x20000;
    else if (self->field_80.x > HotaruHiWatt->boundsM + 0x600000)
        self->field_80.x -= 0x20000;

    self->field_80.y -= self->velocity.x >> 2;
    self->field_74 += self->velocity.x >> 13;
    if (!(self->timer % 30) && self->timer < 120)
        RSDK.PlaySfx(HotaruHiWatt->sfxFly, (self->timer % 30), 255);
    self->angle += self->velocity.x;

    ++self->timer;
    self->position.x = self->field_74 * RSDK.Cos1024(self->angle >> 14) + self->field_80.x;
    self->position.y = self->field_74 * RSDK.Sin1024(self->angle >> 14) + self->field_80.y;
    if (!(Zone->timer & 3))
        CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_5), self->position.x, self->position.y);
    if (self->timer == 128) {
        self->timer = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxAppear, false, 255);
        self->state = HotaruHiWatt_State2_Unknown3;
    }
}

void HotaruHiWatt_State2_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    EntityHotaruHiWatt *parent = (EntityHotaruHiWatt *)self->parents[0];
    if (parent->state != HotaruHiWatt_StateUnknown_Unknown2)
        parent->state = HotaruHiWatt_StateUnknown_Unknown2;
    RSDK.ProcessAnimation(&self->animator3);
    if (!self->timer) {
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->hotaruFrames, 1, &self->animator3, true, 0);
    }

    self->alpha = 8 * ++self->timer;
    if (self->timer == 32) {
        self->timer      = 0;
        self->velocity.y = 0;
        self->stateDraw  = HotaruHiWatt_StateDraw1_Unknown1;
        self->state      = HotaruHiWatt_State2_Unknown4;
        if (self->parents[1]) {
            for (int i = 30; i < 254; i += 28) {
                EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_ELECTRICORB), self->position.x, self->position.y);
                child->scale.x            = i;
                child->angle              = RSDK.Rand(0, 256);
                child->parents[1]         = (Entity *)self;
                child->parents[2]         = self->parents[1];
            }
        }
    }
}

void HotaruHiWatt_State2_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer == 32) {
        self->timer = 0;
        self->state = HotaruHiWatt_State2_Unknown5;
    }
}

void HotaruHiWatt_State2_Unknown5(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator3);
    self->velocity.y += 0x800;
    if (self->velocity.y > 0x20000) {
        self->velocity.y = 0x20000;
        if (self->position.y > HotaruHiWatt->value7 + 0x140000)
            self->state = HotaruHiWatt_State2_Unknown7;
    }
    self->position.y += self->velocity.y;
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State2_Unknown7(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator3);
    self->velocity.y -= 0x1000;
    if (self->velocity.y < -0x20000)
        self->velocity.y = -0x20000;
    self->position.y += self->velocity.y;
    if (self->position.y < HotaruHiWatt->value7 - 0xC00000) {
        HotaruHiWatt_Unknown16();
        destroyEntity(self);
    }
    HotaruHiWatt_CheckPlayerCollisions2();
}

void HotaruHiWatt_State8_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator1);
    EntityHotaruHiWatt *parent  = (EntityHotaruHiWatt *)self->parents[1];
    EntityHotaruHiWatt *parent2 = (EntityHotaruHiWatt *)self->parents[2];

    if (parent->objectID == HotaruHiWatt->objectID && parent2->objectID == HotaruHiWatt->objectID) {
        int scale = (RSDK.Sin256(self->angle + 4 * Zone->timer) >> 7) + self->scale.x;

        self->position.x = parent->position.x + scale * ((parent2->position.x - parent->position.x) >> 8);
        self->position.y = parent->position.y + 0xC0000 + scale * ((parent2->position.y - parent->position.y) >> 8);
        self->position.y += RSDK.Sin256(self->angle + 4 * Zone->timer) << 9;
        HotaruHiWatt_CheckPlayerCollisions3();
    }
    else {
        self->alpha      = 0x100;
        self->inkEffect  = INK_ALPHA;
        self->state      = HotaruHiWatt_State8_Unknown2;
        self->velocity.x = RSDK.Rand(-0x20000, 0x20000);
        self->velocity.y = RSDK.Rand(-0x20000, 0x20000);
    }
}

void HotaruHiWatt_State8_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator1);
    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 8;
}

void HotaruHiWatt_State_Unknown1(void)
{
    RSDK_THIS(HotaruHiWatt);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->onGround) {
        if (self->timer >= 192) {
            if (!self->field_A4) {
                Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
                self->field_A4 = 1;
            }
            self->state = HotaruHiWatt_State_Unknown2;
            RSDK.PlaySfx(HotaruHiWatt->sfxHHWAppear, false, 255);
        }
        else {
            self->timer += 8;
        }
        RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);
    }
}

void HotaruHiWatt_State_Unknown2(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer == 224) {

        for (int angle = 0x20; angle < 0x120; angle += 0x40) {
            EntityHotaruHiWatt *child = CREATE_ENTITY(HotaruHiWatt, intToVoid(HHW_4), self->position.x, self->position.y - 0xC0000);
            child->position.x += RSDK.Cos256(angle) << 16;
            child->position.y += RSDK.Sin256(angle) << 16;
            child->velocity.x = (child->field_88.x - child->position.x) >> 6;
            child->velocity.y = (child->field_88.y - child->position.y) >> 6;
        }

        self->timer      = 0;
        self->velocity.x = 0x50000;
        self->stateDraw  = HotaruHiWatt_StateDraw_Unknown3;
        self->visible    = true;
        self->state      = HotaruHiWatt_State_Unknown3;
    }
}

void HotaruHiWatt_State_Unknown3(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (++self->timer >= 60) {
        self->velocity.x -= 0x3800;
        self->field_74 += self->velocity.x;

        if (self->velocity.x <= 0) {
            self->field_80.x = self->position.x;
            self->field_80.y = self->position.y;
            self->timer      = 192;
            self->stateDraw  = HotaruHiWatt_StateDraw_Unknown4;
            self->state      = HotaruHiWatt_State_Unknown4;
            RSDK.StopSfx(HotaruHiWatt->sfxHHWAppear);
            RSDK.PlaySfx(HotaruHiWatt->sfxFlash, false, 255);
            EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
            fxFade->speedIn      = 128;
            fxFade->speedOut     = 16;
        }
    }
}

void HotaruHiWatt_State_Unknown4(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    self->velocity.x -= 0x3800;
    self->field_74 += self->velocity.x;

    if (self->timer > 0)
        self->timer -= 8;
    RSDK.SetLimitedFade(0, 1, 2, self->timer, 128, 192);

    if (self->alpha > 0) {
        self->alpha -= 8;
    }
    if (self->field_74 <= 0) {
        self->stateDraw = HotaruHiWatt_StateDraw_Destroyed;
        self->state     = self->nextState;
    }
}

void HotaruHiWatt_State_Unknown7(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    if (++self->timer >= 24) {
        self->timer     = 0;
        self->alpha     = 0;
        self->stateDraw = HotaruHiWatt_StateDraw_Unknown1;
        RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 2, &self->animator4, true, 0);
        RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 7, &self->animator1, true, 0);
        RSDK.PlaySfx(HotaruHiWatt->sfxCharge, false, 255);
        self->state = HotaruHiWatt_State_Unknown8;
    }
    HotaruHiWatt_CheckPlayerCollisions();
}

void HotaruHiWatt_State_Unknown8(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    HotaruHiWatt_CheckPlayerCollisions();
    if (self->state == HotaruHiWatt_State_Unknown8) {
        if (self->alpha < 256)
            self->alpha += 4;

        if (++self->timer >= 64) {
            self->timer      = 0;
            self->velocity.x = 0;
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 3, &self->animator4, true, 0);
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 3, &self->animator1, true, 2);
            RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 4, &self->animator5, true, 0);
            self->stateDraw = HotaruHiWatt_StateDraw_Unknown2;
            RSDK.StopSfx(HotaruHiWatt->sfxCharge);

            if (self->position.x < HotaruHiWatt->boundsM)
                self->state = HotaruHiWatt_State_Unknown10;
            else
                self->state = HotaruHiWatt_State_Unknown9;
        }
    }
}

void HotaruHiWatt_State_Unknown9(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator5);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    self->velocity.x -= 0x1000;
    if (self->velocity.x < -0x20000)
        self->velocity.x = -0x20000;
    self->position.x += self->velocity.x;

    if (self->position.x <= HotaruHiWatt->boundsL) {
        self->position.x = HotaruHiWatt->boundsL;
        self->state      = HotaruHiWatt_State_Unknown11;
    }
    int x = self->position.x;
    int y = self->position.y;
    self->position.y += 0x800000;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, 32);
    self->drawPos.x  = self->position.x;
    self->drawPos.y  = self->position.y;
    self->position.x = x;
    self->position.y = y;
    HotaruHiWatt_CheckPlayerCollisions4();
}

void HotaruHiWatt_State_Unknown10(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator5);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    self->velocity.x += 4096;
    if (self->velocity.x > 0x20000)
        self->velocity.x = 0x20000;
    self->position.x += self->velocity.x;

    if (self->position.x >= HotaruHiWatt->boundsR) {
        self->position.x = HotaruHiWatt->boundsR;
        self->state      = HotaruHiWatt_State_Unknown11;
    }
    int x = self->position.x;
    int y = self->position.y;
    self->position.y += 0x800000;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, 32);
    self->drawPos.x  = self->position.x;
    self->drawPos.y  = self->position.y;
    self->position.x = x;
    self->position.y = y;
    HotaruHiWatt_CheckPlayerCollisions4();
}

void HotaruHiWatt_State_Unknown11(void)
{
    RSDK_THIS(HotaruHiWatt);

    RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_80.y) & 0xFFFF0000;

    if (self->alpha > 0)
        self->alpha -= 4;
    if (++self->timer >= 64) {
        self->drawPos.y += 0x800000;
        self->timer      = 0;
        self->stateDraw  = HotaruHiWatt_StateDraw_Destroyed;
        self->velocity.y = 0;
        RSDK.PlaySfx(HotaruHiWatt->sfxFlyUp, false, 255);
        self->state = HotaruHiWatt_State_Unknown6;
    }
    HotaruHiWatt_CheckPlayerCollisions();
}

void HotaruHiWatt_State_Destroyed(void)
{
    RSDK_THIS(HotaruHiWatt);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(HotaruHiWatt->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    if (++self->timer == 60) {
        Debris_FallFlickerAnimSetup(HotaruHiWatt->aniFrames, HotaruHiWatt->debrisInfo, 8);
        self->visible = false;
    }
    else if (self->timer == 90) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->timer   = 0;
        self->state   = HotaruHiWatt_State_Finished;
        self->visible = false;
    }
}

void HotaruHiWatt_State_Finished(void)
{
    RSDK_THIS(HotaruHiWatt);
    if (++self->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->active     = ACTIVE_NORMAL;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void HotaruHiWatt_EditorDraw(void)
{
    RSDK_THIS(HotaruHiWatt);
    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 0, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(HotaruHiWatt->aniFrames, 1, &self->animator3, false, 0);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void HotaruHiWatt_EditorLoad(void)
{
    HotaruHiWatt->aniFrames    = RSDK.LoadSpriteAnimation("SSZ1/HotaruHiWatt.bin", SCOPE_STAGE);
    HotaruHiWatt->hotaruFrames = RSDK.LoadSpriteAnimation("SSZ1/HotaruMKII.bin", SCOPE_STAGE);
}
#endif

void HotaruHiWatt_Serialize(void) {}
