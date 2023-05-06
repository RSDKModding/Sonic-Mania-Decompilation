#ifndef OBJ_PRINTBLOCK_H
#define OBJ_PRINTBLOCK_H

#include "Game.h"

typedef enum {
    PRINTBLOCK_LETTER_E,
    PRINTBLOCK_LETTER_T,
    PRINTBLOCK_LETTER_A,
    PRINTBLOCK_LETTER_O,
    PRINTBLOCK_LETTER_I,
    PRINTBLOCK_LETTER_N,
    PRINTBLOCK_LETTER_S,
    PRINTBLOCK_LETTER_H,
    PRINTBLOCK_LETTER_R,
    PRINTBLOCK_LETTER_D,
    PRINTBLOCK_LETTER_L,
    PRINTBLOCK_LETTER_U,
} PrintBlockLetters;

// Object Class
struct ObjectPrintBlock {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    uint16 sfxLetter;
};

// Entity Class
struct EntityPrintBlock {
    MANIA_PLATFORM_BASE
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    uint8 letter;
};

// Object Struct
extern ObjectPrintBlock *PrintBlock;

// Standard Entity Events
void PrintBlock_Update(void);
void PrintBlock_LateUpdate(void);
void PrintBlock_StaticUpdate(void);
void PrintBlock_Draw(void);
void PrintBlock_Create(void *data);
void PrintBlock_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PrintBlock_EditorDraw(void);
void PrintBlock_EditorLoad(void);
#endif
void PrintBlock_Serialize(void);

// Extra Entity Functions
void PrintBlock_State_Appear(void);
void PrintBlock_State_Disappear(void);

#endif //! OBJ_PRINTBLOCK_H
