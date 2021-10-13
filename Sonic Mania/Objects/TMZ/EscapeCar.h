#ifndef OBJ_ESCAPECAR_H
#define OBJ_ESCAPECAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
    ushort sfxBumper;
    ushort sfxDrop;
} ObjectEscapeCar;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int type;
    int field_64;
    int field_68;
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
void EscapeCar_EditorDraw(void);
void EscapeCar_EditorLoad(void);
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
