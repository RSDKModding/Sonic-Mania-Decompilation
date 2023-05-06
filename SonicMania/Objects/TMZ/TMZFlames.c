// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZFlames Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZFlames *TMZFlames;

void TMZFlames_Update(void)
{
    RSDK_THIS(TMZFlames);

    self->active = ACTIVE_NORMAL;

    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);
}

void TMZFlames_LateUpdate(void) {}

void TMZFlames_StaticUpdate(void) {}

void TMZFlames_Draw(void)
{
    RSDK_THIS(TMZFlames);

    Vector2 drawPos;
    drawPos.y = (ScreenInfo[SceneInfo->currentScreenID].center.y - 112) << 16;
    for (int32 i = 0; i < 0x80; i += 0x10) {
        drawPos.x = (RSDK.Sin256(4 * (i + Zone->timer)) << 11) + self->offset;
        RSDK.DrawSprite(&self->animator, &drawPos, true);

        drawPos.y += 0x200000;
    }
}

void TMZFlames_Create(void *data)
{
    RSDK_THIS(TMZFlames);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_SCALE;
        self->inkEffect     = INK_ADD;
        self->alpha         = 0x100;
        self->scale.x       = 0x400;
        self->scale.y       = 0x200;
        self->offset        = -0x800000;

        self->state = TMZFlames_State_Delay;
        RSDK.SetSpriteAnimation(TMZFlames->aniFrames, 0, &self->animator, true, 0);
    }
}

void TMZFlames_StageLoad(void) { TMZFlames->aniFrames = RSDK.LoadSpriteAnimation("Phantom/Flames.bin", SCOPE_STAGE); }

void TMZFlames_State_Delay(void)
{
    RSDK_THIS(TMZFlames);

    if (++self->timer == 240) {
        self->timer = 0;
        self->state = TMZFlames_State_EnterFlames;
    }
}

void TMZFlames_State_EnterFlames(void)
{
    RSDK_THIS(TMZFlames);

    if (self->offset < 0)
        self->offset += 0x40000;

    if (++self->timer == 260) {
        self->timer = 0;
        self->state = TMZFlames_State_FlamesEnlarge;
    }
}

void TMZFlames_State_FlamesEnlarge(void)
{
    RSDK_THIS(TMZFlames);

    if (self->scale.x < 0x1000)
        self->scale.x += 16;
}

#if GAME_INCLUDE_EDITOR
void TMZFlames_EditorDraw(void) {}

void TMZFlames_EditorLoad(void) {}
#endif

void TMZFlames_Serialize(void) {}
