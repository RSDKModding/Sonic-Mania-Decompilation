#include "SonicMania.h"

ObjectSP500 *SP500;

void SP500_Update(void)
{
    RSDK_THIS(SP500);
    StateMachine_Run(entity->state);
}

void SP500_LateUpdate(void) {}

void SP500_StaticUpdate(void) {}

void SP500_Draw(void)
{
    RSDK_THIS(SP500);

    Vector2 drawPos;
    drawPos.x                = entity->position.x - 0x180000;
    drawPos.y                = entity->position.y;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 3;
    int pos                  = drawPos.x;
    for (int l = 0; l < entity->len; ++l) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        pos = drawPos.x + 0x100000;
        drawPos.x += 0x100000;
    }

    drawPos.x = pos - 0x100000;
    entity->direction ^= FLIP_X;
    entity->animator.frameID = 2;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->direction ^= FLIP_X;
    drawPos.x                = entity->position.x + entity->offset;
    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 1;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->inkEffect = INK_ADD;
    if (entity->flagA) {
        entity->animator.frameID = 4;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }

    if (entity->flagB) {
        entity->animator.frameID = 5;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }
    entity->inkEffect = INK_NONE;
}

void SP500_Create(void *data)
{
    RSDK_THIS(SP500);
    entity->drawFX = FX_FLIP;

    if (!RSDK_sceneInfo->inEditor) {
        if (data) {
            entity->active  = ACTIVE_NORMAL;
            entity->visible = false;
            entity->state   = SP500_Unknown10;
        }
        else {
            entity->active  = ACTIVE_BOUNDS;
            entity->visible = true;
            entity->offL <<= 20;
            entity->offR <<= 20;
            entity->drawOrder     = Zone->drawOrderHigh;
            entity->updateRange.x = (entity->len + 4) << 20;
            entity->updateRange.y = 0x800000;

            if (entity->startDir == 0)
                entity->offset = entity->offL;
            else
                entity->offset = entity->offR;

            if (entity->printDir == 0)
                entity->field_8C = entity->height - 1;
            entity->srcC.x = entity->srcC.x >> 16;
            entity->srcC.y = entity->srcC.y >> 16;
            entity->srcM.x = entity->srcM.x >> 16;
            entity->srcM.y = entity->srcM.y >> 16;
            entity->srcY.x = entity->srcY.x >> 16;
            entity->srcY.y = entity->srcY.y >> 16;
            entity->alpha  = 0xC0;
            RSDK.SetSpriteAnimation(SP500->aniFrames, 0, &entity->animator, true, 0);
            entity->state = SP500_Unknown2;
        }
    }
}

void SP500_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        SP500->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500.bin", SCOPE_STAGE);

    SP500->hitbox1.left   = -32;
    SP500->hitbox1.top    = -24;
    SP500->hitbox1.right  = -16;
    SP500->hitbox1.bottom = 24;
    SP500->hitbox2.left   = 16;
    SP500->hitbox2.top    = -24;
    SP500->hitbox2.right  = 32;
    SP500->hitbox2.bottom = 24;
    SP500->hitbox3.left   = -32;
    SP500->hitbox3.top    = 16;
    SP500->hitbox3.right  = 32;
    SP500->hitbox3.bottom = 24;
    SP500->printLayerID   = RSDK.GetSceneLayerID("Print Source");
    SP500->sfxBeep4       = RSDK.GetSFX("Stage/Beep4.wav");
    SP500->sfxFail        = RSDK.GetSFX("Stage/Fail.wav");
    SP500->sfxButton2     = RSDK.GetSFX("Stage/Button2.wav");
}

void SP500_Unknown1(void)
{
    RSDK_THIS(SP500);
    entity->position.x += entity->offset;
    int offsetX = (entity->position.x & 0xFFFF0000) - (entity->xOffset & 0xFFFF0000);
    int offsetY = (entity->position.y & 0xFFFF0000) - (entity->yOffset & 0xFFFF0000);

    //???
    Entity *offset = (Entity *)&entity->xOffset;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (entity->playerTimers[playerID]) {
            entity->playerTimers[playerID]--;
        }
        else if (Player_CheckCollisionBox(player, offset, &SP500->hitbox1) == C_TOP
                 || Player_CheckCollisionBox(player, offset, &SP500->hitbox2) == C_TOP) {
            player->position.x += offsetX;
            player->position.y += offsetY;
        }
        else if (Player_CheckCollisionBox(player, offset, &SP500->hitbox3) == C_TOP) {
            if (entity->state == SP500_Unknown2 && !player->sidekick) {
                if (!((1 << playerID) & entity->activePlayers)) {
                    entity->activePlayers |= (1 << playerID);
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->interaction     = false;
                    player->tileCollisions  = false;
                    player->groundVel       = 0;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->nextAirState    = 0;
                    player->nextGroundState = 0;
                    player->state           = Player_State_None;
                }
            }
        }
        else {
            entity->activePlayers &= ~(1 << playerID);
        }
    }

    for (int i = 0; i < Player->playerCount; ++i) {
        if ((1 << i) & entity->activePlayers) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            player->position.x   = entity->position.x;
            player->position.y   = entity->position.y;
        }
    }
    offset->position.x = entity->position.x;
    offset->position.y = entity->position.y;
    entity->position.x -= entity->offset;
}

