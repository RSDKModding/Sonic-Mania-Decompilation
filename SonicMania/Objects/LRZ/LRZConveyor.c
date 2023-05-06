// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZConveyor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZConveyor *LRZConveyor;

void LRZConveyor_Update(void)
{
    RSDK_THIS(LRZConveyor);

    if (self->globalControl) {
        self->off       = LRZ2Setup->conveyorOff;
        self->direction = self->startDir ^ LRZ2Setup->conveyorDir;
    }
    else {
        LRZConveyor_HandleBehaviour();
    }

    LRZConveyor_HandlePlayerInteractions();

    if (!self->off) {
        RSDK.ProcessAnimation(&self->wheelAnimator);
        RSDK.ProcessAnimation(&self->smallWheelAnimator);
    }
}

void LRZConveyor_LateUpdate(void) {}

void LRZConveyor_StaticUpdate(void) {}

void LRZConveyor_Draw(void)
{
    LRZConveyor_DrawConveyorLines();
    LRZConveyor_DrawWheels();
}

void LRZConveyor_Create(void *data)
{
    RSDK_THIS(LRZConveyor);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->updateRange.x = (self->length + 0x80) << 16;
    self->updateRange.y = (self->length * (self->slope << 8)) + 0x800000;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->isOff         = self->off;
    self->startDir      = self->direction;

    if (SceneInfo->inEditor) {
        if (!self->speed)
            self->speed = 8;
    }
    else {
        self->speed        = abs(self->speed);
        self->taggedButton = LRZ2Setup_SetupTagLink(self->buttonTag, (Entity *)self);
    }

    self->hitbox.left   = -8;
    self->hitbox.top    = -8;
    self->hitbox.right  = 8;
    self->hitbox.bottom = 8;

    RSDK.SetSpriteAnimation(LRZConveyor->aniFrames, 0, &self->wheelAnimator, true, 0);
    RSDK.SetSpriteAnimation(LRZConveyor->aniFrames, 1, &self->smallWheelAnimator, true, 0);
}

void LRZConveyor_StageLoad(void)
{
    LRZConveyor->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZConveyor.bin", SCOPE_STAGE);

    color lineColors[] = { 0x225BDD, 0x429AFD, 0x51DEFF, 0xAFFDF0 };
    for (int32 i = 0; i < 0x40; ++i) LRZConveyor->lineColors[i] = lineColors[i & 3];
}

void LRZConveyor_HandleBehaviour(void)
{
    RSDK_THIS(LRZConveyor);

    if (self->buttonTag && self->taggedButton) {
        switch (self->buttonBehavior) {
            case LRZCONVEYOR_BEHAVIOR_CHANGEACTIVE:
                if (self->taggedButton->activated)
                    self->off = self->isOff == 0;
                break;

            case LRZCONVEYOR_BEHAVIOR_CHANGEACTIVE_TOGGLE: self->off = self->taggedButton->toggled ^ self->isOff; break;

            case LRZCONVEYOR_BEHAVIOR_CHANGEDIR:
                if (self->taggedButton->activated)
                    self->direction = self->startDir == FLIP_NONE;
                break;

            case LRZCONVEYOR_BEHAVIOR_CHANGEDIR_TOGGLE: self->direction = self->startDir ^ self->taggedButton->toggled; break;

            default: break;
        }
    }
}

void LRZConveyor_HandlePlayerInteractions(void)
{
    RSDK_THIS(LRZConveyor);

    int32 velX = (self->speed << 14) * (2 * (self->direction == FLIP_X) - 1);

    foreach_active(Player, player)
    {
        int32 side = LRZConveyor_HandlePlayerCollisions(self, player, NULL);
        if (side == C_TOP && !self->off) {
            if (player->onGround)
                player->position.y += 0x10000;
            else
                player->velocity.x = velX;

            player->position.x += velX;
        }
        else if ((side == C_LEFT || side == C_RIGHT) && !self->off && !player->onGround) {
            player->velocity.x = velX;
        }
    }
}

