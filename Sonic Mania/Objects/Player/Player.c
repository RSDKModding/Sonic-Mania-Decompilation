// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Player Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPlayer *Player;

Hitbox Player_FallbackHitbox = { -10, -20, 10, 20 };

void Player_Update(void)
{
    RSDK_THIS(Player);

#if RETRO_USE_PLUS
    // Cheat prevention, you can't play as mighty or ray if you don't have plus installed & active
    if (!API.CheckDLC(DLC_PLUS) && self->characterID > ID_KNUCKLES)
        Player_ChangeCharacter(self, ID_SONIC);
#endif

    StateMachine_Run(self->stateInputReplay);
    StateMachine_Run(self->stateInput);

    if (self->classID == Player->classID) {
        if (self->camera) {
            if (self->scrollDelay > 0) {
                self->scrollDelay--;
                if (!self->scrollDelay)
                    self->camera->state = Camera_State_Follow;
            }
            else if (self->state != Player_State_LookUp && self->state != Player_State_Crouch) {
                if (self->camera->lookPos.y > 0)
                    self->camera->lookPos.y -= 2;
                else if (self->camera->lookPos.y < 0)
                    self->camera->lookPos.y += 2;
            }
        }

        if (self->invincibleTimer) {
            if (self->invincibleTimer <= 0) {
                self->invincibleTimer++;
            }
            else {
                self->invincibleTimer--;
                if (!self->invincibleTimer) {
                    Player_ApplyShield(self);

#if RETRO_USE_PLUS
                    if (globals->gameMode != MODE_ENCORE || !self->sidekick) {
#else
                    if (!self->sidekick) {
#endif
                        bool32 stopPlaying = true;
                        foreach_active(Player, player)
                        {
                            if (player->invincibleTimer > 0)
                                stopPlaying = false;
                        }

                        if (stopPlaying)
                            Music_EndQueuedTrack(TRACK_INVINCIBLE, true);
                    }
                }
            }
        }

        if (self->speedShoesTimer > 0) {
            self->speedShoesTimer--;
            if (!self->speedShoesTimer) {
                Player_UpdatePhysicsState(self);

                bool32 stopPlaying = true;
                foreach_active(Player, player)
                {
                    if (player->speedShoesTimer > 0)
                        stopPlaying = false;
                }

                if (stopPlaying)
                    Music_EndQueuedTrack(TRACK_SNEAKERS, true);
            }
        }

        if (self->state != Player_State_Hit) {
            if (self->blinkTimer > 0) {
                self->blinkTimer--;
                self->visible = !(self->blinkTimer & 4);
            }
        }

#if RETRO_USE_PLUS
        if (self->characterID == ID_RAY && self->state != Player_State_RayGlide && !self->isGhost) {
            Player->raySwoopTimer = 0;
            Player->rayDiveTimer  = 0;
        }

        if (self->uncurlTimer > 0)
            self->uncurlTimer--;
#endif

        Player_HandleSuperForm();
        if (self->characterID == ID_TAILS && self->state != Player_State_TailsFlight && self->abilitySpeed)
            self->abilitySpeed = 0;

        // Hurt Player if we're touching T/B or L/R sides at same time
        if (self->collisionFlagH == (1 | 2) || self->collisionFlagV == (1 | 2))
            self->deathType = PLAYER_DEATH_DIE_USESFX;

        self->collisionFlagH = 0;
        self->collisionFlagV = 0;
        if (self->collisionLayers & Zone->moveMask) {
            TileLayer *move  = RSDK.GetTileLayer(Zone->moveLayer);
            move->position.x = -self->moveLayerPosition.x >> 16;
            move->position.y = -self->moveLayerPosition.y >> 16;
        }

        if (self->forceJumpIn)
            self->state = Player_State_StartJumpIn;

        if (self->isTransforming)
            self->state = Player_State_Transform;

        StateMachine_Run(self->state);

        if (self->classID == Player->classID) {
            self->flailing      = false;
            self->invertGravity = false;

            if (self->outerbox) {
                self->groundedStore = self->onGround;
                RSDK.ProcessTileCollisions(self, self->outerbox, self->innerbox);
            }
            else {
                self->outerbox      = RSDK.GetHitbox(&self->animator, 0);
                self->innerbox      = RSDK.GetHitbox(&self->animator, 1);
                self->groundedStore = self->onGround;
                RSDK.ProcessTileCollisions(self, self->outerbox, self->innerbox);
                self->outerbox = NULL;
            }

            self->collisionLayers &= ~Zone->moveMask;
            if (self->onGround && !self->collisionMode)
                self->collisionFlagV |= 1;
        }
    }
}

void Player_LateUpdate(void)
{
    RSDK_THIS(Player);

    if (self->superState == SUPERSTATE_FADEIN && self->state != Player_State_Transform)
        Player_CheckGoSuper(self, 0x7F);

    if (self->state == Player_State_FlyCarried) {
        self->flyCarryLeaderPos.x = self->position.x >> 0x10 << 0x10;
        self->flyCarryLeaderPos.y = self->position.y >> 0x10 << 0x10;
    }

    if (self->deathType) {
        self->abilityValues[0] = 0;

#if RETRO_USE_PLUS
        if (!self->sidekick)
            RSDK.CopyEntity(Zone->entityStorage[1], self, false);
#endif

        bool32 stopInvincibility = self->sidekick || globals->gameMode == MODE_COMPETITION;
#if RETRO_USE_PLUS
        stopInvincibility |= globals->gameMode == MODE_ENCORE;
#endif

        if (stopInvincibility) {
            if (self->invincibleTimer > 1)
                self->invincibleTimer = 1;
            if (self->speedShoesTimer > 1)
                self->speedShoesTimer = 1;
        }
        else {
            self->invincibleTimer = 0;
            self->speedShoesTimer = 0;
        }

        if (self->gravityStrength <= 1)
            self->direction |= FLIP_Y;
        else
            self->direction &= ~FLIP_Y;

        self->visible         = true;
        self->onGround        = false;
        self->groundVel       = 0;
        self->velocity.x      = 0;
        self->nextGroundState = StateMachine_None;
        self->nextAirState    = StateMachine_None;
        self->interaction     = false;
        self->tileCollisions  = false;

        if (globals->gameMode != MODE_COMPETITION)
            self->active = ACTIVE_ALWAYS;

        self->shield         = SHIELD_NONE;
        self->collisionFlagH = 0;
        self->collisionFlagV = 0;
        self->underwater     = false;
        Player_UpdatePhysicsState(self);

        destroyEntity(RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(self), Shield));

        switch (self->deathType) {
            default: break;

            case PLAYER_DEATH_DIE_USESFX:
                RSDK.PlaySfx(Player->sfxHurt, false, 255);
                // [Fallthrough]
            case PLAYER_DEATH_DIE_NOSFX:
                self->deathType  = PLAYER_DEATH_NONE;
                self->velocity.y = -0x68000;
                self->state      = Player_State_Die;

                if (!(self->drawFX & FX_SCALE) || self->scale.x == 0x200)
                    self->drawOrder = Zone->playerDrawHigh;

                if (self->sidekick || globals->gameMode == MODE_COMPETITION) {
                    if (self->camera) {
                        self->scrollDelay   = 2;
                        self->camera->state = StateMachine_None;
                    }
                }
#if RETRO_USE_PLUS
                else if (globals->gameMode == MODE_ENCORE) {
                    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    if (!globals->stock && !sidekick->classID) {
                        RSDK.SetGameMode(ENGINESTATE_FROZEN);
                        SceneInfo->timeEnabled = false;
                    }

                    if (self->camera) {
                        self->scrollDelay   = 2;
                        self->camera->state = StateMachine_None;
                    }
                }
#endif
                else {
                    RSDK.SetGameMode(ENGINESTATE_FROZEN);
                    SceneInfo->timeEnabled = false;
                    if (self->camera) {
                        self->scrollDelay   = 2;
                        self->camera->state = StateMachine_None;
                    }
                }
                break;

            case PLAYER_DEATH_DROWN:
                self->deathType = PLAYER_DEATH_NONE;
                // Bug Details:
                // setting this actually causes a slight bug, as when you die the underwater flag is cleared but your gravity strength isn't updated
                // so if you debug out, you'll have regular speed with a moon jump
                // (yes I know this is super minor but its neat to know anyways)
                self->gravityStrength = 0x1000;
                self->velocity.y      = 0;
                RSDK.PlaySfx(Water->sfxDrown, false, 255);
                self->state = Player_State_Drown;

                if (!self->sidekick) {
                    if (globals->gameMode == MODE_COMPETITION) {
                        Music_EndQueuedTrack(TRACK_DROWNING, false);
                    }
#if RETRO_USE_PLUS
                    else if (globals->gameMode == MODE_ENCORE) {
                        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                        if (globals->stock == 0 && !sidekick->classID) {
                            SceneInfo->timeEnabled = false;
                        }
                    }
#endif
                    else {
                        SceneInfo->timeEnabled = false;
                    }
                }

                if (self->camera) {
                    self->scrollDelay   = 2;
                    self->camera->state = StateMachine_None;
                }
                break;
        }
    }

    if (self->onGround) {
        if (self->nextGroundState) {
            self->state           = self->nextGroundState;
            self->nextGroundState = StateMachine_None;
            if (self->sidekick) 
                RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->scoreBonus = 0;
            else 
                self->scoreBonus     = 0;
        }

        if (self->camera) {
            if (self->animator.animationID == ANI_JUMP)
                self->camera->adjustY = self->jumpOffset;
            else
                self->camera->adjustY = 0;
        }
    }
    else if (self->nextAirState) {
        self->state        = self->nextAirState;
        self->nextAirState = StateMachine_None;
    }

    if (self->tailFrames != (uint16)-1) {
        switch (self->animator.animationID) {
            case ANI_IDLE:
            case ANI_BORED1:
            case ANI_LOOKUP:
            case ANI_CROUCH:
                RSDK.SetSpriteAnimation(self->tailFrames, 0, &self->tailAnimator, false, 0);
                self->tailDirection = self->direction;
                self->tailRotation  = self->rotation;
                break;

            case ANI_JUMP:
                RSDK.SetSpriteAnimation(self->tailFrames, 1, &self->tailAnimator, false, 0);
                if (self->onGround) {
                    self->tailRotation  = self->rotation;
                    self->tailDirection = self->groundVel <= 0;
                }
                else {
                    self->tailRotation = RSDK.ATan2(self->velocity.x, self->velocity.y) << 1;
                    if (self->direction == FLIP_X)
                        self->tailRotation = (self->tailRotation - 256) & 0x1FF;
                    self->tailDirection = self->direction;
                }
                break;

            case ANI_SKID:
            case ANI_PUSH:
            case ANI_HANG:
                RSDK.SetSpriteAnimation(self->tailFrames, 3, &self->tailAnimator, false, 0);
                self->tailDirection = self->direction;
                break;

            case ANI_SPINDASH:
                RSDK.SetSpriteAnimation(self->tailFrames, 2, &self->tailAnimator, false, 0);
                self->tailDirection = self->direction;
                break;

            case ANI_HANGMOVE:
                RSDK.SetSpriteAnimation(self->tailFrames, 4, &self->tailAnimator, false, 0);
                self->tailDirection = self->direction;
                break;

            case ANI_PULLEYHOLD:
                RSDK.SetSpriteAnimation(self->tailFrames, 5, &self->tailAnimator, false, 0);
                self->tailDirection = self->direction;
                if (self->tailDirection)
                    self->tailRotation = self->rotation + 32;
                else
                    self->tailRotation = self->rotation - 32;
                break;

            default: RSDK.SetSpriteAnimation(-1, 0, &self->tailAnimator, false, 0); break;
        }

        RSDK.ProcessAnimation(&self->tailAnimator);
    }

    RSDK.ProcessAnimation(&self->animator);
}

void Player_StaticUpdate(void)
{
#if RETRO_USE_PLUS
    // Moved here from ERZ start since flying can now be done in any stage, not just ERZ
    if (Player->superDashCooldown > 0) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        foreach_all(HUD, hud)
        {
            if (hud)
                hud->enableRingFlash = true;
            foreach_break;
        }
        --Player->superDashCooldown;
    }
#endif

    bool32 flying = false;
    bool32 tired  = false;
    if (SceneInfo->state == ENGINESTATE_REGULAR) {
        foreach_active(Player, player)
        {
            if (player->characterID == ID_TAILS) {
                int32 anim = player->animator.animationID;
                if (anim == ANI_FLY || anim == ANI_FLYLIFT)
                    flying = true;
                if (anim == ANI_FLYTIRED || anim == ANI_FLYLIFTTIRED)
                    tired = true;
            }
        }

        if (flying) {
            if (!Player->playingFlySFX) {
                RSDK.PlaySfx(Player->sfxFlying, true, 255);
                Player->playingFlySFX = true;
            }
        }

        if (tired) {
            if (!Player->playingTiredSFX) {
                RSDK.PlaySfx(Player->sfxTired, true, 255);
                Player->playingTiredSFX = true;
            }
        }
    }

    if (!flying && Player->playingFlySFX) {
        RSDK.StopSfx(Player->sfxFlying);
        Player->playingFlySFX = false;
    }

    if (!tired && Player->playingTiredSFX) {
        RSDK.StopSfx(Player->sfxTired);
        Player->playingTiredSFX = false;
    }

#if RETRO_USE_PLUS
    Player->raySwoopTimer -= 8;
    if (Player->raySwoopTimer < 0)
        Player->raySwoopTimer = 0;

    Player->rayDiveTimer -= 16;
    if (Player->rayDiveTimer < 0)
        Player->rayDiveTimer = 0;
#endif
}

void Player_Draw(void)
{
    RSDK_THIS(Player);

#if RETRO_USE_PLUS
    if (self->isGhost) {
        self->inkEffect = INK_BLEND;
        self->alpha     = 0x7F; // redundant since the ink effect is INK_BLEND, which is basically 50% alpha anyways
    }
#endif

    Entity *parent   = self->abilityPtrs[0];
    Vector2 posStore = self->position;
    if (self->state == Player_State_FlyIn && parent) {
        self->position.x = parent->position.x;
        self->position.y = parent->position.y;
    }

    if (self->tailFrames != (uint16)-1) {
        int32 rotStore  = self->rotation;
        int32 dirStore  = self->direction;
        self->rotation  = self->tailRotation;
        self->direction = self->tailDirection;
        RSDK.DrawSprite(&self->tailAnimator, NULL, false);

        self->rotation  = rotStore;
        self->direction = dirStore;
    }

    // Copy the colours for any other players so stuff like super forms or etc don't effect the other players aside from P1
    // Used for sidekicks, competition players with the same player or etc
    if (self->playerID == 1 && (globals->playerID & 0xFF) == ((globals->playerID >> 8) & 0xFF) && globals->gameMode != MODE_TIMEATTACK) {
        uint32 colorStore[6];
        switch (self->characterID) {
            case ID_SONIC:
                for (int32 c = 0; c < 6; ++c) {
                    colorStore[c] = RSDK.GetPaletteEntry(0, c + 0x40);
                    RSDK.SetPaletteEntry(0, c + 0x40, Player->superPalette_Sonic[c]);
                }

                RSDK.DrawSprite(&self->animator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 0x40, colorStore[c]);
                break;

            case ID_TAILS:
                for (int32 c = 0; c < 6; ++c) {
                    colorStore[c] = RSDK.GetPaletteEntry(0, c + 0x46);
                    RSDK.SetPaletteEntry(0, c + 0x46, Player->superPalette_Tails[c]);
                }

                RSDK.DrawSprite(&self->animator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 0x46, colorStore[c]);
                break;

            case ID_KNUCKLES:
                for (int32 c = 0; c < 6; ++c) {
                    colorStore[c] = RSDK.GetPaletteEntry(0, c + 0x50);
                    RSDK.SetPaletteEntry(0, c + 0x50, Player->superPalette_Knux[c]);
                }

                RSDK.DrawSprite(&self->animator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 0x50, colorStore[c]);
                break;

#if RETRO_USE_PLUS
            case ID_MIGHTY:
                for (int32 c = 0; c < 6; ++c) {
                    colorStore[c] = RSDK.GetPaletteEntry(0, 0x60 + c);
                    RSDK.SetPaletteEntry(0, 0x60 + c, Player->superPalette_Mighty[c]);
                }

                RSDK.DrawSprite(&self->animator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, 0x60 + c, colorStore[c]);
                break;

            case ID_RAY:
                for (int32 c = 0; c < 6; ++c) {
                    colorStore[c] = RSDK.GetPaletteEntry(0, c + 113);
                    RSDK.SetPaletteEntry(0, c + 113, Player->superPalette_Ray[c]);
                }

                RSDK.DrawSprite(&self->animator, NULL, false);

                for (int32 c = 0; c < 6; ++c) RSDK.SetPaletteEntry(0, c + 113, colorStore[c]);
                break;
#endif
        }
    }
    else {
        // Draw the Player, no fancy stuff
        RSDK.DrawSprite(&self->animator, NULL, false);
    }

    if (self->state == Player_State_FlyIn && parent) {
        self->position.x = posStore.x;
        self->position.y = posStore.y;
    }
}

void Player_Create(void *data)
{
    RSDK_THIS(Player);

    if (SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Player->sonicFrames, ANI_IDLE, &self->animator, true, 0);
        self->characterID = ID_SONIC;
    }
    else {
        self->playerID = RSDK.GetEntityID(self);

        // Handle character specific stuff
        switch (self->characterID) {
            default:
            case ID_SONIC:
                self->aniFrames    = Player->sonicFrames;
                self->tailFrames   = -1;
                self->jumpOffset   = 0x50000;
                self->stateAbility = Player_JumpAbility_Sonic;
                self->sensorY      = 0x140000;

                if (globals->medalMods & getMod(MEDAL_PEELOUT)) {
                    self->statePeelout = Player_StartPeelout;
                    for (int32 f = 0; f < 4; ++f) {
                        SpriteFrame *dst = RSDK.GetFrame(self->aniFrames, ANI_DASH, f + 1);
                        SpriteFrame *src = RSDK.GetFrame(self->aniFrames, ANI_FLY, f);

                        *dst = *src;
                    }
                }
                break;

            case ID_TAILS:
                self->aniFrames    = Player->tailsFrames;
                self->tailFrames   = Player->tailsTailsFrames;
                self->jumpOffset   = 0;
                self->stateAbility = Player_JumpAbility_Tails;
                self->sensorY      = 0x100000;
                break;

            case ID_KNUCKLES:
                self->aniFrames    = Player->knuxFrames;
                self->tailFrames   = -1;
                self->jumpOffset   = 0x50000;
                self->stateAbility = Player_JumpAbility_Knux;
                self->sensorY      = 0x140000;
                break;

#if RETRO_USE_PLUS
            case ID_MIGHTY:
                self->aniFrames    = Player->mightyFrames;
                self->tailFrames   = -1;
                self->jumpOffset   = 0x50000;
                self->stateAbility = Player_JumpAbility_Mighty;
                self->sensorY      = 0x140000;
                break;

            case ID_RAY:
                self->aniFrames    = Player->rayFrames;
                self->tailFrames   = -1;
                self->jumpOffset   = 0x50000;
                self->stateAbility = Player_JumpAbility_Ray;
                self->sensorY      = 0x140000;
                break;
#endif
        }

        // Handle Sensor setup
        self->sensorX[0]     = 0xA0000;
        self->sensorX[1]     = 0x50000;
        self->sensorX[2]     = 0;
        self->sensorX[3]     = -0x50000;
        self->sensorX[4]     = -0xA0000;

        self->active         = ACTIVE_NORMAL;
        self->tileCollisions = true;
        self->visible        = true;
        self->drawOrder      = Zone->playerDrawLow;
        self->scale.x        = 0x200;
        self->scale.y        = 0x200;
        self->controllerID   = self->playerID + 1;
        self->state          = Player_State_Ground;

        // Handle Input Setup
        if (!SceneInfo->entitySlot || globals->gameMode == MODE_COMPETITION) {
            self->stateInput = Player_ProcessP1Input;
        }
#if RETRO_USE_PLUS
        else if (SceneInfo->entitySlot == 1 && globals->gameMode == MODE_TIMEATTACK) {
            StateMachine_Run(Player->configureGhostCB);
        }
#endif
        else {
            API_AssignControllerID(self->controllerID, CONT_AUTOASSIGN);
            self->stateInput = Player_ProcessP2Input_AI;
            self->sidekick   = true;
        }

        AnalogStickInfoL[self->controllerID].deadzone = 0.3;

        // Handle Powerups
        self->rings                                   = Player->rings;
        self->ringExtraLife                           = Player->ringExtraLife;
        Player->rings                                 = 0;
        Player->ringExtraLife                         = 100;
        self->hyperRing                               = (Player->powerups >> 6) & 1;
        Player->powerups &= ~0x40;
        if (Player->powerups >= 0x80) {
            self->state = Player_ForceSuperTransform;
            Player->powerups &= ~0x80;
        }

        if (Player->powerups) {
            self->shield = Player->powerups;
            Player_ApplyShield(self);
        }
        Player->powerups                  = 0;

        // Handle Lives/Score setup
        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        if (globals->gameMode == MODE_COMPETITION) {
            self->lives    = session->lives[self->playerID];
            self->score    = 0;
            self->score1UP = 50000;
        }
        else if (globals->gameMode == MODE_TIMEATTACK) {
            self->lives    = 1;
            self->score    = 0;
            self->score1UP = 50000;
        }
        else {
            self->lives    = Player->savedLives;
            self->score    = Player->savedScore;
            self->score1UP = Player->savedScore1UP;
        }

        // Setup the target score
        int32 target = self->score1UP;
        while (self->score1UP <= target) self->score1UP += 50000;

        self->collisionLayers = Zone->collisionLayers;
        self->drawFX          = FX_ROTATE | FX_FLIP;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, true, 0);

        Player_UpdatePhysicsState(self);
        self->maxWalkSpeed = 0x40000;
        self->maxJogSpeed  = 0x60000;
        self->maxRunSpeed  = 0xC0000;
    }
}

void Player_StageLoad(void)
{
    if (!globals->playerID)
        globals->playerID = RSDK.CheckStageFolder("MSZCutscene") ? ID_KNUCKLES : ID_DEFAULT_PLAYER;

    SceneInfo->debugMode = globals->medalMods & getMod(MEDAL_DEBUGMODE);
    SceneInfo->debugMode = true; // TODO: TEMP
#if RETRO_USE_PLUS
    RSDK.SetDebugValue("Debug Mode", &SceneInfo->debugMode, DTYPE_BOOL, false, true);
#endif

    if (globals->medalMods & getMod(MEDAL_ANDKNUCKLES)) {
        globals->playerID &= 0xFF;
        globals->playerID |= ID_KNUCKLES_ASSIST;
    }

    Player->playerCount = 0;
    Player->active      = ACTIVE_ALWAYS;

    // Sprite loading & characterID management
    if (globals->gameMode == MODE_COMPETITION)
        Player_LoadSpritesVS();
    else
        Player_LoadSprites();

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE) {
        Player->playerCount    = 2;
        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        sidekick->playerID     = 1;
    }
    else {
        Player->playerCount = RSDK.GetEntityCount(Player->classID, false);
    }
#else
    Player->playerCount = RSDK.GetEntityCount(Player->classID, false);
#endif

    // Handle Sidekick stuff setup
    Player->nextLeaderPosID = 1;
    Player->lastLeaderPosID = 0;
#if RETRO_USE_PLUS
    Player->disableP2KeyCheck = false;
#endif

    Player->upState        = false;
    Player->downState      = false;
    Player->leftState      = false;
    Player->rightState     = false;
    Player->jumpPressState = false;
    Player->jumpHoldState  = false;

    // Sfx Loading
    Player->sfxJump        = RSDK.GetSfx("Global/Jump.wav");
    Player->sfxLoseRings   = RSDK.GetSfx("Global/LoseRings.wav");
    Player->sfxHurt        = RSDK.GetSfx("Global/Hurt.wav");
    Player->sfxRoll        = RSDK.GetSfx("Global/Roll.wav");
    Player->sfxCharge      = RSDK.GetSfx("Global/Charge.wav");
    Player->sfxRelease     = RSDK.GetSfx("Global/Release.wav");
    Player->sfxPeelCharge  = RSDK.GetSfx("Global/PeelCharge.wav");
    Player->sfxPeelRelease = RSDK.GetSfx("Global/PeelRelease.wav");
    Player->sfxDropdash    = RSDK.GetSfx("Global/DropDash.wav");
    Player->sfxSkidding    = RSDK.GetSfx("Global/Skidding.wav");
    Player->sfxGrab        = RSDK.GetSfx("Global/Grab.wav");
    Player->sfxFlying      = RSDK.GetSfx("Global/Flying.wav");
    Player->sfxTired       = RSDK.GetSfx("Global/Tired.wav");
    Player->sfxLand        = RSDK.GetSfx("Global/Land.wav");
    Player->sfxSlide       = RSDK.GetSfx("Global/Slide.wav");
    Player->sfxOuttahere   = RSDK.GetSfx("Global/OuttaHere.wav");
    Player->sfxTransform2  = RSDK.GetSfx("Stage/Transform2.wav");
#if RETRO_USE_PLUS
    Player->sfxPimPom        = RSDK.GetSfx("Stage/PimPom.wav");
    Player->sfxSwap          = RSDK.GetSfx("Global/Swap.wav");
    Player->sfxSwapFail      = RSDK.GetSfx("Global/SwapFail.wav");
    Player->sfxMightyDeflect = RSDK.GetSfx("Global/MightyDeflect.wav");
    Player->sfxMightyDrill   = RSDK.GetSfx("Global/MightyDrill.wav");
    Player->sfxMightyLand    = RSDK.GetSfx("Global/MightyLand.wav");
    Player->sfxMightyUnspin  = RSDK.GetSfx("Global/MightyUnspin.wav"); 

    Soundboard_LoadSFX("Global/RaySwoop.wav", 41417, Player_CheckRaySwooping, Player_UpdateRaySwoopSFX);
    Soundboard_LoadSFX("Global/RayDive.wav", 72323, Player_CheckRayDiving, Player_UpdateRayDiveSFX);
#endif

    // Handle gotHit values (used for TMZ1 achievement)
    for (int32 p = 0; p < PLAYER_MAX; ++p) Player->gotHit[p] = false;
}

