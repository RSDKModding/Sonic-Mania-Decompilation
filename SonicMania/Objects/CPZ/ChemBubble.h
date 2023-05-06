#ifndef OBJ_CHEMBUBBLE_H
#define OBJ_CHEMBUBBLE_H

#include "Game.h"

// Object Class
struct ObjectChemBubble {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityChemBubble {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 amplitude;
    int32 angleShift;
    Vector2 startPos;
    EntityChemicalPool *parent;
    Animator animator;
};

// Object Struct
extern ObjectChemBubble *ChemBubble;

// Standard Entity Events
void ChemBubble_Update(void);
void ChemBubble_LateUpdate(void);
void ChemBubble_StaticUpdate(void);
void ChemBubble_Draw(void);
void ChemBubble_Create(void *data);
void ChemBubble_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ChemBubble_EditorDraw(void);
void ChemBubble_EditorLoad(void);
#endif
void ChemBubble_Serialize(void);

// Extra Entity Functions
void ChemBubble_State_Appear(void);
void ChemBubble_State_Rising(void);
void ChemBubble_State_Surfaced(void);

#endif //! OBJ_CHEMBUBBLE_H
