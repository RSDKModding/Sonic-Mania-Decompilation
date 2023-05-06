// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIDiorama Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectUIDiorama *UIDiorama;

void UIDiorama_Update(void)
{
    RSDK_THIS(UIDiorama);

    bool32 changeDiorama = self->lastDioramaID != self->dioramaID;

    EntityUIControl *parent = self->parent;
    if (parent) {
        if (parent->active != ACTIVE_ALWAYS)
            self->timer = 2;

        if (self->parentActivity != parent->active && parent->active == ACTIVE_ALWAYS)
            changeDiorama = true;

        self->parentActivity = parent->active;
    }

    if (changeDiorama) {
        self->timer = 12;
        UIDiorama_ChangeDiorama(self->dioramaID);
    }

    if (self->timer > 0) {
        self->timer--;
        RSDK.ProcessAnimation(&self->staticAnimator);
    }

    if (!self->timer) {
        StateMachine_Run(self->state);
    }
}

void UIDiorama_LateUpdate(void) {}

void UIDiorama_StaticUpdate(void)
{
    if (!(UIWidgets->timer & 3))
        RSDK.RotatePalette(0, 60, 63, true);

    foreach_all(UIDiorama, diorama) { RSDK.AddDrawListRef(diorama->drawGroup + 1, RSDK.GetEntitySlot(diorama)); }
}

void UIDiorama_Draw(void)
{
    RSDK_THIS(UIDiorama);

    if (self->timer <= 0) {
        if (SceneInfo->currentDrawGroup == self->drawGroup) {
            RSDK.DrawSprite(&self->maskAnimator, NULL, false);

            self->dioramaPos.x  = self->position.x;
            self->dioramaPos.y  = self->position.y - 0x510000;
            self->dioramaSize.x = 0x1260000;
            self->dioramaSize.y = 0xA20000;
            // Draw Lime BG Rect to clear the screen, ONLY draw over the already existing lime pixels
            // Extra Note: as far as I can tell this doesn't *actually* do anything, the sprite already has a lime area setup
            RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, self->maskColor, 255, INK_MASKED, false);
        }

        self->inkEffect = INK_MASKED;
        StateMachine_Run(self->stateDraw);
        self->inkEffect = INK_NONE;
    }
    else {
        RSDK.DrawSprite(&self->staticAnimator, NULL, false);
    }
}

void UIDiorama_Create(void *data)
{
    RSDK_THIS(UIDiorama);

    if (!SceneInfo->inEditor) {
        self->lastDioramaID = -1;

        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 0, &self->maskAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 1, &self->staticAnimator, true, 0);

        self->active    = ACTIVE_BOUNDS;
        self->visible   = true;
        self->drawGroup = 2;
    }
}

void UIDiorama_StageLoad(void)
{
    UIDiorama->active = ACTIVE_ALWAYS;

    RSDK.SetPaletteMask(0x00FF00);

    UIDiorama->aniFrames       = RSDK.LoadSpriteAnimation("UI/Diorama.bin", SCOPE_STAGE);
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

    UIDiorama->dioramaAlt = UIDIORAMA_ALT_JOG;
    RSDK.AddViewableVariable("Diorama Alt", &UIDiorama->dioramaAlt, VIEWVAR_UINT8, UIDIORAMA_ALT_RUN, UIDIORAMA_ALT_LOOP);
}

