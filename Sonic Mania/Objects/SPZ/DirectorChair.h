#ifndef OBJ_DIRECTORCHAIR_H
#define OBJ_DIRECTORCHAIR_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDirectorChair;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDirectorChair;

// Object Struct
extern ObjectDirectorChair *DirectorChair;

// Standard Entity Events
void DirectorChair_Update(void);
void DirectorChair_LateUpdate(void);
void DirectorChair_StaticUpdate(void);
void DirectorChair_Draw(void);
void DirectorChair_Create(void* data);
void DirectorChair_StageLoad(void);
void DirectorChair_EditorDraw(void);
void DirectorChair_EditorLoad(void);
void DirectorChair_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DIRECTORCHAIR_H
