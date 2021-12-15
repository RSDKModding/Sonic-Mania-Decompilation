// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimePost Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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

        int scaleX2 = abs(RSDK.Cos512(self->rotation));
        int scaleX1 = abs(RSDK.Sin512(self->rotation));

        Vector2 drawPos, drawPos2;
        switch (self->rotation >> 7) {
            case 0:
            case 2:
                drawPos2.x = self->position.x + (scaleX1 << 9);
                drawPos2.y = self->position.y;
                drawPos.x  = -0x500 * scaleX2 - (scaleX1 << 9) + drawPos2.x;
                drawPos.y  = self->position.y;
                break;
            case 1:
            case 3:
                drawPos2.x = self->position.x - (scaleX1 << 9);
                drawPos2.y = self->position.y;
                drawPos.x  = ((scaleX1 - 32) << 9) + drawPos2.x + 0x500 * scaleX2;
                drawPos.y  = self->position.y;
                break;
            default: break;
        }

        self->scale.x = scaleX1;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->scale.x = scaleX2;
        RSDK.DrawSprite(&self->animator1, &drawPos2, false);

        self->drawFX = FX_NONE;
        RSDK.DrawSprite(&self->animator3, NULL, false);
    }
}

void TimePost_Create(void *data)
{
    RSDK_THIS(TimePost);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->animator2, true, 1);
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->animator3, true, 2);
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->spinSpeed     = 0x3000;
        self->spinCount     = 8;
        self->maxAngle      = 0x10000;
        self->scale.y       = 512;
        self->active        = ACTIVE_BOUNDS;
        self->state         = TimePost_State_Setup;
    }
}

void TimePost_StageLoad(void)
{
    TimePost->aniFrames      = RSDK.LoadSpriteAnimation("SSZ1/TimePost.bin", SCOPE_STAGE);
    TimePost->sparkleFrames  = RSDK.LoadSpriteAnimation("SSZ1/TTSparkle.bin", SCOPE_STAGE);
    TimePost->hitbox1.left   = -16;
    TimePost->hitbox1.top    = -40;
    TimePost->hitbox1.right  = 16;
    TimePost->hitbox1.bottom = -24;
    TimePost->hitbox2.left   = -8;
    TimePost->hitbox2.top    = -24;
    TimePost->hitbox2.right  = 8;
    TimePost->hitbox2.bottom = 24;
    TimePost->sfxFuture      = RSDK.GetSfx("SSZ1/Future.wav");
}

void TimePost_Spin(void)
{
    RSDK_THIS(TimePost);

    self->angle += self->spinSpeed;
    if (self->angle >= self->maxAngle) {
        --self->spinCount;
        self->maxAngle += 0x20000;
        self->spinSpeed = minVal(0x600 * (self->spinCount + 1), 0x3000);
        if (!self->spinCount) {
            self->spinSpeed = 0;
            self->angle     = 0x10000;
        }
    }
    self->rotation = (self->angle >> 8) & 0x1FF;
}

void TimePost_CheckPlayerCollisions(void)
{
    RSDK_THIS(TimePost);

    int playerID = 0;
    foreach_active(Player, player)
    {
        if ((!RSDK.GetEntityID(player) || globals->gameMode == MODE_COMPETITION) && !((1 << playerID) & self->field_78)
            && player->position.x > self->position.x) {
            RSDK.PlaySfx(TimePost->sfxFuture, false, 255);
            self->active = ACTIVE_NORMAL;
            if (player->superState == SUPERSTATE_SUPER)
                player->superState = SUPERSTATE_FADEOUT;

            int vel = 0;
            if (player->onGround)
                vel = player->groundVel;
            else
                vel = player->velocity.x;
            self->velocity.y          = -(vel >> 1);
            self->field_64            = vel / 96;
            SceneInfo->timeEnabled = false;
            self->state               = TimePost_State_Spin;
        }
        ++playerID;
    }
}

void TimePost_ParticleCB_TimeSparkles(EntityDebris *entity)
{
    RSDK.SetSpriteAnimation(TimePost->sparkleFrames, 0, &entity->animator, true, 0);
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->timer         = 180;
}

void TimePost_HandleTimeSparkles(void)
{
    bool32 flag = true;
    if (!(Zone->timer % 5)) {
        foreach_active(Player, player)
        {
            if (abs(player->groundVel) >= 0x80000) {
                Vector2 range;
                range.x = 0;
                range.y = 0;
                if (RSDK.CheckOnScreen(player, &range)) {
                    flag = false;
                    ParticleHelpers_SetupParticleFX(Debris_State_Move, TimePost_ParticleCB_TimeSparkles, 0, player->position.x,
                                             player->position.y, 0x200000, 0x200000);
                }
            }
        }
    }
}

void TimePost_State_Setup(void)
{
    RSDK_THIS(TimePost);
    self->state = TimePost_State_CheckPlayerCollisions;
}

void TimePost_State_CheckPlayerCollisions(void) { TimePost_CheckPlayerCollisions(); }

void TimePost_State_Spin(void)
{
    RSDK_THIS(TimePost);

    TimePost_HandleTimeSparkles();
    TimePost_Spin();
    if (!self->spinCount)
        self->state = TimePost_State_FinishedSpin;
}

void TimePost_State_FinishedSpin(void) { TimePost_HandleTimeSparkles(); }

#if RETRO_INCLUDE_EDITOR
void TimePost_EditorDraw(void)
{
    RSDK_THIS(TimePost);
    RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->animator2, false, 1);
    RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &self->animator3, false, 2);

    self->drawFX = FX_SCALE;

    int scaleX2 = abs(RSDK.Cos512(self->rotation));
    int scaleX1 = abs(RSDK.Sin512(self->rotation));

    Vector2 drawPos, drawPos2;
    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos2.x = self->position.x + (scaleX1 << 9);
            drawPos2.y = self->position.y;
            drawPos.x  = -0x500 * scaleX2 - (scaleX1 << 9) + drawPos2.x;
            drawPos.y  = self->position.y;
            break;
        case 1:
        case 3:
            drawPos2.x = self->position.x - (scaleX1 << 9);
            drawPos2.y = self->position.y;
            drawPos.x  = ((scaleX1 - 32) << 9) + drawPos2.x + 0x500 * scaleX2;
            drawPos.y  = self->position.y;
            break;
        default: break;
    }

    self->scale.x = scaleX1;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->scale.x = scaleX2;
    RSDK.DrawSprite(&self->animator1, &drawPos2, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void TimePost_EditorLoad(void) { TimePost->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/TimePost.bin", SCOPE_STAGE); }
#endif

void TimePost_Serialize(void) { RSDK_EDITABLE_VAR(TimePost, VAR_UINT8, type); }
