#include "SonicMania.h"

ObjectButtonDoor *ButtonDoor;

void ButtonDoor_Update(void)
{
    RSDK_THIS(ButtonDoor);
    EntityButton *button = entity->taggedButton;
    if (button && button->field_70) {
        if (!entity->movePos)
            RSDK.PlaySfx(ButtonDoor->sfxOpen, false, 255);

        if (entity->movePos < entity->duration) {
            switch (entity->orientation) {
                case 0: entity->position.y = entity->startPos.y - entity->movePos * (entity->size.y / entity->duration); break;
                case 1: entity->position.x = entity->startPos.x - entity->movePos * (entity->size.x / entity->duration); break;
                case 2: entity->position.y = entity->startPos.y + entity->movePos * (entity->size.y / entity->duration); break;
                case 3: entity->position.x = entity->startPos.x + entity->movePos * (entity->size.x / entity->duration); break;
            }
            entity->movePos++;
        }
        else {
            switch (entity->orientation) {
                case 0: entity->position.y = entity->startPos.y - entity->size.y; break;
                case 1: entity->position.x = entity->startPos.x - entity->size.x; break;
                case 2: entity->position.y = entity->startPos.y + entity->size.y; break;
                case 3: entity->position.x = entity->startPos.x + entity->size.x; break;
            }
        }
    }

    foreach_active(Player, player) { Player_CheckCollisionBox(player, entity, &entity->hitbox); }
}

void ButtonDoor_LateUpdate(void) {}

void ButtonDoor_StaticUpdate(void) {}

void ButtonDoor_Draw(void) { ButtonDoor_DrawSprites(); }

void ButtonDoor_Create(void *data)
{
    RSDK_THIS(ButtonDoor);
    if (!entity->duration)
        entity->duration = 6;
    if (!entity->length)
        entity->length = 2 - (RSDK.CheckStageFolder("HCZ") != 1);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = 1;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = entity->size.x + 0x800000;
    entity->updateRange.y = entity->size.y + 0x800000;
    ButtonDoor_SetupSize();
    ButtonDoor_SetupButtonTag();
    RSDK.SetSpriteAnimation(ButtonDoor->aniFrames, 0, &entity->animator, true, 0);
}

void ButtonDoor_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1")) {
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/ButtonDoor.bin", SCOPE_STAGE);
        ButtonDoor->sfxOpen   = RSDK.GetSFX("Stage/Open.wav");
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/ButtonDoor.bin", SCOPE_STAGE);
        ButtonDoor->sfxOpen   = RSDK.GetSFX("Stage/Open.wav");
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE);
        ButtonDoor->sfxOpen   = RSDK.GetSFX("Stage/Clack.wav");
    }
}

void ButtonDoor_SetupSize(void)
{
    RSDK_THIS(ButtonDoor);

    if (RSDK.CheckStageFolder("LRZ1")) {
        entity->size.y = 0x800000;
        entity->size.x = 0x200000;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        entity->size.y = 0x400000;
        entity->size.x = 0x200000;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        entity->size.y = 0x200000;
        entity->size.x = 0x200000;
    }

    entity->size2 = entity->size;

    if (!entity->orientation || entity->orientation == 2) {
        entity->size.y *= entity->length;
    }
    else if (entity->orientation == 1 || entity->orientation == 3) {
        entity->size.x *= entity->length;
    }

    entity->hitbox.left   = -(entity->size.x >> 17);
    entity->hitbox.top    = -(entity->size.y >> 17);
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.bottom = entity->size.y >> 17;
}

void ButtonDoor_SetupButtonTag(void)
{
    RSDK_THIS(ButtonDoor);

    entity->taggedButton = NULL;
    if (RSDK.CheckStageFolder("LRZ2")) {
        entity->taggedButton = LRZ2Setup_HandleTagSetup(entity->buttonTag, (Entity *)entity);
    }
    else {
        EntityButton *taggedEntity = RSDK.GetEntityByID(RSDK.GetEntityID(entity) - 1);
        bool32 tagged              = false;

        if (entity->buttonTag > 0) {
            if (Button) {
                foreach_all(Button, button)
                {
                    if (button->tag == entity->buttonTag) {
                        taggedEntity = button;
                        tagged       = true;
                        foreach_break;
                    }
                }
            }

            if (PullChain && !tagged) {
                foreach_all(PullChain, chain)
                {
                    if (chain->tag == entity->buttonTag) {
                        taggedEntity = (EntityButton *)chain;
                        tagged       = true;
                        foreach_break;
                    }
                }
            }

            if (TurretSwitch && !tagged) {
                foreach_all(TurretSwitch, turretSwitch)
                {
                    // if (turretSwitch->buttonTag == tag) {
                    //     taggedEntity = (EntityButton *)turretSwitch;
                    //     tagged = true;
                    //     foreach_break;
                    // }
                }
            }
        }

        if (taggedEntity) {
            if ((Button && taggedEntity->objectID == Button->objectID) || (TurretSwitch && taggedEntity->objectID == TurretSwitch->objectID)
                || (PullChain && taggedEntity->objectID == PullChain->objectID)) {
                if (entity) {
                    int32 distX = abs(entity->position.x - taggedEntity->position.x);
                    int32 distY = abs(entity->position.y - taggedEntity->position.y);

                    if (entity->updateRange.x < 0x800000 + distX)
                        entity->updateRange.x = 0x800000 + distX;

                    if (entity->updateRange.y < 0x800000 + distY)
                        entity->updateRange.y = 0x800000 + distY;
                }
                entity->taggedButton = taggedEntity;
            }
        }
    }
}

void ButtonDoor_DrawSprites(void)
{
    RSDK_THIS(ButtonDoor);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    int incX = 0, incY = 0;
    if (!entity->orientation || entity->orientation == 2) {
        drawPos.y = (entity->size2.y >> 1) + entity->position.y - (entity->size.y >> 1);
        incY      = entity->size2.y;
    }
    else if (entity->orientation == 1 || entity->orientation == 3) {
        drawPos.x = (entity->size2.x >> 1) + entity->position.x - (entity->size.x >> 1);
        incX      = entity->size2.x;
    }

    for (int i = 0; i < entity->length; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += incX;
        drawPos.y += incY;
    }
}

void ButtonDoor_EditorDraw(void)
{
    ButtonDoor_SetupSize();
    ButtonDoor_DrawSprites();
}

void ButtonDoor_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/ButtonDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2"))
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/ButtonDoor.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("HCZ"))
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE);
}

void ButtonDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(ButtonDoor, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(ButtonDoor, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(ButtonDoor, VAR_ENUM, duration);
    RSDK_EDITABLE_VAR(ButtonDoor, VAR_UINT8, orientation);
}
