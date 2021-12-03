#include "SonicMania.h"

ObjectUIWidgets *UIWidgets;

void UIWidgets_Update(void) {}

void UIWidgets_LateUpdate(void) {}

void UIWidgets_StaticUpdate(void)
{
    ++UIWidgets->timer;
    UIWidgets->timer &= 0x7FFF;
#if RETRO_USE_PLUS
    UIWidgets->buttonColour = UIWidgets->buttonColours[(UIWidgets->timer >> 1) & 0xF];
#endif  
}

void UIWidgets_Draw(void) {}

void UIWidgets_Create(void *data) {}

void UIWidgets_StageLoad(void)
{
    UIWidgets->active                = ACTIVE_ALWAYS;
    UIWidgets->uiSpriteIndex = RSDK.LoadSpriteAnimation("UI/UIElements.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    UIWidgets->saveSelFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
#endif
    UIWidgets->fontFrames      = RSDK.LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
    UIWidgets_ApplyLanguage();
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 1, &UIWidgets->frameAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &UIWidgets->arrowsAnimator, true, 0);
    UIWidgets->sfxBleep  = RSDK.GetSfx("Global/MenuBleep.wav");
    UIWidgets->sfxAccept = RSDK.GetSfx("Global/MenuAccept.wav");
    UIWidgets->sfxWarp   = RSDK.GetSfx("Global/SpecialWarp.wav");
    UIWidgets->sfxEvent  = RSDK.GetSfx("Special/Event.wav");
    UIWidgets->sfxWoosh  = RSDK.GetSfx("Global/MenuWoosh.wav");
    UIWidgets->sfxFail   = RSDK.GetSfx("Stage/Fail.wav");

#if RETRO_USE_PLUS
    UIWidgets->buttonColours[0]  = 0xB00000;
    UIWidgets->buttonColours[1]  = 0xB81820;
    UIWidgets->buttonColours[2]  = 0xBC3440;
    UIWidgets->buttonColours[3]  = 0xC44C60;
    UIWidgets->buttonColours[4]  = 0xCC6480;
    UIWidgets->buttonColours[5]  = 0xD07CA0;
    UIWidgets->buttonColours[6]  = 0xD898C0;
    UIWidgets->buttonColours[7]  = 0xE0B0E0;
    UIWidgets->buttonColours[8]  = 0xE0B0E0;
    UIWidgets->buttonColours[9]  = 0xD898C0;
    UIWidgets->buttonColours[10] = 0xD07CA0;
    UIWidgets->buttonColours[11] = 0xCC6480;
    UIWidgets->buttonColours[12] = 0xC44C60;
    UIWidgets->buttonColours[13] = 0xBC3440;
    UIWidgets->buttonColours[14] = 0xB81820;
    UIWidgets->buttonColours[15] = 0xB00000;
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
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIWidgets->textFrames = RSDK.LoadSpriteAnimation("UI/TextTC.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }
}

void UIWidgets_DrawRectOutline_Black(int32 height, int32 width, int32 x, int32 y)
{
    int32 w = width << 16 >> 1;
    int32 h = height << 16 >> 1;
    RSDK.DrawRect(x - w, y - h, width << 16, 0x30000, 0x000000, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, y - h, 0x30000, height << 16, 0x000000, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, h + y - 0x30000, width << 16, 0x30000, 0x000000, 255, INK_NONE, false);
    RSDK.DrawRect(w - 0x30000 + x, y - h, 0x30000, height << 16, 0x000000, 255, INK_NONE, false);
}
void UIWidgets_DrawRectOutline_Blended(int32 height, int32 width, int32 x, int32 y)
{
    int32 w = width << 16 >> 1;
    int32 h = height << 16 >> 1;
    RSDK.DrawRect(x - w + 0x30000, y - h, (width << 16) - 0x60000, 0x30000, 0x000000, 255, INK_BLEND, false);
    RSDK.DrawRect(x - w, y - h, 0x30000, height << 16, 0x000000, 255, INK_BLEND, false);
    RSDK.DrawRect(x - w + 0x30000, h + y - 0x30000, (width << 16) - 0x60000, 0x30000, 0x000000, 255, INK_BLEND, false);
    RSDK.DrawRect(w - 0x30000 + x, y - h, 0x30000, height << 16, 0x000000, 255, INK_BLEND, false);
}
void UIWidgets_DrawRectOutline_Flash(int32 height, int32 width, int32 x, int32 y)
{
    int32 w    = width << 16 >> 1;
    int32 h    = height << 16 >> 1;
    colour colour = RSDK.GetPaletteEntry(3, (UIWidgets->timer >> 1) & 0xF);
    RSDK.DrawRect(x - w, y - h, width << 16, 0x30000, colour, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, y - h, 0x30000, height << 16, colour, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, h + y - 0x30000, width << 16, 0x30000, colour, 255, INK_NONE, false);
    RSDK.DrawRect(w - 0x30000 + x, y - h, 0x30000, height << 16, colour, 255, INK_NONE, false);
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
            verts[0].y = (size << 16) + y;
            verts[2].x = (size << 16) + x;
        }
        else {
            verts[1].x = (size << 16) + x;
            verts[2].y = (size << 16) + y;
        }

        if (SceneInfo->inEditor) {
            colour clr = blue | (green << 8) | (red << 16);
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
            RSDK.DrawQuad(verts, 3, red, green, blue, 0xFF, INK_NONE);
        }
    }
}
void UIWidgets_DrawEquilateralTriangle(int32 x, int32 y, int32 size, uint8 flag, int32 red, int32 green, int32 blue, InkEffects ink)
{
    Vector2 verts[3];

    if (flag) {
        verts[0].x = x;
        verts[0].y = y;
        verts[1].x = x;
        verts[1].y = y;
        verts[2].x = x;
        verts[2].y = y;
        if (flag == 1) {
            verts[0].x = x - (size << 16);
            verts[1].x = x + (size << 16);
            verts[2].y = y + (size << 16);
        }

        if (SceneInfo->inEditor) {
            colour clr = blue | (green << 8) | (red << 16);
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
            RSDK.DrawQuad(verts, 3, red, green, blue, 255, ink);
        }
    }
}
void UIWidgets_DrawParallelogram(int32 height, int32 width, int32 edgeSize, int32 red, int32 green, int32 blue, int32 x, int32 y)
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
        colour clr = blue | (green << 8) | (red << 16);
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
        RSDK.DrawQuad(verts, 4, red, green, blue, 0xFF, INK_NONE);
    }
}
void UIWidgets_DrawUpDownArrows(int32 x, int32 y, int32 arrowDist)
{
    Vector2 drawPos;

    drawPos.x                    = x;
    drawPos.y                    = y;
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

    drawPos.x                    = x;
    drawPos.y                    = y;
    UIWidgets->arrowsAnimator.frameID = 0;
    drawPos.x -= arrowDist >> 1;
    RSDK.DrawSprite(&UIWidgets->arrowsAnimator, &drawPos, false);
    UIWidgets->arrowsAnimator.frameID = 1;
    drawPos.x += arrowDist;
    RSDK.DrawSprite(&UIWidgets->arrowsAnimator, &drawPos, false);
}
Vector2 UIWidgets_DrawTriJoinRect(int32 x, int32 y, colour leftColour, colour rightColour)
{
    UIWidgets_DrawRightTriangle(x, y, 13, (leftColour >> 16) & 0xFF, (leftColour >> 8) & 0xFF, leftColour & 0xFF);
    UIWidgets_DrawRightTriangle(x + 0xE0000, y + 0xC0000, -13, (rightColour >> 16) & 0xFF, (rightColour >> 8) & 0xFF, rightColour & 0xFF);

    Vector2 result;
    result.x = x + 0xE0000;
    result.y = y;
    return result;
}

