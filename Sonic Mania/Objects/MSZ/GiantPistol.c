#include "SonicMania.h"

ObjectGiantPistol *GiantPistol;

void GiantPistol_Update(void)
{
    RSDK_THIS(GiantPistol);
    StateMachine_Run(entity->state);
}

void GiantPistol_LateUpdate(void) {}

void GiantPistol_StaticUpdate(void) {}

void GiantPistol_Draw(void)
{
    RSDK_THIS(GiantPistol);
    if (RSDK_sceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator5, NULL, false);
        RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK_sceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&entity->animator7, NULL, false);
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        RSDK.DrawSprite(&entity->animator6, NULL, false);
        RSDK.DrawSprite(&entity->animator8, NULL, false);
    }
}

void GiantPistol_Create(void *data)
{
    RSDK_THIS(GiantPistol);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 6, &entity->animator4, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 4, &entity->animator7, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &entity->animator5, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &entity->animator6, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 3, &entity->animator8, true, 0);
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->startPos      = entity->position;
        if (entity->direction == FLIP_NONE)
            entity->field_74.x = entity->position.x + 0x300000;
        else
            entity->field_74.x = entity->position.x - 0x300000;
        entity->field_74.y = entity->position.y + 0x180000;
        entity->state      = GiantPistol_State_WaitForPlayer;
    }
}

void GiantPistol_StageLoad(void)
{
    GiantPistol->aniFrames      = RSDK.LoadSpriteAnimation("MSZ/Pistol.bin", SCOPE_STAGE);
    GiantPistol->hitbox1.left   = -80;
    GiantPistol->hitbox1.top    = -58;
    GiantPistol->hitbox1.right  = 4;
    GiantPistol->hitbox1.bottom = -16;
    GiantPistol->hitbox2.left   = 4;
    GiantPistol->hitbox2.top    = -26;
    GiantPistol->hitbox2.right  = 44;
    GiantPistol->hitbox2.bottom = -16;
    GiantPistol->hitbox3.left   = 44;
    GiantPistol->hitbox3.top    = -58;
    GiantPistol->hitbox3.right  = 64;
    GiantPistol->hitbox3.bottom = -16;
    GiantPistol->sfxClack       = RSDK.GetSFX("Stage/Clack.wav");
    GiantPistol->sfxCannonFire  = RSDK.GetSFX("Stage/CannonFire.wav");
}

void GiantPistol_State_WaitForPlayer(void)
{
    RSDK_THIS(GiantPistol);

    foreach_active(Player, player)
    {
        int side = Player_CheckCollisionBox(player, entity, &GiantPistol->hitbox2);
        if (side == C_TOP && (!player->sidekick || entity->state == GiantPistol_State_Unknown2)) {
            player->state           = Player_State_None;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
            entity->activePlayers |= (1 << player->playerID);
            entity->state = GiantPistol_State_Unknown2;
            if (entity->direction == FLIP_NONE)
                entity->field_64 = 0x180000;
            else
                entity->field_64 = -0x180000;
            RSDK.PlaySfx(GiantPistol->sfxClack, false, 255);
        }
        Player_CheckCollisionBox(player, entity, &GiantPistol->hitbox1);
        Player_CheckCollisionBox(player, entity, &GiantPistol->hitbox3);
    }
}

void GiantPistol_State_Unknown2(void)
{
    RSDK_THIS(GiantPistol);
    RSDK.ProcessAnimation(&entity->animator3);

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & entity->activePlayers) > 0) {
            player->position.x += (entity->position.x + entity->field_64 - player->position.x) >> 3;
            player->position.y += (entity->position.y - player->position.y - 0x200000) >> 3;
            player->velocity.x = 0;
            player->velocity.y = 0;
        }
    }

    if (entity->animator3.frameID >= entity->animator3.frameCount - 1) {
        entity->state             = GiantPistol_State_Unknown3;
        entity->drawFX            = FX_ROTATE | FX_FLIP;
        entity->animator5.frameID = 1;
        entity->animator6.frameID = 1;
        entity->velocity.y        = -0x20000;
        entity->timer             = 6;

        foreach_active(Player, player)
        {
            if (((1 << (player->playerID)) & entity->activePlayers) > 0) {
                player->blinkTimer = 0;
                player->visible    = false;
                if (player->camera)
                    player->camera->state = StateMachine_None;
            }
        }
    }
}

