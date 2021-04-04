#ifndef OBJ_SCHRODINGERSCAPSULE_H
#define OBJ_SCHRODINGERSCAPSULE_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSchrodingersCapsule;

// Entity Class
typedef struct {
	RSDK_ENTITY
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
void SchrodingersCapsule_EditorDraw(void);
void SchrodingersCapsule_EditorLoad(void);
void SchrodingersCapsule_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_SCHRODINGERSCAPSULE_H
