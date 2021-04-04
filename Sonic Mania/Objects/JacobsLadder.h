#ifndef OBJ_JACOBSLADDER_H
#define OBJ_JACOBSLADDER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectJacobsLadder;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityJacobsLadder;

// Object Struct
extern ObjectJacobsLadder *JacobsLadder;

// Standard Entity Events
void JacobsLadder_Update(void);
void JacobsLadder_LateUpdate(void);
void JacobsLadder_StaticUpdate(void);
void JacobsLadder_Draw(void);
void JacobsLadder_Create(void* data);
void JacobsLadder_StageLoad(void);
void JacobsLadder_EditorDraw(void);
void JacobsLadder_EditorLoad(void);
void JacobsLadder_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_JACOBSLADDER_H
