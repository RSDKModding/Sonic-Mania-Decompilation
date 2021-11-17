#include "SonicMania.h"

ObjectTitleLogo *TitleLogo;

void TitleLogo_Update(void)
{
    RSDK_THIS(TitleLogo);
#if RETRO_USE_PLUS
    StateMachine_Run(entity->state);
#else
    switch (entity->type) {
        case TITLELOGO_RIBBON:
            RSDK.ProcessAnimation(&entity->animator1);
            if (entity->flag)
                RSDK.ProcessAnimation(&entity->animator2);
            break;
        case TITLELOGO_PRESSSTART: ++entity->timer; break;
    }
#endif
}

void TitleLogo_LateUpdate(void) {}

void TitleLogo_StaticUpdate(void) {}

void TitleLogo_Draw(void)
{
    RSDK_THIS(TitleLogo);
    switch (entity->type) {
        case TITLELOGO_EMBLEM:
            RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
        case TITLELOGO_RIBBON:
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

#if RETRO_USE_PLUS
            if (entity->flag)
                RSDK.DrawSprite(&entity->animator3, NULL, false);
#else
            if (entity->flag)
                RSDK.DrawSprite(&entity->animator2, NULL, false);
#endif
            break;
        case TITLELOGO_PRESSSTART:
            if (!(entity->timer & 0x10))
                RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
#if RETRO_USE_PLUS
        case TITLELOGO_PLUS:
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            RSDK.DrawSprite(&entity->animator2, NULL, false);
            break;
#endif
        default: RSDK.DrawSprite(&entity->animator1, NULL, false); break;
    }
}

void TitleLogo_Create(void *data)
{
    RSDK_THIS(TitleLogo);

    entity->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        switch (entity->type) {
            case TITLELOGO_EMBLEM: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 0, &entity->animator1, true, 0); break;
            case TITLELOGO_RIBBON: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 1, &entity->animator1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 3, &entity->animator3, true, 0);
                entity->state = TitleLogo_State_Ribbon;
#else
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 3, &entity->animator2, true, 0);
#endif
                break;
            case TITLELOGO_PRESSSTART:
#if RETRO_USE_PLUS
                entity->state = TitleLogo_State_PressStart;
#else
                TitleLogo_SetupPressStart();
#endif
                break;
#if RETRO_USE_PLUS
            case TITLELOGO_PLUS:
                if (API.CheckDLC(DLC_PLUS)) {
                    RSDK.SetSpriteAnimation(TitleLogo->plusFrames, 0, &entity->animator1, true, 0);
                    entity->storeY = entity->position.y;
                }
                else {
                    destroyEntity(entity);
                    return;
                }
                break;
#endif
            default: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, entity->type + 2, &entity->animator1, true, 0); break;
        }

        switch (entity->type) {
            case TITLELOGO_EMBLEM:
            case TITLELOGO_RIBBON:
            case TITLELOGO_GAMETITLE:
            case TITLELOGO_COPYRIGHT:
            case TITLELOGO_RINGBOTTOM:
            case TITLELOGO_PRESSSTART:
#if RETRO_USE_PLUS
            case TITLELOGO_PLUS:
#endif
                entity->visible   = false;
                entity->active    = ACTIVE_NEVER;
                entity->drawOrder = 4;
                break;
            default:
                entity->active    = ACTIVE_NORMAL;
                entity->visible   = true;
                entity->drawOrder = 4;
                break;
        }
    }
}

void TitleLogo_StageLoad(void)
{
    TitleLogo->aniFrames = RSDK.LoadSpriteAnimation("Title/Logo.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    if (API.CheckDLC(DLC_PLUS))
        TitleLogo->plusFrames = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);
    TitleLogo->sfxPlus = RSDK.GetSFX("Stage/Plus.wav");
#endif
}

