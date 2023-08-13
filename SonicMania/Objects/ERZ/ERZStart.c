// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZStart Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectERZStart *ERZStart;

void ERZStart_Update(void)
{
    RSDK_THIS(ERZStart);

    if (!self->activated) {
        foreach_active(Player, player)
        {
            if (!player->sidekick && Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                CutsceneSeq_StartSequence(self, ERZStart_Cutscene_FadeIn, ERZStart_Cutscene_ShrinkRubyWarpFX, ERZStart_Cutscene_EnterKing,
                                          ERZStart_Cutscene_KingMovingRuby, ERZStart_Cutscene_KingAttachHornRuby,
                                          ERZStart_Cutscene_SetupEggmanReveal, ERZStart_Cutscene_EnterEggman, ERZStart_Cutscene_EggmanKingWrestling,
                                          ERZStart_Cutscene_PostWrestleFadeIn, ERZStart_Cutscene_ReturnCamToSonic,
                                          ERZStart_Cutscene_PreparePlayerTransform, ERZStart_Cutscene_PlayerTransform, ERZStart_Cutscene_StartFight,
                                          ERZStart_Cutscene_Fight, StateMachine_None);

#if MANIA_USE_PLUS
                CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif

                self->activated = true;
            }
        }
    }

    // Have the timer go haywire
    if (--self->timer <= 0) {
        // update every 15, 30, 45 or 60 frames
        self->timer             = 15 * RSDK.Rand(0, 4);
        SceneInfo->seconds      = RSDK.Rand(0, 100);
        SceneInfo->milliseconds = RSDK.Rand(0, 100);
        SceneInfo->minutes      = RSDK.Rand(0, 9);
    }

#if !MANIA_USE_PLUS
    if (ERZStart->superDashCooldown > 0) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        HUD_EnableRingFlash();
        --ERZStart->superDashCooldown;
    }
#endif
}

void ERZStart_LateUpdate(void) {}

void ERZStart_StaticUpdate(void) {}

void ERZStart_Draw(void) {}

void ERZStart_Create(void *data)
{
    RSDK_THIS(ERZStart);

    if (!SceneInfo->inEditor) {
        INIT_ENTITY(self);
        CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
        self->active = ACTIVE_NORMAL;

        ERZStart_SetupObjects();
        ERZStart->timer = 0;
    }
}

void ERZStart_StageLoad(void)
{
    ERZStart->sfxClack     = RSDK.GetSfx("Stage/Clack.wav");
    ERZStart->sfxFlyIn     = RSDK.GetSfx("ERZ/FlyIn.wav");
    ERZStart->sfxHullClose = RSDK.GetSfx("Stage/HullClose.wav");
    ERZStart->sfxRumble    = RSDK.GetSfx("Stage/Rumble.wav");
}

void ERZStart_SetupObjects(void)
{
    foreach_all(PhantomRuby, ruby)
    {
        ERZStart->ruby = ruby;
        foreach_break;
    }

    foreach_all(FXRuby, fxRuby)
    {
        ERZStart->fxRuby    = fxRuby;
        fxRuby->state       = StateMachine_None;
        fxRuby->fadeBlack   = 0x200;
        fxRuby->fadeWhite   = 0x200;
        fxRuby->outerRadius = ScreenInfo->size.x;
        fxRuby->timer       = 64;
        foreach_break;
    }

    foreach_all(PhantomKing, king)
    {
        if (!king->type)
            ERZStart->king = king;
    }

    foreach_all(ChaosEmerald, emerald) { ERZStart->emeralds[emerald->type] = emerald; }

    foreach_all(KleptoMobile, eggman)
    {
        if (!eggman->type)
            ERZStart->eggman = eggman;
    }
}