void UIDiorama_ChangeDiorama(uint8 dioramaID)
{
    RSDK_THIS(UIDiorama);
    int32 ids[] = { 0x00, 0x0C, 0x0C, 0x01, 0x03, 0x0F, 0x0D, 0x0E };

    self->lastDioramaID = dioramaID;
    RSDK.CopyPalette(((ids[self->dioramaID] >> 3) + 1), (32 * ids[self->dioramaID]), 0, 224, 32);
    self->needsSetup = true;

    int32 size = sizeof(int32) + sizeof(Vector2) + sizeof(Animator);
    memset(self->values, 0, size * 16);

    switch (dioramaID) {
        case UIDIORAMA_MANIAMODE:
            self->stateDraw = UIDiorama_Draw_ManiaMode;

            switch (UIDiorama->dioramaAlt) {
                default:
                case UIDIORAMA_ALT_RUN: self->state = UIDiorama_State_ManiaMode_Alt_Run; break;
                case UIDIORAMA_ALT_JOG: self->state = UIDiorama_State_ManiaMode_Alt_Jog; break;
                case UIDIORAMA_ALT_LOOP: self->state = UIDiorama_State_ManiaMode_Alt_Loop; break;
            }
            break;

        case UIDIORAMA_PLUSUPSELL:
            self->stateDraw = UIDiorama_Draw_PlusUpsell;
            self->state     = UIDiorama_State_PlusUpsell;
            break;

        case UIDIORAMA_ENCOREMODE:
            self->stateDraw = UIDiorama_Draw_EncoreMode;
            self->state     = UIDiorama_State_EncoreMode;
            break;

        case UIDIORAMA_TIMEATTACK:
            self->stateDraw = UIDiorama_Draw_TimeAttack;
            self->state     = UIDiorama_State_TimeAttack;
            break;

        case UIDIORAMA_COMPETITION:
            self->stateDraw = UIDiorama_Draw_Competition;
            self->state     = UIDiorama_State_Competition;
            break;

        case UIDIORAMA_OPTIONS:
            self->stateDraw = UIDiorama_Draw_Options;
            self->state     = UIDiorama_State_Options;
            break;

        case UIDIORAMA_EXTRAS:
            self->stateDraw = UIDiorama_Draw_Extras;
            self->state     = UIDiorama_State_Extras;
            break;

        case UIDIORAMA_EXIT:
            self->stateDraw = UIDiorama_Draw_Exit;
            self->state     = UIDiorama_State_Exit;
            break;

        default: break;
    }
}

void UIDiorama_SetText(String *text)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_PlusUpsell *info = (UIDiorama_StateInfo_PlusUpsell *)self->values;

    if (text) {
        int32 lineCount = 0;
        int32 linePos   = 0;

        int32 *linePosPtr = info->linePos;
        for (int32 i = 0; i < text->length; ++i) {
            if (text->chars[linePos] == '\n' && lineCount < 3) {
                linePosPtr[lineCount] = linePos;
                ++lineCount;
            }
            ++linePos;
        }

        info->lineCount = lineCount;
        RSDK.CopyString(&self->texts[0], text);
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &info->textAnimator, true, 0);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->texts[0]);
    }
}

void UIDiorama_State_ManiaMode_Alt_Run(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_ManiaMode *info = (UIDiorama_StateInfo_ManiaMode *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->terrainAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds1Animator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds2Animator, true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds3Animator, true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->mountainsAnimator, true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->backgroundAnimator, true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_RUN, &info->sonicAnimator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_RUN, &info->tailsAnimator, true, 1);
        self->needsSetup = false;
    }
    else {
        info->clouds1ScrollPos += 0x100;
        info->clouds2ScrollPos += 0x80;
        info->clouds3ScrollPos += 0x40;
        info->scrollPos = (info->scrollPos + 0x40000) & 0x3FFFFFFF;
        RSDK.ProcessAnimation(&info->sonicAnimator);
        RSDK.ProcessAnimation(&info->tailsAnimator);
    }
}

void UIDiorama_State_ManiaMode_Alt_Jog(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_ManiaMode *info = (UIDiorama_StateInfo_ManiaMode *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->terrainAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds1Animator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds2Animator, true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds3Animator, true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->mountainsAnimator, true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->backgroundAnimator, true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_WALK, &info->sonicAnimator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_WALK, &info->tailsAnimator, true, 1);
        self->needsSetup = false;
    }
    else {
        info->clouds1ScrollPos += 0x100;
        info->clouds2ScrollPos += 0x80;
        info->clouds3ScrollPos += 0x40;
        info->scrollPos = (info->scrollPos + 0x20000) & 0x3FFFFFFF;
        RSDK.ProcessAnimation(&info->sonicAnimator);
        RSDK.ProcessAnimation(&info->tailsAnimator);
    }
}

