// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AmoebaDroid Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectAmoebaDroid *AmoebaDroid;

void AmoebaDroid_Update(void)
{
    RSDK_THIS(AmoebaDroid);
    StateMachine_Run(self->state);
}

void AmoebaDroid_LateUpdate(void) {}

void AmoebaDroid_StaticUpdate(void) {}

void AmoebaDroid_Draw(void)
{
    RSDK_THIS(AmoebaDroid);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

void AmoebaDroid_Create(void *data)
{
    RSDK_THIS(AmoebaDroid);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->type          = voidToInt(data);
            switch (self->type) {
                case AMOEBADROID_BOSS:
                    self->visible       = false;
                    self->drawOrder     = Zone->drawOrderLow;
                    self->drawFX        = FX_FLIP;
                    self->health        = 6;
                    self->hitbox.left   = -22;
                    self->hitbox.top    = -22;
                    self->hitbox.right  = 22;
                    self->hitbox.bottom = 22;
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 0, &self->animator, true, 0);
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 1, &self->attractorTopAnimator, true, 0);
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 2, &self->attractorSideAnimator, true, 0);
                    self->stateDraw = AmoebaDroid_Draw_AmoebaDroid;
                    self->state     = AmoebaDroid_State_SetupArena;
                    break;
                case AMOEBADROID_BLOB_BIG:
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderHigh;
                    self->hitbox.left   = -40;
                    self->hitbox.top    = -40;
                    self->hitbox.right  = 40;
                    self->hitbox.bottom = 40;
                    self->drawFX        = FX_SCALE;
                    self->inkEffect     = INK_ALPHA;
                    self->scale.y       = 0x200;
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 3, &self->animator, true, 0);
                    self->stateDraw = AmoebaDroid_Draw_BigBlob;
                    self->state     = AmoebaDroid_State_BigBlob;
                    break;
                case AMOEBADROID_BLOB_SMALL:
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderHigh;
                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;
                    self->updateRange.x = 0x200000;
                    self->updateRange.y = 0x200000;
                    self->inkEffect     = INK_ALPHA;
                    self->state         = AmoebaDroid_State_SmallBlob;
                    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 4, &self->animator, true, 0);
                    break;
                case AMOEBADROID_POOLSPLASH:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->state     = AmoebaDroid_State_PoolSplash;
                    self->drawOrder = Zone->hudDrawOrder - 2;
                    RSDK.SetSpriteAnimation(AmoebaDroid->waterFrames, 1, &self->animator, true, 0);
                    break;
                case AMOEBADROID_POOLSPLASH_DELAY:
                    self->active    = ACTIVE_NORMAL;
                    self->state     = AmoebaDroid_State_PoolSplash_Delayed;
                    self->drawOrder = Zone->hudDrawOrder - 2;
                    RSDK.SetSpriteAnimation(AmoebaDroid->waterFrames, 1, &self->animator, true, 0);
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void AmoebaDroid_StageLoad(void)
{
    AmoebaDroid->aniFrames    = RSDK.LoadSpriteAnimation("CPZ/AmoebaDroid.bin", SCOPE_STAGE);
    AmoebaDroid->waterFrames  = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    AmoebaDroid->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    AmoebaDroid->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    AmoebaDroid->sfxGather    = RSDK.GetSfx("CPZ/DroidGather.wav");
    AmoebaDroid->sfxBounce    = RSDK.GetSfx("CPZ/DroidBounce.wav");
    AmoebaDroid->sfxRelease   = RSDK.GetSfx("CPZ/DroidRelease.wav");
}

