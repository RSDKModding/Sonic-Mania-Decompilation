#ifndef OBJ_CPZ2OUTRO_H
#define OBJ_CPZ2OUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCPZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCPZ2Outro;

// Object Struct
extern ObjectCPZ2Outro *CPZ2Outro;

// Standard Entity Events
void CPZ2Outro_Update(void);
void CPZ2Outro_LateUpdate(void);
void CPZ2Outro_StaticUpdate(void);
void CPZ2Outro_Draw(void);
void CPZ2Outro_Create(void* data);
void CPZ2Outro_StageLoad(void);
void CPZ2Outro_EditorDraw(void);
void CPZ2Outro_EditorLoad(void);
void CPZ2Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CPZ2OUTRO_H
