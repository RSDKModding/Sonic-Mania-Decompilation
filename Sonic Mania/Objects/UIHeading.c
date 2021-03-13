#include "../SonicMania.h"

ObjectUIHeading *UIHeading;

void UIHeading_Update()
{
    EntityUIHeading *entity = (EntityUIHeading *)RSDK_sceneInfo->entity;
    if (entity->spriteIndex != UIHeading->spriteIndex) {
        RSDK.SetSpriteAnimation(UIHeading->spriteIndex, entity->headingID, &entity->data, true, 0);
        entity->spriteIndex = UIHeading->spriteIndex;
    }

    if (entity->state)
        entity->state();
}

void UIHeading_LateUpdate() {}

void UIHeading_StaticUpdate() {}

void UIHeading_Draw()
{
    EntityUIHeading *entity = (EntityUIHeading *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data2, 0, 0);
    RSDK.DrawSprite(&entity->data, 0, 0);
}

void UIHeading_Create(void *data)
{
    EntityUIHeading *entity = (EntityUIHeading *)RSDK_sceneInfo->entity;
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

void UIHeading_StageLoad() { UIHeading_LoadSprites(); }

void UIHeading_LoadSprites()
{
    switch (Localization->language) {
        case LANGUAGE_EN: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsEN.bin", SCOPE_STAGE); break;
        case LANGUAGE_FR: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsFR.bin", SCOPE_STAGE); break;
        case LANGUAGE_IT: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsIT.bin", SCOPE_STAGE); break;
        case LANGUAGE_GE: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsGE.bin", SCOPE_STAGE); break;
        case LANGUAGE_SP: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsSP.bin", SCOPE_STAGE); break;
        case LANGUAGE_JP: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsJP.bin", SCOPE_STAGE); break;
        case LANGUAGE_KO: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIHeading->spriteIndex = RSDK.LoadAnimation("UI/HeadingsTC.bin", SCOPE_STAGE); break;
        default: break;
    }
}

void UIHeading_EditorDraw() {}

void UIHeading_EditorLoad() {}

void UIHeading_Serialize() { RSDK_EDITABLE_VAR(UIHeading, VAR_ENUM, headingID); }
