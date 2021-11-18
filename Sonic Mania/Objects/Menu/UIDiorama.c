#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIDiorama *UIDiorama;

void UIDiorama_Update(void)
{
    RSDK_THIS(UIDiorama);

    Entity *parent = self->parent;
    if (parent) {
        if (parent->active != ACTIVE_ALWAYS)
            self->timer = 2;

        self->field_64 = parent->active;
        if (self->field_64 == parent->active || parent->active != ACTIVE_ALWAYS) {
            if (self->dioramaSubID != self->dioramaID) {
                self->timer = 12;
                UIDiorama_ManageStates(self->dioramaID);
            }
        }
    }
    else {
        if (self->dioramaSubID != self->dioramaID) {
            self->timer = 12;
            UIDiorama_ManageStates(self->dioramaID);
        }
    }

    if (self->timer > 0) {
        self->timer--;
        RSDK.ProcessAnimation(&self->animator2);
    }

    if (!self->timer) {
        StateMachine_Run(self->state);
    }
}

void UIDiorama_LateUpdate(void) {}

void UIDiorama_StaticUpdate(void)
{
    if (!(UIWidgets->arrayIndex & 3))
        RSDK.RotatePalette(0, 60, 63, true);

    foreach_all(UIDiorama, diorama) { RSDK.AddDrawListRef(diorama->drawOrder + 1, RSDK.GetEntityID(diorama)); }
}

void UIDiorama_Draw(void)
{
    RSDK_THIS(UIDiorama);
    if (self->timer <= 0) {
        if (SceneInfo->currentDrawGroup == self->drawOrder) {
            RSDK.DrawSprite(&self->animator1, NULL, false);
            RSDK.DrawRect(self->position.x, self->dioramaPos.y, ScreenInfo->width, self->dioramaSize.y, self->maskColour, 255, INK_NONE,
                          false);
                          
            self->dioramaPos.x  = self->position.x;
            self->dioramaPos.y  = self->position.y - 0x510000;
            self->dioramaSize.x = 0x1260000;
            self->dioramaSize.y = 0xA20000;
            RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, self->maskColour, 255,
                          INK_MASKED, false);
        }

        self->inkEffect = INK_MASKED;
        StateMachine_Run(self->stateDraw);
        self->inkEffect = INK_NONE;
    }
    else {
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }
}

void UIDiorama_Create(void *data)
{
    RSDK_THIS(UIDiorama);
    if (!SceneInfo->inEditor) {
        self->dioramaSubID = -1;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 1, &self->animator2, true, 0);
        self->active    = ACTIVE_BOUNDS;
        self->visible   = true;
        self->drawOrder = 2;
    }
}

void UIDiorama_StageLoad(void)
{
    UIDiorama->active    = ACTIVE_ALWAYS;
    UIDiorama->aniFrames = RSDK.LoadSpriteAnimation("UI/Diorama.bin", SCOPE_STAGE);
    RSDK.SetPaletteMask(0x00FF00);
    UIDiorama->capsuleFrames   = RSDK.LoadSpriteAnimation("AIZ/SchrodingersCapsule.bin", SCOPE_STAGE);
    UIDiorama->sonicFrames     = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
    UIDiorama->tailsFrames     = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
    UIDiorama->knuxFramesAIZ   = RSDK.LoadSpriteAnimation("Players/KnuxCutsceneAIZ.bin", SCOPE_STAGE);
    UIDiorama->knuxFramesHCZ   = RSDK.LoadSpriteAnimation("Players/KnuxCutsceneHPZ.bin", SCOPE_STAGE);
    UIDiorama->mightyFrames    = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE);
    UIDiorama->rayFrames       = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE);
    UIDiorama->ringFrames      = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);
    UIDiorama->speedGateFrames = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);
    UIDiorama->bssSonicFrames  = RSDK.LoadSpriteAnimation("SpecialBS/Sonic.bin", SCOPE_STAGE);
    UIDiorama->bssFrames       = RSDK.LoadSpriteAnimation("SpecialBS/StageObjects.bin", SCOPE_STAGE);
    UIDiorama->dioramaAlt      = 1;
    RSDK.SetDebugValue("Diorama Alt", &UIDiorama->dioramaAlt, DTYPE_UINT8, 0, 2);
}

