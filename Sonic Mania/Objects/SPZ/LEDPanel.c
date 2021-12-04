#include "SonicMania.h"

ObjectLEDPanel *LEDPanel;

void LEDPanel_Update(void)
{
    RSDK_THIS(LEDPanel);

    for (self->row = 0; self->row < LEDPanel_RowCount; ++self->row) {
        StateMachine_Run(self->stateRow[self->row]);
    }
}

void LEDPanel_LateUpdate(void) {}

void LEDPanel_StaticUpdate(void) {}

void LEDPanel_Draw(void)
{
    RSDK_THIS(LEDPanel);

    RSDK.DrawRect(self->position.x - (self->size.x >> 1) + 0x80000, self->position.y - (self->size.y >> 1), self->size.x - 0x100000,
                  self->size.y, LEDPanel->storedColour, 512, INK_NONE, false);

    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    int clipX1 = screen->clipBound_X1;
    int clipX2 = screen->clipBound_X2;
    int clipY1 = screen->clipBound_Y1;
    int clipY2 = screen->clipBound_Y2;

    int clipBound_X1 = self->left - screen->position.x + (self->position.x >> 16) + 8;
    int clipBound_X2 = self->right - screen->position.x + (self->position.x >> 16) - 8;
    int clipBound_Y1 = (self->position.y >> 16) + self->top - screen->position.y;
    int clipBound_Y2 = (self->position.y >> 16) + self->bottom - screen->position.y;

    RSDK.SetClipBounds(SceneInfo->currentScreenID, clipBound_X1, clipBound_Y1, clipBound_X2, clipBound_Y2);

    for (int r = 0; r < LEDPanel_RowCount; ++r) {
        RSDK.DrawText(&self->animatorText, &self->textPos[r], &self->activeText[r], 0, self->activeText[r].length, ALIGN_RIGHT, 0, 0,
                      NULL, false);
    }

    screen->clipBound_X1 = clipX1;
    screen->clipBound_X2 = clipX2;
    screen->clipBound_Y1 = clipY1;
    screen->clipBound_Y2 = clipY2;
    RSDK.SetClipBounds(SceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
}

void LEDPanel_Create(void *data)
{
    RSDK_THIS(LEDPanel);

    self->active      = ACTIVE_BOUNDS;
    self->visible     = true;
    self->drawOrder   = Zone->drawOrderLow;
    self->updateRange = self->size;
    RSDK.SetSpriteAnimation(LEDPanel->aniFrames, 0, &self->animatorText, true, 0);
    if (!SceneInfo->inEditor) {
        for (int i = 0; i < LEDPanel_TextCount; ++i) {
            RSDK.SetSpriteString(LEDPanel->aniFrames, 0, &self->text[i]);
            self->textPtrs[i] = &self->text[i];
        }

        self->left   = -(self->size.x >> 17);
        self->right  = self->size.x >> 17;
        self->top    = -(self->size.y >> 17);
        self->bottom = self->size.y >> 17;

        self->curXBoundaryL = (self->size.x >> 1);
        self->newXBoundaryL = (self->size.x >> 1);
        self->curYBoundaryT = -(self->size.y >> 1);
        self->newYBoundaryT = -(self->size.y >> 1);
        self->curXBoundaryR = -(self->size.x >> 1);
        self->newXBoundaryR = -(self->size.x >> 1);
        self->curYBoundaryB = -(self->size.y >> 1);
        self->newYBoundaryB = -(self->size.y >> 1);

        self->boundsMoveSpeed.x = 0x10000;
        self->boundsMoveSpeed.y = 0x10000;

        int offset = 0;
        for (int r = 0; r < LEDPanel_RowCount; ++r) {
            LEDPanel_SetupActiveText(r, self->textPtrs[r]);

            self->seqPtrs[r]       = &self->seq[r];
            self->textMoveVel[r].x = 0x10000;
            self->textMoveVel[r].y = 0x10000;

            self->textPos[r].x = self->position.x;
            self->textPos[r].y = self->position.y;
            self->textPos[r].x += (self->activeText[r].length << 20) - (self->size.x >> 1) + 0x80000;
            self->textPos[r].y += (0x100000 + offset) - (self->size.y >> 1);

            self->textMovePos[r].x = 0;
            self->textMovePos[r].y = offset;

            self->textTargetPos[r].x = 0;
            self->textTargetPos[r].y = offset;

            self->stateRow[r] = StateMachine_None;

            offset += 0x200000;
        }

        for (self->row = 0; self->row < LEDPanel_RowCount; ++self->row) {
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

    RSDK.CopyString(&self->activeText[row], src);

    self->activeTextSize[row] = self->activeText[row].length;
    for (int i = 0; i < self->activeText[row].length; ++i) {
        if (self->activeText[row].text[i] != 27) {
            self->activeTextSize[row] = i;
            break;
        }
    }

    self->activeTextLen[row] = 0;
    for (int i = self->activeText[row].length - 1; i >= 0; --i) {
        if (self->activeText[row].text[i] != 27) {
            self->activeTextLen[row] = i;
            break;
        }
    }
}

void LEDPanel_SetupTextPos(int row, int x, int y)
{
    RSDK_THIS(LEDPanel);

    self->textPos[row].x = self->position.x;
    self->textPos[row].y = self->position.y;
    self->textPos[row].x += (self->activeText[row].length << 20) - (self->size.x >> 1) + x + 0x80000;
    self->textPos[row].y += y - (self->size.y >> 1) + 0x100000;
}

void LEDPanel_HandleCharacters(void)
{
    RSDK_THIS(LEDPanel);

    self->rowFlag[self->row] = false;
    bool32 flag                  = false;

    while (!flag) {
        int charID = -1;
        if (self->rowSeqPos[self->row] < self->seqPtrs[self->row]->length)
            charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++];

        switch (charID) {
            case 'A':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';
                switch (charID) {
                    case 0:
                        self->textMovePos[self->row].x   = -0x100000 * self->activeText[self->row].length;
                        self->textTargetPos[self->row].x = self->textMovePos[self->row].x;
                        break;
                    case 1:
                        self->textMovePos[self->row].x   = 0;
                        self->textTargetPos[self->row].x = 0;
                        break;
                    case 2:
                        self->textMovePos[self->row].x   = (self->size.x >> 1) - (self->activeText[self->row].length << 19) - 0x80000;
                        self->textTargetPos[self->row].x = self->textMovePos[self->row].x;
                        break;
                    case 3:
                        self->textMovePos[self->row].x   = self->size.x - (self->activeText[self->row].length << 20) - 0x100000;
                        self->textTargetPos[self->row].x = self->textMovePos[self->row].x;
                        break;
                    case 4:
                        self->textMovePos[self->row].x   = self->size.x;
                        self->textTargetPos[self->row].x = self->textMovePos[self->row].x;
                        break;
                    default: break;
                }
                LEDPanel_SetupTextPos(self->row, self->textMovePos[self->row].x, self->textMovePos[self->row].y);
                break;
            case 'B':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';
                switch (charID) {
                    case 0:
                        self->textMovePos[self->row].y   = -0x200000;
                        self->textTargetPos[self->row].y = -0x200000;
                        break;
                    case 1:
                        self->textMovePos[self->row].y   = 0;
                        self->textTargetPos[self->row].y = 0;
                        break;
                    case 2:
                        self->textMovePos[self->row].y   = (self->size.y >> 1) - 0x100000;
                        self->textTargetPos[self->row].y = self->textMovePos[self->row].y;
                        break;
                    case 3:
                        self->textMovePos[self->row].y   = self->size.y - 0x200000;
                        self->textTargetPos[self->row].y = self->textMovePos[self->row].y;
                        break;
                    case 4:
                        self->textMovePos[self->row].y   = self->size.y;
                        self->textTargetPos[self->row].y = self->textMovePos[self->row].y;
                        break;
                    default: break;
                }

                LEDPanel_SetupTextPos(self->row, self->textMovePos[self->row].x, self->textMovePos[self->row].y);
                break;
            case 'C':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';
                switch (charID) {
                    case 0: self->textTargetPos[self->row].x = -0x100000 * self->activeText[self->row].length; break;
                    case 1: self->textTargetPos[self->row].x = 0; break;
                    case 2: self->textTargetPos[self->row].x = (self->size.x >> 1) - (self->activeText[self->row].length << 19) - 0x80000;
                        break;
                    case 3: self->textTargetPos[self->row].x = self->size.x - (self->activeText[self->row].length << 20) - 0x100000;
                        break;
                    case 4: self->textTargetPos[self->row].x = self->size.x; break;
                    default: break;
                }
                break;
            case 'D':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';
                switch (charID) {
                    case 0: self->textTargetPos[self->row].y = -0x200000; break;
                    case 1: self->textTargetPos[self->row].y = 0; break;
                    case 2: self->textTargetPos[self->row].y = (self->size.y >> 1) - 0x100000; break;
                    case 3: self->textTargetPos[self->row].y = self->size.y - 0x200000; break;
                    case 4: self->textTargetPos[self->row].y = self->size.y; break;
                    default: break;
                }
                break;
            case 'E':
                self->stateRow[self->row] = LEDPanel_StateText_Move;
                LEDPanel_StateText_Move();
                flag = true;
                break;
            case 'F':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                if (charID == 0) {
                    ++self->activeTextLen[self->row];
                    LEDPanel->text.text[0] = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++];
                    RSDK.SetSpriteString(LEDPanel->aniFrames, 0, &LEDPanel->text);
                    self->activeText[self->row].text[self->activeTextLen[self->row]] = LEDPanel->text.text[0];
                }
                else if (charID == 1) {
                    --self->activeTextSize[self->row];
                    LEDPanel->text.text[0] = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++];
                    RSDK.SetSpriteString(LEDPanel->aniFrames, 0, &LEDPanel->text);
                    self->activeText[self->row].text[self->activeTextSize[self->row]] = LEDPanel->text.text[0];
                }

                break;
            case 'G':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                if (charID == 0) {
                    self->activeText[self->row].text[self->activeTextLen[self->row]--] = 27;
                }
                else if (charID == 1) {
                    self->activeText[self->row].text[self->activeTextSize[self->row]++] = 27;
                }
                break;
            case 'H':
                self->stateRow[self->row] = LEDPanel_StateText_WaitForSignal;
                LEDPanel_StateText_WaitForSignal();
                flag = true;
                break;
            case 'I':
                self->rowFlag[self->row]  = true;
                self->stateRow[self->row] = LEDPanel_HandleCharacters;
                flag                          = true;
                break;
            case 'J': 
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';
                LEDPanel_SetupActiveText(self->row, self->textPtrs[charID]);
                break;
            case 'K':
                self->rowDelay[self->row] = 15;
                self->stateRow[self->row] = LEDPanel_StateText_Delay;
                flag                          = true;
                break;
            case 'L': self->textMoveVel[self->row].x = (self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '/') << 15; break;
            case 'M': self->textMoveVel[self->row].y = (self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '/') << 15; break;
            case 'N': self->boundsMoveSpeed.x = (self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '/') << 15; break;
            case 'O': self->boundsMoveSpeed.y = (self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '/') << 15; break;
            case 'P':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        self->newXBoundaryL = -(self->size.x >> 1);
                        self->curXBoundaryL = -(self->size.x >> 1);
                        self->left          = self->curXBoundaryL >> 16;
                        break;
                    case 2:
                        self->curXBoundaryL = 0;
                        self->newXBoundaryL = 0;
                        self->left          = 0;
                        break;
                    case 3:
                        self->newXBoundaryL = self->size.x >> 1;
                        self->curXBoundaryL = self->size.x >> 1;
                        self->left          = self->curXBoundaryL >> 16;
                        break;
                }
                break;
            case 'Q':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        self->newYBoundaryT = -(self->size.y >> 1);
                        self->curYBoundaryT = -(self->size.y >> 1);
                        self->top           = self->curYBoundaryT >> 16;
                        break;
                    case 2:
                        self->curYBoundaryT = 0;
                        self->newYBoundaryT = 0;
                        self->top           = 0;
                        break;
                    case 3:
                        self->newYBoundaryT = self->size.y >> 1;
                        self->curYBoundaryT = self->size.y >> 1;
                        self->top           = self->curYBoundaryT >> 16;
                        break;
                }
                break;
            case 'R':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        self->newXBoundaryR = -(self->size.x >> 1);
                        self->curXBoundaryR = -(self->size.x >> 1);
                        self->right         = self->curXBoundaryR >> 16;
                        break;
                    case 2:
                        self->curXBoundaryR = 0;
                        self->newXBoundaryR = 0;
                        self->right         = 0;
                        break;
                    case 3:
                        self->newXBoundaryR = self->size.x >> 1;
                        self->curXBoundaryR = self->size.x >> 1;
                        self->right         = self->curXBoundaryR >> 16;
                        break;
                }
                break;
            case 'S':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1:
                        self->newYBoundaryB = -(self->size.y >> 1);
                        self->curYBoundaryB = -(self->size.y >> 1);
                        self->bottom        = self->curYBoundaryB >> 16;
                        break;
                    case 2:
                        self->curYBoundaryB = 0;
                        self->newYBoundaryB = 0;
                        self->bottom        = 0;
                        break;
                    case 3:
                        self->newYBoundaryB = self->size.y >> 1;
                        self->curYBoundaryB = self->size.y >> 1;
                        self->bottom        = self->curYBoundaryB >> 16;
                        break;
                }
                break;
            case 'T':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: self->newXBoundaryL = -(self->size.x >> 1); break;
                    case 2: self->newXBoundaryL = 0; break;
                    case 3: self->newXBoundaryL = self->size.x >> 1; break;
                }
                break;
            case 'U':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: self->newYBoundaryT = -(self->size.y >> 1); break;
                    case 2: self->newYBoundaryT = 0; break;
                    case 3: self->newYBoundaryT = self->size.y >> 1; break;
                }
                break;
            case 'V':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: self->newXBoundaryR = -(self->size.x >> 1); break;
                    case 2: self->newXBoundaryR = 0; break;
                    case 3: self->newXBoundaryR = self->size.x >> 1; break;
                }
                break;
            case 'W':
                charID = self->seqPtrs[self->row]->text[self->rowSeqPos[self->row]++] - '0';

                switch (charID) {
                    default: break;
                    case 1: self->newYBoundaryB = -(self->size.y >> 1); break;
                    case 2: self->newYBoundaryB = 0; break;
                    case 3: self->newYBoundaryB = self->size.y >> 1; break;
                }
                break;
            case 'X':
                self->stateRow[self->row] = LEDPanel_StateText_ChangeClipBounds;
                LEDPanel_StateText_ChangeClipBounds();
                flag = true;
                break;
            default:
                self->rowSeqPos[self->row] = 0;
                break;
        }
    }
}

