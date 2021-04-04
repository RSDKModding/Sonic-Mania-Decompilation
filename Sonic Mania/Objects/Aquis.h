#ifndef OBJ_AQUIS_H
#define OBJ_AQUIS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAquis;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAquis;

// Object Struct
extern ObjectAquis *Aquis;

// Standard Entity Events
void Aquis_Update(void);
void Aquis_LateUpdate(void);
void Aquis_StaticUpdate(void);
void Aquis_Draw(void);
void Aquis_Create(void* data);
void Aquis_StageLoad(void);
void Aquis_EditorDraw(void);
void Aquis_EditorLoad(void);
void Aquis_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_AQUIS_H
