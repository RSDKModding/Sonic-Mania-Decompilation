// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TwistingDoor Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTwistingDoor *TwistingDoor;

void TwistingDoor_Update(void)
{
    RSDK_THIS(TwistingDoor);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    --self->animator.timer;
    Platform_Update();

    if (self->state == TwistingDoor_State_CheckOpen) {
        if (Player_CheckValidState(player1)) {
            if (self->autoOpen) {
                switch (self->type) {
                    case TWISTINGDOOR_V_SHORT:
                    case TWISTINGDOOR_V_LONG: {
                        int32 x  = 0;
                        int32 x2 = 0;
                        if (self->autoOpen) {
                            x2 = player1->position.x;
                            x  = self->position.x;
                            if (player1->position.x >= self->position.x)
                                return;
                        }
                        else {
                            x  = player1->position.x;
                            x2 = self->position.x;
                            if (player1->position.x <= self->position.x)
                                return;
                        }

                        if (x - x2 < 0x400000) {
                            if (abs(player1->position.y - self->position.y) < 0x400000) {
                                self->state  = TwistingDoor_State_Opening;
                                self->active = ACTIVE_NORMAL;
                                self->timer  = self->type < TWISTINGDOOR_H_LONG ? 64 : 128;
                            }
                        }
                        break;
                    }

                    case TWISTINGDOOR_H_SHORT:
                    case TWISTINGDOOR_H_LONG: {
                        int32 y  = 0;
                        int32 y2 = 0;
                        if (self->autoOpen) {
                            y2 = player1->position.y;
                            y  = self->position.y;
                            if (y2 >= y)
                                return;
                        }
                        else {
                            y  = player1->position.y;
                            y2 = self->position.y;
                            if (y <= y2)
                                return;
                        }

                        if (y - y2 < 0x400000) {
                            if (abs(player1->position.x - self->position.x) < 0x400000) {
                                self->state  = TwistingDoor_State_Opening;
                                self->active = ACTIVE_NORMAL;
                                self->timer  = self->type < TWISTINGDOOR_H_LONG ? 64 : 128;
                            }
                        }
                        break;
                    }

                    default: break;
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

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void TwistingDoor_Create(void *data)
{
    RSDK_THIS(TwistingDoor);

    self->collision = PLATFORM_C_SOLID;
    Platform_Create(NULL);

    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(TwistingDoor->aniFrames, self->type, &self->animator, true, 0);

    if (self->type == TWISTINGDOOR_V_SHORT || self->type == TWISTINGDOOR_V_LONG) {
        self->groundVel = self->direction ? 0x10000 : -0x10000;
        self->direction *= FLIP_Y;
    }
    else {
        self->groundVel = self->direction ? -0x10000 : 0x10000;
    }

    self->taggedButton         = NULL;
    EntityButton *taggedButton = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) - 1, Button);

    if (self->buttonTag > 0) {
        foreach_all(Button, button)
        {
            if (button->tag == self->buttonTag) {
                taggedButton = button;
                foreach_break;
            }
        }
    }

    if (taggedButton) {
        self->taggedButton  = taggedButton;
        self->updateRange.x = 0x400000 + abs(self->position.x - taggedButton->position.x);
        self->updateRange.y = 0x400000 + abs(self->position.y - taggedButton->position.y);
    }

    self->state = TwistingDoor_State_CheckOpen;
}

void TwistingDoor_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        TwistingDoor->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TwistingDoor.bin", SCOPE_STAGE);

    TwistingDoor->sfxOpen = RSDK.GetSfx("Stage/Open.wav");
}

void TwistingDoor_State_CheckOpen(void)
{
    RSDK_THIS(TwistingDoor);

    EntityButton *button = self->taggedButton;
    if (button->currentlyActive) {
        RSDK.PlaySfx(TwistingDoor->sfxOpen, false, 255);
        self->active = ACTIVE_NORMAL;
        self->state  = TwistingDoor_State_Opening;
        self->timer  = self->type < TWISTINGDOOR_H_LONG ? 64 : 128;
    }

    self->velocity.y = 0;
    self->velocity.x = 0;
}

void TwistingDoor_State_Opening(void)
{
    RSDK_THIS(TwistingDoor);

    RSDK.ProcessAnimation(&self->animator);

    if (self->type == TWISTINGDOOR_H_SHORT || self->type == TWISTINGDOOR_H_LONG) {
        self->drawPos.x += self->groundVel;
        self->velocity.x = self->groundVel;
    }
    else {
        self->drawPos.y += self->groundVel;
        self->velocity.y = self->groundVel;
    }

    if (--self->timer <= 0) {
        if (self->close)
            self->timer = 180;
        else
            self->active = ACTIVE_NORMAL;

        self->state = TwistingDoor_State_Opened;
    }
}

void TwistingDoor_State_Opened(void)
{
    RSDK_THIS(TwistingDoor);

    self->velocity.x = 0;
    self->velocity.y = 0;
    if (self->close && --self->timer <= 0) {
        self->state = TwistingDoor_State_Closing;
        self->timer = self->type < TWISTINGDOOR_H_LONG ? 64 : 128;
    }
}

void TwistingDoor_State_Closing(void)
{
    RSDK_THIS(TwistingDoor);

    if (--self->animator.frameID < 0)
        self->animator.frameID = 7;

    if (self->type == TWISTINGDOOR_H_SHORT || self->type == TWISTINGDOOR_H_LONG) {
        self->drawPos.x -= self->groundVel;
        self->velocity.x = self->groundVel;
    }
    else {
        self->drawPos.y -= self->groundVel;
        self->velocity.y = self->groundVel;
    }

    if (--self->timer <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = TwistingDoor_State_CheckOpen;
    }
}

#if GAME_INCLUDE_EDITOR
void TwistingDoor_EditorDraw(void)
{
    RSDK_THIS(TwistingDoor);

    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    if (self->direction > FLIP_X)
        self->direction >>= 1;

    self->drawPos = self->position;
    int32 dir     = self->direction;
    if (self->type == TWISTINGDOOR_V_SHORT || self->type == TWISTINGDOOR_V_LONG)
        self->direction *= FLIP_Y;

    TwistingDoor_Draw();

    self->direction = dir;

    if (showGizmos()) {
        self->taggedButton         = NULL;
        EntityButton *taggedButton = RSDK_GET_ENTITY(RSDK.GetEntitySlot(self) - 1, Button);

        if (self->buttonTag > 0) {
            foreach_all(Button, button)
            {
                if (button->tag == self->buttonTag) {
                    taggedButton = button;
                    foreach_break;
                }
            }
        }

        if (taggedButton) {
            self->taggedButton  = taggedButton;
            self->updateRange.x = 0x400000 + abs(self->position.x - taggedButton->position.x);
            self->updateRange.y = 0x400000 + abs(self->position.y - taggedButton->position.y);
        }

        RSDK_DRAWING_OVERLAY(true);
        if (self->taggedButton) {
            DrawHelpers_DrawArrow(self->taggedButton->position.x, self->taggedButton->position.y, self->position.x, self->position.y, 0xFFFF00,
                                  INK_NONE, 0xFF);
        }
        RSDK_DRAWING_OVERLAY(false);
    }
}

void TwistingDoor_EditorLoad(void)
{
    TwistingDoor->aniFrames = RSDK.LoadSpriteAnimation("FBZ/TwistingDoor.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TwistingDoor, type);
    RSDK_ENUM_VAR("Vertical", TWISTINGDOOR_V_SHORT);
    RSDK_ENUM_VAR("Horizontal", TWISTINGDOOR_H_SHORT);
    RSDK_ENUM_VAR("Horizontal (Long)", TWISTINGDOOR_H_LONG);
    RSDK_ENUM_VAR("Vertical (Long)", TWISTINGDOOR_V_LONG);

    RSDK_ACTIVE_VAR(TwistingDoor, direction);
    RSDK_ENUM_VAR("Right/Up", FLIP_NONE);
    RSDK_ENUM_VAR("Left/Down", FLIP_X);

    RSDK_ACTIVE_VAR(TwistingDoor, autoOpen);
    RSDK_ENUM_VAR("False", false);
    RSDK_ENUM_VAR("True", true);
}
#endif

void TwistingDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_UINT8, autoOpen);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_BOOL, close);
}
