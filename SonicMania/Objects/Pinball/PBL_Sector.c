// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_Sector Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_Sector *PBL_Sector;

void PBL_Sector_Update(void)
{
    RSDK_THIS(PBL_Sector);

    if (self->timer <= 0) {
        foreach_active(PBL_Player, player)
        {
            if (self->playerEntered) {
                if (player->position.y <= self->position.y - self->size.y - 0x100000 && player->velocity.y < 0) {
                    foreach_active(PBL_HUD, hud) { PBL_HUD_DisplayMessage(hud, "GO! GO!", PBL_HUD_MSG_FLASH_CRANE); }

                    player->position.y = self->position.y - self->size.y - 0xC0000;
                    player->active     = ACTIVE_NEVER;
                    RSDK.PlaySfx(PBL_Crane->sfxBackGlass, false, 255);

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
                        camera->newCamBoundaryT  = self->position.y - self->size.y;
                        camera->newCamBoundaryB  = self->position.y + self->size.y;

                        self->playerEntered = true;
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
            camera->curCamBoundaryT  = self->position.y - self->size.y;
            camera->newCamBoundaryT  = self->position.y - self->size.y;
            camera->curCamBoundaryB  = self->position.y + self->size.y;
            camera->newCamBoundaryB  = self->position.y + self->size.y;

            self->active = ACTIVE_NORMAL;
        }

        if (self->craneID > PBL_Setup->sectorCount)
            PBL_Setup->sectorCount = self->craneID;
    }
}

void PBL_Sector_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void PBL_Sector_EditorDraw(void)
{
    RSDK_THIS(PBL_Sector);

    RSDK.SetSpriteAnimation(PBL_Sector->aniFrames, 0, &self->animator, true, 8);
    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x << 1, self->size.y << 1, 0xFFFF00);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void PBL_Sector_EditorLoad(void) { PBL_Sector->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void PBL_Sector_Serialize(void)
{
    RSDK_EDITABLE_VAR(PBL_Sector, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(PBL_Sector, VAR_UINT8, craneID);
}
#endif
