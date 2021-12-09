#include "SonicMania.h"

ObjectERZStart *ERZStart;

void ERZStart_Update(void)
{
    RSDK_THIS(ERZStart);

    void *states[] = { ERZStart_CutsceneState_Unknown1,   ERZStart_CutsceneState_Unknown2,  ERZStart_CutsceneState_Unknown3,
                       ERZStart_CutsceneState_Unknown4,   ERZStart_CutsceneState_Unknown5,  ERZStart_CutsceneState_Unknown6,
                       ERZStart_CutsceneState_Unknown7,   ERZStart_CutsceneState_Unknown8,  ERZStart_CutsceneState_Unknown9,
                       ERZStart_CutsceneState_Unknown10,  ERZStart_CutsceneState_Unknown11, ERZStart_CutsceneState_PlayerTransform,
                       ERZStart_CutsceneState_StartFight, ERZStart_CutsceneState_Fight,     NULL };

    if (!self->activated) {
        foreach_active(Player, player)
        {
            if (!player->sidekick && Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                CutsceneSeq_StartSequence((Entity *)self, states);
                self->activated = true;
            }
        }
    }

    if (--self->timer <= 0) {
        self->timer                = 15 * RSDK.Rand(0, 4);
        SceneInfo->seconds      = RSDK.Rand(0, 100);
        SceneInfo->milliseconds = RSDK.Rand(0, 100);
        SceneInfo->minutes      = RSDK.Rand(0, 9);
    }

#if !RETRO_USE_PLUS
    if (ERZStart->superDashCooldown > 0) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        foreach_all(HUD, hud)
        {
            if (hud)
                hud->enableRingFlash = true;
            foreach_break;
        }
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
        ERZStart->ruby = (Entity *)ruby;
        foreach_break;
    }

    foreach_all(FXRuby, fxRuby)
    {
        ERZStart->fxRuby    = (Entity *)fxRuby;
        fxRuby->state       = StateMachine_None;
        fxRuby->fadeBlack   = 0x200;
        fxRuby->fadeWhite   = 0x200;
        fxRuby->outerRadius = ScreenInfo->width;
        fxRuby->timer    = 64;
        foreach_break;
    }

    foreach_all(PhantomKing, king)
    {
        if (!king->type)
            ERZStart->king = (Entity *)king;
    }

    foreach_all(ChaosEmerald, emerald) { ERZStart->emeralds[emerald->type] = emerald; }

    foreach_all(KleptoMobile, eggman)
    {
        if (!eggman->type)
            ERZStart->eggman = (Entity *)eggman;
    }
}

void ERZStart_HandlePlayerHover(EntityCutsceneSeq *seq, void *p, int posY)
{
    EntityPlayer *player = (EntityPlayer *)p;

    player->position.x = 0x300000;
    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
    player->position.x += (player->position.x - player->position.x) >> 3;
    player->position.y += (posY + 0xA00 * RSDK.Sin256(2 * (seq->timer - seq->storedValue2 + 64)) - player->position.y) >> 3;
    player->state = Player_State_None;

    for (int e = 0; e < 7; ++e) {
        EntityChaosEmerald *emerald = ERZStart->emeralds[e];
        emerald->originPos          = player->position;
        emerald->active             = ACTIVE_NORMAL;
    }
}

bool32 ERZStart_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZStart->fxRuby;

    if (!host->timer) {
        player1->position.y = ruby->position.y;
        player1->state      = Player_State_None;
        player1->stateInput = 0;
        CutsceneSeq_LockAllPlayerControl();
        camera->position.x = 0;
        camera->position.y = 0;

        ScreenInfo->position.x = 0;
        ScreenInfo->position.y = 0;
        Zone->playerBoundsT[0]  = 0;
        Zone->cameraBoundsT[0]  = 0;
        camera->boundsT          = 0;
        Zone->playerBoundsB[0]  = ScreenInfo->height;
        Zone->cameraBoundsB[0]  = Zone->playerBoundsB[0];
        camera->boundsB          = Zone->cameraBoundsB[0];
        ruby->state              = ERZStart_RubyHover;

        int angle = 0;
        for (int i = 0; i < 7; ++i) {
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
        Zone->playerBoundsR[0]     = 1552;
        Zone->cameraBoundsR[0]     = 1552;
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
                PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
                return true;
            }
        }
    }
    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);
    return false;
}

