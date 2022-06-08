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
                    player->velocity.x += 0x40000;
                    player->velocity.y = -0x70000;
                    // Bug Details:
                    // this one's the same bug I detailed in SPZ/RockemSockem and was shown off in SDCC 2017
                    // if you glide into the fling ramp as knux the state wont be set to air so you'll be gliding with SPRINGCS anim playing
                    // Fix: set the state to Player_State_Air (see fix commented below)
                    // player->state    = Player_State_Air;
                    player->onGround = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 0);
                }
            }
            else if (right && (player->direction & FLIP_X) && player->velocity.x <= -0x40000) {
                if (Player_CheckCollisionTouch(player, self, &FlingRamp->hitboxRamp)) {
                    player->velocity.x -= 0x40000;
                    player->velocity.y = -0x70000;
                    // Bug Details:
                    // this one's the same bug I detailed in SPZ/RockemSockem and was shown off in SDCC 2017
                    // if you glide into the fling ramp as knux the state wont be set to air so you'll be gliding with SPRINGCS anim playing
                    // Fix: set the state to Player_State_Air (see fix commented below)
                    // player->state    = Player_State_Air;
                    player->onGround = false;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 0);
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

#if RETRO_INCLUDE_EDITOR
void FlingRamp_EditorDraw(void)
{
    RSDK_THIS(FlingRamp);

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

    RSDK_ACTIVE_VAR(FlingRamp, direction);
    RSDK_ENUM_VAR("Flip From Both Sides", FLIP_NONE);
    RSDK_ENUM_VAR("Flip From Left Only", FLIP_X);
    RSDK_ENUM_VAR("Flip From Right Only", FLIP_Y);
}
#endif

void FlingRamp_Serialize(void) { RSDK_EDITABLE_VAR(FlingRamp, VAR_UINT8, direction); }
