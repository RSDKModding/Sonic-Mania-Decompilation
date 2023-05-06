// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BouncePlant Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBouncePlant *BouncePlant;

void BouncePlant_Update(void)
{
    RSDK_THIS(BouncePlant);
    if (self->stood) {
        if (self->instantRecoil) {
            self->depression = 0x100;
        }
        else {
            if (self->depression < 0x100)
                self->depression += 0x20;
        }
    }
    else {
        if (self->depression > 0)
            self->depression -= 0x10;
    }

    self->stood         = false;
    self->instantRecoil = false;
    foreach_active(Player, player)
    {
        if (abs(player->position.x - self->position.x) <= 0x320000 && (player->velocity.y >= 0 || player->onGround)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            if (self->stood) {
                int32 posY = BoucePlant_GetNodeY(player->position.x) + self->stoodPos.y - (playerHitbox->bottom << 16) - 0x40000;
                if (player->position.y > posY - 0x80000) {
                    player->velocity.x += RSDK.Sin256(self->angle) << 13 >> 8;
                    player->position.y    = posY;
                    player->velocity.y    = 0;
                    player->onGround      = true;
                    player->groundedStore = true;
                    player->angle         = 0;
                    player->collisionMode = CMODE_FLOOR;
                    player->groundVel     = player->velocity.x;
                    if (player->state == Player_State_KnuxGlideSlide || player->state == Player_State_KnuxGlideDrop)
                        player->state = Player_State_Ground;
                }
            }
            else {
                int32 posY = BoucePlant_GetNodeStandY(player->position.x) + self->position.y - (playerHitbox->bottom << 16) - 0x40000;
                if (player->position.y > posY - 0x80000 && player->position.y < self->position.y + 0x400000) {
                    player->position.y = posY;
                    if (abs(player->velocity.x) > 0xC0000)
                        self->instantRecoil = true;

                    if (abs(player->position.x - self->centerX) >= abs(player->velocity.x)) {
                        player->velocity.x += RSDK.Sin256(self->angle) << 13 >> 8;
                        player->velocity.y    = 0;
                        player->onGround      = true;
                        player->groundedStore = true;
                        player->angle         = 0;
                        player->collisionMode = CMODE_FLOOR;
                        player->groundVel     = player->velocity.x;

                        if (player->state == Player_State_KnuxGlideSlide || player->state == Player_State_KnuxGlideDrop

#if MANIA_USE_PLUS
                            || player->state == Player_State_MightyHammerDrop
#endif
                        ) {
                            player->state = Player_State_Ground;
                        }
                    }
                    else if (self->depression > 0xA0) {
                        if (self->direction == FLIP_NONE)
                            player->velocity.x = -0xB4000;
                        else
                            player->velocity.x = 0xB4000;
                        player->velocity.y = -0xB4000;
                        player->onGround   = false;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, false, 0);
                        RSDK.PlaySfx(BouncePlant->sfxBouncePlant, false, 255);
                    }
                    else if (abs(player->groundVel) <= 0xC00000) {
                        player->velocity.x += RSDK.Sin256(self->angle) << 13 >> 8;
                        player->velocity.y    = 0;
                        player->onGround      = true;
                        player->groundedStore = true;
                        player->angle         = 0;
                        player->collisionMode = CMODE_FLOOR;
                        player->groundVel     = player->velocity.x;

                        if (player->state == Player_State_KnuxGlideSlide || player->state == Player_State_KnuxGlideDrop
#if MANIA_USE_PLUS
                            || player->state == Player_State_MightyHammerDrop
#endif
                        ) {
                            player->state = Player_State_Ground;
                        }
                    }
                    else {
                        if (self->direction == FLIP_NONE)
                            player->velocity.x = -0xB4000;
                        else
                            player->velocity.x = 0xB4000;
                        player->velocity.y = -0xB4000;
                        player->onGround   = false;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, false, 0);
                        RSDK.PlaySfx(BouncePlant->sfxBouncePlant, false, 255);
                    }
                    self->stood          = true;
                    self->recoilDuration = 60;
                    self->stoodPos.x     = player->position.x;
                    self->stoodPos.y     = self->position.y + BoucePlant_GetNodeStandY(player->position.x);
                }
            }
        }
    }

    if (self->stood) {
        for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) self->drawPos[i].y = self->stoodPos.y + BoucePlant_GetNodeY(self->drawPos[i].x);
    }
    else {
        if (self->recoilDuration <= 0) {
            for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) self->drawPos[i] = self->nodeStartPos[i];
        }
        else {
            self->recoilDuration--;

            for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) {
                self->recoilVelocity[i] += ((self->nodeStartPos[i].y - self->drawPos[i].y) >> 3) - (self->recoilVelocity[i] >> 3);
                self->drawPos[i].y += self->recoilVelocity[i];
            }
        }
    }
}

void BouncePlant_LateUpdate(void) {}

void BouncePlant_StaticUpdate(void) {}

void BouncePlant_Draw(void)
{
    RSDK_THIS(BouncePlant);

    for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) {
        RSDK.DrawSprite(&self->nodeAnimator, &self->drawPos[i], false);
        RSDK.DrawSprite(&self->decorAnimators[i], &self->drawPos[i], false);
    }
}