void UIDiorama_ManageStates(uint8 dioramaID)
{
    RSDK_THIS(UIDiorama);
    int32 ids[] = { 0x00, 0x0C, 0x0C, 0x01, 0x03, 0x0F, 0x0D, 0x0E };

    self->dioramaSubID = dioramaID;
    RSDK.CopyPalette(((ids[self->dioramaID] >> 3) + 1), (32 * ids[self->dioramaID]), 0, 224, 32);
    self->flag = true;

    // Kinda gross way of doing it but afaik its all "data" that changes its purpose based on state
    memset(&self->field_BC, 0, offsetof(EntityUIDiorama, textInfo) - offsetof(EntityUIDiorama, field_BC));

    switch (dioramaID) {
        case 0:
            self->stateDraw = UIDiorama_StateDraw_ManiaMode;
            switch (UIDiorama->dioramaAlt) {
                default: 
                case 0: self->state = UIDiorama_State_ManiaMode_Alt0; break;
                case 1: self->state = UIDiorama_State_ManiaMode_Alt1; break;
                case 2: self->state = UIDiorama_State_ManiaMode_Alt2; break;
            }
            break;
        case 1:
            self->stateDraw = UIDiorama_StateDraw_PlusUpsell;
            self->state     = UIDiorama_State_PlusUpsell;
            break;
        case 2:
            self->stateDraw = UIDiorama_StateDraw_EncoreMode;
            self->state     = UIDiorama_State_EncoreMode;
            break;
        case 3:
            self->stateDraw = UIDiorama_StateDraw_TimeAttack;
            self->state     = UIDiorama_State_TimeAttack;
            break;
        case 4:
            self->stateDraw = UIDiorama_StateDraw_Competition;
            self->state     = UIDiorama_State_Competition;
            break;
        case 5:
            self->stateDraw = UIDiorama_StateDraw_Options;
            self->state     = UIDiorama_State_Options;
            break;
        case 6:
            self->stateDraw = UIDiorama_StateDraw_Extras;
            self->state     = UIDiorama_State_Extras;
            break;
        case 7:
            self->stateDraw = UIDiorama_StateDraw_Exit;
            self->state     = UIDiorama_State_Exit;
            break;
        default: break;
    }
}

void UIDiorama_SetText(TextInfo *info)
{
    RSDK_THIS(UIDiorama);
    if (info) {
        int32 lineCount = 0;
        int32 linePos   = 0;

        int32 *linePosPtr = &self->field_D0;
        for (int32 i = 0; i < info->textLength; ++i) {
            if (info->text[linePos] == '\n' && lineCount < 3) {
                linePosPtr[lineCount] = linePos;
                ++lineCount;
            }
            ++linePos;
        }

        self->field_CC = lineCount;
        RSDK.CopyString(&self->textInfo, info);
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->animators[6], true, 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->textInfo);
    }
}

void UIDiorama_State_ManiaMode_Alt0(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[5], true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_RUN, &self->animators[6], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_RUN, &self->animators[7], true, 1);
        self->flag = false;
    }
    else {
        self->field_C0 += 0x100;
        self->field_C4 += 0x80;
        self->field_C8 += 0x40;
        self->field_BC = (self->field_BC + 0x40000) & 0x3FFFFFFF;
        RSDK.ProcessAnimation(&self->animators[6]);
        RSDK.ProcessAnimation(&self->animators[7]);
    }
}

void UIDiorama_State_ManiaMode_Alt1(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[5], true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_WALK, &self->animators[6], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_WALK, &self->animators[7], true, 1);
        self->flag = false;
    }
    else {
        self->field_C0 += 256;
        self->field_C4 += 128;
        self->field_C8 += 64;
        self->field_BC = (self->field_BC + 0x20000) & 0x3FFFFFFF;
        RSDK.ProcessAnimation(&self->animators[6]);
        RSDK.ProcessAnimation(&self->animators[7]);
    }
}

