#include "SonicMania.h"

ObjectPlatformControl *PlatformControl;

void PlatformControl_Update(void)
{
    RSDK_THIS(PlatformControl);
    entity->active = ACTIVE_NORMAL;
    int32 nodeSlot  = RSDK.GetEntityID(entity) + 1;
    int32 platSlot   = nodeSlot + entity->nodeCount;

    if (entity->field_6C) {
        for (int32 c = 0; c < entity->childCount; ++c) {
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
                platform->collapseDelay = node->nodeFlag;
                if (platform->direction < 4) {
                    if (++platform->speed - nodeSlot >= entity->nodeCount) {
                        switch (entity->type) {
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
                            case 3: entity->speed = 0; break;
                        }
                    }
                }
                else {
                    if (--platform->speed - nodeSlot < 0) {
                        switch (entity->type) {
                            case 0: platform->speed = nodeSlot + (entity->nodeCount - 1); break;
                            case 1:
                                platform->direction = platform->direction ^ 4;
                                platform->speed     = platform->speed + 1;
                                break;
                            case 2: {
                                Entity *ent         = RSDK.GetEntityByID(nodeSlot + entity->nodeCount - 1);
                                platform->drawPos.x = ent->position.x;
                                platform->drawPos.y = ent->position.y;
                                platform->speed     = nodeSlot + (entity->nodeCount - 2);
                                break;
                            }
                        }
                    }
                }
                PlatformControl_ManagePlatformVelocity(platform, RSDK.GetEntityByID(platform->speed));
            }

            platSlot += platform->childCount + 1;
        }
    }
    else {
        EntityButton *control = (EntityButton *)entity->controller;
        if (control && control->activated)
            entity->setActive = true;
        if (entity->setActive) {
            for (int32 c = 0; c < entity->childCount; ++c) {
                EntityPlatform *platform = RSDK_GET_ENTITY(platSlot, Platform);
                if (platform->state == Platform_State_Nothing)
                    platform->state = Platform_Unknown4;
                if (platform->state == Platform_Unknown8) {
                    entity->setActive = false;
                    return;
                }
                platform->speed += nodeSlot;
                platform->active = ACTIVE_NORMAL;
                PlatformControl_ManagePlatformVelocity(platform, RSDK.GetEntityByID(platform->speed));
                platSlot += platform->childCount + 1;
            }
            entity->field_6C = 1;
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->active = ACTIVE_BOUNDS;
        int32 slot       = nodeSlot + entity->nodeCount;
        for (int32 c = 0; c < entity->childCount; ++c) {
            EntityPlatform *platform = RSDK_GET_ENTITY(slot, Platform);
            if (platform->state == Platform_Unknown4) {
                platform->speed -= nodeSlot;
                platform->state  = Platform_State_Nothing;
                platform->active = ACTIVE_BOUNDS;
            }
            slot += platform->childCount + 1;
        }
        entity->field_6C = 0;
    }
}

void PlatformControl_LateUpdate(void) {}

void PlatformControl_StaticUpdate(void) {}

void PlatformControl_Draw(void) {}

void PlatformControl_Create(void *data)
{
    RSDK_THIS(PlatformControl);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_BOUNDS;
        int32 id         = RSDK.GetEntityID(entity) + 1;
        for (int32 i = 0; i < entity->nodeCount; ++i) {
            Entity *node = RSDK.GetEntityByID(id++);
            if (abs(node->position.x - entity->position.x) > entity->updateRange.x) {
                entity->updateRange.x = abs(node->position.x - entity->position.x);
            }
            if (abs(node->position.y - entity->position.y) > entity->updateRange.y) {
                entity->updateRange.y = abs(node->position.y - entity->position.y);
            }
        }
        if (!entity->speed)
            entity->speed = 4;
        entity->updateRange.x += 0x800000;
        entity->updateRange.y += 0x800000;
        id              = RSDK.GetEntityID(entity);
        Entity *control = RSDK.GetEntityByID(id - 1);
        if (entity->buttonTag > 0) {
            bool32 flag = false;
            if (Button) {
                foreach_all(Button, button)
                {
                    if (button->tag == entity->buttonTag) {
                        flag    = true;
                        control = (Entity *)button;
                        foreach_break;
                    }
                }
            }
            if (SDashWheel && !flag) {
                foreach_all(SDashWheel, wheel)
                {
                    if (wheel->tag == entity->buttonTag) {
                        flag = true;
                        control = (Entity *)wheel;
                        foreach_break;
                    }
                }
            }
            if (PullChain && !flag) {
                foreach_all(PullChain, chain)
                {
                    if (chain->tag == entity->buttonTag) {
                        flag    = true;
                        control = (Entity *)chain;
                        foreach_break;
                    }
                }
            }
        }

        if ((Button && control->objectID == Button->objectID) || (SDashWheel && control->objectID == SDashWheel->objectID)
            || (PullChain && control->objectID == PullChain->objectID)) {
            entity->controller = control;
            if (entity->updateRange.y < 0x800000 + abs(entity->position.x - control->position.x)) {
                entity->updateRange.y = 0x800000 + abs(entity->position.x - control->position.x);
            }
            if (entity->updateRange.y < 0x800000 + abs(entity->position.y - control->position.y)) {
                entity->updateRange.y = 0x800000 + abs(entity->position.y - control->position.y);
            }
        }
        else {
            entity->controller = NULL;
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
            platform->velocity.x = entity->speed * (((distX << 16) / distY) >> 2);
        else
            platform->velocity.x = 0;
        platform->velocity.y = entity->speed << 14;
    }
    else {
        platform->velocity.x = entity->speed << 14;
        if (distX)
            platform->velocity.y = entity->speed * (((distY << 16) / distX) >> 2);
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
