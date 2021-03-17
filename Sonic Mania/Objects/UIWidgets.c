#include "../SonicMania.h"

ObjectUIWidgets *UIWidgets;

void UIWidgets_Update() {}

void UIWidgets_LateUpdate() {}

void UIWidgets_StaticUpdate()
{
    ++UIWidgets->arrayIndex;
    UIWidgets->arrayIndex &= 0x7FFF;
    UIWidgets->value = UIWidgets->array1[(UIWidgets->arrayIndex >> 1) & 0xF];
}

void UIWidgets_Draw() {}

void UIWidgets_Create(void *data) {}

void UIWidgets_StageLoad()
{
    UIWidgets->active                = ACTIVE_ALWAYS;
    UIWidgets->uiSpriteIndex         = RSDK.LoadSpriteAnimation("UI/UIElements.bin", SCOPE_STAGE);
    UIWidgets->saveSelectSpriteIndex = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIWidgets->labelSpriteIndex      = RSDK.LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
    UIWidgets_ApplyLanguage();
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 1, &UIWidgets->animData1, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &UIWidgets->animData2, true, 0);
    UIWidgets->sfx_Bleep  = RSDK.GetSFX("Global/MenuBleep.wav");
    UIWidgets->sfx_Accept = RSDK.GetSFX("Global/MenuAccept.wav");
    UIWidgets->sfx_Warp   = RSDK.GetSFX("Global/SpecialWarp.wav");
    UIWidgets->sfx_Event  = RSDK.GetSFX("Special/Event.wav");
    UIWidgets->sfx_Woosh  = RSDK.GetSFX("Global/MenuWoosh.wav");
    UIWidgets->sfx_Fail   = RSDK.GetSFX("Stage/Fail.wav");

    byte *arrPtr = (byte *)UIWidgets->array1;
    arrPtr[2]    = -80;
    arrPtr[1]    = 0;
    arrPtr[0]    = 0;
    arrPtr[6]    = -72;
    arrPtr[5]    = 24;
    arrPtr[4]    = 32;
    arrPtr[10]   = -68;
    arrPtr[9]    = 52;
    arrPtr[8]    = 64;
    arrPtr[14]   = -60;
    arrPtr[13]   = 76;
    arrPtr[12]   = 96;
    arrPtr[18]   = -52;
    arrPtr[17]   = 100;
    arrPtr[16]   = -128;
    arrPtr[22]   = -48;
    arrPtr[21]   = 124;
    arrPtr[20]   = -96;
    arrPtr[26]   = -40;
    arrPtr[25]   = -104;
    arrPtr[24]   = -64;
    arrPtr[30]   = -32;
    arrPtr[29]   = -80;
    arrPtr[28]   = -32;
    arrPtr[34]   = -32;
    arrPtr[33]   = -80;
    arrPtr[32]   = -32;
    arrPtr[38]   = -40;
    arrPtr[37]   = -104;
    arrPtr[36]   = -64;
    arrPtr[42]   = -44;
    arrPtr[41]   = 124;
    arrPtr[40]   = -96;
    arrPtr[46]   = -52;
    arrPtr[45]   = 100;
    arrPtr[44]   = -128;
    arrPtr[50]   = -60;
    arrPtr[49]   = 76;
    arrPtr[48]   = 96;
    arrPtr[54]   = -68;
    arrPtr[53]   = 52;
    arrPtr[52]   = 64;
    arrPtr[58]   = -72;
    arrPtr[57]   = 24;
    arrPtr[56]   = 32;
    arrPtr[62]   = -80;
    arrPtr[61]   = 0;
    arrPtr[60]   = 0;
}

void UIWidgets_ApplyLanguage()
{
    Game_Print("Apply Language %d", Localization->language);
    switch (Localization->language) {
        case LANGUAGE_EN: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE); break;
        case LANGUAGE_FR: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextFR.bin", SCOPE_STAGE); break;
        case LANGUAGE_IT: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextIT.bin", SCOPE_STAGE); break;
        case LANGUAGE_GE: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextGE.bin", SCOPE_STAGE); break;
        case LANGUAGE_SP: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextSP.bin", SCOPE_STAGE); break;
        case LANGUAGE_JP: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextJP.bin", SCOPE_STAGE); break;
        case LANGUAGE_KO: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextKO.bin", SCOPE_STAGE); break;
        case LANGUAGE_SC: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextSC.bin", SCOPE_STAGE); break;
        case LANGUAGE_TC: UIWidgets->textSpriteIndex = RSDK.LoadSpriteAnimation("UI/TextTC.bin", SCOPE_STAGE); break;
        default: break;
    }
}

