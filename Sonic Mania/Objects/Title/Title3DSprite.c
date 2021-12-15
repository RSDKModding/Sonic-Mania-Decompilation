// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Title3DSprite Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTitle3DSprite *Title3DSprite;

void Title3DSprite_Update(void)
{
    RSDK_THIS(Title3DSprite);
    int32 sinY      = (self->position.y >> 8) * RSDK.Sin1024(TitleBG->angle);
    int32 cosY      = (self->position.y >> 8) * RSDK.Cos1024(TitleBG->angle);
    self->relX    = (-sinY - (self->position.x >> 8) * RSDK.Cos1024(TitleBG->angle)) >> 10;
    self->relY    = (cosY - (self->position.x >> 8) * RSDK.Sin1024(TitleBG->angle)) >> 10;
    self->depth3D = self->relY;
}

void Title3DSprite_LateUpdate(void) {}

void Title3DSprite_StaticUpdate(void) {}

void Title3DSprite_Draw(void)
{
    RSDK_THIS(Title3DSprite);
    int32 depth = self->depth3D + Title3DSprite->baseDepth;
    if (depth && depth >= 256) {
        int32 scale = 0x18000 * Title3DSprite->islandSize / depth;
        if (scale > 0x200)
            scale = 0x200;
        self->scale.x = scale;
        self->scale.y = scale;

        Vector2 drawPos;
        drawPos.x = (Title3DSprite->islandSize * self->relX / depth + ScreenInfo->centerX) << 16;
        drawPos.y = (Title3DSprite->islandSize * Title3DSprite->height / depth + 152) << 16;
        RSDK.DrawSprite(&self->animator, &drawPos, true);
    }
}

void Title3DSprite_Create(void *data)
{
    RSDK_THIS(Title3DSprite);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Title3DSprite->aniFrames, 5, &self->animator, true, self->frame);
        self->position.x -= 0x2000000;
        self->position.y -= 0x2000000;
        self->active    = ACTIVE_NORMAL;
        self->visible   = false;
        self->drawOrder = 2;
        self->drawFX    = FX_NONE;
    }
}

void Title3DSprite_StageLoad(void)
{
    Title3DSprite->aniFrames  = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE);
    Title3DSprite->islandSize = 0x90;
    Title3DSprite->height     = 0x2800;
    Title3DSprite->baseDepth  = 0xA000;
}

#if RETRO_INCLUDE_EDITOR
void Title3DSprite_EditorDraw(void)
{
    RSDK_THIS(Title3DSprite);
    RSDK.SetSpriteAnimation(Title3DSprite->aniFrames, 5, &self->animator, true, self->frame);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Title3DSprite_EditorLoad(void) { Title3DSprite->aniFrames = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE); }
#endif

void Title3DSprite_Serialize(void) { RSDK_EDITABLE_VAR(Title3DSprite, VAR_ENUM, frame); }
