// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: NoSwap Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectNoSwap *NoSwap;

void NoSwap_Update(void)
{
    RSDK_THIS(NoSwap);

    if (Player->playerCount) {
        int32 cnt = 0;
        for (int32 p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

            if (!Player_CheckValidState(player)) {
                if (self->noDeathSwap) {
                    if (player->state == Player_State_Death || player->state == Player_State_Drown) {
                        Player->cantSwap = true;
                        ++NoSwap->counter;
                    }
                }
            }
            else {
                self->playerPos.x = player->position.x;
                self->playerPos.y = player->position.y;
            }

            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                if (self->always) {
                    Player->cantSwap = true;
                    ++NoSwap->counter;
                }
                else {
                    ++cnt;
                }
            }
        }

        if (cnt == 1) {
            Player->cantSwap = true;
            ++NoSwap->counter;
        }
    }
}

void NoSwap_LateUpdate(void) {}

void NoSwap_StaticUpdate(void)
{
    if (NoSwap->counter)
        NoSwap->counter = 0;
    else
        Player->cantSwap = false;
}

void NoSwap_Draw(void)
{
    RSDK_THIS(NoSwap);

    RSDK.DrawLine(self->position.x - 16 * self->size.x, self->position.y - 16 * self->size.y, self->position.x + 16 * self->size.x,
                  self->position.y - 16 * self->size.y, 0xFF0000, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - 16 * self->size.x, self->position.y + 16 * self->size.y, self->position.x + 16 * self->size.x,
                  self->position.y + 16 * self->size.y, 0xFF0000, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - 16 * self->size.x, self->position.y - 16 * self->size.y, self->position.x - 16 * self->size.x,
                  self->position.y + 16 * self->size.y, 0xFF0000, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x + 16 * self->size.x, self->position.y - 16 * self->size.y, self->position.x + 16 * self->size.x,
                  self->position.y + 16 * self->size.y, 0xFF0000, 0xFF, INK_NONE, false);
}

void NoSwap_Create(void *data)
{
    RSDK_THIS(NoSwap);

    if (!SceneInfo->inEditor) {
        self->updateRange.x = 16 * self->size.x;
        self->updateRange.y = 16 * self->size.y;

        self->hitbox.left   = -(self->size.x >> 12);
        self->hitbox.top    = -(self->size.y >> 12);
        self->hitbox.right  = (self->size.x >> 12);
        self->hitbox.bottom = (self->size.y >> 12);
        self->active        = ACTIVE_BOUNDS;
    }
}

void NoSwap_StageLoad(void) { NoSwap->active = ACTIVE_ALWAYS; }

#if GAME_INCLUDE_EDITOR
void NoSwap_EditorDraw(void)
{
    RSDK_THIS(NoSwap);

    self->updateRange.x = TILE_SIZE * self->size.x;
    self->updateRange.y = TILE_SIZE * self->size.y;

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        NoSwap_Draw();

        RSDK_DRAWING_OVERLAY(false);
    }

    RSDK.SetSpriteAnimation(NoSwap->aniFrames, 0, &self->unusedAnimator1, true, 8);
    RSDK.DrawSprite(&self->unusedAnimator1, NULL, false);
}

void NoSwap_EditorLoad(void) { NoSwap->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void NoSwap_Serialize(void)
{
    RSDK_EDITABLE_VAR(NoSwap, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(NoSwap, VAR_BOOL, always);
    RSDK_EDITABLE_VAR(NoSwap, VAR_BOOL, noDeathSwap);
}
#endif
