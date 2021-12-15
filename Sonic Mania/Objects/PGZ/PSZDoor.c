// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPSZDoor *PSZDoor;

void PSZDoor_Update(void)
{
    RSDK_THIS(PSZDoor);

    int32 offsetX = 0;
    int32 offsetY = 0;

    int32 speed = 0;
    switch (self->open ^ self->orientation) {
        case PSZDOOR_ORIENATION_D:
            offsetY = -0x10000;
            speed   = -self->speed;
            break;
        case PSZDOOR_ORIENATION_U:
            speed   = self->speed;
            offsetY = 0x10000;
            break;
        case PSZDOOR_ORIENATION_L:
            speed   = self->speed;
            offsetX = -0x10000;
            break;
        case PSZDOOR_ORIENATION_R:
            offsetX = 0x10000;
            speed   = -self->speed;
            break;
        default: break;
    }

    int32 startX = self->doorPos.x;
    int32 startY = self->doorPos.y;

    if (self->shouldMove) {
        if (PSZDoor_GetRemainingDistance() > 0) {
            self->doorPos.x += self->speed * (offsetX >> 3);
            self->doorPos.y += self->speed * (offsetY >> 3);
            self->rotation += speed;
        }

        if (PSZDoor_GetRemainingDistance() < 0) {
            self->doorPos.x = self->targetPos.x;
            self->doorPos.y = self->targetPos.y;
        }
        if (self->doorPos.x == self->targetPos.x && self->doorPos.y == self->targetPos.y) {
            self->shouldMove = false;
            self->open ^= 1;
        }
    }

    int32 posX = self->doorPos.x - startX;
    int32 posY = self->doorPos.y - startY;

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, &self->doorPos, &PSZDoor->hitboxes[self->doorAnimator.frameID]) == C_TOP) {
            if (self->orientation >= PSZDOOR_ORIENATION_L)
                player->position.x += posX;
            else
                player->position.y += posY;
        }

        if (self->automatic) {
            if (self->orientation >= PSZDOOR_ORIENATION_L) {
                if (player->position.y < self->position.y) {
                    if (abs(player->position.x - self->position.x) < 0x200000)
                        flag = true;
                }
            }
            else {
                if (player->position.x < self->position.x) {
                    if (abs(player->position.y - self->position.y) < 0x200000)
                        flag = true;
                }
            }
        }
        else if (self->automatic == 2) {
            if (self->orientation >= PSZDOOR_ORIENATION_L) {
                if (player->position.y > self->position.y) {
                    if (abs(player->position.x - self->position.x) < 0x200000)
                        flag = true;
                }
            }
            else if (player->position.x > self->position.x) {
                if (abs(player->position.y - self->position.y) < 0x200000)
                    flag = true;
            }
        }
    }

    EntityDoorTrigger *trigger = self->trigger;
    if (trigger && ((trigger->objectID == DoorTrigger->objectID && trigger->bulbAnimator.frameID == 1) || flag)) {
        if (self->activeScreens)
            RSDK.PlaySfx(PSZDoor->sfxOpen, false, 255);
        self->startPos   = self->doorPos;
        self->trigger    = 0;
        self->shouldMove   = true;
        self->targetPos.x = self->doorPos.x + 112 * offsetX;
        self->targetPos.y = self->doorPos.y + 112 * offsetY;
    }
}

void PSZDoor_LateUpdate(void) {}

void PSZDoor_StaticUpdate(void) {}

void PSZDoor_Draw(void)
{
    RSDK_THIS(PSZDoor);
    Vector2 drawPos;

    RSDK.DrawSprite(&self->doorAnimator, &self->doorPos, false);

    self->doorAnimator.frameID = 2;
    drawPos.x                 = self->position.x + self->offset.x;
    drawPos.y                 = self->position.y + self->offset.y;
    RSDK.DrawSprite(&self->cogAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->doorAnimator, &drawPos, false);

    if (self->orientation < PSZDOOR_ORIENATION_L)
        drawPos.x -= 2 * self->offset.x;
    else
        drawPos.y -= 2 * self->offset.y;

    int32 rotation     = self->rotation;
    self->rotation = 256 - rotation;
    RSDK.DrawSprite(&self->cogAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->doorAnimator, &drawPos, false);

    self->rotation          = rotation;
    self->doorAnimator.frameID = self->orientation >= PSZDOOR_ORIENATION_L;
}

