// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SignPost Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSignPost *SignPost;

void SignPost_Update(void)
{
    RSDK_THIS(SignPost);
    StateMachine_Run(self->state);
}

void SignPost_LateUpdate(void) {}

void SignPost_StaticUpdate(void) {}

void SignPost_Draw(void)
{
    RSDK_THIS(SignPost);
    Vector2 drawPos;
    if (self->state) {
        self->drawFX = FX_SCALE;
        drawPos.y    = self->position.y;

        Animator *face = &self->facePlateAnimator;
        if (self->rotation <= 128 || self->rotation >= 384)
            face = &self->eggPlateAnimator;

        self->scale.x = abs(RSDK.Cos512(self->rotation));
        int32 scale   = abs(RSDK.Sin512(self->rotation));

        switch (self->rotation >> 7) {
            case 0:
            case 2:
                drawPos.x = self->position.x + (scale << 9);
                RSDK.DrawSprite(face, &drawPos, false);
                drawPos.x += -0xC00 * self->scale.x - (scale << 9);
                break;

            case 1:
            case 3:
                drawPos.x = self->position.x - (scale << 9);
                RSDK.DrawSprite(face, &drawPos, false);
                drawPos.x += (scale + 2 * (3 * self->scale.x - 32)) << 9;
                break;

            default: break;
        }
        self->scale.x = scale;
        RSDK.DrawSprite(&self->sidebarAnimator, &drawPos, false);

        self->drawFX = FX_NONE;
        RSDK.DrawSprite(&self->postTopAnimator, NULL, false);
        RSDK.DrawSprite(&self->standAnimator, NULL, false);
    }
}

void SignPost_Create(void *data)
{
    RSDK_THIS(SignPost);

    if (!self->vsBoundsSize.x)
        self->vsBoundsSize.x = TO_FIXED(512);
    if (!self->vsBoundsSize.y)
        self->vsBoundsSize.y = TO_FIXED(SCREEN_YSIZE);
    if (!self->vsExtendTop)
        self->vsExtendTop = 120;
    if (!self->vsExtendBottom)
        self->vsExtendBottom = 120;

    if (!SceneInfo->inEditor) {
        if (globals->gameMode != MODE_TIMEATTACK) {
            RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_EGGMAN, &self->eggPlateAnimator, true, 0);
            switch (GET_CHARACTER_ID(1)) {
                default:
                case ID_SONIC: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_SONIC, &self->facePlateAnimator, true, 0); break;

                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_TAILS, &self->facePlateAnimator, true, 0); break;

                case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_KNUX, &self->facePlateAnimator, true, 0); break;

#if MANIA_USE_PLUS
                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_MIGHTY, &self->facePlateAnimator, true, 0); break;

                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_RAY, &self->facePlateAnimator, true, 0); break;
