// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: OOZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectOOZSetup *OOZSetup;

void OOZSetup_Update(void) {}

void OOZSetup_LateUpdate(void) {}

void OOZSetup_StaticUpdate(void)
{
    foreach_all(OOZSetup, setup) { RSDK.AddDrawListRef(Zone->playerDrawGroup[0] + 1, RSDK.GetEntitySlot(setup)); }

    OOZSetup->palTimer += 128;
    if (OOZSetup->palTimer >= 256) {
        OOZSetup->palTimer -= 256;

        RSDK.RotatePalette(3, 216, 223, false);
        RSDK.RotatePalette(4, 216, 223, false);
        RSDK.RotatePalette(5, 216, 223, false);
        RSDK.RotatePalette(6, 216, 223, false);
    }

    RSDK.SetLimitedFade(1, 3, 4, OOZSetup->palTimer, 216, 223);
    RSDK.SetLimitedFade(2, 5, 6, OOZSetup->palTimer, 216, 223);
    RSDK.CopyPalette(1, 216, 0, 216, 8);

    if (!(Zone->timer & 3)) {
        ++OOZSetup->background1->deformationOffset;

        OOZSetup->oilAniTimer += 16;
        OOZSetup->oilAniTimer %= 160;

        RSDK.DrawAniTiles(OOZSetup->aniTiles, 760, 0, OOZSetup->oilAniTimer, 128, 16);
        RSDK.DrawAniTiles(OOZSetup->aniTiles, 757, 136, OOZSetup->oilAniTimer, 16, 16);
        RSDK.DrawAniTiles(OOZSetup->aniTiles, 758, 128, OOZSetup->oilAniTimer, 32, 16);
    }

    if (--OOZSetup->glowLightAniTimer <= 0) {
        ++OOZSetup->glowLightAniFrame;

        OOZSetup->glowLightAniFrame &= 7;
        OOZSetup->glowLightAniTimer = OOZSetup->glowLightAniDurations[OOZSetup->glowLightAniFrame];

        RSDK.DrawAniTiles(OOZSetup->aniTiles, 756, 16 * OOZSetup->glowLightAniFrame, 160, 16, 16);
    }

    OOZSetup->swimmingPlayerCount = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if (player->state != Player_State_Static) {
            Hitbox *playerHitbox = Player_GetHitbox(player);
            uint16 tile =
                RSDK.GetTile(Zone->fgLayer[0], player->position.x >> 20, ((playerHitbox->bottom << 16) + player->position.y - 0x10000) >> 20);
            if (tile == (uint16)-1)
                tile = RSDK.GetTile(Zone->fgLayer[1], player->position.x >> 20, ((playerHitbox->bottom << 16) + player->position.y - 0x10000) >> 20);

            int32 tileFlags = RSDK.GetTileFlags(tile, player->collisionPlane);
            if (tileFlags != OOZ_TFLAGS_NORMAL) {
                if (player->shield == SHIELD_FIRE && player->superState != SUPERSTATE_SUPER && tileFlags != OOZ_TFLAGS_OILFALL) {
                    int32 tx = (player->position.x & 0xFFF00000) + 0x70000;
                    int32 ty = player->position.y + ((playerHitbox->bottom + 8) << 16);
                    if (tileFlags == OOZ_TFLAGS_OILPOOL) {
                        ty &= 0xFFF00000;
                        ty -= 0xC0000;
                        if (OOZSetup_StartFire(tx, ty, player->angle)) {
                            EntitySol *sol  = CREATE_ENTITY(Sol, INT_TO_VOID(true), tx - 0x10000, ty);
                            sol->velocity.x = -0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->mainAnimator, true, 0);
                            sol->state = Sol_State_OilFlame;

                            sol             = CREATE_ENTITY(Sol, INT_TO_VOID(true), tx + 0x10000, ty);
                            sol->velocity.x = 0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->mainAnimator, true, 0);
                            sol->state = Sol_State_OilFlame;
                        }
                    }
                    else if (player->onGround) {
                        ty &= 0xFFFF0000;
                        if (OOZSetup_StartFire(tx, ty, player->angle)) {
                            ty -= 0x80000;
                            EntitySol *sol  = CREATE_ENTITY(Sol, INT_TO_VOID(true), tx - 0x10000, ty);
                            sol->velocity.x = -0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->mainAnimator, true, 0);
                            sol->state = Sol_State_FireballOilFlame;

                            sol             = CREATE_ENTITY(Sol, INT_TO_VOID(true), tx + 0x10000, ty);
                            sol->velocity.x = 0x40000;
                            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->mainAnimator, true, 0);
                            sol->state = Sol_State_FireballOilFlame;
                        }
                    }
                }

                switch (tileFlags) {
                    case OOZ_TFLAGS_NORMAL:
                    default: OOZSetup->activePlayers &= ~(1 << playerID); break;

                    case OOZ_TFLAGS_OILPOOL:
                        if (!player->sidekick)
                            OOZSetup->swimmingPlayerCount++;
                        OOZSetup->activePlayers &= ~(1 << playerID);
                        if (player->velocity.y < 0) {
                            player->velocity.y += 0x3800;
                        }
                        else {
                            player->interaction    = true;
                            player->tileCollisions = TILECOLLISION_DOWN;
                            player->state          = OOZSetup_PlayerState_OilPool;
                        }
                        break;

                    case OOZ_TFLAGS_OILSTRIP:
                        if (player->state != Player_State_BubbleBounce
#if MANIA_USE_PLUS
                            && player->state != Player_State_MightyHammerDrop
#endif
                        ) {
                            OOZSetup->activePlayers &= ~(1 << playerID);
                            if (player->onGround) {
                                player->interaction    = true;
                                player->tileCollisions = TILECOLLISION_DOWN;
                                player->state          = OOZSetup_PlayerState_OilStrip;
                            }
                        }
                        break;

                    case OOZ_TFLAGS_OILSLIDE:
                        if (player->state != Player_State_BubbleBounce
#if MANIA_USE_PLUS
                            && player->state != Player_State_MightyHammerDrop
#endif
                        ) {
                            OOZSetup->activePlayers &= ~(1 << playerID);
                            if (player->onGround) {
                                player->interaction    = true;
                                player->tileCollisions = TILECOLLISION_DOWN;
                                if (!player->angle)
                                    player->state = OOZSetup_PlayerState_OilStrip;
                                else
                                    player->state = OOZSetup_PlayerState_OilSlide;
                            }
                        }
                        break;

                    case OOZ_TFLAGS_OILFALL:
                        if (!player->sidekick)
                            OOZSetup->swimmingPlayerCount++;

                        OOZSetup->activePlayers |= 1 << playerID;
                        player->interaction    = true;
                        player->tileCollisions = TILECOLLISION_DOWN;
                        if (player->velocity.y < 0)
                            player->velocity.y += 0xC000;
                        else
                            player->state = OOZSetup_PlayerState_OilFall;
                        break;
                }
            }
            else {
                OOZSetup->activePlayers &= ~(1 << playerID);
            }
        }
    }

    if (OOZSetup->useSmogEffect && SceneInfo->timeEnabled) {
        if (OOZSetup->smogTimer < 0x10000)
            OOZSetup->smogTimer++;
    }
    else {
        if (OOZSetup->smogTimer > 0)
            OOZSetup->smogTimer -= 32;
    }

