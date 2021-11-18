#include "SonicMania.h"

ObjectPaperRoller *PaperRoller;

void PaperRoller_Update(void)
{
    RSDK_THIS(PaperRoller);
    RSDK.ProcessAnimation(&self->animator1);

    int32 x1 = self->position.x - (RSDK.Cos256(self->angle) << 7) * self->length;
    int32 v3 = RSDK.Sin256(self->angle);
    int32 y1 = self->position.y - (v3 << 7) * self->length;
    int32 x2 = (RSDK.Cos256(self->angle) << 7) * self->length + self->position.x;
    int32 y2 = (RSDK.Sin256(self->angle) << 7) * self->length + self->position.y;
    int32 x3 = self->position.x;
    int32 y3 = self->position.y;

    self->position.x = x1;
    self->position.y = y1;
    PaperRoller_Unknown4();
    self->position.x = x2;
    self->position.y = y2;
    PaperRoller_Unknown4();
    self->position.x = x3;
    self->position.y = y3;
    PaperRoller_Unknown5();

    if (self->direction == FLIP_NONE)
        self->angleVal += 6;
    else
        self->angleVal += -6;

    if (self->angleVal < 0)
        self->angleVal += 256;
    self->angleVal = self->angleVal;
}

void PaperRoller_LateUpdate(void) {}

void PaperRoller_StaticUpdate(void) {}

void PaperRoller_Draw(void)
{
    PaperRoller_DrawPaperLines();
    PaperRoller_DrawRollers();
}

