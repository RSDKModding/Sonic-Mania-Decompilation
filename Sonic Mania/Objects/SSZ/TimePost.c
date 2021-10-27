#include "SonicMania.h"

ObjectTimePost *TimePost;

void TimePost_Update(void)
{
    RSDK_THIS(TimePost);
    StateMachine_Run(entity->state);
}

void TimePost_LateUpdate(void) {}

void TimePost_StaticUpdate(void) {}

void TimePost_Draw(void)
{
    RSDK_THIS(TimePost);

    if (entity->state) {
        entity->drawFX = FX_SCALE;

        int scaleX2 = abs(RSDK.Cos512(entity->rotation));
        int scaleX1 = abs(RSDK.Sin512(entity->rotation));

        Vector2 drawPos, drawPos2;
        switch (entity->rotation >> 7) {
            case 0:
            case 2:
                drawPos2.x = entity->position.x + (scaleX1 << 9);
                drawPos2.y = entity->position.y;
                drawPos.x  = -0x500 * scaleX2 - (scaleX1 << 9) + drawPos2.x;
                drawPos.y  = entity->position.y;
                break;
            case 1:
            case 3:
                drawPos2.x = entity->position.x - (scaleX1 << 9);
                drawPos2.y = entity->position.y;
                drawPos.x  = ((scaleX1 - 32) << 9) + drawPos2.x + 0x500 * scaleX2;
                drawPos.y  = entity->position.y;
                break;
            default: break;
        }

        entity->scale.x = scaleX1;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->scale.x = scaleX2;
        RSDK.DrawSprite(&entity->animator1, &drawPos2, false);

        entity->drawFX = FX_NONE;
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
}

void TimePost_Create(void *data)
{
    RSDK_THIS(TimePost);

    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(TimePost->aniFrames, 0, &entity->animator3, true, 2);
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->spinSpeed     = 0x3000;
        entity->spinCount     = 8;
        entity->maxAngle      = 0x10000;
        entity->scale.y       = 512;
        entity->active        = ACTIVE_BOUNDS;
        entity->state         = TimePost_State_Setup;
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
    TimePost->sfxFuture      = RSDK.GetSFX("SSZ1/Future.wav");
}

void TimePost_Spin(void)
{
    RSDK_THIS(TimePost);

    entity->angle += entity->spinSpeed;
    if (entity->angle >= entity->maxAngle) {
        --entity->spinCount;
        entity->maxAngle += 0x20000;
        entity->spinSpeed = minVal(0x600 * (entity->spinCount + 1), 0x3000);
        if (!entity->spinCount) {
            entity->spinSpeed = 0;
            entity->angle     = 0x10000;
        }
    }
    entity->rotation = (entity->angle >> 8) & 0x1FF;
}

void TimePost_CheckPlayerCollisions(void)
{
    RSDK_THIS(TimePost);

    int playerID = 0;
    foreach_active(Player, player)
    {
        if ((!RSDK.GetEntityID(player) || globals->gameMode == MODE_COMPETITION) && !((1 << playerID) & entity->field_78)
            && player->position.x > entity->position.x) {
            RSDK.PlaySfx(TimePost->sfxFuture, false, 255);
            entity->active = ACTIVE_NORMAL;
            if (player->superState == SUPERSTATE_SUPER)
                player->superState = SUPERSTATE_FADEOUT;

            int vel = 0;
            if (player->onGround)
                vel = player->groundVel;
            else
                vel = player->velocity.x;
            entity->velocity.y          = -(vel >> 1);
            entity->field_64            = vel / 96;
            RSDK_sceneInfo->timeEnabled = false;
            entity->state               = TimePost_State_Spin;
        }
        ++playerID;
    }
}

void TimePost_DebrisState_TimeSparkles(EntityDebris *entity)
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
                    ParticleHelpers_Unknown2(Debris_State_LightningSpark, TimePost_DebrisState_TimeSparkles, 0, player->position.x,
                                             player->position.y, 0x200000, 0x200000);
                }
            }
        }
    }
}

void TimePost_State_Setup(void)
{
    RSDK_THIS(TimePost);
    entity->state = TimePost_State_CheckPlayerCollisions;
}

void TimePost_State_CheckPlayerCollisions(void) { TimePost_CheckPlayerCollisions(); }

void TimePost_State_Spin(void)
{
    RSDK_THIS(TimePost);

    TimePost_HandleTimeSparkles();
    TimePost_Spin();
    if (!entity->spinCount)
        entity->state = TimePost_State_FinishedSpin;
}

void TimePost_State_FinishedSpin(void) { TimePost_HandleTimeSparkles(); }

#if RETRO_INCLUDE_EDITOR
void TimePost_EditorDraw(void) {}

void TimePost_EditorLoad(void) {}
#endif

void TimePost_Serialize(void) { RSDK_EDITABLE_VAR(TimePost, VAR_UINT8, type); }
