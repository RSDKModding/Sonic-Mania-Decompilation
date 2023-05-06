// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Valve Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectValve *Valve;

void Valve_Update(void)
{
    RSDK_THIS(Valve);

    RSDK.ProcessAnimation(&self->valveAnimator);
    RSDK.ProcessAnimation(&self->wheelAnimator);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Valve->hitbox))
            OOZSetup->smogTimer = 0;
    }

    if (self->wheelAnimator.animationID == 2 && self->wheelAnimator.frameID == 5 && self->wheelAnimator.timer == 1)
        RSDK.PlaySfx(Valve->sfxClick, false, 0xFF);
}

void Valve_LateUpdate(void) {}

void Valve_StaticUpdate(void) {}

void Valve_Draw(void)
{
    RSDK_THIS(Valve);

    RSDK.DrawSprite(&self->wheelAnimator, NULL, false);
    RSDK.DrawSprite(&self->valveAnimator, NULL, false);
}

void Valve_Create(void *data)
{
    RSDK_THIS(Valve);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0] - 1;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &self->valveAnimator, true, 3);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &self->wheelAnimator, true, 3);
    }
}

void Valve_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        Valve->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Valve.bin", SCOPE_STAGE);

    Valve->hitbox.left   = -16;
    Valve->hitbox.top    = -16;
    Valve->hitbox.right  = 16;
    Valve->hitbox.bottom = 16;

    Valve->sfxClick = RSDK.GetSfx("Stage/Click.wav");
}

#if GAME_INCLUDE_EDITOR
void Valve_EditorDraw(void)
{
    RSDK_THIS(Valve);

    RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &self->valveAnimator, false, 3);
    RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &self->wheelAnimator, false, 3);

    Valve_Draw();
}

void Valve_EditorLoad(void)
{
    Valve->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Valve.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Valve, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void Valve_Serialize(void) { RSDK_EDITABLE_VAR(Valve, VAR_UINT8, direction); }
