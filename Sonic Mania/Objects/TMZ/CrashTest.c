#include "SonicMania.h"

ObjectCrashTest *CrashTest;

void CrashTest_Update(void)
{
    RSDK_THIS(CrashTest);
    StateMachine_Run(entity->state);
    CrashTest_Unknown6();
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator1);
}

void CrashTest_LateUpdate(void) {}

void CrashTest_StaticUpdate(void)
{
    foreach_active(CrashTest, crashtest) { RSDK.AddDrawListRef(Zone->playerDrawLow + 1, RSDK.GetEntityID(crashtest)); }
}

void CrashTest_Draw(void)
{
    RSDK_THIS(CrashTest);
    Vector2 drawPos;
    Vector2 drawPos2;
    Vector2 drawPos3;

    drawPos2 = entity->startPos;
    drawPos2.x += 0x300000 * (2 * (entity->direction != FLIP_NONE) - 1);

    drawPos = entity->startPos;
    drawPos.x += entity->dword84 * (2 * (entity->direction != FLIP_NONE) - 1);

    drawPos3.x = (2 * (entity->direction != FLIP_NONE) - 1) * (entity->length << 16) + entity->startPos.x
                 + 0x340000 * (2 * (entity->direction != FLIP_NONE) - 1);
    drawPos3.y = entity->startPos.y;
    if (RSDK_sceneInfo->currentDrawGroup == Zone->playerDrawLow + 1) {
        if (entity->state != CrashTest_State_Unknown4)
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator3, &drawPos2, false);
        RSDK.DrawSprite(&entity->animator4, &drawPos3, false);
        if (entity->state != CrashTest_State_Unknown4)
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }
}

void CrashTest_Create(void *data)
{
    RSDK_THIS(CrashTest);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = (entity->length + 128) << 16;
    entity->updateRange.y = 0x800000;
    CrashTest_SetupHitboxes();

    int32 positions[] = { -0x100000, -0x180000, 0x80000,  -0x1A0000, -0x100000, -0x180000, 0x00000,  -0x100000,
                          -0x200000, -0x80000,  0x100000, -0x100000, -0x40000,  -0x180000, -0x40000, -0x80000 };

    int32 pos = 0;
    for (int32 i = 0; i < 8; ++i) {
        entity->field_8C[i].x = positions[pos + 0];
        entity->field_8C[i].y = positions[pos + 1];

        pos += 2;
    }

    entity->state = CrashTest_State_Setup;
}

void CrashTest_StageLoad(void)
{
    CrashTest->active = ACTIVE_ALWAYS;

    CrashTest->aniFrames       = RSDK.LoadSpriteAnimation("TMZ1/CrashTest.bin", SCOPE_STAGE);
    CrashTest->sfxExplosion    = RSDK.GetSFX("Stage/Explosion2.wav");
    CrashTest->sfxCrash        = RSDK.GetSFX("TMZ1/Crash.wav");
    CrashTest->sfxTrafficLight = RSDK.GetSFX("TMZ1/TrafficLight.wav");
    CrashTest->sfxCarRev       = RSDK.GetSFX("TMZ1/CarRev.wav");
    CrashTest->sfxSpeedBooster = RSDK.GetSFX("Stage/SpeedBooster.wav");
}

void CrashTest_SetupHitboxes(void)
{
    RSDK_THIS(CrashTest);

    entity->hitbox1.left             = -22;
    entity->hitbox1.top              = -16;
    entity->hitbox1.right            = 14;
    entity->hitbox1.bottom           = 4;

    entity->hitbox2.left             = -40;
    entity->hitbox2.top              = -16;
    entity->hitbox2.right            = -22;
    entity->hitbox2.bottom           = 0;

    entity->hitbox3.left             = -22;
    entity->hitbox3.top              = -4;
    entity->hitbox3.right            = 14;
    entity->hitbox3.bottom           = 0;

    entity->hitbox4.left             = 14;
    entity->hitbox4.top              = -36;
    entity->hitbox4.right            = 25;
    entity->hitbox4.bottom           = 0;

    entity->hitbox5.left             = -12;
    entity->hitbox5.top              = -24;
    entity->hitbox5.right            = 12;
    entity->hitbox5.bottom           = 0;

    entity->hitbox6.left             = 28;
    entity->hitbox6.top              = -22;
    entity->hitbox6.right            = 51;
    entity->hitbox6.bottom           = -2;
}

void CrashTest_CheckOnScreen(void)
{
    RSDK_THIS(CrashTest);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->active     = ACTIVE_BOUNDS;
        entity->state      = CrashTest_State_Setup;
    }
}

void CrashTest_CheckPlayerCrash(void)
{
    RSDK_THIS(CrashTest);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if ((1 << playerID) & entity->activePlayers) {
            entity->activePlayers &= ~(1 << playerID);
            bool32 isSidekick = player->sidekick;
            player->sidekick  = true;
            Player_Hit(player);
            player->sidekick = isSidekick;
        }
    }
}

