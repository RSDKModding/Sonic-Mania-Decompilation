// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Hotaru Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHotaru *Hotaru;

void Hotaru_Update(void)
{
    RSDK_THIS(Hotaru);
    StateMachine_Run(self->state);
}

void Hotaru_LateUpdate(void) {}

void Hotaru_StaticUpdate(void) {}

void Hotaru_Draw(void)
{
    RSDK_THIS(Hotaru);
    Vector2 drawPos;

    self->direction = self->hotaruDir;
    if (self->attackState == HOTARU_ATTACK_ATTACKING) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0x80;
        drawPos.x       = self->offset2.x + self->position.x;
        drawPos.y       = self->offset2.y + self->position.y + 0xC0000;

        drawPos.x += self->electricityOffset.x;
        drawPos.y += self->electricityOffset.y;
        RSDK.DrawSprite(&self->electricityAnimator, &drawPos, false);

        drawPos.x += self->electricityOffset.x;
        drawPos.y += self->electricityOffset.y;
        RSDK.DrawSprite(&self->electricityAnimator, &drawPos, false);

        drawPos.x += self->electricityOffset.x;
        drawPos.y += self->electricityOffset.y;
        RSDK.DrawSprite(&self->electricityAnimator, &drawPos, false);

        self->alpha     = 0x100;
        self->inkEffect = INK_NONE;
    }

    if (!(self->destroyedHotarus & 1)) {
        drawPos.x = self->position.x + self->offset1.x;
        drawPos.y = self->position.y + self->offset1.y;

        if (self->alpha > 0 && (((Zone->timer & 1) << (self->state == Hotaru_State_FinishedAttacking)) & 0x7FFFFFFF)) {
            self->inkEffect = INK_ADD;
            RSDK.DrawSprite(&self->bulbAnimator, &drawPos, false);

            self->inkEffect = INK_NONE;
        }

        RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);
    }

    if (!(self->destroyedHotarus & 2)) {
        self->direction ^= FLIP_X;

        drawPos.x = self->position.x + self->offset2.x;
        drawPos.y = self->position.y + self->offset2.y;

        if (self->alpha > 0 && (((Zone->timer & 1) << (self->state == Hotaru_State_FinishedAttacking)) & 0x7FFFFFFF)) {
            self->inkEffect = INK_ADD;
            RSDK.DrawSprite(&self->bulbAnimator, &drawPos, false);

            self->inkEffect = INK_NONE;
        }

        RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);
    }
}

void Hotaru_Create(void *data)
{
    RSDK_THIS(Hotaru);

    if (!SceneInfo->inEditor) {
        self->distX1 <<= 16;
        self->distX2 <<= 16;
        self->speed <<= 15;

        self->offset2.x     = -self->dist.x;
        self->offset2.y     = -self->dist.y;
        self->offset1       = self->dist;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = MAX(self->distX1, self->distX2) + 0x400000;
        self->updateRange.y = self->dist.y + 0x400000;

        self->drawFX     = FX_ROTATE | FX_FLIP;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->startDir   = self->direction;
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, self->quality, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, 2, &self->bulbAnimator, true, 0);
        RSDK.SetSpriteAnimation(Hotaru->aniFrames, 3, &self->electricityAnimator, true, 0);

        self->state = Hotaru_State_Init;
    }
}

void Hotaru_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Hotaru.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Hotaru.bin", SCOPE_STAGE);

    Hotaru->hitboxBadnik.top    = -6;
    Hotaru->hitboxBadnik.left   = -6;
    Hotaru->hitboxBadnik.right  = 6;
    Hotaru->hitboxBadnik.bottom = 6;

    Hotaru->hitboxTrigger.top    = -32;
    Hotaru->hitboxTrigger.left   = -32;
    Hotaru->hitboxTrigger.right  = 32;
    Hotaru->hitboxTrigger.bottom = 32;

    Hotaru->hitboxElectricity.top    = -8;
    Hotaru->hitboxElectricity.left   = -8;
    Hotaru->hitboxElectricity.right  = 8;
    Hotaru->hitboxElectricity.bottom = 8;

    DEBUGMODE_ADD_OBJ(Hotaru);
}

void Hotaru_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Hotaru, NULL, self->position.x, self->position.y);
}

void Hotaru_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Hotaru_CheckPlayerCollisions(void)
{
    RSDK_THIS(Hotaru);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        if (!(self->destroyedHotarus & 1)) {
            self->position.x += self->offset1.x;
            self->position.y += self->offset1.y;
            if (Player_CheckBadnikTouch(player, self, &Hotaru->hitboxBadnik) && Player_CheckBadnikBreak(player, self, false)) {
                self->destroyedHotarus |= 1;
                self->attackState = HOTARU_ATTACK_IDLE;
            }

            self->position.x = storeX;
            self->position.y = storeY;
        }

        if (!(self->destroyedHotarus & 2)) {
            self->position.x += self->offset2.x;
            self->position.y += self->offset2.y;
            if (Player_CheckBadnikTouch(player, self, &Hotaru->hitboxBadnik) && Player_CheckBadnikBreak(player, &self, false)) {
                self->destroyedHotarus |= 2;
                self->attackState = HOTARU_ATTACK_IDLE;
            }

            self->position.x = storeX;
            self->position.y = storeY;
        }
    }
}

