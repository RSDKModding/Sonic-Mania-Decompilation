// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoBean Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPuyoBean *PuyoBean;

void PuyoBean_Update(void)
{
    RSDK_THIS(PuyoBean);

    StateMachine_Run(self->state);
}

void PuyoBean_LateUpdate(void) {}

void PuyoBean_StaticUpdate(void)
{
    RSDK.ProcessAnimation(&PuyoBean->junkBeanAnimator);

    if (--PuyoBean->shinkDelay <= 0) {
        PuyoBean->shinkDelay = 15 * RSDK.Rand(1, 24);

        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, PUYOBEAN_JUNK + PUYOBEAN_ANI_BOUNCE, &PuyoBean->junkBeanAnimator, true, 0);
    }
}

void PuyoBean_Draw(void)
{
    RSDK_THIS(PuyoBean);

    if (SceneInfo->state != ENGINESTATE_FROZEN) {
        if (self->isJunk)
            RSDK.DrawSprite(&PuyoBean->junkBeanAnimator, NULL, false);
        else
            RSDK.DrawSprite(&self->beanAnimator, NULL, false);
    }
}

void PuyoBean_Create(void *data)
{
    RSDK_THIS(PuyoBean);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = PuyoGame ? Zone->objectDrawGroup[0] : Zone->objectDrawGroup[1];
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->angle        = 0xC0;
        self->position.x   = (self->position.x & 0xFFF00000) + 0x80000;
        self->position.y   = (self->position.y & 0xFFF00000) + 0x80000;
        self->origin.x     = self->position.x - 0x280000;
        self->origin.y     = self->position.y - 0x80000;
        self->controllerID = CONT_P1;

        self->type = VOID_TO_INT(data);
        if (self->type == PUYOBEAN_JUNK)
            self->isJunk = true;

        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type, &self->beanAnimator, true, 0);
    }
}

void PuyoBean_StageLoad(void)
{
    PuyoBean->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, PUYOBEAN_JUNK + PUYOBEAN_ANI_BOUNCE, &PuyoBean->junkBeanAnimator, true, 10);

    PuyoBean->hitboxBean.left   = -8;
    PuyoBean->hitboxBean.top    = -8;
    PuyoBean->hitboxBean.right  = 8;
    PuyoBean->hitboxBean.bottom = 8;

    PuyoBean->sfxLand        = RSDK.GetSfx("Puyo/Land.wav");
    PuyoBean->sfxRotate      = RSDK.GetSfx("Puyo/Rotate.wav");
    PuyoBean->sfxJunk        = RSDK.GetSfx("Puyo/Junk.wav");
    PuyoBean->sfxFall        = RSDK.GetSfx("Puyo/Fall.wav");
    PuyoBean->chainFrames[0] = RSDK.GetSfx("Puyo/Chain0.wav");
    PuyoBean->chainFrames[1] = RSDK.GetSfx("Puyo/Chain1.wav");
    PuyoBean->chainFrames[2] = RSDK.GetSfx("Puyo/Chain2.wav");
    PuyoBean->chainFrames[3] = RSDK.GetSfx("Puyo/Chain3.wav");
    PuyoBean->chainFrames[4] = RSDK.GetSfx("Puyo/Chain4.wav");
    PuyoBean->chainFrames[5] = RSDK.GetSfx("Puyo/Chain5.wav");

    for (int32 i = 0; i < 0x100; ++i) PuyoBean->playfield[i] = NULL;
}

EntityPuyoBean *PuyoBean_GetPuyoBean(int32 playerID, int32 x, int32 y)
{
    if (x >= 0 && y >= 0 && x < PUYO_PLAYFIELD_W && y < PUYO_PLAYFIELD_H)
        return PuyoBean->playfield[128 * playerID + 8 * y + x];

    return NULL;
}

void PuyoBean_Input_Player(void)
{
    RSDK_THIS(PuyoBean);

    if (self->controllerID < PLAYER_COUNT) {
        RSDKControllerState *controller = &ControllerInfo[self->controllerID];
        RSDKAnalogState *stick          = &AnalogStickInfoL[self->controllerID];

        self->down  = controller->keyDown.down;
        self->left  = controller->keyLeft.down;
        self->right = controller->keyRight.down;

#if MANIA_USE_PLUS
        self->down |= stick->vDelta < -0.3;
        self->left |= stick->hDelta < -0.3;
        self->right |= stick->hDelta > 0.3;
#else
        self->down |= stick->vDeltaL < -0.3;
        self->left |= stick->hDeltaL < -0.3;
        self->right |= stick->hDeltaL > 0.3;
#endif

        if (self->left && self->right) {
            self->left  = false;
            self->right = false;
        }
        self->rotateLeft       = controller->keyB.press || controller->keyC.press || controller->keyY.press;
        self->rotateRight      = controller->keyA.press || controller->keyX.press;
        self->forceRotateLeft  = false;
        self->forceRotateRight = false;
    }
}

