#include "SonicMania.h"

ObjectPopOut *PopOut;

void PopOut_Update(void)
{
    RSDK_THIS(PopOut);

    int32 storeDir        = self->direction;
    int32 storeX          = self->position.x;
    int32 storeY          = self->position.y;
    EntitySpring *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Spring);
    if (child->objectID != Spring->objectID && child->objectID != Spikes->objectID)
        child = NULL;

    self->direction = 0;
    if (!self->manualTrigger) {
        self->flag = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                self->flag = true;
                foreach_break;
            }
        }
    }
    else if (PopOut->hasButton) {
        foreach_active(Button, button)
        {
            if (button->tag == self->tag && button->field_70 == 1)
                self->flag = true;
        }
    }

    if (self->flag) {
        if (self->field_7C < self->delay) {
            self->field_7C++;
        }
        else {
            if (self->field_78 < 8) {
                self->field_78++;
                if (self->childType <= 0) {
                    if (child) {
                        if (self->field_78 == 8) {
                            child->timer               = 0;
                            child->animator.animationSpeed = 0;
                            child->animator.frameID        = 0;
                        }
                    }
                }
            }
        }
    }
    else {
        self->field_7C = 0;
        if (self->field_78 > 0) {
            self->field_78--;
            if (self->childType <= 0) {
                if (child) {
                    if (self->field_78 == 7) {
                        child->timer               = 0;
                        child->animator.animationSpeed = 0;
                        child->animator.frameID        = 0;
                    }
                }
            }
        }
    }

    if (child) {
        if (self->field_78 == 8) {
            child->active  = ACTIVE_BOUNDS;
            child->visible = true;
        }
        else {
            child->active  = ACTIVE_NEVER;
            child->visible = false;
        }
    }

    self->active     = (self->field_78 <= 0) ? ACTIVE_BOUNDS : ACTIVE_NORMAL;
    self->position.x = -32 * self->field_80 + self->position.x + 4 * self->field_78 * self->field_80;
    self->position.y = -32 * self->field_84 + self->position.y + 4 * self->field_78 * self->field_84;
    foreach_active(Player, playerLoop) { Player_CheckCollisionBox(playerLoop, self, &self->hitbox2); }
    self->direction  = storeDir;
    self->position.x = storeX;
    self->position.y = storeY;
}

void PopOut_LateUpdate(void) {}

void PopOut_StaticUpdate(void) {}

void PopOut_Draw(void)
{
    RSDK_THIS(PopOut);

    int32 dir            = self->direction;
    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = -16 * self->field_80 + self->position.x + 2 * self->field_78 * self->field_80;
    self->position.y = -16 * self->field_84 + self->position.y + 2 * self->field_78 * self->field_84;
    self->rotation   = self->orientation << 7;

    if (!self->field_78) {
        int32 off = 128;
        if (!dir)
            off = -128;
        self->rotation += off;
    }
    else if (self->field_78 < 8) {
        int32 off = 16;
        if (!dir)
            off = -16;
        self->rotation += (7 - self->field_78) * off;
    }
    RSDK.DrawSprite(&self->animator, NULL, false);
    if (self->field_78 < 8)
        RSDK.DrawSprite(&self->animator2, NULL, false);
    self->position.x = storeX;
    self->position.y = storeY;
    self->direction  = dir;
    self->rotation   = 0;
}

void PopOut_Create(void *data)
{
    RSDK_THIS(PopOut);
    self->drawFX = FX_ROTATE | FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->drawOrder     = Zone->drawOrderLow;
        self->field_64.x    = self->position.x;
        self->field_64.y    = self->position.y;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PopOut->aniFrames, 0, &self->animator, true, 0);
        EntitySpring *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Spring);
        if (child->objectID != Spring->objectID && child->objectID != Spikes->objectID)
            child = NULL;
        if (child->objectID == Spring->objectID) {
            if (child->type & 1)
                self->childType = 1;
            else
                self->childType = 0;
            RSDK.SetSpriteAnimation(PopOut->aniFrames, 1, &self->animator2, true, self->childType);
        }
        else {
            self->childType = 2;
            RSDK.SetSpriteAnimation(PopOut->aniFrames, 2, &self->animator2, true, 0);
        }
        PopOut_SetupHitboxes();

        switch (self->orientation + 4 * self->direction) {
            case 0:
            case 4:
                self->field_80 = 0;
                self->field_84 = -0x10000;
                break;
            case 1:
            case 5:
                self->field_84 = 0;
                self->field_80 = 0x10000;
                break;
            case 2:
            case 6:
                self->field_80 = 0;
                self->field_84 = 0x10000;
                break;
            default:
                self->field_84 = 0;
                self->field_80 = -0x10000;
                break;
        }
    }
}

