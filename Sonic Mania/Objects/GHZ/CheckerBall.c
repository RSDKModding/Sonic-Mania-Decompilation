#include "SonicMania.h"

ObjectCheckerBall *CheckerBall;

void CheckerBall_Update(void)
{
    RSDK_THIS(CheckerBall);
    entity->active = ACTIVE_NORMAL;
    CheckerBall_Unknown2();
    entity->field_60 += entity->field_5C;
    CheckerBall_Unknown1();
    RSDK.ProcessTileCollisions(entity, &CheckerBall->outerBox, &CheckerBall->innerBox);
    CheckerBall_Unknown5();
    CheckerBall_Unknown3();
    entity->roundedPos.x = entity->position.x & 0xFFFF0000;
    entity->roundedPos.y = entity->position.y & 0xFFFF0000;
}

void CheckerBall_LateUpdate(void) {}

void CheckerBall_StaticUpdate(void) {}

void CheckerBall_Draw(void)
{
    RSDK_THIS(CheckerBall);
    entity->animator.frameID = (((entity->field_60 >> 14) + 8) >> 4) & 7;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void CheckerBall_Create(void *data)
{
    RSDK_THIS(CheckerBall);
    entity->visible         = true;
    entity->drawOrder       = Zone->drawOrderLow;
    entity->active          = ACTIVE_BOUNDS;
    entity->updateRange.x   = 0x400000;
    entity->updateRange.y   = 0x400000;
    entity->onGround        = false;
    entity->tileCollisions  = true;
    entity->collisionLayers = Zone->fgLayers;
    entity->roundedPos.x    = entity->position.x;
    entity->roundedPos.y    = entity->position.y;
    entity->collisionPlane  = 0;
    RSDK.SetSpriteAnimation(CheckerBall->aniFrames, 0, &entity->animator, true, 0);
}

void CheckerBall_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ")) {
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("GHZ/CheckerBall.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Blueprint")) {
        CheckerBall->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/CheckerBall.bin", SCOPE_STAGE);
    }

    CheckerBall->outerBox.left   = -9;
    CheckerBall->outerBox.top    = -22;
    CheckerBall->outerBox.right  = 9;
    CheckerBall->outerBox.bottom = 22;
    CheckerBall->innerBox.left   = -8;
    CheckerBall->innerBox.top    = -22;
    CheckerBall->innerBox.right  = 8;
    CheckerBall->innerBox.bottom = 22;
    CheckerBall->hitbox.left     = -22;
    CheckerBall->hitbox.top      = -22;
    CheckerBall->hitbox.right    = 22;
    CheckerBall->hitbox.bottom   = 22;

    DEBUGMODE_ADD_OBJ(CheckerBall);

    CheckerBall->array1[0]  = 0;
    CheckerBall->array1[1]  = 0;
    CheckerBall->array1[2]  = 0;
    CheckerBall->array1[3]  = 0;
    CheckerBall->array1[4]  = 1;
    CheckerBall->array1[5]  = 1;
    CheckerBall->array1[6]  = 1;
    CheckerBall->array1[7]  = 1;
    CheckerBall->array1[8]  = 2;
    CheckerBall->array1[9]  = 2;
    CheckerBall->array1[10] = 3;
    CheckerBall->array1[11] = 3;
    CheckerBall->array1[12] = 4;
    CheckerBall->array1[13] = 4;
    CheckerBall->array1[14] = 5;
    CheckerBall->array1[15] = 6;
    CheckerBall->array1[16] = 7;
    CheckerBall->array1[17] = 8;
    CheckerBall->array1[18] = 9;
    CheckerBall->array1[19] = 10;
    CheckerBall->array1[20] = 12;
    CheckerBall->array1[21] = 14;
    CheckerBall->array1[22] = 16;
    CheckerBall->array1[23] = 20;

    CheckerBall->array2[0]  = -3;
    CheckerBall->array2[1]  = -5;
    CheckerBall->array2[2]  = -8;
    CheckerBall->array2[3]  = -11;
    CheckerBall->array2[4]  = -13;
    CheckerBall->array2[5]  = -16;
    CheckerBall->array2[6]  = -19;
    CheckerBall->array2[7]  = -21;
    CheckerBall->array2[8]  = -24;
    CheckerBall->array2[9]  = -27;
    CheckerBall->array2[10] = -30;
    CheckerBall->array2[11] = -32;
    CheckerBall->array2[12] = -35;
    CheckerBall->array2[13] = -37;
    CheckerBall->array2[14] = -40;
    CheckerBall->array2[15] = -43;
    CheckerBall->array2[16] = -46;
    CheckerBall->array2[17] = -48;
    CheckerBall->array2[18] = -51;
    CheckerBall->array2[19] = -53;
    CheckerBall->array2[20] = -56;
    CheckerBall->array2[21] = -59;
    CheckerBall->array2[22] = -61;
    CheckerBall->array2[23] = -64;
}