#if MANIA_USE_PLUS
    foreach_active(Ring, ring)
    {
        if (ring->state == Ring_State_Lost) {
            uint16 tile = RSDK.GetTile(Zone->fgLayer[0], ring->position.x >> 20, (ring->position.y + 0xE0000) >> 20);
            if (tile == (uint16)-1)
                tile = RSDK.GetTile(Zone->fgLayer[1], ring->position.x >> 20, (ring->position.y + 0xE0000) >> 20);

            if (RSDK.GetTileFlags(tile, ring->collisionPlane) == OOZ_TFLAGS_OILPOOL) {
                ring->velocity.x -= ring->velocity.x >> 4;
                ring->velocity.y = 0x2800;
                ring->drawGroup  = Zone->objectDrawGroup[1];
                if (ring->alpha > 0x40) {
                    ring->alpha     = 0x40;
                    ring->inkEffect = INK_ALPHA;
                }
            }
        }
    }
#endif

    OOZSetup_HandleActiveFlames();

#if MANIA_USE_PLUS
    RSDK.ProcessAnimation(&OOZSetup->animator);
#endif
}

void OOZSetup_Draw(void)
{
    RSDK_THIS(OOZSetup);

#if MANIA_USE_PLUS
    if (SceneInfo->currentDrawGroup != self->drawGroup) {
        foreach_active(Player, player)
        {
            if ((1 << RSDK.GetEntitySlot(player)) & OOZSetup->activePlayers)
                RSDK.DrawSprite(&OOZSetup->animator, &player->position, false);
        }
    }
#endif

    if (self->type)
        RSDK.FillScreen(0xC0C0E8, OOZSetup->smogTimer >> 5, OOZSetup->smogTimer >> 5, OOZSetup->smogTimer >> 4);
    else
        OOZSetup_Draw_Flames();
}