bool32 ERZStart_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZStart->fxRuby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    if (!host->timer)
        fxRuby->state = FXRuby_State_ShrinkRing;

    EntityPhantomKing *king = (EntityPhantomKing *)ERZStart->king;
    if (fxRuby->outerRadius <= 0) {
        ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);
        ruby->drawOrder = Zone->drawOrderLow + 1;
        king->state     = PhantomKing_State_Unknown2;
        return true;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    EntityPhantomKing *king = (EntityPhantomKing *)ERZStart->king;
    if (king->state == PhantomKing_State_Unknown5) {
        ruby->state = ERZStart_RubyMove;
        return true;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    EntityPhantomKing *king = (EntityPhantomKing *)ERZStart->king;
    if (!host->timer) {
        camera->targetPtr           = NULL;
        player1->camera             = NULL;
        Zone->playerBoundActiveL[0] = false;
        Zone->playerBoundActiveR[0] = false;
        Zone->playerBoundActiveT[0] = false;
        host->storedValue2              = camera->position.x;
    }

    int pos = host->storedValue2;
    if (king->position.x - 0x400000 > pos)
        pos = king->position.x - 0x400000;
    camera->position.x = pos;
    if (king->field_224) {
        ruby->state = StateMachine_None;
        return true;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Unknown5(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);
    EntityPhantomKing *king = (EntityPhantomKing *)ERZStart->king;

    if (!host->timer) {
        host->storedValue2 = ruby->position.x;
        host->storedValue1 = ruby->position.y;
    }

    if (host->timer > 0) {
        if (host->timer == 60) {
            RSDK.PlaySfx(ERZStart->sfxClack, false, 255);
            king->drawRuby   = true;
            ruby->position.x = -0x400000;
            ruby->state      = StateMachine_None;
            return true;
        }

        int percent = 0x10000;
        if (host->timer << 16 < 0x3C0000)
            percent = (host->timer << 16) / 60;

        Vector2 pos = MathHelpers_GetBezierPoint(percent, host->storedValue2, host->storedValue1, host->storedValue2, host->storedValue1 - 0x400000, king->rubyPos.x,
                                           king->rubyPos.y - 0x400000, king->rubyPos.x, king->rubyPos.y);

        ruby->position.x += (pos.x - ruby->position.x) >> 2;
        ruby->position.y += (pos.y - ruby->position.y) >> 2;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Unknown6(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    if (host->timer == 30)
        Camera_SetupLerp(0, 0, camera->position.x - (ScreenInfo->width << 16), camera->position.y, 3);
    if (host->timer == 45)
        RSDK.PlaySfx(ERZStart->sfxFlyIn, false, 255);
    return host->timer == 120;
}

bool32 ERZStart_CutsceneState_Unknown7(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZStart->eggman;
    EntityPhantomKing *king    = (EntityPhantomKing *)ERZStart->king;

    uint16 eggmanSlot              = RSDK.GetEntityID(eggman);
    EntityKleptoMobile *eggmanHand = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanArm1 = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanArm2 = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot             = RSDK.GetEntityID(king);
    EntityPhantomKing *kingArm1 = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingArm2 = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    if (!host->timer) {
        host->storedValue2                    = camera->position.x;
        host->storedValue1                    = camera->position.y;
        king->posUnknown.y                = king->position.y;
        king->posUnknown.x                = king->position.x;
        king->state                       = PhantomKing_State_Unknown7;
        king->rotation                    = -4;
        king->animatorRuby.frameID        = 0;
        king->drawOrder                   = eggman->drawOrder + 1;
        king->animatorRuby.speed = 0;

        eggman->state = KleptoMobile_State_Unknown1;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 11, &eggmanHand->animator8, true, 0);
        eggman->position.x = camera->position.x - (ScreenInfo->width << 16);
        eggman->field_70.x = camera->position.x - (ScreenInfo->width << 16);
        eggman->position.y = king->posUnknown.y + 0x100000;
        eggman->field_70.y = king->posUnknown.y + 0x100000;
        eggman->direction  = FLIP_X;
        eggman->velocity.x = 0xD0000;
        eggman->velocity.y = 0;
    }

    eggmanHand->state = KleptoMobile_State3_Unknown2;
    eggmanArm1->state = KleptoMobile_State1_Unknown2;
    eggmanArm2->state = KleptoMobile_State1_Unknown2;

    eggmanHand->position.x = eggman->field_70.x;
    eggmanHand->position.y = eggman->field_70.y;
    eggmanHand->position.x += 0x3C0000;
    eggmanHand->position.y -= 0x3E0000;
    eggmanHand->drawOrder = eggmanArm1->drawOrder + 1;

    eggmanArm1->position.x = eggman->field_70.x;
    eggmanArm1->position.x -= 0x40000;
    eggmanArm1->position.y = eggman->field_70.y;
    eggmanArm1->position.y -= 0x360000;
    eggmanArm1->field_F0.x                = eggman->field_70.x - 0x600000;
    eggmanArm1->field_F0.y                = eggman->field_70.y - 0x280000;
    eggmanArm1->animator9.speed  = 0;
    eggmanArm1->animator10.frameID        = 0;
    eggmanArm1->animator10.speed = 0;
    eggmanArm1->animator9.frameID         = 0;

    eggmanArm2->position.x = eggman->field_70.x;
    eggmanArm2->position.x += 0x2E0000;
    eggmanArm2->position.y = eggman->field_70.y;
    eggmanArm2->position.y += 0x400000;
    eggmanArm2->field_F0.x                = eggman->field_70.x - 0x3A0000;
    eggmanArm2->field_F0.y                = eggman->field_70.y + 0x300000;
    eggmanArm2->animator9.speed  = 0;
    eggmanArm2->animator10.frameID        = 0;
    eggmanArm2->animator10.speed = 0;
    eggmanArm2->animator9.frameID         = 0;

    camera->position.x = maxVal(eggman->field_70.x + 0x200000, host->storedValue2);

    kingArm1->state = PhantomKing_StateArm1_Unknown3;
    kingArm2->state = PhantomKing_StateArm1_Unknown3;

    kingArm1->position.x                = king->posUnknown.x - 0x700000;
    kingArm1->position.y                = king->posUnknown.y - 0x1C0000;
    kingArm1->field_F8                  = king->posUnknown.x + 0x100000;
    kingArm1->field_FC                  = king->posUnknown.y - 0x2A0000;
    kingArm1->animator10.frameID        = 1;
    kingArm1->drawOrder                 = eggmanArm1->drawOrder + 1;
    kingArm1->animator10.speed = 0;

    kingArm2->position.x                = king->posUnknown.x - 0x3E0000;
    kingArm2->position.y                = king->posUnknown.y + 0x520000;
    kingArm2->field_F8                  = king->posUnknown.x + 0x380000;
    kingArm2->field_FC                  = king->posUnknown.y + 0x380000;
    kingArm2->animator10.frameID        = 1;
    kingArm2->drawOrder                 = eggmanArm2->drawOrder + 1;
    kingArm2->animator10.speed = 0;

    if (eggman->position.x >= king->posUnknown.x - 0x580000) {
        eggman->position.x = king->posUnknown.x - 0x580000;
        eggman->velocity.x = 0;
        eggman->velocity.y = 0;
        Camera_ShakeScreen(0, 5, 0);
        RSDK.PlaySfx(ERZStart->sfxHullClose, false, 255);
        return true;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Unknown8(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZStart->fxRuby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZStart->eggman;
    EntityPhantomKing *king    = (EntityPhantomKing *)ERZStart->king;

    uint16 eggmanSlot               = RSDK.GetEntityID(eggman);
    EntityKleptoMobile *eggmanHand1 = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanHand2 = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot               = RSDK.GetEntityID(king);
    EntityPhantomKing *kingChild1 = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingChild2 = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    if (host->timer > 6 && !(host->timer % 6))
        Camera_ShakeScreen(0, 1, 0);
    if (!(host->timer % 37))
        RSDK.PlaySfx(ERZStart->sfxRumble, false, 255);
    if (host->timer == 120) {
        RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &king->animatorRuby, true, 1);
        king->animatorRuby.speed = 1;
    }

    if (host->timer == 172) {
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
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

bool32 ERZStart_CutsceneState_Unknown9(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZStart->fxRuby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZStart->eggman;
    EntityPhantomKing *king    = (EntityPhantomKing *)ERZStart->king;

    uint16 eggmanSlot              = RSDK.GetEntityID(eggman);
    EntityKleptoMobile *eggmanHand = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanArm1 = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanArm2 = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot             = RSDK.GetEntityID(king);
    EntityPhantomKing *kingArm1 = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingArm2 = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    if (!king->animatorRuby.frameID)
        king->animatorRuby.speed = 0;
    if (!host->timer) {
        eggman->position.x = 0x31C0000;
        eggman->position.y = -0x3C0000;
        eggman->state      = StateMachine_None;
        eggmanHand->state  = KleptoMobile_State3_Unknown1;
        eggmanArm1->state  = KleptoMobile_State1_Unknown1;
        eggmanArm2->state  = KleptoMobile_State1_Unknown1;

        eggmanHand->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 12, &eggmanHand->animator8, true, 5);

        kingArm1->state     = PhantomKing_StateArm1_Unknown2;
        kingArm2->state     = PhantomKing_StateArm1_Unknown2;
        king->drawOrder     = Zone->drawOrderLow;
        kingArm1->drawOrder = Zone->drawOrderLow;
        kingArm2->drawOrder = Zone->drawOrderLow;
    }

    if (fxRuby->fadeWhite > 0)
        fxRuby->fadeWhite -= 8;
    return fxRuby->fadeWhite == 0;
}

bool32 ERZStart_CutsceneState_Unknown10(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);
    if (host->timer == 30)
        Camera_SetupLerp(0, 0, ScreenInfo->centerX << 16, camera->position.y, 2);
    if (!host->values[0]) {
        if (camera->position.x == ScreenInfo->centerX << 16) {
            host->values[0] = 1;
            host->storedValue2    = host->timer;
        }
    }
    else if (host->timer - host->storedValue2 == 60) {
        Music_TransitionTrack(TRACK_ERZBOSS, 0.0215);
        return true;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Unknown11(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);
    if (++ERZStart->timer >= 60) {
        ERZStart->timer = false;

        for (int i = 0; i < 7; ++i) {
            EntityChaosEmerald *emerald = ERZStart->emeralds[i];
            emerald->drawFX |= FX_SCALE;
            emerald->scale.x = 0x200;
            emerald->scale.y = 0x200;
        }
        return true;
    }
    return false;
}

bool32 ERZStart_CutsceneState_PlayerTransform(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    if (++ERZStart->timer > 96) {
        ERZStart->timer = false;
        return true;
    }
    else {
        for (int i = 0; i < 7; ++i) {
            EntityChaosEmerald *emerald = ERZStart->emeralds[i];
            emerald->drawOrder          = Zone->drawOrderLow;
            emerald->radius -= 96;
            if (emerald->radius <= 0)
                emerald->visible = false;
            emerald->groundVel += 32;
            emerald->scale.x -= 3;
            emerald->scale.y -= 3;
        }

        if (ERZStart->timer >= 32) {
            if (ERZStart->timer == 32) {
                Player_GiveRings(50, player1, false);
                Player_CheckGoSuper(player1, 0x7F);
            }
        }
        else {
            ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);
        }
    }
    return false;
}

bool32 ERZStart_CutsceneState_StartFight(EntityCutsceneSeq *host)
{
    RSDK_THIS(ERZStart);
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);

    EntityPhantomKing *king = (EntityPhantomKing *)ERZStart->king;
    if (player1->animator.animationID != ANI_TRANSFORM) {
        Zone->cameraBoundsR[0]     = 0x610;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsT[0]     = 0;
        Zone->playerBoundActiveT[0] = true;
        player1->state              = ERZStart_State_PlayerSuperFly;
        player1->abilityValues[0]   = 60;
        player1->velocity.x         = 0x160000;
        player1->onGround           = false;
        player1->stateAbility       = ERZStart_Player_StartSuperFly;
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 255);
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 64;
        player1->stateInput  = Player_ProcessP1Input;
        if (player1->characterID == ID_KNUCKLES) {
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_FLY, &player1->animator, false, 6);
            player1->animator.rotationFlag = 1;
        }
        else {
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_RUN, &player1->animator, false, 0);
        }
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveT[0] = true;
        PhantomKing_SetupKing(king);
        Camera_SetTargetEntity(0, player1);
        camera->targetPtr = (Entity *)player1;
        camera->offset.x  = 0;
        camera->offset.y  = 0;
        camera->state     = Camera_State_Follow;
        player1->camera   = camera;
        foreach_all(RingField, field) { field->running = true; }
        SceneInfo->timeEnabled = true;
        return true;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Fight(EntityCutsceneSeq *host)
{
    EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZStart->eggman;
    EntityPhantomKing *king    = (EntityPhantomKing *)ERZStart->king;

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
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZStart->fxRuby;
    ruby->angle += 2;
    ruby->position.y   = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;
    fxRuby->position.x = ruby->position.x;
    fxRuby->position.y = ruby->position.y;
}

void ERZStart_RubyMove(void)
{
    RSDK_THIS(ERZStart);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;

    if (self->velocity.x < 0x40000)
        self->velocity.x += 0x1800;
    ruby->angle += 2;
    ruby->position.x += ruby->velocity.x;
    ruby->position.y = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;
}

void ERZStart_Player_HandleSuperDash(void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    RSDK_THIS(Player);

    uint8 flags = 0;
    if (player->up)
        flags |= 1;
    if (player->down)
        flags |= 2;
    if (player->left)
        flags |= 4;
    if (player->right)
        flags |= 8;

    int32 angle = 0;
    switch (flags) {
        case 0:
            angle = 0;
            if (player->direction)
                angle = 128;
            break;
        case 1: angle = 192; break;
        case 2: angle = 64; break;
        case 4: angle = 128; break;
        case 5: angle = 160; break;
        case 6: angle = 96; break;
        case 8: angle = 0; break;
        case 9: angle = 224; break;
        case 10: angle = 32; break;
        default: break;
    }

    player->rings -= 5;
    if (player->rings < 0)
        player->rings = 0;
    player->velocity.x = 0x1600 * RSDK.Cos256(angle);
    player->velocity.y = 0x1600 * RSDK.Sin256(angle);
    RSDK.PlaySfx(ItemBox->sfxHyperRing, false, 255);
    RSDK.PlaySfx(Player->sfxPeelRelease, false, 255);
    if (player->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLY, &player->animator, false, 6);
        player->animator.rotationFlag = 1;
    }
    else {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 0);
    }
    player->state            = ERZStart_State_PlayerSuperFly;
    player->abilityValues[0] = 60;
    if (FXFade) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 64;
    }