void AmoebaDroid_HandleSmallBlobMovement(void)
{
    RSDK_THIS(AmoebaDroid);
    int32 angle      = self->blobAngleX;
    self->blobRadius = (self->blobAmplitude * RSDK.Cos256(self->blobAngleY)) >> 8;
    for (int32 i = 0; i < 8; ++i) {
        EntityAmoebaDroid *smallBlob = (EntityAmoebaDroid *)self->blobs[i];

        smallBlob->velocity   = smallBlob->position;
        smallBlob->position.x = self->blobAmplitude * RSDK.Cos256(angle) + self->position.x;
        smallBlob->velocity.x = smallBlob->position.x - smallBlob->velocity.x;
        smallBlob->position.y = self->blobRadius * RSDK.Sin256(angle) + self->position.y;
        smallBlob->velocity.y = (self->blobRadius * RSDK.Sin256(angle) + self->position.y) - smallBlob->velocity.y;
        if ((self->blobAngleX & 0x7F) && angle < 0x80)
            smallBlob->drawOrder = Zone->drawOrderLow - 1;
        else
            smallBlob->drawOrder = Zone->drawOrderLow;
        angle = (angle + 32) & 0xFF;
    }
}

void AmoebaDroid_HandleSmallBlobRelease(bool32 interact)
{
    RSDK_THIS(AmoebaDroid);

    for (int32 i = 0; i < 8; ++i) {
        EntityAmoebaDroid *smallBlob = (EntityAmoebaDroid *)self->blobs[i];
        if (smallBlob) {
            smallBlob->velocity.y -= 0x20000;
            smallBlob->onGround    = true;
            smallBlob->interaction = interact;
            self->blobs[i]         = NULL;
        }
    }
}

void AmoebaDroid_CheckHit(void)
{
    RSDK_THIS(AmoebaDroid);

    if (self->invincibleTimer > 0)
        self->invincibleTimer--;

    foreach_active(Player, player)
    {
        if (!self->invincibleTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            if (--self->health <= 0) {
                AmoebaDroid_HandleSmallBlobRelease(false);
                self->state            = AmoebaDroid_State_Destroyed;
                self->timer            = 0;
                SceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
            }
            else {
                self->invincibleTimer = 48;
                RSDK.PlaySfx(AmoebaDroid->sfxHit, false, 255);
            }
        }
    }
}

void AmoebaDroid_CheckPlayerHit(void)
{
    RSDK_THIS(AmoebaDroid);

    foreach_active(Player, player)
    {
        if (!self->invincibleTimer) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                Player_CheckHit(player, self);
            }
        }
    }
}

void AmoebaDroid_Draw_AmoebaDroid(void)
{
    RSDK_THIS(AmoebaDroid);
    if (self->invincibleTimer & 1) {
        RSDK.CopyPalette(1, 16, 0, 128, 10);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator, NULL, false);
        RSDK.DrawSprite(&self->attractorTopAnimator, NULL, false);
        RSDK.DrawSprite(&self->attractorSideAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->attractorSideAnimator, NULL, false);

        RSDK.CopyPalette(1, 0, 0, 128, 10);
    }
    else {
        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator, NULL, false);
        RSDK.DrawSprite(&self->attractorTopAnimator, NULL, false);
        RSDK.DrawSprite(&self->attractorSideAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->attractorSideAnimator, NULL, false);
    }
}

void AmoebaDroid_Draw_BigBlob(void)
{
    RSDK_THIS(AmoebaDroid);
    Vector2 drawPos = self->position;

    SpriteFrame *frame = RSDK.GetFrame(AmoebaDroid->aniFrames, 3, 0);
    int32 angle        = self->angle;
    int32 sprY         = frame->sprY;

    int32 maxY    = (frame->sprY + 96) << 8;
    frame->height = 1;
    drawPos.y -= RSDK.Sin256(self->angle) << 10;

    for (int32 y = sprY << 8; y < maxY; angle += 2) {
        self->scale.x = (RSDK.Sin256(frame->sprY + 2 * angle) >> 2) + 512;
        frame->sprY   = y >> 8;

        if ((y >> 8) >= sprY)
            RSDK.DrawSprite(&self->animator, &drawPos, false);

        drawPos.y += 0x10000;
        y += (RSDK.Sin256(angle) >> 1) + 256;
    }
    frame->sprY = sprY;
}

