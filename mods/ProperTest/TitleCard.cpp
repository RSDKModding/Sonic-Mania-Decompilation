#include "ProperTest.h"

#include <map>
#include <array>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>

// {stage: {filter: colors}}
std::map<std::string, std::map<int, std::array<int, 7>>> colors;
std::map<int, std::vector<byte>> palette;
bool usePal         = false;
float lumaThreshold = 0.25;

void *defaultDraw = NULL;
std::vector<int> tempPal;
bool white = false;

void TitleCard_Draw(void)
{
    RSDK_THIS(TitleCard);
    auto palIDs = palette[0];
    if (palette[RSDK_sceneInfo->filter].size())
        palIDs = palette[RSDK_sceneInfo->filter];
    uint *pre = new uint[palIDs.size()];
    for (int i = 0; i < palIDs.size(); ++i) pre[i] = RSDK.GetPaletteEntry(0, palIDs[i]);
    RSDK.SetPaletteEntry(0, palIDs[0], tempPal[0]);
    Mod.Super(entity->objectID, SUPER_DRAW, NULL);
    bool check = (defaultDraw == entity->stateDraw || (entity->stateDraw && !globals->atlEnabled && !globals->suppressTitlecard));
    if (check) {
        for (int i = 1; i < palIDs.size(); i++) RSDK.SetPaletteEntry(0, palIDs[i], tempPal[i]);
        uint preW;
        if (white) {
            preW = RSDK.GetPaletteEntry(0, 1);
            RSDK.SetPaletteEntry(0, 1, 0xFFFFFF);
        }

        // redraw it bc fuck you
        entity->decorationData.frameID = 2 * (RSDK_sceneInfo->filter == SCN_FILTER_ENCORE) + 1;
        RSDK.DrawSprite(&entity->decorationData, &entity->decorationPos, true);

        if (white)
            RSDK.SetPaletteEntry(0, 1, preW);
    }
    for (int i = 0; i < (palIDs.size() - 1) * (check) + 1; ++i) RSDK.SetPaletteEntry(0, palIDs[i], pre[i]);
}

void TitleCard_Create(void *data)
{
    RSDK_THIS(TitleCard);
    Mod.Super(entity->objectID, SUPER_CREATE, data);
    if (!globals->suppressTitlecard && !defaultDraw)
        defaultDraw = (void *)entity->stateDraw;
    using namespace std;
    string stage = "";
    for (auto kv : colors) {
        if (!kv.first.length())
            continue;
        if (RSDK.CheckStageFolder(kv.first.c_str())) {
            stage = kv.first;
            break;
        }
    }
    int filter = 0;
    for (auto kv : colors[stage]) {
        if (RSDK_sceneInfo->filter == kv.first) {
            filter = kv.first;
            break;
        }
    }
    auto set = colors[stage][filter];
    for (int i = 0; i < 5; i++) entity->colours[i] = set[i];
    if (!usePal)
        return;
    white = false;
    tempPal.clear();
    tempPal.push_back(set[5]);
    auto palIDs = palette[0];
    if (palette[RSDK_sceneInfo->filter].size())
        palIDs = palette[RSDK_sceneInfo->filter];

    int r = (set[6] >> 16) & 0xFF;
    int g = (set[6] >> 8) & 0xFF;
    int b = set[6] & 0xFF;

    float L = (0.2126 * (r / (float)255) + 0.7152 * (g / (float)255) + 0.0722 * (b / (float)255));

    if (L < lumaThreshold)
        white = true;

    for (int i = 1; i < palIDs.size(); ++i) {
        float percent = ((palIDs.size() + 1 - i) * 0.8 / (float)palIDs.size());
        if (percent < 0)
            percent = 0;
        int ur = r * percent;
        int ug = g * percent;
        int ub = b * percent;
        if (white) {
            ur = (255 - r) * percent;
            ug = (255 - g) * percent;
            ub = (255 - b) * percent;
            // https://stackoverflow.com/questions/8507885/
            float U = -1;
            float W = 0;

            float oR = (ur / (float)255), oG = (ug / (float)255), oB = (ub / (float)255);
            ur = (-.402 * oR + 1.174 * oG + .228 * oB) * 255;
            ug = (.598 * oR + .174 * oG + .228 * oB) * 255;
            ub = (.599 * oR + 1.175 * oG + -.772 * oB) * 255;
            ur = clampVal(ur, 0, 255);
            ug = clampVal(ug, 0, 255);
            ub = clampVal(ub, 0, 255);
        }
        tempPal.push_back((ur << 16) | (ug << 8) | ub);
    }
    if (white)
        reverse(tempPal.begin() + 1, tempPal.end());
}

