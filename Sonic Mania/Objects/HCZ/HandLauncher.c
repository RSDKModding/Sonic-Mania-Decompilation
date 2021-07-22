#include "SonicMania.h"

ObjectHandLauncher *HandLauncher;

void HandLauncher_Update(void)
{
    RSDK_THIS(HandLauncher);

    if (entity->animator2.animationID == 2 && entity->animator2.frameID == entity->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(HandLauncher->spriteIndex, 3, &entity->animator2, true, 0);
    }
    else if (entity->animator2.animationID == 4 && entity->animator2.frameID == entity->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(HandLauncher->spriteIndex, 1, &entity->animator2, true, 0);
    }
    StateMachine_Run(entity->state);
    if (entity->field_80 > 0)
        entity->field_80--;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
}

void HandLauncher_LateUpdate(void) {}

void HandLauncher_StaticUpdate(void)
{
    foreach_active(HandLauncher, launcher) { RSDK.AddDrawListRef(Zone->playerDrawHigh, RSDK.GetEntityID(launcher)); }
}

void HandLauncher_Draw(void)
{
    RSDK_THIS(HandLauncher);
    if (RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawHigh) {
        if (entity->state == HandLauncher_Unknown8)
            RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        if (entity->state != HandLauncher_Unknown8)
            RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void HandLauncher_Create(void *data)
{
    RSDK_THIS(HandLauncher);

    entity->active    = ACTIVE_BOUNDS;
    entity->drawOrder = Zone->drawOrderLow;
    entity->visible   = true;
    entity->drawFX    = FX_FLIP;
    if (!entity->speed)
        entity->speed = 16;
    entity->startPos      = entity->position;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = HandLauncher_Unknown5;
}

void HandLauncher_StageLoad(void)
{
    HandLauncher->active         = ACTIVE_ALWAYS;
    HandLauncher->spriteIndex    = RSDK.LoadSpriteAnimation("HCZ/HandLauncher.bin", SCOPE_STAGE);
    HandLauncher->sfxDunkey      = RSDK.GetSFX("HCZ/Dunkey.wav");
    HandLauncher->hitbox1.left   = -24;
    HandLauncher->hitbox1.top    = -240;
    HandLauncher->hitbox1.right  = 24;
    HandLauncher->hitbox1.bottom = 0;
    HandLauncher->hitbox2.left   = -4;
    HandLauncher->hitbox2.top    = -54;
    HandLauncher->hitbox2.right  = 4;
    HandLauncher->hitbox2.bottom = 0;
    HandLauncher->hitbox3.left   = -32;
    HandLauncher->hitbox3.top    = 0;
    HandLauncher->hitbox3.right  = 32;
    HandLauncher->hitbox3.bottom = 24;
}

void HandLauncher_CheckPlayerCollisions(void)
{
    RSDK_THIS(HandLauncher);
    if (entity->field_80 <= 0) {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if ((entity->state == HandLauncher_Unknown8 || !player->sidekick) && !((1 << playerID) & entity->activePlayers)) {
                if (player->onGround) {
                    if (Player_CheckCollisionTouch(player, entity, &HandLauncher->hitbox2)) {
                        entity->activePlayers |= 1 << playerID;
                        player->groundVel = 0;
                        if (entity->state != HandLauncher_Unknown8) {
                            if (HandLauncher->dunkeyMode)
                                RSDK.PlaySFX(HandLauncher->sfxDunkey, 0, 255);
                            else
                                RSDK.PlaySFX(Player->sfx_Roll, 0, 255);
                        }
                        if (!HandLauncher->dunkeyMode)
                            RSDK.PlaySFX(Player->sfx_Grab, 0, 255);
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_RUN, &player->playerAnimator, false, 0);
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->rotation        = 0;
                        player->state           = Player_State_None;
                    }
                }
            }
        }
    }
}

bool32 HandLauncher_Unknown1(void)
{
    RSDK_THIS(HandLauncher);
    if (entity->activePlayers)
        return true;

    int storeX       = entity->position.x;
    int storeY       = entity->position.y;
    entity->position = entity->playerPos;
    bool32 flag      = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &HandLauncher->hitbox1)) {
            flag = true;
            foreach_break;
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
    return flag;
}

void HandLauncher_State_Release(void)
{
    RSDK_THIS(HandLauncher);
    int vel = entity->speed * (2 * (entity->direction == FLIP_NONE) - 1);
    foreach_active(Player, player)
    {
        int pID = RSDK.GetEntityID(player);
        if (((1 << pID) & entity->activePlayers)) {
            entity->activePlayers &= ~(1 << pID);
            player->onGround  = true;
            player->groundVel = vel << 16;
            player->state     = Player_State_Ground;
        }
    }
}

