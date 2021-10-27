#ifndef OBJ_SCHRODINGERSCAPSULE_H
#define OBJ_SCHRODINGERSCAPSULE_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
} ObjectSchrodingersCapsule;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntitySchrodingersCapsule;

// Object Struct
extern ObjectSchrodingersCapsule *SchrodingersCapsule;

// Standard Entity Events
void SchrodingersCapsule_Update(void);
void SchrodingersCapsule_LateUpdate(void);
void SchrodingersCapsule_StaticUpdate(void);
void SchrodingersCapsule_Draw(void);
void SchrodingersCapsule_Create(void* data);
void SchrodingersCapsule_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SchrodingersCapsule_EditorDraw(void);
void SchrodingersCapsule_EditorLoad(void);
#endif
void SchrodingersCapsule_Serialize(void);

// Extra Entity Functions
void SchrodingersCapsule_Unknown1(void);
void SchrodingersCapsule_Unknown2(void);
void SchrodingersCapsule_Unknown3(void);
void SchrodingersCapsule_Unknown4(void);
void SchrodingersCapsule_Unknown5(void);

#endif

#endif //!OBJ_SCHRODINGERSCAPSULE_H
