#include "SonicMania.h"

ObjectPuyoBean *PuyoBean;

void PuyoBean_Update(void)
{
    RSDK_THIS(PuyoBean);
    StateMachine_Run(self->state);
}

void PuyoBean_LateUpdate(void) {}

void PuyoBean_StaticUpdate(void)
{
    RSDK.ProcessAnimation(&PuyoBean->animator);
    if (--PuyoBean->shinkDelay <= 0) {
        PuyoBean->shinkDelay = 15 * RSDK.Rand(1, 24);
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 32, &PuyoBean->animator, true, 0);
    }
}

void PuyoBean_Draw(void)
{
    RSDK_THIS(PuyoBean);

    if (SceneInfo->state != ENGINESTATE_FROZEN) {
        if (self->flag)
            RSDK.DrawSprite(&PuyoBean->animator, NULL, false);
        else
            RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

void PuyoBean_Create(void *data)
{
    RSDK_THIS(PuyoBean);
    if (!SceneInfo->inEditor) {
        self->visible = true;
        if (PuyoGame)
            self->drawOrder = Zone->drawOrderLow;
        else
            self->drawOrder = Zone->drawOrderHigh;

        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->angle         = 0xC0;
        self->position.x    = (self->position.x & 0xFFF00000) + 0x80000;
        self->position.y    = (self->position.y & 0xFFF00000) + 0x80000;
        self->origin.x      = self->position.x - 0x280000;
        self->origin.y      = self->position.y - 0x80000;
        self->controllerID  = 1;
        self->type          = voidToInt(data);
        if (self->type == 30)
            self->flag = true;
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type, &self->animator, true, 0);
    }
}

void PuyoBean_StageLoad(void)
{
    PuyoBean->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 32, &PuyoBean->animator, true, 10);
    PuyoBean->hitbox.left    = -8;
    PuyoBean->hitbox.top     = -8;
    PuyoBean->hitbox.right   = 8;
    PuyoBean->hitbox.bottom  = 8;
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
    for (int i = 0; i < 0x100; ++i) PuyoBean->gameState[i] = NULL;
}

EntityPuyoBean *PuyoBean_GetPuyoBean(int playerID, int x, int y)
{
    EntityPuyoBean *bean = NULL;
    if (x >= 0 && y >= 0 && x <= 5 && y <= 13) {
        bean = (EntityPuyoBean *)PuyoBean->gameState[128 * playerID + 8 * y + x];
    }
    return bean;
}

void PuyoBean_StateInput_HandlePlayerInputs(void)
{
    RSDK_THIS(PuyoBean);

    if (self->controllerID < PLAYER_MAX) {
#if RETRO_USE_TOUCH_CONTROLS
        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= 0 && ty >= 96 && tx <= ScreenInfo->centerX && ty <= ScreenInfo->height) {
                    int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                    int32 ty = (TouchInfo->y[t] * ScreenInfo->height);
                    tx -= 64;
                    ty -= 192;

                    switch (((RSDK.ATan2(tx, ty) + 32) & 0xFF) >> 6) {
                        case 0:
                            ControllerInfo->keyRight.down |= true;
                            ControllerInfo[self->controllerID].keyRight.down = true;
                            break;
                        case 1:
                            ControllerInfo->keyDown.down |= true;
                            ControllerInfo[self->controllerID].keyDown.down = true;
                            break;
                        case 2:
                            ControllerInfo->keyLeft.down |= true;
                            ControllerInfo[self->controllerID].keyLeft.down = true;
                            break;
                        case 3:
                            ControllerInfo->keyUp.down |= true;
                            ControllerInfo[self->controllerID].keyUp.down = true;
                            break;
                    }
                    break;
                }
            }
        }

        int32 halfX = ScreenInfo->centerX / 2;
        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->centerX && ty >= 96 && tx <= (ScreenInfo->width - halfX) && ty <= ScreenInfo->height) {
                    ControllerInfo->keyA.down |= true;
                    ControllerInfo[self->controllerID].keyA.down = true;
                    break;
                }
                else if (tx >= (ScreenInfo->centerX + halfX) && ty >= 96 && tx <= ScreenInfo->width && ty <= ScreenInfo->height) {
                    ControllerInfo->keyB.down |= true;
                    ControllerInfo[self->controllerID].keyB.down = true;
                    break;
                }
            }
        }

        if (!self->touchLeft) {
            ControllerInfo->keyA.press |= ControllerInfo->keyB.down;
            ControllerInfo[self->controllerID].keyB.press |= ControllerInfo[self->controllerID].keyB.down;
        }
        if (!self->touchRight) {
            ControllerInfo->keyA.press |= ControllerInfo->keyA.down;
            ControllerInfo[self->controllerID].keyA.press |= ControllerInfo[self->controllerID].keyA.down;
        }
        self->touchLeft = ControllerInfo[self->controllerID].keyB.down;
        self->touchRight = ControllerInfo[self->controllerID].keyA.down;

        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->width - 0x80 && ty >= 0 && tx <= ScreenInfo->width && ty <= 0x40) {
                    if (SceneInfo->state == ENGINESTATE_REGULAR) {
                        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                        if (!pauseMenu->objectID) {
                            RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                            pauseMenu->triggerPlayer = self->playerID;
                            if (globals->gameMode == MODE_COMPETITION)
                                pauseMenu->disableRestart = true;
                        }
                    }
                    break;
                }
            }
        }
