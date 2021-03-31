#include "../SonicMania.h"

ObjectImageTrail *ImageTrail;

void ImageTrail_Update() {}

void ImageTrail_LateUpdate()
{
    RSDK_THIS(ImageTrail);
    EntityPlayer *player = (EntityPlayer *)entity->player;

    // Check for fadeouts/destroy flags
    if (player->superState == 2) {
        entity->baseAlpha = 0x100;
    }
    else {
        if (false /*player->state == Player_State_HammerDrop*/) {
            entity->fadeoutTimer = 0x10;
        }
        else {
            if (entity->fadeoutTimer <= 0) {
                if (player->speedShoesTimer < 32) {
                    entity->baseAlpha = player->speedShoesTimer;
                    entity->baseAlpha *= 8;
                    if (entity->baseAlpha == 0) {
                        RSDK.ResetEntityPtr(entity, 0, 0);
                    }
                }
            }
            else {
                entity->fadeoutTimer--;
                entity->baseAlpha = 0x10;
                entity->baseAlpha *= entity->fadeoutTimer;
                if (entity->baseAlpha == 0) {
                    RSDK.ResetEntityPtr(entity, 0, 0);
                }
            }
        }
    }

    // Update recordings
    for (int i = ImageTrail_trackCount - 1; i > 0; --i) {
        entity->statePos[i].x     = entity->statePos[i - 1].x;
        entity->statePos[i].y     = entity->statePos[i - 1].y;
        entity->stateRotation[i]  = entity->stateRotation[i - 1];
        entity->stateScale[i]     = entity->stateScale[i - 1];
        entity->stateDirection[i] = entity->stateDirection[i - 1];
        entity->stateVisible[i]   = entity->stateVisible[i - 1];
        memcpy(&entity->stateAnim[i], &entity->stateAnim[i - 1], sizeof(AnimationData));
    }

    entity->statePos[0].x     = entity->currentPos.x;
    entity->statePos[0].y     = entity->currentPos.y;
    entity->stateRotation[0]  = entity->currentRotation;
    entity->stateDirection[0] = entity->currentDirection;
    entity->stateScale[0]     = entity->currentScale;
    entity->stateVisible[0]   = entity->currentVisible;
    memcpy(&entity->stateAnim[0], &entity->currentAnimData, sizeof(AnimationData));

    // Record Player
    entity->drawOrder        = player->drawOrder - 1;
    entity->currentPos.x     = player->position.x;
    entity->currentPos.y     = player->position.y;
    entity->currentRotation  = player->rotation;
    entity->currentDirection = player->direction;
    memcpy(&entity->currentAnimData, &player->playerAnimData, sizeof(AnimationData));
    if (player->isChibi || !(player->drawFX & FX_SCALE))
        entity->currentScale = 0x200;
    else
        entity->currentScale = player->scale.x;

    // Check if we have enough speed to be visible
    if (abs(player->velocity.x) >= 0x10000 || abs(player->velocity.y) >= 0x10000) {
        entity->currentVisible = player->visible;
    }
    else {
        entity->currentVisible = false;
    }
}

void ImageTrail_StaticUpdate() {}

void ImageTrail_Draw()
{
    RSDK_THIS(ImageTrail);
    int alpha[3] = { 0xA0 * entity->baseAlpha >> 8, entity->baseAlpha >> 1, 0x60 * entity->baseAlpha >> 8 };
    for (int i = 2; i >= 0; --i) {
        int id = (i * 3) - (i - 1);
        if (entity->stateVisible[id]) {
            if (entity->stateScale[id] != 0x200) {
                entity->drawFX |= FX_SCALE;
                entity->scale.x = entity->stateScale[id];
                entity->scale.y = entity->stateScale[id];
            }
            entity->alpha     = alpha[i];
            entity->rotation  = entity->stateRotation[id];
            entity->direction = entity->stateDirection[id];
            RSDK.DrawSprite(&entity->stateAnim[id], &entity->statePos[id], 0);
            entity->drawFX &= ~FX_SCALE;
        }
    }
}

void ImageTrail_Create(void *data)
{
    RSDK_THIS(ImageTrail);
    if (!RSDK_sceneInfo->inEditor) {
        EntityPlayer *player = (EntityPlayer *)data;
        entity->active       = ACTIVE_ALWAYS;
        entity->visible      = true;
        entity->player       = (Entity *)player;
        entity->playerObjID  = player->objectID;
        entity->baseAlpha    = 0x100;
        entity->drawFX       = FX_FLIP | FX_SCALE | FX_ROTATE;
        entity->inkEffect    = INK_ALPHA;

        for (int i = ImageTrail_trackCount - 1; i >= 0; --i) {
            entity->statePos[i].x     = player->position.x;
            entity->statePos[i].y     = player->position.y;
            entity->stateRotation[i]  = player->rotation;
            entity->stateDirection[i] = player->direction;
            entity->stateVisible[i]   = false;
        }
    }
}

void ImageTrail_StageLoad() {}

void ImageTrail_EditorDraw() {}

void ImageTrail_EditorLoad() {}

void ImageTrail_Serialize() {}
