// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WoodChipper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWoodChipper *WoodChipper;

void WoodChipper_Update(void)
{
    RSDK_THIS(WoodChipper);

    StateMachine_Run(self->state);
}

void WoodChipper_LateUpdate(void) {}

void WoodChipper_StaticUpdate(void)
{
    int32 cuttingCount = 0;
    int32 debrisCount  = 0;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
        foreach_active(WoodChipper, chipper)
        {
            if (!chipper->timer)
                cuttingCount++;

            if (chipper->state == WoodChipper_State_Debris)
                debrisCount++;
        }
    }

    if (cuttingCount) {
        if (!WoodChipper->playingWoodSfx) {
            RSDK.PlaySfx(WoodChipper->sfxChipperWood, true, 255);
            WoodChipper->playingWoodSfx = true;
        }
    }
    else if (WoodChipper->playingChipSfx) {
        RSDK.StopSfx(WoodChipper->sfxChipperWood);
        WoodChipper->playingWoodSfx = false;
    }

    if (debrisCount) {
        if (!WoodChipper->playingChipSfx) {
            RSDK.PlaySfx(WoodChipper->sfxChipperChips, true, 255);
            WoodChipper->playingChipSfx = true;
        }
    }
    else if (WoodChipper->playingChipSfx) {
        RSDK.StopSfx(WoodChipper->sfxChipperChips);
        WoodChipper->playingChipSfx = false;
    }
}

void WoodChipper_Draw(void)
{
    RSDK_THIS(WoodChipper);

    if (self->state == WoodChipper_State_Debris) {
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
    else {
        Vector2 drawPos = self->position;

        if (self->height < 0) {
            self->animator.frameID = 3;
            drawPos.y += self->height + (RSDK.Sin256(self->angle) << 10);
            RSDK.DrawSprite(&self->animator, &drawPos, false);

            int32 pos = self->height >> 16;
            if (pos <= -16) {
                self->animator.frameID = 4;
                int32 dist             = -16 - pos;
                if (dist >= -79) {
                    int32 size = dist / 80 + 2;
                    for (int32 i = 0; i < size; ++i) {
                        RSDK.DrawSprite(&self->animator, &drawPos, false);
                        drawPos.y += 0x500000;
                    }
                }
            }
        }

        drawPos.x                 = self->position.x - 0x140000;
        drawPos.y                 = self->position.y - 0x280000;
        self->drawFX              = FX_ROTATE;
        self->sawAnimator.frameID = 0;
        RSDK.DrawSprite(&self->sawAnimator, &drawPos, false);

        drawPos.x += 0x280000;
        self->sawAnimator.frameID = 1;
        self->rotation            = 0x200 - self->rotation;
        RSDK.DrawSprite(&self->sawAnimator, &drawPos, false);

        self->rotation         = 0x200 - self->rotation;
        self->drawFX           = FX_NONE;
        self->animator.frameID = 0;
        drawPos.x              = self->position.x + self->shakeOffsets[0].x;
        drawPos.y              = self->position.y + self->shakeOffsets[0].y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->drawFX           = FX_FLIP;
        self->animator.frameID = 2;
        drawPos.x              = self->position.x + self->shakeOffsets[1].x;
        drawPos.y              = self->position.y + self->shakeOffsets[1].y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->animator.frameID = 6;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->animator.frameID = 1;
        drawPos.x              = self->position.x + self->shakeOffsets[2].x;
        drawPos.y              = self->position.y + self->shakeOffsets[2].y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->drawFX = FX_NONE;
    }
}

void WoodChipper_Create(void *data)
{
    RSDK_THIS(WoodChipper);

    self->active    = ACTIVE_BOUNDS;
    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (!SceneInfo->inEditor) {
        if (data) {
            --self->drawGroup;
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            self->drawFX        = FX_FLIP;
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 1, &self->animator, true, 0);
            self->state = WoodChipper_State_Debris;
        }
        else {
            self->speed <<= 12;
            self->updateRange.x = 0x1000000;
            self->updateRange.y = (self->size + 256) << 16;
            self->height        = -0x10000 * self->size;
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 3, &self->sawAnimator, true, 0);
            self->state = WoodChipper_State_Chipper;
        }
    }
}

void WoodChipper_StageLoad(void)
{
    WoodChipper->active = ACTIVE_ALWAYS;

    WoodChipper->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/WoodChipper.bin", SCOPE_STAGE);

    WoodChipper->hitboxStump.left   = -46;
    WoodChipper->hitboxStump.top    = -48;
    WoodChipper->hitboxStump.right  = 83;
    WoodChipper->hitboxStump.bottom = 32;

    WoodChipper->hitboxWood.left   = -40;
    WoodChipper->hitboxWood.top    = -48;
    WoodChipper->hitboxWood.right  = 40;
    WoodChipper->hitboxWood.bottom = 0;

    WoodChipper->hitboxRazor.left   = -40;
    WoodChipper->hitboxRazor.top    = -58;
    WoodChipper->hitboxRazor.right  = 40;
    WoodChipper->hitboxRazor.bottom = 0;

    WoodChipper->sfxChipperWood  = RSDK.GetSfx("PSZ/ChipperWood.wav");
    WoodChipper->sfxChipperChips = RSDK.GetSfx("PSZ/ChipperChips.wav");
}

