#include "SonicMania.h"

ObjectWoodChipper *WoodChipper;

void WoodChipper_Update(void)
{
    RSDK_THIS(WoodChipper);
    StateMachine_Run(entity->state);
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
    if (entity->state == WoodChipper_State_Debris) {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
    else {
        Vector2 drawPos = entity->position;

        if (entity->field_80 < 0) {
            entity->animator1.frameID = 3;
            drawPos.y += entity->field_80 + (RSDK.Sin256(entity->angle) << 10);
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);

            int32 pos = entity->field_80 >> 16;
            if (pos <= -16) {
                entity->animator1.frameID = 4;
                int32 dist                  = -16 - pos;
                if (dist >= -79) {
                    int32 size = dist / 80 + 2;
                    for (int32 i = 0; i < size; ++i) {
                        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
                        drawPos.y += 0x500000;
                    }
                }
            }
        }

        drawPos.x                 = entity->position.x - 0x140000;
        drawPos.y                 = entity->position.y - 0x280000;
        entity->drawFX            = FX_ROTATE;
        entity->animator2.frameID = 0;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        drawPos.x += 0x280000;
        entity->animator2.frameID = 1;
        entity->rotation          = 512 - entity->rotation;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->rotation          = 512 - entity->rotation;
        entity->drawFX            = 0;
        entity->animator1.frameID = 0;
        drawPos.x                 = entity->position.x + entity->field_68[0].x;
        drawPos.y                 = entity->position.y + entity->field_68[0].y;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        entity->drawFX            = FX_FLIP;
        entity->animator1.frameID = 2;
        drawPos.x                 = entity->position.x + entity->field_68[1].x;
        drawPos.y                 = entity->position.y + entity->field_68[1].y;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        entity->animator1.frameID = 6;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        entity->animator1.frameID = 1;
        drawPos.x                 = entity->position.x + entity->field_68[2].x;
        drawPos.y                 = entity->position.y + entity->field_68[2].y;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        entity->drawFX = FX_NONE;
    }
}

void WoodChipper_Create(void *data)
{
    RSDK_THIS(WoodChipper);
    entity->active    = ACTIVE_BOUNDS;
    entity->visible   = 1;
    entity->drawOrder = Zone->drawOrderLow;
    if (!RSDK_sceneInfo->inEditor) {
        if (data) {
            --entity->drawOrder;
            entity->active        = ACTIVE_NORMAL;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            entity->drawFX        = FX_FLIP;
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 1, &entity->animator1, true, 0);
            entity->state = WoodChipper_State_Debris;
        }
        else {
            entity->speed <<= 12;
            entity->updateRange.x = 0x1000000;
            entity->updateRange.y = (entity->size + 256) << 16;
            entity->field_80      = -0x10000 * entity->size;
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(WoodChipper->aniFrames, 3, &entity->animator2, true, 0);
            entity->state = WoodChipper_State_Chipper;
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
    int32 prevPlayers             = entity->activePlayers;
    entity->activePlayers       = 0;
    WoodChipper->hitboxWood.top = (((RSDK.Sin256(entity->angle) << 10) + entity->field_80) >> 16) - 48;
    EntityPlayer *player2       = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &WoodChipper->hitboxRazor)
            && (!entity->field_80 || (prevPlayers && (prevPlayers != 2 || !player2->sidekick)))
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(1024, player, false, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, entity);
        }

        if (WoodChipper->hitboxWood.top > -48) {
            entity->field_80 = 0;
            Player_CheckCollisionBox(player, entity, &WoodChipper->hitboxStump);
        }
        else if (Player_CheckCollisionBox(player, entity, &WoodChipper->hitboxWood) == 1) {
            entity->activePlayers |= 1 << player->playerID;
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                entity->timer = 1;
                player->state = Player_State_Air;
            }
#endif
        } 
        else {
            Player_CheckCollisionBox(player, entity, &WoodChipper->hitboxStump);
        }
    }
}

