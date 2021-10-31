#include "SonicMania.h"

ObjectMegaChopper *MegaChopper;

void MegaChopper_Update(void)
{
    RSDK_THIS(MegaChopper);
    StateMachine_Run(entity->state);
}

void MegaChopper_LateUpdate(void) {}

void MegaChopper_StaticUpdate(void) {}

void MegaChopper_Draw(void)
{
    RSDK_THIS(MegaChopper);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MegaChopper_Create(void *data)
{
    RSDK_THIS(MegaChopper);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->playerDrawLow + 1;
    entity->startPos      = entity->position;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(MegaChopper->aniFrames, 0, &entity->animator, true, 0);
    entity->state = MegaChopper_State_Setup;
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
    CREATE_ENTITY(MegaChopper, NULL, entity->position.x, entity->position.y);
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
        if (Player_CheckBadnikTouch(player, entity, &MegaChopper->hitbox1)) {
            int blink          = player->blinkTimer;
            player->blinkTimer = 1;
            if (!Player_CheckBadnikBreak(entity, player, true) && Player_CheckCollisionTouch(player, entity, &MegaChopper->hitbox2)) {
                entity->playerPtr   = (Entity *)player;
                entity->playerPos.x = (entity->position.x - player->position.x) & 0xFFFF0000;
                entity->playerPos.y = (entity->position.y - player->position.y) & 0xFFFF0000;
                entity->playerDir   = player->direction;

                if (player->stateInput == Player_ProcessP1Input) {
                    player->stateInput = MegaChopper_PlayerInput_StateP1;
                }
                else if (player->stateInput == Player_ProcessP2Input_Player) {
                    player->stateInput = MegaChopper_PlayerInput_StateP2;
                }
                else if (player->stateInput == Player_ProcessP2Input_AI) {
                    player->stateInput = MegaChopper_PlayerInput_StateP2_AI;
                }
                entity->drawOrder   = player->drawOrder + 1;
                entity->isPermanent = true;
                entity->state       = MegaChopper_State_Unknown3;
            }
            player->blinkTimer = blink;
            foreach_break;
        }
    }
}

void MegaChopper_CheckOnScreen(void)
{
    RSDK_THIS(MegaChopper);
    if (!RSDK.CheckOnScreen(entity, NULL) && RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange) == false) {
        entity->position.x   = entity->startPos.x;
        entity->position.y   = entity->startPos.y;
        EntityPlayer *player = (EntityPlayer *)entity->playerPtr;
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
        entity->nibbleTimer = 0;
        entity->isPermanent = false;
        MegaChopper_Create(NULL);
    }
}

void MegaChopper_PlayerInput_StateP1(void)
{
    RSDK_THIS(Player);
    Player_ProcessP1Input();
    if (entity->state != Player_State_None) {
        entity->up        = false;
        entity->down      = false;
        entity->jumpPress = false;
        entity->jumpHold  = false;
    }
}

void MegaChopper_PlayerInput_StateP2(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_Player();
    entity->up        = false;
    entity->down      = false;
    entity->jumpPress = false;
    entity->jumpHold  = false;
    if (entity->stateInput == Player_ProcessP2Input_AI)
        entity->stateInput = MegaChopper_PlayerInput_StateP2_AI;
}

void MegaChopper_PlayerInput_StateP2_AI(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_AI();
    entity->up        = false;
    entity->down      = false;
    entity->jumpPress = false;
    entity->jumpHold  = false;
    if (entity->stateInput == Player_ProcessP2Input_Player)
        entity->stateInput = MegaChopper_PlayerInput_StateP2;
}

void MegaChopper_State_Setup(void)
{
    RSDK_THIS(MegaChopper);
    if (entity->position.y >= Water->waterLevel) {
        entity->active     = ACTIVE_NORMAL;
        entity->velocity.x = -0x10000;
        entity->state      = MegaChopper_State_Unknown1;
        MegaChopper_State_Unknown1();
    }
    else {
        destroyEntity(entity);
    }
}

void MegaChopper_State_Unknown1(void)
{
    RSDK_THIS(MegaChopper);
    if (++entity->animator.frameID == 6)
        entity->animator.frameID = 0;
    if (entity->animator.frameID == 12)
        entity->animator.frameID = 6;
    EntityPlayer *player = Player_GetNearestPlayer();

    if (entity->position.x >= player->position.x) {
        entity->velocity.x -= 0x800;
        if (entity->velocity.x < -0x20000)
            entity->velocity.x = -0x20000;
    }
    else {
        entity->velocity.x += 0x800;
        if (entity->velocity.x > 0x20000)
            entity->velocity.x = 0x20000;
    }

    if (entity->position.y >= player->position.y)
        entity->position.y -= 0x2000;
    else
        entity->position.y += 0x2000;

    entity->direction = entity->velocity.x > 0;
    if (entity->velocity.y > 0) {
        entity->velocity.y -= 0x3800;
        if (entity->velocity.y < 0)
            entity->velocity.y = 0;
    }
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->position.y < Water->waterLevel) {

        bool32 flag = false;
        foreach_active(Water, water)
        {
            if (water->type == WATER_RECT && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, entity, &Water->hitbox)) {
                flag = true;
            }
        }

        if (!flag) {
            entity->velocity.y = -0x40000;
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = -0x20000;
            else
                entity->velocity.x = 0x20000;
            entity->velocity.x = 0x20000;
            entity->state      = MegaChopper_State_Unknown2;
        }
    }
    MegaChopper_CheckPlayerCollisions();
    MegaChopper_CheckOnScreen();
}

