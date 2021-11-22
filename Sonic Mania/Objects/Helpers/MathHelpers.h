#ifndef OBJ_MATHHELPERS_H
#define OBJ_MATHHELPERS_H

#include "SonicMania.h"

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
#if RETRO_INCLUDE_EDITOR
void MathHelpers_EditorDraw(void);
void MathHelpers_EditorLoad(void);
#endif
void MathHelpers_Serialize(void);

// Extra Entity Functions
void MathHelpers_Lerp(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_LerpSin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_Lerp2Sin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_LerpSin512(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
Vector2 MathHelpers_Unknown5(int32 percent, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4);
int32 MathHelpers_SquareRoot(uint32 num);
int32 MathHelpers_Unknown7(int32 a1, int32 a2, int32 a3, int32 a4, int32 a5, int32 a6, int32 a7, int32 a8);
bool32 MathHelpers_PointInHitbox(int32 direction, int32 x1, int32 y1, Hitbox *hitbox, int32 x2, int32 y2);
bool32 MathHelpers_Unknown9(int32 px1, int32 py1, int32 px2, int32 py2, int32 tx1, int32 tx2, int32 ty1, int32 ty2);
int32 MathHelpers_GetValueSign(int32 px1, int32 py1, int32 px2, int32 py2, int32 tx1, int32 tx2);
bool32 MathHelpers_Unknown11(int32 tx1, int32 tx2, int32 ty1, int32 ty2, int32 px2, int32 py2);
int32 MathHelpers_Unknown12(int32 px1, int32 py1, int32 px2, int32 py2, int32 tx1, int32 tx2, int32 ty1, int32 ty2);
int32 MathHelpers_Unknown13(int32 a1, int32 a2);
bool32 MathHelpers_ConstrainToBox(Vector2 *resultPos, int32 x, int32 y, Vector2 boxPos, Hitbox hitbox);

#endif //!OBJ_MATHHELPERS_H
