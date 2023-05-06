// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ScrewMobile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectScrewMobile *ScrewMobile;

void ScrewMobile_Update(void)
{
    RSDK_THIS(ScrewMobile);

    StateMachine_Run(self->state);
}

void ScrewMobile_LateUpdate(void) {}

void ScrewMobile_StaticUpdate(void)
{
    foreach_active(ScrewMobile, screwMobile)
    {
        if (screwMobile->state != ScrewMobile_StateDepthCharge_Active) {
            RSDK.AddDrawListRef(Zone->playerDrawGroup[0] - 1, RSDK.GetEntitySlot(screwMobile));
        }
    }

    if (ScrewMobile->fanSfxTimer) {
        if (!ScrewMobile->playingFanSfx) {
            RSDK.PlaySfx(ScrewMobile->sfxBigFan, 47208, 0xFF);
            ScrewMobile->playingFanSfx = true;
        }
    }
    else if (ScrewMobile->playingFanSfx) {
        RSDK.StopSfx(ScrewMobile->sfxBigFan);
        ScrewMobile->playingFanSfx = false;
    }

    ScrewMobile->fanSfxTimer = 0;
}

void ScrewMobile_Draw(void)
{
    RSDK_THIS(ScrewMobile);

    StateMachine_Run(self->stateDraw);
}

void ScrewMobile_Create(void *data)
{
    RSDK_THIS(ScrewMobile);

    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(self);
    }
    else {
        self->drawFX = FX_FLIP;
        if (!SceneInfo->inEditor) {
            self->visible = true;

            if (data) {
                switch (VOID_TO_INT(data)) {
                    default: break;

                    case 1:
                        self->active          = ACTIVE_NORMAL;
                        self->drawGroup       = Zone->hudDrawGroup - 2;
                        self->updateRange.x   = 0x800000;
                        self->updateRange.y   = 0x800000;
                        self->whirlpoolHeight = 0x3800;
                        RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 6, &self->mobileAnimator, true, 0);
                        self->state     = ScrewMobile_StateDepthCharge_Active;
                        self->stateDraw = ScrewMobile_Draw_DepthCharge;
                        break;
                }
            }
            else {
                self->active        = ACTIVE_BOUNDS;
                self->drawGroup     = Zone->hudDrawGroup - 1;
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                self->startPos      = self->position;

                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 0, &self->mobileAnimator, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 1, &self->propellerAnimator, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &self->rackAnimator, true, 6);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 2, &self->whirlpoolAnimator, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 3, &self->whirlpoolTopAnimator, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 5, &self->whirlpoolBottomAnimator, true, 0);

                // ???
                // The HCZ1 boss health is managed by DiveEggman... so what was this for??
                self->health                  = 8;
                self->propellerAnimator.speed = 0;
                self->state                   = ScrewMobile_State_CheckPlayerEnter;
                self->stateDraw               = ScrewMobile_Draw_ScrewMobile;
            }
        }
    }
}

void ScrewMobile_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HCZ"))
        ScrewMobile->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ScrewMobile.bin", SCOPE_STAGE);

    ScrewMobile->hitboxCockpit.left   = -32;
    ScrewMobile->hitboxCockpit.top    = -20;
    ScrewMobile->hitboxCockpit.right  = 32;
    ScrewMobile->hitboxCockpit.bottom = 20;

    ScrewMobile->hitboxDepthCharge.left   = -8;
    ScrewMobile->hitboxDepthCharge.top    = -4;
    ScrewMobile->hitboxDepthCharge.right  = 8;
    ScrewMobile->hitboxDepthCharge.bottom = 4;

    ScrewMobile->playingFanSfx = false;
    ScrewMobile->fanSfxTimer   = 0;
    ScrewMobile->active        = ACTIVE_ALWAYS;

    ScrewMobile->sfxButton    = RSDK.GetSfx("Stage/Button2.wav");
    ScrewMobile->sfxEggMobile = RSDK.GetSfx("HCZ/EggMobile.wav");
    ScrewMobile->sfxBigFan    = RSDK.GetSfx("HCZ/BigFan.wav");
    ScrewMobile->sfxImpact    = RSDK.GetSfx("Stage/Impact5.wav");
}

