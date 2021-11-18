#include "SonicMania.h"

ObjectUIHeading *UIHeading;

void UIHeading_Update(void)
{
    RSDK_THIS(UIHeading);
    if (self->textSpriteIndex != UIHeading->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIHeading->textSpriteIndex, self->headingID, &self->animator, true, 0);
        self->textSpriteIndex = UIHeading->textSpriteIndex;
    }
    StateMachine_Run(self->state);
}

void UIHeading_LateUpdate(void) {}

void UIHeading_StaticUpdate(void) {}

void UIHeading_Draw(void)
{
    RSDK_THIS(UIHeading);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void UIHeading_Create(void *data)
{
    RSDK_THIS(UIHeading);
    if (!SceneInfo->inEditor) {
        self->startPos      = self->position;
        self->visible       = true;
        self->drawOrder     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x300000;
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(UIHeading->textSpriteIndex, self->headingID, &self->animator, true, 0);
        self->textSpriteIndex = UIHeading->textSpriteIndex;
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
void UIHeading_EditorDraw(void)
{
    RSDK_THIS(UIHeading);
    self->startPos      = self->position;
    self->drawOrder     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(UIHeading->textSpriteIndex, self->headingID, &self->animator, true, 0);
    self->textSpriteIndex = UIHeading->textSpriteIndex;

    UIHeading_Draw();
}

void UIHeading_EditorLoad(void) { UIHeading->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/HeadingsEN.bin", SCOPE_STAGE); }
#endif

void UIHeading_Serialize(void) { RSDK_EDITABLE_VAR(UIHeading, VAR_ENUM, headingID); }
