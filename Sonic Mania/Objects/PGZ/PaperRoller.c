#include "SonicMania.h"

ObjectPaperRoller *PaperRoller;

void PaperRoller_Update(void)
{
    RSDK_THIS(PaperRoller);
    RSDK.ProcessAnimation(&entity->animator1);

    int x1 = entity->position.x - (RSDK.Cos256(entity->angle) << 7) * entity->length;
    int v3 = RSDK.Sin256(entity->angle);
    int y1 = entity->position.y - (v3 << 7) * entity->length;
    int x2 = (RSDK.Cos256(entity->angle) << 7) * entity->length + entity->position.x;
    int y2 = (RSDK.Sin256(entity->angle) << 7) * entity->length + entity->position.y;
    int x3 = entity->position.x;
    int y3 = entity->position.y;

    entity->position.x = x1;
    entity->position.y = y1;
    PaperRoller_Unknown4();
    entity->position.x = x2;
    entity->position.y = y2;
    PaperRoller_Unknown4();
    entity->position.x = x3;
    entity->position.y = y3;
    PaperRoller_Unknown5();

    if (entity->direction == FLIP_NONE)
        entity->angleVal += 6;
    else
        entity->angleVal += -6;

    if (entity->angleVal < 0)
        entity->angleVal += 256;
    entity->angleVal = entity->angleVal;
}

void PaperRoller_LateUpdate(void) {}

void PaperRoller_StaticUpdate(void) {}

void PaperRoller_Draw(void)
{
    PaperRoller_Unknown2();
    PaperRoller_Unknown3();
}

