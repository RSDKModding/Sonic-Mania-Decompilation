// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HandLauncher Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHandLauncher *HandLauncher;

void HandLauncher_Update(void)
{
    RSDK_THIS(HandLauncher);

    if (self->handAnimator.animationID == 2 && self->handAnimator.frameID == self->handAnimator.frameCount - 1) // Finished "Hand Grab" Animation
        RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 3, &self->handAnimator, true, 0);
    else if (self->handAnimator.animationID == 4
             && self->handAnimator.frameID == self->handAnimator.frameCount - 1) // Finished "Hand Release" Animation
        RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 1, &self->handAnimator, true, 0);

    StateMachine_Run(self->state);

    if (self->grabDelay > 0)
        self->grabDelay--;

    RSDK.ProcessAnimation(&self->baseAnimator);
    RSDK.ProcessAnimation(&self->handAnimator);
}

void HandLauncher_LateUpdate(void) {}

void HandLauncher_StaticUpdate(void)
{
    foreach_active(HandLauncher, launcher) { RSDK.AddDrawListRef(Zone->playerDrawGroup[1], RSDK.GetEntitySlot(launcher)); }
}

void HandLauncher_Draw(void)
{
    RSDK_THIS(HandLauncher);

    if (SceneInfo->currentDrawGroup == Zone->playerDrawGroup[1]) {
        if (self->state == HandLauncher_State_GrabbedPlayer)
            RSDK.DrawSprite(&self->handAnimator, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->baseAnimator, NULL, false);
        if (self->state != HandLauncher_State_GrabbedPlayer)
            RSDK.DrawSprite(&self->handAnimator, NULL, false);
    }
}

void HandLauncher_Create(void *data)
{
    RSDK_THIS(HandLauncher);

    self->active    = ACTIVE_BOUNDS;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->visible   = true;
    self->drawFX    = FX_FLIP;

    if (!self->speed)
        self->speed = 16;

    self->startPos      = self->position;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = HandLauncher_State_Init;
}

void HandLauncher_StageLoad(void)
{
    HandLauncher->active = ACTIVE_ALWAYS;

    HandLauncher->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HandLauncher.bin", SCOPE_STAGE);

    // only the cheat code was removed from HCZ/HandPulley, the actual code for dunkey mode still remains in-tact
    HandLauncher->sfxDunkey = RSDK.GetSfx("HCZ/Dunkey.wav");

    HandLauncher->hitboxRange.left   = -24;
    HandLauncher->hitboxRange.top    = -240;
    HandLauncher->hitboxRange.right  = 24;
    HandLauncher->hitboxRange.bottom = 0;

    HandLauncher->hitboxGrab.left   = -4;
    HandLauncher->hitboxGrab.top    = -54;
    HandLauncher->hitboxGrab.right  = 4;
    HandLauncher->hitboxGrab.bottom = 0;

    // No idea what ths was for tbh
    HandLauncher->hitboxUnused.left   = -32;
    HandLauncher->hitboxUnused.top    = 0;
    HandLauncher->hitboxUnused.right  = 32;
    HandLauncher->hitboxUnused.bottom = 24;
}

void HandLauncher_CheckPlayerCollisions(void)
{
    RSDK_THIS(HandLauncher);

    if (self->grabDelay <= 0) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if ((self->state == HandLauncher_State_GrabbedPlayer || !player->sidekick) && !((1 << playerID) & self->activePlayers)) {
                if (player->onGround) {
                    if (Player_CheckCollisionTouch(player, self, &HandLauncher->hitboxGrab)) {
                        self->activePlayers |= 1 << playerID;
                        player->groundVel = 0;
                        if (self->state != HandLauncher_State_GrabbedPlayer) {
                            if (HandLauncher->dunkeyMode)
                                RSDK.PlaySfx(HandLauncher->sfxDunkey, false, 0xFF);
                            else
                                RSDK.PlaySfx(Player->sfxRoll, false, 0xFF);
                        }

                        if (!HandLauncher->dunkeyMode)
                            RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);

                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 0);
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                        player->rotation        = 0;
                        player->state           = Player_State_Static;
                    }
                }
            }
        }
    }
}

bool32 HandLauncher_CheckPlayerInRange(void)
{
    RSDK_THIS(HandLauncher);

    if (self->activePlayers)
        return true;

    int32 storeX   = self->position.x;
    int32 storeY   = self->position.y;
    self->position = self->playerPos;

    bool32 inRange = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &HandLauncher->hitboxRange)) {
            inRange = true;
            foreach_break;
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
    return inRange;
}

void HandLauncher_ReleasePlayers(void)
{
    RSDK_THIS(HandLauncher);

    int32 releaseVel = self->speed * (2 * (self->direction == FLIP_NONE) - 1);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (((1 << playerID) & self->activePlayers)) {
            self->activePlayers &= ~(1 << playerID);
            player->onGround  = true;
            player->groundVel = releaseVel << 16;
            player->state     = Player_State_Ground;
        }
    }
}

