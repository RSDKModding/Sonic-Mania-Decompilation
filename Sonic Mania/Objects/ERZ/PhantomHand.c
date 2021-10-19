#include "SonicMania.h"

ObjectPhantomHand *PhantomHand;

void PhantomHand_Update(void)
{
    RSDK_THIS(PhantomHand);
    StateMachine_Run(entity->state);
}

void PhantomHand_LateUpdate(void) {}

void PhantomHand_StaticUpdate(void) {}

void PhantomHand_Draw(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.CopyPalette(1, 128, 0, 128, 128);
    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    if (entity->state != PhantomHand_State_Unknown4) {
        entity->inkEffect = INK_ALPHA;
        entity->alpha     = 0xD0;
        RSDK.DrawSprite(&entity->animator2, NULL, false);

        entity->inkEffect = INK_ADD;
        entity->alpha     = 0x100;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void PhantomHand_Create(void *data)
{
    RSDK_THIS(PhantomHand);

    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->parent        = (Entity *)data;
        entity->field_6C      = entity->position.x;
        entity->field_70      = entity->position.y;
        entity->active        = ACTIVE_NORMAL;
        entity->inkEffect     = INK_ALPHA;
        entity->drawFX        = FX_FLIP | FX_SCALE;
        entity->alpha         = 0x100;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->field_64      = 64;
        entity->state         = PhantomHand_State_Summon;
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 0, &entity->animator1, true, 0);
    }
}

void PhantomHand_StageLoad(void)
{
    PhantomHand->aniFrames     = RSDK.LoadSpriteAnimation("Phantom/PhantomHand.bin", SCOPE_STAGE);
    PhantomHand->hitbox.left   = -20;
    PhantomHand->hitbox.top    = -20;
    PhantomHand->hitbox.right  = 20;
    PhantomHand->hitbox.bottom = 20;

    int pos = 3;
    for (int i = 0; i < PhantomHand->debrisInfo[0]; ++i) {
        PhantomHand->debrisInfo[pos + 0] = RSDK.Rand(-0x20000, 0x20000);
        PhantomHand->debrisInfo[pos + 1] = RSDK.Rand(-0x10000, -0x40000);

        pos += 4;
    }

    PhantomHand->grabbedPlayers[0] = false;
    PhantomHand->grabbedPlayers[1] = false;
    PhantomHand->grabbedPlayers[2] = false;
    PhantomHand->grabbedPlayers[3] = false;
}

void PhantomHand_CheckPlayerGrab(int playerX, int playerY)
{
    RSDK_THIS(PhantomHand);

    if (entity->position.x >= playerX) {
        if (entity->position.x > playerX) {
            int dist = (entity->position.x - playerX) >> 5;
            if (dist > 0x10000)
                dist = 0x10000;
            entity->position.x -= dist;
        }
    }
    else {
        int dist = (playerX - entity->position.x) >> 5;
        if (dist > 0x10000)
            dist = 0x10000;
        entity->position.x += dist;
    }

    if (entity->field_70 < playerY)
        entity->field_70 += 0x10000;

    if (playerY <= entity->field_70)
        entity->position.y += (playerY - entity->position.y) >> 3;
    else
        entity->position.y = entity->field_70;

    foreach_active(Player, player)
    {
        if (player->state != Player_State_None) {
            int rx = (player->position.x - entity->position.x) >> 16;
            int ry = (player->position.y - entity->position.y) >> 16;
            if (rx * rx + ry * ry < 0x100) {
                if (!PhantomHand->grabbedPlayers[player->playerID]) {
                    PhantomHand->grabbedPlayers[player->playerID] = true;
                    entity->state                                 = PhantomHand_State_Unknown5;
                }
            }
        }
    }
}

void PhantomHand_State_Summon(void)
{
    RSDK_THIS(PhantomHand);

    if (!entity->timer)
        RSDK.PlaySfx(PhantomEgg->sfxSummon, false, 255);
    entity->position.x += entity->velocity.x;
    if (entity->timer > 0)
        entity->field_64 -= 4;
    entity->timer += entity->field_64;

    if (entity->timer < 0x200 && entity->field_64 < 0) {
        entity->timer = 0x200;
        entity->state = PhantomHand_State_Unknown1;
    }
    entity->field_68 += 8;
    entity->scale.x = entity->timer + ((entity->timer * RSDK.Sin256(entity->field_68)) >> 11);
    entity->scale.y = entity->timer + ((entity->timer * RSDK.Cos256(entity->field_68)) >> 11);
}

