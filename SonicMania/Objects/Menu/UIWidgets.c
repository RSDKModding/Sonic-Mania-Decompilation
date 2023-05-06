// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIWidgets Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIWidgets *UIWidgets;

void UIWidgets_Update(void) {}

void UIWidgets_LateUpdate(void) {}

void UIWidgets_StaticUpdate(void)
{
    ++UIWidgets->timer;
    UIWidgets->timer &= 0x7FFF;

#if MANIA_USE_PLUS
    UIWidgets->buttonColor = UIWidgets->buttonColors[(UIWidgets->timer >> 1) & 0xF];
#endif
}

void UIWidgets_Draw(void) {}

void UIWidgets_Create(void *data) {}

void UIWidgets_StageLoad(void)
{
    UIWidgets->active = ACTIVE_ALWAYS;

    UIWidgets->uiFrames = RSDK.LoadSpriteAnimation("UI/UIElements.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    UIWidgets->saveSelFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
#endif
    UIWidgets->fontFrames = RSDK.LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);

    UIWidgets_ApplyLanguage();
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 1, &UIWidgets->frameAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &UIWidgets->arrowsAnimator, true, 0);

    UIWidgets->sfxBleep  = RSDK.GetSfx("Global/MenuBleep.wav");
    UIWidgets->sfxAccept = RSDK.GetSfx("Global/MenuAccept.wav");
    UIWidgets->sfxWarp   = RSDK.GetSfx("Global/SpecialWarp.wav");
    UIWidgets->sfxEvent  = RSDK.GetSfx("Special/Event.wav");
    UIWidgets->sfxWoosh  = RSDK.GetSfx("Global/MenuWoosh.wav");
    UIWidgets->sfxFail   = RSDK.GetSfx("Stage/Fail.wav");

#if MANIA_USE_PLUS
    UIWidgets->buttonColors[0]  = 0xB00000;
    UIWidgets->buttonColors[1]  = 0xB81820;
    UIWidgets->buttonColors[2]  = 0xBC3440;
    UIWidgets->buttonColors[3]  = 0xC44C60;
    UIWidgets->buttonColors[4]  = 0xCC6480;
    UIWidgets->buttonColors[5]  = 0xD07CA0;
    UIWidgets->buttonColors[6]  = 0xD898C0;
    UIWidgets->buttonColors[7]  = 0xE0B0E0;
    UIWidgets->buttonColors[8]  = 0xE0B0E0;
    UIWidgets->buttonColors[9]  = 0xD898C0;
    UIWidgets->buttonColors[10] = 0xD07CA0;
    UIWidgets->buttonColors[11] = 0xCC6480;
    UIWidgets->buttonColors[12] = 0xC44C60;
    UIWidgets->buttonColors[13] = 0xBC3440;
    UIWidgets->buttonColors[14] = 0xB81820;
    UIWidgets->buttonColors[15] = 0xB00000;
#endif
}

void UIWidgets_ApplyLanguage(void)
{
    LogHelpers_Print("Apply Language %d", Localization->language);

    switch (Localization->language) {
        case LANGUAGE_EN: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE); break;
        case LANGUAGE_FR: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextFR.bin", SCOPE_STAGE); break;
        case LANGUAGE_IT: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextIT.bin", SCOPE_STAGE); break;
        case LANGUAGE_GE: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextGE.bin", SCOPE_STAGE); break;
        case LANGUAGE_SP: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextSP.bin", SCOPE_STAGE); break;
        case LANGUAGE_JP: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextJP.bin", SCOPE_STAGE); break;
#if GAME_VERSION != VER_100
        case LANGUAGE_KO: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextTC.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }
}