#endif
            }
            RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &self->postTopAnimator, true, 0);
            RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &self->sidebarAnimator, true, 1);
            RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &self->standAnimator, true, 2);

            self->updateRange.x = TO_FIXED(64);
            self->updateRange.y = TO_FIXED(64);
            if (globals->gameMode == MODE_COMPETITION) {
                int32 left   = self->vsBoundsOffset.x - (self->vsBoundsSize.x >> 1);
                int32 top    = self->vsBoundsOffset.y - (self->vsBoundsSize.y >> 1);
                int32 right  = self->vsBoundsOffset.x + (self->vsBoundsSize.x >> 1);
                int32 bottom = self->vsBoundsOffset.y + (self->vsBoundsSize.y >> 1);

                int32 extendTop    = -TO_FIXED(self->vsExtendTop);
                int32 extendBottom = TO_FIXED(self->vsExtendBottom);

                if (extendTop < top)
                    top = extendTop;

                if (extendBottom < bottom)
                    bottom = extendBottom;

                if (abs(left) > right)
                    self->updateRange.x = abs(left);
                else
                    self->updateRange.x = right;

                if (abs(top) > bottom)
                    self->updateRange.y = abs(top);
                else
                    self->updateRange.y = bottom;

                self->updateRange.x += TO_FIXED(64);
                self->updateRange.y += TO_FIXED(64);
            }
            self->visible   = true;
            self->drawGroup = Zone->objectDrawGroup[0];
            self->spinSpeed = 0x3000;
            self->spinCount = 8;
            self->maxAngle  = 0x10000;
            self->scale.y   = 0x200;

            bool32 destroy = true;
            switch (self->type) {
                default: break;
                case SIGNPOST_RUNPAST: // Normal (Main Game Only)
                    if (globals->gameMode != MODE_COMPETITION) {
                        self->active = ACTIVE_BOUNDS;
                        self->state  = SignPost_State_Init;
                        destroy      = false;
                    }
                    break;

                case SIGNPOST_DROP: // Hidden (Until Dropped)
                    if (globals->gameMode != MODE_COMPETITION) {
                        self->active = ACTIVE_XBOUNDS;
                        self->state  = StateMachine_None;
                        destroy      = false;
                    }
                    break;

                case SIGNPOST_COMP: // Normal (Competition Only)
                    if (globals->gameMode == MODE_COMPETITION) {
                        self->active = ACTIVE_BOUNDS;
                        self->state  = SignPost_State_Init;
                        destroy      = false;
                    }
                    break;

                case SIGNPOST_DECOR: // Decoration
                    if (globals->gameMode != MODE_COMPETITION) {
                        self->active = ACTIVE_BOUNDS;
                        self->state  = SignPost_State_Done;
                        destroy      = false;
                    }
                    break;
            }

            if (destroy)
                destroyEntity(self);
        }
    }
}

void SignPost_StageLoad(void)
{
    SignPost->aniFrames = RSDK.LoadSpriteAnimation("Global/SignPost.bin", SCOPE_STAGE);

    SignPost->hitboxSignPost.left   = -24;
    SignPost->hitboxSignPost.top    = -22;
    SignPost->hitboxSignPost.right  = 24;
    SignPost->hitboxSignPost.bottom = 8;

    SignPost->hitboxItemBox.left   = -8;
    SignPost->hitboxItemBox.top    = 20;
    SignPost->hitboxItemBox.right  = 8;
    SignPost->hitboxItemBox.bottom = 24;

    SignPost->maxPlayerCount = (1 << Player->playerCount) - 1;

    SignPost->sfxSignPost     = RSDK.GetSfx("Global/SignPost.wav");
    SignPost->sfxSignPost2P   = RSDK.GetSfx("Global/SignPost2p.wav");
    SignPost->sfxTwinkle      = RSDK.GetSfx("Global/Twinkle.wav");
    SignPost->sfxBubbleBounce = RSDK.GetSfx("Global/BubbleBounce.wav");
    SignPost->sfxSlide        = RSDK.GetSfx("Global/Slide.wav");

    DEBUGMODE_ADD_OBJ(SignPost);
}

