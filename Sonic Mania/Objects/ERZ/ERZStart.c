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

    if (!entity->activated) {
        foreach_active(Player, player)
        {
            if (!player->sidekick && Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                CutsceneSeq_StartSequence((Entity *)entity, states);
                entity->activated = true;
            }
        }
    }

    if (--entity->timer <= 0) {
        entity->timer                = 15 * RSDK.Rand(0, 4);
        RSDK_sceneInfo->seconds      = RSDK.Rand(0, 100);
        RSDK_sceneInfo->milliseconds = RSDK.Rand(0, 100);
        RSDK_sceneInfo->minutes      = RSDK.Rand(0, 9);
    }

#if !RETRO_USE_PLUS
    if (ERZStart->superDashCooldown > 0) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        foreach_all(HUD, hud)
        {
            if (hud)
                hud->field_154 = true;
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
    RSDK_THIS(ERZSetup);
    if (!RSDK_sceneInfo->inEditor) {
        INIT_ENTITY(entity);
        CutsceneRules_SetupEntity(entity);
        ERZStart_SetupObjects();
        ERZStart->timer = 0;
    }
}

void ERZStart_StageLoad(void)
{
    ERZStart->sfxClack     = RSDK.GetSFX("Stage/Clack.wav");
    ERZStart->sfxFlyIn     = RSDK.GetSFX("ERZ/FlyIn.wav");
    ERZStart->sfxHullClose = RSDK.GetSFX("Stage/HullClose.wav");
    ERZStart->sfxRumble    = RSDK.GetSFX("Stage/Rumble.wav");
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
        fxRuby->state       = 0;
        fxRuby->fadeBlack   = 0x200;
        fxRuby->fadeWhite   = 0x200;
        fxRuby->outerRadius = RSDK_screens->width;
        fxRuby->field_70    = 64;
        foreach_break;
    }

    foreach_all(PhantomKing, king)
    {
        // if (!king->field_68)
        ERZStart->king = (Entity *)king;
    }

    foreach_all(ChaosEmerald, emerald) { ERZStart->emeralds[emerald->type] = emerald; }

    foreach_all(KleptoMobile, eggman)
    {
        // if (!eggman->field_60)
        ERZStart->eggman = (Entity *)eggman;
    }
}

