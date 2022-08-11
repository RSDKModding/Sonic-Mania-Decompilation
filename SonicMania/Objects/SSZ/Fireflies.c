// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Fireflies Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->drawGroup     = Zone->objectDrawGroup[0];
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
            self->screenCount = RSDK.GetVideoSetting(VIDEOSETTING_SCREENCOUNT);
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
            Vector2 startPos;

            RSDKScreenInfo *screen = &ScreenInfo[self->screenID];
            int32 x                = ((screen->size.x & 0xFFFFFFFE) + 2 * screen->position.x) << 15;
            int32 y                = ((screen->size.y & 0xFFFFFFFE) + 2 * screen->position.y) << 15;
            int32 w                = ZONE_RAND(-screen->size.x, screen->size.x);
            int32 h                = ZONE_RAND(-screen->size.y, screen->size.y);

            startPos.x     = x + (w << 16);
            startPos.y     = y + (h << 16);
            bool32 isLarge = ZONE_RAND(0, 10) > 7;

            EntityFireflies *fireflies = CREATE_ENTITY(Fireflies, INT_TO_VOID(true), startPos.x, startPos.y);
            RSDK.SetSpriteAnimation(Fireflies->aniFrames, isLarge ? 3 : 0, &fireflies->animator, true, 0);
            fireflies->drawGroup = isLarge ? Zone->objectDrawGroup[1] : 1;

            fireflies->duration      = ZONE_RAND(45, 75);
            fireflies->updateRange.x = 0x800000;
            fireflies->updateRange.y = 0x800000;
            fireflies->active        = ACTIVE_NORMAL;
            fireflies->points[0]     = startPos;

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
            if (self->drawGroup == Zone->objectDrawGroup[1])
                animLow = animHigh;

            RSDK.SetSpriteAnimation(Fireflies->aniFrames, animLow, &self->animator, true, frame);
        }
    }
    else {
        frame    = self->animator.animationID % 3;
        animLow  = 1;
        animHigh = 4;
        if (self->drawGroup == Zone->objectDrawGroup[1])
            animLow = animHigh;

        RSDK.SetSpriteAnimation(Fireflies->aniFrames, animLow, &self->animator, true, frame);
    }

    int32 percent  = (self->timer << 16) / self->duration;
    self->position = MathHelpers_GetBezierPoint(percent, self->points[0].x, self->points[0].y, self->points[1].x, self->points[1].y,
                                                self->points[2].x, self->points[2].y, self->points[3].x, self->points[3].y);

    if (!percent)
        self->position = self->points[0];

    if (self->timer + 8 < self->duration) {
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
    RSDK.SetSpriteAnimation(Fireflies->aniFrames, 5, &self->animator, true, 0);

    Fireflies_Draw();
}

void Fireflies_EditorLoad(void) { Fireflies->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Fireflies.bin", SCOPE_STAGE); }
#endif

void Fireflies_Serialize(void) {}
