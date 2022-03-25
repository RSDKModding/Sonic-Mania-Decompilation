// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TornadoPath Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
                    && PlayerHelpers_CheckPlayerPos(self->position.x - (self->size.x >> 1), self->position.y - (self->size.y >> 1), 
                                                    self->position.x + (self->size.x >> 1), self->position.y + (self->size.y >> 1))) {
                    TornadoPath_SetupHitbox();
                    self->active       = ACTIVE_NORMAL;
                    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                    if (camera) {
                        camera->state          = 0;
                        camera->position.x     = self->position.x;
                        camera->position.y     = self->position.y;
                        TornadoPath->cameraPtr = camera;
                    }
                    foreach_all(Player, player) { player->camera = NULL; }
                    ScreenInfo->position.y = (self->position.y >> 16) - ScreenInfo->centerY;
                    self->state            = TornadoPath_State_SetTornadoSpeed;
                    self->targetSpeedStore = self->targetSpeed;
                }
                break;
            case TORNADOPATH_DUD: self->active = ACTIVE_NEVER; break;
            case TORNADOPATH_SETSPEED:
            case TORNADOPATH_10:
                self->active = ACTIVE_NEVER;
                self->timer  = 1;
                self->state  = TornadoPath_State_SetTornadoSpeed;
                break;
            case TORNADOPATH_SETCAMERA:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_ReturnCamera;
                break;
            case TORNADOPATH_4:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_State_DisablePlayerInteractions;
                break;
            case TORNADOPATH_5:
            case TORNADOPATH_6:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_Unknown6;
                break;
            case TORNADOPATH_7:
            case TORNADOPATH_8:
                if (StarPost->postIDs[0]
                    || !PlayerHelpers_CheckPlayerPos(self->position.y - (self->size.y >> 1), self->position.x - (self->size.x >> 1),
                                                     self->position.x + (self->size.x >> 1), self->position.y + (self->size.y >> 1))) {
                    self->active           = ACTIVE_XBOUNDS;
                    self->state            = TornadoPath_State_MoveRightJump;
                    self->targetSpeedStore = self->targetSpeed;
                }
                else {
                    self->active           = ACTIVE_NEVER;
                    self->state            = TornadoPath_State_MoveRightJump;
                    self->targetSpeedStore = self->targetSpeed;
                }
                break;
            case TORNADOPATH_9:
                self->active           = ACTIVE_NEVER;
                self->timer            = 1;
                self->state            = TornadoPath_Unknown9;
                self->targetSpeedStore = self->targetSpeed;
                break;
            case TORNADOPATH_11:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_Unknown11;
                break;
            case TORNADOPATH_12:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_Unknown12;
                break;
            case TORNADOPATH_13:
                self->active = ACTIVE_NEVER;
                self->state  = TornadoPath_Unknown13;
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
    self->hitbox.left   = -self->size.x >> 17;
    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.top    = -(self->size.y >> 17);
    self->hitbox.bottom = self->size.y >> 17;
}

void TornadoPath_HandleMoveSpeed(void)
{
    RSDK_THIS(TornadoPath);
    EntityCamera *camera = TornadoPath->cameraPtr;
    if (camera) {
        EntityTornadoPath *node = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, TornadoPath);
        int32 y                   = (camera->position.y - node->position.y) >> 16;
        int32 x                   = (camera->position.x - node->position.x) >> 16;
        self->angle           = RSDK.ATan2(x, y);
        int32 cx                  = camera->position.x & 0xFFFF0000;
        int32 cy                  = camera->position.y & 0xFFFF0000;
        camera->position.x -= self->targetSpeedStore * RSDK.Cos256(self->angle);
        camera->position.y -= self->targetSpeedStore * RSDK.Sin256(self->angle);
        TornadoPath->hitboxID = self->fastMode;
        int32 radius            = self->targetSpeedStore >> 3;
        if (x * x + y * y < radius) {
            self->active = ACTIVE_NEVER;
            node->active   = ACTIVE_NORMAL;
            if (node->easeToSpeed)
                node->targetSpeedStore = self->targetSpeedStore;
            else
                node->targetSpeedStore = self->targetSpeed;
        }
        TornadoPath->moveVel.x = (camera->position.x & 0xFFFF0000) - cx;
        TornadoPath->moveVel.y = (camera->position.y & 0xFFFF0000) - cy;
    }
}

