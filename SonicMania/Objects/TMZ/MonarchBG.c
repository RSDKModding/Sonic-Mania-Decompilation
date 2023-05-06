// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MonarchBG Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMonarchBG *MonarchBG;

void MonarchBG_Update(void)
{
    RSDK_THIS(MonarchBG);

    StateMachine_Run(self->state);
}

void MonarchBG_LateUpdate(void) {}

void MonarchBG_StaticUpdate(void) {}

void MonarchBG_Draw(void)
{
    RSDK_THIS(MonarchBG);

    Vector2 drawPos;
    drawPos.x = self->position.x - ((self->position.x - ((ScreenInfo->position.x + ScreenInfo->center.x) << 16)) >> 1);
    drawPos.y = self->position.y - 208 * ((self->position.y - ((ScreenInfo->position.y + ScreenInfo->center.y) << 16)) >> 8);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animatorTop, &drawPos, false);
    RSDK.DrawSprite(&self->animatorBottom, &drawPos, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animatorTop, &drawPos, false);
    RSDK.DrawSprite(&self->animatorBottom, &drawPos, false);
}

void MonarchBG_Create(void *data)
{
    RSDK_THIS(MonarchBG);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->drawGroup     = 1;
        self->scale.x       = 0xE00;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->position.x    = 2048 << 16;
        self->position.y    = 3072 << 16;

        RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 0, &self->animatorTop, true, 0);
        RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 1, &self->animatorBottom, true, 0);
    }
}

void MonarchBG_StageLoad(void)
{
    MonarchBG->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MonarchBG.bin", SCOPE_STAGE);

    // Code for loading the MonarchBG mesh???
    // Does this mean the mesh was intended to be in the BG instead of the sprite at one point?
    MonarchBG->meshIndex  = RSDK.LoadMesh("TMZ/MonarchBG.bin", SCOPE_STAGE);
    MonarchBG->sceneIndex = RSDK.Create3DScene("View:Monarch", 0x2000, SCOPE_STAGE);

    RSDK.SetDiffuseColor(MonarchBG->sceneIndex, 0x40, 0x00, 0x40);
    RSDK.SetDiffuseIntensity(MonarchBG->sceneIndex, 8, 8, 8);
    RSDK.SetSpecularIntensity(MonarchBG->sceneIndex, 16, 16, 16);
}

#if GAME_INCLUDE_EDITOR
void MonarchBG_EditorDraw(void)
{
    RSDK_THIS(MonarchBG);

    self->position.x = 0x8000000;
    self->position.y = 0xC000000;
    self->drawFX     = FX_FLIP;

    RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 0, &self->animatorTop, true, 0);
    RSDK.SetSpriteAnimation(MonarchBG->aniFrames, 1, &self->animatorBottom, true, 0);

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animatorTop, NULL, false);
    RSDK.DrawSprite(&self->animatorBottom, NULL, false);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->animatorTop, NULL, false);
    RSDK.DrawSprite(&self->animatorBottom, NULL, false);
}

void MonarchBG_EditorLoad(void) { MonarchBG->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MonarchBG.bin", SCOPE_STAGE); }
#endif

void MonarchBG_Serialize(void) {}
