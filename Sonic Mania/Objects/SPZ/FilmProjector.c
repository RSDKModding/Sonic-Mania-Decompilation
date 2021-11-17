#include "SonicMania.h"

ObjectFilmProjector *FilmProjector;

void FilmProjector_Update(void)
{
    RSDK_THIS(FilmProjector);
    if (entity->pathMovement)
        entity->rotation -= 4;
    else
        entity->rotation += 4;
    entity->rotation &= 0x1FF;
    RSDK.ProcessAnimation(&entity->animator5);
    RSDK.ProcessAnimation(&entity->animator6);
}

void FilmProjector_LateUpdate(void) {}

void FilmProjector_StaticUpdate(void) {}

void FilmProjector_Draw(void)
{
    RSDK_THIS(FilmProjector);
    if (SceneInfo->currentDrawGroup == Zone->drawOrderHigh) {
        Vector2 drawPos;
        entity->drawFX &= ~FX_FLIP;

        drawPos.y = entity->position.y - 0x440000;
        if (entity->direction == FLIP_NONE) {
            drawPos.x = entity->position.x - 0x3C0000;
            RSDK.DrawSprite(&entity->animator5, &drawPos, false);
            entity->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
            entity->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&entity->animator4, &drawPos, false);
            drawPos.x += 0x680000;
        }
        else {
            drawPos.x = entity->position.x + 0x3C0000;
            RSDK.DrawSprite(&entity->animator5, &drawPos, false);
            entity->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
            entity->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&entity->animator4, &drawPos, false);
            drawPos.x -= 0x680000;
        }
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);

        entity->drawFX |= FX_ROTATE;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->drawFX &= ~FX_ROTATE;
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);

        SceneInfo->effectGizmo = true;
        entity->drawFX |= FX_FLIP;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->inkEffect = INK_BLEND;
        RSDK.DrawSprite(&entity->animator6, &entity->posUnknown, false);

        entity->inkEffect = INK_NONE;
    }
    else {
        RSDK.DrawSprite(&entity->animator2, &entity->posUnknown, false);
        RSDK.DrawSprite(&entity->animator6, &entity->posUnknown, false);
        RSDK.AddDrawListRef(Zone->drawOrderHigh, SceneInfo->entitySlot);
    }
}

void FilmProjector_Create(void *data)
{
    RSDK_THIS(FilmProjector);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 0, &entity->animator2, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 1, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 1, &entity->animator4, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 2, &entity->animator5, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->aniFrames, 3, &entity->animator6, true, 1);
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->posUnknown.x  = entity->position.x;
        entity->posUnknown.y  = entity->position.y;
        if (entity->direction == FLIP_NONE)
            entity->posUnknown.x += 0xB80000;
        else
            entity->posUnknown.x -= 0xB80000;
    }
}

void FilmProjector_StageLoad(void) { FilmProjector->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmProjector.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void FilmProjector_EditorDraw(void)
{
    RSDK_THIS(FilmProjector);

    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;

    entity->posUnknown.x = entity->position.x;
    entity->posUnknown.y = entity->position.y;
    if (entity->direction == FLIP_NONE)
        entity->posUnknown.x += 0xB80000;
    else
        entity->posUnknown.x -= 0xB80000;

    RSDK.DrawSprite(&entity->animator2, &entity->posUnknown, false);
    RSDK.DrawSprite(&entity->animator6, &entity->posUnknown, false);

    Vector2 drawPos;
    entity->drawFX &= ~FX_FLIP;

    drawPos.y = entity->position.y - 0x440000;
    if (entity->direction == FLIP_NONE) {
        drawPos.x = entity->position.x - 0x3C0000;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);
        entity->drawFX |= FX_ROTATE;

        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        entity->drawFX &= ~FX_ROTATE;

        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
        drawPos.x += 0x680000;
    }
    else {
        drawPos.x = entity->position.x + 0x3C0000;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);
        entity->drawFX |= FX_ROTATE;

        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        entity->drawFX &= ~FX_ROTATE;

        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
        drawPos.x -= 0x680000;
    }
    RSDK.DrawSprite(&entity->animator5, &drawPos, false);

    entity->drawFX |= FX_ROTATE;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    entity->drawFX &= ~FX_ROTATE;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    SceneInfo->effectGizmo = true;
    entity->drawFX |= FX_FLIP;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_BLEND;
    RSDK.DrawSprite(&entity->animator6, &entity->posUnknown, false);

    entity->inkEffect = INK_NONE;
}

void FilmProjector_EditorLoad(void) { FilmProjector->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/FilmProjector.bin", SCOPE_STAGE); }
#endif

void FilmProjector_Serialize(void)
{
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, pathMovement);
}
