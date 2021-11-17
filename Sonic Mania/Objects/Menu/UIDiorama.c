#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIDiorama *UIDiorama;

void UIDiorama_Update(void)
{
    RSDK_THIS(UIDiorama);

    Entity *parent = entity->parent;
    if (parent) {
        if (parent->active != ACTIVE_ALWAYS)
            entity->timer = 2;

        entity->field_64 = parent->active;
        if (entity->field_64 == parent->active || parent->active != ACTIVE_ALWAYS) {
            if (entity->dioramaSubID != entity->dioramaID) {
                entity->timer = 12;
                UIDiorama_ManageStates(entity->dioramaID);
            }
        }
    }
    else {
        if (entity->dioramaSubID != entity->dioramaID) {
            entity->timer = 12;
            UIDiorama_ManageStates(entity->dioramaID);
        }
    }

    if (entity->timer > 0) {
        entity->timer--;
        RSDK.ProcessAnimation(&entity->animator2);
    }

    if (!entity->timer) {
        StateMachine_Run(entity->state);
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
    if (entity->timer <= 0) {
        if (SceneInfo->currentDrawGroup == entity->drawOrder) {
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            RSDK.DrawRect(entity->position.x, entity->dioramaPos.y, ScreenInfo->width, entity->dioramaSize.y, entity->maskColour, 255, INK_NONE,
                          false);
                          
            entity->dioramaPos.x  = entity->position.x;
            entity->dioramaPos.y  = entity->position.y - 0x510000;
            entity->dioramaSize.x = 0x1260000;
            entity->dioramaSize.y = 0xA20000;
            RSDK.DrawRect(entity->dioramaPos.x, entity->dioramaPos.y, entity->dioramaSize.x, entity->dioramaSize.y, entity->maskColour, 255,
                          INK_MASKED, false);
        }

        entity->inkEffect = INK_MASKED;
        StateMachine_Run(entity->stateDraw);
        entity->inkEffect = INK_NONE;
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
    }
}

void UIDiorama_Create(void *data)
{
    RSDK_THIS(UIDiorama);
    if (!SceneInfo->inEditor) {
        entity->dioramaSubID = -1;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 1, &entity->animator2, true, 0);
        entity->active    = ACTIVE_BOUNDS;
        entity->visible   = true;
        entity->drawOrder = 2;
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

    entity->dioramaSubID = dioramaID;
    RSDK.CopyPalette(((ids[entity->dioramaID] >> 3) + 1), (32 * ids[entity->dioramaID]), 0, 224, 32);
    entity->flag = true;

    // Kinda gross way of doing it but afaik its all "data" that changes its purpose based on state
    memset(&entity->field_BC, 0, offsetof(EntityUIDiorama, textInfo) - offsetof(EntityUIDiorama, field_BC));

    switch (dioramaID) {
        case 0:
            entity->stateDraw = UIDiorama_StateDraw_ManiaMode;
            switch (UIDiorama->dioramaAlt) {
                default: 
                case 0: entity->state = UIDiorama_State_ManiaMode_Alt0; break;
                case 1: entity->state = UIDiorama_State_ManiaMode_Alt1; break;
                case 2: entity->state = UIDiorama_State_ManiaMode_Alt2; break;
            }
            break;
        case 1:
            entity->stateDraw = UIDiorama_StateDraw_PlusUpsell;
            entity->state     = UIDiorama_State_PlusUpsell;
            break;
        case 2:
            entity->stateDraw = UIDiorama_StateDraw_EncoreMode;
            entity->state     = UIDiorama_State_EncoreMode;
            break;
        case 3:
            entity->stateDraw = UIDiorama_StateDraw_TimeAttack;
            entity->state     = UIDiorama_State_TimeAttack;
            break;
        case 4:
            entity->stateDraw = UIDiorama_StateDraw_Competition;
            entity->state     = UIDiorama_State_Competition;
            break;
        case 5:
            entity->stateDraw = UIDiorama_StateDraw_Options;
            entity->state     = UIDiorama_State_Options;
            break;
        case 6:
            entity->stateDraw = UIDiorama_StateDraw_Extras;
            entity->state     = UIDiorama_State_Extras;
            break;
        case 7:
            entity->stateDraw = UIDiorama_StateDraw_Exit;
            entity->state     = UIDiorama_State_Exit;
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

        int32 *linePosPtr = &entity->field_D0;
        for (int32 i = 0; i < info->textLength; ++i) {
            if (info->text[linePos] == '\n' && lineCount < 3) {
                linePosPtr[lineCount] = linePos;
                ++lineCount;
            }
            ++linePos;
        }

        entity->field_CC = lineCount;
        RSDK.CopyString(&entity->textInfo, info);
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animators[6], true, 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->textInfo);
    }
}