void UIWidgets_DrawRectOutline_Black(int32 x, int32 y, int32 width, int32 height)
{
    int32 w = width << 16 >> 1;
    int32 h = height << 16 >> 1;

    RSDK.DrawRect(x - w, y - h, width << 16, TO_FIXED(3), 0x000000, 0xFF, INK_NONE, false);
    RSDK.DrawRect(x - w, y - h, TO_FIXED(3), height << 16, 0x000000, 0xFF, INK_NONE, false);
    RSDK.DrawRect(x - w, h + y - TO_FIXED(3), width << 16, TO_FIXED(3), 0x000000, 0xFF, INK_NONE, false);
    RSDK.DrawRect(x + w - TO_FIXED(3), y - h, TO_FIXED(3), height << 16, 0x000000, 0xFF, INK_NONE, false);
}
void UIWidgets_DrawRectOutline_Blended(int32 x, int32 y, int32 width, int32 height)
{
    int32 w = width << 16 >> 1;
    int32 h = height << 16 >> 1;

    RSDK.DrawRect(x - w + TO_FIXED(3), y - h, (width << 16) - TO_FIXED(6), TO_FIXED(3), 0x000000, 0xFF, INK_BLEND, false);
    RSDK.DrawRect(x - w, y - h, TO_FIXED(3), height << 16, 0x000000, 0xFF, INK_BLEND, false);
    RSDK.DrawRect(x - w + TO_FIXED(3), h + y - TO_FIXED(3), (width << 16) - TO_FIXED(6), TO_FIXED(3), 0x000000, 0xFF, INK_BLEND, false);
    RSDK.DrawRect(x + w - TO_FIXED(3), y - h, TO_FIXED(3), height << 16, 0x000000, 0xFF, INK_BLEND, false);
}
void UIWidgets_DrawRectOutline_Flash(int32 x, int32 y, int32 width, int32 height)
{
    int32 w     = width << 16 >> 1;
    int32 h     = height << 16 >> 1;
    color color = RSDK.GetPaletteEntry(3, (UIWidgets->timer >> 1) & 0xF);

    RSDK.DrawRect(x - w, y - h, width << 16, TO_FIXED(3), color, 0xFF, INK_NONE, false);
    RSDK.DrawRect(x - w, y - h, TO_FIXED(3), height << 16, color, 0xFF, INK_NONE, false);
    RSDK.DrawRect(x - w, h + y - TO_FIXED(3), width << 16, TO_FIXED(3), color, 0xFF, INK_NONE, false);
    RSDK.DrawRect(x + w - TO_FIXED(3), y - h, TO_FIXED(3), height << 16, color, 0xFF, INK_NONE, false);
}
void UIWidgets_DrawRightTriangle(int32 x, int32 y, int32 size, int32 red, int32 green, int32 blue)
{
    Vector2 verts[3];

    if (size) {
        verts[0].x = x;
        verts[0].y = y;
        verts[1].x = x;
        verts[1].y = y;
        verts[2].x = x;
        verts[2].y = y;

        if (size < 0) {
            verts[2].x = (size << 16) + x;
            verts[0].y = (size << 16) + y;
        }
        else {
            verts[1].x = (size << 16) + x;
            verts[2].y = (size << 16) + y;
        }

        if (SceneInfo->inEditor) {
            color clr = (blue << 0) | (green << 8) | (red << 16);

            RSDK.DrawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, clr, 0xFF, INK_NONE, false);
            RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 0xFF, INK_NONE, false);
            RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, clr, 0xFF, INK_NONE, false);
        }
        else {
            int32 sx = ScreenInfo->position.x << 16;
            int32 sy = ScreenInfo->position.y << 16;
            verts[0].x -= sx;
            verts[0].y -= sy;
            verts[1].x -= sx;
            verts[1].y -= sy;
            verts[2].x -= sx;
            verts[2].y -= sy;

            RSDK.DrawFace(verts, 3, red, green, blue, 0xFF, INK_NONE);
        }
    }
}
void UIWidgets_DrawEquilateralTriangle(int32 x, int32 y, int32 size, uint8 sizeMode, int32 red, int32 green, int32 blue, InkEffects ink)
{
    Vector2 verts[3];

    if (sizeMode) {
        verts[0].x = x;
        verts[0].y = y;
        verts[1].x = x;
        verts[1].y = y;
        verts[2].x = x;
        verts[2].y = y;

        if (sizeMode == 1) {
            verts[0].x = x - (size << 16);
            verts[1].x = x + (size << 16);
            verts[2].y = y + (size << 16);
        }

        if (SceneInfo->inEditor) {
            color clr = blue | (green << 8) | (red << 16);
            RSDK.DrawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, clr, 0xFF, ink, false);
            RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 0xFF, ink, false);
            RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, clr, 0xFF, ink, false);
        }
        else {
            int32 sx = ScreenInfo->position.x << 16;
            int32 sy = ScreenInfo->position.y << 16;
            verts[1].y -= sy;
            verts[2].x -= sx;
            verts[0].x -= sx;
            verts[0].y -= sy;
            verts[1].x -= sx;
            verts[2].y -= sy;

            RSDK.DrawFace(verts, 3, red, green, blue, 0xFF, ink);
        }
    }
}
void UIWidgets_DrawParallelogram(int32 x, int32 y, int32 width, int32 height, int32 edgeSize, int32 red, int32 green, int32 blue)
{
    Vector2 verts[4];

    verts[0].x = x - (width << 15);
    verts[0].y = y - (height << 15);
    verts[1].x = x + (width << 15);
    verts[1].y = y - (height << 15);
    verts[2].x = verts[1].x;
    verts[2].y = y + (height << 15);
    verts[3].x = verts[0].x;
    verts[3].y = y + (height << 15);

    if (edgeSize << 16 <= 0) {
        verts[0].x -= edgeSize << 16;
        verts[2].x += edgeSize << 16;
    }
    else {
        verts[1].x += edgeSize << 16;
        verts[3].x -= edgeSize << 16;
    }

    if (SceneInfo->inEditor) {
        color clr = (blue << 0) | (green << 8) | (red << 16);

        RSDK.DrawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, clr, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[2].x, verts[2].y, verts[3].x, verts[3].y, clr, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[3].x, verts[3].y, verts[0].x, verts[0].y, clr, 0xFF, INK_NONE, false);
    }
    else {
        int32 sx = ScreenInfo->position.x << 16;
        int32 sy = ScreenInfo->position.y << 16;
        verts[1].y -= sy;
        verts[2].y -= sy;
        verts[3].y -= sy;
        verts[0].y -= sy;
        verts[0].x -= sx;
        verts[1].x -= sx;
        verts[2].x -= sx;
        verts[3].x -= sx;

        RSDK.DrawFace(verts, 4, red, green, blue, 0xFF, INK_NONE);
    }
}
void UIWidgets_DrawUpDownArrows(int32 x, int32 y, int32 arrowDist)
{
    Vector2 drawPos;
    drawPos.x = x;
    drawPos.y = y;

    UIWidgets->arrowsAnimator.frameID = 2;
    drawPos.y -= arrowDist << 15;
    RSDK.DrawSprite(&UIWidgets->arrowsAnimator, &drawPos, false);

    UIWidgets->arrowsAnimator.frameID = 3;
    drawPos.y += arrowDist << 16;
    RSDK.DrawSprite(&UIWidgets->arrowsAnimator, &drawPos, false);
}
void UIWidgets_DrawLeftRightArrows(int32 x, int32 y, int32 arrowDist)
{
    Vector2 drawPos;

    drawPos.x = x;
    drawPos.y = y;

    UIWidgets->arrowsAnimator.frameID = 0;
    drawPos.x -= arrowDist >> 1;
    RSDK.DrawSprite(&UIWidgets->arrowsAnimator, &drawPos, false);

    UIWidgets->arrowsAnimator.frameID = 1;
    drawPos.x += arrowDist;
    RSDK.DrawSprite(&UIWidgets->arrowsAnimator, &drawPos, false);
}
Vector2 UIWidgets_DrawTriJoinRect(int32 x, int32 y, color leftColor, color rightColor)
{
    Vector2 newPos;
    newPos.x = x + TO_FIXED(14);
    newPos.y = y;

    UIWidgets_DrawRightTriangle(x, newPos.y, 13, (leftColor >> 16) & 0xFF, (leftColor >> 8) & 0xFF, leftColor & 0xFF);
    UIWidgets_DrawRightTriangle(newPos.x, newPos.y + TO_FIXED(12), -13, (rightColor >> 16) & 0xFF, (rightColor >> 8) & 0xFF, rightColor & 0xFF);

    return newPos;
}

