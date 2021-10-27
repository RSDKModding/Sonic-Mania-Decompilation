#ifndef OBJ_FXWAVERING_H
#define OBJ_FXWAVERING_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFXWaveRing;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 angleOffset;
    int32 angle2Offset;
    int32 angle2;
    int32 field_6C;
    int32 field_70;
    char r;
    char g;
    char b;
    int32 field_78;
    Entity *parent;
    Vector2 field_80;
    int32 field_88;
    int32 field_8C;
} EntityFXWaveRing;

// Object Struct
extern ObjectFXWaveRing *FXWaveRing;

// Standard Entity Events
void FXWaveRing_Update(void);
void FXWaveRing_LateUpdate(void);
void FXWaveRing_StaticUpdate(void);
void FXWaveRing_Draw(void);
void FXWaveRing_Create(void* data);
void FXWaveRing_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FXWaveRing_EditorDraw(void);
void FXWaveRing_EditorLoad(void);
#endif
void FXWaveRing_Serialize(void);

// Extra Entity Functions
void FXWaveRing_State_FadeIn(void);
void FXWaveRing_State_Wait(void);
void FXWaveRing_State_FadeOut(void);
#endif

#endif //!OBJ_FXWAVERING_H
