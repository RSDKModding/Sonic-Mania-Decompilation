// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CrashTest Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCrashTest *CrashTest;

void CrashTest_Update(void)
{
    RSDK_THIS(CrashTest);

    StateMachine_Run(self->state);

    CrashTest_HandleLightAnims();

    RSDK.ProcessAnimation(&self->driverAnimator);
    RSDK.ProcessAnimation(&self->carAnimator);
}

void CrashTest_LateUpdate(void) {}

void CrashTest_StaticUpdate(void)
{
    foreach_active(CrashTest, crashtest) { RSDK.AddDrawListRef(Zone->playerDrawGroup[0] + 1, RSDK.GetEntitySlot(crashtest)); }
}

void CrashTest_Draw(void)
{
    RSDK_THIS(CrashTest);

    Vector2 lightPos = self->startPos;
    lightPos.x += 0x300000 * (2 * (self->direction != FLIP_NONE) - 1);

    Vector2 carPos = self->startPos;
    carPos.x += self->travelDistance * (2 * (self->direction != FLIP_NONE) - 1);

    Vector2 stopperPos;
    stopperPos.x =
        (2 * (self->direction != FLIP_NONE) - 1) * (self->length << 16) + self->startPos.x + 0x340000 * (2 * (self->direction != FLIP_NONE) - 1);
    stopperPos.y = self->startPos.y;

    if (SceneInfo->currentDrawGroup == Zone->playerDrawGroup[0] + 1) {
        if (self->state != CrashTest_State_Crashed)
            RSDK.DrawSprite(&self->carAnimator, &carPos, false);
    }
    else {
        RSDK.DrawSprite(&self->lightAnimator, &lightPos, false);
        RSDK.DrawSprite(&self->stopperAnimator, &stopperPos, false);

        if (self->state != CrashTest_State_Crashed)
            RSDK.DrawSprite(&self->driverAnimator, &carPos, false);
    }
}

void CrashTest_Create(void *data)
{
    RSDK_THIS(CrashTest);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = (self->length + 128) << 16;
    self->updateRange.y = 0x800000;

    CrashTest_SetupHitboxes();

    Vector2 positions[] = { { -0x100000, -0x180000 }, { 0x80000, -0x1A0000 },  { -0x100000, -0x180000 }, { 0x00000, -0x100000 },
                            { -0x200000, -0x80000 },  { 0x100000, -0x100000 }, { -0x40000, -0x180000 },  { -0x40000, -0x80000 } };

    for (int32 i = 0; i < 8; ++i) {
        self->debrisOffsets[i].x = positions[i].x;
        self->debrisOffsets[i].y = positions[i].y;
    }

    self->state = CrashTest_State_Init;
}

void CrashTest_StageLoad(void)
{
    CrashTest->active = ACTIVE_ALWAYS;

    CrashTest->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrashTest.bin", SCOPE_STAGE);

    CrashTest->sfxExplosion    = RSDK.GetSfx("Stage/Explosion2.wav");
    CrashTest->sfxCrash        = RSDK.GetSfx("TMZ1/Crash.wav");
    CrashTest->sfxTrafficLight = RSDK.GetSfx("TMZ1/TrafficLight.wav");
    CrashTest->sfxCarRev       = RSDK.GetSfx("TMZ1/CarRev.wav");
    CrashTest->sfxSpeedBooster = RSDK.GetSfx("Stage/SpeedBooster.wav");
}

void CrashTest_SetupHitboxes(void)
{
    RSDK_THIS(CrashTest);

    self->hitboxSeat.left   = -22;
    self->hitboxSeat.top    = -16;
    self->hitboxSeat.right  = 14;
    self->hitboxSeat.bottom = 4;

    self->hitboxFront.left   = -40;
    self->hitboxFront.top    = -16;
    self->hitboxFront.right  = -22;
    self->hitboxFront.bottom = 0;

    self->hitboxFloor.left   = -22;
    self->hitboxFloor.top    = -4;
    self->hitboxFloor.right  = 14;
    self->hitboxFloor.bottom = 0;

    self->hitboxBack.left   = 14;
    self->hitboxBack.top    = -36;
    self->hitboxBack.right  = 25;
    self->hitboxBack.bottom = 0;

    self->hitboxStopper.left   = -12;
    self->hitboxStopper.top    = -24;
    self->hitboxStopper.right  = 12;
    self->hitboxStopper.bottom = 0;

    self->hitboxBooster.left   = 28;
    self->hitboxBooster.top    = -22;
    self->hitboxBooster.right  = 51;
    self->hitboxBooster.bottom = -2;
}

