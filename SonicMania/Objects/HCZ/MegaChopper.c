// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MegaChopper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMegaChopper *MegaChopper;

void MegaChopper_Update(void)
{
    RSDK_THIS(MegaChopper);

    StateMachine_Run(self->state);
}

void MegaChopper_LateUpdate(void) {}

void MegaChopper_StaticUpdate(void) {}

void MegaChopper_Draw(void)
{
    RSDK_THIS(MegaChopper);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MegaChopper_Create(void *data)
{
    RSDK_THIS(MegaChopper);

    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup     = Zone->playerDrawGroup[0] + 1;
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(MegaChopper->aniFrames, 0, &self->animator, true, 0);
    self->state = MegaChopper_State_Init;
}

void MegaChopper_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HCZ"))
        MegaChopper->aniFrames = RSDK.LoadSpriteAnimation("HCZ/MegaChopper.bin", SCOPE_STAGE);

    MegaChopper->hitboxBadnik.left   = -8;
    MegaChopper->hitboxBadnik.top    = -12;
    MegaChopper->hitboxBadnik.right  = 2;
    MegaChopper->hitboxBadnik.bottom = 12;

    MegaChopper->hitboxChop.left   = -1;
    MegaChopper->hitboxChop.top    = -1;
    MegaChopper->hitboxChop.right  = 1;
    MegaChopper->hitboxChop.bottom = 1;

    DEBUGMODE_ADD_OBJ(MegaChopper);
}

void MegaChopper_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(MegaChopper, NULL, self->position.x, self->position.y);
}

void MegaChopper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(MegaChopper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void MegaChopper_CheckPlayerCollisions(void)
{
    RSDK_THIS(MegaChopper);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &MegaChopper->hitboxBadnik)) {
            int32 blink = player->blinkTimer;

            player->blinkTimer = 1;
            if (!Player_CheckBadnikBreak(player, self, true) && Player_CheckCollisionTouch(player, self, &MegaChopper->hitboxChop)) {
                self->grabbedPlayer = player;
                self->playerPos.x   = (self->position.x - player->position.x) & 0xFFFF0000;
                self->playerPos.y   = (self->position.y - player->position.y) & 0xFFFF0000;
                self->playerDir     = player->direction;

                if (player->stateInput == Player_Input_P1)
                    player->stateInput = MegaChopper_Input_GrabbedP1;
                else if (player->stateInput == Player_Input_P2_Player)
                    player->stateInput = MegaChopper_Input_GrabbedP2;
                else if (player->stateInput == Player_Input_P2_AI)
                    player->stateInput = MegaChopper_Input_GrabbedP2_AI;

                self->drawGroup   = player->drawGroup + 1;
                self->isPermanent = true;
                self->state       = MegaChopper_State_Chopping;
            }
            player->blinkTimer = blink;
            foreach_break;
        }
    }
}

void MegaChopper_CheckOffScreen(void)
{
    RSDK_THIS(MegaChopper);

    if (!RSDK.CheckOnScreen(self, NULL) && RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange) == false) {
        self->position = self->startPos;

        EntityPlayer *player = self->grabbedPlayer;
        if (player) {
            if (player->stateInput == MegaChopper_Input_GrabbedP1)
                player->stateInput = Player_Input_P1;
            else if (player->stateInput == MegaChopper_Input_GrabbedP2)
                player->stateInput = Player_Input_P2_Player;
            else if (player->stateInput == MegaChopper_Input_GrabbedP2_AI)
                player->stateInput = Player_Input_P2_AI;
        }

        self->nibbleTimer = 0;
        self->isPermanent = false;

        MegaChopper_Create(NULL);
    }
}

void MegaChopper_Input_GrabbedP1(void)
{
    RSDK_THIS(Player);

    Player_Input_P1();

    if (self->state != Player_State_Static) {
        self->up        = false;
        self->down      = false;
        self->jumpPress = false;
        self->jumpHold  = false;
    }
}