void ERZStart_HandlePlayerHover(EntityPlayer *player, EntityCutsceneSeq *seq, int32 posY)
{
    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
    player->position.x = 0x300000;
    player->position.x += (player->position.x - player->position.x) >> 3;
    player->position.y += (posY + 0xA00 * RSDK.Sin256(2 * (seq->timer - seq->storedTimer + 64)) - player->position.y) >> 3;
    player->state = Player_State_Static;

    for (int32 e = 0; e < 7; ++e) {
        EntityChaosEmerald *emerald = ERZStart->emeralds[e];
        emerald->originPos          = player->position;
        emerald->active             = ACTIVE_NORMAL;
    }
}

bool32 ERZStart_Cutscene_FadeIn(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;
    EntityFXRuby *fxRuby    = ERZStart->fxRuby;

    if (!host->timer) {
        player1->position.y = ruby->position.y;
        player1->state      = Player_State_Static;
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        camera->position.x = 0;
        camera->position.y = 0;

        ScreenInfo->position.x = 0;
        ScreenInfo->position.y = 0;
        Zone->playerBoundsT[0] = 0;
        Zone->cameraBoundsT[0] = 0;
        camera->boundsT        = 0;
        Zone->playerBoundsB[0] = ScreenInfo->size.y;
        Zone->cameraBoundsB[0] = Zone->playerBoundsB[0];
        camera->boundsB        = Zone->cameraBoundsB[0];
        ruby->state            = ERZStart_RubyHover;

        int32 angle = 0;
        for (int32 i = 0; i < 7; ++i) {
            EntityChaosEmerald *emerald = ERZStart->emeralds[i];
            emerald->originPos.x        = player1->position.x;
            emerald->originPos.y        = player1->position.y;
            emerald->angle              = angle;
            emerald->radius             = 0x2000;
            emerald->groundVel          = 512;
            emerald->state              = ChaosEmerald_State_Rotate;

            angle += 0x2492;
        }

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundsR[0]      = 1552;
        Zone->cameraBoundsR[0]      = 1552;
    }

    if (host->timer >= 60) {
        if (fxRuby->fadeBlack > 0) {
            fxRuby->fadeBlack -= 16;
        }
        else {
            if (fxRuby->fadeWhite > 0) {
                fxRuby->fadeWhite -= 16;
            }
            else {
                PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
                return true;
            }
        }
    }

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);
    return false;
}

bool32 ERZStart_Cutscene_ShrinkRubyWarpFX(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;
    EntityFXRuby *fxRuby    = ERZStart->fxRuby;

    if (!host->timer)
        fxRuby->state = FXRuby_State_Shrinking;

    EntityPhantomKing *king = ERZStart->king;
    if (fxRuby->outerRadius <= 0) {
        ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);
        ruby->drawGroup = Zone->objectDrawGroup[0] + 1;
        king->state     = PhantomKing_State_SetupArms;
        return true;
    }

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);
    return false;
}

bool32 ERZStart_Cutscene_EnterKing(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    EntityPhantomKing *king = ERZStart->king;
    if (king->state == PhantomKing_State_TakeRubyAway) {
        ruby->state = ERZStart_RubyMove;
        return true;
    }

    return false;
}

bool32 ERZStart_Cutscene_KingMovingRuby(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    EntityPhantomKing *king = ERZStart->king;
    if (!host->timer) {
        camera->target              = NULL;
        player1->camera             = NULL;
        Zone->playerBoundActiveL[0] = false;
        Zone->playerBoundActiveR[0] = false;
        Zone->playerBoundActiveT[0] = false;
        host->storedTimer           = camera->position.x;
    }

    int32 pos = host->storedTimer;
    if (king->position.x - 0x400000 > pos)
        pos = king->position.x - 0x400000;

    camera->position.x = pos;

    if (king->finishedMovingRuby) {
        ruby->state = StateMachine_None;
        return true;
    }

    return false;
}

