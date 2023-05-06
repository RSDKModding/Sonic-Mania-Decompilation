// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: InvisibleBlock Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectInvisibleBlock *InvisibleBlock;

void InvisibleBlock_Update(void)
{
    RSDK_THIS(InvisibleBlock);

    foreach_active(Player, player)
    {
        if ((self->planeFilter <= 0 || player->collisionPlane == (((uint8)self->planeFilter - 1) & 1)) && (!self->noChibi || !player->isChibi)) {
            switch (Player_CheckCollisionBox(player, self, &self->hitbox)) {
                case C_TOP:
                    if (!self->noCrush)
                        player->collisionFlagV |= 1;
                    break;

                case C_LEFT:
                    if (!self->noCrush)
                        player->collisionFlagH |= 1;
                    break;

                case C_RIGHT:
                    if (!self->noCrush)
                        player->collisionFlagH |= 2;
                    break;

                case C_BOTTOM:
                    if (!self->noCrush)
                        player->collisionFlagV |= 2;
                    break;

                default: break;
            }
        }
    }

    self->visible = DebugMode->debugActive;
}

void InvisibleBlock_LateUpdate(void) {}

void InvisibleBlock_StaticUpdate(void) {}

void InvisibleBlock_Draw(void) { InvisibleBlock_DrawSprites(); }

void InvisibleBlock_Create(void *data)
{
    RSDK_THIS(InvisibleBlock);

    if (!SceneInfo->inEditor) {
        if (self->timeAttackOnly && globals->gameMode < MODE_TIMEATTACK)
            destroyEntity(self);

        self->visible = false;
        self->active  = self->activeNormal ? ACTIVE_NORMAL : ACTIVE_BOUNDS;

        self->updateRange.x = (self->width + 5) << 19;
        self->updateRange.y = (self->height + 5) << 19;

        self->hitbox.right  = 8 * self->width + 8;
        self->hitbox.left   = -self->hitbox.right;
        self->hitbox.bottom = 8 * self->height + 8;
        self->hitbox.top    = -self->hitbox.bottom;
        self->drawGroup     = Zone->objectDrawGroup[1];
    }
}

void InvisibleBlock_StageLoad(void)
{
    InvisibleBlock->aniFrames = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(InvisibleBlock->aniFrames, 2, &InvisibleBlock->animator, true, 0);
    InvisibleBlock->animator.frameID = 10;
}

void InvisibleBlock_DrawSprites(void)
{
    RSDK_THIS(InvisibleBlock);

    Vector2 drawPos;
    drawPos.x = self->position.x - (self->width << 19);
    drawPos.y = self->position.y - (self->height << 19);

    for (int32 y = 0; y <= self->height; ++y) {
        for (int32 x = 0; x <= self->width; ++x) {
            RSDK.DrawSprite(&InvisibleBlock->animator, &drawPos, false);
            drawPos.x += TO_FIXED(16);
        }

        drawPos.x += -TO_FIXED(16) - (self->width << 20);
        drawPos.y += TO_FIXED(16);
    }
}

#if GAME_INCLUDE_EDITOR
void InvisibleBlock_EditorDraw(void)
{
    RSDK_THIS(InvisibleBlock);

    self->updateRange.x = (self->width + 5) << 19;
    self->updateRange.y = (self->height + 5) << 19;

    InvisibleBlock_DrawSprites();
}

void InvisibleBlock_EditorLoad(void)
{
    InvisibleBlock->aniFrames = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(InvisibleBlock->aniFrames, 2, &InvisibleBlock->animator, true, 10);

    RSDK_ACTIVE_VAR(InvisibleBlock, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);
}
#endif

void InvisibleBlock_Serialize(void)
{
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_UINT8, width);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_UINT8, height);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, noCrush);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, activeNormal);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, timeAttackOnly);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, noChibi);
}
