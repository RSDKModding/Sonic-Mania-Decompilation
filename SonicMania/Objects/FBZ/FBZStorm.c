// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FBZStorm Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFBZStorm *FBZStorm;

void FBZStorm_Update(void)
{
    RSDK_THIS(FBZStorm);

    StateMachine_Run(self->state);

    self->velocity.x = -0x40000;
    foreach_active(Current, current)
    {
        int32 strength = -0x10000 * current->strength;
        if (strength < self->velocity.x)
            self->velocity.x = strength;
    }

    for (int32 p = 0; p < Player->playerCount; ++p) {
        if (!RSDK_GET_ENTITY(p, Player)->sidekick) {
            self->screenPosX[p] = ScreenInfo[p].position.x;

            int32 velX          = ((self->screenPosX[p] << 14) - (ScreenInfo[p].position.x << 14)) + self->velocity.x;
            self->stormAngle[p] = RSDK.ATan2(self->velocity.y, -velX) << 1;

            Vector2 *pos = &FBZStorm->raindropPositions[0x40 * p];

            for (int32 i = 0; i < 0x40; ++i) {
                pos->x += velX;
                pos->y += self->velocity.y;

                if (pos->x < -0x1000000)
                    pos->x += 0x2000000;

                if (pos->y < -0xA00000)
                    pos->y += 0x1400000;

                if (pos->x > 0x1000000)
                    pos->x -= 0x2000000;

                if (pos->y > 0xA00000)
                    pos->y -= 0x1400000;

                ++pos;
            }
        }
    }

    // BGStorm layer
    if (RSDK.GetTileLayer(3)->drawGroup[0] < DRAWGROUP_COUNT) {
        self->enabled = true;
        if (self->blendAmount < 0x100) {
            self->blendAmount += 8;
            RSDK.SetLimitedFade(0, FBZStorm->outsideBank, 2, self->blendAmount, 0x80, 0x100);
        }
    }
    else {
        self->enabled = false;
        if (self->blendAmount > 0) {
            self->blendAmount -= 8;
            RSDK.SetLimitedFade(0, FBZStorm->outsideBank, 2, self->blendAmount, 0x80, 0x100);
        }
    }
}

void FBZStorm_LateUpdate(void) {}

void FBZStorm_StaticUpdate(void)
{
    bool32 enabled = false;
    foreach_all(FBZStorm, storm)
    {
        if (storm->enabled)
            enabled = true;
    }

    if (!enabled || RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID == PauseMenu->classID) {
        if (FBZStorm->playingRainSfx) {
            RSDK.StopSfx(FBZStorm->sfxRain);
            FBZStorm->playingRainSfx = false;
        }
    }
}

void FBZStorm_Draw(void)
{
    RSDK_THIS(FBZStorm);
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    int32 centerX  = screen->center.x << 16;
    int32 centerY  = screen->center.y << 16;
    self->rotation = self->stormAngle[SceneInfo->currentScreenID];

    Vector2 *raindropPos = &FBZStorm->raindropPositions[0x40 * SceneInfo->currentScreenID];
    for (int32 i = 0; i < 0x40; ++i) {
        Vector2 drawPos;
        drawPos.x = centerX + raindropPos->x;
        drawPos.y = centerY + raindropPos->y;
        RSDK.DrawSprite(&self->animator, &drawPos, true);
        ++raindropPos;
    }
}

void FBZStorm_Create(void *data)
{
    RSDK_THIS(FBZStorm);

    if (!SceneInfo->inEditor) {
        self->active     = ACTIVE_NORMAL;
        self->drawGroup  = Zone->objectDrawGroup[1];
        self->drawFX     = FX_ROTATE;
        self->inkEffect  = INK_ALPHA;
        self->velocity.x = -0x40000;
        self->velocity.y = 0xC0000;
        self->state      = FBZStorm_State_WaitForActive;

        for (int32 p = 0; p < Player->playerCount; ++p) {
            for (int32 i = 0; i < 0x40; ++i) {
                FBZStorm->raindropPositions[i].x = RSDK.Rand(-0x1000000, 0x1000000);
                FBZStorm->raindropPositions[i].y = RSDK.Rand(-0x800000, 0x800000);
            }
        }

        RSDK.SetSpriteAnimation(FBZStorm->aniFrames, 0, &self->animator, true, 0);
    }
}

void FBZStorm_StageLoad(void)
{
    FBZStorm->active = ACTIVE_ALWAYS;

    FBZStorm->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Storm.bin", SCOPE_STAGE);

    FBZStorm->sfxRain    = RSDK.GetSfx("FBZ/Rain.wav");
    FBZStorm->sfxThunder = RSDK.GetSfx("FBZ/Thunder.wav");
}

void FBZStorm_State_WaitForActive(void)
{
    RSDK_THIS(FBZStorm);

    if (self->enabled) {
        if (RSDK.GetEntityCount(Current->classID, true) > 0)
            self->state = FBZStorm_State_StormStart;
    }
}

void FBZStorm_State_StormStart(void)
{
    RSDK_THIS(FBZStorm);

    self->visible = true;
    if (self->alpha >= 0x80) {
        FBZStorm->outsideBank = 4;
        self->inkEffect       = INK_BLEND;
        self->state           = FBZStorm_State_Storming;
    }
    else {
        self->alpha += 4;
    }

    bool32 enabled = false;
    foreach_all(FBZStorm, storm)
    {
        if (storm->enabled)
            enabled = true;
    }

    if (FBZStorm->playingRainSfx) {
        if (!enabled || RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID == PauseMenu->classID) {
            RSDK.StopSfx(FBZStorm->sfxRain);
            FBZStorm->playingRainSfx = false;
        }
    }
    else {
        RSDK.PlaySfx(FBZStorm->sfxRain, true, 0xFF);
        FBZStorm->playingRainSfx = true;
    }
}

void FBZStorm_State_StormFinish(void)
{
    RSDK_THIS(FBZStorm);

    if (self->alpha <= 0) {
        self->visible = false;
        if (FBZStorm->playingRainSfx) {
            RSDK.StopSfx(FBZStorm->sfxRain);
        }
        self->state = FBZStorm_State_WaitForActive;
    }
    else {
        self->alpha -= 4;
    }
}

void FBZStorm_State_Storming(void)
{
    RSDK_THIS(FBZStorm);

    if (self->thunderTimer <= 0) {
        self->state = FBZStorm_State_Thunder;
    }
    else {
        self->thunderTimer--;
        if (!self->enabled) {
            self->inkEffect = INK_ALPHA;
            self->state     = FBZStorm_State_StormFinish;
        }
    }
}

void FBZStorm_State_Thunder(void)
{
    RSDK_THIS(FBZStorm);

    RSDK.SetLimitedFade(0, 2, 3, FBZStorm->thunderFade[self->thunderTimer], 0x80, 0x100);

    if (!self->thunderTimer)
        RSDK.PlaySfx(FBZStorm->sfxThunder, false, 255);

    if (++self->thunderTimer == 20) {
        self->thunderTimer = RSDK.Rand(180, 320);
        self->state        = FBZStorm_State_Storming;
    }
}

#if GAME_INCLUDE_EDITOR
void FBZStorm_EditorDraw(void) {}

void FBZStorm_EditorLoad(void) {}
#endif

void FBZStorm_Serialize(void) {}
