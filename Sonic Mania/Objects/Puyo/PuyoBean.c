#include "SonicMania.h"

ObjectPuyoBean *PuyoBean;

void PuyoBean_Update(void)
{
    RSDK_THIS(PuyoBean);
    StateMachine_Run(entity->state);
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
        if (entity->flag)
            RSDK.DrawSprite(&PuyoBean->animator, NULL, false);
        else
            RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

void PuyoBean_Create(void *data)
{
    RSDK_THIS(PuyoBean);
    if (!SceneInfo->inEditor) {
        entity->visible = true;
        if (PuyoGame)
            entity->drawOrder = Zone->drawOrderLow;
        else
            entity->drawOrder = Zone->drawOrderHigh;

        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->angle         = 0xC0;
        entity->position.x    = (entity->position.x & 0xFFF00000) + 0x80000;
        entity->position.y    = (entity->position.y & 0xFFF00000) + 0x80000;
        entity->origin.x      = entity->position.x - 0x280000;
        entity->origin.y      = entity->position.y - 0x80000;
        entity->controllerID  = 1;
        entity->type          = voidToInt(data);
        if (entity->type == 30)
            entity->flag = true;
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type, &entity->animator, true, 0);
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
    PuyoBean->sfxLand        = RSDK.GetSFX("Puyo/Land.wav");
    PuyoBean->sfxRotate      = RSDK.GetSFX("Puyo/Rotate.wav");
    PuyoBean->sfxJunk        = RSDK.GetSFX("Puyo/Junk.wav");
    PuyoBean->sfxFall        = RSDK.GetSFX("Puyo/Fall.wav");
    PuyoBean->chainFrames[0] = RSDK.GetSFX("Puyo/Chain0.wav");
    PuyoBean->chainFrames[1] = RSDK.GetSFX("Puyo/Chain1.wav");
    PuyoBean->chainFrames[2] = RSDK.GetSFX("Puyo/Chain2.wav");
    PuyoBean->chainFrames[3] = RSDK.GetSFX("Puyo/Chain3.wav");
    PuyoBean->chainFrames[4] = RSDK.GetSFX("Puyo/Chain4.wav");
    PuyoBean->chainFrames[5] = RSDK.GetSFX("Puyo/Chain5.wav");
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

    if (entity->controllerID < PLAYER_MAX) {
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
                            ControllerInfo[entity->controllerID].keyRight.down = true;
                            break;
                        case 1:
                            ControllerInfo->keyDown.down |= true;
                            ControllerInfo[entity->controllerID].keyDown.down = true;
                            break;
                        case 2:
                            ControllerInfo->keyLeft.down |= true;
                            ControllerInfo[entity->controllerID].keyLeft.down = true;
                            break;
                        case 3:
                            ControllerInfo->keyUp.down |= true;
                            ControllerInfo[entity->controllerID].keyUp.down = true;
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
                    ControllerInfo[entity->controllerID].keyA.down = true;
                    break;
                }
                else if (tx >= (ScreenInfo->centerX + halfX) && ty >= 96 && tx <= ScreenInfo->width && ty <= ScreenInfo->height) {
                    ControllerInfo->keyB.down |= true;
                    ControllerInfo[entity->controllerID].keyB.down = true;
                    break;
                }
            }
        }

        if (!entity->touchLeft) {
            ControllerInfo->keyA.press |= ControllerInfo->keyB.down;
            ControllerInfo[entity->controllerID].keyB.press |= ControllerInfo[entity->controllerID].keyB.down;
        }
        if (!entity->touchRight) {
            ControllerInfo->keyA.press |= ControllerInfo->keyA.down;
            ControllerInfo[entity->controllerID].keyA.press |= ControllerInfo[entity->controllerID].keyA.down;
        }
        entity->touchLeft = ControllerInfo[entity->controllerID].keyB.down;
        entity->touchRight = ControllerInfo[entity->controllerID].keyA.down;

        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->width - 0x80 && ty >= 0 && tx <= ScreenInfo->width && ty <= 0x40) {
                    if (SceneInfo->state == ENGINESTATE_REGULAR) {
                        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                        if (!pauseMenu->objectID) {
                            RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                            pauseMenu->triggerPlayer = entity->playerID;
                            if (globals->gameMode == MODE_COMPETITION)
                                pauseMenu->disableRestart = true;
                        }
                    }
                    break;
                }
            }
        }