// Extra Entity Functions
void Player_LoadSprites(void)
{
    foreach_all(Player, entity)
    {
        int32 pID = globals->playerID & 0xFF;
#if RETRO_USE_PLUS
        if (pID == ID_MIGHTY || pID == ID_RAY)
            pID = ID_SONIC;
#endif

        if (pID & entity->characterID) {
            entity->characterID = globals->playerID & 0xFF;
            switch (entity->characterID) {
                default:
                case ID_SONIC:
                    Player->sonicFrames = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
                    Player->superFrames = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                    break;

                case ID_TAILS:
                    Player->tailsFrames      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
                    Player->tailsTailsFrames = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                    break;

                case ID_KNUCKLES: Player->knuxFrames = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: Player->mightyFrames = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                case ID_RAY: Player->rayFrames = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
            }

            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            RSDK.CopyEntity(player1, entity, true);
            player1->camera = Camera_SetTargetEntity(0, (Entity *)player1);
            RSDK.AddCamera(&player1->position, ScreenInfo->centerX << 16, ScreenInfo->centerY << 16, true);
        }
        else {
            destroyEntity(entity);
        }
    }

    if (((globals->playerID >> 8) & 0xFF) > 0) {
        EntityPlayer *leader   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

        for (int32 i = 0; i < 0x10; ++i) Player->leaderPositionBuffer[i] = leader->position;

        sidekick->classID   = Player->classID;
        sidekick->position.x = leader->position.x;
        sidekick->position.y = leader->position.y;

        if (globals->gameMode != MODE_TIMEATTACK) {
            RSDK.AddCamera(&sidekick->position, ScreenInfo->centerX << 16, ScreenInfo->centerY << 16, true);
            sidekick->position.x -= 0x100000;
        }

        sidekick->characterID = globals->playerID >> 8;
        switch (sidekick->characterID) {
            default:
            case ID_SONIC:
                sidekick->characterID = ID_SONIC;
                Player->sonicFrames   = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
                Player->superFrames   = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                break;

            case ID_TAILS:
                Player->tailsFrames      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
                Player->tailsTailsFrames = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                break;

            case ID_KNUCKLES: Player->knuxFrames = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: Player->mightyFrames = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
            case ID_RAY: Player->rayFrames = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
        }
    }
}
void Player_LoadSpritesVS(void)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    foreach_all(Player, entity)
    {
        if (entity->characterID & 1) {
            int32 slotID = 0;
            for (int32 i = 0; i < session->playerCount; ++i, ++slotID) {
                EntityPlayer *player = RSDK_GET_ENTITY(slotID, Player);
                RSDK.CopyEntity(player, entity, false);
                player->characterID = (globals->playerID >> (8 * i)) & 0xFF;
                switch (player->characterID) {
                    default:
                    case ID_SONIC:
                        player->characterID = ID_SONIC;
                        Player->sonicFrames = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
                        Player->superFrames = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
                        break;

                    case ID_TAILS:
                        Player->tailsFrames      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
                        Player->tailsTailsFrames = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
                        break;

                    case ID_KNUCKLES: Player->knuxFrames = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if RETRO_USE_PLUS
                    case ID_MIGHTY: Player->mightyFrames = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
                    case ID_RAY: Player->rayFrames = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
                }

                player->controllerID = i + 1;
                player->camera       = Camera_SetTargetEntity(i, (Entity *)player);
            }
        }

        destroyEntity(entity);
    }
}
void Player_SaveValues(void)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    globals->restartLives[0] = Player->savedLives = player1->lives;
    globals->restartScore = Player->savedScore = player1->score;
    globals->restartScore1UP = Player->savedScore1UP = player1->score1UP;
}
void Player_GiveScore(EntityPlayer *player, int32 score)
{
    if (player->sidekick)
        player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    player->score += score;
    if (player->score > 9999999)
        player->score = 9999999;

    if (player->score >= player->score1UP) {
        Player_GiveLife(player);
        while (player->score1UP <= player->score) player->score1UP += 50000;
    }
}
void Player_GiveRings(EntityPlayer *player, int32 amount, bool32 playSfx)
{
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    if (globals->gameMode == MODE_COMPETITION)
        session->totalRings[player->playerID] += amount;

    player->rings = clampVal(player->rings + amount, 0, 999);

    if (player->rings >= player->ringExtraLife) {
        Player_GiveLife(player);
        player->ringExtraLife += 100;
        // literally 1984
        if (player->ringExtraLife > 300)
            player->ringExtraLife = 1000;
    }

    if (playSfx) {
        if (Ring->pan) {
            int32 channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
            Ring->pan = 0;
        }
        else {
            int32 channel = RSDK.PlaySfx(Ring->sfxRing, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
            Ring->pan = 1;
        }
    }
}
void Player_GiveLife(EntityPlayer *entity)
{
#if RETRO_USE_PLUS
    if (globals->gameMode != MODE_TIMEATTACK && globals->gameMode != MODE_ENCORE) {
#else
    if (globals->gameMode != MODE_TIMEATTACK) {
#endif
        if (entity->lives < 99)
            entity->lives++;

        Music_PlayQueuedTrack(TRACK_1UP);
    }
}
void Player_ApplyShield(EntityPlayer *player)
{
    if (player->shield && player->superState != SUPERSTATE_SUPER && player->invincibleTimer <= 0) {
        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
        RSDK.ResetEntityPtr(shield, Shield->classID, player);
    }
}
void Player_ChangeCharacter(EntityPlayer *entity, int32 character)
{
    uint16 playerID     = entity->playerID;
    entity->characterID = character;
    globals->playerID &= ~(255 << 8 * playerID);
    globals->playerID |= character << 8 * entity->playerID;

    switch (entity->characterID) {
        default:
        case ID_SONIC:
            Player->sonicFrames = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE);
            Player->superFrames = RSDK.LoadSpriteAnimation("Players/SuperSonic.bin", SCOPE_STAGE);
            if (SizeLaser)
                SizeLaser->sonicFrames = RSDK.LoadSpriteAnimation("Players/ChibiSonic.bin", SCOPE_STAGE);

            if (entity->isChibi) {
                entity->aniFrames  = SizeLaser->sonicFrames;
                entity->jumpOffset = 0x40000;
            }
            else {
                entity->aniFrames  = Player->sonicFrames;
                entity->jumpOffset = 0x50000;
            }

            entity->tailFrames   = -1;
            entity->stateAbility = Player_JumpAbility_Sonic;
            entity->sensorY      = 0x140000;

            if (globals->medalMods & getMod(MEDAL_PEELOUT)) {
                entity->statePeelout = Player_StartPeelout;
                for (int32 f = 0; f < 4; ++f) {
                    SpriteFrame *dst = RSDK.GetFrame(entity->aniFrames, ANI_DASH, f);
                    SpriteFrame *src = RSDK.GetFrame(entity->aniFrames, ANI_FLY, f);

                    *dst = *src;
                }
            }
            break;

        case ID_TAILS:
            Player->tailsFrames      = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);
            Player->tailsTailsFrames = RSDK.LoadSpriteAnimation("Players/TailSprite.bin", SCOPE_STAGE);
            if (SizeLaser) {
                SizeLaser->tailsFrames = RSDK.LoadSpriteAnimation("Players/ChibiTails.bin", SCOPE_STAGE);
                SizeLaser->tailFrames  = RSDK.LoadSpriteAnimation("Players/CTailSprite.bin", SCOPE_STAGE);
            }

            if (entity->isChibi) {
                entity->aniFrames  = SizeLaser->tailsFrames;
                entity->tailFrames = SizeLaser->tailFrames;
                entity->jumpOffset = 0x40000;
            }
            else {
                entity->aniFrames  = Player->tailsFrames;
                entity->tailFrames = Player->tailsTailsFrames;
                entity->jumpOffset = 0;
            }

            entity->stateAbility = Player_JumpAbility_Tails;
            entity->sensorY      = 0x100000;
            break;

        case ID_KNUCKLES:
            Player->knuxFrames = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE);
            if (SizeLaser)
                SizeLaser->knuxFrames = RSDK.LoadSpriteAnimation("Players/ChibiKnux.bin", SCOPE_STAGE);

            if (entity->isChibi) {
                entity->aniFrames  = SizeLaser->knuxFrames;
                entity->jumpOffset = 0x40000;
            }
            else {
                entity->aniFrames  = Player->knuxFrames;
                entity->jumpOffset = 0x50000;
            }

            entity->tailFrames   = -1;
            entity->stateAbility = Player_JumpAbility_Knux;
            entity->sensorY      = 0x140000;
            break;

#if RETRO_USE_PLUS
        case ID_MIGHTY:
            Player->mightyFrames = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE);
            if (SizeLaser)
                SizeLaser->mightyFrames = RSDK.LoadSpriteAnimation("Players/ChibiMighty.bin", SCOPE_STAGE);

            if (entity->isChibi) {
                entity->aniFrames  = SizeLaser->mightyFrames;
                entity->jumpOffset = 0x40000;
            }
            else {
                entity->aniFrames  = Player->mightyFrames;
                entity->jumpOffset = 0x50000;
            }

            entity->tailFrames   = -1;
            entity->stateAbility = Player_JumpAbility_Mighty;
            entity->sensorY      = 0x140000;
            break;

        case ID_RAY:
            Player->rayFrames = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE);
            if (SizeLaser)
                SizeLaser->rayFrames = RSDK.LoadSpriteAnimation("Players/ChibiRay.bin", SCOPE_STAGE);

            if (entity->isChibi) {
                entity->aniFrames  = SizeLaser->rayFrames;
                entity->jumpOffset = 0x40000;
            }
            else {
                entity->aniFrames  = Player->rayFrames;
                entity->jumpOffset = 0x50000;
            }

            entity->tailFrames   = -1;
            entity->stateAbility = Player_JumpAbility_Ray;
            entity->sensorY      = 0x140000;
            break;
#endif
    }

    entity->sensorX[0] = 0xA0000;
    entity->sensorX[1] = 0x50000;
    entity->sensorX[2] = 0;
    entity->sensorX[3] = -0x50000;
    entity->sensorX[4] = -0xA0000;

    if (entity->state == Player_State_KnuxWallClimb || entity->state == Player_State_DropDash || entity->state == Player_State_TailsFlight
        || entity->state == Player_State_KnuxGlideDrop || entity->state == Player_State_KnuxGlideLeft || entity->state == Player_State_KnuxGlideRight
        || entity->state == Player_State_GlideSlide || entity->state == Player_State_KnuxLedgePullUp
#if RETRO_USE_PLUS
        || entity->state == Player_State_MightyHammerDrop || entity->state == Player_State_RayGlide || entity->state == Player_State_MightyUnspin
#endif
    ) {
        entity->state = Player_State_Air;
        RSDK.SetSpriteAnimation(entity->aniFrames, ANI_JUMP, &entity->animator, false, 0);
    }
    else {
        RSDK.SetSpriteAnimation(entity->aniFrames, entity->animator.animationID, &entity->animator, false, 0);
    }

    if (entity->superState != SUPERSTATE_NONE) {
        for (int32 i = 0; i < 6; ++i) {
            RSDK.SetPaletteEntry(0, i + 64, Player->superPalette_Sonic[i]);
            RSDK.SetPaletteEntry(0, i + 70, Player->superPalette_Tails[i]);
            RSDK.SetPaletteEntry(0, i + 80, Player->superPalette_Knux[i]);
#if RETRO_USE_PLUS
            RSDK.SetPaletteEntry(0, i + 96, Player->superPalette_Mighty[i]);
            RSDK.SetPaletteEntry(0, i + 113, Player->superPalette_Ray[i]);
#endif

            if (HCZSetup) {
                RSDK.SetPaletteEntry(1, i + 64, Player->superPalette_Sonic_HCZ[i]);
                RSDK.SetPaletteEntry(1, i + 70, Player->superPalette_Tails_HCZ[i]);
                RSDK.SetPaletteEntry(1, i + 80, Player->superPalette_Knux_HCZ[i]);
#if RETRO_USE_PLUS
                RSDK.SetPaletteEntry(1, i + 96, Player->superPalette_Mighty_HCZ[i]);
                RSDK.SetPaletteEntry(1, i + 113, Player->superPalette_Ray_HCZ[i]);
#endif
            }
            else if (CPZSetup) {
                RSDK.SetPaletteEntry(2, i + 64, Player->superPalette_Sonic_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 70, Player->superPalette_Tails_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 80, Player->superPalette_Knux_CPZ[i]);
#if RETRO_USE_PLUS
                RSDK.SetPaletteEntry(2, i + 96, Player->superPalette_Mighty_CPZ[i]);
                RSDK.SetPaletteEntry(2, i + 113, Player->superPalette_Ray_CPZ[i]);
#endif
            }
        }

        if (FarPlane) {
            RSDK.CopyPalette(0, 64, 3, 64, 6);
            RSDK.CopyPalette(0, 70, 3, 70, 6);
            RSDK.CopyPalette(0, 80, 3, 80, 6);
            RSDK.CopyPalette(0, 96, 3, 96, 6);
            RSDK.CopyPalette(0, 113, 3, 113, 6);
        }
        else if (OOZSetup) {
            RSDK.CopyPalette(0, 64, 1, 64, 6);
            RSDK.CopyPalette(0, 64, 2, 64, 6);
            RSDK.CopyPalette(0, 70, 1, 70, 6);
            RSDK.CopyPalette(0, 70, 2, 70, 6);
            RSDK.CopyPalette(0, 80, 1, 80, 6);
            RSDK.CopyPalette(0, 80, 2, 80, 6);
            RSDK.CopyPalette(0, 96, 1, 96, 6);
            RSDK.CopyPalette(0, 96, 2, 96, 6);
            RSDK.CopyPalette(0, 113, 1, 113, 6);
            RSDK.CopyPalette(0, 113, 2, 113, 6);
        }

        if (entity->superState == SUPERSTATE_SUPER)
            Player_CheckGoSuper(entity, 0xFF);
    }

    Player_UpdatePhysicsState(entity);
}
bool32 Player_CheckGoSuper(EntityPlayer *player, uint8 emeraldMasks)
{
    if (!SceneInfo->timeEnabled && !ERZStart && (!PhantomEgg || PhantomEgg->disableSuperForm))
        return false;

    uint8 emeralds = emeraldMasks;
    if (emeraldMasks == 0xFF) // 0xFF seems to be the "force transform" flag
        emeralds = 0x7F;

#if RETRO_USE_PLUS
    if (Player->canSuperCB) {
        if (!Player->canSuperCB(false))
            return false;
    }
#endif

    if ((player->superState >= SUPERSTATE_SUPER || emeralds != 0x7F || player->rings < 50) && emeraldMasks != 0xFF)
        return false;

#if RETRO_USE_PLUS
    RSDK.StopSfx(Player->sfxSwapFail);
    if (globals->medalMods & getMod(SECRET_SUPERDASH))
        player->stateAbility = ERZStart_Player_StartSuperFly;
#endif

    switch (player->characterID) {
        case ID_SONIC:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 64, Player->superPalette_Sonic[c]);
                RSDK.SetPaletteEntry(7, c + 64, Player->superPalette_Sonic[c + 6]);
            }
            break;

        case ID_TAILS:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 70, Player->superPalette_Tails[c]);
                RSDK.SetPaletteEntry(7, c + 70, Player->superPalette_Tails[c + 12]);
            }
            break;

        case ID_KNUCKLES:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 80, Player->superPalette_Knux[c]);
                RSDK.SetPaletteEntry(7, c + 80, Player->superPalette_Knux[c + 12]);
            }
            break;

#if RETRO_USE_PLUS
        case ID_MIGHTY:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 96, Player->superPalette_Mighty[c]);
                RSDK.SetPaletteEntry(7, c + 96, Player->superPalette_Mighty[c + 12]);
            }
            break;

        case ID_RAY:
            for (int32 c = 0; c < 6; ++c) {
                RSDK.SetPaletteEntry(6, c + 113, Player->superPalette_Ray[c]);
                RSDK.SetPaletteEntry(7, c + 113, Player->superPalette_Ray[c + 12]);
            }
            break;
#endif

        default: break;
    }

    if (player->characterID == ID_SONIC && !player->isChibi)
        player->aniFrames = Player->superFrames;

    if (emeraldMasks == 0xFF) {
        player->superState = SUPERSTATE_SUPER;
        Player_UpdatePhysicsState(player);
    }
    else {
        if (player->isChibi)
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
        else
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_TRANSFORM, &player->animator, true, 0);

        player->invincibleTimer = 60;
        player->velocity.x      = 0;
        player->velocity.y      = 0;
        player->groundVel       = 0;
        player->onGround        = false;
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;
        player->interaction     = false;
        player->state           = Player_State_Transform;
        player->isTransforming  = true;

#if RETRO_USE_PLUS
        if (!ERZStart && globals->superMusicEnabled)
#else
        if (!ERZStart)
#endif
            Music_FadeOut(0.8);

        player->jumpAbilityState = 0;
        player->superState       = SUPERSTATE_FADEIN;
    }

    player->superBlendAmount   = 0;
    player->superBlendState    = 0;
    player->superRingLossTimer = 60;
    player->timer              = 0;
    return true;
}
void Player_BlendSuperSonicColors(int32 bankID)
{
    RSDK_THIS(Player);

    if (self->superState == SUPERSTATE_SUPER) {
        if (self->superBlendState & 1) {
            if (self->superBlendAmount <= 0)
                self->superBlendState &= 2;
            else
                self->superBlendAmount -= 4;
        }
        else if (self->superBlendAmount >= 256) {
            self->superBlendState |= 1;
            if (self->superBlendState == 1) {
                for (int32 i = 0; i < 6; ++i) {
                    RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i + 6]);
                    RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 12]);
                }
                self->superBlendState  = 2;
                self->superBlendAmount = 0;
            }
        }
        else {
            self->superBlendAmount += 4;
        }
    }
    else {
        if (self->superBlendAmount <= 0)
            self->superState = SUPERSTATE_NONE;
        else
            self->superBlendAmount -= 4;
    }

    RSDK.SetLimitedFade(bankID, 6, 7, self->superBlendAmount, 64, 69);
}
void Player_BlendSuperTailsColors(int32 bankID)
{
    RSDK_THIS(Player);

    if (self->superState != SUPERSTATE_SUPER) {
        if (self->superBlendAmount <= 0)
            self->superState = SUPERSTATE_NONE;
        else
            self->superBlendAmount -= 4;
    }
    else if (self->superBlendState) {
        if (self->superBlendAmount <= 0)
            self->superBlendState = 0;
        else
            self->superBlendAmount -= 4;
    }
    else {
        if (self->superBlendAmount >= 256)
            self->superBlendState = 1;
        else
            self->superBlendAmount += 4;
    }

    // Bug Details:
    // set this "76" to "75" to fix tails' super palette overwriting an extra colour
    RSDK.SetLimitedFade(bankID, 6, 7, self->superBlendAmount, 70, 76);
}
void Player_BlendSuperKnuxColors(int32 bankID)
{
    RSDK_THIS(Player);

    if (self->superState != SUPERSTATE_SUPER) {
        if (self->superBlendAmount <= 0)
            self->superState = SUPERSTATE_NONE;
        else
            self->superBlendAmount -= 4;
    }
    else if (self->superBlendState) {
        if (self->superBlendAmount <= 0)
            self->superBlendState = 0;
        else
            self->superBlendAmount -= 4;
    }
    else {
        if (self->superBlendAmount >= 256)
            self->superBlendState = 1;
        else
            self->superBlendAmount += 4;
    }

    // Bug Details:
    // set this "86" to "85" to fix knuckles's super palette overwriting an extra colour
    RSDK.SetLimitedFade(bankID, 6, 7, self->superBlendAmount, 80, 86);
}
#if RETRO_USE_PLUS
void Player_BlendSuperMightyColors(int32 bankID)
{
    RSDK_THIS(Player);

    if (self->superState != SUPERSTATE_SUPER) {
        if (self->superBlendAmount <= 0)
            self->superState = SUPERSTATE_NONE;
        else
            self->superBlendAmount -= 4;
    }
    else if (self->superBlendState) {
        if (self->superBlendAmount <= 0)
            self->superBlendState = 0;
        else
            self->superBlendAmount -= 4;
    }
    else {
        if (self->superBlendAmount >= 256)
            self->superBlendState = 1;
        else
            self->superBlendAmount += 4;
    }

    // Bug Details:
    // set this "102" to "101" to fix mighty's super palette messing up a skin tone colour
    RSDK.SetLimitedFade(bankID, 6, 7, self->superBlendAmount, 96, 102);
}
void Player_BlendSuperRayColors(int32 bankID)
{
    RSDK_THIS(Player);

    if (self->superState != SUPERSTATE_SUPER) {
        if (self->superBlendAmount <= 0)
            self->superState = SUPERSTATE_NONE;
        else
            self->superBlendAmount -= 4;
    }
    else if (self->superBlendState) {
        if (self->superBlendAmount <= 0)
            self->superBlendState = 0;
        else
            self->superBlendAmount -= 4;
    }
    else {
        if (self->superBlendAmount >= 256)
            self->superBlendState = 1;
        else
            self->superBlendAmount += 4;
    }

    RSDK.SetLimitedFade(bankID, 6, 7, self->superBlendAmount, 113, 119);
}
#endif
void Player_HandleSuperForm(void)
{
    RSDK_THIS(Player);

    if (self->superState != SUPERSTATE_NONE) {
        switch (self->characterID) {
            case ID_SONIC:
                Player_BlendSuperSonicColors(0);

                if (FarPlane) {
                    RSDK.CopyPalette(0, 64, 3, 64, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 64, 1, 64, 6);
                    RSDK.CopyPalette(0, 64, 2, 64, 6);
                }
                else if (HCZSetup) {
                    if (self->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic_HCZ[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic_HCZ[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic_HCZ[i]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic_HCZ[i + 6]);
                        }
                    }

                    Player_BlendSuperSonicColors(1);

                    if (self->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic[i]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic[i + 6]);
                        }
                    }
                }
                else if (CPZSetup) {
                    if (self->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic_CPZ[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic_CPZ[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic_CPZ[i]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic_CPZ[i + 6]);
                        }
                    }

                    Player_BlendSuperSonicColors(2);

                    if (self->superBlendState > 1) {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic[i + 6]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic[i + 12]);
                        }
                    }
                    else {
                        for (int32 i = 0; i < 6; ++i) {
                            RSDK.SetPaletteEntry(6, i + 64, Player->superPalette_Sonic[i]);
                            RSDK.SetPaletteEntry(7, i + 64, Player->superPalette_Sonic[i + 6]);
                        }
                    }
                }
                break;

            case ID_TAILS:
                Player_BlendSuperTailsColors(0);

                if (FarPlane) {
                    RSDK.CopyPalette(0, 70, 3, 70, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 70, 1, 70, 6);
                    RSDK.CopyPalette(0, 70, 2, 70, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails_HCZ[i + 12]);
                    }

                    Player_BlendSuperTailsColors(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails_CPZ[i + 12]);
                    }

                    Player_BlendSuperTailsColors(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 70, Player->superPalette_Tails[i]);
                        RSDK.SetPaletteEntry(7, i + 70, Player->superPalette_Tails[i + 12]);
                    }
                }
                break;

            case ID_KNUCKLES:
                Player_BlendSuperKnuxColors(0);

                if (FarPlane) {
                    RSDK.CopyPalette(0, 80, 3, 80, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 80, 1, 80, 6);
                    RSDK.CopyPalette(0, 80, 2, 80, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 80, Player->superPalette_Knux_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 80, Player->superPalette_Knux_HCZ[i + 12]);
                    }

                    Player_BlendSuperKnuxColors(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 80, Player->superPalette_Knux[i]);
                        RSDK.SetPaletteEntry(7, i + 80, Player->superPalette_Knux[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 80, Player->superPalette_Knux_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 80, Player->superPalette_Knux_CPZ[i + 12]);
                    }

                    Player_BlendSuperKnuxColors(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 80, Player->superPalette_Knux[i]);
                        RSDK.SetPaletteEntry(7, i + 80, Player->superPalette_Knux[i + 12]);
                    }
                }
                break;

#if RETRO_USE_PLUS
            case ID_MIGHTY:
                Player_BlendSuperMightyColors(0);

                if (FarPlane) {
                    RSDK.CopyPalette(0, 96, 3, 96, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 96, 1, 96, 6);
                    RSDK.CopyPalette(0, 96, 2, 96, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 96, Player->superPalette_Mighty_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 96, Player->superPalette_Mighty_HCZ[i + 12]);
                    }

                    Player_BlendSuperMightyColors(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 96, Player->superPalette_Mighty[i]);
                        RSDK.SetPaletteEntry(7, i + 96, Player->superPalette_Mighty[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 96, Player->superPalette_Mighty_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 96, Player->superPalette_Mighty_CPZ[i + 12]);
                    }

                    Player_BlendSuperMightyColors(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 96, Player->superPalette_Mighty[i]);
                        RSDK.SetPaletteEntry(7, i + 96, Player->superPalette_Mighty[i + 12]);
                    }
                }
                break;

            case ID_RAY:
                Player_BlendSuperRayColors(0);

                if (FarPlane) {
                    RSDK.CopyPalette(0, 113, 3, 113, 6);
                }
                else if (OOZSetup) {
                    RSDK.CopyPalette(0, 113, 1, 113, 6);
                    RSDK.CopyPalette(0, 113, 2, 113, 6);
                }
                else if (HCZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray_HCZ[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray_HCZ[i + 12]);
                    }

                    Player_BlendSuperRayColors(1);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray[i + 12]);
                    }
                }
                else if (CPZSetup) {
                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray_CPZ[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray_CPZ[i + 12]);
                    }

                    Player_BlendSuperRayColors(2);

                    for (int32 i = 0; i < 6; ++i) {
                        RSDK.SetPaletteEntry(6, i + 113, Player->superPalette_Ray[i]);
                        RSDK.SetPaletteEntry(7, i + 113, Player->superPalette_Ray[i + 12]);
                    }
                }
                break;
