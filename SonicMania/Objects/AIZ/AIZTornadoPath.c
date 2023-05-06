// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZTornadoPath Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAIZTornadoPath *AIZTornadoPath;

void AIZTornadoPath_Update(void)
{
    RSDK_THIS(AIZTornadoPath);
    StateMachine_Run(self->state);
}

void AIZTornadoPath_LateUpdate(void) {}

void AIZTornadoPath_StaticUpdate(void) {}

void AIZTornadoPath_Draw(void) {}

void AIZTornadoPath_Create(void *data)
{
    RSDK_THIS(AIZTornadoPath);
    if (!SceneInfo->inEditor) {
        switch (self->type) {
            case AIZTORNADOPATH_START:
                if (!StarPost->postIDs[0]) {
                    self->active         = ACTIVE_NORMAL;
                    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                    if (camera) {
                        camera->state          = StateMachine_None;
                        camera->position.x     = self->position.x;
                        camera->position.y     = self->position.y;
                        AIZTornadoPath->camera = camera;
                    }

                    foreach_all(Player, player) { player->camera = NULL; }

                    foreach_all(AIZTornado, tornado) { AIZTornadoPath->tornado = tornado; }
                    ScreenInfo->position.y = FROM_FIXED(self->position.y) - ScreenInfo->center.y;
                    self->speed            = self->targetSpeed;
                    self->state            = AIZTornadoPath_State_SetTornadoSpeed;
                }
                break;

            case AIZTORNADOPATH_DUD: self->active = ACTIVE_NEVER; break;

            case AIZTORNADOPATH_SETSPEED:
                self->active = ACTIVE_NEVER;
                self->timer  = 1;
                self->state  = AIZTornadoPath_State_SetTornadoSpeed;
                break;

            case AIZTORNADOPATH_SETCAMERA:
                self->active = ACTIVE_NEVER;
                self->state  = AIZTornadoPath_State_SetPlayerCamera;
                break;

            case AIZTORNADOPATH_DISABLEINTERACTIONS:
                self->active = ACTIVE_NEVER;
                self->state  = AIZTornadoPath_State_DisablePlayerInteractions;
                break;

            case AIZTORNADOPATH_EXITTORNADO:
                self->active = ACTIVE_NEVER;
                self->timer  = 1;
                self->state  = AIZTornadoPath_State_ExitTornadoSequence;
                break;

            case AIZTORNADOPATH_ENTERTORNADO:
                self->state  = AIZTornadoPath_State_PrepareCatchPlayer;
                self->active = (StarPost->postIDs[0] > 0) ? ACTIVE_XBOUNDS : ACTIVE_NEVER;
                self->speed  = self->targetSpeed;
                break;

            case AIZTORNADOPATH_TARGETNODE:
                self->active = ACTIVE_NEVER;
                self->timer  = 1;
                self->speed  = self->targetSpeed;
                self->state  = AIZTornadoPath_State_SetTornadoSpeed;
                break;

            default: break;
        }
    }
}

void AIZTornadoPath_StageLoad(void)
{
    AIZTornadoPath->moveVel.x = 0;
    AIZTornadoPath->moveVel.y = 0;
}

void AIZTornadoPath_HandleMoveSpeed(void)
{
    RSDK_THIS(AIZTornadoPath);
    bool32 usingCamPos        = false;
    EntityCamera *camera      = AIZTornadoPath->camera;
    EntityAIZTornado *tornado = AIZTornadoPath->tornado;

    int32 x = 0, y = 0;
    if (camera && camera->position.x >= ScreenInfo->size.x << 16) {
        x           = camera->position.x;
        y           = camera->position.y;
        usingCamPos = true;
    }
    else {
        x = tornado->newPos.x;
        y = tornado->newPos.y;
    }

    EntityAIZTornadoPath *node = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, AIZTornadoPath);
    int32 xDist                = (x - node->position.x) >> 16;
    int32 yDist                = (y - node->position.y) >> 16;
    self->angle                = RSDK.ATan2(xDist, yDist);
    int32 newPosX              = x - self->speed * RSDK.Cos256(self->angle);
    int32 newPosY              = y - self->speed * RSDK.Sin256(self->angle);
    if (usingCamPos) {
        camera->position.x = newPosX;
        camera->position.y = newPosY;
    }

    int32 spd = self->speed >> 3;
    if (xDist * xDist + yDist * yDist < spd) {
        self->active = ACTIVE_NEVER;
        node->active = ACTIVE_NORMAL;
        if (node->easeToSpeed)
            node->speed = self->speed;
        else
            node->speed = self->targetSpeed;
    }
    AIZTornadoPath->moveVel.x = newPosX - x;
    AIZTornadoPath->moveVel.y = newPosY - y;
}

void AIZTornadoPath_State_SetTornadoSpeed(void)
{
    RSDK_THIS(AIZTornadoPath);

    if (self->speed >= self->targetSpeed) {
        if (self->speed > self->targetSpeed) {
            self->speed -= 16;
            if (self->speed < self->targetSpeed)
                self->speed = self->targetSpeed;
        }
    }
    else {
        self->speed += 16;
        if (self->speed > self->targetSpeed)
            self->speed = self->targetSpeed;
    }

    if (self->timer > 0) {
        self->timer--;
    }
    else {
        AIZTornadoPath_HandleMoveSpeed();
    }
}

