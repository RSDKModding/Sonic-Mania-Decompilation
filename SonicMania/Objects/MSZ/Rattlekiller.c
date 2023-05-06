// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Rattlekiller Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRattlekiller *Rattlekiller;

void Rattlekiller_Update(void)
{
    RSDK_THIS(Rattlekiller);

    if (self->timer) {
        self->timer--;

        if (self->bodyStates[0] == RATTLEKILLER_PLAYERDETECTED) {
            foreach_active(Player, player)
            {
                if (abs(self->bodyPositions[0].x - player->position.x) < 0x600000) {
                    if (abs(self->bodyPositions[0].y - player->position.y) < 0x600000) {
                        self->targetPos = player->position;
                        foreach_break;
                    }
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (self->bodyPositions[0].x <= self->topBounds.x) {
                if (player->position.x >= self->topBounds.x || player->position.x - self->topBounds.x <= -0x600000)
                    continue;
            }
            else if (player->position.x <= self->topBounds.x || player->position.x - self->topBounds.x >= 0x600000) {
                continue;
            }

            if (abs(self->bodyPositions[0].y - player->position.y) < 0x600000) {
                int32 delay = 30;
                for (int32 i = 0; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
                    self->bodyDelays[i]    = 30;
                    self->bodyOriginPos[i] = self->bodyPositions[i];
                    self->bodyStates[i]    = RATTLEKILLER_PLAYERDETECTED;

                    delay += 2;
                }

                self->targetPos.x = player->position.x;
                self->targetPos.y = player->position.y;
                self->timer       = 480;
                foreach_break;
            }
        }
    }

    foreach_active(Player, player)
    {
        for (int32 i = 0; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
            switch (self->bodyStates[i]) {
                case RATTLEKILLER_IDLE:
                    if (--self->bodyDelays[i] <= 0)
                        self->bodyStates[i] = RATTLEKILLER_TWIST_DOWN + (self->bodyVelocities[i].y < 0);
                    break;

                case RATTLEKILLER_TWIST_HORIZONTAL: // circle pole horizontally (no vertical movement)
                    self->bodyAngles[i] = (self->bodyAngles[i] + 2) & 0xFF;
                    self->bodyPositions[i].y += self->bodyVelocities[i].y;
                    self->bodyPositions[i].x = self->topBounds.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                    self->bodyDepth[i]       = RSDK.Cos256(self->bodyAngles[i]);
                    if (self->timer < 320) {
                        if (--self->bodyDelays[i] <= 0)
                            self->bodyStates[i] = RATTLEKILLER_TWIST_DOWN + (self->bodyVelocities[i].y < 0);
                    }
                    break;

                case RATTLEKILLER_TWIST_DOWN: // circle pole (moving downwards)
                    self->bodyAngles[i] = (self->bodyAngles[i] + 2) & 0xFF;
                    self->bodyVelocities[i].y += 0x400;
                    if (self->bodyVelocities[i].y > 0x8000)
                        self->bodyVelocities[i].y = 0x8000;

                    self->bodyPositions[i].y += self->bodyVelocities[i].y;
                    self->bodyPositions[i].x = self->topBounds.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                    self->bodyDepth[i]       = RSDK.Cos256(self->bodyAngles[i]);

                    if (self->bodyPositions[i].y >= self->bottomBounds.y)
                        self->bodyStates[i] = RATTLEKILLER_TWIST_UP;
                    break;

                case RATTLEKILLER_TWIST_UP: // circle pole (moving upwards)
                    self->bodyAngles[i] = (self->bodyAngles[i] + 2) & 0xFF;
                    self->bodyVelocities[i].y -= 0x400;
                    if (self->bodyVelocities[i].y < -0x8000)
                        self->bodyVelocities[i].y = -0x8000;

                    self->bodyPositions[i].y += self->bodyVelocities[i].y;
                    self->bodyPositions[i].x = self->topBounds.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                    self->bodyDepth[i]       = RSDK.Cos256(self->bodyAngles[i]);
                    if (self->bodyPositions[i].y <= self->topBounds.y)
                        self->bodyStates[i] = RATTLEKILLER_TWIST_DOWN;
                    break;

                case RATTLEKILLER_PLAYERDETECTED:
                    if (--self->bodyDelays[i] > 0) {
                        if (self->bodyDelays[i] < 15 && i > 4) {
                            self->bodyAngles[i]      = (self->bodyAngles[i] + 4) & 0xFF;
                            self->bodyPositions[i].x = self->topBounds.x - 0x1800 * RSDK.Sin256(self->bodyAngles[i]);
                            self->bodyDepth[i]       = RSDK.Cos256(self->bodyAngles[i]);
                        }
                    }
                    else if (i) {
                        self->bodyStates[i] = RATTLEKILLER_STRETCHBODY;
                    }
                    else {
                        self->bodyStates[0] = RATTLEKILLER_EXTEND;
                        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, (self->bodyPositions[0].x > self->topBounds.x) + 2, &self->headAnimator,
                                                true, 0);
                        RSDK.PlaySfx(Rattlekiller->sfxRocketJet, false, 255);
                    }
                    break;

                case RATTLEKILLER_STRETCHBODY:
                    if (self->bodyStates[0] < RATTLEKILLER_EXTEND) {
                        self->bodyPositions[i].y += (self->bodyPositions[0].y - self->bodyPositions[i].y) >> 3;
                        if (self->bodyStates[i - 1] == RATTLEKILLER_TWIST_HORIZONTAL) {
                            if (abs(self->bodyPositions[0].y - self->bodyPositions[i].y) < 0x20000) {
                                self->bodyPositions[i].y  = self->bodyPositions[0].y;
                                self->bodyVelocities[i].x = 0;
                                self->bodyVelocities[i].y = 0;
                                self->bodyStates[i]       = RATTLEKILLER_TWIST_HORIZONTAL;
                                self->bodyDelays[i]       = 8 * i + self->bodyDelays[0];
                                self->bodyAngles[i]       = self->bodyAngles[i - 1] - 16;
                            }
                        }
                    }
                    else if (i < RATTLEKILLER_SEGMENT_COUNT - 1) {
                        self->bodyPositions[i].x = (self->bodyPositions[i - 1].x >> 1) + (self->bodyPositions[i + 1].x >> 1);
                        self->bodyPositions[i].y = (self->bodyPositions[i - 1].y >> 1) + (self->bodyPositions[i + 1].y >> 1);
                    }
                    break;

                case RATTLEKILLER_EXTEND: {
                    int32 rx = (self->targetPos.x - self->bodyPositions[i].x) >> 16;
                    int32 ry = (self->targetPos.y - self->bodyPositions[i].y) >> 16;
                    if (rx * rx + ry * ry >= 0x40) {
                        int32 angle               = RSDK.ATan2(rx, ry);
                        self->bodyVelocities[i].x = RSDK.Cos256(angle) << 10;
                        self->bodyPositions[i].x += self->bodyVelocities[i].x;
                        self->bodyPositions[i].y += RSDK.Sin256(angle) << 10;
                        self->bodyVelocities[i].y = RSDK.Sin256(angle) << 10;
                    }
                    else {
                        self->bodyStates[i] = RATTLEKILLER_RETRACT;
                    }
                    RSDK.ProcessAnimation(&self->headAnimator);
                    break;
                }

                case RATTLEKILLER_RETRACT: {
                    int32 rx = (self->bodyOriginPos[i].x - self->bodyPositions[i].x) >> 16;
                    int32 ry = (self->bodyOriginPos[RATTLEKILLER_SEGMENT_COUNT - 1].y - self->bodyPositions[i].y) >> 16;
                    if (rx * rx + ry * ry >= 0x40) {
                        int32 angle               = RSDK.ATan2(rx, ry);
                        self->bodyVelocities[i].x = RSDK.Cos256(angle) << 10;
                        self->bodyPositions[i].x += self->bodyVelocities[i].x;
                        self->bodyPositions[i].y += RSDK.Sin256(angle) << 10;
                        self->bodyVelocities[i].y = RSDK.Sin256(angle) << 10;
                        RSDK.ProcessAnimation(&self->headAnimator);
                    }
                    else {
                        self->bodyStates[i] = RATTLEKILLER_TWIST_HORIZONTAL;
                        if (i > 0)
                            self->bodyAngles[i] = self->bodyAngles[i - 1] - 16;
                        self->bodyPositions[i].x  = self->bodyOriginPos[i].x;
                        self->bodyPositions[i].y  = self->bodyOriginPos[RATTLEKILLER_SEGMENT_COUNT - 1].y;
                        self->bodyDelays[i]       = 8 * i;
                        self->bodyVelocities[i].x = 0;
                        self->bodyVelocities[i].y = 0;
                        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &self->headAnimator, true, 0);
                    }
                    break;
                }

                default: break;
            }
        }

        self->position.x = self->bodyPositions[0].x;
        self->position.y = self->bodyPositions[0].y;
        if (Player_CheckBadnikTouch(player, self, &Rattlekiller->hitboxSegment) && Player_CheckBadnikBreak(player, self, false)) {
            for (int32 i = 1; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPositions[i].x, self->bodyPositions[i].y);

                RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, self->bodyAnimators[i]->animationID, &debris->animator, true,
                                        self->bodyAnimators[i]->frameID);
                debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
                debris->gravityStrength = 0x4800;
                debris->drawGroup       = Zone->objectDrawGroup[1];
                debris->updateRange.x   = 0x400000;
                debris->updateRange.y   = 0x400000;
            }

            destroyEntity(self);
            self->active = ACTIVE_DISABLED;
            foreach_break;
        }
        else {
            for (int32 i = 1; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
                if (self->bodyDepth[i] > 0) {
                    self->position.x = self->bodyPositions[i].x;
                    self->position.y = self->bodyPositions[i].y;
                    if (Player_CheckCollisionTouch(player, self, &Rattlekiller->hitboxSegment)) {
#if MANIA_USE_PLUS
                        if (!Player_CheckMightyUnspin(player, 0x200, 2, &player->uncurlTimer))
#endif
                            Player_Hurt(player, self);
                    }
                }
            }
        }
    }

    self->position.x = self->bodyPositions[0].x;
    self->position.y = self->bodyPositions[0].y;

    Rattlekiller_HandleSorting();
}