#endif
        }
    }

    if (self->superState == SUPERSTATE_SUPER) {
        bool32 canStopSuper = false;
        if (!SceneInfo->timeEnabled && !ERZStart) {
            if (!PhantomEgg || PhantomEgg->disableSuperForm) {
                self->superState = SUPERSTATE_FADEOUT;
                canStopSuper             = true;
            }
        }

        if (!canStopSuper) {
            self->invincibleTimer = 60;
            if (--self->superRingLossTimer <= 0) {
                self->superRingLossTimer = 60;
                if (--self->rings <= 0) {
                    self->rings      = 0;
                    self->superState = SUPERSTATE_FADEOUT;
                }
            }
        }
    }

    if (self->superState == SUPERSTATE_FADEOUT) {
        switch (self->characterID) {
            case ID_SONIC:
                self->stateAbility = Player_JumpAbility_Sonic;
                for (int32 i = 0; i < 6; ++i) {
                    RSDK.SetPaletteEntry(6, i + 0x40, Player->superPalette_Sonic[i]);
                    RSDK.SetPaletteEntry(7, i + 0x40, Player->superPalette_Sonic[i + 6]);
                }
                self->superBlendAmount = 256;
                self->superBlendState  = 1;
                break;

            case ID_TAILS: self->stateAbility = Player_JumpAbility_Tails; break;
            case ID_KNUCKLES: self->stateAbility = Player_JumpAbility_Knux; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: self->stateAbility = Player_JumpAbility_Mighty; break;
            case ID_RAY: self->stateAbility = Player_JumpAbility_Ray; break;
#endif
            default: break;
        }

        if (ERZStart) {
            self->shield   = SHIELD_NONE;
            self->rotation = 0;
            if (self->state == ERZStart_State_PlayerSuperFly || self->state == ERZStart_State_PlayerRebound)
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_HURT, &self->animator, false, 0);
        }
        else {
            bool32 stopPlaying = true;
            foreach_active(Player, player)
            {
                if (player->superState == SUPERSTATE_SUPER)
                    stopPlaying = false;
            }

            if (stopPlaying)
                Music_EndQueuedTrack(TRACK_SUPER, true);

            if (self->state == ERZStart_State_PlayerSuperFly || self->state == ERZStart_State_PlayerRebound)
                self->state = Player_State_Air;
        }

        if (self->characterID == ID_SONIC && !self->isChibi) {
            self->aniFrames = Player->sonicFrames;
            if (!self->animator.animationID)
                self->animator.frameID = 0;

            RSDK.SetSpriteAnimation(self->aniFrames, self->animator.animationID, &self->animator, true, self->animator.frameID);
        }

        self->invincibleTimer = 0;

        if (self->shield)
            Player_ApplyShield(self);

        self->superState = SUPERSTATE_DONE;
        Player_UpdatePhysicsState(self);
    }
}
bool32 Player_CheckP2KeyPress(void)
{
    RSDK_THIS(Player);

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        return false;
#endif

#if RETRO_USE_PLUS
    if (self->controllerID > PLAYER_MAX || Player->disableP2KeyCheck)
        return false;
#else
    if (self->controllerID > PLAYER_MAX)
        return false;
#endif

    return ControllerInfo[self->controllerID].keyUp.down || ControllerInfo[self->controllerID].keyDown.down
           || ControllerInfo[self->controllerID].keyLeft.down || ControllerInfo[self->controllerID].keyRight.down
           || ControllerInfo[self->controllerID].keyA.down || ControllerInfo[self->controllerID].keyB.down
           || ControllerInfo[self->controllerID].keyC.down || ControllerInfo[self->controllerID].keyX.down;
}
EntityPlayer *Player_GetNearestPlayerX(void)
{
    RSDK_THIS_GEN();

    EntityPlayer *targetPlayer = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    int32 targetDistance       = 0x7FFFFFFF;

    foreach_active(Player, player)
    {
        int32 distX = abs(player->position.x - self->position.x);
        if (distX < targetDistance) {
            targetDistance     = distX;
            targetPlayer = player;
        }
    }

    return targetPlayer;
}
EntityPlayer *Player_GetNearestPlayer(void)
{
    RSDK_THIS_GEN();

    EntityPlayer *targetPlayer = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    int32 targetDistance       = 0x7FFFFFFF;

    foreach_active(Player, player)
    {
        int32 distX = abs(player->position.x - self->position.x);
        int32 distY = abs(player->position.y - self->position.y);
        if (distX + distY < targetDistance) {
            targetDistance = distX + distY;
            targetPlayer   = player;
        }
    }

    return targetPlayer;
}
#if RETRO_USE_PLUS
void Player_RemoveEncoreLeader(void)
{
    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (globals->stock) {
        Player_ChangeCharacter(sidekick, globals->stock);
        globals->playerID &= 0xFF;
        globals->playerID |= globals->stock << 8;
        globals->stock >>= 8;
    }
    else {
        globals->playerID &= 0xFF;
        destroyEntity(sidekick);
        sidekick->playerID = 1;
    }
}
#endif
void Player_ResetBoundaries(EntityPlayer *player)
{
    Vector2 size;

    int32 screen = RSDK.GetEntityID(player);
    RSDK.GetLayerSize(Zone->fgLow, &size, true);

    Zone->cameraBoundsL[screen]      = 0;
    Zone->cameraBoundsR[screen]      = size.x;
    Zone->cameraBoundsT[screen]      = 0;
    Zone->cameraBoundsB[screen]      = size.y;
    Zone->playerBoundsL[screen]      = Zone->cameraBoundsL[screen] << 16;
    Zone->playerBoundsR[screen]      = Zone->cameraBoundsR[screen] << 16;
    Zone->playerBoundsT[screen]      = Zone->cameraBoundsT[screen] << 16;
    Zone->playerBoundsB[screen]      = Zone->cameraBoundsB[screen] << 16;
    Zone->deathBoundary[screen]      = Zone->cameraBoundsB[screen] << 16;
    Zone->playerBoundActiveT[screen] = true;
    Zone->playerBoundActiveT[screen] = false;

    if (player->camera) {
        player->camera->boundsL = Zone->cameraBoundsL[screen];
        player->camera->boundsR = Zone->cameraBoundsR[screen];
        player->camera->boundsT = Zone->cameraBoundsT[screen];
        player->camera->boundsB = Zone->cameraBoundsB[screen];
    }
}
void Player_HandleDeath(EntityPlayer *player)
{
    if (player->sidekick) {
        Player->jumpInTimer = 0;

        EntityDust *dust    = CREATE_ENTITY(Dust, intToVoid(1), player->position.x, player->position.y);
        dust->visible       = false;
        dust->active        = ACTIVE_NEVER;
        dust->isPermanent   = true;
        dust->position.y    = (ScreenInfo->position.y - 128) << 16;

        // Sidekicks just respawn, no biggie
#if RETRO_USE_PLUS
        EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (globals->gameMode != MODE_ENCORE || (leader->state != Player_State_Die && leader->state != Player_State_Drown)) {
            player->angle            = 0x80;
            player->state            = Player_State_StartJumpIn;
            player->abilityPtrs[0]   = dust;
            player->abilityValues[0] = 0;
            player->nextAirState     = StateMachine_None;
            player->nextGroundState  = StateMachine_None;
            player->stateInput       = Player_ProcessP2InputLag;
            player->position.x       = -0x400000;
            player->position.y       = -0x400000;
            player->velocity.x       = 0;
            player->velocity.y       = 0;
            player->groundVel        = 0;
            player->tileCollisions   = false;
            player->interaction      = false;
            player->drawOrder        = Zone->playerDrawHigh;
            player->drownTimer       = 0;
            player->active           = ACTIVE_NORMAL;
        }
        else {
            player->state      = Player_State_EncoreRespawn;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->position.x = -0x200000;
            player->position.y = -0x200000;
        }
#else
        player->angle = 0x80;
        player->state = Player_State_StartJumpIn;
        player->abilityPtrs[0] = dust;
        player->abilityValues[0] = 0;
        player->nextAirState = StateMachine_None;
        player->nextGroundState = StateMachine_None;
        player->stateInput = Player_ProcessP2InputLag;
        player->position.x = -0x400000;
        player->position.y = -0x400000;
        player->velocity.x = 0;
        player->velocity.y = 0;
        player->groundVel = 0;
        player->tileCollisions = false;
        player->interaction = false;
        player->drawOrder = Zone->playerDrawHigh;
        player->drownTimer = 0;
        player->active = ACTIVE_NORMAL;
#endif
    }
    else {
#if RETRO_USE_PLUS
        // if we're in encore mode AND can swap to the next player, just do that
        if (globals->gameMode == MODE_ENCORE && Player_SwapMainPlayer(false)) {
            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

            int32 id              = -1;
            for (int32 i = player2->characterID; i > 0; ++id) i >>= 1;
            globals->characterFlags &= ~(1 << id);

            if (player->state == Player_State_FlyIn || player->state == Player_State_JumpIn || player->state == Player_State_StartJumpIn) {
                if (!EncoreIntro) {
                    Player_HandleQuickRespawn(player);
                    Player_ResetBoundaries(player);
                }

                Player_RemoveEncoreLeader();
            }
            else {
                player->blinkTimer   = 120;
                player->scrollDelay  = 0;
                EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                player->camera       = camera;
                camera->target    = (Entity *)player;
                camera->state        = Camera_State_Follow;
                Player_RemoveEncoreLeader();
            }
        }
        else { // regular death event
#endif
            
            if (player->lives > 0 && globals->gameMode != MODE_TIMEATTACK)
                player->lives--;
            Player->savedLives                      = player->lives;
            Player->savedScore                      = player->score;
            Player->savedScore1UP                   = player->score1UP;
            globals->restartLives[player->playerID] = player->lives;

#if RETRO_USE_PLUS
            if (globals->gameMode != MODE_ENCORE) {
                player->rings         = 0;
                player->ringExtraLife = 0;
                globals->restartRings = 0;
                globals->restart1UP   = 100;
            }
#else
        player->rings = 0;
        player->ringExtraLife = 0;
        globals->restartRings = 0;
        globals->restart1UP = 100;
#endif
            globals->coolBonus[player->playerID] = 0;

            EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
            if (globals->gameMode == MODE_COMPETITION) 
                session->lives[player->playerID] = player->lives;

#if RETRO_USE_PLUS
            if (globals->gameMode != MODE_ENCORE) {
#endif
                if (player->lives) {
                    if (Zone->gotTimeOver) {
                        // Time Over!!
                        player->classID        = TYPE_BLANK;

                        EntitySaveGame *saveRAM = SaveGame->saveRAM;
                        if (globals->gameMode == MODE_COMPETITION) {
                            int32 playerID                    = RSDK.GetEntityID(player);
                            EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                            if (!session->finishState[playerID]) {
#if RETRO_USE_PLUS
                                Competition_CalculateScore(playerID, FINISHFLAG_TIMEOVER);
#else
                        CompetitionSession_DeriveWinner(playerID, FINISHFLAG_TIMEOVER);
#endif
                            }
#if RETRO_USE_PLUS
                            foreach_all(HUD, hud) { hud->vsStates[RSDK.GetEntityID(player)] = HUD_State_GoOffScreen; }
#endif
                        }
                        else if (saveRAM) {
                            RSDK.ResetEntitySlot(SLOT_GAMEOVER, GameOver->classID, intToVoid(true));
                            saveRAM->lives    = player->lives;
                            saveRAM->score    = player->score;
                            saveRAM->score1UP = player->score1UP;
#if RETRO_USE_PLUS
                            saveRAM->continues = globals->continues;
                            if (globals->gameMode == MODE_ENCORE) {
                                globals->playerID &= 0xFF;
                                saveRAM->playerID = globals->playerID;

                                int32 id          = -1;
                                for (int32 i = player->characterID; i > 0; ++id) i >>= 1;
                                globals->characterFlags &= ~(1 << id);

                                saveRAM->characterFlags = globals->characterFlags;
                                saveRAM->stock          = globals->stock;
                            }
#endif

                            SaveGame_SaveFile(NULL);

                            EntityGameOver *gameOver = RSDK_GET_ENTITY(SLOT_GAMEOVER, GameOver);
                            gameOver->activeScreens |= 1 << player->playerID;
                            RSDK.SetGameMode(ENGINESTATE_FROZEN);
                            SceneInfo->timeEnabled = false;
                        }
                    }
                    else if (globals->gameMode != MODE_COMPETITION) {
                        // Regular Death, fade out and respawn
                        EntitySaveGame *saveRAM = SaveGame->saveRAM;
                        if (saveRAM) {
                            saveRAM->lives    = player->lives;
                            saveRAM->score    = player->score;
                            saveRAM->score1UP = player->score1UP;
#if RETRO_USE_PLUS
                            saveRAM->continues = globals->continues;
                            if (globals->gameMode == MODE_ENCORE) {
                                globals->playerID &= 0xFF;
                                saveRAM->playerID = globals->playerID;

                                int32 id          = -1;
                                for (int32 i = player->characterID; i > 0; ++id) i >>= 1;
                                globals->characterFlags &= ~(1 << id);

                                saveRAM->characterFlags = globals->characterFlags;
                                saveRAM->stock          = globals->stock;
                            }
#endif
                            SaveGame_SaveFile(NULL);
                        }

                        Music_FadeOut(0.025);
                        Zone_StartFadeOut_MusicFade(10, 0x000000);
                        player->classID = TYPE_BLANK;
                    }
                    else {
                        // Competition (or encore) death, do a quick respawn
                        Player_HandleQuickRespawn(player);
#if RETRO_USE_PLUS
                        if (!player->playerID && globals->gameMode == MODE_ENCORE)
                            Player_HandleQuickRespawn(RSDK_GET_ENTITY(SLOT_PLAYER2, Player));
#endif
                    }
                }
                else {
                    // GGGGGGGame Over...!
                    int32 screenID   = 0;
                    player->classID = TYPE_BLANK;
                    if (player->camera) {
                        screenID                  = player->camera->screenID;
                        player->camera->target = (Entity *)player->camera;
                    }
                    if (globals->gameMode == MODE_COMPETITION) {
                        int32 playerID                    = RSDK.GetEntityID(player);
                        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                        if (!session->finishState[playerID]) {
#if RETRO_USE_PLUS
                            Competition_CalculateScore(playerID, FINISHFLAG_TIMEOVER);
#else
                            CompetitionSession_DeriveWinner(playerID, FINISHFLAG_TIMEOVER);
#endif
                        }
#if RETRO_USE_PLUS
                        foreach_all(HUD, hud) { hud->vsStates[RSDK.GetEntityID(player)] = HUD_State_GoOffScreen; }
#endif
                    }
                    else {
                        EntityGameOver *gameOver = RSDK_GET_ENTITY(SLOT_GAMEOVER, GameOver);
                        RSDK.ResetEntityPtr(gameOver, GameOver->classID, intToVoid(false));
                        gameOver->playerID = RSDK.GetEntityID(player);
                        GameOver->activeScreens |= 1 << screenID;
                        RSDK.SetGameMode(ENGINESTATE_FROZEN);
                        SceneInfo->timeEnabled = false;
                    }
                }

                if (player->classID == Player->classID) {
                    player->abilityValues[0] = 0;
                    Player_ResetBoundaries(player);

                    if (BoundsMarker)
                        BoundsMarker_CheckAllBounds(player, true);
                }
#if RETRO_USE_PLUS
            }
            else {
                // Encore death, lets switch to the next character and be on our way if possible

                int32 id = -1;
                for (int32 i = player->characterID; i > 0; ++id) i >>= 1;
                globals->characterFlags &= ~(1 << id);

                if (!globals->characterFlags) {
                    int32 screenID   = 0;
                    player->classID = TYPE_BLANK;
                    if (player->camera) {
                        screenID               = player->camera->screenID;
                        player->camera->target = (Entity *)player->camera;
                    }

                    EntityGameOver *gameOver = RSDK_GET_ENTITY(SLOT_GAMEOVER, GameOver);
                    RSDK.ResetEntityPtr(gameOver, GameOver->classID, intToVoid(false));
                    gameOver->playerID = RSDK.GetEntityID(player);
                    GameOver->activeScreens |= 1 << screenID;
                    RSDK.SetGameMode(ENGINESTATE_FROZEN);
                    SceneInfo->timeEnabled = false;

                    if (player->classID == Player->classID) {
                        player->abilityValues[0] = 0;
                        Player_ResetBoundaries(player);
                        if (BoundsMarker)
                            BoundsMarker_CheckAllBounds(player, true);
                    }
                }
                else {
                    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    Player->jumpInTimer   = 0;
                    Player_SwapMainPlayer(true);

                    if (globals->stock) {
                        Player_ChangeCharacter(player2, globals->stock);
                        globals->stock >>= 8;
                    }
                    else {
                        globals->playerID &= 0xFF;
                        player2->classID = TYPE_BLANK;
                    }

                    // Bug Details
                    // if you somehow die (drowing is the easiest way) after getting a buddy in AIZ Encore
                    // the game will lock up since it cant do this behaviour
                    // Fix:
                    // remove this check for EncoreIntro and add a xboundary
                    // or maybe check for this and respawn both players anyways
                    // alternatively remove the water
                    if (!EncoreIntro) {
                        // Encore death, do a quick respawn
                        Player_HandleQuickRespawn(player);
                        if (!player->playerID && globals->gameMode == MODE_ENCORE)
                            Player_HandleQuickRespawn(RSDK_GET_ENTITY(SLOT_PLAYER2, Player));

                        if (player->classID == Player->classID) {
                            player->abilityValues[0] = 0;
                            Player_ResetBoundaries(player);
                            if (BoundsMarker)
                                BoundsMarker_CheckAllBounds(player, true);
                        }
                    }
                }
            }
#endif
#if RETRO_USE_PLUS
        }
#endif
    }
}
void Player_HandleQuickRespawn(EntityPlayer *player)
{
    player->drawOrder = Zone->playerDrawLow;
    if (!player->sidekick)
        player->stateInput = Player_ProcessP1Input;
    else
        player->stateInput = Player_ProcessP2Input_AI;
    player->tileCollisions = true;
    player->interaction    = true;
    player->collisionPlane = 0;
    player->onGround       = false;
    player->active         = ACTIVE_NORMAL;
    player->ringExtraLife  = 100;
    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
    int32 screen = RSDK.GetEntityID(player);

    player->position.x  = StarPost->playerPositions[screen].x;
    player->position.y  = StarPost->playerPositions[screen].y;
    player->direction   = StarPost->playerDirections[screen];
    player->scrollDelay = 0;
    if (player->camera) {
        player->camera->position.x = player->position.x;
        player->camera->position.y = player->position.y;
        player->camera->state      = Camera_State_Follow;
        Zone_ReloadScene(screen);
    }
    player->underwater = 0;

    Player_UpdatePhysicsState(player);
    player->drawFX &= ~FX_SCALE;
    player->scale.x = 0x200;
    player->scale.y = 0x200;
    player->state   = Player_State_Air;
}

// Collision
Hitbox *Player_GetHitbox(EntityPlayer *player)
{
    Hitbox *playerHitbox = RSDK.GetHitbox(&player->animator, 0);
    return playerHitbox ? playerHitbox : &Player_FallbackHitbox;
}
Hitbox *Player_GetAltHitbox(EntityPlayer *player)
{
    Hitbox *playerHitbox = RSDK.GetHitbox(&player->animator, 1);
    return playerHitbox ? playerHitbox : &Player_FallbackHitbox;
}

bool32 Player_CheckCollisionTouch(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
#if RETRO_USE_PLUS
    if (player->isGhost)
        return false;
#endif

    Hitbox *playerHitbox = player->outerbox ? player->outerbox : Player_GetHitbox(player);
    Entity *entity = (Entity *)e;

    return RSDK.CheckObjectCollisionTouchBox(entity, entityHitbox, player, playerHitbox);
}
bool32 Player_CheckCollisionBox(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
#if RETRO_USE_PLUS
    if (player->isGhost)
        return C_NONE;
#endif

    Entity *entity = (Entity *)e;
    Hitbox *playerHitbox = player->outerbox ? player->outerbox : Player_GetHitbox(player);

    switch (RSDK.CheckObjectCollisionBox(entity, entityHitbox, player, playerHitbox, true)) {
        default:
        case C_NONE: return C_NONE;

        case C_TOP: {
            player->controlLock   = 0;
            player->collisionMode = CMODE_FLOOR;

            int32 colPos[2];
            if (entity->direction & FLIP_X) {
                colPos[0] = entity->position.x - (entityHitbox->right << 16);
                colPos[1] = entity->position.x - (entityHitbox->left << 16);
            }
            else {
                colPos[0] = entity->position.x + (entityHitbox->left << 16);
                colPos[1] = entity->position.x + (entityHitbox->right << 16);
            }

            int32 sensorX1 = player->position.x + player->sensorX[0];
            int32 sensorX3 = player->position.x + player->sensorX[2];
            int32 sensorX2 = player->position.x + player->sensorX[1];
            int32 sensorX4 = player->position.x + player->sensorX[3];
            int32 sensorX5 = player->position.x + player->sensorX[4];

            if (sensorX1 >= colPos[0] && sensorX1 <= colPos[1])
                player->flailing |= 1;
            if (sensorX2 >= colPos[0] && sensorX2 <= colPos[1])
                player->flailing |= 2;
            if (sensorX3 >= colPos[0] && sensorX3 <= colPos[1])
                player->flailing |= 4;
            if (sensorX4 >= colPos[0] && sensorX4 <= colPos[1])
                player->flailing |= 8;
            if (sensorX5 >= colPos[0] && sensorX5 <= colPos[1])
                player->flailing |= 0x10;

            if (entity->velocity.y <= 0)
                player->collisionFlagV |= 1;
            return C_TOP;
        }

        case C_LEFT:
            player->controlLock = 0;
            if (player->left && player->onGround) {
                // Bug Details:
                // if you spindash while touching the left side of an object that uses this func to collide
                // and if you hold left & are on ground you'll move left
                // thats the -0x8000 vel being applied every frame via ProcessTileCollisions, since spindash state doesn't expect you to have any x
                // velocity unless it says so, so its ignored due to the way collision works, this only happens on the left, as standing idle against
                // a solid object on the right doesn't count as colliding, while standing on the left does Fix (idk why you'd want to): just place a
                // check to make sure player->state isn't Player_State_Spindash here (example fix in the line below, only the velocity needs to be
                // checked for to fix the bug) if (player->state != Player_State_Spindash)
                player->groundVel = -0x8000;
                player->position.x &= 0xFFFF0000;
            }
            return C_LEFT;

        case C_RIGHT:
            player->controlLock = 0;
            if (player->right && player->onGround)
                player->groundVel = 0x8000;
            return C_RIGHT;

        case C_BOTTOM: return C_BOTTOM;
    }
}
bool32 Player_CheckCollisionPlatform(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
#if RETRO_USE_PLUS
    if (player->isGhost)
        return false;
#endif

    Hitbox *playerHitbox = player->outerbox ? player->outerbox : Player_GetHitbox(player);
    Entity *entity = (Entity *)e;

    if (RSDK.CheckObjectCollisionPlatform(entity, entityHitbox, player, playerHitbox, true)) {
        player->controlLock   = 0;
        player->collisionMode = CMODE_FLOOR;

        int32 colPos[2];
        if (entity->direction & FLIP_X) {
            colPos[0] = entity->position.x - (entityHitbox->right << 16);
            colPos[1] = entity->position.x - (entityHitbox->left << 16);
        }
        else {
            colPos[0] = entity->position.x + (entityHitbox->left << 16);
            colPos[1] = entity->position.x + (entityHitbox->right << 16);
        }

        int32 sensorX1 = player->position.x + player->sensorX[0];
        int32 sensorX3 = player->position.x + player->sensorX[2];
        int32 sensorX2 = player->position.x + player->sensorX[1];
        int32 sensorX4 = player->position.x + player->sensorX[3];
        int32 sensorX5 = player->position.x + player->sensorX[4];

        if (sensorX1 >= colPos[0] && sensorX1 <= colPos[1])
            player->flailing |= 0x01;
        if (sensorX2 >= colPos[0] && sensorX2 <= colPos[1])
            player->flailing |= 0x02;
        if (sensorX3 >= colPos[0] && sensorX3 <= colPos[1])
            player->flailing |= 0x04;
        if (sensorX4 >= colPos[0] && sensorX4 <= colPos[1])
            player->flailing |= 0x08;
        if (sensorX5 >= colPos[0] && sensorX5 <= colPos[1])
            player->flailing |= 0x10;

        if (entity->velocity.y <= 0)
            player->collisionFlagV |= 1;

        return true;
    }

    return false;
}

bool32 Player_CheckHit(EntityPlayer *player, void *e)
{
    Entity *entity = (Entity *)e;

    if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown || player->invincibleTimer
        || player->blinkTimer > 0) {
        return false;
    }

    player->velocity.x = player->position.x > entity->position.x ? 0x20000 : -0x20000;
    Player_Hit(player);

    return true;
}
bool32 Player_CheckHitFlip(EntityPlayer *player)
{
    if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown || player->invincibleTimer
        || player->blinkTimer > 0) {
        return false;
    }

    player->velocity.x = player->direction != FLIP_NONE ? 0x20000 : -0x20000;
    Player_Hit(player);

    return true;
}
// Hits the player if not invulnerable and the player's shield doesn't match shield, returns true if player was hit
bool32 Player_CheckElementalHit(EntityPlayer *player, void *entity, int32 shield)
{
    return player->shield != shield ? Player_CheckHit(player, entity) : false;
}
bool32 Player_CheckAttacking(EntityPlayer *player, void *e)
{
    Entity *entity   = (Entity *)e;
    int32 anim       = player->animator.animationID;
    bool32 attacking = player->invincibleTimer != 0 || anim == ANI_JUMP || anim == ANI_SPINDASH;
    switch (player->characterID) {
        case ID_SONIC: attacking |= anim == ANI_DROPDASH; break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: attacking |= anim == ANI_DROPDASH; break;
#endif
        case ID_TAILS:
            if (!attacking && entity) {
                attacking = anim == ANI_FLY || anim == ANI_FLYTIRED || anim == ANI_FLYLIFT;
                if (player->position.y <= entity->position.y)
                    return false;
            }
            break;
        case ID_KNUCKLES: attacking |= anim == ANI_FLY || anim == ANI_FLYLIFTTIRED; break;
        default: break;
    }

    return attacking;
}
bool32 Player_CheckBadnikTouch(EntityPlayer *player, void *e, Hitbox *entityHitbox)
{
    Entity *entity = (Entity *)e;
#if RETRO_USE_PLUS
    if (player->isGhost)
        return false;
#endif

    Hitbox *playerHitbox = Player_GetHitbox(player);
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY && player->jumpAbilityState > 1) {
        Hitbox tempHitbox;

        if (player->isChibi) {
            tempHitbox.left  = -32;
            tempHitbox.right = 32;
        }
        else {
            tempHitbox.left  = -48;
            tempHitbox.right = 48;
        }

        tempHitbox.top    = 0;
        tempHitbox.bottom = playerHitbox->bottom + 16;
        if (RSDK.CheckObjectCollisionTouchBox(entity, entityHitbox, player, &tempHitbox))
            return true;
    }
