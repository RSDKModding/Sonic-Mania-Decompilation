// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZ1Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTMZ1Outro *TMZ1Outro;

void TMZ1Outro_Update(void)
{
    RSDK_THIS(TMZ1Outro);

    if (Zone->actID) {
        void *states[] = { TMZ1Outro_Cutscene2_State1, NULL };
        CutsceneSeq_StartSequence((Entity *)self, states);
        RSDK.SetLimitedFade(0, 5, 4, self->alpha, 128, 256);
    }
    else {
        void *states[] = { TMZ1Outro_Cutscene1_State1, TMZ1Outro_Cutscene1_State2, TMZ1Outro_Cutscene1_State3, TMZ1Outro_Cutscene1_State4, NULL };
        CutsceneSeq_StartSequence((Entity *)self, states);
    }
#if RETRO_USE_PLUS
    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
        RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;
#endif
    self->active = ACTIVE_NEVER;
}

void TMZ1Outro_LateUpdate(void) {}

void TMZ1Outro_StaticUpdate(void) {}

void TMZ1Outro_Draw(void) {}

void TMZ1Outro_Create(void *data)
{
    RSDK_THIS(TMZ1Outro);
    if (!SceneInfo->inEditor) {
        self->active  = ACTIVE_NORMAL;
        self->visible = false;
    }
}

void TMZ1Outro_StageLoad(void) {}

bool32 TMZ1Outro_Cutscene1_State1(EntityCutsceneSeq *host)
{
    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state      = Player_State_Ground;
        player->stateInput = StateMachine_None;
    }

    return true;
}

bool32 TMZ1Outro_Cutscene1_State2(EntityCutsceneSeq *host)
{
    if (host->timer == 120) {
        CrimsonEye_SetArrowDir(CE_ARROW_UP);
        RSDK.PlaySfx(CrimsonEye->sfxBeep, false, 255);
    }

    if (host->timer == 160) {
        Camera_ShakeScreen(0, 0, 6);
        CrimsonEye->scrollLimit            = -0x10000;
        TileLayer *moveLayer               = RSDK.GetSceneLayer(Zone->moveLayer);
        moveLayer->drawLayer[0]            = 0;
        moveLayer->scrollPos               = 0x5000000;
        moveLayer->scrollInfo[0].scrollPos = -0x4D00000;
        RSDK.PlaySfx(CrimsonEye->sfxHullClose, false, 255);
    }

    if (host->timer > 160 && !(host->timer & 0x3F))
        RSDK.PlaySfx(CrimsonEye->sfxElevator, false, 255);

    if (host->timer > 200) {
        if (CrimsonEye->scrollLimit) {
            if (RSDK.GetSceneLayer(Zone->moveLayer)->scrollPos < 0x2300000)
                CrimsonEye->scrollLimit = 0;
        }
        else if (!CrimsonEye->scrollPos) {
            Camera_ShakeScreen(0, 0, 6);
            RSDK.PlaySfx(CrimsonEye->sfxHullClose, false, 255);
            return true;
        }
    }

    foreach_active(ItemBox, itembox) { itembox->position.y -= CrimsonEye->scrollLimit; }
    foreach_active(SignPost, signPost) { signPost->position.y -= CrimsonEye->scrollLimit; }

    return false;
}

bool32 TMZ1Outro_Cutscene1_State3(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ1Outro);

    if (host->timer >= 30) {
        bool32 flag = true;
        foreach_active(Player, player)
        {
            if (abs(self->position.x - player->position.x) >= 0x100000) {
                if (player->position.x > self->position.x) {
                    player->left = true;
                    flag         = false;
                }
                if (player->position.x < self->position.x) {
                    player->right = true;
                    flag          = false;
                }
            }
            else {
                player->left  = false;
                player->right = false;
                player->groundVel -= player->groundVel >> 3;
                player->velocity.x -= player->velocity.x >> 3;
                player->direction = FLIP_NONE;
            }

            player->jumpPress = false;
            player->jumpHold  = true;
            if (player->animator.animationID == ANI_PUSH)
                player->jumpPress = true;
            if (!player->onGround || player->velocity.x)
                flag = false;
        }
        return flag;
    }
    return false;
}

bool32 TMZ1Outro_Cutscene1_State4(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ1Outro);
    self->alpha += 4;
    RSDK.SetLimitedFade(0, 3, 5, self->alpha, 128, 256);

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (camera->offset.x > 0)
        camera->offset.x -= 0x10000;
    if (self->alpha == 320) {
        globals->suppressTitlecard = true;
        globals->suppressAutoMusic = true;
        Zone_StoreEntities((ScreenInfo->position.x + ScreenInfo->centerX) << 16, (ScreenInfo->height + ScreenInfo->position.y) << 16);
        RSDK.LoadScene();
        return true;
    }
    return false;
}

bool32 TMZ1Outro_Cutscene2_State1(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ1Outro);

    self->alpha += 4;
    RSDK.SetLimitedFade(0, 5, 4, self->alpha, 128, 256);
    if (self->alpha >= 0x100) {
        foreach_all(TitleCard, tCard)
        {
            tCard->active              = ACTIVE_NORMAL;
            tCard->state               = TitleCard_State_Initial;
            tCard->stateDraw           = TitleCard_Draw_Default;
            globals->suppressAutoMusic = false;
            Music_PlayTrack(TRACK_STAGE);
            foreach_break;
        }
        return true;
    }
    return false;
}

void TMZ1Outro_EditorDraw(void)
{
    RSDK_THIS(TMZ1Outro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void TMZ1Outro_EditorLoad(void) {}

void TMZ1Outro_Serialize(void) { RSDK_EDITABLE_VAR(TMZ1Outro, VAR_VECTOR2, size); }
