#include "SonicMania.h"

ObjectPopOut *PopOut;

void PopOut_Update(void)
{
    RSDK_THIS(PopOut);

    int storeDir        = entity->direction;
    int storeX          = entity->position.x;
    int storeY          = entity->position.y;
    EntitySpring *child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, Spring);
    if (child->objectID != Spring->objectID && child->objectID != Spikes->objectID)
        child = NULL;

    entity->direction = 0;
    if (!entity->manualTrigger) {
        entity->flag = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                entity->flag = true;
                foreach_break;
            }
        }
    }
    else if (PopOut->hasButton) {
        foreach_active(Button, button)
        {
            if (button->tag == entity->tag && button->field_70 == 1)
                entity->flag = true;
        }
    }

    if (entity->flag) {
        if (entity->field_7C < entity->delay) {
            entity->field_7C++;
        }
        else {
            if (entity->field_78 < 8) {
                entity->field_78++;
                if (entity->childType <= 0) {
                    if (child) {
                        if (entity->field_78 == 8) {
                            child->timer               = 0;
                            child->animator.animationSpeed = 0;
                            child->animator.frameID        = 0;
                        }
                    }
                }
            }
        }
    }
    else {
        entity->field_7C = 0;
        if (entity->field_78 > 0) {
            entity->field_78--;
            if (entity->childType <= 0) {
                if (child) {
                    if (entity->field_78 == 7) {
                        child->timer               = 0;
                        child->animator.animationSpeed = 0;
                        child->animator.frameID        = 0;
                    }
                }
            }
        }
    }

    if (child) {
        if (entity->field_78 == 8) {
            child->active  = ACTIVE_BOUNDS;
            child->visible = true;
        }
        else {
            child->active  = ACTIVE_NEVER;
            child->visible = false;
        }
    }

    entity->active     = (entity->field_78 <= 0) ? ACTIVE_BOUNDS : ACTIVE_NORMAL;
    entity->position.x = -32 * entity->field_80 + entity->position.x + 4 * entity->field_78 * entity->field_80;
    entity->position.y = -32 * entity->field_84 + entity->position.y + 4 * entity->field_78 * entity->field_84;
    foreach_active(Player, playerLoop) { Player_CheckCollisionBox(playerLoop, entity, &entity->hitbox2); }
    entity->direction  = storeDir;
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void PopOut_LateUpdate(void) {}

void PopOut_StaticUpdate(void) {}

void PopOut_Draw(void)
{
    RSDK_THIS(PopOut);

    int dir            = entity->direction;
    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = -16 * entity->field_80 + entity->position.x + 2 * entity->field_78 * entity->field_80;
    entity->position.y = -16 * entity->field_84 + entity->position.y + 2 * entity->field_78 * entity->field_84;
    entity->rotation   = entity->orientation << 7;

    if (!entity->field_78) {
        int off = 128;
        if (!dir)
            off = -128;
        entity->rotation += off;
    }
    else if (entity->field_78 < 8) {
        int off = 16;
        if (!dir)
            off = -16;
        entity->rotation += (7 - entity->field_78) * off;
    }
    RSDK.DrawSprite(&entity->animator, NULL, false);
    if (entity->field_78 < 8)
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    entity->position.x = storeX;
    entity->position.y = storeY;
    entity->direction  = dir;
    entity->rotation   = 0;
}

void PopOut_Create(void *data)
{
    RSDK_THIS(PopOut);
    entity->drawFX = FX_ROTATE | FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->field_64.x    = entity->position.x;
        entity->field_64.y    = entity->position.y;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PopOut->aniFrames, 0, &entity->animator, true, 0);
        EntitySpring *child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, Spring);
        if (child->objectID != Spring->objectID && child->objectID != Spikes->objectID)
            child = NULL;
        if (child->objectID == Spring->objectID) {
            if (child->type & 1)
                entity->childType = 1;
            else
                entity->childType = 0;
            RSDK.SetSpriteAnimation(PopOut->aniFrames, 1, &entity->animator2, true, entity->childType);
            PopOut_Unknown();
        }
        else {
            entity->childType = 2;
            RSDK.SetSpriteAnimation(PopOut->aniFrames, 2, &entity->animator2, true, 0);
            PopOut_Unknown();
        }

        switch (entity->orientation + 4 * entity->direction) {
            case 0:
            case 4:
                entity->field_80 = 0;
                entity->field_84 = -0x10000;
                break;
            case 1:
            case 5:
                entity->field_84 = 0;
                entity->field_80 = 0x10000;
                break;
            case 2:
            case 6:
                entity->field_80 = 0;
                entity->field_84 = 0x10000;
                break;
            default:
                entity->field_84 = 0;
                entity->field_80 = -0x10000;
                break;
        }
    }
}

void PopOut_StageLoad(void)
{
    PopOut->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/PopOut.bin", SCOPE_STAGE);
    if (RSDK.GetObjectIDByName("Button"))
        PopOut->hasButton = true;
}

void PopOut_Unknown(void)
{
    RSDK_THIS(PopOut);
    entity->hitbox.left    = 0;
    entity->hitbox.top     = 0;
    entity->hitbox.right   = 0;
    entity->hitbox.bottom  = 0;
    entity->hitbox2.left   = 0;
    entity->hitbox2.top    = 0;
    entity->hitbox2.right  = 0;
    entity->hitbox2.bottom = 0;
    switch (entity->orientation + 4 * entity->direction) {
        case 0:
            entity->hitbox.right  = 80;
            entity->hitbox.top    = -64;
            entity->hitbox.bottom = 32;
            entity->hitbox2.left  = -20;
            entity->hitbox2.top   = -31;
            break;
        case 1:
            entity->hitbox.right  = 80;
            entity->hitbox.bottom = 64;
            entity->hitbox.left   = -32;
            entity->hitbox2.top   = -32;
            entity->hitbox2.right = 32;
            break;
        case 2:
            entity->hitbox.left    = -80;
            entity->hitbox.top     = -31;
            entity->hitbox.bottom  = 64;
            entity->hitbox2.right  = 32;
            entity->hitbox2.bottom = 32;
            break;
        case 3:
            entity->hitbox.left    = -64;
            entity->hitbox.top     = -79;
            entity->hitbox.right   = 32;
            entity->hitbox2.left   = -32;
            entity->hitbox2.bottom = 32;
            break;
        case 4:
            entity->hitbox.left    = -80;
            entity->hitbox.top     = -63;
            entity->hitbox.bottom  = 32;
            entity->hitbox2.top    = -32;
            entity->hitbox2.bottom = 32;
            break;
        case 5:
            entity->hitbox.top     = -80;
            entity->hitbox.right   = 64;
            entity->hitbox.left    = -32;
            entity->hitbox2.right  = 32;
            entity->hitbox2.bottom = 32;
            break;
        case 6:
            entity->hitbox.right   = 80;
            entity->hitbox.bottom  = 64;
            entity->hitbox.top     = -32;
            entity->hitbox2.left   = -32;
            entity->hitbox2.bottom = 32;
            break;
        default:
            entity->hitbox.right  = 80;
            entity->hitbox.bottom = 32;
            entity->hitbox.left   = -64;
            entity->hitbox2.left  = -20;
            entity->hitbox2.top   = -31;
            break;
    }
}

void PopOut_EditorDraw(void) {}

void PopOut_EditorLoad(void) {}

void PopOut_Serialize(void)
{
    RSDK_EDITABLE_VAR(PopOut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PopOut, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(PopOut, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(PopOut, VAR_BOOL, manualTrigger);
    RSDK_EDITABLE_VAR(PopOut, VAR_ENUM, delay);
}
