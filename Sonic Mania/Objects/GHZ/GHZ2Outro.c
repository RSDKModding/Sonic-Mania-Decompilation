#include "SonicMania.h"

ObjectGHZ2Outro *GHZ2Outro;

void GHZ2Outro_Update(void)
{
    void *states_GHZ2[]  = { GHZ2Outro_CutsceneState1_Unknown1, GHZ2Outro_CutsceneState1_Unknown2, NULL };
    void *states_Outro[] = { GHZ2Outro_CutsceneState2_Unknown1, GHZ2Outro_CutsceneState2_Unknown2,
                              GHZ2Outro_CutsceneState2_Unknown3, GHZ2Outro_CutsceneState2_Unknown4,
                              GHZ2Outro_CutsceneState2_Unknown5, GHZ2Outro_CutsceneState2_Unknown6,
                              GHZ2Outro_LoadNextStage,           NULL };

    RSDK_THIS(GHZ2Outro);
    if (!entity->type) {
        CutsceneSeq_StartSequence((Entity *)entity, states_Outro);
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID) {
            EntityCutsceneSeq *seq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
            seq->skipState         = 3;
            seq->skipCallback      = GHZ2Outro_Cutscene_SkipCB;
        }
        entity->active = ACTIVE_NEVER;
    }
    else {
        CutsceneSeq_StartSequence((Entity *)entity, states_GHZ2);
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipState = 1;

        foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
        entity->active = ACTIVE_NEVER;
    }
}

void GHZ2Outro_LateUpdate(void) {}

void GHZ2Outro_StaticUpdate(void) {}

void GHZ2Outro_Draw(void) {}

void GHZ2Outro_Create(void *data)
{
    RSDK_THIS(GHZ2Outro);
    if (!RSDK_sceneInfo->inEditor) {
        entity->type = data != NULL;

        foreach_all(DERobot, robot) { entity->DERobot = (Entity *)robot; }
        foreach_all(Eggman, eggman) { entity->eggman = (Entity *)eggman; }

        entity->active  = ACTIVE_NORMAL;
        entity->visible = false;
    }
}

void GHZ2Outro_StageLoad(void)
{
    GHZ2Outro->sfxRocketJet   = RSDK.GetSFX("Stage/RocketJet.wav");
    GHZ2Outro->sfxShinobiJump = RSDK.GetSFX("PSZ/ShinobiJump.wav");
    GHZ2Outro->sfxHeliWoosh   = RSDK.GetSFX("SPZ1/HeliWooshIn.wav");
}

bool32 GHZ2Outro_CutsceneState1_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);
    EntityDERobot *robot = (EntityDERobot *)entity->DERobot;
    robot->state       = StateMachine_None;
    robot->active = ACTIVE_NEVER;
    for (int p = 0; p < Player->playerCount; ++p) {
        CutsceneSeq_LockPlayerControl(RSDK_GET_ENTITY(p, Player));
    }

    foreach_active(Player, player)
    {
        player->state = Player_State_Ground;
        if (!player->sidekick) {
            player->stateInput = StateMachine_None;
            player->left       = true;
            player->right      = false;
            player->up         = false;
            player->down       = false;
            player->jumpPress  = false;
        }
    }

    Zone->deathBoundary[0] += 0x4000 << 0x10;
    Zone->deathBoundary[1] += 0x4000 << 0x10;
    Music_PlayTrack(TRACK_STAGE);

    foreach_active(EggPrison, prison) { prison->state = EggPrison_Unknown5; }

    return true;
}
bool32 GHZ2Outro_CutsceneState1_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);
    EntityDERobot *robot = (EntityDERobot *)entity->DERobot;

    foreach_active(Player, player)
    {
        player->jumpPress = false;
        if (player->onGround) {
            if (player->angle > 8 && player->angle < 64 && player->groundVel > -0x20000)
                player->groundVel = -0x20000;
            if (player->position.x - robot->position.x < 0x600000) {
                player->jumpPress = true;
                player->jumpHold  = true;
            }
        }
        if (player->position.x < robot->position.x + 0x200000)
            player->velocity.x -= player->velocity.x >> 4;
        if (player->position.x < robot->position.x - 0x200000)
            player->position.x = robot->position.x - 0x200000;
    }
    
    if (player->position.y > (RSDK_screens->position.y + 0x200) << 16) {
        foreach_active(Player, player) { player->active = ACTIVE_NEVER; }
        RSDK.LoadScene("Cutscenes", "Green Hill Zone 2");

        Zone_StartFadeOut(0x10, 0x000000);
        return true;
    }
    return false;
}

