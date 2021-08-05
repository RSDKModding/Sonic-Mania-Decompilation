#include "SonicMania.h"

ObjectUISaveSlot *UISaveSlot;

void UISaveSlot_Update(void)
{
    RSDK_THIS(UISaveSlot);
    UISaveSlot_SetupButtonElements();
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex) {
        UISaveSlot_Unknown6();
        UISaveSlot_Unknown8();
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
    StateMachine_Run(entity->state);

    if (++entity->field_13C >= 192)
        entity->field_13C -= 192;

    EntityUIControl *control = (EntityUIControl *)entity->parent;

    // ???
    for (int i = 0; i < control->buttonCount; ++i) {
        if (entity == (EntityUISaveSlot *)control->buttons[i])
            break;
    }

    if (entity->flag) {
        entity->field_148 -= 0x600;
        entity->field_140 += entity->field_148;
        if (entity->field_140 >= 0x13600)
            entity->field_140 = 0x13600;

        if (entity->field_140 <= 0x10000 && entity->field_148 < 0) {
            entity->field_140 = 0x10000;
            entity->field_148 = 0;
        }

        entity->field_14C -= 0x1800;
        entity->field_144 += entity->field_14C;
        if (entity->field_144 >= 0x11800)
            entity->field_144 = 0x11800;

        if (entity->field_144 <= 0x8000 && entity->field_14C < 0) {
            entity->field_144 = 0x8000;
            entity->field_14C = 0;
        }
    }
    else if (entity->state != UISaveSlot_Unknown28) {
        if (entity->field_140 > 0) {
            entity->field_140 -= 0x2000;
            if (entity->field_140 < 0)
                entity->field_140 = 0;
        }

        if (entity->field_144 > 0) {
            entity->field_144 -= 0x2000;
            if (entity->field_144 < 0)
                entity->field_144 = 0;
        }
    }

    if (entity->zoneID && (control->buttons[control->field_D8] != (EntityUIButton *)entity || control->activeEntityID == -1)) {
        entity->flag         = false;
        entity->zoneID       = 0;
        entity->state        = UISaveSlot_Unknown22;
        entity->stateUnknown = StateMachine_None;
    }
    if (control->state != UIControl_ProcessInputs || control->active != ACTIVE_ALWAYS) {
        if (entity->zoneID) {
            entity->flag         = false;
            entity->zoneID       = 0;
            entity->state        = UISaveSlot_Unknown22;
            entity->stateUnknown = StateMachine_None;
        }
        entity->active = ACTIVE_BOUNDS;
    }
}

void UISaveSlot_LateUpdate(void) {}

void UISaveSlot_StaticUpdate(void) {}

