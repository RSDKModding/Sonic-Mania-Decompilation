#include "SonicMania.h"

ObjectLEDPanel *LEDPanel;

void LEDPanel_Update(void)
{
    RSDK_THIS(LEDPanel);

    for (entity->row = 0; entity->row < LEDPanel_RowCount; ++entity->row) {
        StateMachine_Run(entity->stateRow[entity->row]);
    }
}

void LEDPanel_LateUpdate(void) {}

void LEDPanel_StaticUpdate(void) {}

void LEDPanel_Draw(void)
{
    RSDK_THIS(LEDPanel);

    RSDK.DrawRect(entity->position.x - (entity->size.x >> 1) + 0x80000, entity->position.y - (entity->size.y >> 1), entity->size.x - 0x100000,
                  entity->size.y, LEDPanel->storedColour, 512, INK_NONE, false);

    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];

    int clipX1 = screen->clipBound_X1;
    int clipX2 = screen->clipBound_X2;
    int clipY1 = screen->clipBound_Y1;
    int clipY2 = screen->clipBound_Y2;

    int clipBound_X1 = entity->left - screen->position.x + (entity->position.x >> 16) + 8;
    int clipBound_X2 = entity->right - screen->position.x + (entity->position.x >> 16) - 8;
    int clipBound_Y1 = (entity->position.y >> 16) + entity->top - screen->position.y;
    int clipBound_Y2 = (entity->position.y >> 16) + entity->bottom - screen->position.y;

    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, clipBound_X1, clipBound_Y1, clipBound_X2, clipBound_Y2);

    for (int r = 0; r < LEDPanel_RowCount; ++r) {
        RSDK.DrawText(&entity->animatorText, &entity->textPos[r], &entity->activeText[r], 0, entity->activeText[r].textLength, ALIGN_RIGHT, 0, 0,
                      NULL, false);
    }

    screen->clipBound_X1 = clipX1;
    screen->clipBound_X2 = clipX2;
    screen->clipBound_Y1 = clipY1;
    screen->clipBound_Y2 = clipY2;
    RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
}

void LEDPanel_Create(void *data)
{
    RSDK_THIS(LEDPanel);

    entity->active      = ACTIVE_BOUNDS;
    entity->visible     = true;
    entity->drawOrder   = Zone->drawOrderLow;
    entity->updateRange = entity->size;
    RSDK.SetSpriteAnimation(LEDPanel->aniFrames, 0, &entity->animatorText, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        for (int i = 0; i < LEDPanel_TextCount; ++i) {
            RSDK.SetSpriteString(LEDPanel->aniFrames, 0, &entity->text[i]);
            entity->textPtrs[i] = &entity->text[i];
        }

        entity->left   = -(entity->size.x >> 17);
        entity->right  = entity->size.x >> 17;
        entity->top    = -(entity->size.y >> 17);
        entity->bottom = entity->size.y >> 17;

        entity->curXBoundaryL = (entity->size.x >> 1);
        entity->newXBoundaryL = (entity->size.x >> 1);
        entity->curYBoundaryT = -(entity->size.y >> 1);
        entity->newYBoundaryT = -(entity->size.y >> 1);
        entity->curXBoundaryR = -(entity->size.x >> 1);
        entity->newXBoundaryR = -(entity->size.x >> 1);
        entity->curYBoundaryB = -(entity->size.y >> 1);
        entity->newYBoundaryB = -(entity->size.y >> 1);

        entity->boundsMoveSpeed.x = 0x10000;
        entity->boundsMoveSpeed.y = 0x10000;

        int offset = 0;
        for (int r = 0; r < LEDPanel_RowCount; ++r) {
            LEDPanel_SetupActiveText(r, entity->textPtrs[r]);

            entity->seqPtrs[r]       = &entity->seq[r];
            entity->textMoveVel[r].x = 0x10000;
            entity->textMoveVel[r].y = 0x10000;

            entity->textPos[r].x = entity->position.x;
            entity->textPos[r].y = entity->position.y;
            entity->textPos[r].x += (entity->activeText[r].textLength << 20) - (entity->size.x >> 1) + 0x80000;
            entity->textPos[r].y += (0x100000 + offset) - (entity->size.y >> 1);

            entity->textMovePos[r].x = 0;
            entity->textMovePos[r].y = offset;

            entity->textTargetPos[r].x = 0;
            entity->textTargetPos[r].y = offset;

            entity->stateRow[r] = StateMachine_None;

            offset += 0x200000;
        }

        for (entity->row = 0; entity->row < LEDPanel_RowCount; ++entity->row) {
            LEDPanel_HandleCharacters();
        }
    }
}

