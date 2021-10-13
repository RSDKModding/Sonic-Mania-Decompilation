#include "SonicMania.h"

ObjectHCZOneWayDoor *HCZOneWayDoor;

void HCZOneWayDoor_Update(void)
{
    RSDK_THIS(HCZOneWayDoor);

    bool32 flag   = false;
    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;

    foreach_active(Player, player) {
        entity->position.x = storeX;
        entity->position.y = storeY;
        Player_CheckCollisionBox(player, entity, &entity->hitbox1);
        entity->position = entity->posUnknown1;
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
                flag = true;
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;

    if (!flag) {
        if (entity->field_74 > 0) {
            switch (entity->orientation) {
                case 0: entity->position.y = entity->posUnknown1.y - entity->field_74 * (entity->posUnknown2.y / entity->duration); break;
                case 1: entity->position.x = entity->posUnknown1.x - entity->field_74 * (entity->posUnknown2.x / entity->duration); break;
                case 2: entity->position.y = entity->posUnknown1.y + entity->field_74 * (entity->posUnknown2.y / entity->duration); break;
                case 3: entity->position.x = entity->posUnknown1.x + entity->field_74 * (entity->posUnknown2.x / entity->duration); break;
            }

            entity->field_74--;
            if (entity->field_74 <= 0) {
                if (entity->activeScreens)
                    RSDK.PlaySfx(HCZOneWayDoor->sfxClack, false, 255);
            }
        }
        else {
            switch (entity->orientation) {
                case 0: entity->position.y = entity->posUnknown1.y; break;
                case 1: entity->position.x = entity->posUnknown1.x; break;
                case 2: entity->position.y = entity->posUnknown1.y; break;
                case 3: entity->position.x = entity->posUnknown1.x; break;
            }
        }
    }
    else {
        if (entity->field_74 >= entity->duration) {
            switch (entity->orientation) {
                case 0: entity->position.y = entity->posUnknown1.y - entity->posUnknown2.y; break;
                case 1: entity->position.x = entity->posUnknown1.x - entity->posUnknown2.x; break;
                case 2: entity->position.y = entity->posUnknown1.y + entity->posUnknown2.y; break;
                case 3: entity->position.x = entity->posUnknown1.x + entity->posUnknown2.x; break;
            }
        }
        else {
            switch (entity->orientation) {
                case 0: entity->position.y = entity->posUnknown1.y - entity->field_74 * (entity->posUnknown2.y / entity->duration); break;
                case 1: entity->position.x = entity->posUnknown1.x - entity->field_74 * (entity->posUnknown2.x / entity->duration); break;
                case 2: entity->position.y = entity->posUnknown1.y + entity->field_74 * (entity->posUnknown2.y / entity->duration); break;
                case 3: entity->position.x = entity->posUnknown1.x + entity->field_74 * (entity->posUnknown2.x / entity->duration); break;
            }

            entity->field_74++;
            if (entity->field_74 >= entity->duration) {
                if (entity->activeScreens)
                    RSDK.PlaySfx(HCZOneWayDoor->sfxClack, false, 255);
            }
        }
    }
}

void HCZOneWayDoor_LateUpdate(void)
{

}

void HCZOneWayDoor_StaticUpdate(void)
{

}

void HCZOneWayDoor_Draw(void)
{ HCZOneWayDoor_DrawSprites(); }

void HCZOneWayDoor_Create(void* data)
{
    RSDK_THIS(HCZOneWayDoor);
    if (!entity->duration)
        entity->duration = 6;
    if (!entity->length)
        entity->length = 2;

    entity->active    = ACTIVE_BOUNDS;
    entity->drawOrder = Zone->drawOrderLow;
    entity->posUnknown1.x  = entity->position.x;
    entity->posUnknown1.y  = entity->position.y;
    entity->visible   = true;
    entity->drawFX    = FX_FLIP;
    HCZOneWayDoor_Unknown1();
    entity->updateRange.x = 0x800000 + entity->posUnknown2.x;
    entity->updateRange.y = 0x800000 + entity->posUnknown2.y;
    RSDK.SetSpriteAnimation(HCZOneWayDoor->aniFrames, 0, &entity->animator, true, 0);
}

void HCZOneWayDoor_StageLoad(void)
{
    HCZOneWayDoor->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE);
    HCZOneWayDoor->sfxClack = RSDK.GetSFX("Stage/Clack2.wav");
}

