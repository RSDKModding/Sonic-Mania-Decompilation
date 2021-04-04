#include "../SonicMania.h"

ObjectUIHeading *UIHeading;

void UIHeading_Update(void)
{
    RSDK_THIS(UIHeading);
    if (entity->spriteIndex != UIHeading->spriteIndex) {
        RSDK.SetSpriteAnimation(UIHeading->spriteIndex, entity->headingID, &entity->data, true, 0);
        entity->spriteIndex = UIHeading->spriteIndex;
    }

    if (entity->state)
        entity->state();
}

void UIHeading_LateUpdate(void) {}

void UIHeading_StaticUpdate(void) {}

void UIHeading_Draw(void)
{
    RSDK_THIS(UIHeading);
    RSDK.DrawSprite(&entity->data2, 0, 0);
    RSDK.DrawSprite(&entity->data, 0, 0);
}

void UIHeading_Create(void *data)
{
    RSDK_THIS(UIHeading);
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x300000;
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 0, &entity->data2, true, 0);
        RSDK.SetSpriteAnimation(UIHeading->spriteIndex, entity->headingID, &entity->data, true, 0);
        entity->spriteIndex = UIHeading->spriteIndex;
    }
}

void UIHeading_StageLoad(void) { UIHeading_LoadSprites(); }

void UIHeading_LoadSprites(void)
{
    switch (Localization->language) {
        case LANGUAGE_EN: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsEN.bin", SCOPE_STAGE); break;
        case LANGUAGE_FR: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsFR.bin", SCOPE_STAGE); break;
        case LANGUAGE_IT: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsIT.bin", SCOPE_STAGE); break;
        case LANGUAGE_GE: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsGE.bin", SCOPE_STAGE); break;
        case LANGUAGE_SP: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsSP.bin", SCOPE_STAGE); break;
        case LANGUAGE_JP: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsJP.bin", SCOPE_STAGE); break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIHeading->spriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsTC.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }
}

void UIHeading_EditorDraw(void) {}

void UIHeading_EditorLoad(void) {}

void UIHeading_Serialize(void) { RSDK_EDITABLE_VAR(UIHeading, VAR_ENUM, headingID); }
