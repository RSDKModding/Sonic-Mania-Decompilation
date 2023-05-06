// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PopOut Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPopOut *PopOut;

void PopOut_Update(void)
{
    RSDK_THIS(PopOut);

    int32 storeDir = self->direction;
    int32 storeX   = self->position.x;
    int32 storeY   = self->position.y;

    EntitySpring *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Spring);
    if (child->classID != Spring->classID && child->classID != Spikes->classID)
        child = NULL;

    self->direction = FLIP_NONE;
    if (!self->manualTrigger) {
        self->shouldAppear = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
                self->shouldAppear = true;
                foreach_break;
            }
        }
    }
    else if (PopOut->hasButton) {
        foreach_active(Button, button)
        {
            if (button->tag == self->tag && button->activated)
                self->shouldAppear = true;
        }
    }

    if (self->shouldAppear) {
        if (self->timer < self->delay) {
            self->timer++;
        }
        else {
            if (self->appearTimer < 8) {
                self->appearTimer++;
                if (self->childType <= POPOUT_CHILD_SPRING_YELLOW && child && self->appearTimer == 8) {
                    child->sfxTimer         = 0;
                    child->animator.speed   = 0;
                    child->animator.frameID = 0;
                }
            }
        }
    }
    else {
        self->timer = 0;
        if (self->appearTimer > 0) {
            self->appearTimer--;
            if (self->childType <= POPOUT_CHILD_SPRING_YELLOW && child && self->appearTimer == 7) {
                child->sfxTimer         = 0;
                child->animator.speed   = 0;
                child->animator.frameID = 0;
            }
        }
    }

    if (child) {
        if (self->appearTimer == 8) {
            child->active  = ACTIVE_BOUNDS;
            child->visible = true;
        }
        else {
            child->active  = ACTIVE_NEVER;
            child->visible = false;
        }
    }

    self->active = (self->appearTimer <= 0) ? ACTIVE_BOUNDS : ACTIVE_NORMAL;

    self->position.x += -32 * self->appearVelocity.x + 4 * self->appearTimer * self->appearVelocity.x;
    self->position.y += -32 * self->appearVelocity.y + 4 * self->appearTimer * self->appearVelocity.y;
    foreach_active(Player, playerLoop) { Player_CheckCollisionBox(playerLoop, self, &self->hitboxSolid); }
    self->direction  = storeDir;
    self->position.x = storeX;
    self->position.y = storeY;
}

void PopOut_LateUpdate(void) {}

void PopOut_StaticUpdate(void) {}

void PopOut_Draw(void)
{
    RSDK_THIS(PopOut);

    int32 dir        = self->direction;
    int32 storeX     = self->position.x;
    int32 storeY     = self->position.y;
    self->position.x = -16 * self->appearVelocity.x + self->position.x + 2 * self->appearTimer * self->appearVelocity.x;
    self->position.y = -16 * self->appearVelocity.y + self->position.y + 2 * self->appearTimer * self->appearVelocity.y;
    self->rotation   = self->orientation << 7;

    if (!self->appearTimer) {
        int32 off = 128;
        if (!dir)
            off = -128;
        self->rotation += off;
    }
    else if (self->appearTimer < 8) {
        int32 off = 16;
        if (!dir)
            off = -16;
        self->rotation += (7 - self->appearTimer) * off;
    }

    RSDK.DrawSprite(&self->mountAnimator, NULL, false);

    if (self->appearTimer < 8)
        RSDK.DrawSprite(&self->childAnimator, NULL, false);

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
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(PopOut->aniFrames, 0, &self->mountAnimator, true, 0);

        EntitySpring *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Spring);
        if (child->classID != Spring->classID && child->classID != Spikes->classID)
            child = NULL;

        if (child->classID == Spring->classID) {
            self->childType = (child->type & 1) ? POPOUT_CHILD_SPRING_RED : POPOUT_CHILD_SPRING_YELLOW;
            RSDK.SetSpriteAnimation(PopOut->aniFrames, 1, &self->childAnimator, true, self->childType);
        }
        else {
            // Assume its spikes
            self->childType = POPOUT_CHILD_SPIKES;
            RSDK.SetSpriteAnimation(PopOut->aniFrames, 2, &self->childAnimator, true, 0);
        }
        PopOut_SetupHitboxes();

        switch (self->orientation + 4 * self->direction) {
            case 0:
            case 4:
                self->appearVelocity.x = 0;
                self->appearVelocity.y = -0x10000;
                break;

            case 1:
            case 5:
                self->appearVelocity.y = 0;
                self->appearVelocity.x = 0x10000;
                break;

            case 2:
            case 6:
                self->appearVelocity.x = 0;
                self->appearVelocity.y = 0x10000;
                break;

            default:
            case 3:
            case 7:
                self->appearVelocity.y = 0;
                self->appearVelocity.x = -0x10000;
                break;
        }
    }
}

void PopOut_StageLoad(void)
{
    PopOut->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/PopOut.bin", SCOPE_STAGE);

    if (RSDK.FindObject("Button"))
        PopOut->hasButton = true;
}

