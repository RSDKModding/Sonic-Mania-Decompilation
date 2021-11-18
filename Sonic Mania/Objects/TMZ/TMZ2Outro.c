#include "SonicMania.h"

ObjectTMZ2Outro *TMZ2Outro;

void TMZ2Outro_Update(void)
{
    RSDK_THIS(TMZ2Outro);

    TMZ2Outro_SetupCutscene();
    self->active = ACTIVE_NEVER;
    foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
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
    TMZ2Outro->sfxAlarm   = RSDK.GetSFX("TMZ3/Alarm.wav");
    TMZ2Outro->sfxCarRev  = RSDK.GetSFX("TMZ1/CarRev.wav");
    TMZ2Outro->sfxGrab    = RSDK.GetSFX("Global/Grab.wav");
    TMZ2Outro->sfxRumble  = RSDK.GetSFX("Stage/Rumble.wav");
    TMZ2Outro->sfxDrop    = RSDK.GetSFX("Stage/Drop.wav");
    TMZ2Outro->sfxImpact  = RSDK.GetSFX("Stage/Impact2.wav");
    TMZ2Outro->sfxRubyGet = RSDK.GetSFX("TMZ3/RubyGet.wav");
    TMZ2Outro->playerID   = ID_NONE;
}

void TMZ2Outro_SetupCutscene(void)
{
    RSDK_THIS(TMZ2Outro);

#if RETRO_USE_PLUS
    if (globals->gameMode != MODE_ENCORE) {
#endif
        void *states[] = { TMZ2Outro_CutsceneState_SetupOutro,     TMZ2Outro_CutsceneState_WatchEggman,
                           TMZ2Outro_CutsceneState_EggmanFall,     TMZ2Outro_CutsceneState_StartAlert,
                           TMZ2Outro_CutsceneState_TimeToEscape,   TMZ2Outro_CutsceneState_HurryToCar,
                           TMZ2Outro_CutsceneState_StartFadeOut,   TMZ2Outro_CutsceneState_FadeOut,
                           TMZ2Outro_CutsceneState_FinishSequence, NULL };

        CutsceneSeq_StartSequence((Entity*)self, states);
#if RETRO_USE_PLUS
    }
    else {
        if (SaveGame->saveRAM->chaosEmeralds == 0x7F) {
            void *states[] = { TMZ2Outro_CutsceneState_SetupOutro,       TMZ2Outro_CutsceneState_WatchEggman,    TMZ2Outro_CutsceneState_EggmanFall,
                               TMZ2Outro_CutsceneState_StartRubyRampage, TMZ2Outro_CutsceneState_OuttaHere,      TMZ2Outro_CutsceneState_TeamEscape,
                               TMZ2Outro_CutsceneState_FadeOut,          TMZ2Outro_CutsceneState_FinishSequence, NULL };
            CutsceneSeq_StartSequence((Entity *)self, states);
        }
        else {
            void *states[] = {
                TMZ2Outro_CutsceneState_SetupOutro,       TMZ2Outro_CutsceneState_WatchEggman,    TMZ2Outro_CutsceneState_EggmanFall,
                TMZ2Outro_CutsceneState_StartRubyRampage, TMZ2Outro_CutsceneState_Panic,          TMZ2Outro_CutsceneState_OuttaHere_BadEnd,
                TMZ2Outro_CutsceneState_FadeOut,          TMZ2Outro_CutsceneState_FinishSequence, NULL
            };

            self->heavyKing = CutsceneHBH_GetEntity(HBH_KINGTMZ2);
            CutsceneSeq_StartSequence((Entity *)self, states);
        }
        foreach_all(TMZFlames, flames) { destroyEntity(flames); }
    }
#endif
}

bool32 TMZ2Outro_CutsceneState_SetupOutro(EntityCutsceneSeq *host)
{
    TMZ2Outro->playerID = globals->playerID;

    foreach_active(Player, player)
    {
        player->stateInput = 0;
        if (player->superState == 2)
            player->superState = 3;

        CutsceneSeq_LockAllPlayerControl();
    }

    foreach_all(BoundsMarker, marker) { destroyEntity(marker); }

    for (int i = 0; i < 0x100; ++i) {
        colour clr = RSDK.GetPaletteEntry(1, i);
        RSDK.SetPaletteEntry(7, i, clr & 0xFF0000);
    }

    return true;
}