void PaperRoller_Create(void *data)
{
    RSDK_THIS(PaperRoller);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_NONE;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    if ((RSDK.Cos256(entity->angle) << 8) * entity->length >= 0)
        entity->updateRange.x += (RSDK.Cos256(entity->angle) << 8) * entity->length;
    else
        entity->updateRange.x += -((RSDK.Cos256(entity->angle) << 8) * entity->length);

    if ((RSDK.Sin256(entity->angle) << 8) * entity->length >= 0)
        entity->updateRange.y += (RSDK.Sin256(entity->angle) << 8) * entity->length;
    else
        entity->updateRange.y += -((RSDK.Sin256(entity->angle) << 8) * entity->length);

    if (entity->length > 256)
        entity->length = 256;

    entity->playerUnknown[0] = 0x7FFF;
    entity->playerUnknown[1] = 0x7FFF;
    entity->playerUnknown[2] = 0x7FFF;
    entity->playerUnknown[3] = 0x7FFF;
    entity->hitbox.left     = -24;
    entity->hitbox.top      = -24;
    entity->hitbox.right    = 24;
    entity->hitbox.bottom   = 24;
    RSDK.SetSpriteAnimation(PaperRoller->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(PaperRoller->aniFrames, 1, &entity->animator2, true, 0);
}

void PaperRoller_StageLoad(void)
{
    uint colours[]         = { 0x0F0F0E8, 0x0D0B898, 0x987870, 0x586868 };
    PaperRoller->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PaperRoller.bin", SCOPE_STAGE);
    for (int i = 0; i < 0x40; ++i) PaperRoller->colours[i] = colours[i & 3];
    PaperRoller->sfxPaper = RSDK.GetSFX("PSZ/Paper.wav");
}

void PaperRoller_Unknown1(uint colour, int len, int posX1, int posY1, int posX2, int posY2, int posX3, int posY3, int posX4, int posY4)
{
    RSDK_THIS(PaperRoller);

    int count = (entity->length / 4) + (entity->length % 4 > 0);
    if (count) {
        int negAngle = -(byte)(entity->angle);

        int posX     = posX1;
        int posY     = posY1;
        int moveX = (posX2 - posX1) / count;
        int moveY = (posY2 - posY1) / count;

        for (int i = 0; i < count; ++i) {
            int clrOffset = 0;
            if (entity->direction)
                clrOffset = len % count;
            else
                clrOffset = count - len % count - 1;
            uint lineClr = PaperRoller->colours[(Zone->timer + clrOffset) % count];

            if (!posY4) {
                RSDK.DrawLine(posX3 + posX, posY3 + posY, posX3 + (posX + moveX), posY3 + (posY + moveY), colour ? colour : lineClr, 127,
                              INK_NONE, false);
            }
            else {
                int distX = (posX - entity->position.x) >> 8;
                int distY = (posY - entity->position.y) >> 8;

                int angValX = distY * RSDK.Sin256(entity->angle) + distX * RSDK.Cos256(entity->angle);

                int angValY = entity->position.y - distX * RSDK.Sin256(entity->angle) + distY * RSDK.Cos256(entity->angle) - entity->position.y;
                int lenY    = 0;

                if (angValX >= posX4) {
                    int length = (entity->length << 15) - posX4;
                    if (length < 0)
                        length = posX4 - (entity->length << 15);
                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * ((length + posX4 - angValX) >> 8) / (length >> 16);
                }
                else {
                    int length = 0;
                    if ((entity->length << 15) + posX4 >= 0)
                        length = (entity->length << 15) + posX4;
                    else
                        length = -(posX4 + (entity->length << 15));

                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * (((entity->length << 15) + angValX) >> 8) / (length >> 16);
                }

                distX = angValX >> 8;
                distY = (lenY + angValY) >> 8;

                int offsetX1 = entity->position.x + distY * RSDK.Sin256(negAngle) + distX * RSDK.Cos256(negAngle);
                int offsetY1 = entity->position.y - distX * RSDK.Sin256(negAngle) + distY * RSDK.Cos256(negAngle);

                distX = ((posX + moveX) - entity->position.x) >> 8;
                distY = ((posY + moveY) - entity->position.y) >> 8;

                angValX = distY * RSDK.Sin256(entity->angle) + distX * RSDK.Cos256(entity->angle);
                angValY = entity->position.y - distX * RSDK.Sin256(entity->angle) + distY * RSDK.Cos256(entity->angle) - entity->position.y;

                lenY = 0;
                if (angValX >= posX4) {
                    int length = (entity->length << 15) - posX4;
                    if (length < 0)
                        length = posX4 - (entity->length << 15);

                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * ((length + posX4 - angValX) >> 8) / (length >> 16);
                }
                else {
                    int length = 0;
                    if ((entity->length << 15) + posX4 >= 0)
                        length = (entity->length << 15) + posX4;
                    else
                        length = -(posX4 + (entity->length << 15));

                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * (((entity->length << 15) + angValX) >> 8) / (length >> 16);
                }

                distX        = angValX >> 8;
                distY        = (lenY + angValY) >> 8;
                int offsetX2 = entity->position.x + distY * RSDK.Sin256(negAngle) + distX * RSDK.Cos256(negAngle);
                int offsetY2 = entity->position.y - distX * RSDK.Sin256(negAngle) + distY * RSDK.Cos256(negAngle);
                RSDK.DrawLine(posX3 + offsetX1, posY3 + offsetY1, posX3 + offsetX2, posY3 + offsetY2, colour ? colour : lineClr, 127, INK_NONE,
                              false);
            }
            ++len;
            posY += moveY;
            posX += moveX;
        }
    }
}

void PaperRoller_Unknown2(void)
{
    RSDK_THIS(PaperRoller);

    int x1 = entity->position.x - (RSDK.Cos256(entity->angle) << 7) * entity->length;
    int y1 = entity->position.y - (RSDK.Sin256(entity->angle) << 7) * entity->length;
    int x2 = (RSDK.Cos256(entity->angle) << 7) * entity->length + entity->position.x;
    int y2 = (RSDK.Sin256(entity->angle) << 7) * entity->length + entity->position.y;

    int len   = entity->length / 4 + (entity->length % 4 > 0);
    int posX1 = x1 + 0x1800 * RSDK.Cos256(entity->angle - 64);
    int posY1 = y1 + 0x1800 * RSDK.Sin256(entity->angle - 64);
    int posX2 = x2 + 0x1800 * RSDK.Cos256(entity->angle - 64);
    int posY2 = y2 + 0x1800 * RSDK.Sin256(entity->angle - 64);

    int altPosX1 = x2 + 0x1800 * RSDK.Cos256(entity->angle + 64);
    int altPosY1 = y2 + 0x1800 * RSDK.Sin256(entity->angle + 64);
    int altPosX2 = x1 + 0x1800 * RSDK.Cos256(entity->angle + 64);
    int altPosY2 = y1 + 0x1800 * RSDK.Sin256(entity->angle + 64);

    PaperRoller_Unknown1(0x000000, 0, posX1, posY1, posX2, posY2, 0, 0, entity->field_8C.x, entity->field_8C.y);
    PaperRoller_Unknown1(0x000000, len, altPosX1, altPosY1, altPosX2, altPosY2, 0, 0, entity->field_94.x, entity->field_94.y);

    int ang = entity->angle + 32;
    if (ang < 0)
        ang = ((-1 - (entity->angle + 32)) & 0xFFFFFF00) + entity->angle + 288;

    int otherPosY1 = 0, otherPosY2 = 0, otherPosX2 = 0, otherPosX1 = 0;
    switch (ang >> 6) {
        case 0:
            otherPosY1 = 0x10000;
            otherPosY2 = -0x10000;
            break;
        case 1:
            otherPosX1 = -0x10000;
            otherPosX2 = 0x10000;
            break;
        case 2:
            otherPosY1 = -0x10000;
            otherPosY2 = 0x10000;
            break;
        case 3:
            otherPosX1 = 0x10000;
            otherPosX2 = -0x10000;
            break;
    }
    PaperRoller_Unknown1(0x000000, 0, posX1, posY1, posX2, posY2, otherPosX1, otherPosY1, entity->field_8C.x, entity->field_8C.y);
    PaperRoller_Unknown1(0x000000, len, altPosX1, altPosY1, altPosX2, altPosY2, otherPosX2, otherPosY2, entity->field_94.x, entity->field_94.y);
    PaperRoller_Unknown1(0xD0B898, 0, posX1, posY1, posX2, posY2, 2 * otherPosX1, 2 * otherPosY1, entity->field_8C.x, entity->field_8C.y);
    PaperRoller_Unknown1(0xD0B898, len, altPosX1, altPosY1, altPosX2, altPosY2, 2 * otherPosX2, 2 * otherPosY2, entity->field_94.x,
                         entity->field_94.y);
}

void PaperRoller_Unknown3(void)
{
    RSDK_THIS(PaperRoller);
    Vector2 drawPos;

    drawPos = entity->position;
    drawPos.x -= (RSDK.Cos256(entity->angle) << 7) * entity->length;
    drawPos.y -= (RSDK.Sin256(entity->angle) << 7) * entity->length;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x += -0x900 * RSDK.Cos256(entity->angleVal);
    drawPos.y += -0x900 * RSDK.Sin256(entity->angleVal);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.x += 0x900 * RSDK.Cos256(entity->angleVal);
    drawPos.y += 0x900 * RSDK.Sin256(entity->angleVal);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos = entity->position;
    drawPos.x += (RSDK.Cos256(entity->angle) << 7) * entity->length;
    drawPos.y += (RSDK.Sin256(entity->angle) << 7) * entity->length;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x += -0x900 * RSDK.Cos256(entity->angleVal);
    drawPos.y += -0x900 * RSDK.Sin256(entity->angleVal);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.x += 0x900 * RSDK.Cos256(entity->angleVal);
    drawPos.y += 0x900 * RSDK.Sin256(entity->angleVal);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void PaperRoller_Unknown4(void)
{
    RSDK_THIS(PaperRoller);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (entity->playerTimer[playerID] <= 0) {
            int distX = abs(entity->position.x - player->position.x);
            int distY = abs(entity->position.y - player->position.y);
            if (MathHelpers_Unknown6((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16)) <= 40 && !entity->playerTimer[playerID]) {
                RSDK.PlaySFX(Player->sfx_Release, false, 255);
                int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);

                int ang = 0;
                if (entity->direction == FLIP_NONE)
                    ang = angle + 64;
                else
                    ang = angle - 64;

                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                player->position.x += 0x2800 * RSDK.Cos256(angle);
                player->position.y += 0x2800 * RSDK.Sin256(angle);
                player->velocity.x = RSDK.Cos256(ang) << 11;
                player->velocity.y = RSDK.Sin256(ang) << 11;
                player->state      = Player_State_Air;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                player->nextAirState           = 0;
                player->nextGroundState        = 0;
                player->onGround               = false;
                player->groundVel              = 0;
                player->tileCollisions         = true;
                player->jumpAbility            = 0;
                player->jumpAbilityTimer       = 0;
                entity->playerTimer[playerID] = 10;
            }
        }
        else {
            entity->playerTimer[playerID]--;
        }
    }
}

