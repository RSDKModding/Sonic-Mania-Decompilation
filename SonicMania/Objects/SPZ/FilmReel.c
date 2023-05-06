// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FilmReel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFilmReel *FilmReel;

void FilmReel_Update(void)
{
    RSDK_THIS(FilmReel);

    self->velocity.x = (self->spinSpeed * RSDK.Cos256(self->angle)) >> 9;
    self->velocity.y = (self->spinSpeed * RSDK.Sin256(self->angle)) >> 9;
    self->lastPos.x += self->velocity.x;
    self->lastPos.y += self->velocity.y;

    int32 prevPathFlags = self->pathFlags;
    self->pathFlags     = 0;
    switch (self->pathDir) {
        case 0:
            if (self->drawPos.x != self->endPos.x) {
                if (self->lastPos.x <= self->drawPos.x) {
                    self->lastPos.x = self->drawPos.x;
                    self->pathFlags = 1;
                }

                if (self->lastPos.x >= self->endPos.x) {
                    self->pathFlags |= 2;
                    self->lastPos.x = self->endPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->lastPos.y <= self->drawPos.y) {
                    self->pathFlags |= 1;
                    self->lastPos.y = self->drawPos.y;
                }

                if (self->lastPos.y >= self->endPos.y) {
                    self->pathFlags |= 2;
                    self->lastPos.y = self->lastPos.y;
                }
            }
            break;

        case 1:
            if (self->drawPos.x != self->endPos.x) {
                if (self->lastPos.x <= self->endPos.x) {
                    self->lastPos.x = self->endPos.x;
                    self->pathFlags = 2;
                }

                if (self->lastPos.x >= self->endPos.x) {
                    self->pathFlags |= 1;
                    self->lastPos.x = self->drawPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->lastPos.y <= self->drawPos.y) {
                    self->pathFlags |= 1;
                    self->lastPos.y = self->drawPos.y;
                }

                if (self->lastPos.y >= self->endPos.y) {
                    self->pathFlags |= 2;
                    self->lastPos.y = self->lastPos.y;
                }
            }
            break;

        case 2:
            if (self->drawPos.x != self->endPos.x) {
                if (self->lastPos.x <= self->endPos.x) {
                    self->lastPos.x = self->endPos.x;
                    self->pathFlags = 2;
                }

                if (self->lastPos.x >= self->drawPos.x) {
                    self->pathFlags |= 1;
                    self->lastPos.x = self->drawPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->lastPos.y <= self->endPos.y) {
                    self->pathFlags |= 2;
                    self->lastPos.y = self->endPos.y;
                }

                if (self->lastPos.y >= self->drawPos.y) {
                    self->pathFlags |= 1;
                    self->lastPos.y = self->drawPos.y;
                }
            }
            break;

        case 3:
            if (self->drawPos.x != self->endPos.x) {
                if (self->lastPos.x <= self->drawPos.x) {
                    self->lastPos.x = self->drawPos.x;
                    self->pathFlags = 1;
                }

                if (self->lastPos.x >= self->endPos.x) {
                    self->pathFlags |= 2;
                    self->lastPos.x = self->endPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->lastPos.y <= self->endPos.y) {
                    self->pathFlags |= 2;
                    self->lastPos.y = self->endPos.y;
                }

                if (self->lastPos.y >= self->drawPos.y) {
                    self->pathFlags |= 1;
                    self->lastPos.y = self->drawPos.y;
                }
            }
            break;

        default: break;
    }

    if (self->pathFlags) {
        if (!prevPathFlags) {
            RSDK.PlaySfx(FilmReel->sfxLanding, false, 255);
            RSDK.StopSfx(FilmReel->sfxUnravel);

            if (self->pathFlags == 1)
                self->active = ACTIVE_BOUNDS;
        }
    }
    else {
        if (!(Zone->timer & 7))
            RSDK.PlaySfx(FilmReel->sfxUnravel, false, 255);

        self->active = ACTIVE_NORMAL;
    }

    self->spinSpeed -= 0x2000;
    self->moveOffset.x = (self->lastPos.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
    self->moveOffset.y = (self->lastPos.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);

    if (!self->spinDirection)
        FilmReel_SpinLeft();
    else
        FilmReel_SpinRight();

    self->spinSpeed = CLAMP(self->spinSpeed, -0x80000, 0x80000);

    self->position = self->lastPos;
    if (!self->pathFlags) {
        if (self->spinDirection)
            self->spinAngle -= self->spinSpeed;
        else
            self->spinAngle += self->spinSpeed;

        self->rotation = (self->spinAngle >> 16) & 0x1FF;
    }

    if (abs(self->pathSize.x) <= abs(self->pathSize.y)) {
        int32 scale   = MIN((abs(self->endPos.y - self->position.y) >> 8) / (abs(self->pathSize.x) >> 16) + 0x100, 0x200);
        self->scale.x = scale;
        self->scale.y = scale;
    }
    else {
        int32 scale   = MIN((abs(self->endPos.x - self->position.x) >> 8) / (abs(self->pathSize.x) >> 16) + 0x100, 0x200);
        self->scale.x = scale;
        self->scale.y = scale;
    }

    self->lineDir = abs(self->position.x - self->drawPos.x) <= abs(self->pathSize.y - self->drawPos.y);
}

void FilmReel_LateUpdate(void) {}

void FilmReel_StaticUpdate(void) {}

void FilmReel_Draw(void)
{
    RSDK_THIS(FilmReel);

    int32 angle = (self->angle - 64) & 0xFF;
    int32 x2    = (16 * self->scale.x * RSDK.Cos256(angle) + self->position.x) & 0xFFFF0000;
    int32 y2    = (16 * self->scale.x * RSDK.Sin256(angle) + self->position.y) & 0xFFFF0000;

    self->celluoidAnimator.frameID = (self->rotation >> 4) & 1;

    color color1 = self->celluoidAnimator.frameID ? 0x204078 : 0x3868A8;
    color color2 = self->celluoidAnimator.frameID ? 0x182850 : 0x204078;

    if (self->lineDir) {
        RSDK.DrawLine(self->drawPos.x, self->drawPos.y, x2, y2, color1, 255, INK_NONE, false);
        RSDK.DrawLine(self->drawPos.x, self->drawPos.y + 0x18000, x2, y2 + 0x18000, color2, 255, INK_NONE, false);
    }
    else {
        RSDK.DrawLine(self->drawPos.x, self->drawPos.y, x2, y2, color1, 255, INK_NONE, false);
        RSDK.DrawLine(self->drawPos.x - 0x18000, self->drawPos.y, x2 - 0x18000, y2, color2, 255, INK_NONE, false);
    }

    RSDK.DrawSprite(&self->pinAnimator, &self->drawPos, false);

    self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->celluoidAnimator, NULL, false);

    self->drawFX = FX_ROTATE;
    RSDK.DrawSprite(&self->reelAnimator, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->edgeAnimator, NULL, false);
}

