#include "SonicMania.h"

ObjectTitleLogo *TitleLogo;

void TitleLogo_Update(void)
{
    RSDK_THIS(TitleLogo);
#if RETRO_USE_PLUS
    StateMachine_Run(entity->state);
#else
    switch (entity->type) {
        case 1:
            RSDK.ProcessAnimation(&entity->animator1);
            if (entity->flag)
                RSDK.ProcessAnimation(&entity->animator2);
            break;
        case 6: ++entity->timer; break;
    }
#endif
}

void TitleLogo_LateUpdate(void) {}

void TitleLogo_StaticUpdate(void) {}

void TitleLogo_Draw(void)
{
    RSDK_THIS(TitleLogo);
    switch (entity->type) {
        case 0:
            RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->animator1, NULL, false);

            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
        case 1:
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
        case 6:
            if (!(entity->timer & 0x10))
                RSDK.DrawSprite(&entity->animator1, NULL, false);
            break;
#if RETRO_USE_PLUS
        case 7:
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
    if (!RSDK_sceneInfo->inEditor) {
        switch (entity->type) {
            case 0: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 0, &entity->animator1, true, 0); break;
            case 1:
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 1, &entity->animator1, true, 0);
#if RETRO_USE_PLUS
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 3, &entity->animator3, true, 0);
                entity->state = TitleLogo_Unknown2;
#else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 3, &entity->animator2, true, 0);
#endif
                break;
            case 6: 
#if RETRO_USE_PLUS
                entity->state = TitleLogo_Unknown3;
#else
                TitleLogo_Unknown1();
#endif
                break;
#if RETRO_USE_PLUS
            case 7:
                if (API.CheckDLC(DLC_PLUS)) {
                    RSDK.SetSpriteAnimation(TitleLogo->plusIndex, 0, &entity->animator1, true, 0);
                    entity->storeY = entity->position.y;
                }
                else {
                    destroyEntity(entity);
                    return;
                }
                break;
#endif
            default: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, entity->type + 2, &entity->animator1, true, 0); break;
        }

        switch (entity->type) {
            case 0:
            case 1:
            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
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
    TitleLogo->logoIndex = RSDK.LoadSpriteAnimation("Title/Logo.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    if (API.CheckDLC(DLC_PLUS))
        TitleLogo->plusIndex = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);
    TitleLogo->sfx_Plus = RSDK.GetSFX("Stage/Plus.wav");
#endif
}

void TitleLogo_Unknown1(void)
{
    RSDK_THIS(TitleLogo);
    switch (Localization->language) {
        case LANGUAGE_EN:
        case LANGUAGE_JP: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 0); break;
        case LANGUAGE_FR:
            if (sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 2);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 1);
            break;
        case LANGUAGE_IT:
            if (sku_platform == PLATFORM_XB1 || sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 4);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 3);
            break;
        case LANGUAGE_GE:
            if (sku_platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 6);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 5);
            break;
        case LANGUAGE_SP: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 7); break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 8); break;
        case LANGUAGE_SC: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 9); break;
        case LANGUAGE_TC: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->animator1, true, 10); break;
#endif
        default: break;
    }
}


#if RETRO_USE_PLUS
void TitleLogo_Unknown2(void)
{
    RSDK_THIS(TitleLogo);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->flag)
        RSDK.ProcessAnimation(&entity->animator3);
}
void TitleLogo_Unknown3(void)
{
    RSDK_THIS(TitleLogo);
    ++entity->timer;
}
void TitleLogo_Unknown4(void)
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
                if (entity->type == 7) {
                    entity->state = TitleLogo_Unknown5;
                }
                else {
                    if (entity->type == 1)
                        entity->state = TitleLogo_Unknown2;
                    else
                        entity->state = StateMachine_None;
                }
            }
        }
    }
}
void TitleLogo_Unknown5(void)
{
    RSDK_THIS(TitleLogo);
    if (entity->timer <= 0) {
        entity->timer = RSDK.Rand(120, 240);
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(TitleLogo->plusIndex, 1, &entity->animator2, true, 0);
#endif
        entity->state = TitleLogo_Unknown6;
    }
    else {
        entity->timer--;
    }
}
void TitleLogo_Unknown6(void)
{
    RSDK_THIS(TitleLogo);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->state = TitleLogo_Unknown5;
    }
}
#endif

void TitleLogo_EditorDraw(void) {}

void TitleLogo_EditorLoad(void) {}

void TitleLogo_Serialize(void) { RSDK_EDITABLE_VAR(TitleLogo, VAR_ENUM, type); }