bool32 ERZStart_Cutscene_KingAttachHornRuby(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);
    EntityPhantomKing *king = ERZStart->king;

    if (!host->timer) {
        host->storedTimer = ruby->position.x;
        host->storedValue = ruby->position.y;
    }

    if (host->timer > 0) {
        if (host->timer == 60) {
            RSDK.PlaySfx(ERZStart->sfxClack, false, 255);

            king->drawRuby   = true;
            ruby->position.x = -0x400000;
            ruby->state      = StateMachine_None;
            return true;
        }

        int32 percent = 0x10000;
        if (host->timer << 16 < 0x3C0000)
            percent = (host->timer << 16) / 60;

        Vector2 pos = MathHelpers_GetBezierPoint(percent, host->storedTimer, host->storedValue, host->storedTimer, host->storedValue - 0x400000,
                                                 king->rubyPos.x, king->rubyPos.y - 0x400000, king->rubyPos.x, king->rubyPos.y);

        ruby->position.x += (pos.x - ruby->position.x) >> 2;
        ruby->position.y += (pos.y - ruby->position.y) >> 2;
    }

    return false;
}

bool32 ERZStart_Cutscene_SetupEggmanReveal(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    if (host->timer == 30)
        Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, camera->position.x - (ScreenInfo->size.x << 16), camera->position.y, 3);

    if (host->timer == 45)
        RSDK.PlaySfx(ERZStart->sfxFlyIn, false, 255);

    return host->timer == 120;
}

bool32 ERZStart_Cutscene_EnterEggman(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    EntityKleptoMobile *eggman = ERZStart->eggman;
    EntityPhantomKing *king    = ERZStart->king;

    uint16 eggmanSlot              = RSDK.GetEntitySlot(eggman);
    EntityKleptoMobile *eggmanHand = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanArmL = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanArmR = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot             = RSDK.GetEntitySlot(king);
    EntityPhantomKing *kingArmL = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingArmR = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    if (!host->timer) {
        host->storedTimer          = camera->position.x;
        host->storedValue          = camera->position.y;
        king->originPos.y          = king->position.y;
        king->originPos.x          = king->position.x;
        king->state                = PhantomKing_State_WrestleEggman;
        king->rotation             = -4;
        king->rubyAnimator.frameID = 0;
        king->drawGroup            = eggman->drawGroup + 1;
        king->rubyAnimator.speed   = 0;

        eggman->state = KleptoMobile_State_CutsceneControlled;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 11, &eggmanHand->handAnimator, true, 0);
        eggman->position.x  = camera->position.x - (ScreenInfo->size.x << 16);
        eggman->originPos.x = camera->position.x - (ScreenInfo->size.x << 16);
        eggman->position.y  = king->originPos.y + 0x100000;
        eggman->originPos.y = king->originPos.y + 0x100000;
        eggman->direction   = FLIP_X;
        eggman->velocity.x  = 0xD0000;
        eggman->velocity.y  = 0;
    }

    eggmanHand->state = KleptoMobile_StateHand_Boss;
    eggmanArmL->state = KleptoMobile_StateArm_Idle;
    eggmanArmR->state = KleptoMobile_StateArm_Idle;

    eggmanHand->position.x = eggman->originPos.x;
    eggmanHand->position.y = eggman->originPos.y;
    eggmanHand->position.x += 0x3C0000;
    eggmanHand->position.y -= 0x3E0000;
    eggmanHand->drawGroup = eggmanArmL->drawGroup + 1;

    eggmanArmL->position.x = eggman->originPos.x;
    eggmanArmL->position.x -= 0x40000;
    eggmanArmL->position.y = eggman->originPos.y;
    eggmanArmL->position.y -= 0x360000;
    eggmanArmL->armBezierPos.x          = eggman->originPos.x - 0x600000;
    eggmanArmL->armBezierPos.y          = eggman->originPos.y - 0x280000;
    eggmanArmL->finger1Animator.speed   = 0;
    eggmanArmL->finger2Animator.frameID = 0;
    eggmanArmL->finger2Animator.speed   = 0;
    eggmanArmL->finger1Animator.frameID = 0;

    eggmanArmR->position.x = eggman->originPos.x;
    eggmanArmR->position.x += 0x2E0000;
    eggmanArmR->position.y = eggman->originPos.y;
    eggmanArmR->position.y += 0x400000;
    eggmanArmR->armBezierPos.x          = eggman->originPos.x - 0x3A0000;
    eggmanArmR->armBezierPos.y          = eggman->originPos.y + 0x300000;
    eggmanArmR->finger1Animator.speed   = 0;
    eggmanArmR->finger2Animator.frameID = 0;
    eggmanArmR->finger2Animator.speed   = 0;
    eggmanArmR->finger1Animator.frameID = 0;

    camera->position.x = MAX(eggman->originPos.x + 0x200000, host->storedTimer);

    kingArmL->state = PhantomKing_StateArm_WrestleEggman;
    kingArmR->state = PhantomKing_StateArm_WrestleEggman;

    kingArmL->position.x           = king->originPos.x - 0x700000;
    kingArmL->position.y           = king->originPos.y - 0x1C0000;
    kingArmL->armBezierPos.x       = king->originPos.x + 0x100000;
    kingArmL->armBezierPos.y       = king->originPos.y - 0x2A0000;
    kingArmL->handAnimator.frameID = 1;
    kingArmL->drawGroup            = eggmanArmL->drawGroup + 1;
    kingArmL->handAnimator.speed   = 0;

    kingArmR->position.x           = king->originPos.x - 0x3E0000;
    kingArmR->position.y           = king->originPos.y + 0x520000;
    kingArmR->armBezierPos.x       = king->originPos.x + 0x380000;
    kingArmR->armBezierPos.y       = king->originPos.y + 0x380000;
    kingArmR->handAnimator.frameID = 1;
    kingArmR->drawGroup            = eggmanArmR->drawGroup + 1;
    kingArmR->handAnimator.speed   = 0;

    if (eggman->position.x >= king->originPos.x - 0x580000) {
        eggman->position.x = king->originPos.x - 0x580000;
        eggman->velocity.x = 0;
        eggman->velocity.y = 0;

        Camera_ShakeScreen(0, 5, 0);
        RSDK.PlaySfx(ERZStart->sfxHullClose, false, 255);
        return true;
    }

    return false;
}