void OOZSetup_Create(void *data)
{
    RSDK_THIS(OOZSetup);

    self->active    = ACTIVE_ALWAYS;
    self->visible   = true;
    self->drawFX    = FX_ROTATE;
    self->drawGroup = self->type ? 14 : Zone->objectDrawGroup[0];
}

void OOZSetup_StageLoad(void)
{
    OOZSetup->aniTiles = RSDK.LoadSpriteSheet("OOZ/AniTiles.gif", SCOPE_STAGE);

    OOZSetup->background1 = RSDK.GetTileLayer(0);
    for (int32 i = 0; i < 0x400; ++i) {
        OOZSetup->background1->deformationData[i] = OOZSetup->deformData[i & 0x3F];
    }

    OOZSetup->smogTimer           = 0;
    OOZSetup->useSmogEffect       = true;
    OOZSetup->swimmingPlayerCount = 0;

    Animals->animalTypes[0] = ANIMAL_ROCKY;
    Animals->animalTypes[1] = ANIMAL_PECKY;

    memset(OOZSetup->flameTimers, 0, sizeof(OOZSetup->flameTimers));
    memset(OOZSetup->flameTimerPtrs, 0, sizeof(OOZSetup->flameTimerPtrs));
    OOZSetup->flameCount = 0;

    OOZSetup->solFrames = RSDK.LoadSpriteAnimation("OOZ/Sol.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(OOZSetup->solFrames, 3, &OOZSetup->flameAnimator, true, 0);

#if MANIA_USE_PLUS
    OOZSetup->splashFrames = RSDK.LoadSpriteAnimation("OOZ/Splash.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(OOZSetup->splashFrames, 0, &OOZSetup->animator, true, 0);
#endif

    if (RSDK.CheckSceneFolder("OOZ2")) {
        foreach_all(OOZ1Outro, cutscene)
        {
            OOZSetup->cutscenePtr = (Entity *)cutscene;
            foreach_break;
        }

        GenericTrigger->callbacks[OOZ_GENERICTRIGGER_ACHIEVEMENT] = OOZSetup_Trigger_AwardAchievement;

        if (!StarPost->postIDs[0])
            Zone->cameraBoundsB[0] = 1600;

        if (isMainGameMode() && globals->enableIntro) {
            if (!CutsceneRules_CheckStageReload()) {
                SaveGame_LoadPlayerState();
                globals->suppressAutoMusic = true;
                globals->suppressTitlecard = true;
            }
        }

#if MANIA_USE_PLUS
        if (isMainGameMode() && CutsceneRules_IsAct2())
            Zone->stageFinishCallback = OOZ2Outro_StageFinish_EndAct2;

        if (SceneInfo->filter & FILTER_ENCORE) {
            RSDK.LoadPalette(0, "EncoreOOZ2.act", 0b0000000011111111);
            RSDK.LoadPalette(2, "EncoreOOZSmog.act", 0b0000000011111111);

            RSDK.CopyPalette(0, 128, 1, 128, 128);
            RSDK.CopyPalette(0, 128, 3, 128, 128);
            RSDK.CopyPalette(0, 128, 4, 128, 128);
            RSDK.RotatePalette(4, 215, 223, false);

            RSDK.CopyPalette(2, 128, 5, 128, 128);
            RSDK.CopyPalette(2, 128, 6, 128, 128);
            RSDK.RotatePalette(6, 215, 223, false);
        }
#endif
    }
#if MANIA_USE_PLUS
    else if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreOOZ1.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 1, 128, 80);
        RSDK.CopyPalette(0, 128, 3, 128, 80);
    }
#endif

    int32 sfxID = Soundboard_LoadSfx("OOZ/Slide.wav", 12382, OOZSetup_SfxCheck_Slide, StateMachine_None);
    if (sfxID >= 0)
        Soundboard->sfxFadeOutDuration[sfxID] = 30;

    sfxID = Soundboard_LoadSfx("OOZ/OilSwim.wav", true, OOZSetup_SfxCheck_OilSwim, StateMachine_None);
    if (sfxID >= 0)
        Soundboard->sfxFadeOutDuration[sfxID] = 30;

    sfxID = Soundboard_LoadSfx("Stage/Flame2.wav", true, OOZSetup_SfxCheck_Flame2, StateMachine_None);
    if (sfxID >= 0)
        Soundboard->sfxFadeOutDuration[sfxID] = 30;
}

