#include "SonicMania.h"

ObjectUIHeading *UIHeading;

void UIHeading_Update(void)
{
    RSDK_THIS(UIHeading);
    if (entity->textSpriteIndex != UIHeading->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIHeading->textSpriteIndex, entity->headingID, &entity->animator, true, 0);
        entity->textSpriteIndex = UIHeading->textSpriteIndex;
    }
    StateMachine_Run(entity->state);
}

void UIHeading_LateUpdate(void) {}

void UIHeading_StaticUpdate(void) {}

void UIHeading_Draw(void)
{
    RSDK_THIS(UIHeading);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void UIHeading_Create(void *data)
{
    RSDK_THIS(UIHeading);
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos      = entity->position;
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x300000;
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(UIHeading->textSpriteIndex, entity->headingID, &entity->animator, true, 0);
        entity->textSpriteIndex = UIHeading->textSpriteIndex;
    }
}

void UIHeading_StageLoad(void) { UIHeading_LoadSprites(); }

void UIHeading_LoadSprites(void)
{
    switch (Localization->language) {
        case LANGUAGE_EN: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsEN.bin", SCOPE_STAGE); break;
        case LANGUAGE_FR: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsFR.bin", SCOPE_STAGE); break;
        case LANGUAGE_IT: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsIT.bin", SCOPE_STAGE); break;
        case LANGUAGE_GE: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsGE.bin", SCOPE_STAGE); break;
        case LANGUAGE_SP: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsSP.bin", SCOPE_STAGE); break;
        case LANGUAGE_JP: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsJP.bin", SCOPE_STAGE); break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsTC.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIHeading_EditorDraw(void) {}

void UIHeading_EditorLoad(void) {}
#endif

void UIHeading_Serialize(void) { RSDK_EDITABLE_VAR(UIHeading, VAR_ENUM, headingID); }