int32 LRZConveyor_HandlePlayerCollisions(EntityLRZConveyor *conveyor, void *p, Hitbox *hitboxPtr)
{
    EntityPlayer *player = (EntityPlayer *)p;

    Hitbox hitbox;
    memset(&hitbox, 0, sizeof(hitbox));

    int32 distX = player->position.x - conveyor->position.x;

    hitbox.right = (conveyor->length + 32) >> 1;
    hitbox.left  = -(conveyor->length + 32) >> 1;

    if (abs(distX) >> 16 > conveyor->length >> 1) {
        if (abs(distX) >> 16 <= (conveyor->length >> 1) + 16) {
            int32 len = conveyor->length >> 1;
            if (distX >= 0) {
                if (distX >> 16 < len)
                    len = distX >> 16;
            }
            else {
                len = -(conveyor->length >> 1);
                if (distX >> 16 > len)
                    len = distX >> 16;
            }

            int32 slope   = (len * (conveyor->slope << 8)) >> 16;
            int32 angle   = RSDK.ATan2(distX - (conveyor->length << 15) * (((distX >> 31) & 0xFFFFFFFE) + 1), 0x100000);
            hitbox.top    = slope - (RSDK.Sin256(angle) >> 4);
            hitbox.bottom = slope + (RSDK.Sin256(angle) >> 4);
        }
    }
    else {
        int32 slope   = ((distX >> 16) * (conveyor->slope << 8)) >> 16;
        hitbox.top    = slope - 16;
        hitbox.bottom = slope + 16;
    }

    int32 storeDir      = conveyor->direction;
    conveyor->direction = FLIP_NONE;

    int32 side = C_NONE;
    if (Player_CheckValidState(player)) {
        side                = Player_CheckCollisionBox(player, conveyor, &hitbox);
        conveyor->direction = storeDir;
    }
    else {
        Hitbox playerHitbox;
        if (hitboxPtr) {
            playerHitbox = *hitboxPtr;
        }
        else {
            playerHitbox.left   = -8;
            playerHitbox.top    = -8;
            playerHitbox.right  = 8;
            playerHitbox.bottom = 8;
        }

        side                = MathHelpers_CheckBoxCollision(conveyor, &hitbox, player, &playerHitbox);
        conveyor->direction = storeDir;
    }

    return side;
}

void LRZConveyor_DrawWheels(void)
{
    RSDK_THIS(LRZConveyor);

    Vector2 drawPos = self->position;
    drawPos.x -= self->length << 15;
    drawPos.y -= (self->slope << 8) * (self->length >> 1);
    RSDK.DrawSprite(&self->wheelAnimator, &drawPos, false);

    int32 len = self->length << 16;
    if (len > 0x300000) {
        int32 size = (len - 0x300001) / 0x300000 + 1;

        if ((len - 0x300001) / 0x300000 != -1) {
            int32 incX = len / size;

            for (int32 i = 0; i < size; ++i) {
                drawPos.x += incX;
                drawPos.y += (incX >> 8) * self->slope;
                RSDK.DrawSprite(&self->smallWheelAnimator, &drawPos, false);
            }
        }
    }

    drawPos = self->position;
    drawPos.x += self->length << 15;
    drawPos.y += (self->slope << 8) * (self->length >> 1);
    RSDK.DrawSprite(&self->wheelAnimator, &drawPos, false);
}

void LRZConveyor_DrawDeformedLine(int32 startX, int32 startY, int32 endX, int32 endY, int32 offsetX, int32 offsetY, int32 len, color *color)
{
    RSDK_THIS(LRZConveyor);

    int32 count = (self->length >> 1) + ((self->length & 1) != 0);

    int32 currentX = startX;
    int32 currentY = startY;
    for (int32 i = 0; i < count; ++i) {
        int32 colorID = 0;
        if (self->direction == FLIP_X)
            colorID = count - len % count - 1;
        else
            colorID = len % count;

        if (!self->off)
            colorID += Zone->timer >> 1;

        int32 prevX = currentX;
        int32 prevY = currentY;
        currentX += (endX - startX) / count;
        currentY += (endY - startY) / count;

        uint32 lineColor = color ? *color : LRZConveyor->lineColors[(colorID % count) & 0x3F];
        RSDK.DrawLine(prevX + offsetX, prevY + offsetY, currentX + offsetX, currentY + offsetY, lineColor, 0x7F, INK_NONE, false);

        ++len;
    }
}