#endif

    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + player->playerID, Shield);
    if (shield->classID == Shield->classID && shield->state == Shield_State_Insta) {
        Hitbox tempHitbox;

        tempHitbox.left   = (playerHitbox->left << 1) - (playerHitbox->left >> 1);
        tempHitbox.top    = (playerHitbox->top << 1) - (playerHitbox->top >> 1);
        tempHitbox.right  = (playerHitbox->right << 1) - (playerHitbox->right >> 1);
        tempHitbox.bottom = (playerHitbox->bottom << 1) - (playerHitbox->bottom >> 1);
        playerHitbox      = &tempHitbox;
    }

    return RSDK.CheckObjectCollisionTouchBox(entity, entityHitbox, player, playerHitbox);
}
bool32 Player_CheckBadnikBreak(EntityPlayer *player, void *e, bool32 destroy)
{
    Entity *entity = (Entity *)e;

    if (Player_CheckAttacking(player, entity)) {
        CREATE_ENTITY(Animals, intToVoid((Animals->animalTypes[(RSDK.Rand(0, 32) >> 4)]) + 1), entity->position.x, entity->position.y);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y);
        explosion->drawOrder       = Zone->objectDrawHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

        if (Zone_GetZoneID() > ZONE_INVALID) {
            int32 characterID = 0;
            switch (player->characterID) {
                case ID_SONIC: characterID = 1; break;
                case ID_TAILS: characterID = 2; break;
                case ID_KNUCKLES: characterID = 3; break;
#if RETRO_USE_PLUS
                case ID_MIGHTY: characterID = 4; break;
#endif
                default: characterID = 0;
#if RETRO_USE_PLUS
                    if (player->characterID == ID_RAY)
                        characterID = 5;
#endif
                    break;
            }

            StatInfo info;
#if RETRO_USE_PLUS
            TimeAttackData_TrackEnemyDefeat(&info, Zone_GetZoneID(), Zone->actID, characterID, SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE),
                                            (entity->position.x >> 0x10), (entity->position.y >> 0x10));
            API.TryTrackStat(&info);
#else
            APICallback_TrackEnemyDefeat(Zone_GetZoneID(), Zone->actID, characterID, (entity->position.x >> 0x10), (entity->position.y >> 0x10));
#endif
        }

        int32 yVel = 0;
        if (player->velocity.y <= 0) {
            yVel = player->velocity.y + 0x10000;
        }
        else if (player->position.y >= entity->position.y
#if RETRO_USE_PLUS
                 || (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_DROPDASH)
#endif
        ) {
            yVel = player->velocity.y - 0x10000;
        }
        else {
            yVel = -(player->velocity.y + 2 * player->gravityStrength);
        }
        player->velocity.y = yVel;

        if (globals->gameMode != MODE_COMPETITION)
            player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        EntityScoreBonus *scoreBonus = CREATE_ENTITY(ScoreBonus, NULL, entity->position.x, entity->position.y);
        scoreBonus->drawOrder        = Zone->objectDrawHigh;
        scoreBonus->animator.frameID = player->scoreBonus;

        switch (player->scoreBonus) {
            case 0: Player_GiveScore(player, 100); break;
            case 1: Player_GiveScore(player, 200); break;
            case 2: Player_GiveScore(player, 500); break;

            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14: Player_GiveScore(player, 1000); break;

            case 15: Player_GiveScore(player, 10000); break;

            default: break;
        }

        if (player->scoreBonus < 15)
            player->scoreBonus++;

        if (destroy) {
            destroyEntity(entity);
            entity->active = ACTIVE_NEVER2;
        }

        return true;
    }
#if RETRO_USE_PLUS
    else if (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_CROUCH) {
        if (player->uncurlTimer)
            return false;

        RSDK.PlaySfx(Player->sfxPimPom, false, 255);

        player->uncurlTimer = 30;
        player->groundVel   = entity->position.x > player->position.x ? -0x10000 : 0x10000;
    }
#endif
    else {
#if RETRO_USE_PLUS
        if (!player->uncurlTimer) {
#endif
            Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
        }
#endif
    }

    return false;
}
bool32 Player_CheckBossHit(EntityPlayer *player, void *e)
{
    Entity *entity = (Entity *)e;

    if (Player_CheckAttacking(player, entity)) {
        player->groundVel  = -player->groundVel;
        player->velocity.x = -player->velocity.x;
        player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);

        if (player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_FLY) {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLYTIRED, &player->animator, false, 0);
            player->state = Player_State_KnuxGlideDrop;
        }
        else {
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop || player->state == Player_State_RayGlide) {
                player->state = Player_State_Air;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                if (player->velocity.y < -0x40000)
                    player->velocity.y = -0x40000;
            }
#endif
        }

        return true;
    }
    else {
        Player_CheckHit(player, entity);
    }

    return false;
}
bool32 Player_CheckProjectileHit(EntityPlayer *player, void *p)
{
    Entity *projectile = (Entity *)p;

    bool32 deflected   = false;
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY) {
        int32 anim = player->animator.animationID;
        if (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH)
            deflected = true;
    }
#endif

    if (player->shield > SHIELD_BLUE || deflected) {
#if RETRO_USE_PLUS
        if (deflected)
            RSDK.PlaySfx(Player->sfxMightyDeflect, false, 0xFE);
#endif

        int32 angle             = RSDK.ATan2(player->position.x - projectile->position.x, player->position.y - projectile->position.y);
        projectile->velocity.x  = -0x800 * RSDK.Cos256(angle);
        projectile->velocity.y  = -0x800 * RSDK.Sin256(angle);
        projectile->interaction = false;
        return true;
    }

    Player_CheckHit(player, projectile);

    return false;
}
#if RETRO_USE_PLUS
bool32 Player_CheckMightyShellHit(EntityPlayer *player, void *e, int32 velX, int32 velY)
{
    Entity *entity = (Entity *)e;

    if (player->characterID == ID_MIGHTY
        && (player->animator.animationID == ANI_CROUCH || player->animator.animationID == ANI_JUMP || player->animator.animationID == ANI_SPINDASH
            || player->animator.animationID == ANI_DROPDASH)) {
        if (!player->uncurlTimer) {
            RSDK.PlaySfx(Player->sfxPimPom, false, 255);
            player->uncurlTimer = 30;
        }

        int32 angle        = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
        entity->velocity.x = velX * RSDK.Cos256(angle);
        entity->velocity.y = velY * RSDK.Sin256(angle);
        return true;
    }

    return false;
}
#endif
bool32 Player_CheckItemBreak(EntityPlayer *player, void *e, bool32 hitIfNotAttacking)
{
    Entity *entity  = (Entity *)e;
    int32 anim      = player->animator.animationID;
    int32 character = player->characterID;

    if (Player_CheckAttacking(player, entity)) {
        if (player->velocity.y <= 0) {
            player->velocity.y += 0x10000;
            return true;
        }
        else if (player->position.y >= entity->position.y
#if RETRO_USE_PLUS
                 || (checkPlayerID(ID_MIGHTY, 1) && anim == ANI_DROPDASH)
#endif
        ) {
            player->velocity.y -= 0x10000;
            return true;
        }
        else {
            player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
            return true;
        }
    }
    else {
        if (hitIfNotAttacking) {
#if RETRO_USE_PLUS
            if (!(character == ID_MIGHTY && anim == ANI_CROUCH)) {
#endif
                Player_CheckHit(player, entity);
#if RETRO_USE_PLUS
            }
#endif
        }
        return false;
    }
}

// Player State Helpers
void Player_UpdatePhysicsState(EntityPlayer *entity)
{
    int32 *tablePtr = NULL;
    switch (entity->characterID) {
        default:
        case ID_SONIC: tablePtr = Player->sonicPhysicsTable; break;
        case ID_TAILS: tablePtr = Player->tailsPhysicsTable; break;
        case ID_KNUCKLES: tablePtr = Player->knuxPhysicsTable; break;
#if RETRO_USE_PLUS
        case ID_MIGHTY: tablePtr = Player->mightyPhysicsTable; break;
        case ID_RAY: tablePtr = Player->rayPhysicsTable; break;
#endif
    }

    int32 tablePos = 0;
    if (entity->underwater) {
        entity->gravityStrength = 0x2000;
        tablePos                = 1;
        if (entity->speedShoesTimer >= 0)
            entity->gravityStrength = 0x1000;
    }
    else {
        entity->gravityStrength = 0x5800;
        if (entity->speedShoesTimer >= 0)
            entity->gravityStrength = 0x3800;
    }

    int32 decelShift = 0;
    if (entity->superState == SUPERSTATE_SUPER) {
        tablePos |= 2;
        decelShift = 2;
    }

    if (entity->speedShoesTimer > 0) {
        tablePos |= 4;
        decelShift = 1;
    }

    int32 tableID               = 8 * tablePos;
    entity->topSpeed            = tablePtr[tableID];
    entity->acceleration        = tablePtr[tableID + 1];
    entity->deceleration        = tablePtr[tableID + 1] >> decelShift;
    entity->airAcceleration     = tablePtr[tableID + 2];
    entity->airDeceleration     = tablePtr[tableID + 3];
    entity->skidSpeed           = tablePtr[tableID + 4];
    entity->rollingFriction     = tablePtr[tableID + 5];
    entity->jumpStrength        = tablePtr[tableID + 6];
    entity->jumpCap             = tablePtr[tableID + 7];
    entity->rollingDeceleration = 0x2000;

    if (entity->speedShoesTimer < 0) {
        entity->topSpeed >>= 1;
        entity->acceleration >>= 1;
        entity->airAcceleration >>= 1;
        entity->skidSpeed >>= 1;
        entity->rollingFriction >>= 1;
        entity->airDeceleration >>= 1;
    }

    if (entity->isChibi) {
        entity->topSpeed -= entity->topSpeed >> 3;
        entity->acceleration -= entity->acceleration >> 4;
        entity->airAcceleration -= entity->airAcceleration >> 4;
        entity->jumpStrength -= entity->jumpStrength >> 3;
        entity->jumpCap -= entity->jumpCap >> 3;
    }
}
void Player_HandleGroundMovement(void)
{
    RSDK_THIS(Player);

    if (self->controlLock > 0) {
        self->controlLock--;

        if (!self->invertGravity)
            self->groundVel += RSDK.Sin256(self->angle) << 13 >> 8;
    }
    else {
        bool32 left  = false;
        bool32 right = false;

        if (self->invertGravity) {
            right = self->left;
            left  = self->right;
        }
        else {
            left  = self->left;
            right = self->right;
        }

        if (left) {
            if (self->groundVel > -self->topSpeed) {
                if (self->groundVel <= 0) {
                    self->groundVel -= self->acceleration;
                }
                else {
                    if (!self->collisionMode && self->groundVel > 0x40000 && !Zone->autoScrollSpeed) {
                        self->direction = FLIP_NONE;
                        self->skidding  = 24;
                    }

                    if (self->groundVel < self->skidSpeed)
                        self->groundVel = -abs(self->skidSpeed);
                    else 
                        self->groundVel -= self->skidSpeed;
                }
            }

            if (self->groundVel <= 0 && self->skidding < 1)
                self->direction = FLIP_X;
        }

        if (right) {
            if (self->groundVel < self->topSpeed) {
                if (self->groundVel >= 0) {
                    self->groundVel += self->acceleration;
                }
                else {
                    if (!self->collisionMode && self->groundVel < -0x40000 && !Zone->autoScrollSpeed) {
                        self->direction = FLIP_X;
                        self->skidding  = 24;
                    }

                    if (self->groundVel > -self->skidSpeed)
                        self->groundVel = abs(self->skidSpeed);
                    else 
                        self->groundVel += self->skidSpeed;
                }
            }

            if (self->groundVel >= 0 && self->skidding < 1)
                self->direction = FLIP_NONE;
        }

        if (self->left || self->right) {
            if (!self->invertGravity) {
                self->groundVel += RSDK.Sin256(self->angle) << 13 >> 8;

                if (self->right) {
                    if (!self->left) {
                        if (self->angle > 0xC0 && self->angle < 0xE4 && self->groundVel > -0x20000 && self->groundVel < 0x28000) {
                            self->controlLock = 30;
                        }
                    }
                }
                else if (self->left) {
                    if (self->angle > 0x1C && self->angle < 0x40 && self->groundVel > -0x28000 && self->groundVel < 0x20000) {
                        self->controlLock = 30;
                    }
                }
            }
        }
        else {
            if (self->groundVel <= 0) {
                self->groundVel += self->deceleration;

                if (self->groundVel > 0)
                    self->groundVel = 0;
            }
            else {
                self->groundVel -= self->deceleration;

                if (self->groundVel < 0)
                    self->groundVel = 0;
            }

            if (!self->invertGravity) {
                if (self->groundVel > 0x2000 || self->groundVel < -0x2000)
                    self->groundVel += RSDK.Sin256(self->angle) << 13 >> 8;

                if (self->angle > 0xC0 && self->angle < 0xE4) {
                    if (abs(self->groundVel) < 0x10000)
                        self->controlLock = 30;
                }

                if (self->angle > 0x1C && self->angle < 0x40) {
                    if (abs(self->groundVel) < 0x10000)
                        self->controlLock = 30;
                }
            }
        }
    }

    if (!self->invertGravity && self->collisionMode != CMODE_FLOOR && self->collisionMode <= CMODE_RWALL) {
        if (self->angle >= 0x40 && self->angle <= 0xC0 && abs(self->groundVel) < 0x20000) {
            self->velocity.x    = self->groundVel * RSDK.Cos256(self->angle) >> 8;
            self->velocity.y    = self->groundVel * RSDK.Sin256(self->angle) >> 8;
            self->onGround      = false;
            self->angle         = 0;
            self->collisionMode = CMODE_FLOOR;
        }
    }
}
void Player_HandleGroundRotation(void)
{
    RSDK_THIS(Player);

    if (self->angle <= 0x04 || self->angle >= 0xFC) {
        if (self->angle <= 0x10 || self->angle >= 0xE8)
            self->rotation = 0;
        else
            self->rotation <<= 1;
    }
    else {
        int32 targetRotation = 0;
        if (self->angle > 0x10 && self->angle < 0xE8)
            targetRotation = self->angle << 1;

        int32 rotate  = targetRotation - self->rotation;
        int32 shift = (abs(self->groundVel) <= 0x60000) + 1;

        if (abs(rotate) >= abs(rotate - 0x200)) {
            if (abs(rotate - 0x200) < abs(rotate + 0x200))
                self->rotation += (rotate - 0x200) >> shift;
            else 
                self->rotation += (rotate + 0x200) >> shift;
        }
        else {
            if (abs(rotate) < abs(rotate + 0x200))
                self->rotation += rotate >> shift;
            else
                self->rotation += (rotate + 0x200) >> shift;
        }

        self->rotation &= 0x1FF;
    }
}
void Player_HandleAirMovement(void)
{
    RSDK_THIS(Player);

    if (self->camera) {
        self->camera->disableYOffset = true;
        self->camera->offset.y    = 0x200000;
    }

    self->velocity.y += self->gravityStrength;
    if (self->velocity.y < self->jumpCap && self->animator.animationID == ANI_JUMP && !self->jumpHold && self->applyJumpCap) {
        self->velocity.x -= self->velocity.x >> 5;
        self->velocity.y = self->jumpCap;
    }

    self->collisionMode = CMODE_FLOOR;
    self->pushing       = 0;

    if (self->rotation >= 0x100) {
        if (self->rotation < 0x200)
            self->rotation += 4;
    }
    else if (self->rotation > 0) {
        self->rotation -= 4;
        self->rotation = self->rotation;
    }
    else
        self->rotation = 0;
}
void Player_HandleAirFriction(void)
{
    RSDK_THIS(Player);

    if (self->velocity.y > -0x40000 && self->velocity.y < 0)
        self->velocity.x -= self->velocity.x >> 5;

    if (self->velocity.x <= -self->topSpeed) {
        if (self->left)
            self->direction = FLIP_X;
    }
    else if (self->left) {
        self->velocity.x -= self->airAcceleration;
        self->direction = FLIP_X;
    }

    if (self->velocity.x >= self->topSpeed) {
        if (self->right)
            self->direction = FLIP_NONE;
    }
    else if (self->right) {
        self->direction = 0;
        self->velocity.x += self->airAcceleration;
    }
}
void Player_StartJump(EntityPlayer *entity)
{
    entity->controlLock = 0;
    entity->onGround    = false;

    if (entity->collisionMode == CMODE_FLOOR && entity->state != Player_State_Roll)
        entity->position.y += entity->jumpOffset;

    int32 jumpForce    = entity->gravityStrength + entity->jumpStrength;
    entity->velocity.x = (entity->groundVel * RSDK.Cos256(entity->angle) + jumpForce * RSDK.Sin256(entity->angle)) >> 8;
    entity->velocity.y = (entity->groundVel * RSDK.Sin256(entity->angle) - jumpForce * RSDK.Cos256(entity->angle)) >> 8;

    if (entity->camera) {
        entity->camera->disableYOffset = true;
        entity->camera->offset.y    = 0x200000;
    }

    RSDK.SetSpriteAnimation(entity->aniFrames, ANI_JUMP, &entity->animator, false, 0);
    if (entity->characterID == ID_TAILS)
        entity->animator.speed = 120;
    else
        entity->animator.speed = ((abs(entity->groundVel) * 0xF0) / 0x60000) + 0x30;

    if (entity->animator.speed > 0xF0)
        entity->animator.speed = 0xF0;

    entity->angle            = 0;
    entity->collisionMode    = CMODE_FLOOR;
    entity->skidding         = 0;
    entity->applyJumpCap     = true;
    entity->jumpAbilityState = 1;

    RSDK.PlaySfx(Player->sfxJump, false, 255);
    entity->state = Player_State_Air;
}
void Player_StartRoll(void)
{
    RSDK_THIS(Player);

    RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);

    self->pushing = 0;
    self->state   = Player_State_Roll;
    if (self->collisionMode == CMODE_FLOOR)
        self->position.y += self->jumpOffset;
}
void Player_StartPeelout(void)
{
    RSDK_THIS(Player);

    RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, true, 0);

    self->state          = Player_State_Peelout;
    self->abilityTimer   = 0;
    self->spindashCharge = 0;
    RSDK.PlaySfx(Player->sfxPeelCharge, false, 255);
}
#if RETRO_USE_PLUS
bool32 Player_SwapMainPlayer(bool32 forceSwap)
{
    RSDK_THIS(Player);

    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    if (!sidekick->classID)
        return false;

    if (!forceSwap) {
        if (Player->cantSwap || self->drawOrder == 2 || !SceneInfo->timeEnabled)
            return false;

        if (Player->jumpInTimer) {
            Player->jumpInTimer = 240;
            return false;
        }

        if (self->state == Player_State_Die || self->state == Player_State_Drown) {
            if (sidekick->state == Player_State_Die || sidekick->state == Player_State_Drown || sidekick->state == Player_State_EncoreRespawn
                || sidekick->state == Player_State_None) {
                return false;
            }
        }
        else {
            if (self->state != Player_State_Ground && self->state != Player_State_Roll)
                return false;
            if (sidekick->state != Player_State_Ground && sidekick->state != Player_State_Roll)
                return false;
        }
    }

    EntityPlayer *leader             = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *sidekickCam        = sidekick->camera;
    int32 sidekickController         = sidekick->controllerID;
    void (*sidekickInputState)(void) = sidekick->stateInput;
    if (sidekick->state == Ice_State_FrozenPlayer) {
        Ice_BreakPlayerBlock(sidekick);
        Ice->playerTimers[0] = 30;
        Ice->playerTimers[1] = 0;
        foreach_all(Ice, ice)
        {
            if (ice->state == Ice_State_PlayerBlock)
                destroyEntity(ice);
        }
    }
    RSDK.CopyEntity(Zone->entityStorage, leader, false);
    RSDK.CopyEntity(leader, sidekick, false);
    RSDK.CopyEntity(sidekick, (Entity *)Zone->entityStorage, false);

    sidekick->state       = Player_State_Air;
    self->controllerID    = sidekick->controllerID;
    self->stateInput      = sidekick->stateInput;
    self->sidekick        = false;
    self->score           = sidekick->score;
    self->score1UP        = sidekick->score1UP;
    self->rings           = sidekick->rings;
    self->ringExtraLife   = sidekick->ringExtraLife;
    self->shield          = sidekick->shield;
    self->camera          = sidekick->camera;
    self->invincibleTimer = sidekick->invincibleTimer;
    self->speedShoesTimer = sidekick->speedShoesTimer;
    self->hyperRing       = sidekick->hyperRing;
    self->playerID        = sidekick->playerID;
    self->scrollDelay     = 0;
    self->camera->state   = Camera_State_Follow;
    Player_UpdatePhysicsState(self);

    sidekick->controllerID    = sidekickController;
    sidekick->stateInput      = sidekickInputState;
    sidekick->sidekick        = true;
    sidekick->score           = 0;
    sidekick->score1UP        = 0;
    sidekick->rings           = 0;
    sidekick->ringExtraLife   = 0;
    sidekick->shield          = SHIELD_NONE;
    sidekick->invincibleTimer = 0;
    sidekick->speedShoesTimer = 0;
    sidekick->camera          = sidekickCam;
    sidekick->hyperRing       = false;
    sidekick->playerID        = RSDK.GetEntityID(sidekick);
    Player_UpdatePhysicsState(sidekick);

    if (sidekick->superState == SUPERSTATE_SUPER)
        Player_CheckGoSuper(self, 0xFF);

    globals->playerID = self->characterID + (sidekick->characterID << 8);
    RSDK.PlaySfx(Player->sfxSwap, false, 255);
    HUD->swapCooldown = 60;

    return true;
}
#endif

// Player States
void Player_HandleRollDeceleration(void)
{
    RSDK_THIS(Player);

    int32 initialVel = self->groundVel;
    if (self->right && self->groundVel < 0)
        self->groundVel += self->rollingDeceleration;

    if (self->left && self->groundVel > 0)
        self->groundVel -= self->rollingDeceleration;

    if (self->groundVel) {
        if (self->groundVel < 0) {
            self->groundVel += self->rollingFriction;

            if (RSDK.Sin256(self->angle) >= 0)
                self->groundVel += 0x1400 * RSDK.Sin256(self->angle) >> 8;
            else
                self->groundVel += 0x5000 * RSDK.Sin256(self->angle) >> 8;

            if (self->groundVel < -0x120000)
                self->groundVel = -0x120000;
        }
        else {
            self->groundVel -= self->rollingFriction;

            if (RSDK.Sin256(self->angle) <= 0)
                self->groundVel += 0x1400 * RSDK.Sin256(self->angle) >> 8;
            else
                self->groundVel += 0x5000 * RSDK.Sin256(self->angle) >> 8;

            if (self->groundVel > 0x120000)
                self->groundVel = 0x120000;
        }
    }
    else {
        self->groundVel += 0x5000 * RSDK.Sin256(self->angle) >> 8;
    }

    switch (self->collisionMode) {
        case CMODE_FLOOR:
            if (self->state == Player_State_ForceRoll_Ground) {
                if (abs(self->groundVel) < 0x10000) {
                    if (self->direction & FLIP_Y)
                        self->groundVel = -0x40000;
                    else
                        self->groundVel = 0x40000;
                }
            }
            else {
                if ((self->groundVel >= 0 && initialVel <= 0) || (self->groundVel <= 0 && initialVel >= 0)) {
                    self->groundVel = 0;
                    self->state     = Player_State_Ground;
                }
            }
            break;

        case CMODE_LWALL:
        case CMODE_RWALL:
            if (self->angle >= 0x40 && self->angle <= 0xC0) {
                if (abs(self->groundVel) < 0x20000) {
                    self->velocity.x    = self->groundVel * RSDK.Cos256(self->angle) >> 8;
                    self->velocity.y    = self->groundVel * RSDK.Sin256(self->angle) >> 8;
                    self->onGround      = false;
                    self->angle         = 0;
                    self->collisionMode = CMODE_FLOOR;
                }
            }
            break;

        case CMODE_ROOF:
            if (self->invertGravity) {
                if ((self->groundVel >= 0 && initialVel <= 0) || (self->groundVel <= 0 && initialVel >= 0)) {
                    self->groundVel = 0;
                    self->state     = Player_State_Ground;
                }
            }
            else {
                if (self->angle >= 0x40 && self->angle <= 0xC0) {
                    if (abs(self->groundVel) < 0x20000) {
                        self->velocity.x    = self->groundVel * RSDK.Cos256(self->angle) >> 8;
                        self->velocity.y    = self->groundVel * RSDK.Sin256(self->angle) >> 8;
                        self->onGround      = false;
                        self->angle         = 0;
                        self->collisionMode = CMODE_FLOOR;
                    }
                }
            }
            break;
    }
}
void Player_Hit(EntityPlayer *player)
{
    uint8 hurtType = PLAYER_HURT_NONE;
    if (player->sidekick) {
        hurtType = PLAYER_HURT_HASSHIELD;
    }
    else {
        int32 entityID       = RSDK.GetEntityID(player);
        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + entityID, Shield);
        if (shield->classID == Shield->classID) {
            player->shield = SHIELD_NONE;
            destroyEntity(shield);
            hurtType = PLAYER_HURT_HASSHIELD;
        }
        else {
            hurtType = (player->rings <= 0) + PLAYER_HURT_RINGLOSS;
        }

        if (!Player->gotHit[player->playerID])
            Player->gotHit[player->playerID] = true;
    }

    player->nextAirState    = StateMachine_None;
    player->nextGroundState = StateMachine_None;
    if (globals->coolBonus[player->playerID] > 0)
        globals->coolBonus[player->playerID] -= 1000;

    switch (hurtType) {
        default: break;

        case PLAYER_HURT_HASSHIELD: // Hurt, no rings (shield/P2/etc)
            player->state = Player_State_Hit;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
            player->velocity.y     = -0x40000;
            player->onGround       = false;
            player->tileCollisions = true;
            player->blinkTimer     = 120;
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x20000;
            }
            RSDK.PlaySfx(Player->sfxHurt, false, 0xFF);
            break;

        case PLAYER_HURT_RINGLOSS: // Hurt, lost rings
            player->state = Player_State_Hit;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
            player->velocity.y     = -0x40000;
            player->onGround       = false;
            player->tileCollisions = true;
            player->blinkTimer     = 120;
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x20000;
            }

            if (player->hyperRing)
                Ring_LoseHyperRings(player, player->rings, player->collisionPlane);
            else
                Ring_LoseRings(player, player->rings, player->collisionPlane);

            player->hyperRing     = false;
            player->rings         = 0;
            player->ringExtraLife = 100;
            RSDK.PlaySfx(Player->sfxLoseRings, false, 0xFF);
            break;

        case PLAYER_HURT_DIE: // Hurt, dies.
            player->deathType = PLAYER_DEATH_DIE_NOSFX;
            break;
    }
}
bool32 Player_CheckValidState(EntityPlayer *player)
{
    if (player->classID == Player->classID && !player->deathType) {
        if (player->state != Player_State_Die && player->state != Player_State_Drown &&
#if RETRO_USE_PLUS
            player->state != Player_State_EncoreRespawn && player->state != Player_State_JumpIn &&
#endif
            player->state != Player_State_StartJumpIn && player->state != Player_State_FlyIn && player->state != Player_State_Transform) {
            return true;
        }
    }

    return false;
}
void Player_CheckStartFlyCarry(EntityPlayer *leader)
{
    RSDK_THIS(Player);

    if (self->flyCarryTimer > 0)
        self->flyCarryTimer--;

    int32 off;
    if (leader->animator.animationID == ANI_JUMP)
        off = self->position.y + 0x210000;
    else
        off = self->position.y + 0x1C0000;

    if (leader->state != Player_State_FlyCarried && (!leader->onGround || self->velocity.y < 0)) {
        bool32 canFlyCarry = (leader->state == Player_State_Roll || leader->state == Player_State_LookUp || leader->state == Player_State_Crouch
                       || leader->state == Player_State_Air || leader->state == Player_State_Ground);
        if (LottoMachine)
            canFlyCarry = canFlyCarry && (!((1 << RSDK.GetEntityID(leader)) & LottoMachine->activePlayers));

        if (canFlyCarry && (leader->animator.animationID != ANI_FAN)) {
            if (abs(self->position.x - leader->position.x) < 0xC0000 && abs(off - leader->position.y) < 0xC0000 && !self->flyCarryTimer
                && !leader->down && !leader->onGround) {
                RSDK.SetSpriteAnimation(leader->aniFrames, ANI_HANG, &leader->animator, false, 0);
                leader->state           = Player_State_FlyCarried;
                leader->nextAirState    = StateMachine_None;
                leader->nextGroundState = StateMachine_None;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
            }
        }
    }

    if (leader->state == Player_State_FlyCarried) {
        int32 entityXPos         = self->position.x;
        int32 entityYPos         = self->position.y;
        int32 entityXVel         = self->velocity.x;
        int32 entityYVel         = self->velocity.y;

        Hitbox *sidekickOuterBox = RSDK.GetHitbox(&self->animator, 0);
        Hitbox *sidekickInnerBox = RSDK.GetHitbox(&self->animator, 1);
        if (self->collisionLayers & Zone->moveMask) {
            TileLayer *move  = RSDK.GetTileLayer(Zone->moveLayer);
            move->position.x = -self->moveLayerPosition.x >> 16;
            move->position.y = -self->moveLayerPosition.y >> 16;
        }

        RSDK.ProcessTileCollisions(self, sidekickOuterBox, sidekickInnerBox);

        if (self->onGround && !self->collisionMode)
            self->collisionFlagV |= 1;

        leader->flyCarrySidekickPos.x = self->position.x & 0xFFFF0000;
        leader->flyCarrySidekickPos.y = self->position.y & 0xFFFF0000;

        self->position.x              = entityXPos;
        self->position.y              = entityYPos;
        self->velocity.x              = entityXVel;
        self->velocity.y              = entityYVel;

        leader->position.y = entityYPos + 0x1C0000;
        leader->position.x = entityXPos;
        leader->velocity.x = entityXVel;
        leader->velocity.y = entityYVel;

        Hitbox *leaderOuterBox = Player_GetHitbox(leader);
        Hitbox *leaderInnerBox = Player_GetAltHitbox(leader);
        if (leader->collisionLayers & Zone->moveMask) {
            TileLayer *move  = RSDK.GetTileLayer(Zone->moveLayer);
            move->position.x = -leader->moveLayerPosition.x >> 16;
            move->position.y = -leader->moveLayerPosition.y >> 16;
        }

        RSDK.ProcessTileCollisions(leader, leaderOuterBox, leaderInnerBox);

        if (leader->onGround)
            leader->collisionFlagV |= 1;

        leader->collisionPlane = self->collisionPlane;
        leader->direction      = self->direction;
        leader->velocity.x     = 0;
        leader->velocity.y     = 0;
    }
}
void Player_P2JumpBackIn(void)
{
    RSDK_THIS(Player);

    EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (leader->classID == Player->classID && (leader->drawOrder != 2 || self->drawOrder != 2)) {
        int32 rx = abs((self->position.x >> 0x10) - ScreenInfo->position.x - ScreenInfo->centerX);
        int32 ry = abs((self->position.y >> 0x10) - ScreenInfo->position.y - ScreenInfo->centerY);

        if (rx >= ScreenInfo->centerX + 96 || ry >= ScreenInfo->centerY + 96)
            ++Player->jumpInTimer;
        else
            Player->jumpInTimer = 0;

        if (Player->jumpInTimer >= 240) {
            Player->jumpInTimer    = 0;
            self->state            = Player_State_StartJumpIn;
            self->forceJumpIn      = true;
            self->position.x       = -0x400000;
            self->position.y       = -0x400000;
            self->abilityValues[0] = 0;
            self->drawFX &= ~FX_SCALE;
            self->nextAirState    = StateMachine_None;
            self->nextGroundState = StateMachine_None;
            self->stateInput      = Player_ProcessP2InputLag;
            self->scale.x         = 0x200;
            self->scale.y         = 0x200;
            self->velocity.x      = 0;
            self->velocity.y      = 0;
            self->groundVel       = 0;
            self->tileCollisions  = false;
            self->interaction     = false;
            self->blinkTimer      = 0;
            self->visible         = true;

            if (leader->underwater && leader->position.y < Water->waterLevel) {
                self->drawOrder  = leader->drawOrder;
                self->drownTimer = 0;
            }
            else {
                self->drawOrder  = Zone->playerDrawHigh;
                self->drownTimer = 0;
            }
        }
    }
}
void Player_ForceSuperTransform(void)
{
    RSDK_THIS(Player);

    if (self->characterID == ID_SONIC)
        self->aniFrames = Player->superFrames;

    self->superState = SUPERSTATE_SUPER;

    destroyEntity(RSDK_GET_ENTITY(self->playerID + Player->playerCount, Shield));
    self->invincibleTimer = 60;
    self->superState      = SUPERSTATE_SUPER;
    Player_UpdatePhysicsState(self);

    RSDK.ResetEntityPtr(RSDK_GET_ENTITY(self->playerID + 2 * Player->playerCount, ImageTrail), ImageTrail->classID, self);
#if RETRO_USE_PLUS
    RSDK.ResetEntityPtr(RSDK_GET_ENTITY(self->playerID + Player->playerCount, SuperSparkle), SuperSparkle->classID, self);
#endif

    self->state = Player_State_Ground;
#if RETRO_GAMEVER != VER_100
    Player_State_Ground();
#endif
}

