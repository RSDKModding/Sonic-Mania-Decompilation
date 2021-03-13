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
void SeltzerBottle_Update();
void SeltzerBottle_LateUpdate();
void SeltzerBottle_StaticUpdate();
void SeltzerBottle_Draw();
void SeltzerBottle_Create(void* data);
void SeltzerBottle_StageLoad();
void SeltzerBottle_EditorDraw();
void SeltzerBottle_EditorLoad();
void SeltzerBottle_Serialize();

// Extra Entity Functions


#endif //!OBJ_SELTZERBOTTLE_H
