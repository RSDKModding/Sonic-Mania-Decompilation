#include "../SonicMania.h"

ObjectFilmProjector *FilmProjector;

void FilmProjector_Update(void)
{
    RSDK_THIS(FilmProjector);
    if (entity->pathMovement)
        entity->rotation -= 4;
    else
        entity->rotation += 4;
    entity->rotation &= 0x1FF;
    RSDK.ProcessAnimation(&entity->data5);
    RSDK.ProcessAnimation(&entity->data6);
}

void FilmProjector_LateUpdate(void)
{

}

void FilmProjector_StaticUpdate(void)
{

}

void FilmProjector_Draw(void)
{
    RSDK_THIS(FilmProjector);
    if (RSDK_sceneInfo->currentDrawGroup == Zone->drawOrderHigh) {
        Vector2 drawPos;
        entity->drawFX &= ~FX_FLIP;

        drawPos.y = entity->position.y - 0x440000;
        if (entity->direction == FLIP_NONE) {
            drawPos.x = entity->position.x - 0x3C0000;
            RSDK.DrawSprite(&entity->data5, &drawPos, false);
            entity->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&entity->data3, &drawPos, false);
            entity->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&entity->data4, &drawPos, false);
            drawPos.x += 0x680000;
        }
        else {
            drawPos.x = entity->position.x + 0x3C0000;
            RSDK.DrawSprite(&entity->data5, &drawPos, false);
            entity->drawFX |= FX_ROTATE;

            RSDK.DrawSprite(&entity->data3, &drawPos, false);
            entity->drawFX &= ~FX_ROTATE;

            RSDK.DrawSprite(&entity->data4, &drawPos, false);
            drawPos.x -= 0x680000;
        }
        RSDK.DrawSprite(&entity->data5, &drawPos, false);

        entity->drawFX |= FX_ROTATE;
        RSDK.DrawSprite(&entity->data3, &drawPos, false);

        entity->drawFX &= ~FX_ROTATE;
        RSDK.DrawSprite(&entity->data4, &drawPos, false);

        RSDK_sceneInfo->effectGizmo = true;
        entity->drawFX |= FX_FLIP;
        RSDK.DrawSprite(&entity->data1, NULL, false);

        entity->inkEffect = INK_BLEND;
        RSDK.DrawSprite(&entity->data6, &entity->posUnknown, false);

        entity->inkEffect = INK_NONE;
    }
    else {
        RSDK.DrawSprite(&entity->data2, &entity->posUnknown, false);
        RSDK.DrawSprite(&entity->data6, &entity->posUnknown, false);
        RSDK.AddDrawListRef(Zone->drawOrderHigh, RSDK_sceneInfo->entitySlot);
    }
}

void FilmProjector_Create(void* data)
{
    RSDK_THIS(FilmProjector);
    RSDK.SetSpriteAnimation(FilmProjector->spriteIndex, 0, &entity->data1, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->spriteIndex, 0, &entity->data2, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->spriteIndex, 1, &entity->data3, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->spriteIndex, 1, &entity->data4, true, 1);
    RSDK.SetSpriteAnimation(FilmProjector->spriteIndex, 2, &entity->data5, true, 0);
    RSDK.SetSpriteAnimation(FilmProjector->spriteIndex, 3, &entity->data6, true, 1);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->posUnknown.x       = entity->position.x;
        entity->posUnknown.y       = entity->position.y;
        if (entity->direction == FLIP_NONE)
            entity->posUnknown.x += 0xB80000;
        else
            entity->posUnknown.x -= 0xB80000;
    }
}

void FilmProjector_StageLoad(void) { FilmProjector->spriteIndex = RSDK.LoadSpriteAnimation("SPZ1/FilmProjector.bin", SCOPE_STAGE); }

void FilmProjector_EditorDraw(void)
{

}

void FilmProjector_EditorLoad(void)
{

}

void FilmProjector_Serialize(void)
{
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(FilmProjector, VAR_UINT8, pathMovement);
}