void GiantPistol_State_Unknown3(void)
{
    RSDK_THIS(GiantPistol);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    if (entity->position.y >= entity->startPos.y) {
        entity->position.y = entity->startPos.y;
        entity->velocity.y = 0;
    }

    if (entity->timer > -64)
        entity->timer--;

    if (entity->direction)
        entity->angle -= entity->timer;
    else
        entity->angle += entity->timer;

    entity->rotation = entity->angle & 0x1FF;
    int angle        = 256 - (entity->rotation >> 1);

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & entity->activePlayers) > 0) {
            player->position.x = entity->position.x + entity->field_64;
            player->position.y = entity->position.y - 0x200000;
            int distX          = (player->position.x - entity->position.x) >> 8;
            int distY          = (player->position.y - entity->position.y) >> 8;
            player->position.x = entity->position.x + distY * RSDK.Sin256(angle) + distX * RSDK.Cos256(angle);
            player->position.y = (entity->position.y - distX * RSDK.Sin256(angle)) + distY * RSDK.Cos256(angle);
        }
    }

    if (entity->angle <= -0x1FC || entity->angle >= 0x1FC) {
        entity->angle    = 0;
        entity->timer    = -(entity->timer >> 1);
        entity->rotation = 0;
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 7, &entity->animator5, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 7, &entity->animator6, true, 1);
        entity->state  = GiantPistol_State_Aiming;
        entity->active = ACTIVE_NORMAL;

        foreach_active(Player, player)
        {
            if (((1 << (player->playerID)) & entity->activePlayers) > 0) {
                if (player->camera)
                    player->camera->state = Camera_State_Follow;
            }
        }
    }
}

void GiantPistol_State_Aiming(void)
{
    RSDK_THIS(GiantPistol);

    if (entity->timer2) {
        entity->timer2--;
    }
    else {
        if (entity->timer > 4)
            entity->timer--;

        if (entity->direction)
            entity->angle -= entity->timer;
        else
            entity->angle += entity->timer;
        entity->angle &= 0x1FF;
    }

    entity->position = entity->startPos;
    entity->rotation = (-RSDK.Sin512(entity->angle) >> 3) & 0x1FF;
    int angle        = 256 - (entity->rotation >> 1);
    Zone_Unknown3(&entity->field_74, &entity->position, angle);

    entity->position.x -= RSDK.Sin512(entity->rotation) << 10;
    entity->position.y -= RSDK.Sin512(entity->rotation) << 10;

    if (entity->direction) {
        if (entity->field_64 <= 0x500000)
            entity->field_64 += 0x40000;
    }
    else {
        if (entity->field_64 > -0x500000)
            entity->field_64 -= 0x40000;
    }

    uint8 flags = 0;
#if RETRO_USE_PLUS
    if (GiantPistol->flag) {
#else 
    if (!Zone->actID && !checkPlayerID(ID_KNUCKLES, 1)) {
#endif
        if (entity->angle == 118 && entity->activePlayers > 0) {
#if RETRO_USE_PLUS
            if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
                MSZSetup_Unknown3();
                MSZSetup_Unknown4(0);
            }
            else
#endif
                MSZSetup_Unknown4(1024);
            flags = 1;
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (((1 << (player->playerID)) & entity->activePlayers) > 0)
                flags |= player->jumpPress;
        }
    }

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & entity->activePlayers) > 0) {
            player->position.x = entity->position.x + entity->field_64;
            player->position.y = entity->position.y - 0x200000;
            int distX          = (player->position.x - entity->position.x) >> 8;
            int distY          = (player->position.y - entity->position.y) >> 8;

            player->position.x = entity->position.x + distY * RSDK.Sin256(angle) + distX * RSDK.Cos256(angle);
            player->position.y = entity->position.y - distX * RSDK.Sin256(angle) + distY * RSDK.Cos256(angle);
            if (flags == 1) {
                player->state            = Player_State_Air;
                player->onGround         = false;
                player->jumpAbilityTimer = 0;
                player->jumpAbility      = 0;
                player->jumpPress        = false;
                player->jumpHold         = false;

                if (entity->direction) {
                    player->velocity.x = 0xC00 * RSDK.Cos512(entity->rotation);
                    player->velocity.y = 0xC00 * RSDK.Sin512(entity->rotation);
                }
                else {
                    player->velocity.x = -0xC00 * RSDK.Cos512(entity->rotation);
                    player->velocity.y = -0xC00 * RSDK.Sin512(entity->rotation);
                }

#if RETRO_USE_PLUS
                if (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE && GiantPistol->flag) {
                    player->velocity.x += 0x18000;
                    player->state           = GiantPistol_Player_State_Unknown1;
                    player->nextGroundState = GiantPistol_Player_State_Unknown2;
                }
                else
#endif
                    if (!Zone->actID && !checkPlayerID(ID_KNUCKLES, 1)) {
                    player->jumpAbilityTimer = 0;
                }
                entity->activePlayers &= ~(1 << player->playerID);
                entity->timer2 = 16;

                player->velocity.x = clampVal(player->velocity.x, -0x120000, 0x120000);
                player->visible    = true;
                Camera_ShakeScreen(player->playerID, -8, -8);
                RSDK.PlaySfx(GiantPistol->sfxCannonFire, false, 255);
            }
        }
    }

    if (!entity->activePlayers && !entity->timer2) {
        if (entity->rotation > 256)
            entity->rotation -= 0x200;
        entity->state = GiantPistol_State_FiredPlayers;
    }
}

