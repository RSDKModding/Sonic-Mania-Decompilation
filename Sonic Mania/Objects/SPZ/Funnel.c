#include "SonicMania.h"

ObjectFunnel *Funnel;

void Funnel_Update(void)
{
    RSDK_THIS(Funnel);

    for (int p = 0; p < Player->playerCount; ++p) {
        if (!entity->playerTimers[p]) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

            if ((1 << p) & entity->activePlayers) {
                if (!Player_CheckValidState(player)) {
                    entity->activePlayers &= ~(1 << p);
                    if (!entity->activePlayers)
                        RSDK.StopSFX(Funnel->sfxFunnel);
                }
                else {
                    if (Player_CheckValidState(player) && player->state == Player_State_None) {
                        if (player->sidekick == false && ++entity->playerScoreTimer[p] >= 30) {
                            entity->playerScoreTimer[p]     = 0;
                            EntityScoreBonus *bonus = CREATE_ENTITY(ScoreBonus, NULL, entity->position.x, entity->position.y - 0x180000);
                            bonus->drawOrder        = Zone->drawOrderHigh;
                            bonus->animator.frameID = 0;
                            Player_GiveScore(player, 100);
                        }

                        entity->playerYVel[p] += 64 + (entity->playerYVel[p] >> 8);

                        int distX = maxVal(((entity->position.y - player->position.y) >> 8) - 0xA00, 0x400);
                        if (distX < 1024)
                            distX = 1024;

                        player->position.x = distX * RSDK.Cos256(entity->playerAngle[p]) + entity->position.x;
                        entity->playerAngle[p] -= entity->playerXVel[p] >> 16;
                        if (entity->playerXVel[p] <= 0)
                            entity->playerXVel[p] = entity->playerXVel[p] - entity->playerYVel[p] - 128;
                        else
                            entity->playerXVel[p] = entity->playerXVel[p] + entity->playerYVel[p] + 128;

                        if (player->position.y >= entity->position.y) {
                            player->scale.y = 0x200;
                            player->scale.x = 0x200;
                        }
                        else {
                            player->drawFX |= FX_SCALE;
                            player->scale.y = ((((player->position.y - entity->position.y) >> 16) * RSDK.Sin256(entity->playerAngle[p])) >> 7) + 0x200;
                            player->scale.x = player->scale.y;
                        }
                        player->velocity.y += (entity->playerYVel[p] >> 5) + 64;
                        if (player->position.y > entity->position.y + 0x140000) {
                            entity->activePlayers &= ~(1 << p);
                            player->drawFX &= ~FX_SCALE;
                            player->interaction     = true;
                            player->tileCollisions  = true;
                            player->scale.y         = 0x200;
                            player->scale.x         = 0x200;
                            player->position.x      = entity->position.x;
                            player->velocity.x      = 0;
                            entity->playerTimers[p] = 32;
                            if (player->camera)
                                player->scrollDelay = 1;
                            if (!entity->activePlayers)
                                RSDK.StopSFX(Funnel->sfxFunnel);
                            Camera->centerBounds.y = 0x20000;
                            player->state          = Player_State_Air;
                        }
                    }
                    else {
                        entity->activePlayers &= ~(1 << p);
                        player->drawFX &= ~FX_SCALE;
                        player->interaction     = true;
                        player->tileCollisions  = true;
                        player->scale.y         = 0x200;
                        player->scale.x         = 0x200;
                        player->position.x      = entity->position.x;
                        player->velocity.x      = 0;
                        entity->playerTimers[p] = 32;
                        if (player->camera)
                            player->scrollDelay = 1;
                        if (!entity->activePlayers)
                            RSDK.StopSFX(Funnel->sfxFunnel);
                        Camera->centerBounds.y = 0x20000;
                        player->state          = Player_State_Air;
                    }
                }
            }
            else {
                if (Player_CheckValidState(player) && Player_CheckCollisionTouch(player, entity, &Funnel->hitbox)) {

                    int dy    = 0;
                    int distY = (entity->position.y - player->position.y) >> 16;
                    if (distY - 10 >= 0)
                        dy = distY - 10;

                    int distX = abs(player->position.x - entity->position.x) >> 16;
                    if (distX <= dy && player->position.y < entity->position.y || player->position.y < entity->position.y - 0x280000 && distX <= 64) {
                        if (player->camera) {
                            player->scrollDelay = 0;
                            Camera_SetupLerp(2, p, entity->position.x, entity->position.y - 0x400000, 8);
                        }

                        if (abs(player->velocity.y) > abs(player->velocity.x)) {
                            if (player->position.x >= entity->position.x)
                                player->velocity.x -= (player->velocity.y >> 1);
                            else
                                player->velocity.x += (player->velocity.y >> 1);
                            player->velocity.y >>= 2;
                        }
                        entity->playerXVel[p] = clampVal(player->velocity.x, -0x100000, 0x100000);

                        int x = ((player->position.x - entity->position.x) >> 16) * ((player->position.x - entity->position.x) >> 16);
                        int y = dy * dy - x;
                        if (player->position.x < entity->position.x)
                            x = -x;
                        entity->playerAngle[p] = RSDK.ATan2(x, y);
                        player->velocity.y -= abs(9 * (player->velocity.x >> 4));
                        if (player->velocity.y < 0x1000)
                            player->velocity.y = 0x1000;
                        if (player->velocity.y > 0x10000)
                            player->velocity.y = 0x10000;
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = 0x80000;
                        else
#endif
                            RSDK.PlaySfx(Funnel->sfxFunnel, false, 255);
                        player->velocity.x  = 0;
                        entity->playerScoreTimer[p] = 0;
                        entity->playerYVel[p] = 0;
                        entity->activePlayers |= 1 << p;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                        player->interaction     = false;
                        player->tileCollisions  = false;
                        player->state           = Player_State_None;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                    }
                    else if (player->velocity.y <= 0x10000) {
                        int max = 0;
                        if (player->position.y >= entity->position.y) {
                            max = 28;
                        }
                        else {
                            max = 0;
                            if (distY >= 0)
                                max = (entity->position.y - player->position.y) >> 16;
                            max += 40;
                        }
                        if (distX < max) {
                            int x = RSDK.Rand(0x20000, maxVal(abs(player->velocity.y + player->velocity.x) >> 8, 0x4000));
                            int y = x;
                            if (player->position.y >= entity->position.y + 0x160000) {
                                x = player->velocity.x;
                            }
                            else {
                                if (player->position.y < entity->position.y && player->velocity.y > 0)
                                    continue;

                                if (player->position.x >= entity->position.x) {
                                    if (player->velocity.x > 0 && player->position.y >= entity->position.y)
                                        continue;
                                }
                                else {
                                    if (player->velocity.x < 0 && player->position.y >= entity->position.y)
                                        continue;
                                    x = -x;
                                }
                            }
                            if (player->position.y < entity->position.y || player->position.y > entity->position.y + 0x160000) {
                                if (player->velocity.y > 0)
                                    continue;
                            }
                            else {
                                y = player->velocity.y;
                            }
                            if (player->playerAnimator.animationID != ANI_FLY) {
                                player->velocity.x = x;
                                player->groundVel  = x;
                            }
                            player->velocity.y  = y;
                            player->onGround    = false;
                            player->jumpAbility = 0;
                            RSDK.PlaySfx(Funnel->sfxPimPom, false, 255);
                        }
                    }
                    else {
                        int max = 0;
                        if (player->position.y >= entity->position.y) {
                            max = 28;
                        }
                        else {
                            max = 0;
                            if (distY >= 0)
                                max = (entity->position.y - player->position.y) >> 16;
                            max += 40;
                        }
                        if (distX < max) {
                            int x = RSDK.Rand(0x20000, maxVal(abs(player->velocity.y + player->velocity.x) >> 8, 0x4000));
                            int y = x;
                            if (player->position.y >= entity->position.y + 0x160000) {
                                x = player->velocity.x;
                                x = -x;
                            }
                            else {
                                if (player->position.y < entity->position.y && player->velocity.y > 0)
                                    continue;
                                else if (player->position.x < entity->position.x) {
                                    if (player->velocity.x < 0 && player->position.y >= entity->position.y)
                                        continue;
                                }
                                else if (player->velocity.x > 0 && player->position.y >= entity->position.y)
                                    continue;
                                else
                                    x = -x;
                            }

                            if (player->position.y < entity->position.y || player->position.y > entity->position.y + 0x160000) {
                                if (player->velocity.y > 0)
                                    continue;
                                y = -y;
                            }
                            else {
                                y = player->velocity.y;
                            }
                            if (player->playerAnimator.animationID != ANI_FLY) {
                                player->velocity.x = x;
                                player->groundVel  = x;
                            }
                            player->velocity.y  = y;
                            player->onGround    = false;
                            player->jumpAbility = 0;
                            RSDK.PlaySfx(Funnel->sfxPimPom, false, 255);
                        }
                    }
                }
            }
        }
        else
            entity->playerTimers[p]--;
    }
}