#endif

        self->down  = ControllerInfo[self->controllerID].keyDown.down;
        self->left  = ControllerInfo[self->controllerID].keyLeft.down;
        self->right = ControllerInfo[self->controllerID].keyRight.down;
        self->down |= AnalogStickInfoL[self->controllerID].vDelta < -0.3;
        self->left |= AnalogStickInfoL[self->controllerID].hDelta < -0.3;
        self->right |= AnalogStickInfoL[self->controllerID].hDelta > 0.3;
        if (self->left && self->right) {
            self->left  = false;
            self->right = false;
        }
        self->rotateLeft = ControllerInfo[self->controllerID].keyB.press || ControllerInfo[self->controllerID].keyC.press
                             || ControllerInfo[self->controllerID].keyY.press;
        self->rotateRight      = ControllerInfo[self->controllerID].keyA.press || ControllerInfo[self->controllerID].keyX.press;
        self->forceRotateLeft  = false;
        self->forceRotateRight = false;
    }
}

void PuyoBean_DestroyPuyoBeans(void)
{
    foreach_all(PuyoBean, bean)
    {
        if (bean->stateInput && bean->state == PuyoBean_State_Controlled) {
            EntityPuyoBean *partner = (EntityPuyoBean *)bean->partner;
            if (partner)
                destroyEntity(partner);
            destroyEntity(bean);
        }
    }
}

void PuyoBean_Unknown3(void)
{
    RSDK_THIS(PuyoBean);

    if (self->stillPos.x >= 0 && self->stillPos.y <= 13 && self->stillPos.x < 6) {
        self->linkFlags = 0;
        self->field_68  = 0;

        int ny               = self->stillPos.y - 1;
        self->linkBeans[0] = NULL;
        if (ny > -1) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny);
            if (bean) {
                if (bean->type == self->type) {
                    self->linkFlags |= 1;
                    self->linkBeans[0] = (Entity *)bean;
                }
            }
        }

        ny                   = self->stillPos.y + 1;
        self->linkBeans[1] = NULL;
        if (ny < 14) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny);
            if (bean) {
                if (bean->type == self->type) {
                    self->linkFlags |= 2;
                    self->linkBeans[1] = (Entity *)bean;
                }
            }
        }

        int nx               = self->stillPos.x - 1;
        self->linkBeans[2] = NULL;
        if (nx > -1) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y);
            if (bean) {
                if (bean->type == self->type) {
                    self->linkFlags |= 4;
                    self->linkBeans[2] = (Entity *)bean;
                }
            }
        }

        nx                   = self->stillPos.x + 1;
        self->linkBeans[3] = NULL;
        if (nx < 6) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y);
            if (bean) {
                if (bean->type == self->type) {
                    self->linkFlags |= 8;
                    self->linkBeans[3] = (Entity *)bean;
                }
            }
        }
    }
}