void GiantPistol_State_FiredPlayers(void)
{
    RSDK_THIS(GiantPistol);

    if (entity->rotation <= 0) {
        entity->rotation += 4;
        if (entity->rotation > 0)
            entity->rotation = 0;
    }
    else {
        entity->rotation -= 4;
        if (entity->rotation < 0)
            entity->rotation = 0;
    }

    if (entity->animator3.frameID > 0)
        entity->animator3.frameID--;

    ++entity->timer2;
    entity->position.x += (entity->startPos.x - entity->position.x) >> 3;
    entity->position.y += (entity->startPos.y - entity->position.y) >> 3;
    if (entity->timer2 == 60) {
        entity->position = entity->startPos;
        entity->timer2   = 0;
        entity->angle    = 0;
        entity->drawFX   = FX_FLIP;
        entity->active   = ACTIVE_BOUNDS;
        entity->state    = GiantPistol_State_WaitForPlayer;
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &entity->animator5, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &entity->animator6, true, 0);
    }
}

void GiantPistol_Player_State_Unknown1(void)
{
    RSDK_THIS(Player);
    entity->left      = false;
    entity->right     = true;
    entity->up        = false;
    entity->down      = false;
    entity->jumpPress = false;
    entity->jumpHold  = false;
    Player_State_Air();
    entity->nextGroundState = GiantPistol_Player_State_Unknown2;
}

void GiantPistol_Player_State_Unknown2(void)
{
    RSDK_THIS(Player);
    entity->left      = true;
    entity->right     = false;
    entity->up        = false;
    entity->down      = false;
    entity->jumpPress = false;
    entity->jumpHold  = false;
    Player_State_Ground();
    entity->state           = GiantPistol_Player_State_Unknown2;
    entity->nextGroundState = GiantPistol_Player_State_Unknown2;
    if (entity->groundVel <= 0) {
        RSDK.SetSpriteAnimation(entity->aniFrames, ANI_IDLE, &entity->playerAnimator, true, 0);
        entity->left            = false;
        entity->skidding        = 0;
        entity->groundVel       = 0;
        entity->velocity.x      = 0;
        entity->direction       = FLIP_NONE;
        entity->state           = Player_State_Ground;
        entity->nextGroundState = Player_State_Ground;
    }
}

void GiantPistol_EditorDraw(void)
{
    RSDK_THIS(GiantPistol);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &entity->animator2, true, 1);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 6, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 4, &entity->animator7, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &entity->animator5, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &entity->animator6, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 3, &entity->animator8, true, 0);

    RSDK.DrawSprite(&entity->animator7, NULL, false);
    RSDK.DrawSprite(&entity->animator4, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator6, NULL, false);
    RSDK.DrawSprite(&entity->animator8, NULL, false);
}

void GiantPistol_EditorLoad(void) { GiantPistol->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Pistol.bin", SCOPE_STAGE); }

void GiantPistol_Serialize(void) { RSDK_EDITABLE_VAR(GiantPistol, VAR_UINT8, direction); }
