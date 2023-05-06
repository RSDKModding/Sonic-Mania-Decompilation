// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_SpeedLines Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUFO_SpeedLines *UFO_SpeedLines;

void UFO_SpeedLines_Update(void) {}

void UFO_SpeedLines_LateUpdate(void)
{
    RSDK_THIS(UFO_SpeedLines);

    EntityUFO_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, UFO_Player);

    if (UFO_Setup->machLevel > 1 && player->groundVel > 0xC0000) {
        self->visible = true;

        for (int32 l = 0; l < UFO_SPEEDLINES_LINE_COUNT; ++l) {
            if (self->lineAlpha[l] < 0x100)
                self->lineAlpha[l] += 0x20;

            if (self->lineX[l] >= player->position.x - 0x800000) {
                if (self->lineX[l] > 0x800000 + player->position.x) {
                    self->lineX[l] -= 0x1000000;
                    self->lineAlpha[l] = 0;
                }
            }
            else {
                self->lineX[l] += 0x1000000;
                self->lineAlpha[l] = 0;
            }

            if (self->lineZ[l] < player->position.y - 0x800000) {
                self->lineZ[l] += 0x1000000;
                self->lineAlpha[l] = 0;
            }
            else if (self->lineZ[l] > 0x800000 + player->position.y) {
                self->lineZ[l] -= 0x1000000;
                self->lineAlpha[l] = 0;
            }
        }
    }
    else {
        for (int32 l = 0; l < UFO_SPEEDLINES_LINE_COUNT; ++l) {
            if (self->lineAlpha[l] <= 0)
                self->visible = false;
            else
                self->lineAlpha[l] -= 8;
        }
    }
}

void UFO_SpeedLines_StaticUpdate(void) {}

void UFO_SpeedLines_Draw(void)
{
    RSDK_THIS(UFO_SpeedLines);

    Matrix *m = &UFO_Camera->matWorld;

    for (int32 l = 0; l < UFO_SPEEDLINES_LINE_COUNT; ++l) {
        int32 x = self->lineX[l] >> 8;
        int32 y = self->lineY[l] >> 8;
        int32 z = self->lineZ[l] >> 8;

        int32 depth = m->values[2][3] + (y * m->values[2][1] >> 8) + (z * m->values[2][2] >> 8) + (x * m->values[2][0] >> 8);
        if (depth >= 0x400) {
            int32 drawY = m->values[1][3] + (y * m->values[1][1] >> 8) + (z * m->values[1][2] >> 8) + (x * m->values[1][0] >> 8);
            int32 drawX = m->values[0][3] + (y * m->values[0][1] >> 8) + (z * m->values[0][2] >> 8) + (x * m->values[0][0] >> 8);

            RSDK.DrawLine(ScreenInfo->center.x + (drawX << 8) / depth, ScreenInfo->center.y - (drawY << 8) / depth,
                          ScreenInfo->center.x + (drawX << 8) / (depth + 0x2000), ScreenInfo->center.y - (drawY << 8) / (depth + 0x2000), 0xD0E0F0,
                          self->lineAlpha[l], INK_ALPHA, true);
        }
    }
}

void UFO_SpeedLines_Create(void *data)
{
    RSDK_THIS(UFO_SpeedLines);
    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawFX    = FX_SCALE | FX_FLIP;
        self->drawGroup = 5;
        self->active    = ACTIVE_NORMAL;

        EntityUFO_Player *player = RSDK_GET_ENTITY(SLOT_PLAYER1, UFO_Player);

        for (int32 l = 0; l < UFO_SPEEDLINES_LINE_COUNT; ++l) {
            self->lineX[l] = player->position.x + RSDK.Rand(-0x800000, 0x800000);
            self->lineY[l] = RSDK.Rand(0x100000, 0x1000000);
            self->lineZ[l] = player->position.y + RSDK.Rand(-0x800000, 0x800000);
        }
    }
}

void UFO_SpeedLines_StageLoad(void) { RSDK.ResetEntitySlot(SLOT_UFO_SPEEDLINES, UFO_SpeedLines->classID, NULL); }

#if GAME_INCLUDE_EDITOR
void UFO_SpeedLines_EditorDraw(void) {}

void UFO_SpeedLines_EditorLoad(void) {}
#endif

void UFO_SpeedLines_Serialize(void) {}
