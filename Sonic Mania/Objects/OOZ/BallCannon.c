#include "SonicMania.h"

ObjectBallCannon *BallCannon;

void BallCannon_Update(void)
{
    RSDK_THIS(BallCannon);
    StateMachine_Run(entity->state);
}

void BallCannon_LateUpdate(void) {}

void BallCannon_StaticUpdate(void) {}

void BallCannon_Draw(void)
{
    RSDK_THIS(BallCannon);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void BallCannon_Create(void *data)
{
    RSDK_THIS(BallCannon);
    entity->drawOrder = Zone->playerDrawLow;
    entity->visible   = true;
    if (!RSDK_sceneInfo->inEditor) {
        if (data) {
            entity->visible       = true;
            entity->drawFX        = FX_ROTATE;
            entity->active        = ACTIVE_NORMAL;
            entity->rotationSpeed = RSDK.Rand(-8, 8);
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &entity->animator, true, voidToInt(data));
            entity->state = BallCannon_Unknown8;
        }
        else {
            entity->drawFX        = FX_ROTATE | FX_FLIP;
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;

            switch (entity->type) {
                case 0:
                    if (entity->angle >= 4)
                        entity->direction = FLIP_X;
                    entity->rotation = (entity->angle + entity->direction + 1) << 7;
                    switch (entity->angle) {
                        case 0:
                        case 5: entity->velocity.y = 0x100000; break;
                        case 1:
                        case 6: entity->velocity.x = -0x100000; break;
                        case 2:
                        case 7: entity->velocity.y = -0x100000; break;
                        case 3:
                        case 4: entity->velocity.x = 0x100000; break;
                        default: break;
                    }
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &entity->animator, true, 0);
                    entity->state = BallCannon_Unknown2;
                    break;
                case 1:
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &entity->animator, true, 0);
                    entity->velocity.y = -0x80000;
                    entity->state      = BallCannon_StateCheckPlayerCollisions;
                    break;
                case 2:
                    RSDK.SetSpriteAnimation(BallCannon->aniFrames, 4, &entity->animator, true, 0);
                    entity->velocity.x = 0x80000;
                    entity->state      = BallCannon_StateCheckPlayerCollisions;
                    break;
            }
        }
    }
}

void BallCannon_StageLoad(void)
{
    BallCannon->aniFrames      = RSDK.LoadSpriteAnimation("OOZ/BallCannon.bin", SCOPE_STAGE);
    BallCannon->hitbox1.top    = -4;
    BallCannon->hitbox1.left   = -4;
    BallCannon->hitbox1.right  = 4;
    BallCannon->hitbox1.bottom = 4;
    BallCannon->hitbox2.top    = -16;
    BallCannon->hitbox2.left   = -16;
    BallCannon->hitbox2.right  = 16;
    BallCannon->hitbox2.bottom = 16;
    BallCannon->hitbox3.top    = -4;
    BallCannon->hitbox3.left   = -8;
    BallCannon->hitbox3.right  = 8;
    BallCannon->hitbox3.bottom = 4;
    BallCannon->sfxLedgeBreak  = RSDK.GetSFX("Stage/LedgeBreak.wav");
    BallCannon->sfxFire        = RSDK.GetSFX("Stage/CannonFire.wav");
}

void BallCannon_Unknown1(void)
{
    RSDK_THIS(BallCannon);
    if (RSDK.CheckOnScreen(entity, NULL)) {
        foreach_all(Player, player)
        {
            if (Player_CheckValidState(player)) {
                int32 playerID = RSDK.GetEntityID(player);

                if (entity->playerTimers[playerID]) {
                    entity->playerTimers[playerID]--;
                }
                else {
                    if ((1 << playerID) & entity->activePlayers) {
                        if (player->state != Player_State_None)
                            entity->activePlayers &= ~(1 << playerID);
                    }
                    else {
                        if (Player_CheckCollisionTouch(player, entity, &BallCannon->hitbox1)) {
                            RSDK.PlaySfx(Player->sfxRoll, false, 255);
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                            player->position.x     = entity->position.x;
                            player->position.y     = entity->position.y;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            player->tileCollisions = false;
                            player->interaction    = false;
                            player->blinkTimer     = 0;
                            player->visible        = false;
                            player->state          = Player_State_None;
                            entity->activePlayers |= 1 << playerID;
                            entity->active   = ACTIVE_NORMAL;
                            entity->rotation = (entity->direction + 1 + entity->angle) << 7;
                            entity->drawFX   = FX_ROTATE;
                            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &entity->animator, true, 0);
                            entity->state = BallCannon_Unknown3;
                        }
                    }
                }
            }
        }
    }
    else {
        entity->active   = ACTIVE_BOUNDS;
        entity->rotation = (entity->direction + entity->angle + 1) << 7;
    }
}

void BallCannon_Unknown2(void) { BallCannon_Unknown1(); }

void BallCannon_Unknown3(void)
{
    RSDK_THIS(BallCannon);
    BallCannon_Unknown1();
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(BallCannon->aniFrames, 1, &entity->animator, true, 0);
        entity->drawFX = FX_FLIP;
        entity->state  = BallCannon_Unknown4;
    }
}

void BallCannon_Unknown4(void)
{
    RSDK_THIS(BallCannon);
    BallCannon_Unknown1();
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(BallCannon->aniFrames, 2, &entity->animator, true, 0);
        entity->drawFX   = FX_ROTATE;
        entity->state    = BallCannon_Unknown5;
        entity->rotation = (entity->angle - entity->direction + 2) << 7;
    }
}

