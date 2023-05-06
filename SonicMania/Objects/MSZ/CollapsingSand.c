// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CollapsingSand Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCollapsingSand *CollapsingSand;

void CollapsingSand_Update(void)
{
    RSDK_THIS(CollapsingSand);

    self->visible = DebugMode->debugActive;

    StateMachine_Run(self->state);
}

void CollapsingSand_LateUpdate(void) {}

void CollapsingSand_StaticUpdate(void) {}

void CollapsingSand_Draw(void)
{
    RSDK_THIS(CollapsingSand);
    Vector2 drawPos;

    // Draw Outline
    drawPos.x = self->position.x - (self->size.x >> 1);
    drawPos.y = self->position.y - (self->size.y >> 1);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + self->size.y, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y + self->size.y, 0xE0E0E0, 0, INK_NONE, false);

    // Draw Corners
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);

    drawPos.x += self->size.x;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);

    drawPos.y += self->size.y;
    self->direction = FLIP_XY;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);

    drawPos.x -= self->size.x;
    self->direction = FLIP_Y;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);
}

void CollapsingSand_Create(void *data)
{
    RSDK_THIS(CollapsingSand);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->position.x &= 0xFFF80000;
    self->position.y &= 0xFFF80000;

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->hitbox.left   = -(self->size.x >> 17);
        self->hitbox.top    = -48 - (self->size.y >> 17);
        self->hitbox.right  = self->size.x >> 17;
        self->hitbox.bottom = self->size.y >> 17;
        self->state         = CollapsingSand_State_CheckPlayerCollisions;
    }
}

void CollapsingSand_StageLoad(void)
{
    CollapsingSand->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SandCollapse.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &CollapsingSand->animator, true, 0);

    CollapsingSand->sfxSandFall = RSDK.GetSfx("MSZ/SandFall.wav");
}

void CollapsingSand_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(CollapsingSand);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && player->onGround && player->collisionMode) {
            self->state        = CollapsingSand_State_CollapseDelay;
            self->collapseLeft = player->position.x <= self->position.x;
            foreach_break;
        }
    }
}

void CollapsingSand_State_CollapseDelay(void)
{
    RSDK_THIS(CollapsingSand);

    if (self->collapseTimer >= self->delay) {
        self->collapseDuration.x = self->size.x >> 20;
        self->collapseDuration.y = self->size.y >> 20;
        self->collapseTimer      = 0;

        if (!self->collapseLeft) {
            self->state     = CollapsingSand_State_CollapseRight;
            self->tilePos.x = ((self->size.x >> 1) + self->position.x - 0x80000) >> 20;
        }
        else {
            self->state     = CollapsingSand_State_CollapseLeft;
            self->tilePos.x = (self->position.x - (self->size.x >> 1) + 0x80000) >> 20;
        }
        RSDK.PlaySfx(CollapsingSand->sfxSandFall, false, 255);

        self->tilePos.y    = ((self->size.y >> 1) + self->position.y - 0x80000) >> 20;
        self->tileEndPos.x = self->tilePos.x;
        self->tileEndPos.y = (self->position.y - (self->size.y >> 1) + 0x80000) >> 20;
    }
    else {
        self->collapseTimer++;
    }
}

