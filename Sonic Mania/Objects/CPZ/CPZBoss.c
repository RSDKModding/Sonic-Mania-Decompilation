#include "SonicMania.h"
#include <time.h>

ObjectCPZBoss *CPZBoss;

void CPZBoss_Update(void)
{
    RSDK_THIS(CPZBoss);
    StateMachine_Run(self->state);
}

void CPZBoss_LateUpdate(void) {}

void CPZBoss_StaticUpdate(void) {}

void CPZBoss_Draw(void)
{
    RSDK_THIS(CPZBoss);
    RSDK.DrawSprite(&self->playerAnimator, NULL, false);
    RSDK.DrawSprite(&self->enterAnimator, &self->startPos, false);
    RSDK.DrawSprite(&self->panelAnimator, &self->startPos, false);
}

void CPZBoss_Create(void *data)
{
    RSDK_THIS(CPZBoss);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        else {
            self->drawOrder     = Zone->playerDrawLow;
            self->startPos.x    = self->position.x;
            self->startPos.y    = self->position.y;
            self->active        = ACTIVE_BOUNDS;
            self->visible       = true;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            if (self->type == CPZBOSS_PLAYER) {
                self->drawFX = FX_FLIP;
                self->state  = CPZBoss_State_SetupArena;
            }
            else {
                RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 2, &self->playerAnimator, true, 0);
                RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 0, &self->panelAnimator, true, 0);
            }
        }
    }
}

void CPZBoss_StageLoad(void)
{
    CPZBoss->aniFrames     = RSDK.LoadSpriteAnimation("Eggman/EggmanCPZ.bin", SCOPE_STAGE);
    CPZBoss->hitbox.left   = -16;
    CPZBoss->hitbox.top    = 8;
    CPZBoss->hitbox.right  = 16;
    CPZBoss->hitbox.bottom = 16;
    CPZBoss->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
    RSDK.SetDrawLayerProperties(Zone->drawOrderHigh, false, NULL);
    RSDK.SetDrawLayerProperties(Zone->drawOrderHigh + 1, false, NULL);
}

void CPZBoss_DrawLayerCB_SetupPuyoDropperClip(void) { RSDK.SetClipBounds(0, 0, 24, ScreenInfo->width, ScreenInfo->height); }
void CPZBoss_DrawLayerCB_RemovePuyoDropperClip(void) { RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height); }

bool32 CPZBoss_CheckMatchReset(void)
{
    RSDK_THIS(CPZBoss);
    if (!RSDK.CheckOnScreen(self, NULL)) {
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh, false, NULL);
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh + 1, false, NULL);
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        PuyoBean->field_20[0] = 0;
        PuyoBean->field_28[0] = 0;
        PuyoBean->field_20[1] = 0;
        PuyoBean->field_28[1] = 0;
        for (int i = 0; i < 0x100; ++i) PuyoBean->gameState[i] = NULL;
        foreach_all(PuyoBean, bean) { destroyEntity(bean); }
        int32 layerID = RSDK.GetSceneLayerID("FG High");
        RSDK.CopyTileLayer(layerID, 438, 150, layerID, 452, 150, 6, 2);
        SceneInfo->timeEnabled = true;
        CPZBoss_Create(NULL);
        return true;
    }
    return false;
}