void UIDiorama_State_ManiaMode_Alt2(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &self->animators[5], true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_RUN, &self->animators[6], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_RUN, &self->animators[7], true, 1);
        self->flag       = false;
        self->field_BC   = 0x1780000;
        self->field_FC.x = -0xC00000;
    }
    else {
        self->field_FC.x += 0x40000;
        self->field_BC &= 0x3FFFFFFF;
        self->field_C0 += 256;
        self->field_C4 += 128;
        self->field_C8 += 64;

        if (self->field_FC.x >= 0xC00000)
            self->field_FC.x -= 0x1800000;
        RSDK.ProcessAnimation(&self->animators[6]);
        RSDK.ProcessAnimation(&self->animators[7]);
    }
}

void UIDiorama_State_PlusUpsell(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &self->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &self->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &self->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &self->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 11, &self->animators[5], true, 0);

        TextInfo info;
        INIT_TEXTINFO(info);
        RSDK.SetText(&info, "", 0);
        RSDK.SetText(&self->textInfo, "", 0);
        Localization_GetString(&info, STR_MIGHTYRAYPLUS);
        UIDiorama_SetText(&info);
        self->field_BC   = 0;
        self->field_FC.y = 0x520000;
        self->field_C8   = 30;
        self->flag       = false;
    }

    if (self->field_C8) {
        self->field_C8--;
    }
    else if (self->field_BC == 1) {
        RSDK.ProcessAnimation(&self->animators[5]);

        if (self->field_C0) {
            self->field_C0 -= 8;
        }
    }
    else {
        self->field_104.y -= 0x7000;
        self->field_FC.y += self->field_104.y;
        if (self->field_FC.y < 0) {
            self->field_FC.y           = 0;
            self->field_BC             = 1;
            self->field_C0             = 256;
            self->animators[4].frameID = 5;
        }
    }
}

void UIDiorama_State_EncoreMode(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 4, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 0, &self->animators[1], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 1, &self->animators[2], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 2, &self->animators[3], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 3, &self->animators[4], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 4, &self->animators[5], true, 0);
        self->flag = false;
    }
    else {
        RSDK.ProcessAnimation(&self->animators[3]);
        RSDK.ProcessAnimation(&self->animators[4]);
        RSDK.ProcessAnimation(&self->animators[5]);
    }
}

void UIDiorama_State_TimeAttack(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 65280;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 5, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_BORED2, &self->animators[1], true, 3);
        self->animators[1].loopIndex  = 3;
        self->animators[1].frameCount = 65;
        RSDK.SetSpriteAnimation(UIDiorama->ringFrames, 0, &self->animators[2], true, 0);
        self->animators[2].animationSpeed = 128;
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 0, &self->animators[3], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 1, &self->animators[4], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 3, &self->animators[5], true, 0);
        self->flag = false;
    }
    else {
        RSDK.ProcessAnimation(&self->animators[1]);
        RSDK.ProcessAnimation(&self->animators[2]);
    }
}

void UIDiorama_State_Competition(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 6, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 10, &self->animators[1], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->ringFrames, 0, &self->animators[2], true, 0);
        self->animators[2].animationSpeed = 128;
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_FLY, &self->animators[3], true, 0);
        self->field_BC = 85;
        RSDK.SetSpriteAnimation(UIDiorama->knuxFramesHCZ, 4, &self->animators[4], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->rayFrames, ANI_FLY, &self->animators[5], true, 5);
        self->field_C0 = 0;
        RSDK.SetSpriteAnimation(UIDiorama->mightyFrames, ANI_LOOKUP, &self->animators[6], true, 5);
        self->flag = false;
    }
    else {
        self->field_C4 += 64;
        self->field_C8 += 24;
        self->field_CC += 32;
        self->field_D0 += 96;
        self->field_D4 += 48;
        self->field_DC += 24;
        self->field_E0 += 32;
        self->field_E4 += 48;
        self->field_E8 += 64;

        self->field_FC.y  = (RSDK.Sin512(UIWidgets->arrayIndex) + 512) << 10;
        self->field_104.x = self->position.x + 0x2B0000;
        self->field_104.y = self->position.y + 0x320000;
        self->field_104.y += RSDK.Sin1024(5 * (UIWidgets->arrayIndex + 128)) << 11;
        self->field_104.y &= 0xFFFF0000;
        self->field_10C.x = self->position.x + 0x8B0000;
        self->field_10C.y = self->position.y - 0xE0000;
        self->field_10C.y += RSDK.Sin256(self->field_BC) << 10;
        self->field_BC++;
        self->field_BC &= 0xFF;

        self->field_114.x = self->field_104.x;
        self->field_114.y = self->field_104.y;
        if (API.CheckDLC(DLC_PLUS))
            self->field_114.x -= 0x100000;
        self->field_114.y -= 0x180000;
        self->field_11C.x = self->position.x + 0x360000;
        self->field_11C.y = self->position.y - 0x350000;
        self->field_11C.y += RSDK.Sin256(self->field_C0) << 10;
        self->field_C0++;
        self->field_C0 &= 0xFF;

        self->field_124.x = self->field_104.x + 0x100000;
        self->field_124.y = self->field_104.y - 0x180000;
        RSDK.ProcessAnimation(&self->animators[1]);
        RSDK.ProcessAnimation(&self->animators[2]);
        RSDK.ProcessAnimation(&self->animators[3]);
        RSDK.ProcessAnimation(&self->animators[4]);
        RSDK.ProcessAnimation(&self->animators[5]);
    }
}