void LEDPanel_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        LEDPanel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/LED.bin", SCOPE_STAGE);
    RSDK.SetText(&LEDPanel->text, " ", 0);
    LEDPanel->storedColour = RSDK.GetPaletteEntry(0, 190);
}

void LEDPanel_SetupActiveText(int row, TextInfo *src)
{
    RSDK_THIS(LEDPanel);

    RSDK.CopyString(&entity->activeText[row], src);

    entity->activeTextSize[row] = entity->activeText[row].textLength;
    for (int i = 0; i < entity->activeText[row].textLength; ++i) {
        if (entity->activeText[row].text[i] != 27) {
            entity->activeTextSize[row] = i;
            break;
        }
    }

    entity->activeTextLen[row] = 0;
    for (int i = entity->activeText[row].textLength - 1; i >= 0; --i) {
        if (entity->activeText[row].text[i] != 27) {
            entity->activeTextLen[row] = i;
            break;
        }
    }
}

void LEDPanel_SetupTextPos(int row, int x, int y)
{
    RSDK_THIS(LEDPanel);

    entity->textPos[row].x = entity->position.x;
    entity->textPos[row].y = entity->position.y;
    entity->textPos[row].x += (entity->activeText[row].textLength << 20) - (entity->size.x >> 1) + x + 0x80000;
    entity->textPos[row].y += y - (entity->size.y >> 1) + 0x100000;
}