void FilmReel_Create(void *data)
{
    RSDK_THIS(FilmReel);

    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 0, &self->reelAnimator, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 1, &self->celluoidAnimator, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 2, &self->edgeAnimator, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 3, &self->pinAnimator, true, 0);

    if (!SceneInfo->inEditor) {
        self->lastPos  = self->position;
        self->drawPos  = self->position;
        self->endPos.x = self->drawPos.x + self->pathSize.x;
        self->endPos.y = self->drawPos.y + self->pathSize.y;

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0xC00000;
        self->updateRange.y = 0xC00000;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;

        self->angle     = RSDK.ATan2(self->pathSize.x, self->pathSize.y);
        self->pathDir   = self->angle >> 6;
        self->pathFlags = 1;
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[0];
    }
}

void FilmReel_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmReel.bin", SCOPE_STAGE);
    else
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/FilmReel.bin", SCOPE_STAGE);

    FilmReel->hitboxWheel.left   = -64;
    FilmReel->hitboxWheel.top    = -64;
    FilmReel->hitboxWheel.right  = 64;
    FilmReel->hitboxWheel.bottom = 64;

    FilmReel->offsetPos.x = 0x800000;
    FilmReel->offsetPos.y = 0x800000;

    FilmReel->sfxUnravel = RSDK.GetSfx("Stage/Unravel.wav");
    FilmReel->sfxLanding = RSDK.GetSfx("Stage/Landing.wav");
}

