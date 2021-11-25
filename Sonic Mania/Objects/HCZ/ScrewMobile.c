#include "SonicMania.h"

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
        if (screwMobile->state != ScrewMobile_State2_Unknown1) {
            RSDK.AddDrawListRef(Zone->playerDrawLow - 1, RSDK.GetEntityID(screwMobile));
        }
    }

    if (ScrewMobile->shouldPlayFanSfx) {
        if (!ScrewMobile->playingFanSfx) {
            RSDK.PlaySfx(ScrewMobile->sfxBigFan, 47208, 255);
            ScrewMobile->playingFanSfx = true;
        }
    }
    else if (ScrewMobile->playingFanSfx) {
        RSDK.StopSfx(ScrewMobile->sfxBigFan);
        ScrewMobile->playingFanSfx = false;
    }
    ScrewMobile->shouldPlayFanSfx = 0;
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
                if (voidToInt(data) == 1) {
                    self->active        = ACTIVE_NORMAL;
                    self->drawOrder     = Zone->hudDrawOrder - 2;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->field_74      = 0x3800;
                    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 6, &self->animator1, true, 0);
                    self->state     = ScrewMobile_State2_Unknown1;
                    self->stateDraw = ScrewMobile_StateDraw2_Unknown;
                }
            }
            else {
                self->active        = ACTIVE_BOUNDS;
                self->drawOrder     = Zone->hudDrawOrder - 1;
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                self->startPos      = self->position;
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 1, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &self->animator3, true, 6);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 2, &self->animator4, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 3, &self->animator5, true, 0);
                RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 5, &self->animator6, true, 0);
                self->field_7C                 = 8;
                self->animator2.speed = 0;
                self->state                    = ScrewMobile_State_Unknown1;
                self->stateDraw                = ScrewMobile_StateDraw1_Unknown;
            }
        }
    }
}

void ScrewMobile_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        ScrewMobile->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ScrewMobile.bin", SCOPE_STAGE);

    ScrewMobile->hitbox1.left   = -32;
    ScrewMobile->hitbox1.top    = -20;
    ScrewMobile->hitbox1.right  = 32;
    ScrewMobile->hitbox1.bottom = 20;

    ScrewMobile->hitbox2.left   = -8;
    ScrewMobile->hitbox2.top    = -4;
    ScrewMobile->hitbox2.right  = 8;
    ScrewMobile->hitbox2.bottom = 4;

    ScrewMobile->playingFanSfx    = false;
    ScrewMobile->shouldPlayFanSfx = 0;
    ScrewMobile->active           = ACTIVE_ALWAYS;
    ScrewMobile->sfxButton        = RSDK.GetSfx("Stage/Button2.wav");
    ScrewMobile->sfxEggMobile     = RSDK.GetSfx("HCZ/EggMobile.wav");
    ScrewMobile->sfxBigFan        = RSDK.GetSfx("HCZ/BigFan.wav");
    ScrewMobile->sfxImpact        = RSDK.GetSfx("Stage/Impact5.wav");
}

