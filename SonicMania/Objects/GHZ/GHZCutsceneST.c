// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GHZCutsceneST Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGHZCutsceneST *GHZCutsceneST;

void GHZCutsceneST_Update(void)
{
    RSDK_THIS(GHZCutsceneST);

    if (!self->setupKnuxCutscene) {
        GHZCutsceneST_SetupKnuxCutscene();
        self->setupKnuxCutscene = true;
    }

    if (!self->activated) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox) && !player->sidekick) {
                CutsceneSeq_StartSequence(self, GHZCutsceneST_Cutscene_FadeIn, GHZCutsceneST_Cutscene_FinishRubyWarp, GHZCutsceneST_Cutscene_ExitHBH,
                                          GHZCutsceneST_Cutscene_SetupGHZ1, StateMachine_None);

#if MANIA_USE_PLUS
                CutsceneSeq_SetSkipType(SKIPTYPE_CALLBACK, GHZCutsceneST_Cutscene_SkipCB);
#endif

                self->activated = true;
            }
        }
    }
    ++self->timer;
}

void GHZCutsceneST_LateUpdate(void) {}

void GHZCutsceneST_StaticUpdate(void) {}

void GHZCutsceneST_Draw(void) {}

void GHZCutsceneST_Create(void *data)
{
    RSDK_THIS(GHZCutsceneST);
    if (!SceneInfo->inEditor) {
        INIT_ENTITY(self);
        CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
        self->active = ACTIVE_BOUNDS;

        GHZCutsceneST_SetupObjects();

        SceneInfo->timeEnabled = false;
    }
}

void GHZCutsceneST_StageLoad(void) {}

void GHZCutsceneST_SetupObjects(void)
{
    foreach_all(Platform, platform)
    {
        if (!platform->frameID) {
            platform->drawGroup     = Zone->objectDrawGroup[1] - 1;
            GHZCutsceneST->platform = platform;
            foreach_break;
        }
    }

    foreach_all(AIZKingClaw, claw)
    {
        GHZCutsceneST->claw = claw;
        foreach_break;
    }

    foreach_all(PhantomRuby, phantomRuby)
    {
        GHZCutsceneST->phantomRuby = phantomRuby;
        foreach_break;
    }

    foreach_all(FXRuby, fxRuby)
    {
        GHZCutsceneST->fxRuby = fxRuby;
        fxRuby->state         = StateMachine_None;
        fxRuby->fadeBlack     = 0x200;
        fxRuby->fadeWhite     = 0x200;
        fxRuby->outerRadius   = ScreenInfo->size.x;
        fxRuby->timer         = 64;
        foreach_break;
    }

    foreach_all(CutsceneHBH, cutsceneHBH) { GHZCutsceneST->cutsceneHBH[cutsceneHBH->characterID] = cutsceneHBH; }
}

void GHZCutsceneST_SetupKnuxCutscene(void)
{
    if (CHECK_CHARACTER_ID(ID_KNUCKLES, 1)) {
        EntityPlatform *platform       = GHZCutsceneST->platform;
        EntityAIZKingClaw *claw        = GHZCutsceneST->claw;
        EntityFXRuby *fxRuby           = GHZCutsceneST->fxRuby;
        EntityPhantomRuby *phantomRuby = GHZCutsceneST->phantomRuby;

        GHZCutsceneST->cutsceneHBH[0]->position.y += 704 << 16;
        GHZCutsceneST->cutsceneHBH[1]->position.y += 704 << 16;
        GHZCutsceneST->cutsceneHBH[2]->position.y += 704 << 16;
        GHZCutsceneST->cutsceneHBH[3]->position.y += 704 << 16;
        GHZCutsceneST->cutsceneHBH[4]->position.y += 704 << 16;
        fxRuby->position.y += 704 << 16;
        phantomRuby->position.y += 704 << 16;
        platform->position.y += 704 << 16;
        platform->drawPos.x   = platform->position.x;
        platform->drawPos.y   = platform->position.y;
        platform->centerPos.x = platform->position.x;
        platform->centerPos.y = platform->position.y;
        claw->position.y += 704 << 16;
    }
}

#if MANIA_USE_PLUS
void GHZCutsceneST_Cutscene_SkipCB(void)
{
#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        RSDK.SetScene("Encore Mode", "Green Hill Zone+ 1");
    else
#endif
        RSDK.SetScene("Mania Mode", "Green Hill Zone 1");
}
#endif