void SignPost_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntitySignPost *signpost     = CREATE_ENTITY(SignPost, NULL, self->position.x, self->position.y);
    signpost->spawnedByDebugMode = true;
}
void SignPost_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_EGGMAN, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &DebugMode->animator, true, 2);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void SignPost_HandleSpin(void)
{
    RSDK_THIS(SignPost);
    self->angle += self->spinSpeed;

    if (self->angle >= self->maxAngle) {
        self->maxAngle += 0x20000;
        self->spinSpeed = MIN(0x600 * self->spinCount, 0x3000);
        if (!--self->spinCount) {
            self->spinSpeed               = 0;
            self->angle                   = 0x10000;
            self->facePlateAnimator.speed = 1;
        }
    }
    self->rotation = (self->angle >> 8) & 0x1FF;
}
void SignPost_HandleSparkles(void)
{
    RSDK_THIS(SignPost);
    if (!(Zone->timer & 3)) {
        int32 x          = self->position.x + RSDK.Rand(-TO_FIXED(24), TO_FIXED(24));
        int32 y          = self->position.y + RSDK.Rand(-TO_FIXED(32), TO_FIXED(8));
        EntityRing *ring = CREATE_ENTITY(Ring, NULL, x, y);

        ring->state     = Ring_State_Sparkle;
        ring->stateDraw = Ring_Draw_Sparkle;
        ring->active    = ACTIVE_NORMAL;
        ring->visible   = false;
        RSDK.SetSpriteAnimation(Ring->aniFrames, self->sparkleType + 2, &ring->animator, true, 0);
        int32 cnt = ring->animator.frameCount;
        if (ring->animator.animationID == 2) {
            ring->alpha = 0xE0;
            cnt >>= 1;
        }
        ring->maxFrameCount  = cnt - 1;
        ring->animator.speed = 6;
        self->sparkleType    = (self->sparkleType + 1) % 3;
    }
}
void SignPost_HandleCamBounds(void)
{
    RSDK_THIS(SignPost);
    int32 x = self->vsBoundsOffset.x + self->position.x;
    int32 y = self->vsBoundsOffset.y + self->position.y;

    Hitbox hitbox;
    hitbox.left   = -self->vsBoundsSize.x >> 17;
    hitbox.top    = -self->vsBoundsSize.y >> 17;
    hitbox.right  = self->vsBoundsSize.x >> 17;
    hitbox.bottom = self->vsBoundsSize.y >> 17;

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (player->classID == Player->classID && !player->sidekick) {
            if (globals->gameMode == MODE_COMPETITION) {
                int32 storeX     = self->position.x;
                int32 storeY     = self->position.y;
                self->position.x = x;
                self->position.y = y;
                if (Player_CheckCollisionTouch(player, self, &hitbox)) {
                    self->position.x            = storeX;
                    self->position.y            = storeY;
                    Zone->cameraBoundsL[p]      = FROM_FIXED(self->position.x) - ScreenInfo[p].center.x;
                    Zone->cameraBoundsR[p]      = FROM_FIXED(self->position.x) + ScreenInfo[p].center.x;
                    Zone->playerBoundActiveR[p] = true;
                }
                else {
                    self->position.x = storeX;
                    self->position.y = storeY;
                }
            }
            else {
                if (self->position.x - player->position.x < TO_FIXED(256) || self->position.x - (Zone->cameraBoundsR[p] << 16) < TO_FIXED(256)) {
                    Zone->cameraBoundsL[p] = FROM_FIXED(self->position.x) - ScreenInfo[p].center.x;
                    Zone->cameraBoundsR[p] = FROM_FIXED(self->position.x) + ScreenInfo[p].center.x;
                }
            }
        }
    }
}
void SignPost_CheckTouch(void)
{
    RSDK_THIS(SignPost);

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (self->activePlayers && RSDK_GET_ENTITY(p + Player->playerCount, Player)->classID == GameOver->classID) {
            self->activePlayers |= 1 << p;
        }
        else if (!p || globals->gameMode == MODE_COMPETITION) {
            if (!((1 << p) & self->activePlayers)) {
                bool32 passedSignpost = false;
                if (globals->gameMode != MODE_COMPETITION) {
                    passedSignpost = player->position.x > self->position.x;
                }
                else if (self->playerPosStore[p].x && self->playerPosStore[p].y) {
                    passedSignpost = MathHelpers_CheckPositionOverlap(
                        player->position.x, player->position.y, self->playerPosStore[p].x, self->playerPosStore[p].y, self->position.x,
                        self->position.y - (self->vsExtendTop << 16), self->position.x, self->position.y + (self->vsExtendBottom << 16));
                }

                if (passedSignpost) {
                    if (!((1 << p) & self->activePlayers) && globals->gameMode == MODE_COMPETITION)
                        Announcer_AnnounceGoal(player->camera->screenID);

                    RSDK.PlaySfx(SignPost->sfxSignPost, false, 0xFF);
                    self->active = ACTIVE_NORMAL;
                    if (player->superState == SUPERSTATE_SUPER)
                        player->superState = SUPERSTATE_FADEOUT;

                    int32 vel = 0;
                    if (player->onGround)
                        vel = player->groundVel;
                    else
                        vel = player->velocity.x;

                    self->velocity.y      = -(vel >> 1);
                    self->gravityStrength = vel / 96;
                    if (globals->gameMode == MODE_COMPETITION) {
                        self->active = ACTIVE_NORMAL;
                        if (!self->activePlayers) {
                            switch (player->characterID) {
                                default:
                                case ID_SONIC:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_SONIC, &self->facePlateAnimator, true, 0);
                                    break;

                                case ID_TAILS:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_TAILS, &self->facePlateAnimator, true, 0);
                                    break;

                                case ID_KNUCKLES:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_KNUX, &self->facePlateAnimator, true, 0);
                                    break;
#if MANIA_USE_PLUS
                                case ID_MIGHTY:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_MIGHTY, &self->facePlateAnimator, true, 0);
                                    break;

                                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_RAY, &self->facePlateAnimator, true, 0); break;
#endif
                            }
                            RSDK.PlaySfx(SignPost->sfxSignPost2P, false, 255);
                        }

                        EntityCompetitionSession *session = CompetitionSession_GetSession();
                        EntityCompetition *manager        = Competition->sessionManager;
                        if (!manager)
                            manager = Competition->sessionManager = CREATE_ENTITY(Competition, NULL, self->position.x, self->position.y);

                        manager->playerFinished[player->playerID]    = true;
                        session->rings[player->playerID]             = player->rings;
                        session->time[player->playerID].minutes      = SceneInfo->minutes;
                        session->time[player->playerID].seconds      = SceneInfo->seconds;
                        session->time[player->playerID].milliseconds = SceneInfo->milliseconds;
                        session->score[player->playerID]             = player->score;
                        session->lives[player->playerID]             = player->lives;
                        CompSession_DeriveWinner(player->playerID, FINISHTYPE_PASSEDSIGNPOST);

                        self->activePlayers |= (1 << p);
                        if (self->activePlayers == SignPost->maxPlayerCount)
                            Music_FadeOut(0.025);
                        self->state = SignPost_State_SpinVS;
                    }
                    else {
#if MANIA_USE_PLUS
                        if (globals->gameMode == MODE_ENCORE) {
                            switch (GET_CHARACTER_ID(1)) {
                                default:
                                case ID_SONIC:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_SONIC, &self->facePlateAnimator, true, 0);
                                    break;

                                case ID_TAILS:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_TAILS, &self->facePlateAnimator, true, 0);
                                    break;

                                case ID_KNUCKLES:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_KNUX, &self->facePlateAnimator, true, 0);
                                    break;

                                case ID_MIGHTY:
                                    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_MIGHTY, &self->facePlateAnimator, true, 0);
                                    break;

                                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_RAY, &self->facePlateAnimator, true, 0); break;
                            }
                        }
