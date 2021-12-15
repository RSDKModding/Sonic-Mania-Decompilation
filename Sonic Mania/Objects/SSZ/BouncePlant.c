// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BouncePlant Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectBouncePlant *BouncePlant;

void BouncePlant_Update(void)
{
    RSDK_THIS(BouncePlant);
    if (self->stood) {
        if (self->speedFlag) {
            self->depression = 256;
        }
        else {
            if (self->depression < 256)
                self->depression += 32;
        }
    }
    else {
        if (self->depression > 0)
            self->depression -= 16;
    }

    self->stood     = false;
    self->speedFlag = false;
    foreach_active(Player, player)
    {
        if (abs(player->position.x - self->position.x) <= 0x320000 && (player->velocity.y >= 0 || player->onGround)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            if (self->stood) {
                int32 posY = BoucePlant_Unknown3(player->position.x) + self->stoodPos.y - (playerHitbox->bottom << 16) - 0x40000;
                if (player->position.y > posY - 0x80000) {
                    player->velocity.x += RSDK.Sin256(self->angle) << 13 >> 8;
                    player->position.y    = posY;
                    player->velocity.y    = 0;
                    player->onGround      = true;
                    player->groundedStore = true;
                    player->angle         = 0;
                    player->collisionMode = CMODE_FLOOR;
                    player->groundVel     = player->velocity.x;
                    if (player->state == Player_State_GlideSlide || player->state == Player_State_KnuxGlideDrop)
                        player->state = Player_State_Ground;
                }
            }
            else {
                int32 posY = BoucePlant_Unknown2(player->position.x) + self->position.y - (playerHitbox->bottom << 16) - 0x40000;
                if (player->position.y > posY - 0x80000 && player->position.y < self->position.y + 0x400000) {
                    player->position.y = posY;
                    if (abs(player->velocity.x) > 0xC0000)
                        self->speedFlag = true;

                    if (abs(player->position.x - self->centerX) >= abs(player->velocity.x)) {
                        player->velocity.x += RSDK.Sin256(self->angle) << 13 >> 8;
                        player->velocity.y    = 0;
                        player->onGround      = true;
                        player->groundedStore = true;
                        player->angle         = 0;
                        player->collisionMode = CMODE_FLOOR;
                        player->groundVel     = player->velocity.x;

                        if (player->state == Player_State_GlideSlide || player->state == Player_State_KnuxGlideDrop

#if RETRO_USE_PLUS 
                            || player->state == Player_State_MightyHammerDrop
#endif
                            ) {
                            player->state = Player_State_Ground;
                        }
                    }
                    else if (self->depression > 160) {
                        if (self->direction == FLIP_NONE)
                            player->velocity.x = -0xB4000;
                        else
                            player->velocity.x = 0xB4000;
                        player->velocity.y = -0xB4000;
                        player->onGround   = false;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->animator, false, 0);
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

                        if (player->state == Player_State_GlideSlide || player->state == Player_State_KnuxGlideDrop
#if RETRO_USE_PLUS
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
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGDIAGONAL, &player->animator, false, 0);
                        RSDK.PlaySfx(BouncePlant->sfxBouncePlant, false, 255);
                    }
                    self->stood      = true;
                    self->timer      = 60;
                    self->stoodPos.x = player->position.x;
                    self->stoodPos.y = self->position.y + BoucePlant_Unknown2(player->position.x);
                }
            }
        }
    }

    if (self->stood) {
        for (int32 i = 0; i < 8; ++i) {
            self->drawPos[i].y = self->stoodPos.y + BoucePlant_Unknown3(self->drawPos[i].x);
        }
    }
    else {
        if (self->timer <= 0) {
            for (int32 i = 0; i < 8; ++i) {
                self->drawPos[i] = self->drawPos2[i];
            }
        }
        else {
            self->timer--;

            for (int32 i = 0; i < 8; ++i) {
                int32 val            = self->unknown[i] + ((self->drawPos2[i].y - self->drawPos[i].y) >> 3) - (self->unknown[i] >> 3);
                self->unknown[i] = val;
                self->drawPos[i].y += val;
            }
        }
    }
}

void BouncePlant_LateUpdate(void) {}

void BouncePlant_StaticUpdate(void) {}

void BouncePlant_Draw(void)
{
    RSDK_THIS(BouncePlant);
    for (int32 i = 0; i < 8; ++i) {
        RSDK.DrawSprite(&self->animator, &self->drawPos[i], false);
        RSDK.DrawSprite(&self->animators[i], &self->drawPos[i], false);
    }
}

