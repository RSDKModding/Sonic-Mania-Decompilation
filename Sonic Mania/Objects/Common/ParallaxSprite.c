#include "SonicMania.h"

ObjectParallaxSprite *ParallaxSprite;

void ParallaxSprite_Update(void)
{
    RSDK_THIS(ParallaxSprite);
    StateMachine_Run(entity->state);
}

void ParallaxSprite_LateUpdate(void) {}

void ParallaxSprite_StaticUpdate(void) {}

void ParallaxSprite_Draw(void)
{
    RSDK_THIS(ParallaxSprite);
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    Vector2 drawPos;

    int32 scrollPosX = ((entity->scrollPos.x + entity->parallaxFactor.x * screen->position.x) & 0x7FFF0000) % entity->loopPoint.x;
    drawPos.x      = entity->position.x - scrollPosX;

    int32 scrollPosY = ((entity->scrollPos.y + entity->parallaxFactor.y * screen->position.y) & 0x7FFF0000) % entity->loopPoint.y;
    drawPos.y      = entity->position.y - scrollPosY;

    int32 loopX = -entity->loopPoint.x >> 2;
    if (drawPos.x < loopX) {
        drawPos.x += entity->loopPoint.x;
    }

    int32 loopY = -entity->loopPoint.y >> 2;
    if (drawPos.y < loopY) {
        drawPos.y += entity->loopPoint.y;
    }

    if (entity->attribute == PSPRITE_ATTR_2) {
        int32 y = (drawPos.y >> 16) - 32;
        int32 x = (drawPos.x >> 16) - 56;
        RSDK.DrawRect(x, y, 112, 64, entity->colour1, 255, 0, true);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 val = (RSDK.Sin256(i + Zone->timer) >> 3) + 48;
            if (val > 64)
                val = 64;
            RSDK.DrawRect(x, y - val + 64, 16, val, entity->colour2, 255, 0, true);
            x += 16;
        }
    }
    else if (entity->attribute == PSPRITE_ATTR_6) {
        RSDK.GetFrame(ParallaxSprite->aniFrames, entity->aniID, 0)->sprX =
            entity->sprX + ((entity->field_B4 + (Zone->timer << entity->field_B0)) & 0x7F);
    }
    RSDK.DrawSprite(&entity->animator, &drawPos, true);
}

void ParallaxSprite_Create(void *data)
{
    RSDK_THIS(ParallaxSprite);
    entity->active    = ACTIVE_NORMAL;
    entity->drawOrder = Zone->fgLayerLow + 1;
    if (data)
        entity->attribute = voidToInt(data);
    switch (entity->attribute) {
        case PSPRITE_ATTR_NONE:
        default:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->drawFX  = FX_ROTATE;
            entity->visible = !entity->hiddenAtStart;
            entity->state   = ParallaxSprite_Unknown1;
            break;
        case PSPRITE_ATTR_2:
#if RETRO_USE_PLUS
            if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
                entity->colour1 = 0x189098;
                entity->colour2 = 0x00D098;
            }
            else {
#endif
                entity->colour1 = 0x885820;
                entity->colour2 = 0xE89850;
#if RETRO_USE_PLUS
            }
#endif
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->drawFX  = FX_ROTATE;
            entity->visible = !entity->hiddenAtStart;
            entity->state   = ParallaxSprite_Unknown1;
            break;
        case PSPRITE_ATTR_SPAWNER:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->state = ParallaxSprite_State_Spawner;
            break;
        case PSPRITE_ATTR_FADEOUT:
            entity->inkEffect = INK_ALPHA;
            entity->visible   = true;
            entity->alpha     = 0x100;
            entity->state     = ParallaxSprite_State_FadeOut;
            break;
        case PSPRITE_ATTR_5:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->visible   = true;
            entity->inkEffect = INK_BLEND;
            entity->drawOrder = Zone->drawOrderHigh + 1;
            entity->state     = ParallaxSprite_Unknown1;
            break;
        case PSPRITE_ATTR_6:
            entity->parallaxFactor.x >>= 8;
            entity->parallaxFactor.y >>= 8;
            entity->sprX      = RSDK.GetFrame(ParallaxSprite->aniFrames, entity->aniID, 1)->sprX;
#if RETRO_USE_PLUS
            entity->field_B0  = RSDK.Random(0, 2, &Zone->randKey);
            entity->field_B4  = RSDK.Random(0, 128, &Zone->randKey);
#else
            entity->field_B0 = RSDK.Rand(0, 2);
            entity->field_B4 = RSDK.Rand(0, 128);
#endif
            entity->inkEffect = INK_MASKED;
            entity->visible   = true;
            entity->state     = NULL;
            break;
    }
    RSDK.SetSpriteAnimation(ParallaxSprite->aniFrames, entity->aniID, &entity->animator, true, 0);
}