void ScrewMobile_State_CheckPlayerEnter(void)
{
    RSDK_THIS(ScrewMobile);

    Zone->cameraBoundsR[0]      = (self->position.x >> 16) + 32;
    Zone->playerBoundsR[0]      = Zone->cameraBoundsR[0] << 16;
    Zone->playerBoundActiveR[0] = true;

    if (Player->playerCount > 1) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

        if (Player_CheckCollisionTouch(player2, self, &ScrewMobile->hitboxCockpit)) {
            player2->direction      = FLIP_X;
            player2->state          = Player_State_Static;
            player2->velocity.x     = 0;
            player2->velocity.y     = 0;
            player2->groundVel      = 0;
            player2->position.x     = self->position.x;
            player2->position.y     = self->position.y - 0x100000;
            player2->tileCollisions = TILECOLLISION_DOWN;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (Player_CheckCollisionTouch(player1, self, &ScrewMobile->hitboxCockpit)) {
        Zone->cameraBoundsT[0]      = (self->position.y >> 16) - 40;
        Zone->playerBoundsR[0]      = 0x7FFF;
        Zone->cameraBoundsR[0]      = 0x7FFF;
        Zone->playerBoundActiveR[0] = false;

        player1->state          = Player_State_Static;
        player1->velocity.x     = 0;
        player1->velocity.y     = 0;
        player1->groundVel      = 0;
        player1->direction      = FLIP_X;
        player1->position.x     = self->position.x;
        player1->position.y     = self->position.y - 0x100000;
        player1->tileCollisions = TILECOLLISION_DOWN;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        RSDK.PlaySfx(ScrewMobile->sfxEggMobile, false, 255);
        DCEvent->canExplodeBombs = true;
        self->state              = ScrewMobile_State_PlayerRiding;
    }
}

void ScrewMobile_State_PlayerRiding(void)
{
    RSDK_THIS(ScrewMobile);

    RSDK.ProcessAnimation(&self->propellerAnimator);
    RSDK.ProcessAnimation(&self->rackAnimator);
    RSDK.ProcessAnimation(&self->whirlpoolAnimator);
    RSDK.ProcessAnimation(&self->whirlpoolTopAnimator);
    RSDK.ProcessAnimation(&self->whirlpoolBottomAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->startPos.y, 2, 10);
    self->position.x += self->velocity.x;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (Player_CheckValidState(player1)) {
        player1->position.x     = self->position.x;
        player1->position.y     = self->position.y - 0x100000;
        player1->velocity.x     = 0;
        player1->velocity.y     = 0;
        player1->outtaHereTimer = 0;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);

        if ((player1->left || player1->right) && !player1->jumpHold) {
            if (player1->left) {
                self->velocity.x -= 0xC00;

                if (self->velocity.x > 0)
                    self->velocity.x -= 0x1200;
                else if (self->velocity.x < -0x20000)
                    self->velocity.x = -0x20000;
            }
            else if (player1->right) {
                self->velocity.x += 0xC00;

                if (self->velocity.x < 0)
                    self->velocity.x += 0x1200;
                else if (self->velocity.x > 0x20000)
                    self->velocity.x = 0x20000;
            }
        }
        else {
            if (self->velocity.x) {
                if (self->velocity.x >= 0) {
                    self->velocity.x -= 0xC00;

                    if (self->velocity.x < 0)
                        self->velocity.x = 0;
                }
                else {
                    self->velocity.x += 0xC00;

                    if (self->velocity.x > 0)
                        self->velocity.x = 0;
                }
            }
        }

        if (self->velocity.x) {
            if (self->velocity.x <= 0) {
                self->direction = FLIP_NONE;

                if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
                    if (self->velocity.x <= -0x20000)
                        RSDK.PlaySfx(ScrewMobile->sfxImpact, false, 0xFF);

                    self->velocity.x = 0;
                }
            }
            else {
                self->direction = FLIP_X;
                if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x200000, 0, true)) {
                    if (self->velocity.x >= 0x20000)
                        RSDK.PlaySfx(ScrewMobile->sfxImpact, false, 0xFF);

                    self->velocity.x = 0;
                }
            }
        }

        player1->direction = self->direction ^ FLIP_X;
    }

    if (Player->playerCount >= 2) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

        if (Player_CheckValidState(player2)) {
            if (player2->state == Player_State_Static) {
                player2->position.x     = self->position.x;
                player2->position.y     = self->position.y - 0x100000;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->outtaHereTimer = 0;
                player2->direction      = self->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }
            else if (Player_CheckCollisionTouch(player2, self, &ScrewMobile->hitboxCockpit)) {
                player2->state          = Player_State_Static;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->groundVel      = 0;
                player2->direction      = FLIP_X;
                player2->position.x     = self->position.x;
                player2->position.y     = self->position.y - 0x100000;
                player2->drawGroup      = Zone->playerDrawGroup[0];
                player2->tileCollisions = TILECOLLISION_DOWN;
                player2->direction      = self->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }
        }
    }

    if (Player_CheckValidState(player1)) {
        if (player1->jumpPress) {
            if (!Zone->cameraBoundsT[0]) {
                Player_Action_Jump(player1);
                self->state = ScrewMobile_State_Idle;
                return;
            }

            if (!self->whirlPoolTimer && ScrewMobile->playingFanSfx == false) {
                self->propellerAnimator.speed = 64;
                RSDK.PlaySfx(ScrewMobile->sfxButton, false, 255);
            }
        }
        if (!player1->jumpHold || self->whirlPoolTimer) {
            EntityWhirlpool *whirlpool = self->whirlpool;
            if (whirlpool) {
                if (whirlpool->classID == Whirlpool->classID)
                    whirlpool->activePlayers = -3;

                self->whirlpool = NULL;
            }

            if (self->propellerAnimator.speed)
                self->propellerAnimator.speed -= 2;

            if (self->propellerAnimator.speed > 0xC0)
                ++ScrewMobile->fanSfxTimer;
        }
        else {
            if (self->whirlpoolHeight > 0) {
                EntityWhirlpool *whirlpool = self->whirlpool;

                if (whirlpool) {
                    if (whirlpool->classID == Whirlpool->classID)
                        whirlpool->position.x = self->position.x;
                    else
                        self->whirlpool = NULL;
                }
                else {
                    Vector2 pos;
                    pos.x                    = 0x1000000;
                    pos.y                    = 0x1000000;
                    whirlpool                = CREATE_ENTITY(Whirlpool, &pos, self->position.x, self->startPos.y + 0xF00000);
                    whirlpool->activePlayers = -1;
                    whirlpool->angVel        = 10;
                    whirlpool->alpha         = 0;
                    whirlpool->isPermanent   = true;
                    self->whirlpool          = whirlpool;
                }
            }

            if (self->propellerAnimator.speed < 0x100)
                self->propellerAnimator.speed += 2;

            if (self->propellerAnimator.speed > 0x40)
                ++ScrewMobile->fanSfxTimer;
        }
    }

    if (self->propellerAnimator.speed < 0xF0) {
        if (self->whirlpoolHeight > 0)
            self->whirlpoolHeight--;
    }
    else {
        if (self->whirlpoolHeight < 86)
            self->whirlpoolHeight++;
    }

    if (player1->down) {
        if (!self->bombPress && !self->timer && self->rackAnimator.frameID == 6) {
            RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &self->rackAnimator, true, 0);
            self->bombPress = true;
        }
    }
    else {
        self->bombPress = false;
    }

    if (self->rackAnimator.frameID == 2 && !self->timer) {
        self->timer = 60;

        EntityScrewMobile *bomb = CREATE_ENTITY(ScrewMobile, INT_TO_VOID(SCREWMOBILE_BOMB), self->position.x, self->position.y + 0x130000);
        if (self->direction)
            bomb->position.x -= 0x350000;
        else
            bomb->position.x += 0x350000;
        bomb->direction  = self->direction;
        bomb->velocity.y = -0x7000;
    }

    if (self->whirlPoolTimer > (player1->jumpHold == true))
        self->whirlPoolTimer--;

    if (self->timer)
        self->timer--;

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;
}