#endif

        entity->down  = ControllerInfo[entity->controllerID].keyDown.down;
        entity->left  = ControllerInfo[entity->controllerID].keyLeft.down;
        entity->right = ControllerInfo[entity->controllerID].keyRight.down;
        entity->down |= AnalogStickInfoL[entity->controllerID].vDelta < -0.3;
        entity->left |= AnalogStickInfoL[entity->controllerID].hDelta < -0.3;
        entity->right |= AnalogStickInfoL[entity->controllerID].hDelta > 0.3;
        if (entity->left && entity->right) {
            entity->left  = false;
            entity->right = false;
        }
        entity->rotateLeft = ControllerInfo[entity->controllerID].keyB.press || ControllerInfo[entity->controllerID].keyC.press
                             || ControllerInfo[entity->controllerID].keyY.press;
        entity->rotateRight      = ControllerInfo[entity->controllerID].keyA.press || ControllerInfo[entity->controllerID].keyX.press;
        entity->forceRotateLeft  = false;
        entity->forceRotateRight = false;
    }
}

void PuyoBean_Unknown2(void)
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

    if (entity->stillPos.x >= 0 && entity->stillPos.y <= 13 && entity->stillPos.x < 6) {
        entity->linkFlags = 0;
        entity->field_68  = 0;

        int ny               = entity->stillPos.y - 1;
        entity->linkBeans[0] = NULL;
        if (ny > -1) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, ny);
            if (bean) {
                if (bean->type == entity->type) {
                    entity->linkFlags |= 1;
                    entity->linkBeans[0] = (Entity *)bean;
                }
            }
        }

        ny                   = entity->stillPos.y + 1;
        entity->linkBeans[1] = NULL;
        if (ny < 14) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, ny);
            if (bean) {
                if (bean->type == entity->type) {
                    entity->linkFlags |= 2;
                    entity->linkBeans[1] = (Entity *)bean;
                }
            }
        }

        int nx               = entity->stillPos.x - 1;
        entity->linkBeans[2] = NULL;
        if (nx > -1) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, nx, entity->stillPos.y);
            if (bean) {
                if (bean->type == entity->type) {
                    entity->linkFlags |= 4;
                    entity->linkBeans[2] = (Entity *)bean;
                }
            }
        }

        nx                   = entity->stillPos.x + 1;
        entity->linkBeans[3] = NULL;
        if (nx < 6) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, nx, entity->stillPos.y);
            if (bean) {
                if (bean->type == entity->type) {
                    entity->linkFlags |= 8;
                    entity->linkBeans[3] = (Entity *)bean;
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
            if (++entity->field_68 > 2)
                return;
            PuyoBean_CheckBeanLinks(link, bean);
        }
        ++linkBeans;
    }
}

