#include "SonicMania.h"

ObjectUICharButton *UICharButton;

void UICharButton_Update(void)
{
    RSDK_THIS(UICharButton);
    entity->touchPosStart.x = 0x600000;
    entity->touchPosStart.y = 0x600000;
    entity->touchPosEnd.x   = 0;
    entity->touchPosEnd.y   = 0;
    StateMachine_Run(entity->state);

    if (entity->flag) {
        entity->field_108 -= 0x600;
        entity->field_10C += entity->field_108;
        if (entity->field_10C <= 0x10000 && entity->field_108 < 0) {
            entity->field_10C = 0x10000;
            entity->field_108 = 0;
        }

        entity->field_110 -= 0x1800;
        entity->field_114 += entity->field_110;
        if (entity->field_114 <= 0x8000 && entity->field_110 < 0) {
            entity->field_114 = 0x8000;
            entity->field_110 = 0;
        }
    }
    else if (entity->state != UICharButton_Unknown11) {
        if (entity->field_10C > 0) {
            entity->field_10C -= 0x2000;
            if (entity->field_10C < 0)
                entity->field_10C = 0;
        }

        if (entity->field_114 > 0) {
            entity->field_114 -= 0x2000;
            if (entity->field_114 < 0)
                entity->field_114 = 0;
        }
    }

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    int id                  = -1;
    for (int i = 0; i < parent->unknownCount1; ++i) {
        if (entity = (EntityUICharButton *)parent->entities[i]) {
            id = i;
            break;
        }
    }

    if (entity->flag && (parent->state != UIControl_ProcessInputs || parent->activeEntityID != id)) {
        entity->flag  = false;
        entity->state = UICharButton_Unknown9;
    }
}

void UICharButton_LateUpdate(void) {}

void UICharButton_StaticUpdate(void) {}

void UICharButton_Draw(void)
{
    RSDK_THIS(UICharButton);
    RSDK.DrawRect(entity->position.x - 0x2D0000, entity->position.y - 0x2D0000, 0x600000, 0x600000, 0xFFFFFF, 127, INK_BLEND, false);
    UICharButton_Unknown2();
    UICharButton_Unknown1();
    UICharButton_Unknown3();
}

void UICharButton_Create(void *data)
{
    RSDK_THIS(UICharButton);
    entity->active          = ACTIVE_BOUNDS;
    entity->drawOrder       = 2;
    entity->visible         = true;
    entity->drawFX          = FX_FLIP;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x300000;
    entity->processButtonCB = UIButton_Unknown6;
    entity->touchCB         = UIButton_ProcessTouch;
    entity->options3        = UICharButton_Unknown4;
    entity->failCB          = 0;
    entity->options5        = UICharButton_Unknown7;
    entity->options6        = UICharButton_Unknown8;
    entity->options7        = UICharButton_Unknown5;
    entity->options8        = UICharButton_Unknown6;
    entity->state           = UICharButton_Unknown9;
}

void UICharButton_StageLoad(void) { UICharButton->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UICharButton_Unknown1(void)
{
    RSDK_THIS(UICharButton);
    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(96, 96, RSDK_sceneInfo->entity->position.x + 0x30000, RSDK_sceneInfo->entity->position.y + 0x30000);

    if (entity->flag)
        UIWidgets_Unknown4(96, 96, entity->position.x, entity->position.y);
    else
        UIWidgets_Unknown2(96, 96, entity->position.x, entity->position.y);
}

void UICharButton_Unknown2(void)
{
    RSDK_THIS(UICharButton);
    UIWidgets_Unknown5(232, (entity->field_10C >> 11), 40, 88, RSDK_sceneInfo->entity->position.x - 0x2D0000,
                       RSDK_sceneInfo->entity->position.y - 0x2D0000);
    UIWidgets_Unknown5(96, (-64 * entity->field_10C) >> 16, 160, 176, entity->position.x + 0x2D0000, entity->position.y + 0x2C0000);
    UIWidgets_Unknown5(88, (-44 * entity->field_10C) >> 16, 112, 224, entity->position.x + 0x2D0000, entity->position.y + 0x2C0000);
}

void UICharButton_Unknown3(void)
{
    RSDK_THIS(UICharButton);
    Vector2 drawPos;

    drawPos = entity->position;
    drawPos.x -= 0x2D0000;
    drawPos.y += 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);

    if (entity->state != UICharButton_Unknown11 || !(entity->timer & 2)) {
        int frame = entity->characterID;
        if (entity->characterID > 2)
            frame = entity->characterID + 1;
        RSDK.SetSpriteAnimation(UICharButton->aniFrames, 1, &entity->animator1, true, frame);
        RSDK.SetSpriteAnimation(UICharButton->aniFrames, 2, &entity->animator2, true, frame);
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y - 0x80000;
        drawPos.x += 4 * entity->field_114;
        drawPos.y += 4 * entity->field_114;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        drawPos.x -= 8 * entity->field_114;
        drawPos.y -= 8 * entity->field_114;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 8, &entity->animator3, true, entity->characterID);
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y + 0x200000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    }
}

void UICharButton_Unknown4(void)
{
    RSDK_THIS(UICharButton);
    entity->timer           = 0;
    entity->state           = UICharButton_Unknown11;
    entity->processButtonCB = 0;
    UITransition_StartTransition(entity->options2, 30);
    if (UIControl_GetUIControl())
        UIControl_GetUIControl()->selectionDisabled = true;
    ((EntityUIControl *)entity->parent)->backoutTimer = 30;
    RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
}

bool32 UICharButton_Unknown5(void)
{
    RSDK_THIS(UICharButton);
    return entity->state == UICharButton_Unknown10;
}

bool32 UICharButton_Unknown6(void)
{
    RSDK_THIS(UICharButton);
    return entity->state == UICharButton_Unknown11;
}

void UICharButton_Unknown7(void)
{
    RSDK_THIS(UICharButton);
    if (!entity->flag) {
        entity->field_10C = 0;
        entity->field_108 = 0x4000;
        entity->field_114 = 0;
        entity->field_110 = 0x8000;
        entity->flag      = true;
        entity->state     = UICharButton_Unknown10;
    }
}

void UICharButton_Unknown8(void)
{
    RSDK_THIS(UICharButton);
    entity->flag  = false;
    entity->state = UICharButton_Unknown9;
}

void UICharButton_Unknown9(void) {}

void UICharButton_Unknown10(void) {}

void UICharButton_Unknown11(void)
{
    RSDK_THIS(UICharButton);
    if (entity->timer >= 30) {
        entity->flag            = false;
        entity->timer           = 0;
        entity->state           = UICharButton_Unknown9;
        entity->processButtonCB = UIButton_ProcessButtonInputs;
    }
    else {
        if (entity->timer == 2) {
            switch (entity->characterID) {
                case 0: RSDK.PlaySFX(Announcer->sfx_Sonic, false, 255);
                case 1: RSDK.PlaySFX(Announcer->sfx_Tails, false, 255);
                case 2: RSDK.PlaySFX(Announcer->sfx_Knuckles, false, 255);
#if RETRO_USE_PLUS
                case 3: RSDK.PlaySFX(Announcer->sfx_Mighty, false, 255);
                case 4: RSDK.PlaySFX(Announcer->sfx_Ray, false, 255);
#endif
                default: break;
            }
        }
        ++entity->timer;
    }
}

void UICharButton_EditorDraw(void) {}

void UICharButton_EditorLoad(void) {}

void UICharButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UICharButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UICharButton, VAR_UINT8, characterID);
}
