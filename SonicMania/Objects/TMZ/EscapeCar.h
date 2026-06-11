#ifndef OBJ_ESCAPECAR_H
#define OBJ_ESCAPECAR_H

#include "Game.h"

typedef enum {
    ESCAPECAR_DRIVER_SONIC = 0,
    ESCAPECAR_DRIVER_KNUX  = 3,
} EscapeCarDrivers;

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
    int32 driver;
    int32 unused1;
    int32 unused2;
    Animator carAnimator;
    Animator driverAnimator;
    Animator thrustAnimator;
} EntityEscapeCar;

// Object Struct
extern ObjectEscapeCar *EscapeCar;

// Standard Entity Events
void EscapeCar_Update(void);
void EscapeCar_LateUpdate(void);
void EscapeCar_StaticUpdate(void);
void EscapeCar_Draw(void);
void EscapeCar_Create(void *data);
void EscapeCar_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EscapeCar_EditorDraw(void);
void EscapeCar_EditorLoad(void);
#endif
void EscapeCar_Serialize(void);

// Extra Entity Functions
void EscapeCar_StateMania_AwaitPlayer(void);
void EscapeCar_StateMania_EnteredCar(void);
void EscapeCar_StateMania_Ride(void);

#if MANIA_USE_PLUS
void EscapeCar_StateEncore_Setup(void);
void EscapeCar_StateEncore_BadEnd(void);
void EscapeCar_StateEncore_GoodEnd(void);
#endif

#endif //! OBJ_ESCAPECAR_H
