#ifndef OBJ_PSZLAUNCHER_H
#define OBJ_PSZLAUNCHER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int value1[33]; //= { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 15, 15, 16, 16, 0 };
    colour value2;
    ushort value3;
} ObjectPSZLauncher;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZLauncher;

// Object Struct
extern ObjectPSZLauncher *PSZLauncher;

// Standard Entity Events
void PSZLauncher_Update(void);
void PSZLauncher_LateUpdate(void);
void PSZLauncher_StaticUpdate(void);
void PSZLauncher_Draw(void);
void PSZLauncher_Create(void* data);
void PSZLauncher_StageLoad(void);
void PSZLauncher_EditorDraw(void);
void PSZLauncher_EditorLoad(void);
void PSZLauncher_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PSZLAUNCHER_H