void PaperRoller_Unknown5(void)
{
    RSDK_THIS(PaperRoller);
    bool32 flagX = false;
    bool32 flagY = false;

    int negAngle       = -entity->angle;
    entity->field_8C.x = 0;
    entity->field_8C.y = 0;
    entity->field_94.x = 0;
    entity->field_94.y = 0;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (player->jumpPress)
            entity->playerUnknown[playerID] = 0;
        else
            ++entity->playerUnknown[playerID];

        int distX = (player->position.x - entity->position.x) >> 8;
        int distY = (player->position.y - entity->position.y) >> 8;
        int angX  = distY * RSDK.Sin256(entity->angle) + distX * RSDK.Cos256(entity->angle) + entity->position.x;
        int angY  = distY * RSDK.Cos256(entity->angle) - distX * RSDK.Sin256(entity->angle) + entity->position.y;

        int angDistX = angX - entity->position.x;
        int angDistY = angY - entity->position.y;
        if (abs(angDistX) <= entity->length << 15 && abs(angDistY) <= 0x280000) {
            int valY = 0;
            if (angDistY < 0) {
                if (abs(angDistY) > 0x180000) {
                    valY = angDistY + 0x280000;
                }
                else {
                    Vector2 pos;
                    pos.x = entity->position.x + angDistX;
                    pos.y = entity->position.y - 0x180000;
                    Zone_Unknown3(&entity->position, &pos, negAngle);

                    Vector2 pos2;
                    pos2.x           = 0;
                    pos2.y           = 0;
                    player->position = pos;

                    Vector2 vel = player->velocity;
                    Zone_Unknown3(&pos2, &vel, entity->angle);
                    int angle = RSDK.ATan2(vel.x, -vel.y);

                    int force = 6;
                    if (player->jumpHold)
                        force = 12;

                    vel.x = force * (RSDK.Cos256(angle) << 8);
                    vel.y = force * (RSDK.Sin256(angle) << 8);
                    if (abs(vel.x) < 0x10000)
                        vel.x += ((2 * (entity->direction == FLIP_NONE) - 1) << 16);
                    Zone_Unknown3(&pos2, &vel, negAngle);
                    player->velocity = vel;

                    player->state = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->nextAirState     = 0;
                    player->nextGroundState  = 0;
                    player->onGround         = false;
                    player->groundVel        = 0;
                    player->tileCollisions   = true;
                    player->jumpAbility      = 0;
                    player->jumpAbilityTimer = 0;
                    RSDK.PlaySFX(PaperRoller->sfxPaper, false, 255);
                    valY = 0x100000;
                }

                if (player->sidekick) {
                    if (!flagX) {
                        entity->field_8C.x = angDistX;
                        entity->field_8C.y = valY;
                    }
                }
                else {
                    entity->field_8C.x = angDistX;
                    entity->field_8C.y = valY;
                    flagX              = true;
                }
            }
            else {
                if (abs(angDistY) > 0x180000) {
                    valY = angDistY - 0x280000;
                }
                else {
                    Vector2 pos;
                    pos.x = entity->position.x + angDistX;
                    pos.y = entity->position.y + 0x180000;
                    Zone_Unknown3(&entity->position, &pos, negAngle);

                    Vector2 pos2;
                    pos2.x           = 0;
                    pos2.y           = 0;
                    player->position = pos;

                    Vector2 vel = player->velocity;
                    Zone_Unknown3(&pos2, &vel, entity->angle);

                    int angle = RSDK.ATan2(vel.x, -vel.y);
                    int force = 6;
                    if (player->jumpHold)
                        force = 12;

                    vel.x = force * (RSDK.Cos256(angle) << 8);
                    vel.y = force * (RSDK.Sin256(angle) << 8);
                    if ((entity->direction == FLIP_NONE && vel.x > -0x10000) || (entity->direction == FLIP_X && vel.x < 0x10000))
                        vel.x += ((2 * (entity->direction != FLIP_NONE) - 1) << 18);
                    Zone_Unknown3(&pos2, &vel, negAngle);
                    player->velocity = vel;

                    player->state = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->nextAirState     = StateMachine_None;
                    player->nextGroundState  = StateMachine_None;
                    player->onGround         = false;
                    player->groundVel        = 0;
                    player->tileCollisions   = true;
                    player->jumpAbility      = 0;
                    player->jumpAbilityTimer = 0;
                    RSDK.PlaySFX(PaperRoller->sfxPaper, false, 255);
                    valY = -0x100000;
                }

                if (!player->sidekick) {
                    flagY              = true;
                    entity->field_94.y = valY;
                    entity->field_94.x = angDistX;
                }
                else if (!flagY) {
                    entity->field_94.y = valY;
                    entity->field_94.x = angDistX;
                }
            }
        }
    }
}

void PaperRoller_EditorDraw(void) {}

void PaperRoller_EditorLoad(void) {}

void PaperRoller_Serialize(void)
{
    RSDK_EDITABLE_VAR(PaperRoller, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PaperRoller, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(PaperRoller, VAR_ENUM, angle);
}