void UIDiorama_State_ManiaMode_Alt0(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[5], true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_RUN, &entity->animators[6], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_RUN, &entity->animators[7], true, 1);
        entity->flag = false;
    }
    else {
        entity->field_C0 += 0x100;
        entity->field_C4 += 0x80;
        entity->field_C8 += 0x40;
        entity->field_BC = (entity->field_BC + 0x40000) & 0x3FFFFFFF;
        RSDK.ProcessAnimation(&entity->animators[6]);
        RSDK.ProcessAnimation(&entity->animators[7]);
    }
}

void UIDiorama_State_ManiaMode_Alt1(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[5], true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_WALK, &entity->animators[6], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_WALK, &entity->animators[7], true, 1);
        entity->flag = false;
    }
    else {
        entity->field_C0 += 256;
        entity->field_C4 += 128;
        entity->field_C8 += 64;
        entity->field_BC = (entity->field_BC + 0x20000) & 0x3FFFFFFF;
        RSDK.ProcessAnimation(&entity->animators[6]);
        RSDK.ProcessAnimation(&entity->animators[7]);
    }
}

void UIDiorama_State_ManiaMode_Alt2(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 2, &entity->animators[5], true, 5);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_RUN, &entity->animators[6], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_RUN, &entity->animators[7], true, 1);
        entity->flag       = false;
        entity->field_BC   = 0x1780000;
        entity->field_FC.x = -0xC00000;
    }
    else {
        entity->field_FC.x += 0x40000;
        entity->field_BC &= 0x3FFFFFFF;
        entity->field_C0 += 256;
        entity->field_C4 += 128;
        entity->field_C8 += 64;

        if (entity->field_FC.x >= 0xC00000)
            entity->field_FC.x -= 0x1800000;
        RSDK.ProcessAnimation(&entity->animators[6]);
        RSDK.ProcessAnimation(&entity->animators[7]);
    }
}

void UIDiorama_State_PlusUpsell(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &entity->animators[1], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &entity->animators[2], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &entity->animators[3], true, 3);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 3, &entity->animators[4], true, 4);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 11, &entity->animators[5], true, 0);

        TextInfo info;
        INIT_TEXTINFO(info);
        RSDK.SetText(&info, "", 0);
        RSDK.SetText(&entity->textInfo, "", 0);
        Localization_GetString(&info, STR_MIGHTYRAYPLUS);
        UIDiorama_SetText(&info);
        entity->field_BC   = 0;
        entity->field_FC.y = 0x520000;
        entity->field_C8   = 30;
        entity->flag       = false;
    }

    if (entity->field_C8) {
        entity->field_C8--;
    }
    else if (entity->field_BC == 1) {
        RSDK.ProcessAnimation(&entity->animators[5]);

        if (entity->field_C0) {
            entity->field_C0 -= 8;
        }
    }
    else {
        entity->field_104.y -= 0x7000;
        entity->field_FC.y += entity->field_104.y;
        if (entity->field_FC.y < 0) {
            entity->field_FC.y           = 0;
            entity->field_BC             = 1;
            entity->field_C0             = 256;
            entity->animators[4].frameID = 5;
        }
    }
}

void UIDiorama_State_EncoreMode(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 4, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 0, &entity->animators[1], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 1, &entity->animators[2], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 2, &entity->animators[3], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 3, &entity->animators[4], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->capsuleFrames, 4, &entity->animators[5], true, 0);
        entity->flag = false;
    }
    else {
        RSDK.ProcessAnimation(&entity->animators[3]);
        RSDK.ProcessAnimation(&entity->animators[4]);
        RSDK.ProcessAnimation(&entity->animators[5]);
    }
}

