#include "SonicMania.h"

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
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &self->animator, true, 0);
    self->state = Batbrain_State_Setup;
}

void Batbrain_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Batbrain->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Batbrain.bin", SCOPE_STAGE);
    Batbrain->hitbox.left   = -12;
    Batbrain->hitbox.top    = -18;
    Batbrain->hitbox.right  = 12;
    Batbrain->hitbox.bottom = 18;
    Batbrain->sfxFlap       = RSDK.GetSfx("Stage/Flap.wav");

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

void Batbrain_CheckHit(void)
{
    RSDK_THIS(Batbrain);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Batbrain->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Batbrain_CheckOnScreen(void)
{
    RSDK_THIS(Batbrain);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        Batbrain_Create(NULL);
    }
}

void Batbrain_State_Setup(void)
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
    int32 targetPos           = 0x7FFFFFFF;
    EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *playerPtr = NULL;

    foreach_active(Player, player)
    {
        int32 distance = abs(player->position.x - self->position.x);
        if (distance >= targetPos) {
            player1 = player;
        }
        else {
            player1   = player;
            targetPos = distance;
        }
        if (distance < 0x800000) {
            if (player->position.y >= self->position.y && (!playerPtr || player->position.y < playerPtr->position.y))
                playerPtr = player;
        }
    }

    if (playerPtr) {
        player       = playerPtr;
        int32 distance = playerPtr->position.y - self->position.y;
        if (distance >= 0 && distance <= 0x800000 && !RSDK.Rand(0, 8)) {
            self->state   = Batbrain_State_DropToPlayer;
            self->targetY = player->position.y;
            self->target  = (Entity*)player;
            RSDK.SetSpriteAnimation(Batbrain->aniFrames, 1, &self->animator, true, 0);
        }
    }
    else {
        player = player1;
    }
    self->direction = player->position.x >= self->position.x;
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
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
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
}

void Batbrain_State_Fly(void)
{
    RSDK_THIS(Batbrain);
    Entity *target = self->target;
    self->position.x += self->velocity.x;

    if (abs(target->position.x - self->position.x) >= 0x800000 && !RSDK.Rand(0, 8))
        self->state = Batbrain_State_FlyToCeiling;

    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(Batbrain->sfxFlap, false, 255);

    RSDK.ProcessAnimation(&self->animator);
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
}

void Batbrain_State_FlyToCeiling(void)
{
    RSDK_THIS(Batbrain);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, 0, -0xC0000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = Batbrain_State_CheckPlayerInRange;
        RSDK.SetSpriteAnimation(Batbrain->aniFrames, 0, &self->animator, true, 0);
    }

    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(Batbrain->sfxFlap, false, 255);

    RSDK.ProcessAnimation(&self->animator);
    Batbrain_CheckHit();
    Batbrain_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Batbrain_EditorDraw(void) { Batbrain_Draw(); }

void Batbrain_EditorLoad(void) { Batbrain->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Batbrain.bin", SCOPE_STAGE); }
#endif

void Batbrain_Serialize(void) {}
