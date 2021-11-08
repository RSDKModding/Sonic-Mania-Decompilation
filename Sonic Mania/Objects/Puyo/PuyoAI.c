#include "SonicMania.h"

ObjectPuyoAI *PuyoAI;

void PuyoAI_Update(void) {}

void PuyoAI_LateUpdate(void) {}

void PuyoAI_StaticUpdate(void) {}

void PuyoAI_Draw(void) {}

void PuyoAI_Create(void *data) {}

void PuyoAI_StageLoad(void) {}

Vector2 PuyoAI_GetBeanPos(int playerID)
{
    Vector2 pos;
    pos.x = -1;
    pos.y = -1;
    foreach_all(PuyoBean, bean)
    {
        if (bean->stateInput && bean->state == PuyoBean_State_Controlled && bean->playerID == playerID) {
            pos.x = bean->stillPos.x;
            pos.y = bean->stillPos.y;
        }
    }
    return pos;
}

void PuyoAI_Unknown2(int playerID)
{
    EntityPuyoBean *bean    = NULL;
    EntityPuyoBean *partner = NULL;

    foreach_all(PuyoBean, beanPtr)
    {
        if (beanPtr->stateInput && beanPtr->state == PuyoBean_State_Controlled && beanPtr->playerID == playerID) {
            bean    = beanPtr;
            partner = (EntityPuyoBean *)bean->partner;
        }
    }

    int prevVal = PuyoAI->value4[playerID];
    int by = partner->stillPos.y;
    if (bean->stillPos.y < by)
        by = bean->stillPos.y;

    PuyoAI->value4[playerID] = by;

    if (prevVal > by) {

        uint8 heightArray[6];
        memset(heightArray, 0, sizeof(heightArray));

        for (int i = 0; i < 6; ++i) {
            heightArray[i] = 13 - PuyoBean_Unknown12(i, playerID, bean, partner);
        }

        by = bean->stillPos.y;
        if (bean->stillPos.y <= partner->stillPos.y)
            by = partner->stillPos.y;

        int sx = 0;
        int ox = 0;
        if (bean->stillPos.x >= partner->stillPos.x)
            sx = partner->stillPos.x;
        else
            sx = bean->stillPos.x;
        if (bean->stillPos.x <= partner->stillPos.x)
            ox = partner->stillPos.x;
        else
            ox = bean->stillPos.x;

        while ((bean->stillPos.x == sx || partner->stillPos.x == sx || heightArray[sx] > by) && sx > 0) {
            sx--;
        }

        while ((bean->stillPos.x == ox || partner->stillPos.x == ox || heightArray[ox] > by) && ox < 6) {
            ox++;
        }

        sx++;
        int lastY = 0;
        int x1    = 0;
        int y1    = 0;
        int x2    = 0;
        int y2    = 0;

        for (int x = sx; x < ox; ++x) {
            for (int p = 0; p < 4; ++p) {
                switch (p) {
                    case 0:
                        if (x >= 5)
                            continue;
                        x1 = x;
                        y1 = heightArray[x];
                        x2 = x + 1;
                        y2 = heightArray[x + 1];
                        break;
                    case 1:
                        x1 = x;
                        y1 = heightArray[x] - 1;
                        x2 = x;
                        y2 = heightArray[x];
                        break;
                    case 2:
                        if (bean->type == partner->type || x >= 5)
                            continue;
                        x1 = x + 1;
                        y1 = heightArray[x + 1];
                        x2 = x;
                        y2 = heightArray[x];
                        break;
                    case 3:
                        x1 = x;
                        y1 = heightArray[x];
                        x2 = x;
                        y2 = heightArray[x] - 1;
                        break;
                    default: break;
                }

                if (x1 <= 5 && y1 <= 13 && x2 <= 5 && y2 <= 13) {
                    int val1 = PuyoAI_Unknown3(bean, playerID, partner, x1, y1, x2, y2);
                    if ((!y1 && (x1 == 2 || x1 == 3) || !y2 && (x2 == 2 || x2 == 3)) && val1 < 16)
                        val1 = -1;
                    int val2 = PuyoBean_Unknown9(bean, playerID, x1, y1);
                    int val3 = PuyoBean_Unknown9(partner, playerID, x2, y2);

                    int cnt = (val2 > 0) + (val3 > 0);

                    if (p == 1 || p == 3)
                        val1 = (0x70000 * val1) >> 19;

                    for (; cnt; --cnt) val1 = (0x30000 * val1) >> 18;

                    int y = y2;
                    if (y1 < y2)
                        y = y1;

                    int valY = (val1 * ((y << 16) / 4 + 1)) >> 16;
                    if (valY > lastY || valY == lastY && RSDK.Rand(0, 10) > 5) {
                        lastY                     = valY;
                        PuyoAI->targetX[playerID] = x1;
                        PuyoAI->value6[playerID]  = p;
                    }
                }
            }
        }
    }
}

