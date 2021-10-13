#include "SonicMania.h"

ObjectBallHog *BallHog;

void BallHog_Update(void)
{
    RSDK_THIS(BallHog);

    StateMachine_Run(entity->state);
}

void BallHog_LateUpdate(void) {}

void BallHog_StaticUpdate(void) {}

void BallHog_Draw(void)
{
    RSDK_THIS(BallHog);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void BallHog_Create(void *data)
{
    RSDK_THIS(BallHog);

    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 4, &entity->animator, true, 0);
        entity->state = BallHog_State_Bomb;
    }
    else {
        entity->startPos = entity->position;
        entity->startDir = entity->direction;
        if (!entity->bombTime)
            entity->bombTime = 6;
        if (!entity->numJumps)
            entity->numJumps = 3;

        entity->drawFX |= FX_FLIP;
        entity->jumpCount = entity->numJumps;
        entity->timer     = 20;
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 0, &entity->animator, true, 0);
        entity->state = BallHog_State_Setup;
    }
}

void BallHog_StageLoad(void)
{
    if (RSDK.CheckStageFolder("TMZ1") || RSDK.CheckStageFolder("TMZ2"))
        BallHog->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/BallHog.bin", SCOPE_STAGE);
    BallHog->hitboxBadnik.left   = -12;
    BallHog->hitboxBadnik.top    = -18;
    BallHog->hitboxBadnik.right  = 12;
    BallHog->hitboxBadnik.bottom = 18;
    BallHog->hitboxBomb.left     = -6;
    BallHog->hitboxBomb.top      = -6;
    BallHog->hitboxBomb.right    = 6;
    BallHog->hitboxBomb.bottom   = 6;

    BallHog->sfxExplosion = RSDK.GetSFX("Stage/Explosion.wav");
    BallHog->sfxArrowHit  = RSDK.GetSFX("PSZ/ArrowHit.wav");
    BallHog->sfxDrop      = RSDK.GetSFX("TMZ1/HogDrop.wav");
    BallHog->sfxJump      = RSDK.GetSFX("TMZ1/HogJump.wav");

    DEBUGMODE_ADD_OBJ(BallHog);
}

void BallHog_DebugSpawn(void)
{
    RSDK_THIS(BallHog);

    CREATE_ENTITY(BallHog, NULL, entity->position.x, entity->position.y);
}

void BallHog_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(BallHog->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void BallHog_CheckPlayerCollisions(void)
{
    RSDK_THIS(BallHog);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &BallHog->hitboxBadnik))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void BallHog_CheckOnScreen(void)
{
    RSDK_THIS(BallHog);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        BallHog_Create(NULL);
    }
}

void BallHog_State_Setup(void)
{
    RSDK_THIS(BallHog);
    entity->active = ACTIVE_NORMAL;
    entity->state  = BallHog_State_Idle;
    BallHog_State_Idle();
}

void BallHog_State_Idle(void)
{
    RSDK_THIS(BallHog);
    if (!--entity->timer) {
        entity->timer = 20;
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 1, &entity->animator, true, 0);
        entity->state = BallHog_State_Jump;
    }
    RSDK.ProcessAnimation(&entity->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_Jump(void)
{
    RSDK_THIS(BallHog);

    if (entity->timer) {
        entity->timer--;
        if (!entity->timer) {
            RSDK.PlaySfx(BallHog->sfxJump, false, 255);
            entity->velocity.y = -0x203D9;
            entity->position.y -= 0x29999;
        }
    }
    else {
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x95C0;

        if (entity->velocity.y > 0 && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, 2)) {
            entity->velocity.y = 0;
            entity->timer      = 10;
            RSDK.SetSpriteAnimation(BallHog->aniFrames, 2, &entity->animator, true, 0);
            entity->state = BallHog_State_Land;
        }
    }
    RSDK.ProcessAnimation(&entity->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_Land(void)
{
    RSDK_THIS(BallHog);

    if (!--entity->timer) {
        RSDK.SetSpriteAnimation(BallHog->aniFrames, 0, &entity->animator, true, 0);

        if (!--entity->jumpCount) {
            entity->timer     = 28;
            entity->jumpCount = entity->numJumps + 1;
            entity->state     = BallHog_State_DropBomb;
        }
        else {
            entity->timer = 20;
            entity->state = BallHog_State_Idle;
        }
    }
    RSDK.ProcessAnimation(&entity->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_DropBomb(void)
{
    RSDK_THIS(BallHog);

    if (--entity->timer) {
        if (entity->timer == 8)
            RSDK.SetSpriteAnimation(BallHog->aniFrames, 3, &entity->animator, true, 0);
    }
    else {
        entity->timer = 18;
        RSDK.PlaySfx(BallHog->sfxDrop, false, 255);

        EntityBallHog *bomb = CREATE_ENTITY(BallHog, intToVoid(true), entity->position.x, entity->position.y);
        if (entity->direction) {
            bomb->position.x += 0x40000;
            bomb->velocity.x = 0x10000;
        }
        else {
            bomb->position.x -= 0x40000;
            bomb->velocity.x = -0x10000;
        }
        bomb->position.y += 0xC0000;
        bomb->timer   = 60 * entity->bombTime;
        entity->state = BallHog_State_Land;
    }
    RSDK.ProcessAnimation(&entity->animator);
    BallHog_CheckPlayerCollisions();
    BallHog_CheckOnScreen();
}

void BallHog_State_Bomb(void)
{
    RSDK_THIS(BallHog);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        if (entity->velocity.y > 0 && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 4)) {
            RSDK.PlaySfx(BallHog->sfxArrowHit, false, 255);
            int32 storeY = entity->position.y;

            entity->velocity.y = -0x30000;
            RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x20000, 0x80000, 8);
            int32 y1 = entity->position.y;

            entity->position.y = storeY;
            RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x20000, 0x80000, 8);
            int32 y2 = entity->position.y;

            entity->position.y = storeY;
            if (entity->velocity.x <= 0 ? y1 < y2 : y2 < y1)
                entity->velocity.x = -entity->velocity.x;
        }
        RSDK.ProcessAnimation(&entity->animator);

        bool32 flag = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &BallHog->hitboxBomb)
#if RETRO_USE_PLUS
                && !Player_CheckMightyUnspin(256, player, 2, &player->uncurlTimer)
#endif
            ) {
                Player_CheckHit(player, entity);
                flag = true;
                foreach_break;
            }
        }

        if (flag || !--entity->timer) {
            RSDK.PlaySfx(BallHog->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            destroyEntity(entity);
        }
    }
    else {
        destroyEntity(entity);
    }
}

void BallHog_EditorDraw(void) {}

void BallHog_EditorLoad(void) {}

void BallHog_Serialize(void)
{
    RSDK_EDITABLE_VAR(BallHog, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BallHog, VAR_UINT8, numJumps);
    RSDK_EDITABLE_VAR(BallHog, VAR_UINT8, bombTime);
}
