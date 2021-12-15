// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EncoreIntro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectEncoreIntro *EncoreIntro;

void EncoreIntro_Update(void)
{
    RSDK_THIS(EncoreIntro);
    if (!self->activated) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                EncoreIntro_SetupCutscene();
                EntityCutsceneSeq *seq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
                if (seq->objectID)
                    seq->skipType = SKIPTYPE_RELOADSCN;
                self->activated = true;
            }
        }
    }

    //skip part 2 flag
    if (self->field_64) {
        void *states[] = { EncoreIntro_CutsceneState_Unknown7,  EncoreIntro_CutsceneState_Unknown8,
                           EncoreIntro_CutsceneState_Unknown9,  EncoreIntro_CutsceneState_Unknown10,
                           EncoreIntro_CutsceneState_Unknown11, EncoreIntro_CutsceneState_Unknown12,
                           EncoreIntro_CutsceneState_Unknown13, EncoreIntro_CutsceneState_Unknown14,
                           EncoreIntro_CutsceneState_Unknown15, EncoreIntro_CutsceneState_Unknown16,
                           EncoreIntro_CutsceneState_Unknown17, EncoreIntro_CutsceneState_Unknown18,
                           EncoreIntro_CutsceneState_Unknown19, EncoreIntro_CutsceneState_Unknown20,
                           EncoreIntro_CutsceneState_Unknown21, EncoreIntro_CutsceneState_Unknown22,
                           EncoreIntro_CutsceneState_Unknown23, EncoreIntro_CutsceneState_Unknown24,
                           EncoreIntro_CutsceneState_Unknown21, NULL };

        EncoreIntro_SetupCutscenePart2();
        CutsceneSeq_StartSequence((Entity *)self, states);
        self->field_64 = false;
    }
}

void EncoreIntro_LateUpdate(void) {}

void EncoreIntro_StaticUpdate(void) {}

void EncoreIntro_Draw(void) {}

void EncoreIntro_Create(void *data)
{
    RSDK_THIS(EncoreIntro);
    if (!SceneInfo->inEditor) {
        INIT_ENTITY(self);
        CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
        EncoreIntro_SetupEntities();

        if (globals->enableIntro) {
            foreach_all(HUD, hud)
            {
                hud->maxOffset = hud->offsets[HUDOFF_SCORE].x;
                hud->offsets[HUDOFF_SCORE].x -= 0x1000000;
                hud->offsets[HUDOFF_TIME].x -= 0x1100000;
                hud->offsets[HUDOFF_RINGS].x -= 0x1200000;
                hud->offsets[HUDOFF_LIFE].x -= 0x1300000;
                hud->state = HUD_State_ComeOnScreen;
            }

            EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
            ruby->alpha             = 0;
            ruby->inkEffect         = INK_ALPHA;
            ruby->state             = EncoreIntro_PhantomRuby_Unknown1;
            RSDK.SetSpriteAnimation(0xFFFF, 0xFFFF, &ruby->animator2, true, 0);
            ruby->animator2.animationID = -1;
        }
        else {
            int32 id = RSDK.GetEntityID(self);

            foreach_all(Animals, animal)
            {
                if (RSDK.GetEntityID(animal) < id) {
                    destroyEntity(animal);
                }
                else {
                    foreach_break;
                }
            }
            RSDK.ResetEntitySlot(id - 3, TYPE_BLANK, NULL);
            RSDK.ResetEntitySlot(id - 2, TYPE_BLANK, NULL);
            self->activated = true;
            self->field_64  = true;
        }
    }
}

void EncoreIntro_StageLoad(void)
{
    EncoreIntro->aniFrames          = RSDK.LoadSpriteAnimation("AIZ/Cutscene.bin", SCOPE_STAGE);
    EncoreIntro->sfxHighFive        = RSDK.GetSfx("Stage/HighFive.wav");
    EncoreIntro->sfxMysticPoof      = RSDK.GetSfx("MSZ/MysticPoof.wav");
    EncoreIntro->sfxKingCharge      = RSDK.GetSfx("LRZ/KingCharge.wav");
    EncoreIntro->sfxMysticHat       = RSDK.GetSfx("MSZ/MysticHat.wav");
    EncoreIntro->sfxMysticTransform = RSDK.GetSfx("MSZ/MysticTransform.wav");
    EncoreIntro->sfxPon             = RSDK.GetSfx("Stage/Pon.wav");
    Music_SetMusicTrack("BuddyBeat.ogg", TRACK_BUDDYBEAT, 85232);
}

void EncoreIntro_SetupEntities(void)
{
    foreach_all(PhantomRuby, ruby)
    {
        EncoreIntro->phantomRuby = (Entity *)ruby;
        foreach_break;
    }

    foreach_all(FXRuby, fxRuby)
    {
        EncoreIntro->fxRuby = (Entity *)fxRuby;
        fxRuby->state       = FXRuby_State_ShrinkRing;
        fxRuby->outerRadius = 0;
        foreach_break;
    }

    foreach_all(RubyPortal, portal)
    {
        EncoreIntro->rubyPortal = (Entity *)portal;
        foreach_break;
    }
}

