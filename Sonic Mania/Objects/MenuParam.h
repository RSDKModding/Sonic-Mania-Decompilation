#ifndef OBJ_MENUPARAM_H
#define OBJ_MENUPARAM_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMenuParam;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMenuParam;

// Object Struct
extern ObjectMenuParam *MenuParam;

// Standard Entity Events
void MenuParam_Update(void);
void MenuParam_LateUpdate(void);
void MenuParam_StaticUpdate(void);
void MenuParam_Draw(void);
void MenuParam_Create(void* data);
void MenuParam_StageLoad(void);
void MenuParam_EditorDraw(void);
void MenuParam_EditorLoad(void);
void MenuParam_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MENUPARAM_H
