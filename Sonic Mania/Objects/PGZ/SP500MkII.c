#include "SonicMania.h"

ObjectSP500MkII *SP500MkII;

void SP500MkII_Update(void)
{
    RSDK_THIS(SP500MkII);
    self->pos2.x = self->position.x + self->field_98;
    self->pos2.y = self->position.y + self->field_9C;

    StateMachine_Run(self->state);

    self->field_74 = false;
    self->field_70 = false;
    if (self->field_7C && self->printRowID < self->printRows)
        self->field_70 = true;
    if (self->printStarted && !self->field_7C && self->state != SP500MkII_Unknown8 && self->printRowID < self->printRows)
        self->field_74 = true;
    RSDK.ProcessAnimation(&self->animator);
}

void SP500MkII_LateUpdate(void) {}

void SP500MkII_StaticUpdate(void) {}

void SP500MkII_Draw(void)
{
    SP500MkII_Unknown3();
    SP500MkII_Unknown2();
    SP500MkII_Unknown1();
}

void SP500MkII_Create(void *data)
{
    RSDK_THIS(SP500MkII);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->pos           = self->position;
    self->drawFX        = FX_FLIP;
    self->visible       = true;
    self->updateRange.x = (self->length + 16) << 19;
    self->updateRange.y = (self->printRows + 4) << 21;
    self->state         = SP500MkII_Unknown5;
}

void SP500MkII_StageLoad(void)
{
    SP500MkII->aniFrames      = RSDK.LoadSpriteAnimation("PSZ1/SP500MkII.bin", SCOPE_STAGE);
    SP500MkII->hitbox1.top    = -24;
    SP500MkII->hitbox1.left   = -32;
    SP500MkII->hitbox1.right  = 32;
    SP500MkII->hitbox1.bottom = 23;
    SP500MkII->hitbox2.top    = -38;
    SP500MkII->hitbox2.left   = -32;
    SP500MkII->hitbox2.right  = 32;
    SP500MkII->hitbox2.bottom = -24;
    SP500MkII->hitbox3.top    = 24;
    SP500MkII->hitbox3.left   = -23;
    SP500MkII->hitbox3.right  = 22;
    SP500MkII->hitbox3.bottom = 39;
}

void SP500MkII_Unknown1(void)
{
    RSDK_THIS(SP500MkII);
    if (SceneInfo->inEditor || DebugMode->debugActive) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;
        self->position.x += self->field_98;
        self->position.y += self->field_9C;
        DrawHelpers_DrawHitboxOutline(0xFFFF00, self->direction, self->position.x, self->position.y, &SP500MkII->hitbox2);
        DrawHelpers_DrawHitboxOutline(0xFFFFFF, self->direction, self->position.x, self->position.y, &SP500MkII->hitbox1);
        DrawHelpers_DrawHitboxOutline(0xFFFFFF, self->direction, self->position.x, self->position.y, &SP500MkII->hitbox3);
        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void SP500MkII_Unknown2(void)
{
    RSDK_THIS(SP500MkII);
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    self->position.x += self->field_98;
    self->position.y += self->field_9C;

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->field_84 + self->position.y;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 1, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.y -= self->field_84;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 0, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    if (self->field_70) {
        self->inkEffect = INK_ADD;
        self->alpha     = 255;
        RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 2, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->inkEffect = INK_NONE;
    }
    if (self->field_74) {
        self->inkEffect = INK_ADD;
        self->alpha     = 255;
        RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 3, &self->animator, true, 0);
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        self->inkEffect = INK_NONE;
    }
    self->position.x = storeX;
    self->position.y = storeY;
}

void SP500MkII_Unknown3(void)
{
    RSDK_THIS(SP500MkII);

    Vector2 drawPos;
    drawPos.x = self->position.x - 0x280000;
    drawPos.y = self->field_9C + self->position.y;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 4, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 5, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += 0x80000;
    self->animator.frameID = 1;
    for (int32 l = 0; l < self->length + 8; ++l) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += 0x80000;
    }

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += 0x80000;
    self->direction = FLIP_X;
    RSDK.SetSpriteAnimation(SP500MkII->aniFrames, 4, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->direction = FLIP_NONE;
}