void UIDiorama_State_TimeAttack(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 65280;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 5, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_BORED2, &entity->animators[1], true, 3);
        entity->animators[1].loopIndex  = 3;
        entity->animators[1].frameCount = 65;
        RSDK.SetSpriteAnimation(UIDiorama->ringFrames, 0, &entity->animators[2], true, 0);
        entity->animators[2].animationSpeed = 128;
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 0, &entity->animators[3], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 1, &entity->animators[4], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->speedGateFrames, 3, &entity->animators[5], true, 0);
        entity->flag = false;
    }
    else {
        RSDK.ProcessAnimation(&entity->animators[1]);
        RSDK.ProcessAnimation(&entity->animators[2]);
    }
}

void UIDiorama_State_Competition(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 6, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 10, &entity->animators[1], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->ringFrames, 0, &entity->animators[2], true, 0);
        entity->animators[2].animationSpeed = 128;
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_FLY, &entity->animators[3], true, 0);
        entity->field_BC = 85;
        RSDK.SetSpriteAnimation(UIDiorama->knuxFramesHCZ, 4, &entity->animators[4], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->rayFrames, ANI_FLY, &entity->animators[5], true, 5);
        entity->field_C0 = 0;
        RSDK.SetSpriteAnimation(UIDiorama->mightyFrames, ANI_LOOKUP, &entity->animators[6], true, 5);
        entity->flag = false;
    }
    else {
        entity->field_C4 += 64;
        entity->field_C8 += 24;
        entity->field_CC += 32;
        entity->field_D0 += 96;
        entity->field_D4 += 48;
        entity->field_DC += 24;
        entity->field_E0 += 32;
        entity->field_E4 += 48;
        entity->field_E8 += 64;

        entity->field_FC.y  = (RSDK.Sin512(UIWidgets->arrayIndex) + 512) << 10;
        entity->field_104.x = entity->position.x + 0x2B0000;
        entity->field_104.y = entity->position.y + 0x320000;
        entity->field_104.y += RSDK.Sin1024(5 * (UIWidgets->arrayIndex + 128)) << 11;
        entity->field_104.y &= 0xFFFF0000;
        entity->field_10C.x = entity->position.x + 0x8B0000;
        entity->field_10C.y = entity->position.y - 0xE0000;
        entity->field_10C.y += RSDK.Sin256(entity->field_BC) << 10;
        entity->field_BC++;
        entity->field_BC &= 0xFF;

        entity->field_114.x = entity->field_104.x;
        entity->field_114.y = entity->field_104.y;
        if (API.CheckDLC(DLC_PLUS))
            entity->field_114.x -= 0x100000;
        entity->field_114.y -= 0x180000;
        entity->field_11C.x = entity->position.x + 0x360000;
        entity->field_11C.y = entity->position.y - 0x350000;
        entity->field_11C.y += RSDK.Sin256(entity->field_C0) << 10;
        entity->field_C0++;
        entity->field_C0 &= 0xFF;

        entity->field_124.x = entity->field_104.x + 0x100000;
        entity->field_124.y = entity->field_104.y - 0x180000;
        RSDK.ProcessAnimation(&entity->animators[1]);
        RSDK.ProcessAnimation(&entity->animators[2]);
        RSDK.ProcessAnimation(&entity->animators[3]);
        RSDK.ProcessAnimation(&entity->animators[4]);
        RSDK.ProcessAnimation(&entity->animators[5]);
    }
}

