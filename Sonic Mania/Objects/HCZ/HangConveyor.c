// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HangConveyor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHangConveyor *HangConveyor;

void HangConveyor_Update(void)
{
    RSDK_THIS(HangConveyor);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        HangConveyor_Unknown2();
        self->active = ACTIVE_NORMAL;
    }
    else {
        self->field_A4[0].x = 0;
        self->field_A4[0].y = 0;
        self->field_A4[1].x = 0;
        self->field_A4[1].y = 0;
        self->field_A4[2].x = 0;
        self->field_A4[2].y = 0;
        self->field_A4[3].x = 0;
        self->field_A4[3].y = 0;
        self->active        = ACTIVE_BOUNDS;
    }
}

void HangConveyor_LateUpdate(void) {}

void HangConveyor_StaticUpdate(void) {}

void HangConveyor_Draw(void) { HangConveyor_DrawSprites(); }

void HangConveyor_Create(void *data)
{
    RSDK_THIS(HangConveyor);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    HangConveyor_Unknown1();
    self->updateRange.x += (self->hitbox1.right - self->hitbox1.left) << 16;
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 2, &self->animator3, true, 0);
}

void HangConveyor_StageLoad(void) { HangConveyor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HangConveyor.bin", SCOPE_STAGE); }

void HangConveyor_DrawSprites(void)
{
    RSDK_THIS(HangConveyor);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += -0x80000 * (self->length != 1 ? self->length - 1 : 0);
    int32 dirStore = self->direction;

    for (int32 i = 0; i < self->length; ++i) {
        self->direction = dirStore != FLIP_NONE;
        drawPos.y         = self->position.y - 0x150000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->direction = dirStore == FLIP_NONE;
        drawPos.y         = self->position.y + 0x150000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x += 0x100000;
    }

    self->direction = dirStore != FLIP_NONE;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y;

    if (dirStore) {
        int32 len = self->length + 3;
        if (self->length == 1)
            len = 4;
        drawPos.x += len << 19;
    }
    else {
        int32 len = self->length + 3;
        if (self->length == 1)
            len = 4;
        drawPos.x += -0x80000 * len;
    }
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->direction = dirStore == FLIP_NONE;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y;

    if (dirStore) {
        int32 len = self->length + 3;
        if (self->length == 1)
            len = 4;
        drawPos.x += -0x80000 * len;
    }
    else {
        int32 len = self->length + 3;
        if (self->length == 1)
            len = 4;
        drawPos.x += len << 19;
    }
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->direction = dirStore;
}

void HangConveyor_Unknown1(void)
{
    RSDK_THIS(HangConveyor);

    self->hitbox3.left   = -8 * (self->length != -6 ? self->length + 6 : 0);
    self->hitbox3.top    = -24;
    self->hitbox3.right  = 8 * (self->length != -6 ? self->length + 6 : 0);
    self->hitbox3.bottom = 24;

    self->hitbox1.left   = -8 * (self->length != -2 ? self->length + 2 : 0);
    self->hitbox1.top    = -26;
    self->hitbox1.right  = 8 * (self->length != -2 ? self->length + 2 : 0);
    self->hitbox1.bottom = -16;

    self->field_64[0] = self->position;
    self->field_64[0].y -= 0x150000;
    self->field_64[0].x += self->hitbox1.left << 16;

    self->field_64[1] = self->position;
    self->field_64[1].y -= 0x150000;
    self->field_64[1].x += self->hitbox1.right << 16;

    self->hitbox2.left   = -4 - 8 * (self->length != -2 ? self->length + 2 : 0);
    self->hitbox2.top    = 16;
    self->hitbox2.right  = 8 * (self->length != -2 ? self->length + 2 : 0) + 4;
    self->hitbox2.bottom = 26;

    self->field_64[2] = self->position;
    self->field_64[2].x += self->hitbox2.left << 16;
    self->field_64[2].y += 0x150000;

    self->field_64[3] = self->position;
    self->field_64[3].y += 0x150000;
    self->field_64[3].x += self->hitbox2.right << 16;
}

void HangConveyor_Unknown2(void)
{
    RSDK_THIS(HangConveyor);

    foreach_active(Player, player)
    {
        if (Player_CheckValidState(player)) {
            int32 playerID = RSDK.GetEntityID(player);
            int32 posY     = player->position.y;
            if (player->animator.animationID != ANI_POLESWINGH && player->animator.animationID != ANI_SHIMMYMOVE)
                posY -= 0x180000;

            int32 prevX                    = self->field_A4[playerID].x;
            int32 prevY                    = self->field_A4[playerID].y;
            self->field_A4[playerID].x = player->position.x;
            self->field_A4[playerID].y = posY;

            bool32 collided1 = true;
            if (abs(player->position.x - prevX) < 0x800000 && abs(posY - prevY) < 0x800000 && (prevX || prevY)) {
                bool32 cFlag = MathHelpers_Unknown12(player->position.x, posY, prevX, prevY, self->field_64[0].x, self->field_64[0].y,
                                                     self->field_64[1].x, self->field_64[1].y);
                if (self->field_84[playerID] || !cFlag) {
                    collided1 = false;
                }

                bool32 collided2 = !self->field_94[playerID]
                                   && MathHelpers_Unknown12(player->position.x, posY, prevX, prevY, self->field_64[2].x, self->field_64[2].y,
                                                            self->field_64[3].x, self->field_64[3].y);

                if (self->field_84[playerID] > 0)
                    self->field_84[playerID]--;
                if (self->field_94[playerID] > 0)
                    self->field_94[playerID]--;

                if (!((1 << playerID) & self->activePlayers1)) {
                    if (!(self->activePlayers4 & (1 << playerID))) {
                        if ((collided1 || collided2) && player->state != Player_State_None && player->animator.animationID != ANI_HURT) {
                            self->activePlayers1 |= (1 << playerID);
                            self->activePlayers4 |= (1 << playerID);
                            if (collided1) {
                                self->activePlayers2 |= 1 << playerID;
                                player->position.y = self->position.y - 0x150000;
                            }
                            if (collided2) {
                                self->activePlayers3 |= 1 << playerID;
                                player->position.y = self->position.y + 0x150000;
                            }
                            RSDK.PlaySfx(Player->sfxGrab, false, 255);
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->animator, true, 0);
                            player->animator.speed = 0;
                            player->nextGroundState               = StateMachine_None;
                            player->nextAirState                  = StateMachine_None;
                            player->velocity.x                    = 0;
                            player->velocity.y                    = 0;
                            player->rotation                      = 0;
                            player->state                         = Player_State_None;
                        }
                    }
                }

                int32 posY = player->position.y;
                if (player->animator.animationID != ANI_POLESWINGH && player->animator.animationID != ANI_SHIMMYMOVE) {
                    posY -= 0x180000;
                }

                collided1 =
                    MathHelpers_PointInHitbox(self->direction, self->position.x, self->position.y, &self->hitbox1, player->position.x, posY);
                collided2 =
                    MathHelpers_PointInHitbox(self->direction, self->position.x, self->position.y, &self->hitbox2, player->position.x, posY);

                bool32 passFlag = false;
                if (((1 << playerID) & self->activePlayers1)) {
                    if (self->field_C4[playerID] > 0) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->animator, true, self->field_C4[playerID] >> 1);
                        player->animator.speed = 0;
                    }
                    else if (player->left) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SHIMMYMOVE, &player->animator, false, 0);
                        player->direction = FLIP_X;
                    }
                    else if (player->right) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SHIMMYMOVE, &player->animator, false, 0);
                        player->direction = FLIP_NONE;
                    }
                    else {
                        if (!(player->animator.animationID == ANI_POLESWINGH && !player->animator.frameID)) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLESWINGH, &player->animator, false, 0);
                            player->animator.speed = 0;
                        }
                    }

                    if (((1 << playerID) & self->activePlayers2)) {
                        player->position.y = self->position.y - 0x150000;
                        if (self->direction) {
                            if (player->right) {
                                player->position.x += 0x20000;
                            }
                            else if (player->left) {
                                player->position.x += 0xB000;
                            }
                            else {
                                player->position.x += 0x15555;
                            }
                        }
                        else if (player->left) {
                            player->position.x -= 0x20000;
                        }
                        else if (player->right) {
                            player->position.x -= 0xB000;
                        }
                        else {
                            player->position.x -= 0x15555;
                        }
                    }

                    if (((1 << playerID) & self->activePlayers3)) {
                        player->position.y = self->position.y + 0x150000;
                        if (self->direction) {
                            if (player->left) {
                                player->position.x -= 0x20000;
                            }
                            else if (player->right) {
                                player->position.x -= 0xB000;
                            }
                            else {
                                player->position.x -= 0x15555;
                            }
                        }
                        else if (player->right) {
                            player->position.x += 0x20000;
                        }
                        else if (player->left) {
                            player->position.x += 0xB000;
                        }
                        else {
                            player->position.x += 0x15555;
                        }
                    }

                    bool32 flag = true;
                    if (!((1 << playerID) & self->activePlayers2) || collided1)
                        flag = false;

                    int32 anim = player->animator.animationID;
                    if (player->jumpPress || (anim != ANI_POLESWINGH && anim != ANI_SHIMMYMOVE) || player->velocity.x || player->velocity.y || flag
                        || (((1 << playerID) & self->activePlayers3) && !collided2)) {
                        self->activePlayers1 &= ~(1 << playerID);
                        player->position.y += 0x180000;
                        if (!player->jumpPress || player->down) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                            player->state = Player_State_Air;
                        }
                        else {
                            Player_StartJump(player);
                        }
                        if (((1 << playerID) & self->activePlayers2))
                            self->field_84[playerID] = 60;
                        if (((1 << playerID) & self->activePlayers3))
                            self->field_94[playerID] = 60;
                    }
                    if (((1 << playerID) & self->activePlayers1))
                        passFlag = true;
                }

                if (((1 << playerID) & self->activePlayers4) && !passFlag) {
                    bool32 flag = false;
                    if (((1 << playerID) & self->activePlayers2) && !Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
                        self->activePlayers2 &= ~(1 << playerID);
                        flag = true;
                    }

                    if (!((1 << playerID) & self->activePlayers3) || Player_CheckCollisionTouch(player, self, &self->hitbox2)) {
                        if (flag)
                            self->activePlayers4 &= ~(1 << playerID);
                    }
                    else {
                        self->activePlayers3 &= ~(1 << playerID);
                        self->activePlayers4 &= ~(1 << playerID);
                    }
                }

                if (!((1 << playerID) & self->activePlayers1))
                    self->field_C4[playerID] = 0;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void HangConveyor_EditorDraw(void) { HangConveyor_DrawSprites(); }

void HangConveyor_EditorLoad(void) { HangConveyor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HangConveyor.bin", SCOPE_STAGE); }
#endif

void HangConveyor_Serialize(void)
{
    RSDK_EDITABLE_VAR(HangConveyor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(HangConveyor, VAR_UINT32, length);
}
