// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoMatch Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPuyoMatch *PuyoMatch;

void PuyoMatch_Update(void)
{
    RSDK_THIS(PuyoMatch);

    StateMachine_Run(self->state);
}

void PuyoMatch_LateUpdate(void) {}

void PuyoMatch_StaticUpdate(void)
{
    foreach_active(PuyoMatch, match) { RSDK.AddDrawListRef(Zone->hudDrawGroup, RSDK.GetEntitySlot(match)); }
}

void PuyoMatch_Draw(void)
{
    RSDK_THIS(PuyoMatch);

    Vector2 drawPos;
    if (SceneInfo->currentDrawGroup != Zone->hudDrawGroup) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        RSDK.DrawSprite(&self->beanLAnimator, &drawPos, false);

        drawPos.y += 0x100000;
        RSDK.DrawSprite(&self->beanRAnimator, &drawPos, false);
    }

    if (self->junkBeanCount > 0)
        PuyoMatch_DrawJunkBeanPreviews();
}

void PuyoMatch_Create(void *data)
{
    RSDK_THIS(PuyoMatch);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->stateInput      = PuyoBean_Input_Player;
        self->comboBonusTable = PuyoMatch->comboBonus;
        self->beanDropPos     = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, PlatformNode)->position;
        self->timer           = 60;

        RSDK.SetSpriteAnimation(PuyoMatch->aniFrames, 1, &self->lightAnimator, true, 0);
    }
}

void PuyoMatch_StageLoad(void)
{
    PuyoMatch->aniFrames = RSDK.LoadSpriteAnimation("Puyo/Combos.bin", SCOPE_STAGE);

    PuyoMatch->comboPower = 120; // the lower this is, the more junk will drop from combos, likewise the higher it is, the less junk will drop
}

void PuyoMatch_AddPuyoCombo(int32 playerID, int32 score)
{
    foreach_active(PuyoMatch, match)
    {
        if (match->playerID == playerID) {
            match->junkDropCount += (score << 8) / PuyoMatch->comboPower;
            match->junkBeanCount = match->junkDropCount >> 8;
        }
    }
}

void PuyoMatch_SetupNextBeans(EntityPuyoMatch *match)
{
    int32 left  = 6 * RSDK.RandSeeded(0, 5, &match->matchKey);
    int32 right = 6 * RSDK.RandSeeded(0, 5, &match->matchKey);

    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, left, &match->beanLAnimator, true, 0);
    RSDK.SetSpriteAnimation(PuyoBean->aniFrames, right, &match->beanRAnimator, true, 0);
}

void PuyoMatch_DropNextBeans(void)
{
    RSDK_THIS(PuyoMatch);

    if (!self->beanLAnimator.frameDuration)
        PuyoMatch_SetupNextBeans(self);

    EntityPuyoBean *partnerBean = CREATE_ENTITY(PuyoBean, INT_TO_VOID(self->beanLAnimator.animationID), self->beanDropPos.x, self->beanDropPos.y);
    EntityPuyoBean *bean        = CREATE_ENTITY(PuyoBean, INT_TO_VOID(self->beanRAnimator.animationID), self->beanDropPos.x, self->beanDropPos.y);

    PuyoMatch_SetupNextBeans(self);

    partnerBean->playerID           = self->playerID;
    partnerBean->partner            = bean;
    partnerBean->beanAnimator.speed = 0;
    partnerBean->controllerID       = self->playerID + 1;
    partnerBean->state              = PuyoBean_State_PartnerControlled;

    bean->playerID      = self->playerID;
    bean->partner       = partnerBean;
    bean->controllerID  = self->playerID + 1;
    bean->state         = PuyoBean_State_Controlled;
    bean->selectedLevel = self->selectedLevel;
    bean->stateInput    = self->stateInput;
    bean->position.y += 0x100000;

    self->beanPtr                             = bean;
    PuyoBean->comboChainCount[self->playerID] = 0;
}