void UIDiorama_State_Options(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_TWISTER, &entity->animators[1], true, 17);
        RSDK.SetSpriteAnimation(UIDiorama->tailsFrames, ANI_TWISTER, &entity->animators[2], true, 19);
        RSDK.SetSpriteAnimation(UIDiorama->knuxFramesAIZ, 1, &entity->animators[3], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &entity->animators[4], true, 1);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &entity->animators[5], true, 2);
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 7, &entity->animators[6], true, 3);
        entity->field_FC.x = 0x560000;
        entity->field_FC.y = -0x240000;
        entity->field_BC   = 128;
        entity->field_C8   = 0;

        entity->field_104.x = 0x940000;
        entity->field_104.y = 0;
        entity->field_C0    = 256;
        entity->field_CC    = 85;

        entity->field_10C.x = 0xCA0000;
        entity->field_10C.y = -0x240000;
        entity->field_C4    = 212;
        entity->field_D0    = 170;

        entity->flag = false;
    }
    else {
        entity->field_BC    = clampVal(entity->field_BC + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        entity->field_114.y = RSDK.Sin256(entity->field_C8) << 10;
        entity->field_C8++;
        entity->field_C8 &= 0xFF;

        entity->field_C0    = clampVal(entity->field_C0 + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        entity->field_11C.y = RSDK.Sin256(entity->field_CC) << 10;
        entity->field_CC++;
        entity->field_CC &= 0xFF;

        entity->field_C4    = clampVal(entity->field_C4 + RSDK.Rand(0, 20) - 10, 0xC8, 0xFF);
        entity->field_124.y = RSDK.Sin256(entity->field_D0) << 10;
        entity->field_D0++;
        entity->field_D0 &= 0xFF;

        RSDK.ProcessAnimation(&entity->animators[3]);
    }
}

void UIDiorama_State_Extras(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 8, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->bssFrames, 8, &entity->animators[1], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->bssSonicFrames, 1, &entity->animators[2], true, 0);
        entity->animators[2].animationSpeed = 12;
        entity->flag                        = false;
    }
    else {
        RSDK.ProcessAnimation(&entity->animators[1]);
        RSDK.ProcessAnimation(&entity->animators[2]);
    }
}

void UIDiorama_State_Exit(void)
{
    RSDK_THIS(UIDiorama);
    if (entity->flag) {
        entity->maskColour = 0x00FF00;
        RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 9, &entity->animators[0], true, 0);
        RSDK.SetSpriteAnimation(UIDiorama->sonicFrames, ANI_OUTTAHERE, &entity->animators[1], true, 0);
        entity->animators[1].loopIndex  = 5;
        entity->animators[1].frameCount = 11;
        entity->flag                    = false;
    }
    else {
        if (entity->animators[1].frameCount == 11 && entity->animators[1].frameID == 10
            && entity->animators[1].animationTimer == entity->animators[1].frameDelay - 1 && ++entity->field_BC == 1) {
            entity->animators[1].loopIndex  = 14;
            entity->animators[1].frameCount = 15;
        }
        if (entity->animators[1].frameID == 14 && !entity->field_C0 && !entity->field_C4) {
            entity->field_C0    = 1;
            entity->field_104.y = -0x40000;
            entity->field_104.x = 0x10000;
        }
        if (entity->field_C0 == 1) {
            entity->field_FC.x += entity->field_104.x;
            entity->field_FC.y += entity->field_104.y;
            entity->field_104.y = entity->field_104.y + 0x3800;
            if (entity->field_FC.y > 0x800000) {
                entity->field_C0 = 0;
                entity->field_C4 = 1;
            }
        }
        RSDK.ProcessAnimation(&entity->animators[1]);
    }
}

void UIDiorama_StateDraw_ManiaMode(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;
    int32 frameWidths[] = { 0x400, 0x200, 0x200, 0x200, 0x200, 0x129 };
    int32 frameSpeeds[] = { 0x100, 0x30, 0x30, 0x30, 0x60, 0x80 };

    if (SceneInfo->currentDrawGroup == entity->drawOrder) {
        drawPos.x = entity->position.x + 0x380000;
        drawPos.y = entity->position.y + 0x1D0000;
        drawPos.x = entity->field_FC.x + entity->position.x + 0x380000;
        drawPos.y = entity->field_FC.y + entity->position.y + 0x1D0000;
        RSDK.DrawSprite(&entity->animators[7], &drawPos, false);

        drawPos.y -= 0x40000;
        drawPos.x += 0x280000;
        RSDK.DrawSprite(&entity->animators[6], &drawPos, false);

        int32 offsets[6];
        offsets[0] = 0;
        offsets[1] = entity->field_C0;
        offsets[2] = entity->field_C4;
        offsets[3] = entity->field_C8;
        offsets[4] = 0;
        offsets[5] = 0;

        for (int32 i = 0; i < 6; ++i) {
            drawPos   = entity->position;
            int32 width = frameWidths[i] << 16;

            int32 offset2 = -(int32)((offsets[i] << 8) + ((frameSpeeds[i] * (uint32)(entity->field_BC >> 4)) >> 4));
            while (offset2 < -0x10000 * frameWidths[i]) {
                offset2 += width;
            }

            drawPos.x += offset2;
            RSDK.DrawSprite(&entity->animators[i], &drawPos, false);

            drawPos.x += width;
            RSDK.DrawSprite(&entity->animators[i], &drawPos, false);
        }

        RSDK.DrawRect(entity->dioramaPos.x, entity->dioramaPos.y, entity->dioramaSize.x, entity->dioramaSize.y, 0x2001A0, 255, INK_MASKED, false);
    }
}