void UIDiorama_State_ManiaMode_Alt_Loop(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_ManiaMode *info = (UIDiorama_StateInfo_ManiaMode *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->terrainAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds1Animator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds2Animator, true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->clouds3Animator, true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->mountainsAnimator, true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &info->backgroundAnimator, true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_RUN, &info->sonicAnimator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_RUN, &info->tailsAnimator, true, 1);
        self->needsSetup  = false;
        info->scrollPos   = 0x1780000;
        info->playerPos.x = -0xC00000;
    }
    else {
        info->playerPos.x += 0x40000;
        if (info->playerPos.x >= 0xC00000)
            info->playerPos.x -= 0x1800000;

        info->scrollPos &= 0x3FFFFFFF;
        info->clouds1ScrollPos += 0x100;
        info->clouds2ScrollPos += 0x80;
        info->clouds3ScrollPos += 0x40;

        RSDK.ProcessAnimation(&info->sonicAnimator);
        RSDK.ProcessAnimation(&info->tailsAnimator);
    }
}

void UIDiorama_State_PlusUpsell(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_PlusUpsell *info = (UIDiorama_StateInfo_PlusUpsell *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &info->dioramaAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &info->flashAnimator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &info->logoAnimator, true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &info->plusAnimator, true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &info->arrowAnimator, true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 11, &info->lightningAnimator, true, 0);

        String text;
        INIT_STRING(text);
        RSDK.InitString(&text, "", 0);
        RSDK.InitString(&self->texts[0], "", 0);
        Localization_GetString(&text, STR_MIGHTYRAYPLUS);
        UIDiorama_SetText(&text);
        info->showFlash  = false;
        info->plusPos.y  = 0x520000;
        info->delay      = 30;
        self->needsSetup = false;
    }

    if (info->delay) {
        info->delay--;
    }
    else if (info->showFlash) {
        RSDK.ProcessAnimation(&info->lightningAnimator);

        if (info->flashAlpha) {
            info->flashAlpha -= 8;
        }
    }
    else {
        info->plusVelocity.y -= 0x7000;
        info->plusPos.y += info->plusVelocity.y;
        if (info->plusPos.y < 0) {
            info->plusPos.y             = 0;
            info->showFlash             = true;
            info->flashAlpha            = 256;
            info->arrowAnimator.frameID = 5;
        }
    }
}

void UIDiorama_State_EncoreMode(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_EncoreMode *info = (UIDiorama_StateInfo_EncoreMode *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 4, &info->dioramaAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 0, &info->capsuleAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 1, &info->buttonAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 2, &info->glassAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 3, &info->mightyAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 4, &info->rayAnimator, true, 0);
        self->needsSetup = false;
    }
    else {
        RSDK.ProcessAnimation(&info->glassAnimator);
        RSDK.ProcessAnimation(&info->mightyAnimator);
        RSDK.ProcessAnimation(&info->rayAnimator);
    }
}

void UIDiorama_State_TimeAttack(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_TimeAttack *info = (UIDiorama_StateInfo_TimeAttack *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 5, &info->dioramaAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_BORED_2, &info->sonicAnimator, true, 3);
        info->sonicAnimator.loopIndex  = 3;
        info->sonicAnimator.frameCount = 65;

        RSDK.SetSpriteAnimation(UIDiorama->ringFrames, 0, &info->ringAnimator, true, 0);
        info->ringAnimator.speed = 128;

        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 0, &info->gateBaseAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 1, &info->gateTopAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 3, &info->gateFinsAnimator, true, 0);
        self->needsSetup = false;
    }
    else {
        RSDK.ProcessAnimation(&info->sonicAnimator);
        RSDK.ProcessAnimation(&info->ringAnimator);
    }
}