#endif

                        SceneInfo->timeEnabled = false;
                        if (vel >= TO_FIXED(4)) {
                            self->state = SignPost_State_Launched;
                        }
                        else {
                            Music_FadeOut(0.025);
                            self->state = SignPost_State_Spin;
                        }
                    }
                }

                self->playerPosStore[p].x = player->position.x;
                self->playerPosStore[p].y = player->position.y;
            }
        }
    }
}

void SignPost_State_Init(void)
{
    RSDK_THIS(SignPost);

    self->state = SignPost_State_AwaitTouch;
}
void SignPost_State_AwaitTouch(void)
{
    SignPost_HandleCamBounds();
    SignPost_CheckTouch();
}
void SignPost_State_Spin(void)
{
    RSDK_THIS(SignPost);

    SignPost_HandleSpin();
    SignPost_HandleSparkles();

    RSDK.ProcessAnimation(&self->facePlateAnimator);

    if (!self->spinCount) {
        self->type  = SIGNPOST_DECOR;
        self->state = SignPost_State_Done;
        Music_PlayTrack(TRACK_ACTCLEAR);
        RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->classID, NULL);
    }
}
void SignPost_State_SpinVS(void)
{
    RSDK_THIS(SignPost);
    SignPost_HandleSpin();
    SignPost_HandleSparkles();
    RSDK.ProcessAnimation(&self->facePlateAnimator);
    SignPost_CheckTouch();

    if (!self->spinCount) {
        if (self->activePlayers >= SignPost->maxPlayerCount) {
            self->type             = SIGNPOST_DECOR;
            self->state            = SignPost_State_Done;
            SceneInfo->timeEnabled = false;
            Zone_StartFadeOut_Competition(10, 0x000000);
        }
        else {
            self->spinSpeed = 0x3000;
            self->spinCount = 8;
            self->maxAngle  = 0x10000;
            self->state     = SignPost_State_AwaitTouch;
        }
    }
}
void SignPost_State_Launched(void)
{
    RSDK_THIS(SignPost);

    SignPost_HandleSpin();
    SignPost_HandleSparkles();

    RSDK.ProcessAnimation(&self->facePlateAnimator);

    self->spinCount = 16;
    self->position.y += self->velocity.y;
    self->velocity.y += self->gravityStrength;

    if (self->velocity.y >= 0)
        self->state = SignPost_State_Falling;
}
void SignPost_State_Falling(void)
{
    RSDK_THIS(SignPost);

    self->active           = ACTIVE_NORMAL;
    SceneInfo->timeEnabled = false;

    if (self->type == SIGNPOST_DROP) {
        self->type = SIGNPOST_RUNPAST;
        if (globals->gameMode < MODE_COMPETITION) {
            switch (GET_CHARACTER_ID(1)) {
                default:
                case ID_SONIC: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_SONIC, &self->facePlateAnimator, true, 0); break;
                case ID_TAILS: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_TAILS, &self->facePlateAnimator, true, 0); break;
                case ID_KNUCKLES: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_KNUX, &self->facePlateAnimator, true, 0); break;
#if MANIA_USE_PLUS
                case ID_MIGHTY: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_MIGHTY, &self->facePlateAnimator, true, 0); break;
                case ID_RAY: RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_RAY, &self->facePlateAnimator, true, 0); break;
