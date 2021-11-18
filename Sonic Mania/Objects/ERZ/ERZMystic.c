#include "SonicMania.h"

ObjectERZMystic *ERZMystic;

void ERZMystic_Update(void)
{
    RSDK_THIS(ERZMystic);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);
    RSDK.ProcessAnimation(&self->animator1);
}

void ERZMystic_LateUpdate(void) {}

void ERZMystic_StaticUpdate(void) {}

void ERZMystic_Draw(void)
{
    RSDK_THIS(ERZMystic);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void ERZMystic_Create(void *data)
{
    RSDK_THIS(ERZMystic);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->field_90      = 0;
        self->field_94      = 1;
        self->field_98      = 2;
        self->cupAlpha[0]   = 0xC0;
        self->cupAlpha[1]   = 0x00;
        self->cupAlpha[2]   = 0xC0;
        self->hitbox.left   = -12;
        self->hitbox.top    = -12;
        self->hitbox.right  = 12;
        self->hitbox.bottom = 12;
        self->correctCup    = 1;
        self->state         = ERZMystic_State_Unknown1;
        self->stateDraw     = ERZMystic_StateDraw_Unknown1;
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 2, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->animator3, true, 1);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 1, &self->animator4, true, 0);
    }
}

void ERZMystic_StageLoad(void) { ERZMystic->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomMystic.bin", SCOPE_STAGE); }

void ERZMystic_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZMystic);

    int storeX         = self->position.x;
    int storeY         = self->position.y;
    self->position.x = self->mysticPos.x;
    self->position.y = self->mysticPos.y;

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            self->invincibilityTimer = 48;
            RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
        }

        if (self->animator5.frameID > 8 && self->animator5.frameID < 26) {
            for (int i = 0; i < 3; ++i) {
                if (i != self->correctCup) {
                    self->position.x = storeX + self->cupPos[i];
                    if (abs(self->position.x - player->position.x) < 0x400000 && player->position.y > self->position.y)
                        Player_CheckHit(player, self);
                }
            }
        }

        self->position.x = self->mysticPos.x;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void ERZMystic_HandleCups(void)
{
    RSDK_THIS(ERZMystic);

    self->field_B4 = RSDK.Rand(0, 3);

    switch (self->field_B4) {
        case 0: self->field_B8 = (RSDK.Rand(0, 256) > 128) + 1; break;
        case 1: self->field_B8 = RSDK.Rand(0, 256) <= 128 ? 2 : 0; break;
        case 2: self->field_B8 = RSDK.Rand(0, 256) <= 128; break;
    }

    self->field_BC = self->cupPos[self->field_B4];
    self->field_C4 = self->cupAlpha[self->field_B4];

    self->field_C0 = self->cupPos[self->field_B8];
    self->field_C8 = self->cupAlpha[self->field_B8];
}

void ERZMystic_StateDraw_Unknown1(void)
{
    RSDK_THIS(ERZMystic);
    Vector2 drawPos;

    RSDK.DrawSprite(&self->animator1, &self->mysticPos, false);
    for (int i = 0; i < 3; ++i) {
        drawPos.x = self->position.x + self->cupPos[i];
        if (i == 1) {
            drawPos.y      = self->cupY;
            self->drawFX = FX_ROTATE | FX_FLIP;
        }
        else {
            drawPos.y = self->position.y;
        }

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->inkEffect = INK_ALPHA;
        self->drawFX    = FX_FLIP;
        self->alpha     = self->cupAlpha[i];
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->inkEffect = INK_NONE;
    }
}

void ERZMystic_StateDraw_Unknown2(void)
{
    RSDK_THIS(ERZMystic);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 128, 0, 128, 128);
        RSDK.DrawSprite(&self->animator1, &self->mysticPos, false);
        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.DrawSprite(&self->animator1, &self->mysticPos, false);
    }

    for (int i = 0; i < 3; ++i) {
        drawPos.x         = self->position.x + self->cupPos[i];
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->inkEffect = INK_ALPHA;
        self->alpha     = self->cupAlpha[i];
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->inkEffect = INK_NONE;
        if (i != self->correctCup)
            RSDK.DrawSprite(&self->animator5, &drawPos, false);
    }
}