void AmoebaDroid_State_SetupArena(void)
{
    RSDK_THIS(AmoebaDroid);
    if (++self->timer >= 8) {
        self->timer                 = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->centerX;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->cameraBoundsT[0]      = (self->position.y >> 16) - ScreenInfo->height;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16);
        AmoebaDroid->arenaLeft      = (Zone->cameraBoundsL[0] + 64) << 16;
        AmoebaDroid->arenaRight     = (Zone->cameraBoundsR[0] - 64) << 16;
        AmoebaDroid->startX         = self->position.x;
        AmoebaDroid->arenaTop       = (Zone->cameraBoundsT[0] + 48) << 16;
        AmoebaDroid->arenaBottom    = (Zone->cameraBoundsB[0] - 8) << 16;
        self->state                 = AmoebaDroid_State_SetupWaterLevel;
    }
}

void AmoebaDroid_State_SetupWaterLevel(void)
{
    RSDK_THIS(AmoebaDroid);
    if (self->timer) {
        self->timer++;
        if (self->timer == 120) {
            self->timer   = 0;
            self->visible = true;
            self->position.y += -0x400000 - (ScreenInfo->height << 16);
            self->state = AmoebaDroid_State_DropIn;
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++self->timer;
        }
        Water->waterLevel       = 0x7FFFFFFF;
        Water->newWaterLevel    = 0x7FFFFFFF;
        Water->targetWaterLevel = 0x7FFFFFFF;
        Water->waterMoveSpeed   = 0;
        foreach_active(Water, water)
        {
            if (water->type == WATER_ADJUST)
                destroyEntity(water);
        }
        RSDK.SetDrawLayerProperties(0, false, NULL);
        RSDK.SetDrawLayerProperties(Zone->hudDrawOrder, false, NULL);
    }
}