void EncoreIntro_SetupCutscene(void)
{
    void *states[] = { EncoreIntro_CutsceneState_Unknown1,  EncoreIntro_CutsceneState_Unknown2,
                       EncoreIntro_CutsceneState_Unknown3,  EncoreIntro_CutsceneState_Unknown4,
                       EncoreIntro_CutsceneState_Unknown5,  EncoreIntro_CutsceneState_Unknown6,
                       EncoreIntro_CutsceneState_Unknown7,  EncoreIntro_CutsceneState_Unknown8,
                       EncoreIntro_CutsceneState_Unknown9,  EncoreIntro_CutsceneState_Unknown10,
                       EncoreIntro_CutsceneState_Unknown11, EncoreIntro_CutsceneState_Unknown12,
                       EncoreIntro_CutsceneState_Unknown13, EncoreIntro_CutsceneState_Unknown14,
                       EncoreIntro_CutsceneState_Unknown15, EncoreIntro_CutsceneState_Unknown16,
                       EncoreIntro_CutsceneState_Unknown17, EncoreIntro_CutsceneState_Unknown18,
                       EncoreIntro_CutsceneState_Unknown19, EncoreIntro_CutsceneState_Unknown20,
                       EncoreIntro_CutsceneState_Unknown21, EncoreIntro_CutsceneState_Unknown22,
                       EncoreIntro_CutsceneState_Unknown23, EncoreIntro_CutsceneState_Unknown24,
                       EncoreIntro_CutsceneState_Unknown21, NULL };
    RSDK_THIS(EncoreIntro);

    CutsceneSeq_StartSequence((Entity *)self, states);
}

void EncoreIntro_SetupCutscenePart2(void)
{
    RSDK_THIS(EncoreIntro);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera  = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    player1->camera         = camera;
    player1->stateInput     = Player_ProcessP1Input;
    player1->tileCollisions = true;
    player1->onGround       = true;
    player1->state          = Player_State_Ground;
    camera->targetPtr       = (Entity *)player1;
    camera->state           = Camera_State_Follow;
    camera->position.x      = player1->position.x;

    Vector2 size;
    RSDK.GetLayerSize(Zone->fgLow, &size, true);
    Zone->cameraBoundsR[0]     = size.x;
    Zone->playerBoundsR[0]     = size.x << 16;
    Zone->playerBoundActiveR[0] = true;
    Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
    Zone->playerBoundsL[0]     = Zone->cameraBoundsL[0] << 16;
    Zone->playerBoundActiveL[0] = true;

    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    foreach_all(SchrodingersCapsule, capsule)
    {
        ruby->position.x = capsule->position.x;
        ruby->state      = EncoreIntro_PhantomRuby_Unknown1;
        ruby->startPos.y = capsule->position.y - 0x800000;
        ruby->position.y = capsule->position.y - 0x800000;
    }
}