void PuyoBean_CheckBeanLinks(EntityPuyoBean *bean, EntityPuyoBean *curLink)
{
    RSDK_THIS(PuyoBean);

    Entity **linkBeans = bean->linkBeans;
    for (int i = 0; i < 4; ++i) {
        EntityPuyoBean *link = (EntityPuyoBean *)bean->linkBeans[i];
        if (link && link != curLink) {
            if (++self->field_68 > 2)
                return;
            PuyoBean_CheckBeanLinks(link, bean);
        }
        ++linkBeans;
    }
}

void PuyoBean_HandleMoveBounds(void)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean *partner = (EntityPuyoBean *)self->partner;
    int entityX             = self->stillPos.x;
    int entityY             = self->stillPos.y;
    int partnerX            = partner->stillPos.x;
    int partnerY            = partner->stillPos.y;

    EntityPuyoBean *bean  = PuyoBean_GetPuyoBean(self->playerID, entityX, entityY);
    EntityPuyoBean *bean2 = PuyoBean_GetPuyoBean(self->playerID, partnerX, partnerY);

    self->position.x -= entityX << 20;
    self->position.y -= entityY << 20;

    bool32 flag = false;

    flag = self->velocity.x < 0;
    if (self->velocity.x >= 0) {
        flag =
            ((self->angle > 0x40 && self->rotateSpeed > 0) || (self->angle < 0xC0 && self->rotateSpeed < 0)) && (self->targetAngle == 0x80);
    }
    if (entityX < 0 || partnerX < 0 || (flag && (bean || bean2))) {
        ++entityX;
        ++partnerX;
    }

    flag = self->velocity.x > 0;
    if (self->velocity.x <= 0) {
        flag =
            ((self->angle < 0x40 && self->rotateSpeed < 0) || (self->angle > 0xC0 && self->rotateSpeed > 0)) && (self->targetAngle == 0x00);
    }
    if (entityX > 5 || partnerX > 5 || (flag && (bean || bean2))) {
        --entityX;
        --partnerX;
    }

    flag = ((self->angle > 0x00 && self->rotateSpeed > 0) || (self->angle < 0x80 && self->rotateSpeed < 0)) && (self->targetAngle == 0x40);
    if (entityY > 13 || partnerY > 13 || (flag && (bean || bean2))) {
        --entityY;
        --partnerY;
    }

    self->stillPos.x  = entityX;
    self->stillPos.y  = entityY;
    partner->stillPos.x = partnerX;
    partner->stillPos.y = partnerY;
    self->position.x += self->stillPos.x << 20;
    self->position.y += self->stillPos.y << 20;
}

bool32 PuyoBean_Unknown6(EntityPuyoBean *bean)
{
    if (bean->targetAngle != 0x40 && bean->targetAngle != 0xC0)
        return 0;

    uint8 flags = 0;
    int y       = bean->stillPos.y;

    if (bean->stillPos.x >= 1) {
        int32 nx = bean->stillPos.x - 1;
        if (PuyoBean_GetPuyoBean(bean->playerID, nx, y))
            flags = 1;
    }
    else {
        flags = 1;
    }

    if (bean->stillPos.x <= 4) {
        int32 nx = bean->stillPos.x + 1;
        if (PuyoBean_GetPuyoBean(bean->playerID, nx, y))
            flags |= 2;
    }
    else {
        flags |= 2;
    }

    return flags == 3;
}