void AmoebaDroid_State_DropIn(void)
{
    RSDK_THIS(AmoebaDroid);
    self->position.y += 0x8000;
    if (++self->timer == 90) {
        self->timer      = 0;
        self->velocity.y = 0x8000;
        self->state      = AmoebaDroid_State_DropIntoPool;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_State_DropIntoPool(void)
{
    RSDK_THIS(AmoebaDroid);
    self->velocity.y += 0x1800;
    self->position.y += self->velocity.y;
    if (self->position.y > AmoebaDroid->arenaBottom) {
        self->velocity.y = self->velocity.y >> 2;
        self->state      = AmoebaDroid_State_SurfaceFromPool;
        ChemicalPool_SetDeform(self->position.x, 0x100000);
        CREATE_ENTITY(AmoebaDroid, intToVoid(AMOEBADROID_POOLSPLASH), self->position.x, self->position.y);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_State_SurfaceFromPool(void)
{
    RSDK_THIS(AmoebaDroid);

    self->velocity.y -= 0x2800;
    self->position.y += self->velocity.y;
    if (self->position.y <= AmoebaDroid->arenaBottom) {
        self->position.y = AmoebaDroid->arenaBottom;
        self->offsetPos  = self->position;
        self->velocity.y = 0;
        self->state      = AmoebaDroid_State_ChooseAttack;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_State_ChooseAttack(void)
{
    RSDK_THIS(AmoebaDroid);

    self->position.y = BadnikHelpers_Oscillate(self->offsetPos.y, 2, 10);

    if (++self->timer == 48) {
        self->timer = 3;
        if (RSDK.Rand(0, 256) > 128)
            self->state = AmoebaDroid_State_SwimLeft;
        else
            self->state = AmoebaDroid_State_SwimRight;

        EntityAmoebaDroid *part = CREATE_ENTITY(AmoebaDroid, intToVoid(AMOEBADROID_BLOB_BIG), self->position.x, self->position.y);
        part->parent            = (Entity *)self;
        self->blobs[0]          = (Entity *)part;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_State_SwimLeft(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&self->attractorTopAnimator);
    RSDK.ProcessAnimation(&self->attractorSideAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->offsetPos.y, 2, 10);

    self->velocity.x -= 0x2000;
    if (self->velocity.x < -0x40000)
        self->velocity.x = -0x40000;
    self->position.x += self->velocity.x;

    if (self->timer <= 0) {
        if (self->timer <= 0) {
            self->velocity.x = 0;
            self->velocity.y = 0;
            self->state      = AmoebaDroid_State_ExitPool;
            ChemicalPool_SetDeform(self->position.x, -0xC0000);
            CREATE_ENTITY(AmoebaDroid, intToVoid(AMOEBADROID_POOLSPLASH_DELAY), self->position.x, self->position.y);
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
        }
    }
    else if (self->position.x < AmoebaDroid->arenaLeft + 0x400000) {
        self->state = AmoebaDroid_State_SwimRight;
        self->timer--;
    }
    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(AmoebaDroid->sfxGather, false, 255);
    ChemicalPool_SetDeform(self->position.x, -0x8000);
}

void AmoebaDroid_State_SwimRight(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&self->attractorTopAnimator);
    RSDK.ProcessAnimation(&self->attractorSideAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->offsetPos.y, 2, 10);

    self->velocity.x += 0x2000;
    if (self->velocity.x > 0x40000)
        self->velocity.x = 0x40000;
    self->position.x += self->velocity.x;

    if (self->timer <= 0) {
        if (self->timer >= 0) {
            self->velocity.x = 0;
            self->velocity.y = 0;
            self->state      = AmoebaDroid_State_ExitPool;
            ChemicalPool_SetDeform(self->position.x, -0xC0000);
            CREATE_ENTITY(AmoebaDroid, intToVoid(AMOEBADROID_POOLSPLASH_DELAY), self->position.x, self->position.y);
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
        }
    }
    else if (self->position.x > AmoebaDroid->arenaRight - 0x400000) {
        self->state = AmoebaDroid_State_SwimLeft;
        self->timer--;
    }
    if (!(Zone->timer & 0xF))
        RSDK.PlaySfx(AmoebaDroid->sfxGather, false, 255);
    ChemicalPool_SetDeform(self->position.x, -0x8000);
}

void AmoebaDroid_State_ExitPool(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&self->attractorTopAnimator);
    RSDK.ProcessAnimation(&self->attractorSideAnimator);
    self->velocity.y -= 0x1800;
    self->position.y += self->velocity.y;
    if (self->position.y < AmoebaDroid->arenaTop + 0x400000) {
        self->timer = 3;
        self->state = AmoebaDroid_State_BounceAttack;
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_State_BounceAttack(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&self->attractorTopAnimator);
    RSDK.ProcessAnimation(&self->attractorSideAnimator);
    self->velocity.y += 0x2000;
    self->position.y += self->velocity.y;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (self->position.x < player1->position.x) {
        self->position.x = self->position.x + 0x10000;
    }
    else if (self->position.x > player1->position.x) {
        self->position.x = self->position.x - 0x10000;
    }

    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, self->collisionPlane, 0, 0x180000, true)) {
        if (--self->timer <= 0) {
            EntityAmoebaDroid *bigBlob = (EntityAmoebaDroid *)self->blobs[0];
            RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 6, &bigBlob->animator, false, 0);
            bigBlob->drawFX    = FX_NONE;
            bigBlob->stateDraw = StateMachine_None;
            bigBlob->state     = AmoebaDroid_State_BigBlob_Disappear;

            self->state = StateMachine_None;
            for (int32 i = 0; i < 8; ++i) {
                self->blobs[i] = (Entity *)CREATE_ENTITY(AmoebaDroid, intToVoid(AMOEBADROID_BLOB_SMALL), self->position.x, self->position.y);
            }

            self->timer         = 0;
            self->blobAngleY     = 196;
            self->blobAmplitude = 0x2800;
            self->state         = AmoebaDroid_State_GatherBlobs;
            RSDK.PlaySfx(AmoebaDroid->sfxRelease, false, 255);
        }
        else {
            self->velocity.y = minVal(-self->velocity.y, -0x20000);
            for (int32 i = 0; i < 8; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x + RSDK.Rand(-0x200000, 0x200000),
                                                     self->position.y + RSDK.Rand(0x100000, 0x180000));
                RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 5, &debris->animator, false, 0);
                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y    = RSDK.Rand(-0x40000, -0x20000);
                debris->gravity       = 0x3800;
                debris->inkEffect     = INK_BLEND;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x200000;
                debris->updateRange.y = 0x200000;
            }
            RSDK.PlaySfx(AmoebaDroid->sfxBounce, false, 255);
        }
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_State_GatherBlobs(void)
{
    RSDK_THIS(AmoebaDroid);
    RSDK.ProcessAnimation(&self->attractorTopAnimator);
    RSDK.ProcessAnimation(&self->attractorSideAnimator);
    AmoebaDroid_HandleSmallBlobMovement();
    self->blobAngleX = (self->blobAngleX + 4) & 0xFF;

    if (self->timer == 60) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        if (self->position.y <= AmoebaDroid->arenaTop) {
            self->offsetPos.x = self->position.x;
            self->offsetPos.y = self->position.y;
            self->timer       = 0;
            self->angle       = 0;
            self->position.y  = AmoebaDroid->arenaTop;
            self->state       = AmoebaDroid_State_SpinBlobs;
        }
    }
    else {
        self->position.y -= 0x8000;
        self->timer++;
        if (self->timer == 60) {
            self->velocity.x = (AmoebaDroid->startX - self->position.x + RSDK.Rand(-0x800000, 0x800000)) >> 6;
            self->velocity.y = (AmoebaDroid->arenaTop - self->position.y) >> 6;
        }
    }
    AmoebaDroid_CheckHit();
}

void AmoebaDroid_State_SpinBlobs(void)
{
    RSDK_THIS(AmoebaDroid);

    self->position.y = BadnikHelpers_Oscillate(self->offsetPos.y, 2, 10);

    if (self->timer < 120) {
        AmoebaDroid_HandleSmallBlobMovement();
        if (self->offsetPos.x <= AmoebaDroid->startX)
            self->blobAngleY -= 2;
        else
            self->blobAngleY += 2;
        self->blobAngleY &= 0xFF;
        self->blobAngleX = (self->blobAngleX + 4) & 0xFF;
        RSDK.ProcessAnimation(&self->attractorTopAnimator);
        RSDK.ProcessAnimation(&self->attractorSideAnimator);
    }
    AmoebaDroid_CheckHit();

    ++self->timer;
    if (self->timer == 120) {
        AmoebaDroid_HandleSmallBlobRelease(true);
        RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 1, &self->attractorTopAnimator, true, 0);
        RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 2, &self->attractorSideAnimator, true, 0);
        RSDK.PlaySfx(AmoebaDroid->sfxRelease, false, 255);
    }
    else if (self->timer == 240) {
        self->timer      = 0;
        self->angle      = 0;
        self->velocity.y = -0x20000;
        self->state      = AmoebaDroid_State_DropIntoPool;
    }
}

