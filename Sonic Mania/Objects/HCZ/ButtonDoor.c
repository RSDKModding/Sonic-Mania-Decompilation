// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ButtonDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectButtonDoor *ButtonDoor;

void ButtonDoor_Update(void)
{
    RSDK_THIS(ButtonDoor);
    EntityButton *button = self->taggedButton;
    if (button && button->activated) {
        if (!self->movePos)
            RSDK.PlaySfx(ButtonDoor->sfxOpen, false, 255);

        if (self->movePos < self->duration) {
            switch (self->orientation) {
                case 0: self->position.y = self->startPos.y - self->movePos * (self->size.y / self->duration); break;
                case 1: self->position.x = self->startPos.x - self->movePos * (self->size.x / self->duration); break;
                case 2: self->position.y = self->startPos.y + self->movePos * (self->size.y / self->duration); break;
                case 3: self->position.x = self->startPos.x + self->movePos * (self->size.x / self->duration); break;
            }
            self->movePos++;
        }
        else {
            switch (self->orientation) {
                case 0: self->position.y = self->startPos.y - self->size.y; break;
                case 1: self->position.x = self->startPos.x - self->size.x; break;
                case 2: self->position.y = self->startPos.y + self->size.y; break;
                case 3: self->position.x = self->startPos.x + self->size.x; break;
            }
        }
    }

    foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &self->hitbox); }
}

void ButtonDoor_LateUpdate(void) {}

void ButtonDoor_StaticUpdate(void) {}

void ButtonDoor_Draw(void) { ButtonDoor_DrawSprites(); }

void ButtonDoor_Create(void *data)
{
    RSDK_THIS(ButtonDoor);
    if (!self->duration)
        self->duration = 6;
    if (!self->length)
        self->length = 2 - (RSDK.CheckStageFolder("HCZ") != 1);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = 1;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = self->size.x + 0x800000;
    self->updateRange.y = self->size.y + 0x800000;
    ButtonDoor_SetupSize();
    ButtonDoor_SetupButtonTag();
    RSDK.SetSpriteAnimation(ButtonDoor->aniFrames, 0, &self->animator, true, 0);
}

void ButtonDoor_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1")) {
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/ButtonDoor.bin", SCOPE_STAGE);
        ButtonDoor->sfxOpen   = RSDK.GetSfx("Stage/Open.wav");
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/ButtonDoor.bin", SCOPE_STAGE);
        ButtonDoor->sfxOpen   = RSDK.GetSfx("Stage/Open.wav");
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        ButtonDoor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ButtonDoor.bin", SCOPE_STAGE);
        ButtonDoor->sfxOpen   = RSDK.GetSfx("Stage/Clack.wav");
    }
}

void ButtonDoor_SetupSize(void)
{
    RSDK_THIS(ButtonDoor);

    if (RSDK.CheckStageFolder("LRZ1")) {
        self->size.y = 0x800000;
        self->size.x = 0x200000;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        self->size.y = 0x400000;
        self->size.x = 0x200000;
    }
    else if (RSDK.CheckStageFolder("HCZ")) {
        self->size.y = 0x200000;
        self->size.x = 0x200000;
    }

    self->size2 = self->size;

    if (!self->orientation || self->orientation == 2) {
        self->size.y *= self->length;
    }
    else if (self->orientation == 1 || self->orientation == 3) {
        self->size.x *= self->length;
    }

    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.top    = -(self->size.y >> 17);
    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.bottom = self->size.y >> 17;
}

void ButtonDoor_SetupButtonTag(void)
{
    RSDK_THIS(ButtonDoor);

    self->taggedButton = NULL;
    if (RSDK.CheckStageFolder("LRZ2")) {
        self->taggedButton = LRZ2Setup_HandleTagSetup(self->buttonTag, (Entity *)self);
    }
    else {
        EntityButton *taggedEntity = RSDK.GetEntityByID(RSDK.GetEntityID(self) - 1);
        bool32 tagged              = false;

        if (self->buttonTag > 0) {
            if (Button) {
                foreach_all(Button, button)
                {
                    if (button->tag == self->buttonTag) {
                        taggedEntity = button;
                        tagged       = true;
                        foreach_break;
                    }
                }
            }

            if (PullChain && !tagged) {
                foreach_all(PullChain, chain)
                {
                    if (chain->tag == self->buttonTag) {
                        taggedEntity = (EntityButton *)chain;
                        tagged       = true;
                        foreach_break;
                    }
                }
            }

            if (TurretSwitch && !tagged) {
                foreach_all(TurretSwitch, turretSwitch)
                {
                    if (turretSwitch->tag == self->buttonTag) {
                        taggedEntity = (EntityButton *)turretSwitch;
                        tagged = true;
                        foreach_break;
                    }
                }
            }
        }

        if (taggedEntity) {
            if ((Button && taggedEntity->objectID == Button->objectID) || (TurretSwitch && taggedEntity->objectID == TurretSwitch->objectID)
                || (PullChain && taggedEntity->objectID == PullChain->objectID)) {
                if (self) {
                    int32 distX = abs(self->position.x - taggedEntity->position.x);
                    int32 distY = abs(self->position.y - taggedEntity->position.y);

                    if (self->updateRange.x < 0x800000 + distX)
                        self->updateRange.x = 0x800000 + distX;

                    if (self->updateRange.y < 0x800000 + distY)
                        self->updateRange.y = 0x800000 + distY;
                }
                self->taggedButton = taggedEntity;
            }
        }
    }
}

void ButtonDoor_DrawSprites(void)
{
    RSDK_THIS(ButtonDoor);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    int incX = 0, incY = 0;
    if (!self->orientation || self->orientation == 2) {
        drawPos.y = (self->size2.y >> 1) + self->position.y - (self->size.y >> 1);
        incY      = self->size2.y;
    }
    else if (self->orientation == 1 || self->orientation == 3) {
        drawPos.x = (self->size2.x >> 1) + self->position.x - (self->size.x >> 1);
        incX      = self->size2.x;
    }

    for (int i = 0; i < self->length; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
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