void Player_State_None(void)
{
    // No code. Do nothing. Just vibe.
}
void Player_State_Ground(void)
{
    RSDK_THIS(Player);
    if (!self->onGround) {
        self->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        if (!self->groundVel) {
            if (self->left) 
                --self->pushing;
            else if (self->right) 
                ++self->pushing;
            else 
                self->pushing = 0;
        }
        else {
            if (!self->left && !self->right) {
                self->pushing = 0;
            }
            else {
                if (abs(self->groundVel) > 0x10000)
                    self->pushing = 0;
            }
        }

        if (self->invertGravity) {
            if (self->collisionMode != CMODE_ROOF)
                self->pushing = 0;
        }
        else if (self->collisionMode != CMODE_FLOOR) {
            self->pushing = 0;
        }

        Player_HandleGroundRotation();
        Player_HandleGroundMovement();

        if (self->camera)
            self->camera->disableYOffset = false;
        self->jumpAbilityState = 0;

        self->nextAirState = Player_State_Air;
        if (self->skidding <= 0) {
            if (self->pushing > -3 && self->pushing < 3) {
                if (self->groundVel || (self->angle >= 0x20 && self->angle <= 0xE0 && !self->invertGravity)) {
                    self->timer          = 0;
                    self->outtaHereTimer = 0;
                    if (abs(self->groundVel) >= self->maxWalkSpeed) {
                        if (abs(self->groundVel) >= self->maxJogSpeed) {
                            if (abs(self->groundVel) < self->maxRunSpeed) {
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_RUN, &self->animator, false, self->animator.animationID - ANI_RUN > 1);

                                self->animator.speed = minVal((abs(self->groundVel) >> 12) + 0x60, 0x200);
                                self->maxJogSpeed    = 0x58000;
                                self->maxRunSpeed    = 0xC0000;
                            }
                            else {
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_DASH, &self->animator, false, self->animator.animationID - ANI_RUN > 1);
                                self->maxRunSpeed = 0xB8000;
                            }
                        }
                        else {
                            if (self->animator.animationID != ANI_WALK || self->animator.frameID == 3)
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_JOG, &self->animator, false, 0);
                            self->animator.speed = (abs(self->groundVel) >> 12) + 0x40;
                            self->maxWalkSpeed   = 0x38000;
                            self->maxJogSpeed    = 0x60000;
                        }
                    }
                    else {
                        if (self->animator.animationID == ANI_JOG) {
                            if (self->animator.frameID == 9)
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 9);
                        }
                        else if (self->animator.animationID == ANI_AIRWALK) {
                            RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, self->animator.frameID);
                        }
                        else {
                            RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 0);
                        }
                        self->animator.speed = (abs(self->groundVel) >> 12) + 48;
                        self->maxWalkSpeed   = 0x40000;
                    }
                }
                else {
                    self->maxWalkSpeed = 0x40000;
                    self->maxJogSpeed  = 0x60000;
                    self->maxRunSpeed  = 0xC0000;

                    Vector2 posStore = self->position;

                    self->flailing |=
                        RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, self->sensorX[0], self->sensorY, 10) << 0;
                    self->flailing |=
                        RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, self->sensorX[1], self->sensorY, 10) << 1;
                    self->flailing |=
                        RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, self->sensorX[2], self->sensorY, 10) << 2;
                    self->flailing |=
                        RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, self->sensorX[3], self->sensorY, 10) << 3;
                    self->flailing |=
                        RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_FLOOR, self->collisionPlane, self->sensorX[4], self->sensorY, 10) << 4;

                    self->position = posStore;
                    switch (self->flailing) {
                        case 0b00000001:
                        case 0b00000011:
                            if (self->direction == FLIP_X || (self->characterID == ID_SONIC && self->superState == SUPERSTATE_SUPER)
                                || self->isChibi) {
                                self->direction = FLIP_X;
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_BALANCE1, &self->animator, false, 0);
                            }
                            else {
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_BALANCE2, &self->animator, false, 0);
                            }
                            break;

                        case 0b00010000:
                        case 0b00011000:
                            if (self->direction && (self->characterID != ID_SONIC || self->superState != SUPERSTATE_SUPER) && !self->isChibi) {
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_BALANCE2, &self->animator, false, 0);
                            }
                            else {
                                self->direction = FLIP_NONE;
                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_BALANCE1, &self->animator, false, 0);
                            }
                            break;

                        // bit 5 & 6 are never set, this code cannot be reached
                        // in theory, bit 5 & 6 would be set using sensorLC & sensorLR equivalents from v4/S2/S3
                        case 0b01101111:
                        case 0b01110100: RSDK.SetSpriteAnimation(self->aniFrames, ANI_BALANCE1, &self->animator, false, 0); break;

                        // Not balancing
                        case 0b00000000:
                        default:
                            switch (self->characterID) {
                                case ID_SONIC:
#if !RETRO_USE_PLUS
                                    // pre-1.05 super sonic didn't have a "bored" anim
                                    if (self->superState != SUPERSTATE_SUPER) {
#endif
                                        if (self->timer != 720 || self->isChibi || self->superState == SUPERSTATE_SUPER) {
                                            if (self->timer < 240) {
                                                self->timer++;
                                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, false, 0);
                                            }
                                            else {
                                                self->timer++;
                                                if (self->animator.animationID == ANI_BORED1) {
                                                    if (self->animator.frameID == 41)
                                                        self->timer = 0;
                                                }
                                                else
                                                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_BORED1, &self->animator, false, 0);
                                            }
                                        }
                                        else {
                                            if (self->animator.animationID == ANI_BORED2) {
                                                if (self->animator.frameID == 67)
                                                    self->timer = 0;
                                            }
                                            else
                                                RSDK.SetSpriteAnimation(self->aniFrames, ANI_BORED2, &self->animator, false, 0);
                                        }
#if !RETRO_USE_PLUS
                                    }
                                    else {
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, false, 0);
                                    }
#endif
                                    break;

                                case ID_TAILS:
                                    if (self->timer < 240) {
                                        self->timer++;
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, false, 0);
                                    }
                                    else if (self->animator.animationID == ANI_BORED1) {
                                        if (self->animator.frameID == 45)
                                            self->timer = 0;
                                    }
                                    else {
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_BORED1, &self->animator, false, 0);
                                    }
                                    break;

                                case ID_KNUCKLES:
                                    if (self->timer < 240) {
                                        self->timer++;
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, false, 0);
                                    }
                                    else if (self->animator.animationID == ANI_BORED1) {
                                        if (self->animator.frameID == 69)
                                            self->timer = 0;
                                    }
                                    else {
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_BORED1, &self->animator, false, 0);
                                    }
                                    break;

#if RETRO_USE_PLUS
                                case ID_MIGHTY:
                                    if (self->timer < 240) {
                                        self->timer++;
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, false, 0);
                                    }
                                    else if (self->animator.animationID == ANI_BORED1) {
                                        if (self->animator.frameID == 35)
                                            self->timer = 0;
                                    }
                                    else {
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_BORED1, &self->animator, false, 0);
                                    }
                                    break;

                                case ID_RAY:
                                    if (self->timer < 240) {
                                        self->timer++;
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, false, 0);
                                    }
                                    else if (self->animator.animationID == ANI_BORED1) {
                                        if (self->animator.frameID == 55)
                                            self->timer = 0;
                                    }
                                    else {
                                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_BORED1, &self->animator, false, 0);
                                    }
                                    break;