void PuyoBean_CheckCollisions(void)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean *partner = (EntityPuyoBean *)self->partner;
    uint8 flags             = 0;

    if (self->position.x - self->origin.x != self->stillPos.x << 20) {
        int32 nx = self->stillPos.x - 1;
        if (self->stillPos.x < 1 || PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y)) {
            self->left = false;
            flags        = 1;
        }

        nx = self->stillPos.x + 1;
        if (self->stillPos.x > 4 || PuyoBean_GetPuyoBean(self->playerID, nx, self->stillPos.y)) {
            self->right = false;
            flags |= 2;
        }

        if (flags == 3) {
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

    self->onGround  = false;
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

int PuyoBean_Unknown8(EntityPuyoBean *bean, int playerID, int x, int y)
{
    PuyoBean->field_C34[6 * y + x] = true;

    int count1 = 0;
    int count2 = 0;
    for (int i = 0; i < 4; ++i) {
        int nx = x;
        int ny = y;
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
                PuyoBean_Unknown11(false, playerID, nx, ny);
                count1 += PuyoBean->field_430;
            }
        }

        for (int b = 0; b < PuyoBean->field_430; ++b) {
            Vector2 positions[4];
            positions[0].x = PuyoBean->field_434[b].x - 1;
            positions[0].y = PuyoBean->field_434[b].y;
            positions[1].x = PuyoBean->field_434[b].x + 1;
            positions[1].y = PuyoBean->field_434[b].y;
            positions[2].x = PuyoBean->field_434[b].x;
            positions[2].y = PuyoBean->field_434[b].y - 1;
            positions[3].x = PuyoBean->field_434[b].x;
            positions[3].y = PuyoBean->field_434[b].y + 1;

            for (int x = 0; x < 4; ++x) {
                int bx = positions[i].x;
                int by = positions[i].y;

                beanState = PuyoBean_GetPuyoBean(playerID, bx, by);
                if (beanState && !PuyoBean->field_C34[bx + 6 * by]) {
                    if (beanState->flag) {
                        PuyoBean->field_C34[bx + 6 * by] = true;
                        ++count2;
                    }
                }
            }
        }
    }

    return count2 + count1 + 1;
}

int PuyoBean_Unknown9(EntityPuyoBean *bean, int playerID, int x, int y)
{
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        int nx = x;
        int ny = y;
        switch (i) {
            case 0: nx = x - 1; break;
            case 1: nx = x + 1; break;
            case 2: ny = y - 1; break;
            case 3: ny = y + 1; break;
            default: break;
        }

        EntityPuyoBean *beanState = PuyoBean_GetPuyoBean(playerID, nx, ny);
        if (beanState && beanState->type != bean->type) {
            PuyoBean_Unknown11(true, playerID, nx, ny);
            if (PuyoBean->field_430 >= 3) {
                if (!PuyoBean_Unknown10(playerID, x, y))
                    count++;
            }
        }
    }

    return count;
}

bool32 PuyoBean_Unknown10(int playerID, int x, int y)
{
    bool32 flagBuffer[0x101];
    memset(flagBuffer, 0, sizeof(flagBuffer));

    for (int b = 0; b < PuyoBean->field_430; ++b) {
        Vector2 positions[4];
        positions[0].x = PuyoBean->field_434[b].x - 1;
        positions[0].y = PuyoBean->field_434[b].y;
        positions[1].x = PuyoBean->field_434[b].x + 1;
        positions[1].y = PuyoBean->field_434[b].y;
        positions[2].x = PuyoBean->field_434[b].x;
        positions[2].y = PuyoBean->field_434[b].y - 1;
        positions[3].x = PuyoBean->field_434[b].x;
        positions[3].y = PuyoBean->field_434[b].y + 1;

        for (int i = 0; i < 4; ++i) {
            int bx = positions[i].x;
            int by = positions[i].y;

            if (!flagBuffer[bx + 6 * by]) {
                flagBuffer[bx + 6 * by] = true;
                if (bx != x || by != y) {
                    EntityPuyoBean *beanState = PuyoBean_GetPuyoBean(playerID, bx, by);
                    if (bx >= 0 && by >= 0 && bx <= 5 && by <= 13 && !beanState)
                        return true;
                }
            }
        }
    }

    return false;
}

