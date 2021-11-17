#include "SonicMania.h"

ObjectFilmReel *FilmReel;

void FilmReel_Update(void)
{
    RSDK_THIS(FilmReel);

    entity->velocity.x = (entity->field_8C * RSDK.Cos256(entity->angle)) >> 9;
    entity->velocity.y = (entity->field_8C * RSDK.Sin256(entity->angle)) >> 9;
    entity->field_74.x += entity->velocity.x;
    entity->field_74.y += entity->velocity.y;
    entity->field_90 = 0;

    switch (entity->field_94) {
        case 0:
            if (entity->drawPos.x != entity->endPos.x) {
                if (entity->field_74.x <= entity->drawPos.x) {
                    entity->field_74.x = entity->drawPos.x;
                    entity->field_90   = 1;
                }
                if (entity->field_74.x >= entity->endPos.x) {
                    entity->field_90 |= 2;
                    entity->field_74.x = entity->endPos.x;
                }
            }

            if (entity->drawPos.y != entity->endPos.y) {
                if (entity->field_74.y <= entity->drawPos.y) {
                    entity->field_90 |= 1;
                    entity->field_74.y = entity->drawPos.y;
                }
                if (entity->field_74.y >= entity->endPos.y) {
                    entity->field_90 |= 2;
                    entity->field_74.y = entity->field_74.y;
                }
            }
            break;
        case 1:
            if (entity->endPos.x != entity->endPos.x) {
                if (entity->field_74.x <= entity->endPos.x) {
                    entity->field_74.x = entity->endPos.x;
                    entity->field_90   = 2;
                }
                if (entity->field_74.x >= entity->endPos.x) {
                    entity->field_90 |= 1;
                    entity->field_74.x = entity->drawPos.x;
                }
            }

            if (entity->drawPos.y != entity->endPos.y) {
                if (entity->field_74.y <= entity->drawPos.y) {
                    entity->field_90 |= 1;
                    entity->field_74.y = entity->drawPos.y;
                }
                if (entity->field_74.y >= entity->endPos.y) {
                    entity->field_90 |= 2;
                    entity->field_74.y = entity->field_74.y;
                }
            }
            break;
        case 2:
            if (entity->drawPos.x != entity->endPos.x) {
                if (entity->field_74.x <= entity->endPos.x) {
                    entity->field_74.x = entity->endPos.x;
                    entity->field_90   = 2;
                }
                if (entity->field_74.x >= entity->drawPos.x) {
                    entity->field_90 |= 1;
                    entity->field_74.x = entity->drawPos.x;
                }
            }

            if (entity->drawPos.y != entity->endPos.y) {
                if (entity->field_74.y <= entity->endPos.y) {
                    entity->field_90 |= 2;
                    entity->field_74.y = entity->endPos.y;
                }
                if (entity->field_74.y >= entity->drawPos.y) {
                    entity->field_90 |= 1;
                    entity->field_74.y = entity->drawPos.y;
                }
            }
            break;
        case 3:
            if (entity->drawPos.x != entity->endPos.x) {
                if (entity->field_74.x <= entity->drawPos.x) {
                    entity->field_74.x = entity->drawPos.x;
                    entity->field_90   = 1;
                }
                if (entity->field_74.x >= entity->endPos.x) {
                    entity->field_90 |= 2;
                    entity->field_74.x = entity->endPos.x;
                }
            }

            if (entity->drawPos.y != entity->endPos.y) {
                if (entity->field_74.y <= entity->endPos.y) {
                    entity->field_90 |= 2;
                    entity->field_74.y = entity->endPos.y;
                }
                if (entity->field_74.y >= entity->drawPos.y) {
                    entity->field_90 |= 1;
                    entity->field_74.y = entity->drawPos.y;
                }
            }
            break;
        default: break;
    }

    if (entity->field_90) {
        if (!entity->field_90) {
            RSDK.PlaySfx(FilmReel->sfxLanding, false, 255);
            RSDK.StopSFX(FilmReel->sfxUnravel);
            if (entity->field_90 == 1)
                entity->active = ACTIVE_BOUNDS;
        }
    }
    else {
        if (!(Zone->timer & 7))
            RSDK.PlaySfx(FilmReel->sfxUnravel, false, 255);
        entity->active = ACTIVE_NORMAL;
    }

    entity->field_8C -= 0x2000;
    entity->field_7C.x = (entity->field_74.x & 0xFFFF0000) - (entity->position.x & 0xFFFF0000);
    entity->field_7C.y = (entity->field_74.y & 0xFFFF0000) - (entity->position.y & 0xFFFF0000);
    if (!entity->spinDirection)
        FilmReel_SpinLeft();
    else
        FilmReel_SpinRight();
    entity->field_8C = clampVal(entity->field_8C, -0x80000, 0x80000);

    entity->position = entity->field_74;
    if (!entity->field_90) {
        if (entity->spinDirection)
            entity->field_84 -= entity->field_8C;
        else
            entity->field_84 += entity->field_8C;
        entity->rotation = (entity->field_84 >> 16) & 0x1FF;
    }

    if (abs(entity->pathSize.x) <= abs(entity->pathSize.y)) {
        int scale = (abs(entity->endPos.y - entity->position.y) >> 8) / (abs(entity->pathSize.x) >> 16) + 256;
        if (scale > 512)
            scale = 512;
        entity->scale.x = scale;
        entity->scale.y = scale;
    }
    else {
        int scale = (abs(entity->endPos.x - entity->position.x) >> 8) / (abs(entity->pathSize.x) >> 16) + 256;
        if (scale > 512)
            scale = 512;
        entity->scale.x = scale;
        entity->scale.y = scale;
    }
    entity->field_98 = abs(entity->position.x - entity->drawPos.x) <= abs(entity->pathSize.y - entity->drawPos.y);
}

