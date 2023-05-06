#ifndef OBJ_MATHHELPERS_H
#define OBJ_MATHHELPERS_H

#include "Game.h"

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
void MathHelpers_Create(void *data);
void MathHelpers_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MathHelpers_EditorDraw(void);
void MathHelpers_EditorLoad(void);
#endif
void MathHelpers_Serialize(void);

// Extra Entity Functions

// Lerp
void MathHelpers_LerpToPos(Vector2 *pos, int32 percent, int32 posX, int32 posY);
void MathHelpers_Lerp(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_LerpSin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_Lerp2Sin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);
void MathHelpers_LerpSin512(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY);

// Bezier/Sqrt
Vector2 MathHelpers_GetBezierPoint(int32 percent, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4);
int32 MathHelpers_SquareRoot(uint32 num);
int32 MathHelpers_Distance(int32 x1, int32 y1, int32 x2, int32 y2);
int32 MathHelpers_GetBezierCurveLength(int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4);

// "Collisions"
bool32 MathHelpers_PointInHitbox(int32 thisX, int32 thisY, int32 otherX, int32 otherY, int32 direction, Hitbox *hitbox);
bool32 MathHelpers_PositionBoxesIntersect(int32 otherX1, int32 otherY1, int32 otherX2, int32 otherY2, int32 thisX1, int32 thisY1, int32 thisX2,
                                          int32 thisY2);
int32 MathHelpers_GetInteractionDir(int32 otherX1, int32 otherY1, int32 otherX2, int32 otherY2, int32 thisX, int32 thisY);
bool32 MathHelpers_CheckValidIntersect(int32 otherX1, int32 otherY1, int32 otherX2, int32 otherY2, int32 thisX, int32 thisY);
int32 MathHelpers_CheckPositionOverlap(int32 otherX1, int32 otherY1, int32 otherX2, int32 otherY2, int32 thisX1, int32 thisY1, int32 thisX2,
                                       int32 thisY2);

int32 MathHelpers_GetEdgeDistance(int32 distance, int32 radius);
bool32 MathHelpers_ConstrainToBox(Vector2 *pos, int32 x, int32 y, Vector2 boxPos, Hitbox hitbox);

// RSDKv5U changed how the setPos param works, so this is added for compatibility
uint8 MathHelpers_CheckBoxCollision(void *thisEntity, Hitbox *thisHitbox, void *otherEntity, Hitbox *otherHitbox);

#endif //! OBJ_MATHHELPERS_H