void PuyoBean_DestroyPuyoBeans(void)
{
    foreach_all(PuyoBean, bean)
    {
        if (bean->stateInput && bean->state == PuyoBean_State_Controlled) {
            if (bean->partner)
                destroyEntity(bean->partner);

            destroyEntity(bean);
        }
    }
}

void PuyoBean_HandleBeanLinks(void)
{
    RSDK_THIS(PuyoBean);

    if (self->stillPos.x >= 0 && self->stillPos.y < PUYO_PLAYFIELD_H && self->stillPos.x < PUYO_PLAYFIELD_W) {
        self->linkSides = 0;
        self->linkCount = 0;

        int32 ny           = self->stillPos.y - 1;
        self->linkBeans[0] = NULL;
        if (ny > -1) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny);
            if (bean && bean->type == self->type) {
                self->linkSides |= 1;
                self->linkBeans[0] = bean;
            }
        }

        ny                 = self->stillPos.y + 1;
        self->linkBeans[1] = NULL;
        if (ny < PUYO_PLAYFIELD_H) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny);
            if (bean && bean->type == self->type) {
                self->linkSides |= 2;
                self->linkBeans[1] = bean;
            }
        }

        int32 nx           = self->stillPos.x - 1;
        self->linkBeans[2] = NULL;
        if (nx > -1) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y);
            if (bean && bean->type == self->type) {
                self->linkSides |= 4;
                self->linkBeans[2] = bean;
            }
        }

        nx                 = self->stillPos.x + 1;
        self->linkBeans[3] = NULL;
        if (nx < PUYO_PLAYFIELD_W) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y);
            if (bean && bean->type == self->type) {
                self->linkSides |= 8;
                self->linkBeans[3] = bean;
            }
        }
    }
}

void PuyoBean_CheckBeanLinks(EntityPuyoBean *bean, EntityPuyoBean *curLink)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean **linkBeans = bean->linkBeans;
    for (int32 i = 0; i < 4; ++i) {
        EntityPuyoBean *link = bean->linkBeans[i];

        if (link && link != curLink) {
            if (++self->linkCount > 2) // Beans can only be connected on 2 axis at a time
                return;

            PuyoBean_CheckBeanLinks(link, bean);
        }

        ++linkBeans;
    }
}

void PuyoBean_HandleMoveBounds(void)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean *partner = self->partner;
    int32 entityX           = self->stillPos.x;
    int32 entityY           = self->stillPos.y;
    int32 partnerX          = partner->stillPos.x;
    int32 partnerY          = partner->stillPos.y;

    EntityPuyoBean *bean  = PuyoBean_GetPuyoBean(self->playerID, entityX, entityY);
    EntityPuyoBean *bean2 = PuyoBean_GetPuyoBean(self->playerID, partnerX, partnerY);

    self->position.x -= entityX << 20;
    self->position.y -= entityY << 20;

    bool32 canMoveRight = self->velocity.x < 0;
    if (self->velocity.x >= 0) {
        canMoveRight =
            ((self->angle > 0x40 && self->rotateSpeed > 0) || (self->angle < 0xC0 && self->rotateSpeed < 0)) && (self->targetAngle == 0x80);
    }

    if (entityX < 0 || partnerX < 0 || (canMoveRight && (bean || bean2))) {
        ++entityX;
        ++partnerX;
    }

    bool32 canMoveLeft = self->velocity.x > 0;
    if (self->velocity.x <= 0) {
        canMoveLeft = ((self->angle < 0x40 && self->rotateSpeed < 0) || (self->angle > 0xC0 && self->rotateSpeed > 0)) && (self->targetAngle == 0x00);
    }

    if (entityX >= PUYO_PLAYFIELD_W || partnerX >= PUYO_PLAYFIELD_W || (canMoveLeft && (bean || bean2))) {
        --entityX;
        --partnerX;
    }

    bool32 canMoveUp =
        ((self->angle > 0x00 && self->rotateSpeed > 0) || (self->angle < 0x80 && self->rotateSpeed < 0)) && (self->targetAngle == 0x40);
    if (entityY >= PUYO_PLAYFIELD_H || partnerY >= PUYO_PLAYFIELD_H || (canMoveUp && (bean || bean2))) {
        --entityY;
        --partnerY;
    }

    self->stillPos.x    = entityX;
    self->stillPos.y    = entityY;
    partner->stillPos.x = partnerX;
    partner->stillPos.y = partnerY;

    self->position.x += self->stillPos.x << 20;
    self->position.y += self->stillPos.y << 20;
}

bool32 PuyoBean_CheckAIRotationDisabled(EntityPuyoBean *bean)
{
    if (bean->targetAngle != 0x40 && bean->targetAngle != 0xC0)
        return 0;

    uint8 disabledAxis = 0;
    int32 y            = bean->stillPos.y;

    if (bean->stillPos.x > 0) {
        int32 nx = bean->stillPos.x - 1;
        if (PuyoBean_GetPuyoBean(bean->playerID, nx, y))
            disabledAxis = 1;
    }
    else {
        disabledAxis = 1;
    }

    if (bean->stillPos.x < (PUYO_PLAYFIELD_W - 1)) {
        int32 nx = bean->stillPos.x + 1;
        if (PuyoBean_GetPuyoBean(bean->playerID, nx, y))
            disabledAxis |= 2;
    }
    else {
        disabledAxis |= 2;
    }

    return disabledAxis == 3;
}