void LEDPanel_StateText_Move(void)
{
    RSDK_THIS(LEDPanel);

    if (self->textMovePos[self->row].x != self->textTargetPos[self->row].x) {
        if (self->textMovePos[self->row].x >= self->textTargetPos[self->row].x) {
            self->textMovePos[self->row].x -= self->textMoveVel[self->row].x;

            if (self->textMovePos[self->row].x < self->textTargetPos[self->row].x)
                self->textMovePos[self->row].x = self->textTargetPos[self->row].x;
        }
        else {
            self->textMovePos[self->row].x += self->textMoveVel[self->row].x;

            if (self->textMovePos[self->row].x > self->textTargetPos[self->row].x)
                self->textMovePos[self->row].x = self->textTargetPos[self->row].x;
        }
    }

    if (self->textMovePos[self->row].y != self->textTargetPos[self->row].y) {
        if (self->textMovePos[self->row].y >= self->textTargetPos[self->row].y) {
            self->textMovePos[self->row].y -= self->textMoveVel[self->row].y;

            if (self->textMovePos[self->row].y < self->textTargetPos[self->row].y)
                self->textMovePos[self->row].y = self->textTargetPos[self->row].y;
        }
        else {
            self->textMovePos[self->row].y += self->textMoveVel[self->row].y;

            if (self->textMovePos[self->row].y > self->textTargetPos[self->row].y)
                self->textMovePos[self->row].y = self->textTargetPos[self->row].y;
        }
    }

    self->textPos[self->row].x = self->position.x;
    self->textPos[self->row].y = self->position.y;
    self->textPos[self->row].x += (self->activeText[self->row].length << 20) - (self->size.x >> 1) + self->textMovePos[self->row].x + 0x80000;
    self->textPos[self->row].y += self->textMovePos[self->row].y - (self->size.y >> 1) + 0x100000;

    if (self->textMovePos[self->row].x == self->textTargetPos[self->row].x) {
        if (self->textMovePos[self->row].y == self->textTargetPos[self->row].y)
            LEDPanel_HandleCharacters();
    }
}

