// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Acetone Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->isActive)
        Acetone_DrawGustFX();
}

void Acetone_Create(void *data)
{
    RSDK_THIS(Acetone);
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->drawOrder     = Zone->playerDrawLow + 1;
    self->updateRange.x = 0x100000;
    self->updateRange.y = 0x100000;
    self->hitbox.left   = -14;
    self->hitbox.right  = 15;
    self->field_A4[0]   = 2;
    self->field_A4[1]   = 2;
    self->field_A4[2]   = 3;
    self->field_A4[3]   = 3;
    Acetone_Unknown3();
    Acetone_Unknown4();
    RSDK.SetSpriteAnimation(Acetone->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Acetone->aniFrames, 1, &self->animator2, true, 0);
    self->state = Acetone_State_IntervalWait;
}

void Acetone_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Acetone->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Acetone.bin", SCOPE_STAGE);
    Acetone->sfxFrostThrower = RSDK.GetSfx("PSZ/FrostThrower.wav");
    Acetone->sfxFreeze       = RSDK.GetSfx("PSZ/Freeze.wav");
}

void Acetone_DrawGustFX(void)
{
    RSDK_THIS(Acetone);

    int32 pos = 0;
    for (int32 i = 0; i < 4; ++i) {
        int32 count = minVal(self->field_B4[i], 3);

        RSDK.SetSpriteAnimation(Acetone->aniFrames, 1, &self->animator2, true, i);
        for (int32 p = 0; p < count; ++p) {
            Vector2 drawPos;
            drawPos.x = self->position.x + self->gustPos[pos + p].x;
            drawPos.y = self->position.y + self->gustPos[pos + p].y;
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
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
                // To fix this up to work as "intended", simply replace the "2"s with "64"
                case ID_SONIC: RSDK.CopyPalette(6, 2, 0, 2, 6); break;
                case ID_TAILS: RSDK.CopyPalette(6, 70, 0, 70, 6); break;
                case ID_KNUCKLES:
                    RSDK.CopyPalette(6, 80, 0, 80, 6);
                    break;

                // This is an unused object that was scrapped before plus was created, so there's no mighty/ray code
                // I've created a mock-up of what mighty/ray code could've looked like, had it been implimented:
                // case ID_MIGHTY: RSDK.CopyPalette(6, 96, 0, 96, 6); break;
                // case ID_RAY: RSDK.CopyPalette(6, 113, 0, 113, 6); break;
            }
            Ink->playerColours[RSDK.GetEntityID(player)] = 0;
        }
    }
}

void Acetone_Unknown3(void)
{
    RSDK_THIS(Acetone);

    for (int32 i = 0; i < 4; ++i) {
        self->field_B4[i] = self->field_A4[i];
        if (self->field_A4[i] > 1) {
            if (RSDK.Rand(0, 10) <= 6)
                self->field_B4[i] = RSDK.Rand(1, self->field_A4[i]);
        }
    }
}

void Acetone_Unknown4(void)
{
    RSDK_THIS(Acetone);
    int32 pos = 0;

    int32 yMin[] = { 2, 20, 45, 55 };
    int32 yMax[] = { 22, 45, 62, 78 };
    int32 xMin[] = { -3, -5, -7, -9 };
    int32 xMax[] = { 3, 5, 7, 9 };
    for (int32 i = 0; i < 4; ++i) {
        if (self->field_B4[i]) {
            for (int32 p = 0; p < self->field_B4[i]; ++p) {
                self->gustPos[pos + p].x = RSDK.Rand(xMin[i], xMax[i]) << 16;
                self->gustPos[pos + p].y = RSDK.Rand(yMin[i], yMax[i]) << 16;
            }
        }
        pos += 3;
    }
}

void Acetone_State_IntervalWait(void)
{
    RSDK_THIS(Acetone);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->active = ACTIVE_NORMAL;
        self->timer  = 0;
        self->isActive   = true;
        self->state  = Acetone_Unknown6;
        RSDK.PlaySfx(Acetone->sfxFrostThrower, false, 255);
    }
}

void Acetone_Unknown6(void)
{
    RSDK_THIS(Acetone);
    self->hitbox.top = 0;
    if (self->timer >= 20)
        self->hitbox.bottom = 80;
    else
        self->hitbox.bottom = 4 * self->timer;

    Acetone_CheckPlayerCollisions();

    self->field_A4[0] = 0;
    if (self->timer > 3)
        self->field_A4[0] = 2;
    else
        self->field_A4[0] = 1;

    self->field_A4[1] = 0;
    if (self->timer <= 8 && self->timer > 5)
        self->field_A4[1] = 1;
    else if (self->timer > 8)
        self->field_A4[1] = 2;

    self->field_A4[2] = 0;
    if (self->timer <= 16) {
        if (self->timer <= 14) {
            if (self->timer > 12)
                self->field_A4[2] = 1;
        }
        else {
            self->field_A4[2] = 2;
        }
    }
    else {
        self->field_A4[2] = 3;
    }

    self->field_A4[3] = 0;
    if (self->timer <= 19) {
        if (self->timer <= 17) {
            if (self->timer > 15)
                self->field_A4[3] = 1;
        }
        else {
            self->field_A4[3] = 2;
        }
    }
    else {
        self->field_A4[3] = 3;
    }

    Acetone_Unknown3();
    Acetone_Unknown4();

    ++self->timer;
    if (self->timer >= self->duration) {
        self->state = Acetone_Unknown7;
        self->timer = 0;
    }
}

void Acetone_Unknown7(void)
{
    RSDK_THIS(Acetone);
    self->hitbox.top    = 4 * self->timer;
    self->hitbox.bottom = 80;
    Acetone_CheckPlayerCollisions();

    self->field_A4[0] = 2;
    if (self->timer <= 8) {
        if (self->timer > 5)
            self->field_A4[0] = 1;
    }
    else {
        self->field_A4[0] = 0;
    }

    self->field_A4[1] = 2;
    if (self->timer <= 8) {
        if (self->timer > 5)
            self->field_A4[1] = 1;
    }
    else {
        self->field_A4[1] = 0;
    }

    self->field_A4[2] = 3;
    if (self->timer <= 19) {
        if (self->timer <= 17) {
            if (self->timer > 15)
                self->field_A4[2] = 2;
        }
        else {
            self->field_A4[2] = 1;
        }
    }
    else {
        self->field_A4[2] = 0;
    }

    self->field_A4[3] = 3;
    if (self->timer <= 16) {
        if (self->timer <= 14) {
            if (self->timer > 12)
                self->field_A4[3] = 2;
        }
        else {
            self->field_A4[3] = 1;
        }
    }
    else {
        self->field_A4[3] = 0;
    }
    Acetone_Unknown3();
    Acetone_Unknown4();

    if (self->timer >= 20) {
        self->active = ACTIVE_BOUNDS;
        self->isActive   = false;
        self->state  = Acetone_State_IntervalWait;
        self->timer  = 0;
    }
    else {
        self->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void Acetone_EditorDraw(void) { Acetone_Draw(); }

void Acetone_EditorLoad(void) { Acetone->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Acetone.bin", SCOPE_STAGE); }
#endif

void Acetone_Serialize(void)
{
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Acetone, VAR_UINT16, duration);
}
