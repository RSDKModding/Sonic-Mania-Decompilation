// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggTV Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEggTV *EggTV;

void EggTV_Update(void)
{
    RSDK_THIS(EggTV);

    // Scanlines are drawn with INK_BLEND so this doesn't actually do anything...
    // (Probably done because it could get real laggy with so many lines)
    if (self->scanlineAlpha < 0x60)
        self->scanlineAlpha += 4;
}

void EggTV_LateUpdate(void) {}

void EggTV_StaticUpdate(void) { RSDK.ProcessAnimation(&EggTV->animator); }

void EggTV_Draw(void)
{
    EggTV_DrawTV();

    if (globals->gameMode != MODE_COMPETITION)
        EggTV_DrawScanlines();
}

void EggTV_Create(void *data)
{
    RSDK_THIS(EggTV);

    self->active    = ACTIVE_BOUNDS;
    self->drawGroup = self->drawOverTV ? Zone->objectDrawGroup[0] : (Zone->fgDrawGroup[0] + 1);

    self->visible       = true;
    self->drawFX        = FX_NONE;
    self->updateRange.x = 0x800000 + self->size.x;
    self->updateRange.y = 0x800000 + self->size.y;

    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.top    = -(self->size.y >> 17);
    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.bottom = self->size.x >> 17;
}

void EggTV_StageLoad(void)
{
    EggTV->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggTV.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EggTV->aniFrames, 0, &EggTV->animator, true, 0);
}

void EggTV_DrawScanlines(void)
{
    RSDK_THIS(EggTV);

    int32 y = (((Zone->timer >> 1) & 1) << 16) - (self->size.y >> 1) + self->position.y;
    if (self->size.y >= 0 && (self->size.y & 0xFFFF0000)) {
        for (int32 i = 0; i < (self->size.y >> 16); i += 2) {
            RSDK.DrawLine(self->position.x - (self->size.x >> 1), y, self->position.x + (self->size.x >> 1), y, 0x404060, self->scanlineAlpha,
                          INK_BLEND, false);
            y += 0x20000;
        }
    }
}
void EggTV_DrawTV(void)
{
    RSDK_THIS(EggTV);

    self->alpha     = 0xFF;
    self->drawFX    = FX_SCALE;
    self->inkEffect = INK_NONE;
    self->scale.x   = ((self->size.x >> 16) << 9) / 0x60;
    self->scale.y   = ((self->size.y >> 16) << 9) / 0x60;
    RSDK.DrawSprite(&EggTV->animator, NULL, false);

    self->drawFX = FX_NONE;
}

#if GAME_INCLUDE_EDITOR
void EggTV_EditorDraw(void)
{
    RSDK_THIS(EggTV);

    self->updateRange.x = 0x800000 + self->size.x;
    self->updateRange.y = 0x800000 + self->size.y;

    EggTV_DrawTV();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void EggTV_EditorLoad(void)
{
    EggTV->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggTV.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EggTV->aniFrames, 0, &EggTV->animator, true, 14);
}
#endif

void EggTV_Serialize(void)
{
    RSDK_EDITABLE_VAR(EggTV, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(EggTV, VAR_BOOL, drawOverTV);
}
