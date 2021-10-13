#include "SonicMania.h"

ObjectTwistingDoor *TwistingDoor;

void TwistingDoor_Update(void)
{
    RSDK_THIS(TwistingDoor);
    --entity->animator.animationTimer;
    Platform_Update();

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (entity->state == TwistingDoor_Unknown1) {
        if (Player_CheckValidState(player1)) {
            if (entity->autoOpen) {
                switch (entity->type) {
                    case 0:
                    case 3: {
                        int32 x  = 0;
                        int32 x2 = 0;
                        if (entity->autoOpen) {
                            x2 = player1->position.x;
                            x  = entity->position.x;
                            if (player1->position.x >= entity->position.x)
                                return;
                        }
                        else {
                            x  = player1->position.x;
                            x2 = entity->position.x;
                            if (player1->position.x <= entity->position.x)
                                return;
                        }
                        if (x - x2 < 0x400000) {
                            if (abs(player1->position.y - entity->position.y) < 0x400000) {
                                entity->state         = TwistingDoor_Unknown2;
                                entity->active        = ACTIVE_NORMAL;
                                entity->collapseDelay = entity->type < 2 ? 64 : 128;
                            }
                        }
                        break;
                    }
                    case 1:
                    case 2: {
                        int32 y  = 0;
                        int32 y2 = 0;
                        if (entity->autoOpen) {
                            y2 = player1->position.y;
                            y  = entity->position.y;
                            if (y2 >= y)
                                return;
                        }
                        else {
                            y  = player1->position.y;
                            y2 = entity->position.y;
                            if (y <= y2)
                                return;
                        }

                        if (y - y2 < 0x400000) {
                            if (abs(player1->position.x - entity->position.x) < 0x400000) {
                                entity->state         = TwistingDoor_Unknown2;
                                entity->active        = ACTIVE_NORMAL;
                                entity->collapseDelay = entity->type < 2 ? 64 : 128;
                            }
                        }
                        break;
                    }
                    default: return;
                }
            }
        }
    }
}

void TwistingDoor_LateUpdate(void) {}

void TwistingDoor_StaticUpdate(void) {}

void TwistingDoor_Draw(void)
{
    RSDK_THIS(TwistingDoor);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void TwistingDoor_Create(void *data)
{
    RSDK_THIS(TwistingDoor);
    entity->collision = 1;
    Platform_Create(NULL);

    entity->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(TwistingDoor->aniFrames, entity->type, &entity->animator, true, 0);

    if (!entity->type || entity->type == 3) {
        if (!entity->direction)
            entity->groundVel = -0x10000;
        else
            entity->groundVel = 0x10000;
        entity->direction = 2 * entity->direction;
    }
    else {
        if (!entity->direction)
            entity->groundVel = 0x10000;
        else
            entity->groundVel = -0x10000;
    }

    EntityButton *button = RSDK_GET_ENTITY(RSDK.GetEntityID(entity) - 1, Button);
    if (entity->buttonTag > 0) {
        foreach_all(Button, buttonCheck)
        {
            if (buttonCheck->tag == entity->buttonTag) {
                button = buttonCheck;
                foreach_break;
            }
        }
    }

    entity->buttonPtr     = button;
    entity->updateRange.x = abs(entity->position.x - button->position.x) + 0x400000;
    entity->updateRange.y = abs(entity->position.y - button->position.y) + 0x400000;
    entity->state         = TwistingDoor_Unknown1;
}

void TwistingDoor_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        TwistingDoor->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TwistingDoor.bin", SCOPE_STAGE);
    TwistingDoor->sfxOpen = RSDK.GetSFX("Stage/Open.wav");
}

void TwistingDoor_Unknown1(void)
{
    RSDK_THIS(TwistingDoor);
    EntityButton *button = entity->buttonPtr;
    if (button->activated) {
        RSDK.PlaySfx(TwistingDoor->sfxOpen, 0, 255);
        entity->active        = ACTIVE_NORMAL;
        entity->state         = TwistingDoor_Unknown2;
        entity->collapseDelay = entity->type < 2 ? 64 : 128;
    }
    entity->velocity.y = 0;
    entity->velocity.x = 0;
}

void TwistingDoor_Unknown2(void)
{
    RSDK_THIS(TwistingDoor);
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->type == 1 || entity->type == 2) {
        entity->drawPos.x += entity->groundVel;
        entity->velocity.x = entity->groundVel;
    }
    else {
        entity->drawPos.y += entity->groundVel;
        entity->velocity.y = entity->groundVel;
    }

    if (--entity->collapseDelay <= 0) {
        if (entity->close)
            entity->collapseDelay = 180;
        else
            entity->active = ACTIVE_NORMAL;
        entity->state = TwistingDoor_Unknown3;
    }
}

void TwistingDoor_Unknown3(void)
{
    RSDK_THIS(TwistingDoor);

    entity->velocity.x = 0;
    entity->velocity.y = 0;
    if (entity->close && --entity->collapseDelay <= 0) {
        entity->state         = TwistingDoor_Unknown4;
        entity->collapseDelay = entity->type < 2 ? 64 : 128;
    }
}

void TwistingDoor_Unknown4(void)
{
    RSDK_THIS(TwistingDoor);
    if (--entity->animator.frameID < 0)
        entity->animator.frameID = 7;

    if (entity->type == 1 || entity->type == 2) {
        entity->drawPos.x -= entity->groundVel;
        entity->velocity.x = entity->groundVel;
    }
    else {
        entity->drawPos.y -= entity->groundVel;
        entity->velocity.y = entity->groundVel;
    }

    if (--entity->collapseDelay <= 0) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = TwistingDoor_Unknown1;
    }
}

void TwistingDoor_EditorDraw(void) {}

void TwistingDoor_EditorLoad(void) {}

void TwistingDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_UINT8, autoOpen);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_BOOL, close);
}
