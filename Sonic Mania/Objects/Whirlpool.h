#ifndef OBJ_WHIRLPOOL_H
#define OBJ_WHIRLPOOL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectWhirlpool;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityWhirlpool;

// Object Struct
extern ObjectWhirlpool *Whirlpool;

// Standard Entity Events
void Whirlpool_Update(void);
void Whirlpool_LateUpdate(void);
void Whirlpool_StaticUpdate(void);
void Whirlpool_Draw(void);
void Whirlpool_Create(void* data);
void Whirlpool_StageLoad(void);
void Whirlpool_EditorDraw(void);
void Whirlpool_EditorLoad(void);
void Whirlpool_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_WHIRLPOOL_H