void UISaveSlot_Draw(void)
{
    RSDK_THIS(UISaveSlot);
    Vector2 drawPos;

    if (entity->type == 0) {
        drawPos.y = entity->position.y + 0x30000;
        drawPos.x = entity->position.x + 0x30000;
        RSDK.DrawRect(entity->position.x - 0x2D0000, entity->position.y - 0x4F0000, 0x600000, 0xA40000, 0xFFFFFF, 127, INK_BLEND, false);

        drawPos.x = entity->position.x - 0x2D0000;
        drawPos.y = entity->position.y - 0x100000;
        UIWidgets_Unknown5(232, (entity->field_140 >> 11), 40, 88, drawPos.x, drawPos.y);

        drawPos.x = entity->position.x + 0x2D0000;
        drawPos.y = 0x4F0000 + entity->position.y;
        UIWidgets_Unknown5(96, (-64 * entity->field_140) >> 16, 160, 176, drawPos.x, drawPos.y);
        UIWidgets_Unknown5(88, (-44 * entity->field_140) >> 16, 112, 224, drawPos.x, drawPos.y);

        drawPos.x = entity->position.x + 0x30000;
        drawPos.y = entity->position.y + 0x30000;
        UIWidgets_Unknown3(164, 96, drawPos.x, drawPos.y);
        RSDK.DrawRect(drawPos.x - 0x2D0000, drawPos.y - 0x130000, 0x5A0000, 0x30000, 0, 255, INK_BLEND, false);

        if (entity->flag)
            UIWidgets_Unknown4(164, 96, entity->position.x, entity->position.y);
        else
            UIWidgets_Unknown2(164, 96, entity->position.x, entity->position.y);

        entity->animator1.frameID = 0;
        drawPos.x                 = entity->position.x;
        drawPos.y                 = entity->position.y - 0x300000;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        if (entity->isNewSave || entity->saveZoneID == 255) {
            RSDK.DrawSprite(&entity->animator10, &drawPos, false);
            RSDK.DrawSprite(&entity->animator11, &drawPos, false);
        }
        else if ((entity->flag || entity->state == UISaveSlot_Unknown28) && entity->saveZoneID <= 13) {
            SpriteFrame *frame = RSDK.GetFrame(UISaveSlot->aniFrames, 5, entity->saveZoneID);
            frame->pivotX      = -43;
            frame->width       = 86;
            frame->sprX        = entity->field_13C;
            if (frame->sprX > 106) {
                int width    = frame->sprX - 106;
                frame->width = 86 - width;
                RSDK.DrawSprite(&entity->animator7, &drawPos, false);
                frame->pivotX += frame->width;
                frame->sprX  = 0;
                frame->width = width;
            }
            RSDK.DrawSprite(&entity->animator7, &drawPos, false);
            RSDK.DrawSprite(&entity->animator8, &drawPos, false);
        }
        else {
            entity->drawFX = FX_FLIP;
            RSDK.DrawSprite(&entity->animator9, &drawPos, false);

            entity->drawFX = FX_NONE;
            RSDK.DrawSprite(&entity->animator8, &drawPos, false);
        }

        if (entity->isNewSave) {
            drawPos.x = entity->position.x;
            drawPos.y = entity->position.y + 0x200000;
            UISaveSlot_Unknown3(drawPos.x, drawPos.y);
        }
        else {
            RSDK.DrawRect(entity->position.x - 0x2D0000, entity->position.y + 0x3D0000, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);

            drawPos.x = entity->position.x - 0x240000;
            drawPos.y = 0x450000 + entity->position.y;
            for (int i = 0; i < 7; ++i) {
                entity->animator6.frameID = ((1 << i) & entity->saveEmeralds) ? i : 7;
                RSDK.DrawSprite(&entity->animator6, &drawPos, false);
                drawPos.x += 0xC0000;
            }

            drawPos.x = entity->position.x;
            drawPos.y = entity->position.y + 0x100000;
            if (
#if RETRO_USE_PLUS
                !entity->encoreMode ||
#endif
                entity->type) {
#if RETRO_USE_PLUS
                if (!entity->saveContinues) {
                    drawPos.y += 0x20000;
                }
#else
                drawPos.y += 0x20000;
#endif
            }
            else {
                drawPos.y += 0x80000;
            }
            UISaveSlot_Unknown3(drawPos.x, drawPos.y);

            drawPos.x = entity->position.x;
            drawPos.y = entity->position.y + 0x330000;
            UISaveSlot_DrawPlayerInfo(drawPos.x, drawPos.y);
        }
    }
    else {
        drawPos.y = entity->position.y + 0x30000;
        drawPos.x = entity->position.x + 0x30000;
        RSDK.DrawRect(entity->position.x - 0x2D0000, entity->position.y - 0x1F0000, 0x600000, 0x440000, 0xFFFFFF, 127, INK_BLEND, false);

        drawPos.x = entity->position.x + 0x30000;
        drawPos.y = entity->position.y + 0x30000;
        UIWidgets_Unknown3(68, 96, drawPos.x, drawPos.y);

        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator1, 0, false);

        drawPos = entity->position;
        UISaveSlot_Unknown3(entity->position.x, entity->position.y);
        RSDK.DrawSprite(&entity->animator8, 0, false);
    }

    if (entity->flag || entity->type == 0) {
        if (entity->type)
            UIWidgets_Unknown4(68, 96, entity->position.x, entity->position.y);

        if (entity->flag && !(entity->field_13C & 8)) {
            drawPos.x = entity->position.x;
            drawPos.y = entity->position.y;
#if RETRO_USE_PLUS
            if (!entity->encoreMode) {
#endif
                if (entity->type == 1) {
                    drawPos.y -= 0x60000;
                    UIWidgets_Unknown8(40, drawPos.x, drawPos.y);
                }
                else if (entity->isNewSave) {
                    drawPos.y += 0x200000;
                    UIWidgets_Unknown8(64, drawPos.x, drawPos.y);
                }
#if RETRO_USE_PLUS
            }
            else if (entity->listID == 1) {
                drawPos.y -= 0x300000;
                UIWidgets_Unknown8(40, drawPos.x, drawPos.y);
            }
#endif
        }
    }
    RSDK.DrawCircle(RSDK_screens->centerX, RSDK_screens->centerY, entity->radius, 0, entity->alpha, INK_ALPHA, true);
}

void UISaveSlot_Create(void *data)
{
    RSDK_THIS(UISaveSlot);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible         = true;
        entity->drawOrder       = 2;
        entity->active          = ACTIVE_BOUNDS;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x300000;
        entity->processButtonCB = UISaveSlot_ProcessButtonCB;
        entity->state           = UISaveSlot_Unknown22;
        if (!entity->type) {
            UISaveSlot_LoadSaveInfo();
        }
        else {
            entity->slotID = NO_SAVE_SLOT;
            UISaveSlot_Unknown6();
        }
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        UISaveSlot_Unknown8();
    }
}

