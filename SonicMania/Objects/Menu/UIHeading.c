// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIHeading Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->drawGroup     = 2;
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
#if GAME_VERSION != VER_100
        case LANGUAGE_KO: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsTC.bin", SCOPE_STAGE); break;
#endif

        default: break;
    }
}

#if GAME_INCLUDE_EDITOR
void UIHeading_EditorDraw(void)
{
    RSDK_THIS(UIHeading);

    self->startPos      = self->position;
    self->drawGroup     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 0, &self->headingAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIHeading->textFrames, self->headingID, &self->textAnimator, true, 0);
    self->textFrames = UIHeading->textFrames;

    UIHeading_Draw();
}

void UIHeading_EditorLoad(void)
{
    UIHeading->textFrames = RSDK.LoadSpriteAnimation("UI/HeadingsEN.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIHeading, headingID);
    RSDK_ENUM_VAR("Main Menu", UIHEADING_MAINMENU);
    RSDK_ENUM_VAR("Mania Mode", UIHEADING_MANIAMODE);
    RSDK_ENUM_VAR("Time Attack", UIHEADING_TIMEATTACK);
    RSDK_ENUM_VAR("Competition", UIHEADING_COMPETITION);
    RSDK_ENUM_VAR("Results", UIHEADING_RESULTS);
    RSDK_ENUM_VAR("Options", UIHEADING_OPTIONS);
    RSDK_ENUM_VAR("Extras", UIHEADING_EXTRAS);
    RSDK_ENUM_VAR("Leaderboards", UIHEADING_LEADERBOARDS);
    RSDK_ENUM_VAR("Secrets", UIHEADING_SECRETS);
    RSDK_ENUM_VAR("Player Select", UIHEADING_PLAYERSEL);
    RSDK_ENUM_VAR("Zone Select", UIHEADING_ZONESEL);
    RSDK_ENUM_VAR("Replays", UIHEADING_REPLAYS);
    RSDK_ENUM_VAR("Encore Mode", UIHEADING_ENCOREMODE);
}
#endif

void UIHeading_Serialize(void) { RSDK_EDITABLE_VAR(UIHeading, VAR_ENUM, headingID); }