void UIDiorama_State_Competition(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Competition *info = (UIDiorama_StateInfo_Competition *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 6, &info->dioramaAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 10, &info->platformAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->ringFrames, 0, &info->ringAnimator, true, 0);
        info->ringAnimator.speed = 128;

        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_FLY, &info->tailsAnimator, true, 0);
        info->tailsAngle = 85;

        RSDK.SetSpriteAnimation(UIDiorama->knuxFramesHCZ, 4, &info->knuxAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->rayFrames, ANI_FLY_UP, &info->rayAnimator, true, 5);
        info->rayAngle = 0;

        RSDK.SetSpriteAnimation(UIDiorama->mightyFrames, ANI_LOOK_UP, &info->mightyAnimator, true, 5);
        self->needsSetup = false;
    }
    else {
        info->scrollPos[0] += 0x40;
        info->scrollPos[1] += 0x18;
        info->scrollPos[2] += 0x20;
        info->scrollPos[3] += 0x60;
        info->scrollPos[4] += 0x30;
        info->scrollPos[5] += 0x18;
        info->scrollPos[6] += 0x20;
        info->scrollPos[7] += 0x30;
        info->scrollPos[8] += 0x40;
        // These 2 don't scroll
        // info->scrollPos[9] += 0x00;
        // info->scrollPos[10] += 0x00;

        info->terrainPos.y = (RSDK.Sin512(UIWidgets->timer) + 512) << 10;

        info->platformPos.x = self->position.x + 0x2B0000;
        info->platformPos.y = self->position.y + 0x320000;
        info->platformPos.y += RSDK.Sin1024(5 * (UIWidgets->timer + 128)) << 11;
        info->platformPos.y &= 0xFFFF0000;

        info->tailsPos.x = self->position.x + 0x8B0000;
        info->tailsPos.y = self->position.y - 0xE0000;
        info->tailsPos.y += RSDK.Sin256(info->tailsAngle) << 10;
        info->tailsAngle = (info->tailsAngle + 1) & 0xFF;

        info->knuxPos.x = info->platformPos.x;
        info->knuxPos.y = info->platformPos.y - 0x180000;
        if (API.CheckDLC(DLC_PLUS))
            info->knuxPos.x -= 0x100000;

        info->rayPos.x = self->position.x + 0x360000;
        info->rayPos.y = self->position.y - 0x350000;
        info->rayPos.y += RSDK.Sin256(info->rayAngle) << 10;
        info->rayAngle = (info->rayAngle + 1) & 0xFF;

        info->mightyPos.x = info->platformPos.x + 0x100000;
        info->mightyPos.y = info->platformPos.y - 0x180000;

        RSDK.ProcessAnimation(&info->platformAnimator);
        RSDK.ProcessAnimation(&info->ringAnimator);
        RSDK.ProcessAnimation(&info->tailsAnimator);
        RSDK.ProcessAnimation(&info->knuxAnimator);
        RSDK.ProcessAnimation(&info->rayAnimator);
    }
}