void MegaChopper_State_Unknown2(void)
{
    RSDK_THIS(MegaChopper);
    if (++entity->animator.frameID == 6)
        entity->animator.frameID = 0;
    if (entity->animator.frameID == 12)
        entity->animator.frameID = 6;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    if (entity->position.y >= Water->waterLevel) {
        entity->state = MegaChopper_State_Unknown1;
    }
    else {
        foreach_active(Water, water)
        {
            if (water->type == WATER_RECT && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, entity, &Water->hitbox)) {
                entity->state = MegaChopper_State_Unknown1;
            }
        }
    }
    MegaChopper_CheckPlayerCollisions();
    MegaChopper_CheckOnScreen();
}

void MegaChopper_State_Unknown3(void)
{
    RSDK_THIS(MegaChopper);
    if (++entity->animator.animationTimer == 3) {
        entity->animator.animationTimer = 0;
        entity->animator.frameID        = (entity->animator.frameID + 6) % 12;
    }
    EntityPlayer *player = (EntityPlayer *)entity->playerPtr;

    if (!player) {
        entity->velocity.y = -0x40000;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = 0x20000;
        else
            entity->velocity.x = -0x20000;
        entity->state = MegaChopper_State_Unknown4;
    }
    else {
        if (player->playerAnimator.animationID == ANI_JUMP) {
            entity->playerPtr = NULL;
            if (player->stateInput == MegaChopper_PlayerInput_StateP1) {
                player->stateInput = Player_ProcessP1Input;
            }
            else if (player->stateInput == MegaChopper_PlayerInput_StateP2) {
                player->stateInput = Player_ProcessP2Input_Player;
            }
            else if (player->stateInput == MegaChopper_PlayerInput_StateP2_AI) {
                player->stateInput = Player_ProcessP2Input_AI;
            }

            entity->velocity.y = -0x40000;
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = 0x20000;
            else
                entity->velocity.x = -0x20000;
            entity->state = MegaChopper_State_Unknown4;
        }
        else {
            if (++entity->nibbleTimer >= 60) {
                entity->nibbleTimer = 0;
                if (!player->rings || player->sidekick) {
                    Player_Hit(player);
                    if (player->position.x > entity->position.x)
                        player->velocity.x = 0x20000;
                    else
                        player->velocity.x = -0x20000;
                    entity->playerPtr = NULL;
                }
                else {
                    player->rings--;
                    if (Ring->pan) {
                        int channel = RSDK.PlaySfx(Ring->sfx_Ring, false, 255);
                        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
                        Ring->pan = 0;
                    }
                    else {
                        int channel = RSDK.PlaySfx(Ring->sfx_Ring, false, 255);
                        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
                        Ring->pan = 1;
                    }
                }
            }

            if (entity->playerPtr) {
                entity->position.x = player->position.x + entity->playerPos.x;
                entity->position.y = player->position.y + entity->playerPos.y;
                if (player->direction != entity->playerDir) {
                    entity->direction ^= FLIP_X;
                    entity->playerDir = player->direction;
                }

                entity->playerDir = player->direction;
                if (entity->lastShakeFlags) {
                    if (!entity->field_72) {
                        entity->shakeCount     = 0;
                        entity->lastShakeFlags = 0;
                    }
                    else {
                        uint8 flags = 0;
                        entity->field_72--;
                        if (player->left)
                            flags = 1;
                        if (player->right)
                            flags |= 2;
                        if (flags) {
                            if (flags != 3 && flags != entity->lastShakeFlags) {
                                entity->lastShakeFlags = flags;
                                if (++entity->shakeCount >= 6) {
                                    entity->playerPtr = NULL;
                                    if (player->stateInput == MegaChopper_PlayerInput_StateP1) {
                                        player->stateInput = Player_ProcessP1Input;
                                    }
                                    else if (player->stateInput == MegaChopper_PlayerInput_StateP2) {
                                        player->stateInput = Player_ProcessP2Input_Player;
                                    }
                                    else if (player->stateInput == MegaChopper_PlayerInput_StateP2_AI) {
                                        player->stateInput = Player_ProcessP2Input_AI;
                                    }

                                    entity->velocity.y = -0x40000;
                                    if (entity->direction == FLIP_NONE)
                                        entity->velocity.x = 0x20000;
                                    else
                                        entity->velocity.x = -0x20000;
                                    entity->state = MegaChopper_State_Unknown4;
                                }
                            }
                        }
                    }
                }
                else if (player->left) {
                    entity->lastShakeFlags = 1;
                    entity->field_72       = 64;
                }
                else if (player->right) {
                    entity->lastShakeFlags = 2;
                    entity->field_72       = 64;
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

                entity->velocity.y = -0x40000;
                if (entity->direction == FLIP_NONE)
                    entity->velocity.x = 0x20000;
                else
                    entity->velocity.x = -0x20000;
                entity->state = MegaChopper_State_Unknown4;
            }
        }
    }

    MegaChopper_CheckOnScreen();
}

void MegaChopper_State_Unknown4(void)
{
    RSDK_THIS(MegaChopper);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    MegaChopper_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void MegaChopper_EditorDraw(void) { MegaChopper_Draw(); }

void MegaChopper_EditorLoad(void) { MegaChopper->aniFrames = RSDK.LoadSpriteAnimation("HCZ/MegaChopper.bin", SCOPE_STAGE); }
#endif

void MegaChopper_Serialize(void) {}
