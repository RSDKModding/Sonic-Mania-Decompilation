#ifndef OBJ_SELTZERBOTTLE_H
#define OBJ_SELTZERBOTTLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSeltzerBottle;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySeltzerBottle;

// Object Struct
extern ObjectSeltzerBottle *SeltzerBottle;

// Standard Entity Events
void SeltzerBottle_Update(void);
void SeltzerBottle_LateUpdate(void);
void SeltzerBottle_StaticUpdate(void);
void SeltzerBottle_Draw(void);
void SeltzerBottle_Create(void* data);
void SeltzerBottle_StageLoad(void);
void SeltzerBottle_EditorDraw(void);
void SeltzerBottle_EditorLoad(void);
void SeltzerBottle_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SELTZERBOTTLE_H
