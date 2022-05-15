// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Piston Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPiston *Piston;

void Piston_Update(void)
{
    RSDK_THIS(Piston);
    if (self->state == Piston_State_WaitForInterval && !((Zone->timer + self->intervalOffset) % self->interval)) {
        self->timer  = self->distance >> 3;
        self->active = ACTIVE_NORMAL;
        switch (self->pistonType) {
            default:
            case PISTON_MOVE_VERTICAL: self->state = Piston_StateMove_Vertical; break;
            case PISTON_UP: self->state = Piston_StateMove_Up; break;
            case PISTON_MOVE_DOWN: self->state = Piston_StateMove_Down; break;
            case PISTON_MOVE_DOWN_REVERSE: self->state = Piston_StateMove_Down_Reverse; break;
            case PISTON_MOVE_RIGHT: self->state = Piston_StateMove_Right; break;
            case PISTON_MOVE_LEFT: self->state = Piston_StateMove_Left; break;
            case PISTON_MOVE_HORIZONTAL: self->state = Piston_StateMove_Horizontal; break;
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
    self->pistonType = self->type;
    self->type       = PLATFORM_FIXED;
    self->collision  = PLATFORM_C_SOLID_ALL;
    if (self->pistonType > PISTON_MOVE_DOWN_REVERSE)
        self->size += 3;

    Platform_Create(NULL);
    if (self->pistonType <= PISTON_MOVE_DOWN_REVERSE) {
        self->updateRange.x = 0x800000;
        self->updateRange.y = (self->distance + 128) << 16;
    }
    else {
        self->updateRange.x = (self->distance + 128) << 16;
        self->updateRange.y = 0x800000;
    }

    self->type = self->pistonType;
    if (self->type == PISTON_MOVE_DOWN_REVERSE) {
        self->state        = Piston_StateActive_WaitForStood;
        self->stateCollide = Piston_Collide_Solid;
    }
    else {
        self->state = Piston_State_WaitForInterval;
    }
}

void Piston_StageLoad(void)
{
    Piston->sfxLand   = RSDK.GetSfx("MMZ/PistonLand.wav");
    Piston->sfxLaunch = RSDK.GetSfx("MMZ/PistonLaunch.wav");
}

void Piston_Collide_Solid(void)
{
    RSDK_THIS(Piston);
    Platform_Collision_AllSolid();
#if MANIA_USE_PLUS
    for (int32 i = 0; i < Player->playerCount; ++i) {
        if (((1 << i) & self->stoodPlayers) != 0) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
        }
    }
#endif
}

void Piston_State_WaitForInterval(void)
{
    // this obj was fun to do and is also very weird
    // there's so many fucking states but they're so tiny and almost exactly the same thing as other states
}

void Piston_StateMove_Vertical(void)
{
    RSDK_THIS(Piston);
    self->velocity.y = 0x80000;
    if (!self->reverse)
        self->drawPos.y += 0x80000;
    else
        self->drawPos.y -= 0x80000;

    if (--self->timer <= 0) {
        self->timer = self->distance;
        self->state = Piston_StateMove_Vertical_Reverse;
    }
}

void Piston_StateMove_Vertical_Reverse(void)
{
    RSDK_THIS(Piston);
    self->velocity.y = 0x10000;
    if (!self->reverse)
        self->drawPos.y += 0x10000;
    else
        self->drawPos.y -= 0x10000;

    if (--self->timer <= 0) {
        self->timer = ACTIVE_BOUNDS;
        self->state = Piston_State_WaitForInterval;
    }
}

void Piston_StateMove_Up(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y -= 0x80000;
    self->velocity.y = 0x80000;

    if (--self->timer <= 0) {
        for (int32 i = 0; i < Player->playerCount; ++i) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if ((1 << i) & self->stoodPlayers) {
                player->velocity.y = -0x100000;
                player->state      = Player_State_Air;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->animator, true, 0);
            }
        }
        self->timer = self->distance;
        self->state = Piston_StateMove_Up_Reverse;
    }
}

void Piston_StateMove_Up_Reverse(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y += 0x10000;
    self->velocity.y = 0x10000;
    if (--self->timer <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Piston_State_WaitForInterval;
    }
}

void Piston_StateMove_Down(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y += 0x80000;
    self->velocity.y = 0x80000;

    if (--self->timer <= 0) {
        self->pistonType = PISTON_MOVE_DOWN_REVERSE;
        self->active     = ACTIVE_BOUNDS;
        self->state      = Piston_State_WaitForInterval;
    }
}

void Piston_StateMove_Down_Reverse(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y -= 0x80000;
    self->velocity.y = 0x80000;

    if (--self->timer <= 0) {
        self->pistonType = PISTON_MOVE_DOWN;
        self->active     = ACTIVE_BOUNDS;
        self->state      = Piston_State_WaitForInterval;
    }
}

