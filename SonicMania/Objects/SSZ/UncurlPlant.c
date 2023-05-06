// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UncurlPlant Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUncurlPlant *UncurlPlant;

void UncurlPlant_Update(void)
{
    RSDK_THIS(UncurlPlant);

    if (!self->stood) {
        if (self->uncurlPercent > 0)
            self->uncurlPercent -= 0x10;
    }
    else {

        if (self->uncurlMode <= 0) { // Very Slow uncurl
            for (int32 n = 0; n < self->stoodNodeID; ++n)
                self->targetNodeAngles[n] += (UncurlPlant->targetNodeAnglesStood[n] - self->targetNodeAngles[n]) >> 2;

            for (int32 n = self->stoodNodeID; n < UNCURLPLANT_NODE_COUNT; ++n)
                self->targetNodeAngles[n] += (UncurlPlant->targetNodeAnglesReleased[n] - self->targetNodeAngles[n]) >> 2;
        }
        else {
            for (int32 n = 0; n < self->stoodNodeID; ++n) self->targetNodeAngles[n] = UncurlPlant->targetNodeAnglesStood[n];

            for (int32 n = self->stoodNodeID; n < UNCURLPLANT_NODE_COUNT; ++n) self->targetNodeAngles[n] = UncurlPlant->targetNodeAnglesReleased[n];
        }

        if (self->uncurlMode > 1)
            self->uncurlPercent = 0x100;
        else if (self->uncurlPercent < 0x100)
            self->uncurlPercent += 0x20;
    }

    for (int32 n = 0; n < UNCURLPLANT_NODE_COUNT; ++n)
        self->nodeAngles[n] =
            UncurlPlant->startingNodeAngles[n] + ((self->uncurlPercent * (self->targetNodeAngles[n] - UncurlPlant->startingNodeAngles[n])) >> 8);

    int32 prevStoodNode = self->stoodNodeID;
    self->stood         = false;
    self->stoodNodeID   = -1;
    self->uncurlMode    = 0;
    UncurlPlant_CalculateDrawPositions();

    foreach_active(Player, player)
    {
        int32 angle  = 0;
        int32 nodeID = 0;
        for (int32 n = 0; n < UNCURLPLANT_NODE_COUNT; ++n) {
            angle += self->nodeAngles[n];
            if (angle >= 0x90)
                break;

            self->position.x = self->nodePositions[n].x;
            self->position.y = self->nodePositions[n].y;
            if (Player_CheckCollisionPlatform(player, self, &UncurlPlant->hitboxNode)) {
                player->position.y += 0x40000;
                self->stood = true;
                if (nodeID > self->stoodNodeID) {
                    if (abs(player->velocity.x) >= 0xC0000)
                        self->uncurlMode = 2; // Fast uncurl
                    else if (abs(player->velocity.x) >= 0x40000)
                        self->uncurlMode = 1; // Slow uncurl

                    self->stoodNodeID = nodeID;
                }
            }
            ++nodeID;
        }
    }

    self->stoodNodeID++;
    if (self->stood) {
        if (prevStoodNode > self->stoodNodeID)
            self->stoodNodeID = prevStoodNode - 1;
        else if (prevStoodNode < self->stoodNodeID)
            self->stoodNodeID = prevStoodNode + 1;
    }

    self->position.x = self->drawPositions[0].x;
    self->position.y = self->drawPositions[0].y;
}

void UncurlPlant_LateUpdate(void) {}

void UncurlPlant_StaticUpdate(void) {}

void UncurlPlant_Draw(void)
{
    RSDK_THIS(UncurlPlant);

    for (int32 i = 0; i < UNCURLPLANT_NODE_COUNT; ++i) {
        RSDK.DrawSprite(&self->nodeAnimator, &self->drawPositions[i], false);
        RSDK.DrawSprite(&self->decorAnimators[i], &self->drawPositions[i], false);
    }
}

void UncurlPlant_Create(void *data)
{
    RSDK_THIS(UncurlPlant);

    if (SceneInfo->inEditor) {
        self->nodeAngles[0] = 0x00;
        self->nodeAngles[1] = -0x10;
        self->nodeAngles[2] = 0x40;
        self->nodeAngles[3] = 0x60;
        self->nodeAngles[4] = 0x70;
        self->nodeAngles[5] = 0x90;
        self->nodeAngles[6] = 0xA0;
        self->nodeAngles[7] = 0xC0;
    }
    else {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1] - 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        UncurlPlant_CalculatePositions();

        RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &self->nodeAnimator, true, 0);
        for (int32 i = 0; i < UNCURLPLANT_NODE_COUNT; ++i) {
            RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &self->decorAnimators[i], true, RSDK.Rand(1, 8));
            self->nodeAngles[i] = UncurlPlant->startingNodeAngles[i];
        }
    }
}

void UncurlPlant_StageLoad(void)
{
    UncurlPlant->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);

    UncurlPlant->hitboxNode.left   = -8;
    UncurlPlant->hitboxNode.top    = -12;
    UncurlPlant->hitboxNode.right  = 8;
    UncurlPlant->hitboxNode.bottom = 8;
}

void UncurlPlant_CalculateDrawPositions(void)
{
    RSDK_THIS(UncurlPlant);

    int32 angle              = self->nodeAngles[0];
    self->drawPositions[0].x = self->position.x;
    self->drawPositions[0].y = self->position.y;

    if (self->direction == FLIP_NONE) {
        for (int32 i = 1; i < UNCURLPLANT_NODE_COUNT; ++i) {
            angle += self->nodeAngles[i];
            self->drawPositions[i].x = self->drawPositions[i - 1].x + (RSDK.Cos1024(angle) << 10);
            self->drawPositions[i].y = self->drawPositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
    else {
        for (int32 i = 1; i < UNCURLPLANT_NODE_COUNT; ++i) {
            angle += self->nodeAngles[i];
            self->drawPositions[i].x = self->drawPositions[i - 1].x - (RSDK.Cos1024(angle) << 10);
            self->drawPositions[i].y = self->drawPositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
}

void UncurlPlant_CalculatePositions(void)
{
    RSDK_THIS(UncurlPlant);

    int32 angle              = UncurlPlant->targetNodeAnglesStood[0];
    self->nodePositions[0].x = self->position.x;
    self->nodePositions[0].y = self->position.y;

    if (self->direction == FLIP_NONE) {
        for (int32 i = 1; i < UNCURLPLANT_NODE_COUNT; ++i) {
            angle += UncurlPlant->targetNodeAnglesStood[i];
            self->nodePositions[i].x = self->nodePositions[i - 1].x + (RSDK.Cos1024(angle) << 10);
            self->nodePositions[i].y = self->nodePositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
    else {
        for (int32 i = 1; i < UNCURLPLANT_NODE_COUNT; ++i) {
            angle += UncurlPlant->targetNodeAnglesStood[i];
            self->nodePositions[i].x = self->nodePositions[i - 1].x - (RSDK.Cos1024(angle) << 10);
            self->nodePositions[i].y = self->nodePositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void UncurlPlant_EditorDraw(void)
{
    RSDK_THIS(UncurlPlant);

    RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &self->nodeAnimator, false, 0);
    UncurlPlant_CalculateDrawPositions();

    for (int32 i = 0; i < UNCURLPLANT_NODE_COUNT; ++i) RSDK.DrawSprite(&self->nodeAnimator, &self->drawPositions[i], false);
}

void UncurlPlant_EditorLoad(void)
{
    UncurlPlant->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UncurlPlant, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void UncurlPlant_Serialize(void) { RSDK_EDITABLE_VAR(UncurlPlant, VAR_UINT8, direction); }
