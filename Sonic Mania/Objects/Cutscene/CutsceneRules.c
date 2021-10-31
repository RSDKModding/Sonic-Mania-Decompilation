#include "SonicMania.h"

ObjectCutsceneRules *CutsceneRules = NULL;

void CutsceneRules_Update(void) {}

void CutsceneRules_LateUpdate(void) {}

void CutsceneRules_StaticUpdate(void) {}

void CutsceneRules_Draw(void) {}

void CutsceneRules_Create(void *data) {}

void CutsceneRules_StageLoad(void) {}

void CutsceneRules_SetupEntity(void *e)
{
    EntityCutsceneRules *entity = (EntityCutsceneRules *)e;
    if (!entity->size.x)
        entity->size.x = 0x1A80000;
    if (!entity->size.y)
        entity->size.y = 0xF00000;
    entity->updateRange.x = 0x800000 + entity->size.x;
    entity->updateRange.y = 0x800000 + entity->size.y;
    entity->hitbox.left   = -entity->size.x >> 17;
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.top    = -entity->size.y >> 17;
    entity->hitbox.bottom = entity->size.y >> 17;
}

void CutsceneRules_DrawCutsceneBounds(void *e)
{
    EntityCutsceneRules *entity = (EntityCutsceneRules *)e;
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
}

#if RETRO_INCLUDE_EDITOR
void CutsceneRules_EditorDraw(void) {}

void CutsceneRules_EditorLoad(void) {}
#endif

void CutsceneRules_Serialize(void) {}
