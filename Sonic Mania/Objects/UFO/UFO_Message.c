#include "SonicMania.h"

ObjectUFO_Message *UFO_Message;

void UFO_Message_Update(void)
{
    RSDK_THIS(UFO_Message);
    StateMachine_Run(entity->state);
}

void UFO_Message_LateUpdate(void) {}

void UFO_Message_StaticUpdate(void) {}

void UFO_Message_Draw(void)
{
    RSDK_THIS(UFO_Message);
    Vector2 drawPos;

    drawPos.x = (RSDK_screens->centerX - entity->timer) << 16;
    drawPos.y = 0x580000;
    RSDK.DrawSprite(&entity->data1, &drawPos, true);

    drawPos.x = (entity->timer + RSDK_screens->centerX) << 16;
    RSDK.DrawSprite(&entity->data2, &drawPos, true);
}

void UFO_Message_Create(void *data)
{
    RSDK_THIS(UFO_Message);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = true;
        entity->drawOrder = 12;
        entity->state     = UFO_Message_Unknown1;

        int type = voidToInt(data);
        RSDK.SetSpriteAnimation(UFO_Message->spriteIndex, 4, &entity->data1, true, 2 * type);
        RSDK.SetSpriteAnimation(UFO_Message->spriteIndex, 4, &entity->data2, true, 2 * type + 1);
        entity->timer = 320;

        foreach_active(UFO_Message, message)
        {
            if (message != entity)
                message->state = UFO_Message_Unknown3;
        }
    }
}

void UFO_Message_StageLoad(void) { UFO_Message->spriteIndex = RSDK.LoadSpriteAnimation("SpecialUFO/HUD.bin", SCOPE_STAGE); }

void UFO_Message_Unknown1(void)
{
    RSDK_THIS(UFO_Message);
    entity->timer -= 16;
    if (entity->timer <= 0)
        entity->state = UFO_Message_Unknown2;
}
void UFO_Message_Unknown2(void)
{
    RSDK_THIS(UFO_Message);
    if (++entity->timer2 >= 180) {
        entity->timer2 = 0;
        entity->state  = UFO_Message_Unknown3;
    }
}
void UFO_Message_Unknown3(void)
{
    RSDK_THIS(UFO_Message);
    entity->timer += 16;
    if (entity->timer > 320)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void UFO_Message_EditorDraw(void) {}

void UFO_Message_EditorLoad(void) {}

void UFO_Message_Serialize(void) {}
