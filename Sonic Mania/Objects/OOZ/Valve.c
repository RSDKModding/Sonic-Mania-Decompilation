#include "SonicMania.h"

ObjectValve *Valve;

void Valve_Update(void)
{
    RSDK_THIS(Valve);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Valve->hitbox))
            OOZSetup->fadeTimer = 0;
    }

    if (entity->animator2.animationID == 2 && entity->animator2.frameID == 5 && entity->animator2.animationTimer == 1)
        RSDK.PlaySFX(Valve->sfxClick, 0, 255);
}

void Valve_LateUpdate(void) {}

void Valve_StaticUpdate(void) {}

void Valve_Draw(void)
{
    RSDK_THIS(Valve);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Valve_Create(void *data)
{
    RSDK_THIS(Valve);
    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow - 1;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &entity->animator1, true, 3);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &entity->animator2, true, 3);
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
    Valve->sfxClick      = RSDK.GetSFX("Stage/Click.wav");
}

void Valve_EditorDraw(void) {}

void Valve_EditorLoad(void) {}

void Valve_Serialize(void) { RSDK_EDITABLE_VAR(Valve, VAR_UINT8, direction); }
