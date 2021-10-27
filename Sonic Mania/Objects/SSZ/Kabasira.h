#ifndef OBJ_KABASIRA_H
#define OBJ_KABASIRA_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectKabasira;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityKabasira;

// Object Struct
extern ObjectKabasira *Kabasira;

// Standard Entity Events
void Kabasira_Update(void);
void Kabasira_LateUpdate(void);
void Kabasira_StaticUpdate(void);
void Kabasira_Draw(void);
void Kabasira_Create(void* data);
void Kabasira_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Kabasira_EditorDraw(void);
void Kabasira_EditorLoad(void);
#endif
void Kabasira_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_KABASIRA_H