void PuyoBean_HandleMoveBounds(void)
{
    RSDK_THIS(PuyoBean);

    EntityPuyoBean *partner = (EntityPuyoBean *)entity->partner;
    int entityX             = entity->stillPos.x;
    int entityY             = entity->stillPos.y;
    int partnerX            = partner->stillPos.x;
    int partnerY            = partner->stillPos.y;

    EntityPuyoBean *bean  = PuyoBean_GetPuyoBean(entity->playerID, entityX, entityY);
    EntityPuyoBean *bean2 = PuyoBean_GetPuyoBean(entity->playerID, partnerX, partnerY);

    entity->position.x -= entityX << 20;
    entity->position.y -= entityY << 20;

    bool32 flag = false;

    flag = entity->velocity.x < 0;
    if (entity->velocity.x >= 0) {
        flag =
            ((entity->angle > 0x40 && entity->rotateSpeed > 0) || (entity->angle < 0xC0 && entity->rotateSpeed < 0)) && (entity->targetAngle == 0x80);
    }
    if (entityX < 0 || partnerX < 0 || (flag && (bean || bean2))) {
        ++entityX;
        ++partnerX;
    }

    flag = entity->velocity.x > 0;
    if (entity->velocity.x <= 0) {
        flag =
            ((entity->angle < 0x40 && entity->rotateSpeed < 0) || (entity->angle > 0xC0 && entity->rotateSpeed > 0)) && (entity->targetAngle == 0x00);
    }
    if (entityX > 5 || partnerX > 5 || (flag && (bean || bean2))) {
        --entityX;
        --partnerX;
    }

    flag = ((entity->angle > 0x00 && entity->rotateSpeed > 0) || (entity->angle < 0x80 && entity->rotateSpeed < 0)) && (entity->targetAngle == 0x40);
    if (entityY > 13 || partnerY > 13 || (flag && (bean || bean2))) {
        --entityY;
        --partnerY;
    }

    entity->stillPos.x  = entityX;
    entity->stillPos.y  = entityY;
    partner->stillPos.x = partnerX;
    partner->stillPos.y = partnerY;
    entity->position.x += entity->stillPos.x << 20;
    entity->position.y += entity->stillPos.y << 20;
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

    EntityPuyoBean *partner = (EntityPuyoBean *)entity->partner;
    uint8 flags             = 0;

    if (entity->position.x - entity->origin.x != entity->stillPos.x << 20) {
        int32 nx = entity->stillPos.x - 1;
        if (entity->stillPos.x < 1 || PuyoBean_GetPuyoBean(entity->playerID, nx, entity->stillPos.y)) {
            entity->left = false;
            flags        = 1;
        }

        nx = entity->stillPos.x + 1;
        if (entity->stillPos.x > 4 || PuyoBean_GetPuyoBean(entity->playerID, nx, entity->stillPos.y)) {
            entity->right = false;
            flags |= 2;
        }

        if (flags == 3) {
            if (entity->angle == 0x40 || entity->angle == 0xC0) {
                if (entity->rotateRight)
                    entity->forceRotateLeft = true;
                else if (entity->rotateLeft)
                    entity->forceRotateRight = true;
            }
            entity->rotateRight = false;
            entity->rotateLeft  = false;
        }
    }

    if (partner->position.x - partner->origin.x != partner->stillPos.x << 20) {
        if (partner->stillPos.x >= 1) {
            int32 nx = partner->stillPos.x - 1;
            if (PuyoBean_GetPuyoBean(entity->playerID, nx, partner->stillPos.y)) {
                entity->left = false;
            }
        }
        else {
            entity->left = false;
        }

        int32 nx = partner->stillPos.x + 1;
        if (partner->stillPos.x > 4 || PuyoBean_GetPuyoBean(entity->playerID, nx, partner->stillPos.y)) {
            entity->right = false;
        }
    }

    entity->onGround  = false;
    partner->onGround = false;

    if (entity->position.y & 0xF0000) {
        int32 ny = entity->stillPos.y + 1;
        if (entity->stillPos.y == 13 || PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, ny)) {
            entity->onGround = true;
        }
    }

    if (partner->position.y & 0xF0000) {
        int32 ny = partner->stillPos.y + 1;
        if (partner->stillPos.y == 13 || PuyoBean_GetPuyoBean(entity->playerID, partner->stillPos.x, ny)) {
            partner->onGround = true;
        }
    }

    entity->onGround |= partner->onGround;
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
    RSDK.ProcessAnimation(&entity->animator);
}

