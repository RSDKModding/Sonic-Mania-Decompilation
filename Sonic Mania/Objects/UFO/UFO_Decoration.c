#include "SonicMania.h"

ObjectUFO_Decoration *UFO_Decoration;

void UFO_Decoration_Update(void)
{
    RSDK_THIS(UFO_Decoration);
    if (RSDK.CheckOnScreen(entity, NULL)) {
        entity->scale.x += (entity->size - entity->scale.x) >> 3;
        entity->active = ACTIVE_NORMAL;
    }
    else {
        entity->scale.x = 0;
        entity->active  = ACTIVE_BOUNDS;
    }

    if (entity->type > 5)
        RSDK.ProcessAnimation(&entity->animator);
}

void UFO_Decoration_LateUpdate(void)
{
    RSDK_THIS(UFO_Decoration);
    int32 z = entity->position.y;
    int32 x = entity->position.x;

    Matrix *mat = &UFO_Camera->matWorld;

    entity->depth3D = mat->values[2][1] * (entity->height >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];

    if (entity->depth3D >= 0x4000) {
        int32 val = (int32)((mat->values[0][3] << 8) + (mat->values[0][2] * (z >> 8) & 0xFFFFFF00) + (mat->values[0][0] * (x >> 8) & 0xFFFFFF00)
                   + (mat->values[0][1] * (entity->height >> 8) & 0xFFFFFF00))
                  / entity->depth3D;
        entity->visible = abs(val) < 256;
    }
}

void UFO_Decoration_StaticUpdate(void) {}

void UFO_Decoration_Draw(void)
{
    RSDK_THIS(UFO_Decoration);
    if (entity->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(UFO_Decoration->sceneIndex);
        RSDK.MatrixScaleXYZ(&entity->matrix1, entity->scale.x, entity->size, entity->scale.x);
        RSDK.MatrixTranslateXYZ(&entity->matrix1, entity->position.x, entity->height, entity->position.y, 0);
        RSDK.MatrixRotateY(&entity->matrix3, entity->angle);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix3, &entity->matrix1);
        RSDK.MatrixMultiply(&entity->matrix2, &entity->matrix2, &UFO_Camera->matWorld);
        if (UFO_Camera->isSS7)
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &UFO_Camera->matTemp);
        else
            RSDK.MatrixMultiply(&entity->matrix3, &entity->matrix3, &UFO_Camera->matView);

        if (entity->type <= 5)
            RSDK.AddModelTo3DScene(UFO_Decoration->modelIndices[entity->type], UFO_Decoration->sceneIndex, UFO_Decoration->drawType, &entity->matrix2,
                                   &entity->matrix3, 0xFFFFFF);
        else
            RSDK.AddMeshFrameTo3DScene(UFO_Decoration->modelIndices[entity->type], UFO_Decoration->sceneIndex, &entity->animator,
                                       UFO_Decoration->drawType, &entity->matrix2, &entity->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(UFO_Decoration->sceneIndex);
    }
}

void UFO_Decoration_Create(void *data)
{
    RSDK_THIS(UFO_Decoration);
    if (!RSDK_sceneInfo->inEditor) {
        if (!entity->size)
            entity->size = 0x400;
        entity->visible       = true;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x4000000;
        entity->updateRange.y = 0x4000000;
        if (entity->type == 6)
            entity->height = 0x600000;
        RSDK.SetModelAnimation(UFO_Decoration->modelIndices[entity->type], &entity->animator, 96, 0, true, 0);
    }
}

void UFO_Decoration_StageLoad(void)
{
    UFO_Decoration->modelIndices[0] = RSDK.LoadMesh("Decoration/Tree.bin", SCOPE_STAGE);
    UFO_Decoration->modelIndices[1] = RSDK.LoadMesh("Decoration/Flower1.bin", SCOPE_STAGE);
    UFO_Decoration->modelIndices[2] = RSDK.LoadMesh("Decoration/Flower2.bin", SCOPE_STAGE);
    UFO_Decoration->modelIndices[3] = RSDK.LoadMesh("Decoration/Flower3.bin", SCOPE_STAGE);
    UFO_Decoration->modelIndices[4] = RSDK.LoadMesh("Decoration/Pillar1.bin", SCOPE_STAGE);
    UFO_Decoration->modelIndices[5] = RSDK.LoadMesh("Decoration/Pillar2.bin", SCOPE_STAGE);
    UFO_Decoration->modelIndices[6] = RSDK.LoadMesh("Decoration/Bird.bin", SCOPE_STAGE);
    UFO_Decoration->modelIndices[7] = RSDK.LoadMesh("Decoration/Fish.bin", SCOPE_STAGE);
    UFO_Decoration->sceneIndex      = RSDK.Create3DScene("View:Special", 4096, SCOPE_STAGE);
    UFO_Decoration->drawType        = S3D_FLATCLR_SHADED_BLENDED_SCREEN;
}

#if RETRO_INCLUDE_EDITOR
void UFO_Decoration_EditorDraw(void) {}

void UFO_Decoration_EditorLoad(void) {}
#endif

void UFO_Decoration_Serialize(void)
{
    RSDK_EDITABLE_VAR(UFO_Decoration, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(UFO_Decoration, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(UFO_Decoration, VAR_ENUM, size);
}
