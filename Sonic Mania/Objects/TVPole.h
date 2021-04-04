#ifndef OBJ_TVPOLE_H
#define OBJ_TVPOLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTVPole;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTVPole;

// Object Struct
extern ObjectTVPole *TVPole;

// Standard Entity Events
void TVPole_Update(void);
void TVPole_LateUpdate(void);
void TVPole_StaticUpdate(void);
void TVPole_Draw(void);
void TVPole_Create(void* data);
void TVPole_StageLoad(void);
void TVPole_EditorDraw(void);
void TVPole_EditorLoad(void);
void TVPole_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TVPOLE_H
