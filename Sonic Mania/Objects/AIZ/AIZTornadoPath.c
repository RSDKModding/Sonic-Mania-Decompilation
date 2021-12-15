// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZTornadoPath Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
                    self->active       = ACTIVE_NORMAL;
                    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                    if (camera) {
                        camera->state          = StateMachine_None;
                        camera->position.x     = self->position.x;
                        camera->position.y     = self->position.y;
                        AIZTornadoPath->camera = (Entity *)camera;
                    }

                    foreach_all(Player, player) { player->camera = NULL; }

                    foreach_all(AIZTornado, tornado) { AIZTornadoPath->tornado = tornado; }
                    ScreenInfo->position.y = (self->position.y >> 0x10) - ScreenInfo->centerY;
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
            case AIZTORNADOPATH_JUMPOFF:
                self->active = ACTIVE_NEVER;
                self->timer  = 1;
                self->state  = AIZTornadoPath_JumpOffTornado;
                break;
            case AIZTORNADOPATH_6:
                self->state  = AIZTornadoPath_State_MoveRightJump;
                self->active = (StarPost->postIDs[0] > 0) ? ACTIVE_XBOUNDS : ACTIVE_NEVER;
                self->speed  = self->targetSpeed;
                break;
            case AIZTORNADOPATH_SETSPEED_ALT:
                self->active = ACTIVE_NEVER;
                self->timer  = 1;
                self->speed  = self->targetSpeed;
                self->state  = AIZTornadoPath_State_SetTornadoSpeed;
                break;
            default: return;
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
    bool32 flag               = false;
    EntityCamera *camera      = (EntityCamera *)AIZTornadoPath->camera;
    EntityAIZTornado *tornado = AIZTornadoPath->tornado;

    int32 x = 0, y = 0;
    if (camera && camera->position.x >= ScreenInfo->width << 16) {
        x    = camera->position.x;
        y    = camera->position.y;
        flag = true;
    }
    else {
        x = tornado->newPos.x;
        y = tornado->newPos.y;
    }
    EntityAIZTornadoPath *node = (EntityAIZTornadoPath *)RSDK.GetEntityByID(SceneInfo->entitySlot + 1);
    int32 xDist                  = (x - node->position.x) >> 16;
    int32 yDist                  = (y - node->position.y) >> 16;
    self->angle              = RSDK.ATan2(xDist, yDist);
    int32 newPosX                = x - self->speed * RSDK.Cos256(self->angle);
    int32 newPosY                = y - self->speed * RSDK.Sin256(self->angle);
    if (flag) {
        camera->position.x = newPosX;
        camera->position.y = newPosY;
    }

    int32 spd = self->speed >> 3;
    if (xDist * xDist + yDist * yDist < spd) {
        self->active = ACTIVE_NEVER;
        node->active   = ACTIVE_NORMAL;
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
    EntityCamera *camera = (EntityCamera *)AIZTornadoPath->camera;
    if (camera) {
        camera->state          = Camera_State_Follow;
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

    foreach_active(Player, playerPtr) { playerPtr->drawOrder = Zone->playerDrawHigh; }

    AIZTornadoPath_HandleMoveSpeed();
    self->state = AIZTornadoPath_State_SetTornadoSpeed;
}

void AIZTornadoPath_JumpOffTornado(void)
{
    foreach_active(AIZTornado, tornado) { tornado->drawOrder = Zone->drawOrderLow; }

    if (!tornado->disableInteractions) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityCamera *camera = (EntityCamera *)AIZTornadoPath->camera;
        if (camera) {
            camera->state          = Camera_State_Follow;
            player->camera         = camera;
            player->collisionPlane = 0;
            player->interaction    = true;
            player->jumpAbility    = 0;
            player->drawOrder      = Zone->playerDrawLow;
            AIZTornadoPath->camera = NULL;
        }
        player->groundVel = AIZTornadoPath->moveVel.x;
        Player_StartJump(player);
        player->right     = true;
        tornado->disableInteractions = true;
    }

    AIZTornadoPath_State_SetTornadoSpeed();
}

void AIZTornadoPath_State_MoveRightJump(void)
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
            self->state             = AIZTornadoPath_State_FlyOff;
            AIZTornadoPath->moveVel.x = 0;
            AIZTornadoPath->moveVel.y = 0;
        }
    }
}

void AIZTornadoPath_State_FlyOff(void)
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
        self->timer      = 0;
        player->stateInput = Player_ProcessP1Input;
        int32 xOffset        = 0;
        foreach_all(AIZTornadoPath, node)
        {
            if (node->type == AIZTORNADOPATH_SETSPEED_ALT) {
                xOffset      = node->position.x - player->position.x;
                node->active = ACTIVE_NORMAL;
            }
        }

        player->position.x += xOffset;
        if (player->camera) {
            player->camera->position.x += xOffset;
            player->camera->state  = StateMachine_None;
            AIZTornadoPath->camera = (Entity *)player->camera;
            player->camera         = NULL;
        }

        foreach_active(AIZTornado, tornado)
        {
            tornado->position.x += xOffset;
            tornado->offsetX = 0x80000;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void AIZTornadoPath_EditorDraw(void)
{
    RSDK_THIS(AIZTornadoPath);
    RSDK.SetSpriteAnimation(AIZTornadoPath->aniFrames, 0, &self->animator, true, 7);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void AIZTornadoPath_EditorLoad(void)
{
    AIZTornadoPath->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(AIZTornadoPath->aniFrames, 0, &AIZTornadoPath->animator, true, 7);

    RSDK_ACTIVE_VAR(AIZTornadoPath, type);
    RSDK_ENUM_VAR("Start Node", AIZTORNADOPATH_START);
    RSDK_ENUM_VAR("Blank Node", AIZTORNADOPATH_DUD);
    RSDK_ENUM_VAR("Set Tornado Speed", AIZTORNADOPATH_SETSPEED);
    RSDK_ENUM_VAR("Return Player Camera", AIZTORNADOPATH_SETCAMERA);
    RSDK_ENUM_VAR("Disable Player Interactions", AIZTORNADOPATH_DISABLEINTERACTIONS);
    RSDK_ENUM_VAR("Jump Off Tornado", AIZTORNADOPATH_JUMPOFF);
    RSDK_ENUM_VAR("Type 6", AIZTORNADOPATH_6);
    RSDK_ENUM_VAR("Set Tornado Speed (Alt)", AIZTORNADOPATH_SETSPEED_ALT);
}
#endif

void AIZTornadoPath_Serialize(void)
{
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, targetSpeed);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(AIZTornadoPath, VAR_BOOL, easeToSpeed);
}
