// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIBackground Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIBackground *UIBackground;

void UIBackground_Update(void)
{
    RSDK_THIS(UIBackground);

    ++self->timer;
}

void UIBackground_LateUpdate(void) {}

void UIBackground_StaticUpdate(void) {}

void UIBackground_Draw(void)
{
    RSDK_THIS(UIBackground);

    StateMachine_Run(self->stateDraw);
}

void UIBackground_Create(void *data)
{
    RSDK_THIS(UIBackground);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 0;
        self->stateDraw = UIBackground_DrawNormal;
    }
}

void UIBackground_StageLoad(void) { UIBackground->activeColors = UIBackground->bgColors; }

void UIBackground_DrawNormal(void)
{
    RSDK_THIS(UIBackground);

    color *colorPtrs = UIBackground->activeColors;

    Vector2 drawPos;
    RSDK.FillScreen(colorPtrs[0], 0xFF, 0xFF, 0xFF);

    drawPos.x = ((RSDK.Sin512(self->timer) >> 3) + 112) * RSDK.Sin256(self->timer) >> 8;
    drawPos.y = ((RSDK.Sin512(self->timer) >> 3) + 112) * RSDK.Cos256(self->timer) >> 8;
    RSDK.DrawCircleOutline(ScreenInfo->center.x, ScreenInfo->center.y, (RSDK.Sin512(self->timer) >> 3) + 108, (RSDK.Sin512(self->timer) >> 3) + 116,
                           colorPtrs[1], 0xFF, INK_NONE, true);

    RSDK.DrawCircle(drawPos.x + ScreenInfo->center.x, drawPos.y + ScreenInfo->center.y, 32, colorPtrs[1], 0xFF, INK_NONE, true);
    RSDK.DrawCircle(ScreenInfo->center.x - drawPos.x, ScreenInfo->center.y - drawPos.y, 16, colorPtrs[1], 0xFF, INK_NONE, true);
    RSDK.DrawCircle(drawPos.x + ScreenInfo->center.x, drawPos.y + ScreenInfo->center.y, 26, colorPtrs[1], 0xFF, INK_NONE, true);

    drawPos.x = ((RSDK.Cos512(self->timer) >> 3) + 144) * RSDK.Cos256(self->timer) >> 8;
    drawPos.y = ((RSDK.Cos512(self->timer) >> 3) + 144) * RSDK.Sin256(self->timer) >> 8;
    RSDK.DrawCircleOutline(ScreenInfo->center.x, ScreenInfo->center.y, (RSDK.Cos512(self->timer) >> 3) + 140, (RSDK.Cos512(self->timer) >> 3) + 148,
                           colorPtrs[2], 0xFF, INK_NONE, true);

    RSDK.DrawCircle(drawPos.x + ScreenInfo->center.x, drawPos.y + ScreenInfo->center.y, 32, colorPtrs[2], 0xFF, INK_NONE, true);
    RSDK.DrawCircle(ScreenInfo->center.x - drawPos.x, ScreenInfo->center.y - drawPos.y, 16, colorPtrs[2], 0xFF, INK_NONE, true);
    RSDK.DrawCircle(drawPos.x + ScreenInfo->center.x, drawPos.y + ScreenInfo->center.y, 26, colorPtrs[0], 0xFF, INK_NONE, true);
}

#if GAME_INCLUDE_EDITOR
void UIBackground_EditorDraw(void)
{
    RSDK_THIS(UIBackground);

    RSDK.SetSpriteAnimation(UIBackground->aniFrames, 0, &self->animator, true, 0);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void UIBackground_EditorLoad(void)
{
    UIBackground->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIBackground, type);
    RSDK_ENUM_VAR("(Unused)", UIBACKGROUND_UNUSED);
}
#endif

void UIBackground_Serialize(void) { RSDK_EDITABLE_VAR(UIBackground, VAR_ENUM, type); }