void PuyoMatch_DropJunkBeans(void)
{
    RSDK_THIS(PuyoMatch);

    int32 beanColumnColount[PUYO_PLAYFIELD_W];
    int32 count = 0;

    for (int32 x = 0; x < PUYO_PLAYFIELD_W; ++x) {
        beanColumnColount[x] = 0;

        for (int32 y = 0; y < PUYO_PLAYFIELD_H; ++y) {
            EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, y);
            if (!bean) {
                ++beanColumnColount[x];
                ++count;
            }
        }
    }

    count = CLAMP(count, 30, self->junkBeanCount);
    self->junkBeanCount -= count;
    self->junkDropCount -= count << 8;

    int32 id     = 6 * RSDK.Rand(0, 4);
    int32 spawnY = self->beanDropPos.y + 0x100000;

    for (int32 i = 0; i < count; ++i) {
        int32 column = PuyoMatch->beanDropColumnIDs[id];
        if (beanColumnColount[column] > 0) {
            EntityPuyoBean *junkBean = CREATE_ENTITY(PuyoBean, INT_TO_VOID(30), self->beanDropPos.x - 0x200000 + (column << 20), spawnY);
            junkBean->playerID       = self->playerID;
            junkBean->origin.x       = self->beanDropPos.x - 0x280000;
            junkBean->origin.y       = self->beanDropPos.y - 0x80000;
            self->beanPtr            = junkBean;
            junkBean->state          = PuyoBean_State_Falling;
            --count;
            beanColumnColount[column]--;
        }

        id = (id + 1) % -24;
        if (!(id % 6))
            spawnY -= 0x100000;
    }
}

void PuyoMatch_DrawJunkBeanPreviews(void)
{
    RSDK_THIS(PuyoMatch);

    Vector2 drawPos;
    drawPos.x = self->beanDropPos.x - 0x280000;
    drawPos.y = self->beanDropPos.y + 0x140000;

    int32 count = self->junkBeanCount;

    for (int32 i = 0; i < count / 30; ++i) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 35, &self->junkPreviewAnimator, true, 0);
        RSDK.DrawSprite(&self->junkPreviewAnimator, &drawPos, false);
        drawPos.x += 0x120000;
    }
    count %= 30;

    for (int32 i = 0; i < count / 6; ++i) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 34, &self->junkPreviewAnimator, true, 0);
        RSDK.DrawSprite(&self->junkPreviewAnimator, &drawPos, false);
        drawPos.x += 0x100000;
    }
    count %= 6;

    for (int32 i = 0; i < count; ++i) {
        RSDK.SetSpriteAnimation(PuyoBean->aniFrames, 33, &self->junkPreviewAnimator, true, 0);
        RSDK.DrawSprite(&self->junkPreviewAnimator, &drawPos, false);
        drawPos.x += 0xE0000;
    }
}

void PuyoMatch_State_HandleMatch(void)
{
    RSDK_THIS(PuyoMatch);

    PuyoBean->disableBeanLink[self->playerID] = false;

    foreach_active(PuyoBean, bean)
    {
        if (bean->playerID == self->playerID) {
            if (bean->state != PuyoBean_State_BeanIdle && bean->state != PuyoBean_State_JunkIdle)
                self->timer = 30;

            if (bean->state == PuyoBean_State_BeanPop) {
                PuyoBean->disableBeanLink[bean->playerID] = true;
                self->state                               = PuyoMatch_State_HandleCombos;
                foreach_break;
            }

            if (bean->state == PuyoBean_State_Falling || bean->state == PuyoBean_State_BeanLand)
                PuyoBean->disableBeanLink[bean->playerID] = true;
        }
    }

    if (self->timer) {
        if (!--self->timer) {
            // if the "dispenser" slot is filled, you lose!
            if (PuyoBean_GetPuyoBean(self->playerID, 2, 2)) {
                self->state = PuyoMatch_State_Lose;

                if (self->playerID) {
                    foreach_active(CollapsingPlatform, platform)
                    {
                        if (platform->position.x > self->position.x) {
                            platform->stoodPos.x = self->position.x;
                            platform->delay      = 1;
                        }
                    }
                }
                else {
                    foreach_active(CollapsingPlatform, platform)
                    {
                        if (platform->position.x < self->position.x) {
                            platform->stoodPos.x = self->position.x;
                            platform->delay      = 1;
                        }
                    }
                }
            }
            else {
                self->comboCount = 0;
                bool32 hasCombo  = false;

                foreach_active(PuyoMatch, match)
                {
                    if (match->comboCount)
                        hasCombo = true;
                }

                if (!self->junkBeanCount || hasCombo)
                    PuyoMatch_DropNextBeans();
                else
                    PuyoMatch_DropJunkBeans();
            }
        }
    }
}