void PuyoBean_Unknown11(bool32 useTempArray, int playerID, int x, int y)
{
    bool32 flagBuffer[0x101];
    memset(flagBuffer, 0, sizeof(flagBuffer));

    bool32 *flagBufferPtr = flagBuffer;
    if (!useTempArray)
        flagBufferPtr = PuyoBean->field_C34;

    if (!flagBufferPtr[x + 6 * y]) {
        for (int i = 0; i < 84; ++i) {
            PuyoBean->field_434[i].x = 0;
            PuyoBean->field_434[i].y = 0;
        }

        PuyoBean->field_430 = 0;
        if (x >= 0 && y >= 0 && x <= 5 && y <= 13) {
            EntityPuyoBean *beanState = PuyoBean_GetPuyoBean(playerID, x, y);
            if (beanState) {
                PuyoBean->field_434[0].x = x;
                PuyoBean->field_434[0].y = y;
                int id                   = 1;

                for (int p = 0; p < id; ++p) {
                    if (id >= 0xFF) {
                        PuyoBean->field_430 = id;
                        break;
                    }

                    int bx = PuyoBean->field_434[id].x;
                    int by = PuyoBean->field_434[id].y;

                    EntityPuyoBean *startBean = NULL, *curBean = NULL;
                    startBean = PuyoBean_GetPuyoBean(playerID, bx, by);

                    flagBufferPtr[bx + 6 * by] = true;
                    if (startBean) {
                        int32 nx = bx - 1;
                        if (nx >= 0) {
                            if (!flagBufferPtr[nx + 6 * by] && by >= 0 && nx <= 5 && by <= 13) {
                                curBean = PuyoBean_GetPuyoBean(playerID, nx, by);
                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->field_434[id].x   = nx;
                                    PuyoBean->field_434[id++].y = by;
                                }
                            }
                        }

                        nx = bx + 1;
                        if (nx <= 5) {
                            if (!flagBufferPtr[nx + 6 * by] && nx >= 0 && by >= 0 && by <= 13) {
                                curBean = PuyoBean_GetPuyoBean(playerID, nx, by);
                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->field_434[id].x   = nx;
                                    PuyoBean->field_434[id++].y = by;
                                }
                            }
                        }

                        int32 ny = by - 1;
                        if (ny >= 0) {
                            if (!flagBufferPtr[bx + 6 * ny] && bx >= 0 && bx <= 5 && ny <= 13) {
                                curBean = PuyoBean_GetPuyoBean(playerID, bx, ny);
                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->field_434[id].x   = bx;
                                    PuyoBean->field_434[id++].y = ny;
                                }
                            }
                        }

                        ny = by + 1;
                        if (ny <= 13) {
                            if (!flagBufferPtr[bx + 6 * ny] && bx >= 0 && ny >= 0 && bx <= 5) {
                                curBean = PuyoBean_GetPuyoBean(playerID, bx, ny);
                                if (curBean && beanState->type == curBean->type) {
                                    PuyoBean->field_434[id].x   = bx;
                                    PuyoBean->field_434[id++].y = ny;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

uint8 PuyoBean_Unknown12(int x, int playerID, EntityPuyoBean *bean1, EntityPuyoBean *bean2)
{
    if (x < 0 || x > 5)
        return 13;
    int row = 0;
    for (int y = 13; y >= 0; --y) {
        if (bean1 && x == bean1->stillPos.x && y == bean1->stillPos.y)
            break;
        if (bean2 && x == bean2->stillPos.x && y == bean2->stillPos.y)
            break;
        if (!PuyoBean_GetPuyoBean(playerID, x, y))
            break;
        ++row;
    }

    return row;
}

void PuyoBean_CalculateStillPos(EntityPuyoBean *bean)
{
    if (bean->state == PuyoBean_State_Idle) {
        EntityPuyoBean *partner = (EntityPuyoBean *)bean->partner;
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

void PuyoBean_State_Idle(void)
{
    RSDK_THIS(PuyoBean);
    RSDK.ProcessAnimation(&self->animator);
}

void PuyoBean_State_Controlled(void)
{
    RSDK_THIS(PuyoBean);
    EntityPuyoBean *partner = (EntityPuyoBean *)self->partner;

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.animationID == self->type + 2 && self->animator.frameID == self->animator.frameCount - 1)
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type, &self->animator, true, 0);

    StateMachine_Run(self->stateInput);

    bool32 prevOnGround = self->onGround;
    PuyoBean_CheckCollisions();
    if (self->down)
        self->fallDelay = 1;
    else
        self->fallDelay = PuyoBean->fallDelays[self->field_A4];

    if (!self->rotateSpeed) {
        self->targetAngle       = self->angle;
        self->forceRotationFlag = false;
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
                self->forceRotationFlag = true;
                self->rotateSpeed       = -8;
                self->targetAngle       = (self->angle + 0x80) & 0xFF;
                RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
            }
            else if (self->forceRotateRight) {
                self->forceRotationFlag = true;
                self->rotateSpeed       = 8;
                self->targetAngle       = (self->angle + 0x80) & 0xFF;
                RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
            }
        }

        if (self->forceRotateLeft || self->forceRotateRight) {
            if (self->angle == 0xC0) {
                self->field_98 = 1;
            }
            else if (self->angle == 0x40) {
                self->field_98 = -1;
            }
        }
    }

    if (self->rotateSpeed < 0) {
        self->rotateSpeed++;
        self->angle -= 8 * (self->forceRotationFlag != 0) + 8;
        self->angle &= 0xFF;
    }
    else if (self->rotateSpeed > 0) {
        self->rotateSpeed--;
        self->angle += 8 * (self->forceRotationFlag != 0) + 8;
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
        else if (self->moveTimer < 3)
            self->position.x += self->velocity.x;
    }

    PuyoBean_CalculateStillPos(self);
    PuyoBean_CalculateStillPos(partner);
    PuyoBean_HandleMoveBounds();
    partner->position.x = RSDK.Cos256(self->angle) << 12;
    partner->position.y = RSDK.Sin256(self->angle) << 12;
    partner->position.x += self->position.x;
    partner->position.y += self->position.y;
    if (self->onGround) {
        if (!prevOnGround) {
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, partner->type + 2, &partner->animator, false, 0);
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 2, &self->animator, false, 0);
            RSDK.PlaySfx(PuyoBean->sfxLand, false, 255);
        }
        if (++self->idleTimer > 60 || self->down) {
            self->position.x  = ((self->origin.x + (self->stillPos.x << 20)) & 0xFFF00000) + 0x80000;
            self->position.y  = ((self->origin.y + (self->stillPos.y << 20)) & 0xFFF00000) + 0x80000;
            partner->position.x = ((partner->origin.x + (partner->stillPos.x << 20)) & 0xFFF00000) + 0x80000;
            partner->position.y = ((partner->origin.y + (partner->stillPos.y << 20)) & 0xFFF00000) + 0x80000;
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, partner->type + 2, &partner->animator, false, 0);
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 2, &self->animator, false, 0);
            self->left        = false;
            self->right       = false;
            self->down        = false;
            self->rotateRight = false;
            self->rotateLeft  = false;
            self->field_80    = 0;
            partner->field_80   = 0;
            partner->state      = PuyoBean_State_Falling;
            self->state       = PuyoBean_State_Falling;
        }
    }
    else {
        if (self->idleTimer > 0)
            self->idleTimer++;
    }
}

