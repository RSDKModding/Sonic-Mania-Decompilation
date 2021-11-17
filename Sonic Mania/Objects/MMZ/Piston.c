#include "SonicMania.h"

ObjectPiston *Piston;

void Piston_Update(void)
{
    RSDK_THIS(Piston);
    if (entity->state == Piston_WaitForInterval && !((Zone->timer + entity->intervalOffset) % entity->interval)) {
        entity->moveTimer = entity->distance >> 3;
        entity->active    = ACTIVE_NORMAL;
        switch (entity->spawnType - 1) {
            case 0: entity->state = Piston_Launch; break;
            case 1: entity->state = Piston_Down; break;
            case 2: entity->state = Piston_Up; break;
            case 3: entity->state = Piston_Right; break;
            case 4: entity->state = Piston_Left; break;
            case 5: entity->state = Piston_RightOrLeft; break;
            default: entity->state = Piston_DownOrUp; break;
        }
    }
    Platform_Update();
}

void Piston_LateUpdate(void) {}

void Piston_StaticUpdate(void) {}

void Piston_Draw(void)
{
    RSDK_THIS(Piston);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void Piston_Create(void *data)
{
    RSDK_THIS(Piston);
    int32 type              = entity->type;
    entity->spawnType     = type;
    entity->type            = PLATFORM_0;
    entity->collisionType   = PLATFORM_C_1;
    if (type > 3)
        entity->size += 3;
    Platform_Create(NULL);
    if (type <= 3) {
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = (entity->distance + 128) << 16;
    }
    else {
        entity->updateRange.x = (entity->distance + 128) << 16;
        entity->updateRange.y = 0x800000;
    }
    entity->type = type;
    if (entity->type == 3) {
        entity->state        = Piston_WaitForPlayers;
        entity->stateCollide = Piston_StateCollide_Solid;
    }
    else
        entity->state = Piston_WaitForInterval;
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
        if (((1 << i) & entity->stoodPlayers) != 0) {
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
    entity->drawPos.y += 0x80000;
    entity->velocity.y = 0x80000;
    if (--entity->moveTimer <= 0) {
        entity->spawnType = 3;
        entity->active    = ACTIVE_BOUNDS;
        entity->state     = Piston_WaitForInterval;
    }
}

void Piston_Up(void)
{
    RSDK_THIS(Piston);
    entity->drawPos.y -= 0x80000;
    entity->velocity.y = 0x80000;
    if (--entity->moveTimer <= 0) {
        entity->spawnType = 2;
        entity->active    = ACTIVE_BOUNDS;
        entity->state     = Piston_WaitForInterval;
    }
}

void Piston_DownOrUp(void)
{
    RSDK_THIS(Piston);
    entity->velocity.y = 0x80000;
    if (!entity->reverse)
        entity->drawPos.y += 0x80000;
    else
        entity->drawPos.y -= 0x80000;
    if (--entity->moveTimer <= 0) {
        entity->moveTimer = entity->distance;
        entity->state     = Piston_UpOrDown;
    }
}

void Piston_UpOrDown(void)
{
    RSDK_THIS(Piston);
    entity->velocity.y = 0x10000;
    if (!entity->reverse)
        entity->drawPos.y += 0x10000;
    else
        entity->drawPos.y -= 0x10000;
    if (--entity->moveTimer <= 0) {
        entity->moveTimer = ACTIVE_BOUNDS;
        entity->state     = Piston_WaitForInterval;
    }
}

void Piston_Launch(void)
{
    RSDK_THIS(Piston);
    entity->drawPos.y -= 0x80000;
    entity->velocity.y = 0x80000;
    if (--entity->moveTimer <= 0) {
        for (int32 i = 0; i < Player->playerCount; ++i) {
            EntityPlayer* player = RSDK_GET_ENTITY(i, Player);
            if (((1 << i) & entity->stoodPlayers) != 0) {
                player->velocity.y = -0x100000;
                player->state      = Player_State_Air;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
            }
        }
        entity->moveTimer = entity->distance;
        entity->state     = Piston_Pullback;
    }
}

void Piston_Pullback(void)
{
    RSDK_THIS(Piston);
    entity->drawPos.y += 0x10000;
    entity->velocity.y = 0x10000;
    if (--entity->moveTimer <= 0) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = Piston_WaitForInterval;
    }
}

void Piston_WaitForPlayers(void)
{
    RSDK_THIS(Piston);
    if (entity->stoodPlayers)
        entity->state = Piston_PrepareLaunch;
}

void Piston_PrepareLaunch(void)
{
    RSDK_THIS(Piston);
    if (entity->stoodPlayers) {
        if (!entity->moveTimer)
            RSDK.PlaySfx(Piston->landSFX, false, 255);
        entity->drawPos.y += 0x10000;
        entity->velocity.y = 0x10000;
        if (++entity->moveTimer >= 16) {
            RSDK.StopSFX(Piston->landSFX);
            RSDK.PlaySfx(Piston->launchSFX, false, 255);
            entity->state = Piston_LaunchAndWait;
        }
    }
    else {
        entity->state = Piston_PullbackOrLaunch;
        RSDK.StopSFX(Piston->landSFX);
    }
}

void Piston_PullbackOrLaunch()
{
    RSDK_THIS(Piston);
    if (entity->stoodPlayers) {
        entity->state = Piston_PrepareLaunch;
    }
    else {
        entity->drawPos.y -= 0x10000;
        entity->velocity.y = 0x10000;
        if (--entity->moveTimer <= 0)
            entity->state = Piston_WaitForPlayers;
    }
}

void Piston_LaunchAndWait(void)
{
    RSDK_THIS(Piston);
    entity->drawPos.y -= 0x40000;
    entity->velocity.y = 0x40000;
    entity->moveTimer -= 4;
    if (entity->moveTimer == 4) {
        for (int32 i = 0; i < Player->playerCount; ++i) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if (((1 << i) & entity->stoodPlayers) != 0) {
                RSDK.PlaySfx(Piston->launchSFX, false, 255);
                player->velocity.y = -0x100000;
                player->state      = Player_State_Air;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->playerAnimator, true, 0);
            }
        }
        entity->state = Piston_WaitForPlayers;
    }
}