void UIWidgets_Unknown2(int a1, int a2, int a3, int a4)
{
    int v4 = a1 << 16;
    int v5 = a2 << 16;
    int v6 = a1 << 16 >> 1;
    int v7 = a2 << 16 >> 1;
    RSDK.DrawRect(a3 - v7, a4 - v6, a2 << 16, 0x30000, 0, 255, 0, 0);
    RSDK.DrawRect(a3 - v7, a4 - v6, 0x30000, v4, 0, 255, 0, 0);
    RSDK.DrawRect(a3 - v7, v6 + a4 - 0x30000, v5, 0x30000, 0, 255, 0, 0);
    RSDK.DrawRect(v7 - 0x30000 + a3, a4 - v6, 0x30000, v4, 0, 255, 0, 0);
}
void UIWidgets_Unknown3(int a1, int a2, int a3, int a4)
{
    int v4 = a2 << 16;
    int v5 = a1 << 16;
    int v6 = v4 - 0x60000;
    int v7 = a1 << 16 >> 1;
    int v8 = v4 >> 1;
    RSDK.DrawRect(a3 - (v4 >> 1) + 0x30000, a4 - v7, v4 - 0x60000, 0x30000, 0, 255, INK_BLEND, 0);
    RSDK.DrawRect(a3 - v8, a4 - v7, 0x30000, v5, 0, 255, INK_BLEND, 0);
    RSDK.DrawRect(a3 - v8 + 0x30000, v7 + a4 - 0x30000, v6, 0x30000, 0, 255, INK_BLEND, 0);
    RSDK.DrawRect(v8 - 0x30000 + a3, a4 - v7, 0x30000, v5, 0, 255, INK_BLEND, 0);
}
void UIWidgets_Unknown4(int a1, int a2, int a3, int a4)
{
    int v4    = a1 << 16;
    int v5    = a1 << 16 >> 1;
    int v6    = a2 << 16;
    int v7    = a2 << 16 >> 1;
    colour v8 = RSDK.GetPaletteEntry(3, (UIWidgets->arrayIndex >> 1) & 0xF);
    RSDK.DrawRect(a3 - v7, a4 - v5, v6, 0x30000, v8, 255, 0, 0);
    RSDK.DrawRect(a3 - v7, a4 - v5, 0x30000, v4, v8, 255, 0, 0);
    RSDK.DrawRect(a3 - v7, v5 + a4 - 0x30000, v6, 0x30000, v8, 255, 0, 0);
    RSDK.DrawRect(v7 - 0x30000 + a3, a4 - v5, 0x30000, v4, v8, 255, 0, 0);
}
void UIWidgets_Unknown5(int red, int a2, int green, int blue, int x, int y)
{
    Vector2 verts[3];

    if (a2) {
        int v7     = x;
        int v8     = y;
        verts[0].x = x;
        verts[0].y = y;
        verts[1].x = x;
        verts[1].y = y;
        verts[2].x = x;
        verts[2].y = y;
        if (a2 <= 0) {
            v8         = (a2 << 16) + y;
            verts[0].y = (a2 << 16) + y;
            verts[2].x = (a2 << 16) + x;
        }
        else {
            v7         = (a2 << 16) + x;
            verts[1].x = (a2 << 16) + x;
            verts[2].y = (a2 << 16) + y;
        }

        if (RSDK_sceneInfo->inEditor) {
            colour clr = blue + ((green + (red << 8)) << 8);
            RSDK.DrawLine(x, verts[0].x, verts[0].y, verts[1].y, clr, 255, 0, 0);
            RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 255, 0, 0);
            RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, clr, 255, 0, 0);
        }
        else {
            int sx = RSDK_screens->position.x << 16;
            int sy = RSDK_screens->position.y << 16;
            verts[1].y -= sy;
            verts[2].y -= sy;
            verts[2].x -= sx;
            verts[0].x = x - sx;
            verts[0].y = y - sy;
            verts[1].x = x - sx;
            RSDK.DrawQuad(verts, 3, red, green, blue, 255, 0);
        }
    }
}
void UIWidgets_Unknown6(char a1, int red, int green, int blue, InkEffects ink, int x, int y)
{
    Vector2 verts[3];

    if (a1) {
        int v8     = x;
        int v9     = x;
        int v10    = y;
        verts[0].x = x;
        verts[0].y = y;
        verts[1].x = x;
        verts[1].y = y;
        verts[2].x = x;
        verts[2].y = y;
        if (a1 == 1) {
            v9         = x - 0x80000;
            v8         = x + 0x80000;
            v10        = y + 0x80000;
            verts[0].x = x - 0x80000;
            verts[1].x = x + 0x80000;
            verts[2].y = y + 0x80000;
        }
        if (RSDK_sceneInfo->inEditor) {
            colour clr = blue + ((green + (red << 8)) << 8);
            RSDK.DrawLine(v9, y, v8, y, clr, 255, ink, 0);
            RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 255, ink, 0);
            RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, clr, 255, ink, 0);
        }
        else {
            int sx = RSDK_screens->position.x << 16;
            int sy = RSDK_screens->position.y << 16;
            verts[1].y -= sy;
            verts[2].x -= sx;
            verts[0].x = v9 - sx;
            verts[0].y = y - sy;
            verts[1].x = v8 - sx;
            verts[2].y = v10 - sy;
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
        colour clr = blue + ((green + (red << 8)) << 8);
        RSDK.DrawLine(v8, Y1, v9, verts[1].y, clr, 255, 0, 0);
        RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, clr, 255, 0, 0);
        RSDK.DrawLine(verts[2].x, verts[2].y, verts[3].x, verts[3].y, clr, 255, 0, 0);
        RSDK.DrawLine(verts[3].x, verts[3].y, verts[0].x, verts[0].y, clr, 255, 0, 0);
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
        RSDK.DrawQuad(verts, 4, red, green, blue, 255, 0);
    }
}
void UIWidgets_Unknown8(int x, int y)
{
    Vector2 drawPos;
    int a1 = 0; //???

    drawPos.x                    = x;
    drawPos.y                    = y;
    UIWidgets->animData2.frameID = 2;
    drawPos.y -= a1 << 15;
    RSDK.DrawSprite(&UIWidgets->animData2, &drawPos, 0);
    UIWidgets->animData2.frameID = 3;
    drawPos.y += a1 << 16;
    RSDK.DrawSprite(&UIWidgets->animData2, &drawPos, 0);
}
void UIWidgets_Unknown9(int x, int y)
{
    Vector2 drawPos;
    int a1 = 0; //???

    drawPos.x                    = x;
    drawPos.y                    = y;
    UIWidgets->animData2.frameID = 0;
    drawPos.x -= a1 >> 1;
    RSDK.DrawSprite(&UIWidgets->animData2, &drawPos, 0);
    UIWidgets->animData2.frameID = 1;
    drawPos.x += a1;
    RSDK.DrawSprite(&UIWidgets->animData2, &drawPos, 0);
}
int UIWidgets_Unknown10(int a1, int a2, int a3, int a4)
{
    UIWidgets_Unknown5((a1 >> 16) & 0xFF, 13, (a1 >> 8) & 0xFF, a1 & 0xFF, a3, a4);
    UIWidgets_Unknown5((a1 >> 16) & 0xFF, -13, (a2)&0xFF, a1 & 0xFF, a3 + 0xE0000, a4 + 0xC0000);
    return a3 + 0xE0000;
}
void UIWidgets_Unknown11(int minutes, int seconds, int milliseconds, int x, int y)
{
    Vector2 drawPos;
    EntityAnimationData data;
    EntityAnimationData data2;
    char strBuf[16];

    memset(&data, 0, sizeof(EntityAnimationData));
    memset(&data2, 0, sizeof(EntityAnimationData));

    RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 9, &data2, true, 9);
    RSDK.DrawSprite(&data2, &drawPos, 0);
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
        RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 8, &data, true, (byte)(strBuf[i] - '0'));
        RSDK.DrawSprite(&data, &drawPos, 0);
        drawPos.x += 0x80000;
    }
}

void UIWidgets_EditorDraw() {}

void UIWidgets_EditorLoad() {}

void UIWidgets_Serialize() {}
