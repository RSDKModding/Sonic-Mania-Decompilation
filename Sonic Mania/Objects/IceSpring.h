#ifndef OBJ_ICESPRING_H
#define OBJ_ICESPRING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectIceSpring;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityIceSpring;

// Object Struct
extern ObjectIceSpring *IceSpring;

// Standard Entity Events
void IceSpring_Update();
void IceSpring_LateUpdate();
void IceSpring_StaticUpdate();
void IceSpring_Draw();
void IceSpring_Create(void* data);
void IceSpring_StageLoad();
void IceSpring_EditorDraw();
void IceSpring_EditorLoad();
void IceSpring_Serialize();

// Extra Entity Functions


#endif //!OBJ_ICESPRING_H
