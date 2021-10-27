#ifndef OBJ_MICDROP_H
#define OBJ_MICDROP_H

#include "SonicMania.h"

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
void MicDrop_Update(void);
void MicDrop_LateUpdate(void);
void MicDrop_StaticUpdate(void);
void MicDrop_Draw(void);
void MicDrop_Create(void* data);
void MicDrop_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MicDrop_EditorDraw(void);
void MicDrop_EditorLoad(void);
#endif
void MicDrop_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MICDROP_H
