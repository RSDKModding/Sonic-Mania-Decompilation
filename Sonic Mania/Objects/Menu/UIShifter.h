#ifndef OBJ_UISHIFTER_H
#define OBJ_UISHIFTER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIShifter;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Vector2 shift;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    Vector2 startPos;
    EntityUIControl *parent;
} EntityUIShifter;

// Object Struct
extern ObjectUIShifter *UIShifter;

// Standard Entity Events
void UIShifter_Update(void);
void UIShifter_LateUpdate(void);
void UIShifter_StaticUpdate(void);
void UIShifter_Draw(void);
void UIShifter_Create(void* data);
void UIShifter_StageLoad(void);
void UIShifter_EditorDraw(void);
void UIShifter_EditorLoad(void);
void UIShifter_Serialize(void);

// Extra Entity Functions
void UIShifter_Unknown(void);
#endif

#endif //!OBJ_UISHIFTER_H
