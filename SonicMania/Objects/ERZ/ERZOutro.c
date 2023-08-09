// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZOutro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectERZOutro *ERZOutro;

void ERZOutro_Update(void)
{
    RSDK_THIS(ERZOutro);

    if (!self->activated) {
        CutsceneSeq_StartSequence(self, ERZOutro_Cutscene_AttackEggman, ERZOutro_Cutscene_AttackRecoil, ERZOutro_Cutscene_LoseEmeralds,
                                  ERZOutro_Cutscene_OpenPortal, ERZOutro_Cutscene_EnterPortal, ERZOutro_Cutscene_FadeOut,
                                  ERZOutro_Cutscene_ShowEnding, StateMachine_None);

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif

        self->activated = true;
    }
}

void ERZOutro_LateUpdate(void) {}

void ERZOutro_StaticUpdate(void) {}

void ERZOutro_Draw(void) {}

void ERZOutro_Create(void *data)
{
    RSDK_THIS(ERZOutro);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NEVER;
}

void ERZOutro_StageLoad(void)
{
    foreach_all(RubyPortal, portal)
    {
        ERZOutro->rubyPortal = portal;
        foreach_break;
    }

    foreach_all(PhantomRuby, ruby)
    {
        ERZOutro->ruby = ruby;
        foreach_break;
    }

    foreach_all(FXRuby, fxRuby)
    {
        ERZOutro->fxRuby = fxRuby;
        foreach_break;
    }

    foreach_all(PhantomKing, king)
    {
        if (!king->type)
            ERZOutro->king = king;
    }

    foreach_all(ChaosEmerald, emerald) { ERZStart->emeralds[emerald->type] = emerald; }

    foreach_all(KleptoMobile, eggman)
    {
        if (!eggman->type)
            ERZOutro->eggman = eggman;
    }

    ERZOutro->savedGame = false;
}

void ERZOutro_SetEmeraldStates(void)
{
    EntityPhantomRuby *ruby = ERZOutro->ruby;

    for (int32 e = 0; e < 7; ++e) {
        EntityChaosEmerald *emerald = ERZStart->emeralds[e];
        emerald->originPos.x        = ruby->startPos.x;
        emerald->originPos.y        = ruby->startPos.y;
        emerald->radius += 0x400;
        emerald->state   = ChaosEmerald_State_Rotate;
        emerald->visible = true;
    }
}

void ERZOutro_HandleRubyHover(void)
{
    EntityPhantomRuby *ruby = ERZOutro->ruby;
    EntityFXRuby *fxRuby    = ERZOutro->fxRuby;

    ruby->angle += 2;
    int32 amplitude = abs(RSDK.Sin256(Zone->timer)) >> 6;
    int32 offset    = amplitude == 0 ? 0 : RSDK.Rand(-amplitude, amplitude);

    ruby->position.x = (offset << 16) + ruby->startPos.x;
    ruby->position.y = (offset << 16) + ruby->startPos.y + (RSDK.Sin256(ruby->angle) << 8);

    fxRuby->position.x = ruby->position.x;
    fxRuby->position.y = ruby->position.y;
}