void UISaveSlot_StageLoad(void) { UISaveSlot->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

byte UISaveSlot_Unknown1(byte id)
{
    switch (id) {
        case 1: return ID_SONIC;
        case 2: return ID_TAILS;
        case 3: return ID_KNUCKLES;
        case 4: return ID_MIGHTY;
        case 5: return ID_RAY;
        default: break;
    }
    return ID_NONE;
}

#if RETRO_USE_PLUS
void UISaveSlot_Unknown2(byte playerID, bool32 isSilhouette, byte buddyID, byte *friendIDs, byte friendCount, int drawX, int drawY)
{
    RSDK_THIS(UISaveSlot);
    Vector2 drawPos;

    Animator *animator = NULL;
    if (!isSilhouette)
        animator = &entity->animator2;
    else
        animator = &entity->animator3;

    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, (isSilhouette != 0) + 19, animator, true, 0);
    entity->drawFX |= FX_FLIP;
    entity->direction = FLIP_NONE;
    switch (buddyID) {
        case ID_TAILS: animator->frameID = 1; break;
        case ID_KNUCKLES: animator->frameID = 2; break;
        case ID_MIGHTY: animator->frameID = 3; break;
        case ID_RAY: animator->frameID = 4; break;
        default: animator->frameID = 0; break;
    }

    int y = drawY;
    if (friendCount)
        y = drawY - 0x80000;
    drawPos.x = drawX + 0x100000;
    drawPos.y = y - 0x40000;

    entity->direction = !animator->frameID || animator->frameID == 3;
    RSDK.DrawSprite(animator, &drawPos, false);

    switch (playerID) {
        case ID_TAILS: animator->frameID = 1; break;
        case ID_KNUCKLES: animator->frameID = 2; break;
        case ID_MIGHTY: animator->frameID = 3; break;
        case ID_RAY: animator->frameID = 4; break;
        default: animator->frameID = 0; break;
    }

    y = drawY;
    if (friendCount)
        y = drawY - 0x80000;
    drawPos.x         = drawX - 0xF0000;
    drawPos.y         = y + 0x30000;
    entity->direction = animator->frameID && animator->frameID != 3;
    RSDK.DrawSprite(animator, &drawPos, false);

    entity->direction = FLIP_NONE;
    entity->drawFX    = FX_NONE;
    if (friendCount && !isSilhouette) {
        RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 3, animator, true, 0);
        int x      = drawX;
        int offset = 0x120000;
        if (entity->saveContinues > 0) {
            offset = 0xA0000;
            x      = drawX - 0x160000;
        }
        drawPos.x = x + ((offset * (friendCount - 1)) >> 1);
        drawPos.y = drawY + 0x1A0000;

        for (int f = friendCount - 1; f >= 0; --f) {
            switch (friendIDs[f]) {
                case ID_TAILS: animator->frameID = 1; break;
                case ID_KNUCKLES: animator->frameID = 2; break;
                case ID_MIGHTY: animator->frameID = 3; break;
                case ID_RAY: animator->frameID = 4; break;
                default: animator->frameID = 0; break;
            }
            RSDK.DrawSprite(animator, &drawPos, false);
            drawPos.x -= offset;
        }
    }
}
#endif

void UISaveSlot_Unknown3(int drawX, int drawY)
{
    RSDK_THIS(UISaveSlot);
    Vector2 drawPos;

    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 1, &entity->animator2, true, 3);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 2, &entity->animator3, true, 3);

    byte friendIDs[3];
    int playerID    = 0;
    int buddyID     = 0;
    int friendCount = 0;

#if RETRO_USE_PLUS
    if (!entity->encoreMode) {
#endif
        int frames[]              = { 3, 0, 1, 2, 4, 5 };
        entity->animator3.frameID = frames[entity->frameID];
        entity->animator2.frameID = frames[entity->frameID];
    }
