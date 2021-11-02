#include "SonicMania.h"

ObjectWaterGush *WaterGush;

void WaterGush_Update(void)
{
    RSDK_THIS(WaterGush);
    WaterGush_SetupHitboxes();
    int32 flag          = entity->flag;
    entity->direction = FLIP_NONE;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (!((1 << playerID) & entity->activePlayers)) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
                entity->active = ACTIVE_NORMAL;
                if (!player->sidekick)
                    entity->flag = true;
                RSDK.PlaySfx(WaterGush->sfxGush, false, 255);
                entity->activePlayers |= 1 << playerID;
                RSDK.SetSpriteAnimation(player->spriteIndex, 18, &player->playerAnimator, true, 6);
                player->nextGroundState = 0;
                player->nextAirState    = 0;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->tileCollisions  = false;
                player->onGround        = false;
                player->state           = Player_State_None;
            }
        }

        if (((1 << playerID) & entity->activePlayers)) {
            int32 xMult = 0;
            int32 yMult = 0;

            int32 posY = 0;
            switch (entity->orientation) {
                default: break;
                case 0:
                    flag  = entity->position.x;
                    posY  = entity->position.y - entity->field_78 - 0x140000;
                    yMult = -1;
                    break;
                case 1:
                    xMult = 1;
                    flag  = entity->field_78 + 0x140000 + entity->position.x;
                    posY  = entity->position.y;
                    break;
                case 2:
                    xMult = -1;
                    flag  = entity->position.x - entity->field_78 - 0x140000;
                    posY  = entity->position.y;
                    break;
            }

            player->position.x += (flag - player->position.x) >> 2;
            player->position.y += (posY - player->position.y) >> 2;
            player->state = Player_State_None;

            if ((!Player_CheckCollisionTouch(player, entity, &entity->hitbox1) && !Player_CheckCollisionTouch(player, entity, &entity->hitbox2))
                || entity->field_84) {
                entity->activePlayers &= ~(1 << playerID);
                player->state          = Player_State_Air;
                player->tileCollisions = true;
                player->onGround       = false;
                player->velocity.x     = xMult * (abs(entity->speed) << 15);
                player->velocity.y     = yMult * (abs(entity->speed) << 15);
            }
        }
    }

    if (entity->flag) {
        if (!flag) {
            RSDK.PlaySfx(Water->sfx_Splash, false, 255);
            WaterGush_Unknown3();
        }

        if (entity->field_84) {
            if (!entity->orientation) {
                entity->field_7C += 0x3800;
                if (entity->field_78 > 0)
                    entity->field_78 -= entity->field_7C;

                entity->field_78 = maxVal(entity->field_78, 0);
                if (!entity->field_78) {
                    entity->flag     = false;
                    entity->field_84 = 0;
                    entity->field_7C = 0;
                }
            }
        }
        else {
            if (entity->field_78 < entity->length << 22) {
                entity->field_78 += (abs(entity->speed) << 15);
            }
            entity->field_78 = minVal(entity->length << 22, entity->field_78);
            if (entity->field_78 == entity->length << 22)
                entity->field_84 = 1;
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->field_78 = 0;
        entity->flag     = false;
        entity->field_84 = 0;
        entity->field_7C = 0;
        entity->active   = ACTIVE_BOUNDS;
    }
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
}

void WaterGush_LateUpdate(void) {}

void WaterGush_StaticUpdate(void) {}

void WaterGush_Draw(void)
{
    RSDK_THIS(WaterGush);
    if (entity->field_78 > 0)
        WaterGush_DrawSprites();
}

void WaterGush_Create(void *data)
{
    RSDK_THIS(WaterGush);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->posUnknown1.x = entity->position.x;
    entity->posUnknown1.y = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    if (entity->orientation == 2 || entity->orientation == 1)
        entity->updateRange.x = (entity->length + 2) << 22;
    entity->updateRange.y = 0x800000;
    if (!entity->orientation)
        entity->updateRange.y = (entity->length + 2) << 22;
    if (!entity->speed)
        entity->speed = 16;
    WaterGush_SetupHitboxes();

    if (entity->orientation) {
        if (entity->orientation == 1) {
            entity->direction = FLIP_NONE;
        }
        else {
            if (entity->orientation == 2)
                entity->direction = FLIP_X;
        }
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 3, &entity->animator2, true, 0);
    }
    else {
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 2, &entity->animator2, true, 0);
    }
}

void WaterGush_StageLoad(void)
{
    WaterGush->aniFrames = RSDK.LoadSpriteAnimation("HCZ/WaterGush.bin", SCOPE_STAGE);
    WaterGush->sfxGush   = RSDK.GetSFX("HCZ/WaterGush.wav");
}