void TornadoPath_State_SetTornadoSpeed(void)
{
    RSDK_THIS(TornadoPath);
    if (self->type == TORNADOPATH_10) {
        foreach_active(Tornado, tornado) { tornado->drawOrder = Zone->playerDrawHigh; }
    }

    if (self->targetSpeedStore >= self->targetSpeed) {
        if (self->targetSpeedStore > self->targetSpeed) {
            self->targetSpeedStore -= 16;
            if (self->targetSpeedStore < self->targetSpeed)
                self->targetSpeedStore = self->targetSpeed;
        }
    }
    else {
        self->targetSpeedStore += 16;
        if (self->targetSpeedStore > self->targetSpeed)
            self->targetSpeedStore = self->targetSpeed;
    }

    if (self->forceSpeed > 0)
        self->targetSpeedStore = self->forceSpeed;

    if (self->timer <= 0) {
        TornadoPath_HandleMoveSpeed();
    }
    else {
        self->timer--;
    }
}

void TornadoPath_State_ReturnCamera(void)
{
    EntityPlayer *player1         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TornadoPath->cameraPtr->state = Camera_State_Follow;
    player1->camera               = TornadoPath->cameraPtr;
}

void TornadoPath_State_DisablePlayerInteractions(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->collisionPlane = 1;
    player1->interaction    = false;

    foreach_active(Tornado, tornado) { tornado->drawOrder = Zone->playerDrawHigh; }

    TornadoPath_HandleMoveSpeed();
    self->state = TornadoPath_State_SetTornadoSpeed;
}

void TornadoPath_Unknown6(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TornadoPath->cameraPtr->state = Camera_State_Follow;

    player1->camera         = TornadoPath->cameraPtr;
    player1->collisionPlane = 0;
    player1->interaction    = true;
    player1->applyJumpCap   = false;
    player1->drawOrder      = Zone->playerDrawLow;
    if (player1->groundedStore)
        Player_StartJump(player1);

    foreach_active(Tornado, tornado)
    {
        if (self->type == 5) {
            tornado->drawOrder = Zone->drawOrderHigh;
            MSZSetup_Unknown4(0);
            tornado->state = Tornado_Unknown8;
        }
        else {
            tornado->drawOrder = Zone->drawOrderLow;
            tornado->state     = Tornado_Unknown9;
        }
    }

    foreach_all(TornadoPath, node)
    {
        bool32 flag = false;
        if (self->type == TORNADOPATH_5)
            flag = node->type == TORNADOPATH_7;
        else
            flag = node->type == TORNADOPATH_8;
        if (flag)
            node->active = ACTIVE_XBOUNDS;
    }

    TornadoPath->cameraPtr = NULL;
    self->active         = ACTIVE_NEVER;
}

void TornadoPath_State_MoveRightJump(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    bool32 flag = false;
    foreach_active(Tornado, tornado)
    {
        if (tornado->state == Tornado_State_PlayerControlled)
            flag = true;
    }

    if (!flag) {
        if (player1->position.x > self->position.x && player1->position.y < self->position.y) {
            self->active      = ACTIVE_NORMAL;
            player1->stateInput = StateMachine_None;
            player1->left       = false;
            player1->right      = true;
            if (player1->pushing > 0) {
                if (player1->onGround)
                    player1->jumpPress = true;
                player1->jumpHold = true;
            }
            if (!player1->onGround) {
                self->state          = TornadoPath_State_FlyOff;
                TornadoPath->moveVel.x = 0;
                TornadoPath->moveVel.y = 0;
            }
        }
    }
}

