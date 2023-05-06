// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Batbrain Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBatbrain *Batbrain;

void Batbrain_Update(void)
{
    RSDK_THIS(Batbrain);
    StateMachine_Run(self->state);
}

void Batbrain_LateUpdate(void) {}

void Batbrain_StaticUpdate(void) {}

void Batbrain_Draw(void)
{
    RSDK_THIS(Batbrain);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Batbrain_Create(void *data)
{
    RSDK_THIS(Batbrain);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &self->animator, true, 0);
    self->state = Batbrain_State_Init;
}

void Batbrain_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Batbrain->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Batbrain.bin", SCOPE_STAGE);

    Batbrain->hitboxBadnik.left   = -12;
    Batbrain->hitboxBadnik.top    = -18;
    Batbrain->hitboxBadnik.right  = 12;
    Batbrain->hitboxBadnik.bottom = 18;

    Batbrain->sfxFlap = RSDK.GetSfx("Stage/Flap.wav");

    DEBUGMODE_ADD_OBJ(Batbrain);
}

void Batbrain_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Batbrain_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Batbrain, NULL, self->position.x, self->position.y);
}

void Batbrain_CheckPlayerCollisions(void)
{
    RSDK_THIS(Batbrain);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Batbrain->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Batbrain_CheckOffScreen(void)
{
    RSDK_THIS(Batbrain);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        Batbrain_Create(NULL);
    }
}

void Batbrain_State_Init(void)
{
    RSDK_THIS(Batbrain);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = 0;
    self->velocity.y = 0;
    self->state      = Batbrain_State_CheckPlayerInRange;
    Batbrain_State_CheckPlayerInRange();
}

void Batbrain_State_CheckPlayerInRange(void)
{
    RSDK_THIS(Batbrain);

    int32 targetDistance       = 0x7FFFFFFF;
    EntityPlayer *backupPlayer = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *targetPlayer = NULL;

    foreach_active(Player, player)
    {
        int32 distance = abs(player->position.x - self->position.x);
        if (distance >= targetDistance) {
            backupPlayer = player;
        }
        else {
            backupPlayer   = player;
            targetDistance = distance;
        }

        if (distance < 0x800000) {
            if (player->position.y >= self->position.y && (!targetPlayer || player->position.y < targetPlayer->position.y))
                targetPlayer = player;
        }
    }

    EntityPlayer *playerPtr = backupPlayer;
    if (targetPlayer) {
        playerPtr      = targetPlayer;
        int32 distance = targetPlayer->position.y - self->position.y;
        if (distance >= 0 && distance <= 0x800000 && !RSDK.Rand(0, 8)) {
            self->state   = Batbrain_State_DropToPlayer;
            self->targetY = playerPtr->position.y;
            self->target  = playerPtr;
            RSDK.SetSpriteAnimation(Batbrain->aniFrames, 1, &self->animator, true, 0);
        }
    }

    self->direction = playerPtr->position.x >= self->position.x;

    Batbrain_CheckPlayerCollisions();
    Batbrain_CheckOffScreen();
}

void Batbrain_State_DropToPlayer(void)
{
    RSDK_THIS(Batbrain);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;
    self->direction = self->target->position.x >= self->position.x;

    if (self->targetY - self->position.y < 0x100000) {
        self->velocity.y = 0;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x10000;
        else
            self->velocity.x = 0x10000;
        RSDK.SetSpriteAnimation(Batbrain->aniFrames, 2, &self->animator, true, 0);
        self->state = Batbrain_State_Fly;
    }

    RSDK.ProcessAnimation(&self->animator);

    Batbrain_CheckPlayerCollisions();
    Batbrain_CheckOffScreen();
}

void Batbrain_State_Fly(void)
{
    RSDK_THIS(Batbrain);

    EntityPlayer *target = self->target;
    self->position.x += self->velocity.x;

    if (abs(target->position.x - self->position.x) >= 0x800000 && !RSDK.Rand(0, 8))
        self->state = Batbrain_State_FlyToCeiling;

    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(Batbrain->sfxFlap, false, 255);

    RSDK.ProcessAnimation(&self->animator);

    Batbrain_CheckPlayerCollisions();
    Batbrain_CheckOffScreen();
}

void Batbrain_State_FlyToCeiling(void)
{
    RSDK_THIS(Batbrain);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1800;

    // RWALL? shouldn't this be ROOF? is this a holdover from v4's weird id system?
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, 0, -0xC0000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = Batbrain_State_CheckPlayerInRange;
        RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &self->animator, true, 0);
    }

    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(Batbrain->sfxFlap, false, 255);

    RSDK.ProcessAnimation(&self->animator);

    Batbrain_CheckPlayerCollisions();
    Batbrain_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Batbrain_EditorDraw(void) { Batbrain_Draw(); }

void Batbrain_EditorLoad(void) { Batbrain->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Batbrain.bin", SCOPE_STAGE); }
#endif

void Batbrain_Serialize(void) {}