int PuyoAI_Unknown3(void *b1, int playerID, void *b2, int32 x1, int32 y1, int32 x2, int32 y2)
{
    EntityPuyoBean *bean1 = (EntityPuyoBean *)b1;
    EntityPuyoBean *bean2 = (EntityPuyoBean *)b2;

    for (int i = 0; i < 84; ++i) PuyoBean->field_C34[i] = false;
    int val1 = PuyoBean_Unknown8(bean1, playerID, x1, y1);
    int val2 = PuyoBean_Unknown8(bean2, playerID, x2, y2);
    if (bean1->type == bean2->type && (x1 == x2 || y1 == y2))
        return 1 << (val2 + val1);
    else
        return (1 << val1) + (1 << val2);
}

void PuyoAI_SetupInputs(void *b, bool32 flag)
{
    EntityPuyoBean *bean = (EntityPuyoBean *)b;

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
    if (!flag) {
        bean->rotateRight = RSDK.Rand(0, 6) > 3;
        bean->rotateLeft  = RSDK.Rand(0, 6) > 3;
    }
    if (bean->left && bean->right) {
        bean->left  = false;
        bean->right = false;
    }
}

void PuyoAI_StateInput(void)
{
    RSDK_THIS(PuyoBean);
    EntityPuyoBean *partner = (EntityPuyoBean *)entity->partner;

    entity->left        = false;
    entity->right       = false;
    entity->rotateLeft  = false;
    entity->rotateRight = false;
    if (PuyoAI->value3[entity->playerID]) {
        bool32 flag1 = false;
        bool32 flag2 = false;
        if (PuyoAI->controlInterval[entity->playerID]) {
            flag1 = !(RSDK.Rand(0, 1024) % PuyoAI->controlInterval[entity->playerID]);
            flag2 = !(Zone->timer % (4 * PuyoAI->controlInterval[entity->playerID]));
        }
        else {
            flag1 = true;
            flag2 = true;
        }

        if (flag1 || flag2) {
            Vector2 pos = PuyoAI_GetBeanPos(entity->playerID);
            if (pos.x >= 0 || pos.y >= 0) {
                bool32 flag = PuyoBean_Unknown6(entity);
                if (RSDK.Rand(0, 100) < PuyoAI->controlChance[entity->playerID]) {
                    PuyoAI_SetupInputs(entity, flag);
                }
                else {
                    PuyoAI_Unknown2(entity->playerID);

                    uint8 flags = 0;
                    if (entity->stillPos.y == partner->stillPos.y) {
                        if (entity->stillPos.x >= partner->stillPos.x) {
                            if (entity->stillPos.y == partner->stillPos.y) {
                                if (entity->stillPos.x > partner->stillPos.x) {
                                    flags = 2;
                                }
                            }
                        }
                    }

                    if (flags != 2 && entity->stillPos.x == partner->stillPos.x) {
                        if (entity->stillPos.y >= partner->stillPos.y) {
                            if (entity->stillPos.x == partner->stillPos.x && entity->stillPos.y > partner->stillPos.y)
                                flags = 3;
                        }
                        else {
                            flags = 1;
                        }
                    }

                    int32 val = PuyoAI->value6[entity->playerID] - flags;
                    if (val == 3)
                        val = -1;
                    if (flag2) {
                        if (flag) {
                            if (val == 2) {
                                entity->rotateLeft = true;
                                flag1              = false;
                            }
                        }
                        else {
                            if (val > 0) {
                                entity->rotateLeft = true;
                                flag1              = false;
                            }
                            else if (val < 0) {
                                entity->rotateRight = true;
                                flag1               = false;
                            }
                        }
                    }

                    if (flag1) {
                        entity->left  = pos.x > PuyoAI->targetX[entity->playerID];
                        entity->right = pos.x < PuyoAI->targetX[entity->playerID];
                        if (!val && !entity->left && !entity->right && pos.x == PuyoAI->targetX[entity->playerID]) {
                            entity->down = true;
                        }
                        else {
                            entity->down = false;
                        }
                    }
                }
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void PuyoAI_EditorDraw(void) {}

void PuyoAI_EditorLoad(void) {}
#endif

void PuyoAI_Serialize(void) {}