#endif
                                default: break;
                            }
                            break;
                    }

                    if (++self->outtaHereTimer >= 72000000) {
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_OUTTAHERE, &self->animator, false, 0);
                        self->state           = Player_State_OuttaHere;
                        self->tileCollisions  = false;
                        self->interaction     = false;
                        self->nextAirState    = StateMachine_None;
                        self->nextGroundState = StateMachine_None;
                        self->velocity.x      = 0;
                        self->velocity.y      = 0;
                        RSDK.PlaySfx(Player->sfxOuttahere, false, 255);
                    }
                }
            }
            else {
                self->pushing = clampVal(self->pushing, -3, 3);
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_PUSH, &self->animator, false, 0);
            }
        }
        else {
            if (self->animator.animationID != ANI_SKID) {
                if (self->animator.animationID == ANI_SKIDTURN) {
                    if (self->animator.frameID == self->animator.frameCount - 1) {
                        self->direction ^= FLIP_X;
                        self->skidding = 1;
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 0);
                    }
                }
                else {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_SKID, &self->animator, false, 0);
                    if (abs(self->groundVel) >= 0x60000) {
                        if (abs(self->groundVel) >= 0xA0000)
                            self->animator.speed = 64;
                        else
                            self->animator.speed = 144;
                    }
                    else {
                        self->skidding -= 8;
                    }

                    RSDK.PlaySfx(Player->sfxSkidding, false, 255);
                    EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
                    dust->state      = Dust_State_Skid;
                }
            }
            else {
                int32 spd = self->animator.speed;
                if (self->direction) {
                    if (self->groundVel >= 0) {
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SKIDTURN, &self->animator, false, 0);
                    }
                }
                else if (self->groundVel <= 0) {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_SKIDTURN, &self->animator, false, 0);
                }

                self->animator.speed = spd;
            }

            --self->skidding;
        }

        if (self->jumpPress) {
            Player_StartJump(self);
            self->timer = 0;
        }
        else {
            if (self->groundVel) {
                int32 minRollVel = self->state == Player_State_Crouch ? 0x11000 : 0x8800;
                if (abs(self->groundVel) >= minRollVel && !self->left && !self->right && self->down) {
                    Player_StartRoll();
                    RSDK.PlaySfx(Player->sfxRoll, false, 255);
                }
            }
            else if (((self->angle < 0x20 || self->angle > 0xE0) && !self->collisionMode) || (self->invertGravity && self->angle == 0x80)) {
                if (self->up) {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_LOOKUP, &self->animator, true, 1);
                    self->timer = 0;
                    self->state = Player_State_LookUp;
                }
                else if (self->down) {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_CROUCH, &self->animator, true, 1);
                    self->timer = 0;
                    self->state = Player_State_Crouch;
                }
            }
        }
    }
}
void Player_State_Air(void)
{
    RSDK_THIS(Player);

    self->tileCollisions = true;
    Player_HandleAirFriction();

    if (self->onGround) {
        self->state = Player_State_Ground;
        if (self->camera)
            self->camera->disableYOffset = false;
        self->jumpAbilityState = 0;
    }
    else {
        Player_HandleAirMovement();
        self->nextGroundState = Player_State_Ground;
        if (self->velocity.y > 0) {
            if (self->animator.animationID >= ANI_SPRINGTWIRL) {
                if (self->animator.animationID <= ANI_SPRINGDIAGONAL) {
                    RSDK.SetSpriteAnimation(self->aniFrames, self->animationReserve, &self->animator, false, 0);
                }
                else if (self->animator.animationID == ANI_SPRINGCS && !self->animator.frameID) {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_AIRWALK, &self->animator, false, 0);
                }
            }
        }

        switch (self->animator.animationID) {
            case ANI_IDLE:
            case ANI_WALK:
                if (self->animator.speed < 64)
                    self->animator.speed = 64;
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_AIRWALK, &self->animator, false, self->animator.frameID);
                break;

            case ANI_LOOKUP:
            case ANI_CROUCH:
            case ANI_SKIDTURN: RSDK.SetSpriteAnimation(self->aniFrames, ANI_AIRWALK, &self->animator, false, 0); break;

            case ANI_JOG: RSDK.SetSpriteAnimation(self->aniFrames, ANI_AIRWALK, &self->animator, false, 0); break;

            case ANI_JUMP:
                if (self->velocity.y >= self->jumpCap)
                    StateMachine_Run(self->stateAbility);
                break;

            case ANI_SKID:
                if (self->skidding <= 0)
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_AIRWALK, &self->animator, false, self->animator.frameID);
                else
                    self->skidding--;
                break;

            case ANI_SPINDASH: RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0); break;

            default: break;
        }
    }
}
void Player_State_Roll(void)
{
    RSDK_THIS(Player);

    Player_HandleGroundRotation();
    Player_HandleRollDeceleration();

    self->applyJumpCap = false;
    if (!self->onGround) {
        self->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        if (self->characterID == ID_TAILS)
            self->animator.speed = 120;
        else
            self->animator.speed = ((abs(self->groundVel) * 0xF0) / 0x60000) + 0x30;

        if (self->animator.speed > 0xF0)
            self->animator.speed = 0xF0;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;

        if (self->jumpPress)
            Player_StartJump(self);
    }
}
void Player_State_ForceRoll_Ground(void)
{
    RSDK_THIS(Player);

    Player_HandleGroundRotation();

    bool32 left  = self->left;
    bool32 right = self->right;
    if (self->controlLock > 0) {
        self->left  = false;
        self->right = false;
        self->controlLock--;
    }

    Player_HandleRollDeceleration();

    self->left         = left;
    self->right        = right;
    self->applyJumpCap = false;

    if (!self->onGround) {
        self->state = Player_State_ForceRoll_Air;
        Player_HandleAirMovement();
    }
    else {
        if (self->characterID == ID_TAILS)
            self->animator.speed = 120;
        else
            self->animator.speed = ((abs(self->groundVel) * 0xF0) / 0x60000) + 0x30;

        if (self->animator.speed > 0xF0)
            self->animator.speed = 0xF0;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
    }
}
void Player_State_ForceRoll_Air(void)
{
    RSDK_THIS(Player);

    Player_HandleGroundRotation();

    bool32 left  = self->left;
    bool32 right = self->right;
    if (self->controlLock > 0) {
        self->left  = false;
        self->right = false;
        self->controlLock--;
    }

    Player_HandleAirFriction();

    self->left            = left;
    self->right           = right;
    self->applyJumpCap    = false;

    self->nextGroundState = Player_State_ForceRoll_Ground;

    if (!self->onGround) {
        Player_HandleAirMovement();
    }
    else {
        self->state = Player_State_ForceRoll_Ground;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
    }
}
void Player_State_LookUp(void)
{
    RSDK_THIS(Player);

    if (self->invertGravity)
        self->rotation = 0x80;

    self->left  = false;
    self->right = false;

    Player_HandleGroundMovement();

    if (self->camera)
        self->camera->disableYOffset = false;

    self->jumpAbilityState = 0;
    self->nextAirState     = Player_State_Air;

    if (self->up) {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_LOOKUP, &self->animator, false, 1);
        if (self->animator.frameID == 5)
            self->animator.speed = 0;

        if (self->timer < 60) {
            self->timer++;
        }
        else if (self->camera) {
            if (self->invertGravity) {
                if (self->camera->lookPos.y < 96)
                    self->camera->lookPos.y += 2;
            }
            else {
                if (self->camera->lookPos.y > -96)
                    self->camera->lookPos.y -= 2;
            }
        }

        if (self->jumpPress) {
            if (self->statePeelout) {
                StateMachine_Run(self->statePeelout);
            }
            else {
                Player_StartJump(self);
            }
        }
    }
    else {
        self->animator.speed = 64;

        if (self->animator.frameID == 0 || self->left || self->right)
            self->state = Player_State_Ground;

        if (self->jumpPress)
            Player_StartJump(self);
    }
}
void Player_State_Crouch(void)
{
    RSDK_THIS(Player);

    if (self->invertGravity)
        self->rotation = 0x80;

    self->left  = false;
    self->right = false;

    Player_HandleGroundMovement();

    if (self->camera)
        self->camera->disableYOffset = false;

    self->jumpAbilityState = 0;
    self->nextAirState     = Player_State_Air;

    if (self->down) {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_CROUCH, &self->animator, false, 1);
        if (self->animator.frameID == 4)
            self->animator.speed = 0;

        if (self->timer < 60) {
            self->timer++;
        }
        else if (self->camera) {
            if (self->invertGravity) {
                if (self->camera->lookPos.y > -96)
                    self->camera->lookPos.y -= 2;
            }
            else {
                if (self->camera->lookPos.y < 96)
                    self->camera->lookPos.y += 2;
            }
        }

        if (self->jumpPress) {
            EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Dust->aniFrames, 1, &dust->animator, true, 0);
            dust->state     = Dust_State_Spindash;
            dust->drawOrder = self->drawOrder;

            RSDK.SetSpriteAnimation(self->aniFrames, ANI_SPINDASH, &self->animator, true, 0);
            self->state          = Player_State_Spindash;
            self->abilityTimer   = 0;
            self->spindashCharge = 0;
            RSDK.PlaySfx(Player->sfxCharge, false, 255);
        }
    }
    else {
        self->animator.speed = 128;

        if (self->animator.frameID == 0 || self->left || self->right)
            self->state = Player_State_Ground;

        if (self->jumpPress)
            Player_StartJump(self);
    }
}
void Player_State_Spindash(void)
{
    RSDK_THIS(Player);

    float chargeSpeeds[13] = { 1.0,       1.0614005, 1.125531,  1.1926451, 1.2630343, 1.3370349, 1.4150375,
                               1.4974997, 1.5849625, 1.6780719, 1.7776076, 1.8845228, 2.00000000 };

    if (self->jumpPress) {
        self->abilityTimer += 0x20000;

        if (self->abilityTimer > 0x90000)
            self->abilityTimer = 0x90000;

        if (self->spindashCharge < 12)
            self->spindashCharge++;

        if (self->spindashCharge < 0)
            self->spindashCharge = 0;

        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SPINDASH, &self->animator, true, 0);
        int32 channel = RSDK.PlaySfx(Player->sfxCharge, false, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[self->spindashCharge]);
    }
    else {
        self->abilityTimer -= self->abilityTimer >> 5;
    }

    if (!self->down) {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);

        if (self->camera && !Zone->autoScrollSpeed) {
            self->scrollDelay   = 15;
            self->camera->state = Camera_State_VLock;
        }

        int32 vel = 0;
        if (self->superState == SUPERSTATE_SUPER)
            vel = (((uint32)self->abilityTimer >> 1) & 0x7FFF8000) + 0xB0000;
        else
            vel = (((uint32)self->abilityTimer >> 1) & 0x7FFF8000) + 0x80000;

        if (self->direction != FLIP_NONE)
            self->groundVel = -vel;
        else
            self->groundVel = vel;

        RSDK.StopSfx(Player->sfxCharge);
        RSDK.PlaySfx(Player->sfxRelease, false, 0xFF);

        RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);

        if (!self->collisionMode)
            self->position.y += self->jumpOffset;

        self->pushing = 0;
        self->state   = Player_State_Roll;
    }
}
void Player_State_Peelout(void)
{
    RSDK_THIS(Player);

    if (self->gravityStrength == 0x1000) {
        if (self->superState == SUPERSTATE_SUPER) {
            self->abilityTimer += 0xA000;
            if (self->abilityTimer > 0x70000)
                self->abilityTimer = 0x70000;
        }
        else {
            self->abilityTimer += 0x6000;
            if (self->abilityTimer > 0x60000)
                self->abilityTimer = 0x60000;
        }
    }
    else if (self->superState == SUPERSTATE_SUPER) {
        self->abilityTimer += 0xC000;
        if (self->abilityTimer > 0xD0000)
            self->abilityTimer = 0xD0000;
    }
    else {
        self->abilityTimer += 0x8000;
        if (self->abilityTimer > 0xC0000)
            self->abilityTimer = 0xC0000;
    }

    if (self->abilityTimer >= self->maxWalkSpeed) {
        if (self->abilityTimer >= self->maxJogSpeed) {
            if (self->abilityTimer < self->maxRunSpeed) {
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_JOG, &self->animator, false, self->animator.animationID - 8 > 1);
                self->animator.speed = (self->abilityTimer >> 12) + 96;
                if (self->animator.speed > 0x200)
                    self->animator.speed = 512;
                self->maxRunSpeed = 0xC0000;
                self->maxJogSpeed = 0x58000;
            }
            else {
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_DASH, &self->animator, false, self->animator.animationID - 8 > 1);
                self->maxRunSpeed = 0xB8000;
            }
        }
        else {
            if (self->animator.animationID != ANI_WALK || self->animator.frameID == 3)
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_RUN, &self->animator, false, 0);
            self->animator.speed = (self->abilityTimer >> 12) + 64;
            self->maxWalkSpeed   = 0x38000;
            self->maxJogSpeed    = 0x60000;
        }
    }
    else {
        if (self->animator.animationID == ANI_JOG) {
            if (self->animator.frameID == ANI_DASH)
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 9);
        }
        else if (self->animator.animationID == ANI_AIRWALK) {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, self->animator.frameID);
        }
        else {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 0);
        }

        self->animator.speed = (self->abilityTimer >> 12) + 48;
        self->maxWalkSpeed   = 0x40000;
    }

    if (!self->up) {
        RSDK.StopSfx(Player->sfxPeelCharge);

        if (self->abilityTimer >= 0x60000) {
            if (self->camera && !Zone->autoScrollSpeed) {
                self->scrollDelay   = 15;
                self->camera->state = Camera_State_VLock;
            }

            if (self->direction)
                self->groundVel = -self->abilityTimer;
            else
                self->groundVel = self->abilityTimer;

            RSDK.PlaySfx(Player->sfxPeelRelease, false, 0xFF);
        }

        self->state = Player_State_Ground;
    }
}
void Player_State_OuttaHere(void)
{
    RSDK_THIS(Player);

    if (self->onGround) {
        if (self->animator.frameID == 14) {
            self->applyJumpCap = false;

            if (self->direction == FLIP_X)
                self->velocity.x = -0x10000;
            else
                self->velocity.x = 0x10000;
            self->velocity.y = -0x58000;

            self->onGround   = false;
            self->stateInput = StateMachine_None;

            if (self->camera)
                self->camera->state = StateMachine_None;
        }
    }
    else {
        Player_HandleAirMovement();
        if (self->velocity.y > 0x100000) {
            RSDK.ResetEntitySlot(SLOT_GAMEOVER, GameOver->classID, intToVoid(false));
            destroyEntity(self);
        }
    }
}
void Player_State_Transform(void)
{
    RSDK_THIS(Player);

    self->position.x += Zone->autoScrollSpeed;

    self->invincibleTimer = 60;

    RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x140000, true);

    if (++self->timer != 36) {
        if (!self->isChibi) {
            if (self->animator.frameID == self->animator.frameCount - 1) {
                self->isTransforming = false;
                self->interaction    = true;
                self->state          = Player_State_Air;
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 3);

#if RETRO_USE_PLUS
                if (!ERZStart && globals->superMusicEnabled)
#else
                if (!ERZStart)
#endif
                    Music_PlayQueuedTrack(TRACK_SUPER);
            }
        }
    }
    else {
        EntityImageTrail *trail = RSDK_GET_ENTITY(2 * Player->playerCount + self->playerID, ImageTrail);
        RSDK.ResetEntityPtr(trail, ImageTrail->classID, self);

        if (ERZStart)
            self->shield = SHIELD_LIGHTNING;
        RSDK.PlaySfx(Player->sfxTransform2, false, 255);

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + self->playerID, Shield);
        RSDK.ResetEntityPtr(shield, SuperSparkle->classID, self);
        self->superState = SUPERSTATE_SUPER;
        Player_UpdatePhysicsState(self);

        if (!self->isChibi) {
            if (self->animator.frameID == self->animator.frameCount - 1) {
                self->isTransforming = false;
                self->interaction    = true;
                self->state          = Player_State_Air;

#if RETRO_USE_PLUS
                if (!ERZStart && globals->superMusicEnabled)
#else
                if (!ERZStart)
#endif
                    Music_PlayQueuedTrack(TRACK_SUPER);
            }
        }
        else {
            self->isTransforming = false;
            self->interaction    = true;
            self->state          = Player_State_Air;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 3);
            Music_PlayQueuedTrack(TRACK_SUPER);
        }
    }
}
void Player_State_Hit(void)
{
    RSDK_THIS(Player);

    if (self->onGround) {
        self->state = Player_State_Ground;

        if (self->velocity.x >= -0x20000) {
            if (self->velocity.x <= 0x20000)
                self->groundVel = 0;
            else
                self->groundVel -= 0x20000;
        }
        else {
            self->groundVel += 0x20000;
        }

        self->pushing     = false;
        self->controlLock = false;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
        self->skidding         = false;
    }
    else {
        if (self->camera) {
            self->camera->disableYOffset = true;
            self->camera->offset.y    = 0x200000;
        }

        self->skidding = 0;

        if (!self->underwater)
            self->velocity.y += 0x3000;
        else
            self->velocity.y += 0xF00;
    }
}
void Player_State_Die(void)
{
    RSDK_THIS(Player);

    if (self->superState == SUPERSTATE_SUPER)
        self->superState = SUPERSTATE_FADEOUT;

    if (self->blinkTimer) {
        self->blinkTimer = 0;
        self->visible    = true;
    }

    self->velocity.x = 0;
    self->velocity.y += self->gravityStrength;

    RSDK.SetSpriteAnimation(self->aniFrames, ANI_DIE, &self->animator, false, 0);

    if (!self->sidekick) {
        if (self->camera) {
            if (self->position.y > self->camera->position.y - self->camera->targetMoveVel.y + 0x1800000)
                self->position.y = self->camera->position.y - self->camera->targetMoveVel.y + 0x1800000;

            self->scrollDelay = 2;
        }
    }

    if (self->velocity.y > 0x100000)
        Player_HandleDeath(self);
}
void Player_State_Drown(void)
{
    RSDK_THIS(Player);

    if (self->superState == SUPERSTATE_SUPER)
        self->superState = SUPERSTATE_FADEOUT;

    if (self->blinkTimer) {
        self->blinkTimer = 0;
        self->visible    = true;
    }

    self->velocity.x = 0;
    self->velocity.y += self->gravityStrength;

    RSDK.SetSpriteAnimation(self->aniFrames, ANI_DROWN, &self->animator, false, 0);

    if (!self->sidekick) {
        if (self->camera) {
            if (self->position.y > self->camera->position.y + 0x1000000)
                self->position.y = self->camera->position.y + 0x1000000;
            self->scrollDelay = 2;
        }
    }

    if (self->velocity.y > 0x100000)
        Player_HandleDeath(self);
}
void Player_State_DropDash(void)
{
    RSDK_THIS(Player);

    if (self->onGround) {
        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;

        if (self->right)
            self->direction = FLIP_NONE;
        if (self->left)
            self->direction = FLIP_X;

        int32 vel = 0x80000;
        int32 cap = 0xC0000;
        if (self->superState == SUPERSTATE_SUPER) {
            vel = 0xC0000;
            cap = 0xD0000;

            if (self->camera)
                Camera_ShakeScreen(self->camera->screenID, 6, 6);
        }

        if (self->direction) {
            if (self->velocity.x <= 0) {
                self->groundVel = (self->groundVel >> 2) - vel;

                if (self->groundVel < -cap)
                    self->groundVel = -cap;
            }
            else if (self->angle) {
                self->groundVel = (self->groundVel >> 1) - vel;
            }
            else {
                self->groundVel = -vel;
            }
        }
        else {
            if (self->velocity.x >= 0) {
                self->groundVel = vel + (self->groundVel >> 2);

                if (self->groundVel > cap)
                    self->groundVel = cap;
            }
            else if (self->angle) {
                self->groundVel = vel + (self->groundVel >> 1);
            }
            else {
                self->groundVel = vel;
            }
        }

        if (self->camera && !Zone->autoScrollSpeed) {
            self->scrollDelay   = 8;
            self->camera->state = Camera_State_VLock;
        }

        EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Dust->aniFrames, 2, &dust->animator, true, 0);
        dust->state = Dust_State_Move;
        dust->position.y += RSDK.GetHitbox(&self->animator, 0)->bottom << 16;
        dust->direction = self->direction;
        dust->drawOrder = self->drawOrder;

        RSDK.StopSfx(Player->sfxDropdash);
        if (self->superState == SUPERSTATE_SUPER)
            RSDK.PlaySfx(Player->sfxPeelRelease, false, 255);
        else
            RSDK.PlaySfx(Player->sfxRelease, false, 255);

        RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
        if (!self->collisionMode)
            self->position.y += self->jumpOffset;
        self->pushing = 0;
        self->state   = Player_State_Roll;
    }
    else if (self->jumpHold) {
        Player_HandleAirFriction();
        Player_HandleAirMovement();

        self->animator.speed += 8;
        if (self->animator.speed > 0x100)
            self->animator.speed = 0x100;
    }
    else {
        self->jumpAbilityState = 0;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
        self->state = Player_State_Air;
    }
}
void Player_State_BubbleBounce(void)
{
    RSDK_THIS(Player);

    if (self->shield == SHIELD_BUBBLE && !self->invincibleTimer && self->superState != SUPERSTATE_SUPER) {
        if (!self->onGround) {
            Player_HandleAirFriction();
            Player_HandleAirMovement();
        }
        else {
            self->controlLock = 0;

            int32 vel = 0;
            if (self->underwater != 1) // only apply lower velocity if in palette water, rect water uses regular vel
                vel = self->gravityStrength + 0x78000;
            else
                vel = self->gravityStrength + 0x40000;
            self->onGround = false;

            self->velocity.x = (self->groundVel * RSDK.Cos256(self->angle) + vel * RSDK.Sin256(self->angle)) >> 8;
            self->velocity.y = (self->groundVel * RSDK.Sin256(self->angle) - vel * RSDK.Cos256(self->angle)) >> 8;

            if (self->camera) {
                self->camera->disableYOffset = true;
                self->camera->offset.y    = 0x200000;
            }

            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
            self->animator.speed = (abs(self->groundVel) >> 12) + 0x30;

            EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(self), Shield);
            RSDK.SetSpriteAnimation(Shield->aniFrames, 9, &shield->fxAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &shield->shieldAnimator, true, 0);
            shield->state = Shield_State_BubbleBounced;
            if (self->animator.speed > 0xF0)
                self->animator.speed = 0xF0;

            self->angle            = 0;
            self->collisionMode    = CMODE_FLOOR;
            self->applyJumpCap     = false;
            self->jumpAbilityState = 1;
            RSDK.PlaySfx(Shield->sfxBubbleBounce, false, 255);

            self->state = Player_State_Air;
        }
    }
    else {
        self->state = Player_State_Air;
    }
}
void Player_State_TailsFlight(void)
{
    RSDK_THIS(Player);

    Player_HandleAirFriction();

    if (self->onGround) {
        self->state = Player_State_Ground;
        Player_HandleAirMovement();
    }
    else {
        if (self->velocity.y < -0x10000)
            self->abilitySpeed = 0x800;

        if (self->velocity.y <= 0) {
            if (self->abilityValue >= 60)
                self->abilitySpeed = 0x800;
            else
                self->abilityValue++;
        }

        self->velocity.y += self->abilitySpeed;

        uint8 slot = 0;
        if (!self->sidekick)
            slot = SceneInfo->entitySlot;

        if (self->position.y < Zone->playerBoundsT[slot] + 0x100000 && self->velocity.y < 0)
            self->velocity.y = 0;

        EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (globals->gameMode != MODE_COMPETITION && !self->isChibi && !leader->isChibi)
            Player_CheckStartFlyCarry(leader);

        if (self->timer >= 480) {
            if (!self->underwater) {
                if (leader->state == Player_State_FlyCarried)
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYLIFTTIRED, &self->animator, false, 0);
                else
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
            }
            else {
                if (leader->state == Player_State_FlyCarried)
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIMLIFT, &self->animator, false, 0);
                else
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIMTIRED, &self->animator, false, 0);
            }
        }
        else {
            if (self->underwater) {
                if (leader->state == Player_State_FlyCarried) {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIMLIFT, &self->animator, false, 0);
                }
                else {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIM, &self->animator, false, 0);
                    self->animator.speed = 128;
                    if (self->velocity.y >= 0)
                        self->animator.speed = 64;
                }
            }
            else {
                if (leader->state != Player_State_FlyCarried || self->velocity.y >= 0)
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 0);
                else
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYLIFT, &self->animator, false, 0);

                if (self->velocity.y >= 0)
                    self->animator.speed = 128;
                else
                    self->animator.speed = 256;
            }

            if (++self->timer == 480) {
                if (!self->underwater) {
                    if (leader->state == Player_State_FlyCarried)
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYLIFTTIRED, &self->animator, false, 0);
                    else
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
                }
                else {
                    if (leader->state == Player_State_FlyCarried)
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIMLIFT, &self->animator, false, 0);
                    else
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIMTIRED, &self->animator, false, 0);
                }
            }
            else if (self->jumpPress && (!self->underwater || leader->state != Player_State_FlyCarried)) {
                self->abilitySpeed = -0x2000;
                self->abilityValue = 0;
            }
        }
    }
}
void Player_State_FlyCarried(void)
{
    RSDK_THIS(Player);

    EntityPlayer *sidekick = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    if (sidekick->state != Player_State_TailsFlight)
        self->state = Player_State_Air;

    if (self->flyCarrySidekickPos.x != self->flyCarryLeaderPos.x)
        self->state = Player_State_Air;

    if (self->onGround && self->velocity.y >= 0)
        self->state = Player_State_Ground;

    if (self->state == Player_State_FlyCarried) {
        if (self->jumpPress && self->down) {
            self->state = Player_State_Air;

            if (!self->underwater)
                self->velocity.y = -0x40000;
            else
                self->velocity.y = -0x20000;

            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
        }
    }
    else {
        sidekick->flyCarryTimer = 30;
    }
}
void Player_State_KnuxGlideLeft(void)
{
    RSDK_THIS(Player);

    if (!self->onGround) {
        if (!self->jumpHold) {
            self->timer        = 0;
            self->abilitySpeed = 0;
            self->velocity.x >>= 2;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
            self->state = Player_State_KnuxGlideDrop;
        }
        else {
            if (self->timer == 256) {
                if (self->abilitySpeed < 0x180000)
                    self->abilitySpeed += 0x400;
            }
            else if (self->abilitySpeed < 0x40000) {
                self->abilitySpeed += 0x1000;
            }

            if (self->velocity.y <= 0x8000)
                self->velocity.y += 0x2000;
            else
                self->velocity.y -= 0x2000;

            if (self->timer < 256)
                self->timer += 4;
            int32 storeX = self->position.x;

            int32 highY = 0, lowY = 0;
            if (self->isChibi) {
                highY = -0x10000;
                lowY  = 0x30000;
            }
            else {
                highY = -0x20000;
                lowY  = 0xB0000;
            }

            bool32 collidedHigh = false, collidedLow = false;
            int32 highPos = 0, lowPos = 0;
            if (self->timer >= 128) {
                self->position.x += self->velocity.x;
                int32 startPos = self->position.x;

                collidedHigh = RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, -0xC0000, highY, true);
                highPos      = self->position.x;

                self->position.x = startPos;
                collidedLow      = RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, -0xC0000, lowY, true);
                lowPos           = self->position.x;
            }

            int32 prevVelX   = self->velocity.x;
            bool32 isMoving  = self->velocity.x;
            self->velocity.x = self->abilitySpeed * RSDK.Cos512(self->timer) >> 9;
            if (!prevVelX && self->velocity.x) {
                isMoving           = false;
                self->abilitySpeed = 0x40000;
                self->velocity.x   = self->abilitySpeed * RSDK.Cos512(self->timer) >> 9;
            }

            self->position.x = storeX;
            if (self->right) {
                self->state            = Player_State_KnuxGlideRight;
                self->direction        = FLIP_NONE;
                self->animator.frameID = 0;
                self->animator.timer   = 0;
            }
            else if (!isMoving && self->timer == 256) {
                if (collidedHigh || collidedLow) {
                    self->abilitySpeed = 0;
                    self->timer        = 0;
                    if (highPos == lowPos) {
                        self->state      = Player_State_KnuxWallClimb;
                        self->velocity.x = 0;
                        self->velocity.y = 0;
                        RSDK.PlaySfx(Player->sfxGrab, false, 255);
                    }
                    else {
                        self->velocity.x >>= 2;
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
                        self->state = Player_State_KnuxGlideDrop;
                    }
                }
                else if (collidedHigh) {
                    self->timer        = 0;
                    self->abilitySpeed = 0;
                    self->velocity.x >>= 2;
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
                    self->state = Player_State_KnuxGlideDrop;
                }
            }
        }
    }
    else if (self->collisionMode) {
        self->state = Player_State_Ground;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
        self->skidding         = 0;
    }
    else {
        self->timer = 0;
        self->state = Player_State_GlideSlide;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYLIFTTIRED, &self->animator, false, 0);
        self->groundVel  = self->velocity.x;

        EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
        dust->state      = Dust_State_GlideSlide;
    }

    if (self->position.y < Zone->playerBoundsT[SceneInfo->entitySlot] + 0x100000) {
        self->velocity.x   = 0;
        self->abilitySpeed = 0;
    }
}
void Player_State_KnuxGlideRight(void)
{
    RSDK_THIS(Player);

    if (!self->onGround) {
        if (!self->jumpHold) {
            self->timer        = 0;
            self->abilitySpeed = 0;
            self->velocity.x >>= 2;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
            self->state = Player_State_KnuxGlideDrop;
        }
        else {
            if (!self->timer) {
                if (self->abilitySpeed < 0x180000)
                    self->abilitySpeed += 0x400;
            }
            else if (self->abilitySpeed < 0x40000) {
                self->abilitySpeed += 0x1000;
            }

            if (self->velocity.y <= 0x8000)
                self->velocity.y += 0x2000;
            else
                self->velocity.y -= 0x2000;

            if (self->timer > 0)
                self->timer -= 4;
            int32 storeX = self->position.x;

            int32 highY = 0, lowY = 0;
            if (self->isChibi) {
                highY = -0x10000;
                lowY  = 0x30000;
            }
            else {
                highY = -0x20000;
                lowY  = 0xB0000;
            }

            bool32 collidedHigh = false, collidedLow = false;
            int32 highPos = 0, lowPos = 0;
            if (self->timer < 128) {
                self->position.x += self->velocity.x;
                int32 startPos = self->position.x;

                collidedHigh = RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, 0xC0000, highY, true);
                highPos      = self->position.x;

                self->position.x = startPos;
                collidedLow      = RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, 0xC0000, lowY, true);
                lowPos           = self->position.x;
            }

            int32 prevVelX   = self->velocity.x;
            bool32 isMoving  = self->velocity.x;
            self->velocity.x = self->abilitySpeed * RSDK.Cos512(self->timer) >> 9;
            if (!prevVelX && self->velocity.x) {
                isMoving           = false;
                self->abilitySpeed = 0x40000;
                self->velocity.x   = self->abilitySpeed * RSDK.Cos512(self->timer) >> 9;
            }

            self->position.x = storeX;
            if (self->left) {
                self->state            = Player_State_KnuxGlideLeft;
                self->direction        = FLIP_X;
                self->animator.frameID = 0;
                self->animator.timer   = 0;
            }
            else if (!isMoving && !self->timer) {
                if (collidedHigh || collidedLow) {
                    self->abilitySpeed = 0;
                    self->timer        = 0;
                    if (highPos == lowPos) {
                        self->state      = Player_State_KnuxWallClimb;
                        self->velocity.x = 0;
                        self->velocity.y = 0;
                        RSDK.PlaySfx(Player->sfxGrab, false, 255);
                    }
                    else {
                        self->velocity.x >>= 2;
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
                        self->state = Player_State_KnuxGlideDrop;
                    }
                }
                else if (collidedHigh) {
                    self->timer        = 0;
                    self->abilitySpeed = 0;
                    self->velocity.x >>= 2;
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
                    self->state = Player_State_KnuxGlideDrop;
                }
            }
        }
    }
    else if (self->collisionMode) {
        self->state = Player_State_Ground;
        if (self->camera)
            self->camera->disableYOffset = false;
        self->jumpAbilityState = 0;
        self->skidding         = 0;
    }
    else {
        self->timer = 0;
        self->state = Player_State_GlideSlide;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYLIFTTIRED, &self->animator, false, 0);
        self->groundVel  = self->velocity.x;

        EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
        dust->state      = Dust_State_GlideSlide;
    }

    if (self->position.y < Zone->playerBoundsT[SceneInfo->entitySlot] + 0x100000) {
        self->velocity.x   = 0;
        self->abilitySpeed = 0;
    }
}
void Player_State_KnuxGlideDrop(void)
{
    RSDK_THIS(Player);

    if (self->onGround) {
        if (!self->timer)
            RSDK.PlaySfx(Player->sfxLand, false, 255);

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;

        if (abs(Zone->autoScrollSpeed) > 0x20000) {
            if (Zone->autoScrollSpeed <= 0) {
                self->groundVel  = Zone->autoScrollSpeed + 0x20000;
                self->velocity.x = Zone->autoScrollSpeed + 0x20000;
            }
            else {
                self->groundVel  = Zone->autoScrollSpeed - 0x20000;
                self->velocity.x = Zone->autoScrollSpeed - 0x20000;
            }
            self->timer = 16;
        }
        else {
            self->groundVel  = 0;
            self->velocity.x = 0;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYLIFT, &self->animator, false, 0);
        }

        if (self->timer >= 16) {
            self->state    = Player_State_Ground;
            self->skidding = 0;
            self->timer    = 0;
        }
        else {
            self->timer++;
        }
    }
    else {
        Player_HandleAirFriction();
        Player_HandleAirMovement();
    }
}
void Player_State_GlideSlide(void)
{
    RSDK_THIS(Player);

    if (!self->onGround) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
        self->state = Player_State_KnuxGlideDrop;
    }
    else {
        if (self->groundVel && !Zone->autoScrollSpeed) {
            if (!self->timer)
                RSDK.PlaySfx(Player->sfxSlide, false, 255);

            self->timer = (self->timer + 1) & 7;
            if (self->animator.frameID == 2) {
                self->animator.speed = 0;
                self->animator.timer = 0;
            }

            bool32 canGetUp = true;
            if (self->groundVel <= 0) {
                self->groundVel += 0x2000;
                if (self->groundVel < 0)
                    canGetUp = false;
            }
            else {
                self->groundVel -= 0x2000;
                if (self->groundVel > 0)
                    canGetUp = false;
            }

            if (canGetUp) {
                self->animator.frameID = 3;
                self->timer            = 0;
                self->groundVel        = 0;
            }

            if (!self->jumpHold) {
                self->groundVel        = 0;
                self->timer            = 0;
                self->animator.frameID = 3;
            }
        }
        else {
            if (self->camera)
                self->camera->disableYOffset = false;

            self->jumpAbilityState = 0;
            self->animator.speed   = 1;

            if (self->timer >= 16) {
                self->state    = Player_State_Ground;
                self->skidding = 0;
            }
            else {
                self->timer++;
            }
        }
    }
}
void Player_State_KnuxWallClimb(void)
{
    RSDK_THIS(Player);

    if (self->onGround) {
        self->state = Player_State_Ground;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
    }
    else {
        if (self->up) {
            if (self->superState == SUPERSTATE_SUPER)
                self->velocity.y = -0x20000;
            else
                self->velocity.y = -0x10000;

            int32 boundary = Zone->playerBoundsT[SceneInfo->entitySlot] + 0x140000;
            if (self->position.y < boundary)
                self->position.y = boundary;
        }
        else if (self->down) {
            if (self->superState == SUPERSTATE_SUPER)
                self->velocity.y = 0x20000;
            else
                self->velocity.y = 0x10000;
        }
        else {
            self->velocity.y = 0;
        }

        if (self->jumpPress) {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
            self->state            = Player_State_Air;
            self->applyJumpCap     = false;
            self->jumpAbilityState = 1;

            RSDK.PlaySfx(Player->sfxJump, false, 255);
            if (self->direction == FLIP_X) {
                self->velocity.x = 0x40000;
                self->groundVel  = 0x40000;
                self->direction  = FLIP_NONE;
            }
            else {
                self->velocity.x = -0x40000;
                self->groundVel  = -0x40000;
                self->direction  = FLIP_X;
            }

            self->velocity.y = -0x38000;
            if (self->underwater) {
                self->velocity.x >>= 1;
                self->groundVel >>= 1;
                self->velocity.y = -0x1C000;
            }
        }
        else {
            Hitbox *hitbox = Player_GetHitbox(self);
            int32 storeX   = self->position.x;

            int32 highY = 0;
            int32 lowY  = 0;

            int32 roofX = 0;
            int32 roofY = 0;
            if (self->isChibi) {
                highY = -0x40000;
                lowY  = 0x40000;
                roofY = -0xC0000;
            }
            else {
                highY = -0xA0000;
                lowY  = 0xB0000;
                roofY = -0x140000;
            }

            bool32 collidedHigh = false, collidedLow = false;
            if (self->direction) {
                collidedHigh = RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, hitbox->left << 16, highY, 8);
                int32 highX  = self->position.x;

                self->position.x = storeX;
                collidedLow      = RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_RWALL, self->collisionPlane, hitbox->left << 16, lowY, 8);
                if (self->velocity.y < 0 && self->position.x < highX)
                    self->velocity.y = 0;
                roofX = -0x40000;
            }
            else {
                collidedHigh = RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, hitbox->right << 16, highY, 8);
                int32 highY  = self->position.x;

                self->position.x = storeX;
                collidedLow      = RSDK.ObjectTileGrip(self, self->collisionLayers, CMODE_LWALL, self->collisionPlane, hitbox->right << 16, lowY, 8);
                if (self->velocity.y < 0 && self->position.x > highY)
                    self->velocity.y = 0;
                roofX = 0x40000;
            }
            self->position.y += self->velocity.y;

            if (RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_ROOF, self->collisionPlane, roofX, roofY, true))
                self->velocity.y = 0;

            if (collidedHigh && collidedLow) {
                if (self->velocity.y >= 0) {
                    if (self->velocity.y <= 0)
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIM, &self->animator, false, 0);
                    else
                        RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIMLIFT, &self->animator, false, 0);
                }
                else {
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIMTIRED, &self->animator, false, 0);
                }
                self->velocity.y = 0;
            }
            else if (collidedHigh) {
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 2);
                self->velocity.y = 0;
                self->state      = Player_State_KnuxGlideDrop;
            }
            else if (collidedLow) {
                self->position.y &= 0xFFF00000;
                if (self->isChibi)
                    self->position.y -= 0x10000;
                else
                    self->position.y += 0xA0000;

                self->velocity.y     = 0;
                self->position.x     = storeX;
                self->state          = Player_State_KnuxLedgePullUp;
                self->timer          = 1;
                self->tileCollisions = false;
                self->velocity.y     = 0;
            }
        }
    }
}
void Player_State_KnuxLedgePullUp(void)
{
    RSDK_THIS(Player);

    self->left  = false;
    self->right = false;
    RSDK.SetSpriteAnimation(self->aniFrames, ANI_DROPDASH, &self->animator, false, 1);

    RSDK.ProcessAnimation(&self->animator);
    if (self->timer != self->animator.frameID) {
        if (self->animator.frameID >= 6) {
            if (self->animator.frameID == 6) {
                self->onGround       = true;
                self->tileCollisions = true;
            }

            if (self->animator.frameID == self->animator.frameCount - 1) {
                self->timer = 0;
                self->state = Player_State_Ground;
            }
        }
        else {
            self->timer = self->animator.frameID;
            if (!self->isChibi) {
                if (self->direction == FLIP_X)
                    self->position.x -= 0x50000;
                else
                    self->position.x += 0x50000;
                self->position.y -= 0x80000;
            }
            else {
                if (self->direction == FLIP_X)
                    self->position.x -= 0x40000;
                else
                    self->position.x += 0x40000;
                self->position.y -= 0x40000;
            }

            if (self->animator.frameID == 6) {
                self->onGround       = true;
                self->tileCollisions = true;
            }

            if (self->animator.frameID == self->animator.frameCount - 1) {
                self->timer = 0;
                self->state = Player_State_Ground;
            }
        }
    }
    else {
        if (self->animator.frameID == 6) {
            self->onGround       = true;
            self->tileCollisions = true;
        }

        if (self->animator.frameID == self->animator.frameCount - 1) {
            self->timer = 0;
            self->state = Player_State_Ground;
        }
    }
}
#if RETRO_USE_PLUS
void Player_State_MightyHammerDrop(void)
{
    RSDK_THIS(Player);

    if (self->onGround) {
        self->controlLock = 0;
        self->onGround    = false;

        int32 dropForce = self->gravityStrength + (self->underwater == 1 ? 0x10000 : 0x20000);
        int32 groundVel = self->groundVel - (self->groundVel >> 2);

        self->velocity.x = (groundVel * RSDK.Cos256(self->angle) + dropForce * RSDK.Sin256(self->angle)) >> 8;
        self->velocity.y = (groundVel * RSDK.Sin256(self->angle) - dropForce * RSDK.Cos256(self->angle)) >> 8;

        if (self->camera) {
            self->camera->disableYOffset = true;
            self->camera->offset.y    = 0x200000;
        }

        RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);

        self->animator.speed = minVal((abs(self->groundVel) >> 12) + 0x30, 0xF0);

        RSDK.StopSfx(Player->sfxMightyDrill);
        RSDK.PlaySfx(Player->sfxMightyLand, false, 0xFF);
        Camera_ShakeScreen(RSDK.GetEntityID(self), 0, 4);

        Hitbox *hitbox = RSDK.GetHitbox(&self->animator, 0);
        Player_SpawnMightyHammerdropDust(0x10000, hitbox);
        Player_SpawnMightyHammerdropDust(-0x10000, hitbox);
        Player_SpawnMightyHammerdropDust(0x18000, hitbox);
        Player_SpawnMightyHammerdropDust(-0x18000, hitbox);
        Player_SpawnMightyHammerdropDust(0x20000, hitbox);
        Player_SpawnMightyHammerdropDust(-0x20000, hitbox);

        self->angle            = 0;
        self->collisionMode    = CMODE_FLOOR;
        self->applyJumpCap     = false;
        self->jumpAbilityState = 3;

        if (self->invincibleTimer > 0) {
            if (self->invincibleTimer < 8)
                self->invincibleTimer = 8;
            self->state = Player_State_Air;
        }
        else {
            self->invincibleTimer = -8;
            self->state           = Player_State_Air;
        }
    }
    else {
        Player_HandleAirFriction();
        Player_HandleAirMovement();

        if (self->velocity.y <= 0x10000) {
            self->state = Player_State_Air;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
        }
    }
}
void Player_State_MightyUnspin(void)
{
    RSDK_THIS(Player);

    Player_HandleAirFriction();

    if (self->onGround) {
        self->state = Player_State_Ground;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
    }
    else {
        ++self->blinkTimer;

        if (self->camera) {
            self->camera->disableYOffset = true;
            self->camera->offset.y    = 0x200000;
        }

        Player_HandleAirMovement();
        self->nextGroundState = Player_State_Ground;
    }
}
void Player_SpawnMightyHammerdropDust(int32 speed, Hitbox *hitbox)
{
    RSDK_THIS(Player);

    EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
    dust->state      = Dust_State_MoveCollide;
    dust->position.y += hitbox->bottom << 16;
    dust->drawOrder       = self->drawOrder;
    dust->collisionPlane  = self->collisionPlane;
    dust->collisionMode   = 0;
    dust->collisionLayers = self->collisionLayers;
    dust->tileCollisions  = 1;
    dust->animator.frameDuration += 4 * (4 - (abs(speed) >> 15));
    dust->velocity.x = dust->groundVel = self->velocity.x * (Zone->autoScrollSpeed != 0) + (speed >> self->isChibi);
    if (self->isChibi) {
        dust->drawFX |= FX_SCALE;
        dust->scale.x = 256;
        dust->scale.y = 256;
    }

    if (!self->angle)
        RSDK.ObjectTileGrip(dust, dust->collisionLayers, CMODE_FLOOR, dust->collisionPlane, 0, 0, 4);

    for (int32 i = 0; i < 0x10; ++i) {
        if (RSDK.ObjectTileGrip(dust, dust->collisionLayers, CMODE_FLOOR, dust->collisionPlane, 0, 0, 8))
            break;

        dust->position.y += 0x80000;
    }
}
bool32 Player_CheckMightyUnspin(EntityPlayer *player, int32 bounceDistance, bool32 checkHammerDrop, int32 *uncurlTimer)
{
    RSDK_THIS_GEN();

    if (player->characterID != ID_MIGHTY)
        return false;

    if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown || player->invincibleTimer
        || player->blinkTimer > 0) {
        return false;
    }

    int32 anim = player->animator.animationID;
    if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
        int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
        if (player->animator.animationID != ANI_FLY) {
            player->velocity.x = bounceDistance * RSDK.Cos256(angle);
            player->groundVel  = player->velocity.x;
        }

        player->velocity.y = bounceDistance * RSDK.Sin256(angle);

        if (checkHammerDrop) {
            if (player->state == Player_State_MightyHammerDrop) {
                player->state = Player_State_Air;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            }

            if (player->animator.animationID == ANI_SPINDASH) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                if (player->state == Player_State_Spindash) {
                    if (player->onGround)
                        player->state = Player_State_Roll;
                    else
                        player->state = Player_State_Air;
                }
            }
        }
        else {
            player->velocity.y = -0x48000;

            if (!(player->direction & FLIP_X))
                player->velocity.x = -0x28000;
            else
                player->velocity.x = 0x28000;

            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y = -0x24000;
            }

            player->blinkTimer = 121;
            player->visible    = true;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLY, &player->animator, false, 0);
            RSDK.PlaySfx(Player->sfxMightyUnspin, false, 255);
            player->onGround         = 0;
            player->applyJumpCap     = false;
            player->jumpAbilityState = 0;
            player->state            = Player_State_MightyUnspin;
        }

        player->applyJumpCap = false;
        if (uncurlTimer && !*uncurlTimer) {
            if (player->animator.animationID != ANI_FLY)
                RSDK.PlaySfx(Player->sfxPimPom, false, 255);
            *uncurlTimer = 8;
        }

        return true;
    }

    if (checkHammerDrop == 2 && anim == ANI_CROUCH) {
        if (!player->uncurlTimer) {
            RSDK.PlaySfx(Player->sfxPimPom, false, 255);
            player->uncurlTimer = 30;
            if (self->position.x > player->position.x)
                player->groundVel = -0x10000;
            else
                player->groundVel = 0x10000;
        }

        return true;
    }

    return false;
}
void Player_State_RayGlide(void)
{
    RSDK_THIS(Player);

    if (self->rotation) {
        if (self->abilityValue < 0x70)
            self->abilityValue += 8;
    }
    else {
        if (self->abilityValue > 0x10)
            self->abilityValue -= 8;
    }

    if (self->abilitySpeed) {
        self->velocity.y += self->abilitySpeed >> (2 - (self->underwater != 0));
        if (self->velocity.y < self->abilitySpeed) {
            self->velocity.y   = self->abilitySpeed;
            self->abilitySpeed = 0;
        }
    }
    else {
        self->velocity.y += self->gravityStrength * RSDK.Cos512(self->abilityValue) >> 9;
    }

    if (self->velocity.y < -0x60000)
        self->velocity.y = -0x60000;

    if (self->rotation == 1) {
        if (self->velocity.y > 0x10000)
            self->velocity.y = self->velocity.y - (self->velocity.y >> 2);
    }

    if (self->velocity.y <= 0) {
        self->abilityValues[0] -= 22 * RSDK.Sin256(80 - self->abilityValue);
        if (self->abilityValues[0] < 0x40000)
            self->abilityValues[0] = 0x40000;
    }
    else if (self->velocity.y > self->abilityValues[0]) {
        self->abilityValues[0] = self->velocity.y - (self->velocity.y >> 6);
    }

    if (self->velocity.x) {
        int32 angle = 0x50 - self->abilityValue;
        if (self->direction) {
            self->velocity.x -= 22 * RSDK.Sin256(angle) >> (uint8)(self->underwater != 0);

            if (self->velocity.x > -0x10000)
                self->velocity.x = -0x10000;

            if (self->velocity.x < -self->abilityValues[0])
                self->velocity.x = -self->abilityValues[0];
        }
        else {
            self->velocity.x += 22 * RSDK.Sin256(angle) >> (uint8)(self->underwater != 0);

            if (self->velocity.x < 0x10000)
                self->velocity.x = 0x10000;

            if (self->velocity.x > self->abilityValues[0]) {
                self->velocity.x = self->abilityValues[0];
            }
        }
    }

    if (self->controlLock) {
        self->controlLock--;
    }
    else if ((!self->right || self->abilityValue != 16) && self->direction == FLIP_X) {
        if (self->left && self->abilityValue == 112 && self->rotation == 1) {
            self->abilitySpeed = 0;
            self->rotation     = 0;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
        }
    }
    else if ((!self->left || self->abilityValue != 16) && self->direction == FLIP_NONE) {
        if (self->right && self->abilityValue == 112 && self->rotation == 1) {
            self->abilitySpeed = 0;
            self->rotation     = 0;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 0);
        }
    }
    else if (!self->rotation) {
        self->rotation = 1;

        if (self->velocity.y > 0x28000 || self->abilityTimer == 256 || (self->underwater && self->velocity.y > 0x18000)) {
            int32 xVel = abs(self->velocity.x);

            self->abilitySpeed = -(self->abilityTimer * ((xVel >> 1) + (xVel >> 2) + (xVel >> 4)) >> 8);
            if (self->underwater)
                self->abilitySpeed = (self->abilitySpeed >> 1) + (self->abilitySpeed >> 3);
            if (self->abilityTimer > 16)
                self->abilityTimer = self->abilityTimer - 32;
            if (self->abilitySpeed < -0x60000)
                self->abilitySpeed = -0x60000;
        }
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 0);
    }

    if (!self->isGhost && !self->underwater) {
        if (self->animator.animationID == ANI_FLYTIRED)
            Player->rayDiveTimer = 256;
        else if (self->animator.animationID == ANI_FLY)
            Player->raySwoopTimer = 256;
    }

    if (self->onGround) {
        if (abs(self->groundVel) < 0x20000)
            self->groundVel <<= 1;

        self->state = Player_State_Ground;

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
        self->skidding         = 0;
        self->abilityValue     = 0;
    }
    else {
        if (!self->jumpHold || self->position.y < Zone->playerBoundsT[self->playerID] + 0x100000) {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
            self->state = Player_State_Air;
        }
        else if (abs(self->velocity.x) < 0x10000) {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
            self->state = Player_State_Air;
        }
    }

    if (RSDK.ObjectTileCollision(self, self->collisionLayers, CMODE_ROOF, self->collisionPlane, 0, -0x100000, false))
        self->abilitySpeed = 0;
}
#endif
void Player_State_FlyIn(void)
{
    RSDK_THIS(Player);

    EntityPlayer *leader = NULL;
    if (self->playerID)
        leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
    else
        leader = (EntityPlayer *)Zone->entityStorage[1];
#endif

    Player->jumpInTimer  = 0;
    self->tileCollisions = false;
    self->interaction    = false;
#if RETRO_GAMEVER != VER_100
    if (SizeLaser) {
        if (leader->isChibi != self->isChibi)
            SizeLaser_SetP2State(self, leader->isChibi);
    }
#endif

    if (self->characterID == ID_TAILS) {
        if (self->underwater)
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIM, &self->animator, false, 0);
        else
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 0);

        self->scale.x = 0x200;
        self->scale.y = 0x200;
        self->drawFX &= ~FX_SCALE;
    }
    else if (self->characterID == ID_KNUCKLES) {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 0);
        self->scale.x = 0x200;
        self->scale.y = 0x200;
        self->drawFX &= ~FX_SCALE;
    }
    else {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
    }

    if (leader->underwater && leader->position.y < Water->waterLevel)
        self->drawOrder = leader->drawOrder;
    else
        self->drawOrder = Zone->playerDrawHigh;

    Entity *parent = self->abilityPtrs[0];
    int32 screenX  = (ScreenInfo->width + ScreenInfo->centerX) << 16;
    int32 screenY  = (ScreenInfo->height + ScreenInfo->centerY) << 16;
    if (parent->position.x < leader->position.x - screenX || parent->position.x > screenX + leader->position.x
        || parent->position.y < leader->position.y - screenY || parent->position.y > leader->position.y + screenY) {
        parent->position   = leader->position;
        parent->position.y = (ScreenInfo->position.y - 128) << 16;
    }

    if (self->camera && self->camera->target != parent) {
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_ENCORE) {
#endif
            self->camera->position.x = parent->position.x;
            self->camera->position.y = parent->position.y;
            Camera_SetTargetEntity(self->camera->screenID, parent);
#if RETRO_USE_PLUS
        }
