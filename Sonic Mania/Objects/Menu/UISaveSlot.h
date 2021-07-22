#ifndef OBJ_UISAVESLOT_H
#define OBJ_UISAVESLOT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUISaveSlot;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityUISaveSlot;

// Object Struct
extern ObjectUISaveSlot *UISaveSlot;

// Standard Entity Events
void UISaveSlot_Update(void);
void UISaveSlot_LateUpdate(void);
void UISaveSlot_StaticUpdate(void);
void UISaveSlot_Draw(void);
void UISaveSlot_Create(void* data);
void UISaveSlot_StageLoad(void);
void UISaveSlot_EditorDraw(void);
void UISaveSlot_EditorLoad(void);
void UISaveSlot_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UISAVESLOT_H
