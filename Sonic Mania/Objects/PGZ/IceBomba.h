#ifndef OBJ_ICEBOMBA_H
#define OBJ_ICEBOMBA_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectIceBomba;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityIceBomba;

// Object Struct
extern ObjectIceBomba *IceBomba;

// Standard Entity Events
void IceBomba_Update(void);
void IceBomba_LateUpdate(void);
void IceBomba_StaticUpdate(void);
void IceBomba_Draw(void);
void IceBomba_Create(void* data);
void IceBomba_StageLoad(void);
void IceBomba_EditorDraw(void);
void IceBomba_EditorLoad(void);
void IceBomba_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ICEBOMBA_H
