// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MathHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMathHelpers *MathHelpers = NULL;

void MathHelpers_Update(void) {}

void MathHelpers_LateUpdate(void) {}

void MathHelpers_StaticUpdate(void) {}

void MathHelpers_Draw(void) {}

void MathHelpers_Create(void *data) {}

void MathHelpers_StageLoad(void) {}

void MathHelpers_LerpToPos(Vector2 *pos, int32 percent, int32 posX, int32 posY)
{
    if (percent < 0) {
        pos->x = 0;
        pos->y = 0;
    }
    else if (percent >= 0x100) {
        pos->x = posX;
        pos->y = posY;
    }
    else {
        pos->x = percent * (posX >> 8);
        pos->y = percent * (posY >> 8);
    }
}

void MathHelpers_Lerp(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY)
{
    if (percent < 0) {
        pos->x = startX;
        pos->y = startY;
    }
    else if (percent >= 0x100) {
        pos->x = endX;
        pos->y = endY;
    }
    else {
        pos->x = startX + percent * ((endX - startX) >> 8);
        pos->y = startY + percent * ((endY - startY) >> 8);
    }
}

void MathHelpers_LerpSin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY)
{
    if (percent < 0) {
        pos->x = startX;
        pos->y = startY;
    }
    else if (percent >= 0x100) {
        pos->x = endX;
        pos->y = endY;
    }
    else {
        int32 val = (RSDK.Sin1024(percent + 0x300) >> 2) + 0x200;
        pos->x    = startX + val * ((endX - startX) >> 8);
        pos->y    = startY + val * ((endY - startY) >> 8);
    }
}

void MathHelpers_Lerp2Sin1024(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY)
{
    if (percent < 0) {
        pos->x = startX;
        pos->y = startY;
    }
    else if (percent >= 0x100) {
        pos->x = endX;
        pos->y = endY;
    }
    else {
        int32 val = RSDK.Sin1024(percent) >> 2;
        pos->x = startX + val * ((endX - startX) >> 8);
        pos->y = startY + val * ((endY - startY) >> 8);
    }
}

void MathHelpers_LerpSin512(Vector2 *pos, int32 percent, int32 startX, int32 startY, int32 endX, int32 endY)
{
    if (percent < 0) {
        pos->x = startX;
        pos->y = startY;
    }
    else if (percent >= 0x100) {
        pos->x = endX;
        pos->y = endY;
    }
    else {
        int32 val = (RSDK.Sin512(percent + 0x180) >> 2) + 0x80;
        pos->x    = startX + val * ((endX - startX) >> 8);
        pos->y    = startY + val * ((endY - startY) >> 8);
    }
}

Vector2 MathHelpers_GetBezierPoint(int32 percent, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4)
{
    int32 invPercent = 0x10000 - percent;
    int32 p1         = invPercent * ((uint32)(invPercent * invPercent) >> 16) >> 16;
    int32 p2         = percent * ((uint32)(invPercent * invPercent) >> 16) >> 16;
    int32 p3         = invPercent * ((uint32)(percent * percent) >> 16) >> 16;
    int32 p4         = percent * ((uint32)(percent * percent) >> 16) >> 16;

    Vector2 result;
    result.x = p4 * (x4 >> 16) + p3 * (x3 >> 16) + p2 * (x2 >> 16) + p1 * (x1 >> 16) + 2 * p2 * (x2 >> 16) + 2 * p3 * (x3 >> 16);
    result.y = p4 * (y4 >> 16) + p3 * (y3 >> 16) + p2 * (y2 >> 16) + p1 * (y1 >> 16) + 2 * p2 * (y2 >> 16) + 2 * p3 * (y3 >> 16);
    return result;
}

int32 MathHelpers_SquareRoot(uint32 num)
{
    int32 val = 0x40000000;
    int32 id  = 0;
    while (val > num) {
        val >>= 2;
    }

    if (val) {
        do {
            if (num >= val + id) {
                num -= val + id;
                id += 2 * val;
            }
            val >>= 2;
            id >>= 1;
        } while (val);
    }

    if (num <= id)
        return id;
    else
        return id + 1;
}

