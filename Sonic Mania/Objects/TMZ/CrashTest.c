#include "SonicMania.h"

ObjectCrashTest *CrashTest;

void CrashTest_Update(void)
{
    RSDK_THIS(CrashTest);
    StateMachine_Run(self->state);
    CrashTest_Unknown6();
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator1);
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

    drawPos2 = self->startPos;
    drawPos2.x += 0x300000 * (2 * (self->direction != FLIP_NONE) - 1);

    drawPos = self->startPos;
    drawPos.x += self->dword84 * (2 * (self->direction != FLIP_NONE) - 1);

    drawPos3.x = (2 * (self->direction != FLIP_NONE) - 1) * (self->length << 16) + self->startPos.x
                 + 0x340000 * (2 * (self->direction != FLIP_NONE) - 1);
    drawPos3.y = self->startPos.y;
    if (SceneInfo->currentDrawGroup == Zone->playerDrawLow + 1) {
        if (self->state != CrashTest_State_Unknown4)
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&self->animator3, &drawPos2, false);
        RSDK.DrawSprite(&self->animator4, &drawPos3, false);
        if (self->state != CrashTest_State_Unknown4)
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }
}

void CrashTest_Create(void *data)
{
    RSDK_THIS(CrashTest);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = (self->length + 128) << 16;
    self->updateRange.y = 0x800000;
    CrashTest_SetupHitboxes();

    int32 positions[] = { -0x100000, -0x180000, 0x80000,  -0x1A0000, -0x100000, -0x180000, 0x00000,  -0x100000,
                          -0x200000, -0x80000,  0x100000, -0x100000, -0x40000,  -0x180000, -0x40000, -0x80000 };

    int32 pos = 0;
    for (int32 i = 0; i < 8; ++i) {
        self->field_8C[i].x = positions[pos + 0];
        self->field_8C[i].y = positions[pos + 1];

        pos += 2;
    }

    self->state = CrashTest_State_Setup;
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

    self->hitbox1.left             = -22;
    self->hitbox1.top              = -16;
    self->hitbox1.right            = 14;
    self->hitbox1.bottom           = 4;

    self->hitbox2.left             = -40;
    self->hitbox2.top              = -16;
    self->hitbox2.right            = -22;
    self->hitbox2.bottom           = 0;

    self->hitbox3.left             = -22;
    self->hitbox3.top              = -4;
    self->hitbox3.right            = 14;
    self->hitbox3.bottom           = 0;

    self->hitbox4.left             = 14;
    self->hitbox4.top              = -36;
    self->hitbox4.right            = 25;
    self->hitbox4.bottom           = 0;

    self->hitbox5.left             = -12;
    self->hitbox5.top              = -24;
    self->hitbox5.right            = 12;
    self->hitbox5.bottom           = 0;

    self->hitbox6.left             = 28;
    self->hitbox6.top              = -22;
    self->hitbox6.right            = 51;
    self->hitbox6.bottom           = -2;
}

void CrashTest_CheckOnScreen(void)
{
    RSDK_THIS(CrashTest);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->active     = ACTIVE_BOUNDS;
        self->state      = CrashTest_State_Setup;
    }
}

