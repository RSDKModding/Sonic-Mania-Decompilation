#include "SonicMania.h"

ObjectLottoBall *LottoBall;

void LottoBall_Update(void)
{
    RSDK_THIS(LottoBall);
    StateMachine_Run(entity->state);
    entity->angle += entity->angleVel;
    entity->rotation = (entity->angle >> 15) & 0x1FF;
}

void LottoBall_LateUpdate(void) {}

void LottoBall_StaticUpdate(void) {}

void LottoBall_Draw(void)
{
    RSDK_THIS(LottoBall);
    Vector2 drawPos;

    if (entity->screenFlags[RSDK_sceneInfo->currentScreenID] || (!entity->screenRelative && entity->state != LottoBall_Unknown4)) {
        RSDK.DrawSprite(&entity->animator1, NULL, entity->screenRelative);
        switch (entity->type) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW:
            case LOTTOBALL_MULTI:
                RSDK.DrawSprite(&entity->animator2, NULL, entity->screenRelative);
                RSDK.DrawSprite(&entity->animator3, NULL, entity->screenRelative);
                break;
            case LOTTOBALL_EGGMAN: RSDK.DrawSprite(&entity->animator2, NULL, entity->screenRelative); break;
            case LOTTOBALL_BIG:
                entity->drawFX = FX_SCALE | FX_ROTATE;
                drawPos.x      = entity->position.x;
                drawPos.y      = entity->position.y;
                drawPos.y += RSDK.Sin256(entity->timer) << 10;
                RSDK.DrawSprite(&entity->animator2, &drawPos, entity->screenRelative);

                entity->drawFX = FX_SCALE;
                break;
            case LOTTOBALL_TOTAL:
                drawPos = entity->position;
                drawPos.x -= 0x90000;
                entity->animator3.frameID = entity->lottoNum / 100;
                RSDK.DrawSprite(&entity->animator3, &drawPos, entity->screenRelative);

                drawPos.x += 0x90000;
                entity->animator3.frameID = (entity->lottoNum / 10) % 10;
                RSDK.DrawSprite(&entity->animator3, &drawPos, entity->screenRelative);

                drawPos.x += 0x90000;
                entity->animator3.frameID = entity->lottoNum % 10;
                RSDK.DrawSprite(&entity->animator3, &drawPos, entity->screenRelative);
                break;
            default: return;
        }
    }
}

void LottoBall_Create(void *data)
{
    RSDK_THIS(LottoBall);

    if (!RSDK_sceneInfo->inEditor) {
        entity->drawFX = FX_ROTATE;
        RSDK.SetSpriteAnimation(LottoBall->aniFrames, 0, &entity->animator1, true, entity->type);
        switch (entity->type) {
            case LOTTOBALL_BLUE:
            case LOTTOBALL_YELLOW:
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &entity->animator2, true, entity->lottoNum / 10);
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &entity->animator3, true, entity->lottoNum % 10);
                break;
            case LOTTOBALL_MULTI:
                entity->lottoNum %= 10;
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &entity->animator2, true, entity->lottoNum % 10);
                RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &entity->animator3, true, 10);
                break;
            case LOTTOBALL_EGGMAN: RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &entity->animator2, true, 11); break;
            default: break;
        }
        entity->active          = ACTIVE_BOUNDS;
        entity->updateRange.x   = 0x400000;
        entity->updateRange.y   = 0xE00000;
        entity->gravityStrength = 0x3800;
        entity->visible         = true;
        entity->drawOrder       = Zone->drawOrderLow;
        entity->startPos        = entity->position;
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

    if (entity->parent) {
        if (entity->timer) {
            entity->timer--;
        }
        else {
            if (entity->position.y >= entity->parent->position.y - 0x200000) {
                entity->state = LottoBall_Unknown2;
            }
            else {
                if (entity->position.x >= entity->parent->position.x)
                    entity->velocity.x -= entity->gravityStrength;
                else
                    entity->velocity.x += entity->gravityStrength;
            }
            entity->velocity.y += entity->gravityStrength;
            entity->position.x += entity->velocity.x;
            entity->position.y += entity->velocity.y;
            entity->angleVel = entity->velocity.x;
        }
    }
}

