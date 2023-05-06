// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Funnel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFunnel *Funnel;

void Funnel_Update(void)
{
    RSDK_THIS(Funnel);

    for (int32 p = 0; p < Player->playerCount; ++p) {
        if (!self->playerTimers[p]) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

            if ((1 << p) & self->activePlayers) {
                if (!Player_CheckValidState(player)) {
                    self->activePlayers &= ~(1 << p);
                    if (!self->activePlayers)
                        RSDK.StopSfx(Funnel->sfxFunnel);
                }
                else {
                    if (Player_CheckValidState(player) && player->state == Player_State_Static) {
                        if (player->sidekick == false && ++self->playerScoreTimer[p] >= 30) {
                            self->playerScoreTimer[p] = 0;
                            EntityScoreBonus *bonus   = CREATE_ENTITY(ScoreBonus, NULL, self->position.x, self->position.y - 0x180000);
                            bonus->drawGroup          = Zone->objectDrawGroup[1];
                            bonus->animator.frameID   = 0;
                            Player_GiveScore(player, 100);
                        }

                        self->playerYVel[p] += 64 + (self->playerYVel[p] >> 8);

                        int32 distX = MAX(((self->position.y - player->position.y) >> 8) - 0xA00, 0x400);

                        player->position.x = distX * RSDK.Cos256(self->playerAngle[p]) + self->position.x;
                        self->playerAngle[p] -= self->playerXVel[p] >> 16;

                        if (self->playerXVel[p] <= 0)
                            self->playerXVel[p] = self->playerXVel[p] - self->playerYVel[p] - 128;
                        else
                            self->playerXVel[p] = self->playerXVel[p] + self->playerYVel[p] + 128;

                        if (player->position.y >= self->position.y) {
                            player->scale.x = 0x200;
                            player->scale.y = 0x200;
                        }
                        else {
                            player->drawFX |= FX_SCALE;
                            player->scale.x = ((((player->position.y - self->position.y) >> 16) * RSDK.Sin256(self->playerAngle[p])) >> 7) + 0x200;
                            player->scale.y = player->scale.x;
                        }

                        player->velocity.y += (self->playerYVel[p] >> 5) + 64;
                        if (player->position.y > self->position.y + 0x140000) {
                            self->activePlayers &= ~(1 << p);
                            player->drawFX &= ~FX_SCALE;
                            player->interaction    = true;
                            player->tileCollisions = TILECOLLISION_DOWN;
                            player->scale.x        = 0x200;
                            player->scale.y        = 0x200;
                            player->position.x     = self->position.x;
                            player->velocity.x     = 0;
                            self->playerTimers[p]  = 32;

                            if (player->camera)
                                player->scrollDelay = 1;

                            if (!self->activePlayers)
                                RSDK.StopSfx(Funnel->sfxFunnel);

                            Camera->centerBounds.y = 0x20000;
                            player->state          = Player_State_Air;
                        }
                    }
                    else {
                        self->activePlayers &= ~(1 << p);
                        player->drawFX &= ~FX_SCALE;
                        player->interaction    = true;
                        player->tileCollisions = TILECOLLISION_DOWN;
                        player->scale.x        = 0x200;
                        player->scale.y        = 0x200;
                        player->position.x     = self->position.x;
                        player->velocity.x     = 0;
                        self->playerTimers[p]  = 32;

                        if (player->camera)
                            player->scrollDelay = 1;

                        if (!self->activePlayers)
                            RSDK.StopSfx(Funnel->sfxFunnel);

                        Camera->centerBounds.y = 0x20000;
                        player->state          = Player_State_Air;
                    }
                }
            }
            else {
                if (Player_CheckValidState(player) && Player_CheckCollisionTouch(player, self, &Funnel->hitboxFunnel)) {

                    int32 dy    = 0;
                    int32 distY = (self->position.y - player->position.y) >> 16;
                    if (distY - 10 >= 0)
                        dy = distY - 10;

                    int32 distX = abs(player->position.x - self->position.x) >> 16;
                    if ((distX <= dy && player->position.y < self->position.y) || (player->position.y < self->position.y - 0x280000 && distX <= 64)) {
                        if (player->camera) {
                            player->scrollDelay = 0;
                            Camera_SetupLerp(CAMERA_LERP_SIN1024_2, p, self->position.x, self->position.y - 0x400000, 8);
                        }

                        if (abs(player->velocity.y) > abs(player->velocity.x)) {
                            if (player->position.x >= self->position.x)
                                player->velocity.x -= (player->velocity.y >> 1);
                            else
                                player->velocity.x += (player->velocity.y >> 1);

                            player->velocity.y >>= 2;
                        }

                        self->playerXVel[p] = CLAMP(player->velocity.x, -0x100000, 0x100000);

                        int32 x = ((player->position.x - self->position.x) >> 16) * ((player->position.x - self->position.x) >> 16);
                        int32 y = dy * dy - x;
                        if (player->position.x < self->position.x)
                            x = -x;

                        self->playerAngle[p] = RSDK.ATan2(x, y);

                        player->velocity.y -= abs(9 * (player->velocity.x >> 4));
                        player->velocity.y = CLAMP(player->velocity.y, 0x1000, 0x10000);

#if MANIA_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = 0x80000;
                        else
#endif
                            RSDK.PlaySfx(Funnel->sfxFunnel, false, 0xFF);

                        player->velocity.x        = 0;
                        self->playerScoreTimer[p] = 0;
                        self->playerYVel[p]       = 0;
                        self->activePlayers |= 1 << p;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

                        player->interaction     = false;
                        player->tileCollisions  = TILECOLLISION_NONE;
                        player->state           = Player_State_Static;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                    }
                    else if (player->velocity.y <= 0x10000) {
                        int32 max = 0;
                        if (player->position.y >= self->position.y) {
                            max = 28;
                        }
                        else {
                            max = 0;
                            if (distY >= 0)
                                max = (self->position.y - player->position.y) >> 16;
                            max += 40;
                        }

                        if (distX < max) {
                            int32 x = RSDK.Rand(0x20000, MAX(abs(player->velocity.y + player->velocity.x) >> 8, 0x4000));
                            int32 y = x;
                            if (player->position.y >= self->position.y + 0x160000) {
                                x = player->velocity.x;
                            }
                            else {
                                if (player->position.y < self->position.y && player->velocity.y > 0)
                                    continue;

                                if (player->position.x >= self->position.x) {
                                    if (player->velocity.x > 0 && player->position.y >= self->position.y)
                                        continue;
                                }
                                else {
                                    if (player->velocity.x < 0 && player->position.y >= self->position.y)
                                        continue;
                                    x = -x;
                                }
                            }

                            if (player->position.y < self->position.y || player->position.y > self->position.y + 0x160000) {
                                if (player->velocity.y > 0)
                                    continue;
                            }
                            else {
                                y = player->velocity.y;
                            }

                            if (player->animator.animationID != ANI_FLY) {
                                player->velocity.x = x;
                                player->groundVel  = x;
                            }

                            player->velocity.y   = y;
                            player->onGround     = false;
                            player->applyJumpCap = false;
                            RSDK.PlaySfx(Funnel->sfxPimPom, false, 0xFF);
                        }
                    }
                    else {
                        int32 max = 0;
                        if (player->position.y >= self->position.y) {
                            max = 28;
                        }
                        else {
                            max = 0;
                            if (distY >= 0)
                                max = (self->position.y - player->position.y) >> 16;
                            max += 40;
                        }

                        if (distX < max) {
                            int32 x = RSDK.Rand(0x20000, MAX(abs(player->velocity.y + player->velocity.x) >> 8, 0x4000));
                            int32 y = x;
                            if (player->position.y >= self->position.y + 0x160000) {
                                x = player->velocity.x;
                                x = -x;
                            }
                            else {
                                if (player->position.y < self->position.y && player->velocity.y > 0)
                                    continue;
                                else if (player->position.x < self->position.x) {
                                    if (player->velocity.x < 0 && player->position.y >= self->position.y)
                                        continue;
                                }
                                else if (player->velocity.x > 0 && player->position.y >= self->position.y)
                                    continue;
                                else
                                    x = -x;
                            }

                            if (player->position.y < self->position.y || player->position.y > self->position.y + 0x160000) {
                                if (player->velocity.y > 0)
                                    continue;
                                y = -y;
                            }
                            else {
                                y = player->velocity.y;
                            }

                            if (player->animator.animationID != ANI_FLY) {
                                player->velocity.x = x;
                                player->groundVel  = x;
                            }

                            player->velocity.y   = y;
                            player->onGround     = false;
                            player->applyJumpCap = false;
                            RSDK.PlaySfx(Funnel->sfxPimPom, false, 0xFF);
                        }
                    }
                }
            }
        }
        else {
            self->playerTimers[p]--;
        }
    }
}