void FilmReel_LateUpdate(void) {}

void FilmReel_StaticUpdate(void) {}

void FilmReel_Draw(void)
{
    RSDK_THIS(FilmReel);

    int angle = (entity->angle - 64) & 0xFF;
    int x2    = (16 * entity->scale.x * RSDK.Cos256(angle) + entity->position.x) & 0xFFFF0000;
    int y2    = (16 * entity->scale.x * RSDK.Sin256(angle) + entity->position.y) & 0xFFFF0000;

    entity->animator2.frameID = (entity->rotation >> 4) & 1;
    colour colour1            = 0x3868A8;
    if (entity->animator2.frameID)
        colour1 = 0x204078;
    colour colour2 = 0x204078;
    if (entity->animator2.frameID)
        colour2 = 0x182850;

    if (entity->field_98) {
        RSDK.DrawLine(entity->drawPos.x, entity->drawPos.y, x2, y2, colour1, 255, INK_NONE, false);
        RSDK.DrawLine(entity->drawPos.x, entity->drawPos.y + 0x18000, x2, y2 + 0x18000, colour2, 255, INK_NONE, false);
    }
    else {
        RSDK.DrawLine(entity->drawPos.x, entity->drawPos.y, x2, y2, colour1, 255, INK_NONE, false);
        RSDK.DrawLine(entity->drawPos.x - 0x18000, entity->drawPos.y, x2 - 0x18000, y2, colour2, 255, INK_NONE, false);
    }
    RSDK.DrawSprite(&entity->animator4, &entity->drawPos, false);

    entity->drawFX = FX_SCALE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->drawFX = FX_ROTATE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void FilmReel_Create(void *data)
{
    RSDK_THIS(FilmReel);

    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 2, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(FilmReel->aniFrames, 3, &entity->animator4, true, 0);
    if (!SceneInfo->inEditor) {
        entity->field_74      = entity->position;
        entity->drawPos       = entity->position;
        entity->endPos.x      = entity->pathSize.x + entity->drawPos.x;
        entity->endPos.y      = entity->pathSize.y + entity->drawPos.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0xC00000;
        entity->updateRange.y = 0xC00000;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;

        entity->angle     = RSDK.ATan2(entity->pathSize.x, entity->pathSize.y);
        entity->field_94  = entity->angle >> 6;
        entity->field_90  = 1;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
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
    FilmReel->sfxUnravel    = RSDK.GetSFX("Stage/Unravel.wav");
    FilmReel->sfxLanding    = RSDK.GetSFX("Stage/Landing.wav");
}

void FilmReel_SpinLeft(void)
{
    RSDK_THIS(FilmReel);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &FilmReel->hitbox)) {
            player->collisionLayers |= Zone->moveID;
            player->moveOffset.x = FilmReel->offsetPos.x - entity->field_74.x;
            player->moveOffset.y = FilmReel->offsetPos.y - entity->field_74.y;
            if (!player->sidekick) {
                if (player->onGround || RSDK.CheckObjectCollisionTouchCircle(entity, 0x400000, player, 0x100000)) {

                    bool32 flag = true;
                    switch (entity->field_90) {
                        case 0:
                            if (player->collisionMode || !player->left && !player->right && player->state != Player_State_Roll) {
                                flag = false;
                            }

                            if (abs(player->position.x - entity->position.x) < 2 * abs(player->groundVel) && flag) {
                                if (player->groundVel < -0x60000)
                                    player->groundVel = -0x60000;
                                if (player->direction == FLIP_X) {
                                    player->position.x = entity->position.x;
                                    entity->field_8C -= player->groundVel;
                                }
                            }
                            else {
                                player->groundVel += entity->field_8C >> 5;
                                if (player->groundVel < -0x60000)
                                    player->groundVel = -0x60000;
                            }
                            break;
                        case 1:
                            if (!player->collisionMode && (player->left || player->right || player->state == Player_State_Roll)
                                && player->direction == FLIP_X) {
                                if (abs(player->position.x - entity->position.x) < 2 * abs(player->groundVel)) {
                                    if (player->groundVel < 0)
                                        player->position.x = entity->position.x;
                                    entity->field_8C -= player->groundVel;
                                }
                            }
                            break;
                        case 2:
                            if (abs(player->position.x - entity->position.x) >= 2 * abs(player->groundVel) || player->collisionMode
                                || player->position.x <= entity->position.x) {
                                if (player->left)
                                    entity->field_8C -= player->groundVel;
                            }
                            else {
                                if (player->groundVel > 0)
                                    player->position.x = entity->position.x;
                                entity->field_8C -= player->groundVel;
                            }
                            break;
                    }

                    if (player->camera && entity->flag) {
                        player->camera->state = 0;
                        player->camera->position.x += entity->field_7C.x + ((entity->position.x - player->camera->position.x) >> 3);
                        player->camera->position.y += entity->field_7C.y + ((entity->position.y - player->camera->position.y - 0x200000) >> 3);
                    }
                    player->position.x += entity->field_7C.x;
                    player->position.y += entity->field_7C.y;
                }
                else {
                    if (player->camera) {
                        if (entity->flag)
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
        if (Player_CheckCollisionTouch(player, entity, &FilmReel->hitbox)) {
            player->collisionLayers |= Zone->moveID;
            player->moveOffset.x = FilmReel->offsetPos.x - entity->field_74.x;
            player->moveOffset.y = FilmReel->offsetPos.y - entity->field_74.y;
            if (!player->sidekick) {
                if (player->onGround || RSDK.CheckObjectCollisionTouchCircle(entity, 0x400000, player, 0x100000)) {

                    bool32 flag = true;
                    switch (entity->field_90) {
                        case 0:
                            if (player->collisionMode || (!player->left && !player->right && player->state != Player_State_Roll)) {
                                flag = false;
                            }

                            if (abs(player->position.x - entity->position.x) < 2 * abs(player->groundVel) && flag) {
                                if (player->groundVel > 0x60000)
                                    player->groundVel = 0x60000;
                                if (player->direction == FLIP_NONE) {
                                    player->position.x = entity->position.x;
                                    entity->field_8C += player->groundVel;
                                }
                            }
                            else {
                                player->groundVel -= entity->field_8C >> 5;
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
                                if (abs(player->position.x - entity->position.x) < 2 * abs(player->groundVel) && player->groundVel > 0) {
                                    player->position.x = entity->position.x;
                                }
                                entity->field_8C += player->groundVel;
                            }
                            break;
                        case 2:
                            if (abs(player->position.x - entity->position.x) >= 2 * abs(player->groundVel) || player->collisionMode
                                || player->position.x >= entity->position.x) {
                                if (player->right)
                                    entity->field_8C += player->groundVel;
                            }
                            else {
                                if (player->groundVel < 0)
                                    player->position.x = entity->position.x;
                                entity->field_8C += player->groundVel;
                            }
                            break;
                    }

                    if (player->camera && entity->flag) {
                        player->camera->state = StateMachine_None;
                        player->camera->position.x += entity->field_7C.x + ((entity->position.x - player->camera->position.x) >> 3);
                        player->camera->position.y += entity->field_7C.y + ((entity->position.y - player->camera->position.y - 0x200000) >> 3);
                    }
                }
                else {
                    if (player->camera) {
                        if (entity->flag)
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

    entity->field_74      = entity->position;
    entity->drawPos       = entity->position;
    entity->endPos.x      = entity->pathSize.x + entity->drawPos.x;
    entity->endPos.y      = entity->pathSize.y + entity->drawPos.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0xC00000;
    entity->updateRange.y = 0xC00000;
    entity->scale.x       = 0x200;
    entity->scale.y       = 0x200;

    entity->angle     = RSDK.ATan2(entity->pathSize.x, entity->pathSize.y);
    entity->field_94  = entity->angle >> 6;
    entity->field_90  = 1;
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    
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
