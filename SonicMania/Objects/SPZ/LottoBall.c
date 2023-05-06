// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LottoBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLottoBall *LottoBall;

void LottoBall_Update(void)
{
    RSDK_THIS(LottoBall);

    StateMachine_Run(self->state);

    self->angle += self->angleVel;
    self->rotation = (self->angle >> 15) & 0x1FF;
}

void LottoBall_LateUpdate(void) {}

void LottoBall_StaticUpdate(void) {}

void LottoBall_Draw(void)
{
    RSDK_THIS(LottoBall);

    Vector2 drawPos;
    if (self->isVisible[SceneInfo->currentScreenID] || (!self->isUIBall && self->state != LottoBall_State_Collected)) {
        RSDK.DrawSprite(&self->ballAnimator, NULL, self->isUIBall);

        switch (self->type) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW:
            case LOTTOBALL_MULTI:
                RSDK.DrawSprite(&self->leftNumAnimator, NULL, self->isUIBall);
                RSDK.DrawSprite(&self->rightNumAnimator, NULL, self->isUIBall);
                break;

            case LOTTOBALL_EGGMAN: RSDK.DrawSprite(&self->leftNumAnimator, NULL, self->isUIBall); break;

            case LOTTOBALL_BIG:
                self->drawFX = FX_SCALE | FX_ROTATE;
                drawPos.x    = self->position.x;
                drawPos.y    = self->position.y;
                drawPos.y += RSDK.Sin256(self->timer) << 10;
                RSDK.DrawSprite(&self->leftNumAnimator, &drawPos, self->isUIBall);

                self->drawFX = FX_SCALE;
                break;

            case LOTTOBALL_TOTAL:
                drawPos = self->position;
                drawPos.x -= 0x90000;
                self->rightNumAnimator.frameID = self->lottoNum / 100;
                RSDK.DrawSprite(&self->rightNumAnimator, &drawPos, self->isUIBall);

                drawPos.x += 0x90000;
                self->rightNumAnimator.frameID = (self->lottoNum / 10) % 10;
                RSDK.DrawSprite(&self->rightNumAnimator, &drawPos, self->isUIBall);

                drawPos.x += 0x90000;
                self->rightNumAnimator.frameID = self->lottoNum % 10;
                RSDK.DrawSprite(&self->rightNumAnimator, &drawPos, self->isUIBall);
                break;

            default: break;
        }
    }
}

void LottoBall_Create(void *data)
{
    RSDK_THIS(LottoBall);

    if (!SceneInfo->inEditor) {
        self->drawFX = FX_ROTATE;
        RSDK.SetSpriteAnimation(LottoBall->aniFrames, 0, &self->ballAnimator, true, self->type);

        switch (self->type) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW:
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->leftNumAnimator, true, self->lottoNum / 10);
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->rightNumAnimator, true, self->lottoNum % 10);
                break;

            case LOTTOBALL_MULTI:
                self->lottoNum %= 10;
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->leftNumAnimator, true, self->lottoNum % 10);
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->rightNumAnimator, true, 10);
                break;

            case LOTTOBALL_EGGMAN: RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->leftNumAnimator, true, 11); break;

            default: break;
        }

        self->active          = ACTIVE_BOUNDS;
        self->updateRange.x   = 0x400000;
        self->updateRange.y   = 0xE00000;
        self->gravityStrength = 0x3800;
        self->visible         = true;
        self->drawGroup       = Zone->objectDrawGroup[0];
        self->startPos        = self->position;
    }
}

void LottoBall_StageLoad(void)
{
    LottoBall->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LottoBall.bin", SCOPE_STAGE);

    LottoBall->sfxLottoBounce = RSDK.GetSfx("Stage/LottoBounce.wav");
    LottoBall->sfxSpew        = RSDK.GetSfx("Stage/SpewBall.wav");
}

void LottoBall_CheckOffScreen(void)
{
    RSDK_THIS(LottoBall);

    self->angleVel = 0;
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->angle      = 0;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->position   = self->startPos;
        self->active     = ACTIVE_BOUNDS;
        self->state      = StateMachine_None;
    }
}

