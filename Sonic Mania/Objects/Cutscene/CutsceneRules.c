#include "SonicMania.h"

ObjectCutsceneRules *CutsceneRules = NULL;

void CutsceneRules_Update(void)
{

}

void CutsceneRules_LateUpdate(void)
{

}

void CutsceneRules_StaticUpdate(void)
{

}

void CutsceneRules_Draw(void)
{

}

void CutsceneRules_Create(void* data)
{

}

void CutsceneRules_StageLoad(void)
{

}


void CutsceneRules_SetupEntity(void *e)
{
    EntityCutsceneRules *entity = (EntityCutsceneRules *)e;
    if (!entity->size.x)
        entity->size.x = 0x1A80000;
    if (!entity->size.y)
        entity->size.y = 0xF00000;
    entity->updateRange.x       = 0x800000 + entity->size.x;
    entity->updateRange.y       = 0x800000 + entity->size.y;
    entity->hitbox.left         = -entity->size.x >> 17;
    entity->hitbox.right        = entity->size.x >> 17;
    entity->hitbox.top          = -entity->size.y >> 17;
    entity->hitbox.bottom       = entity->size.y >> 17;
}

void CutsceneRules_EditorDraw(void)
{

}

void CutsceneRules_EditorLoad(void)
{

}

void CutsceneRules_Serialize(void)
{

}