bool32 ERZStart_Cutscene_EggmanKingWrestling(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;
    EntityFXRuby *fxRuby    = ERZStart->fxRuby;

    EntityKleptoMobile *eggman = ERZStart->eggman;
    EntityPhantomKing *king    = ERZStart->king;

    uint16 eggmanSlot               = RSDK.GetEntitySlot(eggman);
    EntityKleptoMobile *eggmanHand1 = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanHand2 = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot               = RSDK.GetEntitySlot(king);
    EntityPhantomKing *kingChild1 = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingChild2 = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    if (host->timer > 6 && !(host->timer % 6))
        Camera_ShakeScreen(0, 1, 0);

    if (!(host->timer % 37))
        RSDK.PlaySfx(ERZStart->sfxRumble, false, 255);

    if (host->timer == 120) {
        RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &king->rubyAnimator, true, 1);
        king->rubyAnimator.speed = 1;
    }

    if (host->timer == 172) {
        PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
        fxRuby->position.x = king->position.x;
        fxRuby->position.y = king->position.y;
        fxRuby->fadeWhite  = 0x300;

        RSDK.StopSfx(ERZStart->sfxRumble);
        return true;
    }
    else {
        eggmanHand1->position.x -= RSDK.Sin256(16 * host->timer) << 8;
        kingChild1->position.x -= RSDK.Sin256(16 * host->timer) << 8;
        eggmanHand2->position.x += RSDK.Sin256(16 * host->timer) << 8;
        kingChild2->position.x += RSDK.Sin256(16 * host->timer) << 8;
    }

    return false;
}

