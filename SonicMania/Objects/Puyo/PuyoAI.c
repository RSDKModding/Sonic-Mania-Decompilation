// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoAI Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPuyoAI *PuyoAI;

void PuyoAI_Update(void) {}

void PuyoAI_LateUpdate(void) {}

void PuyoAI_StaticUpdate(void) {}

void PuyoAI_Draw(void) {}

void PuyoAI_Create(void *data) {}

void PuyoAI_StageLoad(void) {}

Vector2 PuyoAI_GetBeanPos(int32 playerID)
{
    Vector2 pos = { -1, -1 };

    foreach_all(PuyoBean, bean)
    {
        if (bean->stateInput && bean->state == PuyoBean_State_Controlled && bean->playerID == playerID) {
            pos.x = bean->stillPos.x;
            pos.y = bean->stillPos.y;
        }
    }

    return pos;
}

void PuyoAI_PrepareAction(int32 playerID)
{
    EntityPuyoBean *bean    = NULL;
    EntityPuyoBean *partner = NULL;

    foreach_all(PuyoBean, beanPtr)
    {
        if (beanPtr->stateInput && beanPtr->state == PuyoBean_State_Controlled && beanPtr->playerID == playerID) {
            bean    = beanPtr;
            partner = bean->partner;
        }
    }

    int32 lastBeanY             = PuyoAI->lastBeanY[playerID];
    int32 beanPos                 = MIN(bean->stillPos.y, partner->stillPos.y);
    PuyoAI->lastBeanY[playerID] = beanPos;

    if (lastBeanY > beanPos) {
        uint8 columnHeights[PUYO_PLAYFIELD_W];
        memset(columnHeights, 0, sizeof(columnHeights));

        for (int32 x = 0; x < PUYO_PLAYFIELD_W; ++x) columnHeights[x] = (PUYO_PLAYFIELD_H - 1) - PuyoBean_GetColumnHeight(playerID, x, bean, partner);

        beanPos = MAX(bean->stillPos.y, partner->stillPos.y);

        int32 startX = MIN(bean->stillPos.x, partner->stillPos.x);
        int32 endX   = MAX(bean->stillPos.x, partner->stillPos.x);

        while ((bean->stillPos.x == startX || partner->stillPos.x == startX || columnHeights[startX] > beanPos) && startX > 0) startX--;

        while ((bean->stillPos.x == endX || partner->stillPos.x == endX || columnHeights[endX] > beanPos) && endX < PUYO_PLAYFIELD_W) endX++;

        int32 lastY    = 0;
        int32 beanX    = 0;
        int32 beanY    = 0;
        int32 partnerX = 0;
        int32 partnerY = 0;

        for (int32 x = startX + 1; x < endX; ++x) {
            for (int32 orientation = 0; orientation < 4; ++orientation) {
                switch (orientation) {
                    case 0: // Oriented Left
                        if (x >= (PUYO_PLAYFIELD_W - 1))
                            continue;

                        beanX    = x;
                        beanY    = columnHeights[x];
                        partnerX = x + 1;
                        partnerY = columnHeights[x + 1];
                        break;

                    case 1: // Oriented Up
                        beanX    = x;
                        beanY    = columnHeights[x] - 1;
                        partnerX = x;
                        partnerY = columnHeights[x];
                        break;

                    case 2: // Oriented Right
                        if (bean->type == partner->type || x >= (PUYO_PLAYFIELD_W - 1))
                            continue;

                        beanX    = x + 1;
                        beanY    = columnHeights[x + 1];
                        partnerX = x;
                        partnerY = columnHeights[x];
                        break;

                    case 3: // Oriented Down
                        beanX    = x;
                        beanY    = columnHeights[x];
                        partnerX = x;
                        partnerY = columnHeights[x] - 1;
                        break;

                    default: break;
                }

                if (beanX < PUYO_PLAYFIELD_W && beanY < PUYO_PLAYFIELD_H && partnerX < PUYO_PLAYFIELD_W && partnerY < PUYO_PLAYFIELD_H) {
                    int32 chainComboSize = PuyoAI_GetChainComboSize(playerID, bean, partner, beanX, beanY, partnerX, partnerY);
                    if (chainComboSize < 16) {
                        if (!beanY && (beanX == 2 || beanX == 3))
                            chainComboSize = -1;

                        if (!partnerY && (partnerX == 2 || partnerX == 3))
                            chainComboSize = -1;
                    }

                    int32 beanAvaliableLinks    = PuyoBean_GetAvaliableLinks(playerID, bean, beanX, beanY);
                    int32 partnerAvaliableLinks = PuyoBean_GetAvaliableLinks(playerID, partner, partnerX, partnerY);

                    int32 linkCount = (beanAvaliableLinks > 0) + (partnerAvaliableLinks > 0);

                    if (orientation == 1 || orientation == 3)
                        chainComboSize = (0x70000 * chainComboSize) >> 19;

                    for (; linkCount; --linkCount) chainComboSize = (0x30000 * chainComboSize) >> 18;

                    int32 newBeanY = (chainComboSize * ((MIN(beanY, partnerY) << 16) / 4 + 1)) >> 16;
                    if (newBeanY > lastY || (newBeanY == lastY && RSDK.Rand(0, 10) > 5)) {
                        lastY                             = newBeanY;
                        PuyoAI->desiredColumn[playerID]   = beanX;
                        PuyoAI->desiredRotation[playerID] = orientation;
                    }
                }
            }
        }
    }
}

