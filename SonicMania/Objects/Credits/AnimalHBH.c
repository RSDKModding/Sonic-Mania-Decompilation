// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AnimalHBH Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAnimalHBH *AnimalHBH;

void AnimalHBH_Update(void)
{
    RSDK_THIS(AnimalHBH);

    self->position.x -= 0x10000;

    int32 pos = self->position.x - (ScreenInfo->position.x << 16) - (ScreenInfo->center.x << 16);

    if (pos < 0)
        pos = ((ScreenInfo->position.x + ScreenInfo->center.x) << 16) - self->position.x;

    if (pos < 0x800000)
        AnimalHBH->colorCount = 0x20 * self->type;

    if (!self->type) {
        ++self->timer;
        if ((ControllerInfo->keyA.press || ControllerInfo->keyStart.press))
            self->timer = 1400;

        if (self->timer == 1400) {
            EntityFXFade *fxFade = CREATE_ENTITY(FXFade, NULL, self->position.x, self->position.y);
            fxFade->speedIn      = 12;
            fxFade->wait         = 240;
        }

        if (self->timer == 1536) {
            RSDK.SetScene("Presentation", "Menu");
            RSDK.LoadScene();
        }

        if (AnimalHBH->currentColor < AnimalHBH->colorCount)
            AnimalHBH->currentColor++;

        RSDK.SetPaletteEntry(1, 1, RSDK.GetPaletteEntry(5, AnimalHBH->currentColor));
    }
}

void AnimalHBH_LateUpdate(void) {}

void AnimalHBH_StaticUpdate(void) {}

void AnimalHBH_Draw(void)
{
    RSDK_THIS(AnimalHBH);

    Vector2 drawPos;
    drawPos.x = 2 * (self->position.x - ((ScreenInfo->position.x + ScreenInfo->center.x) << 15));
    drawPos.y = self->position.y;

    RSDK.SetActivePalette(1, 0, ScreenInfo->size.y);
    RSDK.DrawSprite(&self->silhouetteAnimator, &drawPos, false);

    RSDK.SetActivePalette(4, 0, ScreenInfo->size.y);
    RSDK.DrawSprite(&self->animalHBHAnimator, NULL, false);

    RSDK.SetActivePalette(1, 0, ScreenInfo->size.y);
}

void AnimalHBH_Create(void *data)
{
    RSDK_THIS(AnimalHBH);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 2;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(AnimalHBH->aniFrames, self->type, &self->animalHBHAnimator, true, 0);
        RSDK.SetSpriteAnimation(AnimalHBH->silhouetteFrames, self->type, &self->silhouetteAnimator, true, 0);
    }
}

void AnimalHBH_StageLoad(void)
{
    AnimalHBH->aniFrames        = RSDK.LoadSpriteAnimation("Credits/AnimalHBH.bin", SCOPE_STAGE);
    AnimalHBH->silhouetteFrames = RSDK.LoadSpriteAnimation("Credits/Silhouettes.bin", SCOPE_STAGE);

    AnimalHBH->currentColor = 0;
    AnimalHBH->colorCount   = 0;
}

#if GAME_INCLUDE_EDITOR
void AnimalHBH_EditorDraw(void)
{
    RSDK_THIS(AnimalHBH);
    RSDK.SetSpriteAnimation(AnimalHBH->aniFrames, self->type, &self->animalHBHAnimator, true, 0);

    RSDK.DrawSprite(&self->animalHBHAnimator, NULL, false);
}

void AnimalHBH_EditorLoad(void) { AnimalHBH->aniFrames = RSDK.LoadSpriteAnimation("Credits/AnimalHBH.bin", SCOPE_STAGE); }
#endif

void AnimalHBH_Serialize(void) { RSDK_EDITABLE_VAR(AnimalHBH, VAR_UINT8, type); }
