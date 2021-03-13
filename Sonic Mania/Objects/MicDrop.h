#ifndef OBJ_MICDROP_H
#define OBJ_MICDROP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMicDrop;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMicDrop;

// Object Struct
extern ObjectMicDrop *MicDrop;

// Standard Entity Events
void MicDrop_Update();
void MicDrop_LateUpdate();
void MicDrop_StaticUpdate();
void MicDrop_Draw();
void MicDrop_Create(void* data);
void MicDrop_StageLoad();
void MicDrop_EditorDraw();
void MicDrop_EditorLoad();
void MicDrop_Serialize();

// Extra Entity Functions


#endif //!OBJ_MICDROP_H
