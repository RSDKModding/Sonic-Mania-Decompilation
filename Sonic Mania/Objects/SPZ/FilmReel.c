#include "SonicMania.h"

ObjectFilmReel *FilmReel;

void FilmReel_Update(void)
{
    RSDK_THIS(FilmReel);

    self->velocity.x = (self->field_8C * RSDK.Cos256(self->angle)) >> 9;
    self->velocity.y = (self->field_8C * RSDK.Sin256(self->angle)) >> 9;
    self->field_74.x += self->velocity.x;
    self->field_74.y += self->velocity.y;
    self->field_90 = 0;

    switch (self->field_94) {
        case 0:
            if (self->drawPos.x != self->endPos.x) {
                if (self->field_74.x <= self->drawPos.x) {
                    self->field_74.x = self->drawPos.x;
                    self->field_90   = 1;
                }
                if (self->field_74.x >= self->endPos.x) {
                    self->field_90 |= 2;
                    self->field_74.x = self->endPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->field_74.y <= self->drawPos.y) {
                    self->field_90 |= 1;
                    self->field_74.y = self->drawPos.y;
                }
                if (self->field_74.y >= self->endPos.y) {
                    self->field_90 |= 2;
                    self->field_74.y = self->field_74.y;
                }
            }
            break;
        case 1:
            if (self->endPos.x != self->endPos.x) {
                if (self->field_74.x <= self->endPos.x) {
                    self->field_74.x = self->endPos.x;
                    self->field_90   = 2;
                }
                if (self->field_74.x >= self->endPos.x) {
                    self->field_90 |= 1;
                    self->field_74.x = self->drawPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->field_74.y <= self->drawPos.y) {
                    self->field_90 |= 1;
                    self->field_74.y = self->drawPos.y;
                }
                if (self->field_74.y >= self->endPos.y) {
                    self->field_90 |= 2;
                    self->field_74.y = self->field_74.y;
                }
            }
            break;
        case 2:
            if (self->drawPos.x != self->endPos.x) {
                if (self->field_74.x <= self->endPos.x) {
                    self->field_74.x = self->endPos.x;
                    self->field_90   = 2;
                }
                if (self->field_74.x >= self->drawPos.x) {
                    self->field_90 |= 1;
                    self->field_74.x = self->drawPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->field_74.y <= self->endPos.y) {
                    self->field_90 |= 2;
                    self->field_74.y = self->endPos.y;
                }
                if (self->field_74.y >= self->drawPos.y) {
                    self->field_90 |= 1;
                    self->field_74.y = self->drawPos.y;
                }
            }
            break;
        case 3:
            if (self->drawPos.x != self->endPos.x) {
                if (self->field_74.x <= self->drawPos.x) {
                    self->field_74.x = self->drawPos.x;
                    self->field_90   = 1;
                }
                if (self->field_74.x >= self->endPos.x) {
                    self->field_90 |= 2;
                    self->field_74.x = self->endPos.x;
                }
            }

            if (self->drawPos.y != self->endPos.y) {
                if (self->field_74.y <= self->endPos.y) {
                    self->field_90 |= 2;
                    self->field_74.y = self->endPos.y;
                }
                if (self->field_74.y >= self->drawPos.y) {
                    self->field_90 |= 1;
                    self->field_74.y = self->drawPos.y;
                }
            }
            break;
        default: break;
    }

    if (self->field_90) {
        if (!self->field_90) {
            RSDK.PlaySfx(FilmReel->sfxLanding, false, 255);
            RSDK.StopSfx(FilmReel->sfxUnravel);
            if (self->field_90 == 1)
                self->active = ACTIVE_BOUNDS;
        }
    }
    else {
        if (!(Zone->timer & 7))
            RSDK.PlaySfx(FilmReel->sfxUnravel, false, 255);
        self->active = ACTIVE_NORMAL;
    }

    self->field_8C -= 0x2000;
    self->field_7C.x = (self->field_74.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
    self->field_7C.y = (self->field_74.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);
    if (!self->spinDirection)
        FilmReel_SpinLeft();
    else
        FilmReel_SpinRight();
    self->field_8C = clampVal(self->field_8C, -0x80000, 0x80000);

    self->position = self->field_74;
    if (!self->field_90) {
        if (self->spinDirection)
            self->field_84 -= self->field_8C;
        else
            self->field_84 += self->field_8C;
        self->rotation = (self->field_84 >> 16) & 0x1FF;
    }

    if (abs(self->pathSize.x) <= abs(self->pathSize.y)) {
        int scale = (abs(self->endPos.y - self->position.y) >> 8) / (abs(self->pathSize.x) >> 16) + 256;
        if (scale > 512)
            scale = 512;
        self->scale.x = scale;
        self->scale.y = scale;
    }
    else {
        int scale = (abs(self->endPos.x - self->position.x) >> 8) / (abs(self->pathSize.x) >> 16) + 256;
        if (scale > 512)
            scale = 512;
        self->scale.x = scale;
        self->scale.y = scale;
    }
    self->field_98 = abs(self->position.x - self->drawPos.x) <= abs(self->pathSize.y - self->drawPos.y);
}

void FilmReel_LateUpdate(void) {}

void FilmReel_StaticUpdate(void) {}

void FilmReel_Draw(void)
{
    RSDK_THIS(FilmReel);

    int angle = (self->angle - 64) & 0xFF;
    int x2    = (16 * self->scale.x * RSDK.Cos256(angle) + self->position.x) & 0xFFFF0000;
    int y2    = (16 * self->scale.x * RSDK.Sin256(angle) + self->position.y) & 0xFFFF0000;

    self->animator2.frameID = (self->rotation >> 4) & 1;
    colour colour1            = 0x3868A8;
    if (self->animator2.frameID)
        colour1 = 0x204078;
    colour colour2 = 0x204078;
    if (self->animator2.frameID)
        colour2 = 0x182850;

    if (self->field_98) {
        RSDK.DrawLine(self->drawPos.x, self->drawPos.y, x2, y2, colour1, 255, INK_NONE, false);
        RSDK.DrawLine(self->drawPos.x, self->drawPos.y + 0x18000, x2, y2 + 0x18000, colour2, 255, INK_NONE, false);
    }
    else {
        RSDK.DrawLine(self->drawPos.x, self->drawPos.y, x2, y2, colour1, 255, INK_NONE, false);
        RSDK.DrawLine(self->drawPos.x - 0x18000, self->drawPos.y, x2 - 0x18000, y2, colour2, 255, INK_NONE, false);
    }
    RSDK.DrawSprite(&self->animator4, &self->drawPos, false);

    self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->drawFX = FX_ROTATE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void FilmReel_Create(void *data)
{
    RSDK_THIS(FilmReel);

    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 2, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 3, &self->animator4, true, 0);
    if (!SceneInfo->inEditor) {
        self->field_74      = self->position;
        self->drawPos       = self->position;
        self->endPos.x      = self->pathSize.x + self->drawPos.x;
        self->endPos.y      = self->pathSize.y + self->drawPos.y;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0xC00000;
        self->updateRange.y = 0xC00000;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;

        self->angle     = RSDK.ATan2(self->pathSize.x, self->pathSize.y);
        self->field_94  = self->angle >> 6;
        self->field_90  = 1;
        self->visible   = true;
        self->drawOrder = Zone->drawOrderLow;
    }
}

void FilmReel_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmReel.bin", SCOPE_STAGE);
    else
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/FilmReel.bin", SCOPE_STAGE);
    FilmReel->hitbox.left   = -64;
    FilmReel->hitbox.top    = -64;
    FilmReel->hitbox.right  = 64;
    FilmReel->hitbox.bottom = 64;
    FilmReel->offsetPos.x   = 0x800000;
    FilmReel->offsetPos.y   = 0x800000;
    FilmReel->sfxUnravel    = RSDK.GetSfx("Stage/Unravel.wav");
    FilmReel->sfxLanding    = RSDK.GetSfx("Stage/Landing.wav");
}

