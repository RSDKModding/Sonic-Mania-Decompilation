#include "SonicMania.h"

ObjectBouncePlant *BouncePlant;

void BouncePlant_Update(void)
{
    RSDK_THIS(BouncePlant);
    if (entity->stood) {
        if (entity->speedFlag) {
            entity->depression = 256;
        }
        else {
            if (entity->depression < 256)
                entity->depression += 32;
        }
    }
    else {
        if (entity->depression > 0)
            entity->depression -= 16;
    }

    entity->stood     = false;
    entity->speedFlag = false;
    foreach_active(Player, player)
    {
        if (abs(player->position.x - entity->position.x) <= 0x320000 && (player->velocity.y >= 0 || player->onGround)) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            if (entity->stood) {
                int32 posY = BoucePlant_Unknown3(player->position.x) + entity->stoodPos.y - (playerHitbox->bottom << 16) - 0x40000;
                if (player->position.y > posY - 0x80000) {
                    player->velocity.x += RSDK.Sin256(entity->angle) << 13 >> 8;
                    player->position.y    = posY;
                    player->velocity.y    = 0;
                    player->onGround      = true;
                    player->groundedStore = true;
                    player->angle         = 0;
                    player->collisionMode = CMODE_FLOOR;
                    player->groundVel     = player->velocity.x;
                    if (player->state == Player_State_GlideSlide || player->state == Player_State_KnuxGlideDrop)
                        player->state = Player_State_Ground;
                }
            }
            else {
                int32 posY = BoucePlant_Unknown2(player->position.x) + entity->position.y - (playerHitbox->bottom << 16) - 0x40000;
                if (player->position.y > posY - 0x80000 && player->position.y < entity->position.y + 0x400000) {
                    player->position.y = posY;
                    if (abs(player->velocity.x) > 0xC0000)
                        entity->speedFlag = true;

                    if (abs(player->position.x - entity->centerX) >= abs(player->velocity.x)) {
                        player->velocity.x += RSDK.Sin256(entity->angle) << 13 >> 8;
                        player->velocity.y    = 0;
                        player->onGround      = true;
                        player->groundedStore = true;
                        player->angle         = 0;
                        player->collisionMode = CMODE_FLOOR;
                        player->groundVel     = player->velocity.x;

                        if (player->state == Player_State_GlideSlide || player->state == Player_State_KnuxGlideDrop

#if RETRO_USE_PLUS 
                            || player->state == Player_State_MightyHammerDrop
#endif
                            ) {
                            player->state = Player_State_Ground;
                        }
                    }
                    else if (entity->depression > 160) {
                        if (entity->direction == FLIP_NONE)
                            player->velocity.x = -0xB4000;
                        else
                            player->velocity.x = 0xB4000;
                        player->velocity.y = -0xB4000;
                        player->onGround   = false;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGDIAGONAL, &player->playerAnimator, false, 0);
                        RSDK.PlaySfx(BouncePlant->sfxBouncePlant, false, 255);
                    }
                    else if (abs(player->groundVel) <= 0xC00000) {
                        player->velocity.x += RSDK.Sin256(entity->angle) << 13 >> 8;
                        player->velocity.y    = 0;
                        player->onGround      = true;
                        player->groundedStore = true;
                        player->angle         = 0;
                        player->collisionMode = CMODE_FLOOR;
                        player->groundVel     = player->velocity.x;

                        if (player->state == Player_State_GlideSlide || player->state == Player_State_KnuxGlideDrop
#if RETRO_USE_PLUS
                            || player->state == Player_State_MightyHammerDrop
#endif
                            ) {
                            player->state = Player_State_Ground;
                        }
                    }
                    else {
                        if (entity->direction == FLIP_NONE)
                            player->velocity.x = -0xB4000;
                        else
                            player->velocity.x = 0xB4000;
                        player->velocity.y = -0xB4000;
                        player->onGround   = false;
                        player->state      = Player_State_Air;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGDIAGONAL, &player->playerAnimator, false, 0);
                        RSDK.PlaySfx(BouncePlant->sfxBouncePlant, false, 255);
                    }
                    entity->stood      = true;
                    entity->timer      = 60;
                    entity->stoodPos.x = player->position.x;
                    entity->stoodPos.y = entity->position.y + BoucePlant_Unknown2(player->position.x);
                }
            }
        }
    }

    if (entity->stood) {
        for (int32 i = 0; i < 8; ++i) {
            entity->drawPos[i].y = entity->stoodPos.y + BoucePlant_Unknown3(entity->drawPos[i].x);
        }
    }
    else {
        if (entity->timer <= 0) {
            for (int32 i = 0; i < 8; ++i) {
                entity->drawPos[i] = entity->drawPos2[i];
            }
        }
        else {
            entity->timer--;

            for (int32 i = 0; i < 8; ++i) {
                int32 val            = entity->unknown[i] + ((entity->drawPos2[i].y - entity->drawPos[i].y) >> 3) - (entity->unknown[i] >> 3);
                entity->unknown[i] = val;
                entity->drawPos[i].y += val;
            }
        }
    }
}

