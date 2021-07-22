#ifndef OBJ_SPZ2OUTRO_H
#define OBJ_SPZ2OUTRO_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSPZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySPZ2Outro;

// Object Struct
extern ObjectSPZ2Outro *SPZ2Outro;

// Standard Entity Events
void SPZ2Outro_Update(void);
void SPZ2Outro_LateUpdate(void);
void SPZ2Outro_StaticUpdate(void);
void SPZ2Outro_Draw(void);
void SPZ2Outro_Create(void* data);
void SPZ2Outro_StageLoad(void);
void SPZ2Outro_EditorDraw(void);
void SPZ2Outro_EditorLoad(void);
void SPZ2Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPZ2OUTRO_H
