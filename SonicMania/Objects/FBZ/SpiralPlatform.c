// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpiralPlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpiralPlatform *SpiralPlatform;

void SpiralPlatform_Update(void)
{
    RSDK_THIS(SpiralPlatform);

    self->collision = PLATFORM_C_TILED;
    Platform_Update();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &SpiralPlatform->hitboxPlatform) == C_BOTTOM) {
            if (player->onGround && !player->collisionMode)
                player->deathType = PLAYER_DEATH_DIE_USESFX;
        }
    }
}

void SpiralPlatform_LateUpdate(void) {}

void SpiralPlatform_StaticUpdate(void) {}

void SpiralPlatform_Draw(void)
{
    RSDK_THIS(SpiralPlatform);

    RSDK.DrawTile(self->tiles, self->tileSize.x >> 20, self->tileSize.y >> 20, &self->drawPos, NULL, false);
}

void SpiralPlatform_Create(void *data)
{
    RSDK_THIS(SpiralPlatform);

    self->targetPos.x = 0x400000;
    self->targetPos.y = 0x280000;
    self->tileSize.x  = 0x600000;
    self->tileSize.y  = 0x300000;
    TilePlatform_Create(NULL);

    if (!SceneInfo->inEditor) {
        self->drawPos.y += self->offRange.x;

        self->hitbox.left += 17;
        self->hitbox.right -= 17;

        self->offRange.x <<= 10;
        self->offRange.y <<= 10;
        self->speed <<= 16;

        self->fallY         = self->offRange.x;
        self->active        = ACTIVE_XBOUNDS;
        self->updateRange.y = 0x800000 + self->offRange.y;
        self->state         = SpiralPlatform_State_Move;
    }
}

void SpiralPlatform_StageLoad(void)
{
    SpiralPlatform->hitboxPlatform.left   = -48;
    SpiralPlatform->hitboxPlatform.top    = -7;
    SpiralPlatform->hitboxPlatform.right  = 48;
    SpiralPlatform->hitboxPlatform.bottom = 7;
}

void SpiralPlatform_State_Move(void)
{
    RSDK_THIS(SpiralPlatform);

    self->fallY += self->speed;
    self->drawPos.y += self->speed;
    self->velocity.y = self->speed;

    if ((self->speed <= 0 && self->fallY <= self->offRange.y) || (self->speed > 0 && self->fallY >= self->offRange.y)) {
        self->fallY     = 0;
        self->drawPos.y = self->centerPos.y;
    }
}

#if GAME_INCLUDE_EDITOR
void SpiralPlatform_EditorDraw(void)
{
    RSDK_THIS(SpiralPlatform);
    self->active = ACTIVE_BOUNDS;

    RSDK.DrawSprite(&SpiralPlatform->animator, NULL, false);

    if (showGizmos()) {
        DrawHelpers_DrawRectOutline(0x400000, 0x280000, 0x600000, 0x300000, 0xFFFF00);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, 0x400000, 0x280000, 0x00FF00, INK_NONE, 0xFF);
        self->active = ACTIVE_NORMAL;
    }
}

void SpiralPlatform_EditorLoad(void)
{
    SpiralPlatform->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Platform.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(SpiralPlatform->aniFrames, 1, &SpiralPlatform->animator, true, 0);
}
#endif

void SpiralPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_VECTOR2, offRange);
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(SpiralPlatform, VAR_ENUM, childCount);
}