void Piston_Right(void)
{
    RSDK_THIS(Piston);
    entity->drawPos.x += 0x80000;
    entity->velocity.x = 0x80000;
    if (--entity->moveTimer <= 0) {
        entity->spawnType = 5;
        entity->active    = ACTIVE_BOUNDS;
        entity->state     = Piston_WaitForInterval;
    }
}

void Piston_Left(void)
{
    RSDK_THIS(Piston);
    entity->drawPos.x -= 0x80000;
    entity->velocity.x = 0x80000;
    if (--entity->moveTimer <= 0) {
        entity->spawnType = 4;
        entity->active    = ACTIVE_BOUNDS;
        entity->state     = Piston_WaitForInterval;
    }
}

void Piston_RightOrLeft(void)
{
    RSDK_THIS(Piston);
    entity->velocity.x = 0x80000;
    if (!entity->reverse)
        entity->drawPos.x += 0x80000;
    else
        entity->drawPos.x -= 0x80000;
    if (--entity->moveTimer <= 0) {
        entity->moveTimer = entity->distance;
        entity->state     = Piston_LeftOrRight;
    }
}

void Piston_LeftOrRight(void)
{
    RSDK_THIS(Piston);
    entity->velocity.x = 0x10000;
    if (!entity->reverse)
        entity->drawPos.x += 0x10000;
    else
        entity->drawPos.x -= 0x10000;
    if (--entity->moveTimer <= 0) {
        entity->moveTimer = ACTIVE_BOUNDS;
        entity->state     = Piston_WaitForInterval;
    }
}
