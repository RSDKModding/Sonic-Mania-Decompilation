// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TitleLogo Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTitleLogo *TitleLogo;

void TitleLogo_Update(void)
{
    RSDK_THIS(TitleLogo);

#if MANIA_USE_PLUS
    StateMachine_Run(self->state);
#else
    switch (self->type) {
        case TITLELOGO_RIBBON:
            RSDK.ProcessAnimation(&self->mainAnimator);
            if (self->showRibbonCenter)
                RSDK.ProcessAnimation(&self->ribbonCenterAnimator);
            break;

        case TITLELOGO_PRESSSTART: ++self->timer; break;
    }
#endif
}

void TitleLogo_LateUpdate(void) {}

void TitleLogo_StaticUpdate(void) {}

void TitleLogo_Draw(void)
{
    RSDK_THIS(TitleLogo);

    switch (self->type) {
        case TITLELOGO_EMBLEM:
            RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);

            self->direction = FLIP_NONE;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;

        case TITLELOGO_RIBBON:
            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            self->direction = FLIP_NONE;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            if (self->showRibbonCenter)
                RSDK.DrawSprite(&self->ribbonCenterAnimator, NULL, false);
            break;

        case TITLELOGO_PRESSSTART:
            if (!(self->timer & 0x10))
                RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;

#if MANIA_USE_PLUS
        case TITLELOGO_PLUS:
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            RSDK.DrawSprite(&self->plusAnimator, NULL, false);
            break;
#endif

        default: RSDK.DrawSprite(&self->mainAnimator, NULL, false); break;
    }
}

void TitleLogo_Create(void *data)
{
    RSDK_THIS(TitleLogo);

    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        switch (self->type) {
            case TITLELOGO_EMBLEM: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 0, &self->mainAnimator, true, 0); break;

            case TITLELOGO_RIBBON:
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 1, &self->mainAnimator, true, 0);
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 3, &self->ribbonCenterAnimator, true, 0);
#if MANIA_USE_PLUS
                self->state = TitleLogo_State_Ribbon;
#endif
                break;

            case TITLELOGO_PRESSSTART:
#if MANIA_USE_PLUS
                self->state = TitleLogo_State_PressButton;
#else
                TitleLogo_SetupPressStart();
#endif
                break;

#if MANIA_USE_PLUS
            case TITLELOGO_PLUS:
                if (API.CheckDLC(DLC_PLUS)) {
                    RSDK.SetSpriteAnimation(TitleLogo->plusFrames, 0, &self->mainAnimator, true, 0);
                    self->storeY = self->position.y;
                }
                else {
                    destroyEntity(self);
                    return;
                }
                break;
#endif

            default: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, self->type + 2, &self->mainAnimator, true, 0); break;
        }

        switch (self->type) {
            case TITLELOGO_EMBLEM:
            case TITLELOGO_RIBBON:
            case TITLELOGO_GAMETITLE:
            case TITLELOGO_COPYRIGHT:
            case TITLELOGO_RINGBOTTOM:
            case TITLELOGO_PRESSSTART:
#if MANIA_USE_PLUS
            case TITLELOGO_PLUS:
#endif
                self->visible   = false;
                self->active    = ACTIVE_NEVER;
                self->drawGroup = 4;
                break;

            default:
                self->active    = ACTIVE_NORMAL;
                self->visible   = true;
                self->drawGroup = 4;
                break;
        }
    }
}

void TitleLogo_StageLoad(void)
{
    TitleLogo->aniFrames = RSDK.LoadSpriteAnimation("Title/Logo.bin", SCOPE_STAGE);

#if MANIA_USE_PLUS
    if (API.CheckDLC(DLC_PLUS))
        TitleLogo->plusFrames = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);

    TitleLogo->sfxPlus = RSDK.GetSfx("Stage/Plus.wav");
#endif
}

void TitleLogo_SetupPressStart(void)
{
    RSDK_THIS(TitleLogo);

    switch (Localization->language) {
        case LANGUAGE_EN:
        case LANGUAGE_JP: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 0); break;

        case LANGUAGE_FR:
            if (sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 2);
            else
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 1);
            break;

        case LANGUAGE_IT:
            if (sku_platform == PLATFORM_XB1 || sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 4);
            else
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 3);
            break;

        case LANGUAGE_GE:
            if (sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 6);
            else
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 5);
            break;

        case LANGUAGE_SP: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 7); break;