void HandLauncher_HandleGrabbedPlayers(void)
{
    RSDK_THIS(HandLauncher);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (((1 << playerID) & self->activePlayers)) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->direction  = self->direction != FLIP_NONE;
            player->position.x = self->position.x;
            player->position.y = self->position.y - 0x140000;
            if (self->playerPos.y - 0x140000 < player->position.y)
                player->position.y -= 0x140000;
        }
    }
}

void HandLauncher_State_Init(void)
{
    RSDK_THIS(HandLauncher);

    self->hiddenPos = self->startPos;
    self->hiddenPos.y += 0x4E0000;

    self->playerPos = self->hiddenPos;
    self->playerPos.y -= 0x360000;

    self->position = self->hiddenPos;

    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 0, &self->baseAnimator, true, 0);
    self->baseAnimator.speed = 0;
    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 1, &self->handAnimator, true, 0);

    self->state = HandLauncher_State_AwaitPlayer;
}

void HandLauncher_State_AwaitPlayer(void)
{
    RSDK_THIS(HandLauncher);

    // Out of range, lower "Hand Position" to hide it
    self->position = self->hiddenPos;
    self->position.y -= ((self->hiddenPos.y - self->playerPos.y) >> 3) * MIN(self->timer, 8);

    if (self->timer > 0)
        self->timer--;

    if (self->timer < 0)
        self->timer = 0;

    if (HandLauncher_CheckPlayerInRange())
        self->state = HandLauncher_State_TryGrabPlayer;
}

void HandLauncher_State_TryGrabPlayer(void)
{
    RSDK_THIS(HandLauncher);

    // In Range, raise "Hand Position" to show it
    self->position = self->hiddenPos;
    self->position.y -= ((self->hiddenPos.y - self->playerPos.y) >> 3) * MIN(self->timer, 8);

    if (self->timer < 8)
        self->timer++;

    if (self->timer > 8)
        self->timer = 8;

    HandLauncher_CheckPlayerCollisions();

    if (self->activePlayers) {
        RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 2, &self->handAnimator, true, 0);
        self->timer              = 0;
        self->baseAnimator.speed = 1;
        self->state              = HandLauncher_State_GrabbedPlayer;
    }

    HandLauncher_HandleGrabbedPlayers();

    if (!HandLauncher_CheckPlayerInRange())
        self->state = HandLauncher_State_AwaitPlayer;
}

void HandLauncher_State_GrabbedPlayer(void)
{
    RSDK_THIS(HandLauncher);

    if (self->timer < 3) {
        self->position = self->playerPos;

        int32 dist = (self->playerPos.y - self->startPos.y) / 3;
        if (self->timer <= 3)
            self->position.y -= dist * self->timer;
        else
            self->position.y -= dist * 3;
    }
    else {
        if (self->timer < 63) {
            self->position = self->startPos;
        }
        else if (self->timer < 66) {
            self->position = self->playerPos;
            int32 dist     = (self->playerPos.y - self->startPos.y) / 3;
            self->position.y -= dist * MIN(66 - self->timer, 3);
        }
        else {
            HandLauncher_ReleasePlayers();
            if (!HandLauncher->dunkeyMode)
                RSDK.PlaySfx(Player->sfxRelease, false, 0xFF);

            RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 4, &self->handAnimator, true, 0);
            self->position           = self->playerPos;
            self->grabDelay          = 30;
            self->timer              = 0;
            self->baseAnimator.speed = 0;
            self->state              = HandLauncher_State_AwaitPlayer;
        }
    }

    if (self->state == HandLauncher_State_GrabbedPlayer)
        self->timer++;

    HandLauncher_CheckPlayerCollisions();
    HandLauncher_HandleGrabbedPlayers();
}

#if GAME_INCLUDE_EDITOR
void HandLauncher_EditorDraw(void)
{
    RSDK_THIS(HandLauncher);

    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 0, &self->baseAnimator, true, 0);
    RSDK.SetSpriteAnimation(HandLauncher->aniFrames, 1, &self->handAnimator, true, 0);

    RSDK.DrawSprite(&self->baseAnimator, NULL, false);
    RSDK.DrawSprite(&self->handAnimator, NULL, false);

    if (showGizmos()) {
        self->hiddenPos = self->position;
        self->hiddenPos.y += 0x4E0000;
        self->inkEffect = INK_BLEND;

        RSDK.DrawSprite(&self->baseAnimator, &self->hiddenPos, false);
        RSDK.DrawSprite(&self->handAnimator, &self->hiddenPos, false);

        self->inkEffect = INK_NONE;
    }
}

void HandLauncher_EditorLoad(void)
{
    HandLauncher->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HandLauncher.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HandLauncher, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void HandLauncher_Serialize(void)
{
    RSDK_EDITABLE_VAR(HandLauncher, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(HandLauncher, VAR_UINT32, speed);
}
