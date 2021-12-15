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
    drawPos.x      = self->position.x - scrollPosX;

    int32 scrollPosY = ((self->scrollPos.y + self->parallaxFactor.y * screen->position.y) & 0x7FFF0000) % self->loopPoint.y;
    drawPos.y      = self->position.y - scrollPosY;

    int32 loopX = -self->loopPoint.x >> 2;
    if (drawPos.x < loopX) {
        drawPos.x += self->loopPoint.x;
    }

    int32 loopY = -self->loopPoint.y >> 2;
    if (drawPos.y < loopY) {
        drawPos.y += self->loopPoint.y;
    }

    if (self->attribute == PSPRITE_ATTR_COLORS) {
        int32 y = (drawPos.y >> 16) - 32;
        int32 x = (drawPos.x >> 16) - 56;
        RSDK.DrawRect(x, y, 112, 64, self->colour1, 255, INK_NONE, true);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 val = (RSDK.Sin256(i + Zone->timer) >> 3) + 48;
            if (val > 64)
                val = 64;
            RSDK.DrawRect(x, y - val + 64, 16, val, self->colour2, 255, INK_NONE, true);
            x += 16;
        }
    }
    else if (self->attribute == PSPRITE_ATTR_SHIFT) {
        RSDK.GetFrame(ParallaxSprite->aniFrames, self->aniID, 0)->sprX =
            self->sprX + ((self->xSpeed + (Zone->timer << self->timerSpeed)) & 0x7F);
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
        case PSPRITE_ATTR_NONE:
        default:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->drawFX  = FX_ROTATE;
            self->visible = !self->hiddenAtStart;
            self->state   = ParallaxSprite_State_RotateAndScroll;
            break;
        case PSPRITE_ATTR_COLORS:
#if RETRO_USE_PLUS
            if (SceneInfo->filter & FILTER_ENCORE) {
                self->colour1 = 0x189098;
                self->colour2 = 0x00D098;
            }
            else {
#endif
                self->colour1 = 0x885820;
                self->colour2 = 0xE89850;
#if RETRO_USE_PLUS
            }
#endif
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->drawFX  = FX_ROTATE;
            self->visible = !self->hiddenAtStart;
            self->state   = ParallaxSprite_State_RotateAndScroll;
            break;
        case PSPRITE_ATTR_SPAWNER:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->state = ParallaxSprite_State_Spawner;
            break;
        case PSPRITE_ATTR_FADEOUT:
            self->inkEffect = INK_ALPHA;
            self->visible   = true;
            self->alpha     = 0x100;
            self->state     = ParallaxSprite_State_FadeOutAndDie;
            break;
        case PSPRITE_ATTR_BLENDHIGH:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->visible   = true;
            self->inkEffect = INK_BLEND;
            self->drawOrder = Zone->drawOrderHigh + 1;
            self->state     = ParallaxSprite_State_RotateAndScroll;
            break;
        case PSPRITE_ATTR_SHIFT:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->sprX      = RSDK.GetFrame(ParallaxSprite->aniFrames, self->aniID, 1)->sprX;
#if RETRO_USE_PLUS
            self->timerSpeed  = RSDK.RandSeeded(0, 2, &Zone->randSeed);
            self->xSpeed  = RSDK.RandSeeded(0, 128, &Zone->randSeed);
#else
            self->timerSpeed = RSDK.Rand(0, 2);
            self->xSpeed = RSDK.Rand(0, 128);
#endif
            self->inkEffect = INK_MASKED;
            self->visible   = true;
            self->state     = NULL;
            break;
    }
    RSDK.SetSpriteAnimation(ParallaxSprite->aniFrames, self->aniID, &self->animator, true, 0);
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
        EntityParallaxSprite *pSprite = CREATE_ENTITY(ParallaxSprite, intToVoid(PSPRITE_ATTR_FADEOUT), self->position.x, self->position.y);
        pSprite->parallaxFactor.x = self->parallaxFactor.x;
        pSprite->parallaxFactor.y = self->parallaxFactor.y;
        pSprite->loopPoint.x      = self->loopPoint.x;
        pSprite->loopPoint.y      = self->loopPoint.y;
        pSprite->scrollSpeed.y    = RSDK.Rand(0x8000, 0x20000);
        pSprite->acceleration.x    = RSDK.Rand(-0x100, 0x100);
    }
}

void ParallaxSprite_State_FadeOutAndDie(void)
{
    // :D - rmg
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
    if (self->alpha >= 128) {
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

    drawPos.x        = self->position.x;
    drawPos.y        = self->position.y;

    if (self->attribute == PSPRITE_ATTR_COLORS) {
        int32 x = (drawPos.x >> 16) - 56;
        int32 y = (drawPos.y >> 16) - 32;
        RSDK.DrawRect(x << 16, y << 16, 112 << 16, 64 << 16, self->colour1, 255, 0, false);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 val = (RSDK.Sin256(i + Zone->timer) >> 3) + 48;
            if (val > 64)
                val = 64;
            RSDK.DrawRect(x << 16, (y - val + 64) << 16, 16 << 16, val << 16, self->colour2, 255, 0, false);
            x += 16;
        }
    }
    else if (self->attribute == PSPRITE_ATTR_SHIFT) {
        RSDK.GetFrame(ParallaxSprite->aniFrames, self->aniID, 0)->sprX =
            self->sprX + ((self->xSpeed + (Zone->timer << self->timerSpeed)) & 0x7F);
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
    RSDK_ENUM_VAR("Basic", PSPRITE_ATTR_NONE);
    RSDK_ENUM_VAR("Unused (same as Basic)", PSPRITE_ATTR_UNUSED);
    RSDK_ENUM_VAR("Colors (SPZ Billboard)", PSPRITE_ATTR_COLORS);
    RSDK_ENUM_VAR("Spawner", PSPRITE_ATTR_SPAWNER);
    RSDK_ENUM_VAR("Fade Out", PSPRITE_ATTR_FADEOUT);
    RSDK_ENUM_VAR("Blend", PSPRITE_ATTR_BLENDHIGH);
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