void PuyoBean_State_Controlled(void)
{
    RSDK_THIS(PuyoBean);
    EntityPuyoBean *partner = (EntityPuyoBean *)entity->partner;

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.animationID == entity->type + 2 && entity->animator.frameID == entity->animator.frameCount - 1)
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type, &entity->animator, true, 0);

    StateMachine_Run(entity->stateInput);

    bool32 prevOnGround = entity->onGround;
    PuyoBean_CheckCollisions();
    if (entity->down)
        entity->fallDelay = 1;
    else
        entity->fallDelay = PuyoBean->fallDelays[entity->field_A4];

    if (!entity->rotateSpeed) {
        entity->targetAngle       = entity->angle;
        entity->forceRotationFlag = false;
        if (entity->rotateRight) {
            entity->rotateSpeed = -8;
            entity->targetAngle = entity->angle - 0x40;
            if (entity->targetAngle < 0) 
                entity->targetAngle += 0x100;
            RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
        }
        else if (entity->rotateLeft) {
            entity->targetAngle = (entity->angle + 0x40) & 0xFF;
            entity->rotateSpeed = 8;
            RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
        }
        else {
            if (entity->forceRotateLeft) {
                entity->forceRotationFlag = true;
                entity->rotateSpeed       = -8;
                entity->targetAngle       = (entity->angle + 0x80) & 0xFF;
                RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
            }
            else if (entity->forceRotateRight) {
                entity->forceRotationFlag = true;
                entity->rotateSpeed       = 8;
                entity->targetAngle       = (entity->angle + 0x80) & 0xFF;
                RSDK.PlaySfx(PuyoBean->sfxRotate, false, 255);
            }
        }

        if (entity->forceRotateLeft || entity->forceRotateRight) {
            if (entity->angle == 0xC0) {
                entity->field_98 = 1;
            }
            else if (entity->angle == 0x40) {
                entity->field_98 = -1;
            }
        }
    }

    if (entity->rotateSpeed < 0) {
        entity->rotateSpeed++;
        entity->angle -= 8 * (entity->forceRotationFlag != 0) + 8;
        entity->angle &= 0xFF;
    }
    else if (entity->rotateSpeed > 0) {
        entity->rotateSpeed--;
        entity->angle += 8 * (entity->forceRotationFlag != 0) + 8;
        entity->angle &= 0xFF;
    }

    if (!entity->onGround && ++entity->fallTimer >= entity->fallDelay) {
        entity->position.y += 0x80000;
        entity->fallTimer = 0;
    }

    if (!(entity->moveTimer & 3)) {
        if (entity->left) {
            if (entity->velocity.x > 0)
                entity->moveTimer = 0;
            entity->velocity.x = -0x80000;
        }
        else if (entity->right) {
            if (entity->velocity.x < 0)
                entity->moveTimer = 0;
            entity->velocity.x = 0x80000;
        }
    }

    if (!entity->left && !entity->right && !(entity->moveTimer & 3)) {
        entity->moveTimer  = 0;
        entity->velocity.x = 0;
    }
    else {
        if (++entity->moveTimer >= 16) {
            if ((entity->moveTimer & 3) == 1 || (entity->moveTimer & 3) == 2)
                entity->position.x += entity->velocity.x;
        }
        else if (entity->moveTimer < 3)
            entity->position.x += entity->velocity.x;
    }

    PuyoBean_CalculateStillPos(entity);
    PuyoBean_CalculateStillPos(partner);
    PuyoBean_HandleMoveBounds();
    partner->position.x = RSDK.Cos256(entity->angle) << 12;
    partner->position.y = RSDK.Sin256(entity->angle) << 12;
    partner->position.x += entity->position.x;
    partner->position.y += entity->position.y;
    if (entity->onGround) {
        if (!prevOnGround) {
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, partner->type + 2, &partner->animator, false, 0);
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 2, &entity->animator, false, 0);
            RSDK.PlaySfx(PuyoBean->sfxLand, false, 255);
        }
        if (++entity->idleTimer > 60 || entity->down) {
            entity->position.x  = ((entity->origin.x + (entity->stillPos.x << 20)) & 0xFFF00000) + 0x80000;
            entity->position.y  = ((entity->origin.y + (entity->stillPos.y << 20)) & 0xFFF00000) + 0x80000;
            partner->position.x = ((partner->origin.x + (partner->stillPos.x << 20)) & 0xFFF00000) + 0x80000;
            partner->position.y = ((partner->origin.y + (partner->stillPos.y << 20)) & 0xFFF00000) + 0x80000;
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, partner->type + 2, &partner->animator, false, 0);
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 2, &entity->animator, false, 0);
            entity->left        = false;
            entity->right       = false;
            entity->down        = false;
            entity->rotateRight = false;
            entity->rotateLeft  = false;
            entity->field_80    = 0;
            partner->field_80   = 0;
            partner->state      = PuyoBean_State_Falling;
            entity->state       = PuyoBean_State_Falling;
        }
    }
    else {
        if (entity->idleTimer > 0)
            entity->idleTimer++;
    }
}

void PuyoBean_State_GameState(void)
{
    RSDK_THIS(PuyoBean);

    if (entity->stillPos.y < 13) {
        int32 ny = entity->stillPos.y + 1;
        if (entity->stillPos.x < 0 || ny < 0 || entity->stillPos.x > 5 || ny > 13 || !PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, ny)) {
            PuyoBean->gameState[128 * entity->playerID + 8 * entity->stillPos.y + entity->stillPos.x] = NULL;
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 2, &entity->animator, true, 0);
            entity->timer        = 0;
            entity->velocity.y   = 0;
            entity->linkBeans[0] = NULL;
            entity->linkBeans[1] = NULL;
            entity->linkBeans[2] = NULL;
            entity->linkBeans[3] = NULL;
            entity->state        = PuyoBean_State_Falling;
            entity->field_80     = 0;
            return;
        }
    }
    if (!PuyoBean->field_28[entity->playerID]) {
        PuyoBean_Unknown3();
        if (entity->linkFlags > 0)
            PuyoBean_CheckBeanLinks(entity, NULL);

        if (entity->field_68 <= 2) {
            entity->field_A0 = 0;
        }
        else if (++entity->field_A0 > 2) {
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 3, &entity->animator, true, entity->linkFlags);
            entity->timer    = 0;
            entity->field_A0 = 0;
            entity->state    = PuyoBean_Unknown22;
            return;
        }
    }

    if (entity->linkFlags) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 3, &entity->animator, true, entity->linkFlags);
    }
    else {
        if (entity->timer <= 0) {
            if (entity->animator.animationID - entity->type == 3) {
                RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 1, &entity->animator, true, 0);
            }
            else {
                RSDK.ProcessAnimation(&entity->animator);
                if (entity->animator.frameID == entity->animator.frameCount - 1) {
                    entity->timer = RSDK.Rand(120, 240);
                }
            }
        }
        else {
            if (!--entity->timer)
                RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 1, &entity->animator, true, 0);
        }
    }
}