void UIDiorama_State_Options(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_TWISTER, &self->animators[1], true, 17);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_TWISTER, &self->animators[2], true, 19);
        RSDK.SetSpriteAnimation(UIDiorama->knuxFramesAIZ, 1, &self->animators[3], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &self->animators[4], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &self->animators[5], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &self->animators[6], true, 3);
        self->field_FC.x = 0x560000;
        self->field_FC.y = -0x240000;
        self->field_BC   = 128;
        self->field_C8   = 0;

        self->field_104.x = 0x940000;
        self->field_104.y = 0;
        self->field_C0    = 256;
        self->field_CC    = 85;

        self->field_10C.x = 0xCA0000;
        self->field_10C.y = -0x240000;
        self->field_C4    = 212;
        self->field_D0    = 170;

        self->flag = false;
    }
    else {
        self->field_BC    = clampVal(self->field_BC + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        self->field_114.y = RSDK.Sin256(self->field_C8) << 10;
        self->field_C8++;
        self->field_C8 &= 0xFF;

        self->field_C0    = clampVal(self->field_C0 + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        self->field_11C.y = RSDK.Sin256(self->field_CC) << 10;
        self->field_CC++;
        self->field_CC &= 0xFF;

        self->field_C4    = clampVal(self->field_C4 + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        self->field_124.y = RSDK.Sin256(self->field_D0) << 10;
        self->field_D0++;
        self->field_D0 &= 0xFF;

        RSDK.ProcessAnimation(&self->animators[3]);
    }
}

void UIDiorama_State_Extras(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 8, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->bssFrames, 8, &self->animators[1], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->bssSonicFrames, 1, &self->animators[2], true, 0);
        self->animators[2].animationSpeed = 12;
        self->flag                        = false;
    }
    else {
        RSDK.ProcessAnimation(&self->animators[1]);
        RSDK.ProcessAnimation(&self->animators[2]);
    }
}

void UIDiorama_State_Exit(void)
{
    RSDK_THIS(UIDiorama);
    if (self->flag) {
        self->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 9, &self->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_OUTTAHERE, &self->animators[1], true, 0);
        self->animators[1].loopIndex  = 5;
        self->animators[1].frameCount = 11;
        self->flag                    = false;
    }
    else {
        if (self->animators[1].frameCount == 11 && self->animators[1].frameID == 10
            && self->animators[1].animationTimer == self->animators[1].frameDelay - 1 && ++self->field_BC == 1) {
            self->animators[1].loopIndex  = 14;
            self->animators[1].frameCount = 15;
        }
        if (self->animators[1].frameID == 14 && !self->field_C0 && !self->field_C4) {
            self->field_C0    = 1;
            self->field_104.y = -0x40000;
            self->field_104.x = 0x10000;
        }
        if (self->field_C0 == 1) {
            self->field_FC.x += self->field_104.x;
            self->field_FC.y += self->field_104.y;
            self->field_104.y = self->field_104.y + 0x3800;
            if (self->field_FC.y > 0x800000) {
                self->field_C0 = 0;
                self->field_C4 = 1;
            }
        }
        RSDK.ProcessAnimation(&self->animators[1]);
    }
}