bool32 TMZ2Outro_CutsceneState_WatchEggman(EntityCutsceneSeq *host)
{
    bool32 flag = true;
    foreach_active(Player, player)
    {
        if (player->sidekick && player->characterID == ID_KNUCKLES && !checkPlayerID(ID_KNUCKLES, 1)) {
            if (player->stateInput != TMZ2Outro_PlayerStateInput_Escape) {
                Zone->playerBoundActiveR[0] = false;
                player->stateInput          = TMZ2Outro_PlayerStateInput_Escape;
            }
        }
        else {
            int pos = PhantomEgg->boundsM + (player->playerID << 21);
            if (player->position.x >= pos + 0x300000) {
                if (player->position.x <= pos + 0x700000) {
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
            flag = false;
        }
    }

    if (flag) {
        foreach_active(Player, player2)
        {
            if (player2->sidekick && (player2->characterID != ID_KNUCKLES || checkPlayerID(ID_KNUCKLES, 2)))
                player2->stateInput = Player_ProcessP2Input_AI;
        }
    }
    return flag;
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
    if (self->position.x >= 0xAC80000) {
        self->visible        = false;
        self->interaction    = false;
        self->tileCollisions = false;
        self->right          = false;
    }
}

bool32 TMZ2Outro_CutsceneState_EggmanFall(EntityCutsceneSeq *host)
{
    bool32 flag = false;
    foreach_active(Eggman, eggman)
    {
        if (eggman->onGround) {
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 8, &eggman->animator, true, 0);
            foreach_active(Player, player) { player->up = false; }
            flag = true;
            foreach_break;
        }
    }
    return flag;
}

bool32 TMZ2Outro_CutsceneState_StartAlert(EntityCutsceneSeq *host)
{
    if (host->timer == 120) {
        RSDK.PlaySfx(TMZ2Outro->sfxAlarm, true, 255);
        foreach_active(TMZAlert, alert)
        {
            alert->alpha = 16 * RSDK.Rand(-32, -8);
            alert->state = TMZAlert_Unknown1;
        }

        return true;
    }
    return false;
}
bool32 TMZ2Outro_CutsceneState_TimeToEscape(EntityCutsceneSeq *host)
{
    RSDK.SetLimitedFade(0, 7, 1, (RSDK.Cos256(4 * host->timer) >> 1) + 128, 0, 256);

    if (!(host->timer & 7))
        Camera_ShakeScreen(0, 0, 4);

    if (host->timer == 128) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player1->right        = true;
        Zone->screenBoundsR1[0] += 832;
        Zone->playerBoundActiveR[0] = false;
        return true;
    }
    return false;
}

bool32 TMZ2Outro_CutsceneState_HurryToCar(EntityCutsceneSeq *host)
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
        Zone->screenBoundsR1[0] = 0x2000;
        EntityCamera *camera    = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->position.x      = (camera->boundsR - ScreenInfo->centerX) << 16;
        camera->boundsR         = 0x2000;
        camera->state           = StateMachine_None;
        self->velocity.x      = 0;
        return true;
    }
    return false;
}
bool32 TMZ2Outro_CutsceneState_StartFadeOut(EntityCutsceneSeq *host)
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
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 32;
        fxFade->wait         = 90;
        fxFade->speedOut     = 16;
        fxFade->fadeOutBlack = true;
        RSDK.PlaySfx(PhantomEgg->sfxExplosion3, false, 255);
        return true;
    }
    return false;
}