bool32 ERZStart_Cutscene_PostWrestleFadeIn(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;
    EntityFXRuby *fxRuby    = ERZStart->fxRuby;

    EntityKleptoMobile *eggman = ERZStart->eggman;
    EntityPhantomKing *king    = ERZStart->king;

    uint16 eggmanSlot              = RSDK.GetEntitySlot(eggman);
    EntityKleptoMobile *eggmanHand = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanArmL = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanArmR = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot             = RSDK.GetEntitySlot(king);
    EntityPhantomKing *kingArmL = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingArmR = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    if (!king->rubyAnimator.frameID)
        king->rubyAnimator.speed = 0;

    if (!host->timer) {
        eggman->position.x = 0x31C0000;
        eggman->position.y = -0x3C0000;
        eggman->state      = StateMachine_None;
        eggmanHand->state  = KleptoMobile_StateHand_Cutscene;
        eggmanArmL->state  = KleptoMobile_StateArm_Cutscene;
        eggmanArmR->state  = KleptoMobile_StateArm_Cutscene;

        eggmanHand->drawGroup = Zone->objectDrawGroup[0];
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 12, &eggmanHand->handAnimator, true, 5);

        kingArmL->state     = PhantomKing_StateArm_Idle;
        kingArmR->state     = PhantomKing_StateArm_Idle;
        king->drawGroup     = Zone->objectDrawGroup[0];
        kingArmL->drawGroup = Zone->objectDrawGroup[0];
        kingArmR->drawGroup = Zone->objectDrawGroup[0];
    }

    if (fxRuby->fadeWhite > 0)
        fxRuby->fadeWhite -= 8;

    return fxRuby->fadeWhite == 0;
}

bool32 ERZStart_Cutscene_ReturnCamToSonic(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    if (host->timer == 30)
        Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, ScreenInfo->center.x << 16, camera->position.y, 2);

    if (!host->values[0]) {
        if (camera->position.x == ScreenInfo->center.x << 16) {
            host->values[0]   = 1;
            host->storedTimer = host->timer;
        }
    }
    else if (host->timer - host->storedTimer == 60) {
        Music_TransitionTrack(TRACK_ERZBOSS, 0.0215);
        return true;
    }

    return false;
}

bool32 ERZStart_Cutscene_PreparePlayerTransform(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);

    if (++ERZStart->timer >= 60) {
        ERZStart->timer = 0;

        for (int32 i = 0; i < 7; ++i) {
            EntityChaosEmerald *emerald = ERZStart->emeralds[i];
            emerald->drawFX |= FX_SCALE;
            emerald->scale.x = 0x200;
            emerald->scale.y = 0x200;
        }

        return true;
    }

    return false;
}

bool32 ERZStart_Cutscene_PlayerTransform(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = ERZStart->ruby;

    if (++ERZStart->timer > 96) {
        ERZStart->timer = 0;
        return true;
    }
    else {
        for (int32 i = 0; i < 7; ++i) {
            EntityChaosEmerald *emerald = ERZStart->emeralds[i];
            emerald->drawGroup          = Zone->objectDrawGroup[0];
            emerald->radius -= 0x60;

            if (emerald->radius <= 0)
                emerald->visible = false;

            emerald->groundVel += 32;
            emerald->scale.x -= 3;
            emerald->scale.y -= 3;
        }

        if (ERZStart->timer >= 32) {
            if (ERZStart->timer == 32) {
                Player_GiveRings(player1, 50, false);
                Player_TryTransform(player1, 0x7F);
            }
        }
        else {
            ERZStart_HandlePlayerHover(player1, host, ruby->startPos.y);
        }
    }

    return false;
}

bool32 ERZStart_Cutscene_StartFight(EntityCutsceneSeq *host)
{
    RSDK_THIS(ERZStart);
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityPhantomKing *king = ERZStart->king;

    if (player1->animator.animationID != ANI_TRANSFORM) {
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveT[0] = true;
        Zone->cameraBoundsR[0]      = 0x610;
        Zone->cameraBoundsT[0]      = 0;

        player1->state            = ERZStart_State_PlayerSuperFly;
        player1->abilityValues[0] = 60;
        player1->velocity.x       = 0x160000;
        player1->onGround         = false;
        player1->stateAbility     = ERZStart_Player_StartSuperFly;
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 255);

        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 64;
        player1->stateInput  = Player_Input_P1;

        if (player1->characterID == ID_KNUCKLES) {
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_GLIDE, &player1->animator, false, 6);
            player1->animator.rotationStyle = ROTSTYLE_FULL;
        }
        else {
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_RUN, &player1->animator, false, 0);
        }

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveT[0] = true;

        PhantomKing_SetupKing(king);

        Camera_SetTargetEntity(0, player1);
        camera->target   = (Entity *)player1;
        camera->offset.x = 0;
        camera->offset.y = 0;
        camera->state    = Camera_State_FollowXY;
        player1->camera  = camera;

        foreach_all(RingField, field) { field->running = true; }

        SceneInfo->timeEnabled = true;
        return true;
    }

    return false;
}

