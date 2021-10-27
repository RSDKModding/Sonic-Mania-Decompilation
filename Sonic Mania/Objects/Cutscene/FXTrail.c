#include "SonicMania.h"

ObjectFXTrail *FXTrail;

void FXTrail_Update(void) {}

void FXTrail_LateUpdate(void)
{
    RSDK_THIS(FXTrail);
    Entity *parent = (Entity *)entity->parent;
    if (parent && entity->dataPtr && parent->objectID) {
        // Update recordings
        for (int32 i = FXTrail_trackCount - 1; i > 0; --i) {
            entity->statePos[i].x     = entity->statePos[i - 1].x;
            entity->statePos[i].y     = entity->statePos[i - 1].y;
            entity->stateRotation[i]  = entity->stateRotation[i - 1];
            entity->stateDirection[i] = entity->stateDirection[i - 1];
            entity->stateVisible[i]   = entity->stateVisible[i - 1];
            memcpy(&entity->stateAnim[i], &entity->stateAnim[i - 1], sizeof(Animator));
        }
        entity->statePos[0].x     = entity->currentPos.x;
        entity->statePos[0].y     = entity->currentPos.y;
        entity->stateRotation[0]  = entity->currentRotation;
        entity->stateDirection[0] = entity->currentDirection;
        entity->stateVisible[0]   = entity->currentVisible;
        memcpy(&entity->stateAnim[0], &entity->currentAnim, sizeof(Animator));

        // Record Parent
        entity->drawOrder        = parent->drawOrder - 1;
        entity->currentPos.x     = parent->position.x;
        entity->currentPos.y     = parent->position.y;
        entity->currentRotation  = parent->rotation;
        entity->currentDirection = parent->direction;
        memcpy(&entity->currentAnim, entity->dataPtr, sizeof(Animator));
    }
    else {
        destroyEntity(entity);
    }
}

void FXTrail_StaticUpdate(void) {}

void FXTrail_Draw(void)
{
    RSDK_THIS(FXTrail);
    // int32 alpha[3] = { 0xA0 * entity->baseAlpha >> 8, entity->baseAlpha >> 1, 0x60 * entity->baseAlpha >> 8 };
    int32 alpha = 0x60 * entity->baseAlpha >> 8;
    int32 inc   = 0x40 / (ImageTrail_TrackCount / 3);

    for (int32 i = 2; i >= 0; --i) {
        int32 id = (i * 3) - (i - 1);
        if (entity->stateVisible[id]) {
            entity->alpha     = alpha;
            entity->rotation  = entity->stateRotation[id];
            entity->direction = entity->stateDirection[id];
            RSDK.DrawSprite(&entity->stateAnim[id], &entity->statePos[id], 0);
            entity->drawFX &= ~FX_SCALE;
            alpha += inc;
        }
    }
}

void FXTrail_Create(void *data)
{
    RSDK_THIS(FXTrail);
    if (!RSDK_sceneInfo->inEditor) {
        Entity *parent    = (Entity *)data;
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->parent    = (Entity *)parent;
        entity->baseAlpha = 0x100;
        entity->drawFX    = FX_FLIP | FX_ROTATE;
        entity->inkEffect = INK_ALPHA;

        for (int32 i = FXTrail_trackCount - 1; i >= 0; --i) {
            entity->statePos[i].x     = parent->position.x;
            entity->statePos[i].y     = parent->position.y;
            entity->stateRotation[i]  = parent->rotation;
            entity->stateDirection[i] = parent->direction;
            entity->stateVisible[i]   = false;
        }
    }
}

void FXTrail_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void FXTrail_EditorDraw(void) {}

void FXTrail_EditorLoad(void) {}
#endif

void FXTrail_Serialize(void) {}
