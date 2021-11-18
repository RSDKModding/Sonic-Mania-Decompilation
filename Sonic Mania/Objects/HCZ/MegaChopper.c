#include "SonicMania.h"

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
    self->drawOrder     = Zone->playerDrawLow + 1;
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(MegaChopper->aniFrames, 0, &self->animator, true, 0);
    self->state = MegaChopper_State_Setup;
}

void MegaChopper_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        MegaChopper->aniFrames = RSDK.LoadSpriteAnimation("HCZ/MegaChopper.bin", SCOPE_STAGE);
    MegaChopper->hitbox1.left   = -8;
    MegaChopper->hitbox1.top    = -12;
    MegaChopper->hitbox1.right  = 2;
    MegaChopper->hitbox1.bottom = 12;
    MegaChopper->hitbox2.left   = -1;
    MegaChopper->hitbox2.top    = -1;
    MegaChopper->hitbox2.right  = 1;
    MegaChopper->hitbox2.bottom = 1;
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
        if (Player_CheckBadnikTouch(player, self, &MegaChopper->hitbox1)) {
            int blink          = player->blinkTimer;
            player->blinkTimer = 1;
            if (!Player_CheckBadnikBreak(self, player, true) && Player_CheckCollisionTouch(player, self, &MegaChopper->hitbox2)) {
                self->playerPtr   = (Entity *)player;
                self->playerPos.x = (self->position.x - player->position.x) & 0xFFFF0000;
                self->playerPos.y = (self->position.y - player->position.y) & 0xFFFF0000;
                self->playerDir   = player->direction;

                if (player->stateInput == Player_ProcessP1Input) {
                    player->stateInput = MegaChopper_PlayerInput_StateP1;
                }
                else if (player->stateInput == Player_ProcessP2Input_Player) {
                    player->stateInput = MegaChopper_PlayerInput_StateP2;
                }
                else if (player->stateInput == Player_ProcessP2Input_AI) {
                    player->stateInput = MegaChopper_PlayerInput_StateP2_AI;
                }
                self->drawOrder   = player->drawOrder + 1;
                self->isPermanent = true;
                self->state       = MegaChopper_State_Unknown3;
            }
            player->blinkTimer = blink;
            foreach_break;
        }
    }
}

void MegaChopper_CheckOnScreen(void)
{
    RSDK_THIS(MegaChopper);
    if (!RSDK.CheckOnScreen(self, NULL) && RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange) == false) {
        self->position.x   = self->startPos.x;
        self->position.y   = self->startPos.y;
        EntityPlayer *player = (EntityPlayer *)self->playerPtr;
        if (player) {
            if (player->stateInput == MegaChopper_PlayerInput_StateP1) {
                player->stateInput = Player_ProcessP1Input;
            }
            else if (player->stateInput == MegaChopper_PlayerInput_StateP2) {
                player->stateInput = Player_ProcessP2Input_Player;
            }
            else if (player->stateInput == MegaChopper_PlayerInput_StateP2_AI) {
                player->stateInput = Player_ProcessP2Input_AI;
            }
        }
        self->nibbleTimer = 0;
        self->isPermanent = false;
        MegaChopper_Create(NULL);
    }
}

void MegaChopper_PlayerInput_StateP1(void)
{
    RSDK_THIS(Player);
    Player_ProcessP1Input();
    if (self->state != Player_State_None) {
        self->up        = false;
        self->down      = false;
        self->jumpPress = false;
        self->jumpHold  = false;
    }
}

void MegaChopper_PlayerInput_StateP2(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_Player();
    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;
    if (self->stateInput == Player_ProcessP2Input_AI)
        self->stateInput = MegaChopper_PlayerInput_StateP2_AI;
}

void MegaChopper_PlayerInput_StateP2_AI(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_AI();
    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;
    if (self->stateInput == Player_ProcessP2Input_Player)
        self->stateInput = MegaChopper_PlayerInput_StateP2;
}

void MegaChopper_State_Setup(void)
{
    RSDK_THIS(MegaChopper);
    if (self->position.y >= Water->waterLevel) {
        self->active     = ACTIVE_NORMAL;
        self->velocity.x = -0x10000;
        self->state      = MegaChopper_State_Unknown1;
        MegaChopper_State_Unknown1();
    }
    else {
        destroyEntity(self);
    }
}

void MegaChopper_State_Unknown1(void)
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

    if (self->position.y >= player->position.y)
        self->position.y -= 0x2000;
    else
        self->position.y += 0x2000;

    self->direction = self->velocity.x > 0;
    if (self->velocity.y > 0) {
        self->velocity.y -= 0x3800;
        if (self->velocity.y < 0)
            self->velocity.y = 0;
    }
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (self->position.y < Water->waterLevel) {

        bool32 flag = false;
        foreach_active(Water, water)
        {
            if (water->type == WATER_RECT && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Water->hitbox)) {
                flag = true;
            }
        }

        if (!flag) {
            self->velocity.y = -0x40000;
            if (self->direction == FLIP_NONE)
                self->velocity.x = -0x20000;
            else
                self->velocity.x = 0x20000;
            self->velocity.x = 0x20000;
            self->state      = MegaChopper_State_Unknown2;
        }
    }
    MegaChopper_CheckPlayerCollisions();
    MegaChopper_CheckOnScreen();
}