void ScrewMobile_State_Unknown1(void)
{
    RSDK_THIS(ScrewMobile);

    Zone->cameraBoundsR[0]     = (self->position.x >> 16) + 32;
    Zone->playerBoundsR[0]     = Zone->cameraBoundsR[0] << 16;
    Zone->playerBoundActiveR[0] = true;

    if (Player->playerCount > 1) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (Player_CheckCollisionTouch(player2, self, &ScrewMobile->hitbox1)) {
            player2->direction      = FLIP_X;
            player2->state          = Player_State_None;
            player2->velocity.x     = 0;
            player2->velocity.y     = 0;
            player2->groundVel      = 0;
            player2->position.x     = self->position.x;
            player2->position.y     = self->position.y - 0x100000;
            player2->tileCollisions = true;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (Player_CheckCollisionTouch(player1, self, &ScrewMobile->hitbox1)) {
        Zone->cameraBoundsT[0]     = (self->position.y >> 16) - 40;
        Zone->playerBoundsR[0]     = 0x7FFF;
        Zone->cameraBoundsR[0]     = 0x7FFF;
        Zone->playerBoundActiveR[0] = false;
        player1->state              = Player_State_None;
        player1->velocity.x         = 0;
        player1->velocity.y         = 0;
        player1->groundVel          = 0;
        player1->direction          = FLIP_X;
        player1->position.x         = self->position.x;
        player1->position.y         = self->position.y - 0x100000;
        player1->tileCollisions     = true;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        RSDK.PlaySfx(ScrewMobile->sfxEggMobile, false, 255);
        DCEvent->finished = true;
        self->state     = ScrewMobile_State_Unknown2;
    }
}

void ScrewMobile_State_Unknown2(void)
{
    RSDK_THIS(ScrewMobile);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator5);
    RSDK.ProcessAnimation(&self->animator6);

    self->position.y = BadnikHelpers_Oscillate(self->startPos.y, 2, 10);
    self->position.x += self->velocity.x;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (Player_CheckValidState(player1)) {
        player1->position.x = self->position.x;
        player1->position.y = self->position.y;
        player1->position.y -= 0x100000;
        player1->velocity.x     = 0;
        player1->velocity.y     = 0;
        player1->outtaHereTimer = 0;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);

        if (player1->left || player1->right) {
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
                if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
                    if (self->velocity.x <= -0x20000)
                        RSDK.PlaySfx(ScrewMobile->sfxImpact, false, 255);
                    self->velocity.x = 0;
                }
            }
            else {
                self->direction = FLIP_X;
                if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)) {
                    if (self->velocity.x >= 0x20000)
                        RSDK.PlaySfx(ScrewMobile->sfxImpact, false, 255);
                    self->velocity.x = 0;
                }
            }
        }

        player1->direction = self->direction ^ FLIP_X;
    }

    if (Player->playerCount >= 2) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (Player_CheckValidState(player2)) {
            if (player2->state == Player_State_None) {
                player2->position.x     = self->position.x;
                player2->position.y     = self->position.y - 0x100000;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->outtaHereTimer = 0;
                player2->direction      = self->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }
            else if (Player_CheckCollisionTouch(player2, self, &ScrewMobile->hitbox1)) {
                player2->state          = Player_State_None;
                player2->velocity.x     = 0;
                player2->velocity.y     = 0;
                player2->groundVel      = 0;
                player2->direction      = FLIP_X;
                player2->position.x     = self->position.x;
                player2->position.y     = self->position.y - 0x100000;
                player2->drawOrder      = Zone->playerDrawLow;
                player2->tileCollisions = true;
                player2->direction      = self->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }
        }
    }

    if (Player_CheckValidState(player1)) {
        if (player1->jumpPress) {
            if (!Zone->cameraBoundsT[0]) {
                Player_StartJump(player1);
                self->state = ScrewMobile_State_Idle;
                return;
            }
            if (!self->whirlPoolTimer && ScrewMobile->playingFanSfx == false) {
                self->animator2.speed = 64;
                RSDK.PlaySfx(ScrewMobile->sfxButton, false, 255);
            }
        }
        if (!player1->jumpHold || self->whirlPoolTimer) {
            EntityWhirlpool *whirlpool = (EntityWhirlpool *)self->whirlpool;
            if (whirlpool) {
                if (whirlpool->objectID == Whirlpool->objectID)
                    whirlpool->activePlayers = -3;
                self->whirlpool = NULL;
            }

            if (self->animator2.speed)
                self->animator2.speed -= 2;

            if (self->animator2.speed > 0xC0)
                ++ScrewMobile->shouldPlayFanSfx;
        }
        else {
            if (self->field_74 > 0) {
                EntityWhirlpool *whirlpool = (EntityWhirlpool *)self->whirlpool;
                if (whirlpool) {
                    if (whirlpool->objectID == Whirlpool->objectID)
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
                    self->whirlpool        = (Entity *)whirlpool;
                }
            }

            if (self->animator2.speed < 0x100)
                self->animator2.speed += 2;

            if (self->animator2.speed > 0x40)
                ++ScrewMobile->shouldPlayFanSfx;
        }
    }

    if (self->animator2.speed < 0xF0) {
        if (self->field_74 > 0) {
            self->field_74--;
        }
    }
    else if (self->field_74 < 86) {
        self->field_74++;
    }

    if (player1->down) {
        if (!self->bombPress && !self->timer && self->animator3.frameID == 6) {
            RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &self->animator3, true, 0);
            self->bombPress = true;
        }
    }
    else {
        self->bombPress = false;
    }

    if (self->animator3.frameID == 2 && !self->timer) {
        self->timer            = 60;
        EntityScrewMobile *child = CREATE_ENTITY(ScrewMobile, intToVoid(1), self->position.x, self->position.y + 0x130000);
        if (self->direction)
            child->position.x -= 0x350000;
        else
            child->position.x += 0x350000;
        child->direction  = self->direction;
        child->velocity.y = -0x7000;
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
    ScrewMobile_State_Unknown2();
    Zone->cameraBoundsL[0] = ScreenInfo->position.x;
    Zone->playerBoundsL[0] = Zone->cameraBoundsL[0] << 16;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)) {
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        Zone->cameraBoundsL[0] += 2;
        ScreenInfo->position.x = Zone->cameraBoundsL[0];
        Zone->playerBoundsL[0]  = Zone->cameraBoundsL[0] << 16;
        camera->position.x       = Zone->cameraBoundsL[0] << 16;
        if (Zone->cameraBoundsL[0] >= (self->position.x >> 16) - 32)
            self->state = ScrewMobile_State_Idle;
    }
}