void PuyoBean_CheckCollisions(void)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean *partner = self->partner;
    uint8 wallCollisions    = 0;

    if (self->position.x - self->origin.x != self->stillPos.x << 20) {
        int32 nx = self->stillPos.x - 1;

        if (self->stillPos.x <= 0 || PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y)) {
            self->left     = false;
            wallCollisions = 1;
        }

        nx = self->stillPos.x + 1;
        if (self->stillPos.x >= (PUYO_PLAYFIELD_W - 1) || PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y)) {
            self->right = false;
            wallCollisions |= 2;
        }

        if (wallCollisions == 3) {
            if (self->angle == 0x40 || self->angle == 0xC0) {
                if (self->rotateRight)
                    self->forceRotateLeft = true;
                else if (self->rotateLeft)
                    self->forceRotateRight = true;
            }

            self->rotateRight = false;
            self->rotateLeft  = false;
        }
    }

    if (partner->position.x - partner->origin.x != partner->stillPos.x << 20) {
        if (partner->stillPos.x >= 1) {
            int32 nx = partner->stillPos.x - 1;

            if (PuyoBean_GetPuyoBean(self->playerID, nx, partner->stillPos.y)) {
                self->left = false;
            }
        }
        else {
            self->left = false;
        }

        int32 nx = partner->stillPos.x + 1;
        if (partner->stillPos.x > 4 || PuyoBean_GetPuyoBean(self->playerID, nx, partner->stillPos.y)) {
            self->right = false;
        }
    }

    self->onGround    = false;
    partner->onGround = false;

    if (self->position.y & 0xF0000) {
        int32 ny = self->stillPos.y + 1;

        if (self->stillPos.y == 13 || PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny)) {
            self->onGround = true;
        }
    }

    if (partner->position.y & 0xF0000) {
        int32 ny = partner->stillPos.y + 1;

        if (partner->stillPos.y == 13 || PuyoBean_GetPuyoBean(self->playerID, partner->stillPos.x, ny)) {
            partner->onGround = true;
        }
    }

    self->onGround |= partner->onGround;
}

int32 PuyoBean_GetBeanChainRemovalCount(int32 playerID, EntityPuyoBean *bean, int32 x, int32 y)
{
    PuyoBean->beanLinkTable[PUYO_PLAYFIELD_W * y + x] = true;

    int32 beanLinkCount = 0;
    int32 junkBeanCount = 0;

    for (int32 i = 0; i < 4; ++i) {
        int32 nx = x;
        int32 ny = y;
        switch (i) {
            case 0: nx = x - 1; break;
            case 1: nx = x + 1; break;
            case 2: ny = y - 1; break;
            case 3: ny = y + 1; break;
            default: break;
        }

        EntityPuyoBean *beanState = PuyoBean_GetPuyoBean(playerID, nx, ny);
        if (beanState) {
            if (bean->type == beanState->type) {
                PuyoBean_SetupBeanLinkTable(playerID, nx, ny, false);
                beanLinkCount += PuyoBean->beanLinkCount;
            }
        }

        for (int32 b = 0; b < PuyoBean->beanLinkCount; ++b) {
            Vector2 positions[4];

            positions[0].x = PuyoBean->beanLinkPositions[b].x - 1;
            positions[0].y = PuyoBean->beanLinkPositions[b].y;

            positions[1].x = PuyoBean->beanLinkPositions[b].x + 1;
            positions[1].y = PuyoBean->beanLinkPositions[b].y;

            positions[2].x = PuyoBean->beanLinkPositions[b].x;
            positions[2].y = PuyoBean->beanLinkPositions[b].y - 1;

            positions[3].x = PuyoBean->beanLinkPositions[b].x;
            positions[3].y = PuyoBean->beanLinkPositions[b].y + 1;

            for (int32 p = 0; p < 4; ++p) {
                int32 bx = positions[p].x;
                int32 by = positions[p].y;

                beanState = PuyoBean_GetPuyoBean(playerID, bx, by);
                if (beanState && !PuyoBean->beanLinkTable[bx + PUYO_PLAYFIELD_W * by]) {
                    if (beanState->isJunk) {
                        PuyoBean->beanLinkTable[bx + PUYO_PLAYFIELD_W * by] = true;
                        ++junkBeanCount;
                    }
                }
            }
        }
    }

    return junkBeanCount + beanLinkCount + 1;
}

