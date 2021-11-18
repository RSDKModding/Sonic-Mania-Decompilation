#include "SonicMania.h"

ObjectTwistingDoor *TwistingDoor;

void TwistingDoor_Update(void)
{
    RSDK_THIS(TwistingDoor);
    --self->animator.animationTimer;
    Platform_Update();

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (self->state == TwistingDoor_Unknown1) {
        if (Player_CheckValidState(player1)) {
            if (self->autoOpen) {
                switch (self->type) {
                    case PLATFORM_FIXED:
                    case PLATFORM_CIRCULAR: {
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
                                self->state         = TwistingDoor_Unknown2;
                                self->active        = ACTIVE_NORMAL;
                                self->timer = self->type < PLATFORM_MOVING ? 64 : 128;
                            }
                        }
                        break;
                    }
                    case PLATFORM_COLLAPSING:
                    case PLATFORM_MOVING: {
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
                                self->state         = TwistingDoor_Unknown2;
                                self->active        = ACTIVE_NORMAL;
                                self->timer = self->type < PLATFORM_MOVING ? 64 : 128;
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
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void TwistingDoor_Create(void *data)
{
    RSDK_THIS(TwistingDoor);
    self->collision = PLATFORM_C_SOLID_ALL;
    Platform_Create(NULL);

    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(TwistingDoor->aniFrames, self->type, &self->animator, true, 0);

    if (self->type == PLATFORM_FIXED || self->type == PLATFORM_CIRCULAR) {
        if (!self->direction)
            self->groundVel = -0x10000;
        else
            self->groundVel = 0x10000;
        self->direction = 2 * self->direction;
    }
    else {
        if (!self->direction)
            self->groundVel = 0x10000;
        else
            self->groundVel = -0x10000;
    }

    EntityButton *button = RSDK_GET_ENTITY(RSDK.GetEntityID(self) - 1, Button);
    if (self->buttonTag > 0) {
        foreach_all(Button, buttonCheck)
        {
            if (buttonCheck->tag == self->buttonTag) {
                button = buttonCheck;
                foreach_break;
            }
        }
    }

    self->buttonPtr     = button;
    self->updateRange.x = abs(self->position.x - button->position.x) + 0x400000;
    self->updateRange.y = abs(self->position.y - button->position.y) + 0x400000;
    self->state         = TwistingDoor_Unknown1;
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
    EntityButton *button = self->buttonPtr;
    if (button->activated) {
        RSDK.PlaySfx(TwistingDoor->sfxOpen, false, 255);
        self->active        = ACTIVE_NORMAL;
        self->state         = TwistingDoor_Unknown2;
        self->timer = self->type < PLATFORM_MOVING ? 64 : 128;
    }
    self->velocity.y = 0;
    self->velocity.x = 0;
}

void TwistingDoor_Unknown2(void)
{
    RSDK_THIS(TwistingDoor);
    RSDK.ProcessAnimation(&self->animator);

    if (self->type == PLATFORM_COLLAPSING || self->type == PLATFORM_MOVING) {
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
        self->state = TwistingDoor_Unknown3;
    }
}

void TwistingDoor_Unknown3(void)
{
    RSDK_THIS(TwistingDoor);

    self->velocity.x = 0;
    self->velocity.y = 0;
    if (self->close && --self->timer <= 0) {
        self->state         = TwistingDoor_Unknown4;
        self->timer = self->type < 2 ? 64 : 128;
    }
}

void TwistingDoor_Unknown4(void)
{
    RSDK_THIS(TwistingDoor);
    if (--self->animator.frameID < 0)
        self->animator.frameID = 7;

    if (self->type == PLATFORM_COLLAPSING || self->type == PLATFORM_MOVING) {
        self->drawPos.x -= self->groundVel;
        self->velocity.x = self->groundVel;
    }
    else {
        self->drawPos.y -= self->groundVel;
        self->velocity.y = self->groundVel;
    }

    if (--self->timer <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->state  = TwistingDoor_Unknown1;
    }
}

#if RETRO_INCLUDE_EDITOR
void TwistingDoor_EditorDraw(void) {}

void TwistingDoor_EditorLoad(void) {}
#endif

void TwistingDoor_Serialize(void)
{
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_UINT8, autoOpen);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(TwistingDoor, VAR_BOOL, close);
}