void WaterGush_SetupHitboxes(void)
{
    RSDK_THIS(WaterGush);

    switch (entity->orientation) {
        default: break;
        case 0:
            entity->hitbox1.left   = -32;
            entity->hitbox1.top    = -20 - (entity->field_78 >> 16);
            entity->hitbox1.right  = 32;
            entity->hitbox1.bottom = 0;

            entity->hitbox2.left   = -32;
            entity->hitbox2.top    = 0;
            entity->hitbox2.right  = 32;
            entity->hitbox2.bottom = 16;
            break;
        case 1:
            entity->hitbox1.left   = 0;
            entity->hitbox1.top    = -32;
            entity->hitbox1.right  = (entity->field_78 >> 16) + 20;
            entity->hitbox1.bottom = 32;

            if (Music->activeTrack == TRACK_EGGMAN1) {
                entity->hitbox2.left   = -192;
                entity->hitbox2.top    = -16;
                entity->hitbox2.bottom = 16;
            }
            else {
                entity->hitbox2.left   = -16;
                entity->hitbox2.top    = -32;
                entity->hitbox2.bottom = 32;
            }
            entity->hitbox2.right = 0;
            break;
        case 2:
            entity->hitbox1.left   = -20 - (entity->field_78 >> 16);
            entity->hitbox1.top    = -32;
            entity->hitbox1.right  = 0;
            entity->hitbox1.bottom = 32;

            entity->hitbox2.left   = 0;
            entity->hitbox2.top    = -32;
            entity->hitbox2.right  = 16;
            entity->hitbox2.bottom = 32;
            break;
    }
}

void WaterGush_DrawSprites(void)
{
    RSDK_THIS(WaterGush);
    Vector2 drawPos, drawPos2;

    uint8 storeDir = entity->direction;
    drawPos2.x    = entity->position.x;
    drawPos2.y    = entity->position.y;

    int32 offsetX = 0;
    int32 offsetY = 0;
    switch (entity->orientation) {
        default: break;
        case 0:
            entity->direction = FLIP_NONE;
            drawPos2.y -= entity->field_78;
            offsetX = 0;
            offsetY = 0x400000;
            break;
        case 1:
            entity->direction = FLIP_NONE;
            offsetX           = -0x400000;
            offsetY           = 0;
            drawPos2.x += entity->field_78;
            break;
        case 2:
            entity->direction = FLIP_X;
            offsetX           = 0x400000;
            offsetY           = 0;
            drawPos2.x -= entity->field_78;
            break;
    }

    drawPos.x = drawPos2.x;
    drawPos.y = drawPos2.y;
    if (entity->field_78 > 0) {
        int32 count = ((entity->field_78 - 1) >> 22) + 1;
        for (int32 i = 0; i < count; ++i) {
            drawPos.x += offsetX;
            drawPos.y += offsetY;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        }
    }
    RSDK.DrawSprite(&entity->animator2, &drawPos2, false);

    entity->direction = storeDir;
}

