// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ParallaxSprite Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectParallaxSprite *ParallaxSprite;

void ParallaxSprite_Update(void)
{
    RSDK_THIS(ParallaxSprite);

    StateMachine_Run(self->state);
}

void ParallaxSprite_LateUpdate(void) {}

void ParallaxSprite_StaticUpdate(void) {}

void ParallaxSprite_Draw(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    Vector2 drawPos;

    int32 scrollPosX = ((self->scrollPos.x + self->parallaxFactor.x * screen->position.x) & 0x7FFF0000) % self->loopPoint.x;
    drawPos.x        = self->position.x - scrollPosX;

    int32 scrollPosY = ((self->scrollPos.y + self->parallaxFactor.y * screen->position.y) & 0x7FFF0000) % self->loopPoint.y;
    drawPos.y        = self->position.y - scrollPosY;

    int32 loopX = -self->loopPoint.x >> 2;
    if (drawPos.x < loopX)
        drawPos.x += self->loopPoint.x;

    int32 loopY = -self->loopPoint.y >> 2;
    if (drawPos.y < loopY)
        drawPos.y += self->loopPoint.y;

    if (self->attribute == PARALLAXSPRITE_ATTR_COLORS) {
        int32 y = (drawPos.y >> 16) - 32;
        int32 x = (drawPos.x >> 16) - 56;
        RSDK.DrawRect(x, y, 112, 64, self->color1, 255, INK_NONE, true);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 height = minVal((RSDK.Sin256(i + Zone->timer) >> 3) + 48, 64);
            RSDK.DrawRect(x, y - height + 64, 16, height, self->color2, 255, INK_NONE, true);
            x += 16;
        }
    }
    else if (self->attribute == PARALLAXSPRITE_ATTR_SHIFT) {
        RSDK.GetFrame(ParallaxSprite->aniFrames, self->aniID, 0)->sprX = self->sprX + ((self->xSpeed + (Zone->timer << self->timerSpeed)) & 0x7F);
    }

    RSDK.DrawSprite(&self->animator, &drawPos, true);
}

void ParallaxSprite_Create(void *data)
{
    RSDK_THIS(ParallaxSprite);

    self->active    = ACTIVE_NORMAL;
    self->drawOrder = Zone->fgLayerLow + 1;

    if (data)
        self->attribute = voidToInt(data);

    switch (self->attribute) {
        case PARALLAXSPRITE_ATTR_NONE:
        case PARALLAXSPRITE_ATTR_UNUSED:
        default:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->drawFX  = FX_ROTATE;
            self->visible = !self->hiddenAtStart;
            self->state   = ParallaxSprite_State_RotateAndScroll;
            break;

        case PARALLAXSPRITE_ATTR_COLORS:
#if RETRO_USE_PLUS
            if (SceneInfo->filter & FILTER_ENCORE) {
                self->color1 = 0x189098;
                self->color2 = 0x00D098;
            }
            else {
#endif
                self->color1 = 0x885820;
                self->color2 = 0xE89850;
#if RETRO_USE_PLUS
            }
#endif
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->drawFX  = FX_ROTATE;
            self->visible = !self->hiddenAtStart;
            self->state   = ParallaxSprite_State_RotateAndScroll;
            break;

        case PARALLAXSPRITE_ATTR_SPAWNER:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->state = ParallaxSprite_State_Spawner;
            break;

        case PARALLAXSPRITE_ATTR_FADEOUT:
            self->inkEffect = INK_ALPHA;
            self->visible   = true;
            self->alpha     = 0x100;
            self->state     = ParallaxSprite_State_FadeOutAndDestroy;
            break;

        case PARALLAXSPRITE_ATTR_BLENDHIGH:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->visible   = true;
            self->inkEffect = INK_BLEND;
            self->drawOrder = Zone->objectDrawHigh + 1;
            self->state     = ParallaxSprite_State_RotateAndScroll;
            break;

        case PARALLAXSPRITE_ATTR_SHIFT:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->sprX       = RSDK.GetFrame(ParallaxSprite->aniFrames, self->aniID, 1)->sprX;
            self->timerSpeed = ZONE_RAND(0, 2);
            self->xSpeed     = ZONE_RAND(0, 128);
            self->inkEffect  = INK_MASKED;
            self->visible    = true;
            self->state      = NULL;
            break;
    }

    RSDK.SetSpriteAnimation(ParallaxSprite->aniFrames, self->aniID, &self->animator, true, 0);
}

void ParallaxSprite_StageLoad(void)
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

void ParallaxSprite_State_RotateAndScroll(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDK.ProcessAnimation(&self->animator);

    self->rotation = (self->rotation + 2) & 0x1FF;
    self->scrollPos.x += self->scrollSpeed.x;
    self->scrollPos.y += self->scrollSpeed.y;
}