void PaperRoller_Create(void *data)
{
    RSDK_THIS(PaperRoller);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_NONE;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if ((RSDK.Cos256(self->angle) << 8) * self->length >= 0)
        self->updateRange.x += (RSDK.Cos256(self->angle) << 8) * self->length;
    else
        self->updateRange.x += -((RSDK.Cos256(self->angle) << 8) * self->length);

    if ((RSDK.Sin256(self->angle) << 8) * self->length >= 0)
        self->updateRange.y += (RSDK.Sin256(self->angle) << 8) * self->length;
    else
        self->updateRange.y += -((RSDK.Sin256(self->angle) << 8) * self->length);

    if (self->length > 256)
        self->length = 256;

    self->playerUnknown[0] = 0x7FFF;
    self->playerUnknown[1] = 0x7FFF;
    self->playerUnknown[2] = 0x7FFF;
    self->playerUnknown[3] = 0x7FFF;
    self->hitbox.left     = -24;
    self->hitbox.top      = -24;
    self->hitbox.right    = 24;
    self->hitbox.bottom   = 24;
    RSDK.SetSpriteAnimation(PaperRoller->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(PaperRoller->aniFrames, 1, &self->animator2, true, 0);
}

void PaperRoller_StageLoad(void)
{
    uint32 colours[]         = { 0x0F0F0E8, 0x0D0B898, 0x987870, 0x586868 };
    PaperRoller->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PaperRoller.bin", SCOPE_STAGE);
    for (int32 i = 0; i < 0x40; ++i) PaperRoller->colours[i] = colours[i & 3];
    PaperRoller->sfxPaper = RSDK.GetSFX("PSZ/Paper.wav");
}

void PaperRoller_DrawLineDeform(uint32 colour, int32 len, int32 posX1, int32 posY1, int32 posX2, int32 posY2, int32 posX3, int32 posY3, int32 posX4, int32 posY4)
{
    RSDK_THIS(PaperRoller);

    int32 count = (self->length / 4) + (self->length % 4 > 0);
    if (count) {
        int32 negAngle = -(uint8)(self->angle);

        int32 posX     = posX1;
        int32 posY     = posY1;
        int32 moveX = (posX2 - posX1) / count;
        int32 moveY = (posY2 - posY1) / count;

        for (int32 i = 0; i < count; ++i) {
            int32 clrOffset = 0;
            if (self->direction)
                clrOffset = len % count;
            else
                clrOffset = count - len % count - 1;
            uint32 lineClr = PaperRoller->colours[(Zone->timer + clrOffset) % count];

            if (!posY4) {
                RSDK.DrawLine(posX3 + posX, posY3 + posY, posX3 + (posX + moveX), posY3 + (posY + moveY), colour ? colour : lineClr, 127,
                              INK_NONE, false);
            }
            else {
                int32 distX = (posX - self->position.x) >> 8;
                int32 distY = (posY - self->position.y) >> 8;

                int32 angValX = distY * RSDK.Sin256(self->angle) + distX * RSDK.Cos256(self->angle);

                int32 angValY = self->position.y - distX * RSDK.Sin256(self->angle) + distY * RSDK.Cos256(self->angle) - self->position.y;
                int32 lenY    = 0;

                if (angValX >= posX4) {
                    int32 length = (self->length << 15) - posX4;
                    if (length < 0)
                        length = posX4 - (self->length << 15);
                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * ((length + posX4 - angValX) >> 8) / (length >> 16);
                }
                else {
                    int32 length = 0;
                    if ((self->length << 15) + posX4 >= 0)
                        length = (self->length << 15) + posX4;
                    else
                        length = -(posX4 + (self->length << 15));

                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * (((self->length << 15) + angValX) >> 8) / (length >> 16);
                }

                distX = angValX >> 8;
                distY = (lenY + angValY) >> 8;

                int32 offsetX1 = self->position.x + distY * RSDK.Sin256(negAngle) + distX * RSDK.Cos256(negAngle);
                int32 offsetY1 = self->position.y - distX * RSDK.Sin256(negAngle) + distY * RSDK.Cos256(negAngle);

                distX = ((posX + moveX) - self->position.x) >> 8;
                distY = ((posY + moveY) - self->position.y) >> 8;

                angValX = distY * RSDK.Sin256(self->angle) + distX * RSDK.Cos256(self->angle);
                angValY = self->position.y - distX * RSDK.Sin256(self->angle) + distY * RSDK.Cos256(self->angle) - self->position.y;

                lenY = 0;
                if (angValX >= posX4) {
                    int32 length = (self->length << 15) - posX4;
                    if (length < 0)
                        length = posX4 - (self->length << 15);

                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * ((length + posX4 - angValX) >> 8) / (length >> 16);
                }
                else {
                    int32 length = 0;
                    if ((self->length << 15) + posX4 >= 0)
                        length = (self->length << 15) + posX4;
                    else
                        length = -(posX4 + (self->length << 15));

                    if (length >> 16 > 0)
                        lenY = (posY4 >> 8) * (((self->length << 15) + angValX) >> 8) / (length >> 16);
                }

                distX        = angValX >> 8;
                distY        = (lenY + angValY) >> 8;
                int32 offsetX2 = self->position.x + distY * RSDK.Sin256(negAngle) + distX * RSDK.Cos256(negAngle);
                int32 offsetY2 = self->position.y - distX * RSDK.Sin256(negAngle) + distY * RSDK.Cos256(negAngle);
                RSDK.DrawLine(posX3 + offsetX1, posY3 + offsetY1, posX3 + offsetX2, posY3 + offsetY2, colour ? colour : lineClr, 127, INK_NONE,
                              false);
            }
            ++len;
            posY += moveY;
            posX += moveX;
        }
    }
}

void PaperRoller_DrawPaperLines(void)
{
    RSDK_THIS(PaperRoller);

    int32 x1 = self->position.x - (RSDK.Cos256(self->angle) << 7) * self->length;
    int32 y1 = self->position.y - (RSDK.Sin256(self->angle) << 7) * self->length;
    int32 x2 = (RSDK.Cos256(self->angle) << 7) * self->length + self->position.x;
    int32 y2 = (RSDK.Sin256(self->angle) << 7) * self->length + self->position.y;

    int32 len   = self->length / 4 + (self->length % 4 > 0);
    int32 posX1 = x1 + 0x1800 * RSDK.Cos256(self->angle - 64);
    int32 posY1 = y1 + 0x1800 * RSDK.Sin256(self->angle - 64);
    int32 posX2 = x2 + 0x1800 * RSDK.Cos256(self->angle - 64);
    int32 posY2 = y2 + 0x1800 * RSDK.Sin256(self->angle - 64);

    int32 altPosX1 = x2 + 0x1800 * RSDK.Cos256(self->angle + 64);
    int32 altPosY1 = y2 + 0x1800 * RSDK.Sin256(self->angle + 64);
    int32 altPosX2 = x1 + 0x1800 * RSDK.Cos256(self->angle + 64);
    int32 altPosY2 = y1 + 0x1800 * RSDK.Sin256(self->angle + 64);

    PaperRoller_DrawLineDeform(0x000000, 0, posX1, posY1, posX2, posY2, 0, 0, self->field_8C.x, self->field_8C.y);
    PaperRoller_DrawLineDeform(0x000000, len, altPosX1, altPosY1, altPosX2, altPosY2, 0, 0, self->field_94.x, self->field_94.y);

    int32 ang = self->angle + 32;
    if (ang < 0)
        ang = ((-1 - (self->angle + 32)) & 0xFFFFFF00) + self->angle + 288;

    int32 otherPosY1 = 0, otherPosY2 = 0, otherPosX2 = 0, otherPosX1 = 0;
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
    PaperRoller_DrawLineDeform(0x000000, 0, posX1, posY1, posX2, posY2, otherPosX1, otherPosY1, self->field_8C.x, self->field_8C.y);
    PaperRoller_DrawLineDeform(0x000000, len, altPosX1, altPosY1, altPosX2, altPosY2, otherPosX2, otherPosY2, self->field_94.x, self->field_94.y);
    PaperRoller_DrawLineDeform(0xD0B898, 0, posX1, posY1, posX2, posY2, 2 * otherPosX1, 2 * otherPosY1, self->field_8C.x, self->field_8C.y);
    PaperRoller_DrawLineDeform(0xD0B898, len, altPosX1, altPosY1, altPosX2, altPosY2, 2 * otherPosX2, 2 * otherPosY2, self->field_94.x,
                         self->field_94.y);
}

void PaperRoller_DrawRollers(void)
{
    RSDK_THIS(PaperRoller);
    Vector2 drawPos;

    drawPos = self->position;
    drawPos.x -= (RSDK.Cos256(self->angle) << 7) * self->length;
    drawPos.y -= (RSDK.Sin256(self->angle) << 7) * self->length;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.x += -0x900 * RSDK.Cos256(self->angleVal);
    drawPos.y += -0x900 * RSDK.Sin256(self->angleVal);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    drawPos.x += 0x900 * RSDK.Cos256(self->angleVal);
    drawPos.y += 0x900 * RSDK.Sin256(self->angleVal);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    drawPos = self->position;
    drawPos.x += (RSDK.Cos256(self->angle) << 7) * self->length;
    drawPos.y += (RSDK.Sin256(self->angle) << 7) * self->length;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.x += -0x900 * RSDK.Cos256(self->angleVal);
    drawPos.y += -0x900 * RSDK.Sin256(self->angleVal);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    drawPos.x += 0x900 * RSDK.Cos256(self->angleVal);
    drawPos.y += 0x900 * RSDK.Sin256(self->angleVal);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
}

void PaperRoller_Unknown4(void)
{
    RSDK_THIS(PaperRoller);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (self->playerTimer[playerID] <= 0) {
            int32 distX = abs(self->position.x - player->position.x);
            int32 distY = abs(self->position.y - player->position.y);
            if (MathHelpers_SquareRoot((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16)) <= 40 && !self->playerTimer[playerID]) {
                RSDK.PlaySfx(Player->sfxRelease, false, 255);
                int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

                int32 ang = 0;
                if (self->direction == FLIP_NONE)
                    ang = angle + 64;
                else
                    ang = angle - 64;

                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->position.x += 0x2800 * RSDK.Cos256(angle);
                player->position.y += 0x2800 * RSDK.Sin256(angle);
                player->velocity.x = RSDK.Cos256(ang) << 11;
                player->velocity.y = RSDK.Sin256(ang) << 11;
                player->state      = Player_State_Air;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                player->nextAirState           = 0;
                player->nextGroundState        = 0;
                player->onGround               = false;
                player->groundVel              = 0;
                player->tileCollisions         = true;
                player->jumpAbility            = 0;
                player->jumpAbilityTimer       = 0;
                self->playerTimer[playerID] = 10;
            }
        }
        else {
            self->playerTimer[playerID]--;
        }
    }
}

