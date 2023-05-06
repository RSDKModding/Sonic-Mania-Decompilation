#ifndef OBJ_FXSPINRAY_H
#define OBJ_FXSPINRAY_H

#include "Game.h"

// Object Class
struct ObjectFXSpinRay {
    RSDK_OBJECT
};

// Entity Class
struct EntityFXSpinRay {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 spinSpeed;
    int32 r;
    int32 g;
    int32 b;
    int32 size;
    Entity *parent;
    Vector2 offsetPos;
    Vector2 vertexPos[4];
    Vector2 vertices[20];
    Vector2 vertexOffset;
};

// Object Struct
extern ObjectFXSpinRay *FXSpinRay;

// Standard Entity Events
void FXSpinRay_Update(void);
void FXSpinRay_LateUpdate(void);
void FXSpinRay_StaticUpdate(void);
void FXSpinRay_Draw(void);
void FXSpinRay_Create(void *data);
void FXSpinRay_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FXSpinRay_EditorDraw(void);
void FXSpinRay_EditorLoad(void);
#endif
void FXSpinRay_Serialize(void);

// Extra Entity Functions
void FXSpinRay_State_FadeIn(void);
void FXSpinRay_State_Spinning(void);
void FXSpinRay_State_FadeOut(void);

#endif //! OBJ_FXSPINRAY_H