void ScrewMobile_State_BossFinished(void)
{
    RSDK_THIS(ScrewMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->up           = false;
    player1->down         = false;
    player1->left         = false;
    player1->right        = true;
    player1->jumpPress    = false;
    player1->jumpHold     = false;

    ScrewMobile_State_PlayerRiding();

    Zone->cameraBoundsL[0] = ScreenInfo->position.x;
    Zone->playerBoundsL[0] = Zone->cameraBoundsL[0] << 16;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x200000, 0, true)) {
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        Zone->cameraBoundsL[0] += 2;
        ScreenInfo->position.x = Zone->cameraBoundsL[0];
        Zone->playerBoundsL[0] = Zone->cameraBoundsL[0] << 16;
        camera->position.x     = Zone->cameraBoundsL[0] << 16;

        if (Zone->cameraBoundsL[0] >= (self->position.x >> 16) - 32)
            self->state = ScrewMobile_State_Idle;
    }
}

void ScrewMobile_State_Idle(void)
{
    RSDK_THIS(ScrewMobile);

    self->position.y = BadnikHelpers_Oscillate(self->startPos.y, 2, 10);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->state == Player_State_Static) {
        if (player1->jumpPress) {
            Player_Action_Jump(player1);
        }
        else {
            player1->position.x = self->position.x;
            player1->position.y = self->position.y;
            player1->position.y -= 0x100000;
            player1->velocity.x     = 0;
            player1->velocity.y     = 0;
            player1->outtaHereTimer = 0;
            player1->direction      = self->direction ^ FLIP_X;
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);
        }
    }
    else if (player1->velocity.y >= 0 && Player_CheckCollisionTouch(player1, self, &ScrewMobile->hitboxCockpit)) {
        player1->state      = Player_State_Static;
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        player1->groundVel  = 0;
        player1->direction  = FLIP_X;
        player1->position.x = self->position.x;
        player1->position.y = self->position.y;
        player1->position.y -= 0x100000;
        player1->drawGroup      = Zone->playerDrawGroup[0];
        player1->tileCollisions = TILECOLLISION_DOWN;
        player1->direction      = self->direction ^ FLIP_X;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);
    }

    if (Player->playerCount >= 2) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

        if (player2->state == Player_State_Static) {
            if (player2->jumpPress) {
                Player_Action_Jump(player2);
            }
            else {
                player2->position.x = self->position.x;
                player2->position.y = self->position.y;
                player2->position.y -= 0x100000;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->outtaHereTimer = 0;
                player2->direction      = self->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }
        }
        else if (player2->velocity.y >= 0) {
            if (Player_CheckCollisionTouch(player2, self, &ScrewMobile->hitboxCockpit)) {
                player2->state      = Player_State_Static;
                player2->velocity.x = 0;
                player2->velocity.y = 0;
                player2->groundVel  = 0;
                player2->direction  = FLIP_X;
                player2->position.x = self->position.x;
                player2->position.y = self->position.y;
                player2->position.y -= 0x100000;
                player2->drawGroup      = Zone->playerDrawGroup[0];
                player2->tileCollisions = TILECOLLISION_DOWN;
                player2->direction      = self->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }
        }
    }
}

