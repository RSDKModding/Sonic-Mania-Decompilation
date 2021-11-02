#include "SonicMania.h"

ObjectCutsceneRules *CutsceneRules = NULL;

void CutsceneRules_Update(void) {}

void CutsceneRules_LateUpdate(void) {}

void CutsceneRules_StaticUpdate(void) {}

void CutsceneRules_Draw(void) {}

void CutsceneRules_Create(void *data) {}

void CutsceneRules_StageLoad(void) {}

void CutsceneRules_SetupEntity(void *e, Vector2 *size, Hitbox *hitbox)
{
    EntityCutsceneRules *entity = (EntityCutsceneRules *)e;
    if (!size->x)
        size->x = 0x1A80000;
    if (!size->y)
        size->y = 0xF00000;
    entity->updateRange.x = 0x800000 + size->x;
    entity->updateRange.y = 0x800000 + size->y;
    hitbox->left   = -size->x >> 17;
    hitbox->right  = size->x >> 17;
    hitbox->top    = -size->y >> 17;
    hitbox->bottom = size->y >> 17;
}

void CutsceneRules_DrawCutsceneBounds(void *e, Vector2 *size)
{
    EntityCutsceneRules *entity = (EntityCutsceneRules *)e;
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= size->x >> 1;
    drawPos.y -= size->y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + size->x, drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, size->y + drawPos.y, drawPos.x + size->x, size->y + drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + size->y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + size->x, drawPos.y, drawPos.x + size->x, drawPos.y + size->y, 0xFFFF00, 0, INK_NONE, false);
}

#if RETRO_INCLUDE_EDITOR
void CutsceneRules_EditorDraw(void) {}

void CutsceneRules_EditorLoad(void) {}
#endif

void CutsceneRules_Serialize(void) {}