void MegaChopper_Input_GrabbedP2(void)
{
    RSDK_THIS(Player);

    Player_Input_P2_Player();

    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    if (self->stateInput == Player_Input_P2_AI)
        self->stateInput = MegaChopper_Input_GrabbedP2_AI;
}

void MegaChopper_Input_GrabbedP2_AI(void)
{
    RSDK_THIS(Player);

    Player_Input_P2_AI();

    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    if (self->stateInput == Player_Input_P2_Player)
        self->stateInput = MegaChopper_Input_GrabbedP2;
}

void MegaChopper_State_Init(void)
{
    RSDK_THIS(MegaChopper);

    if (self->position.y >= Water->waterLevel) {
        self->active     = ACTIVE_NORMAL;
        self->velocity.x = -0x10000;

        self->state = MegaChopper_State_InWater;
        MegaChopper_State_InWater();
    }
    else {
        destroyEntity(self);
    }
}

void MegaChopper_State_InWater(void)
{
    RSDK_THIS(MegaChopper);

    if (++self->animator.frameID == 6)
        self->animator.frameID = 0;

    if (self->animator.frameID == 12)
        self->animator.frameID = 6;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (self->position.x >= player->position.x) {
        self->velocity.x -= 0x800;

        if (self->velocity.x < -0x20000)
            self->velocity.x = -0x20000;
    }
    else {
        self->velocity.x += 0x800;

        if (self->velocity.x > 0x20000)
            self->velocity.x = 0x20000;
    }

    self->position.y += self->position.y < player->position.y ? 0x2000 : -0x2000;

    self->direction = self->velocity.x > 0;
    if (self->velocity.y > 0) {
        self->velocity.y -= 0x3800;

        if (self->velocity.y < 0)
            self->velocity.y = 0;
    }

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y < Water->waterLevel) {
        bool32 inWater = false;
        foreach_active(Water, water)
        {
            if (water->type == WATER_POOL && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Water->hitboxPoint)) {
                inWater = true;
            }
        }

        if (!inWater) {
            self->velocity.x = self->direction == FLIP_NONE ? -0x20000 : 0x20000;
            self->velocity.y = -0x40000;
            self->state      = MegaChopper_State_OutOfWater;
        }
    }

    MegaChopper_CheckPlayerCollisions();
    MegaChopper_CheckOffScreen();
}

void MegaChopper_State_OutOfWater(void)
{
    RSDK_THIS(MegaChopper);

    if (++self->animator.frameID == 6)
        self->animator.frameID = 0;

    if (self->animator.frameID == 12)
        self->animator.frameID = 6;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (self->position.y >= Water->waterLevel) {
        self->state = MegaChopper_State_InWater;
    }
    else {
        foreach_active(Water, water)
        {
            if (water->type == WATER_POOL && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Water->hitboxPoint)) {
                self->state = MegaChopper_State_InWater;
            }
        }
    }

    MegaChopper_CheckPlayerCollisions();
    MegaChopper_CheckOffScreen();
}