void UIDiorama_StateDraw_ManiaMode(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;
    int32 frameWidths[] = { 0x400, 0x200, 0x200, 0x200, 0x200, 0x129 };
    int32 frameSpeeds[] = { 0x100, 0x30, 0x30, 0x30, 0x60, 0x80 };

    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        drawPos.x = self->position.x + 0x380000;
        drawPos.y = self->position.y + 0x1D0000;
        drawPos.x = self->field_FC.x + self->position.x + 0x380000;
        drawPos.y = self->field_FC.y + self->position.y + 0x1D0000;
        RSDK.DrawSprite(&self->animators[7], &drawPos, false);

        drawPos.y -= 0x40000;
        drawPos.x += 0x280000;
        RSDK.DrawSprite(&self->animators[6], &drawPos, false);

        int32 offsets[6];
        offsets[0] = 0;
        offsets[1] = self->field_C0;
        offsets[2] = self->field_C4;
        offsets[3] = self->field_C8;
        offsets[4] = 0;
        offsets[5] = 0;

        for (int32 i = 0; i < 6; ++i) {
            drawPos   = self->position;
            int32 width = frameWidths[i] << 16;

            int32 offset2 = -(int32)((offsets[i] << 8) + ((frameSpeeds[i] * (uint32)(self->field_BC >> 4)) >> 4));
            while (offset2 < -0x10000 * frameWidths[i]) {
                offset2 += width;
            }

            drawPos.x += offset2;
            RSDK.DrawSprite(&self->animators[i], &drawPos, false);

            drawPos.x += width;
            RSDK.DrawSprite(&self->animators[i], &drawPos, false);
        }

        RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, 0x2001A0, 255, INK_MASKED, false);
    }
}

void UIDiorama_StateDraw_PlusUpsell(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        RSDK.DrawSprite(&self->animators[0], &drawPos, false);
        drawPos.x = self->position.x + 0x500000;
        drawPos.y = self->position.y + 0x2E0000;
        RSDK.DrawSprite(&self->animators[2], &drawPos, false);
        RSDK.DrawSprite(&self->animators[4], &drawPos, false);
        drawPos.y += self->field_FC.y;
        RSDK.DrawSprite(&self->animators[3], &drawPos, false);

        colour clr = 0x000000;
        if (self->field_BC == 1) {
            clr = 0x01D870;
        }
        else {
            clr = 0xF0C801;
        }
        RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, clr, 255, INK_MASKED, false);
    }
    else {
        self->inkEffect = INK_ADD;
        self->alpha     = 255;
        if (self->field_BC == 1) {
            drawPos.x = self->position.x + 0x500000;
            drawPos.y = self->position.y + 0x2E0000;
            RSDK.DrawSprite(&self->animators[5], &drawPos, false);
        }

        self->inkEffect = INK_NONE;
        drawPos.x         = self->position.x + 0x840000;
        drawPos.y         = self->position.y - 0x480000;

        int32 length[5];
        length[0] = self->field_CC;
        length[1] = self->field_D0;
        length[2] = self->field_D4;
        length[3] = self->field_D8;
        length[4] = self->field_DC;

        int32 lineCount = length[0];
        for (int32 i = 0; i < lineCount + 1; ++i) {
            int32 start = 0;
            int32 end   = 0;
            if (i > 0) {
                start = length[i] + 1;
            }
            if (i >= lineCount)
                end = self->textInfo.textLength;
            else
                end = length[i + 1];

            int32 width = -0x8000 * RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->textInfo, start, end, 0);
            drawPos.x += width;
            RSDK.DrawText(&self->animators[6], &drawPos, &self->textInfo, start, end, ALIGN_LEFT, 0, 0, 0, false);

            drawPos.x -= width;
            drawPos.y += 0x120000;
        }

        drawPos           = self->position;
        self->inkEffect = INK_ALPHA;
        self->alpha     = self->field_C0;
        RSDK.DrawSprite(&self->animators[1], &drawPos, false);

        self->inkEffect = INK_NONE;
    }
}