void SP500MkII_CheckPlayerCollisions(void)
{
    RSDK_THIS(SP500MkII);

    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = self->pos2.x;
    self->position.y = self->pos2.y;
    int32 top            = SP500MkII->hitbox2.top << 16;

    int32 dist = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        int32 bottom   = player->position.y + (Player_GetHitbox(player)->bottom << 16);
        Player_CheckCollisionBox(player, self, &SP500MkII->hitbox3);
        int32 side        = Player_CheckCollisionBox(player, self, &SP500MkII->hitbox1);
        bool32 collided = side == C_TOP;
        bool32 touched  = Player_CheckCollisionTouch(player, self, &SP500MkII->hitbox2);

        if (collided) {
            player->position.x += self->field_A0;
            player->position.y += self->field_A4;
        }

        if (touched) {
            int32 distY = minVal(bottom - self->position.y + top, 0xD0000);

            if (dist <= distY) {
                dist = distY;
            }
            if (collided)
                self->activePlayers |= (1 << playerID);
            else
                self->activePlayers &= ~(1 << playerID);
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }

    self->field_84   = dist;
    self->printStarted = self->activePlayers != 0;
    self->position.x = storeX;
    self->position.y = storeY;
}

void SP500MkII_Unknown5(void)
{
    RSDK_THIS(SP500MkII);
    self->field_98 = 0;
    self->field_9C = 0;
    if (self->start == 1)
        self->field_98 = self->length << 19;
    else
        self->field_98 = 0;
    self->timer         = 0;
    self->activePlayers = 0;
    self->field_84      = 0;
    self->printStarted  = false;
    self->field_7C      = false;
    self->field_6C      = 2 * (self->start == 0) - 1;
    self->field_70      = false;
    self->field_74      = false;
    self->state         = SP500MkII_Unknown6;
}

void SP500MkII_Unknown6(void)
{
    RSDK_THIS(SP500MkII);
    self->field_A0 = 0;
    self->field_A4 = 0;
    SP500MkII_CheckPlayerCollisions();

    if (self->activePlayers & 1) {
        ++self->timer;
    }
    else {
        if (self->field_7C) {
            if (self->printRowID < self->printRows)
                self->state = SP500MkII_Unknown7;
        }
        self->timer = 0;
    }
    if (self->timer == 30)
        self->field_7C = 1;
}

void SP500MkII_Unknown7(void)
{
    RSDK_THIS(SP500MkII);
    self->field_A0 = self->field_98;
    self->field_A4 = self->field_9C;

    if (self->field_6C <= 0) {
        if (self->field_98 > 0)
            self->field_98 -= 0x40000;
        if (self->field_98 < 0)
            self->field_98 = 0;

        if (!self->field_98) {
            self->field_6C = 1;
            ++self->printRowID;
            self->state    = SP500MkII_Unknown8;
            self->field_7C = 0;
        }
    }
    else {
        if (self->field_98 < self->length << 19)
            self->field_98 += 0x40000;
        if (self->field_98 > self->length << 19)
            self->field_98 = self->length << 19;

        if (self->field_98 == self->length << 19) {
            self->field_6C = -1;
            ++self->printRowID;
            self->state    = SP500MkII_Unknown8;
            self->field_7C = 0;
        }
    }

    self->field_A0 = self->field_98 - self->field_A0;
    self->field_A4 = 0;
    SP500MkII_CheckPlayerCollisions();
}

void SP500MkII_Unknown8(void)
{
    RSDK_THIS(SP500MkII);
    self->field_A0 = self->field_98;
    self->field_A4 = self->field_9C;

    int32 dist = (self->printRowID * (2 * (self->yDir != 0) - 1)) << 21;
    if (self->yDir == 1) {
        if (self->field_9C < dist)
            self->field_9C += 0x40000;

        if (self->field_9C > dist)
            self->field_9C = dist;
    }
    else {
        if (self->field_9C > dist)
            self->field_9C -= 0x40000;

        if (self->field_9C < dist)
            self->field_9C = dist;
    }

    if (self->field_9C == dist)
        self->state = SP500MkII_Unknown6;
    self->field_A0 = 0;
    self->field_A4 = self->field_9C - self->field_A4;
    SP500MkII_CheckPlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void SP500MkII_EditorDraw(void)
{
    SP500MkII_Unknown3();
    SP500MkII_Unknown2();
    SP500MkII_Unknown1();
}

void SP500MkII_EditorLoad(void) { SP500MkII->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/SP500MkII.bin", SCOPE_STAGE); }
#endif

void SP500MkII_Serialize(void)
{
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT8, start);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT8, yDir);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT32, printRows);
    RSDK_EDITABLE_VAR(SP500MkII, VAR_UINT32, length);
}
