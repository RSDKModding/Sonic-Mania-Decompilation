// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpikeCrusher Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpikeCrusher *SpikeCrusher;

void SpikeCrusher_Update(void) { Platform_Update(); }

void SpikeCrusher_LateUpdate(void) {}

void SpikeCrusher_StaticUpdate(void) {}

void SpikeCrusher_Draw(void)
{
    RSDK_THIS(SpikeCrusher);

    RSDK.DrawSprite(&self->spikeAnimator, &self->drawPos, false);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void SpikeCrusher_Create(void *data)
{
    RSDK_THIS(SpikeCrusher);

    self->collision = PLATFORM_C_SOLID_HURT_BOTTOM;
    Platform_Create(NULL);

    if (!SceneInfo->inEditor) {
        self->drawGroup = Zone->objectDrawGroup[1];
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator, true, 4);
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->spikeAnimator, true, 5);

        while (!RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x7FFF0000, true)) self->position.y += 0x10000;

        self->updateRange.x = 0x800000;
        self->updateRange.y = self->position.y - self->centerPos.y + 0x200000;
        if (self->startDir == FLIP_X)
            self->drawPos.y = self->position.y;

        self->position.y = self->centerPos.y;
        self->state      = SpikeCrusher_State_Init;
    }
}

void SpikeCrusher_StageLoad(void)
{
    SpikeCrusher->sfxHuff   = RSDK.GetSfx("Stage/Huff.wav");
    SpikeCrusher->sfxImpact = RSDK.GetSfx("Stage/Impact4.wav");
}

void SpikeCrusher_CheckOffScreen(void)
{
    RSDK_THIS(SpikeCrusher);

    if (!RSDK.CheckPosOnScreen(&self->drawPos, &self->updateRange) && !RSDK.CheckPosOnScreen(&self->centerPos, &self->updateRange)) {
        self->drawPos    = self->centerPos;
        self->position   = self->centerPos;
        self->velocity.y = 0;
        self->timer      = 0;
        self->active     = ACTIVE_BOUNDS;
        SpikeCrusher_Create(NULL);
    }
}

void SpikeCrusher_State_Init(void)
{
    RSDK_THIS(SpikeCrusher);

    self->active = ACTIVE_NORMAL;
    if (self->startDir == FLIP_NONE)
        self->state = SpikeCrusher_State_MovingDown;
    else
        self->state = SpikeCrusher_State_MovingUp;
}

void SpikeCrusher_State_MovingDown(void)
{
    RSDK_THIS(SpikeCrusher);

    self->drawPos.y += self->velocity.y;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    self->velocity.y += 0x3800;

    if (self->timer < 8 && (self->timer & 1))
        self->animator.frameID = self->timer >> 1;
    else
        self->animator.frameID = 4;
    self->timer++;

    self->position = self->drawPos;
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        if (self->onScreen)
            RSDK.PlaySfx(SpikeCrusher->sfxImpact, false, 0xFF);

        self->timer      = 0;
        self->velocity.y = 0;
        self->state      = SpikeCrusher_State_Crushing;
    }

    self->drawPos = self->position;

    self->position.x = storeX;
    self->position.y = storeY;
    SpikeCrusher_CheckOffScreen();
}

void SpikeCrusher_State_Crushing(void)
{
    RSDK_THIS(SpikeCrusher);

    self->timer += 2;
    self->drawPos.y += 0x20000;
    self->spikeAnimator.frameID = self->timer + 5;

    if (self->timer >= 6) {
        self->timer     = 15;
        self->drawPos.y = self->drawPos.y + 0x8000;
        self->state     = SpikeCrusher_State_CrushBounce;
    }
    SpikeCrusher_CheckOffScreen();
}

void SpikeCrusher_State_CrushBounce(void)
{
    RSDK_THIS(SpikeCrusher);

    --self->timer;
    self->drawPos.y -= 0x8000;

    self->spikeAnimator.frameID = MAX((self->timer >> 1) + 4, 5);

    if (self->timer <= 0)
        self->state = SpikeCrusher_State_ActivateFlames;

    SpikeCrusher_CheckOffScreen();
}

void SpikeCrusher_State_ActivateFlames(void)
{
    RSDK_THIS(SpikeCrusher);

    if (self->timer & 1)
        self->animator.frameID = 3 - (self->timer >> 1);
    else
        self->animator.frameID = 4;

    if (++self->timer >= 8) {
        self->timer = 0;
        self->state = SpikeCrusher_State_MovingUp;
    }

    SpikeCrusher_CheckOffScreen();
}

void SpikeCrusher_State_MovingUp(void)
{
    RSDK_THIS(SpikeCrusher);

    self->drawPos.y -= 0x10000;

    // Animate Flames
    self->animator.frameID = 4 * (!(self->timer & 1));

    if (self->onScreen && !(self->timer & 0x1F))
        RSDK.PlaySfx(SpikeCrusher->sfxHuff, false, 255);

    ++self->timer;
    if (self->drawPos.y <= self->centerPos.y) {
        self->timer     = 0;
        self->drawPos.y = self->centerPos.y;
        self->state     = SpikeCrusher_State_MovingDown;
    }

    self->velocity.y = -0x10000;
    SpikeCrusher_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void SpikeCrusher_EditorDraw(void)
{
    RSDK_THIS(SpikeCrusher);

    if (Platform) {
        self->drawPos = self->position;
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator, true, 4);
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->spikeAnimator, true, 5);

        SpikeCrusher_Draw();
    }

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xFFFF00, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SpikeCrusher_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(SpikeCrusher, startDir);
    RSDK_ENUM_VAR("Moving Down", FLIP_NONE);
    RSDK_ENUM_VAR("Moving Up", FLIP_X);
}
#endif

void SpikeCrusher_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeCrusher, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(SpikeCrusher, VAR_ENUM, startDir);
}
