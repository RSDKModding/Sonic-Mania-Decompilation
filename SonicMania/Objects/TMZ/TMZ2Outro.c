// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZ2Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZ2Outro *TMZ2Outro;

void TMZ2Outro_Update(void)
{
    RSDK_THIS(TMZ2Outro);

    TMZ2Outro_SetupCutscene();
    self->active = ACTIVE_NEVER;

    HUD_MoveOut();
}

void TMZ2Outro_LateUpdate(void) {}

void TMZ2Outro_StaticUpdate(void) {}

void TMZ2Outro_Draw(void) {}

void TMZ2Outro_Create(void *data)
{
    RSDK_THIS(TMZ2Outro);

    self->active = ACTIVE_NORMAL;
    foreach_all(EscapeCar, car) { self->escapeCar = car; }
}

void TMZ2Outro_StageLoad(void)
{
    TMZ2Outro->sfxAlarm   = RSDK.GetSfx("TMZ3/Alarm.wav");
    TMZ2Outro->sfxCarRev  = RSDK.GetSfx("TMZ1/CarRev.wav");
    TMZ2Outro->sfxGrab    = RSDK.GetSfx("Global/Grab.wav");
    TMZ2Outro->sfxRumble  = RSDK.GetSfx("Stage/Rumble.wav");
    TMZ2Outro->sfxDrop    = RSDK.GetSfx("Stage/Drop.wav");
    TMZ2Outro->sfxImpact  = RSDK.GetSfx("Stage/Impact2.wav");
    TMZ2Outro->sfxRubyGet = RSDK.GetSfx("TMZ3/RubyGet.wav");

    TMZ2Outro->playerID = ID_NONE;
}

void TMZ2Outro_SetupCutscene(void)
{
    RSDK_THIS(TMZ2Outro);

#if MANIA_USE_PLUS
    if (globals->gameMode != MODE_ENCORE) {
#endif
        CutsceneSeq_StartSequence(self, TMZ2Outro_Cutscene_SetupOutro, TMZ2Outro_Cutscene_WatchEggman, TMZ2Outro_Cutscene_EggmanFall,
                                  TMZ2Outro_Cutscene_StartAlert, TMZ2Outro_Cutscene_TimeToEscape, TMZ2Outro_Cutscene_HurryToCar,
                                  TMZ2Outro_Cutscene_StartFadeOut, TMZ2Outro_Cutscene_FadeOut, TMZ2Outro_Cutscene_FinishSequence, StateMachine_None);
#if MANIA_USE_PLUS
    }
    else {
        if (SaveGame_AllChaosEmeralds()) {
            CutsceneSeq_StartSequence(self, TMZ2Outro_Cutscene_SetupOutro, TMZ2Outro_Cutscene_WatchEggman, TMZ2Outro_Cutscene_EggmanFall,
                                      TMZ2Outro_Cutscene_StartRubyRampage, TMZ2Outro_Cutscene_OuttaHere, TMZ2Outro_Cutscene_TeamEscape,
                                      TMZ2Outro_Cutscene_FadeOut, TMZ2Outro_Cutscene_FinishSequence, StateMachine_None);
        }
        else {
            self->heavyKing = CutsceneHBH_GetEntity(HBH_KINGTMZ2);
            CutsceneSeq_StartSequence(self, TMZ2Outro_Cutscene_SetupOutro, TMZ2Outro_Cutscene_WatchEggman, TMZ2Outro_Cutscene_EggmanFall,
                                      TMZ2Outro_Cutscene_StartRubyRampage, TMZ2Outro_Cutscene_Panic, TMZ2Outro_Cutscene_OuttaHere_BadEnd,
                                      TMZ2Outro_Cutscene_FadeOut, TMZ2Outro_Cutscene_FinishSequence, StateMachine_None);
        }
        foreach_all(TMZFlames, flames) { destroyEntity(flames); }
    }
#endif

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif
}