void UIDiorama_StateDraw_PlusUpsell(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup == entity->drawOrder) {
        RSDK.DrawSprite(&entity->animators[0], &drawPos, false);
        drawPos.x = entity->position.x + 0x500000;
        drawPos.y = entity->position.y + 0x2E0000;
        RSDK.DrawSprite(&entity->animators[2], &drawPos, false);
        RSDK.DrawSprite(&entity->animators[4], &drawPos, false);
        drawPos.y += entity->field_FC.y;
        RSDK.DrawSprite(&entity->animators[3], &drawPos, false);

        colour clr = 0x000000;
        if (entity->field_BC == 1) {
            clr = 0x01D870;
        }
        else {
            clr = 0xF0C801;
        }
        RSDK.DrawRect(entity->dioramaPos.x, entity->dioramaPos.y, entity->dioramaSize.x, entity->dioramaSize.y, clr, 255, INK_MASKED, false);
    }
    else {
        entity->inkEffect = INK_ADD;
        entity->alpha     = 255;
        if (entity->field_BC == 1) {
            drawPos.x = entity->position.x + 0x500000;
            drawPos.y = entity->position.y + 0x2E0000;
            RSDK.DrawSprite(&entity->animators[5], &drawPos, false);
        }

        entity->inkEffect = INK_NONE;
        drawPos.x         = entity->position.x + 0x840000;
        drawPos.y         = entity->position.y - 0x480000;

        int32 length[5];
        length[0] = entity->field_CC;
        length[1] = entity->field_D0;
        length[2] = entity->field_D4;
        length[3] = entity->field_D8;
        length[4] = entity->field_DC;

        int32 lineCount = length[0];
        for (int32 i = 0; i < lineCount + 1; ++i) {
            int32 start = 0;
            int32 end   = 0;
            if (i > 0) {
                start = length[i] + 1;
            }
            if (i >= lineCount)
                end = entity->textInfo.textLength;
            else
                end = length[i + 1];

            int32 width = -0x8000 * RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->textInfo, start, end, 0);
            drawPos.x += width;
            RSDK.DrawText(&entity->animators[6], &drawPos, &entity->textInfo, start, end, ALIGN_LEFT, 0, 0, 0, false);

            drawPos.x -= width;
            drawPos.y += 0x120000;
        }

        drawPos           = entity->position;
        entity->inkEffect = INK_ALPHA;
        entity->alpha     = entity->field_C0;
        RSDK.DrawSprite(&entity->animators[1], &drawPos, false);

        entity->inkEffect = INK_NONE;
    }
}

void UIDiorama_StateDraw_EncoreMode(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup == entity->drawOrder) {
        entity->animators[0].frameID = 0;
        RSDK.DrawSprite(&entity->animators[0], &drawPos, false);
    }
    else {
        entity->inkEffect = INK_NONE;
        drawPos.y += 0x200000;
        drawPos.x += 0x500000;
        int32 x = drawPos.x;
        RSDK.DrawSprite(&entity->animators[2], &drawPos, false);

        drawPos.x += 0xE0000;
        RSDK.DrawSprite(&entity->animators[4], &drawPos, false);

        drawPos.x -= 0x1C0000;
        RSDK.DrawSprite(&entity->animators[5], &drawPos, false);

        drawPos.x = x;
        RSDK.DrawSprite(&entity->animators[1], &drawPos, false);

        entity->inkEffect = INK_ADD;
        entity->alpha     = 128;
        RSDK.DrawSprite(&entity->animators[3], &drawPos, false);

        entity->inkEffect            = INK_NONE;
        entity->animators[0].frameID = 1;
        RSDK.DrawSprite(&entity->animators[0], &entity->position, false);
    }
}