void LEDPanel_HandleCharacters(void)
{
    RSDK_THIS(LEDPanel);

    entity->rowFlag[entity->row] = false;
    bool32 flag                  = false;

    while (!flag) {
        int charID = -1;
        if (entity->rowSeqPos[entity->row] < entity->seqPtrs[entity->row]->textLength)
            charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++];

        switch (charID) {
            case 'A':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';
                switch (charID) {
                    case 0:
                        entity->textMovePos[entity->row].x   = -0x100000 * entity->activeText[entity->row].textLength;
                        entity->textTargetPos[entity->row].x = entity->textMovePos[entity->row].x;
                        break;
                    case 1:
                        entity->textMovePos[entity->row].x   = 0;
                        entity->textTargetPos[entity->row].x = 0;
                        break;
                    case 2:
                        entity->textMovePos[entity->row].x   = (entity->size.x >> 1) - (entity->activeText[entity->row].textLength << 19) - 0x80000;
                        entity->textTargetPos[entity->row].x = entity->textMovePos[entity->row].x;
                        break;
                    case 3:
                        entity->textMovePos[entity->row].x   = entity->size.x - (entity->activeText[entity->row].textLength << 20) - 0x100000;
                        entity->textTargetPos[entity->row].x = entity->textMovePos[entity->row].x;
                        break;
                    case 4:
                        entity->textMovePos[entity->row].x   = entity->size.x;
                        entity->textTargetPos[entity->row].x = entity->textMovePos[entity->row].x;
                        break;
                    default: break;
                }
                LEDPanel_SetupTextPos(entity->row, entity->textMovePos[entity->row].x, entity->textMovePos[entity->row].y);
                break;
            case 'B':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';
                switch (charID) {
                    case 0:
                        entity->textMovePos[entity->row].y   = -0x200000;
                        entity->textTargetPos[entity->row].y = -0x200000;
                        break;
                    case 1:
                        entity->textMovePos[entity->row].y   = 0;
                        entity->textTargetPos[entity->row].y = 0;
                        break;
                    case 2:
                        entity->textMovePos[entity->row].y   = (entity->size.y >> 1) - 0x100000;
                        entity->textTargetPos[entity->row].y = entity->textMovePos[entity->row].y;
                        break;
                    case 3:
                        entity->textMovePos[entity->row].y   = entity->size.y - 0x200000;
                        entity->textTargetPos[entity->row].y = entity->textMovePos[entity->row].y;
                        break;
                    case 4:
                        entity->textMovePos[entity->row].y   = entity->size.y;
                        entity->textTargetPos[entity->row].y = entity->textMovePos[entity->row].y;
                        break;
                    default: break;
                }

                LEDPanel_SetupTextPos(entity->row, entity->textMovePos[entity->row].x, entity->textMovePos[entity->row].y);
                break;
            case 'C':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';
                switch (charID) {
                    case 0: entity->textTargetPos[entity->row].x = -0x100000 * entity->activeText[entity->row].textLength; break;
                    case 1: entity->textTargetPos[entity->row].x = 0; break;
                    case 2:
                        entity->textTargetPos[entity->row].x = (entity->size.x >> 1) - (entity->activeText[entity->row].textLength << 19) - 0x80000;
                        break;
                    case 3:
                        entity->textTargetPos[entity->row].x = entity->size.x - (entity->activeText[entity->row].textLength << 20) - 0x100000;
                        break;
                    case 4: entity->textTargetPos[entity->row].x = entity->size.x; break;
                    default: break;
                }
                break;
            case 'D':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';
                switch (charID) {
                    case 0: entity->textTargetPos[entity->row].y = -0x200000; break;
                    case 1: entity->textTargetPos[entity->row].y = 0; break;
                    case 2: entity->textTargetPos[entity->row].y = (entity->size.y >> 1) - 0x100000; break;
                    case 3: entity->textTargetPos[entity->row].y = entity->size.y - 0x200000; break;
                    case 4: entity->textTargetPos[entity->row].y = entity->size.y; break;
                    default: break;
                }
                break;
            case 'E':
                entity->stateRow[entity->row] = LEDPanel_StateText_Move;
                LEDPanel_StateText_Move();
                flag = true;
                break;
            case 'F':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                if (charID == 0) {
                    ++entity->activeTextLen[entity->row];
                    LEDPanel->text.text[0] = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++];
                    RSDK.SetSpriteString(LEDPanel->aniFrames, 0, &LEDPanel->text);
                    entity->activeText[entity->row].text[entity->activeTextLen[entity->row]] = LEDPanel->text.text[0];
                }
                else if (charID == 1) {
                    --entity->activeTextSize[entity->row];
                    LEDPanel->text.text[0] = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++];
                    RSDK.SetSpriteString(LEDPanel->aniFrames, 0, &LEDPanel->text);
                    entity->activeText[entity->row].text[entity->activeTextSize[entity->row]] = LEDPanel->text.text[0];
                }

                break;
            case 'G':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                if (charID == 0) {
                    entity->activeText[entity->row].text[entity->activeTextLen[entity->row]--] = 27;
                }
                else if (charID == 1) {
                    entity->activeText[entity->row].text[entity->activeTextSize[entity->row]++] = 27;
                }
                break;
            case 'H':
                entity->stateRow[entity->row] = LEDPanel_StateText_WaitForSignal;
                LEDPanel_StateText_WaitForSignal();
                flag = true;
                break;
            case 'I':
                entity->rowFlag[entity->row]  = true;
                entity->stateRow[entity->row] = LEDPanel_HandleCharacters;
                flag                          = true;
                break;
            case 'J': 
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';
                LEDPanel_SetupActiveText(entity->row, entity->textPtrs[charID]);
                break;
            case 'K':
                entity->rowDelay[entity->row] = 15;
                entity->stateRow[entity->row] = LEDPanel_StateText_Delay;
                flag                          = true;
                break;
            case 'L': entity->textMoveVel[entity->row].x = (entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '/') << 15; break;
            case 'M': entity->textMoveVel[entity->row].y = (entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '/') << 15; break;
            case 'N': entity->boundsMoveSpeed.x = (entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '/') << 15; break;
            case 'O': entity->boundsMoveSpeed.y = (entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '/') << 15; break;
            case 'P':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        entity->newXBoundaryL = -(entity->size.x >> 1);
                        entity->curXBoundaryL = -(entity->size.x >> 1);
                        entity->left          = entity->curXBoundaryL >> 16;
                        break;
                    case 2:
                        entity->curXBoundaryL = 0;
                        entity->newXBoundaryL = 0;
                        entity->left          = 0;
                        break;
                    case 3:
                        entity->newXBoundaryL = entity->size.x >> 1;
                        entity->curXBoundaryL = entity->size.x >> 1;
                        entity->left          = entity->curXBoundaryL >> 16;
                        break;
                }
                break;
            case 'Q':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        entity->newYBoundaryT = -(entity->size.y >> 1);
                        entity->curYBoundaryT = -(entity->size.y >> 1);
                        entity->top           = entity->curYBoundaryT >> 16;
                        break;
                    case 2:
                        entity->curYBoundaryT = 0;
                        entity->newYBoundaryT = 0;
                        entity->top           = 0;
                        break;
                    case 3:
                        entity->newYBoundaryT = entity->size.y >> 1;
                        entity->curYBoundaryT = entity->size.y >> 1;
                        entity->top           = entity->curYBoundaryT >> 16;
                        break;
                }
                break;
            case 'R':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        entity->newXBoundaryR = -(entity->size.x >> 1);
                        entity->curXBoundaryR = -(entity->size.x >> 1);
                        entity->right         = entity->curXBoundaryR >> 16;
                        break;
                    case 2:
                        entity->curXBoundaryR = 0;
                        entity->newXBoundaryR = 0;
                        entity->right         = 0;
                        break;
                    case 3:
                        entity->newXBoundaryR = entity->size.x >> 1;
                        entity->curXBoundaryR = entity->size.x >> 1;
                        entity->right         = entity->curXBoundaryR >> 16;
                        break;
                }
                break;
            case 'S':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        entity->newYBoundaryB = -(entity->size.y >> 1);
                        entity->curYBoundaryB = -(entity->size.y >> 1);
                        entity->bottom        = entity->curYBoundaryB >> 16;
                        break;
                    case 2:
                        entity->curYBoundaryB = 0;
                        entity->newYBoundaryB = 0;
                        entity->bottom        = 0;
                        break;
                    case 3:
                        entity->newYBoundaryB = entity->size.y >> 1;
                        entity->curYBoundaryB = entity->size.y >> 1;
                        entity->bottom        = entity->curYBoundaryB >> 16;
                        break;
                }
                break;
            case 'T':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: entity->newXBoundaryL = -(entity->size.x >> 1); break;
                    case 2: entity->newXBoundaryL = 0; break;
                    case 3: entity->newXBoundaryL = entity->size.x >> 1; break;
                }
                break;
            case 'U':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: entity->newYBoundaryT = -(entity->size.y >> 1); break;
                    case 2: entity->newYBoundaryT = 0; break;
                    case 3: entity->newYBoundaryT = entity->size.y >> 1; break;
                }
                break;
            case 'V':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: entity->newXBoundaryR = -(entity->size.x >> 1); break;
                    case 2: entity->newXBoundaryR = 0; break;
                    case 3: entity->newXBoundaryR = entity->size.x >> 1; break;
                }
                break;
            case 'W':
                charID = entity->seqPtrs[entity->row]->text[entity->rowSeqPos[entity->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: entity->newYBoundaryB = -(entity->size.y >> 1); break;
                    case 2: entity->newYBoundaryB = 0; break;
                    case 3: entity->newYBoundaryB = entity->size.y >> 1; break;
                }
                break;
            case 'X':
                entity->stateRow[entity->row] = LEDPanel_StateText_ChangeClipBounds;
                LEDPanel_StateText_ChangeClipBounds();
                flag = true;
                break;
            default:
                entity->rowSeqPos[entity->row] = 0;
                break;
        }
    }
}