void ScrewMobile_Draw_ScrewMobile(void)
{
    RSDK_THIS(ScrewMobile);

    if (SceneInfo->currentDrawGroup == Zone->playerDrawGroup[0] - 1) {
        self->mobileAnimator.frameID = 1;
        RSDK.DrawSprite(&self->mobileAnimator, NULL, false);
    }
    else {
        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

        self->mobileAnimator.frameID = 0;
        RSDK.DrawSprite(&self->mobileAnimator, NULL, false);
        RSDK.SetPaletteEntry(0, 128, 0x0000);
        RSDK.DrawSprite(&self->rackAnimator, NULL, false);

        int32 storeDir = self->direction;

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->propellerAnimator, NULL, false);

        if (self->propellerAnimator.speed >= 0x40) {
            Vector2 drawPos = self->position;

            self->inkEffect = INK_ALPHA;
            if (self->whirlpoolHeight <= 0) {
                self->alpha = self->propellerAnimator.speed;
            }
            else {
                self->alpha = 0xD0;
                drawPos.y   = Water->waterLevel - 0x100000;

                SpriteFrame *frame = RSDK.GetFrame(ScrewMobile->aniFrames, 2, self->whirlpoolAnimator.frameID);

                frame->height = 32;
                frame->sprY   = 33 * self->whirlpoolAnimator.frameID + 181;
                frame->pivotY = -16;

                int32 count = self->whirlpoolHeight >> 5;
                for (int32 i = 0; i < count; ++i) {
                    RSDK.DrawSprite(&self->whirlpoolAnimator, &drawPos, false);
                    drawPos.y -= 0x200000;
                }

                frame->height = self->whirlpoolHeight % 32;
                frame->sprY += 31 - frame->height;
                frame->pivotY = 16 - frame->height;
                RSDK.DrawSprite(&self->whirlpoolAnimator, &drawPos, false);
                drawPos.y = Water->waterLevel;
                RSDK.DrawSprite(&self->whirlpoolBottomAnimator, &drawPos, false);
                self->alpha = 0x100;
            }

            drawPos.y = Water->waterLevel - (self->whirlpoolHeight << 16);
            RSDK.DrawSprite(&self->whirlpoolTopAnimator, &drawPos, false);
            self->inkEffect = INK_NONE;
        }

        self->direction = storeDir;
    }
}

