#include "SonicMania.h"

ObjectPSZDoor *PSZDoor;

void PSZDoor_Update(void)
{
    RSDK_THIS(PSZDoor);

    int32 offsetX = 0;
    int32 offsetY = 0;

    int32 speed = 0;
    switch (self->open ^ self->orientation) {
        case 0:
            offsetY = -0x10000;
            speed   = -self->speed;
            break;
        case 1:
            speed   = self->speed;
            offsetY = 0x10000;
            break;
        case 2:
            speed   = self->speed;
            offsetX = -0x10000;
            break;
        case 3:
            offsetX = 0x10000;
            speed   = -self->speed;
            break;
        default: break;
    }

    int32 startX = self->startPos.x;
    int32 startY = self->startPos.y;

    Vector2 *pos = &self->startPos;
    if (self->field_70) {
        if (PSZDoor_Unknown1() > 0) {
            self->startPos.x += self->speed * (offsetX >> 3);
            self->startPos.y += self->speed * (offsetY >> 3);
            self->rotation += speed;
        }

        if (PSZDoor_Unknown1() < 0) {
            self->startPos.x = self->field_8C.x;
            self->startPos.y = self->field_8C.y;
        }
        if (self->startPos.x == self->field_8C.x && self->startPos.y == self->field_8C.y) {
            self->field_70 = 0;
            self->open     = self->open ^ 1;
        }
    }

    int32 posX = self->startPos.x - startX;
    int32 posY = self->startPos.y - startY;

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, pos, &PSZDoor->hitboxes[self->animator1.frameID]) == 1) {
            if (self->orientation >= 2)
                player->position.x += posX;
            else
                player->position.y += posY;
        }

        if (self->automatic) {
            if (self->orientation >= 2) {
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
            if (self->orientation >= 2) {
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
    if (trigger && ((trigger->objectID == DoorTrigger->objectID && trigger->animator2.frameID == 1) || flag)) {
        if (self->activeScreens)
            RSDK.PlaySfx(PSZDoor->sfxOpen, false, 255);
        self->field_84   = self->startPos;
        self->trigger    = 0;
        self->field_70   = true;
        self->field_8C.x = self->startPos.x + 112 * offsetX;
        self->field_8C.y = self->startPos.y + 112 * offsetY;
    }
}

void PSZDoor_LateUpdate(void) {}

void PSZDoor_StaticUpdate(void) {}

void PSZDoor_Draw(void)
{
    RSDK_THIS(PSZDoor);
    Vector2 drawPos;

    RSDK.DrawSprite(&self->animator1, &self->startPos, false);

    self->animator1.frameID = 2;
    drawPos.x                 = self->position.x + self->offset.x;
    drawPos.y                 = self->position.y + self->offset.y;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    if (self->orientation < 2)
        drawPos.x -= 2 * self->offset.x;
    else
        drawPos.y -= 2 * self->offset.y;

    int32 rotation     = self->rotation;
    self->rotation = 256 - rotation;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    self->rotation          = rotation;
    self->animator1.frameID = self->orientation >= 2;
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
        RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(PSZDoor->aniFrames, 1, &self->animator2, true, 0);

        self->startPos  = self->position;
        self->drawFX    = FX_ROTATE;
        self->direction = self->orientation & 1;
        if (self->orientation < 2)
            self->direction = 2 * (self->orientation & 1);
        else
            self->animator1.frameID = 1;

        switch (self->orientation) {
            case 0:
                self->offset.y = -0x380000;
                self->offset.x = -0x130000;
                if (self->open)
                    self->startPos.y -= 0x700000;
                break;
            case 1:
                self->offset.y = 0x380000;
                self->offset.x = -0x130000;
                if (self->open)
                    self->startPos.y += 0x700000;
                break;
            case 2:
                self->offset.x = -0x380000;
                self->offset.y = -0x130000;
                if (self->open)
                    self->startPos.x -= 0x700000;
                break;
            case 3:
                self->offset.x = 0x380000;
                self->offset.y = -0x130000;
                if (self->open)
                    self->startPos.x += 0x700000;
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
    PSZDoor->sfxOpen            = RSDK.GetSFX("Stage/Open.wav");
}

int32 PSZDoor_Unknown1(void)
{
    RSDK_THIS(PSZDoor);
    if (self->field_70) {
        switch (self->open ^ self->orientation) {
            case 0: return self->startPos.y - self->field_8C.y; break;
            case 1: return self->field_8C.y - self->startPos.y; break;
            case 2: return self->startPos.x - self->field_8C.x; break;
            case 3: return self->field_8C.x - self->startPos.x; break;
            default: break;
        }
    }
    return 0;
}

#if RETRO_INCLUDE_EDITOR
void PSZDoor_EditorDraw(void) {}

void PSZDoor_EditorLoad(void) {}
#endif

void PSZDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(PSZDoor, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_BOOL, open);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_UINT8, id);
    RSDK_EDITABLE_VAR(PSZDoor, VAR_ENUM, automatic);
}
