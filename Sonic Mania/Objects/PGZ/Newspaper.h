#ifndef OBJ_NEWSPAPER_H
#define OBJ_NEWSPAPER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectNewspaper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityNewspaper;

// Object Struct
extern ObjectNewspaper *Newspaper;

// Standard Entity Events
void Newspaper_Update(void);
void Newspaper_LateUpdate(void);
void Newspaper_StaticUpdate(void);
void Newspaper_Draw(void);
void Newspaper_Create(void* data);
void Newspaper_StageLoad(void);
void Newspaper_EditorDraw(void);
void Newspaper_EditorLoad(void);
void Newspaper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_NEWSPAPER_H
