// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimePost Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTimePost *TimePost;

void TimePost_Update(void)
{
    RSDK_THIS(TimePost);

    StateMachine_Run(self->state);
}

void TimePost_LateUpdate(void) {}

void TimePost_StaticUpdate(void) {}

void TimePost_Draw(void)
{
    RSDK_THIS(TimePost);

    if (self->state) {
        self->drawFX = FX_SCALE;

        Vector2 sidePos, faceplatePos;
        switch (self->rotation >> 7) {
            case 0:
            case 2:
                faceplatePos.x = self->position.x + (abs(RSDK.Sin512(self->rotation)) << 9);
                faceplatePos.y = self->position.y;
                sidePos.x      = -0x500 * abs(RSDK.Cos512(self->rotation)) - (abs(RSDK.Sin512(self->rotation)) << 9) + faceplatePos.x;
                sidePos.y      = self->position.y;
                break;

            case 1:
            case 3:
                faceplatePos.x = self->position.x - (abs(RSDK.Sin512(self->rotation)) << 9);
                faceplatePos.y = self->position.y;
                sidePos.x      = ((abs(RSDK.Sin512(self->rotation)) - 32) << 9) + faceplatePos.x + 0x500 * abs(RSDK.Cos512(self->rotation));
                sidePos.y      = self->position.y;
                break;

            default: break;
        }

        self->scale.x = abs(RSDK.Sin512(self->rotation));
        RSDK.DrawSprite(&self->sideAnimator, &sidePos, false);

        self->scale.x = abs(RSDK.Cos512(self->rotation));
        RSDK.DrawSprite(&self->faceplateAnimator, &faceplatePos, false);

        self->drawFX = FX_NONE;
        RSDK.DrawSprite(&self->standAnimator, NULL, false);
    }
}

void TimePost_Create(void *data)
{
    RSDK_THIS(TimePost);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->faceplateAnimator, true, 0);
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->sideAnimator, true, 1);
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->standAnimator, true, 2);

        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];

        self->spinSpeed = 0x3000;
        self->spinCount = 8;
        self->maxAngle  = 0x10000;
        self->scale.y   = 0x200;
        self->active    = ACTIVE_BOUNDS;
        self->state     = TimePost_State_Init;
    }
}

void TimePost_StageLoad(void)
{
    TimePost->aniFrames     = RSDK.LoadSpriteAnimation("SSZ1/TimePost.bin", SCOPE_STAGE);
    TimePost->sparkleFrames = RSDK.LoadSpriteAnimation("SSZ1/TTSparkle.bin", SCOPE_STAGE);

    TimePost->hitbox.left   = -16;
    TimePost->hitbox.top    = -40;
    TimePost->hitbox.right  = 16;
    TimePost->hitbox.bottom = -24;

    TimePost->hitboxItemBox.left   = -8;
    TimePost->hitboxItemBox.top    = -24;
    TimePost->hitboxItemBox.right  = 8;
    TimePost->hitboxItemBox.bottom = 24;

    TimePost->sfxFuture = RSDK.GetSfx("SSZ1/Future.wav");
}

void TimePost_Spin(void)
{
    RSDK_THIS(TimePost);

    self->angle += self->spinSpeed;

    if (self->angle >= self->maxAngle) {
        self->maxAngle += 0x20000;
        self->spinSpeed = MIN(0x600 * self->spinCount, 0x3000);

        if (!--self->spinCount) {
            self->spinSpeed = 0;
            self->angle     = 0x10000;
        }
    }

    self->rotation = (self->angle >> 8) & 0x1FF;
}

