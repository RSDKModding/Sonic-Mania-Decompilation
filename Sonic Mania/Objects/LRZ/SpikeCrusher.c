// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpikeCrusher Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSpikeCrusher *SpikeCrusher;

void SpikeCrusher_Update(void) { Platform_Update(); }

void SpikeCrusher_LateUpdate(void) {}

void SpikeCrusher_StaticUpdate(void) {}

void SpikeCrusher_Draw(void)
{
    RSDK_THIS(SpikeCrusher);

    RSDK.DrawSprite(&self->animator2, &self->drawPos, false);
    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void SpikeCrusher_Create(void *data)
{
    RSDK_THIS(SpikeCrusher);

    self->collision = PLATFORM_C_HAZARD_BOTTOM;
    Platform_Create(NULL);
    if (!SceneInfo->inEditor) {
        self->drawOrder = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator, true, 4);
        RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator2, true, 5);

        while (!RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x7FFF0000, true)) {
            self->position.y += 0x10000;
        }

        self->updateRange.x = 0x800000;
        self->updateRange.y = self->position.y - self->centerPos.y + 0x200000;
        if (self->startDir == 1)
            self->drawPos.y = self->position.y;
        self->position.y = self->centerPos.y;
        self->state      = SpikeCrusher_State_Unknown1;
    }
}

void SpikeCrusher_StageLoad(void)
{
    SpikeCrusher->sfxHuff   = RSDK.GetSfx("Stage/Huff.wav");
    SpikeCrusher->sfxImpact = RSDK.GetSfx("Stage/Impact4.wav");
}

void SpikeCrusher_CheckOnScreen(void)
{
    RSDK_THIS(SpikeCrusher);

    if (!RSDK.CheckPosOnScreen(&self->drawPos, &self->updateRange) && !RSDK.CheckPosOnScreen(&self->centerPos, &self->updateRange)) {
        self->drawPos.x     = self->centerPos.x;
        self->drawPos.y     = self->centerPos.y;
        self->position.x    = self->centerPos.x;
        self->position.y    = self->centerPos.y;
        self->velocity.y    = 0;
        self->timer = 0;
        self->active        = ACTIVE_BOUNDS;
        SpikeCrusher_Create(NULL);
    }
}

void SpikeCrusher_State_Unknown1(void)
{
    RSDK_THIS(SpikeCrusher);

    self->active = ACTIVE_NORMAL;
    if (self->startDir == 0)
        self->state = SpikeCrusher_State_Unknown2;
    else
        self->state = SpikeCrusher_State_Unknown6;
}

void SpikeCrusher_State_Unknown2(void)
{
    RSDK_THIS(SpikeCrusher);

    self->drawPos.y += self->velocity.y;

    int storeX = self->position.x;
    int storeY = self->position.y;
    self->velocity.y += 0x3800;
    if (self->timer < 8 && (self->timer & 1))
        self->animator.frameID = self->timer >> 1;
    else
        self->animator.frameID = 4;
    self->timer++;
    self->position.x = self->drawPos.x;
    self->position.y = self->drawPos.y;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x200000, true)) {
        if (self->activeScreens)
            RSDK.PlaySfx(SpikeCrusher->sfxImpact, false, 255);
        self->timer         = 0;
        self->velocity.y    = 0;
        self->state         = SpikeCrusher_State_Unknown3;
    }
    self->drawPos.x  = self->position.x;
    self->drawPos.y  = self->position.y;
    self->position.x = storeX;
    self->position.y = storeY;
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown3(void)
{
    RSDK_THIS(SpikeCrusher);

    self->timer += 2;
    self->drawPos.y += 0x20000;
    self->animator2.frameID = self->timer + 5;
    if (self->timer >= 6) {
        self->timer = 15;
        self->drawPos.y     = self->drawPos.y + 0x8000;
        self->state         = SpikeCrusher_State_Unknown4;
    }
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown4(void)
{
    RSDK_THIS(SpikeCrusher);

    --self->timer;
    self->drawPos.y -= 0x8000;

    int frame = (self->timer >> 1) + 4;
    if (frame < 5)
        frame = 5;
    self->animator2.frameID = frame;
    if (self->timer <= 0)
        self->state = SpikeCrusher_State_Unknown5;
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown5(void)
{
    RSDK_THIS(SpikeCrusher);

    if (self->timer & 1)
        self->animator.frameID = 3 - (self->timer >> 1);
    else
        self->animator.frameID = 4;
    self->timer++;

    if (self->timer >= 8) {
        self->timer = 0;
        self->state         = SpikeCrusher_State_Unknown6;
    }
    SpikeCrusher_CheckOnScreen();
}

void SpikeCrusher_State_Unknown6(void)
{
    RSDK_THIS(SpikeCrusher);

    self->drawPos.y -= 0x10000;
    self->animator.frameID = 4 * (!(self->timer & 1));
    if (self->activeScreens && !(self->timer & 0x1F))
        RSDK.PlaySfx(SpikeCrusher->sfxHuff, false, 255);
    ++self->timer;
    if (self->drawPos.y <= self->centerPos.y) {
        self->timer = 0;
        self->drawPos.y     = self->centerPos.y;
        self->state         = SpikeCrusher_State_Unknown2;
    }
    self->velocity.y = -0x10000;
    SpikeCrusher_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void SpikeCrusher_EditorDraw(void)
{
    RSDK_THIS(SpikeCrusher);
    self->drawPos = self->position;
    RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator, true, 4);
    RSDK.SetSpriteAnimation(Platform->aniFrames, 3, &self->animator2, true, 5);

    SpikeCrusher_Draw();
}

void SpikeCrusher_EditorLoad(void) {}
#endif

void SpikeCrusher_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpikeCrusher, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(SpikeCrusher, VAR_ENUM, startDir);
}