void LEDPanel_StateText_Move(void)
{
    RSDK_THIS(LEDPanel);

    if (entity->textMovePos[entity->row].x != entity->textTargetPos[entity->row].x) {
        if (entity->textMovePos[entity->row].x >= entity->textTargetPos[entity->row].x) {
            entity->textMovePos[entity->row].x -= entity->textMoveVel[entity->row].x;

            if (entity->textMovePos[entity->row].x < entity->textTargetPos[entity->row].x)
                entity->textMovePos[entity->row].x = entity->textTargetPos[entity->row].x;
        }
        else {
            entity->textMovePos[entity->row].x += entity->textMoveVel[entity->row].x;

            if (entity->textMovePos[entity->row].x > entity->textTargetPos[entity->row].x)
                entity->textMovePos[entity->row].x = entity->textTargetPos[entity->row].x;
        }
    }

    if (entity->textMovePos[entity->row].y != entity->textTargetPos[entity->row].y) {
        if (entity->textMovePos[entity->row].y >= entity->textTargetPos[entity->row].y) {
            entity->textMovePos[entity->row].y -= entity->textMoveVel[entity->row].y;

            if (entity->textMovePos[entity->row].y < entity->textTargetPos[entity->row].y)
                entity->textMovePos[entity->row].y = entity->textTargetPos[entity->row].y;
        }
        else {
            entity->textMovePos[entity->row].y += entity->textMoveVel[entity->row].y;

            if (entity->textMovePos[entity->row].y > entity->textTargetPos[entity->row].y)
                entity->textMovePos[entity->row].y = entity->textTargetPos[entity->row].y;
        }
    }

    entity->textPos[entity->row].x = entity->position.x;
    entity->textPos[entity->row].y = entity->position.y;
    entity->textPos[entity->row].x +=
        (entity->activeText[entity->row].textLength << 20) - (entity->size.x >> 1) + entity->textMovePos[entity->row].x + 0x80000;
    entity->textPos[entity->row].y += entity->textMovePos[entity->row].y - (entity->size.y >> 1) + 0x100000;

    if (entity->textMovePos[entity->row].x == entity->textTargetPos[entity->row].x) {
        if (entity->textMovePos[entity->row].y == entity->textTargetPos[entity->row].y)
            LEDPanel_HandleCharacters();
    }
}

