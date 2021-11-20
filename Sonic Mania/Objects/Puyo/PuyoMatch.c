#include "SonicMania.h"

ObjectPuyoMatch *PuyoMatch;

void PuyoMatch_Update(void)
{
    RSDK_THIS(PuyoMatch);
    StateMachine_Run(self->state);
}

void PuyoMatch_LateUpdate(void) {}

void PuyoMatch_StaticUpdate(void)
{
    foreach_active(PuyoMatch, match) { RSDK.AddDrawListRef(Zone->hudDrawOrder, RSDK.GetEntityID(match)); }
}

void PuyoMatch_Draw(void)
{
    RSDK_THIS(PuyoMatch);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup != Zone->hudDrawOrder) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
        drawPos.y += 0x100000, RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }

    if (self->field_94 > 0)
        PuyoMatch_DrawNumbers();
}

void PuyoMatch_Create(void *data)
{
    RSDK_THIS(PuyoMatch);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->stateInput    = PuyoBean_StateInput_HandlePlayerInputs;
        self->field_80      = PuyoMatch->value2;
        self->beanDropPos   = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, PlatformNode)->position;
        self->timer         = 60;
        RSDK.SetSpriteAnimation(PuyoMatch->aniFrames, 1, &self->animator3, true, 0);
    }
}

void PuyoMatch_StageLoad(void)
{
    PuyoMatch->aniFrames = RSDK.LoadSpriteAnimation("Puyo/Combos.bin", SCOPE_STAGE);
    PuyoMatch->value1    = 120;
}

void PuyoMatch_StartPuyoAttack(int playerID, int a2)
{
    foreach_active(PuyoMatch, match)
    {
        if (match->playerID == playerID) {
            match->field_90 += (a2 << 8) / PuyoMatch->value1;
            match->field_94 = match->field_90 >> 8;
        }
    }
}

void PuyoMatch_SetupNextBeans(EntityPuyoMatch *match)
{
    int left  = 6 * RSDK.RandSeeded(0, 5, &match->matchKey);
    int right = 6 * RSDK.RandSeeded(0, 5, &match->matchKey);
    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, left, &match->animator1, true, 0);
    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, right, &match->animator2, true, 0);
}

void PuyoMatch_DropNextBeans(void)
{
    RSDK_THIS(PuyoMatch);

    if (!self->animator1.frameDelay)
        PuyoMatch_SetupNextBeans(self);

    EntityPuyoBean *bean1 = CREATE_ENTITY(PuyoBean, intToVoid(self->animator1.animationID), self->beanDropPos.x, self->beanDropPos.y);
    EntityPuyoBean *bean2 = CREATE_ENTITY(PuyoBean, intToVoid(self->animator2.animationID), self->beanDropPos.x, self->beanDropPos.y);
    PuyoMatch_SetupNextBeans(self);
    bean1->playerID                = self->playerID;
    bean1->partner                 = (Entity *)bean2;
    bean1->animator.animationSpeed = 0;
    bean1->controllerID            = self->playerID + 1;
    bean1->state                   = PuyoBean_State_Idle;

    bean2->playerID     = self->playerID;
    bean2->partner      = (Entity *)bean1;
    bean2->controllerID = self->playerID + 1;
    bean2->state        = PuyoBean_State_Controlled;
    bean2->field_A4     = self->field_74;
    bean2->stateInput   = self->stateInput;
    bean2->position.y += 0x100000;

    self->beanPtr                      = bean2;
    PuyoBean->field_20[self->playerID] = 0;
}