void HandLauncher_Unknown3(void)
{
    RSDK_THIS(HandLauncher);
    foreach_active(Player, player)
    {
        if (((1 << RSDK.GetEntityID(player)) & entity->activePlayers)) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->direction  = entity->direction != FLIP_NONE;
            player->position.x = entity->position.x;
            player->position.y = entity->position.y;
            player->position.y -= 0x140000;
            if (entity->playerPos.y - 0x140000 < player->position.y)
                player->position.y -= 0x140000;
        }
    }
}

void HandLauncher_Unknown5(void)
{
    RSDK_THIS(HandLauncher);
    entity->field_74 = entity->startPos;
    entity->field_74.y += 0x4E0000;
    entity->playerPos = entity->field_74;
    entity->playerPos.y -= 0x360000;
    entity->position.x = entity->field_74.x;
    entity->position.y = entity->field_74.y;
    RSDK.SetSpriteAnimation(HandLauncher->spriteIndex, 0, &entity->animator1, true, 0);
    entity->animator1.animationSpeed = 0;
    RSDK.SetSpriteAnimation(HandLauncher->spriteIndex, 1, &entity->animator2, true, 0);
    entity->state = HandLauncher_Unknown6;
}

void HandLauncher_Unknown6(void)
{
    RSDK_THIS(HandLauncher);
    entity->position = entity->field_74;
    entity->position.y -= ((entity->field_74.y - entity->playerPos.y) >> 3) * min(entity->field_7C, 8);
    if (entity->field_7C > 0)
        entity->field_7C--;
    if (entity->field_7C < 0)
        entity->field_7C = 0;

    if (HandLauncher_Unknown1())
        entity->state = HandLauncher_Unknown7;
}

void HandLauncher_Unknown7(void)
{
    RSDK_THIS(HandLauncher);
    entity->position = entity->field_74;
    entity->position.y -= ((entity->field_74.y - entity->playerPos.y) >> 3) * min(entity->field_7C, 8);
    if (entity->field_7C < 8)
        entity->field_7C++;
    if (entity->field_7C > 8)
        entity->field_7C = 8;

    HandLauncher_CheckPlayerCollisions();
    if (entity->activePlayers) {
        RSDK.SetSpriteAnimation(HandLauncher->spriteIndex, 2, &entity->animator2, true, 0);
        entity->field_7C                 = 0;
        entity->animator1.animationSpeed = 1;
        entity->state                    = HandLauncher_Unknown8;
    }

    HandLauncher_Unknown3();
    if (!HandLauncher_Unknown1())
        entity->state = HandLauncher_Unknown6;
}

void HandLauncher_Unknown8(void)
{
    RSDK_THIS(HandLauncher);

    if (entity->field_7C < 3) {
        entity->position = entity->playerPos;
        int dist         = (entity->playerPos.y - entity->startPos.y) / 3;
        if (entity->field_7C <= 3) {
            entity->position.y -= dist * entity->field_7C;
            entity->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
        else {
            entity->position.y -= dist * 3;
            entity->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
    }
    else {
        if (entity->field_7C < 63) {
            entity->position = entity->startPos;
            entity->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
        else if (entity->field_7C < 66) {
            entity->position = entity->playerPos;
            int dist          = (entity->playerPos.y - entity->startPos.y) / 3;
            entity->position.y -= dist * min(66 - entity->field_7C, 3);
            entity->field_7C++;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
        else {
            HandLauncher_State_Release();
            if (!HandLauncher->dunkeyMode) {
                RSDK.PlaySFX(Player->sfx_Release, 0, 255);
            }
            RSDK.SetSpriteAnimation(HandLauncher->spriteIndex, 4, &entity->animator2, true, 0);
            entity->position                 = entity->playerPos;
            entity->field_80                 = 30;
            entity->field_7C                 = 0;
            entity->animator1.animationSpeed = 0;
            entity->state                    = HandLauncher_Unknown6;
            HandLauncher_CheckPlayerCollisions();
            HandLauncher_Unknown3();
        }
    }
}

void HandLauncher_EditorDraw(void) {}

void HandLauncher_EditorLoad(void) {}

void HandLauncher_Serialize(void)
{
    RSDK_EDITABLE_VAR(HandLauncher, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(HandLauncher, VAR_UINT32, speed);
}
