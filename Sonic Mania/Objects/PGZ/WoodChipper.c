#include "SonicMania.h"

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
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        foreach_active(WoodChipper, chipper)
        {
            if (!chipper->timer)
                cuttingCount++;

            if (chipper->state == WoodChipper_State_Debris)
                debrisCount++;
        }
    }

    if (cuttingCount) {
        if (!WoodChipper->playingWoodSFX) {
            RSDK.PlaySfx(WoodChipper->sfxChipperWood, true, 255);
            WoodChipper->playingWoodSFX = true;
        }
    }
    else if (WoodChipper->playingChipSFX) {
        RSDK.StopSFX(WoodChipper->sfxChipperWood);
        WoodChipper->playingWoodSFX = false;
    }

    if (debrisCount) {
        if (!WoodChipper->playingChipSFX) {
            RSDK.PlaySfx(WoodChipper->sfxChipperChips, true, 255);
            WoodChipper->playingChipSFX = true;
        }
    }
    else if (WoodChipper->playingChipSFX) {
        RSDK.StopSFX(WoodChipper->sfxChipperChips);
        WoodChipper->playingChipSFX = false;
    }
}

void WoodChipper_Draw(void)
{
    RSDK_THIS(WoodChipper);
    if (self->state == WoodChipper_State_Debris) {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
    else {
        Vector2 drawPos = self->position;

        if (self->field_80 < 0) {
            self->animator1.frameID = 3;
            drawPos.y += self->field_80 + (RSDK.Sin256(self->angle) << 10);
            RSDK.DrawSprite(&self->animator1, &drawPos, false);

            int32 pos = self->field_80 >> 16;
            if (pos <= -16) {
                self->animator1.frameID = 4;
                int32 dist                  = -16 - pos;
                if (dist >= -79) {
                    int32 size = dist / 80 + 2;
                    for (int32 i = 0; i < size; ++i) {
                        RSDK.DrawSprite(&self->animator1, &drawPos, false);
                        drawPos.y += 0x500000;
                    }
                }
            }
        }

        drawPos.x                 = self->position.x - 0x140000;
        drawPos.y                 = self->position.y - 0x280000;
        self->drawFX            = FX_ROTATE;
        self->animator2.frameID = 0;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        drawPos.x += 0x280000;
        self->animator2.frameID = 1;
        self->rotation          = 512 - self->rotation;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->rotation          = 512 - self->rotation;
        self->drawFX            = 0;
        self->animator1.frameID = 0;
        drawPos.x                 = self->position.x + self->field_68[0].x;
        drawPos.y                 = self->position.y + self->field_68[0].y;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        self->drawFX            = FX_FLIP;
        self->animator1.frameID = 2;
        drawPos.x                 = self->position.x + self->field_68[1].x;
        drawPos.y                 = self->position.y + self->field_68[1].y;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        self->animator1.frameID = 6;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        self->animator1.frameID = 1;
        drawPos.x                 = self->position.x + self->field_68[2].x;
        drawPos.y                 = self->position.y + self->field_68[2].y;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
        self->drawFX = FX_NONE;
    }
}

void WoodChipper_Create(void *data)
{
    RSDK_THIS(WoodChipper);
    self->active    = ACTIVE_BOUNDS;
    self->visible   = 1;
    self->drawOrder = Zone->drawOrderLow;
    if (!SceneInfo->inEditor) {
        if (data) {
            --self->drawOrder;
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x400000;
            self->updateRange.y = 0x400000;
            self->drawFX        = FX_FLIP;
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 1, &self->animator1, true, 0);
            self->state = WoodChipper_State_Debris;
        }
        else {
            self->speed <<= 12;
            self->updateRange.x = 0x1000000;
            self->updateRange.y = (self->size + 256) << 16;
            self->field_80      = -0x10000 * self->size;
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 3, &self->animator2, true, 0);
            self->state = WoodChipper_State_Chipper;
        }
    }
}

void WoodChipper_StageLoad(void)
{
    WoodChipper->active             = ACTIVE_ALWAYS;
    WoodChipper->aniFrames          = RSDK.LoadSpriteAnimation("PSZ2/WoodChipper.bin", SCOPE_STAGE);
    WoodChipper->hitboxStump.left   = -46;
    WoodChipper->hitboxStump.top    = -48;
    WoodChipper->hitboxStump.right  = 83;
    WoodChipper->hitboxStump.bottom = 32;
    WoodChipper->hitboxWood.left    = -40;
    WoodChipper->hitboxWood.top     = -48;
    WoodChipper->hitboxWood.right   = 40;
    WoodChipper->hitboxWood.bottom  = 0;
    WoodChipper->hitboxRazor.left   = -40;
    WoodChipper->hitboxRazor.top    = -58;
    WoodChipper->hitboxRazor.right  = 40;
    WoodChipper->hitboxRazor.bottom = 0;
    WoodChipper->sfxChipperWood     = RSDK.GetSFX("PSZ/ChipperWood.wav");
    WoodChipper->sfxChipperChips    = RSDK.GetSFX("PSZ/ChipperChips.wav");
}

