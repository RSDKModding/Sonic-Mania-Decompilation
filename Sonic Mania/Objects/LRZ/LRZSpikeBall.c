#include "SonicMania.h"

ObjectLRZSpikeBall *LRZSpikeBall;

void LRZSpikeBall_Update(void)
{
    RSDK_THIS(LRZSpikeBall);
    StateMachine_Run(entity->state);
    LRZSpikeBall_CheckPlayerBaseCollisions();
    LRZSpikeBall_CheckPlayerBallCollisions();
    entity->updateRange.y = abs(entity->offset.y) + 0x800000;
    RSDK.ProcessAnimation(&entity->animator2);
}

void LRZSpikeBall_LateUpdate(void) {}

void LRZSpikeBall_StaticUpdate(void) {}

void LRZSpikeBall_Draw(void)
{
    RSDK_THIS(LRZSpikeBall);
    entity->position.x += entity->offset.x;
    entity->position.y += entity->offset.y;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->position.x -= entity->offset.x;
    entity->position.y -= entity->offset.y;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void LRZSpikeBall_Create(void *data)
{
    RSDK_THIS(LRZSpikeBall);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!entity->interval)
        entity->interval = 240;

    entity->hitboxBase.left   = -16;
    entity->hitboxBase.top    = 8;
    entity->hitboxBase.right  = 8;
    entity->hitboxBase.bottom = 16;
    entity->hitboxBall.left   = -10;
    entity->hitboxBall.top    = -10;
    entity->hitboxBall.right  = 10;
    entity->hitboxBall.bottom = 10;
    entity->state             = LRZSpikeBall_State_Setup;
}

void LRZSpikeBall_StageLoad(void)
{
    LRZSpikeBall->aniFrames    = RSDK.LoadSpriteAnimation("LRZ2/LRZSpikeBall.bin", SCOPE_STAGE);
    LRZSpikeBall->sfxCharge    = RSDK.GetSFX("LRZ/Charge.wav");
    LRZSpikeBall->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
}

int LRZSpikeBall_GetFrameID(void)
{
    RSDK_THIS(LRZSpikeBall);

    if (entity->timer <= 0)
        return 0;

    int id = 0, inc = 0, nextInc = 14;
    bool32 flag = false;
    for (; id < entity->timer; id += inc) {
        inc = nextInc;
        if (flag) {
            flag = false;
            if (nextInc - 2 > 1)
                nextInc -= 2;
            else
                nextInc = 1;
        }
        else {
            flag = true;
        }
        id += inc;
    }

    if (!flag)
        return 0;

    if (id - entity->timer < 2 || id - entity->timer > nextInc - 2)
        return 1;
    else
        return 2;
}

void LRZSpikeBall_CheckPlayerBaseCollisions(void)
{
    RSDK_THIS(LRZSpikeBall);

    foreach_active(Player, player) { Player_CheckCollisionBox(player, entity, &entity->hitboxBase); }
}

void LRZSpikeBall_CheckPlayerBallCollisions(void)
{
    RSDK_THIS(LRZSpikeBall);

    int storeX = entity->position.x;
    int storeY = entity->position.y;
    entity->position.x += entity->offset.x;
    entity->position.y += entity->offset.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitboxBall)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, true, &player->uncurlTimer))
#endif
                Player_CheckHit(player, entity);
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void LRZSpikeBall_State_Setup(void)
{
    RSDK_THIS(LRZSpikeBall);

    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &entity->animator2, true, 0);
    entity->offset.x = 0;
    entity->offset.y = 0;
    entity->state    = LRZSpikeBall_State_Unknown1;
}

void LRZSpikeBall_State_Unknown1(void)
{
    RSDK_THIS(LRZSpikeBall);

    if (!entity->interval || !((entity->intervalOffset + Zone->timer) % entity->interval)) {
        entity->timer  = 0;
        entity->active = ACTIVE_NORMAL;
        entity->state  = LRZSpikeBall_State_Unknown2;
        RSDK.PlaySfx(LRZSpikeBall->sfxCharge, false, 255);
    }
}

void LRZSpikeBall_State_Unknown2(void)
{
    RSDK_THIS(LRZSpikeBall);

    entity->animator1.frameID = LRZSpikeBall_GetFrameID();

    if (entity->timer++ >= 142) {
        entity->velocity.y = -0x8000 * entity->strength;
        entity->state      = LRZSpikeBall_State_Unknown3;
        RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 2, &entity->animator2, true, 0);
        RSDK.PlaySfx(LRZSpikeBall->sfxExplosion, false, 255);
    }
}

void LRZSpikeBall_State_Unknown3(void)
{
    RSDK_THIS(LRZSpikeBall);

    entity->offset.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (entity->velocity.y >= 0 && entity->offset.y >= 0) {
        entity->offset.y   = 0;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &entity->animator2, true, 0);
        entity->active = ACTIVE_BOUNDS;
        entity->state  = LRZSpikeBall_State_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void LRZSpikeBall_EditorDraw(void)
{
    RSDK_THIS(LRZSpikeBall);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(LRZSpikeBall->aniFrames, 1, &entity->animator2, true, 0);
    entity->offset.x = 0;
    entity->offset.y = 0;

    entity->inkEffect = INK_NONE;

    LRZSpikeBall_Draw();

    entity->inkEffect  = INK_BLEND;
    entity->velocity.y = -0x8000 * entity->strength;

    do {
        entity->offset.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
    } while (entity->velocity.y < 0 && entity->offset.y < 0);

    entity->position.x += entity->offset.x;
    entity->position.y += entity->offset.y;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->position.x -= entity->offset.x;
    entity->position.y -= entity->offset.y;
}

void LRZSpikeBall_EditorLoad(void) { LRZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZSpikeBall.bin", SCOPE_STAGE); }
#endif

void LRZSpikeBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, interval);
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, intervalOffset);
    RSDK_EDITABLE_VAR(LRZSpikeBall, VAR_ENUM, strength);
}