void CrashTest_CheckPlayerCrash(void)
{
    RSDK_THIS(CrashTest);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if ((1 << playerID) & self->activePlayers) {
            self->activePlayers &= ~(1 << playerID);
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
    int32 storeDir = self->direction;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    int32 storeX2 = (2 * (storeDir != FLIP_NONE) - 1) * (self->length << 16) + self->startPos.x + 0x340000 * (2 * (storeDir != FLIP_NONE) - 1);
    int32 storeY2 = self->startPos.y;

    int32 storeX3 = (2 * (storeDir != FLIP_NONE) - 1) + self->startPos.x;
    int32 storeY3 = self->startPos.y;

    foreach_active(Player, player)
    {
        RSDK.GetEntityID(player);
        if (self->state != CrashTest_State_Unknown4) {
            self->position.y = storeY3;
            self->position.x = storeX3;
            self->direction  = storeDir;
            Player_CheckCollisionBox(player, self, &self->hitbox2);
            Player_CheckCollisionBox(player, self, &self->hitbox3);
            Player_CheckCollisionBox(player, self, &self->hitbox4);
            if (self->field_88) {
                if (Player_CheckCollisionTouch(player, self, &self->hitbox6)) {
                    Player_CheckElementalHit(player, self, SHIELD_FIRE);
                }
            }
        }
        self->position.x = storeX2;
        self->position.y = storeY2;
        self->direction  = FLIP_NONE;
        Player_CheckCollisionBox(player, self, &self->hitbox5);
    }
    self->position.x = storeX;
    self->position.y = storeY;
    self->direction  = storeDir;
}

void CrashTest_CheckPlayerRide(void)
{
    RSDK_THIS(CrashTest);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->dword84 * (2 * (self->direction != FLIP_NONE) - 1) + self->startPos.x;
    self->position.y = self->startPos.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (!((1 << playerID) & self->activePlayers) && !self->playerTimers[playerID]) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
                self->activePlayers |= 1 << playerID;
                player->groundVel = 0;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->playerAnimator, false, 4);
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->state           = Player_State_None;
            }
        }
        if (self->playerTimers[playerID] > 0)
            self->playerTimers[playerID]--;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void CrashTest_CheckPlayerJump(void)
{
    RSDK_THIS(CrashTest);

    int32 x = self->startPos.x + (self->dword84 * (2 * (self->direction != FLIP_NONE) - 1));
    int32 y = self->startPos.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if ((1 << playerID) & self->activePlayers) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            if (self->direction == FLIP_NONE)
                player->velocity.x = -self->velocity.x;
            else
                player->velocity.x = self->velocity.x;
            player->direction  = self->direction == FLIP_NONE;
            player->position.x = x;
            player->position.y = y;
            player->position.y -= 0x100000;
            if (player->jumpPress) {
                self->activePlayers &= ~(1 << playerID);
                Player_StartJump(player);
                if (self->direction == FLIP_NONE)
                    player->velocity.x = -self->velocity.x;
                else
                    player->velocity.x = self->velocity.x;
                self->playerTimers[playerID] = 10;
            }
        }
    }
}

void CrashTest_Unknown6(void)
{
    RSDK_THIS(CrashTest);

    if (self->field_CC != self->field_D0) {
        if (!self->field_CC)
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 9, &self->animator3, true, 0);
        else
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 7, &self->animator3, true, 0);
    }
    if (self->animator3.animationSpeed == 1 && self->animator3.frameID == self->animator3.frameCount - 1) {
        if (self->field_CC)
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 8, &self->animator3, false, 0);
        else
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &self->animator3, false, 0);
    }
    self->field_D0 = self->field_CC;
    RSDK.ProcessAnimation(&self->animator3);
}

void CrashTest_State_Setup(void)
{
    RSDK_THIS(CrashTest);

    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 2, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 4, &self->animator4, true, 0);
    self->dword84       = 0;
    self->timer         = 0;
    self->activePlayers = 0;
    self->velocity.x    = 0;
    self->field_88      = false;
    self->field_CC      = false;
    self->field_D0      = 0;
    self->state         = CrashTest_State_Unknown1;
}

void CrashTest_State_Unknown1(void)
{
    RSDK_THIS(CrashTest);

    CrashTest_CheckPlayerCollisions();
    CrashTest_CheckPlayerRide();
    CrashTest_CheckPlayerJump();

    if (self->activePlayers & 1) {
        self->active = ACTIVE_NORMAL;
        self->timer  = 40;
        self->state  = CrashTest_State_Rev;
        RSDK.PlaySfx(CrashTest->sfxTrafficLight, false, 255);
    }
}

