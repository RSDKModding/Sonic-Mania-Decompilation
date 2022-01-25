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
        self->drawOrder     = Zone->drawOrderLow;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (data) {
            self->inkEffect = INK_ADD;
            self->state     = Fireflies_State_Unknown2;
        }
        else {
            self->state       = Fireflies_State_Unknown1;
            self->screenCount = RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT);
        }
    }
}

void Fireflies_StageLoad(void) { Fireflies->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Fireflies.bin", SCOPE_STAGE); }

void Fireflies_State_Unknown1(void)
{
    RSDK_THIS(Fireflies);

    if (self->timer <= 0) {
        self->timer = 1;
        if (Fireflies->field_8 < 48) {
            Vector2 pos1, pos2;

            RSDKScreenInfo *screen = &ScreenInfo[self->screenID];
            int x              = ((screen->width & 0xFFFFFFFE) + 2 * screen->position.x) << 15;
            int y              = ((screen->height & 0xFFFFFFFE) + 2 * screen->position.y) << 15;
#if RETRO_USE_PLUS
            int w = RSDK.RandSeeded(-screen->width, screen->width, &Zone->randSeed);
            int h = RSDK.RandSeeded(-screen->height, screen->height, &Zone->randSeed);
#else
            int w               = RSDK.Rand(-screen->width, screen->width);
            int h               = RSDK.Rand(-screen->height, screen->height);
#endif
            pos1.y = y + (h << 16);
            pos1.x = x + (w << 16);
#if RETRO_USE_PLUS
            int type = RSDK.RandSeeded(0, 10, &Zone->randSeed) > 7;
#else
            int type            = RSDK.Rand(0, 10) > 7;
#endif

            EntityFireflies *fireflies = CREATE_ENTITY(Fireflies, intToVoid(true), pos1.x, pos1.y);
            RSDK.SetSpriteAnimation(Fireflies->aniFrames, type ? 3 : 0, &fireflies->animator, true, 0);
            if (type)
                fireflies->drawOrder = Zone->drawOrderHigh;
            else
                fireflies->drawOrder = 1;

#if RETRO_USE_PLUS
            fireflies->field_84 = RSDK.RandSeeded(45, 75, &Zone->randSeed);
#else
            fireflies->field_84 = RSDK.Rand(45, 75);
#endif
            fireflies->updateRange.x = 0x800000;
            fireflies->updateRange.y = 0x800000;
            fireflies->active        = ACTIVE_NORMAL;
            fireflies->pos1          = pos1;

#if RETRO_USE_PLUS
            int amplitude = RSDK.RandSeeded(32, 128, &Zone->randSeed);
            int angle     = RSDK.RandSeeded(0, 511, &Zone->randSeed);
#else
            int amplitude       = RSDK.Rand(32, 128);
            int angle           = RSDK.Rand(0, 511);
#endif
            int x4 = pos1.x + amplitude * (RSDK.Cos512(angle) << 7);
            int y4 = amplitude * (RSDK.Sin512(angle) << 7) + pos1.y;

#if RETRO_USE_PLUS
            amplitude = RSDK.RandSeeded(32, 64, &Zone->randSeed);
            angle     = RSDK.RandSeeded(0, 511, &Zone->randSeed);
#else
            amplitude           = RSDK.Rand(32, 64);
            angle               = RSDK.Rand(0, 511);
#endif
            pos2.x          = pos1.x + amplitude * (RSDK.Cos512(angle) << 7);
            pos2.y          = amplitude * (RSDK.Sin512(angle) << 7) + pos1.y;
            fireflies->pos2 = pos2;

#if RETRO_USE_PLUS
            amplitude = RSDK.RandSeeded(32, 64, &Zone->randSeed);
            angle     = RSDK.RandSeeded(0, 511, &Zone->randSeed);
#else
            amplitude           = RSDK.Rand(32, 64);
            angle               = RSDK.Rand(0, 511);
#endif
            fireflies->pos3.x = x4 + amplitude * (RSDK.Cos512(angle) << 7);
            fireflies->pos3.y = amplitude * (RSDK.Sin512(angle) << 7) + y4;

            fireflies->pos4.x = x4;
            fireflies->pos4.y = y4;

            ++Fireflies->field_8;

            self->screenID = (self->screenID + 1) % self->screenCount;
        }
    }
    else {
        self->timer--;
    }
}

void Fireflies_State_Unknown2(void)
{
    RSDK_THIS(Fireflies);

    int frame = 0, anim = 0, anim2 = 0;
    if (self->animator.animationID % 3 || self->animator.frameID != self->animator.frameCount - 1) {
        if (self->timer == self->field_84 - 28) {
            frame = 0;
            anim2 = 5;
            anim  = 2;
            if (self->drawOrder == Zone->drawOrderHigh)
                anim = anim2;

            RSDK.SetSpriteAnimation(Fireflies->aniFrames, anim, &self->animator, true, frame);
        }
    }
    else {
        frame = self->animator.animationID % 3;
        anim  = 1;
        anim2 = 4;
        if (self->drawOrder == Zone->drawOrderHigh)
            anim = anim2;

        RSDK.SetSpriteAnimation(Fireflies->aniFrames, anim, &self->animator, true, frame);
    }

    int percent      = (self->timer << 16) / self->field_84;
    self->position = MathHelpers_GetBezierPoint(percent, self->pos1.x, self->pos1.y, self->pos2.x, self->pos2.y, self->pos3.x, self->pos3.y,
                                            self->pos4.x, self->pos4.y);
    if (!percent)
        self->position = self->pos1;

    if (self->timer < self->field_84 - 8) {
        if (self->alpha < 256)
            self->alpha += 32;
    }
    else {
        self->alpha -= 32;
    }
    if (self->timer >= self->field_84) {
        destroyEntity(self);
        --Fireflies->field_8;
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
