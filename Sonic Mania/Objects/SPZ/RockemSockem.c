#include "SonicMania.h"

ObjectRockemSockem *RockemSockem;

// Extras lol

void RockemSockem_Update(void)
{
    RSDK_THIS(RockemSockem);
    RSDK.ProcessAnimation(&entity->ballAnimator);

    if (entity->timer > 0) {
        entity->timer--;
        if (entity->timer == 1)
            RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 2, &entity->ballAnimator, true, entity->ballAnimator.frameID);
    }

    if (entity->timer2 > 0) {
        entity->timer2--;
        if (entity->timer2 == 1) {
            entity->position.x = entity->startPos.x;
            entity->position.y = entity->startPos.y;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            entity->active     = ACTIVE_BOUNDS;
        }
    }

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.x = entity->velocity.x + ((entity->startPos.x - entity->position.x) >> 6) - (entity->velocity.x >> 4);
    entity->velocity.y = entity->velocity.y + ((entity->startPos.y - entity->position.y) >> 6) - (entity->velocity.y >> 4);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &RockemSockem->hitbox)) {

            int32 vel = 0;
            if (abs(player->velocity.x) <= abs(player->velocity.y)) {
                vel = abs(player->velocity.y);
            }
            else {
                vel = abs(player->velocity.x);
            }
            int32 angle      = RSDK.ATan2((player->position.x - entity->position.x) / 3, player->position.y - entity->position.y);
            int32 distX      = abs(entity->position.x - entity->startPos.x);
            int32 distY      = abs(entity->position.y - entity->startPos.y);
            entity->timer2 = 120;
            if ((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16) < 0x100 && !entity->timer) {
                if ((angle - 32) & 0xC0) {
                    entity->velocity.x -= vel * RSDK.Cos256(angle) >> 8;
                    entity->velocity.y -= vel * RSDK.Sin256(angle) >> 8;
                }
                else {
                    entity->velocity.x -= vel * RSDK.Cos256(angle) >> 9;
                    entity->velocity.y -= 0x2000;
                }

                if (entity->velocity.x >= -0x40000) {
                    if (entity->velocity.x > 0x40000)
                        entity->velocity.x = 0x40000;
                }
                else {
                    entity->velocity.x = -0x40000;
                }
                player->velocity.x -= player->velocity.x >> 4;
                if (player->velocity.y <= 0)
                    player->velocity.y -= player->velocity.y >> 5;
                else
                    player->velocity.y -= player->velocity.y >> 4;
                entity->active = ACTIVE_NORMAL;
            }
            else if (player->velocity.y < 0) {
                entity->active = ACTIVE_NORMAL;
            }
            else {
                if ((((angle - 32) >> 6) & 3) != 1) {
                    if ((((angle - 32) >> 6) & 3) == 2) {
                        int32 spd            = 0;
                        player->velocity.x = (0x700 * RSDK.Cos256(angle) + player->velocity.x) >> 1;
                        if (0x700 * RSDK.Sin256(angle) >= 0)
                            spd = 0x700 * RSDK.Sin256(angle);
                        else
                            spd = -0x700 * RSDK.Sin256(angle);
                        player->velocity.y = -spd;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, false, 0);
                    }
                    else if ((((angle - 32) >> 6) & 3) != 3) {
                        int32 spd            = 0;
                        player->velocity.x = ((RSDK.Cos256(angle) << 9) + player->velocity.x) >> 1;
                        if (RSDK.Sin256(angle) << 10 >= 0)
                            spd = RSDK.Sin256(angle) << 10;
                        else
                            spd = -0x400 * RSDK.Sin256(angle);
                        player->velocity.y = -spd;

                        int32 anim = player->playerAnimator.animationID;
                        if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                            player->playerAnimator.animationID = ANI_WALK;
                    }
                }
                else {
                    int32 spd            = 0;
                    player->velocity.x = (0x500 * RSDK.Cos256(angle) + player->velocity.x) >> 1;
                    if (0x500 * RSDK.Sin256(angle) >= 0)
                        spd = 0x500 * RSDK.Sin256(angle);
                    else
                        spd = -0x500 * RSDK.Sin256(angle);
                    player->velocity.y = -spd;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, false, 0);
                }

                if (entity->ballAnimator.animationID != 3)
                    RSDK.PlaySfx(RockemSockem->sfxRockemSockem, 0, 255);
                player->state    = Player_State_Air;
                player->onGround = false;
                RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 3, &entity->ballAnimator, true, entity->ballAnimator.frameID);
                entity->timer  = 16;
                entity->active = ACTIVE_NORMAL;
            }
        }
    }

    if (entity->position.y > entity->drawPos.y - 0xE0000) {
        entity->position.y = entity->drawPos.y - 0xE0000;
        entity->timer      = 16;
    }

    if (abs(entity->startPos.x - entity->position.x) <= 0x10000)
        entity->field_E8 = 0;
    else
        entity->field_E8 = (entity->startPos.x - entity->position.x) >> 17;
    RockemSockem_Unknown1();
}

