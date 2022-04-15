// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CutsceneRules Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

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
        size->x = WIDE_SCR_XSIZE << 16;
    if (!size->y)
        size->y = SCREEN_YSIZE << 16;
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
    DrawHelpers_DrawRectOutline(entity->position.x, entity->position.y, size->x, size->y, 0xFFFF00);
}

#if RETRO_INCLUDE_EDITOR
void CutsceneRules_EditorDraw(void) {}

void CutsceneRules_EditorLoad(void) {}
#endif

void CutsceneRules_Serialize(void) {}
