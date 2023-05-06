// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Message Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_Message *UFO_Message;

void UFO_Message_Update(void)
{
    RSDK_THIS(UFO_Message);

    StateMachine_Run(self->state);
}

void UFO_Message_LateUpdate(void) {}

void UFO_Message_StaticUpdate(void) {}

void UFO_Message_Draw(void)
{
    RSDK_THIS(UFO_Message);

    Vector2 drawPos;
    drawPos.x = (ScreenInfo->center.x - self->timer) << 16;
    drawPos.y = 0x580000;
    RSDK.DrawSprite(&self->leftAnimator, &drawPos, true);

    drawPos.x = (self->timer + ScreenInfo->center.x) << 16;
    RSDK.DrawSprite(&self->rightAnimator, &drawPos, true);
}

void UFO_Message_Create(void *data)
{
    RSDK_THIS(UFO_Message);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 12;
        self->state     = UFO_Message_State_Appear;

        int32 type = VOID_TO_INT(data);
        RSDK.SetSpriteAnimation(UFO_Message->aniFrames, 4, &self->leftAnimator, true, 2 * type);
        RSDK.SetSpriteAnimation(UFO_Message->aniFrames, 4, &self->rightAnimator, true, 2 * type + 1);
        self->timer = 320;

        foreach_active(UFO_Message, message)
        {
            if (message != self)
                message->state = UFO_Message_State_Exit;
        }
    }
}

void UFO_Message_StageLoad(void) { UFO_Message->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/HUD.bin", SCOPE_STAGE); }

void UFO_Message_State_Appear(void)
{
    RSDK_THIS(UFO_Message);

    self->timer -= 16;

    if (self->timer <= 0)
        self->state = UFO_Message_State_ShowMsg;
}
void UFO_Message_State_ShowMsg(void)
{
    RSDK_THIS(UFO_Message);

    if (++self->displayTime >= 180) {
        self->displayTime = 0;

        self->state = UFO_Message_State_Exit;
    }
}
void UFO_Message_State_Exit(void)
{
    RSDK_THIS(UFO_Message);

    self->timer += 16;
    if (self->timer > 320)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void UFO_Message_EditorDraw(void) {}

void UFO_Message_EditorLoad(void) {}
#endif

void UFO_Message_Serialize(void) {}
