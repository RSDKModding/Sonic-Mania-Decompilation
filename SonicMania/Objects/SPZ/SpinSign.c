// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpinSign Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpinSign *SpinSign;

void SpinSign_Update(void)
{
    RSDK_THIS(SpinSign);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SpinSign->hitboxes[self->type])) {
            int32 vel = (self->type & 1) ? player->velocity.x : player->velocity.y;

            int32 speed = abs((vel >> 7) + (vel >> 6));
            if (speed > self->timer && speed >= 0xC00) {
                self->timer     = speed & -0x80;
                self->active    = ACTIVE_NORMAL;
                self->state     = SpinSign_State_Spinning;
                self->direction = vel < 0;
            }
        }
    }

    StateMachine_Run(self->state);
}

void SpinSign_LateUpdate(void) {}

void SpinSign_StaticUpdate(void) {}

void SpinSign_Draw(void)
{
    RSDK_THIS(SpinSign);

    StateMachine_Run(self->stateDraw);
}

void SpinSign_Create(void *data)
{
    RSDK_THIS(SpinSign);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &self->eggmanAnimator, true, 0);
        RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &self->sidesAnimator, true, 0);
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_SCALE;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->state         = SpinSign_State_Spinning;

        switch (self->type) {
            case SPINSIGN_SONIC_H:
                self->sidesAnimator.frameID = 1;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->frontAnimator, true, 0);
                self->stateDraw = SpinSign_Draw_SonicH;
                break;

            case SPINSIGN_SONIC_V:
                self->sidesAnimator.frameID = 0;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->frontAnimator, true, 0);
                self->stateDraw = SpinSign_Draw_SonicV;
                break;

            case SPINSIGN_MANIA_H:
                self->sidesAnimator.frameID = 1;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->frontAnimator, true, 0);
                self->stateDraw = SpinSign_Draw_ManiaH;
                break;

            case SPINSIGN_MANIA_V:
                self->sidesAnimator.frameID = 2;
                RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->frontAnimator, true, 0);
                self->stateDraw = SpinSign_Draw_ManiaV;
                break;

            default: break;
        }
    }
}

void SpinSign_StageLoad(void)
{
    SpinSign->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/SpinSign.bin", SCOPE_STAGE);

    SpinSign->hitboxes[SPINSIGN_SONIC_H].left   = -24;
    SpinSign->hitboxes[SPINSIGN_SONIC_H].top    = -6;
    SpinSign->hitboxes[SPINSIGN_SONIC_H].right  = 24;
    SpinSign->hitboxes[SPINSIGN_SONIC_H].bottom = 6;

    SpinSign->hitboxes[SPINSIGN_SONIC_V].left   = -6;
    SpinSign->hitboxes[SPINSIGN_SONIC_V].top    = -24;
    SpinSign->hitboxes[SPINSIGN_SONIC_V].right  = 6;
    SpinSign->hitboxes[SPINSIGN_SONIC_V].bottom = 24;

    SpinSign->hitboxes[SPINSIGN_MANIA_H].left   = -24;
    SpinSign->hitboxes[SPINSIGN_MANIA_H].top    = -6;
    SpinSign->hitboxes[SPINSIGN_MANIA_H].right  = 24;
    SpinSign->hitboxes[SPINSIGN_MANIA_H].bottom = 6;

    SpinSign->hitboxes[SPINSIGN_MANIA_V].left   = -6;
    SpinSign->hitboxes[SPINSIGN_MANIA_V].top    = -40;
    SpinSign->hitboxes[SPINSIGN_MANIA_V].right  = 6;
    SpinSign->hitboxes[SPINSIGN_MANIA_V].bottom = 40;

    DEBUGMODE_ADD_OBJ(SpinSign);

    SpinSign->sfxSignPost = RSDK.GetSfx("Global/SignPost.wav");
}

void SpinSign_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(SpinSign, NULL, self->position.x, self->position.y);
}
void SpinSign_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &DebugMode->animator, true, 2);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void SpinSign_State_Spinning(void)
{
    RSDK_THIS(SpinSign);

    if (self->timer) {
        if (self->direction)
            self->angle -= self->timer;
        else
            self->angle += self->timer;

        self->timer -= 0x40;
        if (self->timer < 0x100) {
            if (self->direction && self->angle < 0x4000)
                self->direction ^= FLIP_X;
            else if (!self->direction && self->angle > 0xC000)
                self->direction = FLIP_X;

            self->timer = 0;
            self->state = SpinSign_State_SlowDown;
        }
    }

    self->rotation = (self->angle >> 8) & 0x1FF;
}
void SpinSign_State_SlowDown(void)
{
    RSDK_THIS(SpinSign);

    uint32 ang = self->angle & 0xFFFF0000;
    if (self->direction)
        self->angle -= self->timer;
    else
        self->angle += self->timer;

    self->timer -= 0x20;
    if ((self->angle & 0xFFFF0000) != ang) {
        self->active = ACTIVE_BOUNDS;
        self->timer  = 0;
        self->state  = SpinSign_State_Spinning;
    }

    self->rotation = (self->angle >> 8) & 0x1FF;
}

