#include "SonicMania.h"

ObjectDecoration *Decoration;

void Decoration_Update(void)
{
    RSDK_THIS(Decoration);
    RSDK.ProcessAnimation(&entity->animator);
    entity->rotation += entity->rotSpeed;
    entity->rotation &= 0x1FF;
}

void Decoration_LateUpdate(void) {}

void Decoration_StaticUpdate(void) {}

void Decoration_Draw(void)
{
    RSDK_THIS(Decoration);
    if (entity->tmzFlag) {
        RSDK.CopyPalette(0, 160, 1, 160, 16);
        RSDK.CopyPalette(2, 160, 0, 160, 16);
    }
    Decoration_DrawSprite();
    if (entity->tmzFlag)
        RSDK.CopyPalette(1, 160, 0, 160, 16);
}

void Decoration_Create(void *data)
{
    RSDK_THIS(Decoration);

    entity->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        entity->active  = ACTIVE_BOUNDS;
        entity->visible = true;
        if (entity->rotSpeed)
            entity->drawFX |= FX_ROTATE;
        if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
            entity->tmzFlag = true;
        if (entity->tmzFlag) {
            entity->inkEffect |= INK_ADD;
            entity->alpha = 0xE0;
        }

        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Decoration->aniFrames, entity->type, &entity->animator, true, 0);
        if (RSDK.GetFrameID(&entity->animator))
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;
    }
}

void Decoration_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("CPZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ1"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("FBZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("SSZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("MMZ1") || RSDK.CheckStageFolder("MMZ2"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("MSZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("MSZEnding"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Ending.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("HCZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("TMZ3"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("AIZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
}

void Decoration_DrawSprite(void)
{
    RSDK_THIS(Decoration);
    Vector2 drawPos, repeat;

    repeat.x  = entity->repeatTimes.x >> 16;
    repeat.y  = entity->repeatTimes.y >> 16;
    drawPos.x = entity->position.x - ((repeat.x * entity->repeatSpacing.x) >> 1);
    drawPos.y = entity->position.y - ((repeat.y * entity->repeatSpacing.y) >> 1);

    for (int32 y = 0; y < repeat.y + 1; ++y) {
        drawPos.x = entity->position.x - (repeat.x * entity->repeatSpacing.x >> 1);
        for (int32 x = 0; x < repeat.x + 1; ++x) {
            RSDK.DrawSprite(&entity->animator, &drawPos, false);
            drawPos.x += entity->repeatSpacing.x;
        }
        drawPos.y += entity->repeatSpacing.y;
    }
}

#if RETRO_INCLUDE_EDITOR
void Decoration_EditorDraw(void)
{
    RSDK_THIS(Decoration);
    RSDK.SetSpriteAnimation(Decoration->aniFrames, entity->type, &entity->animator, true, 0);
    if (entity->rotSpeed)
        entity->drawFX |= FX_ROTATE;
    else
        entity->drawFX &= ~FX_ROTATE;


    Decoration_DrawSprite();
}

void Decoration_EditorLoad(void) { Decoration_StageLoad(); }
#endif

void Decoration_Serialize(void)
{
    RSDK_EDITABLE_VAR(Decoration, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Decoration, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Decoration, VAR_ENUM, rotSpeed);
    RSDK_EDITABLE_VAR(Decoration, VAR_VECTOR2, repeatTimes);
    RSDK_EDITABLE_VAR(Decoration, VAR_VECTOR2, repeatSpacing);
}
