// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FrostThrower Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFrostThrower *FrostThrower;

void FrostThrower_Update(void)
{
    RSDK_THIS(FrostThrower);

    StateMachine_Run(self->state);
}

void FrostThrower_LateUpdate(void) {}

void FrostThrower_StaticUpdate(void) {}

void FrostThrower_Draw(void)
{
    RSDK_THIS(FrostThrower);

    RSDK.DrawSprite(&self->dispenseAnimator, NULL, false);

    if (self->isActive)
        FrostThrower_DrawGustFX();
}

void FrostThrower_Create(void *data)
{
    RSDK_THIS(FrostThrower);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->drawGroup     = Zone->playerDrawGroup[0] + 1;
    self->updateRange.x = 0x100000;
    self->updateRange.y = 0x100000;

    self->hitbox.left     = -14;
    self->hitbox.right    = 15;
    self->maxGustCount[0] = 2;
    self->maxGustCount[1] = 2;
    self->maxGustCount[2] = 3;
    self->maxGustCount[3] = 3;

    FrostThrower_HandleGustCount();
    FrostThrower_HandleGustPos();

    RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 0, &self->dispenseAnimator, true, 0);
    RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 1, &self->gustAnimator, true, 0);

    self->state = FrostThrower_State_AwaitInterval;
}

void FrostThrower_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        FrostThrower->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/FrostThrower.bin", SCOPE_STAGE); // this doesn't actually exist in the final game...
    else if (RSDK.CheckSceneFolder("PSZ2"))
        FrostThrower->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/FrostThrower.bin", SCOPE_STAGE);

    FrostThrower->sfxFrostThrower = RSDK.GetSfx("PSZ/FrostThrower.wav");
    FrostThrower->sfxFreeze       = RSDK.GetSfx("PSZ/Freeze.wav");
}

void FrostThrower_DrawGustFX(void)
{
    RSDK_THIS(FrostThrower);

    int32 pos = 0;
    for (int32 i = 0; i < 4; ++i) {
        int32 count = MIN(self->gustCount[i], 3);

        RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 1, &self->gustAnimator, true, i);
        for (int32 p = 0; p < count; ++p) {
            Vector2 drawPos;
            drawPos.x = self->position.x + self->gustPos[pos + p].x;
            drawPos.y = self->position.y + self->gustPos[pos + p].y;
            RSDK.DrawSprite(&self->gustAnimator, &drawPos, false);
        }

        pos += 3;
    }
}

void FrostThrower_CheckPlayerCollisions(void)
{
    RSDK_THIS(FrostThrower);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && !Ice->playerTimers[playerID])
            Ice_FreezePlayer(player);
    }
}

void FrostThrower_HandleGustCount(void)
{
    RSDK_THIS(FrostThrower);

    for (int32 i = 0; i < 4; ++i) {
        self->gustCount[i] = self->maxGustCount[i];
        if (self->maxGustCount[i] > 1) {
            if (RSDK.Rand(0, 10) <= 6)
                self->gustCount[i] = RSDK.Rand(1, self->maxGustCount[i]);
        }
    }
}

void FrostThrower_HandleGustPos(void)
{
    RSDK_THIS(FrostThrower);
    int32 pos = 0;

    int32 yMin[] = { 2, 20, 45, 55 };
    int32 yMax[] = { 22, 45, 62, 78 };
    int32 xMin[] = { -3, -5, -7, -9 };
    int32 xMax[] = { 3, 5, 7, 9 };
    for (int32 i = 0; i < 4; ++i) {
        for (int32 p = 0; p < self->gustCount[i]; ++p) {
            self->gustPos[pos + p].x = RSDK.Rand(xMin[i], xMax[i]) << 16;
            self->gustPos[pos + p].y = RSDK.Rand(yMin[i], yMax[i]) << 16;
        }
        pos += 3;
    }
}

