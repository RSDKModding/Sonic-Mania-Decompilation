// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggTower Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEggTower *EggTower;

void EggTower_Update(void) {}

void EggTower_LateUpdate(void) {}

void EggTower_StaticUpdate(void) {}

void EggTower_Draw(void)
{
    RSDK_THIS(EggTower);

    int32 y = self->position.y - 160 * ((self->position.y - ((ScreenInfo->position.y + ScreenInfo->center.y) << 16)) >> 8);
    if (y > (ScreenInfo->position.y + 288) << 16)
        y -= (y - (ScreenInfo->position.y << 16) - 0x1200000) >> 1;

    RSDK.MatrixScaleXYZ(&self->matWorld, self->scale.x, -self->scale.x, self->scale.x);
    RSDK.MatrixTranslateXYZ(&self->matWorld, (ScreenInfo->position.x + ScreenInfo->center.x) << 16, y, 0, false);
    RSDK.MatrixRotateY(&self->matTemp, (self->rotationX + ScreenInfo->position.x) / -6);
    RSDK.MatrixMultiply(&self->matWorld, &self->matTemp, &self->matWorld);

    RSDK.Prepare3DScene(EggTower->sceneIndex);

    RSDK.AddModelTo3DScene(EggTower->modelIndex, EggTower->sceneIndex, S3D_SOLIDCOLOR_SHADED_BLENDED, &self->matWorld, NULL, 0x000000);

    RSDK.Draw3DScene(EggTower->sceneIndex);
}

void EggTower_Create(void *data)
{
    RSDK_THIS(EggTower);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 1;
        self->scale.x       = 0x37C;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
    }
}

void EggTower_StageLoad(void)
{
    EggTower->modelIndex = RSDK.LoadMesh("SSZ/EggTower.bin", SCOPE_STAGE);
    EggTower->sceneIndex = RSDK.Create3DScene("View:EggTower", 0x2000, SCOPE_STAGE);

    RSDK.SetDiffuseColor(EggTower->sceneIndex, 0xC0, 0xC0, 0xC0);
    RSDK.SetDiffuseIntensity(EggTower->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(EggTower->sceneIndex, 16, 16, 16);
}

#if GAME_INCLUDE_EDITOR
void EggTower_EditorDraw(void) {}

void EggTower_EditorLoad(void) {}
#endif

void EggTower_Serialize(void) {}
