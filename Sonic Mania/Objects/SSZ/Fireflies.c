// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Fireflies Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectFireflies *Fireflies;

void Fireflies_Update(void)
{
    RSDK_THIS(Fireflies);
    StateMachine_Run(self->state);
}

void Fireflies_LateUpdate(void) {}

void Fireflies_StaticUpdate(void) {}

void Fireflies_Draw(void)
{
    RSDK_THIS(Fireflies);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Fireflies_Create(void *data)
{
    RSDK_THIS(Fireflies);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->drawOrder     = Zone->objectDrawLow;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        if (data) {
            self->inkEffect = INK_ADD;
            self->state     = Fireflies_State_Firefly;
        }
        else {
            self->state       = Fireflies_State_Spawner;
            self->screenCount = RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT);
        }
    }
}

void Fireflies_StageLoad(void) { Fireflies->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Fireflies.bin", SCOPE_STAGE); }

void Fireflies_State_Spawner(void)
{
    RSDK_THIS(Fireflies);

    if (self->timer <= 0) {
        self->timer = 1;
        if (Fireflies->activeFireflyCount < 48) {
            Vector2 startPos, points[1];

            RSDKScreenInfo *screen = &ScreenInfo[self->screenID];
            int32 x                = ((screen->width & 0xFFFFFFFE) + 2 * screen->position.x) << 15;
            int32 y                = ((screen->height & 0xFFFFFFFE) + 2 * screen->position.y) << 15;
#if RETRO_USE_PLUS
            int32 w = RSDK.RandSeeded(-screen->width, screen->width, &Zone->randSeed);
            int32 h = RSDK.RandSeeded(-screen->height, screen->height, &Zone->randSeed);
#else
            int32 w             = RSDK.Rand(-screen->width, screen->width);
            int32 h             = RSDK.Rand(-screen->height, screen->height);
#endif

            startPos.x = x + (w << 16);
            startPos.y = y + (h << 16);
#if RETRO_USE_PLUS
            bool32 isLarge = RSDK.RandSeeded(0, 10, &Zone->randSeed) > 7;
#else
            bool32 isLarge      = RSDK.Rand(0, 10) > 7;
#endif

            EntityFireflies *fireflies = CREATE_ENTITY(Fireflies, intToVoid(true), startPos.x, startPos.y);
            RSDK.SetSpriteAnimation(Fireflies->aniFrames, isLarge ? 3 : 0, &fireflies->animator, true, 0);
            if (isLarge)
                fireflies->drawOrder = Zone->objectDrawHigh;
            else
                fireflies->drawOrder = 1;

#if RETRO_USE_PLUS
            fireflies->duration = RSDK.RandSeeded(45, 75, &Zone->randSeed);
#else
            fireflies->duration = RSDK.Rand(45, 75);
#endif
            fireflies->updateRange.x = 0x800000;
            fireflies->updateRange.y = 0x800000;
            fireflies->active        = ACTIVE_NORMAL;
            fireflies->points[0]     = startPos;

#if RETRO_USE_PLUS
            int32 amplitude = RSDK.RandSeeded(32, 128, &Zone->randSeed);
            int32 angle     = RSDK.RandSeeded(0, 511, &Zone->randSeed);
#else
            int32 amplitude     = RSDK.Rand(32, 128);
            int32 angle         = RSDK.Rand(0, 511);
#endif
            int midPosX = startPos.x + amplitude * (RSDK.Cos512(angle) << 7);
            int midPosY = startPos.y + amplitude * (RSDK.Sin512(angle) << 7);

#if RETRO_USE_PLUS
            amplitude = RSDK.RandSeeded(32, 64, &Zone->randSeed);
            angle     = RSDK.RandSeeded(0, 511, &Zone->randSeed);
#else
            amplitude           = RSDK.Rand(32, 64);
            angle               = RSDK.Rand(0, 511);
#endif
            fireflies->points[1].x = midPosX;
            fireflies->points[1].y = midPosY;

#if RETRO_USE_PLUS
            amplitude = RSDK.RandSeeded(32, 64, &Zone->randSeed);
            angle     = RSDK.RandSeeded(0, 511, &Zone->randSeed);
#else
            amplitude           = RSDK.Rand(32, 64);
            angle               = RSDK.Rand(0, 511);
#endif
            fireflies->points[2].x = midPosX + amplitude * (RSDK.Cos512(angle) << 7);
            fireflies->points[2].y = midPosY + amplitude * (RSDK.Sin512(angle) << 7);

            fireflies->points[3].x = midPosX;
            fireflies->points[3].y = midPosY;

            ++Fireflies->activeFireflyCount;

            self->screenID = (self->screenID + 1) % self->screenCount;
        }
    }
    else {
        self->timer--;
    }
}

void Fireflies_State_Firefly(void)
{
    RSDK_THIS(Fireflies);

    int32 frame = 0, animLow = 0, animHigh = 0;
    if (self->animator.animationID % 3 || self->animator.frameID != self->animator.frameCount - 1) {
        if (self->timer == self->duration - 28) {
            frame    = 0;
            animLow  = 2;
            animHigh = 5;
            if (self->drawOrder == Zone->objectDrawHigh)
                animLow = animHigh;

            RSDK.SetSpriteAnimation(Fireflies->aniFrames, animLow, &self->animator, true, frame);
        }
    }
    else {
        frame    = self->animator.animationID % 3;
        animLow  = 1;
        animHigh = 4;
        if (self->drawOrder == Zone->objectDrawHigh)
            animLow = animHigh;

        RSDK.SetSpriteAnimation(Fireflies->aniFrames, animLow, &self->animator, true, frame);
    }

    int32 percent  = (self->timer << 16) / self->duration;
    self->position = MathHelpers_GetBezierPoint(percent, self->points[0].x, self->points[0].y, self->points[1].x, self->points[1].y,
                                                self->points[2].x, self->points[2].y, self->points[3].x, self->points[3].y);
    if (!percent)
        self->position = self->points[0];

    if (self->timer < self->duration - 8) {
        if (self->alpha < 0x100)
            self->alpha += 0x20;
    }
    else {
        self->alpha -= 0x20;
    }

    if (self->timer >= self->duration) {
        destroyEntity(self);
        --Fireflies->activeFireflyCount;
    }

    RSDK.ProcessAnimation(&self->animator);
    ++self->timer;
}

#if RETRO_INCLUDE_EDITOR
void Fireflies_EditorDraw(void)
{
    RSDK_THIS(Fireflies);
    self->drawFX = FX_FLIP;

    Fireflies_Draw();
}

void Fireflies_EditorLoad(void) { Fireflies->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Fireflies.bin", SCOPE_STAGE); }
#endif

void Fireflies_Serialize(void) {}
