// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Decoration Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDecoration *Decoration;

void Decoration_Update(void)
{
    RSDK_THIS(Decoration);

    RSDK.ProcessAnimation(&self->animator);

    self->rotation = (self->rotation + self->rotSpeed) & 0x1FF;
}

void Decoration_LateUpdate(void) {}

void Decoration_StaticUpdate(void) {}

void Decoration_Draw(void)
{
    RSDK_THIS(Decoration);

    if (self->additive) {
        RSDK.CopyPalette(0, 160, 1, 160, 16);
        RSDK.CopyPalette(2, 160, 0, 160, 16);
    }

    Decoration_DrawSprite();

    if (self->additive)
        RSDK.CopyPalette(1, 160, 0, 160, 16);
}

void Decoration_Create(void *data)
{
    RSDK_THIS(Decoration);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active  = ACTIVE_BOUNDS;
        self->visible = true;

        if (self->rotSpeed)
            self->drawFX |= FX_ROTATE;

        if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2"))
            self->additive = true;

        if (self->additive) {
            self->inkEffect |= INK_ADD;
            self->alpha = 0xE0;
        }

        self->updateRange.x = TO_FIXED(128);
        self->updateRange.y = TO_FIXED(128);
        RSDK.SetSpriteAnimation(Decoration->aniFrames, self->type, &self->animator, true, 0);

        if (RSDK.GetFrameID(&self->animator)) // ideally use 'h'
            self->drawGroup = Zone->objectDrawGroup[1];
        else
            self->drawGroup = Zone->objectDrawGroup[0];
    }
}

void Decoration_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("CPZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SPZ1"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("FBZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("SSZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("MMZ1") || RSDK.CheckSceneFolder("MMZ2"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("MSZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Decoration.bin", SCOPE_STAGE);
#if MANIA_USE_PLUS
    else if (RSDK.CheckSceneFolder("MSZEnding"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Ending.bin", SCOPE_STAGE);
#endif
    else if (RSDK.CheckSceneFolder("HCZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("TMZ3"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Decoration.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("AIZ"))
        Decoration->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Decoration.bin", SCOPE_STAGE);
}

void Decoration_DrawSprite(void)
{
    RSDK_THIS(Decoration);

    Vector2 drawPos, repeat;

    repeat.x  = FROM_FIXED(self->repeatTimes.x);
    repeat.y  = FROM_FIXED(self->repeatTimes.y);
    drawPos.x = self->position.x - ((repeat.x * self->repeatSpacing.x) >> 1);
    drawPos.y = self->position.y - ((repeat.y * self->repeatSpacing.y) >> 1);

    for (int32 y = 0; y <= repeat.y; ++y) {
        drawPos.x = self->position.x - (repeat.x * self->repeatSpacing.x >> 1);
        for (int32 x = 0; x <= repeat.x; ++x) {
            RSDK.DrawSprite(&self->animator, &drawPos, false);
            drawPos.x += self->repeatSpacing.x;
        }
        drawPos.y += self->repeatSpacing.y;
    }
}

#if GAME_INCLUDE_EDITOR
void Decoration_EditorDraw(void)
{
    RSDK_THIS(Decoration);

    RSDK.SetSpriteAnimation(Decoration->aniFrames, self->type, &self->animator, true, 0);

    if (self->rotSpeed)
        self->drawFX |= FX_ROTATE;
    else
        self->drawFX &= ~FX_ROTATE;

    Decoration_DrawSprite();
}

void Decoration_EditorLoad(void)
{
    Decoration_StageLoad();

    RSDK_ACTIVE_VAR(Decoration, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void Decoration_Serialize(void)
{
    RSDK_EDITABLE_VAR(Decoration, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Decoration, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Decoration, VAR_ENUM, rotSpeed);
    RSDK_EDITABLE_VAR(Decoration, VAR_VECTOR2, repeatTimes);
    RSDK_EDITABLE_VAR(Decoration, VAR_VECTOR2, repeatSpacing);
}
