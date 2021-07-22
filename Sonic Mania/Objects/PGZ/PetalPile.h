#ifndef OBJ_PETALPILE_H
#define OBJ_PETALPILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[5]; //= { 7, 6, 6, 10, 3 };
    int value2[14]; //= { -65536, 65536, -32768, -65536, -21845, 65536, 0, -65536, 21845, 65536, 32768, -65536, 65536, 65536 };
    int value3[12]; //= { -65536, 65536, -21845, -21845, 65536, -65536, 0, 43690, 43690, 0, 65536, 65536 };
    int value4[12]; //= { 65536, 65536, 21845, -21845, -65536, -65536, 0, 43690, -43690, 0, -65536, 65536 };
    int value5[20]; //= { -65536, 65536, -32768, -65536, -21845, 65536, 0, -65536, 21845, 65536, 32768, -65536, 65536, 65536, -43690, 0, 0, 0, 43690, 0 };
    int value6[6]; //= { 0, -65536, -65536, 65536, 65536, 65536 };
    ushort value7;
    ushort value8;
} ObjectPetalPile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPetalPile;

// Object Struct
extern ObjectPetalPile *PetalPile;

// Standard Entity Events
void PetalPile_Update(void);
void PetalPile_LateUpdate(void);
void PetalPile_StaticUpdate(void);
void PetalPile_Draw(void);
void PetalPile_Create(void* data);
void PetalPile_StageLoad(void);
void PetalPile_EditorDraw(void);
void PetalPile_EditorLoad(void);
void PetalPile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PETALPILE_H