void PSZDoor_Create(void *data)
{
    RSDK_THIS(PSZDoor);
    if (!self->speed)
        self->speed = 8;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.y = 0x800000;
        self->updateRange.x = 0x800000;
        RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 0, &self->doorAnimator, true, 0);
        RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 1, &self->cogAnimator, true, 0);

        self->doorPos  = self->position;
        self->drawFX    = FX_ROTATE;
        self->direction = self->orientation & 1;
        if (self->orientation < PSZDOOR_ORIENATION_L)
            self->direction = 2 * (self->orientation & 1);
        else
            self->doorAnimator.frameID = 1;

        switch (self->orientation) {
            case 0:
                self->offset.y = -0x380000;
                self->offset.x = -0x130000;
                if (self->open)
                    self->doorPos.y -= 0x700000;
                break;
            case 1:
                self->offset.y = 0x380000;
                self->offset.x = -0x130000;
                if (self->open)
                    self->doorPos.y += 0x700000;
                break;
            case 2:
                self->offset.x = -0x380000;
                self->offset.y = -0x130000;
                if (self->open)
                    self->doorPos.x -= 0x700000;
                break;
            case 3:
                self->offset.x = 0x380000;
                self->offset.y = -0x130000;
                if (self->open)
                    self->doorPos.x += 0x700000;
                break;
            default: break;
        }

        foreach_all(DoorTrigger, trigger)
        {
            if (trigger->id == self->id) {
                self->updateRange.x += abs(self->position.x - trigger->position.x);
                self->updateRange.y += abs(self->position.y - trigger->position.y);
                self->trigger = trigger;
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
    PSZDoor->sfxOpen            = RSDK.GetSfx("Stage/Open.wav");
}

int32 PSZDoor_GetRemainingDistance(void)
{
    RSDK_THIS(PSZDoor);
    if (self->shouldMove) {
        switch (self->open ^ self->orientation) {
            case PSZDOOR_ORIENATION_D: return self->doorPos.y - self->targetPos.y; break;
            case PSZDOOR_ORIENATION_U: return self->targetPos.y - self->doorPos.y; break;
            case PSZDOOR_ORIENATION_L: return self->doorPos.x - self->targetPos.x; break;
            case PSZDOOR_ORIENATION_R: return self->targetPos.x - self->doorPos.x; break;
            default: break;
        }
    }
    return 0;
}

#if RETRO_INCLUDE_EDITOR
void PSZDoor_EditorDraw(void)
{
    RSDK_THIS(PSZDoor);
    self->updateRange.y = 0x800000;
    self->updateRange.x = 0x800000;
    RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 0, &self->doorAnimator, true, 0);
    RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 1, &self->cogAnimator, true, 0);

    self->doorPos  = self->position;
    self->drawFX    = FX_ROTATE;
    self->direction = self->orientation & 1;
    if (self->orientation < PSZDOOR_ORIENATION_L)
        self->direction = 2 * (self->orientation & 1);
    else
        self->doorAnimator.frameID = 1;

    switch (self->orientation) {
        case PSZDOOR_ORIENATION_D:
            self->offset.y = -0x380000;
            self->offset.x = -0x130000;
            if (self->open)
                self->doorPos.y -= 0x700000;
            break;
        case PSZDOOR_ORIENATION_U:
            self->offset.y = 0x380000;
            self->offset.x = -0x130000;
            if (self->open)
                self->doorPos.y += 0x700000;
            break;
        case PSZDOOR_ORIENATION_L:
            self->offset.x = -0x380000;
            self->offset.y = -0x130000;
            if (self->open)
                self->doorPos.x -= 0x700000;
            break;
        case PSZDOOR_ORIENATION_R:
            self->offset.x = 0x380000;
            self->offset.y = -0x130000;
            if (self->open)
                self->doorPos.x += 0x700000;
            break;
        default: break;
    }

    PSZDoor_Draw();

    if (showGizmos()) {
        self->doorPos   = self->position;
        self->inkEffect = INK_BLEND;
        self->open ^= 1;

        switch (self->orientation) {
            case PSZDOOR_ORIENATION_D:
                self->offset.y = -0x380000;
                self->offset.x = -0x130000;
                if (self->open)
                    self->doorPos.y -= 0x700000;
                break;
            case PSZDOOR_ORIENATION_U:
                self->offset.y = 0x380000;
                self->offset.x = -0x130000;
                if (self->open)
                    self->doorPos.y += 0x700000;
                break;
            case PSZDOOR_ORIENATION_L:
                self->offset.x = -0x380000;
                self->offset.y = -0x130000;
                if (self->open)
                    self->doorPos.x -= 0x700000;
                break;
            case PSZDOOR_ORIENATION_R:
                self->offset.x = 0x380000;
                self->offset.y = -0x130000;
                if (self->open)
                    self->doorPos.x += 0x700000;
                break;
            default: break;
        }

        PSZDoor_Draw();

        self->open ^= 1;
        self->inkEffect = INK_NONE;
    }
}

void PSZDoor_EditorLoad(void)
{
    PSZDoor->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PSZDoor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PSZDoor, orientation);
    RSDK_ENUM_VAR("Down", PSZDOOR_ORIENATION_D);
    RSDK_ENUM_VAR("Up", PSZDOOR_ORIENATION_U);
    RSDK_ENUM_VAR("Left", PSZDOOR_ORIENATION_L);
    RSDK_ENUM_VAR("Right", PSZDOOR_ORIENATION_R);
}
#endif

void PSZDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(PSZDoor, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_BOOL, open);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_UINT8, id);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_ENUM, automatic);
}