void UIDiorama_StateDraw_TimeAttack(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup == entity->drawOrder) {
        entity->animators[0].frameID = 0;
        RSDK.DrawSprite(&entity->animators[0], &drawPos, false);
    }
    else {
        entity->inkEffect = INK_NONE;
        drawPos.x += 0x340000;
        drawPos.y -= 0x30000;
        RSDK.DrawSprite(&entity->animators[1], &drawPos, false);

        drawPos.x = entity->position.x + 0x9B0000;
        drawPos.y = entity->position.y - 0x400000;
        for (int32 i = 0; i < 3; ++i) {
            RSDK.DrawSprite(&entity->animators[2], &drawPos, false);
            drawPos.x += 0x1C0000;
        }

        Vector2 pos;
        pos.x = entity->position.x + 0x6C0000;
        pos.y = entity->position.y - 0x130000;
        RSDK.DrawSprite(&entity->animators[4], &pos, false);
        RSDK.DrawSprite(&entity->animators[3], &pos, false);

        entity->drawFX  = FX_SCALE;
        entity->scale.y = 0x200;
        drawPos.x       = pos.x;
        drawPos.y       = pos.y;
        if (RSDK.Sin512(0) >= 0)
            entity->scale.x = RSDK.Sin512(0);
        else
            entity->scale.x = -RSDK.Sin512(0);
        drawPos.x                    = pos.x + 0x30000;
        entity->animators[5].frameID = 1;
        RSDK.DrawSprite(&entity->animators[5], &drawPos, false);

        if (RSDK.Cos512(0) >= 0)
            entity->scale.x = RSDK.Cos512(0);
        else
            entity->scale.x = -RSDK.Cos512(0);
        drawPos.x                    = pos.x - 0x30000;
        entity->animators[5].frameID = 0;
        RSDK.DrawSprite(&entity->animators[5], &drawPos, false);

        drawPos.x                    = pos.x + 0x180 * RSDK.Cos512(0);
        entity->animators[5].frameID = 1;
        RSDK.DrawSprite(&entity->animators[5], &drawPos, false);

        if (RSDK.Sin512(0) >= 0)
            entity->scale.x = RSDK.Sin512(0);
        else
            entity->scale.x = -RSDK.Sin512(0);
        drawPos.x                    = pos.x + 0xB40 * RSDK.Cos512(0);
        entity->animators[5].frameID = 2;
        RSDK.DrawSprite(&entity->animators[5], &drawPos, false);

        if (RSDK.Sin512(0) >= 0)
            entity->scale.x = RSDK.Sin512(0);
        else
            entity->scale.x = -RSDK.Sin512(0);
        drawPos.x                    = pos.x + 0x180 * RSDK.Cos512(0);
        entity->animators[5].frameID = 0;
        RSDK.DrawSprite(&entity->animators[5], &drawPos, false);

        if (RSDK.Cos512(0) >= 0)
            entity->scale.x = RSDK.Cos512(0);
        else
            entity->scale.x = -RSDK.Cos512(0);
        drawPos.x                    = pos.x - 0xB40 * RSDK.Sin512(0);
        entity->animators[5].frameID = 2;
        RSDK.DrawSprite(&entity->animators[5], &drawPos, false);

        entity->drawFX = FX_NONE;
    }
}