void ERZMystic_State_Unknown1(void)
{
    RSDK_THIS(ERZMystic);

    self->mysticPos.x = self->position.x;
    self->velocity.y  = 0x40000;
    self->field_70    = -0x80000;
    self->mysticPos.y = self->position.y;
    self->cupY        = self->position.y;
    self->state       = ERZMystic_State_Unknown2;
}

void ERZMystic_State_Unknown2(void)
{
    RSDK_THIS(ERZMystic);

    self->field_70 += 0x3800;
    self->mysticPos.y += self->field_70;
    self->velocity.y -= 0x2800;
    self->cupY += self->velocity.y;

    if (self->cupY <= self->position.y) {
        self->velocity.y = 0;
        self->cupY       = self->position.y;
    }

    self->cupPos[0] += (-0x800000 - self->cupPos[0]) >> 4;
    self->cupPos[2] += ((0x800000 - self->cupPos[2]) >> 4);

    if (self->rotation < 256) {
        self->rotation += 16;
    }

    if (++self->timer == 60) {
        self->cupPos[0] = -0x800000;
        self->cupPos[2] = 0x800000;
        self->timer     = 0;
        self->state     = ERZMystic_State_Unknown3;
    }
}

void ERZMystic_State_Unknown3(void)
{
    RSDK_THIS(ERZMystic);

    self->field_70 += 0x3800;
    self->mysticPos.y += self->field_70;
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x380000, true))
        self->velocity.y = 0;

    if (self->mysticPos.y >= self->position.y - 0x180000) {
        self->mysticPos.y = 0;
        self->field_70    = 0;
        self->onGround    = false;
        self->state       = ERZMystic_State_Unknown4;
    }
}

void ERZMystic_State_Unknown4(void)
{
    RSDK_THIS(ERZMystic);

    self->velocity.y += 0x4000;
    self->position.y += self->velocity.y;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x380000, true)) {
        if (!self->onGround)
            Camera_ShakeScreen(0, 0, 4);
        self->velocity.y = 0;
        self->onGround   = true;
    }

    if (self->rotation >= 512) {
        self->rotation = 0;
        self->state    = ERZMystic_State_Unknown5;
    }
    else {
        self->rotation += 16;
    }
}

void ERZMystic_State_Unknown5(void)
{
    RSDK_THIS(ERZMystic);

    self->velocity.y += 0x4000;
    self->cupY += self->velocity.y;

    if (self->cupY >= self->position.y) {
        Camera_ShakeScreen(0, 0, 4);
        self->velocity.y = 0;
        self->cupY       = self->position.y;
        self->stateDraw  = ERZMystic_StateDraw_Unknown2;
        self->state      = ERZMystic_State_Unknown6;
    }
}

void ERZMystic_State_Unknown6(void)
{
    RSDK_THIS(ERZMystic);

    ++self->timer;
    if (self->cupAlpha[0] < 144) {
        self->cupAlpha[0] += 4;
        self->cupAlpha[2] += 4;
    }

    if (self->timer == 30) {
        self->timer  = 0;
        self->timer2 = 6;
        ERZMystic_HandleCups();
        self->state = ERZMystic_State_Unknown7;
    }
}