void ScrewMobile_StateDepthCharge_Active(void)
{
    RSDK_THIS(ScrewMobile);

    if (RSDK.CheckOnScreen(self, NULL)) {
        self->position.y += self->velocity.y;

        if (self->whirlpoolHeight == 0x3800 && self->position.y >= Water->waterLevel) {
            self->velocity.y      = self->velocity.y >> 2;
            self->whirlpoolHeight = 0;
            CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, Water->waterLevel);
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
        }

        self->velocity.y += self->whirlpoolHeight; // this is "gravityStrength" here

        foreach_active(DiveEggman, eggman)
        {
            if (eggman->state == DiveEggman_StateBomb_Idle || eggman->state == DiveEggman_StateBomb_InWhirlpool
                || eggman->state == DiveEggman_StateBomb_WhirlpoolRise || eggman->state == DiveEggman_StateBomb_Falling) {
                if (RSDK.CheckObjectCollisionTouchBox(self, &ScrewMobile->hitboxDepthCharge, eggman, &DiveEggman->hitboxBomb)) {
                    CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), eggman->position.x, eggman->position.y)->drawGroup =
                        Zone->objectDrawGroup[1];
                    RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);

                    EntityWater *water = CREATE_ENTITY(Water, INT_TO_VOID(WATER_BUBBLE), eggman->position.x, eggman->position.y);
                    water->velocity.y  = -0x8800;
                    water->angle       = 2 * RSDK.Rand(0, 256);
                    water->bubbleX     = water->position.x;
                    water->childPtr    = NULL;
                    RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, true, 0);

                    destroyEntity(eggman);
                    destroyEntity(self);
                    foreach_break;
                }
            }
            else if (RSDK.CheckObjectCollisionTouchBox(self, &ScrewMobile->hitboxDepthCharge, eggman, &DiveEggman->hitboxEggman)) {
                RSDK.PlaySfx(DiveEggman->sfxRockemSockem, false, 255);

                self->velocity.x = RSDK.Rand(-4, 5) << 15;
                self->velocity.y = -0x20000;
                self->state      = ScrewMobile_StateDepthCharge_Debris;
                foreach_break;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void ScrewMobile_StateDepthCharge_Debris(void)
{
    RSDK_THIS(ScrewMobile);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;
    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void ScrewMobile_Draw_DepthCharge(void)
{
    RSDK_THIS(ScrewMobile);

    RSDK.DrawSprite(&self->mobileAnimator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void ScrewMobile_EditorDraw(void)
{
    RSDK_THIS(ScrewMobile);

    self->startPos = self->position;
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 0, &self->mobileAnimator, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 1, &self->propellerAnimator, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &self->rackAnimator, true, 6);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 2, &self->whirlpoolAnimator, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 3, &self->whirlpoolTopAnimator, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 5, &self->whirlpoolBottomAnimator, true, 0);
    self->propellerAnimator.speed = 0;

    ScrewMobile_Draw_ScrewMobile();
}

void ScrewMobile_EditorLoad(void) { ScrewMobile->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ScrewMobile.bin", SCOPE_STAGE); }
#endif

void ScrewMobile_Serialize(void) {}