bool32 TMZ2Outro_Cutscene_SetupOutro(EntityCutsceneSeq *host)
{
    TMZ2Outro->playerID = globals->playerID;

    foreach_active(Player, player)
    {
        player->stateInput = StateMachine_None;
        if (player->superState == SUPERSTATE_SUPER)
            player->superState = SUPERSTATE_FADEOUT;

        CutsceneSeq_LockAllPlayerControl();
    }

    foreach_all(BoundsMarker, marker) { destroyEntity(marker); }

    for (int32 c = 0; c < 0x100; ++c) RSDK.SetPaletteEntry(7, c, RSDK.GetPaletteEntry(1, c) & 0xFF0000);

    return true;
}

bool32 TMZ2Outro_Cutscene_WatchEggman(EntityCutsceneSeq *host)
{
    bool32 finishedMoving = true;

    foreach_active(Player, player)
    {
        if (player->sidekick && player->characterID == ID_KNUCKLES && !CHECK_CHARACTER_ID(ID_KNUCKLES, 1)) {
            if (player->stateInput != TMZ2Outro_PlayerStateInput_Escape) {
                Zone->playerBoundActiveR[0] = false;
                player->stateInput          = TMZ2Outro_PlayerStateInput_Escape;
            }
        }
        else {
            int32 targetPos = PhantomEgg->boundsM + (player->playerID << 21);

            if (player->position.x >= targetPos + 0x300000) {
                if (player->position.x <= targetPos + 0x700000) {
                    player->left  = false;
                    player->right = false;

                    if (abs(player->groundVel) < 0x8000) {
                        player->groundVel = 0;
                        player->direction = FLIP_X;
                        player->up        = true;
                        continue;
                    }
                    else {
                        player->groundVel -= (player->groundVel >> 5);
                    }
                }
                else {
                    player->left  = true;
                    player->right = false;
                }
            }
            else {
                player->left  = false;
                player->right = true;
            }

            finishedMoving = false;
        }
    }

    if (finishedMoving) {
        foreach_active(Player, player2)
        {
            if (player2->sidekick && (player2->characterID != ID_KNUCKLES || CHECK_CHARACTER_ID(ID_KNUCKLES, 2)))
                player2->stateInput = Player_Input_P2_AI;
        }
    }

    return finishedMoving;
}

void TMZ2Outro_PlayerStateInput_Escape(void)
{
    RSDK_THIS(Player);

    self->up        = false;
    self->down      = false;
    self->left      = false;
    self->right     = true;
    self->jumpPress = false;
    self->jumpHold  = false;

    if (self->position.x >= (2760 << 16)) {
        self->visible        = false;
        self->interaction    = false;
        self->tileCollisions = TILECOLLISION_NONE;
        self->right          = false;
    }
}

bool32 TMZ2Outro_Cutscene_EggmanFall(EntityCutsceneSeq *host)
{
    bool32 eggmanOnGround = false;

    foreach_active(Eggman, eggman)
    {
        if (eggman->onGround) {
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 8, &eggman->animator, true, 0);
            foreach_active(Player, player) { player->up = false; }

            eggmanOnGround = true;
            foreach_break;
        }
    }

    return eggmanOnGround;
}

bool32 TMZ2Outro_Cutscene_StartAlert(EntityCutsceneSeq *host)
{
    if (host->timer == 120) {
        RSDK.PlaySfx(TMZ2Outro->sfxAlarm, true, 255);

        foreach_active(TMZAlert, alert)
        {
            alert->alpha = 16 * RSDK.Rand(-32, -8);
            alert->state = TMZAlert_State_Activating;
        }

        return true;
    }

    return false;
}
bool32 TMZ2Outro_Cutscene_TimeToEscape(EntityCutsceneSeq *host)
{
    RSDK.SetLimitedFade(0, 7, 1, (RSDK.Cos256(4 * host->timer) >> 1) + 128, 0, 256);

    if (!(host->timer & 7))
        Camera_ShakeScreen(0, 0, 4);

    if (host->timer == 128) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player1->right        = true;

        Zone->cameraBoundsR[0] += 832;
        Zone->playerBoundActiveR[0] = false;

        return true;
    }

    return false;
}