void UIDiorama_State_Options(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Options *info = (UIDiorama_StateInfo_Options *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &info->dioramaAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_TWISTER, &info->sonicAnimator, true, 17);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_TWISTER, &info->tailsAnimator, true, 19);
        RSDK.SetSpriteAnimation(UIDiorama->knuxFramesAIZ, 1, &info->knuxAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &info->itemConstellationAnimator, true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &info->contConstellationAnimator, true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &info->audioConstellationAnimator, true, 3);
        info->itemPos.x = 0x560000;
        info->itemPos.y = -0x240000;
        info->itemAlpha = 0x80;
        info->itemAngle = 0;

        info->contPos.x = 0x940000;
        info->contPos.y = 0;
        info->contAlpha = 0x100;
        info->contAngle = 85;

        info->audioPos.x = 0xCA0000;
        info->audioPos.y = -0x240000;
        info->audioAlpha = 0xD4;
        info->audioAngle = 170;

        self->needsSetup = false;
    }
    else {
        info->itemAlpha    = CLAMP(info->itemAlpha + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        info->itemOffset.y = RSDK.Sin256(info->itemAngle) << 10;
        info->itemAngle    = (info->itemAngle + 1) & 0xFF;

        info->contAlpha    = CLAMP(info->contAlpha + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        info->contOffset.y = RSDK.Sin256(info->contAngle) << 10;
        info->contAngle    = (info->contAngle + 1) & 0xFF;

        info->audioAlpha    = CLAMP(info->audioAlpha + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        info->audioOffset.y = RSDK.Sin256(info->audioAngle) << 10;
        info->audioAngle    = (info->audioAngle + 1) & 0xFF;

        RSDK.ProcessAnimation(&info->knuxAnimator);
    }
}

void UIDiorama_State_Extras(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Extras *info = (UIDiorama_StateInfo_Extras *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 8, &info->dioramaAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->bssFrames, 8, &info->medalAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->bssSonicFrames, 1, &info->sonicAnimator, true, 0);
        info->sonicAnimator.speed = 12;
        self->needsSetup          = false;
    }
    else {
        RSDK.ProcessAnimation(&info->medalAnimator);
        RSDK.ProcessAnimation(&info->sonicAnimator);
    }
}

void UIDiorama_State_Exit(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Exit *info = (UIDiorama_StateInfo_Exit *)self->values;

    if (self->needsSetup) {
        self->maskColor = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 9, &info->dioramaAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_OUTTA_HERE, &info->sonicAnimator, true, 0);
        info->sonicAnimator.loopIndex  = 5;
        info->sonicAnimator.frameCount = 11;
        self->needsSetup               = false;
    }
    else {
        if (info->sonicAnimator.frameCount == 11 && info->sonicAnimator.frameID == 10
            && info->sonicAnimator.timer == info->sonicAnimator.frameDuration - 1 && ++info->animDelay == 1) {
            info->sonicAnimator.loopIndex  = 14;
            info->sonicAnimator.frameCount = 15;
        }

        if (info->sonicAnimator.frameID == 14 && !info->processVelocity && !info->isOffScreen) {
            info->processVelocity = true;
            info->sonicVelocity.y = -0x40000;
            info->sonicVelocity.x = 0x10000;
        }

        if (info->processVelocity) {
            info->sonicPos.x += info->sonicVelocity.x;
            info->sonicPos.y += info->sonicVelocity.y;
            info->sonicVelocity.y += 0x3800;

            if (info->sonicPos.y > 0x800000) {
                info->processVelocity = false;
                info->isOffScreen     = true;
            }
        }

        RSDK.ProcessAnimation(&info->sonicAnimator);
    }
}

void UIDiorama_Draw_ManiaMode(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_ManiaMode *info = (UIDiorama_StateInfo_ManiaMode *)self->values;

    Vector2 drawPos;
    int32 frameWidths[] = { 0x400, 0x200, 0x200, 0x200, 0x200, 0x129 };
    int32 frameSpeeds[] = { 0x100, 0x30, 0x30, 0x30, 0x60, 0x80 };

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        drawPos.x = self->position.x + 0x380000;
        drawPos.y = self->position.y + 0x1D0000;
        drawPos.x = info->playerPos.x + self->position.x + 0x380000;
        drawPos.y = info->playerPos.y + self->position.y + 0x1D0000;
        RSDK.DrawSprite(&info->tailsAnimator, &drawPos, false);

        drawPos.y -= 0x40000;
        drawPos.x += 0x280000;
        RSDK.DrawSprite(&info->sonicAnimator, &drawPos, false);

        // Draw Stage "Parallax"
        int32 offsets[]            = { 0, info->clouds1ScrollPos, info->clouds2ScrollPos, info->clouds3ScrollPos, 0, 0 };
        Animator *levelAnimators[] = { &info->terrainAnimator, &info->clouds1Animator,   &info->clouds2Animator,
                                       &info->clouds3Animator, &info->mountainsAnimator, &info->backgroundAnimator };

        for (int32 i = 0; i < 6; ++i) {
            drawPos     = self->position;
            int32 width = frameWidths[i] << 16;

            int32 offset2 = -(int32)((offsets[i] << 8) + ((frameSpeeds[i] * (uint32)(info->scrollPos >> 4)) >> 4));
            while (offset2 < -0x10000 * frameWidths[i]) {
                offset2 += width;
            }

            drawPos.x += offset2;
            RSDK.DrawSprite(levelAnimators[i], &drawPos, false);

            drawPos.x += width;
            RSDK.DrawSprite(levelAnimators[i], &drawPos, false);
        }

        // Draw Sky blue rect to fill in the sky BG for any pixels that aren't covered up
        RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, 0x2001A0, 255, INK_MASKED, false);
    }
}