void CheckerBall_DebugSpawn(void)
{
    RSDK.CreateEntity(CheckerBall->objectID, 0, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
}

void CheckerBall_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(CheckerBall->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void CheckerBall_Unknown1(void)
{
    RSDK_THIS(CheckerBall);
    if (entity->onGround) {
        int spd = RSDK.Sin256(entity->angle) << 13 >> 8;
        entity->groundVel += spd;
        entity->field_5C = entity->groundVel;
        if (!spd && !entity->flag) {
            if (entity->groundVel <= 0) {
                entity->groundVel += 0x400;
                if (entity->groundVel > 0)
                    entity->groundVel = 0;
            }
            else {
                entity->groundVel -= 0x400;
                if (entity->groundVel < 0)
                    entity->groundVel = 0;
            }
        }
        entity->velocity.x = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
        entity->velocity.y = entity->groundVel * RSDK.Sin256(entity->angle) >> 8;
        entity->flag       = false;
    }
    else {
        entity->velocity.y += 0x3800;
        entity->groundVel = entity->velocity.x;
        entity->flag      = false;
    }
}

void CheckerBall_Unknown2(void)
{
    RSDK_THIS(CheckerBall);

    Hitbox hitbox;
    hitbox.top    = -22;
    hitbox.bottom = 22;

    int playerID          = 0;
    entity->activePlayers = 0;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int acc              = player->acceleration >> 1;
        if (player->onGround) {
            if (player->position.x <= entity->position.x) {
                hitbox.left  = -23;
                hitbox.right = 0;
                if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                    if (!player->right) {
                        if (entity->velocity.x > 0 && player->groundVel > entity->groundVel) {
                            player->velocity.x -= 0x10000;
                            player->groundVel -= 0x10000;
                        }
                    }
                    else {
                        entity->activePlayers |= 1 << playerID;
                        if (entity->groundVel < player->topSpeed)
                            entity->groundVel += acc;
                        entity->flag       = true;
                        player->position.x = entity->roundedPos.x + ((playerHitbox->left - 22) << 16);
                        player->groundVel  = 0;
                    }
                }
            }
            else {
                hitbox.left  = 0;
                hitbox.right = 23;
                if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                    if (!player->left) {
                        if (entity->velocity.x < 0 && player->groundVel < entity->groundVel) {
                            player->velocity.x += 0x10000;
                            player->groundVel += 0x10000;
                        }
                    }
                    else {
                        entity->activePlayers |= 1 << playerID;
                        if (entity->groundVel > -player->topSpeed)
                            entity->groundVel -= acc;
                        entity->flag       = true;
                        player->position.x = entity->roundedPos.x + ((playerHitbox->right + 22) << 16);
                        player->groundVel  = 0;
                    }
                }
            }
        }
        ++playerID;
    }
}