#if RETRO_USE_PLUS
    else if (!RSDK_sceneInfo->inEditor) {
        if (!entity->isNewSave && entity->type != 1) {
            playerID = entity->saveEncorePlayer;
            buddyID  = entity->saveEncoreBuddy;

            for (int i = 0; i < 3; ++i) {
                friendIDs[i] = 0;
                if (!entity->saveEncoreFriends[i])
                    continue;
                ++friendCount;
                friendIDs[i] = entity->saveEncoreFriends[i];
            }

            if (!buddyID) {
                switch (playerID) {
                    case ID_TAILS: playerID = 1; break;
                    case ID_KNUCKLES: playerID = 2; break;
                    case ID_MIGHTY: playerID = 4; break;
                    case ID_RAY: playerID = 5; break;
                    default: playerID = 0; break;
                }
                entity->animator3.frameID = playerID;
                entity->animator2.frameID = playerID;
            }
            else if (playerID) {
                drawX += 4 * entity->field_144;
                drawY += 4 * entity->field_144;
                UISaveSlot_Unknown2(playerID, true, buddyID, friendIDs, friendCount, drawX, drawY);
                UISaveSlot_Unknown2(playerID, false, buddyID, friendIDs, friendCount, drawX - 8 * entity->field_144, drawY - 8 * entity->field_144);
                return;
            }
        }
        else {
            entity->animator2.frameID = 6;
            entity->animator3.frameID = 6;
        }
    }
    else if (entity->debugEncoreDraw) {
        playerID     = UISaveSlot_Unknown1(entity->dCharPoint);
        buddyID      = UISaveSlot_Unknown1(entity->dCharPartner);
        friendIDs[0] = UISaveSlot_Unknown1(entity->dCharStock1);
        friendIDs[1] = UISaveSlot_Unknown1(entity->dCharStock2);
        friendIDs[2] = UISaveSlot_Unknown1(entity->dCharStock3);
        if (friendIDs[0]) {
            friendCount = 1;
            if (friendIDs[1]) {
                friendCount = 2;
                if (friendIDs[2])
                    friendCount = 3;
            }
        }

        if (!buddyID) {
            switch (playerID) {
                case ID_TAILS: playerID = 1; break;
                case ID_KNUCKLES: playerID = 2; break;
                case ID_MIGHTY: playerID = 4; break;
                case ID_RAY: playerID = 5; break;
                default: playerID = 0; break;
            }
            entity->animator3.frameID = playerID;
            entity->animator2.frameID = playerID;
        }
        else if (playerID) {
            drawX += 4 * entity->field_144;
            drawY += 4 * entity->field_144;
            UISaveSlot_Unknown2(playerID, true, buddyID, friendIDs, friendCount, drawX, drawY);
            UISaveSlot_Unknown2(playerID, false, buddyID, friendIDs, friendCount, drawX - 8 * entity->field_144, drawY - 8 * entity->field_144);
            return;
        }
    }
    else {
        entity->animator2.frameID = 6;
        entity->animator3.frameID = 6;
    }
#endif

    drawX += 4 * entity->field_144;
    drawY += 4 * entity->field_144;
    drawPos.x = drawX;
    drawPos.y = drawY;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawX -= 8 * entity->field_144;
    drawY -= 8 * entity->field_144;
    drawPos.x = drawX;
    drawPos.y = drawY;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void UISaveSlot_DrawPlayerInfo(int drawX, int drawY)
{
    RSDK_THIS(UISaveSlot);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 3, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 21, &entity->animator5, true, 0);

    int playerID = 0;
#if RETRO_USE_PLUS
    if (entity->encoreMode) {
        if (entity->isNewSave || entity->type == 1)
            return;

        if (entity->debugEncoreDraw && RSDK_sceneInfo->inEditor) {
            switch (UISaveSlot_Unknown1(entity->dCharPoint)) {
                case ID_TAILS: playerID = 1; break;
                case ID_KNUCKLES: playerID = 2; break;
                case ID_MIGHTY: playerID = 3; break;
                case ID_RAY: playerID = 4; break;
                default: playerID = 0; break;
            }
        }
        else {
            switch (entity->saveEncorePlayer) {
                case ID_TAILS: playerID = 1; break;
                case ID_KNUCKLES: playerID = 2; break;
                case ID_MIGHTY: playerID = 3; break;
                case ID_RAY: playerID = 4; break;
                default: playerID = 0; break;
            }
        }
    }
    else {
#endif
        int frames[] = { 0, 0, 1, 2, 3, 4 };
        playerID     = frames[entity->frameID];
#if RETRO_USE_PLUS
    }
#endif
    entity->animator4.frameID = playerID;
    entity->animator5.frameID = playerID;

    Animator *animators[2];
    animators[0] = &entity->animator4;
    animators[1] = &entity->animator5;

    int retries[2];
    retries[0] = minVal(entity->saveLives, 99);
#if RETRO_USE_PLUS
    retries[1] = minVal(entity->saveContinues, 99);
#else
    retries[1] = 0;
#endif

    Vector2 positions[2];
    positions[0].x = drawX;
    positions[0].y = drawY;
    positions[1].y = drawY;
    positions[1].x = drawX;

    int count = 1;
    if (retries[1] <= 0) {
        positions[0].x = drawX - 0x20000;
    }
    else {
        positions[0].x = drawX - 0x170000;
        positions[1].x = drawX + 0x170000;
        count          = 2;
    }

    for (int i = 0; i < count; ++i) {
        Vector2 drawPos;

        drawPos.x = positions[i].x;
        drawPos.y = positions[i].y;
        if (i
#if RETRO_USE_PLUS
            || !entity->encoreMode
#endif
        ) {
            entity->animator1.frameID = 3;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);

            drawPos.x -= 0xC0000;
            RSDK.DrawSprite(animators[i], &drawPos, false);

            drawPos.x += 0x140000;
            entity->animator12.frameID = retries[i] / 10;
            RSDK.DrawSprite(&entity->animator12, &drawPos, false);

            drawPos.x += 0x80000;
            entity->animator12.frameID = retries[i] % 10;
            RSDK.DrawSprite(&entity->animator12, &drawPos, false);
        }
    }
}

