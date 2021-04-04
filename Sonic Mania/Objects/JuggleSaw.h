#ifndef OBJ_JUGGLESAW_H
#define OBJ_JUGGLESAW_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectJuggleSaw;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityJuggleSaw;

// Object Struct
extern ObjectJuggleSaw *JuggleSaw;

// Standard Entity Events
void JuggleSaw_Update(void);
void JuggleSaw_LateUpdate(void);
void JuggleSaw_StaticUpdate(void);
void JuggleSaw_Draw(void);
void JuggleSaw_Create(void* data);
void JuggleSaw_StageLoad(void);
void JuggleSaw_EditorDraw(void);
void JuggleSaw_EditorLoad(void);
void JuggleSaw_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JUGGLESAW_H
