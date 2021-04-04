#ifndef OBJ_SNOWFLAKES_H
#define OBJ_SNOWFLAKES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSnowflakes;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySnowflakes;

// Object Struct
extern ObjectSnowflakes *Snowflakes;

// Standard Entity Events
void Snowflakes_Update(void);
void Snowflakes_LateUpdate(void);
void Snowflakes_StaticUpdate(void);
void Snowflakes_Draw(void);
void Snowflakes_Create(void* data);
void Snowflakes_StageLoad(void);
void Snowflakes_EditorDraw(void);
void Snowflakes_EditorLoad(void);
void Snowflakes_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SNOWFLAKES_H