void SP500_Unknown2(void)
{
    RSDK_THIS(SP500);
    SP500_Unknown1();
    if (Zone->timer & 0x20) {
        entity->flagA = true;
        entity->flagB = true;
    }
    else {
        entity->flagA = false;
        entity->flagB = false;
    }

    if (entity->activePlayers) {
        RSDK.PlaySfx(SP500->sfxButton2, false, 255);
        entity->flagA = false;
        entity->flagB = false;
        entity->state = SP500_Unknown4;
    }
}

void SP500_Unknown3(void) { SP500_Unknown1(); }

void SP500_Unknown4(void)
{
    RSDK_THIS(SP500);

    SP500_Unknown1();
    if (++entity->timer >= 60) {
        entity->timer = 0;
        if (Ink && Ink->playerTypes[0]) {
            entity->field_AC = Ink->playerTypes[0] - 1;
            RSDK.PlaySfx(SP500->sfxBeep4, false, 255);
            entity->flagA      = true;
            entity->active     = ACTIVE_NORMAL;
            entity->state      = SP500_Unknown6;
            EntitySP500 *child = CREATE_ENTITY(SP500, intToVoid(1), entity->position.x, entity->position.y);
            child->xOffset     = entity->position.x + ((entity->len - 3) << 19);
            if (entity->printDir)
                child->yOffset = entity->position.y + (entity->height << 19);
            else
                child->yOffset = entity->position.y - (entity->height << 19);
            EntityCamera *camera = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->camera;
            camera->targetPtr    = (Entity *)child;
            child->storedEntity  = (Entity *)RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            entity->storedEntity = (Entity *)child;
        }
        else {
            RSDK.PlaySfx(SP500->sfxFail, false, 255);
            entity->flagB = true;
            entity->state = SP500_Unknown5;
        }
    }
}

void SP500_Unknown5(void)
{
    RSDK_THIS(SP500);
    SP500_Unknown1();
    if (++entity->timer >= 30) {
        entity->timer = 0;
        for (int i = 0; i < Player->playerCount; ++i) {
            if ((1 << i) & entity->activePlayers) {
                RSDK.PlaySfx(Player->sfx_Release, false, 255);
                EntityPlayer *player     = RSDK_GET_ENTITY(i, Player);
                player->visible          = true;
                player->interaction      = true;
                player->tileCollisions   = true;
                player->jumpAbility      = 0;
                player->jumpAbilityTimer = 0;
                player->onGround         = false;
                player->state            = Player_State_Air;
                if (entity->printDir == 0)
                    player->velocity.y = 0x60000;
                else
                    player->velocity.y = -0x60000;
                entity->activePlayers &= ~(1 << i);
                entity->playerTimers[i] = 15;
            }
        }
        entity->state = SP500_Unknown2;
    }
}

void SP500_Unknown6(void)
{
    RSDK_THIS(SP500);
    SP500_Unknown1();
    if (++entity->timer >= 30) {
        entity->timer = 0;
        entity->state = SP500_Unknown7;
    }
}

void SP500_Unknown7(void)
{
    RSDK_THIS(SP500);

    if (entity->field_A8 == entity->startDir) {
        entity->offset += entity->velocity.x;
        if (entity->offset >= entity->offR) {
            entity->state = SP500_Unknown8;
            entity->flagA = true;
        }
    }
    else {
        entity->offset -= entity->velocity.x;
        if (entity->offset <= entity->offL) {
            entity->state = SP500_Unknown8;
            entity->flagA = true;
        }
    }

    ushort tile = 0xFFFF;

    switch (entity->field_AC) {
        default: break;
        case 0: tile = RSDK.GetTileInfo(SP500->printLayerID, entity->srcC.x + (entity->offset >> 20), entity->srcC.y + entity->field_8C); break;
        case 1: tile = RSDK.GetTileInfo(SP500->printLayerID, entity->srcM.x + (entity->offset >> 20), entity->srcM.y + entity->field_8C); break;
        case 2: tile = RSDK.GetTileInfo(SP500->printLayerID, entity->srcY.x + (entity->offset >> 20), entity->srcY.y + entity->field_8C); break;
    }

    if (tile != 0xFFFF) {
        entity->position.y += 0x100000;
        entity->flagA = (Zone->timer >> 1) & 1;
        RSDK.SetTileInfo(Zone->fgLow, (entity->position.x + entity->offset) >> 20, entity->position.y >> 20, tile);

        int posY = entity->position.y;
        if (entity->position.y >= 0x1800000) {
            if (entity->position.y > 0x8800000) {
                posY -= 0xA000000;
                RSDK.SetTileInfo(Zone->fgLow, (entity->position.x + entity->offset) >> 20, posY >> 20, tile);
            }
        }
        else {
            posY += 0xA000000;
            RSDK.SetTileInfo(Zone->fgLow, (entity->position.x + entity->offset) >> 20, posY >> 20, tile);
        }
        entity->position.y -= 0x100000;
        entity->velocity.x = 0x80000;
        SP500_Unknown1();
    }
    else {
        entity->velocity.x = 0x100000;
        entity->flagA      = true;
        SP500_Unknown1();
    }
}