void FilmReel_SpinLeft(void)
{
    RSDK_THIS(FilmReel);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &FilmReel->hitboxWheel)) {
            player->collisionLayers |= Zone->moveLayerMask;
            player->moveLayerPosition.x = FilmReel->offsetPos.x - self->lastPos.x;
            player->moveLayerPosition.y = FilmReel->offsetPos.y - self->lastPos.y;

            if (!player->sidekick) {
                if (player->onGround || RSDK.CheckObjectCollisionTouchCircle(self, 0x400000, player, 0x100000)) {

                    bool32 controllingMovement = true;
                    switch (self->pathFlags) {
                        case 0:
                            if (player->collisionMode || (!player->left && !player->right && player->state != Player_State_Roll)) {
                                controllingMovement = false;
                            }

                            if (abs(player->position.x - self->position.x) < 2 * abs(player->groundVel) && controllingMovement) {
                                if (player->groundVel < -0x60000)
                                    player->groundVel = -0x60000;

                                if (player->direction == FLIP_X) {
                                    player->position.x = self->position.x;
                                    self->spinSpeed -= player->groundVel;
                                }
                            }
                            else {
                                player->groundVel += self->spinSpeed >> 5;
                                if (player->groundVel < -0x60000)
                                    player->groundVel = -0x60000;
                            }
                            break;

                        case 1:
                            if (!player->collisionMode && (player->left || player->right || player->state == Player_State_Roll)
                                && player->direction == FLIP_X) {
                                if (abs(player->position.x - self->position.x) < 2 * abs(player->groundVel)) {
                                    if (player->groundVel < 0)
                                        player->position.x = self->position.x;
                                    self->spinSpeed -= player->groundVel;
                                }
                            }
                            break;

                        case 2:
                            if (abs(player->position.x - self->position.x) >= 2 * abs(player->groundVel) || player->collisionMode
                                || player->position.x <= self->position.x) {
                                if (player->left)
                                    self->spinSpeed -= player->groundVel;
                            }
                            else {
                                if (player->groundVel > 0)
                                    player->position.x = self->position.x;
                                self->spinSpeed -= player->groundVel;
                            }
                            break;
                    }

                    if (player->camera && self->moveCamera) {
                        player->camera->state = StateMachine_None;
                        player->camera->position.x += self->moveOffset.x + ((self->position.x - player->camera->position.x) >> 3);
                        player->camera->position.y += self->moveOffset.y + ((self->position.y - player->camera->position.y - 0x200000) >> 3);
                    }

                    player->position.x += self->moveOffset.x;
                    player->position.y += self->moveOffset.y;
                }
                else if (player->camera && self->moveCamera) {
                    player->camera->state = Camera_State_FollowXY;
                }
            }
        }
    }
}