void TornadoPath_State_FlyOff(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    ++self->timer;
    if (self->timer == 60) {
        foreach_all(Tornado, tornado)
        {
            tornado->position.x = player1->position.x - 0x1400000;
            tornado->position.y = player1->position.y + 0x400000;
            tornado->offsetX   = 0xA0000;
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
    if (!player1->velocity.y && player1->state != Player_State_None) {
        player1->stateInput = Player_ProcessP1Input;
        self->timer       = 0;
        int32 velX            = 0;
        int32 velY            = 0;
        if (self->type == TORNADOPATH_8) {
            foreach_all(TornadoPath, node)
            {
                if (node->type == TORNADOPATH_9) {
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
            camera->state          = StateMachine_None;
            TornadoPath->cameraPtr = camera;
            player1->camera        = NULL;
        }

        foreach_active(Tornado, tornado)
        {
            tornado->position.x += velX;
            tornado->position.y += velY;
            tornado->offsetX = 0x80000;
            tornado->showFlame = false;
        }
        self->state = TornadoPath_State_SetTornadoSpeed;
    }
}

void TornadoPath_Unknown9(void)
{
    RSDK_THIS(TornadoPath);
    self->state            = TornadoPath_Unknown10;
    self->targetSpeedStore = 0;
    TornadoPath->moveVel.x   = 0;
    TornadoPath->moveVel.y   = 0;
    if (UberCaterkiller->defeated)
       self->state = TornadoPath_State_SetTornadoSpeed;
}

void TornadoPath_Unknown10(void)
{
    RSDK_THIS(TornadoPath);
    self->targetSpeedStore = 0;
    TornadoPath->moveVel.x   = 0;
    TornadoPath->moveVel.y   = 0;
    if (UberCaterkiller->defeated)
       self->state = TornadoPath_State_SetTornadoSpeed;
}

void TornadoPath_Unknown11(void)
{
    RSDK_THIS(TornadoPath);
    EntityPlayer *player1    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    self->targetSpeedStore = 0;
    TornadoPath->moveVel.x   = 0;
    TornadoPath->moveVel.y   = 0;
    if (player1->onGround) {
        int32 velocityX = 0;
        int32 velocityY = 0;
        foreach_all(TornadoPath, node)
        {
            if (node->type == TORNADOPATH_9) {
                velocityX    = node->position.x - player1->position.x;
                velocityY    = node->position.y - player1->position.y;
                node->active = ACTIVE_NORMAL;
            }
        }
        player1->position.x += velocityX;
        player1->position.y += velocityY;
        EntityCamera *camera = TornadoPath->cameraPtr;
        camera->position.x += velocityX;
        camera->position.y += velocityY;
        foreach_active(Tornado, tornado)
        {
            tornado->position.x += velocityX;
            tornado->position.y += velocityY;
            tornado->offsetX = 0x80000;
            tornado->showFlame = false;
        }
    }
}

void TornadoPath_Unknown12(void)
{
    RSDK_THIS(TornadoPath);
    if (!self->timer) {
        foreach_all(MSZCutsceneST, cutscene)
        {
            cutscene->active = ACTIVE_NORMAL;
            cutscene->nodePtr = (Entity *)self;
        }
    }
}

void TornadoPath_Unknown13(void)
{
    TornadoPath->moveVel.x = 0;
    TornadoPath->moveVel.y = 0;
    TornadoPath->flag  = true;
}

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
    RSDK_ENUM_VAR("Type 4", TORNADOPATH_4);
    RSDK_ENUM_VAR("Type 5", TORNADOPATH_5);
    RSDK_ENUM_VAR("Type 6", TORNADOPATH_6);
    RSDK_ENUM_VAR("Type 7", TORNADOPATH_7);
    RSDK_ENUM_VAR("Type 8", TORNADOPATH_8);
    RSDK_ENUM_VAR("Type 9", TORNADOPATH_9);
    RSDK_ENUM_VAR("Type 10", TORNADOPATH_10);
    RSDK_ENUM_VAR("Type 11", TORNADOPATH_11);
    RSDK_ENUM_VAR("Type 12", TORNADOPATH_12);
    RSDK_ENUM_VAR("Type 13", TORNADOPATH_13);
}

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