#if GAME_VERSION != VER_100
        case LANGUAGE_KO: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 8); break;
        case LANGUAGE_SC: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 9); break;
        case LANGUAGE_TC: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 10); break;
#endif

        default: break;
    }
}

#if MANIA_USE_PLUS
void TitleLogo_State_Ribbon(void)
{
    RSDK_THIS(TitleLogo);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->showRibbonCenter)
        RSDK.ProcessAnimation(&self->ribbonCenterAnimator);
}
void TitleLogo_State_PressButton(void)
{
    RSDK_THIS(TitleLogo);

    ++self->timer;
}
void TitleLogo_State_HandleSetup(void)
{
    RSDK_THIS(TitleLogo);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->showRibbonCenter)
        RSDK.ProcessAnimation(&self->ribbonCenterAnimator);

    if (self->velocity.y > 0) {
        if (self->position.y >= self->storeY) {
            --self->timer;

            self->position.y = self->storeY;
            self->velocity.y = -(self->velocity.y >> 1);

            if (self->timer <= 0) {
                self->timer = 0;

                if (self->type == TITLELOGO_PLUS)
                    self->state = TitleLogo_State_PlusLogo;
                else
                    self->state = self->type == TITLELOGO_RIBBON ? TitleLogo_State_Ribbon : StateMachine_None;
            }
        }
    }
}

void TitleLogo_State_PlusLogo(void)
{
    RSDK_THIS(TitleLogo);

    if (self->timer <= 0) {
        self->timer = RSDK.Rand(120, 240);

        RSDK.SetSpriteAnimation(TitleLogo->plusFrames, 1, &self->plusAnimator, true, 0);
        self->state = TitleLogo_State_PlusShine;
    }
    else {
        self->timer--;
    }
}
void TitleLogo_State_PlusShine(void)
{
    RSDK_THIS(TitleLogo);

    RSDK.ProcessAnimation(&self->plusAnimator);

    if (self->plusAnimator.frameID == self->plusAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(-1, 0, &self->plusAnimator, true, 0);
        self->state = TitleLogo_State_PlusLogo;
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void TitleLogo_EditorDraw(void)
{
    RSDK_THIS(TitleLogo);

    self->drawFX = FX_FLIP;
    switch (self->type) {
        case TITLELOGO_EMBLEM:
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 0, &self->mainAnimator, true, 0);

            self->direction = FLIP_NONE;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            self->direction = FLIP_NONE;
            break;

        case TITLELOGO_RIBBON:
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 1, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 3, &self->ribbonCenterAnimator, true, 0);

            self->direction = FLIP_X;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            self->direction = FLIP_NONE;
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            RSDK.DrawSprite(&self->ribbonCenterAnimator, NULL, false);
            break;

        case TITLELOGO_PRESSSTART:
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &self->mainAnimator, true, 0);
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;

#if MANIA_USE_PLUS
        case TITLELOGO_PLUS:
            RSDK.SetSpriteAnimation(TitleLogo->plusFrames, 0, &self->mainAnimator, true, 0);

            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;
#endif

        default:
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, self->type + 2, &self->mainAnimator, true, 0);
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;
    }
}

void TitleLogo_EditorLoad(void)
{
    TitleLogo->aniFrames = RSDK.LoadSpriteAnimation("Title/Logo.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    TitleLogo->plusFrames = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);
#endif

    RSDK_ACTIVE_VAR(TitleLogo, type);
    RSDK_ENUM_VAR("Emblem", TITLELOGO_EMBLEM);
    RSDK_ENUM_VAR("Ribbon", TITLELOGO_RIBBON);
    RSDK_ENUM_VAR("Game Title", TITLELOGO_GAMETITLE);
    RSDK_ENUM_VAR("Power LED", TITLELOGO_POWERLED);
    RSDK_ENUM_VAR("Copyright", TITLELOGO_COPYRIGHT);
    RSDK_ENUM_VAR("Ring Bottom", TITLELOGO_RINGBOTTOM);
    RSDK_ENUM_VAR("Press Start", TITLELOGO_PRESSSTART);
#if MANIA_USE_PLUS
    RSDK_ENUM_VAR("Plus Logo", TITLELOGO_PLUS);
#endif
}
#endif

void TitleLogo_Serialize(void) { RSDK_EDITABLE_VAR(TitleLogo, VAR_ENUM, type); }