void CrashTest_CheckPlayerCollisions(void)
{
    RSDK_THIS(CrashTest);
    int32 storeDir = entity->direction;

    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;

    int32 storeX2 = (2 * (storeDir != FLIP_NONE) - 1) * (entity->length << 16) + entity->startPos.x + 0x340000 * (2 * (storeDir != FLIP_NONE) - 1);
    int32 storeY2 = entity->startPos.y;

    int32 storeX3 = (2 * (storeDir != FLIP_NONE) - 1) + entity->startPos.x;
    int32 storeY3 = entity->startPos.y;

    foreach_active(Player, player)
    {
        RSDK.GetEntityID(player);
        if (entity->state != CrashTest_State_Unknown4) {
            entity->position.y = storeY3;
            entity->position.x = storeX3;
            entity->direction  = storeDir;
            Player_CheckCollisionBox(player, entity, &entity->hitbox2);
            Player_CheckCollisionBox(player, entity, &entity->hitbox3);
            Player_CheckCollisionBox(player, entity, &entity->hitbox4);
            if (entity->field_88) {
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox6)) {
                    Player_CheckElementalHit(player, entity, SHIELD_FIRE);
                }
            }
        }
        entity->position.x = storeX2;
        entity->position.y = storeY2;
        entity->direction  = FLIP_NONE;
        Player_CheckCollisionBox(player, entity, &entity->hitbox5);
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
    entity->direction  = storeDir;
}

void CrashTest_CheckPlayerRide(void)
{
    RSDK_THIS(CrashTest);

    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;

    entity->position.x = entity->dword84 * (2 * (entity->direction != FLIP_NONE) - 1) + entity->startPos.x;
    entity->position.y = entity->startPos.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (!((1 << playerID) & entity->activePlayers) && !entity->playerTimers[playerID]) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                entity->activePlayers |= 1 << playerID;
                player->groundVel = 0;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->playerAnimator, false, 4);
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->state           = Player_State_None;
            }
        }
        if (entity->playerTimers[playerID] > 0)
            entity->playerTimers[playerID]--;
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void CrashTest_CheckPlayerJump(void)
{
    RSDK_THIS(CrashTest);

    int32 x = entity->startPos.x + (entity->dword84 * (2 * (entity->direction != FLIP_NONE) - 1));
    int32 y = entity->startPos.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if ((1 << playerID) & entity->activePlayers) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            if (entity->direction == FLIP_NONE)
                player->velocity.x = -entity->velocity.x;
            else
                player->velocity.x = entity->velocity.x;
            player->direction  = entity->direction == FLIP_NONE;
            player->position.x = x;
            player->position.y = y;
            player->position.y -= 0x100000;
            if (player->jumpPress) {
                entity->activePlayers &= ~(1 << playerID);
                Player_StartJump(player);
                if (entity->direction == FLIP_NONE)
                    player->velocity.x = -entity->velocity.x;
                else
                    player->velocity.x = entity->velocity.x;
                entity->playerTimers[playerID] = 10;
            }
        }
    }
}

void CrashTest_Unknown6(void)
{
    RSDK_THIS(CrashTest);

    if (entity->field_CC != entity->field_D0) {
        if (!entity->field_CC)
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 9, &entity->animator3, true, 0);
        else
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 7, &entity->animator3, true, 0);
    }
    if (entity->animator3.animationSpeed == 1 && entity->animator3.frameID == entity->animator3.frameCount - 1) {
        if (entity->field_CC)
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 8, &entity->animator3, false, 0);
        else
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &entity->animator3, false, 0);
    }
    entity->field_D0 = entity->field_CC;
    RSDK.ProcessAnimation(&entity->animator3);
}

void CrashTest_State_Setup(void)
{
    RSDK_THIS(CrashTest);

    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 2, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 4, &entity->animator4, true, 0);
    entity->dword84       = 0;
    entity->timer         = 0;
    entity->activePlayers = 0;
    entity->velocity.x    = 0;
    entity->field_88      = false;
    entity->field_CC      = false;
    entity->field_D0      = 0;
    entity->state         = CrashTest_State_Unknown1;
}

void CrashTest_State_Unknown1(void)
{
    RSDK_THIS(CrashTest);

    CrashTest_CheckPlayerCollisions();
    CrashTest_CheckPlayerRide();
    CrashTest_CheckPlayerJump();

    if (entity->activePlayers & 1) {
        entity->active = ACTIVE_NORMAL;
        entity->timer  = 40;
        entity->state  = CrashTest_State_Rev;
        RSDK.PlaySfx(CrashTest->sfxTrafficLight, false, 255);
    }
}

