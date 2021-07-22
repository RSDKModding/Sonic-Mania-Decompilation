#ifndef OBJ_PSZ2OUTRO_H
#define OBJ_PSZ2OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPSZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZ2Outro;

// Object Struct
extern ObjectPSZ2Outro *PSZ2Outro;

// Standard Entity Events
void PSZ2Outro_Update(void);
void PSZ2Outro_LateUpdate(void);
void PSZ2Outro_StaticUpdate(void);
void PSZ2Outro_Draw(void);
void PSZ2Outro_Create(void* data);
void PSZ2Outro_StageLoad(void);
void PSZ2Outro_EditorDraw(void);
void PSZ2Outro_EditorLoad(void);
void PSZ2Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PSZ2OUTRO_H
