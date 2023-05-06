// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ParallaxSprite Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        int32 x = FROM_FIXED(drawPos.x) - 56;
        int32 y = FROM_FIXED(drawPos.y) - 32;
        RSDK.DrawRect(x, y, 112, 64, self->color1, 255, INK_NONE, true);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 height = MIN((RSDK.Sin256(i + Zone->timer) >> 3) + 48, 64);
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
    self->drawGroup = Zone->fgDrawGroup[0] + 1;

    if (data)
        self->attribute = VOID_TO_INT(data);

    switch (self->attribute) {
        default:
        case PARALLAXSPRITE_ATTR_STANDARD:
        case PARALLAXSPRITE_ATTR_WINDMILL:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->drawFX  = FX_ROTATE;
            self->visible = !self->hiddenAtStart;
            self->state   = ParallaxSprite_State_Normal;
            break;

        case PARALLAXSPRITE_ATTR_COLORS:
#if MANIA_USE_PLUS
            if (SceneInfo->filter & FILTER_ENCORE) {
                self->color1 = 0x189098;
                self->color2 = 0x00D098;
            }
            else {
#endif
                self->color1 = 0x885820;
                self->color2 = 0xE89850;
#if MANIA_USE_PLUS
            }
#endif
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->drawFX  = FX_ROTATE;
            self->visible = !self->hiddenAtStart;
            self->state   = ParallaxSprite_State_Normal;
            break;

        case PARALLAXSPRITE_ATTR_EMITTER:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->state = ParallaxSprite_State_Emitter;
            break;

        case PARALLAXSPRITE_ATTR_PARTICLE:
            self->inkEffect = INK_ALPHA;
            self->visible   = true;
            self->alpha     = 0x100;
            self->state     = ParallaxSprite_State_Particle;
            break;

        case PARALLAXSPRITE_ATTR_BLENDHIGH:
            self->parallaxFactor.x >>= 8;
            self->parallaxFactor.y >>= 8;
            self->visible   = true;
            self->inkEffect = INK_BLEND;
            self->drawGroup = Zone->objectDrawGroup[1] + 1;
            self->state     = ParallaxSprite_State_Normal;
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
    if (RSDK.CheckSceneFolder("AIZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("CPZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CPZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SPZ1"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/SPZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("FBZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("MSZ") || RSDK.CheckSceneFolder("MSZCutscene"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("MSZ/MSZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("OOZ2"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("OOZ/OOZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZParallax.bin", SCOPE_STAGE);
}

void ParallaxSprite_State_Normal(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDK.ProcessAnimation(&self->animator);

    self->rotation = (self->rotation + 2) & 0x1FF;

    self->scrollPos.x += self->scrollSpeed.x;
    self->scrollPos.y += self->scrollSpeed.y;
}

void ParallaxSprite_State_Emitter(void)
{
    RSDK_THIS(ParallaxSprite);

    if (!(Zone->timer & 3)) {
        EntityParallaxSprite *sprite = CREATE_ENTITY(ParallaxSprite, INT_TO_VOID(PARALLAXSPRITE_ATTR_PARTICLE), self->position.x, self->position.y);
        sprite->parallaxFactor.x     = self->parallaxFactor.x;
        sprite->parallaxFactor.y     = self->parallaxFactor.y;
        sprite->loopPoint.x          = self->loopPoint.x;
        sprite->loopPoint.y          = self->loopPoint.y;
        sprite->scrollSpeed.y        = RSDK.Rand(0x8000, 0x20000);
        sprite->acceleration.x       = RSDK.Rand(-0x100, 0x100);
    }
}

void ParallaxSprite_State_Particle(void)
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
        self->state     = ParallaxSprite_State_Normal;
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
        self->state   = ParallaxSprite_State_Normal;
    }
    else {
        self->inkEffect = INK_ALPHA;
        self->alpha -= 4;
    }
}

#if GAME_INCLUDE_EDITOR
void ParallaxSprite_EditorDraw(void)
{
    RSDK_THIS(ParallaxSprite);

    RSDK.SetSpriteAnimation(ParallaxSprite->aniFrames, self->aniID, &self->animator, true, 0);

    Vector2 drawPos = self->position;

    if (self->attribute == PARALLAXSPRITE_ATTR_COLORS) {
        int32 x = FROM_FIXED(drawPos.x) - 56;
        int32 y = FROM_FIXED(drawPos.y) - 32;
        RSDK.DrawRect(x << 16, y << 16, 112 << 16, 64 << 16, self->color1, 255, 0, false);

        for (int32 i = 0; i < 0xE0; i += 0x20) {
            int32 height = MIN((RSDK.Sin256(i + Zone->timer) >> 3) + 48, 64);
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
    if (RSDK.CheckSceneFolder("AIZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("CPZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("CPZ/CPZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SPZ1"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/SPZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("FBZ"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("FBZ/FBZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("MSZ") || RSDK.CheckSceneFolder("MSZCutscene"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("MSZ/MSZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("OOZ2"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("OOZ/OOZParallax.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        ParallaxSprite->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZParallax.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(ParallaxSprite, attribute);
    RSDK_ENUM_VAR("Standard", PARALLAXSPRITE_ATTR_STANDARD);
    RSDK_ENUM_VAR("Windmill (Unused)", PARALLAXSPRITE_ATTR_WINDMILL);
    RSDK_ENUM_VAR("Colors (SPZ Billboard)", PARALLAXSPRITE_ATTR_COLORS);
    RSDK_ENUM_VAR("Emitter", PARALLAXSPRITE_ATTR_EMITTER);
    RSDK_ENUM_VAR("Particle", PARALLAXSPRITE_ATTR_PARTICLE);
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