void Hotaru_CheckOffScreen(void)
{
    RSDK_THIS(Hotaru);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->speed >>= 15;
        self->position  = self->startPos;
        self->direction = self->startDir;
        self->distX1    = self->distX1 >> 16;
        self->distX2    = self->distX2 >> 16;
        Hotaru_Create(NULL);
    }
}

void Hotaru_State_Init(void)
{
    RSDK_THIS(Hotaru);

    self->active      = ACTIVE_NORMAL;
    self->hotaruDir   = FLIP_NONE;
    self->alpha       = 0x00;
    self->attackState = HOTARU_ATTACK_IDLE;

    self->state = Hotaru_State_CheckPlayerInRange;
    Hotaru_State_CheckPlayerInRange();
}

void Hotaru_State_CheckPlayerInRange(void)
{
    RSDK_THIS(Hotaru);

    if (self->hotaruDir) {
        self->offset2.x -= self->speed;
        self->offset1.x += self->speed;
        if (self->offset1.x >= self->distX2) {
            self->offset2.x = -self->distX2;
            self->offset1.x = self->distX2;
            self->hotaruDir = FLIP_NONE;
        }
    }
    else {
        self->offset2.x += self->speed;
        self->offset1.x -= self->speed;
        if (self->offset2.x >= self->distX1) {
            self->offset2.x = self->distX1;
            self->offset1.x = -self->distX1;
            self->hotaruDir = FLIP_X;
        }
    }

    Hotaru_CheckPlayerCollisions();

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    foreach_active(Player, player)
    {
        bool32 inRange = false;

        if (!(self->destroyedHotarus & 1)) {
            self->position.x += self->offset1.x;
            self->position.y += self->offset1.y;

            if (Player_CheckCollisionTouch(player, self, &Hotaru->hitboxTrigger))
                inRange = true;
        }

        self->position.x = storeX;
        self->position.y = storeY;

        if (!(self->destroyedHotarus & 2) && !inRange) {
            self->position.x += self->offset2.x;
            self->position.y += self->offset2.y;

            if (Player_CheckCollisionTouch(player, self, &Hotaru->hitboxTrigger))
                inRange = true;
        }

        if (inRange) {
            self->playerPtr = player;
            self->screenID  = player->sidekick ? 0 : player->camera->screenID;

            self->position.x = storeX;
            self->position.y = storeY;
            self->state      = Hotaru_State_FoundPlayer;
            foreach_break;
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void Hotaru_State_FoundPlayer(void)
{
    RSDK_THIS(Hotaru);

    self->state = Hotaru_State_FlyOnScreen;
    Hotaru_State_FlyOnScreen();
}

void Hotaru_State_FlyOnScreen(void)
{
    RSDK_THIS(Hotaru);

    EntityPlayer *player   = self->playerPtr;
    RSDKScreenInfo *screen = &ScreenInfo[self->screenID];

    int32 screenX = (screen->position.x + screen->center.x) << 16;
    int32 screenY = (screen->position.y + screen->center.y) << 16;

    if (self->position.x <= screenX) {
        self->position.x += player->velocity.x + 0x20000;
        if (self->position.x > screenX)
            self->position.x = screenX;
    }
    else {
        self->position.x -= player->velocity.x - 0x20000;
        if (self->position.x < screenX)
            self->position.x = screenX;
    }

    if (self->position.y <= screenY) {
        self->position.y += player->velocity.y + 0x20000;
        if (self->position.y > screenY)
            self->position.y = screenY;
    }
    else {
        self->position.y -= player->velocity.y - 0x20000;
        if (self->position.y < screenY)
            self->position.y = screenY;
    }

    if (self->position.x == screenX && self->position.y == screenY) {
        self->timer = 100;
        self->state = Hotaru_State_AttachedToScreen;
    }

    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOffScreen();
}

void Hotaru_State_AttachedToScreen(void)
{
    RSDK_THIS(Hotaru);

    RSDKScreenInfo *screen = &ScreenInfo[self->screenID];

    self->position.x = (screen->position.x + screen->center.x) << 16;
    self->position.y = (screen->position.y + screen->center.y) << 16;

    if (++self->rotation == 128) {
        self->rotation             = 0;
        self->mainAnimator.frameID = 1;
        self->state                = Hotaru_State_Charging;
    }

    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOffScreen();
}

void Hotaru_State_Charging(void)
{
    RSDK_THIS(Hotaru);

    if (self->alpha >= 0x80) {
        Hitbox hitboxCharge;
        if (self->offset2.x >= self->offset1.x) {
            hitboxCharge.left  = self->offset1.x >> 16;
            hitboxCharge.right = self->offset2.x >> 16;
        }
        else {
            hitboxCharge.left  = self->offset2.x >> 16;
            hitboxCharge.right = self->offset1.x >> 16;
        }

        if (self->offset2.y >= self->offset1.y) {
            hitboxCharge.top    = self->offset1.y >> 16;
            hitboxCharge.bottom = self->offset2.y >> 16;
        }
        else {
            hitboxCharge.top    = self->offset2.y >> 16;
            hitboxCharge.bottom = self->offset1.y >> 16;
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &hitboxCharge)) {
                self->attackState = HOTARU_ATTACK_CHARGING;
            }
        }
    }

    self->alpha += 2;
    if (self->alpha == 0x100) {
        self->timer = 128;

        if (!self->destroyedHotarus && !self->quality) {
            ++self->attackState;

            self->electricityOffset.x = (self->offset1.x - self->offset2.x) >> 2;
            self->electricityOffset.y = (self->offset1.y - self->offset2.y) >> 2;
        }

        self->state = Hotaru_State_Attacking;
    }

    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOffScreen();
}

void Hotaru_State_Attacking(void)
{
    RSDK_THIS(Hotaru);

    RSDK.ProcessAnimation(&self->electricityAnimator);
    if (--self->timer <= 0) {
        self->attackState = HOTARU_ATTACK_IDLE;
        self->alpha       = 0xC0;
        self->state       = Hotaru_State_FinishedAttacking;
    }

    Hotaru_CheckPlayerCollisions();

    if (self->attackState == HOTARU_ATTACK_ATTACKING) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;

        foreach_active(Player, player)
        {
            self->position.x += self->offset2.x;
            self->position.y += self->offset2.y;

            for (int32 i = 0; i < 3; ++i) {
                self->position.x += self->electricityOffset.x;
                self->position.y += self->electricityOffset.y;

                if (Player_CheckCollisionTouch(player, self, &Hotaru->hitboxElectricity))
                    Player_ElementHurt(player, self, SHIELD_LIGHTNING);
            }

            self->position.x = storeX;
            self->position.y = storeY;
        }
    }

    Hotaru_CheckOffScreen();
}