void LottoBall_Unknown2(void)
{
    RSDK_THIS(LottoBall);

    EntityLottoMachine *parent = (EntityLottoMachine *)entity->parent;
    if (parent) {
        entity->velocity.y += entity->gravityStrength;
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;

        if (entity->timer) {
            entity->timer--;
        }
        else {
            int vel = (abs(entity->velocity.x) + abs(parent->field_74) + abs(entity->velocity.y)) >> 1;
            if (vel >= 0x30000) {
                if (vel > 0x60000)
                    vel = 0x60000;
            }
            else {
                vel           = 0x30000;
                entity->timer = RSDK.Rand(16, 32);
            }
            if (!(Zone->timer & 0xF))
                RSDK.PlaySfx(LottoBall->sfxLottoBounce, false, 255);

            foreach_active(LottoBall, ball)
            {
                if (ball != entity) {
                    int rx = (entity->position.x - ball->position.x) >> 16;
                    int ry = (entity->position.y - ball->position.y) >> 16;
                    if (rx * rx + ry * ry < 0x100) {
                        int angle          = RSDK.ATan2(rx, ry);
                        entity->velocity.x = (vel * RSDK.Cos256(angle)) >> 8;
                        entity->velocity.y = (vel * RSDK.Sin256(angle)) >> 8;
                    }
                }
            }
        }

        int rx = (entity->position.x - parent->position.x) >> 16;
        int ry = (entity->position.y - parent->position.y) >> 16;
        if (rx * rx + ry * ry > 0x1B90) {
            int angle          = RSDK.ATan2(rx, (entity->position.y - parent->position.y) >> 16);
            entity->position.x = 0x5400 * RSDK.Cos256(angle) + parent->position.x;
            entity->position.y = 0x5400 * RSDK.Sin256(angle) + parent->position.y;
            switch (((angle + 32) & 0xFF) >> 6) {
                case 0:
                case 2:
                    entity->timer      = 8;
                    entity->velocity.x = -entity->velocity.x >> 1;
                    break;
                case 1u:
                    entity->velocity.y = -entity->velocity.y >> 1;
                    entity->velocity.x -= parent->field_74;
                    if (parent->state == LottoMachine_State_Unknown1) {
                        entity->active = ACTIVE_NORMAL;
                        entity->state  = LottoBall_CheckOnScreen;
                    }
                    break;
                case 3:
                    entity->velocity.y = maxVal(-entity->velocity.y >> 1, 0x10000);
                    entity->velocity.x += parent->field_74;
                    break;
            }
        }

        if (entity->velocity.x >= -0x40000) {
            if (entity->velocity.x > 0x40000)
                entity->velocity.x = 0x40000;
            entity->angleVel = entity->velocity.x + entity->velocity.y;
        }
        else {
            entity->velocity.x = -0x40000;
            entity->angleVel   = entity->velocity.x + entity->velocity.y;
        }
    }
}

void LottoBall_CheckOnScreen(void)
{
    RSDK_THIS(LottoBall);

    entity->angleVel = 0;
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->angle      = 0;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->active     = ACTIVE_BOUNDS;
        entity->state      = StateMachine_None;
    }
}

void LottoBall_Unknown4(void)
{
    RSDK_THIS(LottoBall);

    if (entity->parent) {
        if (entity->timer == 8)
            RSDK.PlaySfx(LottoBall->sfxSpew, false, 255);

        if (entity->timer > 16) {
            entity->velocity.y += entity->gravityStrength;
            entity->position.y += entity->velocity.y;
        }

        entity->timer++;
        if (entity->timer > 48) {
            entity->drawFX |= FX_SCALE;
            entity->timer          = 0;
            entity->screenRelative = true;
            entity->scale.x        = 0x200;
            entity->scale.y        = 0x200;
            entity->drawOrder      = Zone->hudDrawOrder;
            entity->active         = ACTIVE_NORMAL;
            entity->state          = LottoBall_Unknown5;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, entity->animator1.animationID + 3, &entity->animator1, true, entity->animator1.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, (entity->animator2.animationID + 3), &entity->animator2, true, entity->animator2.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, (entity->animator3.animationID + 3), &entity->animator3, true, entity->animator3.frameID);
            entity->position.x = entity->field_70 - 0x1880000;
            entity->velocity.x = 0x80000;
            entity->position.y = ((RSDK_screens->height - 160) << 16);
            entity->velocity.y = 0x40000;
        }
    }
}

