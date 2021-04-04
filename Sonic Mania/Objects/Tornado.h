#ifndef OBJ_TORNADO_H
#define OBJ_TORNADO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[5]; //= { -131072, -65536, 0, 65536, 131072 };
    ushort value2;
    ushort value3;
    ushort value4;
    ushort value5;
} ObjectTornado;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTornado;

// Object Struct
extern ObjectTornado *Tornado;

// Standard Entity Events
void Tornado_Update(void);
void Tornado_LateUpdate(void);
void Tornado_StaticUpdate(void);
void Tornado_Draw(void);
void Tornado_Create(void* data);
void Tornado_StageLoad(void);
void Tornado_EditorDraw(void);
void Tornado_EditorLoad(void);
void Tornado_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TORNADO_H