bool32 ERZStart_Cutscene_Fight(EntityCutsceneSeq *host)
{
    EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityKleptoMobile *eggman = ERZStart->eggman;
    EntityPhantomKing *king    = ERZStart->king;

    if (player1->superState == SUPERSTATE_SUPER) {
        if (!king->health && !eggman->health)
            return true;
    }
    else if (player1->deathType != PLAYER_DEATH_DIE_USESFX) {
        player1->deathType = PLAYER_DEATH_DIE_USESFX;
    }

    return false;
}

void ERZStart_RubyHover(void)
{
    EntityPhantomRuby *ruby = ERZStart->ruby;
    EntityFXRuby *fxRuby    = ERZStart->fxRuby;

    ruby->angle += 2;
    ruby->position.y = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;

    fxRuby->position.x = ruby->position.x;
    fxRuby->position.y = ruby->position.y;
}

void ERZStart_RubyMove(void)
{
    RSDK_THIS(ERZStart);
    EntityPhantomRuby *ruby = ERZStart->ruby;

    if (self->velocity.x < 0x40000)
        self->velocity.x += 0x1800;

    ruby->angle += 2;
    ruby->position.x += ruby->velocity.x;
    ruby->position.y = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;
}

void ERZStart_Player_HandleSuperDash(EntityPlayer *player)
{
    RSDK_THIS(Player);

    uint8 dashDir = 0;
    if (player->up)
        dashDir |= 1;

    if (player->down)
        dashDir |= 2;

    if (player->left)
        dashDir |= 4;

    if (player->right)
        dashDir |= 8;

    int32 dashAngle    = 0;
    uint8 dashAngles[] = { 0x00, 0xC0, 0x40, 0x00, 0x80, 0xA0, 0x60, 0x00, 0x00, 0xE0, 0x20 };

    if (dashDir == 0 && player->direction)
        dashAngle = 0x80;
    else
        dashAngle = dashAngles[dashDir];

    player->rings -= 5;
    if (player->rings < 0)
        player->rings = 0;

    player->velocity.x = 0x1600 * RSDK.Cos256(dashAngle);
    player->velocity.y = 0x1600 * RSDK.Sin256(dashAngle);

    RSDK.PlaySfx(ItemBox->sfxHyperRing, false, 0xFF);
    RSDK.PlaySfx(Player->sfxPeelRelease, false, 0xFF);

    if (player->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_GLIDE, &player->animator, false, 6);
        player->animator.rotationStyle = ROTSTYLE_FULL;
    }
    else {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 0);
    }

    player->state            = ERZStart_State_PlayerSuperFly;
    player->abilityValues[0] = 60;

    if (FXFade) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 64;
    }

#if MANIA_USE_PLUS
    Player->superDashCooldown = 30;
#else
    ERZStart->superDashCooldown = 30;
#endif
}

