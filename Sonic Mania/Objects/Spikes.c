#include "../SonicMania.h"

ObjectSpikes *Spikes;

void Spikes_Update(void)
{
    RSDK_THIS(Spikes);
    switch (entity->dword6C) {
        case 1:
            if (entity->stagger << 6 == (Zone->timer & 0x40)) {
                if ((Zone->timer & 0x3F) == entity->timer) {
                    entity->dword6C++;
                    if (entity->activeScreens == 1)
                        RSDK.PlaySFX(Spikes->sfx_Move, 0, 255);
                }
            }
            break;
        case 2:
            if (entity->dword70 >= 0x200000) {
                entity->dword6C++;
            }
            else {
                entity->dword70 += 0x80000;
                entity->position.x += entity->velocity.x;
                entity->position.y += entity->velocity.y;
            }
            break;
        case 3:
            if ((Zone->timer & 0x3F) == entity->timer) {
                entity->dword6C++;
                if (entity->activeScreens == 1)
                    RSDK.PlaySFX(Spikes->sfx_Move, 0, 255);
            }
            break;
        case 4:
            if (entity->dword70 <= 0) {
                entity->dword6C = 1;
            }
            else {
                entity->dword70 -= 0x80000;
                entity->position.x -= entity->velocity.x;
                entity->position.y -= entity->velocity.y;
            }
            break;
        case 5:
            if (entity->dword70 >= 0x280000) {
                entity->dword6C = 6;
            }
            else {
                entity->dword70 += 0x80000;
                entity->position.x -= entity->velocity.x;
                entity->position.y -= entity->velocity.y;
            }
            break;
        default: break;
    }

    entity->position.x -= entity->offset.x;
    entity->position.y -= entity->offset.y;
    if (entity->dword6C != 1) {
        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == (((byte)entity->planeFilter - 1) & 1)) {
                EntityShield *shield = (EntityShield *)RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player));
                if ((/*(!Ice || player->state != Ice_State_FrozenPlayer) &&*/ shield->data.animationID != 2) || Press) {
                    int playerVelX = player->velocity.x;
                    int playerVelY = player->velocity.y;

                    byte side = Player_CheckCollisionBox(player, entity, &entity->hitbox);
                    if (side) {
                        if (Ice && player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !Press && !entity->field_7E) {
                            // Ice_Unknown6(16, 0, 0, 0);
                            entity->field_7E = 15;
                        }

                        bool32 flag = false;
                        if (side == 4) {
                            player->collisionFlagV |= 2;
                            flag = 0;
                        }
                        else {
                            if (side == 1) {
                                player->collisionFlagV |= 1u;
                                if (entity->dword70 == 0x80000)
                                    player->onGround = 0;
                            }
                            flag = 0;
#if RETRO_USE_PLUS
                            if (side == 1 && player->state == Player_State_MightyHammerDrop) {
                                if (Ice) {
                                    if (!Press) {
                                        flag = 1;
                                        if (entity->type != 1) {
                                            player->onGround   = 0;
                                            player->velocity.y = playerVelY;
                                        }
                                    }
                                }
                            }
#endif
                        }
                        switch (side) {
                            case 1:
                                player->collisionFlagV |= 1;
                                if (player->velocity.y >= 0 || entity->dword6C == 2) {
                                    player->position.x += entity->offset.x;
                                    player->position.y += entity->offset.y;
                                    if (side == entity->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            case 2:
                                player->collisionFlagH |= 1;
                                if (player->velocity.x >= 0) {
                                    if (entity->dword6C == 2) {
                                        if (side == entity->type)
                                            Spikes_CheckHit(player, playerVelX, playerVelY);
                                    }
                                }
                                else {
                                    if (side == entity->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            case 3:
                                player->collisionFlagH |= 2;
                                if (player->velocity.x <= 0) {
                                    if (side == entity->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                if (entity->dword6C == 2) {
                                    if (side == entity->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            case 4:
                                player->collisionFlagV |= 2;
                                if (player->velocity.y <= 0) {
                                    if (side == entity->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                if (entity->dword6C == 2) {
                                    if (side == entity->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            default: break;
                        }

                        if (flag) {
                            // RSDK.PlaySFX(Ice->sfx_WindowShatter, 0, 255);
                            // Ice_Unknown6(16, 0, 0, 0);
                            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
                        }
                    }
                }
                else {
                    int posStoreX = player->position.x;
                    int posStoreY = player->position.y;

                    byte side = 0;
                    // if (player->state == Ice_State_FrozenPlayer) {
                    //    side = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, &Ice->hitbox2, 0);
                    //}
                    // else {
                    side = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox, player, Player_GetHitbox(player), 0);
                    //}

                    switch (side) {
                        case 1:
                            if (player->velocity.y < 0x40000) {
                                if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
                                    player->position.x += entity->offset.x;
                                    player->position.y += entity->offset.y;
                                }
                            }
                            else {
                                Spikes_Unknown1(0, 0);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                            break;
                        case 2:
                            if (player->velocity.x >= 0x20000) {
                                Spikes_Unknown1(player->velocity.x, 0);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                            else {
                                if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
                                    player->position.x += entity->offset.x;
                                    player->position.y += entity->offset.y;
                                }
                            }
                            break;
                        case 3:
                            if (player->velocity.x > -0x20000) {
                                if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
                                    player->position.x += entity->offset.x;
                                    player->position.y += entity->offset.y;
                                }
                            }
                            else {
                                Spikes_Unknown1(player->velocity.x, 0);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                            break;
                        case 4:
                            if (player->velocity.y > -0x40000) {
                                if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
                                    player->position.x += entity->offset.x;
                                    player->position.y += entity->offset.y;
                                }
                            }
                            else {
                                Spikes_Unknown1(0, player->velocity.y);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                        default:
                            if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
                                player->position.x += entity->offset.x;
                                player->position.y += entity->offset.y;
                            }
                            break;
                    }
                }
            }
        }
    }

    entity->position.x += entity->offset.x;
    entity->position.y += entity->offset.y;
    if (Ice && !Press) {
        RSDK.ProcessAnimation(&entity->animData);
        if (--entity->timer2 <= 0) {
            switch (entity->type) {
                case 1:
                case 4: RSDK.SetSpriteAnimation(Spikes->spriteIndex, 2, &entity->animData, true, 0); break;
                case 2:
                case 3: RSDK.SetSpriteAnimation(Spikes->spriteIndex, 3, &entity->animData, true, 0); break;
                default: break;
            }
            entity->timer2 = RSDK.Rand(0, 240) + 30;
        }
        if (entity->field_7E > 0) {
            entity->field_7E--;
            if (!entity->field_7E)
                Spikes_Unknown1(0, 0);
        }
    }
}

void Spikes_LateUpdate(void) {}

void Spikes_StaticUpdate(void) {}

void Spikes_Draw(void)
{
    if (Spikes->stateDraw)
        Spikes->stateDraw();
}

void Spikes_Create(void *data)
{
    RSDK_THIS(Spikes);
    entity->drawFX = FX_FLIP;
    if (entity->count < 2)
        entity->count = 2;
    if (!RSDK_sceneInfo->inEditor) {
        if (data)
            entity->type = voidToInt(data);

        entity->active  = ACTIVE_BOUNDS;
        int dir         = entity->type & 1;
        entity->visible = true;
        entity->type    = (entity->type >> 1) & 1;
        if (entity->planeFilter > 0 && ((byte)entity->planeFilter - 1) & 2)
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;
        entity->alpha = 128;
        if (entity->type) {
            if (entity->type == 1) {
                entity->updateRange.x = 0x600000;
                entity->updateRange.y = (entity->count + 6) << 20;
                entity->direction     = dir;
                if (entity->direction) {
                    entity->velocity.x = -0x80000;
                    entity->type       = 2;
                }
                else {
                    entity->velocity.x = 0x80000;
                    entity->type       = 3;
                }
                entity->hitbox.left   = -16;
                entity->hitbox.top    = -8 * entity->count;
                entity->hitbox.right  = 16;
                entity->hitbox.bottom = 8 * entity->count;
            }
        }
        else {
            entity->updateRange.x = 0x600000;
            entity->updateRange.y = (entity->count + 6) << 20;
            entity->direction     = FLIP_Y * dir;
            if (entity->direction) {
                entity->velocity.y = 0x80000;
                entity->type       = 4;
            }
            else {
                entity->velocity.y = -0x80000;
                entity->type       = 1;
            }
            entity->hitbox.left   = -8 * entity->count;
            entity->hitbox.top    = -16;
            entity->hitbox.right  = 8 * entity->count;
            entity->hitbox.bottom = 16;
        }

        if (entity->moving) {
            entity->position.x -= 4 * entity->velocity.x;
            entity->position.y -= 4 * entity->velocity.y;
            entity->dword6C = 1;
        }
    }
}

void Spikes_StageLoad(void)
{
    Spikes->stateDraw = Spikes_StateDraw_Stage;
    if (RSDK.CheckStageFolder("FBZ")) {
        Spikes->spriteIndex = RSDK.LoadSpriteAnimation("FBZ/Spikes.bin", SCOPE_STAGE);
    }
    if (RSDK.CheckStageFolder("PSZ2")) {
        Spikes->spriteIndex = RSDK.LoadSpriteAnimation("PSZ2/Spikes.bin", SCOPE_STAGE);
    }
    else {
        Spikes->spriteIndex = RSDK.LoadSpriteAnimation("Global/Spikes.bin", SCOPE_STAGE);
        Spikes->stateDraw   = Spikes_StateDraw_Global;
    }
    RSDK.SetSpriteAnimation(Spikes->spriteIndex, 0, &Spikes->vData, true, 0);
    RSDK.SetSpriteAnimation(Spikes->spriteIndex, 1, &Spikes->hData, true, 0);
    Spikes->dword3C   = 0x100000;
    Spikes->dword40   = 0x100000;
    Spikes->sfx_Move  = RSDK.GetSFX("Global/SpikesMove.wav");
    Spikes->sfx_Spike = RSDK.GetSFX("Global/Spike.wav");
}

void Spikes_StateDraw_Global(void)
{
    Vector2 drawPos;

    RSDK_THIS(Spikes);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    int cnt   = entity->count >> 1;
    switch (entity->type) {
        case 1:
        case 4:
            drawPos.x = 0x100000 - (entity->count << 19) + entity->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->vData, &drawPos, 0);
                drawPos.x += 0x200000;
            }

            if (entity->count & 1) {
                drawPos.x -= 0x100000;
                RSDK.DrawSprite(&Spikes->vData, &drawPos, 0);
            }
            break;
        case 2:
        case 3:
            drawPos.y = 0x100000 - (entity->count << 19) + entity->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->hData, &drawPos, 0);
                drawPos.y += 0x200000;
            }

            if (entity->count & 1) {
                drawPos.y -= 0x100000;
                RSDK.DrawSprite(&Spikes->hData, &drawPos, 0);
            }
            break;
        default: break;
    }
    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animData, 0, 0);
    entity->inkEffect = INK_NONE;
}

void Spikes_StateDraw_Stage(void)
{
    Vector2 drawPos;

    RSDK_THIS(Spikes);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    int cnt   = entity->count >> 1;
    switch (entity->type) {
        case 1:
        case 4:
            drawPos.x = (0x100000 - (entity->count << 19)) + entity->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->vData, &drawPos, 0);
                drawPos.x += 0x200000;
            }
            if (entity->count & 1) {
                drawPos.x -= 0x100000;
                RSDK.DrawSprite(&Spikes->vData, &drawPos, 0);
            }
            break;
        case 2:
        case 3:
            drawPos.y = (0x100000 - (entity->count << 19)) + entity->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->hData, &drawPos, 0);
                drawPos.y += 0x200000;
            }
            if (entity->count & 1) {
                drawPos.y -= 0x100000;
                RSDK.DrawSprite(&Spikes->hData, &drawPos, 0);
            }
            break;
        default: break;
    }
}