void ERZStart_HandlePlayerHover(EntityCutsceneSeq *seq, void *p, int posY)
{
    EntityPlayer *player = (EntityPlayer *)p;

    player->position.x = 0x300000;
    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
    player->position.x += (player->position.x - player->position.x) >> 3;
    player->position.y += (posY + 0xA00 * RSDK.Sin256(2 * (seq->timer - seq->field_68 + 64)) - player->position.y) >> 3;
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

        RSDK_screens->position.x = 0;
        RSDK_screens->position.y = 0;
        Zone->screenBoundsT2[0]  = 0;
        Zone->screenBoundsT1[0]  = 0;
        camera->boundsT          = 0;
        Zone->screenBoundsB2[0]  = RSDK_screens->height;
        Zone->screenBoundsB1[0]  = Zone->screenBoundsB2[0];
        camera->boundsB          = Zone->screenBoundsB1[0];
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
        Zone->screenBoundsR2[0]     = 1552;
        Zone->screenBoundsR1[0]     = 1552;
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
        fxRuby->state = FXRuby_Unknown4;

    EntityPhantomKing *king = (EntityPhantomKing *)ERZStart->king;
    if (fxRuby->outerRadius <= 0) {
        ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);
        ruby->drawOrder = Zone->drawOrderLow + 1;
        // king->state = PhantomKing_Unknown10;
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
    /*if (king->state == PhantomKing_Unknown13) {
        ruby->state = ERZStart_RubyMove;
        return true;
    }*/
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
        host->field_68              = camera->position.x;
    }

    int pos = host->field_68;
    if (king->position.x - 0x400000 > pos)
        pos = king->position.x - 0x400000;
    camera->position.x = pos;
    /*if (king->field_224) {
        ruby->state = StateMachine_None;
        return true;
    }*/
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
        host->field_68 = ruby->position.x;
        host->field_64 = ruby->position.y;
    }

    if (host->timer > 0) {
        if (host->timer == 60) {
            RSDK.PlaySfx(ERZStart->sfxClack, false, 255);
            // king->field_110       = true;
            ruby->position.x = -0x400000;
            ruby->state      = StateMachine_None;
            return true;
        }

        int percent = 0x10000;
        if (host->timer << 16 < 0x3C0000)
            percent = (host->timer << 16) / 60;

        // Vector2 pos = MathHelpers_Unknown5(percent, host->field_68, host->field_64, host->field_68, host->field_64 - 0x400000, king->field_100,
        //                                    king->field_104 - 0x400000,
        //                            king->field_100, king->field_104);
        //
        // ruby->position.x += (pos.x - ruby->position.x) >> 2;
        // ruby->position.y += ((pos.y - host->timer << 16) >> 2);
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
        Camera_SetupLerp(0, 0, camera->position.x - (RSDK_screens->width << 16), camera->position.y, 3);
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

    uint16 eggmanSlot                = RSDK.GetEntityID(eggman);
    EntityKleptoMobile *eggmanChild1 = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanChild2 = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanChild3 = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot               = RSDK.GetEntityID(king);
    EntityPhantomKing *kingChild1 = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingChild2 = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    if (!host->timer) {
        host->field_68 = camera->position.x;
        host->field_64 = camera->position.y;
        // king->field_60.y     = king->position.y;
        // king->field_60.x     = king->position.x;
        // king->state          = PhantomKing_Unknown15;
        king->rotation = -4;
        // king->field_1A4      = 0;
        king->drawOrder = eggman->drawOrder + 1;
        // king->field_1AC      = 0;
        // eggman->state        = KelptoMobile_Unknown8;
        // RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 11, &eggmanChild1->animator, true, 0);
        eggman->position.x = camera->position.x - (RSDK_screens->width << 16);
        // eggman->field_70.x      = camera->position.x - (RSDK_screens->width << 16);
        // eggman->position.y      = king->field_60.y + 0x100000;
        // eggman->field_70.y      = king->field_60.y + 0x100000;
        eggman->direction  = FLIP_X;
        eggman->velocity.x = 0xD0000;
        eggman->velocity.y = 0;
    }

    // eggmanChild1->state           = KelptoMobile_Unknown18;
    // eggmanChild2->state           = KelptoMobile_Unknown22;
    // eggmanChild3->state           = KelptoMobile_Unknown22;
    /*eggmanChild1->position.x = eggman->field_70.x;
    eggmanChild1->position.y = eggman->field_70.y;
    eggmanChild1->position.x += 0x3C0000;
    eggmanChild1->position.y -= 0x3E0000;
    eggmanChild1->drawOrder  = eggmanChild2->drawOrder + 1;

    eggmanChild2->position.x = eggman->field_70.x;
    eggmanChild2->position.x -= 0x40000;
    eggmanChild2->position.y = eggman->field_70.y;
    eggmanChild2->position.y -= 0x360000;
    eggmanChild2->field_F0 = eggman->field_70.x;
    eggmanChild2->field_F0 -= 0x600000;
    eggmanChild2->field_F4 = eggman->field_70.y;
    eggmanChild2->field_F4 -= 0x280000;
    eggmanChild2->field_200       = 0;
    eggmanChild2->field_210       = 0;
    eggmanChild2->field_218       = 0;
    eggmanChild2->field_1F8       = 0;

    eggmanChild3->position.x = eggman->field_70.x;
    eggmanChild3->position.x += 0x2E0000;
    eggmanChild3->position.y = eggman->field_70.y;
    eggmanChild3->position.y += 0x400000;
    eggmanChild3->field_F0 = eggman->field_70.x;
    eggmanChild3->field_F0 -= 0x3A0000;
    eggmanChild3->field_F4 = eggman->field_70.y;
    eggmanChild3->field_F4 += 0x300000;
    eggmanChild3->field_200 = 0;
    eggmanChild3->field_210 = 0;
    eggmanChild3->field_218 = 0;
    eggmanChild3->field_1F8 = 0;

    camera->position.x     = maxVal(eggman->field_70.x + 0x200000, host->field_68);

    kingChild1->state           = PhantomKing_Unknown20;
    kingChild2->state           = PhantomKing_Unknown20;
    kingChild1->position.x = king->field_60.x;
    kingChild1->position.x -= 0x700000;
    kingChild1->position.y = king->field_60.y;
    kingChild1->position.y -= 0x1C0000;
    kingChild1->field_F8 = king->field_60.x;
    kingChild1->field_F8 += 0x100000;
    kingChild1->field_FC = king->field_60.y;
    kingChild1->field_FC -= 0x2A0000;
    kingChild1->field_204       = 3;
    kingChild1->drawOrder       = eggmanChild2->drawOrder + 1;
    kingChild1->field_20C       = 0;

    kingChild2->position.x = king->field_60.x;
    kingChild2->position.x -= 0x3E0000;
    kingChild2->position.y = king->field_60.y;
    kingChild2->position.y += 0x520000;
    kingChild2->field_F8 = king->field_60.x;
    kingChild2->field_F8 += 0x380000;
    kingChild2->field_FC = king->field_60.;
    kingChild2->field_FC += 0x380000;
    kingChild2->field_204      = 1;
    kingChild2->drawOrder      = eggmanChild3->drawOrder + 1;
    kingChild2->field_20C      = 0;

    if (eggman->position.x >= king->field_60.x - 0x580000) {
        eggman->position.x = king->field_60.x - 0x580000;
        eggman->velocity.x = 0;
        eggman->velocity.y = 0;
        Camera_ShakeScreen(5, 0, 0);
        RSDK.PlaySfx(ERZStart->sfxHullClose, false, 255);
        return true;
    }*/
    return false;
}

