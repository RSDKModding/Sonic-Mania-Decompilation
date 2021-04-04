#ifndef OBJ_HANDLAUNCHER_H
#define OBJ_HANDLAUNCHER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHandLauncher;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHandLauncher;

// Object Struct
extern ObjectHandLauncher *HandLauncher;

// Standard Entity Events
void HandLauncher_Update(void);
void HandLauncher_LateUpdate(void);
void HandLauncher_StaticUpdate(void);
void HandLauncher_Draw(void);
void HandLauncher_Create(void* data);
void HandLauncher_StageLoad(void);
void HandLauncher_EditorDraw(void);
void HandLauncher_EditorLoad(void);
void HandLauncher_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HANDLAUNCHER_H