void PopOut_SetupHitboxes(void)
{
    RSDK_THIS(PopOut);
    self->hitboxRange.left   = 0;
    self->hitboxRange.top    = 0;
    self->hitboxRange.right  = 0;
    self->hitboxRange.bottom = 0;

    self->hitboxSolid.left   = 0;
    self->hitboxSolid.top    = 0;
    self->hitboxSolid.right  = 0;
    self->hitboxSolid.bottom = 0;

    switch (self->orientation + 4 * self->direction) {
        case 0:
            self->hitboxRange.right  = 80;
            self->hitboxRange.top    = -64;
            self->hitboxRange.bottom = 32;

            self->hitboxSolid.left = -32;
            self->hitboxSolid.top  = -32;
            break;

        case 1:
            self->hitboxRange.right  = 80;
            self->hitboxRange.bottom = 64;
            self->hitboxRange.left   = -32;

            self->hitboxSolid.top   = -32;
            self->hitboxSolid.right = 32;
            break;

        case 2:
            self->hitboxRange.left   = -80;
            self->hitboxRange.top    = -32;
            self->hitboxRange.bottom = 64;

            self->hitboxSolid.right  = 32;
            self->hitboxSolid.bottom = 32;
            break;

        case 3:
            self->hitboxRange.left  = -64;
            self->hitboxRange.top   = -80;
            self->hitboxRange.right = 32;

            self->hitboxSolid.left   = -32;
            self->hitboxSolid.bottom = 32;
            break;

        case 4:
            self->hitboxRange.left   = -80;
            self->hitboxRange.top    = -63;
            self->hitboxRange.bottom = 32;

            self->hitboxSolid.top    = -32;
            self->hitboxSolid.right = 32;
            break;

        case 5:
            self->hitboxRange.top   = -80;
            self->hitboxRange.right = 64;
            self->hitboxRange.left  = -32;

            self->hitboxSolid.right  = 32;
            self->hitboxSolid.bottom = 32;
            break;

        case 6:
            self->hitboxRange.right  = 80;
            self->hitboxRange.bottom = 64;
            self->hitboxRange.top    = -32;

            self->hitboxSolid.left   = -32;
            self->hitboxSolid.bottom = 32;
            break;

        default:
        case 7:
            self->hitboxRange.right  = 80;
            self->hitboxRange.bottom = 32;
            self->hitboxRange.left   = -64;

            self->hitboxSolid.left = -32;
            self->hitboxSolid.top  = -32;
            break;
    }
}

#if GAME_INCLUDE_EDITOR
void PopOut_EditorDraw(void)
{
    RSDK_THIS(PopOut);

    PopOut->hasButton = false;
    if (RSDK.FindObject("Button"))
        PopOut->hasButton = true;

    RSDK.SetSpriteAnimation(PopOut->aniFrames, 0, &self->mountAnimator, true, 0);

    EntitySpring *child = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Spring);
    if (child->classID != Spring->classID && child->classID != Spikes->classID)
        child = NULL;

    if (child && child->classID == Spring->classID) {
        self->childType = (child->type & 1) ? POPOUT_CHILD_SPRING_RED : POPOUT_CHILD_SPRING_YELLOW;
        RSDK.SetSpriteAnimation(PopOut->aniFrames, 1, &self->childAnimator, true, self->childType);
    }
    else {
        self->childType = POPOUT_CHILD_SPIKES;
        RSDK.SetSpriteAnimation(PopOut->aniFrames, 2, &self->childAnimator, true, 0);
    }

    self->appearTimer      = 0;
    self->appearVelocity.x = 0;
    self->appearVelocity.y = 0;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    PopOut_Draw();

    if (showGizmos()) {
        PopOut_SetupHitboxes();

        switch (self->orientation + 4 * self->direction) {
            case 0:
            case 4:
                self->appearVelocity.x = 0;
                self->appearVelocity.y = -0x10000;
                break;

            case 1:
            case 5:
                self->appearVelocity.y = 0;
                self->appearVelocity.x = 0x10000;
                break;

            case 2:
            case 6:
                self->appearVelocity.x = 0;
                self->appearVelocity.y = 0x10000;
                break;

            default:
            case 3:
            case 7:
                self->appearVelocity.y = 0;
                self->appearVelocity.x = -0x10000;
                break;
        }

        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxRange, FLIP_NONE, 0xFF0000);

        self->appearTimer = 8;
        self->position.x  = -16 * self->appearVelocity.x + self->position.x + 2 * self->appearTimer * self->appearVelocity.x;
        self->position.y  = -16 * self->appearVelocity.y + self->position.y + 2 * self->appearTimer * self->appearVelocity.y;
        self->inkEffect   = INK_BLEND;

        PopOut_Draw();

        self->inkEffect  = INK_NONE;
        self->position.x = storeX;
        self->position.y = storeY;

        if (self->manualTrigger && PopOut->hasButton) {
            EntityButton *taggedButton = NULL;
            foreach_all(Button, button)
            {
                if (button->tag == self->tag) {
                    taggedButton = button;
                    foreach_break;
                }
            }

            if (taggedButton) {
                RSDK_DRAWING_OVERLAY(true);
                if (taggedButton) {
                    DrawHelpers_DrawArrow(taggedButton->position.x, taggedButton->position.y, self->position.x, self->position.y, 0xFFFF00, INK_NONE,
                                          0xFF);
                }
                RSDK_DRAWING_OVERLAY(false);
            }
        }
    }
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