void CPZBoss_State_SetupArena(void)
{
    RSDK_THIS(CPZBoss);
    if (++self->timer >= 8) {
        SceneInfo->timeEnabled = false;
        Music_SetMusicTrack("BossPuyo.ogg", TRACK_EGGMAN2, 846720);
        Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
        Entity *entPtr = RSDK.GetEntityByID(SceneInfo->entitySlot + 1);
        Camera_SetupLerp(2, 0, entPtr->position.x, entPtr->position.y, 8);
        foreach_active(HUD, hud)
        {
            CPZBoss->hudSlotID = RSDK.GetEntityID(hud);
            hud->state         = HUD_State_GoOffScreen;
        }

        foreach_active(Player, player)
        {
            if (player->sidekick)
                player->active = ACTIVE_NEVER;
            else
                player->velocity.y = 0;
        }
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMTails.bin", SCOPE_STAGE); break;
            case ID_KNUCKLES: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMKnux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMMighty.bin", SCOPE_STAGE); break;
            case ID_RAY: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMRay.bin", SCOPE_STAGE); break;
#endif
            default: CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMSonic.bin", SCOPE_STAGE); break;
        }
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &self->enterAnimator, true, 0);
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 0, &self->panelAnimator, true, 0);
        self->timer  = 0;
        self->active = ACTIVE_NORMAL;
        self->state  = CPZBoss_State_EnterPlayer;
    }
}

void CPZBoss_State_EnterPlayer(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset() && ++self->timer >= 30) {
        foreach_active(Player, player)
        {
            if (!player->sidekick)
                player->velocity.y = 0x80000;
        }
        self->state = CPZBoss_State_CheckPlayerReady;
    }
}

void CPZBoss_State_CheckPlayerReady(void)
{
    RSDK_THIS(CPZBoss);
    if (!CPZBoss_CheckMatchReset()) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &CPZBoss->hitbox)) {
                if (player->sidekick) {
                    player->state = Player_State_Air;
                }
                else {
                    player->visible    = false;
                    player->active     = ACTIVE_NEVER;
                    player->up         = false;
                    player->down       = false;
                    player->left       = false;
                    player->right      = false;
                    player->jumpPress  = false;
                    player->jumpHold   = false;
                    player->position.x = self->position.x;
                    player->position.y = self->position.y;
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &self->enterAnimator, true, 0);
                    RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &self->playerAnimator, true, 1);
                    self->direction = FLIP_X;
                    self->state     = CPZBoss_State_PlayPlayerEnterAnim;
                }
            }
        }
    }
}

void CPZBoss_State_PlayPlayerEnterAnim(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset()) {
        RSDK.ProcessAnimation(&self->playerAnimator);
        if (self->playerAnimator.frameID == self->playerAnimator.frameCount - 1) {
            foreach_active(CPZBoss, boss)
            {
                if (boss->type == CPZBOSS_EGGMAN) {
                    RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 4, &boss->playerAnimator, true, 0);
                    boss->state = CPZBoss_State_HandleMatch_Eggman;
                }
            }
            self->state = CPZBoss_State_SetupMatch;
        }
    }
}

void CPZBoss_State_SetupMatch(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset() && ++self->timer == 60) {
        foreach_active(CPZShutter, shutter) { shutter->state = CPZShutter_State_Open; }
        int32 key  = (int32)time(NULL);
        int32 rand = RSDK.RandSeeded(0, 512, &key);

        int32 id = 0;
        foreach_active(PuyoMatch, match)
        {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &match->animator2, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &match->animator1, true, 0);
            match->state    = PuyoMatch_State_HandleMatch;
            match->matchKey = rand;
            PuyoMatch_SetupNextBeans(match);
            if (match->playerID == 1) {
                match->stateInput                        = PuyoAI_StateInput;
                PuyoAI->value3[match->playerID]          = false;
                PuyoAI->value4[match->playerID]          = 14;
                PuyoAI->targetX[match->playerID]         = 0;
                PuyoAI->value6[match->playerID]          = 0;
                PuyoAI->value3[match->playerID]          = true;
                PuyoAI->controlInterval[match->playerID] = 16;
                PuyoAI->controlChance[match->playerID]   = 50;
            }
            CPZBoss->managers[id++] = (Entity *)match;
        }
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 2, &self->playerAnimator, false, 0);
        self->state = CPZBoss_State_HandleMatch_Player;
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh, false, CPZBoss_DrawLayerCB_SetupPuyoDropperClip);
        RSDK.SetDrawLayerProperties(Zone->drawOrderHigh + 1, false, CPZBoss_DrawLayerCB_RemovePuyoDropperClip);
    }
}