bool32 OOZSetup_SfxCheck_Flame2(void)
{
    int32 count = 0;

    foreach_active(Sol, sol)
    {
        if (sol->isFlameFX)
            count++;
    }

    for (int32 i = 0; i < OOZSetup->flameCount; ++i) {
        if (OOZSetup->flameTimerPtrs[i])
            count++;
    }

    return count > 0;
}

bool32 OOZSetup_SfxCheck_Slide(void)
{
    int32 count = 0;

    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            if (player->state == OOZSetup_PlayerState_OilSlide || player->state == OOZSetup_PlayerState_OilStrip)
                ++count;
        }
    }

    return count > 0;
}

bool32 OOZSetup_SfxCheck_OilSwim(void) { return OOZSetup->swimmingPlayerCount > 0; }

void OOZSetup_Draw_Flames(void)
{
    RSDK_THIS(OOZSetup);

    for (int32 i = 0; i < OOZSetup->flameCount; ++i) {
        if (OOZSetup->flameTimerPtrs[i]) {
            self->rotation                  = 2 * (OOZSetup->flamePositions[i].x & 0xFF);
            OOZSetup->flameAnimator.frameID = OOZSetup->flamePositions[i].y & 0xFF;
            RSDK.DrawSprite(&OOZSetup->flameAnimator, &OOZSetup->flamePositions[i], false);
        }
    }
}

void OOZSetup_HandleActiveFlames(void)
{
    RSDK_THIS(OOZSetup);

    for (int32 i = 0; i < OOZSetup->flameCount; ++i) {
        if (OOZSetup->flameTimerPtrs[i]) {
            --(*OOZSetup->flameTimerPtrs[i]);

            if (!*OOZSetup->flameTimerPtrs[i]) {
                OOZSetup->flameTimerPtrs[i] = NULL;
                EntitySol *sol              = CREATE_ENTITY(Sol, INT_TO_VOID(true), OOZSetup->flamePositions[i].x, OOZSetup->flamePositions[i].y);
                sol->isFlameFX              = true;
                sol->rotation               = 2 * (OOZSetup->flamePositions[i].x & 0xFF);
                RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &sol->mainAnimator, true, 0);
                sol->state = Sol_State_FlameDissipate;
            }
            else {
                int32 frame      = OOZSetup->flamePositions[i].y & 0xFF;
                int32 frameTimer = (OOZSetup->flamePositions[i].y >> 8) & 0xFF;

                if (++frameTimer >= 3) {
                    frameTimer = 0;
                    if (++frame > 10)
                        frame = 1;
                }

                // likewise, this too is evil, using the lower 2 bytes to store frame info
                OOZSetup->flamePositions[i].y = frame | (frameTimer << 8) | (OOZSetup->flamePositions[i].y & 0xFFFF0000);
            }

            Vector2 storePos = self->position;
            foreach_active(Player, player)
            {
                self->position = OOZSetup->flamePositions[i];
                if (Player_CheckCollisionTouch(player, self, &Sol->hitboxBadnik)) {
                    self->position = storePos;
                    Player_ElementHurt(player, self, SHIELD_FIRE);
                }
            }
            self->position = storePos;
        }
    }
}