#if RETRO_USE_PLUS
void UIWidgets_DrawTime(int32 x, int32 y, int32 minutes, int32 seconds, int32 milliseconds)
{
    Vector2 drawPos;
    Animator animator;
    Animator arrowsAnimator;
    char strBuf[16];

    memset(&animator, 0, sizeof(Animator));
    memset(&arrowsAnimator, 0, sizeof(Animator));

    drawPos.x = x;
    drawPos.y = y + 0x20000;

    RSDK.SetSpriteAnimation(UIWidgets->saveSelFrames, 9, &arrowsAnimator, true, 9);
    RSDK.DrawSprite(&arrowsAnimator, &drawPos, false);
    drawPos.x += 0x100000;
    drawPos.y -= 0x20000;
    if (minutes) {
        if (minutes != 99 || seconds != 99 || milliseconds != 99) {
            sprintf(strBuf, "%02d:%02d;%02d", minutes, seconds, milliseconds);
        }
        else {
            sprintf(strBuf, "<<:<<;<<");
        }
    }
    else if (!seconds && !milliseconds) {
        sprintf(strBuf, "<<:<<;<<");
    }
    else {
        sprintf(strBuf, "%02d:%02d;%02d", minutes, seconds, milliseconds);
    }

    for (int32 i = 0; i < 8; ++i) {
        if (!strBuf[i])
            break;
        RSDK.SetSpriteAnimation(UIWidgets->saveSelFrames, 8, &animator, true, (uint8)(strBuf[i] - '0'));
        RSDK.DrawSprite(&animator, &drawPos, false);
        drawPos.x += 0x80000;
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void UIWidgets_EditorDraw(void) {}

void UIWidgets_EditorLoad(void)
{
    UIWidgets->uiSpriteIndex = RSDK.LoadSpriteAnimation("UI/UIElements.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    UIWidgets->saveSelFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
#endif
    UIWidgets->fontFrames = RSDK.LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
    UIWidgets->textFrames  = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 1, &UIWidgets->frameAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &UIWidgets->arrowsAnimator, true, 0);
#if RETRO_USE_PLUS
    UIWidgets->buttonColour = 0xF0F0F0;
#endif
}
#endif

void UIWidgets_Serialize(void) {}