void CPZBoss_State_HandleMatch_Player(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset()) {
        EntityPuyoMatch *manager = (EntityPuyoMatch *)CPZBoss->managers[self->type];
        RSDK.ProcessAnimation(&self->playerAnimator);
        if (self->playerAnimator.animationID == 2) {
            if (manager) {
                EntityPuyoBean *bean = manager->beanPtr;
                if (bean) {
                    if (bean->state == PuyoBean_Unknown19) {
                        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 3, &self->playerAnimator, false, 0);
                    }
                    else {
                        if (bean->down)
                            self->playerAnimator.frameID = 3;
                        else if (bean->left)
                            self->playerAnimator.frameID = 1;
                        else if (bean->right)
                            self->playerAnimator.frameID = 2;
                        if (bean->rotateRight)
                            self->playerAnimator.frameID = 4;
                        else if (bean->rotateLeft)
                            self->playerAnimator.frameID = 5;
                    }
                }
            }
        }
        else if (self->playerAnimator.animationID == 3 && self->playerAnimator.frameID == self->playerAnimator.frameCount - 1) {
            RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 2, &self->playerAnimator, false, 0);
        }

        if (ControllerInfo[CONT_P1].keyStart.press) {
            if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID == TYPE_BLANK) {
                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                RSDK.PlaySfx(PauseMenu->sfxAccept, 0, 255);
                RSDK.SetGameMode(ENGINESTATE_FROZEN);
            }
        }
        if (manager) {
            if (manager->state == PuyoMatch_State_Lose) {
                PuyoBean_DestroyPuyoBeans();
                self->timer       = 0;
                self->state       = CPZBoss_State_HandleMatchFinish_PlayerLose;
                PuyoAI->value3[0] = false;
                PuyoAI->value3[1] = false;
                RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 3, &self->playerAnimator, false, 0);
            }
        }
    }
}

void CPZBoss_State_HandleMatch_Eggman(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset()) {
        EntityPuyoMatch *manager = (EntityPuyoMatch *)CPZBoss->managers[self->type];
        if (manager || true) {
            RSDK.ProcessAnimation(&self->playerAnimator);
            if (self->playerAnimator.animationID == 2) {
                EntityPuyoBean *bean = manager->beanPtr;
                if (bean && bean->state == PuyoBean_Unknown19)
                    RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 3, &self->playerAnimator, false, 0);
            }
            else {
                if (self->playerAnimator.animationID == 3 || self->playerAnimator.animationID == 4) {
                    if (self->playerAnimator.frameID == self->playerAnimator.frameCount - 1)
                        RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 2, &self->playerAnimator, false, 0);
                }
            }
            if (manager) {
                if (manager->state == PuyoMatch_State_Lose) {
                    PuyoBean_DestroyPuyoBeans();
                    self->state          = CPZBoss_State_HandleMatchFinish_EggmanLose;
                    self->explosionPos.x = self->position.x + 0x400000;
                    self->explosionPos.y = (ScreenInfo->height + ScreenInfo->position.y) << 16;
                    RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 3, &self->playerAnimator, false, 0);
                }
            }
        }
    }
}

void CPZBoss_State_HandleMatchFinish_EggmanLose(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset()) {
        RSDK.ProcessAnimation(&self->playerAnimator);
        if (!(Zone->timer % 3)) {
            RSDK.PlaySfx(CPZBoss->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int32 x = self->explosionPos.x + RSDK.Rand(-0x300000, 0x300000);
                int32 y = self->explosionPos.y + RSDK.Rand(-0x100000, 0x100000);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->hudDrawOrder;
            }
        }
        self->explosionPos.y -= 0x40000;
        if (self->explosionPos.y < self->position.y - 0xD00000) {
            RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 5, &self->playerAnimator, false, 0);
            self->state = CPZBoss_State_PlayerWin;
            foreach_active(CPZShutter, shutter) { shutter->state = CPZShutter_State_Close; }
        }
    }
}