void UIDiorama_StateDraw_EncoreMode(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        self->animators[0].frameID = 0;
        RSDK.DrawSprite(&self->animators[0], &drawPos, false);
    }
    else {
        self->inkEffect = INK_NONE;
        drawPos.y += 0x200000;
        drawPos.x += 0x500000;
        int32 x = drawPos.x;
        RSDK.DrawSprite(&self->animators[2], &drawPos, false);

        drawPos.x += 0xE0000;
        RSDK.DrawSprite(&self->animators[4], &drawPos, false);

        drawPos.x -= 0x1C0000;
        RSDK.DrawSprite(&self->animators[5], &drawPos, false);

        drawPos.x = x;
        RSDK.DrawSprite(&self->animators[1], &drawPos, false);

        self->inkEffect = INK_ADD;
        self->alpha     = 128;
        RSDK.DrawSprite(&self->animators[3], &drawPos, false);

        self->inkEffect            = INK_NONE;
        self->animators[0].frameID = 1;
        RSDK.DrawSprite(&self->animators[0], &self->position, false);
    }
}

void UIDiorama_StateDraw_TimeAttack(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        self->animators[0].frameID = 0;
        RSDK.DrawSprite(&self->animators[0], &drawPos, false);
    }
    else {
        self->inkEffect = INK_NONE;
        drawPos.x += 0x340000;
        drawPos.y -= 0x30000;
        RSDK.DrawSprite(&self->animators[1], &drawPos, false);

        drawPos.x = self->position.x + 0x9B0000;
        drawPos.y = self->position.y - 0x400000;
        for (int32 i = 0; i < 3; ++i) {
            RSDK.DrawSprite(&self->animators[2], &drawPos, false);
            drawPos.x += 0x1C0000;
        }

        Vector2 pos;
        pos.x = self->position.x + 0x6C0000;
        pos.y = self->position.y - 0x130000;
        RSDK.DrawSprite(&self->animators[4], &pos, false);
        RSDK.DrawSprite(&self->animators[3], &pos, false);

        self->drawFX  = FX_SCALE;
        self->scale.y = 0x200;
        drawPos.x       = pos.x;
        drawPos.y       = pos.y;
        if (RSDK.Sin512(0) >= 0)
            self->scale.x = RSDK.Sin512(0);
        else
            self->scale.x = -RSDK.Sin512(0);
        drawPos.x                    = pos.x + 0x30000;
        self->animators[5].frameID = 1;
        RSDK.DrawSprite(&self->animators[5], &drawPos, false);

        if (RSDK.Cos512(0) >= 0)
            self->scale.x = RSDK.Cos512(0);
        else
            self->scale.x = -RSDK.Cos512(0);
        drawPos.x                    = pos.x - 0x30000;
        self->animators[5].frameID = 0;
        RSDK.DrawSprite(&self->animators[5], &drawPos, false);

        drawPos.x                    = pos.x + 0x180 * RSDK.Cos512(0);
        self->animators[5].frameID = 1;
        RSDK.DrawSprite(&self->animators[5], &drawPos, false);

        if (RSDK.Sin512(0) >= 0)
            self->scale.x = RSDK.Sin512(0);
        else
            self->scale.x = -RSDK.Sin512(0);
        drawPos.x                    = pos.x + 0xB40 * RSDK.Cos512(0);
        self->animators[5].frameID = 2;
        RSDK.DrawSprite(&self->animators[5], &drawPos, false);

        if (RSDK.Sin512(0) >= 0)
            self->scale.x = RSDK.Sin512(0);
        else
            self->scale.x = -RSDK.Sin512(0);
        drawPos.x                    = pos.x + 0x180 * RSDK.Cos512(0);
        self->animators[5].frameID = 0;
        RSDK.DrawSprite(&self->animators[5], &drawPos, false);

        if (RSDK.Cos512(0) >= 0)
            self->scale.x = RSDK.Cos512(0);
        else
            self->scale.x = -RSDK.Cos512(0);
        drawPos.x                    = pos.x - 0xB40 * RSDK.Sin512(0);
        self->animators[5].frameID = 2;
        RSDK.DrawSprite(&self->animators[5], &drawPos, false);

        self->drawFX = FX_NONE;
    }
}

