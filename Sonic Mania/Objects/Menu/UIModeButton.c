#include "SonicMania.h"

ObjectUIModeButton *UIModeButton;

void UIModeButton_Update(void)
{
    RSDK_THIS(UIModeButton);

    entity->touchPosStart.y = 0x380000;
    entity->touchPosStart.x = 0x800000;
    entity->touchPosStart.x += entity->touchPosStart.y;
    entity->touchPosStart.y += 0x60000;
    entity->touchPosEnd.x = 0;
    entity->touchPosEnd.y = -0x120000;
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex || entity->wasDisabled != entity->disabled) {
        UIModeButton_Unknown1();
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        entity->wasDisabled     = entity->disabled;
    }
    StateMachine_Run(entity->state);
}

void UIModeButton_LateUpdate(void) {}

void UIModeButton_StaticUpdate(void) {}

void UIModeButton_Draw(void)
{
    RSDK_THIS(UIModeButton);
    Vector2 drawPos;

    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, (entity->position.y >> 16) - RSDK_screens->position.y);
    drawPos = entity->position;
    drawPos.y += entity->field_118;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    drawPos.x += entity->field_11C;
    drawPos.y += entity->field_11C >> 1;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos = entity->position;
    drawPos.y += entity->field_114;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawPos.x += entity->field_11C;
    drawPos.y += entity->field_11C >> 1;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    drawPos = entity->position;
    drawPos.x -= entity->field_11C;
    drawPos.y -= entity->field_11C;
    UIWidgets_Unknown7(24, 128, 24, 240, 240, 240, drawPos.x, drawPos.y);

    drawPos = entity->position;
    drawPos.x += entity->field_11C;
    drawPos.y += entity->field_11C;
    UIWidgets_Unknown7(24, 128, 24, 0, 0, 0, drawPos.x, drawPos.y);

    if (entity->field_130) {
        drawPos = entity->position;
        drawPos.y += entity->field_110;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);
    }
}

void UIModeButton_Create(void *data)
{
    RSDK_THIS(UIModeButton);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible         = true;
        entity->drawOrder       = 2;
        entity->active          = ACTIVE_BOUNDS;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x400000;
        entity->field_130       = true;
        entity->field_114       = 0x280000;
        entity->field_118       = 0x280000;
        entity->processButtonCB = UIButton_Unknown6;
        entity->touchCB         = UIButton_ProcessTouch;
        entity->options3        = UIModeButton_Options3CB;
        entity->failCB          = UIModeButton_FailCB;
        entity->options5        = UIModeButton_Options5CB;
        entity->options6        = UIModeButton_Options6CB;
        entity->options7        = UIModeButton_Options7CB;
        entity->options8        = UIModeButton_Options8CB;
        UIModeButton_Unknown1();
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
}

void UIModeButton_StageLoad(void) { UIModeButton->aniFrames = RSDK.LoadSpriteAnimation("UI/MainIcons.bin", SCOPE_STAGE); }

void UIModeButton_Unknown1(void)
{
    RSDK_THIS(UIModeButton);
    if (entity->disabled) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 7, &entity->animator5, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
    }
    else {
        switch (entity->buttonID) {
            case 1:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &entity->animator5, true, 1);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &entity->animator1, true, 1);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &entity->animator3, true, 1);
                RSDK.SetSpriteAnimation(0xFFFF, 1, &entity->animator4, true, 0);
                break;
            case 2:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &entity->animator5, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &entity->animator1, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &entity->animator2, true, 3);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &entity->animator3, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &entity->animator4, true, 3);
                break;
            case 3:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &entity->animator5, true, 3);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &entity->animator1, true, 4);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &entity->animator3, true, 4);
                RSDK.SetSpriteAnimation(0xFFFF, 1, &entity->animator4, true, 0);
                break;
            default:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &entity->animator5, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &entity->animator3, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 1, &entity->animator4, true, 0);
                break;
        }
    }
}

bool32 UIModeButton_Options7CB(void)
{
    RSDK_THIS(UIModeButton);
    return entity->state == UIModeButton_Unknown9;
}
bool32 UIModeButton_Options8CB(void)
{
    RSDK_THIS(UIModeButton);
    return entity->state == UIModeButton_Unknown10;
}