int32 PuyoBean_GetAvaliableLinks(int32 playerID, EntityPuyoBean *bean, int32 x, int32 y)
{
    int32 avaliableLinks = 0;
    for (int32 i = 0; i < 4; ++i) {
        int32 bx = x;
        int32 by = y;
        switch (i) {
            default: break;
            case 0: bx = x - 1; break;
            case 1: bx = x + 1; break;
            case 2: by = y - 1; break;
            case 3: by = y + 1; break;
        }

        EntityPuyoBean *beanState = PuyoBean_GetPuyoBean(playerID, bx, by);
        if (beanState && beanState->type != bean->type) {
            PuyoBean_SetupBeanLinkTable(playerID, bx, by, true);

            if (PuyoBean->beanLinkCount >= 3 && !PuyoBean_CheckLinkPosAvaliable(playerID, x, y)) {
                avaliableLinks++;
            }
        }
    }

    return avaliableLinks;
}

bool32 PuyoBean_CheckLinkPosAvaliable(int32 playerID, int32 x, int32 y)
{
    bool32 beanLinkTable[0x101];
    memset(beanLinkTable, 0, sizeof(beanLinkTable));

    for (int32 b = 0; b < PuyoBean->beanLinkCount; ++b) {
        Vector2 possibleLinks[4];

        possibleLinks[0].x = PuyoBean->beanLinkPositions[b].x - 1;
        possibleLinks[0].y = PuyoBean->beanLinkPositions[b].y;

        possibleLinks[1].x = PuyoBean->beanLinkPositions[b].x + 1;
        possibleLinks[1].y = PuyoBean->beanLinkPositions[b].y;

        possibleLinks[2].x = PuyoBean->beanLinkPositions[b].x;
        possibleLinks[2].y = PuyoBean->beanLinkPositions[b].y - 1;

        possibleLinks[3].x = PuyoBean->beanLinkPositions[b].x;
        possibleLinks[3].y = PuyoBean->beanLinkPositions[b].y + 1;

        for (int32 i = 0; i < 4; ++i) {
            int32 bx = possibleLinks[i].x;
            int32 by = possibleLinks[i].y;

            if (!beanLinkTable[bx + PUYO_PLAYFIELD_W * by]) {
                beanLinkTable[bx + PUYO_PLAYFIELD_W * by] = true;

                if (bx != x || by != y) {
                    EntityPuyoBean *beanState = PuyoBean_GetPuyoBean(playerID, bx, by);
                    if (bx >= 0 && by >= 0 && bx <= (PUYO_PLAYFIELD_W - 1) && by <= (PUYO_PLAYFIELD_H - 1) && !beanState)
                        return true;
                }
            }
        }
    }

    return false;
}

void PuyoBean_SetupBeanLinkTable(int32 playerID, int32 x, int32 y, bool32 useTempTable)
{
    bool32 tempBeanLinkTable[0x101];
    memset(tempBeanLinkTable, 0, sizeof(tempBeanLinkTable));

    bool32 *beanLinkTable = useTempTable ? tempBeanLinkTable : PuyoBean->beanLinkTable;

    if (!beanLinkTable[x + PUYO_PLAYFIELD_W * y]) {
        for (int32 i = 0; i < (PUYO_PLAYFIELD_W * PUYO_PLAYFIELD_H); ++i) {
            PuyoBean->beanLinkPositions[i].x = 0;
            PuyoBean->beanLinkPositions[i].y = 0;
        }

        PuyoBean->beanLinkCount = 0;
        if (x >= 0 && y >= 0 && x <= (PUYO_PLAYFIELD_W - 1) && y <= (PUYO_PLAYFIELD_H - 1)) {
            EntityPuyoBean *beanState = PuyoBean_GetPuyoBean(playerID, x, y);
            if (beanState) {
                PuyoBean->beanLinkPositions[0].x = x;
                PuyoBean->beanLinkPositions[0].y = y;
                int32 linkCount                  = 1;

                for (int32 p = 0; p < linkCount; ++p) {
                    if (linkCount >= 0xFF)
                        break;

                    int32 bx = PuyoBean->beanLinkPositions[linkCount].x;
                    int32 by = PuyoBean->beanLinkPositions[linkCount].y;

                    EntityPuyoBean *startBean = NULL, *curBean = NULL;
                    startBean = PuyoBean_GetPuyoBean(playerID, bx, by);

                    beanLinkTable[bx + PUYO_PLAYFIELD_W * by] = true;

                    if (startBean) {
                        int32 nx = bx - 1;
                        if (nx >= 0) {
                            if (!beanLinkTable[nx + PUYO_PLAYFIELD_W * by] && by >= 0 && nx <= (PUYO_PLAYFIELD_W - 1)
                                && by <= (PUYO_PLAYFIELD_H - 1)) {
                                curBean = PuyoBean_GetPuyoBean(playerID, nx, by);

                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->beanLinkPositions[linkCount].x   = nx;
                                    PuyoBean->beanLinkPositions[linkCount++].y = by;
                                }
                            }
                        }

                        nx = bx + 1;
                        if (nx <= (PUYO_PLAYFIELD_W - 1)) {
                            if (!beanLinkTable[nx + PUYO_PLAYFIELD_W * by] && nx >= 0 && by >= 0 && by <= (PUYO_PLAYFIELD_H - 1)) {
                                curBean = PuyoBean_GetPuyoBean(playerID, nx, by);

                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->beanLinkPositions[linkCount].x   = nx;
                                    PuyoBean->beanLinkPositions[linkCount++].y = by;
                                }
                            }
                        }

                        int32 ny = by - 1;
                        if (ny >= 0) {
                            if (!beanLinkTable[bx + PUYO_PLAYFIELD_W * ny] && bx >= 0 && bx <= (PUYO_PLAYFIELD_W - 1)
                                && ny <= (PUYO_PLAYFIELD_H - 1)) {
                                curBean = PuyoBean_GetPuyoBean(playerID, bx, ny);

                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->beanLinkPositions[linkCount].x   = bx;
                                    PuyoBean->beanLinkPositions[linkCount++].y = ny;
                                }
                            }
                        }

                        ny = by + 1;
                        if (ny <= (PUYO_PLAYFIELD_H - 1)) {
                            if (!beanLinkTable[bx + PUYO_PLAYFIELD_W * ny] && bx >= 0 && ny >= 0 && bx <= 5) {
                                curBean = PuyoBean_GetPuyoBean(playerID, bx, ny);

                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->beanLinkPositions[linkCount].x   = bx;
                                    PuyoBean->beanLinkPositions[linkCount++].y = ny;
                                }
                            }
                        }
                    }
                }

                PuyoBean->beanLinkCount = linkCount;
            }
        }
    }
}

