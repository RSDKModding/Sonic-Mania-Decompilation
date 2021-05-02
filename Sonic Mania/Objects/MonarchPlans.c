#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectMonarchPlans *MonarchPlans;

void MonarchPlans_Update(void)
{
    RSDK_THIS(MonarchPlans);

    entity->rotationY = (entity->rotationY + 4) & 0x3FF;
    RSDK.MatrixScaleXYZ(&entity->matrix2, entity->scale.x, -entity->scale.x, entity->scale.x);
    RSDK.MatrixTranslateXYZ(&entity->matrix2, entity->position.x, entity->position.y, 0, 0);
    RSDK.MatrixRotateXYZ(&entity->matrix3, 0, entity->rotationY, entity->rotationZ);
    RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &entity->matrix2);
    RSDK.MatrixRotateX(&entity->matrix1, 0x1E0);
    RSDK.MatrixRotateXYZ(&entity->matrix4, 0, entity->rotationY, entity->rotationZ);
    RSDK.MatrixMultiply(&entity->matrix4, &entity->matrix4, &entity->matrix1);
}

void MonarchPlans_LateUpdate(void) {}

void MonarchPlans_StaticUpdate(void) {}

void MonarchPlans_Draw(void)
{
    RSDK_THIS(MonarchPlans);

    RSDK.Prepare3DScene(MonarchPlans->sceneIndex);
    RSDK.AddModelTo3DScene(MonarchPlans->meshIndex, MonarchPlans->sceneIndex, S3D_FLATCLR_WIREFRAME, &entity->matrix3, &entity->matrix4, 0x609090);
    RSDK.Draw3DScene(MonarchPlans->sceneIndex);
}

void MonarchPlans_Create(void *data)
{
    RSDK_THIS(MonarchPlans);

    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->inkEffect     = INK_BLEND;
        entity->visible       = true;
        entity->updateRange.x = 0x900000;
        entity->updateRange.y = 0x900000;
        entity->scale.x       = 0xA0;
        entity->scale.y       = 0xA0;
        entity->drawOrder     = Zone->drawOrderLow;
    }
}

void MonarchPlans_StageLoad(void)
{
    MonarchPlans->aniFrames  = RSDK.LoadSpriteAnimation("Global/MonarchPlans.bin", SCOPE_STAGE);
    MonarchPlans->meshIndex  = RSDK.LoadMesh("SSZ/MonarchPlans.bin", SCOPE_STAGE);
    MonarchPlans->sceneIndex = RSDK.Create3DScene("View:MonarchPlans", 0x2000, SCOPE_STAGE);

    MonarchPlans->hitbox.left   = -0x18;
    MonarchPlans->hitbox.top    = -0x17;
    MonarchPlans->hitbox.right  = 0x18;
    MonarchPlans->hitbox.bottom = 0x18;
    RSDK.SetDiffuseColour(MonarchPlans->sceneIndex, 160, 160, 160);
    RSDK.SetDiffuseIntensity(MonarchPlans->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(MonarchPlans->sceneIndex, 14, 14, 14);
}

void MonarchPlans_EditorDraw(void) {}

void MonarchPlans_EditorLoad(void) {}

void MonarchPlans_Serialize(void) {}
#endif
