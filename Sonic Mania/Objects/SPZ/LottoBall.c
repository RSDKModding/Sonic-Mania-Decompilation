#include "SonicMania.h"

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

    if (self->screenFlags[SceneInfo->currentScreenID] || (!self->screenRelative && self->state != LottoBall_Unknown4)) {
        RSDK.DrawSprite(&self->animator1, NULL, self->screenRelative);
        switch (self->type) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW:
            case LOTTOBALL_MULTI:
                RSDK.DrawSprite(&self->animator2, NULL, self->screenRelative);
                RSDK.DrawSprite(&self->animator3, NULL, self->screenRelative);
                break;
            case LOTTOBALL_EGGMAN: RSDK.DrawSprite(&self->animator2, NULL, self->screenRelative); break;
            case LOTTOBALL_BIG:
                self->drawFX = FX_SCALE | FX_ROTATE;
                drawPos.x      = self->position.x;
                drawPos.y      = self->position.y;
                drawPos.y += RSDK.Sin256(self->timer) << 10;
                RSDK.DrawSprite(&self->animator2, &drawPos, self->screenRelative);

                self->drawFX = FX_SCALE;
                break;
            case LOTTOBALL_TOTAL:
                drawPos = self->position;
                drawPos.x -= 0x90000;
                self->animator3.frameID = self->lottoNum / 100;
                RSDK.DrawSprite(&self->animator3, &drawPos, self->screenRelative);

                drawPos.x += 0x90000;
                self->animator3.frameID = (self->lottoNum / 10) % 10;
                RSDK.DrawSprite(&self->animator3, &drawPos, self->screenRelative);

                drawPos.x += 0x90000;
                self->animator3.frameID = self->lottoNum % 10;
                RSDK.DrawSprite(&self->animator3, &drawPos, self->screenRelative);
                break;
            default: return;
        }
    }
}

void LottoBall_Create(void *data)
{
    RSDK_THIS(LottoBall);

    if (!SceneInfo->inEditor) {
        self->drawFX = FX_ROTATE;
        RSDK.SetSpriteAnimation(LottoBall->aniFrames, 0, &self->animator1, true, self->type);
        switch (self->type) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW:
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->animator2, true, self->lottoNum / 10);
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->animator3, true, self->lottoNum % 10);
                break;
            case LOTTOBALL_MULTI:
                self->lottoNum %= 10;
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->animator2, true, self->lottoNum % 10);
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->animator3, true, 10);
                break;
            case LOTTOBALL_EGGMAN: RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->animator2, true, 11); break;
            default: break;
        }
        self->active          = ACTIVE_BOUNDS;
        self->updateRange.x   = 0x400000;
        self->updateRange.y   = 0xE00000;
        self->gravityStrength = 0x3800;
        self->visible         = true;
        self->drawOrder       = Zone->drawOrderLow;
        self->startPos        = self->position;
    }
}

void LottoBall_StageLoad(void)
{
    LottoBall->aniFrames      = RSDK.LoadSpriteAnimation("SPZ2/LottoBall.bin", SCOPE_STAGE);
    LottoBall->sfxLottoBounce = RSDK.GetSFX("Stage/LottoBounce.wav");
    LottoBall->sfxSpew        = RSDK.GetSFX("Stage/SpewBall.wav");
}

