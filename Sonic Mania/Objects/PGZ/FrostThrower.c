#include "SonicMania.h"

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
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->flag)
        FrostThrower_Unknown1();
}

void FrostThrower_Create(void *data)
{
    RSDK_THIS(FrostThrower);
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
    FrostThrower_Unknown3();
    FrostThrower_Unknown4();
    RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 1, &self->animator2, true, 0);
    self->state = FrostThrower_Unknown5;
}

void FrostThrower_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        FrostThrower->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/FrostThrower.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        FrostThrower->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/FrostThrower.bin", SCOPE_STAGE);

    FrostThrower->sfxFrostThrower = RSDK.GetSfx("PSZ/FrostThrower.wav");
    FrostThrower->sfxFreeze       = RSDK.GetSfx("PSZ/Freeze.wav");
}

void FrostThrower_Unknown1(void)
{
    RSDK_THIS(FrostThrower);

    int32 pos = 0;
    for (int32 i = 0; i < 4; ++i) {
        int32 count = minVal(self->field_B4[i], 3);

        RSDK.SetSpriteAnimation(FrostThrower->aniFrames, 1, &self->animator2, true, i);
        for (int32 p = 0; p < count; ++p) {
            Vector2 drawPos;
            drawPos.x = self->position.x + self->field_C4[pos + p].x;
            drawPos.y = self->position.y + self->field_C4[pos + p].y;
            RSDK.DrawSprite(&self->animator2, &drawPos, false);
        }
        pos += 3;
    }
}

void FrostThrower_Unknown2(void)
{
    RSDK_THIS(FrostThrower);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && !Ice->playerTimers[RSDK.GetEntityID(player)]) {
            Ice_FreezePlayer(player);
        }
    }
}

void FrostThrower_Unknown3(void)
{
    RSDK_THIS(FrostThrower);

    for (int32 i = 0; i < 4; ++i) {
        self->field_B4[i] = self->field_A4[i];
        if (self->field_A4[i] > 1) {
            if (RSDK.Rand(0, 10) <= 6)
                self->field_B4[i] = RSDK.Rand(1, self->field_A4[i]);
        }
    }
}

void FrostThrower_Unknown4(void)
{
    RSDK_THIS(FrostThrower);
    int32 pos = 0;

    int32 yMin[] = { 2, 20, 45, 55 };
    int32 yMax[] = { 22, 45, 62, 78 };
    int32 xMin[] = { -3, -5, -7, -9 };
    int32 xMax[] = { 3, 5, 7, 9 };
    for (int32 i = 0; i < 4; ++i) {
        if (self->field_B4[i]) {
            for (int32 p = 0; p < self->field_B4[i]; ++p) {
                self->field_C4[pos + p].x = RSDK.Rand(xMin[i], xMax[i]) << 16;
                self->field_C4[pos + p].y = RSDK.Rand(yMin[i], yMax[i]) << 16;
            }
        }
        pos += 3;
    }
}

void FrostThrower_Unknown5(void)
{
    RSDK_THIS(FrostThrower);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->active = ACTIVE_NORMAL;
        self->timer  = 0;
        self->flag   = true;
        self->state  = FrostThrower_Unknown6;
        RSDK.PlaySfx(FrostThrower->sfxFrostThrower, false, 255);
    }
}

void FrostThrower_Unknown6(void)
{
    RSDK_THIS(FrostThrower);
    self->hitbox.top = 0;
    if (self->timer >= 20)
        self->hitbox.bottom = 80;
    else
        self->hitbox.bottom = 4 * self->timer;

    FrostThrower_Unknown2();

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

    FrostThrower_Unknown3();
    FrostThrower_Unknown4();

    ++self->timer;
    if (self->timer >= self->duration) {
        self->state = FrostThrower_Unknown7;
        self->timer = 0;
    }
}

void FrostThrower_Unknown7(void)
{
    RSDK_THIS(FrostThrower);
    self->hitbox.top    = 4 * self->timer;
    self->hitbox.bottom = 80;
    FrostThrower_Unknown2();

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
    FrostThrower_Unknown3();
    FrostThrower_Unknown4();

    if (self->timer >= 20) {
        self->active = ACTIVE_BOUNDS;
        self->flag   = false;
        self->state  = FrostThrower_Unknown5;
        self->timer  = 0;
    }
    else {
        self->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void FrostThrower_EditorDraw(void) {}

void FrostThrower_EditorLoad(void) {}
#endif

void FrostThrower_Serialize(void)
{
    RSDK_EDITABLE_VAR(FrostThrower, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(FrostThrower, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(FrostThrower, VAR_UINT16, duration);
}
