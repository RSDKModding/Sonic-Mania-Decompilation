// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FilmProjector Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectFilmProjector *FilmProjector;

void FilmProjector_Update(void)
{
    RSDK_THIS(FilmProjector);
    if (self->pathMovement)
        self->rotation -= 4;
    else
        self->rotation += 4;
    self->rotation &= 0x1FF;
    RSDK.ProcessAnimation(&self->animator5);
    RSDK.ProcessAnimation(&self->animator6);
}

void FilmProjector_LateUpdate(void) {}

void FilmProjector_StaticUpdate(void) {}

void FilmProjector_Draw(void)
{
    RSDK_THIS(FilmProjector);
    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh) {
        Vector2 drawPos;
        self->drawFX &= ~FX_FLIP;

        drawPos.y = self->position.y - 0x440000;
        if (self->direction == FLIP_NONE) {
            drawPos.x = self->position.x - 0x3C0000;
            RSDK.DrawSprite(&self->animator5, &drawPos, false);
            self->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&self->animator3, &drawPos, false);
            self->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&self->animator4, &drawPos, false);
            drawPos.x += 0x680000;
        }
        else {
            drawPos.x = self->position.x + 0x3C0000;
            RSDK.DrawSprite(&self->animator5, &drawPos, false);
            self->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&self->animator3, &drawPos, false);
            self->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&self->animator4, &drawPos, false);
            drawPos.x -= 0x680000;
        }
        RSDK.DrawSprite(&self->animator5, &drawPos, false);

        self->drawFX |= FX_ROTATE;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->drawFX &= ~FX_ROTATE;
        RSDK.DrawSprite(&self->animator4, &drawPos, false);

        SceneInfo->effectGizmo = true;
        self->drawFX |= FX_FLIP;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->inkEffect = INK_BLEND;
        RSDK.DrawSprite(&self->animator6, &self->posUnknown, false);

        self->inkEffect = INK_NONE;
    }
    else {
        RSDK.DrawSprite(&self->animator2, &self->posUnknown, false);
        RSDK.DrawSprite(&self->animator6, &self->posUnknown, false);
        RSDK.AddDrawListRef(Zone->drawOrderHigh, SceneInfo->entitySlot);
    }
}

void FilmProjector_Create(void *data)
{
    RSDK_THIS(FilmProjector);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 0, &self->animator2, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 1, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 1, &self->animator4, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 2, &self->animator5, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 3, &self->animator6, true, 1);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->posUnknown.x  = self->position.x;
        self->posUnknown.y  = self->position.y;
        if (self->direction == FLIP_NONE)
            self->posUnknown.x += 0xB80000;
        else
            self->posUnknown.x -= 0xB80000;
    }
}

void FilmProjector_StageLoad(void) { FilmProjector->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmProjector.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void FilmProjector_EditorDraw(void)
{
    RSDK_THIS(FilmProjector);

    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;

    self->posUnknown.x = self->position.x;
    self->posUnknown.y = self->position.y;
    if (self->direction == FLIP_NONE)
        self->posUnknown.x += 0xB80000;
    else
        self->posUnknown.x -= 0xB80000;

    RSDK.DrawSprite(&self->animator2, &self->posUnknown, false);
    RSDK.DrawSprite(&self->animator6, &self->posUnknown, false);

    Vector2 drawPos;
    self->drawFX &= ~FX_FLIP;

    drawPos.y = self->position.y - 0x440000;
    if (self->direction == FLIP_NONE) {
        drawPos.x = self->position.x - 0x3C0000;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);
        self->drawFX |= FX_ROTATE;

        RSDK.DrawSprite(&self->animator3, &drawPos, false);
        self->drawFX &= ~FX_ROTATE;

        RSDK.DrawSprite(&self->animator4, &drawPos, false);
        drawPos.x += 0x680000;
    }
    else {
        drawPos.x = self->position.x + 0x3C0000;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);
        self->drawFX |= FX_ROTATE;

        RSDK.DrawSprite(&self->animator3, &drawPos, false);
        self->drawFX &= ~FX_ROTATE;

        RSDK.DrawSprite(&self->animator4, &drawPos, false);
        drawPos.x -= 0x680000;
    }
    RSDK.DrawSprite(&self->animator5, &drawPos, false);

    self->drawFX |= FX_ROTATE;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    self->drawFX &= ~FX_ROTATE;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    SceneInfo->effectGizmo = true;
    self->drawFX |= FX_FLIP;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_BLEND;
    RSDK.DrawSprite(&self->animator6, &self->posUnknown, false);

    self->inkEffect = INK_NONE;
}

void FilmProjector_EditorLoad(void) { FilmProjector->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmProjector.bin", SCOPE_STAGE); }
#endif

void FilmProjector_Serialize(void)
{
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, pathMovement);
}