void CPZBoss_State_PlayerWin(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset()) {
        RSDK.ProcessAnimation(&self->playerAnimator);
        if (!(Zone->timer % 3)) {
            RSDK.PlaySfx(CPZBoss->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int32 x = self->position.x + RSDK.Rand(-0x100000, 0x100000);
                int32 y = self->position.y + RSDK.Rand(-0x100000, 0x100000);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->hudDrawOrder;
            }
        }

        if (++self->timer == 60) {
            self->timer  = 0;
            self->active = ACTIVE_NORMAL;
            self->state  = CPZBoss_State_EggmanFall;
            RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &self->enterAnimator, true, 0);
            RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 6, &self->playerAnimator, false, 0);
            Music_TransitionTrack(TRACK_STAGE, 0.0125);

            RSDK.PlaySfx(PuyoBean->sfxFall, false, 255);
            foreach_active(TippingPlatform, platform)
            {
                if (platform->bossID == TIPPINGPLATFORM_EGGMAN) {
                    platform->state = TippingPlatform_State_Tipping_Boss;
                    platform->timer = 120;
                }
            }

            foreach_active(CPZBoss, boss)
            {
                if (!boss->type) {
                    boss->state = CPZBoss_State_PlayerExit;
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &boss->playerAnimator, false, 0);
                    RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &boss->enterAnimator, true, 0);
                    foreach_all(Player, player)
                    {
                        player->up         = false;
                        player->down       = false;
                        player->left       = false;
                        player->right      = false;
                        player->velocity.x = 0;
                        player->velocity.y = 0;
                        player->groundVel  = 0;
                        if (!player->sidekick) {
                            player->stateInput = StateMachine_None;
                        }
                        player->visible    = true;
                        player->active     = ACTIVE_NORMAL;
                        player->position.x = boss->position.x;
                        player->position.y = boss->position.y;
                    }
                }
            }
        }
    }
}

void CPZBoss_State_HandleMatchFinish_PlayerLose(void)
{
    RSDK_THIS(CPZBoss);

    if (!CPZBoss_CheckMatchReset()) {
        RSDK.ProcessAnimation(&self->playerAnimator);
        if (++self->timer == 60) {
            self->timer = 0;
            self->state = (Type_StateMachine)CPZBoss_CheckMatchReset;
            RSDK.PlaySfx(PuyoBean->sfxFall, false, 255);
            foreach_active(TippingPlatform, platform)
            {
                if (platform->bossID == TIPPINGPLATFORM_PLAYER) {
                    platform->state = TippingPlatform_State_Tipping_Boss;
                    platform->timer = 120;
                }
            }

            foreach_all(Player, player)
            {
                player->visible    = true;
                player->active     = ACTIVE_NORMAL;
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->drawOrder  = Zone->playerDrawLow;
                player->state      = Player_State_Air;
                player->onGround   = false;
                player->velocity.y = -0x20000;
                RSDK.AddDrawListRef(Zone->playerDrawLow, RSDK.GetEntityID(&player));
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->playerAnimator, false, 0);
                RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &self->enterAnimator, true, 0);
            }

            foreach_active(CPZShutter, shutter)
            {
                shutter->active = ACTIVE_NORMAL;
                shutter->state  = CPZShutter_State_Close;
            }

            EntityHUD *hud = RSDK_GET_ENTITY(CPZBoss->hudSlotID, HUD);
            RSDK.ResetEntityPtr(hud, HUD->objectID, NULL);
            hud->maxOffset = hud->offsets[HUDOFF_SCORE].x;
            hud->offsets[HUDOFF_SCORE].x -= 0x1000000;
            hud->offsets[HUDOFF_TIME].x -= 0x1100000;
            hud->offsets[HUDOFF_RINGS].x -= 0x1200000;
            hud->offsets[HUDOFF_LIFE].x -= 0x1300000;
            hud->state = HUD_State_ComeOnScreen;
        }
    }
}