void UISaveSlot_SetupButtonElements(void)
{
    RSDK_THIS(UISaveSlot);
    entity->touchCB  = UIButton_TouchCB_Alt;
    entity->options3 = UISaveSlot_SelectedSave;
    entity->failCB   = NULL;
    entity->options5 = UISaveSlot_Unknown19;
    entity->options6 = UISaveSlot_Unknown20;
    entity->options7 = UISaveSlot_Unknown17;
    entity->options8 = UISaveSlot_Unknown18;

#if RETRO_USE_PLUS
    EntitySaveGame *saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(entity->slotID, entity->encoreMode);
#else
    EntitySaveGame *saveRAM = (EntitySaveGame *)SaveGame_GetDataPtr(entity->slotID);
#endif
    if (entity->type == 1) {
        entity->touchPos1[0].x       = 0x600000;
        entity->touchPos1[0].y       = 0x440000;
        entity->touchPos2[0].x       = 0;
        entity->touchPos2[0].y       = 0;
        entity->touchPosCallbacks[0] = UISaveSlot_SelectedSave;
        entity->touchPos1[1].x       = 0x200000;
        entity->touchPos1[1].y       = 0x200000;
        entity->touchPos2[1].x       = 0;
        entity->touchPos2[1].y       = -0x1A0000;
        entity->touchPosCallbacks[1] = UISaveSlot_NextCharacter;
        entity->touchPos1[2].x       = 0x200000;
        entity->touchPos1[2].y       = 0x200000;
        entity->touchPos2[2].x       = 0;
        entity->touchPos2[2].y       = 0xE0000;
        entity->touchPosCallbacks[2] = UISaveSlot_PrevCharacter;
        entity->touchPosCount        = 3;
    }
    else {
        entity->touchPos1[0].x       = 0x600000;
        entity->touchPos1[0].y       = 0xA40000;
        entity->touchPos2[0].x       = 0;
        entity->touchPos2[0].y       = 0;
        entity->touchPosCallbacks[0] = UISaveSlot_SelectedSave;

        switch (saveRAM->saveState) {
            default:
            case 0:
                entity->touchPos1[1].x       = 0x200000;
                entity->touchPos1[1].y       = 0x200000;
                entity->touchPos2[1].x       = 0;
                entity->touchPos2[1].y       = 0;
                entity->touchPosCallbacks[1] = UISaveSlot_NextCharacter;
                entity->touchPos1[2].x       = 0x200000;
                entity->touchPos1[2].y       = 0x200000;
                entity->touchPos2[2].x       = 0;
                entity->touchPos2[2].y       = 0x400000;
                entity->touchPosCallbacks[2] = UISaveSlot_PrevCharacter;
                entity->touchPosCount        = 3;
                break;
            case 1: entity->touchPosCount = 1; break;
            case 2:
                entity->touchPos1[1].x       = 0x200000;
                entity->touchPos1[1].y       = 0x200000;
                entity->touchPos2[1].y       = -0x440000;
                entity->touchPos2[1].x       = 0;
                entity->touchPosCallbacks[1] = UISaveSlot_NextZone;
                entity->touchPos1[2].x       = 0x200000;
                entity->touchPos1[2].y       = 0x200000;
                entity->touchPos2[2].x       = 0;
                entity->touchPos2[2].y       = 0x280000 - 0x440000;
                entity->touchPosCallbacks[2] = UISaveSlot_PrevZone;
                entity->touchPosCount        = 3;
                break;
        }
    }
}

void UISaveSlot_Unknown6(void)
{
    RSDK_THIS(UISaveSlot);
#if RETRO_USE_PLUS
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 0, &entity->animator10, true, 2);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 2, &entity->animator11, true, 0);
#else
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 1, &entity->animator2, true, 3);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 2, &entity->animator3, true, 3);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 3, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 0, &entity->animator9, true, 2);
#endif
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 7, &entity->animator9, true, 0);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 4, &entity->animator6, true, 0);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 5, &entity->animator7, true, 0);

    if (entity->type)
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 2, &entity->animator8, true, 2);
    else
        RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 6, &entity->animator8, true, 0);
    RSDK.SetSpriteAnimation(UISaveSlot->aniFrames, 8, &entity->animator12, true, 0);
}

