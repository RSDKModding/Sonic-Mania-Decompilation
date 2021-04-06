#include "../SonicMania.h"

ObjectMathHelpers *MathHelpers = NULL;

void MathHelpers_Update(void) {}

void MathHelpers_LateUpdate(void) {}

void MathHelpers_StaticUpdate(void) {}

void MathHelpers_Draw(void) {}

void MathHelpers_Create(void *data) {}

void MathHelpers_StageLoad(void) {}

void MathHelpers_Unknown1(Vector2 *pos, int a1, int a3, int a4, int a5, int a6)
{
    if (a1 < 0) {
        pos->x = a3;
        pos->y = a4;
    }
    else if (a1 >= 0x100) {
        pos->x = a5;
        pos->y = a6;
    }
    else {
        pos->x = a3 + a1 * ((a5 - a3) >> 8);
        pos->y = a4 + a1 * ((a6 - a4) >> 8);
    }
}

void MathHelpers_Unknown2(Vector2 *pos, int a1, int a3, int a4, int a5, int a6)
{
    if (a1 < 0) {
        pos->x = a3;
        pos->y = a4;
    }
    else if (a1 >= 0x100) {
        pos->x = a5;
        pos->y = a6;
    }
    else {
        int val = (RSDK.Sin1024(a1 + 0x300) >> 2) + 256;
        pos->x  = a3 + val * ((a5 - a3) >> 8);
        pos->y  = a4 + val * ((a6 - a4) >> 8);
    }
}

void MathHelpers_Unknown3(Vector2 *pos, int a1, int a3, int a4, int a5, int a6)
{
    if (a1 < 0) {
        pos->x = a3;
        pos->y = a4;
    }
    else if (a1 >= 0x100) {
        pos->x = a5;
        pos->y = a6;
    }
    else {
        pos->x = a3 + (RSDK.Sin1024(a1) >> 2) * ((a5 - a3) >> 8);
        pos->y = a4 + (RSDK.Sin1024(a1) >> 2) * ((a6 - a4) >> 8);
    }
}

void MathHelpers_Unknown4(Vector2 *pos, int a1, int a3, int a4, int a5, int a6)
{
    if (a1 < 0) {
        pos->x = a3;
        pos->y = a4;
    }
    else if (a1 >= 0x100) {
        pos->x = a5;
        pos->y = a6;
    }
    else {
        int val = (RSDK.Sin512(a1 + 0x180) >> 2) + 0x80;
        pos->x  = a3 + val * ((a5 - a3) >> 8);
        pos->y  = a4 + val * ((a6 - a4) >> 8);
    }
}

int Game_Unknown20(int px1, int py1, int px2, int py2, int tx1, int tx2, int ty1, int ty2)
{
    if (!Game_Unknown23(px1, py1, px2, py2, tx1, tx2, ty1, ty2))
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
        int valA = Game_Unknown21(px1, py1, px2, py2, tx1, tx2);
        int valB = Game_Unknown21(px1, py1, px2, py2, ty1, ty2);
        if (valA) {
            if (valA == valB)
                return 0;
        }
        else if (!valB) {
            if (Game_Unknown22(px1, py1, px2, py2, tx1, tx2) || Game_Unknown22(px1, py1, px2, py2, ty1, ty2)
                || Game_Unknown22(tx1, tx2, ty1, ty2, px1, py1)) {
                return true;
            }
            if (!Game_Unknown22(tx1, tx2, ty1, ty2, px2, py2))
                return false;
        }

        int res = Game_Unknown21(tx1, tx2, ty1, ty2, px1, py1);
        if (!res)
            return true;
        if (res == Game_Unknown21(tx1, tx2, ty1, ty2, px2, py2))
            return false;
        return true;
    }
    if (tx1 == px1 && tx2 == py1)
        return true;
    if (tx1 != px2 || tx2 != py2)
        return false;
    return true;
}
int Game_Unknown21(int px1, int py1, int px2, int py2, int tx1, int tx2)
{
    int result = ((tx2 - py1) >> 16) * ((px2 - px1) >> 16) - ((tx1 - px1) >> 16) * ((py2 - py1) >> 16);
    if (((tx2 - py1) >> 16) * ((px2 - px1) >> 16) == ((tx1 - px1) >> 16) * ((py2 - py1) >> 16))
        return 0;
    if (result > 0)
        return 1;
    if (result < 0)
        result = -1;
    return result;
}
bool32 Game_Unknown22(int tx1, int tx2, int ty1, int ty2, int px2, int py2)
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
    return true;
    return false;
}
bool32 Game_Unknown23(int px1, int py1, int px2, int py2, int tx1, int tx2, int ty1, int ty2)
{
    int v8 = px2;
    int v9 = px2;
    if (px1 < px2)
        v9 = px1;
    int v10 = py2;
    int v11 = py2;
    if (py1 < py2)
        v11 = py1;
    int v17 = v11;
    int v12 = ty2;
    if (px1 > px2)
        v8 = px1;
    int v18 = v8;
    int v13 = ty1;
    int v14 = ty2;
    if (py1 > py2)
        v10 = py1;
    int v15 = ty1;
    if (tx1 < ty1)
        v15 = tx1;
    if (tx2 < ty2)
        v14 = tx2;
    if (tx1 > ty1)
        v13 = tx1;
    if (tx2 > ty2)
        v12 = tx2;
    return v9 <= v13 && v18 >= v15 && v17 <= v12 && v10 >= v14;
}

void MathHelpers_EditorDraw(void) {}

void MathHelpers_EditorLoad(void) {}

void MathHelpers_Serialize(void) {}
