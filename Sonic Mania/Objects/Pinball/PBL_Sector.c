#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Sector *PBL_Sector;

void PBL_Sector_Update(void)
{
    RSDK_THIS(PBL_Sector);
    if (self->timer <= 0) {
        foreach_active(PBL_Player, player)
        {
            if (self->flag) {
                if (player->position.y <= self->position.y - self->size.y - 0x100000 && player->velocity.y < 0) {
                    foreach_active(PBL_HUD, hud) { PBL_HUD_DisplayMessage(hud, "GO! GO!", 7); }
                    player->position.y = self->position.y - self->size.y - 0xC0000;
                    player->active     = ACTIVE_NEVER;
                    RSDK.PlaySfx(PBL_Crane->sfxBackGlass, 0, 255);
                    PBL_Setup_GiveScore(1000);
                    if (self->craneID >= PBL_Setup->sectorCount) {
                        self->active = ACTIVE_NEVER;
                        self->timer  = 60;
                    }
                    else {
                        destroyEntity(self);
                    }
                }
            }
            else {
                if (abs(player->position.x - self->position.x) < self->size.x) {
                    if (abs(player->position.y - self->position.y) < self->size.y) {
                        EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
                        camera->dword74          = self->position.y - self->size.y;
                        camera->dword78          = self->position.y + self->size.y;
                        self->flag             = true;
                    }
                }
            }
        }
    }
    else {
        self->timer--;
    }
}

void PBL_Sector_LateUpdate(void) {}

void PBL_Sector_StaticUpdate(void) {}

void PBL_Sector_Draw(void) {}

void PBL_Sector_Create(void *data)
{
    RSDK_THIS(PBL_Sector);
    if (!SceneInfo->inEditor) {
        self->updateRange.x = self->size.x;
        self->updateRange.y = self->size.y;
        if (self->craneID == PBL_Setup->sectorID) {
            EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
            camera->dword6C          = self->position.y - self->size.y;
            camera->dword74          = self->position.y - self->size.y;
            camera->dword70          = self->size.y + self->position.y;
            camera->dword78          = self->size.y + self->position.y;
            self->active           = ACTIVE_NORMAL;
        }
        if (self->craneID > PBL_Setup->sectorCount)
            PBL_Setup->sectorCount = self->craneID;
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
