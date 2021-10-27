#include "SonicMania.h"

ObjectDAControl *DAControl;

void DAControl_Update(void)
{
    RSDK_THIS(DAControl);
    StateMachine_Run(entity->state);
}

void DAControl_LateUpdate(void) {}

void DAControl_StaticUpdate(void) {}

void DAControl_Draw(void)
{
    RSDK_THIS(DAControl);
    Vector2 drawPos;

    entity->direction = FLIP_NONE;
    for (int32 i = 0; i < 2; ++i) {
        drawPos                   = entity->position;
        entity->animator1.frameID = 0;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        drawPos.y += 0x80000;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        drawPos.y += 0x80000;
        entity->animator1.frameID = 2;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        drawPos.y -= 0x170000;
        entity->animator1.frameID = 3;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        ++entity->direction;
    }

    entity->direction = FLIP_NONE;
    drawPos.y -= 0x130000;
    entity->animator1.frameID = 4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x = entity->position.x - 0x700000;
    drawPos.y = entity->position.y + 0x1E0000;
    for (int32 i = 0; i < 5; ++i) {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        RSDK.DrawSprite(&entity->animator3[i], &drawPos, false);
        drawPos.x += 0x380000;
    }

    drawPos.x = entity->position.x - 0x800000;
    drawPos.y = entity->position.y;
    if (!RSDK_sceneInfo->inEditor)
        RSDK.DrawText(&entity->animator4, &drawPos, &entity->text, 0, entity->text.textLength, ALIGN_LEFT, 0, 0, 0, false);
}

void DAControl_Create(void *data)
{
    RSDK_THIS(DAControl);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 4, &entity->animator4, true, 0);
        for (int32 i = 0; i < 5; ++i) RSDK.SetSpriteAnimation(DAControl->aniFrames, 2, &entity->animator3[i], true, i);

        RSDK.SetText(&entity->text, "SELECT A TRACK...", 0);
        RSDK.SetSpriteAnimation(DAControl->aniFrames, 4, &entity->animator4, true, 0);
        RSDK.SetSpriteString(DAControl->aniFrames, 4, &entity->text);
    }
}

void DAControl_StageLoad(void) { DAControl->aniFrames = RSDK.LoadSpriteAnimation("UI/DAGarden.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void DAControl_EditorDraw(void) {}

void DAControl_EditorLoad(void) {}
#endif

void DAControl_Serialize(void) {}