void UIDiorama_Draw_PlusUpsell(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_PlusUpsell *info = (UIDiorama_StateInfo_PlusUpsell *)self->values;

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);

        drawPos.x = self->position.x + 0x500000;
        drawPos.y = self->position.y + 0x2E0000;
        RSDK.DrawSprite(&info->logoAnimator, &drawPos, false);
        RSDK.DrawSprite(&info->arrowAnimator, &drawPos, false);

        drawPos.y += info->plusPos.y;
        RSDK.DrawSprite(&info->plusAnimator, &drawPos, false);

        color rectColor = info->showFlash ? 0x01D870 : 0xF0C801;
        RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, rectColor, 0xFF, INK_MASKED, false);
    }
    else {
        self->inkEffect = INK_ADD;
        self->alpha     = 0xFF;
        if (info->showFlash) {
            drawPos.x = self->position.x + 0x500000;
            drawPos.y = self->position.y + 0x2E0000;
            RSDK.DrawSprite(&info->lightningAnimator, &drawPos, false);
        }

        self->inkEffect = INK_NONE;
        drawPos.x       = self->position.x + 0x840000;
        drawPos.y       = self->position.y - 0x480000;

        int32 length[5];
        length[0] = info->lineCount;
        length[1] = info->linePos[0];
        length[2] = info->linePos[1];
        length[3] = info->linePos[2];
        length[4] = info->linePos[3];

        int32 lineCount = length[0];
        for (int32 i = 0; i < lineCount + 1; ++i) {
            int32 start = 0;
            int32 end   = 0;
            if (i > 0)
                start = length[i] + 1;

            if (i >= lineCount)
                end = self->texts[0].length;
            else
                end = length[i + 1];

            int32 width = -0x8000 * RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->texts[0], start, end, 0);
            drawPos.x += width;
            RSDK.DrawText(&info->textAnimator, &drawPos, &self->texts[0], start, end, ALIGN_LEFT, 0, NULL, NULL, false);

            drawPos.x -= width;
            drawPos.y += 0x120000;
        }

        drawPos         = self->position;
        self->inkEffect = INK_ALPHA;
        self->alpha     = info->flashAlpha;
        RSDK.DrawSprite(&info->flashAnimator, &drawPos, false);

        self->inkEffect = INK_NONE;
    }
}

void UIDiorama_Draw_EncoreMode(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_EncoreMode *info = (UIDiorama_StateInfo_EncoreMode *)self->values;

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        info->dioramaAnimator.frameID = 0;
        RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);
    }
    else {
        self->inkEffect = INK_NONE;
        drawPos.y += 0x200000;
        drawPos.x += 0x500000;
        int32 x = drawPos.x;
        RSDK.DrawSprite(&info->buttonAnimator, &drawPos, false);

        drawPos.x += 0xE0000;
        RSDK.DrawSprite(&info->mightyAnimator, &drawPos, false);

        drawPos.x -= 0x1C0000;
        RSDK.DrawSprite(&info->rayAnimator, &drawPos, false);

        drawPos.x = x;
        RSDK.DrawSprite(&info->capsuleAnimator, &drawPos, false);

        self->inkEffect = INK_ADD;
        self->alpha     = 0x80;
        RSDK.DrawSprite(&info->glassAnimator, &drawPos, false);

        self->inkEffect               = INK_NONE;
        info->dioramaAnimator.frameID = 1;
        RSDK.DrawSprite(&info->dioramaAnimator, &self->position, false);
    }
}