bool32 ERZStart_CutsceneState_Unknown8(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZStart->fxRuby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZStart->eggman;
    EntityPhantomKing *king    = (EntityPhantomKing *)ERZStart->king;

    uint16 eggmanSlot                = RSDK.GetEntityID(eggman);
    EntityKleptoMobile *eggmanChild1 = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanChild2 = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot               = RSDK.GetEntityID(king);
    EntityPhantomKing *kingChild1 = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingChild2 = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);

    if (host->timer > 6 && !(host->timer % 6))
        Camera_ShakeScreen(1, 0, 0);
    if (!(host->timer % 37))
        RSDK.PlaySfx(ERZStart->sfxRumble, false, 255);
    if (host->timer == 120) {
        // RSDK.SetSpriteAnimation(PhantomKing->aniFrames, 8, &king->animator, true, 1);
        // king->animator.animationSpeed = 1;
    }

    if (host->timer == 172) {
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
        fxRuby->position.x = king->position.x;
        fxRuby->position.y = king->position.y;
        fxRuby->fadeWhite  = 0x300;
        RSDK.StopSFX(ERZStart->sfxRumble);
        return true;
    }
    else {
        eggmanChild1->position.x -= RSDK.Sin256(16 * host->timer) << 8;
        kingChild1->position.x -= RSDK.Sin256(16 * host->timer) << 8;
        eggmanChild2->position.x += RSDK.Sin256(16 * host->timer) << 8;
        kingChild2->position.x += RSDK.Sin256(16 * host->timer) << 8;
    }
    return false;
}

