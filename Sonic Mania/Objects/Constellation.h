#ifndef OBJ_CONSTELLATION_H
#define OBJ_CONSTELLATION_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectConstellation;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityConstellation;

// Object Struct
extern ObjectConstellation *Constellation;

// Standard Entity Events
void Constellation_Update(void);
void Constellation_LateUpdate(void);
void Constellation_StaticUpdate(void);
void Constellation_Draw(void);
void Constellation_Create(void* data);
void Constellation_StageLoad(void);
void Constellation_EditorDraw(void);
void Constellation_EditorLoad(void);
void Constellation_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CONSTELLATION_H
