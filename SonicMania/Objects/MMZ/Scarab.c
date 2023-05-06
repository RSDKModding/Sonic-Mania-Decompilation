// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Scarab Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectScarab *Scarab;

void Scarab_Update(void)
{
    RSDK_THIS(Scarab);

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->backLegAnimator);
    RSDK.ProcessAnimation(&self->frontLegAnimator);

    self->moveOffset.x = -self->position.x;
    self->moveOffset.y = -self->position.y;

    StateMachine_Run(self->state);

    self->moveOffset.x += self->position.x;
    self->moveOffset.y += self->position.y;

    Scarab_CheckPlayerCollisions();
    Scarab_HandleChildMove();
    Scarab_HandlePlayerGrab();

    if (self->state != Scarab_State_Init) {
        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            int32 x = -self->position.x;
            int32 y = -self->position.y;

            self->direction    = self->startDir;
            self->position     = self->startPos;
            self->moveOffset.x = self->position.x + x;
            self->moveOffset.y = self->position.y + y;

            Scarab_HandleChildMove();
            Scarab_Create(NULL);
        }
    }
}

void Scarab_LateUpdate(void) {}

void Scarab_StaticUpdate(void)
{
    foreach_active(Scarab, scarab) { RSDK.AddDrawListRef(scarab->drawGroupHigh, RSDK.GetEntitySlot(scarab)); }
}

void Scarab_Draw(void)
{
    RSDK_THIS(Scarab);

    if (SceneInfo->currentDrawGroup == self->drawGroupHigh) {
        RSDK.DrawSprite(&self->frontLegAnimator, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->backLegAnimator, NULL, false);
        RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    }
}

void Scarab_Create(void *data)
{
    RSDK_THIS(Scarab);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (self->planeFilter == PLANEFILTER_NONE) {
        self->drawGroupLow  = Zone->objectDrawGroup[0];
        self->drawGroupHigh = Zone->playerDrawGroup[1];
    }
    else if ((uint8)(self->planeFilter - 1) & 2) {
        self->drawGroupLow  = Zone->objectDrawGroup[1];
        self->drawGroupHigh = Zone->playerDrawGroup[1];
    }
    else {
        self->drawGroupLow  = Zone->objectDrawGroup[0];
        self->drawGroupHigh = Zone->playerDrawGroup[0];
    }

    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!self->amplitude)
        self->amplitude = 100;

    self->startPos = self->position;
    self->startDir = self->direction;
    self->state    = Scarab_State_Init;
}

void Scarab_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        Scarab->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Scarab.bin", SCOPE_STAGE);

    Scarab->hitboxBadnik.left   = -16;
    Scarab->hitboxBadnik.top    = -14;
    Scarab->hitboxBadnik.right  = 14;
    Scarab->hitboxBadnik.bottom = 6;

    Scarab->hitboxGrab.left   = -48;
    Scarab->hitboxGrab.top    = -14;
    Scarab->hitboxGrab.right  = -17;
    Scarab->hitboxGrab.bottom = 6;

    Scarab->active = ACTIVE_ALWAYS;

    DEBUGMODE_ADD_OBJ(Scarab);
}

void Scarab_DebugSpawn(void)
{
    RSDK_THIS(Scarab);

    EntityScarab *scarab = CREATE_ENTITY(Scarab, NULL, self->position.x, self->position.y);
    scarab->direction    = self->direction;
    scarab->startDir     = self->direction;
}

void Scarab_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 4, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Scarab_CheckPlayerCollisions(void)
{
    RSDK_THIS(Scarab);

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
            int32 playerID = RSDK.GetEntitySlot(player);
            if (Player_CheckBadnikTouch(player, self, &Scarab->hitboxBadnik)) {
                Scarab_HandlePlayerRelease();
                Player_CheckBadnikBreak(player, self, true);
            }

            if (!self->childCount) {
                if (Player_CheckCollisionTouch(player, self, &Scarab->hitboxGrab)) {
                    if (!((1 << playerID) & self->grabbedPlayers) && !self->playerTimers[playerID]) {
                        self->grabbedPlayers |= 1 << playerID;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                        player->groundVel       = 0;
                        player->onGround        = false;
                        player->tileCollisions  = TILECOLLISION_NONE;
                        player->state           = Player_State_Static;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        self->isPermanent       = true;
                    }
                }
            }
        }
    }
}

void Scarab_HandleChildMove(void)
{
    RSDK_THIS(Scarab);

    int32 slot = SceneInfo->entitySlot + 1;
    for (int32 c = 0; c < self->childCount; ++c) {
        EntityItemBox *child = RSDK_GET_ENTITY(slot + c, ItemBox);
        child->position.x += self->moveOffset.x;
        child->position.y += self->moveOffset.y;

        if (child->classID == ItemBox->classID)
            child->moveOffset = self->moveOffset;
    }
}

