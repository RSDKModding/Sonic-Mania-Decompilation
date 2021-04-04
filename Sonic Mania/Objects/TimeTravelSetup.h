#ifndef OBJ_TIMETRAVELSETUP_H
#define OBJ_TIMETRAVELSETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTimeTravelSetup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTimeTravelSetup;

// Object Struct
extern ObjectTimeTravelSetup *TimeTravelSetup;

// Standard Entity Events
void TimeTravelSetup_Update(void);
void TimeTravelSetup_LateUpdate(void);
void TimeTravelSetup_StaticUpdate(void);
void TimeTravelSetup_Draw(void);
void TimeTravelSetup_Create(void* data);
void TimeTravelSetup_StageLoad(void);
void TimeTravelSetup_EditorDraw(void);
void TimeTravelSetup_EditorLoad(void);
void TimeTravelSetup_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TIMETRAVELSETUP_H