void ERZStart_State_PlayerSuperFly(void)
{
    RSDK_THIS(Player);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->onGround) {
        player1->state = Player_State_Ground;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, false, 0);
    }
    else {
        if (player1->up) {
            if (self->velocity.y > -player1->topSpeed) {
                self->velocity.y -= player1->acceleration;

                if (player1->velocity.y > 0)
                    self->velocity.y -= player1->acceleration;
            }
        }
        else if (player1->down) {
            if (self->velocity.y < player1->topSpeed) {
                self->velocity.y += player1->acceleration;

                if (player1->velocity.y < 0)
                    self->velocity.y += player1->acceleration;
            }
        }
        else {
            if (player1->velocity.y) {
                if (player1->velocity.y < 0) {
                    player1->velocity.y += player1->deceleration;

                    if (player1->velocity.y > 0)
                        player1->velocity.y = 0;
                }
                else {
                    player1->velocity.y -= player1->deceleration;

                    if (player1->velocity.y < 0)
                        player1->velocity.y = 0;
                }
            }
        }

        int32 velY = player1->velocity.y;
        if (velY >= -player1->topSpeed) {
            if (velY > player1->topSpeed)
                player1->velocity.y = velY - (player1->acceleration >> 1) - player1->acceleration;
        }
        else {
            player1->velocity.y = player1->acceleration + velY + (player1->acceleration >> 1);
        }

        if (player1->left) {
            if (player1->characterID == ID_KNUCKLES && self->direction == FLIP_NONE)
                RSDK.SetSpriteAnimation(player1->aniFrames, ANI_GLIDE, &player1->animator, true, 0);

            if (self->velocity.x > -player1->topSpeed) {
                self->velocity.x -= player1->acceleration;

                if (player1->velocity.x > 0)
                    self->velocity.x -= player1->acceleration;
            }

            player1->direction = FLIP_X;
        }
        else if (player1->right) {
            if (player1->characterID == ID_KNUCKLES && self->direction == FLIP_X)
                RSDK.SetSpriteAnimation(player1->aniFrames, ANI_GLIDE, &player1->animator, true, 0);

            if (self->velocity.x < player1->topSpeed) {
                self->velocity.x += player1->acceleration;

                if (player1->velocity.x < 0)
                    self->velocity.x += player1->acceleration;
            }

            player1->direction = FLIP_NONE;
        }
        else {
            if (player1->velocity.x) {
                if (player1->velocity.x < 0) {
                    player1->velocity.x += player1->deceleration;

                    if (player1->velocity.x > 0)
                        player1->velocity.x = 0;
                }
                else {
                    player1->velocity.x -= player1->deceleration;

                    if (player1->velocity.x < 0)
                        player1->velocity.x = 0;
                }
            }
        }

        int32 velX = player1->velocity.x;
        if (velX >= -player1->topSpeed) {
            if (velX > player1->topSpeed)
                player1->velocity.x = velX - (player1->acceleration >> 1) - player1->acceleration;
        }
        else {
            player1->velocity.x = player1->acceleration + velX + (player1->acceleration >> 1);
        }

        if (player1->abilityValues[0]) {
            player1->abilityValues[0]--;
        }
        else if (player1->jumpPress) {
            if (player1->rings > 10)
                ERZStart_Player_HandleSuperDash(player1);
        }
    }
}

void ERZStart_Player_StartSuperFly(void)
{
    RSDK_THIS(Player);

    if (self->jumpAbilityState == 1 && self->jumpPress) {
        if (self->characterID == ID_KNUCKLES) {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_GLIDE, &self->animator, false, 6);
            self->animator.rotationStyle = ROTSTYLE_FULL;
        }
        else {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_RUN, &self->animator, false, 0);
        }

        self->state = ERZStart_State_PlayerSuperFly;
    }
}

void ERZStart_State_PlayerRebound(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (entity->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(entity->aniFrames, ANI_GLIDE, &entity->animator, false, 6);
        entity->animator.rotationStyle = ROTSTYLE_FULL;
    }
    else {
        RSDK.SetSpriteAnimation(entity->aniFrames, ANI_RUN, &entity->animator, false, 0);
    }

    entity->rotation += 0x20;
    entity->onGround = false;

    if (entity->rotation == 0x600) {
        entity->rotation = 0;
        entity->state    = ERZStart_State_PlayerSuperFly;
    }
}

#if GAME_INCLUDE_EDITOR
void ERZStart_EditorDraw(void)
{
    RSDK_THIS(ERZStart);

    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void ERZStart_EditorLoad(void) {}
#endif

void ERZStart_Serialize(void) { RSDK_EDITABLE_VAR(ERZOutro, VAR_VECTOR2, size); }