void PuyoMatch_Unknown4(void)
{
    RSDK_THIS(PuyoMatch);

    int validSlotCount[6];
    int count = 0;

    for (int x = 0; x < 6; ++x) {
        validSlotCount[x] = 0;

        for (int y = 0; y < 14; ++y) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, y);
            if (!bean) {
                ++validSlotCount[x];
                ++count;
            }
        }
    }

    count = minVal(count, 30);
    if (count > self->field_94)
        count = self->field_94;
    self->field_94 -= count;
    self->field_90 -= count << 8;

    int id     = 6 * RSDK.Rand(0, 4);
    int spawnY = self->beanDropPos.y + 0x100000;

    for (int i = 0; i < count; ++i) {
        int slot = PuyoMatch->value6[id];
        if (validSlotCount[slot] > 0) {
            EntityPuyoBean *bean = CREATE_ENTITY(PuyoBean, intToVoid(30), self->beanDropPos.x - 0x200000 + (slot << 20), spawnY);
            bean->playerID       = self->playerID;
            bean->origin.x       = self->beanDropPos.x - 0x280000;
            bean->origin.y       = self->beanDropPos.y - 0x80000;
            self->beanPtr      = bean;
            bean->state          = PuyoBean_State_Falling;
            --count;
            validSlotCount[slot]--;
        }
        id = (id + 1) % -24;
        if (!(id % 6))
            spawnY -= 0x100000;
    }
}

void PuyoMatch_DrawNumbers(void)
{
    RSDK_THIS(PuyoMatch);
    Vector2 drawPos;

    drawPos.x = self->beanDropPos.x - 0x280000;
    drawPos.y = self->beanDropPos.y + 0x140000;

    int count = self->field_94;

    for (int i = 0; i < count / 30; ++i) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 35, &self->animator4, true, 0);
        RSDK.DrawSprite(&self->animator4, &drawPos, false);
        drawPos.x += 0x120000;
    }
    count %= 30;

    for (int i = 0; i < count / 6; ++i) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 34, &self->animator4, true, 0);
        RSDK.DrawSprite(&self->animator4, &drawPos, false);
        drawPos.x += 0x100000;
    }
    count %= 6;

    for (int i = 0; i < count; ++i) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 33, &self->animator4, true, 0);
        RSDK.DrawSprite(&self->animator4, &drawPos, false);
        drawPos.x += 0xE0000;
    }
}

void PuyoMatch_State_Unknown1(void)
{
    RSDK_THIS(PuyoMatch);

    PuyoBean->field_28[self->playerID] = 0;

    foreach_active(PuyoBean, bean)
    {
        if (bean->playerID == self->playerID) {
            if (bean->state != PuyoBean_State_GameState && bean->state != PuyoBean_Unknown20)
                self->timer = 30;

            if (bean->state == PuyoBean_Unknown23) {
                PuyoBean->field_28[bean->playerID] = 1;
                self->state                      = PuyoMatch_State_Unknown2;
                foreach_break;
            }

            if (bean->state == PuyoBean_State_Falling || bean->state == PuyoBean_Unknown18)
                PuyoBean->field_28[bean->playerID] = 1;
        }
    }

    if (self->timer) {
        if (!--self->timer) {
            // if the "dispenser" slot is filled, you lose!
            if (PuyoBean_GetPuyoBean(self->playerID, 2, 2)) {
                self->state = PuyoMatch_State_Unknown4;

                if (self->playerID) {
                    foreach_active(CollapsingPlatform, platform)
                    {
                        if (platform->position.x > self->position.x) {
                            platform->playerPos.x = self->position.x;
                            platform->delay       = 1;
                        }
                    }
                }
                else {
                    foreach_active(CollapsingPlatform, platform)
                    {
                        if (platform->position.x < self->position.x) {
                            platform->playerPos.x = self->position.x;
                            platform->delay       = 1;
                        }
                    }
                }
            }
            else {
                self->comboCount = 0;
                bool32 flag      = false;

                foreach_active(PuyoMatch, match)
                {
                    if (match->comboCount)
                        flag = true;
                }

                if (!self->field_94 || flag)
                    PuyoMatch_DropNextBeans();
                else
                    PuyoMatch_Unknown4();
            }
        }
    }
}