uint8 PuyoBean_GetColumnHeight(int32 playerID, int32 column, EntityPuyoBean *bean, EntityPuyoBean *partner)
{
    if (column < 0 || column >= PUYO_PLAYFIELD_W)
        return PUYO_PLAYFIELD_H - 1;

    int32 height = 0;
    for (int32 y = (PUYO_PLAYFIELD_H - 1); y >= 0; --y) {
        if (bean && column == bean->stillPos.x && y == bean->stillPos.y)
            break;

        if (partner && column == partner->stillPos.x && y == partner->stillPos.y)
            break;

        if (!PuyoBean_GetPuyoBean(playerID, column, y))
            break;

        ++height;
    }

    return height;
}

void PuyoBean_CalculateStillPos(EntityPuyoBean *bean)
{
    if (bean->state == PuyoBean_State_PartnerControlled) {
        EntityPuyoBean *partner = bean->partner;

        switch (partner->targetAngle >> 6) {
            case 0:
                bean->stillPos.y = partner->stillPos.y;
                bean->stillPos.x = partner->stillPos.x + 1;
                break;

            case 1:
                bean->stillPos.x = partner->stillPos.x;
                bean->stillPos.y = partner->stillPos.y + 1;
                break;

            case 2:
                bean->stillPos.y = partner->stillPos.y;
                bean->stillPos.x = partner->stillPos.x - 1;
                break;

            case 3:
                bean->stillPos.x = partner->stillPos.x;
                bean->stillPos.y = partner->stillPos.y - 1;
                break;

            default:
                bean->stillPos.x = partner->stillPos.x;
                bean->stillPos.y = partner->stillPos.y;
                break;
        }
    }
    else {
        bean->stillPos.x = (bean->position.x - bean->origin.x) >> 20;
        bean->stillPos.y = (bean->position.y - bean->origin.y) >> 20;
    }
}

void PuyoBean_State_PartnerControlled(void)
{
    RSDK_THIS(PuyoBean);

    RSDK.ProcessAnimation(&self->beanAnimator);
}

