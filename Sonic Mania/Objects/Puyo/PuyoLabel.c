#include "SonicMania.h"

ObjectPuyoLabel *PuyoLabel;

void PuyoLabel_Update(void) {}

void PuyoLabel_LateUpdate(void) {}

void PuyoLabel_StaticUpdate(void) {}

void PuyoLabel_Draw(void)
{
    RSDK_THIS(PuyoLabel);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PuyoLabel_Create(void *data)
{
    RSDK_THIS(PuyoLabel);
    entity->active        = ACTIVE_NORMAL;
    entity->drawOrder     = 10;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(PuyoLabel->aniFrames, entity->listID, &entity->animator, true, entity->frame);
}

void PuyoLabel_StageLoad(void) { PuyoLabel->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void PuyoLabel_EditorDraw(void) {}

void PuyoLabel_EditorLoad(void) {}
#endif

void PuyoLabel_Serialize(void)
{
    RSDK_EDITABLE_VAR(PuyoLabel, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(PuyoLabel, VAR_ENUM, frame);
}