void PuyoBean_State_Falling(void)
{
    RSDK_THIS(PuyoBean);

    if (++entity->timer > 8) {
        entity->velocity.y += 0x3800;
        entity->position.y += entity->velocity.y;

        foreach_active(PuyoBean, bean)
        {
            if (bean != entity && bean->state == PuyoBean_State_Falling && entity->position.x == bean->position.x) {
                if (entity->position.y + 0x100000 > bean->position.y && entity->position.y < bean->position.y) {
                    entity->position.y = bean->position.y - 0x100000;
                    entity->velocity.y = bean->velocity.y;
                    foreach_break;
                }
            }
        }

        PuyoBean_CalculateStillPos(entity);

        if (entity->stillPos.y >= 0) {
            if (entity->stillPos.y > 13)
                entity->stillPos.y = 13;
            int32 y  = entity->stillPos.y;
            int32 ny = y + 1;

            if (y >= 13 || PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, ny)) {
                if ((entity->position.y & 0xF0000) >= 0x80000) {
                    entity->position.y = (y << 20) + entity->origin.y + 0x80000;
                    entity->timer      = 0;
                    int slot           = 128 * entity->playerID + 8 * entity->stillPos.y + entity->stillPos.x;
                    int slot2          = slot + 0x600;
                    if (entity->flag) {
                        PuyoBean->gameState[slot] = RSDK.GetEntityByID(slot2);
                        entity->state             = PuyoBean_Unknown19;
                        if (entity->velocity.y > 0x8000) {
                            RSDK.PlaySfx(PuyoBean->sfxJunk, false, 255);
                        }
                    }
                    else {
                        PuyoBean->gameState[slot] = RSDK.GetEntityByID(slot2);
                        entity->state             = PuyoBean_Unknown18;
                        if (entity->velocity.y > 0x8000) {
                            RSDK.PlaySfx(PuyoBean->sfxLand, false, 255);
                        }
                    }

                    entity->velocity.y = 0;
                    RSDK.AddDrawListRef(entity->drawOrder, slot2);
                    RSDK.CopyEntity(RSDK_GET_ENTITY(slot2, PuyoBean), entity, true);
                }
            }
        }
    }
}

void PuyoBean_Unknown18(void)
{
    RSDK_THIS(PuyoBean);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, entity->stillPos.y);
        if (bean) {
            if (bean != entity)
                destroyEntity(bean);
        }
        entity->timer = RSDK.Rand(120, 240);
        entity->state = PuyoBean_State_GameState;
    }
}

void PuyoBean_Unknown19(void)
{
    RSDK_THIS(PuyoBean);

    if (++entity->timer == 2) {
        entity->timer = 0;
        entity->state = PuyoBean_Unknown20;
        PuyoBean_CalculateStillPos(entity);
    }
}

