// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HangConveyor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHangConveyor *HangConveyor;

void HangConveyor_Update(void)
{
    RSDK_THIS(HangConveyor);

    RSDK.ProcessAnimation(&self->endAnimator);
    RSDK.ProcessAnimation(&self->startAnimator);
    RSDK.ProcessAnimation(&self->middleAnimator);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        HangConveyor_HandlePlayerInteractions();
        self->active = ACTIVE_NORMAL;
    }
    else {
        for (int32 p = 0; p < PLAYER_COUNT; ++p) {
            self->playerPositions[p].x = 0;
            self->playerPositions[p].y = 0;
        }

        self->active = ACTIVE_BOUNDS;
    }
}

void HangConveyor_LateUpdate(void) {}

void HangConveyor_StaticUpdate(void) {}

void HangConveyor_Draw(void) { HangConveyor_DrawSprites(); }

void HangConveyor_Create(void *data)
{
    RSDK_THIS(HangConveyor);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    HangConveyor_SetupHitboxes();
    self->updateRange.x += (self->hitboxFallCheckTop.right - self->hitboxFallCheckTop.left) << 16;

    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 0, &self->endAnimator, true, 0);
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 1, &self->startAnimator, true, 0);
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 2, &self->middleAnimator, true, 0);
}

void HangConveyor_StageLoad(void) { HangConveyor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HangConveyor.bin", SCOPE_STAGE); }

void HangConveyor_DrawSprites(void)
{
    RSDK_THIS(HangConveyor);

    Vector2 drawPos = self->position;
    drawPos.x += -0x80000 * (self->length != 1 ? self->length - 1 : 0);
    int32 dirStore = self->direction;

    for (int32 i = 0; i < self->length; ++i) {
        self->direction = dirStore != FLIP_NONE;
        drawPos.y       = self->position.y - 0x150000;
        RSDK.DrawSprite(&self->middleAnimator, &drawPos, false);

        self->direction = dirStore == FLIP_NONE;
        drawPos.y       = self->position.y + 0x150000;
        RSDK.DrawSprite(&self->middleAnimator, &drawPos, false);

        drawPos.x += 0x100000;
    }

    self->direction = dirStore != FLIP_NONE;
    drawPos.x       = self->position.x;
    drawPos.y       = self->position.y;

    int32 len = self->length == 1 ? 4 : (self->length + 3);
    if (dirStore) {
        drawPos.x += len << 19;
    }
    else {
        drawPos.x += -0x80000 * len;
    }
    RSDK.DrawSprite(&self->endAnimator, &drawPos, false);

    self->direction = dirStore == FLIP_NONE;
    drawPos.x       = self->position.x;
    drawPos.y       = self->position.y;

    if (dirStore) {
        drawPos.x += -0x80000 * len;
    }
    else {
        drawPos.x += len << 19;
    }
    RSDK.DrawSprite(&self->startAnimator, &drawPos, false);

    self->direction = dirStore;
}

void HangConveyor_SetupHitboxes(void)
{
    RSDK_THIS(HangConveyor);

    self->hitboxSpikeBallRange.left   = -8 * (self->length != -6 ? self->length + 6 : 0);
    self->hitboxSpikeBallRange.top    = -24;
    self->hitboxSpikeBallRange.right  = 8 * (self->length != -6 ? self->length + 6 : 0);
    self->hitboxSpikeBallRange.bottom = 24;

    self->hitboxFallCheckTop.left   = -8 * (self->length != -2 ? self->length + 2 : 0);
    self->hitboxFallCheckTop.top    = -26;
    self->hitboxFallCheckTop.right  = 8 * (self->length != -2 ? self->length + 2 : 0);
    self->hitboxFallCheckTop.bottom = -16;

    // Top Left End
    self->endPosTopLeft.x = self->position.x + (self->hitboxFallCheckTop.left << 16);
    self->endPosTopLeft.y = self->position.y - 0x150000;

    // Top Right End
    self->endPosTopRight.x = self->position.x + (self->hitboxFallCheckTop.right << 16);
    self->endPosTopRight.y = self->position.y - 0x150000;

    self->hitboxFallCheckBottom.left   = -4 - 8 * (self->length != -2 ? self->length + 2 : 0);
    self->hitboxFallCheckBottom.top    = 16;
    self->hitboxFallCheckBottom.right  = 8 * (self->length != -2 ? self->length + 2 : 0) + 4;
    self->hitboxFallCheckBottom.bottom = 26;

    // Bottom Left End
    self->endPosBottomLeft.x = self->position.x + (self->hitboxFallCheckBottom.left << 16);
    self->endPosBottomLeft.y = self->position.y + 0x150000;

    // Bottom Right End
    self->endPosBottomRight.x = self->position.x + (self->hitboxFallCheckBottom.right << 16);
    self->endPosBottomRight.y = self->position.y + 0x150000;
}

void HangConveyor_HandlePlayerInteractions(void)
{
    RSDK_THIS(HangConveyor);

    foreach_active(Player, player)
    {
        if (Player_CheckValidState(player)) {
            int32 playerID = RSDK.GetEntitySlot(player);

            int32 playerY = player->position.y;
            if (player->animator.animationID != ANI_POLE_SWING_H && player->animator.animationID != ANI_SHIMMY_MOVE)
                playerY -= 0x180000;

            int32 prevX                       = self->playerPositions[playerID].x;
            int32 prevY                       = self->playerPositions[playerID].y;
            self->playerPositions[playerID].x = player->position.x;
            self->playerPositions[playerID].y = playerY;

            if (abs(player->position.x - prevX) < 0x800000 && abs(playerY - prevY) < 0x800000 && (prevX || prevY)) {
                bool32 collidedTop = MathHelpers_CheckPositionOverlap(player->position.x, playerY, prevX, prevY, self->endPosTopLeft.x,
                                                                      self->endPosTopLeft.y, self->endPosTopRight.x, self->endPosTopRight.y);
                collidedTop        = collidedTop && !self->grabDelayTop[playerID];

                bool32 collidedBottom =
                    MathHelpers_CheckPositionOverlap(player->position.x, playerY, prevX, prevY, self->endPosBottomLeft.x, self->endPosBottomLeft.y,
                                                     self->endPosBottomRight.x, self->endPosBottomRight.y);
                collidedBottom = collidedBottom && !self->grabDelayBottom[playerID];

                if (self->grabDelayTop[playerID] > 0)
                    self->grabDelayTop[playerID]--;

                if (self->grabDelayBottom[playerID] > 0)
                    self->grabDelayBottom[playerID]--;

                if (!((1 << playerID) & self->movementActivePlayers) && !(self->checkableActivePlayers & (1 << playerID))) {
                    if ((collidedTop || collidedBottom) && player->state != Player_State_Static && player->animator.animationID != ANI_HURT) {
                        self->movementActivePlayers |= (1 << playerID);
                        self->checkableActivePlayers |= (1 << playerID);
                        if (collidedTop) {
                            self->activePlayersTop |= 1 << playerID;
                            player->position.y = self->position.y - 0x150000;
                        }
                        if (collidedBottom) {
                            self->activePlayersBottom |= 1 << playerID;
                            player->position.y = self->position.y + 0x150000;
                        }

                        RSDK.PlaySfx(Player->sfxGrab, false, 255);
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLE_SWING_H, &player->animator, true, 0);
                        player->animator.speed  = 0;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->velocity.x      = 0;
                        player->velocity.y      = 0;
                        player->rotation        = 0;
                        player->state           = Player_State_Static;
                    }
                }

                int32 newPlayerY = player->position.y;
                if (player->animator.animationID != ANI_POLE_SWING_H && player->animator.animationID != ANI_SHIMMY_MOVE)
                    newPlayerY -= 0x180000;

                collidedTop    = MathHelpers_PointInHitbox(self->position.x, self->position.y, player->position.x, newPlayerY, self->direction,
                                                        &self->hitboxFallCheckTop);
                collidedBottom = MathHelpers_PointInHitbox(self->position.x, self->position.y, player->position.x, newPlayerY, self->direction,
                                                           &self->hitboxFallCheckBottom);

                if (((1 << playerID) & self->movementActivePlayers)) {
                    if (self->fanTimer[playerID] > 0) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLE_SWING_H, &player->animator, true, self->fanTimer[playerID] >> 1);
                        player->animator.speed = 0;
                    }
                    else if (player->left) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SHIMMY_MOVE, &player->animator, false, 0);
                        player->direction = FLIP_X;
                    }
                    else if (player->right) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SHIMMY_MOVE, &player->animator, false, 0);
                        player->direction = FLIP_NONE;
                    }
                    else if (!(player->animator.animationID == ANI_POLE_SWING_H && !player->animator.frameID)) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_POLE_SWING_H, &player->animator, false, 0);
                        player->animator.speed = 0;
                    }

                    if ((1 << playerID) & self->activePlayersTop) {
                        player->position.y = self->position.y - 0x150000;

                        int32 moveVelocity = 0;
                        if (self->direction) {
                            moveVelocity = 0x15555;

                            if (player->right)
                                moveVelocity = 0x20000;
                            else if (player->left)
                                moveVelocity = 0xB000;
                        }
                        else {
                            moveVelocity = -0x15555;

                            if (player->left)
                                moveVelocity = -0x20000;
                            if (player->right)
                                moveVelocity = -0xB000;
                        }

                        player->position.x += moveVelocity;
                    }

                    if ((1 << playerID) & self->activePlayersBottom) {
                        player->position.y = self->position.y + 0x150000;

                        int32 moveVelocity = 0;
                        if (self->direction) {
                            moveVelocity = -0x15555;

                            if (player->left)
                                moveVelocity = -0x20000;
                            else if (player->right)
                                moveVelocity = -0xB000;
                        }
                        else {
                            moveVelocity = 0x15555;

                            if (player->right)
                                moveVelocity = 0x20000;
                            if (player->left)
                                moveVelocity = 0xB000;
                        }

                        player->position.x += moveVelocity;
                    }

                    bool32 noCollision = false;
                    if (((1 << playerID) & self->activePlayersTop) && !collidedTop)
                        noCollision = true;
                    if (((1 << playerID) & self->activePlayersBottom) && !collidedBottom)
                        noCollision = true;

                    int32 anim = player->animator.animationID;
                    if (player->jumpPress || (anim != ANI_POLE_SWING_H && anim != ANI_SHIMMY_MOVE) || player->velocity.x || player->velocity.y
                        || noCollision) {

                        self->movementActivePlayers &= ~(1 << playerID);
                        player->position.y += 0x180000;

                        if (!player->jumpPress || player->down) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                            player->state = Player_State_Air;
                        }
                        else {
                            Player_Action_Jump(player);
                        }

                        if ((1 << playerID) & self->activePlayersTop)
                            self->grabDelayTop[playerID] = 60;

                        if ((1 << playerID) & self->activePlayersBottom)
                            self->grabDelayBottom[playerID] = 60;
                    }
                }

                if (!((1 << playerID) & self->movementActivePlayers) && ((1 << playerID) & self->checkableActivePlayers)) {
                    if (((1 << playerID) & self->activePlayersTop) && !Player_CheckCollisionTouch(player, self, &self->hitboxFallCheckTop)) {
                        self->activePlayersTop &= ~(1 << playerID);
                        self->checkableActivePlayers &= ~(1 << playerID);
                    }

                    if (((1 << playerID) & self->activePlayersBottom) && !Player_CheckCollisionTouch(player, self, &self->hitboxFallCheckBottom)) {
                        self->activePlayersBottom &= ~(1 << playerID);
                        self->checkableActivePlayers &= ~(1 << playerID);
                    }
                }

                if (!((1 << playerID) & self->movementActivePlayers))
                    self->fanTimer[playerID] = 0;
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void HangConveyor_EditorDraw(void) { HangConveyor_DrawSprites(); }

void HangConveyor_EditorLoad(void)
{
    HangConveyor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HangConveyor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HangConveyor, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void HangConveyor_Serialize(void)
{
    RSDK_EDITABLE_VAR(HangConveyor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(HangConveyor, VAR_UINT32, length);
}