void TitleLogo_SetupPressStart(void)
{
    RSDK_THIS(TitleLogo);
    switch (Localization->language) {
        case LANGUAGE_EN:
        case LANGUAGE_JP: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 0); break;
        case LANGUAGE_FR:
            if (sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 2);
            else
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 1);
            break;
        case LANGUAGE_IT:
            if (sku_platform == PLATFORM_XB1 || sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 4);
            else
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 3);
            break;
        case LANGUAGE_GE:
            if (sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 6);
            else
                RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 5);
            break;
        case LANGUAGE_SP: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 7); break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 8); break;
        case LANGUAGE_SC: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 9); break;
        case LANGUAGE_TC: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 10); break;
#endif
        default: break;
    }
}

#if RETRO_USE_PLUS
void TitleLogo_State_Ribbon(void)
{
    RSDK_THIS(TitleLogo);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->flag)
        RSDK.ProcessAnimation(&entity->animator3);
}
void TitleLogo_State_PressStart(void)
{
    RSDK_THIS(TitleLogo);
    ++entity->timer;
}
void TitleLogo_State_HandleSetup(void)
{
    RSDK_THIS(TitleLogo);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->flag)
        RSDK.ProcessAnimation(&entity->animator3);

    if (entity->velocity.y > 0) {
        if (entity->position.y >= entity->storeY) {
            --entity->timer;
            entity->position.y = entity->storeY;
            entity->velocity.y = -(entity->velocity.y >> 1);
            if (entity->timer <= 0) {

                entity->timer = 0;
                if (entity->type == TITLELOGO_PLUS) {
                    entity->state = TitleLogo_State_PlusLogo;
                }
                else {
                    if (entity->type == TITLELOGO_RIBBON)
                        entity->state = TitleLogo_State_Ribbon;
                    else
                        entity->state = StateMachine_None;
                }
            }
        }
    }
}

void TitleLogo_State_PlusLogo(void)
{
    RSDK_THIS(TitleLogo);
    if (entity->timer <= 0) {
        entity->timer = RSDK.Rand(120, 240);
        RSDK.SetSpriteAnimation(TitleLogo->plusFrames, 1, &entity->animator2, true, 0);
        entity->state = TitleLogo_State_PlusShine;
    }
    else {
        entity->timer--;
    }
}
void TitleLogo_State_PlusShine(void)
{
    RSDK_THIS(TitleLogo);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->state = TitleLogo_State_PlusLogo;
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void TitleLogo_EditorDraw(void)
{
    RSDK_THIS(TitleLogo);

    entity->drawFX = FX_FLIP;
    switch (entity->type) {
        case TITLELOGO_EMBLEM:
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 0, &entity->animator1, true, 0);

            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

            entity->direction = FLIP_NONE;
            break;
        case TITLELOGO_RIBBON: RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 1, &entity->animator1, true, 0);
#if RETRO_USE_PLUS
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 3, &entity->animator3, true, 0);
#else
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 3, &entity->animator2, true, 0);
#endif

            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

#if RETRO_USE_PLUS
            RSDK.DrawSprite(&entity->animator3, NULL, false);
#else
            RSDK.DrawSprite(&entity->animator2, NULL, false);
#endif
            break;
        case TITLELOGO_PRESSSTART:
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, 8, &entity->animator1, true, 0);
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
#if RETRO_USE_PLUS
        case TITLELOGO_PLUS:
            RSDK.SetSpriteAnimation(TitleLogo->plusFrames, 0, &entity->animator1, true, 0);

            RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
#endif
        default:
            RSDK.SetSpriteAnimation(TitleLogo->aniFrames, entity->type + 2, &entity->animator1, true, 0);
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
    }
}

void TitleLogo_EditorLoad(void)
{
    TitleLogo->aniFrames = RSDK.LoadSpriteAnimation("Title/Logo.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
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
#if RETRO_USE_PLUS
    RSDK_ENUM_VAR("Plus Logo", TITLELOGO_PLUS);
#endif
}
#endif

void TitleLogo_Serialize(void) { RSDK_EDITABLE_VAR(TitleLogo, VAR_ENUM, type); }
