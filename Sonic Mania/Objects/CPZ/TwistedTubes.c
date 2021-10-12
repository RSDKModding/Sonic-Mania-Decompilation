#include "SonicMania.h"

ObjectTwistedTubes *TwistedTubes;

void TwistedTubes_Update(void)
{
    RSDK_THIS(TwistedTubes);
    StateMachine_Run(entity->state);
}

void TwistedTubes_LateUpdate(void) {}

void TwistedTubes_StaticUpdate(void) {}

void TwistedTubes_Draw(void)
{
    RSDK_THIS(TwistedTubes);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.y += 0x400000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawPos.y += 0x200000;
    for (int h = 0; h < entity->height; ++h) {
        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        RSDK.DrawSprite(&entity->animator4, &drawPos, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);

        drawPos.y += 0x200000;
    }

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    RSDK.DrawSprite(&entity->animator5, &drawPos, false);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->animator5, &drawPos, false);

    entity->direction = FLIP_NONE;
}

void TwistedTubes_Create(void *data)
{
    RSDK_THIS(TwistedTubes);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        if (data) {
            entity->active    = ACTIVE_NORMAL;
            entity->playerPtr = (EntityPlayer *)data;
            if (entity->playerPtr->position.x > entity->position.x)
                entity->state = TwistedTubes_Unknown3;
            else
                entity->state = TwistedTubes_Unknown2;
        }
        else {
            entity->visible       = true;
            entity->drawOrder     = Zone->drawOrderHigh;
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.y = (entity->height + 4) << 21;
            entity->updateRange.x = 0x800000;
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &entity->animator2, true, 1);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &entity->animator3, true, 2);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &entity->animator4, true, 3);
            RSDK.SetSpriteAnimation(TwistedTubes->aniFrames, 0, &entity->animator5, true, 4);
            entity->state = TwistedTubes_HandleInteractions;
        }
    }
}

void TwistedTubes_StageLoad(void)
{
    TwistedTubes->aniFrames = RSDK.LoadSpriteAnimation("CPZ/TwistedTubes.bin", SCOPE_STAGE);

    TwistedTubes->hitbox1.left   = -32;
    TwistedTubes->hitbox1.top    = -16;
    TwistedTubes->hitbox1.right  = 32;
    TwistedTubes->hitbox1.bottom = 16;

    TwistedTubes->hitbox2.left   = -56;
    TwistedTubes->hitbox2.top    = 24;
    TwistedTubes->hitbox2.right  = -32;
    TwistedTubes->hitbox2.bottom = 64;

    TwistedTubes->hitbox3.left   = 32;
    TwistedTubes->hitbox3.top    = 24;
    TwistedTubes->hitbox3.right  = 56;
    TwistedTubes->hitbox3.bottom = 64;

    TwistedTubes->sfxTravel = RSDK.GetSFX("Tube/Travel.wav");
}

void TwistedTubes_HandleInteractions(void)
{
    RSDK_THIS(TwistedTubes);

    foreach_active(Player, player)
    {
        if (player->state != Player_Unknown7) {
            Player_CheckCollisionBox(player, entity, &TwistedTubes->hitbox1);
            int pID = RSDK.GetEntityID(player);

            bool32 flag = false;
            if (Player_CheckCollisionTouch(player, entity, &TwistedTubes->hitbox2)) {
                EntityTwistedTubes *tube = CREATE_ENTITY(TwistedTubes, player, entity->position.x, entity->position.y + 0x300000);
                tube->isPermanent        = true;
                tube->height             = entity->height + 2;
                flag                     = true;
            }
            else if (Player_CheckCollisionTouch(player, entity, &TwistedTubes->hitbox3)) {
                EntityTwistedTubes *tube = CREATE_ENTITY(TwistedTubes, player, entity->position.x, entity->position.y + 0x300000);
                tube->isPermanent        = true;
                tube->height             = entity->height + 3;
                flag                     = true;
            }

            if (flag) {
                player->state                  = Player_Unknown7;
                player->drawOrder              = 1;
                player->interaction            = false;
                player->tileCollisions         = false;
                player->onGround               = false;
                player->velocity.x             = 0;
                player->velocity.y             = 0;
                player->nextAirState           = StateMachine_None;
                player->nextGroundState        = StateMachine_None;
                TwistedTubes->playerFlags[pID] = true;
                RSDK.PlaySfx(TwistedTubes->sfxTravel, 0, 255);
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                player->playerAnimator.animationSpeed = 240;
            }
        }
    }
}