int32 PuyoAI_GetChainComboSize(int32 playerID, EntityPuyoBean *bean, EntityPuyoBean *partner, int32 beanX, int32 beanY, int32 partnerX,
                               int32 partnerY)
{
    for (int32 i = 0; i < (PUYO_PLAYFIELD_W * PUYO_PLAYFIELD_H); ++i) PuyoBean->beanLinkTable[i] = false;

    int32 removeCount        = PuyoBean_GetBeanChainRemovalCount(playerID, bean, beanX, beanY);
    int32 partnerRemoveCount = PuyoBean_GetBeanChainRemovalCount(playerID, partner, partnerX, partnerY);

    if (bean->type == partner->type && (beanX == partnerX || beanY == partnerY))
        return 1 << (removeCount + partnerRemoveCount);
    else
        return (1 << removeCount) + (1 << partnerRemoveCount);
}

void PuyoAI_SetupInputs(EntityPuyoBean *bean, bool32 rotationDisabled)
{
    bean->down = RSDK.Rand(0, 6) > 3;

    if (((bean->position.x - bean->origin.x) & 0xFFF00000) == 0x200000) {
        if (RSDK.Rand(0, 2)) {
            bean->left  = false;
            bean->right = true;
        }
        else {
            bean->left  = true;
            bean->right = false;
        }
    }
    else {
        bean->left  = RSDK.Rand(0, 2);
        bean->right = RSDK.Rand(0, 2);
    }

    if (!rotationDisabled) {
        bean->rotateRight = RSDK.Rand(0, 6) > 3;
        bean->rotateLeft  = RSDK.Rand(0, 6) > 3;
    }

    if (bean->left && bean->right) {
        bean->left  = false;
        bean->right = false;
    }
}

void PuyoAI_Input_AI(void)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean *partner = self->partner;

    self->left        = false;
    self->right       = false;
    self->rotateLeft  = false;
    self->rotateRight = false;

    if (PuyoAI->isAI[self->playerID]) {
        bool32 canMove   = false;
        bool32 canRotate = false;
        if (PuyoAI->controlInterval[self->playerID]) {
            canMove   = !(RSDK.Rand(0, 1024) % PuyoAI->controlInterval[self->playerID]);
            canRotate = !(Zone->timer % (4 * PuyoAI->controlInterval[self->playerID]));
        }
        else {
            canMove   = true;
            canRotate = true;
        }

        if (canMove || canRotate) {
            Vector2 beanPos = PuyoAI_GetBeanPos(self->playerID);
            if (beanPos.x >= 0 || beanPos.y >= 0) {
                bool32 rotationDisabled = PuyoBean_CheckAIRotationDisabled(self);
                if (RSDK.Rand(0, 100) < PuyoAI->controlChance[self->playerID]) {
                    PuyoAI_SetupInputs(self, rotationDisabled);
                }
                else {
                    PuyoAI_PrepareAction(self->playerID);

                    uint8 currentRotation = 0;
                    if (self->stillPos.y == partner->stillPos.y && self->stillPos.x >= partner->stillPos.x) {
                        if (self->stillPos.y == partner->stillPos.y && self->stillPos.x > partner->stillPos.x) {
                            currentRotation = 2;
                        }
                    }

                    if (currentRotation != 2 && self->stillPos.x == partner->stillPos.x) {
                        if (self->stillPos.y >= partner->stillPos.y) {
                            if (self->stillPos.x == partner->stillPos.x && self->stillPos.y > partner->stillPos.y)
                                currentRotation = 3;
                        }
                        else {
                            currentRotation = 1;
                        }
                    }

                    int32 targetRotation = PuyoAI->desiredRotation[self->playerID] - currentRotation;
                    if (targetRotation == 3)
                        targetRotation = -1;

                    if (canRotate) {
                        if ((rotationDisabled && targetRotation == 2) || (!rotationDisabled && targetRotation > 0)) {
                            self->rotateLeft = true;
                            canMove          = false;
                        }
                        else if (!rotationDisabled && targetRotation < 0) {
                            self->rotateRight = true;
                            canMove           = false;
                        }
                    }

                    if (canMove) {
                        self->left  = beanPos.x > PuyoAI->desiredColumn[self->playerID];
                        self->right = beanPos.x < PuyoAI->desiredColumn[self->playerID];

                        if (!targetRotation && !self->left && !self->right && beanPos.x == PuyoAI->desiredColumn[self->playerID])
                            self->down = true;
                        else
                            self->down = false;
                    }
                }
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void PuyoAI_EditorDraw(void) {}

void PuyoAI_EditorLoad(void) {}
#endif

void PuyoAI_Serialize(void) {}