void Funnel_LateUpdate(void) {}

void Funnel_StaticUpdate(void)
{
    foreach_all(Funnel, funnel) { RSDK.AddDrawListRef(Zone->drawOrderLow, RSDK.GetEntityID(funnel)); }
}

void Funnel_Draw(void)
{
    RSDK_THIS(Funnel);

    entity->animator.frameID = RSDK_sceneInfo->currentDrawGroup == entity->drawOrder;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Funnel_Create(void *data)
{
    RSDK_THIS(Funnel);

    RSDK.SetSpriteAnimation(Funnel->aniFrames, 0, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh + 1;
        entity->state         = Funnel_Unknown1;
    }
}

void Funnel_StageLoad(void)
{
    Funnel->aniFrames     = RSDK.LoadSpriteAnimation("SPZ2/Funnel.bin", SCOPE_STAGE);
    Funnel->hitbox.left   = -64;
    Funnel->hitbox.top    = -48;
    Funnel->hitbox.right  = 64;
    Funnel->hitbox.bottom = 0;
    Funnel->active        = ACTIVE_ALWAYS;
    Funnel->sfxFunnel     = RSDK.GetSFX("SPZ/Funnel.wav");
    Funnel->sfxPimPom     = RSDK.GetSFX("Stage/PimPom.wav");
}

void Funnel_Unknown1(void) {}

#if RETRO_INCLUDE_EDITOR
void Funnel_EditorDraw(void)
{
    RSDK_THIS(Funnel);
    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Funnel_EditorLoad(void) { Funnel->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Funnel.bin", SCOPE_STAGE); }
#endif

void Funnel_Serialize(void) {}
