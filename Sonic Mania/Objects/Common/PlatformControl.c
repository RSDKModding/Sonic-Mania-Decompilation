// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlatformControl Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPlatformControl *PlatformControl;

void PlatformControl_Update(void)
{
    RSDK_THIS(PlatformControl);
    self->active = ACTIVE_NORMAL;
    int32 nodeSlot  = RSDK.GetEntityID(self) + 1;
    int32 platSlot   = nodeSlot + self->nodeCount;

    if (self->isActive) {
        for (int32 c = 0; c < self->childCount; ++c) {
            EntityPlatform *platform = RSDK_GET_ENTITY(platSlot, Platform);
            EntityPlatformNode *node = RSDK_GET_ENTITY(platform->speed, PlatformNode);
            int32 flags                = 0;
            if (platform->velocity.x <= 0) {
                if (platform->drawPos.x <= node->position.x)
                    flags = 1;
            }
            else {
                if (platform->drawPos.x >= node->position.x)
                    flags = 1;
            }
            if (platform->velocity.y <= 0) {
                if (platform->drawPos.y <= node->position.y)
                    flags |= 2;
            }
            else {
                if (platform->drawPos.y >= node->position.y)
                    flags |= 2;
            }

            if (flags == 3) {
                platform->timer = node->nodeFlag;
                if (platform->direction < 4) {
                    if (++platform->speed - nodeSlot >= self->nodeCount) {
                        switch (self->type) {
                            case 0: platform->speed = nodeSlot; break;
                            case 1:
                                --platform->speed;
                                platform->direction = platform->direction ^ 4;
                                break;
                            case 2: {
                                Entity *ent         = RSDK.GetEntityByID(nodeSlot);
                                platform->drawPos.x = ent->position.x;
                                platform->drawPos.y = ent->position.y;
                                platform->speed     = nodeSlot + 1;
                                break;
                            }
                            case 3: self->speed = 0; break;
                        }
                    }
                }
                else {
                    if (--platform->speed - nodeSlot < 0) {
                        switch (self->type) {
                            case 0: platform->speed = nodeSlot + (self->nodeCount - 1); break;
                            case 1:
                                platform->direction = platform->direction ^ 4;
                                platform->speed     = platform->speed + 1;
                                break;
                            case 2: {
                                Entity *ent         = RSDK.GetEntityByID(nodeSlot + self->nodeCount - 1);
                                platform->drawPos.x = ent->position.x;
                                platform->drawPos.y = ent->position.y;
                                platform->speed     = nodeSlot + (self->nodeCount - 2);
                                break;
                            }
                        }
                    }
                }
                PlatformControl_ManagePlatformVelocity(platform, (Entity*)RSDK_GET_ENTITY(platform->speed, PlatformNode));
            }

            platSlot += platform->childCount + 1;
        }
    }
    else {
        EntityButton *button = (EntityButton *)self->taggedButton;
        if (button && button->currentlyActive)
            self->setActive = true;
        if (self->setActive) {
            for (int32 c = 0; c < self->childCount; ++c) {
                EntityPlatform *platform = RSDK_GET_ENTITY(platSlot, Platform);
                if (platform->state == Platform_State_WaitForControl)
                    platform->state = Platform_State_Controlled;
                if (platform->state == Platform_State_ActivateControlOnStood) {
                    self->setActive = false;
                    return;
                }
                platform->speed += nodeSlot;
                platform->active = ACTIVE_NORMAL;
                PlatformControl_ManagePlatformVelocity(platform, (Entity *)RSDK_GET_ENTITY(platform->speed, PlatformNode));
                platSlot += platform->childCount + 1;
            }
            self->isActive = true;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active = ACTIVE_BOUNDS;
        int32 slot       = nodeSlot + self->nodeCount;
        for (int32 c = 0; c < self->childCount; ++c) {
            EntityPlatform *platform = RSDK_GET_ENTITY(slot, Platform);
            if (platform->state == Platform_State_Controlled) {
                platform->speed -= nodeSlot;
                platform->state  = Platform_State_WaitForControl;
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
        int32 id         = RSDK.GetEntityID(self) + 1;
        for (int32 i = 0; i < self->nodeCount; ++i) {
            Entity *node = RSDK.GetEntityByID(id++);
            if (abs(node->position.x - self->position.x) > self->updateRange.x) 
                self->updateRange.x = abs(node->position.x - self->position.x);

            if (abs(node->position.y - self->position.y) > self->updateRange.y) 
                self->updateRange.y = abs(node->position.y - self->position.y);
        }

        if (!self->speed)
            self->speed = 4;
        self->updateRange.x += 0x800000;
        self->updateRange.y += 0x800000;
        id              = RSDK.GetEntityID(self);
        Entity *control = RSDK.GetEntityByID(id - 1);
        if (self->buttonTag > 0) {
            bool32 flag = false;
            if (Button) {
                foreach_all(Button, button)
                {
                    if (button->tag == self->buttonTag) {
                        flag    = true;
                        control = (Entity *)button;
                        foreach_break;
                    }
                }
            }
            if (SDashWheel && !flag) {
                foreach_all(SDashWheel, wheel)
                {
                    if (wheel->tag == self->buttonTag) {
                        flag = true;
                        control = (Entity *)wheel;
                        foreach_break;
                    }
                }
            }
            if (PullChain && !flag) {
                foreach_all(PullChain, chain)
                {
                    if (chain->tag == self->buttonTag) {
                        flag    = true;
                        control = (Entity *)chain;
                        foreach_break;
                    }
                }
            }
        }

        if ((Button && control->objectID == Button->objectID) || (SDashWheel && control->objectID == SDashWheel->objectID)
            || (PullChain && control->objectID == PullChain->objectID)) {
            self->taggedButton = control;
            if (self->updateRange.y < 0x800000 + abs(self->position.x - control->position.x)) {
                self->updateRange.y = 0x800000 + abs(self->position.x - control->position.x);
            }
            if (self->updateRange.y < 0x800000 + abs(self->position.y - control->position.y)) {
                self->updateRange.y = 0x800000 + abs(self->position.y - control->position.y);
            }
        }
        else {
            self->taggedButton = NULL;
        }
    }
}

void PlatformControl_StageLoad(void) {}

void PlatformControl_ManagePlatformVelocity(EntityPlatform *platform, Entity *node)
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

#if RETRO_INCLUDE_EDITOR
void PlatformControl_EditorDraw(void) {}

void PlatformControl_EditorLoad(void) {}
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