bool32 ERZOutro_Cutscene_AttackEggman(EntityCutsceneSeq *host)
{
    EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityKleptoMobile *eggman = ERZOutro->eggman;
    EntityFXRuby *fxRuby       = ERZOutro->fxRuby;
    EntityERZOutro *entity     = (EntityERZOutro *)host->activeEntity;

    uint16 eggmanSlot              = RSDK.GetEntitySlot(eggman);
    EntityKleptoMobile *eggmanHand = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanArmL = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanArmR = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    if (!host->timer) {
        host->storedValue = player1->rings;
        eggman->state     = StateMachine_None;
        if (player1->characterID == ID_KNUCKLES)
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_GLIDE, &player1->animator, false, 6);
        else
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_RUN, &player1->animator, false, 0);

        player1->state           = ERZStart_State_PlayerSuperFly;
        player1->nextAirState    = StateMachine_None;
        player1->nextGroundState = StateMachine_None;
        player1->onGround        = false;
        player1->stateInput      = StateMachine_None;

        PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
        fxRuby->fadeWhite = 768;

        Zone->playerBoundActiveT[0] = false;
        CutsceneSeq_LockPlayerControl(player1);
        SceneInfo->timeEnabled = false;

        foreach_all(RingField, field) { field->running = false; }
        foreach_all(Ring, ring) { destroyEntity(ring); }

        ERZOutro->playerPos.x = player1->position.x;
        ERZOutro->playerPos.y = player1->position.y;
    }

    if (fxRuby->fadeWhite > 0)
        fxRuby->fadeWhite -= 8;

    int32 posX = entity->position.x - entity->size.x;
    int32 posY = entity->position.y;

    if (host->timer <= 0) {
        player1->rings = host->storedValue;
    }
    else if (host->timer < 120) {
        if (player1->characterID == ID_KNUCKLES)
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_GLIDE, &player1->animator, false, 6);
        else
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_RUN, &player1->animator, false, 0);

        int32 x           = player1->position.x;
        int32 y           = player1->position.y;
        player1->position = MathHelpers_GetBezierPoint((host->timer << 16) / 120, ERZOutro->playerPos.x, ERZOutro->playerPos.y, ERZOutro->playerPos.x,
                                                       ERZOutro->playerPos.y - 0x2000000, posX - 0x2000000, posY, posX, posY);

        player1->velocity.y = player1->position.y - y;
        player1->velocity.x = player1->position.x - x;
        player1->direction  = x >= player1->position.x;
        player1->rings      = host->storedValue;
    }
    else {
        player1->position.x = posX;
        player1->position.y = posY;
        player1->direction  = FLIP_NONE;
        player1->right      = true;
        ERZStart_Player_HandleSuperDash(player1);
        player1->velocity.y >>= 1;
        player1->right = false;

        eggman->originPos = entity->position;
        eggman->originPos.x += entity->size.x;
        eggman->position   = eggman->originPos;
        eggman->velocity.x = 0;
        eggman->velocity.y = 0;
        eggman->direction  = FLIP_NONE;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &eggman->eggmanAnimator, true, 0);
        eggman->state       = KleptoMobile_State_CutsceneControlled;
        eggman->holdingRuby = true;

        eggmanHand->state       = KleptoMobile_StateHand_Cutscene;
        eggmanHand->originPos.x = eggman->position.x;
        eggmanHand->originPos.y = eggman->position.y;
        eggmanHand->position.x  = eggman->position.x;
        eggmanHand->position.y  = eggman->position.y;

        eggmanArmL->state       = KleptoMobile_StateArm_Cutscene;
        eggmanArmL->originPos.x = eggman->position.x;
        eggmanArmL->originPos.y = eggman->position.y;
        eggmanArmL->position.x  = eggman->position.x;
        eggmanArmL->position.y  = eggman->position.y;

        eggmanArmR->state       = KleptoMobile_StateArm_Cutscene;
        eggmanArmR->originPos.x = eggman->position.x;
        eggmanArmR->originPos.y = eggman->position.y;
        eggmanArmR->position.x  = eggman->position.x;
        eggmanArmR->position.y  = eggman->position.y;
        return true;
    }

    return false;
}

bool32 ERZOutro_Cutscene_AttackRecoil(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityKleptoMobile *eggman = ERZOutro->eggman;
    EntityFXRuby *fxRuby       = ERZOutro->fxRuby;
    EntityRubyPortal *portal   = ERZOutro->rubyPortal;
    EntityPhantomRuby *ruby    = ERZOutro->ruby;

    fxRuby->position = eggman->rubyPos;
    int32 x          = eggman->position.x - 0x400000;
    int32 y          = eggman->originPos.y - 0x200000;

    if (!host->values[0]) {
        if (player1->position.x >= eggman->position.x - 0x200000) {
            host->storedTimer = player1->position.y;

            player1->position.x = eggman->position.x - 0x200000;
            player1->velocity.x = -0x40000;
            player1->velocity.y = -0x40000;
            player1->state      = Player_State_Static;
            player1->superState = SUPERSTATE_FADEOUT;
            player1->shield     = SHIELD_NONE;
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_HURT, &player1->animator, true, 0);

            host->values[0]   = true;
            fxRuby->state     = FXRuby_State_Expanding;
            fxRuby->drawGroup = Zone->objectDrawGroup[0] - 1;
            player1->camera   = 0;
            camera->target    = 0;
            Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, x, y, 3);

            portal->position.x = x;
            portal->position.y = y;
            Music_TransitionTrack(TRACK_EGGMAN1, 0.2);

            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 16, &eggman->eggmanAnimator, true, 0);
            eggman->holdingRuby        = false;
            eggman->state              = KleptoMobile_State_CutsceneExplode;
            eggman->timer              = 0;
            eggman->invincibilityTimer = 48;
            eggman->velocity.x         = 0x60000;
            RSDK.PlaySfx(KleptoMobile->sfxHit, false, 255);

            ruby->visible  = true;
            ruby->startPos = eggman->rubyPos;
            ruby->position = eggman->rubyPos;
        }
    }
    else {
        if (!host->values[1]) {
            ruby->startPos.x += 0x20000;
            if (eggman->velocity.x <= 0x10000)
                host->values[1] = true;
            else
                eggman->velocity.x -= 0x2000;
        }

        if (!host->values[2]) {
            player1->velocity.y += 0x3800;
            if (player1->velocity.y > 0 && player1->position.y >= host->storedTimer) {
                RSDK.SetSpriteAnimation(player1->aniFrames, ANI_FAN, &player1->animator, true, 0);
                player1->position.y = host->storedTimer;
                player1->velocity.x = 0;
                player1->velocity.y = 0;
                host->values[2]     = true;
            }
        }

        ERZOutro_HandleRubyHover();

        if (host->values[1] && host->values[2])
            return true;
    }

    player1->rings = host->storedValue;
    return false;
}

