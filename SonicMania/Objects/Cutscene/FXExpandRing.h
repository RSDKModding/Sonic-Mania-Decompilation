#ifndef OBJ_FXEXPANDRING_H
#define OBJ_FXEXPANDRING_H

#include "Game.h"

// Object Class
struct ObjectFXExpandRing {
    RSDK_OBJECT
};

// Entity Class
struct EntityFXExpandRing {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 expandSpeed;
    int32 color;
    int32 expandMax;
    int32 innerRadius;
    int32 outerRadius;
    Entity *parent;
    Vector2 posOffset;
};

// Object Struct
extern ObjectFXExpandRing *FXExpandRing;

// Standard Entity Events
void FXExpandRing_Update(void);
void FXExpandRing_LateUpdate(void);
void FXExpandRing_StaticUpdate(void);
void FXExpandRing_Draw(void);
void FXExpandRing_Create(void *data);
void FXExpandRing_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FXExpandRing_EditorDraw(void);
void FXExpandRing_EditorLoad(void);
#endif
void FXExpandRing_Serialize(void);

// Extra Entity Functions
void FXExpandRing_State_FadeIn(void);
void FXExpandRing_State_Expand(void);
void FXExpandRing_State_FadeOut(void);

#endif //! OBJ_FXEXPANDRING_H
