// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Acetone Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAcetone *Acetone;

// This object goes completely unused in regular gameplay
// afterall, so does the entire PGZ1 ink gimmick
// and even then, this is basically just a tweaked copy of FrostThrower anyways

void Acetone_Update(void)
{
    RSDK_THIS(Acetone);
    StateMachine_Run(self->state);
}

void Acetone_LateUpdate(void) {}

void Acetone_StaticUpdate(void) {}

void Acetone_Draw(void)
{
    RSDK_THIS(Acetone);

    RSDK.DrawSprite(&self->dispenseAnimator, NULL, false);

    if (self->isActive)
        Acetone_DrawGustFX();
}

void Acetone_Create(void *data)
{
    RSDK_THIS(Acetone);

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

    Acetone_HandleGustCount();
    Acetone_HandleGustPos();

    RSDK.SetSpriteAnimation(Acetone->aniFrames, 0, &self->dispenseAnimator, true, 0);
    RSDK.SetSpriteAnimation(Acetone->aniFrames, 1, &self->gustAnimator, true, 0);

    self->state = Acetone_State_AwaitInterval;
}

void Acetone_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Acetone->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Acetone.bin", SCOPE_STAGE);

    Acetone->sfxFrostThrower = RSDK.GetSfx("PSZ/FrostThrower.wav");
    Acetone->sfxFreeze       = RSDK.GetSfx("PSZ/Freeze.wav");
}

void Acetone_DrawGustFX(void)
{
    RSDK_THIS(Acetone);

    int32 pos = 0;
    for (int32 i = 0; i < 4; ++i) {
        int32 count = MIN(self->gustCount[i], 3);

        RSDK.SetSpriteAnimation(Acetone->aniFrames, 1, &self->gustAnimator, true, i);
        for (int32 p = 0; p < count; ++p) {
            Vector2 drawPos;
            drawPos.x = self->position.x + self->gustPos[pos + p].x;
            drawPos.y = self->position.y + self->gustPos[pos + p].y;
            RSDK.DrawSprite(&self->gustAnimator, &drawPos, false);
        }

        pos += 3;
    }
}

void Acetone_CheckPlayerCollisions(void)
{
    RSDK_THIS(Acetone);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            switch (player->characterID) {
                // This actually wont work on sonic specifically, it uses the "old" sonic palette
                // This palette starts at index 2, instead of index 64 like usual
                // To fix this up to work as "intended", simply replace the "PLAYER_PALETTE_INDEX_SONIC_OLD"s with "PLAYER_PALETTE_INDEX_SONIC"
                case ID_SONIC: RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_SONIC_OLD, 0, PLAYER_PALETTE_INDEX_SONIC_OLD, 6); break;

                case ID_TAILS: RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_TAILS, 0, PLAYER_PALETTE_INDEX_TAILS, PLAYER_PRIMARY_COLOR_COUNT); break;

                case ID_KNUCKLES:
                    RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_KNUX, 0, PLAYER_PALETTE_INDEX_KNUX, PLAYER_PRIMARY_COLOR_COUNT);
                    break;

                    // This is an unused object that was scrapped before plus was created, so there's no mighty/ray code
                    // I've created a mock-up of what mighty/ray code could've looked like, had it been implemented:
                    // case ID_MIGHTY: RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_MIGHTY, 0, PLAYER_PALETTE_INDEX_MIGHTY, PLAYER_PRIMARY_COLOR_COUNT); break;
                    // case ID_RAY: RSDK.CopyPalette(6, PLAYER_PALETTE_INDEX_RAY, 0, PLAYER_PALETTE_INDEX_RAY, PLAYER_PRIMARY_COLOR_COUNT); break;
            }

            Ink->playerColors[RSDK.GetEntitySlot(player)] = 0;
        }
    }
}

void Acetone_HandleGustCount(void)
{
    RSDK_THIS(Acetone);

    for (int32 i = 0; i < 4; ++i) {
        self->gustCount[i] = self->maxGustCount[i];

        if (self->maxGustCount[i] > 1) {
            if (RSDK.Rand(0, 10) <= 6)
                self->gustCount[i] = RSDK.Rand(1, self->maxGustCount[i]);
        }
    }
}

void Acetone_HandleGustPos(void)
{
    RSDK_THIS(Acetone);
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

void Acetone_State_AwaitInterval(void)
{
    RSDK_THIS(Acetone);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->active   = ACTIVE_NORMAL;
        self->timer    = 0;
        self->isActive = true;
        self->state    = Acetone_State_Dispensing;
        RSDK.PlaySfx(Acetone->sfxFrostThrower, false, 0xFF);
    }
}

void Acetone_State_Dispensing(void)
{
    RSDK_THIS(Acetone);

    self->hitbox.top = 0;
    if (self->timer >= 20)
        self->hitbox.bottom = 80;
    else
        self->hitbox.bottom = 4 * self->timer;

    Acetone_CheckPlayerCollisions();

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

    Acetone_HandleGustCount();
    Acetone_HandleGustPos();

    if (++self->timer >= self->duration) {
        self->state = Acetone_State_StopDispensing;
        self->timer = 0;
    }
}

void Acetone_State_StopDispensing(void)
{
    RSDK_THIS(Acetone);

    self->hitbox.top    = 4 * self->timer;
    self->hitbox.bottom = 80;

    Acetone_CheckPlayerCollisions();

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

    Acetone_HandleGustCount();
    Acetone_HandleGustPos();

    if (self->timer++ >= 20) {
        self->active   = ACTIVE_BOUNDS;
        self->isActive = false;
        self->state    = Acetone_State_AwaitInterval;
        self->timer    = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void Acetone_EditorDraw(void)
{
    RSDK_THIS(Acetone);

    RSDK.SetSpriteAnimation(Acetone->aniFrames, 0, &self->dispenseAnimator, true, 0);
    RSDK.SetSpriteAnimation(Acetone->aniFrames, 1, &self->gustAnimator, true, 0);

    Acetone_Draw();
}

void Acetone_EditorLoad(void) { Acetone->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Acetone.bin", SCOPE_STAGE); }
#endif

void Acetone_Serialize(void)
{
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, duration);
}