void Rattlekiller_LateUpdate(void) {}

void Rattlekiller_StaticUpdate(void)
{
    foreach_active(Rattlekiller, rattlekiller) { RSDK.AddDrawListRef(Zone->objectDrawGroup[0] - 1, RSDK.GetEntitySlot(rattlekiller)); }
}

void Rattlekiller_Draw(void)
{
    RSDK_THIS(Rattlekiller);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        for (int32 i = 0; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
            int32 id = self->bodyIDs[i];

            if (self->bodyDepth[id] > 0) {
                Animator *animator = self->bodyAnimators[id];
                if (animator->animationID < 2)
                    animator->frameID = self->bodyAngles[id] >> 4;

                RSDK.DrawSprite(animator, &self->bodyPositions[id], false);
            }
        }
    }
    else {
        for (int32 i = 0; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
            int32 id = self->bodyIDs[i];

            if (self->bodyDepth[id] <= 0) {
                Animator *animator = self->bodyAnimators[id];

                if (animator->animationID < 2)
                    animator->frameID = self->bodyAngles[id] >> 4;
                RSDK.DrawSprite(animator, &self->bodyPositions[id], false);
            }
        }
    }
}

void Rattlekiller_Create(void *data)
{
    RSDK_THIS(Rattlekiller);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->topBounds     = self->position;
        self->bottomBounds  = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        if (data)
            self->length = VOID_TO_INT(data);
        else if (!self->length)
            self->length = 128;

        self->topBounds.y -= self->length << 15;
        self->bottomBounds.y += self->length << 15;

        int32 delay = 0;
        for (int32 i = 0; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
            self->bodyIDs[i]       = i;
            self->bodyAnimators[i] = &self->bodyAnimator;
            self->bodyDelays[i]    = delay;
            self->bodyPositions[i] = self->topBounds;

            delay += 8;
        }
        self->bodyAnimators[0] = &self->headAnimator;

        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &self->headAnimator, true, 0);
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 1, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 4, &self->tailAnimator, true, 0);
    }
}