bool32 TMZ2Outro_Cutscene_HurryToCar(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ2Outro);

    RSDK.SetLimitedFade(0, 7, 1, (RSDK.Cos256(4 * host->timer) >> 1) + 128, 0, 256);

    if (!(host->timer & 7))
        Camera_ShakeScreen(0, 0, 4);

    EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityEscapeCar *escapeCar = self->escapeCar;

    if (escapeCar->position.x - player1->position.x < 0x1800000)
        player1->right = false;

    if (escapeCar->position.x - player1->position.x < 0xC00000) {
        player1->jumpPress = player1->onGround;
        player1->jumpHold  = true;
    }

    if (host->timer == 248)
        RSDK.PlaySfx(TMZ2Outro->sfxGrab, false, 255);

    if (host->timer == 384) {
        Zone->cameraBoundsR[0] = 0x2000;

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->position.x   = (camera->boundsR - ScreenInfo->center.x) << 16;
        camera->boundsR      = 0x2000;
        camera->state        = StateMachine_None;

        self->velocity.x = 0;
        return true;
    }

    return false;
}
bool32 TMZ2Outro_Cutscene_StartFadeOut(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ2Outro);

    RSDK.SetLimitedFade(0, 7, 1, (RSDK.Cos256(4 * host->timer) >> 1) + 128, 0, 256);

    if (host->timer == 15)
        RSDK.PlaySfx(TMZ2Outro->sfxCarRev, false, 255);

    if (!(host->timer & 7))
        Camera_ShakeScreen(0, 0, 4);

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    if (self->velocity.x < 0x170000)
        self->velocity.x += 0x2000;

    camera->position.x += self->velocity.x;

    if (host->timer == 200) {
        Music_FadeOut(0.0125);
        RSDK.PlaySfx(PhantomEgg->sfxExplosion3, false, 255);
    }

    if (host->timer == 260)
        RSDK.PlaySfx(PhantomEgg->sfxExplosion3, false, 255);

    if (host->timer == 320) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 32;
        fxFade->wait         = 90;
        fxFade->speedOut     = 16;
        fxFade->fadeOutBlack = true;

        RSDK.PlaySfx(PhantomEgg->sfxExplosion3, false, 255);

        return true;
    }

    return false;
}