bool32 OOZSetup_StartFire(int32 posX, int32 posY, int32 angle)
{
    RSDK_THIS(OOZSetup);

    int32 pos = (posX >> 20) + (posY >> 20 << 10);

    if (pos < 0x20000) {
        if (!OOZSetup->flameTimers[pos]) {
            int32 i = 0;
            for (; i < 399; ++i) {
                if (!OOZSetup->flameTimerPtrs[i])
                    break;
            }
            // if we get to 399 active flames just use that slot over and over 

            OOZSetup->flameTimerPtrs[i]   = &OOZSetup->flameTimers[pos];
            OOZSetup->flamePositions[i].x = posX;
            OOZSetup->flamePositions[i].y = posY;
            OOZSetup->flamePositions[i].x &= 0xFFFF0000;
            OOZSetup->flamePositions[i].y &= 0xFFFF0000;
            // this is so evil, using the bottom byte of the XPos to store the angle
            OOZSetup->flamePositions[i].x |= angle;

            if (i + 1 > OOZSetup->flameCount)
                OOZSetup->flameCount = i + 1;

            OOZSetup->flameTimers[pos]                                                                                     = 0xF0;
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y - 0x60000)->drawGroup = self->drawGroup;

            return true;
        }
    }

    return false;
}

void OOZSetup_Trigger_AwardAchievement(void)
{
    if (!OOZSetup->hasAchievement) {
        API_UnlockAchievement(&achievementList[ACH_OOZ]);
        OOZSetup->hasAchievement = true;
    }
}

void OOZSetup_PlayerState_OilPool(void)
{
    RSDK_THIS(Player);

    int32 top          = self->topSpeed;
    int32 acc          = self->acceleration;
    self->topSpeed     = (self->topSpeed >> 1) + (self->topSpeed >> 3);
    self->acceleration = (self->acceleration >> 1) + (self->acceleration >> 3);

    if (self->groundVel <= 0) {
        if (self->groundVel < -self->topSpeed) {
            self->groundVel += 0x3800;
        }
    }
    else if (self->groundVel > self->topSpeed) {
        self->groundVel -= 0x3800;
    }

    self->position.y += 0x10000;

    self->up       = false;
    self->down     = false;
    self->onGround = true;

    Player_State_Ground();

    self->topSpeed     = top;
    self->acceleration = acc;
}

