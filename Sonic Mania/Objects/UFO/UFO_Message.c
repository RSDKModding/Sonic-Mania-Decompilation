#include "SonicMania.h"

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

    drawPos.x = (ScreenInfo->centerX - self->timer) << 16;
    drawPos.y = 0x580000;
    RSDK.DrawSprite(&self->animator1, &drawPos, true);

    drawPos.x = (self->timer + ScreenInfo->centerX) << 16;
    RSDK.DrawSprite(&self->animator2, &drawPos, true);
}

void UFO_Message_Create(void *data)
{
    RSDK_THIS(UFO_Message);
    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawOrder = 12;
        self->state     = UFO_Message_Unknown1;

        int32 type = voidToInt(data);
        RSDK.SetSpriteAnimation(UFO_Message->aniFrames, 4, &self->animator1, true, 2 * type);
        RSDK.SetSpriteAnimation(UFO_Message->aniFrames, 4, &self->animator2, true, 2 * type + 1);
        self->timer = 320;

        foreach_active(UFO_Message, message)
        {
            if (message != self)
                message->state = UFO_Message_Unknown3;
        }
    }
}

void UFO_Message_StageLoad(void) { UFO_Message->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/HUD.bin", SCOPE_STAGE); }

void UFO_Message_Unknown1(void)
{
    RSDK_THIS(UFO_Message);
    self->timer -= 16;
    if (self->timer <= 0)
        self->state = UFO_Message_Unknown2;
}
void UFO_Message_Unknown2(void)
{
    RSDK_THIS(UFO_Message);
    if (++self->timer2 >= 180) {
        self->timer2 = 0;
        self->state  = UFO_Message_Unknown3;
    }
}
void UFO_Message_Unknown3(void)
{
    RSDK_THIS(UFO_Message);
    self->timer += 16;
    if (self->timer > 320)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void UFO_Message_EditorDraw(void) {}

void UFO_Message_EditorLoad(void) {}
#endif

void UFO_Message_Serialize(void) {}