void PuyoMatch_State_HandleCombos(void)
{
    RSDK_THIS(PuyoMatch);

    self->comboBeanCount = 0;
    if (++self->comboCount == 3 && self->stateInput == PuyoBean_Input_Player)
        API_UnlockAchievement(&achievementList[ACH_CPZ]);

    uint8 comboColors          = 0;
    EntityPuyoBean *targetBean = NULL;
    foreach_active(PuyoBean, bean)
    {
        if (bean->playerID == self->playerID) {
            if (bean->state == PuyoBean_State_BeginBeanPop || bean->state == PuyoBean_State_BeanPop) {
                if (!self->comboBeanCount++)
                    targetBean = bean;
                comboColors |= 1 << (bean->type / 6);
            }
        }
    }

    // Bonus for getting lots of beans in one go
    int32 slot = self->comboBeanCount - 4;
    if (slot >= 7)
        slot = 7 - slot;
    self->beanBonus = PuyoMatch->beanBonusTable[slot];

    // Bonus for getting multiple combos in one go
    self->concurrentBonus = 0;
    for (int32 b = 0; b < 5; ++b) {
        if (GET_BIT(comboColors, b))
            ++self->concurrentBonus;
    }

    self->concurrentBonus = PuyoMatch->concurrentBonusTable[self->concurrentBonus];

    // Bonus for chaining multiple combos together
    int32 chainBonus = self->comboBonusTable[MIN(PuyoBean->comboChainCount[self->playerID], 23)];

    int32 comboBonus = CLAMP(self->beanBonus + self->concurrentBonus + chainBonus, 1, 999);
    self->comboScore = 10 * comboBonus * self->comboBeanCount;

    if (PuyoBean->comboChainCount[self->playerID] < 23)
        PuyoBean->comboChainCount[self->playerID]++;

    slot = -2;
    if (!self->playerID)
        slot = 2;

    EntityPuyoMatch *match = RSDK_GET_ENTITY(SceneInfo->entitySlot + slot, PuyoMatch);

    EntityPuyoAttack *attack = CREATE_ENTITY(PuyoAttack, INT_TO_VOID(self->playerID ^ 1), targetBean->position.x, targetBean->position.y);
    attack->targetPos.x      = match->beanDropPos.x - 0x100000;
    attack->targetPos.y      = match->beanDropPos.y + 0xC0000;
    attack->score            = self->comboScore;
    self->score += self->comboScore;
    self->state = PuyoMatch_State_HandleComboEnd;
}

void PuyoMatch_State_HandleComboEnd(void)
{
    RSDK_THIS(PuyoMatch);

    bool32 continueCombos = false;
    foreach_active(PuyoBean, bean)
    {
        if (bean->playerID == self->playerID) {
            if (bean->state == PuyoBean_State_BeginBeanPop || bean->state == PuyoBean_State_BeanPop) {
                continueCombos = true;
            }
        }
    }

    if (!continueCombos)
        self->state = PuyoMatch_State_HandleMatch;
}

void PuyoMatch_State_Lose(void)
{
    RSDK_THIS(PuyoMatch);

    if (++self->timer == 8) {
        int32 delays[] = { 12, 8, 0, 4, 6, 16 };

        for (int32 x = 0; x < PUYO_PLAYFIELD_W; ++x) {
            for (int32 y = 0; y < PUYO_PLAYFIELD_H; ++y) {
                EntityPuyoBean *bean = PuyoBean_GetPuyoBean(self->playerID, x, y);
                if (bean) {
                    bean->state = PuyoBean_State_MatchLoseFall;
                    bean->timer = delays[x];
                }
            }
        }

        RSDK.SetSpriteAnimation(-1, 0, &self->beanLAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->beanRAnimator, true, 0);

        StateMachine_Run(self->matchWinCB);

        foreach_active(PuyoMatch, match)
        {
            if (match->playerID != self->playerID) {
                RSDK.SetSpriteAnimation(-1, 0, &match->beanLAnimator, true, 0);
                RSDK.SetSpriteAnimation(-1, 0, &match->beanRAnimator, true, 0);

                StateMachine_Run(match->matchLoseCB);

                if (RSDK.CheckSceneFolder("CPZ"))
                    match->state = StateMachine_None;
            }
        }

        self->state = StateMachine_None;
    }
}

#if GAME_INCLUDE_EDITOR
void PuyoMatch_EditorDraw(void)
{
    RSDK_THIS(PuyoMatch);

    RSDK.SetSpriteAnimation(PuyoMatch->aniFrames, self->playerID ? 18 : 6, &self->unusedAnimator, false, 0);
    RSDK.DrawSprite(&self->unusedAnimator, NULL, false);
}

void PuyoMatch_EditorLoad(void)
{
    PuyoMatch->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PuyoMatch, playerID);
    RSDK_ENUM_VAR("Player 1", PUYOGAME_PLAYER1);
    RSDK_ENUM_VAR("Player 2", PUYOGAME_PLAYER2);
}
#endif

void PuyoMatch_Serialize(void) { RSDK_EDITABLE_VAR(PuyoMatch, VAR_ENUM, playerID); }