void BallCannon_Unknown5(void)
{
    RSDK_THIS(BallCannon);
    BallCannon_Unknown1();
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        foreach_all(Player, player)
        {
            if (Player_CheckValidState(player)) {
                int32 playerID = RSDK.GetEntityID(player);

                if (((1 << playerID) & entity->activePlayers)) {
                    RSDK.PlaySfx(BallCannon->sfxFire, false, 255);
                    player->velocity = entity->velocity;
                    player->visible  = true;
                    if (entity->exit) {
                        player->onGround       = false;
                        player->jumpAbility    = 0;
                        player->state          = Player_State_Air;
                        player->tileCollisions = true;
                        player->interaction    = true;
                    }
                    entity->activePlayers &= ~(1 << playerID);
                    entity->playerTimers[playerID] = 15;
                }
            }
        }
        entity->state = BallCannon_Unknown2;
    }
}

void BallCannon_StateCheckPlayerCollisions(void)
{
    RSDK_THIS(BallCannon);

    foreach_active(Player, player)
    {
        Animator *animator = &player->playerAnimator;
        int32 storeVelY      = player->velocity.y;
        int32 storeVelX      = player->velocity.x;
        int32 storeX         = player->position.x;
        int32 storeY         = player->position.y;

        if (Player_CheckCollisionBox(player, entity, &BallCannon->hitbox2) == 1) {
            if (player->playerAnimator.animationID == ANI_JUMP || player->state == Player_State_DropDash
#if RETRO_USE_PLUS
                || player->state == Player_State_MightyHammerDrop
#endif
                ) {
                if (storeVelY >= 0 && !player->groundedStore) {
                    for (int32 i = 0; i < 16; ++i) {
                        EntityBallCannon *debris =
                            CREATE_ENTITY(BallCannon, intToVoid((i & 3) + 1), entity->position.x + BallCannon->array1[(i * 2) + 0],
                                          entity->position.y + BallCannon->array2[(i * 2) + 1]);
                        debris->velocity.x = BallCannon->array2[(i * 2) + 0];
                        debris->velocity.y = BallCannon->array2[(i * 2) + 1];
                    }

                    RSDK.PlaySfx(BallCannon->sfxLedgeBreak, false, 255);
                    memcpy(&player->playerAnimator, animator, sizeof(Animator));
                    player->velocity.x = storeVelX;
                    player->velocity.y = storeVelY;
                    player->position.x = storeX;
                    player->position.y = storeY;
                    player->onGround   = false;
                    entity->active     = ACTIVE_NORMAL;
                    entity->visible    = false;
                    entity->state      = BallCannon_Unknown7;
                    foreach_break;
                }
            }
        }
    }
}

void BallCannon_Unknown7(void)
{
    RSDK_THIS(BallCannon);

    if (RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL)) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);

            if (entity->playerTimers[playerID]) {
                entity->playerTimers[playerID]--;
            }
            else {
                if (Player_CheckCollisionTouch(player, entity, &BallCannon->hitbox3)) {
                    RSDK.SetSpriteAnimation(player->aniFrames, 10, &player->playerAnimator, false, 0);
                    RSDK.PlaySfx(BallCannon->sfxFire, false, 255);
                    player->state                  = Player_State_None;
                    player->nextGroundState        = 0;
                    player->nextAirState           = 0;
                    player->position.x             = entity->position.x;
                    player->position.y             = entity->position.y;
                    player->velocity               = entity->velocity;
                    player->tileCollisions         = false;
                    player->interaction            = false;
                    player->onGround               = false;
                    entity->playerTimers[playerID] = 15;
                }
            }
        }
    }
    else {
        entity->visible = true;
        for (int32 i = 0; i < Player->playerCount; ++i) {
            entity->playerTimers[i] = 0;
        }
        entity->state = BallCannon_StateCheckPlayerCollisions;
    }
}

void BallCannon_Unknown8(void)
{
    RSDK_THIS(BallCannon);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->rotation += entity->rotationSpeed;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void BallCannon_EditorDraw(void)
{
    RSDK_THIS(BallCannon);

    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;

    switch (entity->type) {
        case 0:
            if (entity->angle >= 4)
                entity->direction = FLIP_X;
            entity->rotation = (entity->angle + entity->direction + 1) << 7;
            switch (entity->angle) {
                case 0:
                case 5: entity->velocity.y = 0x100000; break;
                case 1:
                case 6: entity->velocity.x = -0x100000; break;
                case 2:
                case 7: entity->velocity.y = -0x100000; break;
                case 3:
                case 4: entity->velocity.x = 0x100000; break;
                default: break;
            }
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 0, &entity->animator, true, 0);
            break;
        case 1:
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 3, &entity->animator, true, 0);
            break;
        case 2:
            RSDK.SetSpriteAnimation(BallCannon->aniFrames, 4, &entity->animator, true, 0);
            break;
    }

    BallCannon_Draw();
}

void BallCannon_EditorLoad(void) { BallCannon->aniFrames = RSDK.LoadSpriteAnimation("OOZ/BallCannon.bin", SCOPE_STAGE); }
#endif

void BallCannon_Serialize(void)
{
    RSDK_EDITABLE_VAR(BallCannon, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BallCannon, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(BallCannon, VAR_BOOL, exit);
}