#endif
            }
        }
    }

    foreach_active(Player, player)
    {
        if (self->velocity.y >= 0) {
            if (player->velocity.y < 0 && player->animator.animationID == ANI_JUMP && !player->onGround) {
                if (Player_CheckCollisionTouch(player, self, &SignPost->hitboxSignPost)) {
                    self->velocity.x = (self->position.x - player->position.x) >> 4;
                    self->velocity.y = -TO_FIXED(2);
                    RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
                    EntityScoreBonus *scoreBonus = CREATE_ENTITY(ScoreBonus, NULL, self->position.x, self->position.y);
                    scoreBonus->drawGroup        = Zone->objectDrawGroup[1];
                    scoreBonus->animator.frameID = 0;
                    Player_GiveScore(player, 100);
                }
            }
        }
    }

    SignPost_HandleSpin();
    SignPost_HandleSparkles();
    RSDK.ProcessAnimation(&self->facePlateAnimator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->spinCount = 16;

    if (self->velocity.x >= 0) {
        if (self->position.x > TO_FIXED(ScreenInfo->position.x + ScreenInfo->size.x - 32)) {
            self->velocity.x = -self->velocity.x;
        }
        else if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, TO_FIXED(24), 0, true)) {
            self->velocity.x = -self->velocity.x;
        }
    }
    else {
        if (self->position.x < TO_FIXED(ScreenInfo->position.x + 32)) {
            self->velocity.x = -self->velocity.x;
        }
        else if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -TO_FIXED(24), 0, true)) {
            self->velocity.x = -self->velocity.x;
        }
    }

    self->velocity.y += 0xC00;
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, TO_FIXED(24), true)) {
        foreach_active(ItemBox, itemBox)
        {
            if (itemBox->hidden) {
                if (RSDK.CheckObjectCollisionTouchBox(itemBox, &ItemBox->hitboxHidden, self, &SignPost->hitboxItemBox)) {
                    RSDK.PlaySfx(SignPost->sfxBubbleBounce, false, 255);
                    itemBox->velocity.y = -TO_FIXED(5);
                    itemBox->hidden     = false;
                    itemBox->state      = ItemBox_State_Falling;
                    self->itemBounceCount++;
                    self->velocity.y = -TO_FIXED(2);
                    if (self->itemBounceCount == 2)
                        API_UnlockAchievement(&achievementList[ACH_SIGNPOST]);
                }
            }
        }

        if (self->velocity.y >= 0) {
            RSDK.PlaySfx(SignPost->sfxSlide, false, 255);
            self->spinCount  = 4;
            self->velocity.y = 0;
            Music_FadeOut(0.025);
            self->state = SignPost_State_Spin;
        }
    }
}
void SignPost_State_Done(void)
{
    RSDK_THIS(SignPost);

    RSDK.ProcessAnimation(&self->facePlateAnimator);

    if (self->spawnedByDebugMode) {
        Zone->stageFinishCallback  = StateMachine_None;
        globals->atlEnabled        = false;
        globals->enableIntro       = false;
        globals->suppressTitlecard = false;
        globals->suppressAutoMusic = false;
    }
}