void BouncePlant_Create(void *data)
{
    RSDK_THIS(BouncePlant);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        BoucePlant_Unknown1();

        if (self->direction) {
            self->centerX = self->position.x - 0x180000;
            self->angle   = 64;
        }
        else {
            self->centerX = self->position.x + 0x180000;
            self->angle   = 192;
        }

        RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->animator, true, 0);
        for (int32 i = 0; i < 8; ++i) {
            RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->animators[i], true, RSDK.Rand(1, 8));
            self->drawPos[i].x = self->drawPos2[i].x;
            self->drawPos[i].y = self->drawPos2[i].y;
        }
    }
}

void BouncePlant_StageLoad(void)
{
    BouncePlant->aniFrames      = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);
    BouncePlant->hitbox.left    = -50;
    BouncePlant->hitbox.left    = -12;
    BouncePlant->hitbox.right   = 50;
    BouncePlant->hitbox.right   = 8;
    BouncePlant->sfxBouncePlant = RSDK.GetSfx("SSZ1/BouncePlant.wav");
}

void BoucePlant_Unknown1(void)
{
    RSDK_THIS(BouncePlant);

    if (self->direction == FLIP_NONE) {
        int32 x = self->position.x - 0x2A0000;
        for (int32 i = 0; i < 8; ++i) {
            self->drawPos2[i].x = x;
            x += 0xC0000;
        }
    }
    else {
        int32 x = self->position.x + 0x2A0000;
        for (int32 i = 0; i < 8; ++i) {
            self->drawPos2[i].x = x;
            x -= 0xC0000;
        }
    }

    int32 y = self->position.y + 0x2A0000;
    for (int32 i = 0; i < 8; ++i) {
        self->drawPos2[i].y = y;
        y -= 0xC0000;
    }
}

int32 BoucePlant_Unknown2(int32 x)
{
    RSDK_THIS(BouncePlant);

    int32 dist = 0;
    int32 pos  = 0;
    if (self->direction) {
        int32 val = self->position.x - 0x180000;
        dist    = x - self->position.x;
        if (x < val) {
            pos = 3 * (x - val);
        }
        else {
            pos = (x - val) / 3;
        }
    }
    else {
        int32 val = self->position.x + 0x180000;
        dist    = self->position.x - x;
        if (x > val) {
            pos = 3 * (val - x);
        }
        else {
            pos = (val - x) / 3;
        }
    }

    pos += 0x200000;
    dist = clampVal(dist, -0x320000, 0x320000);
    pos  = clampVal(pos, -0x320000, 0x320000);
    return dist + ((self->depression * (pos - dist)) >> 8);
}

int32 BoucePlant_Unknown3(int32 x)
{
    RSDK_THIS(BouncePlant);

    int32 y         = 0;
    int32 distanceX = 0;
    int32 distanceY = 0;
    if (self->direction) {
        y = (x - self->stoodPos.x) >> 15;
        if (x < self->stoodPos.x) {
            distanceX = (self->stoodPos.x - self->drawPos2[7].x) >> 16;
            distanceY = (self->stoodPos.y - self->drawPos2[7].y) >> 16;
        }
        else {
            distanceX = (self->drawPos2[0].x - self->stoodPos.x) >> 16;
            distanceY = (self->drawPos2[0].y - self->stoodPos.y) >> 16;
        }
    }
    else {
        y = (self->stoodPos.x - x) >> 15;
        if (x > self->stoodPos.x) {
            distanceX = (self->drawPos2[7].x - self->stoodPos.x) >> 16;
            distanceY = (self->stoodPos.y - self->drawPos2[7].y) >> 16;
        }
        else {
            distanceX = (self->stoodPos.x - self->drawPos2[0].x) >> 16;
            distanceY = (self->drawPos2[0].y - self->stoodPos.y) >> 16;
        }
    }

    if (distanceX > 0)
        y = y * distanceY / distanceX;
    return y << 15;
}

#if RETRO_INCLUDE_EDITOR
void BouncePlant_EditorDraw(void)
{
    RSDK_THIS(BouncePlant);

    self->drawOrder     = Zone->drawOrderLow;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    BoucePlant_Unknown1();

    if (self->direction) {
        self->centerX = self->position.x - 0x180000;
        self->angle   = 64;
    }
    else {
        self->centerX = self->position.x + 0x180000;
        self->angle   = 192;
    }

    RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->animator, true, 0);
    for (int32 i = 0; i < 8; ++i) {
        RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &self->animators[i], true, 1);
        self->drawPos[i].x = self->drawPos2[i].x;
        self->drawPos[i].y = self->drawPos2[i].y;
    }

    BouncePlant_Draw();
}

void BouncePlant_EditorLoad(void) { BouncePlant->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE); }
#endif

void BouncePlant_Serialize(void) { RSDK_EDITABLE_VAR(BouncePlant, VAR_UINT8, direction); }
