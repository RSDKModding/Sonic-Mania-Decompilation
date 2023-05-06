// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TornadoPath Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTornadoPath *TornadoPath;

void TornadoPath_Update(void)
{
    RSDK_THIS(TornadoPath);

    StateMachine_Run(self->state);
}

void TornadoPath_LateUpdate(void) {}

void TornadoPath_StaticUpdate(void) {}

void TornadoPath_Draw(void) {}

void TornadoPath_Create(void *data)
{
    RSDK_THIS(TornadoPath);

    if (!SceneInfo->inEditor) {
        switch (self->type) {
            case TORNADOPATH_START:
                if (!StarPost->postIDs[0]
                    && CutsceneRules_CheckPlayerPos(self->position.x - (self->size.x >> 1), self->position.y - (self->size.y >> 1),
                                                    self->position.x + (self->size.x >> 1), self->position.y + (self->size.y >> 1))) {
                    TornadoPath_SetupHitbox();
                    self->active = ACTIVE_NORMAL;

                    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                    if (camera) {
                        camera->state       = StateMachine_None;
                        camera->position.x  = self->position.x;
                        camera->position.y  = self->position.y;
                        TornadoPath->camera = camera;
                    }

                    foreach_all(Player, player) { player->camera = NULL; }

                    ScreenInfo->position.y = (self->position.y >> 16) - ScreenInfo->center.y;
                    self->state            = TornadoPath_State_SetTornadoSpeed;
                    self->currentSpeed     = self->targetSpeed;
                }
                break;

            case TORNADOPATH_DUD: self->active = ACTIVE_NEVER; break;

            case TORNADOPATH_SETSPEED:
            case TORNADOPATH_SETSPEED_HIGH:
                self->active = ACTIVE_NEVER;
                self->timer  = 1;
                self->state  = TornadoPath_State_SetTornadoSpeed;
                break;

            case TORNADOPATH_SETCAMERA:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_ReturnCamera;
                break;

            case TORNADOPATH_DISABLEINTERACTIONS:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_DisablePlayerInteractions;
                break;

            case TORNADOPATH_EXITTORNADO_STOPAUTOSCROLL:
            case TORNADOPATH_EXITTORNADO_TRAINSEQ:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_ExitTornadoSequence;
                break;

            case TORNADOPATH_ENTERTORNADO:
            case TORNADOPATH_ENTERTORNADO_FLYTOBOSS:
                if (!StarPost->postIDs[0]
                    && CutsceneRules_CheckPlayerPos(self->position.x - (self->size.x >> 1), self->position.y - (self->size.y >> 1),
                                                    self->position.x + (self->size.x >> 1), self->position.y + (self->size.y >> 1))) {
                    self->active       = ACTIVE_NEVER;
                    self->state        = TornadoPath_State_PrepareCatchPlayer;
                    self->currentSpeed = self->targetSpeed;
                }
                else {
                    self->active       = ACTIVE_XBOUNDS;
                    self->state        = TornadoPath_State_PrepareCatchPlayer;
                    self->currentSpeed = self->targetSpeed;
                }
                break;

            case TORNADOPATH_HANDLEBOSS_MSZ1ST:
                self->active       = ACTIVE_NEVER;
                self->timer        = 1;
                self->state        = TornadoPath_State_UberCaterkillerBossNode;
                self->currentSpeed = self->targetSpeed;
                break;

            case TORNADOPATH_GOTOSTOP:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_GoToStopNode;
                break;

            case TORNADOPATH_SETUPMSZ1CUTSCENEST:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_SetupMSZ1CutsceneST;
                break;

            case TORNADOPATH_CRASHFINISH:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_FinishCrash;
                break;

            default: break;
        }
    }
}

void TornadoPath_StageLoad(void)
{
    TornadoPath->moveVel.x = 0;
    TornadoPath->moveVel.y = 0;
    TornadoPath->hitboxID  = 0;
}