int32 MathHelpers_GetBezierCurveLength(int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4)
{
    int32 lastX = x1;
    int32 lastY = y1;

    int32 length = 0;
    // 0x10000 = 1.0
    // 0xCCC == 0.05
    for (int32 percent = 0xCCC; percent <= 0x10000; percent += 0xCCC) {
        Vector2 point = MathHelpers_GetBezierPoint(percent, x1, y1, x2, y2, x3, y3, x4, y4);

        int32 distX = abs(point.x - lastX);
        int32 distY = abs(point.y - lastY);
        length += MathHelpers_SquareRoot((distX >> 16) * (distX >> 16) + (distY >> 16) * (distY >> 16)) << 16;
        lastX = point.x;
        lastY = point.y;
    }
    return length;
}

bool32 MathHelpers_PointInHitbox(int32 direction, int32 thisX, int32 thisY, Hitbox *hitbox, int32 otherX, int32 otherY)
{
    int32 left, top, right, bottom;

    if ((direction & FLIP_X)) {
        left  = -hitbox->left;
        right = -hitbox->right;
    }
    else {
        right = hitbox->right;
        left  = hitbox->left;
    }

    if ((direction & FLIP_Y)) {
        bottom = -hitbox->bottom;
        top    = -hitbox->top;
    }
    else {
        bottom = hitbox->bottom;
        top    = hitbox->top;
    }
    int32 hitboxX2 = right;
    if (left < right)
        hitboxX2 = left;
    int32 hitboxX1 = left;
    if (right > left)
        hitboxX1 = right;

    int32 hitboxY1 = top;
    int32 hitboxY2 = bottom;
    if (top < bottom)
        hitboxY2 = top;
    if (bottom > top)
        hitboxY1 = bottom;
    return otherX >= thisX + (hitboxX2 << 16) && otherX <= thisX + (hitboxX1 << 16) && otherY >= thisY + (hitboxY2 << 16) && otherY <= thisY + (hitboxY1 << 16);
}

bool32 MathHelpers_Unknown9(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX1, int32 extendY1, int32 extendX2, int32 extendY2)
{
    int32 left   = x < prevX ? x : prevX;
    int32 top    = y < prevY ? y : prevY;
    int32 right  = x > prevX ? x : prevX;
    int32 bottom = y > prevY ? y : prevY;

    int32 left2   = extendX1 < extendX2 ? extendX1 : extendX2;
    int32 top2    = extendY1 < extendY2 ? extendY1 : extendY2;
    int32 right2  = extendX1 > extendX2 ? extendX1 : extendX2;
    int32 bottom2 = extendY1 > extendY2 ? extendY1 : extendY2;

    return left <= right2 && right >= left2 && top <= bottom2 && bottom >= top2;
}
int32 MathHelpers_GetValueSign(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX, int32 extendY)
{
    int32 value = ((extendY - y) >> 16) * ((prevX - x) >> 16) - ((extendX - x) >> 16) * ((prevY - y) >> 16);
    return value > 0 ? 1 : value < 0 ? -1 : 0;
}
bool32 MathHelpers_Unknown11(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX, int32 extendY)
{
    if (prevX <= x) {
        if (prevX >= x) {
            if (prevY <= y) {
                if (prevY >= y) {
                    if (x > extendX)
                        return false;
                    return y <= extendY;
                }
                else {
                    if (prevY > extendY)
                        return false;
                    return extendY <= y;
                }
            }
            else {
                if (y > extendY)
                    return false;
                return extendY <= prevY;
            }
        }
        else {
            if (prevX > extendX)
                return false;
            return extendX <= x;
        }
    }
    else {
        if (x > extendX)
            return false;
        return extendX <= prevX;
    }
}
int32 MathHelpers_Unknown12(int32 x, int32 y, int32 prevX, int32 prevY, int32 extendX1, int32 extendY1, int32 extendX2, int32 extendY2)
{
    if (!MathHelpers_Unknown9(x, y, prevX, prevY, extendX1, extendY1, extendX2, extendY2))
        return false;

    if (x == prevX && y == prevY) {
        if (x != extendX1 || y != extendY1) {
            if (x == extendX2 && y == extendY2)
                return true;
            return false;
        }
        return true;
    }

    if (extendX1 != extendX2 || extendY1 != extendY2) {
        int32 valA = MathHelpers_GetValueSign(x, y, prevX, prevY, extendX1, extendY1);
        int32 valB = MathHelpers_GetValueSign(x, y, prevX, prevY, extendX2, extendY2);
        if (valA) {
            if (valA == valB)
                return 0;
        }
        else if (!valB) {
            if (MathHelpers_Unknown11(x, y, prevX, prevY, extendX1, extendY1) || MathHelpers_Unknown11(x, y, prevX, prevY, extendX2, extendY2)
                || MathHelpers_Unknown11(extendX1, extendY1, extendX2, extendY2, x, y)) {
                return true;
            }
            if (!MathHelpers_Unknown11(extendX1, extendY1, extendX2, extendY2, prevX, prevY))
                return false;
        }

        int32 res = MathHelpers_GetValueSign(extendX1, extendY1, extendX2, extendY2, x, y);
        if (!res)
            return true;
        if (res == MathHelpers_GetValueSign(extendX1, extendY1, extendX2, extendY2, prevX, prevY))
            return false;
        return true;
    }
    if (extendX1 == x && extendY1 == y)
        return true;
    if (extendX1 != prevX || extendY1 != prevY)
        return false;
    return true;
}
int32 MathHelpers_Unknown13(int32 distance, int32 radius)
{
    uint32 v1 = abs(distance);
    uint32 v2 = abs(radius);

    uint32 r1 = (v1 >> 16) * (v2 >> 16) << 16;
    uint32 r2 = (v1 >> 16) * (v2 & 0xFFFF);
    uint32 r3 = (v1 & 0xFFFF) * (v2 >> 16);
    uint32 r4 = (v1 & 0xFFFF) * (v2 & 0xFFFF) >> 16;

    uint32 val = r1 + r2 + r3 + r4;
    if ((radius ^ ~distance) >= 0) //if the signs do not match
        return -(int32)val;
    else
        return val;
}

