#include "../SonicMania.h"

ObjectTitleLogo *TitleLogo;

void TitleLogo_Update()
{
    RSDK_THIS(TitleLogo);
    if (entity->state)
        entity->state();
}

void TitleLogo_LateUpdate() {}

void TitleLogo_StaticUpdate() {}

void TitleLogo_Draw()
{
    RSDK_THIS(TitleLogo);
    switch (entity->type) {
        case 0:
            RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->data1, 0, 0);
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->data1, 0, 0);
            break;
        case 1:
            entity->direction = FLIP_X;
            RSDK.DrawSprite(&entity->data1, 0, 0);
            entity->direction = FLIP_NONE;
            RSDK.DrawSprite(&entity->data1, 0, 0);
            if (entity->flag)
                RSDK.DrawSprite(&entity->data3, 0, 0);
            break;
        case 6:
            if (!(entity->timer & 0x10))
                RSDK.DrawSprite(&entity->data1, 0, 0);
            break;
        case 7:
            RSDK.DrawSprite(&entity->data1, 0, 0);
            RSDK.DrawSprite(&entity->data2, 0, 0);
            break;
        default: RSDK.DrawSprite(&entity->data1, 0, 0); break;
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
                if (User.CheckDLC(DLC_PLUS)) {
                    RSDK.SetSpriteAnimation(TitleLogo->plusIndex, 0, &entity->data1, true, 0);
                    entity->storeY = entity->position.y;
                }
                else {
                    RSDK.ResetEntityPtr(entity, 0, 0);
                    return;
                }
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

void TitleLogo_StageLoad()
{
    TitleLogo->logoIndex = RSDK.LoadSpriteAnimation("Title/Logo.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    if (User.CheckDLC(DLC_PLUS))
        TitleLogo->plusIndex = RSDK.LoadSpriteAnimation("Title/PlusLogo.bin", SCOPE_STAGE);
    TitleLogo->sfx_Plus = RSDK.GetSFX("Stage/Plus.wav");
#endif
}

void TitleLogo_Unknown1()
{
    RSDK_THIS(TitleLogo);
    switch (Localization->language) {
        case 0:
        case 5: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 0); break;
        case 1:
            if (RSDK_sku->platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 2);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 1);
            break;
        case 2:
            if (RSDK_sku->platform == PLATFORM_XB1 || RSDK_sku->platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 4);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 3);
            break;
        case 3:
            if (RSDK_sku->platform == PLATFORM_SWITCH)
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 6);
            else
                RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 5);
            break;
        case 4: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 7); break;
        case 6: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 8); break;
        case 7: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 9); break;
        case 8: RSDK.SetSpriteAnimation(TitleLogo->logoIndex, 8, &entity->data1, true, 10); break;
        default: break;
    }
}
void TitleLogo_Unknown2()
{
    RSDK_THIS(TitleLogo);

    RSDK.ProcessAnimation(&entity->data1);
    if (entity->flag)
        RSDK.ProcessAnimation(&entity->data3);
}
void TitleLogo_Unknown3()
{
    RSDK_THIS(TitleLogo);
    ++entity->timer;
}
void TitleLogo_Unknown4()
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
                        entity->state = NULL;
                }
            }
        }
    }
}
void TitleLogo_Unknown5()
{
    RSDK_THIS(TitleLogo);
    if (entity->timer <= 0) {
        entity->timer = RSDK.Rand(120, 240);
        RSDK.SetSpriteAnimation(TitleLogo->plusIndex, 1, &entity->data2, true, 0);
        entity->state = TitleLogo_Unknown6;
    }
    else {
        entity->timer--;
    }
}
void TitleLogo_Unknown6()
{
    RSDK_THIS(TitleLogo);
    RSDK.ProcessAnimation(&entity->data2);
    if (entity->data2.frameID == entity->data2.frameCount - 1) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->data2, true, 0);
        entity->state = TitleLogo_Unknown5;
    }
}

void TitleLogo_EditorDraw() {}

void TitleLogo_EditorLoad() {}

void TitleLogo_Serialize() { RSDK_EDITABLE_VAR(TitleLogo, VAR_ENUM, type); }
