#include "../SonicMania.h"

ObjectForceSpin *ForceSpin;

void ForceSpin_Update(void)
{
    RSDK_THIS(ForceSpin);
    foreach_active(Player, player)
    {
        int x     = (player->position.x - entity->position.x) >> 8;
        int y     = (player->position.y - entity->position.y) >> 8;
        int scanX = (y * RSDK.Sin256(entity->negAngle)) + (x * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        int scanY = (y * RSDK.Cos256(entity->negAngle)) - (x * RSDK.Sin256(entity->negAngle)) + entity->position.y;
        int pos   = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle);
        RSDK.Cos256(entity->negAngle);
        RSDK.Sin256(entity->negAngle);

        int xDif = abs(scanX - entity->position.x);
        int yDif = abs(scanY - entity->position.y);

        if (xDif < 0x180000 && yDif < entity->size << 19) {
            if (scanX + pos >= entity->position.x) {
                if (entity->direction) {
                    if (player->state == Player_State_ForceRoll || player->state == Player_State_RollLock) {
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                    }
                }
                else {
                    ForceSpin_SetPlayerState(player);
                }
            }
            else {
                if (!entity->direction) {
                    if (player->state == Player_State_ForceRoll || player->state == Player_State_RollLock) {
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                    }
                }
                else {
                    ForceSpin_SetPlayerState(player);
                }
            }
        }
    }
    entity->visible = DebugMode->debugActive;
}

void ForceSpin_LateUpdate(void) {}

void ForceSpin_StaticUpdate(void) {}

void ForceSpin_Draw(void) { ForceSpin_DrawSprites(); }

void ForceSpin_Create(void *data)
{
    RSDK_THIS(ForceSpin);
    RSDK.SetSpriteAnimation(ForceSpin->spriteIndex, 0, &entity->animator, true, 0);
    entity->drawFX |= FX_FLIP;
    entity->animator.frameID = 4;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_BOUNDS;

        int x = 0;
        if (entity->size * RSDK.Sin256(entity->angle) << 11 >= 0)
            x = entity->size * RSDK.Sin256(entity->angle) << 11;
        else
            x = -0x800 * entity->size * RSDK.Sin256(entity->angle);
        entity->updateRange.x = x + 0x200000;

        int y = 0;
        if (entity->size * RSDK.Cos256(entity->angle) << 11 >= 0)
            y = entity->size * RSDK.Cos256(entity->angle) << 11;
        else
            y = -0x800 * entity->size * RSDK.Cos256(entity->angle);
        entity->visible       = false;
        entity->updateRange.y = y + 0x200000;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->negAngle      = (byte) - (byte)entity->angle;
    }
}

void ForceSpin_StageLoad(void) { ForceSpin->spriteIndex = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void ForceSpin_DrawSprites(void)
{
    Vector2 drawPos;

    RSDK_THIS(ForceSpin);
    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3((Entity *)entity, &drawPos, entity->angle);

    for (int i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }
}
void ForceSpin_SetPlayerState(void *plr)
{
    RSDK_THIS(ForceSpin);
    EntityPlayer *player = (EntityPlayer *)plr;
    if (player->state != Player_State_ForceRoll && player->state != Player_State_RollLock) {
        if (player->playerAnimator.animationID != ANI_JUMP) {
            RSDK.PlaySFX(Player->sfx_Roll, 0, 255);
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, 0, 0);
            if (!player->collisionMode && player->onGround)
                player->position.y += player->cameraOffset;
            player->pushing = 0;
        }

        player->nextAirState = StateMachine_None;
        if (player->onGround)
            player->state = Player_State_ForceRoll;
        else
            player->state = Player_State_RollLock;

        player->nextGroundState = StateMachine_None;
        if (abs(player->groundVel) < 0x10000) {
            if (entity->direction & FLIP_X)
                player->groundVel = -0x40000;
            else
                player->groundVel = 0x40000;
        }
    }
}

void ForceSpin_EditorDraw(void) {}

void ForceSpin_EditorLoad(void) {}

void ForceSpin_Serialize(void)
{
    RSDK_EDITABLE_VAR(ForceSpin, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_INT32, angle);
}