void LottoBall_Unknown1(void)
{
    RSDK_THIS(LottoBall);

    if (self->parent) {
        if (self->timer) {
            self->timer--;
        }
        else {
            if (self->position.y >= self->parent->position.y - 0x200000) {
                self->state = LottoBall_Unknown2;
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

void LottoBall_Unknown2(void)
{
    RSDK_THIS(LottoBall);

    EntityLottoMachine *parent = (EntityLottoMachine *)self->parent;
    if (parent) {
        self->velocity.y += self->gravityStrength;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        if (self->timer) {
            self->timer--;
        }
        else {
            int vel = (abs(self->velocity.x) + abs(parent->field_74) + abs(self->velocity.y)) >> 1;
            if (vel >= 0x30000) {
                if (vel > 0x60000)
                    vel = 0x60000;
            }
            else {
                vel           = 0x30000;
                self->timer = RSDK.Rand(16, 32);
            }
            if (!(Zone->timer & 0xF))
                RSDK.PlaySfx(LottoBall->sfxLottoBounce, false, 255);

            foreach_active(LottoBall, ball)
            {
                if (ball != self) {
                    int rx = (self->position.x - ball->position.x) >> 16;
                    int ry = (self->position.y - ball->position.y) >> 16;
                    if (rx * rx + ry * ry < 0x100) {
                        int angle          = RSDK.ATan2(rx, ry);
                        self->velocity.x = (vel * RSDK.Cos256(angle)) >> 8;
                        self->velocity.y = (vel * RSDK.Sin256(angle)) >> 8;
                    }
                }
            }
        }

        int rx = (self->position.x - parent->position.x) >> 16;
        int ry = (self->position.y - parent->position.y) >> 16;
        if (rx * rx + ry * ry > 0x1B90) {
            int angle          = RSDK.ATan2(rx, (self->position.y - parent->position.y) >> 16);
            self->position.x = 0x5400 * RSDK.Cos256(angle) + parent->position.x;
            self->position.y = 0x5400 * RSDK.Sin256(angle) + parent->position.y;
            switch (((angle + 32) & 0xFF) >> 6) {
                case 0:
                case 2:
                    self->timer      = 8;
                    self->velocity.x = -self->velocity.x >> 1;
                    break;
                case 1u:
                    self->velocity.y = -self->velocity.y >> 1;
                    self->velocity.x -= parent->field_74;
                    if (parent->state == LottoMachine_State_Unknown1) {
                        self->active = ACTIVE_NORMAL;
                        self->state  = LottoBall_CheckOnScreen;
                    }
                    break;
                case 3:
                    self->velocity.y = maxVal(-self->velocity.y >> 1, 0x10000);
                    self->velocity.x += parent->field_74;
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

void LottoBall_CheckOnScreen(void)
{
    RSDK_THIS(LottoBall);

    self->angleVel = 0;
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->angle      = 0;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->active     = ACTIVE_BOUNDS;
        self->state      = StateMachine_None;
    }
}

void LottoBall_Unknown4(void)
{
    RSDK_THIS(LottoBall);

    if (self->parent) {
        if (self->timer == 8)
            RSDK.PlaySfx(LottoBall->sfxSpew, false, 255);

        if (self->timer > 16) {
            self->velocity.y += self->gravityStrength;
            self->position.y += self->velocity.y;
        }

        self->timer++;
        if (self->timer > 48) {
            self->drawFX |= FX_SCALE;
            self->timer          = 0;
            self->screenRelative = true;
            self->scale.x        = 0x200;
            self->scale.y        = 0x200;
            self->drawOrder      = Zone->hudDrawOrder;
            self->active         = ACTIVE_NORMAL;
            self->state          = LottoBall_Unknown5;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->animator1.animationID + 3, &self->animator1, true, self->animator1.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, (self->animator2.animationID + 3), &self->animator2, true, self->animator2.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, (self->animator3.animationID + 3), &self->animator3, true, self->animator3.frameID);
            self->position.x = self->field_70 - 0x1880000;
            self->velocity.x = 0x80000;
            self->position.y = ((ScreenInfo->height - 160) << 16);
            self->velocity.y = 0x40000;
        }
    }
}

void LottoBall_Unknown5(void)
{
    RSDK_THIS(LottoBall);

    if (self->parent) {
        self->velocity.y += 0x4800;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        if (self->position.x > self->field_70)
            self->position.x = self->field_70;
        if (self->position.y > self->field_74) {
            self->position.y = self->field_74;
            self->velocity.y = -(self->velocity.y >> 1);
            if (self->position.x >= self->field_70) {
                self->position.x = self->field_70;
                self->state      = StateMachine_None;
            }
        }
    }
}

void LottoBall_Unknown6(void)
{
    RSDK_THIS(LottoBall);

    ++self->timer;
    self->position.x += ((ScreenInfo->centerX << 16) - self->position.x) >> 3;
    if (self->timer > 24) {
        self->scale.x -= (self->scale.x >> 3);
        self->scale.y -= (self->scale.y >> 3);
        if (self->scale.x < 8) {
            self->position.x     = self->startPos.x;
            self->position.y     = self->startPos.y;
            self->scale.y        = 8;
            self->scale.x        = 8;
            self->angle          = 0;
            self->velocity.x     = 0;
            self->velocity.y     = 0;
            self->active         = ACTIVE_BOUNDS;
            self->screenRelative = false;
            self->drawOrder      = Zone->drawOrderLow;
            self->screenFlags[0] = false;
            self->screenFlags[1] = false;
            self->screenFlags[2] = false;
            self->screenFlags[3] = false;
            self->state          = LottoBall_Unknown7;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->animator1.animationID - 3, &self->animator1, true, self->animator1.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->animator2.animationID - 3, &self->animator2, true, self->animator2.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, self->animator3.animationID - 3, &self->animator3, true, self->animator3.frameID);
        }
    }
}

void LottoBall_Unknown7(void)
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

void LottoBall_Unknown8(void)
{
    RSDK_THIS(LottoBall);
    self->timer += 8;

    int scale = self->scale.x + ((0x214 - self->scale.x) >> 3);
    if (scale > 0x200)
        scale = 0x200;
    self->scale.x = scale;
    self->scale.y = scale;
    if (self->timer > 512) {
        self->timer    = 0;
        self->type     = LOTTOBALL_TOTAL;
        self->state    = LottoBall_Unknown6;
        self->lottoNum = abs(self->ringCount);
        if (self->ringCount < 0)
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 8, &self->animator3, true, 0);
        else
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 7, &self->animator3, true, 0);
    }
}

#if RETRO_INCLUDE_EDITOR
void LottoBall_EditorDraw(void)
{
    RSDK_THIS(LottoBall);
    RSDK.SetSpriteAnimation(LottoBall->aniFrames, 0, &self->animator1, true, self->type);
    switch (self->type) {
        case LOTTOBALL_BLUE:
        case LOTTOBALL_YELLOW:
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->animator2, true, self->lottoNum / 10);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->animator3, true, self->lottoNum % 10);
            break;
        case LOTTOBALL_MULTI:
            self->lottoNum %= 10;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->animator2, true, self->lottoNum % 10);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &self->animator3, true, 10);
            break;
        case LOTTOBALL_EGGMAN: RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &self->animator2, true, 11); break;
        default: break;
    }
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0xE00000;
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;

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