void PuyoBean_State_Controlled(void)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean *partner = self->partner;

    RSDK.ProcessAnimation(&self->beanAnimator);

    if (self->beanAnimator.animationID == self->type + PUYOBEAN_ANI_BOUNCE && self->beanAnimator.frameID == self->beanAnimator.frameCount - 1)
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type, &self->beanAnimator, true, 0);

    StateMachine_Run(self->stateInput);

    bool32 prevOnGround = self->onGround;
    PuyoBean_CheckCollisions();
    self->fallDelay = self->down ? 1 : PuyoBean->fallDelays[self->selectedLevel];

    if (!self->rotateSpeed) {
        self->targetAngle         = self->angle;
        self->forceRotationActive = false;

        if (self->rotateRight) {
            self->rotateSpeed = -8;

            self->targetAngle = self->angle - 0x40;
            if (self->targetAngle < 0)
                self->targetAngle += 0x100;

            RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
        }
        else if (self->rotateLeft) {
            self->targetAngle = (self->angle + 0x40) & 0xFF;
            self->rotateSpeed = 8;

            RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
        }
        else {
            if (self->forceRotateLeft) {
                self->forceRotationActive = true;

                self->rotateSpeed = -8;
                self->targetAngle = (self->angle + 0x80) & 0xFF;

                RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
            }
            else if (self->forceRotateRight) {
                self->forceRotationActive = true;

                self->rotateSpeed = 8;
                self->targetAngle = (self->angle + 0x80) & 0xFF;

                RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
            }
        }

        if (self->forceRotateLeft || self->forceRotateRight) {
            if (self->angle == 0xC0)
                self->rotationDir = 1;
            else if (self->angle == 0x40)
                self->rotationDir = -1;
        }
    }

    if (self->rotateSpeed < 0) {
        self->rotateSpeed++;
        self->angle -= 8 * (self->forceRotationActive != false) + 8;
        self->angle &= 0xFF;
    }
    else if (self->rotateSpeed > 0) {
        self->rotateSpeed--;
        self->angle += 8 * (self->forceRotationActive != false) + 8;
        self->angle &= 0xFF;
    }

    if (!self->onGround && ++self->fallTimer >= self->fallDelay) {
        self->position.y += 0x80000;
        self->fallTimer = 0;
    }

    if (!(self->moveTimer & 3)) {
        if (self->left) {
            if (self->velocity.x > 0)
                self->moveTimer = 0;

            self->velocity.x = -0x80000;
        }
        else if (self->right) {
            if (self->velocity.x < 0)
                self->moveTimer = 0;

            self->velocity.x = 0x80000;
        }
    }

    if (!self->left && !self->right && !(self->moveTimer & 3)) {
        self->moveTimer  = 0;
        self->velocity.x = 0;
    }
    else {
        if (++self->moveTimer >= 16) {
            if ((self->moveTimer & 3) == 1 || (self->moveTimer & 3) == 2)
                self->position.x += self->velocity.x;
        }
        else {
            if (self->moveTimer < 3)
                self->position.x += self->velocity.x;
        }
    }

    PuyoBean_CalculateStillPos(self);
    PuyoBean_CalculateStillPos(partner);
    PuyoBean_HandleMoveBounds();

    partner->position.x = self->position.x + (RSDK.Cos256(self->angle) << 12);
    partner->position.y = self->position.y + (RSDK.Sin256(self->angle) << 12);

    if (self->onGround) {
        if (!prevOnGround) {
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, partner->type + PUYOBEAN_ANI_BOUNCE, &partner->beanAnimator, false, 0);
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_BOUNCE, &self->beanAnimator, false, 0);

            RSDK.PlaySfx(PuyoBean->sfxLand, false, 255);
        }
        if (++self->idleTimer > 60 || self->down) {
            self->position.x    = ((self->origin.x + (self->stillPos.x << 20)) & 0xFFF00000) + 0x80000;
            self->position.y    = ((self->origin.y + (self->stillPos.y << 20)) & 0xFFF00000) + 0x80000;
            partner->position.x = ((partner->origin.x + (partner->stillPos.x << 20)) & 0xFFF00000) + 0x80000;
            partner->position.y = ((partner->origin.y + (partner->stillPos.y << 20)) & 0xFFF00000) + 0x80000;

            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, partner->type + PUYOBEAN_ANI_BOUNCE, &partner->beanAnimator, false, 0);
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_BOUNCE, &self->beanAnimator, false, 0);

            self->left        = false;
            self->right       = false;
            self->down        = false;
            self->rotateRight = false;
            self->rotateLeft  = false;
            self->popTimer    = 0;
            partner->popTimer = 0;

            partner->state = PuyoBean_State_Falling;
            self->state    = PuyoBean_State_Falling;
        }
    }
    else {
        if (self->idleTimer > 0)
            self->idleTimer++;
    }
}

void PuyoBean_State_BeanIdle(void)
{
    RSDK_THIS(PuyoBean);

    // Check if bean should be falling
    if (self->stillPos.y < (PUYO_PLAYFIELD_H - 1)) {
        int32 ny = self->stillPos.y + 1;

        if (self->stillPos.x < 0 || ny < 0 || self->stillPos.x > (PUYO_PLAYFIELD_W - 1) || ny > (PUYO_PLAYFIELD_H - 1)
            || !PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny)) {
            PuyoBean->playfield[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = NULL;

            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_BOUNCE, &self->beanAnimator, true, 0);

            self->timer        = 0;
            self->velocity.y   = 0;
            self->linkBeans[0] = NULL;
            self->linkBeans[1] = NULL;
            self->linkBeans[2] = NULL;
            self->linkBeans[3] = NULL;
            self->state        = PuyoBean_State_Falling;
            self->popTimer     = 0;
        }
    }

    if (self->state == PuyoBean_State_BeanIdle && !PuyoBean->disableBeanLink[self->playerID]) {
        // Handle Bean links & Combos
        PuyoBean_HandleBeanLinks();
        if (self->linkSides > 0)
            PuyoBean_CheckBeanLinks(self, NULL);

        if (self->linkCount <= 2) {
            self->connectTimer = 0;
        }
        else if (++self->connectTimer > 2) {
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_CONNECT, &self->beanAnimator, true, self->linkSides);

            self->timer        = 0;
            self->connectTimer = 0;
            self->state        = PuyoBean_State_BeginBeanPop;
        }
    }

    if (self->state == PuyoBean_State_BeanIdle) {
        // Handle Bean animations
        if (self->linkSides) {
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_CONNECT, &self->beanAnimator, true, self->linkSides);
        }
        else {
            if (self->timer <= 0) {
                if (self->beanAnimator.animationID - self->type == PUYOBEAN_ANI_CONNECT) {
                    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_IDLE, &self->beanAnimator, true, 0);
                }
                else {
                    RSDK.ProcessAnimation(&self->beanAnimator);

                    if (self->beanAnimator.frameID == self->beanAnimator.frameCount - 1)
                        self->timer = RSDK.Rand(120, 240);
                }
            }
            else {
                if (!--self->timer)
                    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_IDLE, &self->beanAnimator, true, 0);
            }
        }
    }
}