void TornadoPath_SetupHitbox(void)
{
    RSDK_THIS(TornadoPath);

    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.top    = -(self->size.y >> 17);
    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.bottom = self->size.y >> 17;
}

void TornadoPath_HandleMoveSpeed(void)
{
    RSDK_THIS(TornadoPath);

    EntityCamera *camera = TornadoPath->camera;
    if (camera) {
        EntityTornadoPath *node = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, TornadoPath);
        int32 y                 = (camera->position.y - node->position.y) >> 16;
        int32 x                 = (camera->position.x - node->position.x) >> 16;
        self->angle             = RSDK.ATan2(x, y);

        int32 cx = camera->position.x & 0xFFFF0000;
        int32 cy = camera->position.y & 0xFFFF0000;
        camera->position.x -= self->currentSpeed * RSDK.Cos256(self->angle);
        camera->position.y -= self->currentSpeed * RSDK.Sin256(self->angle);
        TornadoPath->hitboxID = self->fastMode;

        int32 radius = self->currentSpeed >> 3;
        if (x * x + y * y < radius) {
            self->active       = ACTIVE_NEVER;
            node->active       = ACTIVE_NORMAL;
            node->currentSpeed = node->easeToSpeed ? self->currentSpeed : self->targetSpeed;
        }

        TornadoPath->moveVel.x = (camera->position.x & 0xFFFF0000) - cx;
        TornadoPath->moveVel.y = (camera->position.y & 0xFFFF0000) - cy;
    }
}

void TornadoPath_State_SetTornadoSpeed(void)
{
    RSDK_THIS(TornadoPath);

    if (self->type == TORNADOPATH_SETSPEED_HIGH) {
        foreach_active(Tornado, tornado) { tornado->drawGroup = Zone->playerDrawGroup[1]; }
    }

    if (self->currentSpeed >= self->targetSpeed) {
        if (self->currentSpeed > self->targetSpeed) {
            self->currentSpeed -= 16;

            if (self->currentSpeed < self->targetSpeed)
                self->currentSpeed = self->targetSpeed;
        }
    }
    else {
        self->currentSpeed += 16;
        if (self->currentSpeed > self->targetSpeed)
            self->currentSpeed = self->targetSpeed;
    }

    if (self->forceSpeed > 0)
        self->currentSpeed = self->forceSpeed;

    if (self->timer <= 0) {
        TornadoPath_HandleMoveSpeed();
    }
    else {
        self->timer--;
    }
}

void TornadoPath_State_ReturnCamera(void)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    TornadoPath->camera->state = Camera_State_FollowXY;
    player1->camera            = TornadoPath->camera;
}

void TornadoPath_State_DisablePlayerInteractions(void)
{
    RSDK_THIS(TornadoPath);

    EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->collisionPlane = 1;
    player1->interaction    = false;

    foreach_active(Tornado, tornado) { tornado->drawGroup = Zone->playerDrawGroup[1]; }

    TornadoPath_HandleMoveSpeed();
    self->state = TornadoPath_State_SetTornadoSpeed;
}

void TornadoPath_State_ExitTornadoSequence(void)
{
    RSDK_THIS(TornadoPath);

    EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TornadoPath->camera->state = Camera_State_FollowXY;

    player1->camera         = TornadoPath->camera;
    player1->collisionPlane = 0;
    player1->interaction    = true;
    player1->applyJumpCap   = false;
    player1->drawGroup      = Zone->playerDrawGroup[0];

    if (player1->groundedStore)
        Player_Action_Jump(player1);

    foreach_active(Tornado, tornado)
    {
        if (self->type == TORNADOPATH_EXITTORNADO_STOPAUTOSCROLL) {
            tornado->drawGroup = Zone->objectDrawGroup[1];
            MSZSetup_ReloadBGParallax_Multiply(0x000);
            tornado->state = Tornado_State_FlyAway_Right;
        }
        else {
            tornado->drawGroup = Zone->objectDrawGroup[0];
            tornado->state     = Tornado_State_FlyAway_Left;
        }
    }

    foreach_all(TornadoPath, node)
    {
        bool32 isNextNode = false;
        if (self->type == TORNADOPATH_EXITTORNADO_STOPAUTOSCROLL)
            isNextNode = node->type == TORNADOPATH_ENTERTORNADO;
        else
            isNextNode = node->type == TORNADOPATH_ENTERTORNADO_FLYTOBOSS;

        if (isNextNode)
            node->active = ACTIVE_XBOUNDS;
    }

    TornadoPath->camera = NULL;
    self->active        = ACTIVE_NEVER;
}