void UIDiorama_StateDraw_Competition(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup == entity->drawOrder) {
        drawPos.x = entity->position.x + 0xAB0000;
        drawPos.y = entity->position.y - 0x190000;
        for (int32 i = 0; i < 3; ++i) {
            drawPos.x += 0x200000;
            RSDK.DrawSprite(&entity->animators[2], &drawPos, false);
        }
        drawPos = entity->field_104;
        RSDK.DrawSprite(&entity->animators[1], &drawPos, false);

        drawPos                      = entity->position;
        entity->animators[0].frameID = 0;
        RSDK.DrawSprite(&entity->animators[0], &drawPos, false);

        int32 *val = &entity->field_C0;
        drawPos.y += entity->field_FC.y;
        for (int32 i = 1; i <= 11; ++i) {
            drawPos.x                    = entity->position.x;
            entity->animators[0].frameID = i;

            int32 offset = -(val[i - 1] << 8);
            if (offset < -0x2000000)
                offset += ((-0x2000001 - offset) & 0xFE000000) + 0x2000000;
            drawPos.x += offset;
            RSDK.DrawSprite(&entity->animators[0], &drawPos, false);

            drawPos.x += 0x2000000;
            RSDK.DrawSprite(&entity->animators[0], &drawPos, false);
        }

        RSDK.DrawRect(entity->dioramaPos.x, entity->dioramaPos.y, entity->dioramaSize.x, entity->dioramaSize.y, 0x860F0, 255, INK_MASKED, false);
    }
    else {
        entity->inkEffect   = INK_NONE;
        int32 count           = API.CheckDLC(DLC_PLUS) ? 4 : 2;
        Vector2 *drawPosPtr = &entity->field_10C;

        for (int32 i = 0; i < count; ++i) {
            RSDK.DrawSprite(&entity->animators[3 + i], &drawPosPtr[i], false);
        }
    }
}

void UIDiorama_StateDraw_Options(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup != entity->drawOrder) {
        Vector2 *offsets  = &entity->field_FC;
        Vector2 *offsets2 = &entity->field_114;
        int32 *alpha        = &entity->field_BC;

        for (int32 i = 0; i < 3; ++i) {
            drawPos = entity->position;
            drawPos.x += offsets[i].x;
            drawPos.y += offsets[i].y;
            drawPos.x += offsets2[i].x;
            drawPos.y += offsets2[i].y;

            entity->alpha     = alpha[i];
            entity->inkEffect = INK_ALPHA;
            RSDK.DrawSprite(&entity->animators[4 + i], &drawPos, false);

            entity->inkEffect = INK_ADD;
            RSDK.DrawSprite(&entity->animators[4 + i], &drawPos, false);
        }

        entity->inkEffect = INK_NONE;
        drawPos.x         = entity->position.x + 0x380000;
        drawPos.y         = entity->position.y + 0x1E0000;
        RSDK.DrawSprite(&entity->animators[3], &drawPos, false);

        drawPos.x = entity->position.x + 0x6C0000;
        drawPos.y = entity->position.y + 0x210000;
        RSDK.DrawSprite(&entity->animators[2], &drawPos, false);

        drawPos.y -= 0x30000;
        drawPos.x += 0x180000;
        RSDK.DrawSprite(&entity->animators[1], &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&entity->animators[0], &drawPos, false);
    }
}

void UIDiorama_StateDraw_Extras(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup != entity->drawOrder) {
        entity->inkEffect = INK_NONE;
        drawPos.x         = entity->position.x + 0x520000;
        drawPos.y         = entity->position.y - 0x150000;
        RSDK.DrawSprite(&entity->animators[1], &drawPos, false);

        drawPos.x = entity->position.x + 0x520000;
        drawPos.y = entity->position.y + 0x390000;
        RSDK.DrawSprite(&entity->animators[2], &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&entity->animators[0], &drawPos, false);
    }
}

void UIDiorama_StateDraw_Exit(void)
{
    RSDK_THIS(UIDiorama);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup != entity->drawOrder) {
        entity->inkEffect = INK_NONE;
        drawPos.x += 0x480000;
        drawPos.y += 0xD0000;
        drawPos.x += entity->field_FC.x;
        drawPos.y += entity->field_FC.y;
        RSDK.DrawSprite(&entity->animators[1], &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&entity->animators[0], &drawPos, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void UIDiorama_EditorDraw(void)
{
    RSDK_THIS(UIDiorama);
    RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(UIDiorama->aniFrames, 1, &entity->animator2, true, 0);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

}

void UIDiorama_EditorLoad(void) { UIDiorama->aniFrames = RSDK.LoadSpriteAnimation("UI/Diorama.bin", SCOPE_STAGE); }
#endif

void UIDiorama_Serialize(void) { RSDK_EDITABLE_VAR(UIDiorama, VAR_ENUM, dioramaID); }
#endif