void PhantomHand_State_Unknown1(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->field_68 += 16;
    entity->position.x += entity->velocity.x;
    entity->scale.x = ((entity->timer * RSDK.Sin256(entity->field_68)) >> 11) + 0x200;
    entity->scale.y = ((entity->timer * RSDK.Cos256(entity->field_68)) >> 11) + 0x200;
    if (entity->timer <= 0) {
        entity->drawFX = FX_FLIP;
        if (Player->playerCount < 2)
            entity->state = PhantomHand_State_Unknown2;
        else
            entity->state = PhantomHand_State_Unknown3;
    }
    else {
        entity->timer -= 32;
    }
}

void PhantomHand_State_Unknown2(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&entity->animator1);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->interaction == true && player1->state == Player_State_None) {
        entity->state = PhantomHand_State_Unknown4;
    }
    else {
        if ((entity->direction && player1->position.x <= PhantomEgg->value7) || (entity->direction && player1->position.x >= PhantomEgg->value7)) {
            PhantomHand_CheckPlayerGrab(entity->parent->position.x, player1->position.y);
        }
        else {
            PhantomHand_CheckPlayerGrab(player1->position.x, player1->position.y);
        }
    }
}

void PhantomHand_State_Unknown3(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&entity->animator1);

    bool32 flag = true;
    for (int i = SLOT_PLAYER1; i < SLOT_PLAYER3; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
        if (player->objectID == Player->objectID)
            flag = flag && player->interaction && player->state == Player_State_None;
    }

    if (flag) {
        entity->state = PhantomHand_State_Unknown4;
    }
    else {
        int dist = 0x7FFFFFFF;

        EntityPlayer *playerPtr = NULL;
        if (entity->direction == FLIP_NONE) {
            foreach_active(Player, player)
            {
                if (entity->state != Player_State_None && entity->position.x - entity->position.x < dist) {
                    dist      = entity->position.x - entity->position.x;
                    playerPtr = player;
                }
            }
            if (playerPtr)
                PhantomHand_CheckPlayerGrab(playerPtr->position.x, playerPtr->position.y);
        }
        else {
            foreach_active(Player, player)
            {
                if (entity->state != Player_State_None && player->position.x - entity->position.x < dist) {
                    dist      = player->position.x - entity->position.x;
                    playerPtr = player;
                }
            }
        }
        if (playerPtr)
            PhantomHand_CheckPlayerGrab(playerPtr->position.x, playerPtr->position.y);
    }
}

void PhantomHand_State_Unknown5(void)
{
    RSDK_THIS(PhantomHand);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &PhantomHand->hitbox)) {
            player->state           = Player_State_None;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->position.x      = entity->position.x;
            player->position.y      = entity->position.y;
            player->velocity.x      = 0;
            player->velocity.y      = 0;
            player->onGround        = false;
            player->groundVel       = 0;
            if (!player->playerID)
                entity->field_78 = true;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
        }
    }

    if (++entity->timer2 == 8) {
        entity->timer2 = 0;
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 1, &entity->animator1, true, 0);
        entity->state = PhantomHand_State_Unknown6;
    }
}

void PhantomHand_State_Unknown6(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == 5) {
        entity->drawOrder = Zone->drawOrderHigh;
    }
    if (entity->animator1.frameID == 10) {
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 2, &entity->animator2, true, 0);
        entity->state = PhantomHand_State_Unknown7;
    }
}

void PhantomHand_State_Unknown7(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(PhantomHand->aniFrames, 3, &entity->animator3, true, 0);
        entity->state = PhantomHand_State_Unknown8;
    }
}

void PhantomHand_State_Unknown8(void)
{
    RSDK_THIS(PhantomHand);

    RSDK.ProcessAnimation(&entity->animator3);
    if (entity->animator3.frameID == entity->animator3.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
        // if (entity->field_78)
        //    PhantomEgg_Unknown6();
        entity->state = StateMachine_None;
    }
}

void PhantomHand_State_Unknown9(void)
{
    RSDK_THIS(PhantomHand);
    Debris_FallFlickerAnimSetup(PhantomHand->aniFrames, PhantomHand->debrisInfo, 4);
    RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &PhantomHand->hitbox)) {
            player->state                                 = Player_State_Air;
            player->velocity.y                            = -0x40000;
            player->onGround                              = false;
            player->groundVel                             = 0;
            PhantomHand->grabbedPlayers[player->playerID] = false;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
        }
    }
    destroyEntity(entity);
}

void PhantomHand_State_Unknown4(void)
{
    RSDK_THIS(PhantomHand);

    if (entity->alpha <= 0)
        destroyEntity(entity);
    else
        entity->alpha -= 8;
}

void PhantomHand_EditorDraw(void) {}

void PhantomHand_EditorLoad(void) {}

void PhantomHand_Serialize(void) {}
