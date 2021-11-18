#include "SonicMania.h"

ObjectPiston *Piston;

void Piston_Update(void)
{
    RSDK_THIS(Piston);
    if (self->state == Piston_WaitForInterval && !((Zone->timer + self->intervalOffset) % self->interval)) {
        self->moveTimer = self->distance >> 3;
        self->active    = ACTIVE_NORMAL;
        switch (self->spawnType - 1) {
            case 0: self->state = Piston_Launch; break;
            case 1: self->state = Piston_Down; break;
            case 2: self->state = Piston_Up; break;
            case 3: self->state = Piston_Right; break;
            case 4: self->state = Piston_Left; break;
            case 5: self->state = Piston_RightOrLeft; break;
            default: self->state = Piston_DownOrUp; break;
        }
    }
    Platform_Update();
}

void Piston_LateUpdate(void) {}

void Piston_StaticUpdate(void) {}

void Piston_Draw(void)
{
    RSDK_THIS(Piston);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void Piston_Create(void *data)
{
    RSDK_THIS(Piston);
    int32 type              = self->type;
    self->spawnType     = type;
    self->type            = PLATFORM_FIXED;
    self->collisionType   = PLATFORM_C_1;
    if (type > 3)
        self->size += 3;
    Platform_Create(NULL);
    if (type <= 3) {
        self->updateRange.x = 0x800000;
        self->updateRange.y = (self->distance + 128) << 16;
    }
    else {
        self->updateRange.x = (self->distance + 128) << 16;
        self->updateRange.y = 0x800000;
    }
    self->type = type;
    if (self->type == 3) {
        self->state        = Piston_WaitForPlayers;
        self->stateCollide = Piston_StateCollide_Solid;
    }
    else
        self->state = Piston_WaitForInterval;
}

void Piston_StageLoad(void)
{
    Piston->landSFX   = RSDK.GetSFX("MMZ/PistonLand.wav");
    Piston->launchSFX = RSDK.GetSFX("MMZ/PistonLaunch.wav");
}

void Piston_EditorDraw(void) { Piston_Draw(); }

void Piston_EditorLoad(void)
{ // unsure of what to do here
}

void Piston_Serialize(void)
{
    RSDK_EDITABLE_VAR(Piston, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Piston, VAR_BOOL, reverse);
    RSDK_EDITABLE_VAR(Piston, VAR_UINT8, size);
    RSDK_EDITABLE_VAR(Piston, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(Piston, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Piston, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Piston, VAR_ENUM, distance);
}

void Piston_WaitForInterval(void)
{
    // this obj was fun to do and is also very weird
    // there's so many fucking states but they're so tiny and almost exactly the same thing as other states
    // i don't like the naming though maybe i go thru it again later
}

void Piston_StateCollide_Solid(void)
{
    RSDK_THIS(Piston);
    Platform_CollisionState_AllSolid();
#if RETRO_USE_PLUS
    for (int32 i = 0; i < Player->playerCount; ++i) {
        if (((1 << i) & self->stoodPlayers) != 0) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
        }
    }
#endif
}

void Piston_Down(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y += 0x80000;
    self->velocity.y = 0x80000;
    if (--self->moveTimer <= 0) {
        self->spawnType = 3;
        self->active    = ACTIVE_BOUNDS;
        self->state     = Piston_WaitForInterval;
    }
}

void Piston_Up(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y -= 0x80000;
    self->velocity.y = 0x80000;
    if (--self->moveTimer <= 0) {
        self->spawnType = 2;
        self->active    = ACTIVE_BOUNDS;
        self->state     = Piston_WaitForInterval;
    }
}

void Piston_DownOrUp(void)
{
    RSDK_THIS(Piston);
    self->velocity.y = 0x80000;
    if (!self->reverse)
        self->drawPos.y += 0x80000;
    else
        self->drawPos.y -= 0x80000;
    if (--self->moveTimer <= 0) {
        self->moveTimer = self->distance;
        self->state     = Piston_UpOrDown;
    }
}

void Piston_UpOrDown(void)
{
    RSDK_THIS(Piston);
    self->velocity.y = 0x10000;
    if (!self->reverse)
        self->drawPos.y += 0x10000;
    else
        self->drawPos.y -= 0x10000;
    if (--self->moveTimer <= 0) {
        self->moveTimer = ACTIVE_BOUNDS;
        self->state     = Piston_WaitForInterval;
    }
}

void Piston_Launch(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y -= 0x80000;
    self->velocity.y = 0x80000;
    if (--self->moveTimer <= 0) {
        for (int32 i = 0; i < Player->playerCount; ++i) {
            EntityPlayer* player = RSDK_GET_ENTITY(i, Player);
            if (((1 << i) & self->stoodPlayers) != 0) {
                player->velocity.y = -0x100000;
                player->state      = Player_State_Air;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
            }
        }
        self->moveTimer = self->distance;
        self->state     = Piston_Pullback;
    }
}

void Piston_Pullback(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y += 0x10000;
    self->velocity.y = 0x10000;
    if (--self->moveTimer <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Piston_WaitForInterval;
    }
}

void Piston_WaitForPlayers(void)
{
    RSDK_THIS(Piston);
    if (self->stoodPlayers)
        self->state = Piston_PrepareLaunch;
}

void Piston_PrepareLaunch(void)
{
    RSDK_THIS(Piston);
    if (self->stoodPlayers) {
        if (!self->moveTimer)
            RSDK.PlaySfx(Piston->landSFX, false, 255);
        self->drawPos.y += 0x10000;
        self->velocity.y = 0x10000;
        if (++self->moveTimer >= 16) {
            RSDK.StopSFX(Piston->landSFX);
            RSDK.PlaySfx(Piston->launchSFX, false, 255);
            self->state = Piston_LaunchAndWait;
        }
    }
    else {
        self->state = Piston_PullbackOrLaunch;
        RSDK.StopSFX(Piston->landSFX);
    }
}

void Piston_PullbackOrLaunch()
{
    RSDK_THIS(Piston);
    if (self->stoodPlayers) {
        self->state = Piston_PrepareLaunch;
    }
    else {
        self->drawPos.y -= 0x10000;
        self->velocity.y = 0x10000;
        if (--self->moveTimer <= 0)
            self->state = Piston_WaitForPlayers;
    }
}

void Piston_LaunchAndWait(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y -= 0x40000;
    self->velocity.y = 0x40000;
    self->moveTimer -= 4;
    if (self->moveTimer == 4) {
        for (int32 i = 0; i < Player->playerCount; ++i) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if (((1 << i) & self->stoodPlayers) != 0) {
                RSDK.PlaySfx(Piston->launchSFX, false, 255);
                player->velocity.y = -0x100000;
                player->state      = Player_State_Air;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
            }
        }
        self->state = Piston_WaitForPlayers;
    }
}