#endif
    }

    int32 maxDistance = 0;
    if (self->abilityValues[0] || self->characterID == ID_TAILS || self->characterID == ID_KNUCKLES) {
        maxDistance    = Player->targetLeaderPosition.x - parent->position.x;
        int32 distance = maxDistance >> 4;

        if (self->characterID != ID_TAILS && self->characterID != ID_KNUCKLES) {
            distance = ((Player->targetLeaderPosition.x - parent->position.x) >> 5) + ((Player->targetLeaderPosition.x - parent->position.x) >> 4);
        }

        if (parent->position.x >= Player->targetLeaderPosition.x + 0x40000) {
            self->direction = FLIP_X;

            if (distance < -0xC0000)
                distance = -0xC0000;

            if (leader->velocity.x < 0)
                distance += leader->velocity.x;

            distance -= 0x10000;
            if (distance < maxDistance) {
                distance    = maxDistance;
                maxDistance = 0;
            }
        }
        else if (parent->position.x <= Player->targetLeaderPosition.x - 0x40000) {
            self->direction = FLIP_NONE;

            if (distance > 0xC0000)
                distance = 0xC0000;

            if (leader->velocity.x > 0)
                distance += leader->velocity.x;

            distance += 0x10000;
            if (distance > maxDistance) {
                distance    = maxDistance;
                maxDistance = 0;
            }
        }
        parent->position.x += distance;

        int32 yVel = 0;
        if (self->characterID == ID_TAILS || self->characterID == ID_KNUCKLES) {
            yVel = 0x10000;
#if RETRO_USE_PLUS
            if (globals->gameMode == MODE_ENCORE)
                yVel = 0x20000;
#endif
        }
        else {
            yVel = 0x30000;
        }

        if (parent->position.y > Player->targetLeaderPosition.y)
            parent->position.y -= yVel;
        else if (parent->position.y < Player->targetLeaderPosition.y)
            parent->position.y += yVel;

        parent->position.x &= 0xFFFF0000;
        parent->position.y &= 0xFFFF0000;
    }
    else {
        self->drawOrder    = Zone->playerDrawHigh;
        parent->position.x = Player->targetLeaderPosition.x;
        parent->position.y = (ScreenInfo->centerY + 32 + ScreenInfo->position.y) << 16;
        parent->position.y += (ScreenInfo->centerY - 32) * RSDK.Sin512(self->angle) << 8;
        self->drawFX |= FX_SCALE;
        self->scale.x = 0x2000 - 16 * self->angle - 8 * self->angle;
        self->scale.y = 0x2000 - 16 * self->angle - 8 * self->angle;
        if (self->scale.x < 0x200) {
            self->scale.x = 0x200;
            self->scale.y = 0x200;
        }

        self->angle += 3;
        if (self->angle >= 512 || (self->angle >= 368 && parent->position.y >= leader->position.y)) {
            self->abilityValues[0] = 1;
            self->drawFX &= ~FX_SCALE;
            self->scale.x    = 0x200;
            self->scale.y    = 0x200;
            self->position.x = parent->position.x;
        }
    }

    if (leader->classID == Player->classID) {
        if (leader->state != Player_State_Die && leader->state != Player_State_Drown && leader->state != Player_State_ForceRoll_Ground) {
            if (abs(maxDistance) <= 0x40000 && abs(Player->targetLeaderPosition.y - parent->position.y) < 0x20000)
                Player_EndFlyJumpIn(self, leader);
        }
    }
}
void Player_State_JumpIn(void)
{
    RSDK_THIS(Player);

    EntityPlayer *leader = NULL;
    if (self->playerID)
        leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#if RETRO_USE_PLUS
    else
        leader = (EntityPlayer *)Zone->entityStorage[1];
#endif

    RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
    Entity *parent = self->abilityPtrs[0];
    if (Player_CheckValidState(leader)) {
        parent->position.x = leader->position.x;
        parent->position.y = leader->position.y;
        parent->position.y -= 0x180000;
    }

    self->velocity.y += 0x4800;
    self->onGround   = false;
    self->velocity.x = 0;
    self->abilityValues[0] += self->velocity.y;
    self->position.x = parent->position.x;
    self->position.y = parent->position.y + self->abilityValues[0];

    if (self->scale.x > 512) {
        self->scale.x -= 10;
        if (self->scale.x <= 0x200) {
            self->drawFX &= ~FX_SCALE;
            self->scale.x = 0x200;
        }
        self->scale.y = self->scale.x;
    }

    if (self->velocity.y >= 0) {
        if (self->position.y >= parent->position.y) {
            self->drawFX &= ~FX_SCALE;
            self->scale.x = 0x200;
            self->scale.y = 0x200;
            Player_EndFlyJumpIn(self, leader);
        }
    }
}
void Player_State_StartJumpIn(void)
{
    RSDK_THIS(Player);

    if (++self->abilityValues[0] == 2) {
        EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (leader->classID == Player->classID) {
            EntityDust *dust  = CREATE_ENTITY(Dust, intToVoid(1), leader->position.x, leader->position.y);
            dust->visible     = false;
            dust->active      = ACTIVE_NEVER;
            dust->isPermanent = true;
            dust->position.y  = (ScreenInfo->position.y - 128) << 16;
#if RETRO_USE_PLUS
            self->tileCollisions = false;
            self->interaction    = false;
            self->forceJumpIn    = false;
            self->drawOrder      = Zone->playerDrawHigh + 1;
            self->angle          = 128;

            if ((self->characterID != ID_TAILS && self->characterID != ID_KNUCKLES) || globals->gameMode == MODE_ENCORE) {
                self->state = Player_State_JumpIn;
                self->drawFX |= FX_SCALE;
                self->scale.x = 0x400;
                self->scale.y = 0x400;

                self->abilityValues[0] = 0x100000 - leader->position.y + ((ScreenInfo->height + ScreenInfo->position.y) << 16);
                if (self->abilityValues[0] < 0xA80000) {
                    self->velocity.y = self->abilityValues[0] / -12;
                }
                else
                    self->velocity.y = -0xE0000;
            }
            else {
                self->state            = Player_State_FlyIn;
                self->abilityValues[0] = 0;
            }
#else
            self->forceJumpIn = false;
            self->state = Player_State_FlyIn;
#endif
            self->abilityPtrs[0] = dust;
        }
    }
}
void Player_EndFlyJumpIn(EntityPlayer *player, EntityPlayer *leader)
{
    RSDK_THIS(Player);

    Entity *parent         = player->abilityPtrs[0];
    player->abilityPtrs[0] = NULL;

    if (!SizeLaser || player->state == Player_State_FlyIn) {
        player->state = Player_State_Air;
    }
#if RETRO_USE_PLUS
    else if (globals->gameMode != MODE_ENCORE) {
        player->state = Player_State_Air;
    }
#endif
    else if (!player->isChibi && (leader->isChibi || leader->state == SizeLaser_P2JumpInShrink)) {
        player->drawFX |= FX_SCALE;
        player->scale.x          = 0x200;
        player->scale.y          = 0x200;
        player->velocity.x       = 0;
        player->velocity.y       = 0;
        player->groundVel        = 0;
        player->nextAirState     = 0;
        player->nextGroundState  = 0;
        player->interaction      = 0;
        player->abilityPtrs[0]   = SizeLaser_P2JumpInResize;
        player->abilityValues[0] = parent->position.x - player->position.x;
        player->abilityValues[1] = parent->position.y - player->position.y;
        player->state            = SizeLaser_P2JumpInShrink;
    }
    else if (!player->isChibi || (leader->isChibi || leader->state == SizeLaser_P2JumpInGrow)) {
        player->state = Player_State_Air;
    }
    else {
        player->drawFX |= FX_SCALE;
        player->scale.x         = 0x140;
        player->scale.y         = 0x140;
        player->velocity.x      = 0;
        player->velocity.y      = 0;
        player->groundVel       = 0;
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;

        player->tailFrames = -1;
        switch (player->characterID) {
            default:
            case ID_SONIC:
                if (self->superState == SUPERSTATE_SUPER)
                    player->aniFrames = Player->superFrames;
                else
                    player->aniFrames = Player->sonicFrames;
                break;

            case ID_TAILS:
                player->aniFrames  = Player->tailsFrames;
                player->tailFrames = Player->tailsTailsFrames;
                break;

            case ID_KNUCKLES: player->aniFrames = Player->knuxFrames; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: player->aniFrames = Player->mightyFrames; break;
            case ID_RAY: player->aniFrames = Player->rayFrames; break;
#endif
        }

        player->interaction      = false;
        player->abilityPtrs[0]   = SizeLaser_P2JumpInResize;
        player->abilityValues[0] = parent->position.x - player->position.x;
        player->abilityValues[1] = parent->position.y - player->position.y;
        player->state            = SizeLaser_P2JumpInGrow;
    }

    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
    player->onGround       = false;
    player->tileCollisions = true;
    player->interaction    = true;
    player->controlLock    = 0;
    player->angle          = 0;

    if (!player->sidekick) {
        player->stateInput = Player_ProcessP1Input;
    }
    else {
        player->stateInput     = Player_ProcessP2Input_AI;
        EntityPlayer *plr1     = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player->velocity.x     = plr1->velocity.x;
        player->groundVel      = plr1->groundVel;
        Player->upState        = 0;
        Player->downState      = 0;
        Player->leftState      = 0;
        Player->rightState     = 0;
        Player->jumpHoldState  = 0;
        Player->jumpPressState = 0;
    }

    player->collisionPlane = leader->collisionPlane;
    player->drawOrder      = leader->drawOrder;
    player->active         = ACTIVE_NORMAL;
    player->position.x     = parent->position.x;
    player->position.y     = parent->position.y;
    if (player->playerID == 0)
        player->blinkTimer = 120;

    if (player->camera) {
        Camera_SetTargetEntity(player->camera->screenID, (Entity *)player);
        player->camera->state = Camera_State_Follow;
    }
    destroyEntity(parent);

    Player->upState        = false;
    Player->downState      = false;
    Player->leftState      = false;
    Player->rightState     = false;
    Player->jumpPressState = false;
    Player->jumpHoldState  = false;

    if (BoundsMarker)
        BoundsMarker_CheckAllBounds(player, false);
}
void Player_State_EncoreRespawn(void)
{
    RSDK_THIS(Player);

    EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (leader->state != Player_State_Die && leader->state != Player_State_Drown) {
#if RETRO_USE_PLUS
        if (globals->stock) {
            Player_ChangeCharacter(self, globals->stock & 0xFF);
            globals->stock >>= 8;
            EntityDust *dust       = CREATE_ENTITY(Dust, intToVoid(1), leader->position.x, leader->position.y);
            dust->visible          = false;
            dust->active           = ACTIVE_NEVER;
            dust->isPermanent      = true;
            dust->position.y       = (ScreenInfo->position.y - 128) << 16;
            leader->angle          = 128;
            self->state            = Player_State_FlyIn;
            self->abilityPtrs[0]   = dust;
            self->abilityValues[0] = 0;
            self->nextAirState     = StateMachine_None;
            self->nextGroundState  = StateMachine_None;
            self->stateInput       = Player_ProcessP2InputLag;
            self->position.x       = -0x400000;
            self->position.y       = -0x400000;
            self->velocity.x       = 0;
            self->velocity.y       = 0;
            self->groundVel        = 0;
            self->tileCollisions   = false;
            self->interaction      = false;
            self->drawOrder        = Zone->playerDrawHigh;
            self->drownTimer       = 0;
            self->active           = ACTIVE_NORMAL;
        }
        else {
#endif
            leader->state    = StateMachine_None;
            leader->classID = TYPE_BLANK;
#if RETRO_USE_PLUS
        }
#endif
    }
}
void Player_State_Victory(void)
{
    RSDK_THIS(Player);

    self->applyJumpCap = false;
    self->drownTimer   = 0;

    if (!self->onGround) {
        Player_HandleAirMovement();
    }
    else {
        switch (self->characterID) {
            case ID_SONIC:
                if (self->animator.frameID != 3) {
                    self->groundVel = 0;
                }
                else {
                    self->velocity.y = -0x32000;
                    if (self->direction == FLIP_X)
                        self->velocity.x = -0xE000;
                    else
                        self->velocity.x = 0xE000;
                    self->onGround = false;
                }
                break;

            case ID_KNUCKLES:
                if (self->animator.frameID != 2 || self->animator.timer != 3) {
                    self->groundVel = 0;
                }
                else {
                    self->velocity.y = -0x30000;
                    if (self->direction == FLIP_X)
                        self->velocity.x = -0xE000;
                    else
                        self->velocity.x = 0xE000;
                    self->onGround = false;
                }
                break;

#if RETRO_USE_PLUS
            case ID_MIGHTY:
            case ID_RAY:
                self->nextAirState    = StateMachine_None;
                self->nextGroundState = StateMachine_None;
                self->groundVel       = 0;
                self->velocity.x      = 0;
                self->velocity.y      = 0;
                if (self->animator.animationID == ANI_VICTORY) {
                    if (self->animator.timer != 1) {
                        self->velocity.x = 0;
                        self->groundVel  = 0;
                    }
                    else {
                        int32 vel = RSDK.GetFrameID(&self->animator) - '0';
                        if (!self->direction)
                            self->groundVel = -(vel << 16);
                        else
                            self->groundVel = (vel << 16);
                    }
                }
                break;
#endif
            default:
            case ID_TAILS: self->groundVel = 0; break;
        }

        if (self->camera)
            self->camera->disableYOffset = false;

        self->jumpAbilityState = 0;
    }
}
void Player_State_Bubble(void)
{
    // yep, thats all it is
}
void Player_State_WaterSlide(void)
{
    RSDK_THIS(Player);

    if (!self->onGround) {
        self->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        if (self->camera)
            self->camera->disableYOffset = false;
        self->jumpAbilityState = 0;

        if (self->angle) {
            if (self->angle <= 128) {
                if (self->groundVel < 0x80000) {
                    self->groundVel += 0x4000;
                }
            }
            else if (self->groundVel > -0x80000) {
                self->groundVel -= 0x4000;
            }
        }

        int32 vel = RSDK.Sin256(self->angle) << 13 >> 8;
        self->groundVel += vel;
        self->controlLock = 30;
        self->direction   = vel + self->groundVel < 0;
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLUME, &self->animator, false, 0);
#endif
    }

    if (self->jumpPress) {
        Player_StartJump(self);

        if ((self->angle < 0x80 && self->velocity.x > 0) || (self->angle <= 0x80 && self->velocity.x < 0))
            self->velocity.x += (self->jumpStrength + (self->jumpStrength >> 1)) * RSDK.Sin256(self->angle) >> 8;
    }
}
void Player_State_TransportTube(void)
{
    // Nothin
}