void UIDiorama_Draw_TimeAttack(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_TimeAttack *info = (UIDiorama_StateInfo_TimeAttack *)self->values;

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        info->dioramaAnimator.frameID = 0;
        RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);
    }
    else {
        self->inkEffect = INK_NONE;
        drawPos.x += 0x340000;
        drawPos.y -= 0x30000;
        RSDK.DrawSprite(&info->sonicAnimator, &drawPos, false);

        drawPos.x = self->position.x + 0x9B0000;
        drawPos.y = self->position.y - 0x400000;
        for (int32 i = 0; i < 3; ++i) {
            RSDK.DrawSprite(&info->ringAnimator, &drawPos, false);
            drawPos.x += 0x1C0000;
        }

        Vector2 gatePos;
        gatePos.x = self->position.x + 0x6C0000;
        gatePos.y = self->position.y - 0x130000;
        RSDK.DrawSprite(&info->gateTopAnimator, &gatePos, false);
        RSDK.DrawSprite(&info->gateBaseAnimator, &gatePos, false);

        self->drawFX                   = FX_SCALE;
        self->scale.y                  = 0x200;
        drawPos.x                      = gatePos.x;
        drawPos.y                      = gatePos.y;
        self->scale.x                  = abs(RSDK.Sin512(0));
        drawPos.x                      = gatePos.x + 0x30000;
        info->gateFinsAnimator.frameID = 1;
        RSDK.DrawSprite(&info->gateFinsAnimator, &drawPos, false);

        self->scale.x                  = abs(RSDK.Cos512(0));
        drawPos.x                      = gatePos.x - 0x30000;
        info->gateFinsAnimator.frameID = 0;
        RSDK.DrawSprite(&info->gateFinsAnimator, &drawPos, false);

        drawPos.x                      = gatePos.x + 0x180 * RSDK.Cos512(0);
        info->gateFinsAnimator.frameID = 1;
        RSDK.DrawSprite(&info->gateFinsAnimator, &drawPos, false);

        self->scale.x                  = abs(RSDK.Sin512(0));
        drawPos.x                      = gatePos.x + 0xB40 * RSDK.Cos512(0);
        info->gateFinsAnimator.frameID = 2;
        RSDK.DrawSprite(&info->gateFinsAnimator, &drawPos, false);

        self->scale.x                  = abs(RSDK.Sin512(0));
        drawPos.x                      = gatePos.x + 0x180 * RSDK.Cos512(0);
        info->gateFinsAnimator.frameID = 0;
        RSDK.DrawSprite(&info->gateFinsAnimator, &drawPos, false);

        self->scale.x                  = abs(RSDK.Cos512(0));
        drawPos.x                      = gatePos.x - 0xB40 * RSDK.Sin512(0);
        info->gateFinsAnimator.frameID = 2;
        RSDK.DrawSprite(&info->gateFinsAnimator, &drawPos, false);

        self->drawFX = FX_NONE;
    }
}

void UIDiorama_Draw_Competition(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Competition *info = (UIDiorama_StateInfo_Competition *)self->values;

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        drawPos.x = self->position.x + 0xAB0000;
        drawPos.y = self->position.y - 0x190000;
        for (int32 i = 0; i < 3; ++i) {
            drawPos.x += 0x200000;
            RSDK.DrawSprite(&info->ringAnimator, &drawPos, false);
        }

        drawPos = info->platformPos;
        RSDK.DrawSprite(&info->platformAnimator, &drawPos, false);

        drawPos                       = self->position;
        info->dioramaAnimator.frameID = 0;
        RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);

        drawPos.y += info->terrainPos.y;
        for (int32 i = 0; i < 11; ++i) {
            drawPos.x                     = self->position.x;
            info->dioramaAnimator.frameID = i + 1;

            int32 scrollPos = -(info->scrollPos[i] << 8);
            if (scrollPos < -0x2000000)
                scrollPos += ((-0x2000000 - scrollPos) & 0xFE000000) + 0x2000000;
            drawPos.x += scrollPos;
            RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);

            drawPos.x += 0x2000000;
            RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);
        }

        RSDK.DrawRect(self->dioramaPos.x, self->dioramaPos.y, self->dioramaSize.x, self->dioramaSize.y, 0x860F0, 255, INK_MASKED, false);
    }
    else {
        self->inkEffect   = INK_NONE;
        int32 playerCount = API.CheckDLC(DLC_PLUS) ? 4 : 2;

        Vector2 *playerPos[]        = { &info->tailsPos, &info->knuxPos, &info->rayPos, &info->mightyPos };
        Animator *playerAnimators[] = { &info->tailsAnimator, &info->knuxAnimator, &info->rayAnimator, &info->mightyAnimator };

        for (int32 i = 0; i < playerCount; ++i) {
            RSDK.DrawSprite(playerAnimators[i], playerPos[i], false);
        }
    }
}