void MegaChopper_State_Chopping(void)
{
    RSDK_THIS(MegaChopper);

    if (++self->animator.timer == 3) {
        self->animator.timer   = 0;
        self->animator.frameID = (self->animator.frameID + 6) % 12;
    }

    EntityPlayer *player = self->grabbedPlayer;

    if (!player) {
        self->velocity.x = self->direction == FLIP_NONE ? 0x20000 : -0x20000;
        self->velocity.y = -0x40000;
        self->state      = MegaChopper_State_ShakenOff;
    }
    else {
        if (player->animator.animationID == ANI_JUMP) {
            self->grabbedPlayer = NULL;

            if (player->stateInput == MegaChopper_Input_GrabbedP1)
                player->stateInput = Player_Input_P1;
            else if (player->stateInput == MegaChopper_Input_GrabbedP2)
                player->stateInput = Player_Input_P2_Player;
            else if (player->stateInput == MegaChopper_Input_GrabbedP2_AI)
                player->stateInput = Player_Input_P2_AI;

            self->velocity.x = self->direction == FLIP_NONE ? 0x20000 : -0x20000;
            self->velocity.y = -0x40000;
            self->state      = MegaChopper_State_ShakenOff;
        }
        else {
            if (++self->nibbleTimer >= 60) {
                self->nibbleTimer = 0;

                if (!player->rings || player->sidekick) {
                    Player_Hit(player);
                    player->velocity.x  = player->position.x > self->position.x ? 0x20000 : -0x20000;
                    self->grabbedPlayer = NULL;
                }
                else {
                    player->rings--;
                    if (Ring->pan) {
                        int32 channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                        Ring->pan = 0;
                    }
                    else {
                        int32 channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                        Ring->pan = 1;
                    }
                }
            }

            if (self->grabbedPlayer) {
                self->position.x = player->position.x + self->playerPos.x;
                self->position.y = player->position.y + self->playerPos.y;

                if (player->direction != self->playerDir) {
                    self->direction ^= FLIP_X;
                    self->playerDir = player->direction;
                }

                self->playerDir = player->direction;
                if (self->lastShakeFlags) {
                    if (!self->shakeTimer) {
                        self->shakeCount     = 0;
                        self->lastShakeFlags = 0;
                    }
                    else {
                        self->shakeTimer--;

                        uint8 shakeFlags = 0;
                        if (player->left)
                            shakeFlags = 1;
                        if (player->right)
                            shakeFlags |= 2;

                        if (shakeFlags) {
                            if (shakeFlags != 3 && shakeFlags != self->lastShakeFlags) {
                                self->lastShakeFlags = shakeFlags;
                                if (++self->shakeCount >= 6) {
                                    self->grabbedPlayer = NULL;

                                    if (player->stateInput == MegaChopper_Input_GrabbedP1)
                                        player->stateInput = Player_Input_P1;
                                    else if (player->stateInput == MegaChopper_Input_GrabbedP2)
                                        player->stateInput = Player_Input_P2_Player;
                                    else if (player->stateInput == MegaChopper_Input_GrabbedP2_AI)
                                        player->stateInput = Player_Input_P2_AI;

                                    self->velocity.x = self->direction == FLIP_NONE ? 0x20000 : -0x20000;
                                    self->velocity.y = -0x40000;
                                    self->state      = MegaChopper_State_ShakenOff;
                                }
                            }
                        }
                    }
                }
                else if (player->left) {
                    self->lastShakeFlags = 1;
                    self->shakeTimer     = 64;
                }
                else if (player->right) {
                    self->lastShakeFlags = 2;
                    self->shakeTimer     = 64;
                }
            }
            else {
                if (player->stateInput == MegaChopper_Input_GrabbedP1)
                    player->stateInput = Player_Input_P1;
                else if (player->stateInput == MegaChopper_Input_GrabbedP2)
                    player->stateInput = Player_Input_P2_Player;
                else if (player->stateInput == MegaChopper_Input_GrabbedP2_AI)
                    player->stateInput = Player_Input_P2_AI;

                self->velocity.x = self->direction == FLIP_NONE ? 0x20000 : -0x20000;
                self->velocity.y = -0x40000;
                self->state      = MegaChopper_State_ShakenOff;
            }
        }
    }

    MegaChopper_CheckOffScreen();
}

void MegaChopper_State_ShakenOff(void)
{
    RSDK_THIS(MegaChopper);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    MegaChopper_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void MegaChopper_EditorDraw(void) { MegaChopper_Draw(); }

void MegaChopper_EditorLoad(void) { MegaChopper->aniFrames = RSDK.LoadSpriteAnimation("HCZ/MegaChopper.bin", SCOPE_STAGE); }
#endif

void MegaChopper_Serialize(void) {}