#if MANIA_USE_PLUS
void UIWidgets_DrawTime(int32 x, int32 y, int32 minutes, int32 seconds, int32 milliseconds)
{
    Vector2 drawPos;
    Animator animator;
    Animator arrowsAnimator;
    char strBuf[16];

    memset(&animator, 0, sizeof(Animator));
    memset(&arrowsAnimator, 0, sizeof(Animator));

    drawPos.x = x;
    drawPos.y = y + TO_FIXED(2);

    RSDK.SetSpriteAnimation(UIWidgets->saveSelFrames, 9, &arrowsAnimator, true, 9);
    RSDK.DrawSprite(&arrowsAnimator, &drawPos, false);

    drawPos.x += TO_FIXED(16);
    drawPos.y -= TO_FIXED(2);
    if (minutes) {
        if (minutes != 99 || seconds != 99 || milliseconds != 99)
            sprintf_s(strBuf, (int32)sizeof(strBuf), "%02d:%02d;%02d", minutes, seconds, milliseconds);
        else
            sprintf_s(strBuf, (int32)sizeof(strBuf), "<<:<<;<<");
    }
    else {
        if (!seconds && !milliseconds)
            sprintf_s(strBuf, (int32)sizeof(strBuf), "<<:<<;<<");
        else
            sprintf_s(strBuf, (int32)sizeof(strBuf), "%02d:%02d;%02d", minutes, seconds, milliseconds);
    }

    for (int32 i = 0; i < 8; ++i) {
        if (!strBuf[i])
            break;

        RSDK.SetSpriteAnimation(UIWidgets->saveSelFrames, 8, &animator, true, (uint8)(strBuf[i] - '0'));
        RSDK.DrawSprite(&animator, &drawPos, false);

        drawPos.x += TO_FIXED(8);
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void UIWidgets_EditorDraw(void) {}

void UIWidgets_EditorLoad(void)
{
    UIWidgets->uiFrames = RSDK.LoadSpriteAnimation("UI/UIElements.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    UIWidgets->saveSelFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
#endif
    UIWidgets->fontFrames = RSDK.LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
    UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 1, &UIWidgets->frameAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &UIWidgets->arrowsAnimator, true, 0);

#if MANIA_USE_PLUS
    UIWidgets->buttonColor = 0xF0F0F0;
#endif
}
#endif

void UIWidgets_Serialize(void) {}