void WoodChipper_HandlePlayerCollisions(void)
{
    RSDK_THIS(WoodChipper);
    int32 prevPlayers             = self->activePlayers;
    self->activePlayers       = 0;
    WoodChipper->hitboxWood.top = (((RSDK.Sin256(self->angle) << 10) + self->field_80) >> 16) - 48;
    EntityPlayer *player2       = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &WoodChipper->hitboxRazor)
            && (!self->field_80 || (prevPlayers && (prevPlayers != 2 || !player2->sidekick)))) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer))
#endif
            Player_CheckHit(player, self);
        }

        if (WoodChipper->hitboxWood.top > -48) {
            self->field_80 = 0;
            Player_CheckCollisionBox(player, self, &WoodChipper->hitboxStump);
        }
        else if (Player_CheckCollisionBox(player, self, &WoodChipper->hitboxWood) == 1) {
            self->activePlayers |= 1 << player->playerID;
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                self->timer = 1;
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
    WoodChipper_HandlePlayerCollisions();
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (!self->activePlayers || (self->activePlayers == 2 && player2->sidekick)) {
        if (self->field_80) {
            if (self->timer < 30)
                self->timer++;

            if (self->angle > 0) {
                self->angle -= 4;
            }
            self->field_68[0].x = 0;
            self->field_68[0].y = 0;
            self->field_68[1].x = 0;
            self->field_68[1].y = 0;
            self->field_68[2].x = 0;
            self->field_68[2].y = 0;
        }
        else {
            self->field_68[0].x = 2 * RSDK.Rand(-1, 2);
            self->field_68[0].y = 2 * RSDK.Rand(-1, 2);
            self->field_68[1].x = 2 * RSDK.Rand(-1, 2);
            self->field_68[1].y = 2 * RSDK.Rand(-1, 2);
            self->field_68[2].x = 2 * RSDK.Rand(-1, 2);
            self->field_68[2].y = 2 * RSDK.Rand(-1, 2);
            self->rotation      = (self->rotation + 8) & 0x1FF;
        }
    }
    else {
        if (self->timer <= 0) {
            if (self->field_80 < 0) {
                int32 val = ((RSDK.Sin256(self->angle) << 10) + self->field_80) & 0xFFFF0000;

                self->field_80 += self->speed;
                if (self->field_80 > 0)
                    self->field_80 = 0;

                if (self->angle < 64)
                    self->angle += 4;

                int32 move = val - (((RSDK.Sin256(self->angle) << 10) + self->field_80) & 0xFFFF0000);

                foreach_active(Player, player)
                {
                    if ((1 << player->playerID) & self->activePlayers)
                        player->position.y -= move;
                }

                for (int32 i = self->speed >> 12; i > 0; --i) {
                    EntityWoodChipper *debris =
                        CREATE_ENTITY(WoodChipper, intToVoid(1), self->position.x, ((RSDK.Rand(0, 17) - 40) << 16) + self->position.y);
                    if (self->direction) {
                        debris->position.x += -0x530000 - (RSDK.Rand(-3, 4) << 16);
                        debris->velocity.x = -RSDK.Rand(4, 17) << 15;
                    }
                    else {
                        debris->position.x += (RSDK.Rand(-3, 4) + 83) << 16;
                        debris->velocity.x = RSDK.Rand(4, 17) << 15;
                    }
                    debris->animator1.animationSpeed = RSDK.Rand(1, 3);
                    debris->direction                = RSDK.Rand(0, 4);
                }
            }
            self->field_68[0].x = 2 * RSDK.Rand(-1, 2);
            self->field_68[0].y = 2 * RSDK.Rand(-1, 2);
            self->field_68[1].x = 2 * RSDK.Rand(-1, 2);
            self->field_68[1].y = 2 * RSDK.Rand(-1, 2);
            self->field_68[2].x = 2 * RSDK.Rand(-1, 2);
            self->field_68[2].y = 2 * RSDK.Rand(-1, 2);
            self->rotation      = (self->rotation + 8) & 0x1FF;
        }
        else {
            self->timer--;
            self->field_68[0].x = 0;
            self->field_68[0].y = 0;
            self->field_68[1].x = 0;
            self->field_68[1].y = 0;
            self->field_68[2].x = 0;
            self->field_68[2].y = 0;

            int32 val = ((RSDK.Sin256(self->angle) << 10) + (self->field_80 & 0xFC00)) & 0xFFFF0000;

            if (self->angle < 64)
                self->angle += 4;

            int32 move = val - (((RSDK.Sin256(self->angle) << 10) + (self->field_80 & 0xFC00)) & 0xFFFF0000);

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
    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void WoodChipper_EditorDraw(void) {}

void WoodChipper_EditorLoad(void) {}
#endif

void WoodChipper_Serialize(void)
{
    RSDK_EDITABLE_VAR(WoodChipper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(WoodChipper, VAR_UINT16, size);
    RSDK_EDITABLE_VAR(WoodChipper, VAR_ENUM, speed);
}