void AmoebaDroid_State_BigBlob(void)
{
    RSDK_THIS(AmoebaDroid);
    EntityAmoebaDroid *parent = (EntityAmoebaDroid *)self->parent;
    self->angle               = (self->angle + 4) & 0xFF;
    if (parent->state == AmoebaDroid_State_SpawnSignPost || parent->state == AmoebaDroid_State_Destroyed) {
        if (self->alpha > 0) {
            self->alpha -= 4;
            if (self->alpha <= 0)
                destroyEntity(self);
        }
    }
    else {
        AmoebaDroid_CheckPlayerHit();
        if (self->alpha < 128) {
            self->alpha++;
            self->position.x = parent->position.x;
            self->position.y = parent->position.y;
        }
    }
    self->position.x = parent->position.x;
    self->position.y = parent->position.y;
}

void AmoebaDroid_State_SmallBlob(void)
{
    RSDK_THIS(AmoebaDroid);
    if (self->alpha < 192)
        self->alpha += 4;
    if (self->onGround) {
        self->position.x += self->velocity.x;
        self->velocity.y += 0x2000;
        self->position.y += self->velocity.y;
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, self->collisionPlane, 0, 0x60000, true)) {
            for (int32 i = 0; i < 4; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x + RSDK.Rand(-0x40000, 0x40000),
                                                     self->position.y + RSDK.Rand(-0x40000, 0x40000));
                RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 5, &debris->animator, false, 0);
                debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
                debris->velocity.y    = -0x20000;
                debris->gravity       = 0x3800;
                debris->inkEffect     = INK_BLEND;
                debris->drawOrder     = Zone->drawOrderLow;
                debris->updateRange.x = 0x200000;
                debris->updateRange.y = 0x200000;
            }
            destroyEntity(self);
        }
        else {
            if (self->interaction && self->drawOrder == Zone->drawOrderLow)
                AmoebaDroid_CheckPlayerHit();

            if (!RSDK.CheckOnScreen(self, NULL))
                destroyEntity(self);
        }
    }
    else {
        if (self->interaction && self->drawOrder == Zone->drawOrderLow)
            AmoebaDroid_CheckPlayerHit();

        if (!RSDK.CheckOnScreen(self, NULL))
            destroyEntity(self);
    }
}

