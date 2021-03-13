#ifndef OBJ_MATHHELPERS_H
#define OBJ_MATHHELPERS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectMathHelpers;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityMathHelpers;

// Object Entity
extern ObjectMathHelpers *MathHelpers;

// Standard Entity Events
void MathHelpers_Update();
void MathHelpers_LateUpdate();
void MathHelpers_StaticUpdate();
void MathHelpers_Draw();
void MathHelpers_Create(void* data);
void MathHelpers_StageLoad();
void MathHelpers_EditorDraw();
void MathHelpers_EditorLoad();
void MathHelpers_Serialize();

// Extra Entity Functions


#endif //!OBJ_MATHHELPERS_H