void TwistedTubes_Unknown2(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = entity->playerPtr;
    player->position.x += (entity->position.x - player->position.x - 0x380000) >> 3;
    player->position.y += (entity->position.y - player->position.y - 0x80000) >> 3;
    if (++entity->timer == 8) {
        entity->timer     = 0;
        entity->direction = FLIP_NONE;
        entity->state     = TwistedTubes_Unknown6;
    }
}

void TwistedTubes_Unknown3(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = entity->playerPtr;
    player->position.x += (entity->position.x - player->position.x + 0x380000) >> 3;
    player->position.y += (entity->position.y - player->position.y - 0x80000) >> 3;
    if (++entity->timer == 8) {
        entity->timer     = 0;
        entity->direction = FLIP_X;
        entity->state     = TwistedTubes_Unknown4;
    }
}

void TwistedTubes_Unknown4(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = entity->playerPtr;
    entity->angle += 8;
    player->position.x = entity->position.x + 0x3800 * RSDK.Cos256(entity->angle);
    player->position.y = entity->position.y - (RSDK.Cos256(entity->angle) << 12);
    player->velocity.x = -0x400000;
    player->velocity.y = 0x10000;
    if (entity->angle >= 128) {
        entity->position.y += 0x100000;
        entity->angle = 0;
        entity->state = TwistedTubes_Unknown5;
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        destroyEntity(entity);
    }
}

void TwistedTubes_Unknown5(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = entity->playerPtr;
    entity->angle += 8;
    player->position.x = entity->position.x - 0x3800 * RSDK.Cos256(entity->angle);
    player->position.y = entity->position.y - (RSDK.Cos256(entity->angle) << 9);
    player->velocity.x = 0x40000;
    player->velocity.y = 0x10000;

    if (entity->angle < 128) {
        if (entity->angle >= 64 && entity->direction == FLIP_X && entity->height == 1) {
            player->position.y                                  = entity->position.y;
            player->velocity.x                                  = 0xC0000;
            player->velocity.y                                  = 0;
            entity->state                                       = TwistedTubes_Unknown8;
            TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        }
    }
    else {
        entity->angle = 0;
        if (--entity->height) {
            entity->position.y += 0x100000;
            entity->state = TwistedTubes_Unknown4;
        }
        else {
            player->position.y = entity->position.y;
            player->velocity.y = 0;
            if (entity->direction == FLIP_X)
                entity->state = TwistedTubes_Unknown8;
            else
                entity->state = TwistedTubes_Unknown7;
            TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        }
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        destroyEntity(entity);
    }
}

void TwistedTubes_Unknown6(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = entity->playerPtr;
    entity->angle += 8;
    player->position.x = entity->position.x - 0x3800 * RSDK.Cos256(entity->angle);
    player->position.y = entity->position.y - (RSDK.Cos256(entity->angle) << 12);
    player->velocity.x = 0x40000;
    player->velocity.y = 0x10000;

    if (entity->angle >= 128) {
        entity->position.y += 0x200000;
        entity->angle = 0;
        entity->state = TwistedTubes_Unknown4;
    }

    if (!Player_CheckValidState(player)) {
        TwistedTubes->playerFlags[RSDK.GetEntityID(player)] = false;
        destroyEntity(entity);
    }
}

void TwistedTubes_Unknown7(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = entity->playerPtr;
    player->position.y   = entity->position.y;
    if (player->velocity.x <= -0x80000)
        destroyEntity(entity);
    else
        player->velocity.x -= 0x8000;

    if (!Player_CheckValidState(player))
        destroyEntity(entity);
}

void TwistedTubes_Unknown8(void)
{
    RSDK_THIS(TwistedTubes);
    EntityPlayer *player = entity->playerPtr;
    player->position.y   = RSDK_sceneInfo->entity->position.y;

    if (player->velocity.x >= 0x80000)
        destroyEntity(entity);
    else
        player->velocity.x += 0x8000;
    if (!Player_CheckValidState(player))
        destroyEntity(entity);
}

void TwistedTubes_EditorDraw(void) {}

void TwistedTubes_EditorLoad(void) {}

void TwistedTubes_Serialize(void) { RSDK_EDITABLE_VAR(TwistedTubes, VAR_UINT8, height); }
