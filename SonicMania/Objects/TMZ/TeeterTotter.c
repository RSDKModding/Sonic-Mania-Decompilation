// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TeeterTotter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTeeterTotter *TeeterTotter;

void TeeterTotter_Update(void)
{
    RSDK_THIS(TeeterTotter);

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->animator);
}

void TeeterTotter_LateUpdate(void) {}

void TeeterTotter_StaticUpdate(void) {}

void TeeterTotter_Draw(void)
{
    RSDK_THIS(TeeterTotter);

    int32 x       = self->position.x;
    int32 y       = self->position.y;
    int32 offsetX = 0x100000 - (self->length << 21) + x;

    for (uint32 i = 0; i < 2 * self->length; ++i) {
        if (!((1 << i) & self->inactiveSegments)) {
            Vector2 drawPos;
            drawPos.x = (offsetX + (i << 21)) & 0xFFFF0000;
            drawPos.y = (self->segmentPosition[i] + y) & 0xFFFF0000;

            RSDK.SetSpriteAnimation(TeeterTotter->aniFrames, 0, &self->animator, true, (i + self->color) % 2);

            RSDK.DrawSprite(&self->animator, &drawPos, false);
        }
    }
}

void TeeterTotter_Create(void *data)
{
    RSDK_THIS(TeeterTotter);

    self->length        = CLAMP(self->length, 1, TEETERTOTTER_SEGMENT_COUNT / 2);
    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->origin        = self->position;
    self->updateRange.x = (self->length + 2) << 22;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.y = (self->length + 4) << 21;

    self->hitbox.left   = -16;
    self->hitbox.top    = -16;
    self->hitbox.right  = 16;
    self->hitbox.bottom = 16;

    self->state = TeeterTotter_State_Init;
}

void TeeterTotter_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2"))
        TeeterTotter->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TeeterTotter.bin", SCOPE_STAGE);
}

int32 TeeterTotter_CheckPlayerCollisions(void)
{
    RSDK_THIS(TeeterTotter);

    int32 heaviestSegment   = -1;
    int32 stoodSegmentIDs[] = { -1, -1, -1, -1 };

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    int32 x = 0x100000 - (self->length << 21) + self->position.x;

    for (uint32 s = 0; s < 2 * self->length; ++s) {
        if (!((1 << heaviestSegment) & self->inactiveSegments)) {
            self->position.x = (x + (s << 21)) & 0xFFFF0000;
            self->position.y = (self->segmentPosition[s] + storeY) & 0xFFFF0000;

            foreach_active(Player, player)
            {
                int32 playerID = RSDK.GetEntitySlot(player);
                if (self->playerIDs[playerID] == (int32)s) {
                    player->position.y += self->segmentVelocity[s];
                    player->position.y += 0x10000;
                }

                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                    if (!player->sidekick)
                        heaviestSegment = s;

                    stoodSegmentIDs[playerID] = s;
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) self->playerIDs[i] = stoodSegmentIDs[i];

    return heaviestSegment;
}

void TeeterTotter_ProcessSegmentGravity(void)
{
    RSDK_THIS(TeeterTotter);

    int32 x       = self->position.x;
    int32 y       = self->position.y;
    int32 offsetX = x + (0x100000 - (self->length << 21));

    for (uint32 i = 0; i < 2 * self->length; ++i) {
        if (!((1 << i) & self->inactiveSegments)) {
            self->segmentVelocity[i] += 0x3800;
            self->segmentPosition[i] += self->segmentVelocity[i];

            Vector2 segmentPos;
            segmentPos.x = (offsetX + (i << 21)) & 0xFFFF0000;
            segmentPos.y = (y + self->segmentPosition[i]) & 0xFFFF0000;
            if (!RSDK.CheckPosOnScreen(&segmentPos, &self->updateRange))
                self->inactiveSegments |= 1 << i;
        }
    }
}

void TeeterTotter_HandleSegmentPositions(void)
{
    RSDK_THIS(TeeterTotter);

    uint8 len = self->length;
    for (uint32 i = 0; i < 2 * self->length; ++i) {
        int8 pos = i - len;
        if (pos >= 0)
            pos++;

        self->segmentPosition[i] = (self->fallPos >> 1) * (int8)(2 * pos + 2 * ((int8)(2 * pos) <= 0) - 1);
    }
}

void TeeterTotter_State_Init(void)
{
    RSDK_THIS(TeeterTotter);

    self->unused2      = 0;
    self->fallPos      = 0;
    self->fallVelocity = 0;

    for (uint32 i = 0; i < 2 * self->length; ++i) {
        self->segmentPosition[i] = 0;
        self->segmentVelocity[i] = 0;
    }
    self->inactiveSegments = 0;

    self->visible = true;
    self->active  = ACTIVE_NORMAL;
    self->state   = TeeterTotter_State_Teeter;
}

void TeeterTotter_State_Teeter(void)
{
    RSDK_THIS(TeeterTotter);

    int32 prevVal[TEETERTOTTER_SEGMENT_COUNT];
    for (uint32 i = 0; i < 2 * self->length; ++i) {
        prevVal[i] = self->segmentPosition[i];
    }

    TeeterTotter_HandleSegmentPositions();

    for (uint32 i = 0; i < 2 * self->length; ++i) {
        self->segmentVelocity[i] = self->segmentPosition[i] - prevVal[i];
    }

    int32 id = TeeterTotter_CheckPlayerCollisions();
    if (id > -1) {
        int32 segment = id - self->length;
        if (segment >= 0)
            ++segment;

        self->fallVelocity += 32 * segment;
    }

    self->fallPos += self->fallVelocity;

    if (abs(self->fallPos) > 0x200000)
        self->state = TeeterTotter_State_Fall;
}

void TeeterTotter_State_Fall(void)
{
    RSDK_THIS(TeeterTotter);

    TeeterTotter_ProcessSegmentGravity();

    bool32 fullyInactive = true;
    for (uint32 i = 0; i < 2 * self->length; ++i) fullyInactive &= ((1 << i) & self->inactiveSegments) != 0;

    if (fullyInactive || !(2 * self->length)) {
        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->origin, &self->updateRange)) {
            self->position = self->origin;
            self->visible  = false;
            self->active   = ACTIVE_BOUNDS;
            self->state    = TeeterTotter_State_Init;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void TeeterTotter_EditorDraw(void)
{
    RSDK_THIS(TeeterTotter);

    self->length        = CLAMP(self->length, 1, TEETERTOTTER_SEGMENT_COUNT / 2);
    self->origin        = self->position;
    self->updateRange.x = (self->length + 2) << 22;
    self->updateRange.y = (self->length + 4) << 21;

    TeeterTotter_Draw();
}

void TeeterTotter_EditorLoad(void)
{
    TeeterTotter->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TeeterTotter.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TeeterTotter, color);
    RSDK_ENUM_VAR("Orange, Blue", TEETERTOTTER_COLOR_ORANGEBLUE);
    RSDK_ENUM_VAR("Blue, Orange", TEETERTOTTER_COLOR_BLUEORANGE);
}
#endif

void TeeterTotter_Serialize(void)
{
    RSDK_EDITABLE_VAR(TeeterTotter, VAR_UINT8, color);
    RSDK_EDITABLE_VAR(TeeterTotter, VAR_UINT32, length);
}