void Spikes_Unknown1(int a1, int a2)
{
    RSDK_THIS(Spikes);
    // RSDK.PlaySFX(Ice->sfx_WindowShatter, 0, 255);
    // Ice_Unknown6(16, a1, a2, 0);
    RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
}
void Spikes_CheckHit(EntityPlayer *player, int playerVelX, int playerVelY)
{
    RSDK_THIS(Spikes);
    if (player->state == Player_State_Hit)
        return;
    if (!Player_CheckValidState(player) || player->invincibleTimer || player->blinkTimer > 0)
        return;

#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY
        && (player->playerAnimator.animationID == ANI_JUMP || player->playerAnimator.animationID == ANI_SPINDASH
            || player->playerAnimator.animationID == ANI_DROPDASH)) {

        if (abs(playerVelX) < 0x20000) {
            switch (entity->type) {
                case 1:
                    if (player->playerAnimator.animationID != ANI_DROPDASH || (Ice && !Press)) {
                        player->velocity.y = -0x48000;
                        if (!(player->direction & FLIP_X))
                            player->velocity.x = 0x48000;
                        else
                            player->velocity.x = -0x48000;
                        player->state = Player_State_Air;
                    }
                    else {
                        player->velocity.y = -0x48000;
                        if (!(player->direction & FLIP_X))
                            player->velocity.x = -0x28000;
                        else
                            player->velocity.x = 0x28000;
                        player->blinkTimer = 60;
                        player->state      = Player_State_Hit;
                        RSDK.StopSFX(Player->sfx_MightyDrill);
                    }

                    player->velocity.x -= player->velocity.x >> 2;
                    return;
                case 2:
                    player->velocity.y = -0x40000;
                    player->velocity.x = -0x28000;
                    player->state      = Player_State_Air;
                    return;
                case 3:
                    player->velocity.y = -0x40000;
                    player->velocity.x = 0x28000;
                    player->state      = Player_State_Air;
                    return;
                case 4:
                    player->velocity.y = 0x20000;
                    player->state      = Player_State_Air;
                    return;
                default: break;
            }
            if (entity->type >= 1 && entity->type <= 4) {
                player->onGround         = false;
                player->jumpAbility      = 0;
                player->jumpAbilityTimer = 0;
                if (player->state == Player_State_Hit) {
                    RSDK.SetSpriteAnimation(player->spriteIndex, 18, &player->playerAnimator, 0, 0);
                    RSDK.PlaySFX(Spikes->sfx_Spike, 0, 255);
                }
                else {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLY, &player->playerAnimator, 0, 0);
                    RSDK.PlaySFX(Player->sfx_MightyUnspin, 0, 255);
                }
            }

            if (player->playerAnimator.animationID != ANI_FLY)
                RSDK.PlaySFX(Player->sfx_PimPom, 0, 255);
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y >>= 1;
            }
        }
        else if (entity->type == 1) {
            if (player->playerAnimator.animationID == ANI_DROPDASH) {
                player->velocity.y = -0x48000;
                if ((player->direction & FLIP_X) == 0)
                    player->velocity.x = 0x48000;
                else
                    player->velocity.x = -0x48000;
                player->state = Player_State_Air;
                player->velocity.x -= player->velocity.x >> 2;
                player->onGround         = 0;
                player->jumpAbility      = 0;
                player->jumpAbilityTimer = 0;
                if (player->state == Player_State_Hit) {
                    RSDK.SetSpriteAnimation(player->spriteIndex, 18, &player->playerAnimator, 0, 0);
                    RSDK.PlaySFX(Spikes->sfx_Spike, 0, 255);
                }
                else {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLY, &player->playerAnimator, 0, 0);
                    RSDK.PlaySFX(Player->sfx_MightyUnspin, 0, 255);
                }
                if (player->playerAnimator.animationID != ANI_FLY)
                    RSDK.PlaySFX(Player->sfx_PimPom, 0, 255);
                if (player->underwater) {
                    player->velocity.x >>= 1;
                    player->velocity.y >>= 1;
                }
            }
            else if (playerVelY > 0x28000) {
                player->velocity.y       = -0x20000;
                player->state            = Player_State_Air;
                player->onGround         = 0;
                player->jumpAbility      = 0;
                player->jumpAbilityTimer = 0;
                RSDK.PlaySFX(Player->sfx_PimPom, 0, 255);

                player->groundedStore   = 1;
                player->state           = Player_State_Roll;
                player->nextGroundState = Player_State_Roll;
                player->nextAirState    = Player_State_Air;
            }
            else {
                player->groundedStore   = 1;
                player->state           = Player_State_Roll;
                player->nextGroundState = Player_State_Roll;
                player->nextAirState    = Player_State_Air;
            }
        }
        return; // dont do the code below
    }
#endif

    if (player->position.x > entity->position.x)
        player->velocity.x = 0x20000;
    else
        player->velocity.x = -0x20000;
    Player_Hit(player);
    if (player->hurtFlag == 1) {
        player->hurtFlag = 2;
        RSDK.PlaySFX(Spikes->sfx_Spike, 0, 255);
    }
    else if (player->state == Player_State_Hit && (player->shield || player->sidekick)) {
        RSDK.StopSFX(Player->sfx_Hurt);
        RSDK.PlaySFX(Spikes->sfx_Spike, 0, 255);
    }
}

void Spikes_EditorDraw(void) {}

void Spikes_EditorLoad(void) {}

void Spikes_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spikes, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Spikes, VAR_BOOL, moving);
    RSDK_EDITABLE_VAR(Spikes, VAR_UINT8, count);
    RSDK_EDITABLE_VAR(Spikes, VAR_UINT8, stagger);
    RSDK_EDITABLE_VAR(Spikes, VAR_UINT8, timer);
    RSDK_EDITABLE_VAR(Spikes, VAR_ENUM, planeFilter);
}
