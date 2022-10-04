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
        self->updateRange.x = TO_FIXED(128);
        self->updateRange.y = TO_FIXED(128);

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
            int32 screenCenterX    = TO_FIXED(screen->position.x + (screen->size.x / 2));
            int32 screenCenterY    = TO_FIXED(screen->position.y + (screen->size.y / 2));
            int32 offsetX          = TO_FIXED(ZONE_RAND(-screen->size.x, screen->size.x));
            int32 offsetY          = TO_FIXED(ZONE_RAND(-screen->size.y, screen->size.y));

            startPos.x     = screenCenterX + offsetX;
            startPos.y     = screenCenterY + offsetY;
            bool32 isLarge = ZONE_RAND(0, 10) > 7;

            EntityFireflies *fireflies = CREATE_ENTITY(Fireflies, INT_TO_VOID(true), startPos.x, startPos.y);
            RSDK.SetSpriteAnimation(Fireflies->aniFrames, isLarge ? 3 : 0, &fireflies->animator, true, 0);
            fireflies->drawGroup = isLarge ? Zone->objectDrawGroup[1] : 1;

            fireflies->duration      = ZONE_RAND(45, 75);
            fireflies->updateRange.x = TO_FIXED(128);
            fireflies->updateRange.y = TO_FIXED(128);
            fireflies->active        = ACTIVE_NORMAL;

            int32 points3_delta = ZONE_RAND(32, 128);
            int32 points3_angle = ZONE_RAND(0, 511);
            int32 points3_x = startPos.x + points3_delta * (RSDK.Cos512(points3_angle) << 7);
            int32 points3_y = startPos.y + points3_delta * (RSDK.Sin512(points3_angle) << 7);

            int32 points1_delta = ZONE_RAND(32, 64);
            int32 points1_angle = ZONE_RAND(0, 511);
            int32 points1_x = startPos.x + points1_delta * (RSDK.Cos512(points1_angle) << 7);
            int32 points1_y = startPos.y + points1_delta * (RSDK.Sin512(points1_angle) << 7);

            int32 points2_delta = ZONE_RAND(32, 64);
            int32 points2_angle = ZONE_RAND(0, 511);
            int32 points2_x = points3_x + points2_delta * (RSDK.Cos512(points2_angle) << 7);
	    int32 points2_y = points3_y + points2_delta * (RSDK.Sin512(points2_angle) << 7);

            fireflies->points[0].x = startPos.x;
            fireflies->points[0].y = startPos.y;
            fireflies->points[1].x = points1_x;
            fireflies->points[1].y = points1_y;
            fireflies->points[2].x = points2_x;
            fireflies->points[2].y = points2_y;
            fireflies->points[3].x = points3_x;
            fireflies->points[3].y = points3_y;

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

    int32 percent  = TO_FIXED(self->timer) / self->duration;
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
