#ifndef OBJ_CLAPPERBOARD_H
#define OBJ_CLAPPERBOARD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectClapperboard;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityClapperboard;

// Object Struct
extern ObjectClapperboard *Clapperboard;

// Standard Entity Events
void Clapperboard_Update(void);
void Clapperboard_LateUpdate(void);
void Clapperboard_StaticUpdate(void);
void Clapperboard_Draw(void);
void Clapperboard_Create(void* data);
void Clapperboard_StageLoad(void);
void Clapperboard_EditorDraw(void);
void Clapperboard_EditorLoad(void);
void Clapperboard_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CLAPPERBOARD_H