void CollapsingSand_State_CollapseLeft(void)
{
    RSDK_THIS(CollapsingSand);

    if (self->collapseTimer) {
        self->collapseTimer = (self->collapseTimer + 1) & 3;
    }
    else {
        if (self->collapseDuration.x > 0 || self->collapseDuration.y > 0) {
            int32 tx = self->tilePos.x;
            int32 ty = self->tilePos.y;

            for (int32 x = (tx << 20) + 0x80000, y = (ty << 20) + 0x80000; tx >= self->tileEndPos.x && ty >= self->tileEndPos.y; --tx, --ty) {
                bool32 spawnSand = false;

                uint16 tile = RSDK.GetTile(Zone->fgLayer[0], tx, ty);
                if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                    RSDK.SetTile(Zone->fgLayer[0], tx, ty, -1);
                    spawnSand = true;
                }

                tile = RSDK.GetTile(Zone->fgLayer[1], tx, ty);
                if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                    RSDK.SetTile(Zone->fgLayer[1], tx, ty, -1);
                    spawnSand = true;
                }

                if (spawnSand) {
                    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                    RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                    debris->drawGroup = Zone->objectDrawGroup[1];
                    debris->timer     = 44;
                    if (self->collapseDuration.x > 0) {
                        // Replace ground tile with a flat one
                        if (ty == self->tilePos.y)
                            RSDK.SetTile(Zone->fgLayer[0], tx, ty, (168 + (self->collapseDuration.x & 1)) | 0b1111000000000000);
                    }
                }

                x -= 0x100000;
                y -= 0x100000;
            }

            if (self->collapseDuration.x > 0) {
                ++self->tilePos.x;
                --self->collapseDuration.x;
            }
            else {
                --self->tilePos.y;
                --self->collapseDuration.y;
            }
            self->collapseTimer = (self->collapseTimer + 1) & 3;
        }
        else {
            foreach_all(ItemBox, itembox)
            {
                if (abs(itembox->position.x - self->position.x) < self->size.x >> 1) {
                    if (abs(itembox->position.y - self->position.y >= 0) < self->size.y >> 1 && itembox->state == ItemBox_State_Idle)
                        itembox->state = ItemBox_State_Falling;
                }
            }

            destroyEntity(self);
        }
    }
}

void CollapsingSand_State_CollapseRight(void)
{
    RSDK_THIS(CollapsingSand);

    if (self->collapseTimer) {
        self->collapseTimer = (self->collapseTimer + 1) & 3;
    }
    else {
        if (self->collapseDuration.x > 0 || self->collapseDuration.y > 0) {
            int32 tx = self->tilePos.x;
            int32 ty = self->tilePos.y;

            for (int32 x = (tx << 20) + 0x80000, y = (ty << 20) + 0x80000; tx <= self->tileEndPos.x && ty >= self->tileEndPos.y; ++tx, --ty) {
                bool32 spawnSand = false;

                uint16 tile = RSDK.GetTile(Zone->fgLayer[0], tx, ty);
                if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                    RSDK.SetTile(Zone->fgLayer[0], tx, ty, -1);
                    spawnSand = true;
                }

                tile = RSDK.GetTile(Zone->fgLayer[1], tx, ty);
                if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                    RSDK.SetTile(Zone->fgLayer[1], tx, ty, -1);
                    spawnSand = true;
                }

                if (spawnSand) {
                    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                    RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                    debris->drawGroup = Zone->objectDrawGroup[1];
                    debris->timer     = 44;
                    if (self->collapseDuration.x > 0) {
                        // Replace ground tile with a flat one
                        if (ty == self->tilePos.y)
                            RSDK.SetTile(Zone->fgLayer[0], tx, ty, (169 - (self->collapseDuration.x & 1)) | 0b1111000000000000);
                    }
                }

                x += 0x100000;
                y -= 0x100000;
            }

            if (self->collapseDuration.x > 0) {
                --self->tilePos.x;
                --self->collapseDuration.x;
            }
            else {
                --self->tilePos.y;
                --self->collapseDuration.y;
            }

            self->collapseTimer = (self->collapseTimer + 1) & 3;
        }
        else {
            foreach_all(ItemBox, itembox)
            {
                if (abs(itembox->position.x - self->position.x) < self->size.x >> 1) {
                    if (abs(itembox->position.y - self->position.y >= 0) < self->size.y >> 1 && itembox->state == ItemBox_State_Idle)
                        itembox->state = ItemBox_State_Falling;
                }
            }
            destroyEntity(self);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void CollapsingSand_EditorDraw(void) { CollapsingSand_Draw(); }

void CollapsingSand_EditorLoad(void)
{
    CollapsingSand->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SandCollapse.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &CollapsingSand->animator, true, 0);
}
#endif

void CollapsingSand_Serialize(void)
{
    RSDK_EDITABLE_VAR(CollapsingSand, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(CollapsingSand, VAR_ENUM, delay);
}