void UIDiorama_StateDraw_Competition(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawOrder) {
        drawPos.x = self->position.x + 0xAB0000;
        drawPos.y = self->position.y - 0x190000;
        for (int32 i = 0; i < 3; ++i) {
            drawPos.x += 0x200000;
            RSDK.DrawSprite(&self->animators[2], &drawPos, false);
        }
        drawPos = self->field_104;
        RSDK.DrawSprite(&self->animators[1], &drawPos, false);

        drawPos                      = self->position;
        self->animators[0].frameID = 0;
        RSDK.DrawSprite(&self->animators[0], &drawPos, false);

        int32 *val = &self->field_C0;
        drawPos.y += self->field_FC.y;
        for (int32 i = 1; i <= 11; ++i) {
            drawPos.x                    = self->position.x;
            self->animators[0].frameID = i;

            int32 offset = -(val[i - 1] << 8);
            if (offset < -0x2000000)
                offset += ((-0x2000001 - offset) & 0xFE000000) + 0x2000000;
            drawPos.x += offset;
            RSDK.DrawSprite(&self->animators[0], &drawPos, false);

            drawPos.x += 0x2000000;
            RSDK.DrawSprite(&self->animators[0], &drawPos, false);
        }

        RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, 0x860F0, 255, INK_MASKED, false);
    }
    else {
        self->inkEffect   = INK_NONE;
        int32 count           = API.CheckDLC(DLC_PLUS) ? 4 : 2;
        Vector2 *drawPosPtr = &self->field_10C;

        for (int32 i = 0; i < count; ++i) {
            RSDK.DrawSprite(&self->animators[3 + i], &drawPosPtr[i], false);
        }
    }
}

void UIDiorama_StateDraw_Options(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup != self->drawOrder) {
        Vector2 *offsets  = &self->field_FC;
        Vector2 *offsets2 = &self->field_114;
        int32 *alpha        = &self->field_BC;

        for (int32 i = 0; i < 3; ++i) {
            drawPos = self->position;
            drawPos.x += offsets[i].x;
            drawPos.y += offsets[i].y;
            drawPos.x += offsets2[i].x;
            drawPos.y += offsets2[i].y;

            self->alpha     = alpha[i];
            self->inkEffect = INK_ALPHA;
            RSDK.DrawSprite(&self->animators[4 + i], &drawPos, false);

            self->inkEffect = INK_ADD;
            RSDK.DrawSprite(&self->animators[4 + i], &drawPos, false);
        }

        self->inkEffect = INK_NONE;
        drawPos.x         = self->position.x + 0x380000;
        drawPos.y         = self->position.y + 0x1E0000;
        RSDK.DrawSprite(&self->animators[3], &drawPos, false);

        drawPos.x = self->position.x + 0x6C0000;
        drawPos.y = self->position.y + 0x210000;
        RSDK.DrawSprite(&self->animators[2], &drawPos, false);

        drawPos.y -= 0x30000;
        drawPos.x += 0x180000;
        RSDK.DrawSprite(&self->animators[1], &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&self->animators[0], &drawPos, false);
    }
}

void UIDiorama_StateDraw_Extras(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup != self->drawOrder) {
        self->inkEffect = INK_NONE;
        drawPos.x         = self->position.x + 0x520000;
        drawPos.y         = self->position.y - 0x150000;
        RSDK.DrawSprite(&self->animators[1], &drawPos, false);

        drawPos.x = self->position.x + 0x520000;
        drawPos.y = self->position.y + 0x390000;
        RSDK.DrawSprite(&self->animators[2], &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&self->animators[0], &drawPos, false);
    }
}

void UIDiorama_StateDraw_Exit(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup != self->drawOrder) {
        self->inkEffect = INK_NONE;
        drawPos.x += 0x480000;
        drawPos.y += 0xD0000;
        drawPos.x += self->field_FC.x;
        drawPos.y += self->field_FC.y;
        RSDK.DrawSprite(&self->animators[1], &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&self->animators[0], &drawPos, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void UIDiorama_EditorDraw(void)
{
    RSDK_THIS(UIDiorama);
    RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 1, &self->animator2, true, 0);

    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);

}

void UIDiorama_EditorLoad(void) { UIDiorama->aniFrames = RSDK.LoadSpriteAnimation("UI/Diorama.bin", SCOPE_STAGE); }
#endif

void UIDiorama_Serialize(void) { RSDK_EDITABLE_VAR(UIDiorama, VAR_ENUM, dioramaID); }
#endif
