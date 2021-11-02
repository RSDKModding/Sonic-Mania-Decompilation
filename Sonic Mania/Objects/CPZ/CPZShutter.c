#include "SonicMania.h"

ObjectCPZShutter *CPZShutter;

void CPZShutter_Update(void)
{
    RSDK_THIS(CPZShutter);
    StateMachine_Run(entity->state);
}

void CPZShutter_LateUpdate(void) {}

void CPZShutter_StaticUpdate(void) {}

void CPZShutter_Draw(void)
{
    RSDK_THIS(CPZShutter);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void CPZShutter_Create(void *data)
{
    RSDK_THIS(CPZShutter);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->active        = ACTIVE_BOUNDS;
        entity->drawOrder     = Zone->drawOrderHigh - 2;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(CPZShutter->aniFrames, 0, &entity->animator, true, 0);
    }
}

void CPZShutter_StageLoad(void) { CPZShutter->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Shutter.bin", SCOPE_STAGE); }

void CPZShutter_State_Open(void)
{
    RSDK_THIS(CPZShutter);
    entity->position.y -= 0x20000;
    if (++entity->timer == 24) {
        entity->timer = 0;
        entity->state = StateMachine_None;
    }
}

void CPZShutter_State_Close(void)
{
    RSDK_THIS(CPZShutter);
    entity->position.y += 0x20000;
    if (++entity->timer == 24) {
        entity->timer  = 0;
        entity->active = ACTIVE_BOUNDS;
        entity->state  = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void CPZShutter_EditorDraw(void)
{
    RSDK_THIS(CPZShutter);
    RSDK.SetSpriteAnimation(CPZShutter->aniFrames, 0, &entity->animator, true, 0);
    CPZShutter_Draw();
}

void CPZShutter_EditorLoad(void) { CPZShutter->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Shutter.bin", SCOPE_STAGE); }
#endif

void CPZShutter_Serialize(void) {}
