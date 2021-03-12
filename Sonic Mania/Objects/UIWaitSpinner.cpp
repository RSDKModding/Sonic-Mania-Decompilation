#include "../SonicMania.hpp"

ObjectUIWaitSpinner *UIWaitSpinner;

void UIWaitSpinner_Update()
{
    EntityUIWaitSpinner *entity = (EntityUIWaitSpinner *)RSDK_sceneInfo->entity;

    if (entity->state)
        entity->state();

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
    RSDK.ProcessAnimation(&entity->data);
}

void UIWaitSpinner_LateUpdate()
{

}

void UIWaitSpinner_StaticUpdate()
{

}

void UIWaitSpinner_Draw()
{
    EntityUIWaitSpinner *entity = (EntityUIWaitSpinner *)RSDK_sceneInfo->entity;
    Vector2 drawPos; 

    drawPos.y = 0;
    drawPos.x = (RSDK_screens->width - 24) << 16;
    drawPos.y = (RSDK_screens->height - 24) << 16;
    RSDK.DrawSprite(&entity->data, &drawPos, true);
}

void UIWaitSpinner_Create(void* data)
{
    EntityUIWaitSpinner *entity                 = (EntityUIWaitSpinner *)RSDK_sceneInfo->entity;
    entity->active    = ACTIVE_ALWAYS;
    entity->visible   = 1;
    entity->drawOrder = 15;
    RSDK.SetSpriteAnimation(UIWaitSpinner->spriteIndex, 0, &entity->data, true, 0);
    entity->state = UIWaitSpinner_State_Wait;
}

void UIWaitSpinner_StageLoad()
{
    UIWaitSpinner->timer         = 0;
    UIWaitSpinner->activeSpinner = 0;
    UIWaitSpinner->spriteIndex   = RSDK.LoadAnimation("UI/WaitSpinner.bin", SCOPE_STAGE);
}

void UIWaitSpinner_Wait()
{
    ++UIWaitSpinner->timer;
    EntityUIWaitSpinner *activeSpinner = (EntityUIWaitSpinner *)UIWaitSpinner->activeSpinner;
    if (UIWaitSpinner->timer <= 0) {
        if (activeSpinner)
            activeSpinner->state = UIWaitSpinner_State_Wait2;
    }
    else {
        if (!activeSpinner) {
            activeSpinner                = (EntityUIWaitSpinner *)RSDK.SpawnEntity(UIWaitSpinner->objectID, 0, 0, 0);
            activeSpinner->field_3C      = 1;
            UIWaitSpinner->activeSpinner = activeSpinner;
        }
        activeSpinner->state = UIWaitSpinner_State_Wait;
    }
}
void UIWaitSpinner_WaitReplay()
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
            activeSpinner                = (EntityUIWaitSpinner *)RSDK.SpawnEntity(UIWaitSpinner->objectID, 0, 0, 0);
            activeSpinner->field_3C      = 1;
            UIWaitSpinner->activeSpinner = activeSpinner;
        }
        activeSpinner->state = UIWaitSpinner_State_Wait;
    }
}
void UIWaitSpinner_State_Wait()
{
    EntityUIWaitSpinner *entity = (EntityUIWaitSpinner *)RSDK_sceneInfo->entity;
    if (entity->timer >= 16) {
        entity->timer = 16;
        entity->flag  = true;
    }
    else {
        entity->timer += 3;
    }
}
void UIWaitSpinner_State_Wait2()
{
    EntityUIWaitSpinner *entity = (EntityUIWaitSpinner *)RSDK_sceneInfo->entity;
    if (entity->timer <= 0) {
        UIWaitSpinner->activeSpinner = 0;
        RSDK.DestroyEntity(entity, 0, 0);
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

void UIWaitSpinner_EditorDraw()
{

}

void UIWaitSpinner_EditorLoad()
{

}

void UIWaitSpinner_Serialize()
{

}