void LottoBall_State_FallIntoMachine(void)
{
    RSDK_THIS(LottoBall);

    if (self->parent) {
        if (self->timer) {
            self->timer--;
        }
        else {
            if (self->position.y >= self->parent->position.y - 0x200000) {
                self->state = LottoBall_State_InMachine;
            }
            else {
                if (self->position.x >= self->parent->position.x)
                    self->velocity.x -= self->gravityStrength;
                else
                    self->velocity.x += self->gravityStrength;
            }

            self->velocity.y += self->gravityStrength;
            self->position.x += self->velocity.x;
            self->position.y += self->velocity.y;
            self->angleVel = self->velocity.x;
        }
    }
}

void LottoBall_State_InMachine(void)
{
    RSDK_THIS(LottoBall);

    EntityLottoMachine *parent = self->parent;
    if (parent) {
        self->velocity.y += self->gravityStrength;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        if (self->timer) {
            self->timer--;
        }
        else {
            int32 vel = (abs(self->velocity.x) + abs(parent->spinSpeed) + abs(self->velocity.y)) >> 1;
            if (vel > 0x60000) {
                vel = 0x60000;
            }
            else if (vel < 0x30000) {
                vel         = 0x30000;
                self->timer = RSDK.Rand(16, 32);
            }

            if (!(Zone->timer & 0xF))
                RSDK.PlaySfx(LottoBall->sfxLottoBounce, false, 255);

            foreach_active(LottoBall, ball)
            {
                if (ball != self) {
                    int32 rx = (self->position.x - ball->position.x) >> 16;
                    int32 ry = (self->position.y - ball->position.y) >> 16;

                    if (rx * rx + ry * ry < 0x100) {
                        int32 angle      = RSDK.ATan2(rx, ry);
                        self->velocity.x = (vel * RSDK.Cos256(angle)) >> 8;
                        self->velocity.y = (vel * RSDK.Sin256(angle)) >> 8;
                    }
                }
            }
        }

        int32 rx = (self->position.x - parent->position.x) >> 16;
        int32 ry = (self->position.y - parent->position.y) >> 16;
        if (rx * rx + ry * ry > 0x1B90) {
            int32 angle      = RSDK.ATan2(rx, (self->position.y - parent->position.y) >> 16);
            self->position.x = 0x5400 * RSDK.Cos256(angle) + parent->position.x;
            self->position.y = 0x5400 * RSDK.Sin256(angle) + parent->position.y;
            switch (((angle + 0x20) & 0xFF) >> 6) {
                case 0:
                case 2:
                    self->timer      = 8;
                    self->velocity.x = -self->velocity.x >> 1;
                    break;

                case 1:
                    self->velocity.y = -self->velocity.y >> 1;
                    self->velocity.x -= parent->spinSpeed;
                    if (parent->state == LottoMachine_State_Startup) {
                        self->active = ACTIVE_NORMAL;
                        self->state  = LottoBall_CheckOffScreen;
                    }
                    break;

                case 3:
                    self->velocity.y = MAX(-self->velocity.y >> 1, 0x10000);
                    self->velocity.x += parent->spinSpeed;
                    break;
            }
        }

        if (self->velocity.x >= -0x40000) {
            if (self->velocity.x > 0x40000)
                self->velocity.x = 0x40000;
            self->angleVel = self->velocity.x + self->velocity.y;
        }
        else {
            self->velocity.x = -0x40000;
            self->angleVel   = self->velocity.x + self->velocity.y;
        }
    }
}

void LottoBall_State_Collected(void)
{
    RSDK_THIS(LottoBall);

    if (self->parent) {
        if (self->timer == 8)
            RSDK.PlaySfx(LottoBall->sfxSpew, false, 0xFF);

        if (self->timer > 16) {
            self->velocity.y += self->gravityStrength;
            self->position.y += self->velocity.y;
        }

        if (++self->timer > 48) {
            self->drawFX |= FX_SCALE;
            self->timer     = 0;
            self->isUIBall  = true;
            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            self->drawGroup = Zone->hudDrawGroup;
            self->active    = ACTIVE_NORMAL;
            self->state     = LottoBall_State_CollectFall;

            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->ballAnimator.animationID + 3, &self->ballAnimator, true, self->ballAnimator.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, (self->leftNumAnimator.animationID + 3), &self->leftNumAnimator, true,
                                    self->leftNumAnimator.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, (self->rightNumAnimator.animationID + 3), &self->rightNumAnimator, true,
                                    self->rightNumAnimator.frameID);

            self->position.x = self->bounds.x - 0x1880000;
            self->position.y = ((ScreenInfo->size.y - 160) << 16);
            self->velocity.x = 0x80000;
            self->velocity.y = 0x40000;
        }
    }
}