void Funnel_LateUpdate(void) {}

void Funnel_StaticUpdate(void)
{
    foreach_all(Funnel, funnel) { RSDK.AddDrawListRef(Zone->objectDrawGroup[0], RSDK.GetEntitySlot(funnel)); }
}

void Funnel_Draw(void)
{
    RSDK_THIS(Funnel);

    self->animator.frameID = SceneInfo->currentDrawGroup == self->drawGroup;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Funnel_Create(void *data)
{
    RSDK_THIS(Funnel);

    RSDK.SetSpriteAnimation(Funnel->aniFrames, 0, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1] + 1;
        self->state         = Funnel_State_None;
    }
}

void Funnel_StageLoad(void)
{
    Funnel->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Funnel.bin", SCOPE_STAGE);

    Funnel->hitboxFunnel.left   = -64;
    Funnel->hitboxFunnel.top    = -48;
    Funnel->hitboxFunnel.right  = 64;
    Funnel->hitboxFunnel.bottom = 0;

    Funnel->active = ACTIVE_ALWAYS;

    Funnel->sfxFunnel = RSDK.GetSfx("SPZ/Funnel.wav");
    Funnel->sfxPimPom = RSDK.GetSfx("Stage/PimPom.wav");
}

void Funnel_State_None(void)
{
    // this is never used actually LOL
}

#if GAME_INCLUDE_EDITOR
void Funnel_EditorDraw(void)
{
    RSDK_THIS(Funnel);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Funnel_EditorLoad(void) { Funnel->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Funnel.bin", SCOPE_STAGE); }
#endif

void Funnel_Serialize(void) {}