void PuyoBean_Unknown20(void)
{
    RSDK_THIS(PuyoBean);

    bool32 flag = false;

    entity->linkBeans[0] = NULL;
    if (entity->stillPos.x > -1) {
        int y = entity->stillPos.y - 1;
        if (entity->stillPos.x >= 0 && y >= 0 && entity->stillPos.x <= 5 && y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    entity->linkBeans[1] = NULL;
    if (!flag && entity->stillPos.y < 14) {
        int y = entity->stillPos.y + 1;
        if (entity->stillPos.x >= 0 && y >= 0 && entity->stillPos.x <= 5 && y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    entity->linkBeans[2] = NULL;
    if (!flag && entity->stillPos.x > -1) {
        int x = entity->stillPos.x - 1;
        if (x >= 0 && entity->stillPos.y >= 0 && x <= 5 && entity->stillPos.y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, x, entity->stillPos.y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    entity->linkBeans[3] = NULL;
    if (!flag && entity->stillPos.x < 6) {
        int x = entity->stillPos.x + 1;
        if (x >= 0 && entity->stillPos.y >= 0 && x <= 5 && entity->stillPos.y <= 13) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(entity->playerID, x, entity->stillPos.y);
            if (bean) {
                if (bean->state == PuyoBean_Unknown23 && bean->field_80 < 2)
                    flag = true;
            }
        }
    }

    if (flag) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 31, &entity->animator, true, 0);
        entity->state = PuyoBean_Unknown21;
        entity->timer = 0;
    }
    else {
        if (entity->stillPos.y < 13) {
            int y = entity->stillPos.y + 1;
            if (entity->stillPos.x < 0 || y < 0 || entity->stillPos.x > 5 || y > 13
                || !PuyoBean_GetPuyoBean(entity->playerID, entity->stillPos.x, y)) {
                PuyoBean->gameState[128 * entity->playerID + 8 * entity->stillPos.y + entity->stillPos.x] = NULL;
                entity->velocity.y                                                                        = 0;
                entity->field_80                                                                          = 0;
                entity->state                                                                             = PuyoBean_State_Falling;
                entity->timer                                                                             = 0;
            }
        }
    }
}

void PuyoBean_Unknown21(void)
{
    RSDK_THIS(PuyoBean);

    bool32 flag = false;
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        flag            = true;
        entity->visible = false;
    }
    else {
        RSDK.ProcessAnimation(&entity->animator);
    }

    if (entity->timer < 26)
        entity->timer++;

    if (entity->timer == 26) {
        PuyoBean->gameState[128 * entity->playerID + 8 * entity->stillPos.y + entity->stillPos.x] = 0;
    }
    if (entity->timer >= 26) {
        if (flag)
            destroyEntity(entity);
    }
}

void PuyoBean_Unknown22(void)
{
    RSDK_THIS(PuyoBean);

    PuyoBean_Unknown3();
    ++entity->timer;
    entity->visible = !(entity->timer & 1);
    if (entity->timer == 24) {
        entity->timer    = RSDK.Rand(8, 16);
        entity->field_80 = 0;
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 4, &entity->animator, true, 0);
        entity->state = PuyoBean_Unknown23;
        RSDK.PlaySfx(PuyoBean->chainFrames[minVal(PuyoBean->field_20[entity->playerID], 5)], false, 255);
    }
}

void PuyoBean_Unknown23(void)
{
    RSDK_THIS(PuyoBean);

    if (entity->field_80 >= 2)
        PuyoBean->gameState[128 * entity->playerID + 8 * entity->stillPos.y + entity->stillPos.x] = NULL;
    else
        entity->field_80++;

    if (entity->timer <= 0) {
        for (int angle = 0; angle < 0x100; angle += 0x20) {
            int x                = RSDK.Cos256(angle) << 10;
            int y                = RSDK.Sin256(angle) << 10;
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, x + entity->position.x, y + entity->position.y);
            debris->state        = Debris_State_Fall;
            debris->gravity      = 0x4000;
            debris->timer        = 14;
            debris->velocity.x   = RSDK.Cos256(angle) << 9;
            debris->velocity.y   = RSDK.Sin256(angle) << 9;
            debris->drawOrder    = Zone->drawOrderHigh + 1;
            RSDK.SetSpriteAnimation(PuyoBean->aniFrames, entity->type + 5, &debris->animator, true, 0);
        }
        destroyEntity(entity);
    }
    else {
        entity->timer--;
    }
}

void PuyoBean_Unknown24(void)
{
    RSDK_THIS(PuyoBean);

    if (entity->timer <= 0) {
        entity->velocity.y += 0x3800;
        entity->position.y += entity->velocity.y;

        Vector2 range;
        range.x = 0x800000;
        range.y = 0x800000;
        if (!RSDK.CheckOnScreen(entity, &range))
            destroyEntity(entity);
    }
    else {
        entity->velocity.y = 0;
        entity->timer--;
    }
}

#if RETRO_INCLUDE_EDITOR
void PuyoBean_EditorDraw(void)
{
    RSDK_THIS(PuyoBean);
    RSDK.DrawSprite(&PuyoBean->animator, NULL, false);
}

void PuyoBean_EditorLoad(void)
{
    PuyoBean->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 0, &PuyoBean->animator, true, 0);
}
#endif

void PuyoBean_Serialize(void) {}
