#ifndef OBJ_POPCORNMACHINE_H
#define OBJ_POPCORNMACHINE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPopcornMachine;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPopcornMachine;

// Object Struct
extern ObjectPopcornMachine *PopcornMachine;

// Standard Entity Events
void PopcornMachine_Update(void);
void PopcornMachine_LateUpdate(void);
void PopcornMachine_StaticUpdate(void);
void PopcornMachine_Draw(void);
void PopcornMachine_Create(void* data);
void PopcornMachine_StageLoad(void);
void PopcornMachine_EditorDraw(void);
void PopcornMachine_EditorLoad(void);
void PopcornMachine_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_POPCORNMACHINE_H