void PuyoBean_State_GameState(void)
{
    RSDK_THIS(PuyoBean);

    if (self->stillPos.y < 13) {
        int32 ny = self->stillPos.y + 1;
        if (self->stillPos.x < 0 || ny < 0 || self->stillPos.x > 5 || ny > 13 || !PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny)) {
            PuyoBean->gameState[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = NULL;
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 2, &self->animator, true, 0);
            self->timer        = 0;
            self->velocity.y   = 0;
            self->linkBeans[0] = NULL;
            self->linkBeans[1] = NULL;
            self->linkBeans[2] = NULL;
            self->linkBeans[3] = NULL;
            self->state        = PuyoBean_State_Falling;
            self->field_80     = 0;
            return;
        }
    }
    if (!PuyoBean->field_28[self->playerID]) {
        PuyoBean_Unknown3();
        if (self->linkFlags > 0)
            PuyoBean_CheckBeanLinks(self, NULL);

        if (self->field_68 <= 2) {
            self->field_A0 = 0;
        }
        else if (++self->field_A0 > 2) {
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 3, &self->animator, true, self->linkFlags);
            self->timer    = 0;
            self->field_A0 = 0;
            self->state    = PuyoBean_Unknown22;
            return;
        }
    }

    if (self->linkFlags) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 3, &self->animator, true, self->linkFlags);
    }
    else {
        if (self->timer <= 0) {
            if (self->animator.animationID - self->type == 3) {
                RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 1, &self->animator, true, 0);
            }
            else {
                RSDK.ProcessAnimation(&self->animator);
                if (self->animator.frameID == self->animator.frameCount - 1) {
                    self->timer = RSDK.Rand(120, 240);
                }
            }
        }
        else {
            if (!--self->timer)
                RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 1, &self->animator, true, 0);
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
            if (self->stillPos.y > 13)
                self->stillPos.y = 13;
            int32 y  = self->stillPos.y;
            int32 ny = y + 1;

            if (y >= 13 || PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, ny)) {
                if ((self->position.y & 0xF0000) >= 0x80000) {
                    self->position.y = (y << 20) + self->origin.y + 0x80000;
                    self->timer      = 0;
                    int slot           = 128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x;
                    int slot2          = slot + 0x600;
                    if (self->flag) {
                        PuyoBean->gameState[slot] = RSDK.GetEntityByID(slot2);
                        self->state             = PuyoBean_Unknown19;
                        if (self->velocity.y > 0x8000) {
                            RSDK.PlaySfx(PuyoBean->sfxJunk, false, 255);
                        }
                    }
                    else {
                        PuyoBean->gameState[slot] = RSDK.GetEntityByID(slot2);
                        self->state             = PuyoBean_Unknown18;
                        if (self->velocity.y > 0x8000) {
                            RSDK.PlaySfx(PuyoBean->sfxLand, false, 255);
                        }
                    }

                    self->velocity.y = 0;
                    RSDK.AddDrawListRef(self->drawOrder, slot2);
                    RSDK.CopyEntity(RSDK_GET_ENTITY(slot2, PuyoBean), self, true);
                }
            }
        }
    }
}