void BouncePlant_LateUpdate(void) {}

void BouncePlant_StaticUpdate(void) {}

void BouncePlant_Draw(void)
{
    RSDK_THIS(BouncePlant);
    for (int32 i = 0; i < 8; ++i) {
        RSDK.DrawSprite(&entity->animator, &entity->drawPos[i], false);
        RSDK.DrawSprite(&entity->animators[i], &entity->drawPos[i], false);
    }
}

void BouncePlant_Create(void *data)
{
    RSDK_THIS(BouncePlant);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        BoucePlant_Unknown1();

        if (entity->direction) {
            entity->centerX = entity->position.x - 0x180000;
            entity->angle   = 64;
        }
        else {
            entity->centerX = entity->position.x + 0x180000;
            entity->angle   = 192;
        }

        RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &entity->animator, true, 0);
        for (int32 i = 0; i < 8; ++i) {
            RSDK.SetSpriteAnimation(BouncePlant->aniFrames, 1, &entity->animators[i], true, RSDK.Rand(1, 8));
            entity->drawPos[i].x = entity->drawPos2[i].x;
            entity->drawPos[i].y = entity->drawPos2[i].y;
        }
    }
}

void BouncePlant_StageLoad(void)
{
    BouncePlant->aniFrames      = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);
    BouncePlant->hitbox.left    = -50;
    BouncePlant->hitbox.left    = -12;
    BouncePlant->hitbox.right   = 50;
    BouncePlant->hitbox.right   = 8;
    BouncePlant->sfxBouncePlant = RSDK.GetSFX("SSZ1/BouncePlant.wav");
}

void BoucePlant_Unknown1(void)
{
    RSDK_THIS(BouncePlant);

    if (entity->direction == FLIP_NONE) {
        int32 x = entity->position.x - 0x2A0000;
        for (int32 i = 0; i < 8; ++i) {
            entity->drawPos2[i].x = x;
            x += 0xC0000;
        }
    }
    else {
        int32 x = entity->position.x + 0x2A0000;
        for (int32 i = 0; i < 8; ++i) {
            entity->drawPos2[i].x = x;
            x -= 0xC0000;
        }
    }

    int32 y = entity->position.y + 0x2A0000;
    for (int32 i = 0; i < 8; ++i) {
        entity->drawPos2[i].y = y;
        y -= 0xC0000;
    }
}

int32 BoucePlant_Unknown2(int32 x)
{
    RSDK_THIS(BouncePlant);

    int32 dist = 0;
    int32 pos  = 0;
    if (entity->direction) {
        int32 val = entity->position.x - 0x180000;
        dist    = x - entity->position.x;
        if (x < val) {
            pos = 3 * (x - val);
        }
        else {
            pos = (x - val) / 3;
        }
    }
    else {
        int32 val = entity->position.x + 0x180000;
        dist    = entity->position.x - x;
        if (x > val) {
            pos = 3 * (val - x);
        }
        else {
            pos = (val - x) / 3;
        }
    }

    pos += 0x200000;
    dist = clampVal(dist, -0x320000, 0x320000);
    pos  = clampVal(pos, -0x320000, 0x320000);
    return dist + ((entity->depression * (pos - dist)) >> 8);
}

int32 BoucePlant_Unknown3(int32 x)
{
    RSDK_THIS(BouncePlant);

    int32 y         = 0;
    int32 distanceX = 0;
    int32 distanceY = 0;
    if (entity->direction) {
        y = (x - entity->stoodPos.x) >> 15;
        if (x < entity->stoodPos.x) {
            distanceX = (entity->stoodPos.x - entity->drawPos2[7].x) >> 16;
            distanceY = (entity->stoodPos.y - entity->drawPos2[7].y) >> 16;
        }
        else {
            distanceX = (entity->drawPos2[0].x - entity->stoodPos.x) >> 16;
            distanceY = (entity->drawPos2[0].y - entity->stoodPos.y) >> 16;
        }
    }
    else {
        y = (entity->stoodPos.x - x) >> 15;
        if (x > entity->stoodPos.x) {
            distanceX = (entity->drawPos2[7].x - entity->stoodPos.x) >> 16;
            distanceY = (entity->stoodPos.y - entity->drawPos2[7].y) >> 16;
        }
        else {
            distanceX = (entity->stoodPos.x - entity->drawPos2[0].x) >> 16;
            distanceY = (entity->drawPos2[0].y - entity->stoodPos.y) >> 16;
        }
    }

    if (distanceX > 0)
        y = y * distanceY / distanceX;
    return y << 15;
}

#if RETRO_INCLUDE_EDITOR
void BouncePlant_EditorDraw(void) {}

void BouncePlant_EditorLoad(void) {}
#endif

void BouncePlant_Serialize(void) { RSDK_EDITABLE_VAR(BouncePlant, VAR_UINT8, direction); }