void ScrewMobile_State_Idle(void)
{
    RSDK_THIS(ScrewMobile);

    self->position.y = BadnikHelpers_Oscillate(self->startPos.y, 2, 10);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->state == Player_State_None) {
        if (player1->jumpPress) {
            Player_StartJump(player1);
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
    else if (player1->velocity.y >= 0 && Player_CheckCollisionTouch(player1, self, &ScrewMobile->hitbox1)) {
        player1->state      = Player_State_None;
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        player1->groundVel  = 0;
        player1->direction  = FLIP_X;
        player1->position.x = self->position.x;
        player1->position.y = self->position.y;
        player1->position.y -= 0x100000;
        player1->drawOrder      = Zone->playerDrawLow;
        player1->tileCollisions = true;
        player1->direction      = self->direction ^ FLIP_X;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);
    }

    if (Player->playerCount >= 2) {
        EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
        if (player2->state == Player_State_None) {
            if (player2->jumpPress) {
                Player_StartJump(player2);
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
            if (Player_CheckCollisionTouch(player2, self, &ScrewMobile->hitbox1)) {
                player2->state      = Player_State_None;
                player2->velocity.x = 0;
                player2->velocity.y = 0;
                player2->groundVel  = 0;
                player2->direction  = FLIP_X;
                player2->position.x = self->position.x;
                player2->position.y = self->position.y;
                player2->position.y -= 0x100000;
                player2->drawOrder      = Zone->playerDrawLow;
                player2->tileCollisions = true;
                player2->direction      = self->direction ^ FLIP_X;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }
        }
    }
}

void ScrewMobile_StateDraw1_Unknown(void)
{
    RSDK_THIS(ScrewMobile);
    Vector2 drawPos;

    if (SceneInfo->currentDrawGroup == Zone->playerDrawLow - 1) {
        self->animator1.frameID = 1;
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
    else {
        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

        self->animator1.frameID = 0;
        RSDK.DrawSprite(&self->animator1, NULL, false);
        RSDK.SetPaletteEntry(0, 128, 0x0000);
        RSDK.DrawSprite(&self->animator3, NULL, false);

        int storeDir = self->direction;

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        if (self->animator2.speed >= 0x40) {
            drawPos.x         = self->position.x;
            drawPos.y         = self->position.y;
            self->inkEffect = INK_ALPHA;
            if (self->field_74 <= 0) {
                self->alpha = self->animator2.speed;
            }
            else {
                self->alpha = 0xD0;
                drawPos.y     = Water->waterLevel - 0x100000;

                SpriteFrame *frame = RSDK.GetFrame(ScrewMobile->aniFrames, 2, self->animator4.frameID);

                frame->height = 32;
                frame->sprY   = 33 * self->animator4.frameID + 181;
                frame->pivotY = -16;

                int count = self->field_74 >> 5;
                for (int i = 0; i < count; ++i) {
                    RSDK.DrawSprite(&self->animator4, &drawPos, false);
                    drawPos.y -= 0x200000;
                }

                frame->height = self->field_74 % 32;
                frame->sprY += 31 - frame->height;
                frame->pivotY = 16 - frame->height;
                RSDK.DrawSprite(&self->animator4, &drawPos, false);
                drawPos.y = Water->waterLevel;
                RSDK.DrawSprite(&self->animator6, &drawPos, false);
                self->alpha = 0x100;
            }
            drawPos.y = Water->waterLevel - (self->field_74 << 16);
            RSDK.DrawSprite(&self->animator5, &drawPos, false);
            self->inkEffect = INK_NONE;
        }
        self->direction = storeDir;
    }
}

void ScrewMobile_State2_Unknown1(void)
{
    RSDK_THIS(ScrewMobile);

    if (RSDK.CheckOnScreen(self, NULL)) {
        self->position.y += self->velocity.y;
        if (self->field_74 == 0x3800 && self->position.y >= Water->waterLevel) {
            self->velocity.y = self->velocity.y >> 2;
            self->field_74   = 0;
            CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), self->position.x, Water->waterLevel);
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
        }
        self->velocity.y += self->field_74;

        foreach_active(DiveEggman, eggman)
        {
            if (eggman->state == DiveEggman_State2_Unknown1 || eggman->state == DiveEggman_State2_Unknown2 || eggman->state == DiveEggman_State2_Unknown3
                || eggman->state == DiveEggman_State2_Unknown4) {
                if (RSDK.CheckObjectCollisionTouchBox(self, &ScrewMobile->hitbox2, eggman, &DiveEggman->hitbox2)) {
                    CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), eggman->position.x, eggman->position.y)->drawOrder = Zone->drawOrderHigh;
                    RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);
                    EntityWater *water = CREATE_ENTITY(Water, intToVoid(WATER_BUBBLE), eggman->position.x, eggman->position.y);

                    water->velocity.y = -0x8800;
                    water->angle      = 2 * RSDK.Rand(0, 256);
                    water->bubbleX   = water->position.x;
                    water->childPtr   = 0;
                    RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, true, 0);
                    destroyEntity(eggman);
                    destroyEntity(self);
                    foreach_break;
                }
            }
            else if (RSDK.CheckObjectCollisionTouchBox(self, &ScrewMobile->hitbox2, eggman, &DiveEggman->hitbox3)) {
                RSDK.PlaySfx(DiveEggman->sfxRockemSockem, false, 255);
                self->velocity.x = RSDK.Rand(-4, 5) << 15;
                self->velocity.y = -0x20000;
                self->state      = ScrewMobile_State2_Unknown2;
                foreach_break;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void ScrewMobile_State2_Unknown2(void)
{
    RSDK_THIS(ScrewMobile);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;
    self->visible ^= FLIP_X;
    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void ScrewMobile_StateDraw2_Unknown(void)
{
    RSDK_THIS(ScrewMobile);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void ScrewMobile_EditorDraw(void)
{
    RSDK_THIS(ScrewMobile);

    self->startPos = self->position;
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 1, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 7, &self->animator3, true, 6);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 2, &self->animator4, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 3, &self->animator5, true, 0);
    RSDK.SetSpriteAnimation(ScrewMobile->aniFrames, 5, &self->animator6, true, 0);
    self->field_7C                 = 8;
    self->animator2.speed = 0;

    ScrewMobile_StateDraw1_Unknown();
}

void ScrewMobile_EditorLoad(void) { ScrewMobile->aniFrames = RSDK.LoadSpriteAnimation("HCZ/ScrewMobile.bin", SCOPE_STAGE); }
#endif

void ScrewMobile_Serialize(void) {}
