// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DashLift Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDashLift *DashLift;

void DashLift_Update(void) { Platform_Update(); }

void DashLift_LateUpdate(void) {}

void DashLift_StaticUpdate(void) {}

void DashLift_Draw(void)
{
    RSDK_THIS(DashLift);

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void DashLift_Create(void *data)
{
    RSDK_THIS(DashLift);

    self->collision = PLATFORM_C_SOLID;
    Platform_Create(NULL);

    if (!SceneInfo->inEditor) {
        self->drawGroup = Zone->playerDrawGroup[1];
        RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 4);

        self->drawPos.y += self->startOff << 15;
        self->updateRange.y = self->length << 15;

        self->amplitude.x = self->position.y - (self->length << 15);
        self->amplitude.y = self->position.y + (self->length << 15);

        self->updateRange.x = 0x800000;
        self->state         = DashLift_State_Init;
    }
}

void DashLift_StageLoad(void) { DashLift->sfxPulley = RSDK.GetSfx("Stage/Pulley.wav"); }

void DashLift_CheckOffScreen(void)
{
    RSDK_THIS(DashLift);

    if (!RSDK.CheckPosOnScreen(&self->drawPos, &self->updateRange) && !RSDK.CheckPosOnScreen(&self->centerPos, &self->updateRange)) {
        self->drawPos       = self->centerPos;
        self->position      = self->centerPos;
        self->activePlayers = 0;
        self->active        = ACTIVE_BOUNDS;
        DashLift_Create(NULL);
    }
}

void DashLift_State_Init(void)
{
    RSDK_THIS(DashLift);

    self->active = ACTIVE_NORMAL;
    self->state  = DashLift_State_HandleDash;
}

void DashLift_State_HandleDash(void)
{
    RSDK_THIS(DashLift);

    int32 totalSpeed = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (!((1 << playerID) & self->stoodPlayers)) {
            if (((1 << playerID) & self->activePlayers)) {
                player->position.x = self->position.x;
                self->activePlayers &= ~(1 << playerID);
                if (player->animator.animationID == ANI_JUMP)
                    player->velocity.x = 0;
            }
        }
        else {
            int32 anim = player->animator.animationID;
            if (anim == ANI_SPINDASH || (anim == ANI_JUMP && ((1 << playerID) & self->activePlayers))) {
                self->activePlayers |= (1 << playerID);

                foreach_active(Dust, dust)
                {
                    if (dust->parent == (Entity *)player)
                        destroyEntity(dust);
                }

                player->velocity.x -= player->velocity.x >> 5;
                player->position.x = self->position.x;
                if (player->direction == FLIP_X) {
                    if ((player->sidekick && totalSpeed < 0) || self->drawPos.y >= self->amplitude.y)
                        continue;

                    int32 speed = 0;
                    if (player->state == Player_State_Spindash) {
                        if (player->superState == SUPERSTATE_SUPER)
                            speed = ((player->abilityTimer >> 1) & 0x7FFF8000) + 0xB0000;
                        else
                            speed = ((player->abilityTimer >> 1) & 0x7FFF8000) + 0x80000;
                    }
                    else {
                        speed = -player->groundVel;
                    }

                    speed -= 0x40000;
                    if (speed < 0) {
                        speed              = 0;
                        player->groundVel  = 0;
                        player->velocity.x = 0;
                    }

                    self->drawPos.y += speed >> 2;
                    totalSpeed += speed >> 2;
                    self->animator.timer += abs((speed >> 2) >> 16);

                    if (self->animator.timer >= 4) {
                        self->animator.timer = 0;
                        if (++self->animator.frameID > 4)
                            self->animator.frameID = 0;
                    }

                    if (!(Zone->timer & 0xF))
                        RSDK.PlaySfx(DashLift->sfxPulley, false, 255);
                }
                else {
                    if ((player->sidekick && totalSpeed > 0) || self->drawPos.y <= self->amplitude.x)
                        continue;

                    int32 speed = 0;
                    if (player->state == Player_State_Spindash) {
                        if (player->superState == SUPERSTATE_SUPER)
                            speed = -((player->abilityTimer >> 1) & 0x7FFF8000) - 0xB0000;
                        else
                            speed = -((player->abilityTimer >> 1) & 0x7FFF8000) - 0x80000;
                    }
                    else {
                        speed = -player->groundVel;
                    }

                    speed += 0x40000;
                    if (speed > 0) {
                        speed              = 0;
                        player->groundVel  = 0;
                        player->velocity.x = 0;
                    }

                    self->drawPos.y += speed >> 2;
                    totalSpeed += speed >> 2;
                    self->animator.timer -= abs((speed >> 2) >> 16);

                    if (self->animator.timer <= 0) {
                        self->animator.timer = 3;
                        if (--self->animator.frameID < 0)
                            self->animator.frameID = 4;
                    }

                    if (!(Zone->timer & 0xF))
                        RSDK.PlaySfx(DashLift->sfxPulley, false, 255);
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }

    DashLift_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void DashLift_EditorDraw(void)
{
    RSDK_THIS(DashLift);

    self->drawPos = self->position;
    RSDK.SetSpriteAnimation(Platform->aniFrames, 2, &self->animator, true, 4);

    DashLift_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        self->drawPos.y += self->startOff << 15;
        self->updateRange.y = self->length << 15;

        self->amplitude.x = self->position.y - (self->length << 15);
        self->amplitude.y = self->position.y + (self->length << 15);

        self->inkEffect = INK_BLEND;

        // Offset Preview
        DashLift_Draw();

        self->drawPos = self->position;
        self->drawPos.y += self->length << 15;

        // Length Preview
        DashLift_Draw();

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);

        self->inkEffect = INK_NONE;

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void DashLift_EditorLoad(void) {}
#endif

void DashLift_Serialize(void)
{
    RSDK_EDITABLE_VAR(DashLift, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(DashLift, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(DashLift, VAR_ENUM, startOff);
}