void OOZSetup_PlayerState_OilStrip(void)
{
    RSDK_THIS(Player);

    int32 acc  = self->acceleration;
    int32 top  = self->topSpeed;
    int32 skid = self->skidSpeed;
    int32 dec  = self->deceleration;

    Animator animator;
    memcpy(&animator, &self->animator, sizeof(Animator));

    self->position.y += 0x10000;

    self->skidSpeed    = self->skidSpeed >> 3;
    self->topSpeed     = (self->topSpeed >> 1) + (self->topSpeed >> 2);
    self->acceleration = 0x400;
    self->deceleration = self->deceleration >> 3;

    self->up       = false;
    self->down     = false;
    self->skidding = 0;
    self->onGround = true;

    Player_State_Ground();

#if MANIA_USE_PLUS
    if ((animator.animationID == ANI_HURT || animator.animationID == ANI_FLUME) && self->groundedStore && self->onGround) {
#else
    if (animator.animationID == ANI_HURT && self->groundedStore && self->onGround) {
#endif
        if (abs(self->groundVel) >= 0x20000) {
            memcpy(&self->animator, &animator, sizeof(Animator));
            if (self->animator.timer >= 3)
                self->animator.timer = 256;

            if (self->angle == 0x40 || self->angle == 0xC0) {
                self->onGround = false;
                self->state    = Player_State_Air;
            }
        }
    }

    self->acceleration = acc;
    self->topSpeed     = top;
    self->skidSpeed    = skid;
    self->deceleration = dec;
}

void OOZSetup_PlayerState_OilSlide(void)
{
    RSDK_THIS(Player);

    if (!self->onGround) {
        self->state = Player_State_Air;
        Player_HandleAirMovement();
    }
    else {
        Player_Gravity_False();

        if (self->angle) {
            if (self->angle <= 0x80) {
                if (self->groundVel < 0x80000)
                    self->groundVel += 0x4000;
            }
            else {
                if (self->groundVel > -0x80000)
                    self->groundVel -= 0x4000;
            }
        }

        self->groundVel += RSDK.Sin256(self->angle) << 13 >> 8;
        self->controlLock = 30;
        self->direction   = self->groundVel < 0;

#if MANIA_USE_PLUS
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_FLUME, &self->animator, false, 0);
#else
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_HURT, &self->animator, false, 2);
#endif
    }

    if (self->jumpPress) {
        Player_Action_Jump(self);

        if (self->angle <= 0x80) {
            if (self->velocity.x < 0)
                self->velocity.x += ((self->jumpStrength + (self->jumpStrength >> 1)) * RSDK.Sin256(self->angle)) >> 8;
        }
        else {
            if (self->velocity.x > 0)
                self->velocity.x += ((self->jumpStrength + (self->jumpStrength >> 1)) * RSDK.Sin256(self->angle)) >> 8;
        }
    }
}

void OOZSetup_PlayerState_OilFall(void)
{
    RSDK_THIS(Player);

    int32 top = self->topSpeed;
    int32 acc = self->acceleration;

    self->topSpeed     = (self->topSpeed >> 2) + (self->topSpeed >> 3);
    self->acceleration = (self->acceleration >> 2) + (self->acceleration >> 3);

    if (self->groundVel <= 0) {
        if (self->groundVel < -self->topSpeed)
            self->groundVel += 0x3800;
    }
    else {
        if (self->groundVel > self->topSpeed)
            self->groundVel -= 0x3800;
    }

    self->up            = false;
    self->down          = false;
    self->groundedStore = false;
    self->onGround      = false;
    self->velocity.y    = 0x10000;

    if (abs(self->velocity.x) > 0x20000) {
        if (self->velocity.x <= 0)
            self->velocity.x += 0xC000;
        else
            self->velocity.x -= 0xC000;
    }

    if (!self->left && !self->right)
        self->velocity.x -= self->velocity.x >> 4;

    RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);

    Player_HandleGroundMovement();

    if (self->camera)
        self->camera->disableYOffset = false;

    self->jumpAbilityState = 0;
    self->nextAirState     = Player_State_Air;

    if (self->jumpPress) {
        Player_Action_Jump(self);

        self->jumpAbilityState = 0;
        self->timer            = 0;
    }

    self->topSpeed     = top;
    self->acceleration = acc;
}

#if GAME_INCLUDE_EDITOR
void OOZSetup_EditorDraw(void) {}

void OOZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(OOZSetup, type);
    RSDK_ENUM_VAR("Manage Flames", OOZSETUP_FLAMES);
    RSDK_ENUM_VAR("Manage Fade", OOZSETUP_FADE);

    RSDK_ACTIVE_VAR(WarpDoor, effect);
    RSDK_ENUM_VAR("None", WARPDOOR_EFFECT_NONE);
    RSDK_ENUM_VAR("To Sub", OOZ_WARPDOOR_EFFECT_TO_SUB);
    RSDK_ENUM_VAR("From Sub", OOZ_WARPDOOR_EFFECT_FROM_SUB);

    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Award Achievement", OOZ_GENERICTRIGGER_ACHIEVEMENT);
}
#endif

void OOZSetup_Serialize(void) { RSDK_EDITABLE_VAR(OOZSetup, VAR_UINT8, type); }