void ParallaxSprite_State_Spawner(void)
{
    RSDK_THIS(ParallaxSprite);

    if (!(Zone->timer & 3)) {
        EntityParallaxSprite *sprite = CREATE_ENTITY(ParallaxSprite, intToVoid(PARALLAXSPRITE_ATTR_FADEOUT), self->position.x, self->position.y);
        sprite->parallaxFactor.x     = self->parallaxFactor.x;
        sprite->parallaxFactor.y     = self->parallaxFactor.y;
        sprite->loopPoint.x          = self->loopPoint.x;
        sprite->loopPoint.y          = self->loopPoint.y;
        sprite->scrollSpeed.y        = RSDK.Rand(0x8000, 0x20000);
        sprite->acceleration.x       = RSDK.Rand(-0x100, 0x100);
    }
}

void ParallaxSprite_State_FadeOutAndDestroy(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDK.ProcessAnimation(&self->animator);

    self->scrollSpeed.x += self->acceleration.x;
    self->scrollSpeed.y += self->acceleration.y;
    self->scrollPos.x += self->scrollSpeed.x;
    self->scrollPos.y += self->scrollSpeed.y;

    self->alpha -= 2;
    if (!self->alpha)
        destroyEntity(self);
}

void ParallaxSprite_State_FadeIntoHalf(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDK.ProcessAnimation(&self->animator);

    self->scrollPos.x += self->scrollSpeed.x;
    self->scrollPos.y += self->scrollSpeed.y;

    if (self->alpha >= 0x80) {
        self->inkEffect = INK_BLEND;
        self->visible   = true;
        self->state     = ParallaxSprite_State_RotateAndScroll;
    }
    else {
        self->inkEffect = INK_ALPHA;
        self->alpha += 4;
    }
}

void ParallaxSprite_State_FadeOut(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDK.ProcessAnimation(&self->animator);

    self->scrollPos.x += self->scrollSpeed.x;
    self->scrollPos.y += self->scrollSpeed.y;

    if (self->alpha <= 0) {
        self->visible = false;
        self->state   = ParallaxSprite_State_RotateAndScroll;
    }
    else {
        self->inkEffect = INK_ALPHA;
        self->alpha -= 4;
    }
}

#if RETRO_INCLUDE_EDITOR
void ParallaxSprite_EditorDraw(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDK.SetSpriteAnimation(ParallaxSprite->aniFrames, self->aniID, &self->animator, true, 0);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    if (self->attribute == PARALLAXSPRITE_ATTR_COLORS) {
        int32 x = (drawPos.x >> 16) - 56;
        int32 y = (drawPos.y >> 16) - 32;
        RSDK.DrawRect(x << 16, y << 16, 112 << 16, 64 << 16, self->color1, 255, 0, false);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 height = minVal((RSDK.Sin256(i + Zone->timer) >> 3) + 48, 64);
            RSDK.DrawRect(x << 16, (y - height + 64) << 16, 16 << 16, height << 16, self->color2, 255, 0, false);
            x += 16;
        }
    }
    else if (self->attribute == PARALLAXSPRITE_ATTR_SHIFT) {
        RSDK.GetFrame(ParallaxSprite->aniFrames, self->aniID, 0)->sprX = self->sprX + ((self->xSpeed + (Zone->timer << self->timerSpeed)) & 0x7F);
    }

    RSDK.DrawSprite(&self->animator, &drawPos, false);
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

    RSDK_ACTIVE_VAR(ParallaxSprite, attribute);
    RSDK_ENUM_VAR("Basic", PARALLAXSPRITE_ATTR_NONE);
    RSDK_ENUM_VAR("Unused (same as Basic)", PARALLAXSPRITE_ATTR_UNUSED);
    RSDK_ENUM_VAR("Colors (SPZ Billboard)", PARALLAXSPRITE_ATTR_COLORS);
    RSDK_ENUM_VAR("Spawner", PARALLAXSPRITE_ATTR_SPAWNER);
    RSDK_ENUM_VAR("Fade Out", PARALLAXSPRITE_ATTR_FADEOUT);
    RSDK_ENUM_VAR("Blend", PARALLAXSPRITE_ATTR_BLENDHIGH);
}
#endif

void ParallaxSprite_Serialize(void)
{
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_UINT8, aniID);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_UINT8, attribute);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, parallaxFactor);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, scrollSpeed);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_VECTOR2, loopPoint);
    RSDK_EDITABLE_VAR(ParallaxSprite, VAR_BOOL, hiddenAtStart);
}