void Piston_Right(void)
{
    RSDK_THIS(Piston);
    self->drawPos.x += 0x80000;
    self->velocity.x = 0x80000;
    if (--self->moveTimer <= 0) {
        self->spawnType = 5;
        self->active    = ACTIVE_BOUNDS;
        self->state     = Piston_WaitForInterval;
    }
}

void Piston_Left(void)
{
    RSDK_THIS(Piston);
    self->drawPos.x -= 0x80000;
    self->velocity.x = 0x80000;
    if (--self->moveTimer <= 0) {
        self->spawnType = 4;
        self->active    = ACTIVE_BOUNDS;
        self->state     = Piston_WaitForInterval;
    }
}

void Piston_RightOrLeft(void)
{
    RSDK_THIS(Piston);
    self->velocity.x = 0x80000;
    if (!self->reverse)
        self->drawPos.x += 0x80000;
    else
        self->drawPos.x -= 0x80000;
    if (--self->moveTimer <= 0) {
        self->moveTimer = self->distance;
        self->state     = Piston_LeftOrRight;
    }
}

void Piston_LeftOrRight(void)
{
    RSDK_THIS(Piston);
    self->velocity.x = 0x10000;
    if (!self->reverse)
        self->drawPos.x += 0x10000;
    else
        self->drawPos.x -= 0x10000;
    if (--self->moveTimer <= 0) {
        self->moveTimer = ACTIVE_BOUNDS;
        self->state     = Piston_WaitForInterval;
    }
}
