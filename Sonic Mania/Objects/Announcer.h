#ifndef OBJ_ANNOUNCER_H
#define OBJ_ANNOUNCER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectAnnouncer;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityAnnouncer;

// Object Struct
extern ObjectAnnouncer *Announcer;

// Standard Entity Events
void Announcer_Update();
void Announcer_LateUpdate();
void Announcer_StaticUpdate();
void Announcer_Draw();
void Announcer_Create(void* data);
void Announcer_StageLoad();
void Announcer_EditorDraw();
void Announcer_EditorLoad();
void Announcer_Serialize();

// Extra Entity Functions


#endif //!OBJ_ANNOUNCER_H