void LEDPanel_StateText_Delay(void)
{
    RSDK_THIS(LEDPanel);

    if (--entity->rowDelay[entity->row] <= 0)
        LEDPanel_HandleCharacters();
}

void LEDPanel_StateText_ChangeClipBounds(void)
{
    RSDK_THIS(LEDPanel);

    int count = 0;

    if (entity->curXBoundaryL == entity->newXBoundaryL) {
        count = 1;
    }
    else if (entity->curXBoundaryL >= entity->newXBoundaryL) {
        entity->curXBoundaryL -= entity->boundsMoveSpeed.x;
        if (entity->curXBoundaryL < entity->newXBoundaryL)
            entity->curXBoundaryL = entity->newXBoundaryL;
    }
    else {
        entity->curXBoundaryL += entity->boundsMoveSpeed.x;
        if (entity->curXBoundaryL > entity->newXBoundaryL)
            entity->curXBoundaryL = entity->newXBoundaryL;
    }

    if (entity->curXBoundaryR == entity->newXBoundaryR) {
        ++count;
    }
    else {
        if (entity->curXBoundaryR < entity->newXBoundaryR) {
            entity->curXBoundaryR += entity->boundsMoveSpeed.x;
            if (entity->curXBoundaryR > entity->newXBoundaryR)
                entity->curXBoundaryR = entity->newXBoundaryR;
        }
        else {
            entity->curXBoundaryR -= entity->boundsMoveSpeed.x;
            if (entity->curXBoundaryR < entity->newXBoundaryR)
                entity->curXBoundaryR = entity->newXBoundaryR;
        }
    }

    if (entity->curYBoundaryT == entity->newYBoundaryT) {
        ++count;
    }
    else if (entity->curYBoundaryT >= entity->newYBoundaryT) {
        entity->curYBoundaryT -= entity->boundsMoveSpeed.y;
        if (entity->curYBoundaryT < entity->newYBoundaryT)
            entity->curYBoundaryT = entity->newYBoundaryT;
    }
    else {
        entity->curYBoundaryT += entity->boundsMoveSpeed.y;
        if (entity->curYBoundaryT > entity->newYBoundaryT)
            entity->curYBoundaryT = entity->newYBoundaryT;
    }

    if (entity->curYBoundaryB == entity->newYBoundaryB) {
        ++count;
    }
    else {
        if (entity->curYBoundaryB < entity->newYBoundaryB) {
            entity->curYBoundaryB += entity->boundsMoveSpeed.y;
            if (entity->curYBoundaryB > entity->newYBoundaryB)
                entity->curYBoundaryB = entity->newYBoundaryB;
        }
        else {
            entity->curYBoundaryB -= entity->boundsMoveSpeed.y;
            if (entity->curYBoundaryB < entity->newYBoundaryB)
                entity->curYBoundaryB = entity->newYBoundaryB;
        }
    }

    entity->left   = entity->curXBoundaryL >> 16;
    entity->top    = entity->curYBoundaryT >> 16;
    entity->right  = entity->curXBoundaryR >> 16;
    entity->bottom = entity->curYBoundaryB >> 16;

    if (count == 4)
        LEDPanel_HandleCharacters();
}

void LEDPanel_StateText_WaitForSignal(void)
{
    RSDK_THIS(LEDPanel);

    int row = entity->row ^ 1;
    if (entity->rowFlag[row]) {
        entity->rowFlag[row]          = false;
        entity->stateRow[entity->row] = StateMachine_None;
        LEDPanel_HandleCharacters();
    }
}

#if RETRO_INCLUDE_EDITOR
void LEDPanel_EditorDraw(void)
{
    RSDK_THIS(LEDPanel);
    Vector2 drawPos = entity->position;

    const char *text = "LEDPANEL";

    drawPos.x -= 4 * 0x100000;

    for (int i = 0; i < 8; ++i) {
        entity->animatorText.frameID = text[i] - 'A';

        RSDK.DrawSprite(&entity->animatorText, &drawPos, false);

        drawPos.x += 0x100000;
    }
}

void LEDPanel_EditorLoad(void) { LEDPanel->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/LED.bin", SCOPE_STAGE); }
#endif

void LEDPanel_Serialize(void)
{
    RSDK_EDITABLE_ARRAY(LEDPanel, VAR_STRING, text, LEDPanel_TextCount);
    RSDK_EDITABLE_ARRAY(LEDPanel, VAR_STRING, seq, LEDPanel_RowCount);
    RSDK_EDITABLE_VAR(LEDPanel, VAR_VECTOR2, size);
}
