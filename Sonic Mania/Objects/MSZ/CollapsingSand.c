#include "SonicMania.h"

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

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->size.x >> 1;
    drawPos.y -= self->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y - 0x10000, 0xE0E0E0u, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, self->size.y + drawPos.y, drawPos.x + self->size.x, self->size.y + drawPos.y, 0xE0E0E0u, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + self->size.y, 0xE0E0E0u, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + self->size.x, drawPos.y - 0x10000, drawPos.x + self->size.x, drawPos.y + self->size.y, 0xE0E0E0u, 0, INK_NONE,
                  false);
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
    self->drawOrder = Zone->drawOrderLow;
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

    CollapsingSand->sfxSandFall = RSDK.GetSFX("MSZ/SandFall.wav");
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
        self->field_70      = self->size.x >> 20;
        self->field_74      = self->size.y >> 20;
        self->collapseTimer = 0;
        if (!self->collapseLeft) {
            self->state = CollapsingSand_State_CollapseRight;
            self->tileX = ((self->size.x >> 1) + self->position.x - 0x80000) >> 20;
        }
        else {
            self->state = CollapsingSand_State_CollapseLeft;
            self->tileX = (self->position.x - (self->size.x >> 1) + 0x80000) >> 20;
        }
        RSDK.PlaySfx(CollapsingSand->sfxSandFall, false, 255);

        self->tileY    = ((self->size.y >> 1) + self->position.y - 0x80000) >> 20;
        self->tileMaxX = self->tileX;
        self->tileMaxY = (self->position.y - (self->size.y >> 1) + 0x80000) >> 20;
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
        if (self->field_70 > 0) {
            int tx = self->tileX;
            int ty = self->tileY;
            if (tx >= self->tileMaxX) {
                int x = (tx << 20) + 0x80000;
                int y = (ty << 20) + 0x80000;

                while (true) {
                    if (ty < self->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                        if (ty == self->tileY)
                            RSDK.SetTileInfo(Zone->fgLow, tx, ty, ((self->field_70 & 1) + 168) | 0xF000);
                    }

                    y -= 0x100000;
                    tx--;
                    ty--;
                    x -= 0x100000;
                    if (tx < self->tileMaxX)
                        break;
                }
            }

            ++self->tileX;
            --self->field_70;
            self->collapseTimer = (self->collapseTimer + 1) & 3;
        }
        else if (self->field_74 <= 0) {
            foreach_all(ItemBox, itembox)
            {
                if (abs(itembox->position.x - self->position.x) < self->size.x >> 1) {
                    if (abs(itembox->position.y - self->position.y >= 0) < self->size.y >> 1 && itembox->state == ItemBox_State_Normal)
                        itembox->state = ItemBox_State_Falling;
                }
            }
            destroyEntity(self);
        }
        else {
            int tx = self->tileX;
            int ty = self->tileY;

            if (tx >= self->tileMaxX) {
                int x = (tx << 20) + 0x80000;
                int y = ((ty << 20) + 0x80000);

                while (true) {
                    if (ty < self->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                    }

                    x -= 0x100000;
                    y -= 0x100000;
                    tx--;
                    ty--;
                    if (tx < self->tileMaxX)
                        break;
                }
            }

            --self->tileY;
            --self->field_74;
            self->collapseTimer = (self->collapseTimer + 1) & 3;
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
        if (self->field_70 > 0) {
            int tx = self->tileX;
            int ty = self->tileY;

            if (tx <= self->tileMaxX) {
                int x = (tx << 20) + 0x80000;
                int y = ((ty << 20) + 0x80000);
                while (true) {
                    if (ty < self->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                        if (ty == self->tileY)
                            RSDK.SetTileInfo(Zone->fgLow, tx, ty, (169 - (self->field_70 & 1)) | 0xF000);
                    }

                    x += 0x100000;
                    y -= 0x100000;
                    ++tx;
                    --ty;
                    if (tx > self->tileMaxX)
                        break;
                }
            }

            --self->tileX;
            --self->field_70;
            self->collapseTimer = (self->collapseTimer + 1) & 3;
        }
        else if (self->field_74 <= 0) {
            foreach_all(ItemBox, itembox)
            {
                if (abs(itembox->position.x - self->position.x) < self->size.x >> 1) {
                    if (abs(itembox->position.y - self->position.y >= 0) < self->size.y >> 1 && itembox->state == ItemBox_State_Normal)
                        itembox->state = ItemBox_State_Falling;
                }
            }
            destroyEntity(self);
        }
        else {
            int tx = self->tileX;
            int ty = self->tileY;

            if (tx <= self->tileMaxX) {
                int x = ((tx << 20) + 0x80000);
                int y = (ty << 20) + 0x80000;
                while (true) {
                    if (ty < self->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = 1;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                    }

                    x += 0x100000;
                    y -= 0x100000;
                    ++tx;
                    --ty;

                    if (tx > self->tileMaxX)
                        break;
                }
            }

            --self->tileY;
            --self->field_74;
            self->collapseTimer = (self->collapseTimer + 1) & 3;
        }
    }
}

void CollapsingSand_EditorDraw(void) { CollapsingSand_Draw(); }

void CollapsingSand_EditorLoad(void)
{
    CollapsingSand->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SandCollapse.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &CollapsingSand->animator, true, 0);
}

void CollapsingSand_Serialize(void)
{
    RSDK_EDITABLE_VAR(CollapsingSand, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(CollapsingSand, VAR_ENUM, delay);
}