void FilmReel_SpinRight(void)
{
    RSDK_THIS(FilmReel);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &FilmReel->hitboxWheel)) {
            player->collisionLayers |= Zone->moveLayerMask;
            player->moveLayerPosition.x = FilmReel->offsetPos.x - self->lastPos.x;
            player->moveLayerPosition.y = FilmReel->offsetPos.y - self->lastPos.y;
            if (!player->sidekick) {
                if (player->onGround || RSDK.CheckObjectCollisionTouchCircle(self, 0x400000, player, 0x100000)) {

                    bool32 controllingMovement = true;
                    switch (self->pathFlags) {
                        case 0:
                            if (player->collisionMode || (!player->left && !player->right && player->state != Player_State_Roll)) {
                                controllingMovement = false;
                            }

                            if (abs(player->position.x - self->position.x) < 2 * abs(player->groundVel) && controllingMovement) {
                                if (player->groundVel > 0x60000)
                                    player->groundVel = 0x60000;

                                if (player->direction == FLIP_NONE) {
                                    player->position.x = self->position.x;
                                    self->spinSpeed += player->groundVel;
                                }
                            }
                            else {
                                player->groundVel -= self->spinSpeed >> 5;
                                if (player->groundVel > 0x60000)
                                    player->groundVel = 0x60000;
                            }
                            break;

                        case 1:
                            if (player->collisionMode || (!player->left && !player->right && player->state != Player_State_Roll)
                                || player->direction) {
                                controllingMovement = false;
                            }

                            if (controllingMovement) {
                                if (abs(player->position.x - self->position.x) < 2 * abs(player->groundVel) && player->groundVel > 0) {
                                    player->position.x = self->position.x;
                                }
                                self->spinSpeed += player->groundVel;
                            }
                            break;

                        case 2:
                            if (abs(player->position.x - self->position.x) >= 2 * abs(player->groundVel) || player->collisionMode
                                || player->position.x >= self->position.x) {
                                if (player->right)
                                    self->spinSpeed += player->groundVel;
                            }
                            else {
                                if (player->groundVel < 0)
                                    player->position.x = self->position.x;
                                self->spinSpeed += player->groundVel;
                            }
                            break;
                    }

                    if (player->camera && self->moveCamera) {
                        player->camera->state = StateMachine_None;
                        player->camera->position.x += self->moveOffset.x + ((self->position.x - player->camera->position.x) >> 3);
                        player->camera->position.y += self->moveOffset.y + ((self->position.y - player->camera->position.y - 0x200000) >> 3);
                    }
                }
                else if (player->camera && self->moveCamera) {
                    player->camera->state = Camera_State_FollowXY;
                }
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void FilmReel_EditorDraw(void)
{
    RSDK_THIS(FilmReel);

    self->lastPos       = self->position;
    self->drawPos       = self->position;
    self->endPos.x      = self->position.x + self->pathSize.x;
    self->endPos.y      = self->position.y + self->pathSize.y;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0xC00000;
    self->updateRange.y = 0xC00000;
    self->scale.x       = 0x200;
    self->scale.y       = 0x200;

    self->angle     = RSDK.ATan2(self->pathSize.x, self->pathSize.y);
    self->pathDir   = self->angle >> 6;
    self->pathFlags = 1;
    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    FilmReel_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->inkEffect = INK_BLEND;
        self->drawPos   = self->endPos;
        FilmReel_Draw();

        self->drawFX = FX_SCALE;
        RSDK.DrawSprite(&self->celluoidAnimator, &self->drawPos, false);

        self->drawFX = FX_ROTATE;
        RSDK.DrawSprite(&self->reelAnimator, &self->drawPos, false);

        self->drawFX = FX_NONE;
        RSDK.DrawSprite(&self->edgeAnimator, &self->drawPos, false);

        self->inkEffect = INK_NONE;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void FilmReel_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmReel.bin", SCOPE_STAGE);
    else
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/FilmReel.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FilmReel, spinDirection);
    RSDK_ENUM_VAR("Clockwise", FLIP_NONE);
    RSDK_ENUM_VAR("Counter-Clockwise", FLIP_X);
}
#endif

void FilmReel_Serialize(void)
{
    RSDK_EDITABLE_VAR(FilmReel, VAR_VECTOR2, pathSize);
    RSDK_EDITABLE_VAR(FilmReel, VAR_UINT8, spinDirection);
}