#if MANIA_USE_PLUS
bool32 TMZ2Outro_Cutscene_StartRubyRampage(EntityCutsceneSeq *host)
{
    if (host->timer == 60) {
        foreach_active(PhantomRuby, ruby)
        {
            int32 pos         = ScreenInfo->position.x + (SaveGame_AllChaosEmeralds() ? 64 : 96);
            ruby->startPos.x  = pos << 16;
            ruby->startPos.y  = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
            ruby->velocity.y  = 0;
            ruby->isPermanent = true;
            ruby->drawGroup   = Zone->objectDrawGroup[1];
            ruby->active      = ACTIVE_NORMAL;
            ruby->state       = PhantomRuby_State_MoveToPos;
        }
    }

    if (host->timer > 60) {
        foreach_active(Player, player)
        {
            if (player->position.x >= (ScreenInfo->center.x - 64 + ScreenInfo->position.x) << 16) {
                player->left  = true;
                player->right = false;

                if (player->groundVel < -0x20000)
                    player->groundVel = -0x20000;
            }
            else {
                player->up        = true;
                player->left      = false;
                player->right     = false;
                player->groundVel = 0;
            }
        }
    }

    if (host->timer == 120) {
        foreach_active(TMZAlert, alert)
        {
            alert->alpha = 16 * RSDK.Rand(-32, -8);
            alert->state = TMZAlert_State_Activating;
        }

        for (int32 i = 0; i < 7; ++i) RSDK.SetPaletteEntry(2, i - 96, TMZ2Outro->colors[i]);

        if (SaveGame_AllChaosEmeralds()) {
            CREATE_ENTITY(RubyPortal, RubyPortal_State_EncoreEnd, (ScreenInfo->position.x + 64) << 16,
                          (ScreenInfo->position.y + ScreenInfo->center.y) << 16);
        }

        return true;
    }

    return false;
}
bool32 TMZ2Outro_Cutscene_Panic(EntityCutsceneSeq *host)
{
    if (host->timer == 90) {
        foreach_active(Eggman, eggman)
        {
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 9, &eggman->animator, true, 0);
            eggman->direction  = FLIP_NONE;
            eggman->state      = Eggman_State_WalkOffScreen;
            eggman->velocity.x = 0x10000;
            RSDK.ObjectTileGrip(eggman, Zone->collisionLayers, CMODE_FLOOR, eggman->collisionPlane, 0, 0x1C0000, 14);
        }
    }

    if (host->timer < 120) {
        foreach_active(Player, player)
        {
            if (player->position.x >= (ScreenInfo->center.x - 64 + ScreenInfo->position.x) << 16) {
                player->left  = true;
                player->right = false;

                if (player->groundVel < -0x20000)
                    player->groundVel = -0x20000;
            }
            else {
                player->up        = true;
                player->left      = false;
                player->right     = false;
                player->groundVel = 0;
            }
        }
    }
    else if (host->timer == 256) {
        foreach_active(Player, player)
        {
            player->up    = false;
            player->state = Player_State_Static;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_BALANCE_1 + player->playerID, &player->animator, false, 0);
        }

        RSDK.PlaySfx(TMZ2Outro->sfxRumble, false, 255);
        RSDK.SetChannelAttributes(RSDK.PlaySfx(TMZ2Outro->sfxCarRev, false, 255), 1.0, 0.75, 1.0);
        Camera_ShakeScreen(0, 0, 4);
        Zone->playerBoundActiveR[0] = false;
    }
    else {
        if (host->timer == 290) {
            foreach_active(Player, player)
            {
                player->state = Player_State_Ground;
                player->left  = false;
                player->right = true;
            }

            return true;
        }
    }

    return false;
}
bool32 TMZ2Outro_Cutscene_OuttaHere_BadEnd(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ2Outro);

    if (host->timer == 120) {
        Music_FadeOut(0.0125);

        foreach_active(Player, player) { player->active = ACTIVE_NEVER; }

        CutsceneHBH_KingTMZ2Setup();

        if (self->heavyKing) {
            self->heavyKing->visible        = true;
            self->heavyKing->collisionPlane = 1;
            RSDK.PlaySfx(TMZ2Outro->sfxDrop, false, 255);
        }
    }

    if (host->timer == 166)
        RSDK.PlaySfx(TMZ2Outro->sfxImpact, false, 255);

    if (host->timer == 240) {
        if (self->heavyKing) {
            RSDK.SetSpriteAnimation(self->heavyKing->aniFrames, 3, &self->heavyKing->mainAnimator, false, 0);

            foreach_active(PhantomRuby, ruby)
            {
                ruby->startPos.y -= 0x100000;
                ruby->state = PhantomRuby_State_MoveToPos;
            }
        }
    }

    if (host->timer == 256) {
        foreach_active(PhantomRuby, ruby)
        {
            EntityFXSpinRay *spinRay = CREATE_ENTITY(FXSpinRay, NULL, ruby->position.x, ruby->position.y);
            spinRay->parent          = (Entity *)ruby;
            spinRay->r               = 240;
            spinRay->g               = 0;
            spinRay->b               = 240;

            RSDK.PlaySfx(TMZ2Outro->sfxRubyGet, false, 255);
        }
    }

    if (host->timer == 408) {
        if (self->heavyKing) {
            RSDK.SetSpriteAnimation(self->heavyKing->aniFrames, 4, &self->heavyKing->mainAnimator, false, 0);

            foreach_active(PhantomRuby, ruby)
            {
                ruby->startPos.y += 0x180000;
                ruby->state = PhantomRuby_State_MoveToPos;
            }
        }
    }

    if (host->timer == 420) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 32;
        fxFade->wait         = 90;
        fxFade->speedOut     = 16;
        fxFade->fadeOutBlack = true;
        fxFade->isPermanent  = true;

        PhantomRuby_PlaySfx(RUBYSFX_ATTACK1);
        return true;
    }

    return false;
}
bool32 TMZ2Outro_Cutscene_OuttaHere(EntityCutsceneSeq *host)
{
    if (host->timer >= 120) {
        if (host->timer == 240) {
            foreach_active(Player, player)
            {
                player->up         = false;
                player->onGround   = false;
                player->velocity.x = 0x20000;
                player->velocity.y = -0x40000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
            }

            Zone->cameraBoundsR[0]      = 0x2000;
            Zone->playerBoundActiveR[0] = false;
        }
        else if (host->timer == 290) {
            foreach_active(Player, player)
            {
                player->left      = false;
                player->right     = true;
                player->groundVel = 0x40000;
                player->topSpeed  = 0x50000;
                player->drawGroup = Zone->playerDrawGroup[1];
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (player->position.x >= (ScreenInfo->center.x - 64 + ScreenInfo->position.x) << 16) {
                player->left  = true;
                player->right = false;

                if (player->groundVel < -0x20000)
                    player->groundVel = -0x20000;
            }
            else {
                player->up        = true;
                player->left      = false;
                player->right     = false;
                player->groundVel = 0;
            }
        }
    }

    if (host->timer == 90) {
        foreach_active(Eggman, eggman)
        {
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 9, &eggman->animator, true, 0);

            eggman->direction  = FLIP_NONE;
            eggman->state      = Eggman_State_WalkOffScreen;
            eggman->velocity.x = 0x10000;

            RSDK.ObjectTileGrip(eggman, Zone->collisionLayers, CMODE_FLOOR, eggman->collisionPlane, 0, 0x1C0000, 14);
        }
    }

    if (host->timer == 300) {
        foreach_active(RubyPortal, portal)
        {
            portal->velocity.x  = 0x20000;
            portal->isPermanent = true;
            portal->active      = ACTIVE_NORMAL;
        }

        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->state = StateMachine_None;

        return true;
    }

    return false;
}
bool32 TMZ2Outro_Cutscene_TeamEscape(EntityCutsceneSeq *host)
{
    RSDK_THIS(TMZ2Outro);

    if (!(Zone->timer & 7))
        Camera_ShakeScreen(0, 0, 4);

    if (!(Zone->timer & 0x1F))
        RSDK.PlaySfx(TMZ2Outro->sfxRumble, false, 255);

    foreach_active(RubyPortal, portal)
    {
        if (portal->velocity.x < 0x71000)
            portal->velocity.x += 0x2000;
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (host->timer >= 400)
        camera->position.x += 0x50000;
    else
        camera->position.x += 0x70000;

    if (host->timer == 120)
        RSDK.PlaySfx(TMZ2Outro->sfxCarRev, false, 255);

    if (host->timer == 320) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        Player_ChangeCharacter(player1, ID_SONIC);

        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        RSDK.CopyEntity(player2, player1, false);
        player2->playerID = 1;
        Player_ChangeCharacter(player2, ID_TAILS);

        EntityPlayer *player3 = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        RSDK.CopyEntity(player3, player1, false);
        player3->playerID = 3;
        Player_ChangeCharacter(player3, ID_KNUCKLES);

        EntityPlayer *player4 = RSDK_GET_ENTITY(SLOT_PLAYER5, Player);
        RSDK.CopyEntity(player4, player1, false);
        player4->playerID = 4;
        Player_ChangeCharacter(player4, ID_MIGHTY);

        EntityPlayer *player5 = RSDK_GET_ENTITY(SLOT_PLAYER6, Player);
        RSDK.CopyEntity(player5, player1, false);
        player5->playerID = 5;
        Player_ChangeCharacter(player5, ID_RAY);

        SceneInfo->timeEnabled = true;

        Player_TryTransform(player1, 0xFF);
        EntitySuperSparkle *sparkle = RSDK_GET_ENTITY(Player->playerCount, SuperSparkle);
        RSDK.ResetEntity(sparkle, SuperSparkle->classID, player1);

        Player->playerCount = 6;
        int32 offsetX       = 0;
        for (int32 i = 0; i < Player->playerCount; ++i) {
            if (i != 2) {
                EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
                player->groundVel    = 0x100000;
                player->topSpeed     = 0x100000;
                player->rings        = 100;
                if (i)
                    player->sidekick = true;
                player->position.x += offsetX;
                offsetX -= 0x200000;
                player->stateInput = StateMachine_None;
            }
        }

        Zone->deathBoundary[0] = 0x8000000;
        RSDK.PlaySfx(Player->sfxTransform2, false, 255);
        Music_FadeOut(0.0125);
    }

    if (host->timer == 420)
        RSDK.PlaySfx(TMZ2Outro->sfxDrop, false, 255);

    if (host->timer == 512) {

        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 32;
        fxFade->wait         = 90;
        fxFade->speedOut     = 16;
        fxFade->fadeOutBlack = 1;
        fxFade->isPermanent  = true;

        RSDK.SetChannelAttributes(RSDK.PlaySfx(PhantomRuby->sfxL[0], false, 0), 1.0, -1.0, 1.0);
        RSDK.SetChannelAttributes(RSDK.PlaySfx(PhantomRuby->sfxR[0], false, 0), 1.0, 1.0, 1.0);
        return true;
    }

    return false;
}
#endif