void CheckerBall_Unknown3(void)
{
    RSDK_THIS(CheckerBall);
    int x              = ((entity->position.x - entity->collisionOffset.x) & 0xFFFF0000) - entity->roundedPos.x;
    int y              = ((entity->position.y - entity->collisionOffset.y) & 0xFFFF0000) - entity->roundedPos.y;
    int startX         = entity->position.x;
    int startY         = entity->position.y;
    entity->position.x = entity->roundedPos.x;
    entity->position.y = entity->roundedPos.y;

    int playerID = 0;
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if ((1 << playerID) & entity->activePlayers) {
            if (player->position.x >= entity->position.x)
                player->position.x = (22 - playerHitbox->left) << 16;
            else
                player->position.x = (-22 - playerHitbox->right) << 16;
            player->position.x += startX;
        }
        else {
            int result = Player_CheckCollisionBox(player, entity, &CheckerBall->hitbox);
            if (result == 4 && entity->velocity.y >= 0 && player->onGround) {
                player->hurtFlag = true;
            }
            else if (result == 1) {
                player->position.x += x + (x >> 1) + entity->collisionOffset.x;
                player->position.y += y + entity->collisionOffset.y;

                if (player->velocity.x) {
                    if (player->velocity.x <= 0)
                        entity->groundVel += player->acceleration >> 2;
                    else
                        entity->groundVel -= player->acceleration >> 2;
                    entity->flag = true;
                }
                entity->position.x = startX;
                entity->position.y = startY;
                entity->position.x = entity->roundedPos.x;
                entity->position.y = entity->roundedPos.y;
                if (!Player_CheckCollisionBox(player, entity, &CheckerBall->hitbox)) {
                    player->groundVel += x + (x >> 1);
                    player->velocity.x  = player->groundVel;
                    player->onGround    = false;
                    player->controlLock = 15;
                }
            }
        }
        ++playerID;
    }
    entity->position.x        = startX;
    entity->position.y        = startY;
    entity->collisionOffset.x = 0;
    entity->collisionOffset.y = 0;
}

void CheckerBall_BadnikBreak(void *b, Hitbox *hitbox)
{
    RSDK_THIS(CheckerBall);
    Entity *badnik = (Entity *)b;
    if (RSDK.CheckObjectCollisionTouchBox(badnik, hitbox, entity, &CheckerBall->hitbox)) {
        int id = RSDK.Rand(0, 32);
        RSDK.CreateEntity(Animals->objectID, intToVoid(Animals->animalTypes[id >> 4] + 1), badnik->position.x, badnik->position.y);
        RSDK.CreateEntity(Explosion->objectID, intToVoid(1), badnik->position.x, badnik->position.y - 0x100000)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySFX(Explosion->sfx_Destroy, 0, 255);
        EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityScoreBonus *bonus = (EntityScoreBonus *)RSDK.CreateEntity(ScoreBonus->objectID, 0, badnik->position.x, badnik->position.y);
        bonus->drawOrder        = Zone->drawOrderHigh;
        bonus->animator.frameID     = player1->scoreBonus;
        switch (player1->scoreBonus) {
            case 0: Player_GiveScore(player1, 100); break;
            case 1: Player_GiveScore(player1, 200); break;
            case 2: Player_GiveScore(player1, 500); break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE: Player_GiveScore(player1, 1000); break;
            case 0xF: Player_GiveScore(player1, 10000); break;
            default: break;
        }

        if (player1->scoreBonus < 15)
            player1->scoreBonus++;
        RSDK.ResetEntityPtr(badnik, TYPE_BLANK, NULL);
    }
}

