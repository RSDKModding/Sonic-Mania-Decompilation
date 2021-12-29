#ifndef OBJ_MATHHELPERS_H
#define OBJ_MATHHELPERS_H

#include "SonicMania.h"

// Object Class
struct ObjectMathHelpers {
	RSDK_OBJECT
};

// Entity Class
struct EntityMathHelpers {
	RSDK_ENTITY
};

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
void MathHelpers_LerpToPos(Vector2 *pos, int32 percent, int32 posX, int32 posY);
void MathHelpers_Lerp(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_LerpSin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_Lerp2Sin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_LerpSin512(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
Vector2 MathHelpers_GetBezierPoint(int32 percent, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4);
int32 MathHelpers_SquareRoot(uint32 num);
int32 MathHelpers_GetBezierCurveLength(int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4);
bool32 MathHelpers_PointInHitbox(int32 direction, int32 thisX, int32 thisY, Hitbox *hitbox, int32 otherX, int32 otherY); // USERCALL -> thisX, thisY, otherX, otherY, direction, hitbox
bool32 MathHelpers_Unknown9(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX1, int32 extendY1, int32 extendX2, int32 extendY2);
int32 MathHelpers_GetValueSign(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX, int32 extendY);
bool32 MathHelpers_Unknown11(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX, int32 extendY);
int32 MathHelpers_Unknown12(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX1, int32 extendY1, int32 extendX2, int32 extendY2);
int32 MathHelpers_Unknown13(int32 distance, int32 radius);
bool32 MathHelpers_ConstrainToBox(Vector2 *resultPos, int32 x, int32 y, Vector2 boxPos, Hitbox hitbox);

#endif //!OBJ_MATHHELPERS_H
