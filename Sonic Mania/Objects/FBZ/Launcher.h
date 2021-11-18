#ifndef OBJ_LAUNCHER_H
#define OBJ_LAUNCHER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxLaunch;
} ObjectLauncher;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    int32 releaseDelay;
    int32 speedIncDelay;
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
#if RETRO_INCLUDE_EDITOR
void Launcher_EditorDraw(void);
void Launcher_EditorLoad(void);
#endif
void Launcher_Serialize(void);

// Extra Entity Functions
void Launcher_StateCollide(void);
void Launcher_State_None(void);
void Launcher_State_HandleLaunch(void);
void Launcher_State_ReturnToStart(void);

#endif //!OBJ_LAUNCHER_H