void FrostThrower_State_AwaitInterval(void)
{
    RSDK_THIS(FrostThrower);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->active   = ACTIVE_NORMAL;
        self->timer    = 0;
        self->isActive = true;
        self->state    = FrostThrower_State_Dispensing;
        RSDK.PlaySfx(FrostThrower->sfxFrostThrower, false, 255);
    }
}

void FrostThrower_State_Dispensing(void)
{
    RSDK_THIS(FrostThrower);

    self->hitbox.top = 0;
    if (self->timer >= 20)
        self->hitbox.bottom = 80;
    else
        self->hitbox.bottom = 4 * self->timer;

    FrostThrower_CheckPlayerCollisions();

    if (self->timer > 3)
        self->maxGustCount[0] = 2;
    else
        self->maxGustCount[0] = 1;

    if (self->timer > 8)
        self->maxGustCount[1] = 2;
    else if (self->timer > 5)
        self->maxGustCount[1] = 1;
    else
        self->maxGustCount[1] = 0;

    if (self->timer > 16)
        self->maxGustCount[2] = 3;
    else if (self->timer > 14)
        self->maxGustCount[2] = 2;
    else if (self->timer > 12)
        self->maxGustCount[2] = 1;
    else
        self->maxGustCount[2] = 0;

    if (self->timer > 19)
        self->maxGustCount[3] = 3;
    else if (self->timer > 17)
        self->maxGustCount[3] = 2;
    else if (self->timer > 15)
        self->maxGustCount[3] = 1;
    else
        self->maxGustCount[3] = 0;

    FrostThrower_HandleGustCount();
    FrostThrower_HandleGustPos();

    if (++self->timer >= self->duration) {
        self->state = FrostThrower_State_StopDispensing;
        self->timer = 0;
    }
}

void FrostThrower_State_StopDispensing(void)
{
    RSDK_THIS(FrostThrower);

    self->hitbox.top    = 4 * self->timer;
    self->hitbox.bottom = 80;

    FrostThrower_CheckPlayerCollisions();

    if (self->timer > 8)
        self->maxGustCount[0] = 0;
    else if (self->timer > 5)
        self->maxGustCount[0] = 1;
    else
        self->maxGustCount[1] = 2;

    if (self->timer > 8)
        self->maxGustCount[1] = 0;
    else if (self->timer > 5)
        self->maxGustCount[1] = 1;
    else
        self->maxGustCount[1] = 2;

    if (self->timer > 19)
        self->maxGustCount[2] = 0;
    else if (self->timer > 17)
        self->maxGustCount[2] = 1;
    else if (self->timer > 15)
        self->maxGustCount[2] = 2;
    else
        self->maxGustCount[2] = 3;

    if (self->timer > 16)
        self->maxGustCount[3] = 0;
    else if (self->timer > 14)
        self->maxGustCount[3] = 1;
    else if (self->timer > 12)
        self->maxGustCount[3] = 2;
    else
        self->maxGustCount[3] = 3;

    FrostThrower_HandleGustCount();
    FrostThrower_HandleGustPos();

    if (self->timer++ >= 20) {
        self->active   = ACTIVE_BOUNDS;
        self->isActive = false;
        self->state    = FrostThrower_State_AwaitInterval;
        self->timer    = 0;
    }
}

#if GAME_INCLUDE_EDITOR
void FrostThrower_EditorDraw(void)
{
    RSDK_THIS(FrostThrower);

    RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 0, &self->dispenseAnimator, true, 0);
    RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 1, &self->gustAnimator, true, 0);

    FrostThrower_Draw();
}

void FrostThrower_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        FrostThrower->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/FrostThrower.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        FrostThrower->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/FrostThrower.bin", SCOPE_STAGE);
}
#endif

void FrostThrower_Serialize(void)
{
    RSDK_EDITABLE_VAR(FrostThrower, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(FrostThrower, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(FrostThrower, VAR_UINT16, duration);
}