void MegaChopper_State_Unknown2(void)
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
        self->state = MegaChopper_State_Unknown1;
    }
    else {
        foreach_active(Water, water)
        {
            if (water->type == WATER_RECT && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Water->hitbox)) {
                self->state = MegaChopper_State_Unknown1;
            }
        }
    }
    MegaChopper_CheckPlayerCollisions();
    MegaChopper_CheckOnScreen();
}

void MegaChopper_State_Unknown3(void)
{
    RSDK_THIS(MegaChopper);
    if (++self->animator.animationTimer == 3) {
        self->animator.animationTimer = 0;
        self->animator.frameID        = (self->animator.frameID + 6) % 12;
    }
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (!player) {
        self->velocity.y = -0x40000;
        if (self->direction == FLIP_NONE)
            self->velocity.x = 0x20000;
        else
            self->velocity.x = -0x20000;
        self->state = MegaChopper_State_Unknown4;
    }
    else {
        if (player->playerAnimator.animationID == ANI_JUMP) {
            self->playerPtr = NULL;
            if (player->stateInput == MegaChopper_PlayerInput_StateP1) {
                player->stateInput = Player_ProcessP1Input;
            }
            else if (player->stateInput == MegaChopper_PlayerInput_StateP2) {
                player->stateInput = Player_ProcessP2Input_Player;
            }
            else if (player->stateInput == MegaChopper_PlayerInput_StateP2_AI) {
                player->stateInput = Player_ProcessP2Input_AI;
            }

            self->velocity.y = -0x40000;
            if (self->direction == FLIP_NONE)
                self->velocity.x = 0x20000;
            else
                self->velocity.x = -0x20000;
            self->state = MegaChopper_State_Unknown4;
        }
        else {
            if (++self->nibbleTimer >= 60) {
                self->nibbleTimer = 0;
                if (!player->rings || player->sidekick) {
                    Player_Hit(player);
                    if (player->position.x > self->position.x)
                        player->velocity.x = 0x20000;
                    else
                        player->velocity.x = -0x20000;
                    self->playerPtr = NULL;
                }
                else {
                    player->rings--;
                    if (Ring->pan) {
                        int channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                        Ring->pan = 0;
                    }
                    else {
                        int channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
                        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                        Ring->pan = 1;
                    }
                }
            }

            if (self->playerPtr) {
                self->position.x = player->position.x + self->playerPos.x;
                self->position.y = player->position.y + self->playerPos.y;
                if (player->direction != self->playerDir) {
                    self->direction ^= FLIP_X;
                    self->playerDir = player->direction;
                }

                self->playerDir = player->direction;
                if (self->lastShakeFlags) {
                    if (!self->field_72) {
                        self->shakeCount     = 0;
                        self->lastShakeFlags = 0;
                    }
                    else {
                        uint8 flags = 0;
                        self->field_72--;
                        if (player->left)
                            flags = 1;
                        if (player->right)
                            flags |= 2;
                        if (flags) {
                            if (flags != 3 && flags != self->lastShakeFlags) {
                                self->lastShakeFlags = flags;
                                if (++self->shakeCount >= 6) {
                                    self->playerPtr = NULL;
                                    if (player->stateInput == MegaChopper_PlayerInput_StateP1) {
                                        player->stateInput = Player_ProcessP1Input;
                                    }
                                    else if (player->stateInput == MegaChopper_PlayerInput_StateP2) {
                                        player->stateInput = Player_ProcessP2Input_Player;
                                    }
                                    else if (player->stateInput == MegaChopper_PlayerInput_StateP2_AI) {
                                        player->stateInput = Player_ProcessP2Input_AI;
                                    }

                                    self->velocity.y = -0x40000;
                                    if (self->direction == FLIP_NONE)
                                        self->velocity.x = 0x20000;
                                    else
                                        self->velocity.x = -0x20000;
                                    self->state = MegaChopper_State_Unknown4;
                                }
                            }
                        }
                    }
                }
                else if (player->left) {
                    self->lastShakeFlags = 1;
                    self->field_72       = 64;
                }
                else if (player->right) {
                    self->lastShakeFlags = 2;
                    self->field_72       = 64;
                }
            }
            else {
                if (player->stateInput == MegaChopper_PlayerInput_StateP1) {
                    player->stateInput = Player_ProcessP1Input;
                }
                else if (player->stateInput == MegaChopper_PlayerInput_StateP2) {
                    player->stateInput = Player_ProcessP2Input_Player;
                }
                else if (player->stateInput == MegaChopper_PlayerInput_StateP2_AI) {
                    player->stateInput = Player_ProcessP2Input_AI;
                }

                self->velocity.y = -0x40000;
                if (self->direction == FLIP_NONE)
                    self->velocity.x = 0x20000;
                else
                    self->velocity.x = -0x20000;
                self->state = MegaChopper_State_Unknown4;
            }
        }
    }

    MegaChopper_CheckOnScreen();
}

void MegaChopper_State_Unknown4(void)
{
    RSDK_THIS(MegaChopper);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    MegaChopper_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void MegaChopper_EditorDraw(void) { MegaChopper_Draw(); }

void MegaChopper_EditorLoad(void) { MegaChopper->aniFrames = RSDK.LoadSpriteAnimation("HCZ/MegaChopper.bin", SCOPE_STAGE); }
#endif

void MegaChopper_Serialize(void) {}