void PuyoBean_State_Falling(void)
{
    RSDK_THIS(PuyoBean);

    if (++self->timer > 8) {
        self->velocity.y += 0x3800;
        self->position.y += self->velocity.y;

        foreach_active(PuyoBean, bean)
        {
            if (bean != self && bean->state == PuyoBean_State_Falling && self->position.x == bean->position.x) {
                if (self->position.y + 0x100000 > bean->position.y && self->position.y < bean->position.y) {
                    self->position.y = bean->position.y - 0x100000;
                    self->velocity.y = bean->velocity.y;
                    foreach_break;
                }
            }
        }

        PuyoBean_CalculateStillPos(self);

        if (self->stillPos.y >= 0) {
            if (self->stillPos.y > (PUYO_PLAYFIELD_H - 1))
                self->stillPos.y = (PUYO_PLAYFIELD_H - 1);

            int32 y  = self->stillPos.y;
            int32 ny = y + 1;

            if (y >= (PUYO_PLAYFIELD_H - 1) || PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny)) {
                if ((self->position.y & 0xF0000) >= 0x80000) {
                    self->position.y    = (y << 20) + self->origin.y + 0x80000;
                    self->timer         = 0;
                    int32 playfieldSlot = 128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x;
                    int32 entitySlot    = 0x600 + playfieldSlot;

                    if (self->isJunk) {
                        PuyoBean->playfield[playfieldSlot] = RSDK_GET_ENTITY(entitySlot, PuyoBean);
                        self->state                        = PuyoBean_State_JunkLand;
                        if (self->velocity.y > 0x8000)
                            RSDK.PlaySfx(PuyoBean->sfxJunk, false, 255);
                    }
                    else {
                        PuyoBean->playfield[playfieldSlot] = RSDK_GET_ENTITY(entitySlot, PuyoBean);
                        self->state                        = PuyoBean_State_BeanLand;
                        if (self->velocity.y > 0x8000)
                            RSDK.PlaySfx(PuyoBean->sfxLand, false, 255);
                    }

                    self->velocity.y = 0;
                    RSDK.AddDrawListRef(self->drawGroup, entitySlot);
                    RSDK.CopyEntity(RSDK_GET_ENTITY(entitySlot, PuyoBean), self, true);
                }
            }
        }
    }
}

void PuyoBean_State_BeanLand(void)
{
    RSDK_THIS(PuyoBean);

    RSDK.ProcessAnimation(&self->beanAnimator);

    if (self->beanAnimator.frameID == self->beanAnimator.frameCount - 1) {
        EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, self->stillPos.y);
        if (bean && bean != self)
            destroyEntity(bean);

        self->timer = RSDK.Rand(120, 240);
        self->state = PuyoBean_State_BeanIdle;
    }
}

void PuyoBean_State_JunkLand(void)
{
    RSDK_THIS(PuyoBean);

    if (++self->timer == 2) {
        self->timer = 0;
        self->state = PuyoBean_State_JunkIdle;

        PuyoBean_CalculateStillPos(self);
    }
}