#if RETRO_USE_PLUS
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
                RSDK.SetSpriteAnimation(player1->aniFrames, ANI_FLY, &player1->animator, true, 0);
            if (self->velocity.x > -player1->topSpeed) {
                self->velocity.x -= player1->acceleration;
                if (player1->velocity.x > 0)
                    self->velocity.x -= player1->acceleration;
            }
            player1->direction = FLIP_X;
        }
        else if (player1->right) {
            if (player1->characterID == ID_KNUCKLES && self->direction == FLIP_X)
                RSDK.SetSpriteAnimation(player1->aniFrames, ANI_FLY, &player1->animator, true, 0);
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

    if (self->jumpAbilityTimer == 1 && self->jumpPress) {
        if (self->characterID == ID_KNUCKLES) {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 6);
            self->animator.rotationFlag = 1;
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
        RSDK.SetSpriteAnimation(entity->aniFrames, ANI_FLY, &entity->animator, false, 6);
        entity->animator.rotationFlag = 1;
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

#if RETRO_INCLUDE_EDITOR
void ERZStart_EditorDraw(void)
{
    RSDK_THIS(ERZStart);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void ERZStart_EditorLoad(void) {}
#endif

void ERZStart_Serialize(void) { RSDK_EDITABLE_VAR(ERZOutro, VAR_VECTOR2, size); }
