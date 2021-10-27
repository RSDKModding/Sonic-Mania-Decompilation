#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Sector *PBL_Sector;

void PBL_Sector_Update(void)
{
    RSDK_THIS(PBL_Sector);
    if (entity->timer <= 0) {
        foreach_active(PBL_Player, player)
        {
            if (entity->flag) {
                if (player->position.y <= entity->position.y - entity->size.y - 0x100000 && player->velocity.y < 0) {
                    foreach_active(PBL_HUD, hud) { PBL_HUD_DisplayMessage(hud, "GO! GO!", 7); }
                    player->position.y = entity->position.y - entity->size.y - 0xC0000;
                    player->active     = ACTIVE_NEVER;
                    RSDK.PlaySfx(PBL_Crane->sfxBackGlass, 0, 255);
                    PBL_Setup_GiveScore(1000);
                    if (entity->craneID >= PBL_Setup->sectorCount) {
                        entity->active = ACTIVE_NEVER;
                        entity->timer  = 60;
                    }
                    else {
                        destroyEntity(entity);
                    }
                }
            }
            else {
                if (abs(player->position.x - entity->position.x) < entity->size.x) {
                    if (abs(player->position.y - entity->position.y) < entity->size.y) {
                        EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
                        camera->dword74          = entity->position.y - entity->size.y;
                        camera->dword78          = entity->position.y + entity->size.y;
                        entity->flag             = true;
                    }
                }
            }
        }
    }
    else {
        entity->timer--;
    }
}

void PBL_Sector_LateUpdate(void) {}

void PBL_Sector_StaticUpdate(void) {}

void PBL_Sector_Draw(void) {}

void PBL_Sector_Create(void *data)
{
    RSDK_THIS(PBL_Sector);
    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = entity->size.x;
        entity->updateRange.y = entity->size.y;
        if (entity->craneID == PBL_Setup->sectorID) {
            EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
            camera->dword6C          = entity->position.y - entity->size.y;
            camera->dword74          = entity->position.y - entity->size.y;
            camera->dword70          = entity->size.y + entity->position.y;
            camera->dword78          = entity->size.y + entity->position.y;
            entity->active           = ACTIVE_NORMAL;
        }
        if (entity->craneID > PBL_Setup->sectorCount)
            PBL_Setup->sectorCount = entity->craneID;
    }
}

void PBL_Sector_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void PBL_Sector_EditorDraw(void) {}

void PBL_Sector_EditorLoad(void) {}
#endif

void PBL_Sector_Serialize(void)
{
    RSDK_EDITABLE_VAR(PBL_Sector, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(PBL_Sector, VAR_UINT8, craneID);
}
#endif