void LottoBall_Unknown5(void)
{
    RSDK_THIS(LottoBall);

    if (entity->parent) {
        entity->velocity.y += 0x4800;
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        if (entity->position.x > entity->field_70)
            entity->position.x = entity->field_70;
        if (entity->position.y > entity->field_74) {
            entity->position.y = entity->field_74;
            entity->velocity.y = -(entity->velocity.y >> 1);
            if (entity->position.x >= entity->field_70) {
                entity->position.x = entity->field_70;
                entity->state      = StateMachine_None;
            }
        }
    }
}

void LottoBall_Unknown6(void)
{
    RSDK_THIS(LottoBall);

    ++entity->timer;
    entity->position.x += ((RSDK_screens->centerX << 16) - entity->position.x) >> 3;
    if (entity->timer > 24) {
        entity->scale.x -= (entity->scale.x >> 3);
        entity->scale.y -= (entity->scale.y >> 3);
        if (entity->scale.x < 8) {
            entity->position.x     = entity->startPos.x;
            entity->position.y     = entity->startPos.y;
            entity->scale.y        = 8;
            entity->scale.x        = 8;
            entity->angle          = 0;
            entity->velocity.x     = 0;
            entity->velocity.y     = 0;
            entity->active         = ACTIVE_BOUNDS;
            entity->screenRelative = false;
            entity->drawOrder      = Zone->drawOrderLow;
            entity->screenFlags[0] = false;
            entity->screenFlags[1] = false;
            entity->screenFlags[2] = false;
            entity->screenFlags[3] = false;
            entity->state          = LottoBall_Unknown7;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, entity->animator1.animationID - 3, &entity->animator1, true, entity->animator1.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, entity->animator2.animationID - 3, &entity->animator2, true, entity->animator2.frameID);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, entity->animator3.animationID - 3, &entity->animator3, true, entity->animator3.frameID);
        }
    }
}

void LottoBall_Unknown7(void)
{
    RSDK_THIS(LottoBall);

    entity->scale.x += entity->scale.x >> 3;
    entity->scale.y += entity->scale.y >> 3;
    if (entity->scale.x >= 0x200) {
        entity->drawFX &= ~FX_SCALE;
        entity->scale.y = 0x200;
        entity->scale.x = 0x200;
        entity->state   = StateMachine_None;
    }
}

void LottoBall_Unknown8(void)
{
    RSDK_THIS(LottoBall);
    entity->timer += 8;

    int scale = entity->scale.x + ((0x214 - entity->scale.x) >> 3);
    if (scale > 0x200)
        scale = 0x200;
    entity->scale.x = scale;
    entity->scale.y = scale;
    if (entity->timer > 512) {
        entity->timer    = 0;
        entity->type     = LOTTOBALL_TOTAL;
        entity->state    = LottoBall_Unknown6;
        entity->lottoNum = abs(entity->ringCount);
        if (entity->ringCount < 0)
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 8, &entity->animator3, true, 0);
        else
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 7, &entity->animator3, true, 0);
    }
}

#if RETRO_INCLUDE_EDITOR
void LottoBall_EditorDraw(void)
{
    RSDK_THIS(LottoBall);
    RSDK.SetSpriteAnimation(LottoBall->aniFrames, 0, &entity->animator1, true, entity->type);
    switch (entity->type) {
        case LOTTOBALL_BLUE:
        case LOTTOBALL_YELLOW:
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &entity->animator2, true, entity->lottoNum / 10);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &entity->animator3, true, entity->lottoNum % 10);
            break;
        case LOTTOBALL_MULTI:
            entity->lottoNum %= 10;
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &entity->animator2, true, entity->lottoNum % 10);
            RSDK.SetSpriteAnimation(LottoBall->aniFrames, 2, &entity->animator3, true, 10);
            break;
        case LOTTOBALL_EGGMAN: RSDK.SetSpriteAnimation(LottoBall->aniFrames, 1, &entity->animator2, true, 11); break;
        default: break;
    }
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0xE00000;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;

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