void CrashTest_CheckOffScreen(void)
{
    RSDK_THIS(CrashTest);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position = self->startPos;
        self->active   = ACTIVE_BOUNDS;
        self->state    = CrashTest_State_Init;
    }
}

void CrashTest_HandlePlayerCrash(void)
{
    RSDK_THIS(CrashTest);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if ((1 << playerID) & self->activePlayers) {
            self->activePlayers &= ~(1 << playerID);
            bool32 isSidekick = player->sidekick;

            player->sidekick = true;
            Player_Hit(player);

            player->sidekick = isSidekick;
        }
    }
}

void CrashTest_CheckPlayerCollisionsSolid(void)
{
    RSDK_THIS(CrashTest);

    int32 storeDir = self->direction;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    int32 stopperX = (2 * (storeDir != FLIP_NONE) - 1) * (self->length << 16) + self->startPos.x + 0x340000 * (2 * (storeDir != FLIP_NONE) - 1);
    int32 stopperY = self->startPos.y;

    int32 carX = (2 * (storeDir != FLIP_NONE) - 1) + self->startPos.x;
    int32 carY = self->startPos.y;

    foreach_active(Player, player)
    {
        RSDK.GetEntitySlot(player);
        if (self->state != CrashTest_State_Crashed) {
            self->position.y = carY;
            self->position.x = carX;
            self->direction  = storeDir;

            Player_CheckCollisionBox(player, self, &self->hitboxFront);
            Player_CheckCollisionBox(player, self, &self->hitboxFloor);
            Player_CheckCollisionBox(player, self, &self->hitboxBack);

            if (self->boosterActive) {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxBooster)) {
                    Player_ElementHurt(player, self, SHIELD_FIRE);
                }
            }
        }

        self->position.x = stopperX;
        self->position.y = stopperY;
        self->direction  = FLIP_NONE;
        Player_CheckCollisionBox(player, self, &self->hitboxStopper);
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

    self->position.x = self->travelDistance * (2 * (self->direction != FLIP_NONE) - 1) + self->startPos.x;
    self->position.y = self->startPos.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (!((1 << playerID) & self->activePlayers) && !self->playerTimers[playerID]) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxSeat)) {
                self->activePlayers |= 1 << playerID;
                player->groundVel = 0;

                RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 4);

                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->state           = Player_State_Static;
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

    int32 x = self->startPos.x + (self->travelDistance * (2 * (self->direction != FLIP_NONE) - 1));
    int32 y = self->startPos.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((1 << playerID) & self->activePlayers) {
            player->velocity.x = 0;
            player->velocity.y = 0;

            player->velocity.x = self->direction == FLIP_NONE ? -self->velocity.x : self->velocity.x;
            player->direction  = self->direction == FLIP_NONE;

            player->position.x = x;
            player->position.y = y;
            player->position.y -= 0x100000;

            if (player->jumpPress) {
                self->activePlayers &= ~(1 << playerID);
                Player_Action_Jump(player);
                player->velocity.x           = self->direction == FLIP_NONE ? -self->velocity.x : self->velocity.x;
                self->playerTimers[playerID] = 10;
            }
        }
    }
}

void CrashTest_HandleLightAnims(void)
{
    RSDK_THIS(CrashTest);

    if (self->turningGreen != self->prevTurningGreen) {
        if (self->turningGreen)
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 7, &self->lightAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 9, &self->lightAnimator, true, 0);
    }

    // finished "Turning [clr]" Animation
    if (self->lightAnimator.speed == 1 && self->lightAnimator.frameID == self->lightAnimator.frameCount - 1) {
        if (self->turningGreen)
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 8, &self->lightAnimator, false, 0);
        else
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &self->lightAnimator, false, 0);
    }

    self->prevTurningGreen = self->turningGreen;

    RSDK.ProcessAnimation(&self->lightAnimator);
}

void CrashTest_State_Init(void)
{
    RSDK_THIS(CrashTest);

    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 0, &self->carAnimator, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 2, &self->driverAnimator, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &self->lightAnimator, true, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 4, &self->stopperAnimator, true, 0);

    self->travelDistance   = 0;
    self->timer            = 0;
    self->activePlayers    = 0;
    self->velocity.x       = 0;
    self->boosterActive    = false;
    self->turningGreen     = false;
    self->prevTurningGreen = false;

    self->state = CrashTest_State_AwaitPlayer;
}

