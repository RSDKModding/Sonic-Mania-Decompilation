// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Dust Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_Dust *UFO_Dust;

void UFO_Dust_Update(void) {}

void UFO_Dust_LateUpdate(void)
{
    RSDK_THIS(UFO_Dust);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        destroyEntity(self);
    }
    else {
        int32 x = self->position.x >> 8;
        int32 y = self->height >> 8;
        int32 z = self->position.y >> 8;

        Matrix *mat = &UFO_Camera->matWorld;

        self->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
        self->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
        self->zdepth = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
    }
}

void UFO_Dust_StaticUpdate(void) {}

void UFO_Dust_Draw(void)
{
    RSDK_THIS(UFO_Dust);

    if (self->zdepth >= 0x400) {
        Vector2 drawPos;
        drawPos.x = (ScreenInfo->center.x + (self->worldX << 8) / self->zdepth) << 16;
        drawPos.y = (ScreenInfo->center.y - (self->worldY << 8) / self->zdepth) << 16;

        self->scale.x = 0x1000000 / self->zdepth;
        self->scale.y = 0x1000000 / self->zdepth;
        RSDK.DrawSprite(&self->animator, &drawPos, true);
    }
}

void UFO_Dust_Create(void *data)
{
    RSDK_THIS(UFO_Dust);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_SCALE | FX_FLIP;
        self->drawGroup     = 4;
        self->active        = ACTIVE_RBOUNDS;
        self->updateRange.x = 0x400;
        self->updateRange.y = 0x400;

        self->position.x += RSDK.Rand(-0x80000, 0x80000);
        self->position.y += RSDK.Rand(-0x80000, 0x80000);
        self->height = RSDK.Rand(0x40000, 0x100000);

        RSDK.SetSpriteAnimation(UFO_Dust->aniFrames, 0, &self->animator, true, 0);
    }
}

void UFO_Dust_StageLoad(void) { UFO_Dust->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/Dust.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void UFO_Dust_EditorDraw(void) {}

void UFO_Dust_EditorLoad(void) {}
#endif

void UFO_Dust_Serialize(void) { RSDK_EDITABLE_VAR(UFO_Dust, VAR_ENUM, height); }