bool32 ERZOutro_Cutscene_LoseEmeralds(EntityCutsceneSeq *host)
{
    EntityPhantomRuby *ruby = ERZOutro->ruby;

    ERZOutro_HandleRubyHover();

    if (!host->timer) {
        int32 angle = 0;
        for (int32 e = 0; e < 7; ++e) {
            EntityChaosEmerald *emerald = ERZStart->emeralds[e];
            emerald->angle              = angle;
            emerald->radius             = 0;
            emerald->scale.x            = 0x200;
            emerald->scale.y            = 0x200;
            emerald->groundVel          = 0x200;
            emerald->state              = ChaosEmerald_State_Rotate;
            emerald->visible            = true;

            angle += 0x2492;
        }
    }

    if (host->timer >= 30) {
        for (int32 e = 0; e < 7; ++e) ERZStart->emeralds[e]->radius = 0x2000;
    }
    else {
        for (int32 e = 0; e < 7; ++e) ERZStart->emeralds[e]->radius = (host->timer << 13) / 30;
    }

    for (int32 e = 0; e < 7; ++e) ERZStart->emeralds[e]->originPos = ruby->startPos;

    return host->timer == 90;
}

bool32 ERZOutro_Cutscene_OpenPortal(EntityCutsceneSeq *host)
{
    EntityRubyPortal *portal = ERZOutro->rubyPortal;

    ERZOutro_HandleRubyHover();

    if (host->timer == 90) {
        portal->state     = RubyPortal_State_Opened;
        portal->drawGroup = Zone->objectDrawGroup[0];
        portal->visible   = true;
        PhantomRuby_PlaySfx(RUBYSFX_REDCUBE);
    }

    if (host->timer >= 90) {
        ERZOutro_SetEmeraldStates();
        return portal->state == StateMachine_None;
    }

    return false;
}

