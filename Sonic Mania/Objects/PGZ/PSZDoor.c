#include "SonicMania.h"

ObjectPSZDoor *PSZDoor;

void PSZDoor_Update(void)
{
    RSDK_THIS(PSZDoor);

    int offsetX = 0;
    int offsetY = 0;

    int speed = 0;
    switch (entity->open ^ entity->orientation) {
        case 0:
            offsetY = -0x10000;
            speed   = -entity->speed;
            break;
        case 1:
            speed   = entity->speed;
            offsetY = 0x10000;
            break;
        case 2:
            speed   = entity->speed;
            offsetX = -0x10000;
            break;
        case 3:
            offsetX = 0x10000;
            speed   = -entity->speed;
            break;
        default: break;
    }

    int startX = entity->startPos.x;
    int startY = entity->startPos.y;

    Vector2 *pos = &entity->startPos;
    if (entity->field_70) {
        if (PSZDoor_Unknown1() > 0) {
            entity->startPos.x += entity->speed * (offsetX >> 3);
            entity->startPos.y += entity->speed * (offsetY >> 3);
            entity->rotation += speed;
        }

        if (PSZDoor_Unknown1() < 0) {
            entity->startPos.x = entity->field_8C.x;
            entity->startPos.y = entity->field_8C.y;
        }
        if (entity->startPos.x == entity->field_8C.x && entity->startPos.y == entity->field_8C.y) {
            entity->field_70 = 0;
            entity->open     = entity->open ^ 1;
        }
    }

    int posX = entity->startPos.x - startX;
    int posY = entity->startPos.y - startY;

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, pos, &PSZDoor->hitboxes[entity->animator1.frameID]) == 1) {
            if (entity->orientation >= 2)
                player->position.x += posX;
            else
                player->position.y += posY;
        }

        if (entity->automatic) {
            if (entity->orientation >= 2) {
                if (player->position.y < entity->position.y) {
                    if (abs(player->position.x - entity->position.x) < 0x200000)
                        flag = true;
                }
            }
            else {
                if (player->position.x < entity->position.x) {
                    if (abs(player->position.y - entity->position.y) < 0x200000)
                        flag = true;
                }
            }
        }
        else if (entity->automatic == 2) {
            if (entity->orientation >= 2) {
                if (player->position.y > entity->position.y) {
                    if (abs(player->position.x - entity->position.x) < 0x200000)
                        flag = true;
                }
            }
            else if (player->position.x > entity->position.x) {
                if (abs(player->position.y - entity->position.y) < 0x200000)
                    flag = true;
            }
        }
    }

    EntityDoorTrigger *trigger = entity->trigger;
    if (trigger && (trigger->objectID == DoorTrigger->objectID && trigger->animator2.frameID == 1 || flag)) {
        if (entity->activeScreens)
            RSDK.PlaySFX(PSZDoor->sfxOpen, false, 255);
        entity->field_84   = entity->startPos;
        entity->trigger    = 0;
        entity->field_70   = true;
        entity->field_8C.x = entity->startPos.x + 112 * offsetX;
        entity->field_8C.y = entity->startPos.y + 112 * offsetY;
    }
}

void PSZDoor_LateUpdate(void) {}

void PSZDoor_StaticUpdate(void) {}

void PSZDoor_Draw(void)
{
    RSDK_THIS(PSZDoor);
    Vector2 drawPos;

    RSDK.DrawSprite(&entity->animator1, &entity->startPos, false);

    entity->animator1.frameID = 2;
    drawPos.x                 = entity->position.x + entity->offset.x;
    drawPos.y                 = entity->position.y + entity->offset.y;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    if (entity->orientation < 2)
        drawPos.x -= 2 * entity->offset.x;
    else
        drawPos.y -= 2 * entity->offset.y;

    int rotation     = entity->rotation;
    entity->rotation = 256 - rotation;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->rotation          = rotation;
    entity->animator1.frameID = entity->orientation >= 2;
}

void PSZDoor_Create(void *data)
{
    RSDK_THIS(PSZDoor);
    if (!entity->speed)
        entity->speed = 8;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->updateRange.y = 0x800000;
        entity->updateRange.x = 0x800000;
        RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 1, &entity->animator2, true, 0);

        entity->startPos  = entity->position;
        entity->drawFX    = FX_ROTATE;
        entity->direction = entity->orientation & 1;
        if (entity->orientation < 2)
            entity->direction = 2 * (entity->orientation & 1);
        else
            entity->animator1.frameID = 1;

        switch (entity->orientation) {
            case 0:
                entity->offset.y = -0x380000;
                entity->offset.x = -0x130000;
                if (entity->open)
                    entity->startPos.y -= 0x700000;
                break;
            case 1:
                entity->offset.y = 0x380000;
                entity->offset.x = -0x130000;
                if (entity->open)
                    entity->startPos.y += 0x700000;
                break;
            case 2:
                entity->offset.x = -0x380000;
                entity->offset.y = -0x130000;
                if (entity->open)
                    entity->startPos.x -= 0x700000;
                break;
            case 3:
                entity->offset.x = 0x380000;
                entity->offset.y = -0x130000;
                if (entity->open)
                    entity->startPos.x += 0x700000;
                break;
            default: break;
        }

        foreach_all(DoorTrigger, trigger)
        {
            if (trigger->id == entity->id) {
                entity->updateRange.x += abs(entity->position.x - trigger->position.x);
                entity->updateRange.y += abs(entity->position.y - trigger->position.y);
                entity->trigger = trigger;
            }
        }
    }
}

void PSZDoor_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        PSZDoor->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PSZDoor.bin", SCOPE_STAGE);
    PSZDoor->hitboxes[0].top    = -56;
    PSZDoor->hitboxes[0].left   = -12;
    PSZDoor->hitboxes[0].bottom = 56;
    PSZDoor->hitboxes[0].right  = 12;
    PSZDoor->hitboxes[1].top    = -12;
    PSZDoor->hitboxes[1].left   = -56;
    PSZDoor->hitboxes[1].bottom = 12;
    PSZDoor->hitboxes[1].right  = 56;
    PSZDoor->sfxOpen            = RSDK.GetSFX("Stage/Open.wav");
}

int PSZDoor_Unknown1(void)
{
    RSDK_THIS(PSZDoor);
    if (entity->field_70) {
        switch (entity->open ^ entity->orientation) {
            case 0: return entity->startPos.y - entity->field_8C.y; break;
            case 1: return entity->field_8C.y - entity->startPos.y; break;
            case 2: return entity->startPos.x - entity->field_8C.x; break;
            case 3: return entity->field_8C.x - entity->startPos.x; break;
            default: break;
        }
    }
    return 0;
}

void PSZDoor_EditorDraw(void) {}

void PSZDoor_EditorLoad(void) {}

void PSZDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(PSZDoor, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_BOOL, open);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_UINT8, id);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_ENUM, automatic);
}