void FilmReel_SpinLeft(void)
{
    RSDK_THIS(FilmReel);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &FilmReel->hitbox)) {
            player->collisionLayers |= Zone->moveID;
            player->moveOffset.x = FilmReel->offsetPos.x - self->field_74.x;
            player->moveOffset.y = FilmReel->offsetPos.y - self->field_74.y;
            if (!player->sidekick) {
                if (player->onGround || RSDK.CheckObjectCollisionTouchCircle(self, 0x400000, player, 0x100000)) {

                    bool32 flag = true;
                    switch (self->field_90) {
                        case 0:
                            if (player->collisionMode || !player->left && !player->right && player->state != Player_State_Roll) {
                                flag = false;
                            }

                            if (abs(player->position.x - self->position.x) < 2 * abs(player->groundVel) && flag) {
                                if (player->groundVel < -0x60000)
                                    player->groundVel = -0x60000;
                                if (player->direction == FLIP_X) {
                                    player->position.x = self->position.x;
                                    self->field_8C -= player->groundVel;
                                }
                            }
                            else {
                                player->groundVel += self->field_8C >> 5;
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
                                    self->field_8C -= player->groundVel;
                                }
                            }
                            break;
                        case 2:
                            if (abs(player->position.x - self->position.x) >= 2 * abs(player->groundVel) || player->collisionMode
                                || player->position.x <= self->position.x) {
                                if (player->left)
                                    self->field_8C -= player->groundVel;
                            }
                            else {
                                if (player->groundVel > 0)
                                    player->position.x = self->position.x;
                                self->field_8C -= player->groundVel;
                            }
                            break;
                    }

                    if (player->camera && self->flag) {
                        player->camera->state = 0;
                        player->camera->position.x += self->field_7C.x + ((self->position.x - player->camera->position.x) >> 3);
                        player->camera->position.y += self->field_7C.y + ((self->position.y - player->camera->position.y - 0x200000) >> 3);
                    }
                    player->position.x += self->field_7C.x;
                    player->position.y += self->field_7C.y;
                }
                else {
                    if (player->camera) {
                        if (self->flag)
                            player->camera->state = Camera_State_Follow;
                    }
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
        if (Player_CheckCollisionTouch(player, self, &FilmReel->hitbox)) {
            player->collisionLayers |= Zone->moveID;
            player->moveOffset.x = FilmReel->offsetPos.x - self->field_74.x;
            player->moveOffset.y = FilmReel->offsetPos.y - self->field_74.y;
            if (!player->sidekick) {
                if (player->onGround || RSDK.CheckObjectCollisionTouchCircle(self, 0x400000, player, 0x100000)) {

                    bool32 flag = true;
                    switch (self->field_90) {
                        case 0:
                            if (player->collisionMode || (!player->left && !player->right && player->state != Player_State_Roll)) {
                                flag = false;
                            }

                            if (abs(player->position.x - self->position.x) < 2 * abs(player->groundVel) && flag) {
                                if (player->groundVel > 0x60000)
                                    player->groundVel = 0x60000;
                                if (player->direction == FLIP_NONE) {
                                    player->position.x = self->position.x;
                                    self->field_8C += player->groundVel;
                                }
                            }
                            else {
                                player->groundVel -= self->field_8C >> 5;
                                if (player->groundVel > 0x60000)
                                    player->groundVel = 0x60000;
                            }
                            break;
                        case 1:
                            if (player->collisionMode || (!player->left && !player->right && player->state != Player_State_Roll)
                                || player->direction) {
                                flag = false;
                            }

                            if (flag) {
                                if (abs(player->position.x - self->position.x) < 2 * abs(player->groundVel) && player->groundVel > 0) {
                                    player->position.x = self->position.x;
                                }
                                self->field_8C += player->groundVel;
                            }
                            break;
                        case 2:
                            if (abs(player->position.x - self->position.x) >= 2 * abs(player->groundVel) || player->collisionMode
                                || player->position.x >= self->position.x) {
                                if (player->right)
                                    self->field_8C += player->groundVel;
                            }
                            else {
                                if (player->groundVel < 0)
                                    player->position.x = self->position.x;
                                self->field_8C += player->groundVel;
                            }
                            break;
                    }

                    if (player->camera && self->flag) {
                        player->camera->state = StateMachine_None;
                        player->camera->position.x += self->field_7C.x + ((self->position.x - player->camera->position.x) >> 3);
                        player->camera->position.y += self->field_7C.y + ((self->position.y - player->camera->position.y - 0x200000) >> 3);
                    }
                }
                else {
                    if (player->camera) {
                        if (self->flag)
                            player->camera->state = Camera_State_Follow;
                    }
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void FilmReel_EditorDraw(void)
{
    RSDK_THIS(FilmReel);

    self->field_74      = self->position;
    self->drawPos       = self->position;
    self->endPos.x      = self->pathSize.x + self->drawPos.x;
    self->endPos.y      = self->pathSize.y + self->drawPos.y;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0xC00000;
    self->updateRange.y = 0xC00000;
    self->scale.x       = 0x200;
    self->scale.y       = 0x200;

    self->angle     = RSDK.ATan2(self->pathSize.x, self->pathSize.y);
    self->field_94  = self->angle >> 6;
    self->field_90  = 1;
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    
    FilmReel_Draw();
}

void FilmReel_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmReel.bin", SCOPE_STAGE);
    else
        FilmReel->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/FilmReel.bin", SCOPE_STAGE);
}
#endif

void FilmReel_Serialize(void)
{
    RSDK_EDITABLE_VAR(FilmReel, VAR_VECTOR2, pathSize);
    RSDK_EDITABLE_VAR(FilmReel, VAR_UINT8, spinDirection);
}