void Scarab_HandlePlayerGrab(void)
{
    RSDK_THIS(Scarab);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if ((1 << playerID) & self->grabbedPlayers) {
            if (self->playerTimers[playerID] < 60 && player->interaction && player->state == Player_State_Static) {
                player->position.x = self->position.x + ((2 * (self->direction != FLIP_NONE) - 1) << 21);
                player->position.y = self->position.y - 0xA0000;
                player->velocity.x = self->state == Scarab_State_Move ? (0x6000 * self->moveDir) : 0;
                player->velocity.y = 0;
                ++self->playerTimers[playerID];
            }
            else {
                self->grabbedPlayers &= ~(1 << playerID);
                self->playerTimers[playerID] = -10;

                if (player->state != Player_State_FlyToPlayer && player->state != Player_State_ReturnToPlayer) {
                    player->tileCollisions = TILECOLLISION_DOWN;
                    player->state          = Player_State_Air;
                    player->velocity.x     = 0xA0000 * (2 * (self->direction != FLIP_NONE) - 1);
                    player->velocity.y     = 0;
                }

                self->isPermanent = false;
            }
        }
        else {
            if (self->playerTimers[playerID] < 0)
                self->playerTimers[playerID]++;
        }
    }
}

void Scarab_HandlePlayerRelease(void)
{
    RSDK_THIS(Scarab);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((1 << playerID) & self->grabbedPlayers) {
            self->grabbedPlayers &= ~(1 << playerID);
            self->playerTimers[playerID] = -10;

            if (player->state != Player_State_FlyToPlayer && player->state != Player_State_ReturnToPlayer) {
                player->tileCollisions = TILECOLLISION_DOWN;
                player->state          = Player_State_Air;
                player->velocity.x     = 0xA0000 * (2 * (self->direction != FLIP_NONE) - 1);
                player->velocity.y     = 0;
            }

            self->isPermanent = false;
        }
    }
}

void Scarab_State_Init(void)
{
    RSDK_THIS(Scarab);

    self->active    = ACTIVE_NORMAL;
    self->state     = Scarab_State_Move;
    self->pullCount = 0;
    self->timer     = 0;
    self->moveDir   = 2 * (self->direction != FLIP_NONE) - 1;

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 1, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 5, &self->backLegAnimator, true, 0);
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 3, &self->frontLegAnimator, true, 0);

    Scarab_State_Move();
}

void Scarab_State_Move(void)
{
    RSDK_THIS(Scarab);

    if (self->frontLegAnimator.frameID == self->frontLegAnimator.frameCount - 1)
        ++self->pullCount;

    if (self->pullCount == 9) {
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 4, &self->backLegAnimator, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 2, &self->frontLegAnimator, true, 0);
        self->timer = 45;
        self->state = Scarab_State_Wait;
    }

    int32 x = self->position.x + 0x6000 * self->moveDir;
    int32 y = self->position.y;

    self->position.x = x;
    if (abs(x - self->startPos.x) < (self->amplitude << 16) && RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x60000, 2)) {
        self->position.x = x;
    }
    else {
        self->position.y = y;
        self->position.x = self->startPos.x + (self->amplitude << 16) * self->moveDir;
        self->moveDir    = -self->moveDir;
    }

    RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x60000, 4);
}

void Scarab_State_Wait(void)
{
    RSDK_THIS(Scarab);

    if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 1, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 5, &self->backLegAnimator, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 3, &self->frontLegAnimator, true, 0);

        self->pullCount = 0;
        self->state     = Scarab_State_Move;
    }
    else {
        self->timer--;
    }
}

#if GAME_INCLUDE_EDITOR
void Scarab_EditorDraw(void)
{
    RSDK_THIS(Scarab);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 4, &self->backLegAnimator, true, 0);
    RSDK.DrawSprite(&self->backLegAnimator, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 2, &self->frontLegAnimator, true, 0);
    RSDK.DrawSprite(&self->frontLegAnimator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Scarab_EditorLoad(void)
{
    Scarab->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Scarab.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Scarab, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);

    RSDK_ACTIVE_VAR(Scarab, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Scarab_Serialize(void)
{
    RSDK_EDITABLE_VAR(Scarab, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Scarab, VAR_INT32, amplitude);
    RSDK_EDITABLE_VAR(Scarab, VAR_UINT8, childCount);
    RSDK_EDITABLE_VAR(Scarab, VAR_ENUM, planeFilter);
}