void SpinSign_Draw_SonicH(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    self->drawFX = FX_SCALE;
    drawPos.x    = self->position.x;

    Animator *animator = self->rotation <= 0x80 || self->rotation >= 0x180 ? &self->eggmanAnimator : &self->frontAnimator;
    animator->frameID  = (Zone->timer >> 4) & 1;

    self->scale.y = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale   = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = self->position.y + (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.y += -0xF00 * self->scale.y - (scale << 9);
            break;

        case 1:
        case 3:
            drawPos.y = self->position.y - (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.y += ((scale - 64) << 9) + 0xF00 * self->scale.y;
            break;

        default: break;
    }

    self->scale.y               = scale;
    self->sidesAnimator.frameID = (animator->frameID + 2) ^ (self->rotation < 256);
    RSDK.DrawSprite(&self->sidesAnimator, &drawPos, false);
}
void SpinSign_Draw_SonicV(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.y    = self->position.y;
    self->drawFX = FX_SCALE;

    Animator *animator = self->rotation <= 0x80 || self->rotation >= 0x180 ? &self->eggmanAnimator : &self->frontAnimator;
    animator->frameID  = (Zone->timer >> 4) & 1;

    self->scale.x = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale   = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = self->position.x + (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.x += -0xF00 * self->scale.x - (scale << 9);
            break;

        case 1:
        case 3:
            drawPos.x = self->position.x - (scale << 9);
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.x += ((scale - 64) << 9) + 0xF00 * self->scale.x;
            break;

        default: break;
    }

    self->scale.x               = scale;
    self->sidesAnimator.frameID = animator->frameID ^ (self->rotation < 256);
    RSDK.DrawSprite(&self->sidesAnimator, &drawPos, false);
}
void SpinSign_Draw_ManiaH(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.x                   = self->position.x;
    self->drawFX                = FX_SCALE;
    self->frontAnimator.frameID = self->rotation <= 0x80 || self->rotation >= 0x180;

    self->scale.y = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale   = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.y = self->position.y + (scale << 9);
            RSDK.DrawSprite(&self->frontAnimator, &drawPos, false);
            drawPos.y += -0x1400 * self->scale.y - (scale << 9);
            break;

        case 1:
        case 3:
            drawPos.y = self->position.y - (scale << 9);
            RSDK.DrawSprite(&self->frontAnimator, &drawPos, false);
            drawPos.y += (scale + 2 * (5 * self->scale.y - 32)) << 9;
            break;

        default: break;
    }

    self->scale.y               = scale;
    self->sidesAnimator.frameID = self->frontAnimator.frameID + 6;
    RSDK.DrawSprite(&self->sidesAnimator, &drawPos, false);
}
void SpinSign_Draw_ManiaV(void)
{
    RSDK_THIS(SpinSign);
    Vector2 drawPos;

    drawPos.y                   = self->position.y;
    self->drawFX                = FX_SCALE;
    self->frontAnimator.frameID = self->rotation <= 0x80 || self->rotation >= 0x180;

    self->scale.x = abs(RSDK.Cos512(self->rotation)) + 1;
    int32 scale   = abs(RSDK.Sin512(self->rotation)) + 1;

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = self->position.x + (scale << 9);
            RSDK.DrawSprite(&self->frontAnimator, &drawPos, false);
            drawPos.x += -0xC00 * self->scale.x - (scale << 9);
            break;

        case 1:
        case 3:
            drawPos.x = self->position.x - (scale << 9);
            RSDK.DrawSprite(&self->frontAnimator, &drawPos, false);
            drawPos.x += (scale + 2 * (3 * self->scale.x - 32)) << 9;
            break;

        default: break;
    }

    self->scale.x               = scale;
    self->sidesAnimator.frameID = self->frontAnimator.frameID + 4;
    RSDK.DrawSprite(&self->sidesAnimator, &drawPos, false);
}

#if GAME_INCLUDE_EDITOR
void SpinSign_EditorDraw(void)
{
    RSDK_THIS(SpinSign);

    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 3, &self->eggmanAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpinSign->aniFrames, 4, &self->sidesAnimator, true, 0);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->scale.x       = 0x200;
    self->scale.y       = 0x200;

    switch (self->type) {
        case SPINSIGN_SONIC_H:
            self->sidesAnimator.frameID = 1;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->frontAnimator, true, 0);
            SpinSign_Draw_SonicH();
            break;

        case SPINSIGN_SONIC_V:
            self->sidesAnimator.frameID = 0;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 0, &self->frontAnimator, true, 0);
            SpinSign_Draw_SonicV();
            break;

        case SPINSIGN_MANIA_H:
            self->sidesAnimator.frameID = 1;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->frontAnimator, true, 0);
            SpinSign_Draw_ManiaH();
            break;

        case SPINSIGN_MANIA_V:
            self->sidesAnimator.frameID = 2;
            RSDK.SetSpriteAnimation(SpinSign->aniFrames, 5, &self->frontAnimator, true, 0);
            SpinSign_Draw_ManiaV();
            break;

        default: break;
    }
}

void SpinSign_EditorLoad(void)
{
    SpinSign->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/SpinSign.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SpinSign, type);
    RSDK_ENUM_VAR("Sonic (Horizontal Spin)", SPINSIGN_SONIC_H);
    RSDK_ENUM_VAR("Sonic (Vertical Spin)", SPINSIGN_SONIC_V);
    RSDK_ENUM_VAR("Mania (Horizontal Spin)", SPINSIGN_MANIA_H);
    RSDK_ENUM_VAR("Mania (Vertical Spin)", SPINSIGN_MANIA_V);
}
#endif

void SpinSign_Serialize(void) { RSDK_EDITABLE_VAR(SpinSign, VAR_UINT8, type); }