void LRZConveyor_DrawConveyorLines(void)
{
    RSDK_THIS(LRZConveyor);

    color color = 0x51DEFF;

    int32 slopeLen = (self->length >> 1) * (self->slope << 8);
    int32 startX   = self->position.x - (self->length << 15);
    int32 startY   = slopeLen + self->position.y + 0xF0000;
    int32 endX     = self->position.x + (self->length << 15);
    int32 endY     = slopeLen + self->position.y - 0x100000;
    int32 slopeY   = self->position.y - slopeLen;

    int32 len = (self->length >> 1) + ((self->length & 1) != 0);
    LRZConveyor_DrawDeformedLine(startX, slopeY - 0x100000, endX, endY, 0, 0, 0, NULL);
    LRZConveyor_DrawDeformedLine(endX, startY, startX, slopeY + 0xF0000, 0, 0, len, NULL);
    LRZConveyor_DrawDeformedLine(startX, slopeY - 0x100000, endX, endY, 0, 0x10000, 0, NULL);
    LRZConveyor_DrawDeformedLine(endX, startY, startX, slopeY + 0xF0000, 0, -0x10000, len, NULL);
    LRZConveyor_DrawDeformedLine(startX, slopeY - 0x100000, endX, endY, 0, 0x20000, 0, &color);
    LRZConveyor_DrawDeformedLine(endX, startY, startX, slopeY + 0xF0000, 0, -0x20000, len, &color);
}

#if GAME_INCLUDE_EDITOR
void LRZConveyor_EditorDraw(void)
{
    RSDK_THIS(LRZConveyor);

    self->startPos      = self->position;
    self->updateRange.x = (self->length + 0x80) << 16;
    self->updateRange.y = (self->length * (self->slope << 8)) + 0x800000;
    self->startDir      = self->direction;

    int32 speed = self->speed;
    self->speed = abs(self->speed);
    self->speed = speed;

    LRZConveyor_Draw();

    if (showGizmos()) {
        self->taggedButton = LRZ2Setup_SetupTagLink(self->buttonTag, (Entity *)self);

        RSDK_DRAWING_OVERLAY(true);
        if (self->taggedButton) {
            DrawHelpers_DrawArrow(self->taggedButton->position.x, self->taggedButton->position.y, self->position.x, self->position.y, 0xFFFF00,
                                  INK_NONE, 0xFF);
        }
        RSDK_DRAWING_OVERLAY(false);
    }
}

void LRZConveyor_EditorLoad(void)
{
    LRZConveyor_StageLoad();

    RSDK_ACTIVE_VAR(LRZConveyor, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);

    RSDK_ACTIVE_VAR(LRZConveyor, buttonBehavior);
    RSDK_ENUM_VAR("Change Activated", LRZCONVEYOR_BEHAVIOR_CHANGEACTIVE);
    RSDK_ENUM_VAR("Change Activated (Togglable)", LRZCONVEYOR_BEHAVIOR_CHANGEACTIVE_TOGGLE);
    RSDK_ENUM_VAR("Change Direction", LRZCONVEYOR_BEHAVIOR_CHANGEDIR);
    RSDK_ENUM_VAR("Change Direction (Togglable)", LRZCONVEYOR_BEHAVIOR_CHANGEDIR_TOGGLE);
}
#endif

void LRZConveyor_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_ENUM, slope);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_UINT32, length);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_BOOL, globalControl);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_UINT8, buttonBehavior);
    RSDK_EDITABLE_VAR(LRZConveyor, VAR_BOOL, off);
}
