#include "SonicMania.h"

ObjectUFO_Decoration *UFO_Decoration;

void UFO_Decoration_Update(void)
{
    RSDK_THIS(UFO_Decoration);
    if (RSDK.CheckOnScreen(self, NULL)) {
        self->scale.x += (self->size - self->scale.x) >> 3;
        self->active = ACTIVE_NORMAL;
    }
    else {
        self->scale.x = 0;
        self->active  = ACTIVE_BOUNDS;
    }

    if (self->type > 5)
        RSDK.ProcessAnimation(&self->animator);
}

void UFO_Decoration_LateUpdate(void)
{
    RSDK_THIS(UFO_Decoration);
    int32 z = self->position.y;
    int32 x = self->position.x;

    Matrix *mat = &UFO_Camera->matWorld;

    self->depth3D = mat->values[2][1] * (self->height >> 16) + mat->values[2][2] * (z >> 16) + mat->values[2][0] * (x >> 16) + mat->values[2][3];

    if (self->depth3D >= 0x4000) {
        int32 val = (int32)((mat->values[0][3] << 8) + (mat->values[0][2] * (z >> 8) & 0xFFFFFF00) + (mat->values[0][0] * (x >> 8) & 0xFFFFFF00)
                   + (mat->values[0][1] * (self->height >> 8) & 0xFFFFFF00))
                  / self->depth3D;
        self->visible = abs(val) < 256;
    }
}

void UFO_Decoration_StaticUpdate(void) {}

void UFO_Decoration_Draw(void)
{
    RSDK_THIS(UFO_Decoration);
    if (self->depth3D >= 0x4000) {
        RSDK.Prepare3DScene(UFO_Decoration->sceneIndex);
        RSDK.MatrixScaleXYZ(&self->matrix1, self->scale.x, self->size, self->scale.x);
        RSDK.MatrixTranslateXYZ(&self->matrix1, self->position.x, self->height, self->position.y, 0);
        RSDK.MatrixRotateY(&self->matrix3, self->angle);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix3, &self->matrix1);
        RSDK.MatrixMultiply(&self->matrix2, &self->matrix2, &UFO_Camera->matWorld);
        if (UFO_Camera->isSS7)
            RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &UFO_Camera->matTemp);
        else
            RSDK.MatrixMultiply(&self->matrix3, &self->matrix3, &UFO_Camera->matView);

        if (self->type <= 5)
            RSDK.AddModelTo3DScene(UFO_Decoration->modelIndices[self->type], UFO_Decoration->sceneIndex, UFO_Decoration->drawType, &self->matrix2,
                                   &self->matrix3, 0xFFFFFF);
        else
            RSDK.AddMeshFrameTo3DScene(UFO_Decoration->modelIndices[self->type], UFO_Decoration->sceneIndex, &self->animator,
                                       UFO_Decoration->drawType, &self->matrix2, &self->matrix3, 0xFFFFFF);
        RSDK.Draw3DScene(UFO_Decoration->sceneIndex);
    }
}

void UFO_Decoration_Create(void *data)
{
    RSDK_THIS(UFO_Decoration);
    if (!SceneInfo->inEditor) {
        if (!self->size)
            self->size = 0x400;
        self->visible       = true;
        self->drawOrder     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x4000000;
        self->updateRange.y = 0x4000000;
        if (self->type == 6)
            self->height = 0x600000;
        RSDK.SetModelAnimation(UFO_Decoration->modelIndices[self->type], &self->animator, 96, 0, true, 0);
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