void CrashTest_State_AwaitPlayer(void)
{
    RSDK_THIS(CrashTest);

    CrashTest_CheckPlayerCollisionsSolid();
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
        self->boosterActive = true;

        RSDK.SetSpriteAnimation(CrashTest->aniFrames, 1, &self->carAnimator, true, 0);
        RSDK.SetSpriteAnimation(CrashTest->aniFrames, 3, &self->driverAnimator, true, 0);

        self->active       = ACTIVE_NORMAL;
        self->turningGreen = true;
        self->state        = CrashTest_State_Move;
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

    self->travelDistance += self->velocity.x;

    bool32 crashed = false;
    if (self->travelDistance >= self->length << 16) {
        self->travelDistance = self->length << 16;
        crashed              = true;
    }

    CrashTest_CheckPlayerCollisionsSolid();
    CrashTest_CheckPlayerRide();
    CrashTest_CheckPlayerJump();
    CrashTest_CheckOffScreen();

    if (crashed) {
        int32 x = self->travelDistance * (2 * (self->direction != FLIP_NONE) - 1) + self->startPos.x;

        Camera_ShakeScreen(0, 5, 5);
        CrashTest_HandlePlayerCrash();

        self->timer = 0;
        RSDK.PlaySfx(CrashTest->sfxCrash, false, 255);

        int32 storeX = self->position.x;
        int32 storeY = self->position.y;

        self->state        = CrashTest_State_Crashed;
        self->turningGreen = false;

        for (int32 i = 0; i < 8; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x + self->debrisOffsets[i].x * (2 * (self->direction == FLIP_NONE) - 1),
                                                 self->startPos.y + self->debrisOffsets[i].y);
            RSDK.SetSpriteAnimation(CrashTest->aniFrames, 5, &debris->animator, true, i);
            debris->drawFX |= FX_ROTATE;

            debris->rotSpeed        = self->direction == FLIP_NONE ? -8 : 8;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->gravityStrength = 0x3800;
            debris->direction       = self->direction;
            debris->velocity.x      = (RSDK.Rand(-32, 32) << 12) + (self->velocity.x >> 1) * (2 * (self->direction != FLIP_NONE) - 1);
            debris->velocity.y      = -0x1000 * RSDK.Rand(32, 128);
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
}

void CrashTest_State_Crashed(void)
{
    RSDK_THIS(CrashTest);

    CrashTest_CheckOffScreen();
    CrashTest_CheckPlayerCollisionsSolid();

    if (self->timer >= 30) {
        Vector2 range = { 0x200000, 0x200000 };

        if (!RSDK.CheckPosOnScreen(&self->startPos, &range))
            self->state = CrashTest_State_Init;
    }
    else {
        int32 startX = self->travelDistance * (2 * (self->direction != FLIP_NONE) - 1) + self->startPos.x;

        if (!(Zone->timer % 3)) {
            RSDK.PlaySfx(CrashTest->sfxExplosion, false, 255);

            if (Zone->timer & 4) {
                int32 x                    = startX + (RSDK.Rand(-16, 16) << 16);
                int32 y                    = self->startPos.y + (RSDK.Rand(-22, 0) << 16);
                EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + 2), x, y);
                explosion->drawGroup       = Zone->objectDrawGroup[1];
            }
        }

        ++self->timer;
    }
}

#if GAME_INCLUDE_EDITOR
void CrashTest_EditorDraw(void)
{
    RSDK_THIS(CrashTest);

    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 0, &self->carAnimator, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 2, &self->driverAnimator, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 6, &self->lightAnimator, false, 0);
    RSDK.SetSpriteAnimation(CrashTest->aniFrames, 4, &self->stopperAnimator, false, 0);

    Vector2 lightPos = self->startPos;
    lightPos.x += 0x300000 * (2 * (self->direction != FLIP_NONE) - 1);

    Vector2 carPos = self->startPos;
    carPos.x += self->travelDistance * (2 * (self->direction != FLIP_NONE) - 1);

    Vector2 stopperPos;
    stopperPos.x =
        (2 * (self->direction != FLIP_NONE) - 1) * (self->length << 16) + self->startPos.x + 0x340000 * (2 * (self->direction != FLIP_NONE) - 1);
    stopperPos.y = self->startPos.y;

    RSDK.DrawSprite(&self->lightAnimator, &lightPos, false);
    RSDK.DrawSprite(&self->stopperAnimator, &stopperPos, false);
    RSDK.DrawSprite(&self->driverAnimator, &carPos, false);
    RSDK.DrawSprite(&self->carAnimator, &carPos, false);
}

void CrashTest_EditorLoad(void)
{
    CrashTest->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/CrashTest.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(CrashTest, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);

    // dunno what "debugParticles" is, maybe a way for the devs to test the "particles" of this object in editor?
}
#endif

void CrashTest_Serialize(void)
{
    RSDK_EDITABLE_VAR(CrashTest, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(CrashTest, VAR_INT32, length);
    RSDK_EDITABLE_VAR(CrashTest, VAR_BOOL, debugParticles);
}
