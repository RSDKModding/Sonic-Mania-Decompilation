// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlatformControl Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPlatformControl *PlatformControl;

void PlatformControl_Update(void)
{
    RSDK_THIS(PlatformControl);

    self->active = ACTIVE_NORMAL;

    int32 startNodeSlot = RSDK.GetEntitySlot(self) + 1;
    int32 platformSlot  = startNodeSlot + self->nodeCount;

    if (self->isActive) {
        for (int32 c = 0; c < self->childCount; ++c) {
            EntityPlatform *platform = RSDK_GET_ENTITY(platformSlot, Platform);
            EntityPlatformNode *node = RSDK_GET_ENTITY(platform->speed, PlatformNode);

            int32 finishDir = 0;
            if (platform->velocity.x <= 0) {
                if (platform->drawPos.x <= node->position.x)
                    finishDir = 1;
            }
            else {
                if (platform->drawPos.x >= node->position.x)
                    finishDir = 1;
            }

            if (platform->velocity.y <= 0) {
                if (platform->drawPos.y <= node->position.y)
                    finishDir |= 2;
            }
            else {
                if (platform->drawPos.y >= node->position.y)
                    finishDir |= 2;
            }

            if (finishDir == (1 | 2)) {
                platform->timer = node->nodeFlag;

                if (platform->direction < 4) {
                    int32 nodeID = ++platform->speed - startNodeSlot;
                    if (nodeID >= self->nodeCount) {
                        switch (self->type) {
                            case PLATFORMCONTROL_CIRCUIT: platform->speed = startNodeSlot; break;

                            case PLATFORMCONTROL_REVERSE:
                                --platform->speed;
                                platform->direction = platform->direction ^ 4;
                                break;

                            case PLATFORMCONTROL_TELEPORT: {
                                Entity *startNode   = RSDK_GET_ENTITY_GEN(startNodeSlot);
                                platform->drawPos.x = startNode->position.x;
                                platform->drawPos.y = startNode->position.y;
                                platform->speed     = startNodeSlot + 1;
                                break;
                            }

                            case PLATFORMCONTROL_STOP: self->speed = 0; break;
                        }
                    }
                }
                else {
                    int32 nodeID = --platform->speed - startNodeSlot;
                    if (nodeID < 0) {
                        switch (self->type) {
                            default:
                            case PLATFORMCONTROL_STOP: break;

                            case PLATFORMCONTROL_CIRCUIT: platform->speed = startNodeSlot + (self->nodeCount - 1); break;

                            case PLATFORMCONTROL_REVERSE:
                                platform->direction = platform->direction ^ 4;
                                platform->speed     = platform->speed + 1;
                                break;

                            case PLATFORMCONTROL_TELEPORT: {
                                Entity *startNode   = RSDK_GET_ENTITY_GEN(startNodeSlot + self->nodeCount - 1);
                                platform->drawPos.x = startNode->position.x;
                                platform->drawPos.y = startNode->position.y;
                                platform->speed     = startNodeSlot + (self->nodeCount - 2);
                                break;
                            }
                        }
                    }
                }

                PlatformControl_ManagePlatformVelocity(platform, RSDK_GET_ENTITY(platform->speed, PlatformNode));
            }

            platformSlot += platform->childCount + 1;
        }
    }
    else {
        EntityButton *button = self->taggedButton;

        if (button && button->currentlyActive)
            self->setActive = true;

        if (self->setActive) {
            for (int32 c = 0; c < self->childCount; ++c) {
                EntityPlatform *platform = RSDK_GET_ENTITY(platformSlot, Platform);
                if (platform->state == Platform_State_PathStop)
                    platform->state = Platform_State_Path;

                if (platform->state == Platform_State_PathReact) {
                    self->setActive = false;
                    return;
                }

                platform->speed += startNodeSlot;
                platform->active = ACTIVE_NORMAL;
                PlatformControl_ManagePlatformVelocity(platform, RSDK_GET_ENTITY(platform->speed, PlatformNode));
                platformSlot += platform->childCount + 1;
            }

            self->isActive = true;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active = ACTIVE_BOUNDS;

        int32 slot = startNodeSlot + self->nodeCount;
        for (int32 c = 0; c < self->childCount; ++c) {
            EntityPlatform *platform = RSDK_GET_ENTITY(slot, Platform);
            if (platform->state == Platform_State_Path) {
                platform->speed -= startNodeSlot;
                platform->state  = Platform_State_PathStop;
                platform->active = ACTIVE_BOUNDS;
            }
            slot += platform->childCount + 1;
        }

        self->isActive = false;
    }
}

void PlatformControl_LateUpdate(void) {}

void PlatformControl_StaticUpdate(void) {}

void PlatformControl_Draw(void) {}

void PlatformControl_Create(void *data)
{
    RSDK_THIS(PlatformControl);

    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_BOUNDS;

        int32 id = RSDK.GetEntitySlot(self) + 1;
        for (int32 i = 0; i < self->nodeCount; ++i) {
            Entity *node = RSDK_GET_ENTITY_GEN(id++);

            if (abs(node->position.x - self->position.x) > self->updateRange.x)
                self->updateRange.x = abs(node->position.x - self->position.x);

            if (abs(node->position.y - self->position.y) > self->updateRange.y)
                self->updateRange.y = abs(node->position.y - self->position.y);
        }

        if (!self->speed)
            self->speed = 4;

        self->updateRange.x += TO_FIXED(128);
        self->updateRange.y += TO_FIXED(128);

        self->taggedButton         = NULL;
        EntityButton *taggedButton = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) - 1, Button);
        if (self->buttonTag > 0) {
            bool32 foundButton = false;
            if (Button) {
                foreach_all(Button, button)
                {
                    if (button->tag == self->buttonTag) {
                        taggedButton = button;
                        foundButton  = true;
                        foreach_break;
                    }
                }
            }

            if (SDashWheel && !foundButton) {
                foreach_all(SDashWheel, wheel)
                {
                    if (wheel->tag == self->buttonTag) {
                        taggedButton = (EntityButton *)wheel;
                        foundButton  = true;
                        foreach_break;
                    }
                }
            }

            if (PullChain && !taggedButton) {
                foreach_all(PullChain, chain)
                {
                    if (chain->tag == self->buttonTag) {
                        taggedButton = (EntityButton *)chain;
                        foundButton  = true;
                        foreach_break;
                    }
                }
            }
        }

        if (taggedButton) {
            if ((Button && taggedButton->classID == Button->classID) || (SDashWheel && taggedButton->classID == SDashWheel->classID)
                || (PullChain && taggedButton->classID == PullChain->classID)) {
                self->taggedButton = taggedButton;
                if (self->updateRange.x < TO_FIXED(128) + abs(self->position.x - taggedButton->position.x))
                    self->updateRange.x = TO_FIXED(128) + abs(self->position.x - taggedButton->position.x);

                if (self->updateRange.y < TO_FIXED(128) + abs(self->position.y - taggedButton->position.y))
                    self->updateRange.y = TO_FIXED(128) + abs(self->position.y - taggedButton->position.y);
            }
        }
    }
}