bool32 ERZStart_CutsceneState_Unknown9(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZStart->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZStart->fxRuby;

    ERZStart_HandlePlayerHover(host, player1, ruby->startPos.y);

    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZStart->eggman;
    EntityPhantomKing *king    = (EntityPhantomKing *)ERZStart->king;

    uint16 eggmanSlot                = RSDK.GetEntityID(eggman);
    EntityKleptoMobile *eggmanChild1 = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanChild2 = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanChild3 = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    uint16 kingSlot               = RSDK.GetEntityID(king);
    EntityPhantomKing *kingChild1 = RSDK_GET_ENTITY(kingSlot - 1, PhantomKing);
    EntityPhantomKing *kingChild2 = RSDK_GET_ENTITY(kingSlot + 1, PhantomKing);
    // if (!king->animator.frameID)
    //    king->animator.animationSpeed = 0;
    if (!host->timer) {
        eggman->position.x = 0x31C0000;
        eggman->position.y = -0x3C0000;
        // eggman->state                = StateMachine_None;
        // eggmanChild1->state          = KelptoMobile_Unknown17;
        // eggmanChild2->state          = KelptoMobile_Unknown21;
        // eggmanChild3->state          = KelptoMobile_Unknown21;
        eggmanChild1->drawOrder = Zone->drawOrderLow;
        // RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 12, &eggmanChild1->animator, true, 5);
        // kingChild1->state          = PhantomKing_Unknown19;
        // kingChild2->state          = PhantomKing_Unknown19;
        king->drawOrder       = Zone->drawOrderLow;
        kingChild1->drawOrder = Zone->drawOrderLow;
        kingChild2->drawOrder = Zone->drawOrderLow;
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
        Camera_SetupLerp(0, 0, RSDK_screens->centerX << 16, camera->position.y, 2);
    if (!host->field_6C[0]) {
        if (camera->position.x == RSDK_screens->centerX << 16) {
            host->field_6C[0] = 1;
            host->field_68    = host->timer;
        }
    }
    else if (host->timer - host->field_68 == 60) {
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
    if (player1->playerAnimator.animationID != ANI_TRANSFORM) {
        Zone->screenBoundsR1[0]     = 0x610;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsT1[0]     = 0;
        Zone->playerBoundActiveT[0] = true;
        player1->state              = ERZStart_State_PlayerSuperFly;
        player1->abilityValues[0]   = 60;
        player1->velocity.x         = 0x160000;
        player1->onGround           = false;
        player1->movesetState       = ERZStart_Player_StartSuperFly;
        RSDK.PlaySfx(Player->sfx_PeelRelease, false, 255);
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 64;
        player1->stateInput  = Player_ProcessP1Input;
        if (player1->characterID == ID_KNUCKLES) {
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLY, &player1->playerAnimator, false, 6);
            player1->playerAnimator.rotationFlag = 1;
        }
        else {
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_RUN, &player1->playerAnimator, false, 0);
        }
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveT[0] = true;
        // PhantomKing_Unknown6(king);
        Camera_SetTargetEntity(0, player1);
        camera->targetPtr = (Entity *)player1;
        camera->offset.x  = 0;
        camera->offset.y  = 0;
        camera->state     = Camera_State_Follow;
        player1->camera   = camera;
        foreach_all(RingField, field) { field->running = true; }
        RSDK_sceneInfo->timeEnabled = true;
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
        // if (!king->health && !eggman->health)
        //    return true;
    }
    else if (player1->hurtFlag != 1) {
        player1->hurtFlag = 1;
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

    if (entity->velocity.x < 0x40000)
        entity->velocity.x += 0x1800;
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
    RSDK.PlaySfx(ItemBox->sfx_HyperRing, false, 255);
    RSDK.PlaySfx(Player->sfx_PeelRelease, false, 255);
    if (player->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLY, &player->playerAnimator, false, 6);
        player->playerAnimator.rotationFlag = 1;
    }
    else {
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_RUN, &player->playerAnimator, false, 0);
    }
    player->state            = ERZStart_State_PlayerSuperFly;
    player->abilityValues[0] = 60;
    if (FXFade) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
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
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, false, 0);
    }
    else {
        if (player1->up) {
            if (entity->velocity.y > -player1->topSpeed) {
                entity->velocity.y -= player1->acceleration;
                if (player1->velocity.y > 0)
                    entity->velocity.y -= player1->acceleration;
            }
        }
        else if (player1->down) {
            if (entity->velocity.y < player1->topSpeed) {
                entity->velocity.y += player1->acceleration;
                if (player1->velocity.y < 0)
                    entity->velocity.y += player1->acceleration;
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
            if (player1->characterID == ID_KNUCKLES && entity->direction == FLIP_NONE)
                RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLY, &player1->playerAnimator, true, 0);
            if (entity->velocity.x > -player1->topSpeed) {
                entity->velocity.x -= player1->acceleration;
                if (player1->velocity.x > 0)
                    entity->velocity.x -= player1->acceleration;
            }
            player1->direction = FLIP_X;
        }
        else if (player1->right) {
            if (player1->characterID == ID_KNUCKLES && entity->direction == FLIP_X)
                RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLY, &player1->playerAnimator, true, 0);
            if (entity->velocity.x < player1->topSpeed) {
                entity->velocity.x += player1->acceleration;
                if (player1->velocity.x < 0)
                    entity->velocity.x += player1->acceleration;
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

    if (entity->jumpAbilityTimer == 1 && entity->jumpPress) {
        if (entity->characterID == ID_KNUCKLES) {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 6);
            entity->playerAnimator.rotationFlag = 1;
        }
        else {
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimator, false, 0);
        }
        entity->state = ERZStart_State_PlayerSuperFly;
    }
}

void ERZStart_State_PlayerRebound(void)
{
    EntityPlayer *entity = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (entity->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_FLY, &entity->playerAnimator, false, 6);
        entity->playerAnimator.rotationFlag = 1;
    }
    else {
        RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_RUN, &entity->playerAnimator, false, 0);
    }

    entity->rotation += 0x20;
    entity->onGround = false;
    if (entity->rotation == 0x600) {
        entity->rotation = 0;
        entity->state    = ERZStart_State_PlayerSuperFly;
    }
}

void ERZStart_EditorDraw(void) {}

void ERZStart_EditorLoad(void) {}

void ERZStart_Serialize(void) { RSDK_EDITABLE_VAR(ERZOutro, VAR_VECTOR2, size); }