void PopOut_StageLoad(void)
{
    PopOut->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/PopOut.bin", SCOPE_STAGE);
    if (RSDK.GetObjectIDByName("Button"))
        PopOut->hasButton = true;
}

void PopOut_SetupHitboxes(void)
{
    RSDK_THIS(PopOut);
    self->hitbox.left    = 0;
    self->hitbox.top     = 0;
    self->hitbox.right   = 0;
    self->hitbox.bottom  = 0;
    self->hitbox2.left   = 0;
    self->hitbox2.top    = 0;
    self->hitbox2.right  = 0;
    self->hitbox2.bottom = 0;
    switch (self->orientation + 4 * self->direction) {
        case 0:
            self->hitbox.right  = 80;
            self->hitbox.top    = -64;
            self->hitbox.bottom = 32;
            self->hitbox2.left  = -20;
            self->hitbox2.top   = -32;
            break;
        case 1:
            self->hitbox.right  = 80;
            self->hitbox.bottom = 64;
            self->hitbox.left   = -32;
            self->hitbox2.top   = -32;
            self->hitbox2.right = 32;
            break;
        case 2:
            self->hitbox.left    = -80;
            self->hitbox.top     = -32;
            self->hitbox.bottom  = 64;
            self->hitbox2.right  = 32;
            self->hitbox2.bottom = 32;
            break;
        case 3:
            self->hitbox.left    = -64;
            self->hitbox.top     = -80;
            self->hitbox.right   = 32;
            self->hitbox2.left   = -32;
            self->hitbox2.bottom = 32;
            break;
        case 4:
            self->hitbox.left    = -80;
            self->hitbox.top     = -63;
            self->hitbox.bottom  = 32;
            self->hitbox2.top    = -32;
            self->hitbox2.bottom = 32;
            break;
        case 5:
            self->hitbox.top     = -80;
            self->hitbox.right   = 64;
            self->hitbox.left    = -32;
            self->hitbox2.right  = 32;
            self->hitbox2.bottom = 32;
            break;
        case 6:
            self->hitbox.right   = 80;
            self->hitbox.bottom  = 64;
            self->hitbox.top     = -32;
            self->hitbox2.left   = -32;
            self->hitbox2.bottom = 32;
            break;
        default:
            self->hitbox.right  = 80;
            self->hitbox.bottom = 32;
            self->hitbox.left   = -64;
            self->hitbox2.left  = -20;
            self->hitbox2.top   = -32;
            break;
    }
}

#if RETRO_INCLUDE_EDITOR
void PopOut_EditorDraw(void)
{
    RSDK_THIS(PopOut);
    self->field_64.x    = self->position.x;
    self->field_64.y    = self->position.y;
    RSDK.SetSpriteAnimation(PopOut->aniFrames, 0, &self->animator, true, 0);

    EntitySpring *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Spring);
    if (child->objectID != Spring->objectID && child->objectID != Spikes->objectID)
        child = NULL;
    if (child && child->objectID == Spring->objectID) {
        if (child->type & 1)
            self->childType = 1;
        else
            self->childType = 0;
        RSDK.SetSpriteAnimation(PopOut->aniFrames, 1, &self->animator2, true, self->childType);
    }
    else {
        self->childType = 2;
        RSDK.SetSpriteAnimation(PopOut->aniFrames, 2, &self->animator2, true, 0);
    }
    
    PopOut_Draw();
}

void PopOut_EditorLoad(void) { PopOut->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/PopOut.bin", SCOPE_STAGE); }
#endif

void PopOut_Serialize(void)
{
    RSDK_EDITABLE_VAR(PopOut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PopOut, VAR_UINT8, orientation);
    RSDK_EDITABLE_VAR(PopOut, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(PopOut, VAR_BOOL, manualTrigger);
    RSDK_EDITABLE_VAR(PopOut, VAR_ENUM, delay);
}
