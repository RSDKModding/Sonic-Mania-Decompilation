#include "SonicMania.h"

ObjectWhirlpool *Whirlpool;

void Whirlpool_Update(void)
{
    RSDK_THIS(Whirlpool);

    entity->angle += entity->angVel;
    if (entity->angle < 0)
        entity->angle += 1024;

    entity->angle %= 0x3FF;
    if (entity->activePlayers == 0xFF) {
        if (entity->alpha < 256)
            entity->alpha += 2;
    }
    else if (entity->activePlayers != 0xFE) {
        if (entity->activePlayers == 0xFD) {
            entity->alpha -= 2;
            if (entity->alpha <= 0)
                destroyEntity(entity);
        }
        else {
            foreach_active(Player, player)
            {
                int playerID = RSDK.GetEntityID(player);
                if (!((1 << playerID) & entity->activePlayers)) {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                        entity->active = ACTIVE_NORMAL;
                        entity->activePlayers |= (1 << playerID);
                        player->nextGroundState     = 0;
                        player->nextAirState        = 0;
                        player->velocity.x          = 0;
                        player->velocity.y          = 0;
                        player->groundVel           = 0;
                        player->onGround            = false;
                        player->tileCollisions      = true;
                        player->state               = Player_State_Air;
                        entity->field_288[playerID] = abs(player->position.x - entity->position.x) >> 16;

                        if (player->position.x > entity->position.x)
                            entity->field_278[playerID] = 0;
                        else
                            entity->field_278[playerID] = 512;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, true, 0);
                    }
                }

                if ((entity->activePlayers & (1 << playerID))) {
                    if (player->state == Player_State_Air) {
                        entity->field_278[playerID] += entity->angVel;

                        if (entity->field_278[playerID] < 0)
                            entity->field_278[playerID] += 0x400;
                        entity->field_278[playerID] %= 0x3FF;
                        int prevX          = player->position.x;
                        int prevY          = player->position.y;
                        player->position.x = entity->position.x;
                        player->position.x += (RSDK.Cos1024(entity->field_278[playerID]) << 6) * entity->field_288[playerID];
                        player->position.y += entity->yVel << 15;
                        player->velocity.x = player->position.x - prevX;
                        player->velocity.y = player->position.y - prevY;
                        player->position.x = prevX;
                        player->position.y = prevY;
                        if (entity->field_278[playerID] <= 512)
                            player->drawOrder = Zone->playerDrawHigh;
                        else
                            player->drawOrder = Zone->playerDrawLow;

                        if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox) || player->playerAnimator.animationID != ANI_FAN) {
                            entity->activePlayers &= ~(1 << playerID);
                            player->state = Player_State_Air;
                        }
                    }
                    else {
                        entity->activePlayers &= ~(1 << playerID);
                    }
                }
            }
        }
    }
}

void Whirlpool_LateUpdate(void) {}

void Whirlpool_StaticUpdate(void)
{
    bool32 flag = false;
    foreach_active(Whirlpool, pool)
    {
        if (pool->activePlayers) {
            flag             = true;
            Whirlpool->timer = 30;
        }
    }

    if (!flag) {
        if (Whirlpool->timer <= 30) {
            if (Whirlpool->timer > 0)
                Whirlpool->timer--;
        }
        else {
            Whirlpool->timer = 30;
        }
    }

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID == PauseMenu->objectID)
        Whirlpool->timer = 0;

    if (Whirlpool->playingSFX) {
        RSDK.SetChannelAttributes(Whirlpool->sfxChannel, minVal(Whirlpool->timer, 30) / 30.0, 0.0, 1.0);
    }

    bool32 stopFlag = !Whirlpool->timer;
    if (Whirlpool->timer > 0) {
        if (!Whirlpool->playingSFX) {
            Whirlpool->sfxChannel = RSDK.PlaySFX(Whirlpool->sfxWhirlpool, 56284, 255);
            Whirlpool->playingSFX = true;
        }
    }
    else if (stopFlag) {
        if (Whirlpool->playingSFX) {
            RSDK.StopSFX(Whirlpool->sfxWhirlpool);
            Whirlpool->playingSFX = false;
        }
    }
}

void Whirlpool_Draw(void) { WhirlPool_DrawSprites(); }

void Whirlpool_Create(void *data)
{
    RSDK_THIS(Whirlpool);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->posUnknown2   = entity->position;
    entity->inkEffect     = INK_ALPHA;
    entity->visible       = true;
    entity->updateRange.x = 0x800000 + entity->size.x;
    entity->updateRange.y = 0x800000 + entity->size.y;
    entity->drawFX        = FX_FLIP;
    entity->alpha         = 256;
    if (data)
        entity->size = *(Vector2 *)data;

    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.left   = -(entity->size.x >> 17);
    entity->hitbox.bottom = entity->size.y >> 17;
    entity->hitbox.top    = -(entity->size.y >> 17);
    if (RSDK_sceneInfo->inEditor) {
        if (!entity->size.x && !entity->size.y) {
            entity->size.x = 0x800000;
            entity->size.y = 0x800000;
        }
        if (!entity->yVel)
            entity->yVel = 4;
        if (!entity->angVel)
            entity->angVel = 12;
    }
    else {
        Whirlpool_SetupBubbles();
        RSDK.SetSpriteAnimation(Whirlpool->aniFrames, 3, &entity->animator, true, 0);
    }
}

void Whirlpool_StageLoad(void)
{
    Whirlpool->active       = ACTIVE_ALWAYS;
    Whirlpool->aniFrames    = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    Whirlpool->sfxWhirlpool = RSDK.GetSFX("HCZ/Whirlpool.wav");
}

void WhirlPool_DrawSprites(void)
{
    RSDK_THIS(Whirlpool);
    Vector2 drawPos;

    drawPos    = entity->position;
    int offset = (entity->yVel * Zone->timer % (entity->size.y >> 15)) << 15;
    for (int i = 0; i < 0x80; ++i) {
        int angle = entity->angle + entity->field_78[i];
        if (angle < 0)
            angle += 0x400;
        drawPos.x = entity->position.x;
        drawPos.x += (entity->field_78[i] >> 16) * (RSDK.Cos1024(angle % 1023) << 6);
        drawPos.y = entity->position.y + offset % entity->size.y + (2 * (i & 1) - 1) * (((i % 6) << 17) & 0xFFFC0000) - (entity->size.y >> 1);
        RSDK.SetSpriteAnimation(Whirlpool->aniFrames, 3, &entity->animator, true, (i & 3) + 3);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        offset += entity->size.y >> 7;
    }
}

void Whirlpool_SetupBubbles(void)
{
    RSDK_THIS(Whirlpool);

    for (int i = 0; i < 0x80; ++i) {
        int rand1           = RSDK.Rand(entity->hitbox.left, entity->hitbox.right);
        int rand2           = RSDK.Rand(0, 1024);
        entity->field_78[i] = (rand1 << 16) | rand2;
    }
}

void Whirlpool_EditorDraw(void) {}

void Whirlpool_EditorLoad(void) {}

void Whirlpool_Serialize(void)
{
    RSDK_EDITABLE_VAR(Whirlpool, VAR_ENUM, angVel);
    RSDK_EDITABLE_VAR(Whirlpool, VAR_ENUM, yVel);
    RSDK_EDITABLE_VAR(Whirlpool, VAR_VECTOR2, size);
}
