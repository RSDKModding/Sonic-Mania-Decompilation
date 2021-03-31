#include "../SonicMania.h"

ObjectParallaxSprite *ParallaxSprite;

void ParallaxSprite_Update()
{
    RSDK_THIS(ParallaxSprite);
    CallFunction(entity->state);
}

void ParallaxSprite_LateUpdate() {}

void ParallaxSprite_StaticUpdate() {}

void ParallaxSprite_Draw()
{
    RSDK_THIS(ParallaxSprite);
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    Vector2 drawPos;

    int scrollPosX = ((entity->scrollPos.x + entity->parallaxFactor.x * screen->position.x) & 0x7FFF0000) % entity->loopPoint.x;
    drawPos.x      = entity->position.x - scrollPosX;

    int scrollPosY = ((entity->scrollPos.y + entity->parallaxFactor.y * screen->position.y) & 0x7FFF0000) % entity->loopPoint.y;
    drawPos.y      = entity->position.y - scrollPosY;

    int loopX = -entity->loopPoint.x >> 2;
    if (drawPos.x < loopX) {
        drawPos.x += entity->loopPoint.x;
    }

    int loopY = -entity->loopPoint.y >> 2;
    if (drawPos.y < loopY) {
        drawPos.y += entity->loopPoint.y;
    }

    if (entity->attribute == 2) {
        int y = (drawPos.y >> 16) - 32;
        int x = (drawPos.x >> 16) - 56;
        RSDK.DrawRect(x, y, 112, 64, entity->unknownPosA.x, 255, 0, true);

        for (int i = 0; i < 0xE0; i += 0x20) {
            int val = (RSDK.Sin256(i + Zone->timer) >> 3) + 48;
            if (val > 64)
                val = 64;
            RSDK.DrawRect(x, y - val + 64, 16, val, entity->unknownPosA.y, 255, 0, true);
            x += 16;
        }
    }
    else if (entity->attribute == 6) {
        RSDK.GetFrame(ParallaxSprite->aniFrames, entity->aniID, 0)->sprX =
            entity->sprX + (((ushort)(entity->field_B4) + ((ushort)(Zone->timer) << entity->field_B0)) & 0x7F);
    }
    RSDK.DrawSprite(&entity->data, &drawPos, true);
}

void ParallaxSprite_Create(void *data)
{
    RSDK_THIS(ParallaxSprite);
    entity->active    = ACTIVE_NORMAL;
    entity->drawOrder = Zone->fgLayerLow + 1;
    if (data)
        entity->attribute = (byte)voidToInt(data);
    switch (entity->attribute) {
        case 2:
            if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
                entity->unknownPosA.x = 0x189098;
                entity->unknownPosA.y = 0xD098;
            }
            else {
                entity->unknownPosA.x = 0x885820;
                entity->unknownPosA.y = 0xE89850;
            }
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->drawFX  = FX_ROTATE;
            entity->visible = !entity->hiddenAtStart;
            entity->state   = ParallaxSprite_Unknown1;
            break;
        case 3:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->state = ParallaxSprite_CreateCopy;
            break;
        case 4:
            entity->inkEffect = INK_ALPHA;
            entity->visible   = true;
            entity->alpha     = 0x100;
            entity->state     = ParallaxSprite_Unknown3;
            break;
        case 5:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->visible   = true;
            entity->inkEffect = INK_BLEND;
            entity->drawOrder = Zone->drawOrderHigh + 1;
            entity->state     = ParallaxSprite_Unknown1;
            break;
        case 6:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->sprX      = RSDK.GetFrame(ParallaxSprite->aniFrames, entity->aniID, 1)->sprX;
            entity->field_B0  = RSDK.Random(0, 2, &Zone->timeStart);
            entity->field_B4  = RSDK.Random(0, 128, &Zone->timeStart);
            entity->inkEffect = INK_MASKED;
            entity->visible   = true;
            entity->state     = NULL;
            break;
        default:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->drawFX  = FX_ROTATE;
            entity->visible = !entity->hiddenAtStart;
            entity->state   = ParallaxSprite_Unknown1;
            break;
    }
    RSDK.SetSpriteAnimation(ParallaxSprite->aniFrames, entity->aniID, &entity->data, true, 0);
}

void ParallaxSprite_StageLoad()
{
    if (RSDK.CheckStageFolder("AIZ")) {
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("CPZ")) {
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CPZParallax.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("SPZ1")) {
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/SPZParallax.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("FBZ")) {
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZParallax.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("MSZ") || RSDK.CheckStageFolder("MSZCutscene")) {
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("MSZ/MSZParallax.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("OOZ2")) {
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("OOZ/OOZParallax.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3")) {
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZParallax.bin", SCOPE_STAGE);
    }
}

void ParallaxSprite_Unknown1()
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->data);
    entity->rotation = (entity->rotation + 2) & 0x1FF;
    entity->scrollPos.x += entity->scrollSpeed.x;
    entity->scrollPos.y += entity->scrollSpeed.y;
}

void ParallaxSprite_CreateCopy()
{
    RSDK_THIS(ParallaxSprite);
    if (!(Zone->timer & 3)) {
        EntityParallaxSprite *pSprite =
            (EntityParallaxSprite *)RSDK.CreateEntity(ParallaxSprite->objectID, (void *)4, entity->position.x, entity->position.y);
        pSprite->parallaxFactor.x = entity->parallaxFactor.x;
        pSprite->parallaxFactor.y = entity->parallaxFactor.y;
        pSprite->loopPoint.x      = entity->loopPoint.x;
        pSprite->loopPoint.y      = entity->loopPoint.y;
        pSprite->scrollSpeed.y    = RSDK.Rand(0x8000, 0x20000);
        pSprite->unknownPosB.x    = RSDK.Rand(-0x100, 0x100);
    }
}

void ParallaxSprite_Unknown3()
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->data);
    entity->scrollSpeed.x += entity->unknownPosB.x;
    entity->scrollSpeed.y += entity->unknownPosB.y;
    entity->scrollPos.x += entity->scrollSpeed.x;
    entity->scrollPos.y += entity->scrollSpeed.y;
    entity->alpha -= 2;
    if (!entity->alpha)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void ParallaxSprite_Unknown4()
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->data);
    entity->scrollPos.x += entity->scrollSpeed.x;
    entity->scrollPos.y += entity->scrollSpeed.y;
    if (entity->alpha >= 128) {
        entity->inkEffect = INK_BLEND;
        entity->visible   = true;
        entity->state     = ParallaxSprite_Unknown1;
    }
    else {
        entity->inkEffect = INK_ALPHA;
        entity->alpha += 4;
    }
}

void ParallaxSprite_Unknown5()
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->data);
    entity->scrollPos.x += entity->scrollSpeed.x;
    entity->scrollPos.y += entity->scrollSpeed.y;
    if (entity->alpha <= 0) {
        entity->visible = false;
        entity->state   = ParallaxSprite_Unknown1;
    }
    else {
        entity->inkEffect = INK_ALPHA;
        entity->alpha -= 4;
    }
}

void ParallaxSprite_EditorDraw() {}

void ParallaxSprite_EditorLoad() {}

void ParallaxSprite_Serialize()
{
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_UINT8, aniID);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_UINT8, attribute);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, parallaxFactor);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, scrollSpeed);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, loopPoint);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_BOOL, hiddenAtStart);
}
