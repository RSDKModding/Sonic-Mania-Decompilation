// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DrawHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDrawHelpers *DrawHelpers = NULL;

void DrawHelpers_Update(void) {}

void DrawHelpers_LateUpdate(void) {}

void DrawHelpers_StaticUpdate(void) {}

void DrawHelpers_Draw(void) {}

void DrawHelpers_Create(void *data) {}

void DrawHelpers_StageLoad(void) {}

void DrawHelpers_DrawHitboxOutline(int32 x, int32 y, Hitbox *hitbox, uint8 direction, uint32 color)
{
    int16 left, top, right, bottom;

    if (direction & FLIP_X) {
        right = -hitbox->right;
        left  = -hitbox->left;
    }
    else {
        left  = hitbox->left;
        right = hitbox->right;
    }

    if (direction & FLIP_Y) {
        top    = -hitbox->top;
        bottom = -hitbox->bottom;
    }
    else {
        top    = hitbox->top;
        bottom = hitbox->bottom;
    }

    RSDK.DrawLine(x + (left << 16), y + (top << 16), x + (right << 16), y + (top << 16), color, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (right << 16), y + (top << 16), x + (right << 16), y + (bottom << 16), color, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (right << 16), y + (bottom << 16), x + (left << 16), y + (bottom << 16), color, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (left << 16), y + (bottom << 16), x + (left << 16), y + (top << 16), color, 0xFF, INK_NONE, false);
}

void DrawHelpers_DrawArrowAdditive(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color)
{
    int32 angle = RSDK.ATan2(x1 - x2, y1 - y2);
    RSDK.DrawLine(x1, y1, x2, y2, color, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle + 12) << 12), y2 + (RSDK.Sin256(angle + 12) << 12), color, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle - 12) << 12), y2 + (RSDK.Sin256(angle - 12) << 12), color, 0x7F, INK_ADD, false);
}

void DrawHelpers_DrawIsocelesTriangle(int32 x1, int32 y1, int32 x2, int32 y2, int32 edgeSize, uint32 color, uint32 inkEffect, uint32 alpha)
{
    Vector2 verts[3];

    int32 angle = RSDK.ATan2(x2 - x1, y2 - y1);

    verts[0].x = x2;
    verts[0].y = y2;
    verts[1].x = x1 + (edgeSize << 7) * RSDK.Cos256(angle + 64);
    verts[1].y = y1 + (edgeSize << 7) * RSDK.Sin256(angle + 64);
    verts[2].x = x1 + (edgeSize << 7) * RSDK.Cos256(angle - 64);
    verts[2].y = y1 + (edgeSize << 7) * RSDK.Sin256(angle - 64);

    if (SceneInfo->inEditor) {
        RSDK.DrawLine(x2, y2, verts[1].x, verts[1].y, color, 255, INK_NONE, false);
        RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, color, 255, INK_NONE, false);
        RSDK.DrawLine(verts[2].x, verts[2].y, x2, y2, color, 255, INK_NONE, false);
    }
    else {
        int32 screenX = ScreenInfo->position.x << 16;
        int32 screenY = ScreenInfo->position.y << 16;
        verts[0].x -= screenX;
        verts[0].y -= screenY;
        verts[1].x -= screenX;
        verts[1].y -= screenY;
        verts[2].x -= screenX;
        verts[2].y -= screenY;
        RSDK.DrawFace(verts, 3, (color >> 16) & 0xFF, (color >> 8) & 0xFF, (color >> 0) & 0xFF, alpha, inkEffect);
    }
}

void DrawHelpers_DrawCross(int32 x, int32 y, int32 sizeX, int32 sizeY, uint32 color)
{
    if (x || y) {
        RSDK.DrawLine(x - (sizeX >> 1), y - (sizeY >> 1), x + (sizeX >> 1), y + (sizeY >> 1), color, 0x7F, INK_NONE, false);
        RSDK.DrawLine(x + (sizeX >> 1), y - (sizeY >> 1), x - (sizeX >> 1), y + (sizeY >> 1), color, 0x7F, INK_NONE, false);
    }
}

// Custom ones!!

// Adds alpha & ink effect in params for extra customizability
void DrawHelpers_DrawArrow(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color, uint32 inkEffect, uint32 alpha)
{
    int32 angle = RSDK.ATan2(x1 - x2, y1 - y2);
    RSDK.DrawLine(x1, y1, x2, y2, color, alpha, inkEffect, false);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle + 12) << 12), y2 + (RSDK.Sin256(angle + 12) << 12), color, alpha, inkEffect, false);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle - 12) << 12), y2 + (RSDK.Sin256(angle - 12) << 12), color, alpha, inkEffect, false);
}

void DrawHelpers_DrawRectOutline(int32 x, int32 y, int32 sizeX, int32 sizeY, uint32 color)
{
    Vector2 drawPos;

    drawPos.x = x - (sizeX >> 1);
    drawPos.y = y - (sizeY >> 1);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y - TO_FIXED(1), drawPos.x + sizeX, drawPos.y - TO_FIXED(1), color, 0x00, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y + sizeY, drawPos.x + sizeX, drawPos.y + sizeY, color, 0x00, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - TO_FIXED(1), drawPos.y - TO_FIXED(1), drawPos.x - TO_FIXED(1), drawPos.y + sizeY, color, 0x00, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + sizeX, drawPos.y - TO_FIXED(1), drawPos.x + sizeX, drawPos.y + sizeY, color, 0x00, INK_NONE, false);
}

void DrawHelpers_DrawArenaBounds(int32 left, int32 top, int32 right, int32 bottom, uint8 sideMasks, uint32 color)
{
    RSDK_THIS_GEN();

    left <<= 16;
    top <<= 16;
    right <<= 16;
    bottom <<= 16;

    // left
    if (sideMasks & 1) {
        RSDK.DrawLine(self->position.x + left, self->position.y + top, self->position.x + left, self->position.y + bottom, color, 0, INK_NONE, false);
    }

    // top
    if (sideMasks & 2) {
        RSDK.DrawLine(self->position.x + left, self->position.y + top, self->position.x + right, self->position.y + top, color, 0, INK_NONE, false);
    }

    // right
    if (sideMasks & 4) {
        RSDK.DrawLine(self->position.x + right, self->position.y + top, self->position.x + right, self->position.y + bottom, color, 0, INK_NONE,
                      false);
    }

    // bottom
    if (sideMasks & 8) {
        RSDK.DrawLine(self->position.x + left, self->position.y + bottom, self->position.x + right, self->position.y + bottom, color, 0, INK_NONE,
                      false);
    }
}

#if GAME_INCLUDE_EDITOR
void DrawHelpers_EditorDraw(void) {}

void DrawHelpers_EditorLoad(void) {}
#endif

void DrawHelpers_Serialize(void) {}