void WoodChipper_HandlePlayerCollisions(void)
{
    RSDK_THIS(WoodChipper);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    int32 prevPlayers   = self->activePlayers;
    self->activePlayers = 0;

    WoodChipper->hitboxWood.top = (((RSDK.Sin256(self->angle) << 10) + self->height) >> 16) - 48;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &WoodChipper->hitboxRazor)
            && (!self->height || (prevPlayers && (prevPlayers != 0b10 || !player2->sidekick)))) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, false, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }

        if (WoodChipper->hitboxWood.top > -48) {
            self->height = 0;
            Player_CheckCollisionBox(player, self, &WoodChipper->hitboxStump);
        }
        else if (Player_CheckCollisionBox(player, self, &WoodChipper->hitboxWood) == C_TOP) {
            self->activePlayers |= 1 << player->playerID;

#if MANIA_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                self->timer   = 1;
                player->state = Player_State_Air;
            }
#endif
        }
        else {
            Player_CheckCollisionBox(player, self, &WoodChipper->hitboxStump);
        }
    }
}

void WoodChipper_State_Chipper(void)
{
    RSDK_THIS(WoodChipper);

    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    WoodChipper_HandlePlayerCollisions();

    if (!self->activePlayers || (self->activePlayers == 2 && player2->sidekick)) {
        if (self->height) {
            if (self->timer < 30)
                self->timer++;

            if (self->angle > 0)
                self->angle -= 4;

            self->shakeOffsets[0].x = 0;
            self->shakeOffsets[0].y = 0;
            self->shakeOffsets[1].x = 0;
            self->shakeOffsets[1].y = 0;
            self->shakeOffsets[2].x = 0;
            self->shakeOffsets[2].y = 0;
        }
        else {
            self->shakeOffsets[0].x = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[0].y = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[1].x = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[1].y = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[2].x = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[2].y = 2 * RSDK.Rand(-1, 2);

            self->rotation = (self->rotation + 8) & 0x1FF;
        }
    }
    else {
        if (self->timer <= 0) {
            if (self->height < 0) {
                int32 height = ((RSDK.Sin256(self->angle) << 10) + self->height) & 0xFFFF0000;

                self->height += self->speed;
                if (self->height > 0)
                    self->height = 0;

                if (self->angle < 0x40)
                    self->angle += 4;

                int32 move = height - (((RSDK.Sin256(self->angle) << 10) + self->height) & 0xFFFF0000);

                foreach_active(Player, player)
                {
                    if ((1 << player->playerID) & self->activePlayers)
                        player->position.y -= move;
                }

                for (int32 i = self->speed >> 12; i > 0; --i) {
                    int32 x                   = self->position.x;
                    int32 y                   = self->position.y + ((RSDK.Rand(0, 17) - 40) << 16);
                    EntityWoodChipper *debris = CREATE_ENTITY(WoodChipper, INT_TO_VOID(true), x, y);

                    if (self->direction) {
                        debris->position.x += -0x530000 - (RSDK.Rand(-3, 4) << 16);
                        debris->velocity.x = -RSDK.Rand(4, 17) << 15;
                    }
                    else {
                        debris->position.x += (RSDK.Rand(-3, 4) + 83) << 16;
                        debris->velocity.x = RSDK.Rand(4, 17) << 15;
                    }

                    debris->animator.speed = RSDK.Rand(1, 3);
                    debris->direction      = RSDK.Rand(0, 4);
                }
            }

            self->shakeOffsets[0].x = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[0].y = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[1].x = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[1].y = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[2].x = 2 * RSDK.Rand(-1, 2);
            self->shakeOffsets[2].y = 2 * RSDK.Rand(-1, 2);

            self->rotation = (self->rotation + 8) & 0x1FF;
        }
        else {
            self->timer--;

            self->shakeOffsets[0].x = 0;
            self->shakeOffsets[0].y = 0;
            self->shakeOffsets[1].x = 0;
            self->shakeOffsets[1].y = 0;
            self->shakeOffsets[2].x = 0;
            self->shakeOffsets[2].y = 0;

            int32 height = ((RSDK.Sin256(self->angle) << 10) + (self->height & 0xFC00)) & 0xFFFF0000;

            if (self->angle < 0x40)
                self->angle += 4;

            int32 move = height - (((RSDK.Sin256(self->angle) << 10) + (self->height & 0xFC00)) & 0xFFFF0000);

            foreach_active(Player, player)
            {
                if ((1 << player->playerID) & self->activePlayers)
                    player->position.y -= move;
            }
        }
    }
}

void WoodChipper_State_Debris(void)
{
    RSDK_THIS(WoodChipper);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void WoodChipper_EditorDraw(void)
{
    RSDK_THIS(WoodChipper);

    self->updateRange.x = 0x1000000;
    self->updateRange.y = (self->size + 256) << 16;
    self->height        = -0x10000 * self->size;

    RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 3, &self->sawAnimator, true, 0);

    WoodChipper_Draw();
}

void WoodChipper_EditorLoad(void)
{
    WoodChipper->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/WoodChipper.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(WoodChipper, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void WoodChipper_Serialize(void)
{
    RSDK_EDITABLE_VAR(WoodChipper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(WoodChipper, VAR_UINT16, size);
    RSDK_EDITABLE_VAR(WoodChipper, VAR_ENUM, speed);
}
