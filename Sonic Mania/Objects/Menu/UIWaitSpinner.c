#include "SonicMania.h"

ObjectUIWaitSpinner *UIWaitSpinner;

void UIWaitSpinner_Update(void)
{
    RSDK_THIS(UIWaitSpinner);
    StateMachine_Run(entity->state);

    entity->visible = true;
    if (entity->timer < 16) {
        if (entity->timer == 8) {
            entity->inkEffect = INK_BLEND;
        }
        else if (!entity->timer) {
            entity->visible = false;
        }
    }
    else {
        entity->inkEffect = INK_NONE;
    }

    entity->inkEffect = INK_ALPHA;
    if (16 * entity->timer < 255)
        entity->alpha = 16 * entity->timer;
    else
        entity->alpha = 255;
    RSDK.ProcessAnimation(&entity->animator);
}

void UIWaitSpinner_LateUpdate(void) {}

void UIWaitSpinner_StaticUpdate(void) {}

void UIWaitSpinner_Draw(void)
{
    RSDK_THIS(UIWaitSpinner);
    Vector2 drawPos;

    drawPos.y = 0;
    drawPos.x = (RSDK_screens->width - 24) << 16;
    drawPos.y = (RSDK_screens->height - 24) << 16;
    RSDK.DrawSprite(&entity->animator, &drawPos, true);
}

void UIWaitSpinner_Create(void *data)
{
    RSDK_THIS(UIWaitSpinner);
    entity->active    = ACTIVE_ALWAYS;
    entity->visible   = 1;
    entity->drawOrder = 15;
    RSDK.SetSpriteAnimation(UIWaitSpinner->spriteIndex, 0, &entity->animator, true, 0);
    entity->state = UIWaitSpinner_State_Wait;
}

void UIWaitSpinner_StageLoad(void)
{
    UIWaitSpinner->timer         = 0;
    UIWaitSpinner->activeSpinner = 0;
    UIWaitSpinner->spriteIndex   = RSDK.LoadSpriteAnimation("UI/WaitSpinner.bin", SCOPE_STAGE);
}

void UIWaitSpinner_Wait(void)
{
    ++UIWaitSpinner->timer;
    EntityUIWaitSpinner *activeSpinner = (EntityUIWaitSpinner *)UIWaitSpinner->activeSpinner;
    if (UIWaitSpinner->timer <= 0) {
        if (activeSpinner)
            activeSpinner->state = UIWaitSpinner_State_Wait2;
    }
    else {
        if (!activeSpinner) {
            activeSpinner                = CREATE_ENTITY(UIWaitSpinner, NULL, 0, 0);
            activeSpinner->isPermanent   = true;
            UIWaitSpinner->activeSpinner = (Entity *)activeSpinner;
        }
        activeSpinner->state = UIWaitSpinner_State_Wait;
    }
}
void UIWaitSpinner_Wait2(void)
{
    if (UIWaitSpinner->timer > 0) {
        UIWaitSpinner->timer--;
    }

    EntityUIWaitSpinner *activeSpinner = (EntityUIWaitSpinner *)UIWaitSpinner->activeSpinner;
    if (UIWaitSpinner->timer <= 0) {
        if (activeSpinner)
            activeSpinner->state = UIWaitSpinner_State_Wait2;
    }
    else {
        if (!activeSpinner) {
            activeSpinner                = CREATE_ENTITY(UIWaitSpinner, NULL, 0, 0);
            activeSpinner->isPermanent   = true;
            UIWaitSpinner->activeSpinner = (Entity *)activeSpinner;
        }
        activeSpinner->state = UIWaitSpinner_State_Wait;
    }
}
void UIWaitSpinner_State_Wait(void)
{
    RSDK_THIS(UIWaitSpinner);
    if (entity->timer >= 16) {
        entity->timer = 16;
        entity->flag  = true;
    }
    else {
        entity->timer += 3;
    }
}
void UIWaitSpinner_State_Wait2(void)
{
    RSDK_THIS(UIWaitSpinner);
    if (entity->timer <= 0) {
        UIWaitSpinner->activeSpinner = NULL;
        destroyEntity(entity);
    }
    else if (entity->flag) {
        entity->timer -= 3;
    }
    else if (entity->timer >= 16) {
        entity->timer = 16;
        entity->flag  = true;
    }
    else {
        entity->timer += 3;
    }
}

void UIWaitSpinner_EditorDraw(void) {}

void UIWaitSpinner_EditorLoad(void) {}

void UIWaitSpinner_Serialize(void) {}
