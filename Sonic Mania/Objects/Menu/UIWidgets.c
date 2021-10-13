#include "SonicMania.h"

ObjectUIWidgets *UIWidgets;

void UIWidgets_Update(void) {}

void UIWidgets_LateUpdate(void) {}

void UIWidgets_StaticUpdate(void)
{
    ++UIWidgets->arrayIndex;
    UIWidgets->arrayIndex &= 0x7FFF;
#if RETRO_USE_PLUS
    UIWidgets->buttonColour = UIWidgets->buttonColours[(UIWidgets->arrayIndex >> 1) & 0xF];
#endif  
}

void UIWidgets_Draw(void) {}

void UIWidgets_Create(void *data) {}

void UIWidgets_StageLoad(void)
{
    UIWidgets->active                = ACTIVE_ALWAYS;
    UIWidgets->uiSpriteIndex = RSDK.LoadSpriteAnimation("UI/UIElements.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
    UIWidgets->saveSelectSpriteIndex = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
#endif
    UIWidgets->labelSpriteIndex      = RSDK.LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
    UIWidgets_ApplyLanguage();
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 1, &UIWidgets->animator1, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &UIWidgets->animator2, true, 0);
    UIWidgets->sfx_Bleep  = RSDK.GetSFX("Global/MenuBleep.wav");
    UIWidgets->sfx_Accept = RSDK.GetSFX("Global/MenuAccept.wav");
    UIWidgets->sfx_Warp   = RSDK.GetSFX("Global/SpecialWarp.wav");
    UIWidgets->sfx_Event  = RSDK.GetSFX("Special/Event.wav");
    UIWidgets->sfx_Woosh  = RSDK.GetSFX("Global/MenuWoosh.wav");
    UIWidgets->sfx_Fail   = RSDK.GetSFX("Stage/Fail.wav");

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
        case LANGUAGE_EN: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE); break;
        case LANGUAGE_FR: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextFR.bin", SCOPE_STAGE); break;
        case LANGUAGE_IT: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextIT.bin", SCOPE_STAGE); break;
        case LANGUAGE_GE: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextGE.bin", SCOPE_STAGE); break;
        case LANGUAGE_SP: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextSP.bin", SCOPE_STAGE); break;
        case LANGUAGE_JP: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextJP.bin", SCOPE_STAGE); break;
#if RETRO_GAMEVER != VER_100
        case LANGUAGE_KO: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextTC.bin", SCOPE_STAGE); break;
#endif
        default: break;
    }
}