void PuyoBean_Unknown18(void)
{
    RSDK_THIS(PuyoBean);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, self->stillPos.y);
        if (bean) {
            if (bean != self)
                destroyEntity(bean);
        }
        self->timer = RSDK.Rand(120, 240);
        self->state = PuyoBean_State_GameState;
    }
}

void PuyoBean_Unknown19(void)
{
    RSDK_THIS(PuyoBean);

    if (++self->timer == 2) {
        self->timer = 0;
        self->state = PuyoBean_Unknown20;
        PuyoBean_CalculateStillPos(self);
    }
}

void PuyoBean_Unknown20(void)
{
    RSDK_THIS(PuyoBean);

    bool32 flag = false;

    self->linkBeans[0] = NULL;
    if (self->stillPos.x > -1) {
        int y = self->stillPos.y - 1;
        if (self->stillPos.x >= 0 && y >= 0 && self->stillPos.x <= 5 && y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    self->linkBeans[1] = NULL;
    if (!flag && self->stillPos.y < 14) {
        int y = self->stillPos.y + 1;
        if (self->stillPos.x >= 0 && y >= 0 && self->stillPos.x <= 5 && y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    self->linkBeans[2] = NULL;
    if (!flag && self->stillPos.x > -1) {
        int x = self->stillPos.x - 1;
        if (x >= 0 && self->stillPos.y >= 0 && x <= 5 && self->stillPos.y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, self->stillPos.y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    self->linkBeans[3] = NULL;
    if (!flag && self->stillPos.x < 6) {
        int x = self->stillPos.x + 1;
        if (x >= 0 && self->stillPos.y >= 0 && x <= 5 && self->stillPos.y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, self->stillPos.y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    if (flag) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 31, &self->animator, true, 0);
        self->state = PuyoBean_Unknown21;
        self->timer = 0;
    }
    else {
        if (self->stillPos.y < 13) {
            int y = self->stillPos.y + 1;
            if (self->stillPos.x < 0 || y < 0 || self->stillPos.x > 5 || y > 13
                || !PuyoBean_GetPuyoBean(self->playerID, self->stillPos.x, y)) {
                PuyoBean->gameState[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = NULL;
                self->velocity.y                                                                        = 0;
                self->field_80                                                                          = 0;
                self->state                                                                             = PuyoBean_State_Falling;
                self->timer                                                                             = 0;
            }
        }
    }
}

void PuyoBean_Unknown21(void)
{
    RSDK_THIS(PuyoBean);

    bool32 flag = false;
    if (self->animator.frameID == self->animator.frameCount - 1) {
        flag            = true;
        self->visible = false;
    }
    else {
        RSDK.ProcessAnimation(&self->animator);
    }

    if (self->timer < 26)
        self->timer++;

    if (self->timer == 26) {
        PuyoBean->gameState[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = 0;
    }
    if (self->timer >= 26) {
        if (flag)
            destroyEntity(self);
    }
}

void PuyoBean_Unknown22(void)
{
    RSDK_THIS(PuyoBean);

    PuyoBean_Unknown3();
    ++self->timer;
    self->visible = !(self->timer & 1);
    if (self->timer == 24) {
        self->timer    = RSDK.Rand(8, 16);
        self->field_80 = 0;
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 4, &self->animator, true, 0);
        self->state = PuyoBean_Unknown23;
        RSDK.PlaySfx(PuyoBean->chainFrames[minVal(PuyoBean->field_20[self->playerID], 5)], false, 255);
    }
}

void PuyoBean_Unknown23(void)
{
    RSDK_THIS(PuyoBean);

    if (self->field_80 >= 2)
        PuyoBean->gameState[128 * self->playerID + 8 * self->stillPos.y + self->stillPos.x] = NULL;
    else
        self->field_80++;

    if (self->timer <= 0) {
        for (int angle = 0; angle < 0x100; angle += 0x20) {
            int x                = RSDK.Cos256(angle) << 10;
            int y                = RSDK.Sin256(angle) << 10;
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x + self->position.x, y + self->position.y);
            debris->state        = Debris_State_Fall;
            debris->gravity      = 0x4000;
            debris->timer        = 14;
            debris->velocity.x   = RSDK.Cos256(angle) << 9;
            debris->velocity.y   = RSDK.Sin256(angle) << 9;
            debris->drawOrder    = Zone->drawOrderHigh + 1;
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, self->type + 5, &debris->animator, true, 0);
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

        Vector2 range;
        range.x = 0x800000;
        range.y = 0x800000;
        if (!RSDK.CheckOnScreen(self, &range))
            destroyEntity(self);
    }
    else {
        self->velocity.y = 0;
        self->timer--;
    }
}

#if RETRO_INCLUDE_EDITOR
void PuyoBean_EditorDraw(void)
{
    RSDK.DrawSprite(&PuyoBean->animator, NULL, false);
}

void PuyoBean_EditorLoad(void)
{
    PuyoBean->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 0, &PuyoBean->animator, true, 0);
}
#endif

void PuyoBean_Serialize(void) {}
