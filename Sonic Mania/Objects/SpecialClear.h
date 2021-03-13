#ifndef OBJ_SPECIALCLEAR_H
#define OBJ_SPECIALCLEAR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpecialClear;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpecialClear;

// Object Struct
extern ObjectSpecialClear *SpecialClear;

// Standard Entity Events
void SpecialClear_Update();
void SpecialClear_LateUpdate();
void SpecialClear_StaticUpdate();
void SpecialClear_Draw();
void SpecialClear_Create(void* data);
void SpecialClear_StageLoad();
void SpecialClear_EditorDraw();
void SpecialClear_EditorLoad();
void SpecialClear_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPECIALCLEAR_H