void SetStageColors(const char *stage, int *c, int count, int filter)
{
    for (int i = 0; i < count; i++) colors[stage][filter][i] = c[i];
}
void SetBaseColors(int *c, int count, int filter) { SetStageColors("", c, count, filter); }

void ResetColors()
{
    colors.clear();
    // regular
    colors[""][0][0] = 0xF08C18;
    colors[""][0][1] = 0x60C0A0;
    colors[""][0][2] = 0xF05030;
    colors[""][0][3] = 0x4060B0;
    colors[""][0][4] = 0xF0C800; // BG
    colors[""][0][5] = 0xF08C18; // act
    colors[""][0][6] = 0xF0B000; //"SONIC MANIA"
    // encore
    colors[""][5][0] = 0x3751A2;
    colors[""][5][1] = 0xC7525B;
    colors[""][5][2] = 0x428FC3;
    colors[""][5][3] = 0xDFB13F;
    colors[""][5][4] = 0x6BAE99; // BG
    colors[""][5][5] = 0x3050A8; // act
    colors[""][5][6] = 0x60A0B0; //"SONIC MANIA"
    palette.clear();
}

void LoadFromSettings(const char *id)
{
    using namespace std;
    if (Mod.GetConfigBool(id, "PZTC:Defaults", true))
        ResetColors();
    else {
        colors.clear();
        palette.clear();
    }
    usePal = Mod.GetConfigBool(id, "PZTC:ModifyPalette", true);
    if (usePal) {
        // regular
        // to index #46: FUCK you
        palette[0] = { 52, 45, 46, 44, 43, 16 };
        // encore
        palette[5] = { 56, 9, 35, 34, 33 };
    }
    lumaThreshold = (float)Mod.GetConfigInteger(id, "PZTC:LumaThreshold", 25) / 100;
    TextInfo buf;
    INIT_TEXTINFO(buf);
    while (Mod.ForeachConfigCategory(id, &buf)) {
        if (!(buf.text[0] == 'P' && buf.text[1] == 'Z' && buf.text[2] == 'T' && buf.text[3] == 'C'))
            continue;
        char *cstr = new char[buf.length];
        RSDK.GetCString(cstr, &buf);
        string str(cstr);
        string colorstr = str;
        TextInfo copyBuf;
        INIT_TEXTINFO(copyBuf);
        Mod.GetConfigString(id, (str + ":Copy").c_str(), &copyBuf, "");
        if (copyBuf.textLength) {
            char *cstr = new char[copyBuf.length];
            RSDK.GetCString(cstr, &copyBuf);
            colorstr = string("PZTC-") + cstr;
        }
        istringstream strs(str);

        string stage = "";
        int filter   = 0;
        int i        = 0;
        string buf;
        while (getline(strs, buf, '-')) {
            // i accidentally went rmg-code here, so let me explain
            if (i++ == 0)
                continue; // this increments i and checks if we're on "TTC." if we are, skip
            if (i == 4)
                break; // if we're for whatever reason on the 4th iteration (as in, after filter), skip
            try {
                filter = stoi(buf, nullptr, 0); // if we can turn it into a number,
                i      = 3;                     // assume that we just did filter and move on
            } catch (...) {
            } // if it's not a number do nothing
            if (i == 2)
                stage = buf; // set stage if we're on stage
        }
        colors[stage][filter][0] = Mod.GetConfigInteger(id, (colorstr + ":C1").c_str(), colors[""][filter][0]);
        colors[stage][filter][1] = Mod.GetConfigInteger(id, (colorstr + ":C2").c_str(), colors[""][filter][1]);
        colors[stage][filter][2] = Mod.GetConfigInteger(id, (colorstr + ":C3").c_str(), colors[""][filter][2]);
        colors[stage][filter][3] = Mod.GetConfigInteger(id, (colorstr + ":C4").c_str(), colors[""][filter][3]);
        colors[stage][filter][4] = Mod.GetConfigInteger(id, (colorstr + ":BG").c_str(), colors[""][filter][4]);
        colors[stage][filter][5] = Mod.GetConfigInteger(id, (colorstr + ":ACT").c_str(), colors[""][filter][5]);
        int decor                = colors[stage][filter][4];
        if (decor == colors[""][filter][4])
            decor = colors[""][filter][6];
        colors[stage][filter][6] = Mod.GetConfigInteger(id, (colorstr + ":DECOR").c_str(), decor);
    }
}