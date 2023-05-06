// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HCZOneWayDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHCZOneWayDoor *HCZOneWayDoor;

void HCZOneWayDoor_Update(void)
{
    RSDK_THIS(HCZOneWayDoor);

    bool32 shouldShut = false;
    int32 storeX      = self->position.x;
    int32 storeY      = self->position.y;

    foreach_active(Player, player)
    {
        self->position.x = storeX;
        self->position.y = storeY;
        Player_CheckCollisionBox(player, self, &self->hitboxSolid);

        self->position = self->startPos;
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger))
                shouldShut = true;
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;

    if (shouldShut) {
        if (self->movePos >= self->duration) {
            switch (self->orientation) {
                case HCZONEWAYDOOR_UP: self->position.y = self->startPos.y - self->size.y; break;
                case HCZONEWAYDOOR_LEFT: self->position.x = self->startPos.x - self->size.x; break;
                case HCZONEWAYDOOR_DOWN: self->position.y = self->startPos.y + self->size.y; break;
                case HCZONEWAYDOOR_RIGHT: self->position.x = self->startPos.x + self->size.x; break;
            }
        }
        else {
            switch (self->orientation) {
                case HCZONEWAYDOOR_UP: self->position.y = self->startPos.y - self->movePos * (self->size.y / self->duration); break;
                case HCZONEWAYDOOR_LEFT: self->position.x = self->startPos.x - self->movePos * (self->size.x / self->duration); break;
                case HCZONEWAYDOOR_DOWN: self->position.y = self->startPos.y + self->movePos * (self->size.y / self->duration); break;
                case HCZONEWAYDOOR_RIGHT: self->position.x = self->startPos.x + self->movePos * (self->size.x / self->duration); break;
            }

            if (++self->movePos >= self->duration) {
                if (self->onScreen)
                    RSDK.PlaySfx(HCZOneWayDoor->sfxClack, false, 255);
            }
        }
    }
    else {
        if (self->movePos > 0) {
            switch (self->orientation) {
                case HCZONEWAYDOOR_UP: self->position.y = self->startPos.y - self->movePos * (self->size.y / self->duration); break;
                case HCZONEWAYDOOR_LEFT: self->position.x = self->startPos.x - self->movePos * (self->size.x / self->duration); break;
                case HCZONEWAYDOOR_DOWN: self->position.y = self->startPos.y + self->movePos * (self->size.y / self->duration); break;
                case HCZONEWAYDOOR_RIGHT: self->position.x = self->startPos.x + self->movePos * (self->size.x / self->duration); break;
            }

            if (--self->movePos <= 0) {
                if (self->onScreen)
                    RSDK.PlaySfx(HCZOneWayDoor->sfxClack, false, 255);
            }
        }
        else {
            switch (self->orientation) {
                case HCZONEWAYDOOR_UP: self->position.y = self->startPos.y; break;
                case HCZONEWAYDOOR_LEFT: self->position.x = self->startPos.x; break;
                case HCZONEWAYDOOR_DOWN: self->position.y = self->startPos.y; break;
                case HCZONEWAYDOOR_RIGHT: self->position.x = self->startPos.x; break;
            }
        }
    }
}

void HCZOneWayDoor_LateUpdate(void) {}

void HCZOneWayDoor_StaticUpdate(void) {}

void HCZOneWayDoor_Draw(void) { HCZOneWayDoor_DrawSprites(); }

void HCZOneWayDoor_Create(void *data)
{
    RSDK_THIS(HCZOneWayDoor);

    if (!self->duration)
        self->duration = 6;

    if (!self->length)
        self->length = 2;

    self->active    = ACTIVE_BOUNDS;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->startPos  = self->position;
    self->visible   = true;
    self->drawFX    = FX_FLIP;
    HCZOneWayDoor_SetupHitboxes();
    self->updateRange.x = 0x800000 + self->size.x;
    self->updateRange.y = 0x800000 + self->size.y;
    RSDK.SetSpriteAnimation(HCZOneWayDoor->aniFrames, 0, &self->animator, true, 0);
}

void HCZOneWayDoor_StageLoad(void)
{
    HCZOneWayDoor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE);

    HCZOneWayDoor->sfxClack = RSDK.GetSfx("Stage/Clack2.wav");
}

