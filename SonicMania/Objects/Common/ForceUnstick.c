// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ForceUnstick Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectForceUnstick *ForceUnstick;

void ForceUnstick_Update(void)
{
    RSDK_THIS(ForceUnstick);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            player->collisionMode = CMODE_FLOOR;

            if (self->breakClimb && player->state == Player_State_KnuxWallClimb) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_GLIDE_DROP, &player->animator, false, 2);
                player->state = Player_State_KnuxGlideDrop;
            }
        }
    }

    self->visible = DebugMode->debugActive;
}

void ForceUnstick_LateUpdate(void) {}

void ForceUnstick_StaticUpdate(void) {}

void ForceUnstick_Draw(void) { ForceUnstick_DrawSprites(); }

void ForceUnstick_Create(void *data)
{
    RSDK_THIS(ForceUnstick);

    if (!SceneInfo->inEditor) {
        self->updateRange.x = self->width << 19;
        self->updateRange.y = self->height << 19;

        self->hitbox.right  = 8 * self->width + 8;
        self->hitbox.left   = -self->hitbox.right;
        self->hitbox.bottom = 8 * self->height + 8;
        self->hitbox.top    = -self->hitbox.bottom;

        self->visible   = false;
        self->active    = ACTIVE_BOUNDS;
        self->drawGroup = Zone->objectDrawGroup[1];
    }
}

void ForceUnstick_StageLoad(void)
{
    ForceUnstick->aniFrames = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(ForceUnstick->aniFrames, 2, &ForceUnstick->animator, true, 6);
}

void ForceUnstick_DrawSprites(void)
{
    RSDK_THIS(ForceUnstick);

    Vector2 drawPos;
    drawPos.x = self->position.x - (self->width << 19);
    drawPos.y = self->position.y - (self->height << 19);

    ForceUnstick->animator.frameID = self->breakClimb ? 9 : 6;
    for (int32 y = 0; y < self->height + 1; ++y) {
        for (int32 x = 0; x < self->width + 1; ++x) {
            RSDK.DrawSprite(&ForceUnstick->animator, &drawPos, false);
            drawPos.x += TO_FIXED(16);
        }
        drawPos.x += -TO_FIXED(16) - (self->width << 20);
        drawPos.y += TO_FIXED(16);
    }
}

#if GAME_INCLUDE_EDITOR
void ForceUnstick_EditorDraw(void) { ForceUnstick_DrawSprites(); }

void ForceUnstick_EditorLoad(void)
{
    ForceUnstick->aniFrames = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(ForceUnstick->aniFrames, 2, &ForceUnstick->animator, true, 6);
}
#endif

void ForceUnstick_Serialize(void)
{
    RSDK_EDITABLE_VAR(ForceUnstick, VAR_UINT8, width);
    RSDK_EDITABLE_VAR(ForceUnstick, VAR_UINT8, height);
    RSDK_EDITABLE_VAR(ForceUnstick, VAR_BOOL, breakClimb);
}
