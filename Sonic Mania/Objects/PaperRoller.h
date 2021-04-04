#ifndef OBJ_PAPERROLLER_H
#define OBJ_PAPERROLLER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPaperRoller;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPaperRoller;

// Object Struct
extern ObjectPaperRoller *PaperRoller;

// Standard Entity Events
void PaperRoller_Update(void);
void PaperRoller_LateUpdate(void);
void PaperRoller_StaticUpdate(void);
void PaperRoller_Draw(void);
void PaperRoller_Create(void* data);
void PaperRoller_StageLoad(void);
void PaperRoller_EditorDraw(void);
void PaperRoller_EditorLoad(void);
void PaperRoller_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PAPERROLLER_H