void CrashTest_State_Rev(void)
{
    RSDK_THIS(CrashTest);

    if (self->timer <= 0) {
        self->field_88 = true;
        RSDK.SetSpriteAnimation(CrashTest->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(CrashTest->aniFrames, 3, &self->animator2, true, 0);
        self->active   = ACTIVE_NORMAL;
        self->field_CC = true;
        self->state    = CrashTest_State_Move;
        RSDK.PlaySfx(CrashTest->sfxCarRev, false, 255);
    }
    else {
        self->timer--;
    }
}

void CrashTest_State_Move(void)
{
    RSDK_THIS(CrashTest);

    if (self->velocity.x < 0x100000) {
        self->velocity.x += 0x2000;
        if (self->velocity.x > 0x100000)
            self->velocity.x = 0x100000;
    }

    self->dword84 += self->velocity.x;
    bool32 flag = false;
    if (self->dword84 >= self->length << 16) {
        self->dword84 = self->length << 16;
        flag            = true;
    }
    CrashTest_CheckPlayerCollisions();
    CrashTest_CheckPlayerRide();
    CrashTest_CheckPlayerJump();
    CrashTest_CheckOnScreen();

    if (flag) {
        int32 x = self->dword84 * (2 * (self->direction != FLIP_NONE) - 1) + self->startPos.x;

        Camera_ShakeScreen(0, 5, 5);
        CrashTest_CheckPlayerCrash();
        self->timer = 0;
        RSDK.PlaySfx(CrashTest->sfxCrash, false, 255);
        int32 storeX     = self->position.x;
        int32 storeY     = self->position.y;
        self->state    = CrashTest_State_Unknown4;
        self->field_CC = false;

        for (int i = 0; i < 8; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x + self->field_8C[i].x * (2 * (self->direction == FLIP_NONE) - 1),
                                                 self->startPos.y + self->field_8C[i].y);
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 5, &debris->animator, true, i);
            debris->drawFX |= FX_ROTATE;

            if (self->direction == FLIP_NONE)
                debris->rotSpeed = -8;
            else
                debris->rotSpeed = 8;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->gravity    = 0x3800;
            debris->direction  = self->direction;
            debris->velocity.x = (RSDK.Rand(-32, 32) << 12) + (self->velocity.x >> 1) * (2 * (self->direction != FLIP_NONE) - 1);
            debris->velocity.y = -0x1000 * RSDK.Rand(32, 128);
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void CrashTest_State_Unknown4(void)
{
    RSDK_THIS(CrashTest);

    CrashTest_CheckOnScreen();
    CrashTest_CheckPlayerCollisions();
    if (self->timer >= 30) {
        Vector2 range;
        range.x = 0x200000;
        range.y = 0x200000;

        if (!RSDK.CheckPosOnScreen(&self->startPos, &range))
            self->state = CrashTest_State_Setup;
    }
    else {
        int32 startX = self->dword84 * (2 * (self->direction != FLIP_NONE) - 1) + self->startPos.x;

        if (!(Zone->timer % 3)) {
            RSDK.PlaySfx(CrashTest->sfxExplosion, false, 255);

            if (Zone->timer & 4) {
                int32 x                    = startX + (RSDK.Rand(-16, 16) << 16);
                int32 y                    = self->startPos.y + (RSDK.Rand(-22, 0) << 16);
                EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), x, y);
                explosion->drawOrder       = Zone->drawOrderHigh;
            }
        }
        ++self->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void CrashTest_EditorDraw(void)
{
    RSDK_THIS(CrashTest);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 2, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &self->animator3, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 4, &self->animator4, false, 0);

    Vector2 drawPos;
    Vector2 drawPos2;
    Vector2 drawPos3;

    drawPos2 = self->startPos;
    drawPos2.x += 0x300000 * (2 * (self->direction != FLIP_NONE) - 1);

    drawPos = self->startPos;
    drawPos.x += self->dword84 * (2 * (self->direction != FLIP_NONE) - 1);

    drawPos3.x = (2 * (self->direction != FLIP_NONE) - 1) * (self->length << 16) + self->startPos.x
                 + 0x340000 * (2 * (self->direction != FLIP_NONE) - 1);
    drawPos3.y = self->startPos.y;

    RSDK.DrawSprite(&self->animator3, &drawPos2, false);
    RSDK.DrawSprite(&self->animator4, &drawPos3, false);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
}

void CrashTest_EditorLoad(void) { CrashTest->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrashTest.bin", SCOPE_STAGE); }
#endif

void CrashTest_Serialize(void)
{
    RSDK_EDITABLE_VAR(CrashTest, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(CrashTest, VAR_INT32, length);
    RSDK_EDITABLE_VAR(CrashTest, VAR_BOOL, debugParticles);
}
