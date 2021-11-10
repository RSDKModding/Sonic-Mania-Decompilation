#include "SonicMania.h"

ObjectERZOutro *ERZOutro;

void ERZOutro_Update(void)
{
    RSDK_THIS(ERZOutro);

    void *states[] = { ERZOutro_CutsceneState_AttackEggman, ERZOutro_CutsceneState_AttackRecoil,
                       ERZOutro_CutsceneState_LoseEmeralds, ERZOutro_CutsceneState_OpenPortal,
                       ERZOutro_CutsceneState_EnterPortal,  ERZOutro_CutsceneState_FadeOut,
                       ERZOutro_CutsceneState_ShowEnding,   NULL };

    if (!entity->activated) {
        CutsceneSeq_StartSequence((Entity *)entity, states);
        entity->activated = true;
    }
}

void ERZOutro_LateUpdate(void) {}

void ERZOutro_StaticUpdate(void) {}

void ERZOutro_Draw(void) {}

void ERZOutro_Create(void *data)
{
    RSDK_THIS(ERZOutro);
    INIT_ENTITY(entity);
    CutsceneRules_SetupEntity(entity, &entity->size, &entity->hitbox);
    entity->active = ACTIVE_NEVER;
}

void ERZOutro_StageLoad(void)
{
    foreach_all(RubyPortal, portal)
    {
        ERZOutro->rubyPortal = (Entity *)portal;
        foreach_break;
    }

    foreach_all(PhantomRuby, ruby)
    {
        ERZOutro->ruby = (Entity *)ruby;
        foreach_break;
    }

    foreach_all(FXRuby, fxRuby)
    {
        ERZOutro->fxRuby = (Entity *)fxRuby;
        foreach_break;
    }

    foreach_all(PhantomKing, king)
    {
        if (!king->type)
            ERZOutro->king = (Entity *)king;
    }

    foreach_all(ChaosEmerald, emerald) { ERZStart->emeralds[emerald->type] = emerald; }

    foreach_all(KleptoMobile, eggman)
    {
        if (!eggman->type)
            ERZOutro->eggman = (Entity *)eggman;
    }

    ERZOutro->savedGame = false;
}

void ERZOutro_SetEmeraldStates(void)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZOutro->ruby;

    for (int e = 0; e < 7; ++e) {
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
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZOutro->ruby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)ERZOutro->fxRuby;

    ruby->angle += 2;
    int amp          = abs(RSDK.Sin256(Zone->timer)) >> 6;
    ruby->position.x = (RSDK.Rand(-amp, amp) << 16) + ruby->startPos.x;
    ruby->position.y = (RSDK.Sin256(ruby->angle) << 8) + (RSDK.Rand(-amp, amp) << 16) + ruby->startPos.y;

    fxRuby->position.x = ruby->position.x;
    fxRuby->position.y = ruby->position.y;
}

