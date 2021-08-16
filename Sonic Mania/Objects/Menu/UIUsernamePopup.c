#include "SonicMania.h"

ObjectUIUsernamePopup *UIUsernamePopup;

void UIUsernamePopup_Update(void)
{
    RSDK_THIS(UIUsernamePopup);
    StateMachine_Run(entity->state);
}

void UIUsernamePopup_LateUpdate(void) {}

void UIUsernamePopup_StaticUpdate(void) {}

void UIUsernamePopup_Draw(void)
{
    RSDK_THIS(UIUsernamePopup);
    if (entity->drawFlag)
        UIUsernamePopup_DrawSprites();
}

void UIUsernamePopup_Create(void *data)
{
    RSDK_THIS(UIUsernamePopup);
    entity->active       = ACTIVE_ALWAYS;
    entity->drawOrder    = 13;
    entity->posUnknown.x = 0;
    entity->posUnknown.y = 0;
    entity->dword6C      = 0x180000;
    entity->visible      = true;
    entity->state        = UIUsernamePopup_State_Setup;
}

void UIUsernamePopup_StageLoad(void)
{
    foreach_all(UIUsernamePopup, popup) { UIUsernamePopup->popup = (Entity *)popup; }
}

void UIUsernamePopup_ShowPopup(void)
{
    EntityUIUsernamePopup *entity = (EntityUIUsernamePopup *)UIUsernamePopup->popup;
    if (entity->state == UIUsernamePopup_State_Setup) {
        if (API_GetUsername(&entity->username)) {
#if RETRO_USE_PLUS
            RSDK.PrintText(0, &entity->username);
#endif
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animator, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->username);
            int width        = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->username, 0, entity->username.textLength, 0);
            entity->state    = UIUsernamePopup_Unknown4;
            entity->timer    = 0;
            entity->field_68 = (width + 16) << 16;
            entity->timeOut  = 30;
        }
    }
}

void UIUsernamePopup_DrawSprites(void)
{
    RSDK_THIS(UIUsernamePopup);
    Vector2 drawPos;

    drawPos.x = entity->posUnknown.x + entity->dword6C + (RSDK_screens->position.x << 16) + (entity->field_68 >> 1);
    drawPos.y =
        entity->posUnknown.y + (RSDK_screens->centerY << 16) - (entity->dword6C >> 1) + ((RSDK_screens->centerY + RSDK_screens->position.y) << 16);
    UIWidgets_Unknown7(entity->dword6C >> 16, entity->field_68 >> 16, entity->dword6C >> 16, 16, 124, 16, drawPos.x, drawPos.y);

    int width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->username, 0, entity->username.textLength, 0);
    drawPos.y -= 0x10000;
    drawPos.x -= width << 15;
    RSDK.DrawText(&entity->animator, &drawPos, &entity->username, 0, entity->username.textLength, ALIGN_LEFT, 0, 0, 0, false);
}

void UIUsernamePopup_State_Setup(void)
{
    RSDK_THIS(UIUsernamePopup);
    entity->drawFlag = false;
    entity->timer    = 0;
}

void UIUsernamePopup_Unknown4(void)
{
    RSDK_THIS(UIUsernamePopup);

    if (entity->timer >= entity->timeOut) {
        if (entity->timer >= entity->timeOut + 8) {
            entity->posUnknown.x = 0;
            entity->posUnknown.y = 0;
            entity->timer        = 0;
            entity->state        = UIUsernamePopup_Unknown5;
        }
        else {
            entity->drawFlag = true;
            MathHelpers_Unknown1(&entity->posUnknown, 32 * clampVal(entity->timer - entity->timeOut, 0, 8), -entity->dword6C, entity->dword6C, 0, 0);
            ++entity->timer;
        }
    }
    else {
        entity->drawFlag = false;
        entity->timer++;
    }
}

void UIUsernamePopup_Unknown5(void)
{
    RSDK_THIS(UIUsernamePopup);
    if (entity->timer >= 180) {
        entity->timer = 0;
        entity->state = UIUsernamePopup_Unknown6;
    }
    else {
        entity->timer++;
    }
}

void UIUsernamePopup_Unknown6(void)
{
    RSDK_THIS(UIUsernamePopup);
    if (entity->timer >= 8) {
        entity->posUnknown.y = entity->dword6C;
        entity->timer        = 0;
        entity->drawFlag     = false;
        entity->posUnknown.x = -entity->dword6C;
        entity->state        = UIUsernamePopup_State_Setup;
    }
    else {
        entity->drawFlag = true;

        int val = 32 * maxVal(entity->timer, 0);
        if (val > 0) {
            if (val < 256) {
                entity->posUnknown.x = val * (-entity->dword6C >> 8);
                entity->posUnknown.y = val * (entity->dword6C >> 8);
            }
            else {
                entity->posUnknown.x = -entity->dword6C;
                entity->posUnknown.y = entity->dword6C;
            }
        }
        else {
            entity->posUnknown.x = 0;
            entity->posUnknown.y = 0;
        }
        ++entity->timer;
    }
}

void UIUsernamePopup_EditorDraw(void) {}

void UIUsernamePopup_EditorLoad(void) {}

void UIUsernamePopup_Serialize(void) {}