#if RETRO_USE_PLUS
bool32 TMZ2Outro_CutsceneState_StartRubyRampage(EntityCutsceneSeq *host)
{
    if (host->timer == 60) {
        foreach_active(PhantomRuby, ruby)
        {
            int pos = ScreenInfo->position.x;
            if (SaveGame->saveRAM->chaosEmeralds == 0x7F)
                pos += 64;
            else
                pos += 96;
            ruby->startPos.x  = pos << 16;
            ruby->startPos.y  = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
            ruby->velocity.y  = 0;
            ruby->isPermanent = true;
            ruby->drawOrder   = Zone->drawOrderHigh;
            ruby->active      = ACTIVE_NORMAL;
            ruby->state       = PhantomRuby_Unknown9;
        }
    }

    if (host->timer > 60) {
        foreach_active(Player, player)
        {
            if (player->position.x >= (ScreenInfo->centerX - 64 + ScreenInfo->position.x) << 16) {
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
            alert->state = TMZAlert_Unknown1;
        }

        for (int i = 0; i < 7; ++i) RSDK.SetPaletteEntry(2, i - 96, TMZ2Outro->colours[i]);
        if (SaveGame->saveRAM->chaosEmeralds == 0x7F)
            CREATE_ENTITY(RubyPortal, RubyPortal_Unknown8, (ScreenInfo->position.x + 64) << 16,
                          (ScreenInfo->position.y + ScreenInfo->centerY) << 16);
        return true;
    }
    return false;
}
bool32 TMZ2Outro_CutsceneState_Panic(EntityCutsceneSeq *host)
{
    if (host->timer == 90) {
        foreach_active(Eggman, eggman)
        {
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 9, &eggman->animator, true, 0);
            eggman->direction  = FLIP_NONE;
            eggman->state      = Eggman_Unknown7;
            eggman->velocity.x = 0x10000;
            RSDK.ObjectTileGrip(eggman, Zone->fgLayers, CMODE_FLOOR, eggman->collisionPlane, 0, 0x1C0000, 14);
        }
    }

    if (host->timer < 120) {
        foreach_active(Player, player)
        {
            if (player->position.x >= (ScreenInfo->centerX - 64 + ScreenInfo->position.x) << 16) {
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
            player->state = Player_State_None;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_BALANCE1 + player->playerID, &player->animator, false, 0);
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
bool32 TMZ2Outro_CutsceneState_OuttaHere_BadEnd(EntityCutsceneSeq *host)
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
            RSDK.SetSpriteAnimation(self->heavyKing->aniFrames, 3, &self->heavyKing->animator, false, 0);
            foreach_active(PhantomRuby, ruby)
            {
                ruby->startPos.y -= 0x100000;
                ruby->state = PhantomRuby_Unknown9;
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
            RSDK.SetSpriteAnimation(self->heavyKing->aniFrames, 4, &self->heavyKing->animator, false, 0);
            foreach_active(PhantomRuby, ruby)
            {
                ruby->startPos.y += 0x180000;
                ruby->state = PhantomRuby_Unknown9;
            }
        }
    }

    if (host->timer == 420) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 32;
        fxFade->wait         = 90;
        fxFade->speedOut     = 16;
        fxFade->fadeOutBlack = true;
        fxFade->isPermanent  = true;
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
        return true;
    }
    return false;
}
bool32 TMZ2Outro_CutsceneState_OuttaHere(EntityCutsceneSeq *host)
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

            Zone->screenBoundsR1[0]     = 0x2000;
            Zone->playerBoundActiveR[0] = false;
        }
        else if (host->timer == 290) {
            foreach_active(Player, player)
            {
                player->left      = false;
                player->right     = true;
                player->groundVel = 0x40000;
                player->topSpeed  = 0x50000;
                player->drawOrder = Zone->playerDrawHigh;
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (player->position.x >= (ScreenInfo->centerX - 64 + ScreenInfo->position.x) << 16) {
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
            eggman->state      = Eggman_Unknown7;
            eggman->velocity.x = 0x10000;
            RSDK.ObjectTileGrip(eggman, Zone->fgLayers, CMODE_FLOOR, eggman->collisionPlane, 0, 0x1C0000, 14);
        }
    }

    if (host->timer == 300) {
        foreach_active(PhantomRuby, ruby)
        {
            ruby->velocity.x  = 0x20000;
            ruby->isPermanent = true;
            ruby->active      = ACTIVE_NORMAL;
        }
        RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->state = StateMachine_None;
        return true;
    }
    return false;
}
bool32 TMZ2Outro_CutsceneState_TeamEscape(EntityCutsceneSeq *host)
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

        EntityPlayer *player3 = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
        RSDK.CopyEntity(player3, player1, false);
        player3->playerID = 3;
        Player_ChangeCharacter(player3, ID_KNUCKLES);

        EntityPlayer *player4 = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        RSDK.CopyEntity(player4, player1, false);
        player4->playerID = 4;
        Player_ChangeCharacter(player4, ID_MIGHTY);

        EntityPlayer *player5 = RSDK_GET_ENTITY(5, Player);
        RSDK.CopyEntity(player5, player1, false);
        player5->playerID = 5;
        Player_ChangeCharacter(player5, ID_RAY);

        SceneInfo->timeEnabled = true;

        Player_CheckGoSuper(player1, 0xFF);
        EntitySuperSparkle *sparkle = RSDK_GET_ENTITY(Player->playerCount, SuperSparkle);
        RSDK.ResetEntityPtr(sparkle, SuperSparkle->objectID, player1);

        Player->playerCount = 6;
        int offsetX         = 0;
        for (int i = 0; i < Player->playerCount; ++i) {
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

        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
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

bool32 TMZ2Outro_CutsceneState_FadeOut(EntityCutsceneSeq *host)
{
    RSDK.StopSFX(TMZ2Outro->sfxAlarm);
    return host->timer == 180;
}
bool32 TMZ2Outro_CutsceneState_FinishSequence(EntityCutsceneSeq *host)
{
    bool32 goodEndingFlag = false;
#if RETRO_USE_PLUS
    if (!(SceneInfo->filter & FILTER_ENCORE))
#endif
        goodEndingFlag =
            (checkPlayerID(ID_SONIC, 1) || (checkPlayerID(ID_KNUCKLES, 1) && checkPlayerID(ID_KNUCKLES, 2))) && SaveGame->saveRAM->chaosEmeralds == 0x7F;

    bool32 saveFlag = false;
    if (!goodEndingFlag)
        saveFlag = globals->saveSlotID != NO_SAVE_SLOT;

    if (saveFlag) {
        if (!host->timer) {
            globals->playerID = TMZ2Outro->playerID;
            TMZ2Outro->flag1  = false;
            SaveGame_SaveProgress();
            if (Zone_IsAct2())
                GameProgress_MarkZoneCompleted(Zone_GetZoneID());
            GameProgress_GiveEnding(1);
            SaveGame_SaveFile(TMZ2Outro_SaveFileCB);
            UIWaitSpinner_Wait();
        }

        if (!TMZ2Outro->flag1)
            return false;
        else
            UIWaitSpinner_Wait2();
    }

    if (goodEndingFlag) {
        ++SceneInfo->listPos;
        RSDK.LoadScene();
        return true;
    }
    else {
        globals->playerID = TMZ2Outro->playerID;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE) {
            RSDK.SetScene("Presentation", "Credits");
        }
        else {
#endif
            switch (globals->playerID & 0xFF) {
                case ID_TAILS: RSDK.SetScene("Videos", "Bad End - Tails"); break;
                case ID_KNUCKLES:
                    if (checkPlayerID(ID_KNUCKLES, 2))
                        RSDK.SetScene("Videos", "True End?");
                    else
                        RSDK.SetScene("Videos", "Bad End - Knux");
                    break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: RSDK.SetScene("Videos", "Bad End - Mighty"); break;
                case ID_RAY: RSDK.SetScene("Videos", "Bad End - Ray"); break;
#endif
                default: RSDK.SetScene("Videos", "Bad End - Sonic"); break;
            }
#if RETRO_USE_PLUS
        }
#endif
        API_UnlockAchievement("ACH_GAME_CLEARED");
        RSDK.LoadScene();
        return true;
    }
    return false;
}

void TMZ2Outro_SaveFileCB(int32 status) { TMZ2Outro->flag1 = true; }

void TMZ2Outro_EditorDraw(void) {}

void TMZ2Outro_EditorLoad(void) {}

void TMZ2Outro_Serialize(void) {}