bool32 TMZ2Outro_Cutscene_FadeOut(EntityCutsceneSeq *host)
{
    RSDK.StopSfx(TMZ2Outro->sfxAlarm);

    return host->timer == 180;
}
bool32 TMZ2Outro_Cutscene_FinishSequence(EntityCutsceneSeq *host)
{
    bool32 hasGoodEnding = false;

#if MANIA_USE_PLUS
    if (!(SceneInfo->filter & FILTER_ENCORE))
#endif
        hasGoodEnding = (CHECK_CHARACTER_ID(ID_SONIC, 1) || (CHECK_CHARACTER_ID(ID_KNUCKLES, 1) && CHECK_CHARACTER_ID(ID_KNUCKLES, 2)))
                        && SaveGame_AllChaosEmeralds();

    bool32 isSaveSlot = false;
    if (!hasGoodEnding)
        isSaveSlot = globals->saveSlotID != NO_SAVE_SLOT;

    if (isSaveSlot) {
        if (!host->timer) {
            globals->playerID         = TMZ2Outro->playerID;
            TMZ2Outro->finishedSaving = false;

            SaveGame_SaveProgress();
            if (Zone_IsZoneLastAct())
                GameProgress_MarkZoneCompleted(Zone_GetZoneID());

            GameProgress_GiveEnding(1);
            SaveGame_SaveFile(TMZ2Outro_SaveFileCB);

            UIWaitSpinner_StartWait();
        }

        if (!TMZ2Outro->finishedSaving)
            return false;
        else
            UIWaitSpinner_FinishWait();
    }

    if (hasGoodEnding) {
        ++SceneInfo->listPos;
        RSDK.LoadScene();
        return true;
    }
    else {
        globals->playerID = TMZ2Outro->playerID;
#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_ENCORE) {
            RSDK.SetScene("Presentation", "Credits");
        }
        else {
#endif
            switch (GET_CHARACTER_ID(1)) {
                default:
                case ID_SONIC: RSDK.SetScene("Videos", "Bad End - Sonic"); break;

                case ID_TAILS: RSDK.SetScene("Videos", "Bad End - Tails"); break;

                case ID_KNUCKLES:
                    if (CHECK_CHARACTER_ID(ID_KNUCKLES, 2))
                        RSDK.SetScene("Videos", "True End?");
                    else
                        RSDK.SetScene("Videos", "Bad End - Knux");
                    break;

#if MANIA_USE_PLUS
                case ID_MIGHTY: RSDK.SetScene("Videos", "Bad End - Mighty"); break;

                case ID_RAY: RSDK.SetScene("Videos", "Bad End - Ray"); break;
#endif
            }
#if MANIA_USE_PLUS
        }
#endif

        API_UnlockAchievement(&achievementList[ACH_GAME_CLEARED]);
        RSDK.LoadScene();

        return true;
    }

    return false;
}

#if MANIA_USE_PLUS
void TMZ2Outro_SaveFileCB(bool32 success) { TMZ2Outro->finishedSaving = true; }
#else
void TMZ2Outro_SaveFileCB(void) { TMZ2Outro->finishedSaving = true; }
#endif

#if GAME_INCLUDE_EDITOR
void TMZ2Outro_EditorDraw(void) {}

void TMZ2Outro_EditorLoad(void) {}
#endif

void TMZ2Outro_Serialize(void) {}
