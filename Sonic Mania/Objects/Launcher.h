#ifndef OBJ_LAUNCHER_H
#define OBJ_LAUNCHER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLauncher;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLauncher;

// Object Struct
extern ObjectLauncher *Launcher;

// Standard Entity Events
void Launcher_Update(void);
void Launcher_LateUpdate(void);
void Launcher_StaticUpdate(void);
void Launcher_Draw(void);
void Launcher_Create(void* data);
void Launcher_StageLoad(void);
void Launcher_EditorDraw(void);
void Launcher_EditorLoad(void);
void Launcher_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LAUNCHER_H
