#ifndef OBJ_GHZ2OUTRO_H
#define OBJ_GHZ2OUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectGHZ2Outro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityGHZ2Outro;

// Object Struct
extern ObjectGHZ2Outro *GHZ2Outro;

// Standard Entity Events
void GHZ2Outro_Update(void);
void GHZ2Outro_LateUpdate(void);
void GHZ2Outro_StaticUpdate(void);
void GHZ2Outro_Draw(void);
void GHZ2Outro_Create(void* data);
void GHZ2Outro_StageLoad(void);
void GHZ2Outro_EditorDraw(void);
void GHZ2Outro_EditorLoad(void);
void GHZ2Outro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_GHZ2OUTRO_H