void WoodChipper_State_Chipper(void)
{
    RSDK_THIS(WoodChipper);
    WoodChipper_HandlePlayerCollisions();
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (!entity->activePlayers || (entity->activePlayers == 2 && player2->sidekick)) {
        if (entity->field_80) {
            if (entity->timer < 30)
                entity->timer++;

            if (entity->angle > 0) {
                entity->angle -= 4;
            }
            entity->field_68[0].x = 0;
            entity->field_68[0].y = 0;
            entity->field_68[1].x = 0;
            entity->field_68[1].y = 0;
            entity->field_68[2].x = 0;
            entity->field_68[2].y = 0;
        }
        else {
            entity->field_68[0].x = 2 * RSDK.Rand(-1, 2);
            entity->field_68[0].y = 2 * RSDK.Rand(-1, 2);
            entity->field_68[1].x = 2 * RSDK.Rand(-1, 2);
            entity->field_68[1].y = 2 * RSDK.Rand(-1, 2);
            entity->field_68[2].x = 2 * RSDK.Rand(-1, 2);
            entity->field_68[2].y = 2 * RSDK.Rand(-1, 2);
            entity->rotation      = (entity->rotation + 8) & 0x1FF;
        }
    }
    else {
        if (entity->timer <= 0) {
            if (entity->field_80 < 0) {
                int32 val = ((RSDK.Sin256(entity->angle) << 10) + entity->field_80) & 0xFFFF0000;

                entity->field_80 += entity->speed;
                if (entity->field_80 > 0)
                    entity->field_80 = 0;

                if (entity->angle < 64)
                    entity->angle += 4;

                int32 move = val - (((RSDK.Sin256(entity->angle) << 10) + entity->field_80) & 0xFFFF0000);

                foreach_active(Player, player)
                {
                    if ((1 << player->playerID) & entity->activePlayers)
                        player->position.y -= move;
                }

                for (int32 i = entity->speed >> 12; i > 0; --i) {
                    EntityWoodChipper *debris =
                        CREATE_ENTITY(WoodChipper, intToVoid(1), entity->position.x, ((RSDK.Rand(0, 17) - 40) << 16) + entity->position.y);
                    if (entity->direction) {
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
            entity->field_68[0].x = 2 * RSDK.Rand(-1, 2);
            entity->field_68[0].y = 2 * RSDK.Rand(-1, 2);
            entity->field_68[1].x = 2 * RSDK.Rand(-1, 2);
            entity->field_68[1].y = 2 * RSDK.Rand(-1, 2);
            entity->field_68[2].x = 2 * RSDK.Rand(-1, 2);
            entity->field_68[2].y = 2 * RSDK.Rand(-1, 2);
            entity->rotation      = (entity->rotation + 8) & 0x1FF;
        }
        else {
            entity->timer--;
            entity->field_68[0].x = 0;
            entity->field_68[0].y = 0;
            entity->field_68[1].x = 0;
            entity->field_68[1].y = 0;
            entity->field_68[2].x = 0;
            entity->field_68[2].y = 0;

            int32 val = ((RSDK.Sin256(entity->angle) << 10) + (entity->field_80 & 0xFC00)) & 0xFFFF0000;

            if (entity->angle < 64)
                entity->angle += 4;

            int32 move = val - (((RSDK.Sin256(entity->angle) << 10) + (entity->field_80 & 0xFC00)) & 0xFFFF0000);

            foreach_active(Player, player)
            {
                if ((1 << player->playerID) & entity->activePlayers)
                    player->position.y -= move;
            }
        }
    }
}

void WoodChipper_State_Debris(void)
{
    RSDK_THIS(WoodChipper);
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void WoodChipper_EditorDraw(void) {}

void WoodChipper_EditorLoad(void) {}

void WoodChipper_Serialize(void)
{
    RSDK_EDITABLE_VAR(WoodChipper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(WoodChipper, VAR_UINT16, size);
    RSDK_EDITABLE_VAR(WoodChipper, VAR_ENUM, speed);
}
