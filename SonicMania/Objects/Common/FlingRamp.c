// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FlingRamp Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFlingRamp *FlingRamp;

void FlingRamp_Update(void)
{
    RSDK_THIS(FlingRamp);

    foreach_active(Player, player)
    {
        if (player->onGround) {
            bool32 left  = self->direction == FLIP_NONE || self->direction == FLIP_X;
            bool32 right = self->direction == FLIP_NONE || self->direction == FLIP_Y;

            if (left && !(player->direction & FLIP_X) && player->velocity.x >= 0x40000) {
                if (Player_CheckCollisionTouch(player, self, &FlingRamp->hitboxRamp)) {
                    player->velocity.x += TO_FIXED(4);
                    player->velocity.y = -TO_FIXED(7);

                    // Bug Details:
                    // this one's the same bug I detailed in SPZ/RockemSockem and was shown off in SDCC 2017
                    // if you glide into the fling ramp as knux the state wont be set to air so you'll be gliding with SPRINGCS anim playing
                    // Fix: set the state to Player_State_Air (see fix commented below)
                    // player->state    = Player_State_Air;
                    player->onGround = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, true, 0);
                }
            }
            else if (right && (player->direction & FLIP_X) && player->velocity.x <= -0x40000) {
                if (Player_CheckCollisionTouch(player, self, &FlingRamp->hitboxRamp)) {
                    player->velocity.x -= TO_FIXED(4);
                    player->velocity.y = -TO_FIXED(7);

                    // Bug Details:
                    // this one's the same bug I detailed in SPZ/RockemSockem and was shown off in SDCC 2017
                    // if you glide into the fling ramp as knux the state wont be set to air so you'll be gliding with SPRINGCS anim playing
                    // Fix: set the state to Player_State_Air (see fix commented below)
                    // player->state    = Player_State_Air;
                    player->onGround = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_CS, &player->animator, true, 0);
                }
            }
        }
    }
}

void FlingRamp_LateUpdate(void) {}

void FlingRamp_StaticUpdate(void) {}

void FlingRamp_Draw(void) {}

void FlingRamp_Create(void *data)
{
    RSDK_THIS(FlingRamp);

    if (!SceneInfo->inEditor) {
        self->active  = ACTIVE_BOUNDS;
        self->visible = false;
    }
}

void FlingRamp_StageLoad(void)
{
    FlingRamp->hitboxRamp.left   = -16;
    FlingRamp->hitboxRamp.top    = -16;
    FlingRamp->hitboxRamp.right  = 16;
    FlingRamp->hitboxRamp.bottom = 16;
}

#if GAME_INCLUDE_EDITOR
void FlingRamp_EditorDraw(void)
{
    RSDK_THIS(FlingRamp);

    Animator animator;
    RSDK.SetSpriteAnimation(FlingRamp->hitboxRamp.left, 0, &animator, true, 6);
    RSDK.DrawSprite(&animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        if (self->direction < FLIP_Y)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x - 0x300000, self->position.y, 0xFFFF00, INK_NONE, 0xFF);

        if (self->direction != FLIP_X)
            DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + 0x300000, self->position.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void FlingRamp_EditorLoad(void)
{
    FlingRamp->hitboxRamp.left = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FlingRamp, direction);
    RSDK_ENUM_VAR("Omni", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
    RSDK_ENUM_VAR("Left", FLIP_Y);
}
#endif

void FlingRamp_Serialize(void) { RSDK_EDITABLE_VAR(FlingRamp, VAR_UINT8, direction); }
