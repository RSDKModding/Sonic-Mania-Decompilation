// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIHeading Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUIHeading *UIHeading;

void UIHeading_Update(void)
{
    RSDK_THIS(UIHeading);
    if (self->textFrames != UIHeading->textFrames) {
        RSDK.SetSpriteAnimation(UIHeading->textFrames, self->headingID, &self->textAnimator, true, 0);
        self->textFrames = UIHeading->textFrames;
    }
    StateMachine_Run(self->state);
}

void UIHeading_LateUpdate(void) {}

void UIHeading_StaticUpdate(void) {}

void UIHeading_Draw(void)
{
    RSDK_THIS(UIHeading);
    RSDK.DrawSprite(&self->headingAnimator, NULL, false);
    RSDK.DrawSprite(&self->textAnimator, NULL, false);
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
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 0, &self->headingAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIHeading->textFrames, self->headingID, &self->textAnimator, true, 0);
        self->textFrames = UIHeading->textFrames;
    }
}

void UIHeading_StageLoad(void) { UIHeading_LoadSprites(); }

void UIHeading_LoadSprites(void)
{
    switch (Localization->language) {
        case LANGUAGE_EN: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsEN.bin", SCOPE_STAGE); break;
        case LANGUAGE_FR: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsFR.bin", SCOPE_STAGE); break;
        case LANGUAGE_IT: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsIT.bin", SCOPE_STAGE); break;
        case LANGUAGE_GE: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsGE.bin", SCOPE_STAGE); break;
        case LANGUAGE_SP: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsSP.bin", SCOPE_STAGE); break;
        case LANGUAGE_JP: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsJP.bin", SCOPE_STAGE); break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsTC.bin", SCOPE_STAGE); break;
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
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 0, &self->headingAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIHeading->textFrames, self->headingID, &self->textAnimator, true, 0);
    self->textFrames = UIHeading->textFrames;

    UIHeading_Draw();
}

void UIHeading_EditorLoad(void) { UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsEN.bin", SCOPE_STAGE); }
#endif

void UIHeading_Serialize(void) { RSDK_EDITABLE_VAR(UIHeading, VAR_ENUM, headingID); }
