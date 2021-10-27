#include "SonicMania.h"

ObjectERZRider *ERZRider;

void ERZRider_Update(void)
{
    RSDK_THIS(ERZRider);
    StateMachine_Run(entity->state);
}

void ERZRider_LateUpdate(void) {}

void ERZRider_StaticUpdate(void) {}

void ERZRider_Draw(void)
{
    RSDK_THIS(ERZRider);
    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void ERZRider_Create(void *data)
{
    RSDK_THIS(ERZRider);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->isJimmy       = voidToInt(data);
        if (!voidToInt(data)) {
            entity->drawFX = FX_FLIP;
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 4, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 5, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(ERZRider->aniFrames, 6, &entity->animator4, true, 0);
            entity->startPos  = entity->position;
            entity->stateDraw = ERZRider_Unknown1;
            entity->state     = StateMachine_None;
        }
    }
}

void ERZRider_StageLoad(void) { ERZRider->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomRider.bin", SCOPE_STAGE); }

void ERZRider_Unknown1(void)
{
    // thats all folks!
}

#if RETRO_INCLUDE_EDITOR
void ERZRider_EditorDraw(void) {}

void ERZRider_EditorLoad(void) {}
#endif

void ERZRider_Serialize(void) {}
