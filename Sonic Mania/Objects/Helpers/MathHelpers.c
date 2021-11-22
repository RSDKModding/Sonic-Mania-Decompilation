#include "SonicMania.h"

ObjectMathHelpers *MathHelpers = NULL;

void MathHelpers_Update(void) {}

void MathHelpers_LateUpdate(void) {}

void MathHelpers_StaticUpdate(void) {}

void MathHelpers_Draw(void) {}

void MathHelpers_Create(void *data) {}

void MathHelpers_StageLoad(void) {}

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

Vector2 MathHelpers_Unknown5(int32 percent, int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4)
{
    int32 invPercent = 0x10000 - percent;
    int32 v10        = invPercent * ((uint32)(invPercent * invPercent) >> 16) >> 16;
    int32 v11        = percent * ((uint32)(invPercent * invPercent) >> 16) >> 16;
    int32 v12        = percent * ((uint32)(percent * percent) >> 16) >> 16;
    int32 v13        = (0x10000 - percent) * ((uint32)(percent * percent) >> 16) >> 16;

    Vector2 result;
    result.x = v12 * (x4 >> 16) + v13 * (x3 >> 16) + v11 * (x2 >> 16) + v10 * (x1 >> 16) + 2 * v11 * (x2 >> 16) + 2 * v13 * (x3 >> 16);
    result.y = v12 * (y4 >> 16) + v13 * (y3 >> 16) + v11 * (y2 >> 16) + v10 * (y1 >> 16) + 2 * v11 * (y2 >> 16) + 2 * v13 * (y3 >> 16);
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

int32 MathHelpers_Unknown7(int32 x1, int32 y1, int32 x2, int32 y2, int32 x3, int32 y3, int32 x4, int32 y4)
{
    int32 startX1 = x1;
    int32 startY1 = y1;

    int32 val = 0;
    for (int32 percent = 0xCCC; percent <= 0x10000; percent += 0xCCC) {
        Vector2 res = MathHelpers_Unknown5(percent, x1, y1, x2, y2, x3, y3, x4, y4);
        int32 x     = abs(res.x - startX1);
        int32 y     = abs(res.y - startY1);
        val += MathHelpers_SquareRoot((x >> 16) * (x >> 16) + (y >> 16) * (y >> 16)) << 16;
        startX1 = res.x;
        startY1 = res.y;
    }
    return val;
}

bool32 MathHelpers_PointInHitbox(int32 direction, int32 x1, int32 y1, Hitbox *hitbox, int32 x2, int32 y2)
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
    return x2 >= x1 + (hitboxX2 << 16) && x2 <= x1 + (hitboxX1 << 16) && y2 >= y1 + (hitboxY2 << 16) && y2 <= y1 + (hitboxY1 << 16);
}

bool32 MathHelpers_Unknown9(int32 px1, int32 py1, int32 px2, int32 py2, int32 tx1, int32 tx2, int32 ty1, int32 ty2)
{
    int32 left   = px1 < px2 ? px1 : px2;
    int32 top    = py1 < py2 ? py1 : py2;
    int32 right  = px1 > px2 ? px1 : px2;
    int32 bottom = py1 > py2 ? py1 : py2;

    int32 v12 = tx2 > ty2 ? tx2 : ty2;
    int32 v13 = tx1 > ty1 ? tx1 : ty1;
    int32 v14 = tx2 < ty2 ? tx2 : ty2;
    int32 v15 = tx1 < ty1 ? tx1 : ty1;

    return left <= v13 && right >= v15 && top <= v12 && bottom >= v14;
}
int32 MathHelpers_GetValueSign(int32 px1, int32 py1, int32 px2, int32 py2, int32 tx1, int32 tx2)
{
    int32 value = ((tx2 - py1) >> 16) * ((px2 - px1) >> 16) - ((tx1 - px1) >> 16) * ((py2 - py1) >> 16);
    return value > 0 ? 1 : value < 0 ? -1 : 0;
}
bool32 MathHelpers_Unknown11(int32 tx1, int32 tx2, int32 ty1, int32 ty2, int32 px2, int32 py2)
{
    if (ty1 <= tx1) {
        if (ty1 >= tx1) {
            if (ty2 <= tx2) {
                if (ty2 >= tx2) {
                    if (tx1 > px2)
                        return false;
                    if (tx1 <= px2)
                        return true;
                }
                else {
                    if (ty2 > py2)
                        return false;
                    if (ty2 <= py2)
                        return true;
                }
            }
            else {
                if (tx2 > py2)
                    return false;
                if (tx2 <= px2)
                    return true;
            }
        }
        else {
            if (ty1 > px2)
                return false;
            if (ty1 <= px2)
                return true;
        }
    }
    else {
        if (tx1 > px2)
            return false;
        if (tx1 <= px2)
            return true;
    }
    return false;
}
int32 MathHelpers_Unknown12(int32 px1, int32 py1, int32 px2, int32 py2, int32 tx1, int32 tx2, int32 ty1, int32 ty2)
{
    if (!MathHelpers_Unknown9(px1, py1, px2, py2, tx1, tx2, ty1, ty2))
        return false;

    if (px1 == px2 && py1 == py2) {
        if (px1 != tx1 || py1 != tx2) {
            if (px1 == ty1 && py1 == ty2)
                return true;
            return false;
        }
        return true;
    }

    if (tx1 != ty1 || tx2 != ty2) {
        int32 valA = MathHelpers_GetValueSign(px1, py1, px2, py2, tx1, tx2);
        int32 valB = MathHelpers_GetValueSign(px1, py1, px2, py2, ty1, ty2);
        if (valA) {
            if (valA == valB)
                return 0;
        }
        else if (!valB) {
            if (MathHelpers_Unknown11(px1, py1, px2, py2, tx1, tx2) || MathHelpers_Unknown11(px1, py1, px2, py2, ty1, ty2)
                || MathHelpers_Unknown11(tx1, tx2, ty1, ty2, px1, py1)) {
                return true;
            }
            if (!MathHelpers_Unknown11(tx1, tx2, ty1, ty2, px2, py2))
                return false;
        }

        int32 res = MathHelpers_GetValueSign(tx1, tx2, ty1, ty2, px1, py1);
        if (!res)
            return true;
        if (res == MathHelpers_GetValueSign(tx1, tx2, ty1, ty2, px2, py2))
            return false;
        return true;
    }
    if (tx1 == px1 && tx2 == py1)
        return true;
    if (tx1 != px2 || tx2 != py2)
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
                radius = (((posTop2 - y) * (1.0 / 65536.0)) / (radius * (1.0 / 65536.0))) * -65536.0;
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
                        resultPos->x = x - radius;
                        resultPos->y = boxPos.y + (bottom << 16);
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
