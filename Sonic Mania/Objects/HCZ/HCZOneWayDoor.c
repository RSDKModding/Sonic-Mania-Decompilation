#include "SonicMania.h"

ObjectHCZOneWayDoor *HCZOneWayDoor;

void HCZOneWayDoor_Update(void)
{
    RSDK_THIS(HCZOneWayDoor);

    bool32 flag   = false;
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player) {
        self->position.x = storeX;
        self->position.y = storeY;
        Player_CheckCollisionBox(player, self, &self->hitbox1);
        self->position = self->posUnknown1;
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox2)) {
                flag = true;
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    if (!flag) {
        if (self->field_74 > 0) {
            switch (self->orientation) {
                case 0: self->position.y = self->posUnknown1.y - self->field_74 * (self->posUnknown2.y / self->duration); break;
                case 1: self->position.x = self->posUnknown1.x - self->field_74 * (self->posUnknown2.x / self->duration); break;
                case 2: self->position.y = self->posUnknown1.y + self->field_74 * (self->posUnknown2.y / self->duration); break;
                case 3: self->position.x = self->posUnknown1.x + self->field_74 * (self->posUnknown2.x / self->duration); break;
            }

            self->field_74--;
            if (self->field_74 <= 0) {
                if (self->activeScreens)
                    RSDK.PlaySfx(HCZOneWayDoor->sfxClack, false, 255);
            }
        }
        else {
            switch (self->orientation) {
                case 0: self->position.y = self->posUnknown1.y; break;
                case 1: self->position.x = self->posUnknown1.x; break;
                case 2: self->position.y = self->posUnknown1.y; break;
                case 3: self->position.x = self->posUnknown1.x; break;
            }
        }
    }
    else {
        if (self->field_74 >= self->duration) {
            switch (self->orientation) {
                case 0: self->position.y = self->posUnknown1.y - self->posUnknown2.y; break;
                case 1: self->position.x = self->posUnknown1.x - self->posUnknown2.x; break;
                case 2: self->position.y = self->posUnknown1.y + self->posUnknown2.y; break;
                case 3: self->position.x = self->posUnknown1.x + self->posUnknown2.x; break;
            }
        }
        else {
            switch (self->orientation) {
                case 0: self->position.y = self->posUnknown1.y - self->field_74 * (self->posUnknown2.y / self->duration); break;
                case 1: self->position.x = self->posUnknown1.x - self->field_74 * (self->posUnknown2.x / self->duration); break;
                case 2: self->position.y = self->posUnknown1.y + self->field_74 * (self->posUnknown2.y / self->duration); break;
                case 3: self->position.x = self->posUnknown1.x + self->field_74 * (self->posUnknown2.x / self->duration); break;
            }

            self->field_74++;
            if (self->field_74 >= self->duration) {
                if (self->activeScreens)
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

void HCZOneWayDoor_Draw(void) { HCZOneWayDoor_DrawSprites(); }

void HCZOneWayDoor_Create(void* data)
{
    RSDK_THIS(HCZOneWayDoor);
    if (!self->duration)
        self->duration = 6;
    if (!self->length)
        self->length = 2;

    self->active    = ACTIVE_BOUNDS;
    self->drawOrder = Zone->drawOrderLow;
    self->posUnknown1.x  = self->position.x;
    self->posUnknown1.y  = self->position.y;
    self->visible   = true;
    self->drawFX    = FX_FLIP;
    HCZOneWayDoor_Unknown1();
    self->updateRange.x = 0x800000 + self->posUnknown2.x;
    self->updateRange.y = 0x800000 + self->posUnknown2.y;
    RSDK.SetSpriteAnimation(HCZOneWayDoor->aniFrames, 0, &self->animator, true, 0);
}

void HCZOneWayDoor_StageLoad(void)
{
    HCZOneWayDoor->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE);
    HCZOneWayDoor->sfxClack = RSDK.GetSFX("Stage/Clack2.wav");
}

void HCZOneWayDoor_Unknown1(void)
{
    RSDK_THIS(HCZOneWayDoor);

    self->posUnknown2.x = 0x200000;
    self->posUnknown2.y = 0x200000;
    self->posUnknown3.x = self->posUnknown2.x;
    self->posUnknown3.y = self->posUnknown2.y;

    if (!self->orientation || self->orientation == 2) {
        self->posUnknown2.y = self->length << 21;
    }
    else if (self->orientation == 1 || self->orientation == 3) {
        self->posUnknown2.x = self->length << 21;
    }

    self->hitbox1.right  = self->posUnknown2.x >> 17;
    self->hitbox1.bottom = self->posUnknown2.y >> 17;
    self->hitbox1.left   = -(self->posUnknown2.x >> 17);
    self->hitbox1.top    = -(self->posUnknown2.y >> 17);
    switch (self->orientation) {
        case 0:
        case 2:
            self->hitbox2.top    = -(self->posUnknown2.y >> 17);
            self->hitbox2.left   = -(self->posUnknown2.x >> 17);
            self->hitbox2.right  = self->posUnknown2.x >> 17;
            self->hitbox2.bottom = self->posUnknown2.y >> 17;
            if (self->detectDirection == 1) {
                self->hitbox2.right = (self->posUnknown2.x >> 17) + (self->posUnknown3.y >> 14);
            }
            else if (!self->detectDirection) {
                self->hitbox2.left = -(self->posUnknown2.x >> 17) - (self->posUnknown3.y >> 14);
            }
            break;
        case 1:
        case 3:
            self->hitbox2.right  = self->posUnknown2.y >> 17;
            self->hitbox2.top    = -(self->posUnknown2.y >> 17);
            self->hitbox2.left   = -(self->posUnknown2.x >> 17);
            self->hitbox2.bottom = self->posUnknown2.y >> 17;
            if (self->detectDirection == 1) {
                self->hitbox2.top = -(self->posUnknown2.y >> 17) - (self->posUnknown3.x >> 14);
            }
            else if (!self->detectDirection) {
                self->hitbox2.bottom = (self->posUnknown2.y >> 17) + (self->posUnknown3.x >> 14);
            }
            break;
        default: break;
    }
}

void HCZOneWayDoor_DrawSprites(void)
{
    RSDK_THIS(HCZOneWayDoor);
    Vector2 drawPos;

    drawPos.x  = self->position.x;
    drawPos.y  = self->position.y;

    int32 offsetX = 0, offsetY = 0;
    if (!self->orientation || self->orientation == 2) {
        drawPos.y = (self->posUnknown3.y >> 1) + self->position.y - (self->posUnknown2.y >> 1);
        offsetY   = self->posUnknown3.y;
    }
    else if (self->orientation == 1 || self->orientation == 3) {
        drawPos.x = (self->posUnknown3.x >> 1) + self->position.x - (self->posUnknown2.x >> 1);
        offsetX   = self->posUnknown3.x;

    }
    
    for (int32 i = 0; i < self->length; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += offsetX;
        drawPos.y += offsetY;
    }
}

#if RETRO_INCLUDE_EDITOR
void HCZOneWayDoor_EditorDraw(void) { HCZOneWayDoor_DrawSprites(); }

void HCZOneWayDoor_EditorLoad(void) { HCZOneWayDoor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE); }
#endif

void HCZOneWayDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_ENUM, duration);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_UINT8, detectDirection);
}