void CPZBoss_State_EggmanFall(void)
{
    RSDK_THIS(CPZBoss);

    RSDK.ProcessAnimation(&self->playerAnimator);
    if (self->timer > 32) {
        self->velocity.y += 0x2800;
        self->position.y += self->velocity.y;
    }
    self->timer++;
    if (self->timer == 95)
        self->state = StateMachine_None;
}

void CPZBoss_State_PlayerExit(void)
{
    RSDK_THIS(CPZBoss);
    if (++self->timer == 240) {
        self->timer = 0;
        foreach_active(Player, player)
        {
            player->state           = Player_State_Air;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->jumpAbility     = 0;
            player->onGround        = false;
            player->position.x      = self->position.x;
            player->position.y      = self->position.y;
            player->velocity.x      = 0;
            player->velocity.y      = -0x80000;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            Zone->playerBoundActiveR[player->playerID] = true;
        }

        EntityTransportTube *tube = RSDK_GET_ENTITY(SceneInfo->entitySlot + 3, TransportTube);
        tube->dirMask             = TRANSPORTTUBE_DIR_S | TRANSPORTTUBE_DIR_E;
        TransportTube_SetupDirections(tube);

        EntityHUD *hud = RSDK_GET_ENTITY(CPZBoss->hudSlotID, HUD);
        RSDK.ResetEntityPtr(hud, HUD->objectID, NULL);
        hud->maxOffset = hud->offsets[HUDOFF_SCORE].x;
        hud->offsets[HUDOFF_SCORE].x -= 0x1000000;
        hud->offsets[HUDOFF_TIME].x -= 0x1100000;
        hud->offsets[HUDOFF_RINGS].x -= 0x1200000;
        hud->offsets[HUDOFF_LIFE].x -= 0x1300000;
        hud->state = HUD_State_ComeOnScreen;

        self->active = ACTIVE_NORMAL;
        self->state  = CPZBoss_State_Destroyed;
    }
}

void CPZBoss_State_Destroyed(void)
{
    RSDK_THIS(CPZBoss);

    if (++self->timer == 24) {
        for (int32 i = 0; i < Player->playerCount; ++i) {
            EntityPlayer *player = RSDK_GET_ENTITY(i, Player);
            if (player->camera)
                player->camera->state = Camera_State_Follow;
            if (!player->sidekick)
                player->stateInput = Player_ProcessP1Input;
        }
        self->active = ACTIVE_BOUNDS;
        self->state  = StateMachine_None;
    }
}

#if RETRO_INCLUDE_EDITOR
void CPZBoss_EditorDraw(void)
{
    RSDK_THIS(CPZBoss);
    self->startPos = self->position;

    if (self->type == CPZBOSS_PLAYER) {
        self->drawFX = FX_FLIP;
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 1, &self->enterAnimator, true, 0);
        RSDK.SetSpriteAnimation(CPZBoss->playerFrames, 0, &self->panelAnimator, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->playerAnimator, true, 0);
    }
    else {
        RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 2, &self->playerAnimator, true, 0);
        RSDK.SetSpriteAnimation(CPZBoss->aniFrames, 0, &self->panelAnimator, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->enterAnimator, true, 0);
    }

    CPZBoss_Draw();
}

void CPZBoss_EditorLoad(void)
{
    CPZBoss->aniFrames    = RSDK.LoadSpriteAnimation("Eggman/EggmanCPZ.bin", SCOPE_STAGE);
    CPZBoss->playerFrames = RSDK.LoadSpriteAnimation("CPZ/MBMSonic.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(CPZBoss, type);
    RSDK_ENUM_VAR("Player", CPZBOSS_PLAYER);
    RSDK_ENUM_VAR("Eggman", CPZBOSS_EGGMAN);
}
#endif

void CPZBoss_Serialize(void) { RSDK_EDITABLE_VAR(CPZBoss, VAR_UINT8, type); }
