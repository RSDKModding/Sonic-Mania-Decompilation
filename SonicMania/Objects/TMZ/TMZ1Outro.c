// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZ1Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZ1Outro *TMZ1Outro;

void TMZ1Outro_Update(void)
{
    RSDK_THIS(TMZ1Outro);

    if (Zone->actID) {
        CutsceneSeq_StartSequence(self, TMZ1Outro_CutsceneAct2_BeginAct2, StateMachine_None);
        RSDK.SetLimitedFade(0, 5, 4, self->alpha, 128, 256);
    }
    else {
        CutsceneSeq_StartSequence(self, TMZ1Outro_CutsceneAct1_SetupPlayers, TMZ1Outro_CutsceneAct1_ElevatorRide,
                                  TMZ1Outro_CutsceneAct1_HeadForEntrance, TMZ1Outro_CutsceneAct1_EnterMonarch, StateMachine_None);
    }

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN, StateMachine_None);
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

bool32 TMZ1Outro_CutsceneAct1_SetupPlayers(EntityCutsceneSeq *host)
{
    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state      = Player_State_Ground;
        player->stateInput = StateMachine_None;
    }

    return true;
}

bool32 TMZ1Outro_CutsceneAct1_ElevatorRide(EntityCutsceneSeq *host)
{
    if (host->timer == 120) {
        CrimsonEye_SetArrowDir(CE_ARROW_UP);
        RSDK.PlaySfx(CrimsonEye->sfxBeep, false, 255);
    }

    if (host->timer == 160) {
        Camera_ShakeScreen(0, 0, 6);
        CrimsonEye->targetElevatorSpeed    = -0x10000;
        TileLayer *moveLayer               = RSDK.GetTileLayer(Zone->moveLayer);
        moveLayer->drawGroup[0]            = 0;
        moveLayer->scrollPos               = 0x5000000;
        moveLayer->scrollInfo[0].scrollPos = -0x4D00000;
        RSDK.PlaySfx(CrimsonEye->sfxHullClose, false, 255);
    }

    if (host->timer > 160 && !(host->timer & 0x3F))
        RSDK.PlaySfx(CrimsonEye->sfxElevator, false, 255);

    if (host->timer > 200) {
        if (CrimsonEye->targetElevatorSpeed) {
            if (RSDK.GetTileLayer(Zone->moveLayer)->scrollPos < 0x2300000)
                CrimsonEye->targetElevatorSpeed = 0;
        }
        else if (!CrimsonEye->elevatorSpeed) {
            Camera_ShakeScreen(0, 0, 6);
            RSDK.PlaySfx(CrimsonEye->sfxHullClose, false, 0xFF);
            return true;
        }
    }

    foreach_active(ItemBox, itembox)
    {
        // NOTE:
        // broken monitors are carried up due to the differences how they work compared to unbroken ones
        // unbroken monitors do not do any collision/gravity checks, so they're moved offscreen as expected
        // however, unbroken monitors DO do gravity & collision, so the elevator collision overrides the code here and allows em to be carried into
        // act 2
        itembox->position.y -= CrimsonEye->targetElevatorSpeed;
    }

    foreach_active(SignPost, signPost) { signPost->position.y -= CrimsonEye->targetElevatorSpeed; }

    return false;
}

bool32 TMZ1Outro_CutsceneAct1_HeadForEntrance(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ1Outro);

    if (host->timer >= 30) {
        bool32 finishedMoving = true;

        foreach_active(Player, player)
        {
            if (abs(self->position.x - player->position.x) >= 0x100000) {
                if (player->position.x > self->position.x) {
                    player->left   = true;
                    finishedMoving = false;
                }

                if (player->position.x < self->position.x) {
                    player->right  = true;
                    finishedMoving = false;
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
                finishedMoving = false;
        }

        return finishedMoving;
    }
    return false;
}

bool32 TMZ1Outro_CutsceneAct1_EnterMonarch(EntityCutsceneSeq *host)
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

        Zone_StoreEntities((ScreenInfo->position.x + ScreenInfo->center.x) << 16, (ScreenInfo->size.y + ScreenInfo->position.y) << 16);
        RSDK.LoadScene();

        return true;
    }

    return false;
}

bool32 TMZ1Outro_CutsceneAct2_BeginAct2(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ1Outro);

    self->alpha += 4;
    RSDK.SetLimitedFade(0, 5, 4, self->alpha, 128, 256);

    if (self->alpha >= 0x100) {
        foreach_all(TitleCard, titleCard)
        {
            titleCard->active    = ACTIVE_NORMAL;
            titleCard->state     = TitleCard_State_SetupBGElements;
            titleCard->stateDraw = TitleCard_Draw_SlideIn;

            globals->suppressAutoMusic = false;
            Music_PlayTrack(TRACK_STAGE);
            foreach_break;
        }

        return true;
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void TMZ1Outro_EditorDraw(void)
{
    RSDK_THIS(TMZ1Outro);

    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void TMZ1Outro_EditorLoad(void) {}
#endif

void TMZ1Outro_Serialize(void) { RSDK_EDITABLE_VAR(TMZ1Outro, VAR_VECTOR2, size); }
