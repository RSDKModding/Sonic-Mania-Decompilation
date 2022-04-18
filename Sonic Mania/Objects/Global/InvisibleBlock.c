// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: InvisibleBlock Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
        if (self->timeAttackOnly && globals->gameMode < MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        self->visible = false;
        self->active  = self->activeNormal ? ACTIVE_NORMAL : ACTIVE_BOUNDS;

        self->updateRange.x = (self->width + 5) << 19;
        self->updateRange.y = (self->height + 5) << 19;

        self->hitbox.right  = 8 * self->width + 8;
        self->hitbox.left   = -self->hitbox.right;
        self->hitbox.bottom = 8 * self->height + 8;
        self->hitbox.top    = -self->hitbox.bottom;
        self->drawOrder     = Zone->objectDrawHigh;
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
    drawPos.x                    = self->position.x;
    drawPos.y                    = self->position.y;
    drawPos.x -= self->width << 19;
    drawPos.y -= self->height << 19;

    for (int32 y = 0; y <= self->height; ++y) {
        for (int32 x = 0; x <= self->width; ++x) {
            RSDK.DrawSprite(&InvisibleBlock->animator, &drawPos, false);
            drawPos.x += 0x100000;
        }
        drawPos.x += -0x100000 - (self->width << 20);
        drawPos.y += 0x100000;
    }
}

#if RETRO_INCLUDE_EDITOR
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
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
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