void UIModeButton_Options5CB(void)
{
    RSDK_THIS(UIModeButton);
    if (entity->state != UIModeButton_Unknown9) {
        entity->field_110 = 0;
        entity->field_114 = 0x180000;
        entity->field_118 = 0x180000;
        entity->field_11C = 0;
        entity->field_120 = -0x20000;
        entity->field_124 = -0x80000;
        entity->field_128 = -0x80000;
        entity->field_12C = -0x20000;
        entity->state     = UIModeButton_Unknown9;
    }
}

void UIModeButton_Options3CB(void)
{
    RSDK_THIS(UIModeButton);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if (entity->buttonID == 2) {
#if RETRO_USE_PLUS
        int32 id = API_MostRecentActiveControllerID(1, 0, 5);
#else
        int32 id = API_MostRecentActiveControllerID(0);
#endif
        API_ResetControllerAssignments();
        API_AssignControllerID(1, id);
        API_AssignControllerID(2, CONT_AUTOASSIGN);
        API_AssignControllerID(3, CONT_AUTOASSIGN);
        API_AssignControllerID(4, CONT_AUTOASSIGN);
    }
    UITransition_StartTransition(entity->options2, 14);
    if (entity->stopMusic)
        RSDK.StopChannel(Music->channelID);
    entity->flag         = false;
    entity->state        = UIModeButton_Unknown10;
    parent->backoutTimer = 30;
    RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
}

void UIModeButton_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfx_Fail, false, 255); }

void UIModeButton_Options6CB(void)
{
    RSDK_THIS(UIModeButton);
    entity->state = UIModeButton_Unknown8;
}

void UIModeButton_Unknown8(void)
{
    RSDK_THIS(UIModeButton);

    if (entity->field_110 < 0) {
        entity->field_110 += 0x8000;
        if (entity->field_110 > 0)
            entity->field_110 = 0;
    }
    entity->field_114 += (0x280000 - entity->field_114) >> 3;
    entity->field_118 += (0x280000 - entity->field_118) >> 3;

    if (entity->field_11C < 0) {
        entity->field_11C += 0x10000;
        if (entity->field_11C > 0)
            entity->field_11C = 0;
    }
}

void UIModeButton_Unknown9(void)
{
    RSDK_THIS(UIModeButton);
    entity->field_120 += 0x4000;
    entity->field_110 += entity->field_120;
    if (entity->field_110 >= 0 && entity->field_120 > 0) {
        entity->field_110 = 0;
        entity->field_120 = 0;
    }

    entity->field_124 += 0xA000;
    entity->field_114 += entity->field_124;
    if (entity->field_114 >= -0xC0000 && entity->field_124 > 0) {
        entity->field_114 = -0xC0000;
        entity->field_124 = 0;
    }

    entity->field_128 += 0x8000;
    entity->field_118 += entity->field_128;
    if (entity->field_118 >= -0xC0000 && entity->field_128 > 0) {
        entity->field_118 = -0xC0000;
        entity->field_128 = 0;
    }

    entity->field_12C += 0x4800;
    entity->field_11C += entity->field_12C;
    if (entity->field_11C >= -0x20000 && entity->field_12C > 0) {
        entity->field_11C = -0x20000;
        entity->field_12C = 0;
    }
}

void UIModeButton_Unknown10(void)
{
    RSDK_THIS(UIModeButton);
    UIModeButton_Unknown9();
    if (++entity->timer == 30) {
        entity->timer = 0;
        entity->state = UIModeButton_Unknown9;
    }
    entity->field_130 = !((entity->timer >> 1) & 1);
}

#if RETRO_INCLUDE_EDITOR
void UIModeButton_EditorDraw(void) {}

void UIModeButton_EditorLoad(void) {}
#endif

void UIModeButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIModeButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIModeButton, VAR_ENUM, buttonID);
    RSDK_EDITABLE_VAR(UIModeButton, VAR_BOOL, stopMusic);
}