void CheckerBall_Unknown5(void)
{
    RSDK_THIS(CheckerBall);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    foreach_active(PlaneSwitch, planeSwitch)
    {
        int x     = (entity->position.x - planeSwitch->position.x) >> 8;
        int y     = (entity->position.y - planeSwitch->position.y) >> 8;
        int scanX = (y * RSDK.Sin256(planeSwitch->negAngle)) + (x * RSDK.Cos256(planeSwitch->negAngle)) + planeSwitch->position.x;
        int scanY = (y * RSDK.Cos256(planeSwitch->negAngle)) - (x * RSDK.Sin256(planeSwitch->negAngle)) + planeSwitch->position.y;
        int pos   = ((entity->velocity.y >> 8) * RSDK.Sin256(planeSwitch->negAngle)) + (entity->velocity.x >> 8) * RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Sin256(planeSwitch->negAngle);
        if (!(planeSwitch->onPath && !entity->onGround)) {
            int xDif = abs(scanX - planeSwitch->position.x);
            int yDif = abs(scanY - planeSwitch->position.y);

            if (xDif < 0x180000 && yDif < planeSwitch->size << 19) {
                if (scanX + pos >= planeSwitch->position.x) {
                    entity->collisionPlane = (planeSwitch->flags >> 3) & 1;
                    if (!(planeSwitch->flags & 4)) {
                        entity->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        entity->drawOrder = Zone->playerDrawHigh;
                    }
                }
                else {
                    entity->collisionPlane = (planeSwitch->flags >> 1) & 1;
                    if (!(planeSwitch->flags & 1)) {
                        entity->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        entity->drawOrder = Zone->playerDrawHigh;
                    }
                }
            }
        }
    }

    foreach_active(CheckerBall, checkerBall)
    {
        if (checkerBall != entity && entity->collisionPlane == checkerBall->collisionPlane) {
            int pos = abs(entity->position.x - checkerBall->position.x) >> 17;
            if (pos >= 24)
                pos = 23;
            Hitbox hitbox;
            hitbox.top       = CheckerBall->array1[pos] - 22 + (CheckerBall->array1[pos] >> 1);
            hitbox.bottom    = -hitbox.top;
            hitbox.left      = -22;
            hitbox.right     = 22;
            switch (RSDK.CheckObjectCollisionBox(checkerBall, &hitbox, entity, &hitbox, true)) {
                case 1:
                    if (entity->position.x >= checkerBall->position.x) {
                        entity->groundVel += 0xC00;
                        entity->velocity.x += 0xC00;
                        checkerBall->groundVel -= 0xC00;
                        checkerBall->velocity.x -= 0xC00;
                        checkerBall->field_5C -= 0xC00;
                    }
                    else {
                        entity->groundVel -= 0xC00;
                        entity->velocity.x -= 0xC00;
                        checkerBall->groundVel += 0xC00;
                        checkerBall->velocity.x += 0xC00;
                        checkerBall->field_5C += 0xC00;
                    }
                    break;
                case 2:
                case 3:
                    if (entity->groundVel <= 0) {
                        if (entity->groundVel < 0 && entity->position.x > checkerBall->position.x) {
                            entity->velocity.x = checkerBall->velocity.x;
                            entity->groundVel  = checkerBall->groundVel;
                        }
                    }
                    else if (entity->position.x < checkerBall->position.x) {
                        entity->velocity.x      = checkerBall->velocity.x;
                        entity->groundVel       = checkerBall->groundVel;
                        checkerBall->groundVel  = entity->groundVel;
                        checkerBall->velocity.x = entity->velocity.x;
                    }
                    else {
                        if (entity->groundVel < 0 && entity->position.x > checkerBall->position.x) {
                            entity->velocity.x = checkerBall->velocity.x;
                            entity->groundVel  = checkerBall->groundVel;
                        }
                    }
                    break;
                case 4:
                    if (entity->velocity.y <= 0) {
                        if (entity->velocity.y < 0 && entity->position.y > checkerBall->position.y) {
                            entity->velocity.y      = checkerBall->velocity.y;
                            checkerBall->velocity.y = entity->velocity.y;
                            if (entity->onGround) {
                                entity->velocity.x = checkerBall->velocity.x;
                                entity->groundVel  = checkerBall->groundVel;
                            }
                            if (checkerBall->onGround) {
                                checkerBall->groundVel  = entity->groundVel;
                                checkerBall->velocity.x = entity->velocity.x;
                            }
                        }
                    }
                    if (entity->position.y < checkerBall->position.y) {
                        entity->velocity.y      = checkerBall->velocity.y;
                        checkerBall->velocity.y = entity->velocity.y;
                        if (entity->onGround) {
                            entity->velocity.x = checkerBall->velocity.x;
                            entity->groundVel  = checkerBall->groundVel;
                        }
                        if (checkerBall->onGround) {
                            checkerBall->groundVel  = entity->groundVel;
                            checkerBall->velocity.x = entity->velocity.x;
                        }
                    }
                    else {
                        if (entity->velocity.y < 0 && entity->position.y > checkerBall->position.y) {
                            entity->velocity.y      = checkerBall->velocity.y;
                            checkerBall->velocity.y = entity->velocity.y;
                            if (entity->onGround) {
                                entity->velocity.x = checkerBall->velocity.x;
                                entity->groundVel  = checkerBall->groundVel;
                            }
                            if (checkerBall->onGround) {
                                checkerBall->groundVel  = entity->groundVel;
                                checkerBall->velocity.x = entity->velocity.x;
                            }
                        }
                    }
                    break;
                default: break;
            }

            if (RSDK.CheckObjectCollisionTouchBox(checkerBall, &hitbox, entity, &hitbox)) {
                if (entity->position.x >= checkerBall->position.x)
                    entity->position.x += 0x10000;
                else
                    entity->position.x -= 0x10000;

                if (entity->position.y >= checkerBall->position.y)
                    entity->position.y += 0x10000;
                else
                    entity->position.y -= 0x10000;
            }
        }
    }

    foreach_active(Platform, platform)
    {
        if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
            platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
            int result           = 0;
            if (platform->collision)
                result = RSDK.CheckObjectCollisionBox(platform, &platform->hitbox, entity, &CheckerBall->hitbox, true);
            else
                result = RSDK.CheckObjectCollisionPlatform(platform, &platform->hitbox, entity, &CheckerBall->hitbox, true);
            if (result == 1) {
                if (platform->state == Platform_State_Collapsing && !platform->collapseDelay)
                    platform->collapseDelay = 30;
                platform->stood = true;
                entity->position.x += platform->collisionOffset.x;
                entity->position.y += platform->collisionOffset.y;
                entity->position.y &= 0xFFFF0000;
                entity->onGround          = true;
                entity->collisionOffset.x = platform->collisionOffset.x;
                entity->collisionOffset.y = platform->collisionOffset.y;
            }
            platform->position.x = platform->centerPos.x;
            platform->position.y = platform->centerPos.y;
        }
    }

    foreach_active(Bridge, bridge)
    {
        if (entity->position.x > bridge->startPos && entity->position.x < bridge->endPos) {
            if (entity == (EntityCheckerBall*)bridge->playerPtr) {
                int distance     = bridge->endPos - bridge->startPos;
                bridge->stoodPos = entity->position.x - bridge->startPos;
                bridge->field_68 = RSDK.Sin512((bridge->stoodPos >> 8) / (distance >> 16)) * (distance >> 13);
                if (entity->position.y > bridge->position.y - 0x300000) {
                    if (entity->velocity.y < 0) {
                        bridge->playerPtr = (Entity *)-2;
                    }
                    else {
                        ++bridge->activePlayerCount;
                        entity->position.y = bridge->position.y + bridge->field_6C - ((CheckerBall->hitbox.bottom + 8) << 16);
                        if (!entity->onGround) {
                            entity->onGround  = true;
                            entity->groundVel = entity->velocity.x;
                        }
                        entity->velocity.y = 0;
                    }
                }
            }
            else {
                Hitbox bridgeHitbox;
                bridgeHitbox.left  = -0x400;
                bridgeHitbox.right = 0x400;

                if (!bridge->activePlayerCount)
                    bridge->stoodPos = entity->position.x - bridge->startPos;

                if (entity->velocity.y >= 0) {
                    int divisor = 0;
                    int ang     = 0;
                    if (entity->position.x - bridge->startPos > bridge->stoodPos) {
                        divisor = bridge->endPos - bridge->startPos - bridge->stoodPos;
                        ang     = (bridge->endPos - entity->position.x) << 7;
                    }
                    else {
                        divisor = bridge->stoodPos;
                        ang     = (entity->position.x - bridge->startPos) << 7;
                    }

                    int hitY = (bridge->field_6C * RSDK.Sin512(ang / divisor) >> 9) - 0x80000;
                    if (entity->velocity.y < 0x8000) {
                        bridgeHitbox.bottom = (hitY >> 16);
                        bridgeHitbox.top    = (hitY >> 16) - 8;
                    }
                    else {
                        bridgeHitbox.top    = (hitY >> 16);
                        bridgeHitbox.bottom = (hitY >> 16) + 8;
                    }

                    if (RSDK.CheckObjectCollisionTouchBox(bridge, &bridgeHitbox, entity, &CheckerBall->hitbox)) {
                        ++bridge->activePlayerCount;

                        entity->position.y = hitY + bridge->position.y - (CheckerBall->hitbox.bottom << 16);
                        if (!entity->onGround) {
                            entity->onGround  = true;
                            entity->groundVel = entity->velocity.x;
                        }

                        if (bridge->playerPtr == (Entity *)-1) {
                            bridge->playerPtr = (Entity *)entity;
                            if (entity->velocity.y < 256)
                                bridge->timer = 0x80;
                        }

                        if (bridge->playerPtr == (Entity *)-2)
                            bridge->playerPtr = (Entity *)entity;
                        entity->velocity.y = 0;
                    }
                }
            }
        }
        else if (entity == (EntityCheckerBall *)bridge->playerPtr) {
            bridge->playerPtr = (Entity *)-2;
            bridge->timer     = 32;
        }
    }

    foreach_active(Batbrain, batBrain) { CheckerBall_BadnikBreak(batBrain, &Batbrain->hitbox); }

    foreach_active(BuzzBomber, buzzBomber)
    {
        if (buzzBomber->state == BuzzBomber_Unknown6 || buzzBomber->state == BuzzBomber_Unknown7) {
            if (RSDK.CheckObjectCollisionTouchBox(buzzBomber, &BuzzBomber->hitbox2, entity, &CheckerBall->hitbox)) {
                int angle              = RSDK.ATan2(entity->position.x - buzzBomber->position.x, entity->position.y - buzzBomber->position.y);
                buzzBomber->velocity.x = -0x80 * RSDK.Cos256(angle);
                buzzBomber->velocity.y = -0x80 * RSDK.Sin256(angle);
            }
        }
        else {
            CheckerBall_BadnikBreak(buzzBomber, &BuzzBomber->hitbox1);
        }
    }

    foreach_active(Chopper, chopper) { CheckerBall_BadnikBreak(chopper, &Chopper->hitbox); }

    foreach_active(Crabmeat, crabmeat)
    {
        if (crabmeat->state == Crabmeat_State_Projectile) {
            if (RSDK.CheckObjectCollisionTouchBox(crabmeat, &Crabmeat->projectileHitbox, entity, &CheckerBall->hitbox)) {
                int angle            = RSDK.ATan2(entity->position.x - crabmeat->position.x, entity->position.y - crabmeat->position.y);
                crabmeat->velocity.x = -0x80 * RSDK.Cos256(angle);
                crabmeat->velocity.y = -0x80 * RSDK.Sin256(angle);
            }
        }
        else {
            CheckerBall_BadnikBreak(crabmeat, &Crabmeat->hitbox);
        }
    }

    foreach_active(Motobug, motobug)
    {
        if (motobug->state != Motobug_State_Smoke)
            CheckerBall_BadnikBreak(motobug, &Motobug->hitbox);
    }

    foreach_active(Newtron, newtron)
    {
        if (newtron->state == Newtron_State_Projectile) {
            if (RSDK.CheckObjectCollisionTouchBox(newtron, &Newtron->hitbox3, entity, &CheckerBall->hitbox)) {
                int angle           = RSDK.ATan2(entity->position.x - newtron->position.x, entity->position.y - newtron->position.y);
                newtron->velocity.x = -0x800 * RSDK.Cos256(angle);
                newtron->velocity.y = -0x800 * RSDK.Sin256(angle);
            }
        }
        else {
            if (newtron->type == 1)
                CheckerBall_BadnikBreak(newtron, &Newtron->hitbox2);
            else
                CheckerBall_BadnikBreak(newtron, &Newtron->hitbox1);
        }
    }

    foreach_active(Splats, splats) { CheckerBall_BadnikBreak(splats, &Splats->hitbox1); }

    foreach_active(ItemBox, itemBox)
    {
        if ((itemBox->state == ItemBox_State_Normal || itemBox->state == ItemBox_State_Falling)
            && RSDK.CheckObjectCollisionTouchBox(itemBox, &ItemBox->hitbox, entity, &CheckerBall->hitbox)) {
            RSDK.CreateEntity(TYPE_BLANK, 0, itemBox->position.x, itemBox->position.y);
            itemBox->storedEntity  = (Entity *)player1;
            itemBox->alpha         = 0x100;
            itemBox->contentsSpeed = -0x38000;
            itemBox->active        = 1;
            itemBox->velocity.y    = -0x20000;
            itemBox->state         = ItemBox_State_Contents;
            RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 1, &itemBox->brokenData, true, 0);
            itemBox->brokenData.frameID = ItemBox->brokenFrame++;
            ItemBox->brokenFrame %= 3;
            RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->overlayData, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &itemBox->debrisData, true, 0);
            RSDK.CreateEntity(Explosion->objectID, 0, itemBox->position.x, itemBox->position.y - 0x100000);

            for (int i = 0; i < 6; ++i) {
                EntityDebris *debris = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, 0, itemBox->position.x + RSDK.Rand(-0x80000, 0x80000),
                                                                         itemBox->position.y + RSDK.Rand(-0x80000, 0x80000));
                debris->state        = Debris_State_Fall;
                debris->gravity      = 0x4000;
                debris->velocity.x   = RSDK.Rand(0, 0x20000);
                if (debris->position.x < itemBox->position.x)
                    debris->velocity.x = -debris->velocity.x;
                debris->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                debris->drawFX     = FX_FLIP;
                debris->direction  = i & 3;
                debris->drawOrder  = Zone->drawOrderHigh;
                RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 6, &debris->animator, true, RSDK.Rand(0, 4));
            }

            RSDK.PlaySFX(ItemBox->sfx_Destroy, 0, 255);
            itemBox->active = ACTIVE_NORMAL;
            if (itemBox->type == 13) {
                if (itemBox->type == 7) {
                    switch (player1->characterID) {
                        case ID_SONIC: itemBox->type = 7; break;
                        case ID_TAILS: itemBox->type = 8; break;
                        case ID_KNUCKLES: itemBox->type = 9; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY: itemBox->type = 15; break;
                        case ID_RAY: itemBox->type = 16; break;
#endif
                        default: break;
                    }
                }
                itemBox->contentsData.frameID = itemBox->type;
            }
        }
    }

    foreach_active(BreakableWall, breakableWall)
    {
        if (breakableWall->state == BreakableWall_State_BreakableSides
            && RSDK.CheckObjectCollisionTouchBox(breakableWall, &breakableWall->hitbox, entity, &CheckerBall->hitbox)) {
            int *arrayPtr2 = BreakableWall->value3;

            int *arrayPtr = NULL;
            if (entity->position.x >= breakableWall->position.x)
                arrayPtr = BreakableWall->value5;
            else
                arrayPtr = BreakableWall->value4;

            for (int y = 0; y < 4; ++y) {
                for (int x = 0; x < 2; ++x) {
                    EntityBreakableWall *block = (EntityBreakableWall *)RSDK.CreateEntity(
                        BreakableWall->objectID, intToVoid(1), breakableWall->position.x + arrayPtr2[0], arrayPtr2[1] + breakableWall->position.y);
                    block->tileInfo   = RSDK.GetTileInfo(Zone->fgHigh, (breakableWall->position.x + arrayPtr2[0]) >> 20,
                                                       (arrayPtr2[1] + breakableWall->position.y) >> 20);
                    block->velocity.x = arrayPtr[0];
                    block->velocity.y = arrayPtr[1];
                    RSDK.SetTileInfo(Zone->fgHigh, (breakableWall->position.x + arrayPtr2[0]) >> 20, (arrayPtr2[1] + breakableWall->position.y) >> 20,
                                     0xFFFF);
                    arrayPtr2 += 2;
                    arrayPtr += 2;
                }
            }
            RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
            RSDK.ResetEntityPtr(breakableWall, TYPE_BLANK, NULL);
        }
    }

    foreach_active(CollapsingPlatform, collapsingPlatform)
    {
        if (RSDK.CheckObjectCollisionTouchBox(collapsingPlatform, &collapsingPlatform->hitbox, entity, &CheckerBall->hitbox) && entity->onGround) {
            collapsingPlatform->playerPos.x = entity->position.x;
        }
    }

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionBox(spikes, &spikes->hitbox, entity, &CheckerBall->hitbox, true)) {
            entity->onGround = true;
            entity->position.x += spikes->offset.x;
            entity->position.y += spikes->offset.y;
        }
    }

    foreach_active(Spring, spring)
    {
        int result = RSDK.CheckObjectCollisionBox(spring, &spring->hitbox, entity, &CheckerBall->hitbox, true);
        if (result) {
            if (spring->state == Spring_State_Vertical) {
                if (spring->direction) {
                    if (result == 4) {
                        entity->onGround   = false;
                        entity->velocity.y = spring->velocity.y;
                        RSDK.SetSpriteAnimation(Spring->spriteIndex, spring->type, &spring->animator, true, 0);
                        RSDK.PlaySFX(Spring->sfx_Spring, 0, 255);
                    }
                }
                else if (result == 1) {
                    entity->onGround = false;
                    entity->velocity.y = spring->velocity.y;
                    RSDK.SetSpriteAnimation(Spring->spriteIndex, spring->type, &spring->animator, true, 0);
                    RSDK.PlaySFX(Spring->sfx_Spring, 0, 255);
                }
            }
            else {
                if (spring->state == Spring_State_Diagonal) {
                    if (!entity->onGround && entity->velocity.y < 0 && abs(entity->velocity.x) <= -entity->velocity.y) {
                        continue;
                    }
                    else {
                        entity->onGround   = false;
                        entity->velocity.x = spring->velocity.x;
                        entity->velocity.y = spring->velocity.y;
                        RSDK.SetSpriteAnimation(Spring->spriteIndex, spring->type, &spring->animator, true, 0);
                        RSDK.PlaySFX(Spring->sfx_Spring, 0, 255);
                    }
                }
                else if (spring->state == Spring_State_Horizontal) {
                    if (!spring->direction) {
                        if (result == 3) {
                            if (!spring->onGround || entity->onGround) {
                                entity->velocity.x = spring->velocity.x;
                                entity->groundVel  = spring->velocity.x;
                                RSDK.SetSpriteAnimation(Spring->spriteIndex, spring->type, &spring->animator, true, 0);
                                RSDK.PlaySFX(Spring->sfx_Spring, 0, 255);
                            }
                        }
                    }
                    else {
                        if (result == 2) {
                            if (!spring->onGround || entity->onGround) {
                                entity->velocity.x = spring->velocity.x;
                                entity->groundVel  = spring->velocity.x;
                                RSDK.SetSpriteAnimation(Spring->spriteIndex, spring->type, &spring->animator, true, 0);
                                RSDK.PlaySFX(Spring->sfx_Spring, 0, 255);
                            }
                        }
                    }
                }
            }
        }
    }
}

void CheckerBall_EditorDraw(void) {}

void CheckerBall_EditorLoad(void) {}

void CheckerBall_Serialize(void) {}