void PuyoBean_State_JunkIdle(void)
{
    RSDK_THIS(PuyoBean);

    bool32 popJunk = false;

    self->linkBeans[0] = NULL;
    if (self->stillPos.x > -1) {
        int32 y = self->stillPos.y - 1;
        if (self->stillPos.x >= 0 && y >= 0 && self->stillPos.x <= (PUYO_PLAYFIELD_W - 1) && y <= (PUYO_PLAYFIELD_H - 1)) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, y);

            if (bean && bean->state == PuyoBean_State_BeanPop && bean->popTimer < 2)
                popJunk = true;
        }
    }

    self->linkBeans[1] = NULL;
    if (!popJunk && self->stillPos.y < PUYO_PLAYFIELD_H) {
        int32 y = self->stillPos.y + 1;
        if (self->stillPos.x >= 0 && y >= 0 && self->stillPos.x <= (PUYO_PLAYFIELD_W - 1) && y <= (PUYO_PLAYFIELD_H - 1)) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, y);
            if (bean && bean->state == PuyoBean_State_BeanPop && bean->popTimer < 2)
                popJunk = true;
        }
    }

    self->linkBeans[2] = NULL;
    if (!popJunk && self->stillPos.x > -1) {
        int32 x = self->stillPos.x - 1;
        if (x >= 0 && self->stillPos.y >= 0 && x <= (PUYO_PLAYFIELD_W - 1) && self->stillPos.y <= (PUYO_PLAYFIELD_H - 1)) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, self->stillPos.y);
            if (bean && bean->state == PuyoBean_State_BeanPop && bean->popTimer < 2)
                popJunk = true;
        }
    }

    self->linkBeans[3] = NULL;
    if (!popJunk && self->stillPos.x < PUYO_PLAYFIELD_W) {
        int32 x = self->stillPos.x + 1;
        if (x >= 0 && self->stillPos.y >= 0 && x <= (PUYO_PLAYFIELD_W - 1) && self->stillPos.y <= (PUYO_PLAYFIELD_H - 1)) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, self->stillPos.y);
            if (bean && bean->state == PuyoBean_State_BeanPop && bean->popTimer < 2)
                popJunk = true;
        }
    }

    if (popJunk) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, PUYOBEAN_JUNK + PUYOBEAN_ANI_IDLE, &self->beanAnimator, true, 0);

        self->state = PuyoBean_State_JunkPopped;
        self->timer = 0;
    }
    else {
        if (self->stillPos.y < (PUYO_PLAYFIELD_H - 1)) {
            int32 y = self->stillPos.y + 1;
            if (self->stillPos.x < 0 || y < 0 || self->stillPos.x > (PUYO_PLAYFIELD_W - 1) || y > (PUYO_PLAYFIELD_H - 1)
                || !PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, y)) {
                PuyoBean->playfield[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = NULL;

                self->velocity.y = 0;
                self->popTimer   = 0;
                self->state      = PuyoBean_State_Falling;
                self->timer      = 0;
            }
        }
    }
}

void PuyoBean_State_JunkPopped(void)
{
    RSDK_THIS(PuyoBean);

    bool32 animationFinished = false;
    if (self->beanAnimator.frameID == self->beanAnimator.frameCount - 1) {
        animationFinished = true;
        self->visible     = false;
    }
    else {
        RSDK.ProcessAnimation(&self->beanAnimator);
    }

    if (self->timer < 26)
        self->timer++;

    if (self->timer == 26)
        PuyoBean->playfield[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = NULL;

    if (self->timer >= 26 && animationFinished)
        destroyEntity(self);
}

void PuyoBean_State_BeginBeanPop(void)
{
    RSDK_THIS(PuyoBean);

    PuyoBean_HandleBeanLinks();

    ++self->timer;
    self->visible = !(self->timer & 1);

    if (self->timer == 24) {
        self->timer    = RSDK.Rand(8, 16);
        self->popTimer = 0;
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_POP, &self->beanAnimator, true, 0);

        self->state = PuyoBean_State_BeanPop;
        RSDK.PlaySfx(PuyoBean->chainFrames[MIN(PuyoBean->comboChainCount[self->playerID], 5)], false, 255);
    }
}

void PuyoBean_State_BeanPop(void)
{
    RSDK_THIS(PuyoBean);

    if (self->popTimer >= 2)
        PuyoBean->playfield[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = NULL;
    else
        self->popTimer++;

    if (self->timer <= 0) {
        for (int32 angle = 0; angle < 0x100; angle += 0x20) {
            int32 x              = RSDK.Cos256(angle) << 10;
            int32 y              = RSDK.Sin256(angle) << 10;
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x + self->position.x, y + self->position.y);

            debris->state           = Debris_State_Fall;
            debris->gravityStrength = 0x4000;
            debris->timer           = 14;
            debris->velocity.x      = RSDK.Cos256(angle) << 9;
            debris->velocity.y      = RSDK.Sin256(angle) << 9;
            debris->drawGroup       = Zone->objectDrawGroup[1] + 1;
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + PUYOBEAN_ANI_DEBRIS, &debris->animator, true, 0);
        }

        destroyEntity(self);
    }
    else {
        self->timer--;
    }
}

void PuyoBean_State_MatchLoseFall(void)
{
    RSDK_THIS(PuyoBean);

    if (self->timer <= 0) {
        self->velocity.y += 0x3800;
        self->position.y += self->velocity.y;

        Vector2 range = { 0x800000, 0x800000 };
        if (!RSDK.CheckOnScreen(self, &range))
            destroyEntity(self);
    }
    else {
        self->velocity.y = 0;
        self->timer--;
    }
}

#if GAME_INCLUDE_EDITOR
void PuyoBean_EditorDraw(void) { RSDK.DrawSprite(&PuyoBean->junkBeanAnimator, NULL, false); }

void PuyoBean_EditorLoad(void)
{
    PuyoBean->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, PUYOBEAN_BLUE + PUYOBEAN_ANI_FLASH, &PuyoBean->junkBeanAnimator, true, 0);
}
#endif

void PuyoBean_Serialize(void) {}
