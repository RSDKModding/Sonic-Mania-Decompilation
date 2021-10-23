#include "SonicMania.h"

ObjectPuyoGame *PuyoGame;

void PuyoGame_Update(void)
{
    RSDK_THIS(PuyoGame);
    if (!entity->started) {
        // PuyoGame_SetupStartingEntities();
        entity->started = true;
    }

    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator);

    if ((RSDK_controller->keyStart.press || RSDK_unknown->field_10 == true) && !RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID) {
        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
        RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->triggerPlayer = 1;
        RSDK.PlaySfx(PauseMenu->sfxAccept, false, 255);
        RSDK.SetGameMode(ENGINESTATE_FROZEN);
    }
}

void PuyoGame_LateUpdate(void) {}

void PuyoGame_StaticUpdate(void) {}

void PuyoGame_Draw(void) {}

void PuyoGame_Create(void *data)
{
    RSDK_THIS(PuyoGame);
    entity->active        = ACTIVE_NORMAL;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = false;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!RSDK_sceneInfo->inEditor)
        RSDK.AddCamera(entity, RSDK_screens->centerX << 16, RSDK_screens->centerY << 16, true);
    // entity->state = PuyoGame_Unknown6;
}

void PuyoGame_StageLoad(void)
{
    PuyoGame->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoBeans.bin", SCOPE_STAGE);
    foreach_all(FXFade, fxFade) { PuyoGame->fxFade = fxFade; }

    TextInfo buffer;
    Localization_GetString(&buffer, STR_RPC_PLAYING);
    API.SetRichPresence(0, &buffer);
    RSDK.ResetEntitySlot(SLOT_ZONE, TYPE_BLANK, NULL);
}

void PuyoGame_EditorDraw(void) {}

void PuyoGame_EditorLoad(void) {}

void PuyoGame_Serialize(void) {}