void WaterGush_Unknown3(void)
{
    RSDK_THIS(WaterGush);

    int32 velX1   = 0;
    int32 velY1   = 0;
    int32 dir1    = 0;
    int32 spawnX1 = 0;
    int32 spawnY1 = 0;

    int32 velX2   = 0;
    int32 velY2   = 0;
    int32 dir2    = 0;
    int32 spawnX2 = 0;
    int32 spawnY2 = 0;

    int32 velX3   = 0;
    int32 velY3   = 0;
    int32 dir3    = 0;
    int32 spawnY3 = 0;
    int32 spawnX3 = 0;

    int32 velX4   = 0;
    int32 velY4   = 0;
    int32 dir4    = 0;
    int32 spawnX4 = 0;
    int32 spawnY4 = 0;

    switch (entity->orientation) {
        default: break;
        case 0:
            velX1   = -0x30000;
            velY1   = -0x4000 * abs(entity->speed);
            dir1    = FLIP_X;
            spawnY1 = entity->position.y;
            spawnX1 = entity->position.x - 0x200000;

            velX2   = -0x28000;
            velY2   = -0x8000 * abs(entity->speed);
            dir2    = FLIP_X;
            spawnX2 = entity->position.x - 0x100000;
            spawnY2 = entity->position.y;

            velX3   = 0x28000;
            velY3   = -0x8000 * abs(entity->speed);
            dir4    = FLIP_NONE;
            spawnX3 = entity->position.x + 0x100000;
            spawnY3 = spawnY1;

            velX4   = 0x30000;
            velY4   = -0x4000 * abs(entity->speed);
            dir3    = FLIP_NONE;
            spawnX4 = entity->position.x + 0x200000;
            spawnY4 = entity->position.y;
            break;
        case 1:
            velX1   = abs(entity->speed) << 14;
            velY1   = -0x30000;
            dir1    = FLIP_NONE;
            spawnX1 = entity->position.x;
            spawnY1 = entity->position.y - 0x200000;

            velX2   = abs(entity->speed) << 15;
            velY2   = -0x28000;
            dir2    = FLIP_NONE;
            spawnX2 = entity->position.x;
            spawnY2 = entity->position.y - 0x100000;

            velX3   = velX2;
            velY3   = 0x28000;
            dir3    = FLIP_NONE;
            spawnX3 = entity->position.x;
            spawnY3 = entity->position.y + 0x100000;

            velX4   = velX1;
            velY4   = 0x30000;
            dir4    = FLIP_NONE;
            spawnX4 = entity->position.x;
            spawnY4 = entity->position.y + 0x200000;
            break;
        case 2:
            velX1   = -0x4000 * abs(entity->speed);
            velY1   = -0x30000;
            dir1    = FLIP_X;
            spawnX1 = entity->position.x;
            spawnY1 = entity->position.y - 0x200000;

            velX2   = -0x8000 * abs(entity->speed);
            velY2   = -0x28000;
            dir2    = FLIP_X;
            spawnX2 = entity->position.x;
            spawnY2 = entity->position.y - 0x100000;

            velX3   = velX2;
            velY3   = 0x28000;
            dir3    = FLIP_X;
            spawnY3 = entity->position.y + 0x100000;
            spawnY4 = entity->position.y + 0x200000;

            velX4   = velX1;
            velY4   = 0x30000;
            dir4    = FLIP_X;
            spawnX4 = entity->position.x;
            spawnX3 = entity->position.x;
            break;
    }

    for (int32 i = 0; i < 2; ++i) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX1, spawnY1);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.y = velY1;
        debris->velocity.y >>= i;
        debris->velocity.x = velX1;
        debris->direction  = dir1;
        debris->drawFX     = FX_FLIP;
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->gravity    = 0x3800;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX2, spawnY2);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.y = velY2;
        debris->velocity.y >>= i;
        debris->velocity.x = velX2;
        debris->direction  = dir2;
        debris->drawFX     = FX_SCALE | FX_FLIP;
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->gravity    = 0x3800;
        debris->scale.x    = 512;
        debris->scale.y    = 512;
        debris->scaleInc.x = 8 >> i;
        debris->scaleInc.y = 8 >> i;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX3, spawnY3);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.x = velX3;
        debris->velocity.y = velY3;
        debris->velocity.y >>= i;
        debris->direction  = dir3;
        debris->drawFX     = FX_SCALE | FX_FLIP;
        debris->drawOrder  = Zone->drawOrderHigh;
        debris->gravity    = 0x3800;
        debris->scale.x    = 512;
        debris->scale.y    = 512;
        debris->scaleInc.x = 8 >> i;
        debris->scaleInc.y = 8 >> i;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX4, spawnY4);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
        debris->position.x += RSDK.Rand(-0x80000, 0x80000);
        debris->position.y += RSDK.Rand(-0x80000, 0x80000);
        debris->velocity.x = velX4;
        debris->velocity.y = velY4;
        debris->velocity.y >>= i;
        debris->direction = dir4;
        debris->drawFX    = FX_FLIP;
        debris->drawOrder = Zone->drawOrderHigh;
        debris->gravity   = 0x3800;
    }
}

#if RETRO_INCLUDE_EDITOR
void WaterGush_EditorDraw(void)
{
    RSDK_THIS(WaterGush);

    if (entity->orientation) {
        if (entity->orientation == 1) {
            entity->direction = FLIP_NONE;
        }
        else {
            if (entity->orientation == 2)
                entity->direction = FLIP_X;
        }
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 3, &entity->animator2, true, 0);
    }
    else {
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(WaterGush->aniFrames, 2, &entity->animator2, true, 0);
    }

    WaterGush_DrawSprites();
}

void WaterGush_EditorLoad(void) { WaterGush->aniFrames = RSDK.LoadSpriteAnimation("HCZ/WaterGush.bin", SCOPE_STAGE); }
#endif

void WaterGush_Serialize(void)
{
    RSDK_EDITABLE_VAR(WaterGush, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(WaterGush, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(WaterGush, VAR_INT32, speed);
}