void BouncePlant_Create(void *data)
{
    RSDK_THIS(BouncePlant);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        BoucePlant_SetupNodePositions();

        if (self->direction) {
            self->centerX = self->position.x - 0x180000;
            self->angle   = 0x40;
        }
        else {
            self->centerX = self->position.x + 0x180000;
            self->angle   = 0xC0;
        }

        RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->nodeAnimator, true, 0);
        for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) {
            RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->decorAnimators[i], true, RSDK.Rand(1, 8));
            self->drawPos[i].x = self->nodeStartPos[i].x;
            self->drawPos[i].y = self->nodeStartPos[i].y;
        }
    }
}

void BouncePlant_StageLoad(void)
{
    BouncePlant->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);

    BouncePlant->hitbox.left  = -50;
    BouncePlant->hitbox.left  = -12;
    BouncePlant->hitbox.right = 50;
    BouncePlant->hitbox.right = 8;

    BouncePlant->sfxBouncePlant = RSDK.GetSfx("SSZ1/BouncePlant.wav");
}

void BoucePlant_SetupNodePositions(void)
{
    RSDK_THIS(BouncePlant);

    if (self->direction == FLIP_NONE) {
        int32 x = self->position.x - 0x2A0000;
        for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) {
            self->nodeStartPos[i].x = x;
            x += 0xC0000;
        }
    }
    else {
        int32 x = self->position.x + 0x2A0000;
        for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) {
            self->nodeStartPos[i].x = x;
            x -= 0xC0000;
        }
    }

    int32 y = self->position.y + 0x2A0000;
    for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) {
        self->nodeStartPos[i].y = y;
        y -= 0xC0000;
    }
}

int32 BoucePlant_GetNodeStandY(int32 x)
{
    RSDK_THIS(BouncePlant);

    int32 dist = 0;
    int32 pos  = 0;
    if (self->direction == FLIP_NONE) {
        int32 right = self->position.x + 0x180000;
        dist        = self->position.x - x;
        if (x > right)
            pos = 3 * (right - x);
        else
            pos = (right - x) / 3;
    }
    else {
        int32 left = self->position.x - 0x180000;
        dist       = x - self->position.x;
        if (x < left)
            pos = 3 * (x - left);
        else
            pos = (x - left) / 3;
    }

    dist = CLAMP(dist, -0x320000, 0x320000);
    pos  = CLAMP(pos + 0x200000, -0x320000, 0x320000);
    return dist + ((self->depression * (pos - dist)) >> 8);
}

int32 BoucePlant_GetNodeY(int32 x)
{
    RSDK_THIS(BouncePlant);

    int32 y         = 0;
    int32 distanceX = 0;
    int32 distanceY = 0;
    if (self->direction) {
        y = (x - self->stoodPos.x) >> 15;
        if (x < self->stoodPos.x) {
            distanceX = (self->stoodPos.x - self->nodeStartPos[BOUNCEPLANT_NODE_COUNT - 1].x) >> 16;
            distanceY = (self->stoodPos.y - self->nodeStartPos[BOUNCEPLANT_NODE_COUNT - 1].y) >> 16;
        }
        else {
            distanceX = (self->nodeStartPos[0].x - self->stoodPos.x) >> 16;
            distanceY = (self->nodeStartPos[0].y - self->stoodPos.y) >> 16;
        }
    }
    else {
        y = (self->stoodPos.x - x) >> 15;
        if (x > self->stoodPos.x) {
            distanceX = (self->nodeStartPos[BOUNCEPLANT_NODE_COUNT - 1].x - self->stoodPos.x) >> 16;
            distanceY = (self->stoodPos.y - self->nodeStartPos[BOUNCEPLANT_NODE_COUNT - 1].y) >> 16;
        }
        else {
            distanceX = (self->stoodPos.x - self->nodeStartPos[0].x) >> 16;
            distanceY = (self->nodeStartPos[0].y - self->stoodPos.y) >> 16;
        }
    }

    if (distanceX > 0)
        y = y * distanceY / distanceX;
    return y << 15;
}

#if GAME_INCLUDE_EDITOR
void BouncePlant_EditorDraw(void)
{
    RSDK_THIS(BouncePlant);

    self->drawGroup     = Zone->objectDrawGroup[0];
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    BoucePlant_SetupNodePositions();

    if (self->direction) {
        self->centerX = self->position.x - 0x180000;
        self->angle   = 0x40;
    }
    else {
        self->centerX = self->position.x + 0x180000;
        self->angle   = 0xC0;
    }

    RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->nodeAnimator, true, 0);
    for (int32 i = 0; i < BOUNCEPLANT_NODE_COUNT; ++i) {
        RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->decorAnimators[i], true, 1);
        self->drawPos[i].x = self->nodeStartPos[i].x;
        self->drawPos[i].y = self->nodeStartPos[i].y;
    }

    BouncePlant_Draw();
}

void BouncePlant_EditorLoad(void)
{
    BouncePlant->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BouncePlant, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void BouncePlant_Serialize(void) { RSDK_EDITABLE_VAR(BouncePlant, VAR_UINT8, direction); }
