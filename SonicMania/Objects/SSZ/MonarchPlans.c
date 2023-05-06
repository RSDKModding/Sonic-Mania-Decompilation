// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MonarchPlans Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectMonarchPlans *MonarchPlans;

void MonarchPlans_Update(void)
{
    RSDK_THIS(MonarchPlans);

    self->rotationY = (self->rotationY + 4) & 0x3FF;

    RSDK.MatrixScaleXYZ(&self->matWorldTemp, self->scale.x, -self->scale.x, self->scale.x);
    RSDK.MatrixTranslateXYZ(&self->matWorldTemp, self->position.x, self->position.y, 0, 0);
    RSDK.MatrixRotateXYZ(&self->matWorld, 0, self->rotationY, self->rotationZ);
    RSDK.MatrixMultiply(&self->matWorld, &self->matWorld, &self->matWorldTemp);

    RSDK.MatrixRotateX(&self->matNormalTemp, 0x1E0);
    RSDK.MatrixRotateXYZ(&self->matNormal, 0, self->rotationY, self->rotationZ);
    RSDK.MatrixMultiply(&self->matNormal, &self->matNormal, &self->matNormalTemp);
}

void MonarchPlans_LateUpdate(void) {}

void MonarchPlans_StaticUpdate(void) {}

void MonarchPlans_Draw(void)
{
    RSDK_THIS(MonarchPlans);

    RSDK.Prepare3DScene(MonarchPlans->sceneIndex);

    RSDK.AddModelTo3DScene(MonarchPlans->meshIndex, MonarchPlans->sceneIndex, S3D_WIREFRAME, &self->matWorld, &self->matNormal, 0x609090);

    RSDK.Draw3DScene(MonarchPlans->sceneIndex);
}

void MonarchPlans_Create(void *data)
{
    RSDK_THIS(MonarchPlans);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->inkEffect     = INK_BLEND;
        self->visible       = true;
        self->updateRange.x = 0x900000;
        self->updateRange.y = 0x900000;
        self->scale.x       = 0xA0;
        self->scale.y       = 0xA0;
        self->drawGroup     = Zone->objectDrawGroup[0];
    }
}

void MonarchPlans_StageLoad(void)
{
    // ???
    // what the...?
    // don't even have the slightest of clues as to what this was
    // why is it in Data/Sprites/Global/ ???
    MonarchPlans->aniFrames = RSDK.LoadSpriteAnimation("Global/MonarchPlans.bin", SCOPE_STAGE);

    MonarchPlans->meshIndex  = RSDK.LoadMesh("SSZ/MonarchPlans.bin", SCOPE_STAGE);
    MonarchPlans->sceneIndex = RSDK.Create3DScene("View:MonarchPlans", 0x2000, SCOPE_STAGE);

    // ???
    MonarchPlans->hitbox.left   = -24;
    MonarchPlans->hitbox.top    = -24;
    MonarchPlans->hitbox.right  = 24;
    MonarchPlans->hitbox.bottom = 24;

    RSDK.SetDiffuseColor(MonarchPlans->sceneIndex, 0xA0, 0xA0, 0xA0);
    RSDK.SetDiffuseIntensity(MonarchPlans->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(MonarchPlans->sceneIndex, 14, 14, 14);
}

#if GAME_INCLUDE_EDITOR
void MonarchPlans_EditorDraw(void) {}

void MonarchPlans_EditorLoad(void) {}
#endif

void MonarchPlans_Serialize(void) {}
#endif