bool32 ERZOutro_CutsceneState_AttackEggman(EntityCutsceneSeq *host)
{
    EntityPlayer *player1      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZOutro->eggman;
    EntityFXRuby *fxRuby       = (EntityFXRuby *)ERZOutro->fxRuby;
    EntityERZOutro *entity     = (EntityERZOutro *)host->cutsceneCurEntity;

    uint16 eggmanSlot               = RSDK.GetEntityID(eggman);
    EntityKleptoMobile *eggmanChild = RSDK_GET_ENTITY(eggmanSlot - 2, KleptoMobile);
    EntityKleptoMobile *eggmanArm1  = RSDK_GET_ENTITY(eggmanSlot - 1, KleptoMobile);
    EntityKleptoMobile *eggmanArm2  = RSDK_GET_ENTITY(eggmanSlot + 1, KleptoMobile);

    if (!host->timer) {
        host->field_64 = player1->rings;
        eggman->state  = StateMachine_None;
        if (player1->characterID == ID_KNUCKLES)
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLY, &player1->playerAnimator, false, 6);
        else
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_RUN, &player1->playerAnimator, false, 0);
        player1->state           = ERZStart_State_PlayerSuperFly;
        player1->nextAirState    = StateMachine_None;
        player1->nextGroundState = StateMachine_None;
        player1->onGround        = false;
        player1->stateInput      = StateMachine_None;
        PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
        fxRuby->fadeWhite           = 768;
        Zone->playerBoundActiveT[0] = false;
        CutsceneSeq_LockPlayerControl(player1);
        RSDK_sceneInfo->timeEnabled = false;

        foreach_all(RingField, field) { field->running = false; }
        foreach_all(Ring, ring) { destroyEntity(ring); }

        ERZOutro->field_8 = player1->position.x;
        ERZOutro->field_C = player1->position.y;
    }

    if (fxRuby->fadeWhite > 0)
        fxRuby->fadeWhite -= 8;

    int posX = (entity->position.x - entity->size.x);
    int posY = entity->position.y;

    if (host->timer <= 0) {
        player1->rings = host->field_64;
    }
    else if (host->timer < 120) {
        if (player1->characterID == ID_KNUCKLES)
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLY, &player1->playerAnimator, false, 6);
        else
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_RUN, &player1->playerAnimator, false, 0);
        int x             = player1->position.x;
        int y             = player1->position.y;
        player1->position = MathHelpers_Unknown5((host->timer << 16) / 120, ERZOutro->field_8, ERZOutro->field_C, ERZOutro->field_8,
                                                 ERZOutro->field_C - 0x2000000, posX - 0x2000000, posY, posX, posY);

        player1->velocity.y = player1->position.y - y;
        player1->velocity.x = player1->position.x - x;
        player1->direction  = x >= player1->position.x;
        player1->rings      = host->field_64;
    }
    else {
        player1->position.x = posX;
        player1->position.y = posY;
        player1->direction  = FLIP_NONE;
        player1->right      = true;
        ERZStart_Player_HandleSuperDash(player1);
        player1->velocity.y >>= 1;
        player1->right   = false;
        eggman->field_70 = entity->position;
        eggman->field_70.x += entity->size.x;
        eggman->position   = eggman->field_70;
        eggman->velocity.x = 0;
        eggman->velocity.y = 0;
        eggman->direction  = FLIP_NONE;
        RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 13, &eggman->animator3, true, 0);
        eggman->state     = KleptoMobile_State_Unknown1;
        eggman->field_120 = 1;

        eggmanChild->state      = KleptoMobile_State3_Unknown1;
        eggmanChild->field_70.x = eggman->position.x;
        eggmanChild->field_70.y = eggman->position.y;
        eggmanChild->position.x = eggman->position.x;
        eggmanChild->position.y = eggman->position.y;

        eggmanArm1->state      = KleptoMobile_State1_Unknown1;
        eggmanArm1->field_70.x = eggman->position.x;
        eggmanArm1->field_70.y = eggman->position.y;
        eggmanArm1->position.x = eggman->position.x;
        eggmanArm1->position.y = eggman->position.y;

        eggmanArm2->state      = KleptoMobile_State1_Unknown1;
        eggmanArm2->field_70.x = eggman->position.x;
        eggmanArm2->field_70.y = eggman->position.y;
        eggmanArm2->position.x = eggman->position.x;
        eggmanArm2->position.y = eggman->position.y;
        return true;
    }

    return false;
}

bool32 ERZOutro_CutsceneState_AttackRecoil(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);

    EntityKleptoMobile *eggman = (EntityKleptoMobile *)ERZOutro->eggman;
    EntityFXRuby *fxRuby       = (EntityFXRuby *)ERZOutro->fxRuby;
    EntityRubyPortal *portal   = (EntityRubyPortal *)ERZOutro->rubyPortal;
    EntityPhantomRuby *ruby    = (EntityPhantomRuby *)ERZOutro->ruby;

    fxRuby->position = eggman->circlePos;
    int x            = eggman->position.x - 0x400000;
    int y            = eggman->field_70.y - 0x200000;

    if (!host->field_6C[0]) {
        if (player1->position.x >= eggman->position.x - 0x200000) {
            host->field_68      = player1->position.y;
            player1->position.x = eggman->position.x - 0x200000;
            player1->velocity.x = -0x40000;
            player1->velocity.y = -0x40000;
            player1->state      = Player_State_None;
            player1->superState = SUPERSTATE_FADEOUT;
            player1->shield     = SHIELD_NONE;
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_HURT, &player1->playerAnimator, true, 0);
            host->field_6C[0] = 1;
            fxRuby->state     = FXRuby_Unknown3;
            fxRuby->drawOrder = Zone->drawOrderLow - 1;
            player1->camera   = 0;
            camera->targetPtr = 0;
            Camera_SetupLerp(0, 0, x, y, 3);
            portal->position.x = x;
            portal->position.y = y;
            Music_TransitionTrack(TRACK_EGGMAN1, 0.2);
            RSDK.SetSpriteAnimation(KleptoMobile->aniFrames, 16, &eggman->animator3, true, 0);
            eggman->field_120          = 0;
            eggman->state              = KleptoMobile_State_CutsceneExplode;
            eggman->timer              = 0;
            eggman->invincibilityTimer = 48;
            eggman->velocity.x         = 0x60000;
            RSDK.PlaySfx(KleptoMobile->sfxHit, false, 255);
            ruby->visible  = true;
            ruby->startPos = eggman->circlePos;
            ruby->position = eggman->circlePos;
        }
    }
    else {
        if (!host->field_6C[1]) {
            ruby->startPos.x += 0x20000;
            if (eggman->velocity.x <= 0x10000)
                host->field_6C[1] = 1;
            else
                eggman->velocity.x -= 0x2000;
        }
        if (!host->field_6C[2]) {
            player1->velocity.y += 0x3800;
            if (player1->velocity.y > 0 && player1->position.y >= host->field_68) {
                RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FAN, &player1->playerAnimator, true, 0);
                player1->position.y = host->field_68;
                player1->velocity.x = 0;
                player1->velocity.y = 0;
                host->field_6C[2]   = 1;
            }
        }
        ERZOutro_HandleRubyHover();
        if (host->field_6C[1] && host->field_6C[2])
            return true;
    }
    player1->rings = host->field_64;
    return false;
}