void HCZOneWayDoor_SetupHitboxes(void)
{
    RSDK_THIS(HCZOneWayDoor);

    self->size.x        = 0x200000;
    self->size.y        = 0x200000;
    self->initialSize.x = self->size.x;
    self->initialSize.y = self->size.y;

    if (self->orientation == HCZONEWAYDOOR_UP || self->orientation == HCZONEWAYDOOR_DOWN)
        self->size.y = self->length << 21;
    else if (self->orientation == HCZONEWAYDOOR_LEFT || self->orientation == HCZONEWAYDOOR_RIGHT)
        self->size.x = self->length << 21;

    self->hitboxSolid.right  = self->size.x >> 17;
    self->hitboxSolid.bottom = self->size.y >> 17;
    self->hitboxSolid.left   = -(self->size.x >> 17);
    self->hitboxSolid.top    = -(self->size.y >> 17);

    switch (self->orientation) {
        case HCZONEWAYDOOR_UP:
        case HCZONEWAYDOOR_DOWN:
            self->hitboxTrigger.top    = -(self->size.y >> 17);
            self->hitboxTrigger.left   = -(self->size.x >> 17);
            self->hitboxTrigger.right  = self->size.x >> 17;
            self->hitboxTrigger.bottom = self->size.y >> 17;

            if (self->detectDirection == FLIP_X)
                self->hitboxTrigger.right = (self->size.x >> 17) + (self->initialSize.y >> 14);
            else if (self->detectDirection == FLIP_NONE)
                self->hitboxTrigger.left = -(self->size.x >> 17) - (self->initialSize.y >> 14);
            break;

        case HCZONEWAYDOOR_LEFT:
        case HCZONEWAYDOOR_RIGHT:
            self->hitboxTrigger.right  = self->size.y >> 17;
            self->hitboxTrigger.top    = -(self->size.y >> 17);
            self->hitboxTrigger.left   = -(self->size.x >> 17);
            self->hitboxTrigger.bottom = self->size.y >> 17;

            if (self->detectDirection == FLIP_X)
                self->hitboxTrigger.top = -(self->size.y >> 17) - (self->initialSize.x >> 14);
            else if (self->detectDirection == FLIP_NONE)
                self->hitboxTrigger.bottom = (self->size.y >> 17) + (self->initialSize.x >> 14);
            break;

        default: break;
    }
}

void HCZOneWayDoor_DrawSprites(void)
{
    RSDK_THIS(HCZOneWayDoor);

    Vector2 drawPos = self->position;

    int32 offsetX = 0, offsetY = 0;
    if (self->orientation == HCZONEWAYDOOR_UP || self->orientation == HCZONEWAYDOOR_DOWN) {
        drawPos.y = (self->initialSize.y >> 1) + self->position.y - (self->size.y >> 1);
        offsetY   = self->initialSize.y;
    }
    else if (self->orientation == HCZONEWAYDOOR_LEFT || self->orientation == HCZONEWAYDOOR_RIGHT) {
        drawPos.x = (self->initialSize.x >> 1) + self->position.x - (self->size.x >> 1);
        offsetX   = self->initialSize.x;
    }

    for (int32 i = 0; i < self->length; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += offsetX;
        drawPos.y += offsetY;
    }
}

#if GAME_INCLUDE_EDITOR
void HCZOneWayDoor_EditorDraw(void) { HCZOneWayDoor_DrawSprites(); }

void HCZOneWayDoor_EditorLoad(void)
{
    HCZOneWayDoor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(HCZOneWayDoor, orientation);
    RSDK_ENUM_VAR("Up", HCZONEWAYDOOR_UP);
    RSDK_ENUM_VAR("Left", HCZONEWAYDOOR_LEFT);
    RSDK_ENUM_VAR("Down", HCZONEWAYDOOR_DOWN);
    RSDK_ENUM_VAR("Right", HCZONEWAYDOOR_RIGHT);

    RSDK_ACTIVE_VAR(HCZOneWayDoor, detectDirection);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE); // Left/Top
    RSDK_ENUM_VAR("Flipped", FLIP_X);    // Right/Bottom
}
#endif

void HCZOneWayDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_ENUM, duration);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(HCZOneWayDoor, VAR_UINT8, detectDirection);
}