void Piston_StateMove_Right(void)
{
    RSDK_THIS(Piston);
    self->drawPos.x += 0x80000;
    self->velocity.x = 0x80000;
    if (--self->timer <= 0) {
        self->pistonType = PISTON_MOVE_LEFT;
        self->active     = ACTIVE_BOUNDS;
        self->state      = Piston_State_WaitForInterval;
    }
}

void Piston_StateMove_Left(void)
{
    RSDK_THIS(Piston);
    self->drawPos.x -= 0x80000;
    self->velocity.x = 0x80000;
    if (--self->timer <= 0) {
        self->pistonType = PISTON_MOVE_RIGHT;
        self->active     = ACTIVE_BOUNDS;
        self->state      = Piston_State_WaitForInterval;
    }
}

void Piston_StateMove_Horizontal(void)
{
    RSDK_THIS(Piston);
    self->velocity.x = 0x80000;
    if (!self->reverse)
        self->drawPos.x += 0x80000;
    else
        self->drawPos.x -= 0x80000;

    if (--self->timer <= 0) {
        self->timer = self->distance;
        self->state = Piston_StateMove_Horizontal_Reverse;
    }
}

void Piston_StateMove_Horizontal_Reverse(void)
{
    RSDK_THIS(Piston);
    self->velocity.x = 0x10000;
    if (!self->reverse)
        self->drawPos.x += 0x10000;
    else
        self->drawPos.x -= 0x10000;

    if (--self->timer <= 0) {
        self->timer = ACTIVE_BOUNDS;
        self->state = Piston_State_WaitForInterval;
    }
}

void Piston_StateActive_WaitForStood(void)
{
    RSDK_THIS(Piston);
    if (self->stoodPlayers)
        self->state = Piston_StateActive_PreparingLaunch;
}

void Piston_StateActive_PreparingLaunch(void)
{
    RSDK_THIS(Piston);
    if (self->stoodPlayers) {
        if (!self->timer)
            RSDK.PlaySfx(Piston->sfxLand, false, 255);

        self->drawPos.y += 0x10000;
        self->velocity.y = 0x10000;
        if (++self->timer >= 16) {
            RSDK.StopSfx(Piston->sfxLand);
            RSDK.PlaySfx(Piston->sfxLaunch, false, 255);
            self->state = Piston_StateActive_LaunchPlayers;
        }
    }
    else {
        self->state = Piston_StateActive_ReturnToStartPos;
        RSDK.StopSfx(Piston->sfxLand);
    }
}

void Piston_StateActive_LaunchPlayers(void)
{
    RSDK_THIS(Piston);
    self->drawPos.y -= 0x40000;
    self->velocity.y = 0x40000;
    self->timer -= 4;
    if (self->timer == 4) {
        for (int32 i = 0; i < Player->playerCount; ++i) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if ((1 << i) & self->stoodPlayers) {
                RSDK.PlaySfx(Piston->sfxLaunch, false, 255);
                player->velocity.y = -0x100000;
                player->state      = Player_State_Air;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGTWIRL, &player->animator, true, 0);
            }
        }
        self->state = Piston_StateActive_WaitForStood;
    }
}

void Piston_StateActive_ReturnToStartPos(void)
{
    RSDK_THIS(Piston);
    if (self->stoodPlayers) {
        self->state = Piston_StateActive_PreparingLaunch;
    }
    else {
        self->drawPos.y -= 0x10000;
        self->velocity.y = 0x10000;
        if (--self->timer <= 0)
            self->state = Piston_StateActive_WaitForStood;
    }
}

#if RETRO_INCLUDE_EDITOR
void Piston_EditorDraw(void) { Piston_Draw(); }

void Piston_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(Piston, type);
    RSDK_ENUM_VAR("Use Move Distance (Vertical)", PISTON_MOVE_VERTICAL);
    RSDK_ENUM_VAR("Move Up Then Down", PISTON_UP);
    RSDK_ENUM_VAR("Move Down Then Up", PISTON_MOVE_DOWN);
    RSDK_ENUM_VAR("Launch players When Stood On", PISTON_MOVE_DOWN_REVERSE);
    RSDK_ENUM_VAR("Move Right Then Left", PISTON_MOVE_RIGHT);
    RSDK_ENUM_VAR("Move Left Then Right", PISTON_MOVE_LEFT);
    RSDK_ENUM_VAR("Use Move Distance (Horizontal)", PISTON_MOVE_HORIZONTAL);

    RSDK_ACTIVE_VAR(Piston, size);
    RSDK_ENUM_VAR("1 Barrel", PISTON_SIZE_1);
    RSDK_ENUM_VAR("2 Barrels", PISTON_SIZE_2);
    RSDK_ENUM_VAR("3 Barrels", PISTON_SIZE_3);
}
#endif

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