void Rattlekiller_StageLoad(void)
{
    Rattlekiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Rattlekiller.bin", SCOPE_STAGE);

    Rattlekiller->hitboxSegment.left   = -8;
    Rattlekiller->hitboxSegment.top    = -8;
    Rattlekiller->hitboxSegment.right  = 8;
    Rattlekiller->hitboxSegment.bottom = 8;

    Rattlekiller->sfxRocketJet = RSDK.GetSfx("Stage/RocketJet.wav");
    // Idk where it would've been used but "MSZ/RKillerRattle.wav" was prolly intended for this object

    DEBUGMODE_ADD_OBJ(Rattlekiller);
}

void Rattlekiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityRattlekiller *rattlekiller = CREATE_ENTITY(Rattlekiller, INT_TO_VOID(0x80), self->position.x, self->position.y);
    rattlekiller->direction          = FLIP_NONE;
}

void Rattlekiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rattlekiller_HandleSorting(void)
{
    RSDK_THIS(Rattlekiller);

    for (int32 i = 0; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
        for (int32 ii = RATTLEKILLER_SEGMENT_COUNT - 1; ii > i; --ii) {
            int32 id1 = self->bodyIDs[ii - 1];
            int32 id2 = self->bodyIDs[ii];
            if (self->bodyDepth[id1] > self->bodyDepth[id2]) {
                self->bodyIDs[ii - 1] = id2;
                self->bodyIDs[ii]     = id1;
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Rattlekiller_EditorDraw(void)
{
    RSDK_THIS(Rattlekiller);

    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 0, &self->headAnimator, false, 0);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 1, &self->bodyAnimator, false, 0);
    RSDK.SetSpriteAnimation(Rattlekiller->aniFrames, 4, &self->tailAnimator, false, 0);

    self->topBounds    = self->position;
    self->bottomBounds = self->position;

    int32 length = self->length;
    if (!length)
        length = 128;

    self->topBounds.y -= length << 15;
    self->bottomBounds.y += length << 15;

    int32 delay = 0;
    for (int32 i = 0; i < RATTLEKILLER_SEGMENT_COUNT; ++i) {
        self->bodyIDs[i]       = i;
        self->bodyAnimators[i] = &self->bodyAnimator;
        self->bodyDelays[i]    = delay;
        self->bodyPositions[i] = self->position;

        delay += 8;
    }

    Rattlekiller_HandleSorting();

    int32 drawGroup = self->drawGroup;

    // Draw Lower Depth
    self->drawGroup = -1;
    Rattlekiller_Draw();

    // Draw Higher Depth
    self->drawGroup = SceneInfo->currentDrawGroup;
    Rattlekiller_Draw();

    self->drawGroup = drawGroup;

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->topBounds.x, self->topBounds.y, 0xFFFF00, INK_NONE, 0xFF);
        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->bottomBounds.x, self->bottomBounds.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Rattlekiller_EditorLoad(void)
{
    Rattlekiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Rattlekiller.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Rattlekiller, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Rattlekiller_Serialize(void)
{
    RSDK_EDITABLE_VAR(Rattlekiller, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Rattlekiller, VAR_ENUM, length);
}