bool32 ERZOutro_CutsceneState_LoseEmeralds(EntityCutsceneSeq *host)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)ERZOutro->ruby;
    ERZOutro_HandleRubyHover();

    if (!host->timer) {
        int angle = 0;
        for (int e = 0; e < 7; ++e) {
            EntityChaosEmerald *emerald = ERZStart->emeralds[e];
            emerald->angle              = angle;
            emerald->radius             = 0;
            emerald->scale.x            = 512;
            emerald->scale.y            = 512;
            emerald->groundVel          = 512;
            emerald->state              = ChaosEmerald_State_Rotate;
            emerald->visible            = true;

            angle += 0x2492;
        }
    }

    if (host->timer >= 30) {
        for (int e = 0; e < 7; ++e) {
            ERZStart->emeralds[e]->radius = 0x2000;
        }
    }
    else {
        for (int e = 0; e < 7; ++e) {
            ERZStart->emeralds[e]->radius = (host->timer << 13) / 30;
        }
    }

    for (int e = 0; e < 7; ++e) ERZStart->emeralds[e]->originPos = ruby->startPos;
    return host->timer == 90;
}

bool32 ERZOutro_CutsceneState_OpenPortal(EntityCutsceneSeq *host)
{
    EntityRubyPortal *portal = (EntityRubyPortal *)ERZOutro->rubyPortal;
    ERZOutro_HandleRubyHover();
    if (host->timer == 90) {
        portal->state     = RubyPortal_Unknown4;
        portal->drawOrder = Zone->drawOrderLow;
        portal->visible   = true;
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
    }
    if (host->timer >= 90) {
        ERZOutro_SetEmeraldStates();
        return portal->state == StateMachine_None;
    }
    return false;
}