void Hotaru_State_FinishedAttacking(void)
{
    RSDK_THIS(Hotaru);

    self->position.y -= 0x20000;

    Hotaru_CheckPlayerCollisions();
    Hotaru_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Hotaru_EditorDraw(void)
{
    RSDK_THIS(Hotaru);

    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = MAX(self->distX1 << 16, self->distX2 << 16) + 0x400000;
    self->updateRange.y = self->dist.y + 0x400000;

    self->drawFX    = FX_ROTATE | FX_FLIP;
    self->offset2.x = -self->dist.x;
    self->offset2.y = -self->dist.y;
    self->offset1   = self->dist;

    RSDK.SetSpriteAnimation(Hotaru->aniFrames, self->quality, &self->mainAnimator, true, 0);
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 2, &self->bulbAnimator, true, 0);
    RSDK.SetSpriteAnimation(Hotaru->aniFrames, 3, &self->electricityAnimator, true, 0);

    Hotaru_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        // Left Hotaru
        self->offset1.x = -(self->distX1 << 16);
        self->offset2.x = (self->distX1 << 16);
        self->hotaruDir = FLIP_X;
        self->inkEffect = INK_BLEND;
        Hotaru_Draw();

        // Right Hotaru
        self->offset1.x = (self->distX2 << 16);
        self->offset2.x = -(self->distX2 << 16);
        self->hotaruDir = FLIP_NONE;
        self->inkEffect = INK_BLEND;
        Hotaru_Draw();

        self->inkEffect = INK_NONE;

        if (self->distX1 && self->distX2) {
            // Left Hotaru
            DrawHelpers_DrawArrow(self->position.x - (self->distX1 << 16), self->position.y, self->position.x - (self->distX2 << 16),
                                  self->position.y, 0x00FF00, INK_NONE, 0xFF);
            DrawHelpers_DrawArrow(self->position.x - (self->distX2 << 16), self->position.y, self->position.x - (self->distX1 << 16),
                                  self->position.y, 0x00FF00, INK_NONE, 0xFF);

            // Right Hotaru
            DrawHelpers_DrawArrow(self->position.x + (self->distX1 << 16), self->position.y, self->position.x + (self->distX2 << 16),
                                  self->position.y, 0x00FF00, INK_NONE, 0xFF);
            DrawHelpers_DrawArrow(self->position.x + (self->distX2 << 16), self->position.y, self->position.x + (self->distX1 << 16),
                                  self->position.y, 0x00FF00, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Hotaru_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Hotaru.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        Hotaru->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Hotaru.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Hotaru, quality);
    RSDK_ENUM_VAR("Good", HOTARU_GOOD);
    RSDK_ENUM_VAR("Bad", HOTARU_BAD);

    RSDK_ACTIVE_VAR(Hotaru, preset);
    RSDK_ENUM_VAR("(Unused)", 0);
}
#endif

void Hotaru_Serialize(void)
{
    RSDK_EDITABLE_VAR(Hotaru, VAR_UINT8, quality);
    RSDK_EDITABLE_VAR(Hotaru, VAR_VECTOR2, dist);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, distX1);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, distX2);
    RSDK_EDITABLE_VAR(Hotaru, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Hotaru, VAR_UINT8, preset);
}