void PlatformControl_StageLoad(void) {}

void PlatformControl_ManagePlatformVelocity(EntityPlatform *platform, EntityPlatformNode *node)
{
    RSDK_THIS(PlatformControl);

    int32 distX = abs((node->position.x - platform->drawPos.x) >> 16);
    int32 distY = abs((node->position.y - platform->drawPos.y) >> 16);

    if (distY >= distX) {
        if (distY)
            platform->velocity.x = self->speed * (((distX << 16) / distY) >> 2);
        else
            platform->velocity.x = 0;

        platform->velocity.y = self->speed << 14;
    }
    else {
        platform->velocity.x = self->speed << 14;

        if (distX)
            platform->velocity.y = self->speed * (((distY << 16) / distX) >> 2);
        else
            platform->velocity.y = 0;
    }

    if (node->position.x < platform->drawPos.x)
        platform->velocity.x = -platform->velocity.x;
    if (node->position.y < platform->drawPos.y)
        platform->velocity.y = -platform->velocity.y;
}

#if GAME_INCLUDE_EDITOR
void PlatformControl_EditorDraw(void)
{
    RSDK_THIS(PlatformControl);

    if (HUD) {
        Animator animator;
        RSDK.SetSpriteAnimation(HUD->aniFrames, 0, &animator, false, 7);
        RSDK.DrawSprite(&animator, NULL, false);
    }

    self->updateRange.x = TO_FIXED(128);
    self->updateRange.y = TO_FIXED(128);

    if (showGizmos()) {
        self->taggedButton         = NULL;
        EntityButton *taggedButton = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) - 1, Button);
        if (self->buttonTag > 0) {
            bool32 foundButton = false;
            if (Button) {
                foreach_all(Button, button)
                {
                    if (button->tag == self->buttonTag) {
                        taggedButton = button;
                        foundButton  = true;
                        foreach_break;
                    }
                }
            }

            if (SDashWheel && !foundButton) {
                foreach_all(SDashWheel, wheel)
                {
                    if (wheel->tag == self->buttonTag) {
                        taggedButton = (EntityButton *)wheel;
                        foundButton  = true;
                        foreach_break;
                    }
                }
            }

            if (PullChain && !taggedButton) {
                foreach_all(PullChain, chain)
                {
                    if (chain->tag == self->buttonTag) {
                        taggedButton = (EntityButton *)chain;
                        foundButton  = true;
                        foreach_break;
                    }
                }
            }
        }

        if (taggedButton) {
            if ((Button && taggedButton->classID == Button->classID) || (SDashWheel && taggedButton->classID == SDashWheel->classID)
                || (PullChain && taggedButton->classID == PullChain->classID)) {
                self->taggedButton = taggedButton;
                if (self->updateRange.x < TO_FIXED(128) + abs(self->position.x - taggedButton->position.x))
                    self->updateRange.x = TO_FIXED(128) + abs(self->position.x - taggedButton->position.x);

                if (self->updateRange.y < TO_FIXED(128) + abs(self->position.y - taggedButton->position.y))
                    self->updateRange.y = TO_FIXED(128) + abs(self->position.y - taggedButton->position.y);
            }
        }

        RSDK_DRAWING_OVERLAY(true);
        if (self->taggedButton) {
            DrawHelpers_DrawArrow(self->taggedButton->position.x, self->taggedButton->position.y, self->position.x, self->position.y, 0xFFFF00,
                                  INK_NONE, 0xFF);
        }

        int32 startNodeSlot = RSDK.GetEntitySlot(self) + 1;

        /*EntityPlatformNode *lastNode = RSDK_GET_ENTITY(startNodeSlot, PlatformNode);
        for (int32 n = 1; n < self->nodeCount; ++n) {
            EntityPlatformNode *node = RSDK_GET_ENTITY(startNodeSlot + n, PlatformNode);

            if (lastNode && node) {
                DrawHelpers_DrawArrow(lastNode->position.x, lastNode->position.y, node->position.x, node->position.y, 0xFF0000, INK_NONE, 0xFF);
            }
            lastNode = node;
        }

        if (lastNode) {
            EntityPlatformNode *node = RSDK_GET_ENTITY(startNodeSlot, PlatformNode);
            if (node && self->type != PLATFORMCONTROL_STOP) {
                DrawHelpers_DrawArrow(lastNode->position.x, lastNode->position.y, node->position.x, node->position.y, 0xFF0000, INK_NONE, 0xFF);
            }
        }*/

        int32 platformSlot = startNodeSlot + self->nodeCount;
        for (int32 c = 0; c < self->childCount; ++c) {
            EntityPlatform *platform = RSDK_GET_ENTITY(platformSlot, Platform);
            platformSlot++;

            if (platform) {
                platformSlot += platform->childCount;
                DrawHelpers_DrawArrow(self->position.x, self->position.y, platform->position.x, platform->position.y, 0xFFFFFF, INK_NONE, 0xFF);
            }
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void PlatformControl_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(PlatformControl, type);
    RSDK_ENUM_VAR("Circuit", PLATFORMCONTROL_CIRCUIT);
    RSDK_ENUM_VAR("Reverse", PLATFORMCONTROL_REVERSE);
    RSDK_ENUM_VAR("Teleport", PLATFORMCONTROL_TELEPORT);
    RSDK_ENUM_VAR("Stop", PLATFORMCONTROL_STOP);
}
#endif

void PlatformControl_Serialize(void)
{
    RSDK_EDITABLE_VAR(PlatformControl, VAR_ENUM, nodeCount);
    RSDK_EDITABLE_VAR(PlatformControl, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(PlatformControl, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(PlatformControl, VAR_BOOL, setActive);
    RSDK_EDITABLE_VAR(PlatformControl, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(PlatformControl, VAR_ENUM, buttonTag);
}