bool32 ERZOutro_CutsceneState_EnterPortal(EntityCutsceneSeq *host)
{
    EntityPlayer *player1    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPhantomRuby *ruby  = (EntityPhantomRuby *)ERZOutro->ruby;
    EntityFXRuby *fxRuby     = (EntityFXRuby *)ERZOutro->fxRuby;
    EntityRubyPortal *portal = (EntityRubyPortal *)ERZOutro->rubyPortal;

    ERZOutro_SetEmeraldStates();
    if (!host->timer) {
        ERZOutro->field_24 = 0;
        host->field_68     = RSDK.ATan2(player1->position.x - portal->position.x, player1->position.y - portal->position.y) << 16;
        int rx             = abs(portal->position.x - player1->position.x) >> 16;
        int ry             = abs(portal->position.y - player1->position.y) >> 16;
        host->field_64     = MathHelpers_SquareRoot(rx * rx + ry * ry) << 16;
        player1->drawFX |= FX_SCALE;
        player1->scale.x = 0x200;
        player1->scale.y = 0x200;
    }
    if (host->timer == 60)
        PhantomRuby_Unknown2(ruby);

    if (host->timer < 108) {
        ERZOutro_HandleRubyHover();
    }
    else {
        if (host->timer == 108) {
            ERZOutro_HandleRubyHover();
            ruby->startPos.x   = ruby->position.x;
            ruby->startPos.y   = ruby->position.y;
            ERZOutro->field_2C = RSDK.ATan2(ruby->position.x - portal->position.x, ruby->position.y - portal->position.y) << 16;
            int rx             = abs(portal->position.x - ruby->position.x) >> 16;
            int ry             = abs(portal->position.y - ruby->position.y) >> 16;
            ERZOutro->field_28 = MathHelpers_SquareRoot(rx * rx + ry * ry) << 16;
            ruby->drawFX |= FX_SCALE;
            ruby->scale.x = 0x200;
            ruby->scale.y = 0x200;
            PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
            Music_FadeOut(0.2);
        }
        ERZOutro->field_24 += 0x800;

        host->field_68 += ERZOutro->field_24;
        host->field_68 &= 0xFFFFFF00;
        player1->position.x = portal->position.x;
        player1->position.y = portal->position.y;
        player1->position.x += ((host->field_64 >> 9) & 0xFFFFFF80) * RSDK.Cos512(host->field_68 >> 15);
        player1->position.y += ((host->field_64 >> 9) & 0xFFFFFF80) * RSDK.Sin512(host->field_68 >> 15);

        ERZOutro->field_2C += ERZOutro->field_24;
        ERZOutro->field_2C &= 0xFFFFFF00;
        ruby->position.x = portal->position.x;
        ruby->position.y = portal->position.y;
        ruby->position.x += ((ERZOutro->field_28 >> 9) & 0xFFFFFF80) * RSDK.Cos512(ERZOutro->field_2C >> 15);
        ruby->position.y += ((ERZOutro->field_28 >> 9) & 0xFFFFFF80) * RSDK.Sin512(ERZOutro->field_2C >> 15);

        if (fxRuby->fadeWhite >= 0x200)
            host->field_6C[0] = 1;
        else
            fxRuby->fadeWhite += 2;
        int rx   = abs(portal->position.x - player1->position.x) >> 16;
        int ry   = abs(portal->position.y - player1->position.y) >> 16;
        int dist = MathHelpers_SquareRoot(rx * rx + ry * ry);

        player1->scale.x = 8 * minVal(dist, 0x40);
        player1->scale.y = 8 * minVal(dist, 0x40);

        rx            = abs(portal->position.x - ruby->position.x) >> 16;
        ry            = abs(portal->position.y - ruby->position.y) >> 16;
        dist          = MathHelpers_SquareRoot(rx * rx + ry * ry);
        ruby->scale.x = 8 * minVal(dist, 0x40);
        ruby->scale.y = 8 * minVal(dist, 0x40);

        if (ERZOutro->field_28 > 0)
            ERZOutro->field_28 -= 0x8000;

        if (host->field_64 <= 0)
            host->field_6C[1] = 1;
        else
            host->field_64 -= 0x8000;
        return host->field_6C[0] && host->field_6C[1];
    }
    return false;
}

bool32 ERZOutro_CutsceneState_FadeOut(EntityCutsceneSeq *host)
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

bool32 ERZOutro_CutsceneState_ShowEnding(EntityCutsceneSeq *host)
{
    if (globals->saveSlotID != NO_SAVE_SLOT) {
        if (!host->timer) {
            ERZOutro->savedGame = false;
            SaveGame_SaveProgress();
            GameProgress_GiveEnding(2);
            SaveGame_SaveFile(ERZOutro_SaveFileCB);
            UIWaitSpinner_Wait();
        }
        if (!ERZOutro->savedGame)
            return false;
        UIWaitSpinner_Wait2();
    }
    API_UnlockAchievement("ACH_GAME_CLEARED");
    if (checkPlayerID(ID_KNUCKLES, 1) && checkPlayerID(ID_KNUCKLES, 2))
        RSDK.SetScene("Videos", "True End?");
    else
        RSDK.SetScene("Videos", "Good End");
    RSDK.LoadScene();
    return true;
}

void ERZOutro_SaveFileCB(int32 status) { ERZOutro->savedGame = true; }

#if RETRO_INCLUDE_EDITOR
void ERZOutro_EditorDraw(void)
{
    RSDK_THIS(ERZOutro);
    CutsceneRules_DrawCutsceneBounds(entity, &entity->size);
}

void ERZOutro_EditorLoad(void) {}
#endif

void ERZOutro_Serialize(void) { RSDK_EDITABLE_VAR(ERZOutro, VAR_VECTOR2, size); }