void LottoBall_State_CollectFall(void)
{
    RSDK_THIS(LottoBall);

    if (self->parent) {
        self->velocity.y += 0x4800;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        if (self->position.x > self->bounds.x)
            self->position.x = self->bounds.x;

        if (self->position.y > self->bounds.y) {
            self->position.y = self->bounds.y;
            self->velocity.y = -(self->velocity.y >> 1);

            if (self->position.x >= self->bounds.x) {
                self->position.x = self->bounds.x;
                self->state      = StateMachine_None;
            }
        }
    }
}

void LottoBall_State_SetupUIBall(void)
{
    RSDK_THIS(LottoBall);

    ++self->timer;
    self->position.x += ((ScreenInfo->center.x << 16) - self->position.x) >> 3;

    if (self->timer > 24) {
        self->scale.x -= (self->scale.x >> 3);
        self->scale.y -= (self->scale.y >> 3);

        if (self->scale.x < 8) {
            self->position     = self->startPos;
            self->scale.y      = 8;
            self->scale.x      = 8;
            self->angle        = 0;
            self->velocity.x   = 0;
            self->velocity.y   = 0;
            self->active       = ACTIVE_BOUNDS;
            self->isUIBall     = false;
            self->drawGroup    = Zone->objectDrawGroup[0];
            self->isVisible[0] = false;
            self->isVisible[1] = false;
            self->isVisible[2] = false;
            self->isVisible[3] = false;
            self->state        = LottoBall_State_EnterUIBall;

            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->ballAnimator.animationID - 3, &self->ballAnimator, true, self->ballAnimator.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->leftNumAnimator.animationID - 3, &self->leftNumAnimator, true,
                                    self->leftNumAnimator.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->rightNumAnimator.animationID - 3, &self->rightNumAnimator, true,
                                    self->rightNumAnimator.frameID);
        }
    }
}

void LottoBall_State_EnterUIBall(void)
{
    RSDK_THIS(LottoBall);

    self->scale.x += self->scale.x >> 3;
    self->scale.y += self->scale.y >> 3;

    if (self->scale.x >= 0x200) {
        self->drawFX &= ~FX_SCALE;
        self->scale.y = 0x200;
        self->scale.x = 0x200;
        self->state   = StateMachine_None;
    }
}

void LottoBall_State_ShowUIBall(void)
{
    RSDK_THIS(LottoBall);

    self->timer += 8;

    int32 scale   = MIN(self->scale.x + ((0x214 - self->scale.x) >> 3), 0x200);
    self->scale.x = scale;
    self->scale.y = scale;

    if (self->timer > 512) {
        self->timer    = 0;
        self->type     = LOTTOBALL_TOTAL;
        self->state    = LottoBall_State_SetupUIBall;
        self->lottoNum = abs(self->ringCount);
        RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->ringCount < 0 ? 8 : 7, &self->rightNumAnimator, true, 0);
    }
}

#if GAME_INCLUDE_EDITOR
void LottoBall_EditorDraw(void)
{
    RSDK_THIS(LottoBall);

    RSDK.SetSpriteAnimation(LottoBall->aniFrames, 0, &self->ballAnimator, true, self->type);

    switch (self->type) {
        case LOTTOBALL_BLUE:
        case LOTTOBALL_YELLOW:
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->leftNumAnimator, true, self->lottoNum / 10);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->rightNumAnimator, true, self->lottoNum % 10);
            break;

        case LOTTOBALL_MULTI:
            self->lottoNum %= 10;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->leftNumAnimator, true, self->lottoNum % 10);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->rightNumAnimator, true, 10);
            break;

        case LOTTOBALL_EGGMAN: RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->leftNumAnimator, true, 11); break;

        default: break;
    }

    self->updateRange.x = 0x400000;
    self->updateRange.y = 0xE00000;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];

    LottoBall_Draw();
}

void LottoBall_EditorLoad(void)
{
    LottoBall->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/LottoBall.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LottoBall, type);
    RSDK_ENUM_VAR("Blue", LOTTOBALL_BLUE);
    RSDK_ENUM_VAR("Yellow", LOTTOBALL_YELLOW);
    RSDK_ENUM_VAR("Multiplier", LOTTOBALL_MULTI);
    RSDK_ENUM_VAR("Eggman", LOTTOBALL_EGGMAN);
}
#endif

void LottoBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(LottoBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LottoBall, VAR_UINT8, lottoNum);
}
