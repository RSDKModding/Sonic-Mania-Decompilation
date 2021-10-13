#include "SonicMania.h"

ObjectDecoration *Decoration;

void Decoration_Update(void)
{
    EntityDecoration *entity = (EntityDecoration *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->animator);
    entity->rotation += entity->rotSpeed;
    entity->rotation &= 0x1FF;
}

void Decoration_LateUpdate(void)
{

}

void Decoration_StaticUpdate(void)
{

}

void Decoration_Draw(void)
{
    EntityDecoration *entity = (EntityDecoration *)RSDK_sceneInfo->entity;
    if (entity->tmzFlag) {
        RSDK.CopyPalette(0, 160, 1, 160, 16);
        RSDK.CopyPalette(2, 160, 0, 160, 16);
    }
    Decoration_DrawSprite();
    if (entity->tmzFlag)
        RSDK.CopyPalette(1, 160, 0, 160, 16);
}

void Decoration_Create(void* data)
{
    EntityDecoration *entity        = (EntityDecoration *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
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
        RSDK.SetSpriteAnimation(Decoration->spriteIndex, entity->type, &entity->animator, true, 0);
        if (RSDK.GetFrameID(&entity->animator))
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;
    }
}

void Decoration_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("CPZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ1")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("SPZ1/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("FBZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("SSZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MMZ1") || RSDK.CheckStageFolder("MMZ2")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("MMZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MSZ")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MSZEnding")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/Ending.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("HCZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("TMZ1/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("Phantom/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("AIZ")) {
        Decoration->spriteIndex = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
    }
}

void Decoration_DrawSprite(void) {
    EntityDecoration *entity = (EntityDecoration *)RSDK_sceneInfo->entity;
    Vector2 drawPos, repeat;

    repeat.x  = entity->repeatTimes.x >> 16;
    repeat.y  = entity->repeatTimes.y >> 16;
    drawPos.x = entity->position.x - ((repeat.x * entity->repeatSpacing.x) >> 1);
    drawPos.y = entity->position.y - ((repeat.y * entity->repeatSpacing.y) >> 1);

    for (int32 y = 0; y < repeat.y + 1; ++y) {
        drawPos.x = entity->position.x - (repeat.x * entity->repeatSpacing.x >> 1);
        for (int32 x = 0; x < repeat.x + 1; ++x) {
            RSDK.DrawSprite(&entity->animator, &drawPos, 0);
            drawPos.x += entity->repeatSpacing.x;
        }
        drawPos.y += entity->repeatSpacing.y;
    }
}

void Decoration_EditorDraw(void)
{

}

void Decoration_EditorLoad(void)
{

}

void Decoration_Serialize(void)
{
    RSDK_EDITABLE_VAR(Decoration, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Decoration, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Decoration, VAR_ENUM, rotSpeed);
    RSDK_EDITABLE_VAR(Decoration, VAR_VECTOR2, repeatTimes);
    RSDK_EDITABLE_VAR(Decoration, VAR_VECTOR2, repeatSpacing);
}

