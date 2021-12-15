// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Valve Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectValve *Valve;

void Valve_Update(void)
{
    RSDK_THIS(Valve);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Valve->hitbox))
            OOZSetup->fadeTimer = 0;
    }

    if (self->animator2.animationID == 2 && self->animator2.frameID == 5 && self->animator2.timer == 1)
        RSDK.PlaySfx(Valve->sfxClick, 0, 255);
}

void Valve_LateUpdate(void) {}

void Valve_StaticUpdate(void) {}

void Valve_Draw(void)
{
    RSDK_THIS(Valve);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Valve_Create(void *data)
{
    RSDK_THIS(Valve);
    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow - 1;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &self->animator1, true, 3);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &self->animator2, true, 3);
    }
}

void Valve_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        Valve->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Valve.bin", SCOPE_STAGE);
    Valve->hitbox.left   = -16;
    Valve->hitbox.top    = -16;
    Valve->hitbox.right  = 16;
    Valve->hitbox.bottom = 16;
    Valve->sfxClick      = RSDK.GetSfx("Stage/Click.wav");
}

#if RETRO_INCLUDE_EDITOR
void Valve_EditorDraw(void)
{
    RSDK_THIS(Valve);
    RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &self->animator1, false, 3);
    RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &self->animator2, false, 3);

    Valve_Draw();
}

void Valve_EditorLoad(void) { Valve->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Valve.bin", SCOPE_STAGE); }
#endif

void Valve_Serialize(void) { RSDK_EDITABLE_VAR(Valve, VAR_UINT8, direction); }