void PaperRoller_Unknown5(void)
{
    RSDK_THIS(PaperRoller);
    bool32 flagX = false;
    bool32 flagY = false;

    int32 negAngle       = -self->angle;
    self->field_8C.x = 0;
    self->field_8C.y = 0;
    self->field_94.x = 0;
    self->field_94.y = 0;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (player->jumpPress)
            self->playerUnknown[playerID] = 0;
        else
            ++self->playerUnknown[playerID];

        int32 distX = (player->position.x - self->position.x) >> 8;
        int32 distY = (player->position.y - self->position.y) >> 8;
        int32 angX  = distY * RSDK.Sin256(self->angle) + distX * RSDK.Cos256(self->angle) + self->position.x;
        int32 angY  = distY * RSDK.Cos256(self->angle) - distX * RSDK.Sin256(self->angle) + self->position.y;

        int32 angDistX = angX - self->position.x;
        int32 angDistY = angY - self->position.y;
        if (abs(angDistX) <= self->length << 15 && abs(angDistY) <= 0x280000) {
            int32 valY = 0;
            if (angDistY < 0) {
                if (abs(angDistY) > 0x180000) {
                    valY = angDistY + 0x280000;
                }
                else {
                    Vector2 pos;
                    pos.x = self->position.x + angDistX;
                    pos.y = self->position.y - 0x180000;
                    Zone_Unknown3(&self->position, &pos, negAngle);

                    Vector2 pos2;
                    pos2.x           = 0;
                    pos2.y           = 0;
                    player->position = pos;

                    Vector2 vel = player->velocity;
                    Zone_Unknown3(&pos2, &vel, self->angle);
                    int32 angle = RSDK.ATan2(vel.x, -vel.y);

                    int32 force = 6;
                    if (player->jumpHold)
                        force = 12;

                    vel.x = force * (RSDK.Cos256(angle) << 8);
                    vel.y = force * (RSDK.Sin256(angle) << 8);
                    if (abs(vel.x) < 0x10000)
                        vel.x += ((2 * (self->direction == FLIP_NONE) - 1) << 16);
                    Zone_Unknown3(&pos2, &vel, negAngle);
                    player->velocity = vel;

                    player->state = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->nextAirState     = 0;
                    player->nextGroundState  = 0;
                    player->onGround         = false;
                    player->groundVel        = 0;
                    player->tileCollisions   = true;
                    player->jumpAbility      = 0;
                    player->jumpAbilityTimer = 0;
                    RSDK.PlaySfx(PaperRoller->sfxPaper, false, 255);
                    valY = 0x100000;
                }

                if (player->sidekick) {
                    if (!flagX) {
                        self->field_8C.x = angDistX;
                        self->field_8C.y = valY;
                    }
                }
                else {
                    self->field_8C.x = angDistX;
                    self->field_8C.y = valY;
                    flagX              = true;
                }
            }
            else {
                if (abs(angDistY) > 0x180000) {
                    valY = angDistY - 0x280000;
                }
                else {
                    Vector2 pos;
                    pos.x = self->position.x + angDistX;
                    pos.y = self->position.y + 0x180000;
                    Zone_Unknown3(&self->position, &pos, negAngle);

                    Vector2 pos2;
                    pos2.x           = 0;
                    pos2.y           = 0;
                    player->position = pos;

                    Vector2 vel = player->velocity;
                    Zone_Unknown3(&pos2, &vel, self->angle);

                    int32 angle = RSDK.ATan2(vel.x, -vel.y);
                    int32 force = 6;
                    if (player->jumpHold)
                        force = 12;

                    vel.x = force * (RSDK.Cos256(angle) << 8);
                    vel.y = force * (RSDK.Sin256(angle) << 8);
                    if ((self->direction == FLIP_NONE && vel.x > -0x10000) || (self->direction == FLIP_X && vel.x < 0x10000))
                        vel.x += ((2 * (self->direction != FLIP_NONE) - 1) << 18);
                    Zone_Unknown3(&pos2, &vel, negAngle);
                    player->velocity = vel;

                    player->state = Player_State_Air;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->nextAirState     = StateMachine_None;
                    player->nextGroundState  = StateMachine_None;
                    player->onGround         = false;
                    player->groundVel        = 0;
                    player->tileCollisions   = true;
                    player->jumpAbility      = 0;
                    player->jumpAbilityTimer = 0;
                    RSDK.PlaySfx(PaperRoller->sfxPaper, false, 255);
                    valY = -0x100000;
                }

                if (!player->sidekick) {
                    flagY              = true;
                    self->field_94.y = valY;
                    self->field_94.x = angDistX;
                }
                else if (!flagY) {
                    self->field_94.y = valY;
                    self->field_94.x = angDistX;
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PaperRoller_EditorDraw(void) {}

void PaperRoller_EditorLoad(void) {}
#endif

void PaperRoller_Serialize(void)
{
    RSDK_EDITABLE_VAR(PaperRoller, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PaperRoller, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(PaperRoller, VAR_ENUM, angle);
}