void UIWidgets_Unknown2(int height, int width, int x, int y)
{
    int w = width << 16 >> 1;
    int h = height << 16 >> 1;
    RSDK.DrawRect(x - w, y - h, width << 16, 0x30000, 0x000000, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, y - h, 0x30000, height << 16, 0x000000, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, h + y - 0x30000, width << 16, 0x30000, 0x000000, 255, INK_NONE, false);
    RSDK.DrawRect(w - 0x30000 + x, y - h, 0x30000, height << 16, 0x000000, 255, INK_NONE, false);
}
void UIWidgets_Unknown3(int height, int width, int x, int y)
{
    int w = width << 16 >> 1;
    int h = height << 16 >> 1;
    RSDK.DrawRect(x - w + 0x30000, y - h, (width << 16) - 0x60000, 0x30000, 0x000000, 255, INK_BLEND, false);
    RSDK.DrawRect(x - w, y - h, 0x30000, height << 16, 0x000000, 255, INK_BLEND, false);
    RSDK.DrawRect(x - w + 0x30000, h + y - 0x30000, (width << 16) - 0x60000, 0x30000, 0x000000, 255, INK_BLEND, false);
    RSDK.DrawRect(w - 0x30000 + x, y - h, 0x30000, height << 16, 0x000000, 255, INK_BLEND, false);
}
void UIWidgets_Unknown4(int height, int width, int x, int y)
{
    int w    = width << 16 >> 1;
    int h    = height << 16 >> 1;
    colour colour = RSDK.GetPaletteEntry(3, (UIWidgets->arrayIndex >> 1) & 0xF);
    RSDK.DrawRect(x - w, y - h, width << 16, 0x30000, colour, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, y - h, 0x30000, height << 16, colour, 255, INK_NONE, false);
    RSDK.DrawRect(x - w, h + y - 0x30000, width << 16, 0x30000, colour, 255, INK_NONE, false);
    RSDK.DrawRect(w - 0x30000 + x, y - h, 0x30000, height << 16, colour, 255, INK_NONE, false);
}
void UIWidgets_Unknown5(int red, int a2, int green, int blue, int x, int y)
{
    Vector2 verts[3];

    if (a2) {
        verts[0].x = x;
        verts[0].y = y;
        verts[1].x = x;
        verts[1].y = y;
        verts[2].x = x;
        verts[2].y = y;

        if (a2 < 0) {
            verts[0].y = (a2 << 16) + y;
            verts[2].x = (a2 << 16) + x;
        }
        else {
            verts[1].x = (a2 << 16) + x;
            verts[2].y = (a2 << 16) + y;
        }

        if (RSDK_sceneInfo->inEditor) {
            colour clr = blue | (green << 8) | (red << 16);
            RSDK.DrawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, clr, 0xFF, INK_NONE, false);
            RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 0xFF, INK_NONE, false);
            RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, clr, 0xFF, INK_NONE, false);
        }
        else {
            int sx = RSDK_screens->position.x << 16;
            int sy = RSDK_screens->position.y << 16;
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
void UIWidgets_Unknown6(char a1, int red, int green, int blue, InkEffects ink, int x, int y)
{
    Vector2 verts[3];

    if (a1) {
        verts[0].x = x;
        verts[0].y = y;
        verts[1].x = x;
        verts[1].y = y;
        verts[2].x = x;
        verts[2].y = y;
        if (a1 == 1) {
            verts[0].x = x - 0x80000;
            verts[1].x = x + 0x80000;
            verts[2].y = y + 0x80000;
        }

        if (RSDK_sceneInfo->inEditor) {
            colour clr = blue | (green << 8) | (red << 16);
            RSDK.DrawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, clr, 0xFF, ink, false);
            RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 0xFF, ink, false);
            RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, clr, 0xFF, ink, false);
        }
        else {
            int sx = RSDK_screens->position.x << 16;
            int sy = RSDK_screens->position.y << 16;
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
void UIWidgets_Unknown7(int a1, int a2, int a3, int red, int green, int blue, int a7, int a8)
{
    Vector2 verts[4];

    int v8     = a7 - (a2 << 15);
    verts[0].x = v8;
    int v9     = a7 + (a2 << 15);
    int v10    = v8;
    int v11    = a1 << 15;
    int Y1     = a8 - v11;
    verts[0].y = a8 - v11;
    verts[1].y = a8 - v11;
    verts[3].y = v11 + a8;
    int v12    = v9;
    verts[2].y = v11 + a8;
    int v13    = a3 << 16;
    int v18    = v9;
    verts[1].x = v9;
    verts[3].x = v8;
    verts[2].x = v9;
    if (a3 << 16 <= 0) {
        v8 -= v13;
        v12        = v9 + v13;
        verts[0].x = v8;
        verts[2].x = v9 + v13;
    }
    else {
        v9 += v13;
        v10        = v8 - v13;
        v18        = v9;
        verts[1].x = v9;
        verts[3].x = v8 - v13;
    }

    if (RSDK_sceneInfo->inEditor) {
        colour clr = blue | (green << 8) | (red << 16);
        RSDK.DrawLine(v8, Y1, v9, verts[1].y, clr, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 255, INK_NONE, false);
        RSDK.DrawLine(verts[2].x, verts[2].y, verts[3].x, verts[3].y, clr, 255, INK_NONE, false);
        RSDK.DrawLine(verts[3].x, verts[3].y, verts[0].x, verts[0].y, clr, 255, INK_NONE, false);
    }
    else {
        int sx = RSDK_screens->position.x << 16;
        int sy = RSDK_screens->position.y << 16;
        verts[1].y -= sy;
        verts[2].y -= sy;
        verts[3].y -= sy;
        verts[0].y = Y1 - sy;
        verts[0].x = v8 - sx;
        verts[1].x = v18 - sx;
        verts[2].x = v12 - sx;
        verts[3].x = v10 - sx;
        RSDK.DrawQuad(verts, 4, red, green, blue, 0xFF, INK_NONE);
    }
}
void UIWidgets_Unknown8(int a1, int x, int y)
{
    Vector2 drawPos;

    drawPos.x                    = x;
    drawPos.y                    = y;
    UIWidgets->animator2.frameID = 2;
    drawPos.y -= a1 << 15;
    RSDK.DrawSprite(&UIWidgets->animator2, &drawPos, false);
    UIWidgets->animator2.frameID = 3;
    drawPos.y += a1 << 16;
    RSDK.DrawSprite(&UIWidgets->animator2, &drawPos, false);
}
void UIWidgets_Unknown9(int a1, int x, int y)
{
    Vector2 drawPos;

    drawPos.x                    = x;
    drawPos.y                    = y;
    UIWidgets->animator2.frameID = 0;
    drawPos.x -= a1 >> 1;
    RSDK.DrawSprite(&UIWidgets->animator2, &drawPos, false);
    UIWidgets->animator2.frameID = 1;
    drawPos.x += a1;
    RSDK.DrawSprite(&UIWidgets->animator2, &drawPos, false);
}
Vector2 UIWidgets_Unknown10(colour colour1, colour colour2, int drawX, int drawY)
{
    UIWidgets_Unknown5((colour1 >> 16) & 0xFF,  13, (colour1 >> 8) & 0xFF, colour1 & 0xFF, drawX, drawY);
    UIWidgets_Unknown5((colour2 >> 16) & 0xFF, -13, (colour2 >> 8) & 0xFF, colour2 & 0xFF, drawX + 0xE0000, drawY + 0xC0000);

    Vector2 result;
    result.x = drawX + 0xE0000;
    result.y = drawY;
    return result;
}

#if RETRO_USE_PLUS
void UIWidgets_Unknown11(int minutes, int seconds, int milliseconds, int x, int y)
{
    Vector2 drawPos;
    Animator animator;
    Animator animator2;
    char strBuf[16];

    memset(&animator, 0, sizeof(Animator));
    memset(&animator2, 0, sizeof(Animator));

    drawPos.x = x;
    drawPos.y = y + 0x20000;

    RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 9, &animator2, true, 9);
    RSDK.DrawSprite(&animator2, &drawPos, false);
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

    for (int i = 0; i < 8; ++i) {
        if (!strBuf[i])
            break;
        RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 8, &animator, true, (uint8)(strBuf[i] - '0'));
        RSDK.DrawSprite(&animator, &drawPos, false);
        drawPos.x += 0x80000;
    }
}
#endif

void UIWidgets_EditorDraw(void) {}

void UIWidgets_EditorLoad(void) {}

void UIWidgets_Serialize(void) {}