bool32 ERZOutro_Cutscene_EnterPortal(EntityCutsceneSeq *host)
{
    EntityPlayer *player1    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPhantomRuby *ruby  = ERZOutro->ruby;
    EntityFXRuby *fxRuby     = ERZOutro->fxRuby;
    EntityRubyPortal *portal = ERZOutro->rubyPortal;

    ERZOutro_SetEmeraldStates();

    if (!host->timer) {
        ERZOutro->rubyPortalAcceleration = 0;

        host->storedTimer = RSDK.ATan2(player1->position.x - portal->position.x, player1->position.y - portal->position.y) << 16;
        host->storedValue = MathHelpers_Distance(player1->position.x, player1->position.y, portal->position.x, portal->position.y);

        player1->drawFX |= FX_SCALE;
        player1->scale.x = 0x200;
        player1->scale.y = 0x200;
    }

    if (host->timer == 60)
        PhantomRuby_SetupFlash(ruby);

    if (host->timer < 108) {
        ERZOutro_HandleRubyHover();
    }
    else {
        if (host->timer == 108) {
            ERZOutro_HandleRubyHover();

            ruby->startPos.x = ruby->position.x;
            ruby->startPos.y = ruby->position.y;

            ERZOutro->rubyPortalAngle  = RSDK.ATan2(ruby->position.x - portal->position.x, ruby->position.y - portal->position.y) << 16;
            ERZOutro->rubyPortalRadius = MathHelpers_Distance(ruby->position.x, ruby->position.y, portal->position.x, portal->position.y);

            ruby->drawFX |= FX_SCALE;
            ruby->scale.x = 0x200;
            ruby->scale.y = 0x200;
            PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);

            Music_FadeOut(0.2);
        }

        ERZOutro->rubyPortalAcceleration += 0x800;

        host->storedTimer += ERZOutro->rubyPortalAcceleration;
        host->storedTimer &= 0xFFFFFF00;
        player1->position.x = portal->position.x;
        player1->position.y = portal->position.y;
        player1->position.x += ((host->storedValue >> 9) & 0xFFFFFF80) * RSDK.Cos512(host->storedTimer >> 15);
        player1->position.y += ((host->storedValue >> 9) & 0xFFFFFF80) * RSDK.Sin512(host->storedTimer >> 15);

        ERZOutro->rubyPortalAngle += ERZOutro->rubyPortalAcceleration;
        ERZOutro->rubyPortalAngle &= 0xFFFFFF00;
        ruby->position.x = portal->position.x;
        ruby->position.y = portal->position.y;
        ruby->position.x += ((ERZOutro->rubyPortalRadius >> 9) & 0xFFFFFF80) * RSDK.Cos512(ERZOutro->rubyPortalAngle >> 15);
        ruby->position.y += ((ERZOutro->rubyPortalRadius >> 9) & 0xFFFFFF80) * RSDK.Sin512(ERZOutro->rubyPortalAngle >> 15);

        if (fxRuby->fadeWhite >= 0x200)
            host->values[0] = true;
        else
            fxRuby->fadeWhite += 2;

        int32 rx   = abs(portal->position.x - player1->position.x) >> 16;
        int32 ry   = abs(portal->position.y - player1->position.y) >> 16;
        int32 dist = MathHelpers_SquareRoot(rx * rx + ry * ry);

        player1->scale.x = 8 * MIN(dist, 0x40);
        player1->scale.y = 8 * MIN(dist, 0x40);

        rx            = abs(portal->position.x - ruby->position.x) >> 16;
        ry            = abs(portal->position.y - ruby->position.y) >> 16;
        dist          = MathHelpers_SquareRoot(rx * rx + ry * ry);
        ruby->scale.x = 8 * MIN(dist, 0x40);
        ruby->scale.y = 8 * MIN(dist, 0x40);

        if (ERZOutro->rubyPortalRadius > 0)
            ERZOutro->rubyPortalRadius -= 0x8000;

        if (host->storedValue <= 0)
            host->values[1] = true;
        else
            host->storedValue -= 0x8000;

        return host->values[0] && host->values[1];
    }

    return false;
}

bool32 ERZOutro_Cutscene_FadeOut(EntityCutsceneSeq *host)
{
    EntityFXRuby *fxRuby = (EntityFXRuby *)ERZOutro->fxRuby;
    ERZOutro_HandleRubyHover();

    if (host->timer >= 90) {
        if (fxRuby->fadeBlack < 512)
            fxRuby->fadeBlack += 8;

        return host->timer == 150;
    }

    return false;
}

bool32 ERZOutro_Cutscene_ShowEnding(EntityCutsceneSeq *host)
{
    if (globals->saveSlotID != NO_SAVE_SLOT) {
        if (!host->timer) {
            ERZOutro->savedGame = false;
            SaveGame_SaveProgress();
            GameProgress_GiveEnding(GAMEPROGRESS_ENDING_GOOD);
            SaveGame_SaveFile(ERZOutro_SaveFileCB);
            UIWaitSpinner_StartWait();
        }

        if (!ERZOutro->savedGame)
            return false;

        UIWaitSpinner_FinishWait();
    }

    API_UnlockAchievement(&achievementList[ACH_GAME_CLEARED]);

    if (CHECK_CHARACTER_ID(ID_KNUCKLES, 1) && CHECK_CHARACTER_ID(ID_KNUCKLES, 2))
        RSDK.SetScene("Videos", "True End?");
    else
        RSDK.SetScene("Videos", "Good End");

    RSDK.LoadScene();
    return true;
}

#if MANIA_USE_PLUS
void ERZOutro_SaveFileCB(bool32 success) { ERZOutro->savedGame = true; }
#else
void ERZOutro_SaveFileCB(void) { ERZOutro->savedGame = true; }
#endif

#if GAME_INCLUDE_EDITOR
void ERZOutro_EditorDraw(void)
{
    RSDK_THIS(ERZOutro);

    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void ERZOutro_EditorLoad(void) {}
#endif

void ERZOutro_Serialize(void) { RSDK_EDITABLE_VAR(ERZOutro, VAR_VECTOR2, size); }