void UISaveSlot_LoadSaveInfo(void)
{
    RSDK_THIS(UISaveSlot);
#if RETRO_USE_PLUS
    EntitySaveGame *saveRAM = (EntitySaveGame*)SaveGame_GetDataPtr(entity->slotID, entity->encoreMode);
#else                         
    EntitySaveGame *saveRAM = (EntitySaveGame*)SaveGame_GetDataPtr(entity->slotID);
#endif
    int saveState = saveRAM->saveState;
    if (saveState == 1 || saveState == 2) {
#if RETRO_USE_PLUS
        if (entity->encoreMode) {
            entity->saveEncorePlayer = saveRAM->playerID & 0xFF;
            entity->saveEncoreBuddy  = (saveRAM->playerID >> 8) & 0xFF;

            int friends = saveRAM->stock;
            for (int i = 0; i < 3; ++i) {
                entity->saveEncoreFriends[i] = ID_NONE;
                if (!friends)
                    continue;
                entity->saveEncoreFriends[i] = friends & 0xFF;
                friends >>= 8;
            }

            entity->frameID = entity->saveEncorePlayer;
        }
        else {
#endif
            entity->frameID = saveRAM->characterID;
#if RETRO_USE_PLUS
        }
#endif
    }

    switch (saveState) {
        default: break;
        case 0:
            entity->saveZoneID   = 0;
            entity->saveEmeralds = 0;
            entity->saveLives    = 3;
#if RETRO_USE_PLUS
            entity->saveContinues = 0;
            entity->frameID       = entity->encoreMode ? 6 : 0;
#else
            entity->frameID = 0;
#endif
            entity->isNewSave = true;
            entity->listID    = 0;
            break;
        case 1:
            entity->saveZoneID   = saveRAM->zoneID;
            entity->saveEmeralds = saveRAM->chaosEmeralds;
            entity->saveLives    = saveRAM->lives;
#if RETRO_USE_PLUS
            entity->saveContinues = saveRAM->continues;
#endif
            entity->isNewSave = false;
            entity->listID    = 0;
            break;
        case 2:
            entity->saveZoneID   = 255;
            entity->saveEmeralds = saveRAM->chaosEmeralds;
            entity->saveLives    = saveRAM->lives;
#if RETRO_USE_PLUS
            entity->saveContinues = saveRAM->continues;
#endif
            entity->listID    = 1;
            entity->isNewSave = false;
            break;
    }
    UISaveSlot_Unknown6();
    UISaveSlot_Unknown8();
}
void UISaveSlot_Unknown8(void)
{
    RSDK_THIS(UISaveSlot);

    entity->animator11.frameID = entity->listID;
    entity->animator7.frameID  = entity->saveZoneID;
    if (entity->type == 0) {
        entity->animator8.frameID = entity->saveZoneID;
        if (entity->zoneID) {
#if RETRO_USE_PLUS
            if (entity->encoreMode)
                RSDK.CopyPalette(((entity->saveZoneID >> 3) + 4), 32 * entity->saveZoneID, 0, 224, 32);
            else
#endif
                RSDK.CopyPalette(((entity->saveZoneID >> 3) + 1), 32 * entity->saveZoneID, 0, 224, 32);
        }
    }
}

void UISaveSlot_DeleteDLG_CB(void)
{
    EntityUIDialog *dialog     = (EntityUIDialog *)UIDialog->activeDialog;
    EntityUISaveSlot *saveSlot = (EntityUISaveSlot *)dialog->entityPtr;
    dialog->parent->state      = 0;
    UIWaitSpinner_Wait();
#if RETRO_USE_PLUS
    int *saveRAM = SaveGame_GetDataPtr(saveSlot->slotID % 8, saveSlot->encoreMode);
#else
    int *saveRAM = SaveGame_GetDataPtr(saveSlot->slotID % 8);
#endif
    memset(saveRAM, 0, 0x400);
    SaveGame_SaveFile(UISaveSlot_DeleteSaveCB);
}

void UISaveSlot_DeleteSaveCB(int status)
{
    EntityUIDialog *dialog     = (EntityUIDialog *)UIDialog->activeDialog;
    EntityUISaveSlot *saveSlot = (EntityUISaveSlot *)dialog->entityPtr;
    UIWaitSpinner_Wait2();
    if (dialog->state != UIDialog_Unknown13) {
        dialog->parent->selectionDisabled = true;
        dialog->field_5C                  = 0;
        dialog->state                     = UIDialog_Unknown13;
        dialog->curCallback               = NULL;
    }

    Entity *storeEntity    = RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity = (Entity *)saveSlot;
    UISaveSlot_LoadSaveInfo();
    UISaveSlot_Unknown8();
    RSDK_sceneInfo->entity = storeEntity;
}

void UISaveSlot_ProcessButtonCB(void)
{
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
#if RETRO_USE_PLUS
    EntitySaveGame *saveRAM = (EntitySaveGame*)SaveGame_GetDataPtr(entity->slotID, entity->encoreMode);
#else                         
    EntitySaveGame *saveRAM = (EntitySaveGame*)SaveGame_GetDataPtr(entity->slotID);
#endif

    entity->active = ACTIVE_NORMAL;
    UIControl_Unknown15(control, entity->position.x, 0);
    if (control->position.x == control->posUnknown.x) {
        if (control->columnCount > 1) {
            if (UIControl->keyLeft) {
                if (control->activeEntityID > 0) {
                    control->activeEntityID--;
                    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
                    UISaveSlot_Unknown21();
                    return;
                }
            }
            else if (UIControl->keyRight) {
                if (control->activeEntityID < control->buttonCount - 1) {
                    control->activeEntityID++;
                    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
                    UISaveSlot_Unknown21();
                    return;
                }
            }
        }

        StateMachine_Run(entity->stateUnknown);

        TextInfo strBuffer;
        INIT_TEXTINFO(strBuffer);

        if (UIControl->keyConfirm) {
            if (API.CheckDLC(DLC_PLUS) || entity->frameID < 4) {
                UISaveSlot_SelectedSave();
            }
            else {
                Localization_GetString(&strBuffer, STR_ENCOREREQUIRED);
                EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&strBuffer);
                if (dialog) {
                    UIDialog_AddButton(2, dialog, NULL, true);
                    UIDialog_Setup(dialog);
                }
            }
        }
        else if (UIControl->keyX && saveRAM->saveState && !entity->type) {
            Localization_GetString(&strBuffer, STR_DELETEPOPUP);
            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&strBuffer);
            if (dialog) {
                dialog->entityPtr = (Entity *)entity;
                UIDialog_AddButton(1, dialog, NULL, true);
                UIDialog_AddButton(0, dialog, UISaveSlot_DeleteDLG_CB, false);
                UIDialog_Setup(dialog);
            }
        }
        else if (!entity->zoneID && control->buttons[control->field_D8] == (EntityUIButton *)entity && control->state == UIControl_ProcessInputs) {
            UISaveSlot_Unknown19();
        }
    }
}

