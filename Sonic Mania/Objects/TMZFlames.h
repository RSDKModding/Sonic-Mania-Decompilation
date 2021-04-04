#ifndef OBJ_TMZFLAMES_H
#define OBJ_TMZFLAMES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTMZFlames;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTMZFlames;

// Object Struct
extern ObjectTMZFlames *TMZFlames;

// Standard Entity Events
void TMZFlames_Update(void);
void TMZFlames_LateUpdate(void);
void TMZFlames_StaticUpdate(void);
void TMZFlames_Draw(void);
void TMZFlames_Create(void* data);
void TMZFlames_StageLoad(void);
void TMZFlames_EditorDraw(void);
void TMZFlames_EditorLoad(void);
void TMZFlames_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TMZFLAMES_H
