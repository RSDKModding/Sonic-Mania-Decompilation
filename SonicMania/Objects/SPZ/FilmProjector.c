// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FilmProjector Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFilmProjector *FilmProjector;

void FilmProjector_Update(void)
{
    RSDK_THIS(FilmProjector);

    self->rotation += self->pathMovement != FLIP_NONE ? -4 : 4;
    self->rotation &= 0x1FF;

    RSDK.ProcessAnimation(&self->celluoidAnimator);
    RSDK.ProcessAnimation(&self->eggmanAnimator);
}

void FilmProjector_LateUpdate(void) {}

void FilmProjector_StaticUpdate(void) {}

void FilmProjector_Draw(void)
{
    RSDK_THIS(FilmProjector);

    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[1]) {
        Vector2 drawPos;
        self->drawFX &= ~FX_FLIP;

        drawPos.y = self->position.y - 0x440000;
        if (self->direction == FLIP_NONE) {
            drawPos.x = self->position.x - 0x3C0000;
            RSDK.DrawSprite(&self->celluoidAnimator, &drawPos, false);
            self->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&self->filmReelAnimator, &drawPos, false);
            self->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&self->reelEdgeAnimator, &drawPos, false);
            drawPos.x += 0x680000;
        }
        else {
            drawPos.x = self->position.x + 0x3C0000;
            RSDK.DrawSprite(&self->celluoidAnimator, &drawPos, false);
            self->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&self->filmReelAnimator, &drawPos, false);
            self->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&self->reelEdgeAnimator, &drawPos, false);
            drawPos.x -= 0x680000;
        }
        RSDK.DrawSprite(&self->celluoidAnimator, &drawPos, false);

        self->drawFX |= FX_ROTATE;
        RSDK.DrawSprite(&self->filmReelAnimator, &drawPos, false);

        self->drawFX &= ~FX_ROTATE;
        RSDK.DrawSprite(&self->reelEdgeAnimator, &drawPos, false);

        // ???
        // This is the ONLY reference to this in the entire game
        // (Not counting the editor funcs since they're for RE2 and custom-made)
        SceneInfo->effectGizmo = true;

        self->drawFX |= FX_FLIP;
        RSDK.DrawSprite(&self->projectorAnimator, NULL, false);

        self->inkEffect = INK_BLEND;
        RSDK.DrawSprite(&self->eggmanAnimator, &self->filmPos, false);

        self->inkEffect = INK_NONE;
    }
    else {
        RSDK.DrawSprite(&self->screenAnimator, &self->filmPos, false);
        RSDK.DrawSprite(&self->eggmanAnimator, &self->filmPos, false);
        RSDK.AddDrawListRef(Zone->objectDrawGroup[1], SceneInfo->entitySlot);
    }
}

void FilmProjector_Create(void *data)
{
    RSDK_THIS(FilmProjector);

    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 0, &self->projectorAnimator, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 0, &self->screenAnimator, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 1, &self->filmReelAnimator, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 1, &self->reelEdgeAnimator, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 2, &self->celluoidAnimator, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 3, &self->eggmanAnimator, true, 1);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];

        self->filmPos = self->position;
        if (self->direction == FLIP_NONE)
            self->filmPos.x += 0xB80000;
        else
            self->filmPos.x -= 0xB80000;
    }
}

void FilmProjector_StageLoad(void) { FilmProjector->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmProjector.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void FilmProjector_EditorDraw(void)
{
    RSDK_THIS(FilmProjector);

    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;

    self->filmPos.x = self->position.x;
    self->filmPos.y = self->position.y;
    if (self->direction == FLIP_NONE)
        self->filmPos.x += 0xB80000;
    else
        self->filmPos.x -= 0xB80000;

    RSDK.DrawSprite(&self->screenAnimator, &self->filmPos, false);
    RSDK.DrawSprite(&self->eggmanAnimator, &self->filmPos, false);

    Vector2 drawPos;
    self->drawFX &= ~FX_FLIP;

    drawPos.y = self->position.y - 0x440000;
    if (self->direction == FLIP_NONE) {
        drawPos.x = self->position.x - 0x3C0000;
        RSDK.DrawSprite(&self->celluoidAnimator, &drawPos, false);
        self->drawFX |= FX_ROTATE;

        RSDK.DrawSprite(&self->filmReelAnimator, &drawPos, false);
        self->drawFX &= ~FX_ROTATE;

        RSDK.DrawSprite(&self->reelEdgeAnimator, &drawPos, false);
        drawPos.x += 0x680000;
    }
    else {
        drawPos.x = self->position.x + 0x3C0000;
        RSDK.DrawSprite(&self->celluoidAnimator, &drawPos, false);
        self->drawFX |= FX_ROTATE;

        RSDK.DrawSprite(&self->filmReelAnimator, &drawPos, false);
        self->drawFX &= ~FX_ROTATE;

        RSDK.DrawSprite(&self->reelEdgeAnimator, &drawPos, false);
        drawPos.x -= 0x680000;
    }
    RSDK.DrawSprite(&self->celluoidAnimator, &drawPos, false);

    self->drawFX |= FX_ROTATE;
    RSDK.DrawSprite(&self->filmReelAnimator, &drawPos, false);

    self->drawFX &= ~FX_ROTATE;
    RSDK.DrawSprite(&self->reelEdgeAnimator, &drawPos, false);

    self->drawFX |= FX_FLIP;
    RSDK.DrawSprite(&self->projectorAnimator, NULL, false);

    self->inkEffect = INK_BLEND;
    RSDK.DrawSprite(&self->eggmanAnimator, &self->filmPos, false);

    self->inkEffect = INK_NONE;
}

void FilmProjector_EditorLoad(void)
{
    FilmProjector->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmProjector.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FilmProjector, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);

    // Confusing name, but controls what dir the projector wheels spin
    RSDK_ACTIVE_VAR(FilmProjector, pathMovement);
    RSDK_ENUM_VAR("Clockwise", FLIP_NONE);
    RSDK_ENUM_VAR("Counter-Clockwise", FLIP_X);
}
#endif

void FilmProjector_Serialize(void)
{
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, pathMovement);
}
