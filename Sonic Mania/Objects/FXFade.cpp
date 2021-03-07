#include "../SonicMania.hpp"

ObjectFXFade *FXFade;

void FXFade_Update()
{
    EntityFXFade *entity = (EntityFXFade *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void FXFade_LateUpdate()
{

}

void FXFade_StaticUpdate()
{

}

void FXFade_Draw()
{
    EntityFXFade *entity = (EntityFXFade *)RSDK_sceneInfo->entity;
    RSDK.FillScreen(entity->color, entity->timer, entity->timer - 128, entity->timer - 256);
}

void FXFade_Create(void *data)
{
    EntityFXFade *entity = (EntityFXFade *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        entity->active  = ACTIVE_NORMAL;
        if (RSDK.CheckStageFolder("Menu")) {
            entity->drawOrder = 14;
        }
        else if (Zone) {
            if (!entity->overHUD)
                entity->drawOrder = Zone->uiDrawHigh - 1;
            else
                entity->drawOrder = Zone->uiDrawHigh;
        }
        else {
            entity->drawOrder = 15;
        }
        if (!entity->speedIn)
            entity->speedIn = 32;
        if (!entity->speedOut)
            entity->speedOut = 32;
        if (!entity->color)
            entity->color = (uint)(size_t)data;
        if (!entity->eventOnly) {
            entity->dword80 = 0;
            if (entity->timer <= 0)
                entity->state = FXFade_State_FadeIn;
            else
                entity->state = FXFade_State_FadeOut;
        }
    }
}

void FXFade_StageLoad()
{

}

void FXFade_StopAll()
{
    Entity *entity = NULL;
    while (RSDK.GetObjects(FXFade->objectID, &entity)) RSDK.DestroyEntity(entity, 0, 0);
}

void FXFade_State_FadeIn()
{
    EntityFXFade *entity = (EntityFXFade *)RSDK_sceneInfo->entity;
    if (entity->timer >= 512) {
        if (entity->oneWay) {
            entity->state = NULL;
            if (entity->dword80)
                RSDK.InitSceneLoad();
        }
        else if (entity->wait <= 0) {
            if (entity->fadeOutBlack)
                entity->state = FXFade_State_FadeOutBlack;
            else
                entity->state = FXFade_State_FadeOut;
        }
        else {
            entity->state = FXFade_State_Wait;
        }
    }
    else {
        entity->timer += entity->speedIn;
    }
}
void FXFade_State_Wait()
{
    EntityFXFade *entity = (EntityFXFade *)RSDK_sceneInfo->entity;
    if (--entity->wait <= 0) {
        if (entity->fadeOutBlack)
            entity->state = FXFade_State_FadeOutBlack;
        else
            entity->state = FXFade_State_FadeOut;
    }
}
void FXFade_State_FadeOut()
{
    EntityFXFade *entity = (EntityFXFade *)RSDK_sceneInfo->entity;
    if (entity->timer <= 0) {
        if (entity->oneWay)
            entity->state = NULL;
        else
            RSDK.DestroyEntity(entity, 0, 0);
    }
    else {
        entity->timer -= entity->speedOut;
    }
}
void FXFade_State_FadeOutBlack()
{
    EntityFXFade *entity = (EntityFXFade *)RSDK_sceneInfo->entity;
    if (entity->color >> 16) {
        if (((entity->color >> 16) - entity->speedOut) >= 0)
            entity->color += ((((entity->color >> 8) & 0xFF) + (((entity->color >> 16) - entity->speedOut) << 8)) << 8);
        else
            entity->color = 0;
    }
    else if ((entity->color >> 8) & 0xFF) {
        if ((((entity->color >> 8) & 0xFF) - entity->speedOut) >= 0)
            entity->color += (((entity->color >> 8) & 0xFF) - entity->speedOut) << 8;
        else
            entity->color = 0;
    }
    else if (entity->color & 0xFF) {
        if ((entity->color & 0xFF) - entity->speedOut < 0)
            entity->color = 0;
        else
            entity->color = (entity->color & 0xFF) - entity->speedOut;
    }
    else {
        entity->state = NULL;
    }
}

void FXFade_EditorDraw()
{

}

void FXFade_EditorLoad()
{

}

void FXFade_Serialize()
{
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, speedIn);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, wait);
    RSDK_EDITABLE_VAR(FXFade, VAR_ENUM, speedOut);
    RSDK_EDITABLE_VAR(FXFade, VAR_COLOUR, color);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, oneWay);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, eventOnly);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, overHUD);
    RSDK_EDITABLE_VAR(FXFade, VAR_BOOL, fadeOutBlack);
}