void UIDiorama_Draw_Options(void)
{
    RSDK_THIS(UIDiorama);

    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Options *info = (UIDiorama_StateInfo_Options *)self->values;

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup != self->drawGroup) {
        Vector2 *positions[]  = { &info->itemPos, &info->contPos, &info->audioPos };
        Vector2 *offsets[]    = { &info->itemOffset, &info->contOffset, &info->audioOffset };
        int32 alpha[]         = { info->itemAlpha, info->contAlpha, info->audioAlpha };
        Animator *animators[] = { &info->itemConstellationAnimator, &info->contConstellationAnimator, &info->audioConstellationAnimator };

        // Draw Constellations
        for (int32 i = 0; i < 3; ++i) {
            drawPos = self->position;
            drawPos.x += positions[i]->x;
            drawPos.y += positions[i]->y;
            drawPos.x += offsets[i]->x;
            drawPos.y += offsets[i]->y;

            self->alpha     = alpha[i];
            self->inkEffect = INK_ALPHA;
            RSDK.DrawSprite(animators[i], &drawPos, false);

            self->inkEffect = INK_ADD;
            RSDK.DrawSprite(animators[i], &drawPos, false);
        }

        self->inkEffect = INK_NONE;
        drawPos.x       = self->position.x + 0x380000;
        drawPos.y       = self->position.y + 0x1E0000;
        RSDK.DrawSprite(&info->knuxAnimator, &drawPos, false);

        drawPos.x = self->position.x + 0x6C0000;
        drawPos.y = self->position.y + 0x210000;
        RSDK.DrawSprite(&info->tailsAnimator, &drawPos, false);

        drawPos.y -= 0x30000;
        drawPos.x += 0x180000;
        RSDK.DrawSprite(&info->sonicAnimator, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);
    }
}

void UIDiorama_Draw_Extras(void)
{
    RSDK_THIS(UIDiorama);

    Vector2 drawPos;
    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Extras *info = (UIDiorama_StateInfo_Extras *)self->values;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup != self->drawGroup) {
        self->inkEffect = INK_NONE;
        drawPos.x       = self->position.x + 0x520000;
        drawPos.y       = self->position.y - 0x150000;
        RSDK.DrawSprite(&info->medalAnimator, &drawPos, false);

        drawPos.x = self->position.x + 0x520000;
        drawPos.y = self->position.y + 0x390000;
        RSDK.DrawSprite(&info->sonicAnimator, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);
    }
}

void UIDiorama_Draw_Exit(void)
{
    RSDK_THIS(UIDiorama);

    Vector2 drawPos;
    // Using this makes these states FAR more readable
    UIDiorama_StateInfo_Exit *info = (UIDiorama_StateInfo_Exit *)self->values;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup != self->drawGroup) {
        self->inkEffect = INK_NONE;
        drawPos.x += 0x480000;
        drawPos.y += 0xD0000;
        drawPos.x += info->sonicPos.x;
        drawPos.y += info->sonicPos.y;
        RSDK.DrawSprite(&info->sonicAnimator, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&info->dioramaAnimator, &drawPos, false);
    }
}

#if GAME_INCLUDE_EDITOR
void UIDiorama_EditorDraw(void)
{
    RSDK_THIS(UIDiorama);

    RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 0, &self->maskAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 1, &self->staticAnimator, true, 0);

    RSDK.DrawSprite(&self->maskAnimator, NULL, false);
    RSDK.DrawSprite(&self->staticAnimator, NULL, false);
}

void UIDiorama_EditorLoad(void) { UIDiorama->aniFrames = RSDK.LoadSpriteAnimation("UI/Diorama.bin", SCOPE_STAGE); }
#endif

void UIDiorama_Serialize(void) { RSDK_EDITABLE_VAR(UIDiorama, VAR_ENUM, dioramaID); }
#endif