void HCZOneWayDoor_Unknown1(void)
{
    RSDK_THIS(HCZOneWayDoor);

    entity->posUnknown2.x = 0x200000;
    entity->posUnknown2.y = 0x200000;
    entity->posUnknown3.x = entity->posUnknown2.x;
    entity->posUnknown3.y = entity->posUnknown2.y;

    if (!entity->orientation || entity->orientation == 2) {
        entity->posUnknown2.y = entity->length << 21;
    }
    else if (entity->orientation == 1 || entity->orientation == 3) {
        entity->posUnknown2.x = entity->length << 21;
    }

    entity->hitbox1.right  = entity->posUnknown2.x >> 17;
    entity->hitbox1.bottom = entity->posUnknown2.y >> 17;
    entity->hitbox1.left   = -(entity->posUnknown2.x >> 17);
    entity->hitbox1.top    = -(entity->posUnknown2.y >> 17);
    switch (entity->orientation) {
        case 0:
        case 2:
            entity->hitbox2.top    = -(entity->posUnknown2.y >> 17);
            entity->hitbox2.left   = -(entity->posUnknown2.x >> 17);
            entity->hitbox2.right  = entity->posUnknown2.x >> 17;
            entity->hitbox2.bottom = entity->posUnknown2.y >> 17;
            if (entity->detectDirection == 1) {
                entity->hitbox2.right = (entity->posUnknown2.x >> 17) + (entity->posUnknown3.y >> 14);
            }
            else if (!entity->detectDirection) {
                entity->hitbox2.left = -(entity->posUnknown2.x >> 17) - (entity->posUnknown3.y >> 14);
            }
            break;
        case 1:
        case 3:
            entity->hitbox2.right  = entity->posUnknown2.y >> 17;
            entity->hitbox2.top    = -(entity->posUnknown2.y >> 17);
            entity->hitbox2.left   = -(entity->posUnknown2.x >> 17);
            entity->hitbox2.bottom = entity->posUnknown2.y >> 17;
            if (entity->detectDirection == 1) {
                entity->hitbox2.top = -(entity->posUnknown2.y >> 17) - (entity->posUnknown3.x >> 14);
            }
            else if (!entity->detectDirection) {
                entity->hitbox2.bottom = (entity->posUnknown2.y >> 17) + (entity->posUnknown3.x >> 14);
            }
            break;
        default: break;
    }
}

void HCZOneWayDoor_DrawSprites(void)
{
    RSDK_THIS(HCZOneWayDoor);
    Vector2 drawPos;

    drawPos.x  = entity->position.x;
    drawPos.y  = entity->position.y;

    int32 offsetX = 0, offsetY = 0;
    if (!entity->orientation || entity->orientation == 2) {
        drawPos.y = (entity->posUnknown3.y >> 1) + entity->position.y - (entity->posUnknown2.y >> 1);
        offsetY   = entity->posUnknown3.y;
    }
    else if (entity->orientation == 1 || entity->orientation == 3) {
        drawPos.x = (entity->posUnknown3.x >> 1) + entity->position.x - (entity->posUnknown2.x >> 1);
        offsetX   = entity->posUnknown3.x;

    }
    
    for (int32 i = 0; i < entity->length; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += offsetX;
        drawPos.y += offsetY;
    }
}

void HCZOneWayDoor_EditorDraw(void)
{

}

void HCZOneWayDoor_EditorLoad(void)
{

}

void HCZOneWayDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_ENUM, duration);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_UINT8, detectDirection);
}