void UISaveSlot_SelectedSave(void)
{
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    if (control->position.x == control->posUnknown.x) {
        control->state          = 0;
        entity->state           = UISaveSlot_Unknown28;
        entity->flag            = false;
        entity->zoneID          = 0;
        entity->processButtonCB = NULL;
        foreach_all(UISaveSlot, saveSlot)
        {
            if (saveSlot != entity) {
                if (saveSlot->position.x >= entity->position.x) {
                    if (saveSlot->position.x > entity->position.x)
                        saveSlot->velocity.x = 0x200000;
                }
                else {
                    saveSlot->velocity.x = -0x200000;
                }
                saveSlot->state = UISaveSlot_Unknown23;
            }
        }

        foreach_all(UIButtonPrompt, prompt) { prompt->visible = false; }

        int id = RSDK.MostRecentActiveControllerID(1, 0, 5);
        RSDK.ResetControllerAssignments();
        RSDK.AssignControllerID(1, id);
        RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
        RSDK.StopChannel(Music->channelID);
    }
}

void UISaveSlot_NextCharacter(void)
{
    RSDK_THIS(UISaveSlot);
    ++entity->frameID;

    int player = entity->frameID;
#if RETRO_USE_PLUS
    int max = API.CheckDLC(DLC_PLUS) ? 6 : 4;
#else
    int max = 4;
#endif
    while (player >= max) {
        player -= max;
    }
    entity->frameID   = player;
    entity->field_14C = 0x8000;
    entity->field_144 = 0;
    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
    UISaveSlot_Unknown8();
}

void UISaveSlot_PrevCharacter(void)
{
    RSDK_THIS(UISaveSlot);
    --entity->frameID;

    int player = entity->frameID;
#if RETRO_USE_PLUS
    int max = API.CheckDLC(DLC_PLUS) ? 6 : 4;
#else
    int max = 4;
#endif
    while (player < 0) {
        player += max;
    }
    entity->frameID   = player;
    entity->field_14C = 0x8000;
    entity->field_144 = 0;
    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
    UISaveSlot_Unknown8();
}

void UISaveSlot_NextZone(void)
{
    RSDK_THIS(UISaveSlot);

    if (entity->saveZoneID == 255) {
        entity->saveZoneID = 0;
    }
    else {
        entity->saveZoneID++;
        if (entity->saveZoneID > 11)
            entity->saveZoneID = 0;
    }
    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
    UISaveSlot_Unknown8();
}

void UISaveSlot_PrevZone(void)
{
    RSDK_THIS(UISaveSlot);

    if (entity->saveZoneID == 255) {
        entity->saveZoneID = 11;
    }
    else {
        entity->saveZoneID--;
        if (entity->saveZoneID < 0)
            entity->saveZoneID = 11;
    }
    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
    UISaveSlot_Unknown8();
}

bool32 UISaveSlot_Unknown17(void)
{
    RSDK_THIS(UISaveSlot);
    return entity->flag;
}
bool32 UISaveSlot_Unknown18(void)
{
    RSDK_THIS(UISaveSlot);
    return entity->state == UISaveSlot_Unknown28;
}

void UISaveSlot_Unknown19(void)
{
    RSDK_THIS(UISaveSlot);
    if (!entity->zoneID) {
        entity->flag   = true;
        entity->zoneID = 1;
        UISaveSlot_Unknown8();
        entity->field_148 = 0x4000;
        entity->field_14C = 0x8000;
#if RETRO_USE_PLUS
        entity->stateUnknown = StateMachine_None;
#endif
        if (!entity->isNewSave && entity->type != 1) {
            if (entity->listID) {
                entity->state = UISaveSlot_Unknown27;
            }
            else {
                entity->state = UISaveSlot_Unknown25;
            }
        }
#if RETRO_USE_PLUS
        else if (entity->encoreMode) {
            entity->state = UISaveSlot_Unknown25;
        }
#endif
        else {
            entity->state = UISaveSlot_Unknown24;
#if RETRO_USE_PLUS
            entity->stateUnknown = UISaveSlot_Unknown26;
#endif
        }
    }
}