void TornadoPath_State_PrepareCatchPlayer(void)
{
    RSDK_THIS(TornadoPath);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    bool32 isPlayerControlled = false;
    foreach_active(Tornado, tornado)
    {
        if (tornado->state == Tornado_State_PlayerControlled)
            isPlayerControlled = true;
    }

    if (!isPlayerControlled) {
        if (player1->position.x > self->position.x && player1->position.y < self->position.y) {
            self->active        = ACTIVE_NORMAL;
            player1->stateInput = StateMachine_None;
            player1->left       = false;
            player1->right      = true;
            if (player1->pushing > 0) {
                if (player1->onGround)
                    player1->jumpPress = true;

                player1->jumpHold = true;
            }

            if (!player1->onGround) {
                self->state            = TornadoPath_State_CatchPlayer;
                TornadoPath->moveVel.x = 0;
                TornadoPath->moveVel.y = 0;
            }
        }
    }
}

void TornadoPath_State_CatchPlayer(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    ++self->timer;
    if (self->timer == 60) {
        foreach_all(Tornado, tornado)
        {
            tornado->position.x = player1->position.x - 0x1400000;
            tornado->position.y = player1->position.y + 0x400000;
            tornado->offsetX    = 0xA0000;
            tornado->showFlame  = true;
            tornado->state      = Tornado_State_PlayerControlled;
            tornado->active     = ACTIVE_NORMAL;
        }
    }

    if (self->timer == 90) {
        player1->right      = false;
        player1->velocity.x = 0;
        player1->groundVel  = 0;
    }

    if (!player1->velocity.y && player1->state != Player_State_Static) {
        player1->stateInput = Player_Input_P1;
        self->timer         = 0;

        int32 velX = 0;
        int32 velY = 0;
        if (self->type == TORNADOPATH_ENTERTORNADO_FLYTOBOSS) {
            foreach_all(TornadoPath, node)
            {
                if (node->type == TORNADOPATH_HANDLEBOSS_MSZ1ST) {
                    velX         = node->position.x - player1->position.x;
                    velY         = node->position.y - player1->position.y;
                    node->active = ACTIVE_NORMAL;
                }
            }
        }

        player1->position.x += velX;
        player1->position.y += velY;
        EntityCamera *camera = player1->camera;
        if (camera) {
            camera->position.x += velX;
            camera->position.y += velY;
            camera->state       = StateMachine_None;
            TornadoPath->camera = camera;
            player1->camera     = NULL;
        }

        foreach_active(Tornado, tornado)
        {
            tornado->position.x += velX;
            tornado->position.y += velY;
            tornado->offsetX   = 0x80000;
            tornado->showFlame = false;
        }

        self->state = TornadoPath_State_SetTornadoSpeed;
    }
}

void TornadoPath_State_UberCaterkillerBossNode(void)
{
    RSDK_THIS(TornadoPath);

    self->state = TornadoPath_State_HandleUberCaterkillerBoss;
    TornadoPath_State_HandleUberCaterkillerBoss();
}

void TornadoPath_State_HandleUberCaterkillerBoss(void)
{
    RSDK_THIS(TornadoPath);

    self->currentSpeed = 0;

    TornadoPath->moveVel.x = 0;
    TornadoPath->moveVel.y = 0;

    if (UberCaterkiller->defeated)
        self->state = TornadoPath_State_SetTornadoSpeed;
}