void AIZTornadoPath_State_SetPlayerCamera(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera = AIZTornadoPath->camera;

    if (camera) {
        camera->state          = Camera_State_FollowXY;
        player->camera         = camera;
        AIZTornadoPath->camera = NULL;
    }
}

void AIZTornadoPath_State_DisablePlayerInteractions(void)
{
    RSDK_THIS(AIZTornadoPath);
    EntityPlayer *player   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player->collisionPlane = 1;
    player->interaction    = false;

    foreach_active(Player, playerPtr) { playerPtr->drawGroup = Zone->playerDrawGroup[1]; }

    AIZTornadoPath_HandleMoveSpeed();
    self->state = AIZTornadoPath_State_SetTornadoSpeed;
}

void AIZTornadoPath_State_ExitTornadoSequence(void)
{
    foreach_active(AIZTornado, tornado) { tornado->drawGroup = Zone->objectDrawGroup[0]; }

    if (!tornado->disableInteractions) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityCamera *camera = AIZTornadoPath->camera;

        if (camera) {
            camera->state          = Camera_State_FollowXY;
            player->camera         = camera;
            player->collisionPlane = 0;
            player->interaction    = true;
            player->applyJumpCap   = false;
            player->drawGroup      = Zone->playerDrawGroup[0];
            AIZTornadoPath->camera = NULL;
        }
        player->groundVel = AIZTornadoPath->moveVel.x;
        Player_Action_Jump(player);
        player->right                = true;
        tornado->disableInteractions = true;
    }

    AIZTornadoPath_State_SetTornadoSpeed();
}

void AIZTornadoPath_State_PrepareCatchPlayer(void)
{
    RSDK_THIS(AIZTornadoPath);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player->position.x > self->position.x) {
        player->stateInput = StateMachine_None;
        player->left       = false;
        player->right      = true;
        if (player->pushing > 0) {
            player->jumpPress         = true;
            player->jumpHold          = true;
            self->state               = AIZTornadoPath_State_CatchPlayer;
            AIZTornadoPath->moveVel.x = 0;
            AIZTornadoPath->moveVel.y = 0;
        }
    }
}

void AIZTornadoPath_State_CatchPlayer(void)
{
    RSDK_THIS(AIZTornadoPath);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    ++self->timer;
    if (self->timer == 60) {
        foreach_all(AIZTornado, tornado)
        {
            tornado->position.x = player->position.x - 0x1400000;
            tornado->position.y = player->position.y + 0x400000;
            tornado->offsetX    = 0x40000;
            tornado->state      = AIZTornado_State_Move;
            tornado->active     = ACTIVE_NORMAL;
        }
    }

    if (self->timer == 90) {
        self->timer        = 0;
        player->stateInput = Player_Input_P1;
        int32 velX         = 0;
        foreach_all(AIZTornadoPath, node)
        {
            if (node->type == AIZTORNADOPATH_TARGETNODE) {
                velX         = node->position.x - player->position.x;
                node->active = ACTIVE_NORMAL;
            }
        }

        player->position.x += velX;
        if (player->camera) {
            player->camera->position.x += velX;
            player->camera->state  = StateMachine_None;
            AIZTornadoPath->camera = player->camera;
            player->camera         = NULL;
        }

        foreach_active(AIZTornado, tornado)
        {
            tornado->position.x += velX;
            tornado->offsetX = 0x80000;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void AIZTornadoPath_EditorDraw(void)
{
    RSDK_THIS(AIZTornadoPath);
    RSDK.SetSpriteAnimation(AIZTornadoPath->aniFrames, 0, &self->unusedAnimator, true, 7);
    RSDK.DrawSprite(&self->unusedAnimator, NULL, false);
}

void AIZTornadoPath_EditorLoad(void)
{
    AIZTornadoPath->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(AIZTornadoPath->aniFrames, 0, &AIZTornadoPath->unusedAnimator, true, 7);

    RSDK_ACTIVE_VAR(AIZTornadoPath, type);
    RSDK_ENUM_VAR("Start Node", AIZTORNADOPATH_START);
    RSDK_ENUM_VAR("Blank Node", AIZTORNADOPATH_DUD);
    RSDK_ENUM_VAR("Set Tornado Speed", AIZTORNADOPATH_SETSPEED);
    RSDK_ENUM_VAR("Return Player Camera", AIZTORNADOPATH_SETCAMERA);
    RSDK_ENUM_VAR("Disable Player Interactions", AIZTORNADOPATH_DISABLEINTERACTIONS);
    RSDK_ENUM_VAR("Exit Tornado Sequence", AIZTORNADOPATH_EXITTORNADO);
    RSDK_ENUM_VAR("Enter Tornado Sequence (Fly to Target Node)", AIZTORNADOPATH_ENTERTORNADO);
    RSDK_ENUM_VAR("Set Tornado Speed (Target Node)", AIZTORNADOPATH_TARGETNODE);
}
#endif

void AIZTornadoPath_Serialize(void)
{
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, targetSpeed);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_BOOL, easeToSpeed);
}