void TimePost_CheckPlayerCollisions(void)
{
    RSDK_THIS(TimePost);

    int32 playerID = 0;
    foreach_active(Player, player)
    {
        if ((!RSDK.GetEntitySlot(player) || globals->gameMode == MODE_COMPETITION) && !((1 << playerID) & self->activePlayers)
            && player->position.x > self->position.x) {
            RSDK.PlaySfx(TimePost->sfxFuture, false, 255);

            self->active = ACTIVE_NORMAL;
            if (player->superState == SUPERSTATE_SUPER)
                player->superState = SUPERSTATE_FADEOUT;

            // Lol this was prolly copied from Global/SignPost, there's no reason for this obj to use YVelocity and the methods are the same as they
            // are there
            int32 vel              = player->onGround ? player->groundVel : player->velocity.x;
            self->velocity.y       = -(vel >> 1);
            self->gravityStrength  = vel / 96;
            SceneInfo->timeEnabled = false;
            self->state            = TimePost_State_Spinning;
        }

        ++playerID;
    }
}

void TimePost_Particle_TimeSparkle(EntityDebris *debris)
{
    RSDK.SetSpriteAnimation(TimePost->sparkleFrames, 0, &debris->animator, true, 0);

    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;
    debris->drawGroup     = Zone->objectDrawGroup[1];
    debris->timer         = 180;
}

void TimePost_HandleTimeSparkles(void)
{
    bool32 spawnedDebris = true;

    Vector2 onScreenRange;
    onScreenRange.x = 0;
    onScreenRange.y = 0;

    if (!(Zone->timer % 5)) {
        foreach_active(Player, player)
        {
            if (abs(player->groundVel) >= 0x80000) {
                if (RSDK.CheckOnScreen(player, &onScreenRange)) {
                    spawnedDebris = false;
                    ParticleHelpers_SetupParticleFX(Debris_State_Move, TimePost_Particle_TimeSparkle, 0, player->position.x, player->position.y,
                                                    0x200000, 0x200000);
                }
            }
        }
    }
}

void TimePost_State_Init(void)
{
    RSDK_THIS(TimePost);

    self->state = TimePost_State_CheckPlayerCollisions;
}

void TimePost_State_CheckPlayerCollisions(void) { TimePost_CheckPlayerCollisions(); }

void TimePost_State_Spinning(void)
{
    RSDK_THIS(TimePost);

    TimePost_HandleTimeSparkles();
    TimePost_Spin();

    if (!self->spinCount)
        self->state = TimePost_State_FinishedSpin;
}

void TimePost_State_FinishedSpin(void) { TimePost_HandleTimeSparkles(); }

#if GAME_INCLUDE_EDITOR
void TimePost_EditorDraw(void)
{
    RSDK_THIS(TimePost);

    RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->faceplateAnimator, false, 0);
    RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->sideAnimator, false, 1);
    RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->standAnimator, false, 2);

    self->drawFX = FX_SCALE;

    Vector2 sidePos, faceplatePos;
    switch (self->rotation >> 7) {
        case 0:
        case 2:
            faceplatePos.x = self->position.x + (abs(RSDK.Sin512(self->rotation)) << 9);
            faceplatePos.y = self->position.y;
            sidePos.x      = -0x500 * abs(RSDK.Cos512(self->rotation)) - (abs(RSDK.Sin512(self->rotation)) << 9) + faceplatePos.x;
            sidePos.y      = self->position.y;
            break;

        case 1:
        case 3:
            faceplatePos.x = self->position.x - (abs(RSDK.Sin512(self->rotation)) << 9);
            faceplatePos.y = self->position.y;
            sidePos.x      = ((abs(RSDK.Sin512(self->rotation)) - 32) << 9) + faceplatePos.x + 0x500 * abs(RSDK.Cos512(self->rotation));
            sidePos.y      = self->position.y;
            break;

        default: break;
    }

    self->scale.x = abs(RSDK.Sin512(self->rotation));
    RSDK.DrawSprite(&self->sideAnimator, &sidePos, false);

    self->scale.x = abs(RSDK.Cos512(self->rotation));
    RSDK.DrawSprite(&self->faceplateAnimator, &faceplatePos, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->standAnimator, NULL, false);
}

void TimePost_EditorLoad(void)
{
    TimePost->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/TimePost.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TimePost, type); // might have been "Past"/"Future" at one point?
    RSDK_ENUM_VAR("(Unused)", 0);
}
#endif

void TimePost_Serialize(void) { RSDK_EDITABLE_VAR(TimePost, VAR_UINT8, type); }
