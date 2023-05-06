// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FXTrail Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFXTrail *FXTrail;

void FXTrail_Update(void) {}

void FXTrail_LateUpdate(void)
{
    RSDK_THIS(FXTrail);

    Entity *parent = self->parent;

    if (parent && self->animatorPtr && parent->classID) {
        // Update recordings
        for (int32 i = FXTRAIL_TRACK_COUNT - 1; i > 0; --i) {
            self->statePos[i].x     = self->statePos[i - 1].x;
            self->statePos[i].y     = self->statePos[i - 1].y;
            self->stateRotation[i]  = self->stateRotation[i - 1];
            self->stateDirection[i] = self->stateDirection[i - 1];
            self->stateVisible[i]   = self->stateVisible[i - 1];
            memcpy(&self->stateAnim[i], &self->stateAnim[i - 1], sizeof(Animator));
        }

        self->statePos[0].x     = self->currentPos.x;
        self->statePos[0].y     = self->currentPos.y;
        self->stateRotation[0]  = self->currentRotation;
        self->stateDirection[0] = self->currentDirection;
        self->stateVisible[0]   = self->currentVisible;
        memcpy(&self->stateAnim[0], &self->currentAnim, sizeof(Animator));

        // Record Parent
        self->drawGroup        = parent->drawGroup - 1;
        self->currentPos.x     = parent->position.x;
        self->currentPos.y     = parent->position.y;
        self->currentRotation  = parent->rotation;
        self->currentDirection = parent->direction;
        memcpy(&self->currentAnim, self->animatorPtr, sizeof(Animator));
    }
    else {
        destroyEntity(self);
    }
}

void FXTrail_StaticUpdate(void) {}

void FXTrail_Draw(void)
{
    RSDK_THIS(FXTrail);

    int32 alpha = 0x60 * self->baseAlpha >> 8;
    int32 inc   = 0x40 / (IMAGETRAIL_TRACK_COUNT / 3);

    for (int32 i = 2; i >= 0; --i) {
        int32 id = (i * 3) - (i - 1);
        if (self->stateVisible[id]) {
            self->alpha     = alpha;
            self->rotation  = self->stateRotation[id];
            self->direction = self->stateDirection[id];
            RSDK.DrawSprite(&self->stateAnim[id], &self->statePos[id], false);

            self->drawFX &= ~FX_SCALE;
            alpha += inc;
        }
    }
}

void FXTrail_Create(void *data)
{
    RSDK_THIS(FXTrail);
    if (!SceneInfo->inEditor) {
        Entity *parent = (Entity *)data;

        self->active    = ACTIVE_ALWAYS;
        self->visible   = true;
        self->parent    = parent;
        self->baseAlpha = 0x100;
        self->drawFX    = FX_FLIP | FX_ROTATE;
        self->inkEffect = INK_ALPHA;

        for (int32 i = FXTRAIL_TRACK_COUNT - 1; i >= 0; --i) {
            self->statePos[i].x     = parent->position.x;
            self->statePos[i].y     = parent->position.y;
            self->stateRotation[i]  = parent->rotation;
            self->stateDirection[i] = parent->direction;
            self->stateVisible[i]   = false;
        }
    }
}

void FXTrail_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void FXTrail_EditorDraw(void) {}

void FXTrail_EditorLoad(void) {}
#endif

void FXTrail_Serialize(void) {}
