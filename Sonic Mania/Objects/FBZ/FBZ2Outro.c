#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectFBZ2Outro *FBZ2Outro;

void FBZ2Outro_Update(void)
{
    RSDK_THIS(FBZ2Outro);
    FBZ2Outro_StartCutscene(entity);
    entity->active = ACTIVE_NEVER;
}

void FBZ2Outro_LateUpdate(void) {}

void FBZ2Outro_StaticUpdate(void) {}

void FBZ2Outro_Draw(void) {}

void FBZ2Outro_Create(void *data)
{
    RSDK_THIS(FBZ2Outro);
    entity->active      = ACTIVE_NORMAL;
    entity->isPermanent = true;
}

void FBZ2Outro_StageLoad(void) {}

void FBZ2Outro_StartCutscene(EntityFBZ2Outro *outro)
{
    void *states[] = { FBZ2Outro_CutsceneState_Unknown1, FBZ2Outro_CutsceneState_Unknown2, FBZ2Outro_CutsceneState_Unknown3, NULL };

    CutsceneSeq_StartSequence((Entity *)outro, states);

    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
        RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;

    foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
}

bool32 FBZ2Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state      = Player_State_Ground;
        player->stateInput = 0;
        player->right      = true;
    }

    Vector2 size;
    RSDK.GetLayerSize(Zone->fgLow, &size, true);
    size.x -= 128;
    for (int32 p = 0; p < Player->playerCount; ++p) {
        Zone->screenBoundsR1[p]     = size.x;
        Zone->playerBoundActiveR[p] = false;
    }

    foreach_all(HangGlider, glider) { glider->active = ACTIVE_NORMAL; }
    return true;
}

bool32 FBZ2Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    foreach_active(Player, player)
    {
        player->jumpPress = false;
        if (player->playerAnimator.animationID == ANI_PUSH) {
            player->jumpPress = true;
            player->jumpHold  = true;
        }
        else {
            if (player->velocity.y > 0x20000)
                player->jumpHold = false;
        }

        if (player->sidekick) {
            foreach_active(HangGlider, glider)
            {
                if (player->onGround && glider->position.x - player->position.x < 0x1600000 && player->position.y - glider->position.y > 0x300000) {
                    player->jumpPress    = true;
                    player->jumpHold     = true;
                    player->jumpStrength = 0x70000;
                }
            }
        }
    }

    if (RSDK.GetEntityCount(Player->objectID, true)) {
        foreach_active(HangGlider, glider)
        {
            if (glider->state == HangGlider_Unknown2) {
                foreach_active(FBZFan, fan)
                {
                    if (abs(fan->position.x - glider->position.x) < 0x400000 && fan->position.y - glider->position.y < 0xA00000)
                        glider->velocity.y -= 0x3000;
                }
            }
        }
    }
    else {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            Zone->screenBoundsT1[p] = Zone->screenBoundsB1[p] - RSDK_screens->height;
        }
        return true;
    }
    return false;
}

bool32 FBZ2Outro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    foreach_active(HangGlider, glider)
    {
        foreach_active(FBZFan, fan)
        {
            if (abs(fan->position.x - glider->position.x) < 0x400000 && fan->position.y - glider->position.y < 0xA00000)
                glider->velocity.y -= 0x3000;
        }
        if (glider->playerPtr == (Entity *)player1 && !glider->activeScreens) {
            Zone_StartFadeOut(10, 0x000000);
            return true;
        }
    }
    return false;
}

void FBZ2Outro_EditorDraw(void) {}

void FBZ2Outro_EditorLoad(void) {}

void FBZ2Outro_Serialize(void) {}
#endif
