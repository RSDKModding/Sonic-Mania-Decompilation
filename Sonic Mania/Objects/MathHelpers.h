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
void MathHelpers_Update(void);
void MathHelpers_LateUpdate(void);
void MathHelpers_StaticUpdate(void);
void MathHelpers_Draw(void);
void MathHelpers_Create(void* data);
void MathHelpers_StageLoad(void);
void MathHelpers_EditorDraw(void);
void MathHelpers_EditorLoad(void);
void MathHelpers_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_MATHHELPERS_H
