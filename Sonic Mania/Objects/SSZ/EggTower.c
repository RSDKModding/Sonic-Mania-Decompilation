#include "SonicMania.h"

ObjectEggTower *EggTower;

void EggTower_Update(void) {}

void EggTower_LateUpdate(void) {}

void EggTower_StaticUpdate(void) {}

void EggTower_Draw(void)
{
    RSDK_THIS(EggTower);
    int y = entity->position.y - 160 * ((entity->position.y - ((RSDK_screens->position.y + RSDK_screens->centerY) << 16)) >> 8);
    if (y > (RSDK_screens->position.y + 288) << 16)
        y -= (y - (RSDK_screens->position.y << 16) - 0x1200000) >> 1;
    RSDK.MatrixScaleXYZ(&entity->matrix2, entity->scale.x, -entity->scale.x, entity->scale.x);
    RSDK.MatrixTranslateXYZ(&entity->matrix2, (RSDK_screens->position.x + RSDK_screens->centerX) << 16, y, 0, false);
    RSDK.MatrixRotateY(&entity->matrix3, (entity->rotationX + RSDK_screens->position.x) / -6);
    RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix3, &entity->matrix2);
    RSDK.Prepare3DScene(EggTower->sceneIndex);
    RSDK.AddModelTo3DScene(EggTower->modelIndex, EggTower->sceneIndex, S3D_FLATCLR_SHADED_BLENDED, &entity->matrix2, NULL, 0x000000);
    RSDK.Draw3DScene(EggTower->sceneIndex);
}

void EggTower_Create(void *data)
{
    RSDK_THIS(EggTower);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 1;
        entity->scale.x       = 0x37C;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
    }
}

void EggTower_StageLoad(void)
{
    EggTower->modelIndex = RSDK.LoadMesh("SSZ/EggTower.bin", SCOPE_STAGE);
    EggTower->sceneIndex = RSDK.Create3DScene("View:EggTower", 0x2000, SCOPE_STAGE);
    RSDK.SetDiffuseColour(EggTower->sceneIndex, 192, 192, 192);
    RSDK.SetDiffuseIntensity(EggTower->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(EggTower->sceneIndex, 16, 16, 16);
}

void EggTower_EditorDraw(void) {}

void EggTower_EditorLoad(void) {}

void EggTower_Serialize(void) {}
