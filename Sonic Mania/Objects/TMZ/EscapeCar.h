#ifndef OBJ_ESCAPECAR_H
#define OBJ_ESCAPECAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxBumper;
    uint16 sfxDrop;
} ObjectEscapeCar;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 type;
    int32 field_64;
    int32 field_68;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityEscapeCar;

// Object Struct
extern ObjectEscapeCar *EscapeCar;

// Standard Entity Events
void EscapeCar_Update(void);
void EscapeCar_LateUpdate(void);
void EscapeCar_StaticUpdate(void);
void EscapeCar_Draw(void);
void EscapeCar_Create(void* data);
void EscapeCar_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void EscapeCar_EditorDraw(void);
void EscapeCar_EditorLoad(void);
#endif
void EscapeCar_Serialize(void);

// Extra Entity Functions
void EscapeCar_StateMania_Unknown1(void);
void EscapeCar_StateMania_CheckPlayerCollisions(void);
void EscapeCar_StateMania_Ride(void);

#if RETRO_USE_PLUS
void EscapeCar_StateEncore_Unknown1(void);
void EscapeCar_StateEncore_BadEnd(void);
void EscapeCar_StateEncore_GoodEnd(void);
#endif

#endif //!OBJ_ESCAPECAR_H