void LEDPanel_StateText_Delay(void)
{
    RSDK_THIS(LEDPanel);

    if (--self->rowDelay[self->row] <= 0)
        LEDPanel_HandleCharacters();
}

void LEDPanel_StateText_ChangeClipBounds(void)
{
    RSDK_THIS(LEDPanel);

    int count = 0;

    if (self->curXBoundaryL == self->newXBoundaryL) {
        count = 1;
    }
    else if (self->curXBoundaryL >= self->newXBoundaryL) {
        self->curXBoundaryL -= self->boundsMoveSpeed.x;
        if (self->curXBoundaryL < self->newXBoundaryL)
            self->curXBoundaryL = self->newXBoundaryL;
    }
    else {
        self->curXBoundaryL += self->boundsMoveSpeed.x;
        if (self->curXBoundaryL > self->newXBoundaryL)
            self->curXBoundaryL = self->newXBoundaryL;
    }

    if (self->curXBoundaryR == self->newXBoundaryR) {
        ++count;
    }
    else {
        if (self->curXBoundaryR < self->newXBoundaryR) {
            self->curXBoundaryR += self->boundsMoveSpeed.x;
            if (self->curXBoundaryR > self->newXBoundaryR)
                self->curXBoundaryR = self->newXBoundaryR;
        }
        else {
            self->curXBoundaryR -= self->boundsMoveSpeed.x;
            if (self->curXBoundaryR < self->newXBoundaryR)
                self->curXBoundaryR = self->newXBoundaryR;
        }
    }

    if (self->curYBoundaryT == self->newYBoundaryT) {
        ++count;
    }
    else if (self->curYBoundaryT >= self->newYBoundaryT) {
        self->curYBoundaryT -= self->boundsMoveSpeed.y;
        if (self->curYBoundaryT < self->newYBoundaryT)
            self->curYBoundaryT = self->newYBoundaryT;
    }
    else {
        self->curYBoundaryT += self->boundsMoveSpeed.y;
        if (self->curYBoundaryT > self->newYBoundaryT)
            self->curYBoundaryT = self->newYBoundaryT;
    }

    if (self->curYBoundaryB == self->newYBoundaryB) {
        ++count;
    }
    else {
        if (self->curYBoundaryB < self->newYBoundaryB) {
            self->curYBoundaryB += self->boundsMoveSpeed.y;
            if (self->curYBoundaryB > self->newYBoundaryB)
                self->curYBoundaryB = self->newYBoundaryB;
        }
        else {
            self->curYBoundaryB -= self->boundsMoveSpeed.y;
            if (self->curYBoundaryB < self->newYBoundaryB)
                self->curYBoundaryB = self->newYBoundaryB;
        }
    }

    self->left   = self->curXBoundaryL >> 16;
    self->top    = self->curYBoundaryT >> 16;
    self->right  = self->curXBoundaryR >> 16;
    self->bottom = self->curYBoundaryB >> 16;

    if (count == 4)
        LEDPanel_HandleCharacters();
}

void LEDPanel_StateText_WaitForSignal(void)
{
    RSDK_THIS(LEDPanel);

    int row = self->row ^ 1;
    if (self->rowFlag[row]) {
        self->rowFlag[row]          = false;
        self->stateRow[self->row] = StateMachine_None;
        LEDPanel_HandleCharacters();
    }
}

#if RETRO_INCLUDE_EDITOR
void LEDPanel_EditorDraw(void)
{
    RSDK_THIS(LEDPanel);
    Vector2 drawPos = self->position;

    const char *text = "LEDPANEL";

    drawPos.x -= 4 * 0x100000;

    for (int i = 0; i < 8; ++i) {
        self->animatorText.frameID = text[i] - 'A';

        RSDK.DrawSprite(&self->animatorText, &drawPos, false);

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
