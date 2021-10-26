#ifndef OBJ_FXSPINRAY_H
#define OBJ_FXSPINRAY_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFXSpinRay;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 offset;
    int32 r;
    int32 g;
    int32 b;
    int32 field_70;
    Entity *parent;
    Vector2 field_78;
    Vector2 field_80[4];
    Vector2 vertices[20];
    Vector2 field_140;
} EntityFXSpinRay;

// Object Struct
extern ObjectFXSpinRay *FXSpinRay;

// Standard Entity Events
void FXSpinRay_Update(void);
void FXSpinRay_LateUpdate(void);
void FXSpinRay_StaticUpdate(void);
void FXSpinRay_Draw(void);
void FXSpinRay_Create(void* data);
void FXSpinRay_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FXSpinRay_EditorDraw(void);
void FXSpinRay_EditorLoad(void);
#endif
void FXSpinRay_Serialize(void);

// Extra Entity Functions
void FXSpinRay_State_FadeIn(void);
void FXSpinRay_State_Wait(void);
void FXSpinRay_State_FadeOut(void);

#endif //!OBJ_FXSPINRAY_H