void ERZMystic_State_Unknown7(void)
{
    RSDK_THIS(ERZMystic);

    if (abs(self->field_C0 - self->field_BC) <= 0x800000)
        self->timer += 16;
    else
        self->timer += 8;

    int cup1 = self->field_B4;
    int cup2 = self->field_B8;

    int pos1 = self->field_BC;
    if (self->timer > 0) {
        if (self->timer < 256)
            pos1 += ((self->field_C0 - pos1) >> 8) * ((RSDK.Sin512(self->timer + 384) >> 2) + 128);
        else
            pos1 = self->field_C0;
    }
    self->cupPos[cup1] = pos1;

    int pos2 = self->field_C0;
    if (self->timer > 0) {
        if (self->timer < 256) {
            pos2 += ((self->field_BC - pos2) >> 8) * ((RSDK.Sin512(self->timer + 384) >> 2) + 128);
        }
        else {
            pos2 = self->field_BC;
        }
    }
    self->cupPos[cup2] = pos2;

    self->cupAlpha[cup1] = abs(self->cupPos[cup1]) / 0xE38E;
    self->cupAlpha[cup2] = abs(self->cupPos[cup2]) / 0xE38E;
    if (self->timer == 0x100) {
        self->timer = 0;
        if (self->timer2 <= 1) {
            self->cupY -= 0x600000;
            self->mysticPos.x = self->position.x + self->cupPos[self->correctCup];
            self->mysticPos.y = self->position.y;
            self->field_60.x  = self->mysticPos.x;
            self->field_60.y  = self->mysticPos.y;
            self->velocity.y  = -0x10000;
            self->state       = ERZMystic_State_Unknown8;
        }
        else {
            self->timer2--;
            ERZMystic_HandleCups();
        }
    }
}

void ERZMystic_State_Unknown8(void)
{
    RSDK_THIS(ERZMystic);

    self->velocity.y -= 0x3800;
    self->position.y += self->velocity.y;
    self->cupAlpha[0] -= self->cupAlpha[0] >> 4;
    self->cupAlpha[1] -= self->cupAlpha[1] >> 4;
    self->cupAlpha[2] -= self->cupAlpha[2] >> 4;
    if (self->position.y <= self->cupY) {
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 3, &self->animator5, true, 0);
        self->velocity.y = 0;
        self->position.y = self->cupY;
        self->state      = ERZMystic_State_Unknown9;
    }

    self->angle       = (self->angle + 4) & 0xFF;
    self->mysticPos.y = (RSDK.Sin256(self->angle) << 11) + self->field_60.y;

    if (self->position.y - self->cupY < 0x200000)
        ERZMystic_CheckPlayerCollisions();
}

void ERZMystic_State_Unknown9(void)
{
    RSDK_THIS(ERZMystic);

    RSDK.ProcessAnimation(&self->animator5);
    self->cupAlpha[0] -= self->cupAlpha[0] >> 4;
    self->cupAlpha[1] -= self->cupAlpha[1] >> 4;
    self->cupAlpha[2] -= self->cupAlpha[2] >> 4;
    if (self->animator5.frameID == self->animator5.frameCount - 1) {
        self->field_70 = 0;
        self->state    = ERZMystic_State_Unknown10;
    }

    self->angle       = (self->angle + 4) & 0xFF;
    self->mysticPos.y = (RSDK.Sin256(self->angle) << 11) + self->field_60.y;
    ERZMystic_CheckPlayerCollisions();
}

void ERZMystic_State_Unknown10(void)
{
    RSDK_THIS(ERZMystic);

    self->cupPos[0] += ((self->cupPos[self->correctCup] - self->cupPos[0]) >> 3);
    self->cupPos[1] += ((self->cupPos[self->correctCup] - self->cupPos[1]) >> 3);
    self->cupPos[2] += ((self->cupPos[self->correctCup] - self->cupPos[2]) >> 3);
    self->field_70 -= 0x3000;
    self->mysticPos.y += self->field_70;

    if (self->mysticPos.y < self->position.y)
        self->mysticPos.y = self->position.y;

    ERZMystic_CheckPlayerCollisions();
    if (++self->timer == 60) {
        foreach_all(ERZKing, king)
        {
            king->active     = ACTIVE_NORMAL;
            king->position.x = self->mysticPos.x;
        }
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void ERZMystic_EditorDraw(void)
{
    RSDK_THIS(ERZMystic);
    self->mysticPos   = self->position;
    self->cupY        = self->position.y;
    self->cupAlpha[0] = 0x60;
    self->cupAlpha[1] = 0;
    self->cupAlpha[2] = 0x60;
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 2, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->animator3, true, 1);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 1, &self->animator4, true, 0);

    ERZMystic_StateDraw_Unknown1();
}

void ERZMystic_EditorLoad(void) { ERZMystic->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomMystic.bin", SCOPE_STAGE); }
#endif

void ERZMystic_Serialize(void) {}