void TornadoPath_State_GoToStopNode(void)
{
    RSDK_THIS(TornadoPath);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->currentSpeed     = 0;
    TornadoPath->moveVel.x = 0;
    TornadoPath->moveVel.y = 0;

    if (player1->onGround) {
        int32 velocityX = 0;
        int32 velocityY = 0;

        foreach_all(TornadoPath, node)
        {
            if (node->type == TORNADOPATH_HANDLEBOSS_MSZ1ST) {
                velocityX    = node->position.x - player1->position.x;
                velocityY    = node->position.y - player1->position.y;
                node->active = ACTIVE_NORMAL;
            }
        }

        player1->position.x += velocityX;
        player1->position.y += velocityY;

        EntityCamera *camera = TornadoPath->camera;
        camera->position.x += velocityX;
        camera->position.y += velocityY;
        foreach_active(Tornado, tornado)
        {
            tornado->position.x += velocityX;
            tornado->position.y += velocityY;
            tornado->offsetX   = 0x80000;
            tornado->showFlame = false;
        }
    }
}

void TornadoPath_State_SetupMSZ1CutsceneST(void)
{
    RSDK_THIS(TornadoPath);

    if (!self->timer) {
        foreach_all(MSZCutsceneST, cutscene)
        {
            cutscene->active   = ACTIVE_NORMAL;
            cutscene->pathNode = self;
        }
    }
}

void TornadoPath_State_FinishCrash(void)
{
    TornadoPath->moveVel.x     = 0;
    TornadoPath->moveVel.y     = 0;
    TornadoPath->crashFinished = true;
}

#if GAME_INCLUDE_EDITOR
void TornadoPath_EditorDraw(void)
{
    RSDK_THIS(TornadoPath);

    RSDK.SetSpriteAnimation(TornadoPath->aniFrames, 0, &self->animator, true, 7);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TornadoPath_EditorLoad(void)
{
    TornadoPath->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(TornadoPath->aniFrames, 0, &TornadoPath->animator, true, 7);

    RSDK_ACTIVE_VAR(TornadoPath, type);
    RSDK_ENUM_VAR("Start Node", TORNADOPATH_START);
    RSDK_ENUM_VAR("Blank Node", TORNADOPATH_DUD);
    RSDK_ENUM_VAR("Set Tornado Speed", TORNADOPATH_SETSPEED);
    RSDK_ENUM_VAR("Return Player Camera", TORNADOPATH_SETCAMERA);
    RSDK_ENUM_VAR("Disable Player Interactions", TORNADOPATH_DISABLEINTERACTIONS);
    RSDK_ENUM_VAR("Exit Tornado Sequence (Stop AutoScroll)", TORNADOPATH_EXITTORNADO_STOPAUTOSCROLL);
    RSDK_ENUM_VAR("Exit Tornado Sequence (Start Train Sequence)", TORNADOPATH_EXITTORNADO_TRAINSEQ);
    RSDK_ENUM_VAR("Enter Tornado Sequence", TORNADOPATH_ENTERTORNADO);
    RSDK_ENUM_VAR("Enter Tornado Sequence (Fly to Boss Node)", TORNADOPATH_ENTERTORNADO_FLYTOBOSS);
    RSDK_ENUM_VAR("Handle Uber Caterkiller Boss", TORNADOPATH_HANDLEBOSS_MSZ1ST);
    RSDK_ENUM_VAR("Set Tornado Speed (Set High Group)", TORNADOPATH_SETSPEED_HIGH);
    RSDK_ENUM_VAR("Goto \"Stop Tornado\" Node", TORNADOPATH_GOTOSTOP);
    RSDK_ENUM_VAR("Setup MSZ1CutsceneST", TORNADOPATH_SETUPMSZ1CUTSCENEST);
    RSDK_ENUM_VAR("Finish Crash", TORNADOPATH_CRASHFINISH);
}
#endif

void TornadoPath_Serialize(void)
{
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, targetSpeed);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, timer);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_BOOL, easeToSpeed);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_BOOL, fastMode);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(TornadoPath, VAR_ENUM, forceSpeed);
}
