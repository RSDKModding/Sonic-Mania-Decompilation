#include "../SonicMania.h"

ObjectPullChain *PullChain;

void PullChain_Update(void)
{
    RSDK_THIS(PullChain);
    if (!entity->decorMode) {
        if (entity->flag)
            entity->flag = false;
        foreach_active(Player, player)
        {
            int plrID = RSDK.GetEntityID(player);
            if (entity->timer[plrID] > 0)
                entity->timer[plrID]--;
            if (!player->sidekick) {
                if (((1 << plrID) & entity->activePlayers1)) {
                    if (entity->chainOffset < 0x100000)
                        entity->chainOffset += 0x8000;
                    bool32 flag = entity->chainOffset == 0x100000;
                    if (entity->chainOffset > 0x100000) {
                        entity->chainOffset = 0x100000;
                        flag                = entity->chainOffset == 0x100000;
                    }

                    if (flag) {
                        if (!entity->field_64) {
                            entity->flag     = true;
                            entity->field_70 = 1;
                            entity->field_68 = entity->field_68 == 0;
                        }
                        entity->field_64 = 1;
                    }
                }
                entity->position.y = entity->basePos.y + entity->chainOffset;
            }

            int playerID = 1 << plrID;
            if (!((1 << plrID) & entity->activePlayers1)) {
                if (!(entity->activePlayers2 & playerID)) {
                    if (!Current || true/*!(playerID & Current->activePlayers)*/) {
                        int x = abs(player->position.x - entity->position.x);
                        int y = abs((player->position.y - 0x180000) - entity->position.y);
                        if (MathHelpers_Unknown6((y >> 16) * (y >> 16) + (x >> 16) * (x >> 16)) <= 8 && player->state != Player_State_None
                            && !entity->timer[plrID]) {
                            entity->activePlayers1 |= (1 << plrID);
                            entity->activePlayers2 |= (1 << plrID);
                            RSDK.PlaySFX(Player->sfx_Grab, 0, 255);
                            if (!player->sidekick)
                                RSDK.PlaySFX(PullChain->sfxPullChain, 0, 255);
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, true, 6);
                            player->nextGroundState = 0;
                            player->nextAirState    = 0;
                            player->velocity.x      = 0;
                            player->velocity.y      = 0;
                            player->state           = Player_State_None;
                            if (!player->sidekick) {
                                entity->field_C4  = 0;
                                entity->field_C8  = 0;
                                entity->field_D4  = 0;
                                entity->field_D8  = 0;
                                entity->field_E4  = 0;
                                entity->field_E8  = 0;
                                entity->field_F4  = 0;
                                entity->field_F8  = 0;
                                entity->field_104 = 0;
                                entity->field_108 = 0;
                            }
                        }
                    }
                }
            }

            if ((playerID & entity->activePlayers1)) {
                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                player->position.y += 0x1C0000;
                if (player->jumpPress || player->playerAnimator.animationID != ANI_HANG || player->velocity.x || player->velocity.y) {
                    entity->activePlayers1 &= ~(1 << plrID);
                    if (player->jumpPress) {
                        if (entity->chainOffset < 0x100000 && !player->sidekick)
                            RSDK.StopSFX(PullChain->sfxPullChain);
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                        player->velocity.x = 0;
                        player->velocity.y = -0x20000;
                        player->state      = Player_State_Air;
                    }
                    entity->timer[plrID] = 30;
                }
                if ((playerID & entity->activePlayers1))
                    continue;
            }

            if ((entity->activePlayers2 & playerID)) {
                int x = abs(player->position.x - entity->position.x);
                int y = abs(player->position.y - 0x180000 - entity->position.y);
                if (MathHelpers_Unknown6((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) > 4)
                    entity->activePlayers2 &= ~(1 << plrID);
            }
        }

        if (!entity->activePlayers1) {
            entity->field_64 = 0;
            if (entity->chainOffset > 0)
                entity->chainOffset -= 0x8000;
            if (entity->chainOffset < 0)
                entity->chainOffset = 0;
        }
    }
}

void PullChain_LateUpdate(void) {}

void PullChain_StaticUpdate(void) {}

void PullChain_Draw(void)
{
    RSDK_THIS(PullChain);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    for (int i = 0; i < entity->length; ++i) {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        drawPos.y -= 0x80000;
    }

    for (int i = 0; i < entity->chainOffset; i += 0x80000) {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        drawPos.y -= 0x80000;
    }
}

void PullChain_Create(void *data)
{
    RSDK_THIS(PullChain);
    entity->active = ACTIVE_BOUNDS;
    if (!entity->decorMode)
        entity->drawOrder = Zone->playerDrawLow;
    else
        entity->drawOrder = Zone->drawOrderLow;
    entity->basePos       = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->hitbox.left   = -10;
    entity->hitbox.top    = 63;
    entity->hitbox.right  = 10;
    entity->hitbox.bottom = 176;
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 0, &entity->animator, true, entity->decorMode);
    RSDK.SetSpriteAnimation(PullChain->aniFrames, 1, &entity->animator2, true, entity->decorMode);
}

void PullChain_StageLoad(void)
{
    PullChain->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/PullChain.bin", SCOPE_STAGE);
    PullChain->sfxPullChain = RSDK.GetSFX("HCZ/PullChain.wav");
}

void PullChain_EditorDraw(void) {}

void PullChain_EditorLoad(void) {}

void PullChain_Serialize(void)
{
    RSDK_EDITABLE_VAR(PullChain, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(PullChain, VAR_BOOL, walkOnto);
    RSDK_EDITABLE_VAR(PullChain, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(PullChain, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(PullChain, VAR_BOOL, decorMode);
}