void PuyoMatch_State_Unknown2(void)
{
    RSDK_THIS(PuyoMatch);

    self->comboBeanCount = 0;
    if (++self->comboCount == 3 && self->stateInput == PuyoBean_StateInput_HandlePlayerInputs)
        API_UnlockAchievement("ACH_CPZ");

    uint8 flags = 0;
    EntityPuyoBean *targetBean = NULL;
    foreach_active(PuyoBean, bean)
    {
        if (bean->playerID == self->playerID) {
            if (bean->state == PuyoBean_Unknown22 || bean->state == PuyoBean_Unknown23) {
                if (!self->comboBeanCount++)
                    targetBean = bean;
                flags |= 1 << (bean->type / 6);
            }
        }
    }

    int slot = self->comboBeanCount - 4;
    if (slot >= 7)
        slot = 7 - slot;
    self->field_88 = PuyoMatch->value4[slot];

    self->field_84 = 0;
    for (int b = 0; b < 5; ++b) {
        if (getBit(flags, b))
            ++self->field_84;
    }

    self->field_84 = PuyoMatch->value5[self->field_84];

    int val2         = minVal(PuyoBean->field_20[self->playerID], 23);
    int val          = clampVal(self->field_88 + self->field_84 + self->field_80[val2], 1, 999);
    self->comboScore = 10 * val * self->comboBeanCount;

    if (PuyoBean->field_20[self->playerID] < 23)
        PuyoBean->field_20[self->playerID]++;

    slot = -2;
    if (!self->playerID)
        slot = 2;

    EntityPuyoMatch *match = RSDK_GET_ENTITY(SceneInfo->entitySlot + slot, PuyoMatch);

    EntityPuyoAttack *attack = CREATE_ENTITY(PuyoAttack, intToVoid(self->playerID ^ 1), targetBean->position.x, targetBean->position.y);
    attack->targetPos.x      = match->beanDropPos.x - 0x100000;
    attack->targetPos.y      = match->beanDropPos.y + 0xC0000;
    attack->score            = self->comboScore;
    self->score += self->comboScore;
    self->state = PuyoMatch_State_Unknown3;
}

void PuyoMatch_State_Unknown3(void)
{
    RSDK_THIS(PuyoMatch);

    bool32 flag = false;
    foreach_active(PuyoBean, bean)
    {
        if (bean->playerID == self->playerID) {
            if (bean->state == PuyoBean_Unknown22 || bean->state == PuyoBean_Unknown23) {
                flag = true;
            }
        }
    }

    if (!flag)
        self->state = PuyoMatch_State_Unknown1;
}

void PuyoMatch_State_Unknown4(void)
{
    RSDK_THIS(PuyoMatch);

    if (++self->timer == 8) {
        int vals[] = { 12, 8, 0, 4, 6, 16 };

        for (int x = 0; x < 6; ++x) {
            for (int y = 0; y < 14; ++y) {
                EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, y);
                if (bean) {
                    bean->state = PuyoBean_Unknown24;
                    bean->timer = vals[x];
                }
            }
        }

        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        StateMachine_Run(self->matchFinishCB);

        foreach_active(PuyoMatch, match)
        {
            if (match->playerID != self->playerID) {
                RSDK.SetSpriteAnimation(0xFFFF, 0, &match->animator1, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &match->animator2, true, 0);
                StateMachine_Run(match->matchLoseCB);
                if (RSDK.CheckStageFolder("CPZ"))
                    match->state = StateMachine_None;
            }
        }
        self->state = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void PuyoMatch_EditorDraw(void)
{
    RSDK_THIS(PuyoMatch);
    RSDK.SetSpriteAnimation(PuyoMatch->aniFrames, self->playerID ? 18 : 6, &self->animator5, false, 0);
    RSDK.DrawSprite(&self->animator5, NULL, false);
}

void PuyoMatch_EditorLoad(void) { PuyoMatch->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE); }
#endif

void PuyoMatch_Serialize(void) { RSDK_EDITABLE_VAR(PuyoMatch, VAR_ENUM, playerID); }
