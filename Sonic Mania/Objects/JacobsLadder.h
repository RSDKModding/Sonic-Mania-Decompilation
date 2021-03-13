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
void JacobsLadder_Update();
void JacobsLadder_LateUpdate();
void JacobsLadder_StaticUpdate();
void JacobsLadder_Draw();
void JacobsLadder_Create(void* data);
void JacobsLadder_StageLoad();
void JacobsLadder_EditorDraw();
void JacobsLadder_EditorLoad();
void JacobsLadder_Serialize();

// Extra Entity Functions


#endif //!OBJ_JACOBSLADDER_H
