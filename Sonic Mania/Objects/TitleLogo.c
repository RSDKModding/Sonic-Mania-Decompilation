#include "../SonicMania.h"

ObjectTitleLogo *TitleLogo;

void TitleLogo_Update(void)
{
    RSDK_THIS(TitleLogo);
    StateMachine_Run(entity->state);
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
            RSDK.DrawSprite(&entity->data1, NULL, false);
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->data1, NULL, false);
            break;
        case 1:
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->data1, NULL, false);
            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->data1, NULL, false);
            if (entity->flag)
                RSDK.DrawSprite(&entity->data3, NULL, false);
            break;
        case 6:
            if (!(entity->timer & 0x10))
                RSDK.DrawSprite(&entity->data1, NULL, false);
            break;
        case 7:
            RSDK.DrawSprite(&entity->data1, NULL, false);
            RSDK.DrawSprite(&entity->data2, NULL, false);
            break;
        default: RSDK.DrawSprite(&entity->data1, NULL, false); break;
    }
}

void TitleLogo_Create(void *data)
{
    RSDK_THIS(TitleLogo);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        switch (entity->type) {
            case 0: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 0, &entity->data1, true, 0); break;
            case 1:
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 1, &entity->data1, true, 0);
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 3, &entity->data3, true, 0);
                entity->state = TitleLogo_Unknown2;
                break;
            case 6: entity->state = TitleLogo_Unknown3; break;
            case 7:
#if RETRO_USE_PLUS
                if (API.CheckDLC(DLC_PLUS)) {
                    RSDK.SetSpriteAnimation(TitleLogo->plusIndex, 0, &entity->data1, true, 0);
                    entity->storeY = entity->position.y;
                }
                else {
#endif
                    RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
                    return;
#if RETRO_USE_PLUS
                }
#endif
                break;
            default: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, entity->type + 2, &entity->data1, true, 0); break;
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
        case LANGUAGE_JP: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 0); break;
        case LANGUAGE_FR:
#if RETRO_USE_PLUS
            if (RSDK_sku->platform == PLATFORM_SWITCH)
#else
            if (RSDK_info->platform == PLATFORM_SWITCH)
#endif
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 2);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 1);
            break;
        case LANGUAGE_IT:
#if RETRO_USE_PLUS
            if (RSDK_sku->platform == PLATFORM_XB1 || RSDK_sku->platform == PLATFORM_SWITCH)
#else
            if (RSDK_info->platform == PLATFORM_XB1 || RSDK_info->platform == PLATFORM_SWITCH)
            #endif
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 4);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 3);
            break;
        case LANGUAGE_GE:
#if RETRO_USE_PLUS
            if (RSDK_sku->platform == PLATFORM_SWITCH)
#else
            if (RSDK_info->platform == PLATFORM_SWITCH)
#endif
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 6);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 5);
            break;
        case LANGUAGE_SP: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 7); break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 8); break;
        case LANGUAGE_SC: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 9); break;
        case LANGUAGE_TC: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 10); break;
#endif
        default: break;
    }
}
void TitleLogo_Unknown2(void)
{
    RSDK_THIS(TitleLogo);

    RSDK.ProcessAnimation(&entity->data1);
    if (entity->flag)
        RSDK.ProcessAnimation(&entity->data3);
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
    RSDK.ProcessAnimation(&entity->data1);
    if (entity->flag)
        RSDK.ProcessAnimation(&entity->data3);

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
        RSDK.SetSpriteAnimation(TitleLogo->plusIndex, 1, &entity->data2, true, 0);
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
    RSDK.ProcessAnimation(&entity->data2);
    if (entity->data2.frameID == entity->data2.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->data2, true, 0);
        entity->state = TitleLogo_Unknown5;
    }
}

void TitleLogo_EditorDraw(void) {}

void TitleLogo_EditorLoad(void) {}

void TitleLogo_Serialize(void) { RSDK_EDITABLE_VAR(TitleLogo, VAR_ENUM, type); }