bool32 MathHelpers_ConstrainToBox(Vector2 *resultPos, int32 x, int32 y, Vector2 boxPos, Hitbox hitbox)
{
    int32 left   = minVal(hitbox.left, hitbox.right);
    int32 right  = maxVal(hitbox.right, hitbox.left);
    int32 top    = minVal(hitbox.top, hitbox.bottom);
    int32 bottom = maxVal(hitbox.bottom, hitbox.top);

    int32 posLeft2   = boxPos.x + (left << 16);
    int32 posTop2    = boxPos.y + (top << 16);
    int32 posRight2  = boxPos.x + (right << 16);
    int32 posBottom2 = boxPos.y + (bottom << 16);

    if (x > posLeft2 && x < posRight2 && y > posTop2 && y < posBottom2)
        return false;

    int32 posY = 0;
    if ((x ^ boxPos.x) & 0xFFFF0000) {
        if (!((y ^ boxPos.y) & 0xFFFF0000)) {
            if (resultPos) {
                if (x <= boxPos.x)
                    resultPos->x = posLeft2;
                else
                    resultPos->x = posRight2;
                resultPos->y = y & 0xFFFF0000;
            }
        }
        else {
            double div = 1.0f / 65536.0f;

            int32 radius = (((boxPos.y - y) * div) / ((boxPos.x - x) * div)) * 65536.0f;
            if (!radius)
                return false;

            if (x <= boxPos.x) {
                posY = y + MathHelpers_Unknown13(posLeft2 - x, radius);
                if (posTop2 <= posY && posY <= posBottom2) {
                    if (resultPos) {
                        resultPos->x = posLeft2;
                        resultPos->y = posY;
                    }
                    return true;
                }
            }

            if (x >= boxPos.x) {
                posY = y + MathHelpers_Unknown13(posRight2 - x, radius);
                if (posTop2 <= posY && posY <= posBottom2) {
                    if (resultPos) {
                        resultPos->x = posRight2;
                        resultPos->y = posY;
                    }
                    return true;
                }
            }

            if (y <= boxPos.y) {
                radius = (((posTop2 - y) * div) / (radius * div)) * -65536.0;
                if (posLeft2 <= x - radius && x - radius <= posRight2) {
                    if (resultPos) {
                        resultPos->x = x - radius;
                        resultPos->y = posTop2;
                    }
                    return true;
                }
            }

            if (y >= boxPos.y) {
                radius = x - ((((posBottom2 - y) * div) / (radius * div)) * -65536.0f);
                if (posLeft2 <= radius && radius <= posRight2) {
                    if (resultPos) {
                        resultPos->x = radius;
                        resultPos->y = posBottom2;
                    }
                }
            }
            else {
                return false;
            }
        }
    }
    else {
        if (resultPos) {
            resultPos->x = x & 0xFFFF0000;
            if (y <= boxPos.y)
                resultPos->y = posTop2;
            else
                resultPos->y = posBottom2;
        }
    }
    return true;
}

#if RETRO_INCLUDE_EDITOR
void MathHelpers_EditorDraw(void) {}

void MathHelpers_EditorLoad(void) {}
#endif

void MathHelpers_Serialize(void) {}
