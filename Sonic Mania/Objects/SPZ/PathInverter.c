#include "SonicMania.h"

ObjectPathInverter *PathInverter;

void PathInverter_Update(void)
{
    RSDK_THIS(PathInverter);
    StateMachine_Run(entity->state);
}

void PathInverter_LateUpdate(void) {}

void PathInverter_StaticUpdate(void) {}

void PathInverter_Draw(void)
{
    RSDK_THIS(PathInverter);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PathInverter_Create(void *data)
{
    RSDK_THIS(PathInverter);
    RSDK.SetSpriteAnimation(PathInverter->aniFrames, entity->type, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        Hitbox *hitbox        = RSDK.GetHitbox(&entity->animator, 0);
        entity->size.x        = hitbox->right;
        entity->size.y        = hitbox->bottom;
        entity->size2.x       = 2 * entity->size.x;
        entity->size2.y       = 2 * entity->size.y;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;

        switch (entity->type) {
            default: break;
            case PATHINVERTER_H: entity->state = PathInverter_State_Horizontal; break;
            case PATHINVERTER_V: entity->state = PathInverter_State_Vertical; break;
        }
    }
}

void PathInverter_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ2"))
        PathInverter->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/PathInverter.bin", SCOPE_STAGE);
}

void PathInverter_HandlePathSwitch(void *p)
{
    RSDK_THIS(PathInverter);

    EntityPlayer *player = (EntityPlayer *)p;
    if (!entity->playerPtrs[player->playerID]) {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if (player->position.y > entity->position.y == player->position.x > entity->position.x)
            entity->playerFlipFlags[player->playerID] = (entity->size.y + playerHitbox->bottom) << 7;
        else
            entity->playerFlipFlags[player->playerID] = -((entity->size.y << 7) + (playerHitbox->bottom << 7));
        if (player->collisionMode == CMODE_ROOF)
            player->direction ^= FLIP_X;
        entity->playerPtrs[player->playerID] = (Entity*)player;
        player->tileCollisions            = false;

        player->velocity.x                    = player->groundVel * RSDK.Cos256(player->angle) >> 8;
        player->velocity.y                    = player->groundVel * RSDK.Sin256(player->angle) >> 8;
        entity->groundVelStore[player->playerID] = player->groundVel;

        if (player->state == Player_State_Roll) {
            if (player->velocity.x >= -0xC0000) {
                if (player->velocity.x > 0xC0000)
                    player->velocity.x = 0xC0000;
            }
            else {
                player->velocity.x = -0xC0000;
            }
        }
        else if (player->velocity.x >= -0x80000) {
            if (player->velocity.x > 0x80000)
                player->velocity.x = 0x80000;
        }
        else {
            player->velocity.x = -0x80000;
        }
        player->state           = Player_State_None;
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;
    }

    int32 val    = entity->size.x + ((entity->position.x - player->position.x) >> 16);
    int32 angle2 = 4 * (3 * val) / entity->size2.x;
    int32 angle  = (val << 8) / entity->size2.x;
    if (player->playerAnimator.animationID != ANI_JUMP || !player->groundedStore) {
        int32 frame = 12 - angle2;
        if (player->collisionMode != CMODE_ROOF * (entity->playerFlipFlags[player->playerID] >= 0))
            frame = angle2;
        if (frame >= 0)
            frame %= 24;
        else
            frame += 24;
        if (player->collisionMode)
            player->direction &= ~FLIP_Y;
        else
            player->direction |= FLIP_Y;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->playerAnimator, true, frame);
    }
    player->onGround   = true;
    player->position.y = entity->playerFlipFlags[player->playerID] * RSDK.Cos512(angle) + entity->position.y;
}
void PathInverter_State_Horizontal(void)
{
    RSDK_THIS(PathInverter);
    foreach_active(Player, player)
    {
        if (player->collisionMode == CMODE_FLOOR || player->collisionMode == CMODE_ROOF) {
            if (abs(entity->position.x - player->position.x) >> 16 >= entity->size.x
                || abs(entity->position.y - player->position.y) >> 16 >= entity->size.y + 32) {
                int32 playerID = player->playerID;
                if (entity->playerPtrs[playerID]) {
                    entity->playerPtrs[playerID] = 0;
                    player->groundVel       = entity->groundVelStore[player->playerID];
                    player->groundVel       = -player->groundVel;
                    player->velocity.x      = -player->velocity.x;
                    if (player->collisionMode == CMODE_ROOF) {
                        player->direction ^= FLIP_X;
                    }
                    player->collisionMode = (player->collisionMode - 2) & 3;
                    player->angle = (player->angle + 0x80) & 0xFF;
                    player->controlLock = 30;
                    if (player->playerAnimator.animationID == ANI_JUMP) {
                        player->state = Player_State_Roll;
                    }
                    else {
                        player->direction &= FLIP_X;
                        player->direction ^= FLIP_X;
                        player->state = Player_State_Ground;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JOG, &player->playerAnimator, false, 0);
                        player->rotation = player->angle << 1;
                    }
                    player->tileCollisions = true;
                }
            }
            else if (player->state == Player_State_None) {
                PathInverter_HandlePathSwitch(player);
            }
            else {
                if (!player->onGround) {
                    if (entity->playerPtrs[player->playerID])
                        PathInverter_HandlePathSwitch(player);
                }
                else {
                    if (abs(player->groundVel) < 0x40000) {
                        if (entity->playerPtrs[player->playerID])
                            PathInverter_HandlePathSwitch(player);
                    }
                    else if (player->velocity.x <= 0) {
                        if (player->position.x <= entity->position.x + 0x80000)
                            entity->playerPtrs[player->playerID] = 0;
                        else
                            PathInverter_HandlePathSwitch(player);
                    }
                    else {
                        if (player->position.x >= entity->position.x - 0x80000)
                            entity->playerPtrs[player->playerID] = 0;
                        else
                            PathInverter_HandlePathSwitch(player);
                    }
                }
            }
        }
    }
}
void PathInverter_State_Vertical(void)
{
    // Nothing !
}

#if RETRO_INCLUDE_EDITOR
void PathInverter_EditorDraw(void)
{
    RSDK_THIS(PathInverter);
    RSDK.SetSpriteAnimation(PathInverter->aniFrames, entity->type, &entity->animator, true, 0);

    PathInverter_Draw();
}

void PathInverter_EditorLoad(void)
{
    PathInverter->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/PathInverter.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PathInverter, type);
    RSDK_ENUM_VAR("Horizontal", PATHINVERTER_H);
    RSDK_ENUM_VAR("Vertical", PATHINVERTER_V);
}
#endif

void PathInverter_Serialize(void) { RSDK_EDITABLE_VAR(PathInverter, VAR_UINT8, type); }
