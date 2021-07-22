#ifndef OBJ_PRINTBLOCK_H
#define OBJ_PRINTBLOCK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    ushort aniFrames;
    ushort sfxLetter;
} ObjectPrintBlock;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    ushort interval;
    ushort intervalOffset;
    ushort duration;
    byte letter;
} EntityPrintBlock;

// Object Struct
extern ObjectPrintBlock *PrintBlock;

// Standard Entity Events
void PrintBlock_Update(void);
void PrintBlock_LateUpdate(void);
void PrintBlock_StaticUpdate(void);
void PrintBlock_Draw(void);
void PrintBlock_Create(void* data);
void PrintBlock_StageLoad(void);
void PrintBlock_EditorDraw(void);
void PrintBlock_EditorLoad(void);
void PrintBlock_Serialize(void);

// Extra Entity Functions
void PrintBlock_Unknown1(void);
void PrintBlock_Unknown2(void);

#endif //!OBJ_PRINTBLOCK_H