void RockemSockem_LateUpdate(void) {}

void RockemSockem_StaticUpdate(void) {}

void RockemSockem_Draw(void)
{
    RSDK_THIS(RockemSockem);
    for (int32 i = 0; i < RockemSockem_coilCount; ++i) {
        entity->rotation = entity->jointRotations[i];
        RSDK.DrawSprite(&entity->jointAnimator, &entity->jointPositions[i], false);
    }
    RSDK.DrawSprite(&entity->baseAnimator, &entity->drawPos, false);
    if (entity->ballAnimator.frameID > 9)
        entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->ballAnimator, NULL, false);
    entity->direction = FLIP_NONE;
}

void RockemSockem_Create(void *data)
{
    RSDK_THIS(RockemSockem);
    RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 0, &entity->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 1, &entity->jointAnimator, true, 0);
    RSDK.SetSpriteAnimation(RockemSockem->aniFrames, 2, &entity->ballAnimator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawPos.x = entity->position.x;
        entity->drawPos.y = entity->position.y;
        entity->position.y -= (0xE0000 + (0x50000 * RockemSockem_coilCount));
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->angle         = 0x80;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->drawFX        = FX_ROTATE | FX_FLIP;
    }
}

void RockemSockem_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        RockemSockem->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/RockemSockem.bin", SCOPE_STAGE);
    else
        RockemSockem->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/RockemSockem.bin", SCOPE_STAGE);
    RockemSockem->hitbox.left     = -16;
    RockemSockem->hitbox.top      = -14;
    RockemSockem->hitbox.right    = 16;
    RockemSockem->hitbox.bottom   = 14;
    RockemSockem->sfxRockemSockem = RSDK.GetSFX("Stage/RockemSockem.wav");
}

void RockemSockem_Unknown1(void)
{
    RSDK_THIS(RockemSockem);
    int32 x    = (RSDK.Sin256(entity->angle) << 11) + entity->drawPos.x;
    int32 y    = (RSDK.Cos256(entity->angle) << 11) + entity->drawPos.y;
    int32 ang  = RSDK.ATan2(y - entity->position.y, (RSDK.Sin256(entity->angle) << 11) + entity->drawPos.x - entity->position.x);
    int32 posX = (0xC00 * RSDK.Sin256(ang) + entity->position.x) & 0xFFFF0000;
    int32 posY = (0xC00 * RSDK.Cos256(ang) + entity->position.y) & 0xFFFF0000;

    int32 angle = entity->angle;
    x >>= 16;
    y >>= 16;
    posX >>= 16;
    posY >>= 16;

    entity->jointRotations[0]   = 2 * (640 - angle);
    entity->jointPositions[0].x = 0x10002 * x;
    entity->jointPositions[0].y = 0x10002 * y;

    angle += entity->field_E8;
    entity->jointRotations[1]   = 2 * (640 - angle);
    entity->jointPositions[1].x = 0x2AAB * (x + posX + 4 * x);
    entity->jointPositions[1].y = 0x2AAB * (y + posY + 4 * y);

    angle += entity->field_E8;
    entity->jointRotations[2]   = 2 * (640 - angle);
    entity->jointPositions[2].x = 0x5556 * (posX + 2 * x);
    entity->jointPositions[2].y = 0x5556 * (posY + 2 * y);

    angle += entity->field_E8;
    entity->jointRotations[3]   = 2 * (640 - angle);
    entity->jointPositions[3].x = 0x8001 * (x + posX);
    entity->jointPositions[3].y = 0x8001 * (y + posY);

    angle += entity->field_E8;
    entity->jointRotations[4]   = 2 * (640 - angle);
    entity->jointPositions[4].x = 0x5556 * (x + 2 * posX);
    entity->jointPositions[4].y = 0x5556 * (y + 2 * posY);

    angle -= entity->field_E8;
    entity->jointRotations[5]   = 2 * (640 - angle);
    entity->jointPositions[5].x = 0x2AAB * (posX + x + 4 * posX);
    entity->jointPositions[5].y = 0x2AAB * (posY + y + 4 * posY);
}

void RockemSockem_EditorDraw(void) {}

void RockemSockem_EditorLoad(void) {}

void RockemSockem_Serialize(void) {}