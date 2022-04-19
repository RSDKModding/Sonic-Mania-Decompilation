#ifndef OBJ_LEDPANEL_H
#define OBJ_LEDPANEL_H

#include "SonicMania.h"

#define LEDPanel_TextCount (8)
#define LEDPanel_RowCount  (2)

// Object Class
struct ObjectLEDPanel {
    RSDK_OBJECT
    uint16 aniFrames;
    TextInfo text;
    color storedColor;
};

// Entity Class
struct EntityLEDPanel {
    RSDK_ENTITY
    TextInfo text[LEDPanel_TextCount];
    TextInfo seq[LEDPanel_RowCount];
    TextInfo *seqPtrs[LEDPanel_RowCount];
    int32 rowSeqPos[LEDPanel_RowCount];
    StateMachine(stateRow[LEDPanel_RowCount]);
    TextInfo *textPtrs[LEDPanel_TextCount];
    TextInfo activeText[LEDPanel_RowCount];
    Vector2 textMovePos[LEDPanel_RowCount];
    Vector2 textPos[LEDPanel_RowCount];
    Vector2 textTargetPos[LEDPanel_RowCount];
    Vector2 textMoveVel[LEDPanel_RowCount];
    int32 activeTextSize[LEDPanel_RowCount];
    int32 activeTextLen[LEDPanel_RowCount];
    bool32 rowSignaled[LEDPanel_RowCount];
    Vector2 size;
    int32 left;
    int32 right;
    int32 top;
    int32 bottom;
    int32 curXBoundaryL;
    int32 curYBoundaryT;
    int32 curXBoundaryR;
    int32 curYBoundaryB;
    int32 newXBoundaryL;
    int32 newYBoundaryT;
    int32 newXBoundaryR;
    int32 newYBoundaryB;
    Vector2 boundsMoveSpeed;
    int32 rowDelay[LEDPanel_RowCount];
    uint8 row;
    Animator animatorText;
};

// Object Struct
extern ObjectLEDPanel *LEDPanel;

// Standard Entity Events
void LEDPanel_Update(void);
void LEDPanel_LateUpdate(void);
void LEDPanel_StaticUpdate(void);
void LEDPanel_Draw(void);
void LEDPanel_Create(void* data);
void LEDPanel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LEDPanel_EditorDraw(void);
void LEDPanel_EditorLoad(void);
#endif
void LEDPanel_Serialize(void);

// Extra Entity Functions
void LEDPanel_SetupActiveText(int index, TextInfo *src);
void LEDPanel_SetupTextPos(int index, int x, int y);
void LEDPanel_HandleCharacters(void);

void LEDPanel_StateText_Move(void);
void LEDPanel_StateText_Delay(void);
void LEDPanel_StateText_ChangeClipBounds(void);
void LEDPanel_StateText_WaitForSignal(void);

#endif //!OBJ_LEDPANEL_H