void UISaveSlot_Unknown20(void)
{
    RSDK_THIS(UISaveSlot);
    entity->flag         = false;
    entity->zoneID       = 0;
    entity->state        = UISaveSlot_Unknown22;
    entity->stateUnknown = StateMachine_None;
}

void UISaveSlot_Unknown21(void)
{
    RSDK_THIS(UISaveSlot);
#if RETRO_USE_PLUS
    EntitySaveGame *saveRAM = (EntitySaveGame*)SaveGame_GetDataPtr(entity->slotID, entity->encoreMode);
#else
    EntitySaveGame *saveRAM = (EntitySaveGame*)SaveGame_GetDataPtr(entity->slotID);
#endif

    if (!saveRAM->saveState) {
#if RETRO_USE_PLUS
        int frame = entity->encoreMode ? 6 : 0;
#else
        int frame = 0;
#endif
        if (entity->frameID != frame) {
            entity->frameID = frame;
            UISaveSlot_Unknown8();
        }
    }
    if (!entity->type && saveRAM->saveState == 2) {
        entity->saveZoneID = 255;
        UISaveSlot_Unknown8();
    }
}

void UISaveSlot_Unknown22(void)
{
    RSDK_THIS(UISaveSlot);
    RSDK.ProcessAnimation(&entity->animator9);
    entity->direction = entity->animator9.frameID & 3;
}

void UISaveSlot_Unknown23(void)
{
    RSDK_THIS(UISaveSlot);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
}

void UISaveSlot_Unknown24(void)
{
    // so true!
}

void UISaveSlot_Unknown25(void)
{
    RSDK_THIS(UISaveSlot);
    if (!entity->zoneID)
        entity->state = UISaveSlot_Unknown22;
}

void UISaveSlot_Unknown26(void)
{
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    if (control->active == ACTIVE_ALWAYS) {
        if (UIControl->keyUp) {
            UISaveSlot_NextCharacter();
        }
        else if (UIControl->keyDown) {
            UISaveSlot_PrevCharacter();
        }
    }
}

void UISaveSlot_Unknown27(void)
{
    RSDK_THIS(UISaveSlot);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    if (control->active == ACTIVE_ALWAYS) {
        if (UIControl->keyUp) {
            UISaveSlot_NextZone();
        }
        else if (UIControl->keyDown) {
            UISaveSlot_PrevZone();
        }
    }
}

void UISaveSlot_Unknown28(void)
{
    RSDK_THIS(UISaveSlot);

    ++entity->timer;
#if RETRO_USE_PLUS
    if (entity->encoreMode && (entity->isNewSave || entity->type == 1)) {
        EntityFXRuby *fxRuby = entity->fxRuby;
        int rubyY            = entity->position.y;

        int val = (4 * entity->field_144);
        if (entity->type != 1)
            rubyY += 0x200000;

        if (entity->timer == 32) {
            PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
            fxRuby            = CREATE_ENTITY(FXRuby, NULL, entity->position.x - val, rubyY - val);
            fxRuby->fadeWhite = 0;
            fxRuby->fadeBlack = 0;
            fxRuby->drawOrder = 15;
            entity->fxRuby    = fxRuby;
        }
        if (fxRuby) {
            fxRuby->position.x = entity->position.x - val;
            fxRuby->position.y = rubyY - val;
        }
        if (entity->timer == 152)
            PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);

        if (entity->timer >= 92) {
            if (fxRuby->fadeWhite < 512) {
                fxRuby->fadeWhite += 8;
            }
            else if (fxRuby->fadeBlack < 512) {
                fxRuby->fadeBlack += 8;
            }
            else if (entity->timer == 302) {
                StateMachine_Run(entity->options2);
                entity->state = StateMachine_None;
            }
        }
    }
    else {
#endif
        if (entity->timer == 32)
            RSDK.PlaySFX(UIWidgets->sfx_Warp, false, 255);
        if (entity->timer > 32) {
            entity->alpha += 32;
            entity->radius += 12;
        }
        if (entity->radius > 512) {
            StateMachine_Run(entity->options2);
            entity->state = StateMachine_None;
        }
#if RETRO_USE_PLUS
    }
#endif
}

void UISaveSlot_EditorDraw(void) {}

void UISaveSlot_EditorLoad(void) {}

void UISaveSlot_Serialize(void)
{
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_ENUM, slotID);
#if RETRO_USE_PLUS
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_BOOL, encoreMode);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_BOOL, debugEncoreDraw);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_UINT8, dCharPoint);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_UINT8, dCharPartner);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_UINT8, dCharStock1);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_UINT8, dCharStock2);
    RSDK_EDITABLE_VAR(UISaveSlot, VAR_UINT8, dCharStock3);
#endif
}