bool32 EncoreIntro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    EntityFXRuby *fxRuby    = (EntityFXRuby *)EncoreIntro->fxRuby;
    if (SceneInfo->minutes || SceneInfo->seconds) {
        host->stateID   = 21;
        host->skipType = SKIPTYPE_DISABLED;
    }
    else {
        if (!host->timer) {
            globals->stock          = ID_NONE;
            globals->characterFlags = ID_SONIC;
            Player_ChangeCharacter(player1, ID_SONIC);
            destroyEntity(RSDK.GetEntityByID(SLOT_PLAYER2));
            player1->alpha      = 0;
            player1->inkEffect  = INK_ALPHA;
            ruby->alpha         = 0;
            ruby->inkEffect     = INK_ALPHA;
            player1->position.y = ruby->position.y;
            player1->state      = Player_State_None;
            player1->stateInput = StateMachine_None;
            CutsceneSeq_LockAllPlayerControl();
            player1->groundVel  = 0;
            player1->velocity.x = 0;
            player1->direction  = FLIP_NONE;
            foreach_all(CutsceneHBH, cutsceneHBH) { cutsceneHBH->drawOrder = Zone->drawOrderLow; }
            Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - SCREEN_YSIZE;
            Zone->playerBoundsT[0] = Zone->cameraBoundsB[0] - SCREEN_YSIZE;
        }
        else if (host->timer == 240) {
            fxRuby->delay = 32;
            fxRuby->state    = FXRuby_State_IncreaseStageDeform;
            PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
            Camera_ShakeScreen(0, 4, 4);
            Music_TransitionTrack(TRACK_EGGMAN1, 0.01);
            foreach_active(Animals, animal)
            {
                animal->behaviour = 0;
                animal->active    = ACTIVE_NORMAL;
            }
        }

        if (host->timer < 360) {
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_FAN, &player1->animator, false, 0);
            player1->position.x += (player1->position.x - player1->position.x) >> 3;
            player1->position.y += (0xA00 * RSDK.Sin256(2 * (host->timer - host->storedValue2)) + ruby->position.y - player1->position.y) >> 3;
            player1->state = Player_State_None;
        }
        else {
            ruby->alpha     = 0;
            ruby->inkEffect = INK_ALPHA;
            return true;
        }
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (host->timer >= 60) {
        player->alpha += 2;
        if (player->alpha >= 0x100) {
            player->inkEffect = INK_NONE;
            RSDK.Sin256(2 * (host->timer - host->storedValue2));
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
            player->state     = Player_State_Air;
            player->up        = true;
            host->values[0] = 1;
            return true;
        }
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    if (host->timer >= 60) {
        ruby->alpha += 2;
        if (ruby->alpha >= 0x100) {
            ruby->inkEffect = INK_NONE;
            return true;
        }
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown4(EntityCutsceneSeq *host)
{
    EntityPhantomRuby *ruby  = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    EntityRubyPortal *portal = (EntityRubyPortal *)EncoreIntro->rubyPortal;

    if (host->timer >= 60) {
        portal->alpha -= 4;
        if (portal->alpha <= 0) {
            destroyEntity(portal);
            ruby->state = EncoreIntro_PhantomRuby_Unknown2;
            Music_FadeOut(0.012);
            host->skipType = SKIPTYPE_DISABLED;
            return true;
        }
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown5(EntityCutsceneSeq *host)
{
    return true; // what the
}

bool32 EncoreIntro_CutsceneState_Unknown6(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (host->timer == 60)
        player->up = false;
    if (host->timer >= 120) {
        foreach_all(TitleCard, titleCard)
        {
            titleCard->active    = ACTIVE_NORMAL;
            titleCard->state     = TitleCard_State_Initial;
            titleCard->stateDraw = TitleCard_Draw_Default;
            foreach_break;
        }
        foreach_all(HUD, hud)
        {
            hud->vsStates[0] = HUD_State_ComeOnScreen;
            hud->state                = hud->vsStates[0];
        }
        Music_PlayTrack(TRACK_STAGE);
        EncoreIntro_SetupCutscenePart2();
        return true;
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown7(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK_THIS(EncoreIntro);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    if (player->position.x <= self->position.x + 0x2000000) {
        Zone->cameraBoundsL[0]     = ScreenInfo->position.x;
        Zone->playerBoundsL[0]     = Zone->cameraBoundsL[0] << 16;
        Zone->playerBoundActiveL[0] = true;
    }
    else {
        Zone->cameraBoundsT[0] = 0;
        Zone->playerBoundsT[0] = 0;
    }

    if (ruby->activeScreens) {
        Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - SCREEN_YSIZE;
        Zone->playerBoundsT[0] = Zone->cameraBoundsB[0] - SCREEN_YSIZE;
        Music_TransitionTrack(TRACK_EGGMAN1, 0.05);
        return true;
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown8(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK_THIS(EncoreIntro);
    EntityActClear *actClear = RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear);
    if (!self->field_88) {
        if (actClear->objectID == ActClear->objectID)
            self->field_88 = 1;
    }
    else {
        player->velocity.x = 0;
        player->groundVel  = 0;
        if (actClear->objectID != ActClear->objectID) {
            self->field_88 = false;
            Music_TransitionTrack(TRACK_EGGMAN2, 0.05);

            StarPost->playerPositions[0] = player->position;
            StarPost->playerPositions[1] = player->position;
            player->state                = EncoreIntro_PlayerState_BuddySel;
            player->stateInput           = EncoreIntro_PlayerState_InputNone;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, true, 0);

            EntityPlayer *buddy1 = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
            buddy1->state        = Player_State_Ground;
            RSDK.SetSpriteAnimation(buddy1->aniFrames, ANI_IDLE, &buddy1->animator, true, 0);

            EntityPlayer *buddy2 = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
            buddy2->state        = Player_State_Ground;
            RSDK.SetSpriteAnimation(buddy2->aniFrames, ANI_IDLE, &buddy2->animator, true, 0);
            return true;
        }
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown9(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    player->drawOrder      = Zone->playerDrawHigh;
    EntityPlayer *selBuddy = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
    if (player->position.x > selBuddy->position.x - 0x380000) {
        globals->characterFlags = ID_SONIC | ID_MIGHTY;
    }
    else {
        selBuddy = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        if (player->position.x < selBuddy->position.x + 0x380000)
            globals->characterFlags = ID_SONIC | ID_RAY;
        else
            return false;
    }
    player->state           = EncoreIntro_PlayerState_HandleAir;
    player->nextAirState    = StateMachine_None;
    player->nextGroundState = Player_State_None;
    player->onGround        = false;
    globals->stock          = ID_NONE;
    if (globals->characterFlags == (ID_SONIC | ID_RAY)) {
        RSDK.SetSpriteAnimation(EncoreIntro->aniFrames, 0, &player->animator, true, 0);
        player->velocity.x = -0xC000;
    }
    else {
        RSDK.SetSpriteAnimation(EncoreIntro->aniFrames, 1, &player->animator, true, 0);
        player->velocity.x = 0x8000;
    }
    player->velocity.y = -0x30000;
    CutsceneSeq_LockPlayerControl(player);
    player->stateInput = StateMachine_None;

    EntityPlayer *buddy = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    RSDK.CopyEntity(buddy, selBuddy, true);
    buddy->drawOrder       = Zone->playerDrawHigh;
    buddy->state           = EncoreIntro_PlayerState_HandleAir;
    buddy->nextAirState    = StateMachine_None;
    buddy->nextGroundState = Player_State_None;
    buddy->onGround        = false;

    if (globals->characterFlags == (ID_SONIC | ID_RAY)) {
        RSDK.SetSpriteAnimation(EncoreIntro->aniFrames, 2, &buddy->animator, true, 0);
        buddy->velocity.x = 0xC000;
    }
    else {
        RSDK.SetSpriteAnimation(EncoreIntro->aniFrames, 3, &buddy->animator, true, 0);
        buddy->velocity.x = 0x8000;
    }
    buddy->velocity.y = -0x30000;
    globals->playerID = (buddy->characterID << 8) + ID_SONIC;
    Music_FadeOut(0.025);
    RSDK.PlaySfx(EncoreIntro->sfxHighFive, 0, 255);

    if (globals->characterFlags == (ID_SONIC | ID_RAY))
        CREATE_ENTITY(AIZEncoreTutorial, intToVoid(5), (buddy->position.x >> 1) + (player->position.x >> 1), player->position.y - 0x480000);
    else
        CREATE_ENTITY(AIZEncoreTutorial, intToVoid(6), (buddy->position.x >> 1) + (player->position.x >> 1), player->position.y - 0x480000);

    return true;
}

bool32 EncoreIntro_CutsceneState_Unknown10(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy  = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    RSDK_THIS(EncoreIntro);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    HeavyMystic_Unknown2();
    if (player->onGround) {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, true, 0);
        player->velocity.x = 0;
        player->groundVel  = 0;
    }
    if (host->timer > 300)
        player->direction = ruby->position.x < player->position.x;

    if (buddy->onGround) {
        RSDK.SetSpriteAnimation(buddy->aniFrames, ANI_IDLE, &buddy->animator, true, 0);
        buddy->velocity.x = 0;
        buddy->groundVel  = 0;
    }
    if (host->timer > 300)
        buddy->direction = ruby->position.x < buddy->position.x;

    if (host->timer == 18) {
        if (globals->characterFlags == (ID_SONIC | ID_RAY)) {
            player->velocity.x = 0;
            buddy->velocity.x  = 0;
        }
    }
    else if (host->timer == 300) {
        EntityPlayer *otherBuddy = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
        if (!otherBuddy->objectID)
            otherBuddy = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
        self->position.x = otherBuddy->position.x;
        self->position.y = otherBuddy->position.y;
    }
    else {
        if (host->timer != 420) {
            if (host->timer == 480) {
                EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
                RSDK.SetSpriteAnimation(mystic->aniFrames, 1, &mystic->animator, true, 0);
            }
            else {
                if (host->timer == 496) {
                    RSDK.PlaySfx(EncoreIntro->sfxMysticHat, 0, 255);
                }
                else if (host->timer < 570) {
                    EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
                    if (mystic->animator.frameID == mystic->animator.frameCount - 1)
                        RSDK.SetSpriteAnimation(mystic->aniFrames, 0, &mystic->animator, true, 0);
                }
                else {
                    return true;
                }
            }
        }
        else {
            EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
            EntityPlayer *otherBuddy  = RSDK_GET_ENTITY(SLOT_PLAYER3, Player);
            if (!otherBuddy->objectID)
                otherBuddy = RSDK_GET_ENTITY(SLOT_PLAYER4, Player);
            mystic->position.x = otherBuddy->position.x;
            mystic->position.y = otherBuddy->position.y;
            mystic->direction  = otherBuddy->direction ^ 1;
            mystic->drawOrder  = Zone->playerDrawHigh - 1;
            destroyEntity(otherBuddy);

            RSDK.PlaySfx(EncoreIntro->sfxMysticPoof, 0, 255);
            CREATE_ENTITY(Explosion, intToVoid(2), mystic->position.x, mystic->position.y)->drawOrder = Zone->playerDrawHigh - 1;
            Music_PlayTrack(TRACK_HBHMISCHIEF);

            for (int32 i = 0; i < 2; ++i) {
                EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
                player->state        = Player_State_Air;
                player->onGround     = false;
                player->velocity.y   = -0x38000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
            }
        }
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown11(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy  = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    unused(camera);

    RSDK_THIS(EncoreIntro);
    EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
    EntityPhantomRuby *ruby   = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    player->direction = ruby->position.x < player->position.x;
    buddy->direction  = ruby->position.x < buddy->position.x;

    int32 offset = -0x320000;
    if (buddy->direction)
        offset = 0x320000;
    int32 pos = offset + ruby->position.x;

    if (mystic->position.x != pos) {
        if (mystic->position.x >= pos) {
            mystic->position.x -= 0x20000;
            if (mystic->position.x < offset + ruby->position.x)
                mystic->position.x = offset + ruby->position.x;
        }
        else {
            mystic->position.x += 0x20000;
            if (mystic->position.x > offset + ruby->position.x)
                mystic->position.x = offset + ruby->position.x;
        }

        mystic->position.y -= 0x18000;
        mystic->velocity.y = -0x18000;
        if (host->timer > 24) {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_LOOKUP, &player->animator, false, 1);
            if (player->animator.frameID == 5)
                player->animator.speed = 0;

            RSDK.SetSpriteAnimation(buddy->aniFrames, ANI_LOOKUP, &buddy->animator, false, 1);
            if (buddy->animator.frameID == 5)
                buddy->animator.speed = 0;
        }
        self->position.x = mystic->position.x;
        self->position.y = mystic->position.y;
        HeavyMystic_Unknown2();
    }
    else {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_LOOKUP, &player->animator, false, 1);
        if (player->animator.frameID == 5)
            player->animator.speed = 0;

        RSDK.SetSpriteAnimation(buddy->aniFrames, ANI_LOOKUP, &buddy->animator, false, 1);
        if (buddy->animator.frameID == 5)
            buddy->animator.speed = 0;

        if (mystic->position.y > ruby->startPos.y + 0x140000) {
            mystic->position.y += mystic->velocity.y;
            self->position.x = mystic->position.x;
            self->position.y = mystic->position.y;
            HeavyMystic_Unknown2();
        }
        else {
            RSDK.SetSpriteAnimation(mystic->aniFrames, 2, &mystic->animator, true, 0);
            return true;
        }
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown12(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy  = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    RSDK_THIS(EncoreIntro);
    EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
    EntityPhantomRuby *ruby   = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    player->direction = ruby->position.x < player->position.x;
    buddy->direction  = ruby->position.x < buddy->position.x;

    if (host->timer == 33) {
        RSDK.PlaySfx(EncoreIntro->sfxMysticTransform, 0, 255);
        ruby->state = StateMachine_None;
    }
    else if (host->timer >= 34) {
        if (ruby->position.y < mystic->position.y) {
            ruby->startPos.y += 0x18000;
            ruby->position.y += 0x18000;
        }
        else if (ruby->visible) {
            ruby->visible = false;
        }
        else if (host->timer == 75) {
            RSDK.SetSpriteAnimation(mystic->aniFrames, 0, &mystic->animator, true, 0);
            Zone->cameraBoundsR[0] = 16 * RSDK.GetSceneLayer(Zone->fgLow)->width;
            Zone->playerBoundsR[0] = 16 * RSDK.GetSceneLayer(Zone->fgLow)->width;
            Zone->cameraBoundsT[0] = 784;
            Zone->playerBoundsT[0] = 784;
            mystic->direction       = FLIP_NONE;
            return true;
        }
    }

    self->position.x = mystic->position.x;
    self->position.y = mystic->position.y;
    HeavyMystic_Unknown2();
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown13(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy  = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    RSDK_THIS(EncoreIntro);
    EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
    EntityPhantomRuby *ruby   = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    player->direction = mystic->position.x < player->position.x;
    buddy->direction  = mystic->position.x < buddy->position.x;
    if (host->timer >= 60) {
        mystic->position.y -= 0x800000;
        mystic->position.x = CutsceneHBH_GetEntity(HBH_KINGDAMAGED)->position.x - 0x580000;
        ruby->position.y -= 0x800000;
        SceneInfo->timeEnabled = true;

        player->state      = Player_State_Ground;
        player->stateInput = Player_ProcessP1Input;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, true, 0);

        buddy->state      = Player_State_Ground;
        buddy->stateInput = Player_ProcessP2Input_AI;
        RSDK.SetSpriteAnimation(buddy->aniFrames, ANI_IDLE, &buddy->animator, true, 0);
        return true;
    }
    else {
        mystic->velocity.x += 0x1800;
        if (mystic->velocity.x > 0x30000)
            mystic->velocity.x = 0x30000;
        if (mystic->activeScreens == 1)
            --host->timer;
        mystic->position.x += mystic->velocity.x;
        self->position.x = mystic->position.x;
        self->position.y = mystic->position.y;
        HeavyMystic_Unknown2();
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown14(EntityCutsceneSeq *host)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    RSDK_THIS(AIZEncoreTutorial);
    EntityCutsceneHBH *king = CutsceneHBH_GetEntity(HBH_KINGDAMAGED);

    if (player->position.x >= king->position.x - 0x2600000) {
        self->position.x = player->position.x;
        self->position.y = player->position.y;
        player->stateInput = StateMachine_None;
        if (player->onGround)
            player->state = Player_State_Ground;
        CutsceneSeq_LockAllPlayerControl();

        player->right = true;
        if (player->velocity.x >= 0x20000)
            player->velocity.x = 0x20000;
        self->velocity.x = player->velocity.x;
        camera->targetPtr = (Entity *)self;
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID) {
            EntityCutsceneSeq *cutsceneSeq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
            cutsceneSeq->skipType         = SKIPTYPE_CALLBACK;
            cutsceneSeq->skipCallback      = AIZEncoreTutorial_State_ReturnToCutscene;
        }
        foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
        return true;
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown15(EntityCutsceneSeq *host)
{
    RSDK_THIS(EncoreIntro);
    EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
    EntityCutsceneHBH *king   = CutsceneHBH_GetEntity(HBH_KINGDAMAGED);
    EntityPlayer *player      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy       = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (self->position.x >= king->position.x - 0x880000) {
        if (self->velocity.x == 0x60000) {
            player->groundVel  = 0x30000;
            player->velocity.x = 0x30000;
            player->position.x = (ScreenInfo->position.x - 32) << 16;
            buddy->groundVel   = 0x30000;
            buddy->position.x  = (ScreenInfo->position.x - 96) << 16;
            buddy->velocity.x  = 0x30000;
            buddy->drawFX &= ~FX_SCALE;
            buddy->position.y     = player->position.y;
            buddy->state          = Player_State_Ground;
            buddy->tileCollisions = true;
            buddy->stateInput     = StateMachine_None;
            buddy->scale.x        = 0;
            buddy->scale.y        = 0;
            buddy->drawOrder      = player->drawOrder - 1;
        }

        self->velocity.x -= 0x1800;
        if (self->velocity.x < 0x4C000) {
            player->right = false;
            buddy->right  = false;
            player->up    = true;
            buddy->up     = true;
        }
        if (buddy->state == Player_State_Ground && buddy->position.x >= player->position.x - 0x200000)
            buddy->velocity.x = player->velocity.x;
        if (self->velocity.x <= -0x2D000) {
            RSDK.SetSpriteAnimation(mystic->aniFrames, 2, &mystic->animator, true, 0);
            self->velocity.x = 0;
            return true;
        }
    }
    else {
        self->velocity.x += 0x6000;
        if (self->velocity.x > 0x60000)
            self->velocity.x = 0x60000;

        player->state = Player_State_Ground;
        player->right = true;
        if (player->groundVel > 0x20000)
            player->groundVel = 0x20000;
        if (buddy->groundVel > ((buddy->position.x <= player->position.x) + 1) << 16)
            buddy->groundVel = 0x20000;
    }

    if (self->velocity.x > 0)
        self->position.x += self->velocity.x;

    self->position.y = mystic->position.y;
    if (self->position.x <= mystic->position.x) {
        int32 storeX         = self->position.x;
        int32 storeY         = self->position.y;
        self->position.x = mystic->position.x;
        self->position.y = mystic->position.y;
        HeavyMystic_Unknown2();
        self->position.x = storeX;
        self->position.y = storeY;
    }
    else {
        self->position.x = mystic->position.x;
        self->position.y = mystic->position.y;
        HeavyMystic_Unknown2();
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown16(EntityCutsceneSeq *host)
{
    EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
    EntityCutsceneHBH *king   = CutsceneHBH_GetEntity(HBH_KINGDAMAGED);
    EntityPhantomRuby *ruby   = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    EntityPlayer *player      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy       = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    player->timer = 0;
    buddy->timer  = 0;
    if (buddy->state == Player_State_Ground && buddy->position.x >= player->position.x - 0x200000)
        buddy->velocity.x = player->velocity.x;

    switch (host->timer) {
        case 33:
            RSDK.PlaySfx(EncoreIntro->sfxPon, 0, 255);
            ruby->visible    = 1;
            ruby->position.x = mystic->position.x + 0x320000;
            ruby->position.y = mystic->position.y;
            ruby->velocity.x = 0;
            ruby->velocity.y = -0x40000;
            ruby->state      = EncoreIntro_PhantomRuby_Unknown3;
            Music_TransitionTrack(TRACK_EGGMAN1, 0.025);
            HeavyMystic_Unknown2();
            break;
        case 40:
            mystic->drawOrder = Zone->playerDrawLow;
            HeavyMystic_Unknown2();
            break;
        case 75:
            RSDK.SetSpriteAnimation(mystic->aniFrames, 0, &mystic->animator, true, 0);
            player->up = false;
            buddy->up  = false;
            HeavyMystic_Unknown2();
            break;
        default:
            if (ruby->position.y < king->position.y + 0x60000) {
                HeavyMystic_Unknown2();
            }
            else {
                ruby->position.y = king->position.y + 0x60000;
                ruby->state      = StateMachine_None;
                RSDK.PlaySfx(Player->sfxGrab, 0, 255);
                RSDK.SetSpriteAnimation(king->aniFrames, 3, &king->animator2, true, 0);
                return true;
            }
            break;
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown17(EntityCutsceneSeq *host)
{
    EntityCutsceneHBH *king   = CutsceneHBH_GetEntity(HBH_KINGDAMAGED);
    EntityPhantomRuby *ruby   = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    EntityPlayer *player      = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy       = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    player->timer = 0;
    buddy->timer  = 0;
    if (buddy->state == Player_State_Ground && buddy->position.x >= player->position.x - 0x200000)
        buddy->velocity.x = player->velocity.x;

    switch (host->timer) {
        case 9:
            ruby->drawOrder = Zone->drawOrderLow;
            HeavyMystic_Unknown2();
            break;
        case 42:
            RSDK.SetSpriteAnimation(king->aniFrames, 2, &king->animator, true, 0);
            HeavyMystic_Unknown2();
            break;
        case 58:
        case 66:
        case 74:
            RSDK.PlaySfx(EncoreIntro->sfxKingCharge, 0, 255);
            HeavyMystic_Unknown2();
            break;
        case 102: return true;
        default: HeavyMystic_Unknown2(); break;
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown18(EntityCutsceneSeq *host)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    if (!host->timer)
        PhantomRuby_SetupFlash(ruby);
    if (ruby->flashFinished)
        return true;
    HeavyMystic_Unknown2();
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown19(EntityCutsceneSeq *host)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    EntityPlayer *player    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *buddy     = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    EntityFXRuby *fxRuby = NULL;
    if (host->timer) {
        fxRuby = (EntityFXRuby *)EncoreIntro->fxRuby;
    }
    else {
        fxRuby              = CREATE_ENTITY(FXRuby, NULL, ruby->position.x, ruby->position.y);
        fxRuby->drawOrder   = Zone->playerDrawHigh + 1;
        EncoreIntro->fxRuby = (Entity *)fxRuby;
        PhantomRuby_PlaySFX(7);
        Camera_ShakeScreen(0, 4, 4);
        player->drawOrder = Zone->playerDrawHigh + 1;
        if (buddy->objectID == Player->objectID)
            buddy->drawOrder = Zone->playerDrawHigh + 1;
    }

    if (!host->values[0]) {
        if (fxRuby->fullyExpanded) {
            if (host->storedValue2) {
                if (host->timer == host->storedValue2 + 30) {
                    fxRuby->delay = 64;
                    fxRuby->state    = FXRuby_State_IncreaseStageDeform;
                    PhantomRuby_PlaySFX(4);
                    Camera_ShakeScreen(0, 4, 4);
                }
                else if (host->timer == host->storedValue2 + 210) {
                    fxRuby->delay = 32;
                    fxRuby->state    = FXRuby_State_IncreaseStageDeform;
                    PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
                    Camera_ShakeScreen(0, 4, 4);
                    Music_FadeOut(0.025);
                    host->storedValue2    = host->timer;
                    host->values[0] = 1;
                }
            }
            else {
                host->storedValue2 = host->timer;
            }

            if (host->timer >= host->storedValue2 + 32) {
                EntityPlayer *players[2];
                players[0] = player;
                players[1] = buddy;

                for (int32 i = 0, angle = 0; angle < 0x80; ++i, angle += 0x40) {
                    if (!players[i])
                        break;
                    EntityPlayer *playerPtr = players[i];
                    RSDK.SetSpriteAnimation(playerPtr->aniFrames, ANI_FAN, &playerPtr->animator, false, 0);

                    playerPtr->position.x += (playerPtr->position.x - playerPtr->position.x) >> 3;
                    playerPtr->position.y +=
                        (0xA00 * RSDK.Sin256(2 * (host->timer + angle - host->storedValue2)) + ruby->position.y - playerPtr->position.y) >> 3;
                    playerPtr->tileCollisions = false;
                    playerPtr->velocity.x     = 0;
                    playerPtr->velocity.y     = 0;
                    playerPtr->state          = Player_State_None;
                }
            }
        }
        HeavyMystic_Unknown2();
    }
    else {
        if (fxRuby->fadeWhite >= 512) {
            if (fxRuby->fadeBlack >= 512) {
                if (host->timer < host->storedValue2 + 150) {
                    HeavyMystic_Unknown2();
                }
                else {
                    return true;
                }
            }
            else {
                fxRuby->fadeBlack += 16;
                HeavyMystic_Unknown2();
            }
        }
        else {
            fxRuby->fadeWhite += 16;
            HeavyMystic_Unknown2();
        }
    }

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown20(EntityCutsceneSeq *host)
{
    RSDK_THIS(EncoreIntro);
    Player->playerCount = 2;
    SaveGame_SavePlayerState();
    SaveGame->saveRAM->saveState = 1; // save file is active
    RSDK.SetScene("Cutscenes", "Green Hill Zone");
    EncoreIntro->field_28 = true;
    SaveGame_SaveFile(EncoreIntro_SaveGameCB);
    if (EncoreIntro->field_28) {
        UIWaitSpinner_StartWait();
        if (EncoreIntro->field_28)
            return true;
    }
    RSDK.LoadScene();
    destroyEntity(self);

    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown21(EntityCutsceneSeq *host)
{
    RSDK_THIS(EncoreIntro);
    if (!EncoreIntro->field_28) {
        RSDK.LoadScene();
        destroyEntity(self);
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown22(EntityCutsceneSeq *host)
{
    EntityFXRuby *fxRuby = (EntityFXRuby *)EncoreIntro->fxRuby;

    if (fxRuby->fadeBlack >= 512) {
        EntityPlayer *player         = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityCamera *camera         = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        EntityPhantomRuby *ruby      = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
        destroyEntity(RubyPortal);

        ruby->state     = EncoreIntro_PhantomRuby_Unknown2;
        ruby->inkEffect = INK_NONE;

        player->inkEffect = INK_NONE;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
        player->state          = Player_State_Air;
        player->up             = true;
        player->camera         = NULL;
        host->values[0]      = true;
        player->stateInput     = Player_ProcessP1Input;
        player->tileCollisions = true;
        player->onGround       = true;
        player->state          = Player_State_Ground;
        player->camera         = camera;

        Camera_SetupLerp(0, 0, camera->position.x, camera->position.y, 0);
        camera->targetPtr = (Entity *)player;
        camera->state     = Camera_State_Follow;

        Hitbox *playerHitbox = Player_GetHitbox(player);
        while (
            !RSDK.ObjectTileGrip(player, player->collisionLayers, player->collisionMode, player->collisionPlane, 0, playerHitbox->bottom << 16, 8)) {
            player->position.y += 0x80000;
        }

        if (!SceneInfo->minutes && !SceneInfo->seconds) {
            EntityTitleCard *titleCard = (EntityTitleCard *)CutsceneSeq_GetEntity(TitleCard->objectID);
            titleCard->active          = ACTIVE_NORMAL;
            titleCard->state           = TitleCard_State_Initial;
            titleCard->stateDraw       = TitleCard_Draw_Default;
        }

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);
        Zone->playerBoundsR[0]     = size.x;
        Zone->cameraBoundsR[0]     = size.x;
        Zone->playerBoundActiveR[0] = true;

        foreach_all(HUD, hud)
        {
            hud->vsStates[0] = HUD_State_ComeOnScreen;
            hud->state                = hud->vsStates[0];
        }

        foreach_all(SchrodingersCapsule, capsule)
        {
            ruby->position.x = capsule->position.x;
            ruby->startPos.y = capsule->position.y - 0x800000;
            ruby->state      = EncoreIntro_PhantomRuby_Unknown1;
        }

        foreach_active(Animals, animal) { destroyEntity(animal); }
        return true;
    }
    else {
        fxRuby->fadeBlack += 16;
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown23(EntityCutsceneSeq *host)
{
    EntityFXRuby *fxRuby = (EntityFXRuby *)EncoreIntro->fxRuby;

    if (fxRuby->fadeBlack <= 0) {
        host->stateID = 6;
        host->timer   = 0;
    }
    else {
        fxRuby->fadeBlack -= 16;
    }
    return false;
}

bool32 EncoreIntro_CutsceneState_Unknown24(EntityCutsceneSeq *host)
{
    EntityFXRuby *fxRuby = (EntityFXRuby *)EncoreIntro->fxRuby;

    if (fxRuby->fadeWhite < 512) {
        fxRuby->fadeWhite += 16;
    }
    else if (fxRuby->fadeBlack < 512) {
        fxRuby->fadeBlack += 16;
        return 0;
    }
    else if (host->timer >= 150) {
        return EncoreIntro_CutsceneState_Unknown20(host);
    }
    return false;
}

void EncoreIntro_SaveGameCB(int32 status)
{
    UIWaitSpinner_FinishWait();
    EncoreIntro->field_28 = 0;
}

void EncoreIntro_PhantomRuby_Unknown1(void)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    EntityFXRuby *fxRuby      = (EntityFXRuby *)EncoreIntro->fxRuby;
    ruby->angle += 2;
    ruby->position.y = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;
    fxRuby->position.x = ruby->position.x;
    fxRuby->position.y = ruby->position.y;
}
void EncoreIntro_PhantomRuby_Unknown2(void)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    if (ruby->velocity.x < 0x40000)
        ruby->velocity.x += 0x1800;
    ruby->angle += 2;
    ruby->position.x += ruby->velocity.x;
    ruby->position.y = (RSDK.Sin256(ruby->angle) << 10) + ruby->startPos.y;
}
void EncoreIntro_PhantomRuby_Unknown3(void)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;
    ruby->position.x += ruby->velocity.x;
    ruby->position.y += ruby->velocity.y;
    ruby->velocity.y += 0x3800;
}
void EncoreIntro_PhantomRuby_Unknown4(void)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    ruby->active = ACTIVE_NORMAL;
    if (ruby->velocity.y <= -0x60000) {
        ruby->velocity.y = -ruby->velocity.y;
        ruby->velocity.x = 0;
        ruby->state      = EncoreIntro_PhantomRuby_Unknown5;
    }
    else {
        ruby->position.y += ruby->velocity.y;
        ruby->velocity.y -= 0x3800;
    }
    RSDK.SetChannelAttributes(Music->channelID, 1.0 - (((ruby->startPos.y - ruby->position.y) >> 16) / 120.0), 0.0, 1.0);
}
void EncoreIntro_PhantomRuby_Unknown5(void)
{
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)EncoreIntro->phantomRuby;

    if (ruby->velocity.x >= 60) {
        if (ruby->velocity.y <= 0) {
            ruby->velocity.y = 0;
            ruby->active     = ACTIVE_BOUNDS;
            ruby->state      = EncoreIntro_PhantomRuby_Unknown1;
        }
        else {
            ruby->velocity.y -= 0x3800;
            ruby->position.y += ruby->velocity.y;
        }
        RSDK.SetChannelAttributes(Music->channelID, 1.0 - (((ruby->startPos.y - ruby->position.y) >> 16) / 120.0), 0.0, 1.0);
    }
    else {
        ruby->velocity.x++;
    }
}

void EncoreIntro_PlayerState_BuddySel(void)
{
    RSDK_THIS(Player);
    self->groundVel = clampVal(self->groundVel, -0x20000, 0x20000);
    Player_State_Ground();
}

void EncoreIntro_PlayerState_HandleAir(void)
{
    Player_HandleAirFriction();
    Player_HandleAirMovement();
}

void EncoreIntro_PlayerState_InputNone(void)
{
    RSDK_THIS(Player);
    ControllerInfo[1].keyX.down       = false;
    ControllerInfo[1].keyX.press      = false;
    ControllerInfo[1].keyY.down       = false;
    ControllerInfo[1].keyY.press      = false;
    ControllerInfo[1].keyZ.down       = false;
    ControllerInfo[1].keyZ.press      = false;
    ControllerInfo[1].keyStart.down   = false;
    ControllerInfo[1].keyStart.press  = false;
    ControllerInfo[1].keySelect.down  = false;
    ControllerInfo[1].keySelect.press = false;
    Player_ProcessP1Input();
    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;
}

#if RETRO_INCLUDE_EDITOR
void EncoreIntro_EditorDraw(void)
{
    RSDK_THIS(EncoreIntro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void EncoreIntro_EditorLoad(void) {}
#endif

void EncoreIntro_Serialize(void) { RSDK_EDITABLE_VAR(EncoreIntro, VAR_VECTOR2, size); }
#endif