bool32 GHZ2Outro_CutsceneState2_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);
    if (host->timer >= 8) {
        CutsceneSeq_LockAllPlayerControl();

        foreach_active(Player, player)
        {
            player->stateInput = StateMachine_None;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
        }

        foreach_all(CutsceneHBH, cutsceneHBH) { cutsceneHBH->drawOrder = Zone->drawOrderLow; }

        foreach_all(DERobot, robot)
        {
            robot->state  = StateMachine_None;
            robot->active  = ACTIVE_NORMAL;
            robot->visible = true;
        }
        EntityDERobot *deRobot = (EntityDERobot *)entity->DERobot;
        deRobot->state  = DERobot_Unknown43;

        CutsceneHBH_Unknown8();
        CutsceneHBH_Unknown10();
        CutsceneHBH_Unknown9();
        EntityEggman *eggman = (EntityEggman *)entity->eggman;
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, 9, &eggman->animator, true, 0);
        eggman->direction = FLIP_NONE;
        eggman->state     = Eggman_Unknown1;

        foreach_all(PhantomRuby, ruby)
        {
            entity->phantomRuby = (Entity *)ruby;
            ruby->state         = PhantomRuby_Unknown7;
            ruby->drawFX        = FX_ROTATE;
        }

        FXRuby_Unknown1();
        return true;
    }
    return false;
}
bool32 GHZ2Outro_CutsceneState2_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    foreach_active(Player, player)
    {
        if (player->sidekick == 1) {
            if (host->timer >= 60) {
                if (host->timer == 60) {
                    player->state = Player_State_Ground;
                    player->right = true;
                }
            }
            else {
                player->groundVel = 0;
                if (player->onGround && player->groundedStore) {
                    player->state = Player_State_None;
                    if (player->characterID == ID_TAILS)
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BALANCE2, &player->playerAnimator, false, 0);
                    else
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_IDLE, &player->playerAnimator, false, 0);
                }
            }
        }
        else if (player->onGround && player->groundedStore) {
            player->right = true;
        }

        if (player->position.x > entity->position.x - 0x180000 * player->playerID - 0xE00000) {
            player->right     = FLIP_NONE;
            player->direction = FLIP_NONE;
            if (!player->skidding && player->groundVel > 0) {
                player->skidding = 12;
            }

            player->groundVel -= player->groundVel >> 4;
            if (player->groundVel < 0x10000)
                player->skidding = 0;
        }
    }

    if (host->timer == 90) {
        Music_TransitionTrack(TRACK_HBHMISCHIEF, 0.0125);
    }
    if (host->timer == 120)
        Camera_Unknown3(3, 0, entity->position.x, entity->position.y, 4);
    if (host->timer >= 208) {
        foreach_active(Player, player)
        {
            player->down   = true;
            player->camera = NULL;
        }
        return true;
    }
    return false;
}
bool32 GHZ2Outro_CutsceneState2_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    EntityEggman *eggman = (EntityEggman *)entity->eggman;
    if (host->timer == 90) {
        RSDK.SetSpriteAnimation(Eggman->spriteIndex, 5, &eggman->animator, true, 2);
        eggman->timer      = 30;
        eggman->velocity.x = 0;
        eggman->velocity.y = -0x30000;
        eggman->stateStore = Eggman_Unknown3;
        eggman->state      = Eggman_Unknown5;
        eggman->animID     = 3;
    }
    if (host->timer == 160) {
        foreach_active(DERobot, robot)
        {
            if (robot->aniID == 2 && robot->frameID == 2) {
                robot->state = DERobot_Unknown23;
            }
        }
        RSDK.PlaySFX(DERobot->sfxButton2, 0, 255);
    }
    if (host->timer == 240) {
        eggman->state  = Eggman_Unknown2;
        eggman->animID = 2;
        Music_SetMusicTrack("RubyPresence.ogg", TRACK_EGGMAN1, 198457);
        Music_TransitionTrack(TRACK_EGGMAN1, 0.025);
    }

    if (host->timer == 272) {
        EntityCutsceneHBH *shinobi = CutsceneHBH_GetEntity(HBH_MYSTIC);
        if (shinobi)
            shinobi->state = CutsceneHBH_Unknown15;
        Camera_Unknown3(0, 0, entity->position.x, entity->position.y - 0x400000, 1);
        RSDK.PlaySFX(GHZ2Outro->sfxRocketJet, 0, 255);
    }

    if (host->timer == 288) {
        EntityCutsceneHBH *rider = CutsceneHBH_GetEntity(HBH_RIDER);
        if (rider) {
            if (rider->direction == FLIP_X) {
                rider->timer = 0;
                RSDK.SetSpriteAnimation(rider->spriteIndex, 2, &rider->animator, true, 0);
            }
            rider->state = CutsceneHBH_Unknown17;
        }
        RSDK.PlaySFX(GHZ2Outro->sfxHeliWoosh, 0, 255);
    }
    if (host->timer == 304) {
        CutsceneHBH_Unknown7();
        RSDK.PlaySFX(GHZ2Outro->sfxShinobiJump, 0, 255);
    }

    if (host->timer == 320) {
        EntityCutsceneHBH *gunner = CutsceneHBH_GetEntity(HBH_GUNNER);
        if (gunner) {
            RSDK.SetSpriteAnimation(gunner->spriteIndex, 4, &gunner->animator, true, 0);
            gunner->state = CutsceneHBH_Unknown12;
        }

        EntityCutsceneHBH *king = CutsceneHBH_GetEntity(HBH_KING);
        if (king) {
            king->direction ^= FLIP_X;
            king->state = CutsceneHBH_Unknown18;
        }
        RSDK.PlaySFX(GHZ2Outro->sfxRocketJet, 0, 255);
        foreach_active(Player, player) { player->down = false; }
        EntityPhantomRuby *ruby = (EntityPhantomRuby *)entity->phantomRuby;
        ruby->startPos.x        = ruby->position.x;
        ruby->startPos.y        = ruby->position.y;
        ruby->state             = PhantomRuby_Unknown5;
        return true;
    }
    return false;
}
bool32 GHZ2Outro_CutsceneState2_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);
    EntityPlayer *player2   = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)entity->phantomRuby;

    if (ruby) {
        if (ruby->state == PhantomRuby_Unknown5) {
            if (player2->objectID == Player->objectID && player2->characterID == ID_TAILS) {
                player2->state = Player_State_None;
                RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_SKID, &player2->playerAnimator, false, 0);
            }
            return true;
        }
    }
    return false;
}
bool32 GHZ2Outro_CutsceneState2_Unknown5(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)entity->phantomRuby;
    if (!host->timer)
        PhantomRuby_Unknown2(ruby);

    if (ruby->flag) {
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
        return true;
    }
    return false;
}
bool32 GHZ2Outro_CutsceneState2_Unknown6(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);
    EntityPhantomRuby *ruby = (EntityPhantomRuby *)entity->phantomRuby;
    EntityPlayer *player1  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2  = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    EntityFXRuby *fxRuby = NULL;
    if (host->timer) {
        fxRuby = (EntityFXRuby *)entity->fxRuby;
    }
    else {
        fxRuby            = (EntityFXRuby *)RSDK.CreateEntity(FXRuby->objectID, 0, ruby->position.x, ruby->position.y);
        fxRuby->drawOrder = Zone->playerDrawHigh;
        entity->fxRuby    = (Entity *)fxRuby;
        Camera_ShakeScreen(4, 0, 4);
        player1->drawOrder = Zone->playerDrawHigh + 1;
        if (player2->objectID == Player->objectID)
            player2->drawOrder = Zone->playerDrawHigh + 1;
    }

    if (!host->field_6C[0]) {
        if (fxRuby->flag) {
            if (host->field_68) {
                if (host->timer == host->field_68 + 48) {
                    fxRuby->field_74 = 64;
                    fxRuby->state     = FXRuby_Unknown6;
                    PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
                    Camera_ShakeScreen(4, 0, 4);
                }
                else if (host->timer == host->field_68 + 180) {
                    fxRuby->field_74 = 32;
                    fxRuby->state     = FXRuby_Unknown6;
                    PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
                    Camera_ShakeScreen(4, 0, 4);
                    Music_FadeOut(0.025);
                    host->field_68    = host->timer;
                    host->field_6C[0] = true;
                }
            }
            else {
                host->field_68 = host->timer;
            }

            if (host->timer >= host->field_68 + 52) {
                int id = 0;
                for (int angle = 0; angle < 0x80; angle += 0x10) {
                    EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
                    if (!player || player->objectID == TYPE_BLANK)
                        break;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);

                    int valX = (ruby->position.x - 0x400000) - player->position.x;
                    int valY = (ruby->position.y - (0xA00000 + 944)) - player->position.y;

                    player->position.x += ((RSDK.Cos256(2 * (angle + host->timer - host->field_68)) << 12) + valX) >> 5;
                    player->position.y += ((RSDK.Sin256(2 * (angle + host->timer - host->field_68)) << 12) + valY) >> 5;
                    player->state          = Player_State_None;
                    player->tileCollisions = false;
                    player->onGround       = false;
                }
            }
        }
    }
    else {
        if (fxRuby->fadeWhite >= 512) {
            if (fxRuby->fadeBlack >= 512) {
                if (host->timer == host->field_68 + 150)
                    return true;
            }
            else {
                fxRuby->fadeBlack += 16;
            }
        }
        else {
            fxRuby->fadeWhite += 16;
        }
    }

    return false;
}

bool32 GHZ2Outro_LoadNextStage(EntityCutsceneSeq *host)
{
    if (host->timer == 16) {
        globals->enableIntro = true;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.LoadScene("Encore Mode", "Chemical Plant Zone+ 1");
        else
#endif
            RSDK.LoadScene("Mania Mode", "Chemical Plant Zone 1");
        RSDK.InitSceneLoad();
        return true;
    }
    return false;
}

void GHZ2Outro_Cutscene_SkipCB(void)
{
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        RSDK.LoadScene("Encore Mode", "Chemical Plant Zone+ 1");
    else
#endif
        RSDK.LoadScene("Mania Mode", "Chemical Plant Zone 1");
}

void GHZ2Outro_EditorDraw(void) {}

void GHZ2Outro_EditorLoad(void) {}

void GHZ2Outro_Serialize(void) {}