void AmoebaDroid_State_BigBlob_Disappear(void)
{
    RSDK_THIS(AmoebaDroid);
    EntityAmoebaDroid *parent = (EntityAmoebaDroid *)self->parent;
    if (parent) {
        self->position.x = parent->position.x;
        self->position.y = parent->position.y;
    }
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void AmoebaDroid_State_PoolSplash_Delayed(void)
{
    RSDK_THIS(AmoebaDroid);
    if (++self->timer == 16) {
        self->timer   = 0;
        self->visible = true;
        self->state   = AmoebaDroid_State_PoolSplash;
    }
}

void AmoebaDroid_State_PoolSplash(void)
{
    RSDK_THIS(AmoebaDroid);
    int32 pos = (self->position.x + 0x80000) >> 20;
    foreach_active(ChemicalPool, pool)
    {
        if (pos >= pool->startX && pos < pool->endX)
            self->position.y = pool->offsetY + ChemicalPool->surfaceDeformation[pos];
    }

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void AmoebaDroid_State_Destroyed(void)
{
    RSDK_THIS(AmoebaDroid);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(AmoebaDroid->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int32 x = (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16) + self->position.x;
            int32 y = (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16) + self->position.y;
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    ++self->timer;
    if (self->timer == 30) {
        Debris_FallFlickerAnimSetup(AmoebaDroid->aniFrames, AmoebaDroid->debrisInfo2, 7);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->attractorTopAnimator, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->attractorSideAnimator, true, 0);
    }
    else if (self->timer == 60) {
        Debris_FallFlickerAnimSetup(AmoebaDroid->aniFrames, AmoebaDroid->debrisInfo1, 7);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator, true, 0);
    }
    else if (self->timer == 90) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->timer   = 0;
        self->visible = false;
        self->state   = AmoebaDroid_State_SpawnSignPost;
    }
}

void AmoebaDroid_State_SpawnSignPost(void)
{
    RSDK_THIS(AmoebaDroid);
    if (++self->timer == 60) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->active     = ACTIVE_NORMAL;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
        }
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void AmoebaDroid_EditorDraw(void)
{
    RSDK_THIS(AmoebaDroid);

    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 0, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 1, &self->attractorTopAnimator, true, 0);
    RSDK.SetSpriteAnimation(AmoebaDroid->aniFrames, 2, &self->attractorSideAnimator, true, 0);

    AmoebaDroid_Draw_AmoebaDroid();
}

void AmoebaDroid_EditorLoad(void)
{
    AmoebaDroid->aniFrames   = RSDK.LoadSpriteAnimation("CPZ/AmoebaDroid.bin", SCOPE_STAGE);
    AmoebaDroid->waterFrames = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(AmoebaDroid, type);
    RSDK_ENUM_VAR("Boss", AMOEBADROID_BOSS);
}
#endif

void AmoebaDroid_Serialize(void) { RSDK_EDITABLE_VAR(AmoebaDroid, VAR_ENUM, type); }
