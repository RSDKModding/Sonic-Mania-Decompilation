// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: OOZFlames Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectOOZFlames *OOZFlames;

void OOZFlames_Update(void)
{
    RSDK_THIS(OOZFlames);

    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);
}

void OOZFlames_LateUpdate(void) {}

void OOZFlames_StaticUpdate(void) {}

void OOZFlames_Draw(void)
{
    RSDK_THIS(OOZFlames);

    Vector2 drawPos;
    drawPos.x = (ScreenInfo[SceneInfo->currentScreenID].center.x - 240) << 16;

    for (int32 angle = 0; angle < 0x100; angle += 0x10) {
        drawPos.y = (RSDK.Sin256(4 * (angle + Zone->timer)) << 11) + self->flamePos;
        RSDK.DrawSprite(&self->animator, &drawPos, true);

        drawPos.x += 0x200000;
    }
}

void OOZFlames_Create(void *data)
{
    RSDK_THIS(OOZFlames);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[1];
        self->active    = ACTIVE_NORMAL;
        self->drawFX    = FX_SCALE;
        self->inkEffect = INK_ADD;
        self->alpha     = 0x100;
        self->scale.x   = 0x200;
        self->scale.y   = 0x200;
        self->flamePos  = 0x1400000;
        self->state     = OOZFlames_State_Appear;
        RSDK.SetSpriteAnimation(OOZFlames->aniFrames, 0, &self->animator, true, 0);
    }
}

void OOZFlames_StageLoad(void) { OOZFlames->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Flames.bin", SCOPE_STAGE); }

void OOZFlames_State_Appear(void)
{
    RSDK_THIS(OOZFlames);

    if (self->flamePos > 0xE00000)
        self->flamePos -= 0x20000;

    if (++self->timer == 120) {
        self->timer = 0;
        self->state = OOZFlames_State_Rise;
    }
}

void OOZFlames_State_Rise(void)
{
    RSDK_THIS(OOZFlames);

    if (self->scale.y < 0x1000)
        self->scale.y += 0x10;
}

#if GAME_INCLUDE_EDITOR
void OOZFlames_EditorDraw(void) {}

void OOZFlames_EditorLoad(void) {}
#endif

void OOZFlames_Serialize(void) {}