void ParallaxSprite_StageLoad(void)
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

void ParallaxSprite_Unknown1(void)
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->animator);
    entity->rotation = (entity->rotation + 2) & 0x1FF;
    entity->scrollPos.x += entity->scrollSpeed.x;
    entity->scrollPos.y += entity->scrollSpeed.y;
}

void ParallaxSprite_State_Spawner(void)
{
    RSDK_THIS(ParallaxSprite);
    if (!(Zone->timer & 3)) {
        EntityParallaxSprite *pSprite = CREATE_ENTITY(ParallaxSprite, intToVoid(PSPRITE_ATTR_FADEOUT), entity->position.x, entity->position.y);
        pSprite->parallaxFactor.x = entity->parallaxFactor.x;
        pSprite->parallaxFactor.y = entity->parallaxFactor.y;
        pSprite->loopPoint.x      = entity->loopPoint.x;
        pSprite->loopPoint.y      = entity->loopPoint.y;
        pSprite->scrollSpeed.y    = RSDK.Rand(0x8000, 0x20000);
        pSprite->unknownPosB.x    = RSDK.Rand(-0x100, 0x100);
    }
}

void ParallaxSprite_State_FadeOut(void)
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->animator);
    entity->scrollSpeed.x += entity->unknownPosB.x;
    entity->scrollSpeed.y += entity->unknownPosB.y;
    entity->scrollPos.x += entity->scrollSpeed.x;
    entity->scrollPos.y += entity->scrollSpeed.y;
    entity->alpha -= 2;
    if (!entity->alpha)
        destroyEntity(entity);
}

void ParallaxSprite_Unknown4(void)
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->animator);
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

void ParallaxSprite_Unknown5(void)
{
    RSDK_THIS(ParallaxSprite);
    RSDK.ProcessAnimation(&entity->animator);
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

void ParallaxSprite_EditorDraw(void)
{
    RSDK_THIS(ParallaxSprite);
    RSDK.SetSpriteAnimation(ParallaxSprite->aniFrames, entity->aniID, &entity->animator, true, 0);
    Vector2 drawPos;

    drawPos.x        = entity->position.x;
    drawPos.y        = entity->position.y;

    if (entity->attribute == PSPRITE_ATTR_2) {
        int32 x = (drawPos.x >> 16) - 56;
        int32 y = (drawPos.y >> 16) - 32;
        RSDK.DrawRect(x, y, 112, 64, entity->colour1, 255, 0, true);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 val = (RSDK.Sin256(i + Zone->timer) >> 3) + 48;
            if (val > 64)
                val = 64;
            RSDK.DrawRect(x, y - val + 64, 16, val, entity->colour2, 255, 0, true);
            x += 16;
        }
    }
    else if (entity->attribute == PSPRITE_ATTR_6) {
        RSDK.GetFrame(ParallaxSprite->aniFrames, entity->aniID, 0)->sprX =
            entity->sprX + ((entity->field_B4 + (Zone->timer << entity->field_B0)) & 0x7F);
    }
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void ParallaxSprite_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("CPZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CPZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SPZ1"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/SPZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("FBZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("MSZ") || RSDK.CheckStageFolder("MSZCutscene"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("MSZ/MSZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("OOZ2"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("OOZ/OOZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZParallax.bin", SCOPE_STAGE);
}

void ParallaxSprite_Serialize(void)
{
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_UINT8, aniID);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_UINT8, attribute);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, parallaxFactor);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, scrollSpeed);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, loopPoint);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_BOOL, hiddenAtStart);
}
