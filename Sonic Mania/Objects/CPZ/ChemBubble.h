#ifndef OBJ_CHEMBUBBLE_H
#define OBJ_CHEMBUBBLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectChemBubble;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 shiftY2;
    int32 shiftY;
    Vector2 startPos;
    Entity *parent;
    Animator animator;
} EntityChemBubble;

// Object Struct
extern ObjectChemBubble *ChemBubble;

// Standard Entity Events
void ChemBubble_Update(void);
void ChemBubble_LateUpdate(void);
void ChemBubble_StaticUpdate(void);
void ChemBubble_Draw(void);
void ChemBubble_Create(void* data);
void ChemBubble_StageLoad(void);
void ChemBubble_EditorDraw(void);
void ChemBubble_EditorLoad(void);
void ChemBubble_Serialize(void);

// Extra Entity Functions
void ChemBubble_Unknown1(void);
void ChemBubble_Unknown2(void);
void ChemBubble_Unknown3(void);

#endif //!OBJ_CHEMBUBBLE_H
