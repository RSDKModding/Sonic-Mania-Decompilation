#include "../SonicMania.hpp"

ObjectDecoration *Decoration;

void Decoration_Update()
{
    EntityDecoration *entity = (EntityDecoration *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    entity->rotation += entity->rotSpeed;
    entity->rotation &= 0x1FF;
}

void Decoration_LateUpdate()
{

}

void Decoration_StaticUpdate()
{

}

void Decoration_Draw()
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
        RSDK.SetSpriteAnimation(Decoration->spriteIndex, entity->type, &entity->data, true, 0);
        if (RSDK.GetFrameID(&entity->data))
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;
    }
}

void Decoration_StageLoad()
{
    if (RSDK.CheckStageFolder("GHZ")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("GHZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("CPZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ1")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("SPZ1/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("FBZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("SSZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MMZ1") || RSDK.CheckStageFolder("MMZ2")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("MMZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MSZ")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("MSZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MSZEnding")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("MSZ/Ending.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("HCZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("TMZ1/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("TMZ3")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("Phantom/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("AIZ")) {
        Decoration->spriteIndex = RSDK.LoadAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
    }
}

void Decoration_DrawSprite() {
    EntityDecoration *entity = (EntityDecoration *)RSDK_sceneInfo->entity;
    Vector2 drawPos, repeat;

    repeat.x  = entity->repeatTimes.x >> 16;
    repeat.y  = entity->repeatTimes.y >> 16;
    drawPos.x = entity->position.x - ((repeat.x * entity->repeatSpacing.x) >> 1);
    drawPos.y = entity->position.y - ((repeat.y * entity->repeatSpacing.y) >> 1);

    for (int y = 0; y < repeat.y + 1; ++y) {
        drawPos.x = entity->position.x - (repeat.x * entity->repeatSpacing.x >> 1);
        for (int x = 0; x < repeat.x + 1; ++x) {
            RSDK.DrawSprite(&entity->data, &drawPos, 0);
            drawPos.x += entity->repeatSpacing.x;
        }
        drawPos.y += entity->repeatSpacing.y;
    }
}

void Decoration_EditorDraw()
{

}

void Decoration_EditorLoad()
{

}

void Decoration_Serialize()
{

}

