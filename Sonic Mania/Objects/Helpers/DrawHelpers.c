#include "SonicMania.h"

ObjectDrawHelpers *DrawHelpers = NULL;

void DrawHelpers_Update(void) {}

void DrawHelpers_LateUpdate(void) {}

void DrawHelpers_StaticUpdate(void) {}

void DrawHelpers_Draw(void) {}

void DrawHelpers_Create(void *data) {}

void DrawHelpers_StageLoad(void) {}

void DrawHelpers_DrawHitboxOutline(uint32 colour, uint8 dir, int32 x, int32 y, Hitbox *hitbox)
{
    int16 left, top, right, bottom;

    if (dir & FLIP_X) {
        right = -hitbox->right;
        left  = -hitbox->left;
    }
    else {
        left  = hitbox->left;
        right = hitbox->right;
    }

    if (dir & FLIP_Y) {
        top    = -hitbox->top;
        bottom = -hitbox->bottom;
    }
    else {
        top    = hitbox->top;
        bottom = hitbox->bottom;
    }

    RSDK.DrawLine(x + (left << 16), y + (top << 16), x + (right << 16), y + (top << 16), colour, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (right << 16), y + (top << 16), x + (right << 16), y + (bottom << 16), colour, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (right << 16), y + (bottom << 16), x + (left << 16), y + (bottom << 16), colour, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (left << 16), y + (bottom << 16), x + (left << 16), y + (top << 16), colour, 0xFF, INK_NONE, false);
}

void DrawHelpers_DrawArrow(uint32 colour, int32 x1, int32 y1, int32 x2, int32 y2)
{
    int32 angle = RSDK.ATan2(x1 - x2, y1 - y2);
    RSDK.DrawLine(x1, y1, x2, y2, colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle + 12) << 12), y2 + (RSDK.Sin256(angle + 12) << 12), colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle - 12) << 12), y2 + (RSDK.Sin256(angle - 12) << 12), colour, 0x7F, INK_ADD, false);
}

void DrawHelpers_DrawDebug4(uint32 colour, int32 a2, int32 alpha, int32 x1, int32 y1, int32 x2, int32 y2)
{
    Vector2 verts[3];

    int32 angle  = RSDK.ATan2(x2 - x1, y2 - y1);
    verts[0].x = x2;
    verts[0].y = x2;
    verts[1].x = (RSDK.Cos256(angle + 64) << 9) + x1;
    verts[1].y = (RSDK.Sin256(angle + 64) << 9) + y1;
    verts[2].x = (RSDK.Cos256(angle - 64) << 9) + x1;
    verts[2].y = (RSDK.Sin256(angle - 64) << 9) + y1;

    if (SceneInfo->inEditor) {
        RSDK.DrawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, colour, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, colour, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, colour, 0xFF, INK_NONE, false);
    }
    else {
        verts[0].x -= ScreenInfo->position.x << 16;
        verts[0].y -= ScreenInfo->position.y << 16;
        verts[1].x -= ScreenInfo->position.x << 16;
        verts[1].y -= ScreenInfo->position.y << 16;
        verts[2].x -= ScreenInfo->position.x << 16;
        verts[2].y -= ScreenInfo->position.y << 16;
        RSDK.DrawQuad(verts, 3, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, (colour >> 0) & 0xFF, alpha, INK_ALPHA);
    }
}

void DrawHelpers_DrawDebug5(uint32 colour, int32 x, int32 y)
{
    if (x || y) {
        RSDK.DrawLine(x - 0x100000, y - 0x100000, x + 0x100000, y + 0x100000, colour, 0x7F, INK_NONE, false);
        RSDK.DrawLine(x + 0x100000, y - 0x100000, x - 0x100000, y + 0x100000, colour, 0x7F, INK_NONE, false);
    }
}

void DrawHelpers_DrawRectOutline(uint32 colour, int32 x, int32 y, int32 sizeX, int32 sizeY)
{
    Vector2 drawPos;

    drawPos.x = x;
    drawPos.y = y;
    drawPos.x -= sizeX >> 1;
    drawPos.y -= sizeY >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + sizeX, drawPos.y - 0x10000, colour, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, sizeY + drawPos.y, drawPos.x + sizeX, sizeY + drawPos.y, colour, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + sizeY, colour, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + sizeX, drawPos.y - 0x10000, drawPos.x + sizeX, drawPos.y + sizeY, colour, 0, INK_NONE, false);
}

void DrawHelpers_DrawArenaBounds(uint32 colour, uint8 flags, int32 left, int32 top, int32 right, int32 bottom)
{
    RSDK_THIS_GEN();

    left <<= 16;
    top <<= 16;
    right <<= 16;
    bottom <<= 16;

    // left
    if (flags & 1) {
        RSDK.DrawLine(self->position.x + left, self->position.y + top, self->position.x + left, self->position.y + bottom, colour, 0,
                      INK_NONE, false);
    }

    // top
    if (flags & 2) {
        RSDK.DrawLine(self->position.x + left, self->position.y + top, self->position.x + right, self->position.y + top, colour, 0, INK_NONE,
                      false);
    }

    // right
    if (flags & 4) {
        RSDK.DrawLine(self->position.x + right, self->position.y + top, self->position.x + right, self->position.y + bottom, colour, 0,
                      INK_NONE, false);
    }

    // bottom
    if (flags & 8) {
        RSDK.DrawLine(self->position.x + left, self->position.y + bottom, self->position.x + right, self->position.y + bottom, colour, 0,
                      INK_NONE, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void DrawHelpers_EditorDraw(void) {}

void DrawHelpers_EditorLoad(void) {}
#endif

void DrawHelpers_Serialize(void) {}
