// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TAEmerald Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTAEmerald *TAEmerald;

void TAEmerald_Update(void)
{
    RSDK_THIS(TAEmerald);

    StateMachine_Run(self->state);
}

void TAEmerald_LateUpdate(void) {}

void TAEmerald_StaticUpdate(void) {}

void TAEmerald_Draw(void)
{
    RSDK_THIS(TAEmerald);

    RSDK.SetActivePalette(3, 0, ScreenInfo->size.y);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TAEmerald_Create(void *data)
{
    RSDK_THIS(TAEmerald);

    if (!SceneInfo->inEditor) {
        self->originPos     = self->position;
        self->angle         = 16 * self->color;
        self->visible       = true;
        self->drawGroup     = 1;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->state = TAEmerald_State_Oscillate;
        RSDK.SetSpriteAnimation(TAEmerald->aniFrames, 7, &self->animator, true, self->color);

        if (SaveGame_GetSaveRAM() && SaveGame_GetEmerald(self->color)) {
            self->visible = false;
        }
    }
}

void TAEmerald_StageLoad(void) { TAEmerald->aniFrames = RSDK.LoadSpriteAnimation("Special/Results.bin", SCOPE_STAGE); }

void TAEmerald_State_Oscillate(void)
{
    RSDK_THIS(TAEmerald);

    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->originPos.y;
    self->angle      = (self->angle + 4) & 0xFF;
}

void TAEmerald_State_MoveCircle(void)
{
    RSDK_THIS(TAEmerald);

    if (self->timer > 0) {
        self->timer--;
    }
    else {
        if (self->direction) {
            self->angle += 2;

            if (self->angle >= 0) {
                self->direction = FLIP_NONE;
                self->state     = StateMachine_None;
            }
        }
        else {
            self->angle -= 2;

            if (self->angle <= -0x80) {
                self->direction = FLIP_X;
                self->state     = StateMachine_None;
            }
        }

        self->rotation   = 4 * self->angle;
        self->position.x = 0x4800 * RSDK.Cos256(self->angle) + self->originPos.x;
        self->position.y = 0x4800 * RSDK.Sin256(self->angle) + self->originPos.y;
    }
}

#if RETRO_INCLUDE_EDITOR
void TAEmerald_EditorDraw(void)
{
    RSDK_THIS(TAEmerald);

    RSDK.SetSpriteAnimation(TAEmerald->aniFrames, 7, &self->animator, true, self->color);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TAEmerald_EditorLoad(void)
{
    TAEmerald->aniFrames = RSDK.LoadSpriteAnimation("Special/Results.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TAEmerald, color);
    RSDK_ENUM_VAR("Green", CHAOSEMERALD_GREEN);
    RSDK_ENUM_VAR("Yellow", CHAOSEMERALD_YELLOW);
    RSDK_ENUM_VAR("Blue", CHAOSEMERALD_BLUE);
    RSDK_ENUM_VAR("Purple", CHAOSEMERALD_PURPLE);
    RSDK_ENUM_VAR("Gray", CHAOSEMERALD_GRAY);
    RSDK_ENUM_VAR("Cyan", CHAOSEMERALD_CYAN);
    RSDK_ENUM_VAR("Red", CHAOSEMERALD_RED);
}
#endif

void TAEmerald_Serialize(void) { RSDK_EDITABLE_VAR(TAEmerald, VAR_UINT8, color); }