void CrashTest_State_Rev(void)
{
    RSDK_THIS(CrashTest);

    if (entity->timer <= 0) {
        entity->field_88 = true;
        RSDK.SetSpriteAnimation(CrashTest->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(CrashTest->aniFrames, 3, &entity->animator2, true, 0);
        entity->active   = ACTIVE_NORMAL;
        entity->field_CC = true;
        entity->state    = CrashTest_State_Move;
        RSDK.PlaySfx(CrashTest->sfxCarRev, false, 255);
    }
    else {
        entity->timer--;
    }
}

void CrashTest_State_Move(void)
{
    RSDK_THIS(CrashTest);

    if (entity->velocity.x < 0x100000) {
        entity->velocity.x += 0x2000;
        if (entity->velocity.x > 0x100000)
            entity->velocity.x = 0x100000;
    }

    entity->dword84 += entity->velocity.x;
    bool32 flag = false;
    if (entity->dword84 >= entity->length << 16) {
        entity->dword84 = entity->length << 16;
        flag            = true;
    }
    CrashTest_CheckPlayerCollisions();
    CrashTest_CheckPlayerRide();
    CrashTest_CheckPlayerJump();
    CrashTest_CheckOnScreen();

    if (flag) {
        int32 x = entity->dword84 * (2 * (entity->direction != FLIP_NONE) - 1) + entity->startPos.x;

        Camera_ShakeScreen(0, 5, 5);
        CrashTest_CheckPlayerCrash();
        entity->timer = 0;
        RSDK.PlaySfx(CrashTest->sfxCrash, false, 255);
        int32 storeX     = entity->position.x;
        int32 storeY     = entity->position.y;
        entity->state    = CrashTest_State_Unknown4;
        entity->field_CC = false;

        for (int i = 0; i < 8; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x + entity->field_8C[i].x * (2 * (entity->direction == FLIP_NONE) - 1),
                                                 entity->startPos.y + entity->field_8C[i].y);
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 5, &debris->animator, true, i);
            debris->drawFX |= FX_ROTATE;

            if (entity->direction == FLIP_NONE)
                debris->rotSpeed = -8;
            else
                debris->rotSpeed = 8;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->gravity    = 0x3800;
            debris->direction  = entity->direction;
            debris->velocity.x = (RSDK.Rand(-32, 32) << 12) + (entity->velocity.x >> 1) * (2 * (entity->direction != FLIP_NONE) - 1);
            debris->velocity.y = -0x1000 * RSDK.Rand(32, 128);
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
    }
}

void CrashTest_State_Unknown4(void)
{
    RSDK_THIS(CrashTest);

    CrashTest_CheckOnScreen();
    CrashTest_CheckPlayerCollisions();
    if (entity->timer >= 30) {
        Vector2 range;
        range.x = 0x200000;
        range.y = 0x200000;

        if (!RSDK.CheckPosOnScreen(&entity->startPos, &range))
            entity->state = CrashTest_State_Setup;
    }
    else {
        int32 startX = entity->dword84 * (2 * (entity->direction != FLIP_NONE) - 1) + entity->startPos.x;

        if (!(Zone->timer % 3)) {
            RSDK.PlaySfx(CrashTest->sfxExplosion, false, 255);

            if (Zone->timer & 4) {
                int32 x                    = startX + (RSDK.Rand(-16, 16) << 16);
                int32 y                    = entity->startPos.y + (RSDK.Rand(-22, 0) << 16);
                EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), x, y);
                explosion->drawOrder       = Zone->drawOrderHigh;
            }
        }
        ++entity->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void CrashTest_EditorDraw(void)
{
    RSDK_THIS(CrashTest);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 2, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &entity->animator3, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 4, &entity->animator4, false, 0);

    Vector2 drawPos;
    Vector2 drawPos2;
    Vector2 drawPos3;

    drawPos2 = entity->startPos;
    drawPos2.x += 0x300000 * (2 * (entity->direction != FLIP_NONE) - 1);

    drawPos = entity->startPos;
    drawPos.x += entity->dword84 * (2 * (entity->direction != FLIP_NONE) - 1);

    drawPos3.x = (2 * (entity->direction != FLIP_NONE) - 1) * (entity->length << 16) + entity->startPos.x
                 + 0x340000 * (2 * (entity->direction != FLIP_NONE) - 1);
    drawPos3.y = entity->startPos.y;

    RSDK.DrawSprite(&entity->animator3, &drawPos2, false);
    RSDK.DrawSprite(&entity->animator4, &drawPos3, false);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
}

void CrashTest_EditorLoad(void) { CrashTest->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrashTest.bin", SCOPE_STAGE); }
#endif

void CrashTest_Serialize(void)
{
    RSDK_EDITABLE_VAR(CrashTest, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(CrashTest, VAR_INT32, length);
    RSDK_EDITABLE_VAR(CrashTest, VAR_BOOL, debugParticles);
}