#if GAME_INCLUDE_EDITOR
void SignPost_EditorDraw(void)
{
    RSDK_THIS(SignPost);
    Vector2 drawPos;

    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_EGGMAN, &self->eggPlateAnimator, true, 0);
    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &self->postTopAnimator, true, 0);
    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &self->sidebarAnimator, true, 1);
    RSDK.SetSpriteAnimation(SignPost->aniFrames, SIGNPOSTANI_POST, &self->standAnimator, true, 2);

    self->drawFX = FX_SCALE;
    drawPos.y    = self->position.y;

    self->scale.x = abs(RSDK.Cos512(self->rotation));
    int32 scale   = abs(RSDK.Sin512(self->rotation));

    switch (self->rotation >> 7) {
        case 0:
        case 2:
            drawPos.x = self->position.x + (scale << 9);
            RSDK.DrawSprite(&self->eggPlateAnimator, &drawPos, false);
            drawPos.x += -0xC00 * self->scale.x - (scale << 9);
            break;

        case 1:
        case 3:
            drawPos.x = self->position.x - (scale << 9);
            RSDK.DrawSprite(&self->eggPlateAnimator, &drawPos, false);
            drawPos.x += (scale + 2 * (3 * self->scale.x - 32)) << 9;
            break;
        default: break;
    }
    self->scale.x = scale;
    RSDK.DrawSprite(&self->sidebarAnimator, &drawPos, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->postTopAnimator, NULL, false);
    RSDK.DrawSprite(&self->standAnimator, NULL, false);

    self->updateRange.x = TO_FIXED(64);
    self->updateRange.y = TO_FIXED(64);
    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 left   = self->vsBoundsOffset.x - (self->vsBoundsSize.x >> 1);
        int32 top    = self->vsBoundsOffset.y - (self->vsBoundsSize.y >> 1);
        int32 right  = self->vsBoundsOffset.x + (self->vsBoundsSize.x >> 1);
        int32 bottom = self->vsBoundsOffset.y + (self->vsBoundsSize.y >> 1);

        int32 extendTop    = -TO_FIXED(self->vsExtendTop);
        int32 extendBottom = TO_FIXED(self->vsExtendBottom);

        if (extendTop < top)
            top = extendTop;

        if (extendBottom < bottom)
            bottom = extendBottom;

        if (abs(left) > right)
            self->updateRange.x = abs(left);
        else
            self->updateRange.x = right;

        if (abs(top) > bottom)
            self->updateRange.y = abs(top);
        else
            self->updateRange.y = bottom;

        self->updateRange.x += TO_FIXED(64);
        self->updateRange.y += TO_FIXED(64);

        DrawHelpers_DrawArenaBounds(FROM_FIXED(left), FROM_FIXED(top), FROM_FIXED(right + 64), FROM_FIXED(bottom + 64), 1 | 2 | 4 | 8, 0xFFFF00);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SignPost_EditorLoad(void)
{
    SignPost->aniFrames = RSDK.LoadSpriteAnimation("Global/SignPost.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SignPost, type);
    RSDK_ENUM_VAR("Run Past", SIGNPOST_RUNPAST);
    RSDK_ENUM_VAR("Drop", SIGNPOST_DROP);
    RSDK_ENUM_VAR("Competition", SIGNPOST_COMP);
}
#endif

void SignPost_Serialize(void)
{
    RSDK_EDITABLE_VAR(SignPost, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(SignPost, VAR_VECTOR2, vsBoundsSize);
    RSDK_EDITABLE_VAR(SignPost, VAR_VECTOR2, vsBoundsOffset);
    RSDK_EDITABLE_VAR(SignPost, VAR_ENUM, vsExtendTop);
    RSDK_EDITABLE_VAR(SignPost, VAR_ENUM, vsExtendBottom);
}