bool32 GHZCutsceneST_Cutscene_FadeIn(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    EntityFXRuby *fxRuby    = GHZCutsceneST->fxRuby;
    EntityPhantomRuby *ruby = GHZCutsceneST->phantomRuby;

    if (host->timer) {
        if (host->timer >= 60) {
            if (fxRuby->fadeBlack <= 0) {
                if (fxRuby->fadeWhite <= 0) {
                    PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
                    return true;
                }
                fxRuby->fadeWhite -= 16;
            }
            else {
                fxRuby->fadeBlack -= 16;
            }
        }
    }
    else {
        player1->position.y = ruby->position.y;
        player1->state      = Player_State_Static;
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        if (player2->classID == Player->classID) {
            player2->position.y = ruby->position.y;
            player2->state      = Player_State_Static;
            player2->stateInput = StateMachine_None;
        }
    }

    int32 id = 0;
    for (int32 angle = 0; angle < 0x80; angle += 0x40) {
        EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
        if (!player || player->classID == TYPE_BLANK)
            break;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, 0, 0);
        player->position.x += (player->position.x - player->position.x) >> 3;
        player->position.y += (0xA00 * RSDK.Sin256(2 * (host->timer + angle - host->storedTimer)) + ruby->position.y - player->position.y) >> 3;
        player->state = Player_State_Static;
    }

    return false;
}
bool32 GHZCutsceneST_Cutscene_FinishRubyWarp(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityPhantomRuby *ruby = GHZCutsceneST->phantomRuby;
    EntityFXRuby *fxRuby    = GHZCutsceneST->fxRuby;
    if (!host->timer)
        fxRuby->state = FXRuby_State_Shrinking;

    EntityPlayer **curPlayer = &player1;
    if (fxRuby->outerRadius <= 0) {
        int32 id = 0;
        for (int32 angle = 0; angle < 0x80; angle += 0x10) {
            EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
            if (!player || player->classID == TYPE_BLANK)
                break;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
            int32 x            = (player->position.x - player->position.x) >> 3;
            int32 y            = (0xA00 * RSDK.Sin256(2 * (angle + host->timer - host->storedTimer)) + ruby->position.y - player->position.y) >> 3;
            player->velocity.x = (x >> 8) * (x >> 8);
            player->velocity.y = (y >> 8) * (y >> 8);
            player->state      = Player_State_Air;
            player->up         = true;
            player->camera     = NULL;
            ++curPlayer;
        }

        host->values[0] = true;
        Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, camera->position.x, camera->position.y, 0);
        return true;
    }
    else {
        int32 id = 0;
        for (int32 angle = 0; angle < 0x80; angle += 0x10) {
            EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
            if (!player || player->classID == TYPE_BLANK)
                break;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, 0, 0);
            player->position.x += (player->position.x - player->position.x) >> 3;
            player->position.y += (0xA00 * RSDK.Sin256(2 * (host->timer + angle - host->storedTimer)) + ruby->position.y - player->position.y) >> 3;
            player->state = Player_State_Static;
            ++curPlayer;
        }
        return false;
    }
    return false;
}
bool32 GHZCutsceneST_Cutscene_ExitHBH(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    EntityPhantomRuby *ruby  = GHZCutsceneST->phantomRuby;
    EntityAIZKingClaw *claw  = GHZCutsceneST->claw;
    EntityPlatform *platform = GHZCutsceneST->platform;

    if (host->timer >= 60) {
        if (host->timer == 60) {
            int32 id = 0;
            for (int32 i = 0; i < 2; ++i) {
                EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
                if (!player || player->classID == TYPE_BLANK)
                    break;
                player->up = false;
            }

            Music_FadeOut(0.025);
        }

        if (host->timer == 180) {
            player1->camera = camera;
            Camera_SetTargetEntity(0, player1);
            return true;
        }
        else {
            claw->velocity.y -= 0x1800;
            for (int32 hbhChar = 0; hbhChar < 5; ++hbhChar) {
                EntityCutsceneHBH *hbh = GHZCutsceneST->cutsceneHBH[hbhChar];
                switch (hbhChar) {
                    case HBH_GUNNER:
                        hbh->velocity.x += 0x800;
                        hbh->velocity.y -= 0x1800;
                        hbh->position.x += hbh->velocity.x;
                        hbh->position.y += hbh->velocity.y;
                        break;

                    case HBH_SHINOBI:
                        if (host->timer == 60) {
                            RSDK.SetSpriteAnimation(hbh->aniFrames, 3, &hbh->mainAnimator, true, 0);
                            RSDK.SetSpriteAnimation(-1, 0, &hbh->fxAnimator, true, 0);
                        }
                        hbh->position.x -= 0x4000;
                        hbh->position.y -= 0x40000;
                        break;

                    case HBH_MYSTIC:
                        hbh->velocity.x += 0x2000;
                        hbh->velocity.y -= 0x1000;
                        hbh->position.x += hbh->velocity.x;
                        hbh->position.y += hbh->velocity.y;
                        break;

                    case HBH_RIDER:
                    case HBH_KING:
                        hbh->position.x += claw->velocity.x;
                        hbh->position.y += claw->velocity.y;
                        break;

                    default: break;
                }
            }

            ruby->position.x += claw->velocity.x;
            ruby->position.y += claw->velocity.y;
            platform->position.x += claw->velocity.x;
            platform->position.y += claw->velocity.y;
            platform->drawPos.x   = platform->position.x;
            platform->drawPos.y   = platform->position.y;
            platform->centerPos.x = platform->position.x;
            platform->centerPos.y = platform->position.y;
            claw->position.x += claw->velocity.x;
            claw->position.y += claw->velocity.y;
        }
    }

    return false;
}

bool32 GHZCutsceneST_Cutscene_SetupGHZ1(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZCutsceneST);

#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        RSDK.SetScene("Encore Mode", "");
    else
#endif
        RSDK.SetScene("Mania Mode", "");

    globals->parallaxOffset[0] = self->timer;
    EntityPlayer *player       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player->onGround           = true;
    player->state              = Player_State_Ground;
    Zone_StoreEntities((ScreenInfo->position.x + ScreenInfo->center.x) << 16, (ScreenInfo->size.y + ScreenInfo->position.y) << 16);
    RSDK.LoadScene();
    return true;
}

#if GAME_INCLUDE_EDITOR
void GHZCutsceneST_EditorDraw(void)
{
    RSDK_THIS(GHZCutsceneST);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void GHZCutsceneST_EditorLoad(void) {}
#endif

void GHZCutsceneST_Serialize(void) { RSDK_EDITABLE_VAR(GHZCutsceneST, VAR_VECTOR2, size); }