void Player_JumpAbility_Sonic(void)
{
    RSDK_THIS(Player);

    bool32 dropdashAllowed = false;

    if (self->jumpAbilityState == 1) {
#if RETRO_USE_PLUS
        if (self->stateInput != Player_ProcessP2Input_AI || (self->up && globals->gameMode != MODE_ENCORE)) {
#else
        if (self->stateInput != Player_ProcessP2Input_AI) {
#endif
            if (self->jumpPress
#if RETRO_GAMEVER == VER_100
                && !Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds)
#endif
            ) {
                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(self), Shield);
                if (self->invincibleTimer) {
                    if (shield->classID != Shield->classID || shield->shieldAnimator.animationID != 10) {
                        if (!(globals->medalMods & getMod(MEDAL_NODROPDASH)))
                            ++self->jumpAbilityState;
                    }
                }
                else {
                    switch (self->shield) {
                        case SHIELD_NONE:
                            if (globals->medalMods & getMod(MEDAL_INSTASHIELD)) {
                                self->invincibleTimer  = -8;
                                self->jumpAbilityState = 0;
                                RSDK.PlaySfx(Shield->sfxInstaShield, false, 255);
                                RSDK.ResetEntityPtr(shield, Shield->classID, self);
                                shield->inkEffect = INK_ADD;
                                shield->alpha     = 0x100;
                                RSDK.SetSpriteAnimation(Shield->aniFrames, 10, &shield->shieldAnimator, true, 0);
                                shield->state = Shield_State_Insta;
                            }
                            // [Fallthrough]
                        case SHIELD_BLUE:
                            // returns 0 if dropdash (bit 4) is disabled
                            // returns 1 if dropdash is enabled and instashield (bit 3) is disabled
                            // returns 2 if dropdash AND instashield are enabled
                            if (!(globals->medalMods & getMod(MEDAL_NODROPDASH)))
                                self->jumpAbilityState = ((~(globals->medalMods & 0xFF) >> 3) & 2);
                            break;

                        case SHIELD_BUBBLE:
                            self->velocity.x >>= 1;
                            self->velocity.y      = 0x80000;
                            self->state           = Player_State_BubbleBounce;
                            self->nextGroundState = StateMachine_None;
                            self->nextAirState    = StateMachine_None;
                            RSDK.SetSpriteAnimation(Shield->aniFrames, 7, &shield->fxAnimator, true, 0);
                            RSDK.SetSpriteAnimation(Shield->aniFrames, 8, &shield->shieldAnimator, true, 0);
                            shield->state = Shield_State_StartBubbleBounce;
                            RSDK.PlaySfx(Shield->sfxBubbleBounce, false, 255);
                            break;

                        case SHIELD_FIRE:
                            self->jumpAbilityState = 0;
                            self->velocity.x       = self->direction == FLIP_X ? -0x80000 : 0x80000;
                            self->velocity.y       = 0;
                            RSDK.SetSpriteAnimation(Shield->aniFrames, 2, &shield->shieldAnimator, true, 0);
                            shield->state     = Shield_State_FireDash;
                            shield->direction = self->direction;
                            if (self->camera && !Zone->autoScrollSpeed) {
                                self->scrollDelay   = 15;
                                self->camera->state = Camera_State_VLock;
                            }
                            RSDK.PlaySfx(Shield->sfxFireDash, false, 255);
                            break;

                        case SHIELD_LIGHTNING:
                            self->jumpAbilityState = 0;
                            self->velocity.y       = self->invertGravity ? 0x58000 : -0x58000;
                            shield->state = Shield_State_LightningJump;
                            RSDK.PlaySfx(Shield->sfxLightningJump, false, 255);
                            break;

                        default: break;
                    }
                }
            }
#if RETRO_GAMEVER != VER_100
            else {
                if (ControllerInfo[self->controllerID].keyY.press)
                    Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds);
            }
#endif
            return;
        }

        dropdashAllowed = true;
    }

    if ((self->jumpAbilityState >= 2 || dropdashAllowed) && self->jumpHold) {
        if (++self->jumpAbilityState > 22) {
            self->state           = Player_State_DropDash;
            self->nextGroundState = StateMachine_None;
            self->nextAirState    = StateMachine_None;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_DROPDASH, &self->animator, false, 0);
            RSDK.PlaySfx(Player->sfxDropdash, false, 255);
        }
    }
}
void Player_JumpAbility_Tails(void)
{
    RSDK_THIS(Player);

    if (self->jumpPress && self->jumpAbilityState == 1
        && (self->stateInput != Player_ProcessP2Input_AI
            || (self->up
#if RETRO_USE_PLUS
                && globals->gameMode != MODE_ENCORE
#endif
                ))
#if RETRO_GAMEVER == VER_100
        && !Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds)
#endif
    ) {
        if (!self->invertGravity) {
            self->jumpAbilityState = 0;
            self->timer            = 0;
            self->abilitySpeed     = 0x800;
            if (!self->underwater)
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 0);
            else
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_SWIM, &self->animator, false, 0);
            self->state           = Player_State_TailsFlight;
            self->nextGroundState = StateMachine_None;
            self->nextAirState    = StateMachine_None;
        }
    }
#if RETRO_GAMEVER != VER_100
    else if (ControllerInfo[self->controllerID].keyY.press)
        Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds);
#endif
}
void Player_JumpAbility_Knux(void)
{
    RSDK_THIS(Player);

    if (self->jumpPress && self->jumpAbilityState == 1
        && (self->stateInput != Player_ProcessP2Input_AI
            || (self->up
#if RETRO_USE_PLUS
                && globals->gameMode != MODE_ENCORE
#endif
                ))
#if RETRO_GAMEVER == VER_100
        && !Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds)
#endif
    ) {
        if (!self->invertGravity) {
            self->jumpAbilityState = 0;
            self->abilitySpeed     = 0x40000;

            if (self->velocity.y < 0)
                self->velocity.y = 0;

            if (self->direction) {
                self->state      = Player_State_KnuxGlideLeft;
                self->velocity.x = -0x40000;
                self->timer      = 0x100;
            }
            else {
                self->state      = Player_State_KnuxGlideRight;
                self->velocity.x = 0x40000;
                self->timer      = 0;
            }

            self->nextGroundState = StateMachine_None;
            self->nextAirState    = StateMachine_None;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 6);
        }
    }
#if RETRO_GAMEVER != VER_100
    else if (ControllerInfo[self->controllerID].keyY.press)
        Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds);
#endif
}
#if RETRO_USE_PLUS
void Player_JumpAbility_Mighty(void)
{
    RSDK_THIS(Player);

    if (self->jumpAbilityState <= 1) {
        if (self->jumpPress && self->jumpAbilityState == 1
            && (self->stateInput != Player_ProcessP2Input_AI || (self->up && globals->gameMode != MODE_ENCORE))) {
            if (!self->invertGravity) {
                self->velocity.x >>= 1;
                self->velocity.y       = self->underwater ? 0x80000 : 0xC0000;
                self->jumpAbilityState = 0;
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_DROPDASH, &self->animator, false, 2);
                self->nextAirState    = StateMachine_None;
                self->nextGroundState = StateMachine_None;
                RSDK.PlaySfx(Player->sfxRelease, false, 0xFF);

                EntityImageTrail *trail = RSDK_GET_ENTITY(2 * Player->playerCount + RSDK.GetEntityID(self), ImageTrail);
                RSDK.ResetEntityPtr(trail, ImageTrail->classID, self);
                if (self->camera && !Zone->autoScrollSpeed) {
                    self->scrollDelay   = 8;
                    self->camera->state = Camera_State_HLock;
                }

                RSDK.PlaySfx(Player->sfxMightyDrill, false, 0xFE);
                self->state = Player_State_MightyHammerDrop;
            }
        }
        else if (ControllerInfo[self->controllerID].keyY.press)
            Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds);
    }
    else if (--self->jumpAbilityState == 1)
        self->jumpAbilityState = 0;
}
void Player_JumpAbility_Ray(void)
{
    RSDK_THIS(Player);

    if (self->jumpPress && self->jumpAbilityState == 1
        && (self->stateInput != Player_ProcessP2Input_AI || (self->up && globals->gameMode != MODE_ENCORE))) {
        if (!self->invertGravity) {
            self->jumpAbilityState = 0;

            int32 newXVel = self->velocity.x - (self->velocity.x >> 3);
            if (self->direction)
                self->velocity.x = minVal(newXVel, self->underwater ? -0x18000 : -0x30000);
            else
                self->velocity.x = maxVal(newXVel, self->underwater ? 0x18000 : 0x30000);

            if ((self->direction || !self->right) && (self->direction != FLIP_X || !self->left)) {
                if (!self->underwater)
                    Player->raySwoopTimer = 256;

                RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLY, &self->animator, false, 3);
                self->rotation = 1;

                self->velocity.x >>= 1;
                int32 vel = abs(self->velocity.x);
                self->abilitySpeed = minVal(-((vel >> 1) + (vel >> 2) + (vel >> 4)) >> (uint8)(self->underwater != 0), 0x40000);
            }
            else {
                if (!self->underwater)
                    Player->rayDiveTimer = 256;

                RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLYTIRED, &self->animator, false, 3);
                self->rotation     = 0;
                self->abilitySpeed = 0;
            }

            self->velocity.y >>= 1;
            self->abilityValue     = 64;
            self->controlLock      = 0;
            self->abilityValues[0] = abs(self->velocity.x);
            self->state            = Player_State_RayGlide;
            self->abilityTimer     = 256;
            self->nextAirState     = StateMachine_None;
            self->timer            = 0;
        }
    }
    else if (ControllerInfo[self->controllerID].keyY.press)
        Player_CheckGoSuper(self, SaveGame->saveRAM->chaosEmeralds);
}

bool32 Player_CheckRayDiving(void) { return Player->rayDiveTimer > 0; }
bool32 Player_CheckRaySwooping(void) { return Player->raySwoopTimer > 0; }

void Player_UpdateRayDiveSFX(int32 sfxID)
{
    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfxID], (float)Player->rayDiveTimer * (1.0f / 256.0f), 0.0, 1.0);
}
void Player_UpdateRaySwoopSFX(int32 sfxID)
{
    RSDK.SetChannelAttributes(Soundboard->sfxChannel[sfxID], (Player->raySwoopTimer * 0.8f) * (1.0f / 256.0f), 0.0, 1.0);
}
#endif

void Player_ProcessP1Input(void)
{
    RSDK_THIS(Player);

    if (self->controllerID < PLAYER_MAX) {
        if (globals->gameMode != MODE_COMPETITION || Announcer->finishedCountdown) {
#if RETRO_USE_TOUCH_CONTROLS
            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

                if (TouchInfo->down[t]) {
                    if (tx >= 0 && ty >= 96 && tx <= ScreenInfo->centerX && ty <= ScreenInfo->height) {
                        int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                        int32 ty = (TouchInfo->y[t] * ScreenInfo->height);
                        tx -= 64;
                        ty -= 192;

                        switch (((RSDK.ATan2(tx, ty) + 32) & 0xFF) >> 6) {
                            case 0:
                                ControllerInfo->keyRight.down |= true;
                                ControllerInfo[self->controllerID].keyRight.down = true;
                                break;
                            case 1:
                                ControllerInfo->keyDown.down |= true;
                                ControllerInfo[self->controllerID].keyDown.down = true;
                                break;
                            case 2:
                                ControllerInfo->keyLeft.down |= true;
                                ControllerInfo[self->controllerID].keyLeft.down = true;
                                break;
                            case 3:
                                ControllerInfo->keyUp.down |= true;
                                ControllerInfo[self->controllerID].keyUp.down = true;
                                break;
                        }
                        break;
                    }
                }
            }

            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

                if (TouchInfo->down[t]) {
                    if (tx >= ScreenInfo->centerX && ty >= 96 && tx <= ScreenInfo->width && ty <= ScreenInfo->height) {
                        ControllerInfo->keyA.down |= true;
                        ControllerInfo[self->controllerID].keyA.down = true;
                        break;
                    }
                }
            }

            if (!self->touchJump) {
                ControllerInfo->keyA.press |= ControllerInfo->keyA.down;
                ControllerInfo[self->controllerID].keyA.press |= ControllerInfo[self->controllerID].keyA.down;
            }
            self->touchJump = ControllerInfo[self->controllerID].keyA.down;

            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

                if (TouchInfo->down[t]) {
                    if (tx >= ScreenInfo->width - 0x80 && ty >= 0 && tx <= ScreenInfo->width && ty <= 0x40) {
                        if (SceneInfo->state == ENGINESTATE_REGULAR) {
                            EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                            bool32 allowPause                = true;
#if RETRO_USE_PLUS
                            if (ActClear && ActClear->actClearActive)
                                allowPause = false;
#endif
                            if (!RSDK.GetEntityCount(TitleCard->classID, false) && !pauseMenu->classID && allowPause) {
                                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                                pauseMenu->triggerPlayer = self->playerID;
                                if (globals->gameMode == MODE_COMPETITION)
                                    pauseMenu->disableRestart = true;
                            }
                        }
                        break;
                    }
                }
            }

#if RETRO_USE_PLUS
            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
                int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

                if (TouchInfo->down[t]) {
                    if (tx >= ScreenInfo->width - 0x100 && ty >= 0 && tx <= ScreenInfo->width - 0x80 && ty <= 0x40) {
                        if (globals->gameMode == MODE_ENCORE) {
                            if (HUD->swapCooldown || !Player_CheckValidState(self) || !Player_SwapMainPlayer(false)) {
                                RSDK.PlaySfx(Player->sfxSwapFail, false, 0xFF);
                            }
                        }
                        break;
                    }
                }
            }
#endif
#endif

            RSDKControllerState *controller = &ControllerInfo[self->controllerID];
            self->up                        = controller->keyUp.down;
            self->down                      = controller->keyDown.down;
            self->left                      = controller->keyLeft.down;
            self->right                     = controller->keyRight.down;

            self->up |= AnalogStickInfoL[self->controllerID].keyUp.down;
            self->down |= AnalogStickInfoL[self->controllerID].keyDown.down;
            self->left |= AnalogStickInfoL[self->controllerID].keyLeft.down;
            self->right |= AnalogStickInfoL[self->controllerID].keyRight.down;
            self->up |= AnalogStickInfoL[self->controllerID].vDelta > 0.3;
            self->down |= AnalogStickInfoL[self->controllerID].vDelta < -0.3;
            self->left |= AnalogStickInfoL[self->controllerID].hDelta < -0.3;
            self->right |= AnalogStickInfoL[self->controllerID].hDelta > 0.3;
            if (self->left && self->right) {
                self->left  = false;
                self->right = false;
            }
            self->jumpPress = controller->keyA.press || controller->keyB.press || controller->keyC.press || controller->keyX.press;
            self->jumpHold  = controller->keyA.down || controller->keyB.down || controller->keyC.down || controller->keyX.down;

            if (!LottoMachine || !((1 << self->playerID) & LottoMachine->activePlayers)) {
#if RETRO_USE_PLUS
                if (sku_platform == PLATFORM_DEV && controller->keyZ.press) {
                    Zone->swapGameMode = true;
                    RSDK.PlaySfx(Player->sfxTransform2, false, 0xFE);
                    Zone_StartFadeOut(64, 0xF0F0F0);
                }
                // TODO: TEMP!! I SOULD REMOVE THIS!!!
                else if (sku_platform == PLATFORM_DEV && controller->keySelect.press) {
                    self->characterID <<= 1;
                    if (self->characterID > ID_RAY)
                        self->characterID = 1;

                    Player_ChangeCharacter(self, self->characterID);
                }

                if (globals->gameMode == MODE_ENCORE && controller->keyY.press) {
                    if (HUD->swapCooldown || !Player_CheckValidState(self) || !Player_SwapMainPlayer(false)) {
                        RSDK.PlaySfx(Player->sfxSwapFail, false, 0xFF);
                    }
                }
#else
                // TODO: TEMP!! I SOULD REMOVE THIS!!!
                if (GameInfo->platform == PLATFORM_DEV && controller->keySelect.press) {
                    self->characterID <<= 1;
                    if (self->characterID > ID_KNUCKLES)
                        self->characterID = 1;

                    Player_ChangeCharacter(self, self->characterID);
                }
#endif

                if (self->controllerID == CONT_P1 && SceneInfo->debugMode && self->state != Player_State_Transform
                    && ControllerInfo[CONT_P1].keyX.press && globals->gameMode != MODE_TIMEATTACK) {
                    self->classID   = DebugMode->classID;
                    self->velocity.x = 0;
                    self->velocity.y = 0;
                    self->groundVel  = 0;
                    self->drawOrder  = Zone->playerDrawHigh;
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_AIRWALK, &self->animator, true, 0);
                    RSDK.SetGameMode(ENGINESTATE_REGULAR);
                    self->jumpHold         = false;
                    self->jumpPress        = false;
                    self->visible          = true;
                    self->active           = ACTIVE_NORMAL;
                    DebugMode->debugActive = true;
                }
            }

            if (controller->keyStart.press || Unknown_pausePress) {
                if (SceneInfo->state == ENGINESTATE_REGULAR) {
                    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                    bool32 allowPause                = true;
#if RETRO_USE_PLUS
                    if (ActClear && ActClear->actClearActive)
                        allowPause = false;
#endif
                    if (!RSDK.GetEntityCount(TitleCard->classID, false) && !pauseMenu->classID && allowPause) {
                        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                        pauseMenu->triggerPlayer = self->playerID;
                        if (globals->gameMode == MODE_COMPETITION)
                            pauseMenu->disableRestart = true;
                    }
                }
            }
        }
    }
}
void Player_ProcessP2InputLag(void)
{
    RSDK_THIS(Player);
    EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Player->upState <<= 1;
    Player->upState |= leader->up;
    Player->upState &= 0xFFFF;

    Player->downState <<= 1;
    Player->downState |= leader->down;
    Player->downState &= 0xFFFF;

    Player->leftState <<= 1;
    Player->leftState |= leader->left;
    Player->leftState &= 0xFFFF;

    Player->rightState <<= 1;
    Player->rightState |= leader->right;
    Player->rightState &= 0xFFFF;

    Player->jumpPressState <<= 1;
    Player->jumpPressState |= leader->jumpPress;
    Player->jumpPressState &= 0xFFFF;

    Player->jumpHoldState <<= 1;
    Player->jumpHoldState |= leader->jumpHold;
    Player->jumpHoldState &= 0xFFFF;

    if (leader->state == Player_State_FlyCarried) {
        Player->downState <<= 15;
        Player->leftState <<= 15;
        Player->rightState <<= 15;
        Player->jumpPressState <<= 15;
        Player->jumpHoldState <<= 15;
    }

    self->up        = Player->upState >> 15;
    self->down      = Player->downState >> 15;
    self->left      = Player->leftState >> 15;
    self->right     = Player->rightState >> 15;
    self->jumpPress = Player->jumpPressState >> 15;
    self->jumpHold  = Player->jumpHoldState >> 15;

    if (Player_CheckValidState(leader)) {
        Player->leaderPositionBuffer[Player->lastLeaderPosID].x = leader->position.x;
        Player->leaderPositionBuffer[Player->lastLeaderPosID].y = leader->position.y;
        Player->lastLeaderPosID                                 = (Player->lastLeaderPosID + 1) & 0xF;
        Player->nextLeaderPosID                                 = (Player->nextLeaderPosID + 1) & 0xF;
        if (!leader->onGround && leader->groundedStore) {
            Player->targetLeaderPosition.x = leader->position.x;
            Player->targetLeaderPosition.y = leader->position.y;
        }
        else {
            Player->targetLeaderPosition.x = Player->leaderPositionBuffer[Player->nextLeaderPosID].x;
            Player->targetLeaderPosition.y = Player->leaderPositionBuffer[Player->nextLeaderPosID].y;
        }
    }
    else {
        int32 pos = Player->lastLeaderPosID - 1;
        if (pos < 0)
            pos += 16;

        Player->targetLeaderPosition.x = Player->leaderPositionBuffer[pos].x;
        Player->targetLeaderPosition.y = Player->leaderPositionBuffer[pos].y;
    }
}
void Player_ProcessP2Input_AI(void)
{
    RSDK_THIS(Player);

    EntityPlayer *leader = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Player_ProcessP2InputLag();

    if (self->state == Player_State_TailsFlight && leader->state == Player_State_FlyCarried) {
        self->up        = leader->up;
        self->down      = leader->down;
        self->left      = leader->left;
        self->right     = leader->right;
        self->jumpHold  = leader->jumpHold;
        self->jumpPress = leader->jumpPress;
    }
    else if (leader->classID == Player->classID && leader->state != Player_State_FlyCarried) {
        int32 leaderPos = Player->targetLeaderPosition.x;
        if (leader->onGround || leader->groundedStore) {
            if (abs(leader->groundVel) < 0x20000) {
                if (!leader->direction)
                    leaderPos -= 0x200000;
                else
                    leaderPos += 0x200000;
            }
        }

        int32 distance = leaderPos - self->position.x;
        if (distance) {
            if (distance >= 0) {
                if (distance >= 0x300000) {
                    self->left  = false;
                    self->right = true;
                }

                if (self->groundVel && self->direction == FLIP_NONE)
                    self->position.x += 0xC0 * RSDK.Cos256(self->angle);
            }
            else {
                if (distance <= -0x300000) {
                    self->right = false;
                    self->left  = true;
                }

                if (self->groundVel && self->direction == FLIP_X) {
                    self->position.x -= 0xC0 * RSDK.Cos256(self->angle);
                }
            }
        }

        uint8 autoJump = 0;
        if (self->animator.animationID == ANI_PUSH) {
            if (leader->direction == self->direction && leader->animator.animationID == ANI_PUSH)
                Player->autoJumpTimer = 0;
            else
                autoJump = ++Player->autoJumpTimer < 30 ? 1 : 0;
        }
        else {
            if (self->position.y - Player->targetLeaderPosition.y <= 0x200000) {
                Player->autoJumpTimer = 0;
                if (self->controlLock > 0 && abs(self->groundVel) < 0x8000)
                    self->stateInput = Player_ProcessP2Input_JumpIn;
                autoJump = 2;
            }
            else {
                ++Player->autoJumpTimer;
                autoJump = Player->autoJumpTimer < 64 ? 1 : 0;
            }
        }

        if (autoJump == 1) {
            self->jumpHold = true;
        }
        else if (autoJump == 0) {
            if (self->onGround) {
                if (!self->jumpHold)
                    self->jumpPress = true;

                self->jumpHold = true;
            }

            self->applyJumpCap    = false;
            Player->autoJumpTimer = 0;
        }

        if (self->controlLock > 0 && abs(self->groundVel) < 0x8000)
            self->stateInput = Player_ProcessP2Input_JumpIn;
    }

    if (Player_CheckP2KeyPress())
        self->stateInput = Player_ProcessP2Input_Player;

    Player_P2JumpBackIn();
}
void Player_ProcessP2Input_JumpIn(void)
{
    RSDK_THIS(Player);

    self->up        = false;
    self->down      = false;
    self->left      = false;
    self->right     = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    if (!self->controlLock && self->onGround && self->groundVel < 0x4000) {
        self->groundVel       = 0;
        self->stateInput      = Player_ProcessP2Input_JumpDelay;

        Player->autoJumpTimer = 1;
        if (self->animator.animationID != ANI_SPINDASH) {
            self->down      = true;
            self->direction = self->position.x >= Player->targetLeaderPosition.x;
        }
    }

    if (Player_CheckP2KeyPress())
        self->stateInput = Player_ProcessP2Input_Player;

    Player_P2JumpBackIn();
}
void Player_ProcessP2Input_JumpDelay(void)
{
    RSDK_THIS(Player);

    if (Player->autoJumpTimer >= 64) {
        Player->autoJumpTimer = 0;
        self->down            = false;
        self->jumpPress       = false;
        self->stateInput      = Player_ProcessP2Input_AI;
    }
    else {
        self->down      = true;
        self->jumpPress = !(Player->autoJumpTimer & 0xF);
        ++Player->autoJumpTimer;
    }

    if (Player_CheckP2KeyPress())
        self->stateInput = Player_ProcessP2Input_Player;

    Player_P2JumpBackIn();
}
void Player_ProcessP2Input_Player(void)
{
    RSDK_THIS(Player);

    if (self->controllerID <= CONT_P4) {
        if (!API_InputIDIsDisconnected(self->controllerID)) {
            self->up    = ControllerInfo[self->controllerID].keyUp.down;
            self->down  = ControllerInfo[self->controllerID].keyDown.down;
            self->left  = ControllerInfo[self->controllerID].keyLeft.down;
            self->right = ControllerInfo[self->controllerID].keyRight.down;

            self->up |= AnalogStickInfoL[self->controllerID].vDelta > 0.3;
            self->down |= AnalogStickInfoL[self->controllerID].vDelta < -0.3;
            self->left |= AnalogStickInfoL[self->controllerID].hDelta < -0.3;
            self->right |= AnalogStickInfoL[self->controllerID].hDelta > 0.3;

            if (self->left && self->right) {
                self->left  = false;
                self->right = false;
            }

            self->jumpPress = ControllerInfo[self->controllerID].keyA.press || ControllerInfo[self->controllerID].keyB.press
                              || ControllerInfo[self->controllerID].keyC.press || ControllerInfo[self->controllerID].keyX.press;
            self->jumpHold = ControllerInfo[self->controllerID].keyA.down || ControllerInfo[self->controllerID].keyB.down
                             || ControllerInfo[self->controllerID].keyC.down || ControllerInfo[self->controllerID].keyX.down;

            if (self->right || self->up || self->down || self->left) {
                Player->aiInputSwapTimer = 0;
            }
            else if (++Player->aiInputSwapTimer >= 600) {
                self->stateInput = Player_ProcessP2Input_AI;
#if RETRO_USE_PLUS
                RSDK.AssignControllerID(self->controllerID, CONT_AUTOASSIGN);
#else
                APICallback_AssignControllerID(self->controllerID, CONT_AUTOASSIGN);
#endif
            }

            if (ControllerInfo[self->controllerID].keyStart.press && SceneInfo->state == ENGINESTATE_REGULAR) {
                EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
                if (!RSDK.GetEntityCount(TitleCard->classID, false) && !pauseMenu->classID) {
                    RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                    pauseMenu->triggerPlayer = 0;
                }
            }

            Player_P2JumpBackIn();
        }
        else {
            self->stateInput = Player_ProcessP2Input_AI;
            API_AssignControllerID(self->controllerID, CONT_AUTOASSIGN);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Player_EditorDraw(void)
{
    RSDK_THIS(Player);

    RSDK.SetSpriteAnimation(Player->sonicFrames, 0, &self->animator, true, self->characterID);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Player_EditorLoad(void)
{
    Player->sonicFrames = RSDK.LoadSpriteAnimation("Editor/PlayerIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Player, characterID);
    RSDK_ENUM_VAR("None", CHAR_NONE);
    RSDK_ENUM_VAR("Sonic", CHAR_SONIC);
    RSDK_ENUM_VAR("Tails", CHAR_TAILS);
    RSDK_ENUM_VAR("Sonic & Tails", CHAR_SONIC_TAILS);
    RSDK_ENUM_VAR("Knuckles", CHAR_KNUX);
    RSDK_ENUM_VAR("Sonic & Knuckles", CHAR_SONIC_KNUX);
    RSDK_ENUM_VAR("Tails & Knuckles", CHAR_TAILS_KNUX);
    RSDK_ENUM_VAR("Sonic, Tails & Knuckles", CHAR_SONIC_TAILS_KNUX);
}
#endif

void Player_Serialize(void) { RSDK_EDITABLE_VAR(Player, VAR_ENUM, characterID); }