void SP500_Unknown8(void)
{
    RSDK_THIS(SP500);

    if ((entity->printDir || entity->field_8C > 0) && (entity->printDir != 1 || entity->field_8C < entity->height - 1)) {
        if (entity->printDir)
            entity->position.y += 0x40000;
        else
            entity->position.y -= 0x40000;
        if (++entity->timer == 4) {
            entity->timer = 0;
            if (entity->printDir) {
                ++entity->field_8C;
                entity->field_A8 ^= 1;
                entity->state = SP500_Unknown7;
            }
            else {
                --entity->field_8C;
                entity->field_A8 ^= 1;
                entity->state = SP500_Unknown7;
            }
        }
    }
    else {
        RSDK.PlaySfx(SP500->sfxBeep4, false, 255);
        entity->state = SP500_Unknown9;
    }
    SP500_Unknown1();
}

void SP500_Unknown9(void)
{
    RSDK_THIS(SP500);

    entity->timer++;
    switch (entity->timer) {
        case 30: RSDK.PlaySfx(SP500->sfxBeep4, false, 255); break;
        case 60: {
            for (int i = 0; i < Player->playerCount; i += 2) {
                EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

                switch (player->characterID) {
                    case ID_SONIC: RSDK.CopyPalette(6, 2, 0, 2, 6); break;
                    case ID_TAILS: RSDK.CopyPalette(6, 70, 0, 70, 6); break;
                    case ID_KNUCKLES: RSDK.CopyPalette(6, 80, 0, 80, 6); break;
                }
                Ink->playerTypes[i] = 0;
            }
            EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            player1->camera->targetPtr = (Entity *)player1;
            EntitySP500 *machine       = (EntitySP500 *)entity->storedEntity;
            machine->xOffset           = player1->position.x;
            machine->yOffset           = player1->position.y;
            machine->state             = SP500_Unknown10;
            machine->storedEntity      = NULL;
            entity->storedEntity       = NULL;
            break;
        }
        case 90:
            for (int i = SLOT_PLAYER1; i < Player->playerCount; ++i) {
                if ((1 << i) & entity->activePlayers) {
                    RSDK.PlaySfx(Player->sfx_Release, false, 255);
                    EntityPlayer *player     = RSDK_GET_ENTITY(i, Player);
                    player->visible          = true;
                    player->interaction      = true;
                    player->tileCollisions   = true;
                    player->jumpAbility      = 0;
                    player->jumpAbilityTimer = 0;
                    player->onGround         = false;
                    player->state            = Player_State_Air;
                    if (entity->printDir == 0)
                        player->velocity.y = -0x100000;
                    else
                        player->velocity.y = 0x100000;
                    entity->activePlayers &= ~(1 << i);
                    entity->playerTimers[i] = 15;
                }
            }
            entity->flagA = false;
            entity->state = SP500_Unknown3;
            break;
    }
    SP500_Unknown1();
}

void SP500_Unknown10(void)
{
    RSDK_THIS(SP500);

    byte flags = 0;
    if (entity->position.x == entity->xOffset) {
        flags = 1;
    }
    else if (entity->position.x >= entity->xOffset) {
        entity->position.x -= 0x80000;
        if (entity->position.x < entity->xOffset)
            entity->position.x = entity->xOffset;
    }
    else {
        entity->position.x += 0x80000;
        if (entity->position.x > entity->xOffset)
            entity->position.x = entity->xOffset;
    }

    if (entity->position.y == entity->yOffset) {
        ++flags;
    }
    else if (entity->position.y >= entity->yOffset) {
        entity->position.y -= 0x80000;
        if (entity->position.y < entity->yOffset)
            entity->position.y = entity->yOffset;
    }
    else {
        entity->position.y += 0x80000;
        if (entity->position.y > entity->yOffset)
            entity->position.y = entity->yOffset;
    }

    if (flags == 2) {
        if (entity->storedEntity)
            entity->state = StateMachine_None;
        else
            destroyEntity(entity);
    }
}

void SP500_EditorDraw(void) {}

void SP500_EditorLoad(void) {}

void SP500_Serialize(void)
{
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, len);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, offL);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, offR);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, startDir);
    RSDK_EDITABLE_VAR(SP500, VAR_ENUM, printDir);
    RSDK_EDITABLE_VAR(SP500, VAR_VECTOR2, srcC);
    RSDK_EDITABLE_VAR(SP500, VAR_VECTOR2, srcM);
    RSDK_EDITABLE_VAR(SP500, VAR_VECTOR2, srcY);
}
