// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GigaMetal Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectGigaMetal *GigaMetal;

void GigaMetal_Update(void)
{
    RSDK_THIS(GigaMetal);

    StateMachine_Run(self->state);
}

void GigaMetal_LateUpdate(void) {}

void GigaMetal_StaticUpdate(void) {}

void GigaMetal_Draw(void)
{
    RSDK_THIS(GigaMetal);

    if (GigaMetal->explodeTimer <= 0) {
        if (GigaMetal->invincibleTimer & 1) {
            RSDK.CopyPalette(2, 32, 0, 32, 16);
            RSDK.CopyPalette(2, 160, 0, 160, 10);
            RSDK.CopyPalette(2, 240, 0, 240, 7);

            if (self->stateDraw) {
                StateMachine_Run(self->stateDraw);
            }
            else {
                RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            }

            RSDK.CopyPalette(1, 32, 0, 32, 16);
            RSDK.CopyPalette(1, 160, 0, 160, 10);
            RSDK.CopyPalette(1, 240, 0, 240, 7);
        }
        else {
            if (self->stateDraw) {
                StateMachine_Run(self->stateDraw);
            }
            else {
                RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            }
        }
    }
    else {
        RSDK.SetLimitedFade(0, 4, 5, GigaMetal->explodeTimer, 0, 256);

        if (self->stateDraw) {
            StateMachine_Run(self->stateDraw);
        }
        else {
            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
        }

        RSDK.CopyPalette(4, 1, 0, 1, 255);
    }
}

void GigaMetal_Create(void *data)
{
    RSDK_THIS(GigaMetal);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->drawGroup     = Zone->objectDrawGroup[0] - 1;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            int32 slot          = RSDK.GetEntitySlot(self);

            if (data)
                self->aniID = VOID_TO_INT(data);

            switch (self->aniID) {
                case GIGAMETAL_HEAD:
                    self->body           = RSDK_GET_ENTITY(slot - 1, GigaMetal);
                    self->frontArm       = RSDK_GET_ENTITY(slot + 3, GigaMetal);
                    self->backArm        = RSDK_GET_ENTITY(slot - 2, GigaMetal);
                    self->stateDraw      = GigaMetal_Draw_Head;
                    self->componentPos.x = 0x140000;
                    self->componentPos.y = -0x240000;

                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_HEAD, &self->mainAnimator, true, 0);
                    break;

                case GIGAMETAL_SHOULDER:
                    self->body     = RSDK_GET_ENTITY(slot - 5, GigaMetal);
                    self->frontArm = RSDK_GET_ENTITY(slot - 1, GigaMetal);

                    self->stateDraw      = GigaMetal_Draw_Shoulder;
                    self->componentPos.y = -0x2C0000;
                    self->componentPos.x = -0x240000;
                    self->drawGroup      = Zone->objectDrawGroup[1];

                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_SHOULDER, &self->mainAnimator, true, 0);
                    break;

                case GIGAMETAL_BODY: {
                    self->head     = RSDK_GET_ENTITY(slot + 1, GigaMetal);
                    self->ruby     = RSDK_GET_ENTITY(slot + 2, PhantomRuby);
                    self->cover    = RSDK_GET_ENTITY(slot + 3, GigaMetal);
                    self->frontArm = RSDK_GET_ENTITY(slot + 4, GigaMetal);
                    self->backArm  = RSDK_GET_ENTITY(slot - 1, GigaMetal);
                    self->shoulder = RSDK_GET_ENTITY(slot + 5, GigaMetal);

                    self->active = ACTIVE_BOUNDS;
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_BODY, &self->mainAnimator, true, 0);

                    foreach_all(MetalSonic, metal) { self->metalSonic = metal; }

                    self->health = 8;
                    self->state  = GigaMetal_StateBody_AwaitPlayer;
                    break;
                }

                case GIGAMETAL_COVER:
                    self->body = RSDK_GET_ENTITY(slot - 3, GigaMetal);

                    self->stateDraw = GigaMetal_Draw_Cover;
                    self->inkEffect = INK_ADD;
                    self->alpha     = 0xFF;

                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_COVER, &self->mainAnimator, true, 0);
                    break;

                case GIGAMETAL_ARMFRONT:
                    self->body = RSDK_GET_ENTITY(slot - 4, GigaMetal);

                    self->stateDraw      = GigaMetal_Draw_Arm;
                    self->componentPos.x = -0x1C0000;
                    self->componentPos.y = -0x2C0000;
                    self->drawGroup      = Zone->objectDrawGroup[1];

                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->jointAnimator, true, 1);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->armAnimator, true, 2);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->handAnimator, true, 3);
                    break;

                case GIGAMETAL_ARMBACK:
                    self->body = RSDK_GET_ENTITY(slot + 1, GigaMetal);

                    self->stateDraw      = GigaMetal_Draw_Arm;
                    self->componentPos.x = -0xC0000;
                    self->componentPos.y = -0x2C0000;

                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->jointAnimator, true, 1);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->armAnimator, true, 2);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->handAnimator, true, 3);
                    break;

                case GIGAMETAL_LASEREDGE:
                    self->drawFX    = FX_ROTATE;
                    self->laserSize = -1;
                    self->angle     = 0x40;
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->scale.x   = 0x200;
                    self->scale.y   = 0x200;
                    self->state     = GigaMetal_State_Laser;
                    self->stateDraw = GigaMetal_Draw_LaserEdge;

                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASERBEAM, &self->jointAnimator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->armAnimator, true, 1);
                    break;

                case GIGAMETAL_SHARD:
                    self->scale.x           = RSDK.Rand(0x200, 0x400);
                    self->scale.y           = RSDK.Rand(0x200, 0x400);
                    self->rotationAngles[0] = RSDK.Rand(0, 0x400);
                    self->rotationAngles[1] = RSDK.Rand(0, 0x400);
                    self->rotationAngles[2] = RSDK.Rand(0, 0x400);

                    self->active     = ACTIVE_NORMAL;
                    self->visible    = true;
                    self->drawGroup  = Zone->objectDrawGroup[1];
                    self->velocity.x = RSDK.Rand(-0x10000, 0x10000);
                    self->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                    self->groundVel  = RSDK.Rand(-16, 16);

                    self->state     = GigaMetal_State_Shard;
                    self->stateDraw = GigaMetal_Draw_Shard;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void GigaMetal_StageLoad(void)
{
    GigaMetal->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/GigaMetal.bin", SCOPE_STAGE);

    GigaMetal->hitboxLaser.left   = -12;
    GigaMetal->hitboxLaser.top    = -12;
    GigaMetal->hitboxLaser.right  = 12;
    GigaMetal->hitboxLaser.bottom = 12;

    GigaMetal->hitboxHand.left   = -12;
    GigaMetal->hitboxHand.top    = -10;
    GigaMetal->hitboxHand.right  = 12;
    GigaMetal->hitboxHand.bottom = 10;

    GigaMetal->hitboxCore.left   = -40;
    GigaMetal->hitboxCore.top    = -16;
    GigaMetal->hitboxCore.right  = 40;
    GigaMetal->hitboxCore.bottom = 40;

    GigaMetal->hitboxHead.left   = -56;
    GigaMetal->hitboxHead.top    = -124;
    GigaMetal->hitboxHead.right  = 16;
    GigaMetal->hitboxHead.bottom = -64;

    GigaMetal->sfxRoar   = RSDK.GetSfx("SSZ2/MSRoar.wav");
    GigaMetal->sfxImpact = RSDK.GetSfx("Stage/Impact6.wav");
    GigaMetal->sfxTarget = RSDK.GetSfx("SSZ2/MSTarget.wav");
    GigaMetal->sfxPimpom = RSDK.GetSfx("Stage/PimPom.wav");
    GigaMetal->sfxCannon = RSDK.GetSfx("SSZ2/MSCannon.wav");

    RSDK.CopyPalette(0, 1, 4, 1, 255);
    // Pink blend palette
    for (int32 i = 0; i < 256; ++i) RSDK.SetPaletteEntry(5, i, 0xF00080);

    GigaMetal->invincibleTimer = 0;
    GigaMetal->explodeTimer    = 0;

    if (StarPost->postIDs[0]) {
        foreach_all(Music, music)
        {
            if (music->playOnLoad)
                music->trackID = TRACK_METALSONIC;
        }

        Music->restartTrackID = TRACK_METALSONIC;
    }
}

void GigaMetal_Draw_Cover(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x = self->body->position.x + self->componentPos.x;
    self->position.y = self->body->position.y + self->componentPos.y;

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void GigaMetal_Draw_Head(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x = self->body->position.x + self->componentPos.x;
    self->position.y = self->body->position.y + self->componentPos.y;

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->jointAnimator, NULL, false);
    RSDK.DrawSprite(&self->armAnimator, NULL, false);
}

void GigaMetal_Draw_Shoulder(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x = self->body->position.x + self->componentPos.x;
    self->position.y = self->body->position.y + self->componentPos.y;

    self->position.x = MIN(3 * (self->position.x >> 2) + ((self->frontArm->position.x - 0xC0000) >> 2), self->position.x);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void GigaMetal_Draw_Arm(void)
{
    RSDK_THIS(GigaMetal);
    Vector2 drawPos;

    self->position.x = self->body->position.x + self->componentPos.x;
    self->position.y = self->body->position.y + self->componentPos.y;

    int32 angle      = self->rotationAngles[0] >> 6;
    drawPos.x        = (RSDK.Sin1024(angle) << 11) + self->position.x;
    drawPos.y        = (RSDK.Cos1024(angle) << 11) + self->position.y;
    self->position.x = drawPos.x;
    self->position.y = drawPos.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    angle += (self->rotationAngles[1] >> 6);
    drawPos.x += 0x600 * RSDK.Sin1024(angle);
    drawPos.y += 0x600 * RSDK.Cos1024(angle);

    if (self->mainAnimator.animationID == GIGAMETAL_ARMFRONT) {
        self->drawFX   = FX_ROTATE;
        self->rotation = -(angle >> 1);
        RSDK.DrawSprite(&self->armAnimator, &drawPos, false);

        self->drawFX = FX_NONE;
        RSDK.DrawSprite(&self->jointAnimator, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&self->jointAnimator, &drawPos, false);

        self->drawFX   = FX_ROTATE;
        self->rotation = -(angle >> 1);
        RSDK.DrawSprite(&self->armAnimator, &drawPos, false);

        self->drawFX = FX_NONE;
    }

    drawPos.x += RSDK.Sin1024(angle) << 9;
    drawPos.y += RSDK.Cos1024(angle) << 9;

    angle += 0x100;
    drawPos.x += 0x1500 * RSDK.Sin1024(angle);
    drawPos.y += 0x1500 * RSDK.Cos1024(angle);
    self->drawFX   = FX_ROTATE;
    self->rotation = -((angle + ((self->rotationAngles[2] >> 6) - 256)) >> 1);
    RSDK.DrawSprite(&self->handAnimator, &drawPos, false);

    self->drawFX = FX_NONE;
}

void GigaMetal_Draw_LaserEdge(void)
{
    RSDK_THIS(GigaMetal);

    Vector2 drawPos;
    self->position.x = self->body->position.x;
    self->position.y = self->body->position.y;

    // Draw Laser Edge (Bit that emits laser)
    int32 angle    = self->angle - self->body->rotation;
    self->rotation = self->angle + self->body->rotation;
    drawPos.x      = (RSDK.Sin512(angle + 8) << 12) + self->position.x;
    drawPos.y      = (RSDK.Cos512(angle + 8) << 12) + self->position.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    int32 moveX = (RSDK.Sin512(angle) << 12) & 0xFFFF0000;
    int32 moveY = (RSDK.Cos512(angle) << 12) & 0xFFFF0000;

    // Draw Laser
    for (int32 i = 0; i < self->laserSize; ++i) {
        RSDK.DrawSprite(&self->jointAnimator, &drawPos, false);
        drawPos.x += moveX;
        drawPos.y += moveY;
    }

    // Draw Laser Edge (Bit that hits the ground)
    RSDK.DrawSprite(&self->armAnimator, &drawPos, false);
}

void GigaMetal_Draw_Shard(void)
{
    RSDK_THIS(GigaMetal);

    Vector2 vertices[3];

    int32 x = ScreenInfo[SceneInfo->currentScreenID].position.x << 16;
    int32 y = ScreenInfo[SceneInfo->currentScreenID].position.y << 16;

    int32 angle   = self->angle + self->rotationAngles[0];
    vertices[0].x = self->scale.x * RSDK.Sin1024(angle) - x + self->position.x;
    vertices[0].y = self->scale.y * RSDK.Cos1024(angle) + self->position.y - y;

    angle         = self->angle + self->rotationAngles[1];
    vertices[1].x = self->scale.x * RSDK.Sin1024(angle) - x + self->position.x;
    vertices[1].y = self->scale.y * RSDK.Cos1024(angle) + self->position.y - y;

    angle         = self->angle + self->rotationAngles[2];
    vertices[2].x = self->scale.x * RSDK.Sin1024(angle) - x + self->position.x;
    vertices[2].y = self->scale.y * RSDK.Cos1024(angle) + self->position.y - y;

    RSDK.DrawFace(vertices, 3, 0xF0, 0x00, 0x80, 0xFF, INK_ADD);
}

void GigaMetal_HandleCameraMovement(void)
{
    Zone->cameraBoundsL[0] += 2;
    Zone->cameraBoundsR[0] += 2;

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    camera->boundsL      = Zone->cameraBoundsL[0];
    camera->boundsR      = Zone->cameraBoundsR[0];
    camera->position.x   = (Zone->cameraBoundsR[0] + camera->boundsL) << 15;

    MetalSonic_HandleStageWrap();
}

void GigaMetal_CheckPlayerCollisions(void)
{
    RSDK_THIS(GigaMetal);

    if (GigaMetal->invincibleTimer > 0)
        GigaMetal->invincibleTimer--;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &GigaMetal->hitboxHead) && player->velocity.x < 0 && Player_CheckBossHit(player, self)) {
            player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
            RSDK.PlaySfx(GigaMetal->sfxPimpom, false, 0xFF);
        }
    }

    foreach_active(Player, playerPtr)
    {
        if (!GigaMetal->invincibleTimer && Player_CheckBadnikTouch(playerPtr, self, &GigaMetal->hitboxCore) && Player_CheckBossHit(playerPtr, self)) {
            playerPtr->velocity.x += Zone->autoScrollSpeed;
            GigaMetal_Hit();
        }
    }
}

void GigaMetal_Hit(void)
{
    RSDK_THIS(GigaMetal);

    if (--self->health <= 0) {
        EntityGigaMetal *head     = self->head;
        EntityGigaMetal *frontArm = self->frontArm;
        EntityGigaMetal *backArm  = self->backArm;
        head->state               = StateMachine_None;
        frontArm->state           = StateMachine_None;
        backArm->state            = StateMachine_None;

        self->timer            = 0;
        self->state            = GigaMetal_StateBody_Destroyed;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        GigaMetal->invincibleTimer = 40;
        RSDK.PlaySfx(MetalSonic->sfxHit, false, 255);

        EntityGigaMetal *cover = self->cover;
        ++cover->mainAnimator.frameID;
    }
}

void GigaMetal_StateBody_AwaitPlayer(void)
{
    RSDK_THIS(GigaMetal);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.y < self->position.y && !player1->collisionPlane) {

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;

        Zone->cameraBoundsL[0] = FROM_FIXED(self->position.x) - ScreenInfo->center.x + 32;
        Zone->cameraBoundsR[0] = FROM_FIXED(self->position.x) + ScreenInfo->center.x + 32;
        Zone->cameraBoundsT[0] = FROM_FIXED(self->position.y) - ScreenInfo->size.y + 16;
        Zone->cameraBoundsB[0] = FROM_FIXED(self->position.y) + 16;
        Zone->deathBoundary[0] = FROM_FIXED(self->position.y) + 16;

        EntityMetalSonic *metal = self->metalSonic;
        if (metal) {
            if (!metal->onScreen) {
                metal->position.x = self->position.x - 0xC00000;
                metal->position.y = self->position.y + 0x400000;
            }
            metal->angle       = 0;
            metal->targetPos.x = self->position.x - 0x400000;
            metal->targetPos.y = self->position.y - 0x500000;
            metal->timer       = 0;
            metal->visible     = true;
            metal->active      = ACTIVE_NORMAL;
            metal->state       = MetalSonic_State_WaitForRuby;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &metal->metalSonicAnimator, false, 0);
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BOOSTER_WEAK, &metal->boosterAnimator, false, 0);
        }

        self->position.x -= 0x500000;
        self->position.y -= 0x380000;
        self->active = ACTIVE_NORMAL;
        self->state  = GigaMetal_StateBody_SetupComponents;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
    }
}

void GigaMetal_StateBody_SetupComponents(void)
{
    RSDK_THIS(GigaMetal);

    EntityMetalSonic *metal = self->metalSonic;

    // Wait for metal sonic to become inactive (should happen during the white flash)
    if (metal->active == ACTIVE_NEVER) {
        EntityGigaMetal *head = self->head;
        head->visible         = true;
        head->active          = ACTIVE_NORMAL;

        EntityPhantomRuby *ruby = self->ruby;
        ruby->startPos.x        = self->position.x + 0x200000;
        ruby->startPos.y        = self->position.y + 0x100000;
        ruby->drawGroup         = self->drawGroup;
        ruby->position.x        = ruby->startPos.x;
        ruby->position.y        = ruby->startPos.y;

        EntityGigaMetal *cover = self->cover;
        cover->visible         = true;
        cover->active          = ACTIVE_NORMAL;

        EntityGigaMetal *shoulder = self->shoulder;
        shoulder->visible         = true;
        shoulder->active          = ACTIVE_NORMAL;

        EntityGigaMetal *frontArm = self->frontArm;
        frontArm->visible         = true;
        frontArm->active          = ACTIVE_NORMAL;
        frontArm->state           = GigaMetal_StateArm_Idle_Front;

        EntityGigaMetal *backArm = self->backArm;
        backArm->visible         = true;
        backArm->active          = ACTIVE_NORMAL;
        backArm->state           = GigaMetal_StateArm_Idle_Front;

        self->visible = true;
        self->state   = GigaMetal_StateBody_Transformed;
        RSDK.CopyTileLayer(Zone->fgLayer[1], 0, 34, Zone->fgLayer[1], 0, 26, 256, 6);

        foreach_active(Player, player)
        {
            if (player->position.x < self->position.x + 0xE00000)
                player->position.x = self->position.x + 0xE00000;

            player->position.y = ruby->position.y + 0x80000;
            player->groundVel  = 0;
            player->velocity.x = 0;
        }
    }
}

void GigaMetal_StateBody_Transformed(void)
{
    RSDK_THIS(GigaMetal);

    if (++self->timer == 120) {
        EntityGigaMetal *head = self->head;
        head->state           = GigaMetal_StateHead_PrepareRoar;

        EntityGigaMetal *frontArm = self->frontArm;
        frontArm->state           = GigaMetal_StateArm_Idle_Behind;

        EntityGigaMetal *backArm = self->backArm;
        backArm->state           = GigaMetal_StateArm_Idle_Behind;

        self->timer = 0;
        self->state = GigaMetal_StateBody_Roar;
    }
}

void GigaMetal_StateBody_Roar(void)
{
    RSDK_THIS(GigaMetal);

    ++self->timer;
    if (!(self->timer & 7))
        Camera_ShakeScreen(0, 0, 4);

    if (self->timer == 120) {
        EntityGigaMetal *head = self->head;
        head->state           = GigaMetal_StateHead_FinishRoar;

        EntityGigaMetal *frontArm = self->frontArm;
        frontArm->timer           = 48;
        frontArm->state           = GigaMetal_StateArm_Marching;

        EntityGigaMetal *backArm = self->backArm;
        backArm->timer           = 48;
        backArm->angle           = 128;
        backArm->state           = GigaMetal_StateArm_Marching;

        self->targetPos.x     = self->position.x;
        self->targetPos.y     = self->position.y + 0x80000;
        self->timer           = 320;
        self->velocity.x      = 0x20000;
        self->state           = GigaMetal_StateBody_Marching;
        Zone->autoScrollSpeed = 0x20000;
    }
}

void GigaMetal_StateBody_Marching(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x += self->velocity.x;

    if ((RSDK.Sin256(self->angle) & 0x200000) != 0)
        self->position.y = self->targetPos.y - -(RSDK.Sin256(self->angle) << 10);
    else
        self->position.y = self->targetPos.y - (RSDK.Sin256(self->angle) << 10);

    self->angle = (self->angle + 2) & 0xFF;

    EntityPhantomRuby *ruby = self->ruby;
    ruby->position.x        = self->position.x + 0x200000;
    ruby->startPos.y        = self->position.y + 0x100000;

    EntityGigaMetal *head = self->head;
    if (!(self->angle & 0x7F)) {
        if (!head->state) {
            head->targetPos.x = head->componentPos.x;
            head->targetPos.y = head->componentPos.y;
            head->velocity.y  = 0x10000;
            head->state       = GigaMetal_StateHead_Impact;
            RSDK.PlaySfx(GigaMetal->sfxImpact, false, 255);
        }

        EntityGigaMetal *shoulder = self->shoulder;
        shoulder->targetPos.x     = shoulder->componentPos.x;
        shoulder->targetPos.y     = shoulder->componentPos.y;
        shoulder->velocity.y      = -0x12000;
        shoulder->state           = GigaMetal_StateShoulder_Impact;

        Camera_ShakeScreen(0, 0, 4);
    }

    if (!head->state || head->state == GigaMetal_StateHead_Impact)
        --self->timer;

    if (self->timer <= 0) {
        EntityGigaMetal *frontArm = self->frontArm;
        EntityGigaMetal *backArm  = self->backArm;

        self->timer = RSDK.Rand(240, 480);
        if (RSDK.Rand(0, 1024) <= 0x200) {
            if (self->attackTimer & 0xFFFFFF00)
                self->attackTimer = 0;
            else
                self->attackTimer++;

            if ((self->attackTimer & 0xFF) > 2)
                self->attackTimer = 0x100;
        }
        else {
            if ((self->attackTimer & 0xFFFFFF00) == 0x100)
                ++self->attackTimer;
            else
                self->attackTimer = 0x100;

            if ((self->attackTimer & 0xFF) > 2)
                self->attackTimer = 0;
        }

        head->state = (self->attackTimer & 0xFFFFFF00) == 0x100 ? GigaMetal_StateHead_TargetingPlayer : GigaMetal_StateHead_PrepareLaser;

        frontArm->state = GigaMetal_StateArm_Idle_Behind;
        backArm->state  = GigaMetal_StateArm_Idle_Behind;
    }

    GigaMetal_HandleCameraMovement();
    GigaMetal_CheckPlayerCollisions();

    if ((Zone->timer & 7) == 2) {
        int32 endX = ((ScreenInfo->size.x + ScreenInfo->position.x) >> 4) + 1;
        int32 endY = (ScreenInfo->size.y + ScreenInfo->position.y) >> 4;

        RSDK.CopyTileLayer(Zone->fgLayer[1], endX, endY - 5, Zone->fgLayer[1], endX, endY - 5 + 8, 1, 6);
        if (endX > 224)
            RSDK.CopyTileLayer(Zone->fgLayer[1], endX - 224, endY - 5, Zone->fgLayer[1], endX - 224, endY - 5 + 8, 1, 6);

        int32 tileX  = (ScreenInfo->position.x >> 4) + 8;
        int32 spawnY = (endY << 20) + 0x80000;

        for (int32 delay = 4; delay < 40; delay += 6) {
            uint16 tile = RSDK.GetTile(Zone->fgLayer[1], tileX, endY);

            if (tile != (uint16)-1) {
                EntityBreakableWall *block = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_DYNAMIC), (tileX << 20) + 0x80000, spawnY);
                block->drawGroup           = Zone->objectDrawGroup[1];
                block->targetLayer         = Zone->fgLayer[1];
                block->tileInfo            = tile;
                block->tilePos.x           = tileX;
                block->tilePos.y           = endY;
                block->timer               = delay;
                block->active              = ACTIVE_NORMAL;

                if (tileX > 224) {
                    block                = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_DYNAMIC), (tileX << 20) - 0xDF80000, spawnY);
                    block->drawGroup     = Zone->objectDrawGroup[1];
                    block->targetLayer   = Zone->fgLayer[1];
                    block->tileInfo      = tile;
                    block->tilePos.y     = endY;
                    block->tilePos.x     = tileX - 224;
                    block->timer         = delay;
                    block->updateRange.x = 0x10000000;
                    block->active        = ACTIVE_NORMAL;
                }
            }

            --endY;
            spawnY -= 0x100000;
        }
    }
}

void GigaMetal_StateBody_Destroyed(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y += 0x1000;
    self->position.y += self->velocity.y;

    if (self->position.y >= self->targetPos.y) {
        EntityGigaMetal *head = self->head;
        self->velocity.y      = -0xC000;
        head->velocity.y      = 0x18000;
        head->targetPos       = head->componentPos;
        head->state           = GigaMetal_StateHead_Impact;

        EntityGigaMetal *shoulder = self->shoulder;
        shoulder->velocity.y      = -0x18000;
        shoulder->targetPos       = shoulder->componentPos;
        shoulder->state           = GigaMetal_StateShoulder_Impact;
        Camera_ShakeScreen(0, 0, 4);
    }

    EntityPhantomRuby *ruby = self->ruby;
    ruby->position.x        = self->position.x + 0x200000;
    ruby->startPos.y        = self->position.y + 0x100000;

    if (!(Zone->timer & 7)) {
        int32 x = self->position.x + RSDK.Rand(-0x600000, 0x600000);
        int32 y = self->position.y + RSDK.Rand(-0x600000, 0x600000);
        CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
    }

    if (!(Zone->timer % 6))
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 255);

    if ((Zone->timer & 0x3F) == 32)
        RSDK.PlaySfx(MSBomb->sfxExplosion, false, 255);

    foreach_active(Player, player)
    {
        if (player->position.x < self->position.x + 0x400000) {
            player->velocity.x = 0x40000;

            if (player->velocity.y >= 0) {
                player->velocity.y = -0x40000;
                player->onGround   = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            }
        }
    }

    if (GigaMetal->explodeTimer >= 240) {
        GigaMetal->explodeTimer = 0;
        EntityMetalSonic *metal = self->metalSonic;

        metal->position.x = self->position.x - 0x100000;
        metal->position.y = self->position.y - 0x200000;
        metal->active     = ACTIVE_NORMAL;
        metal->state      = MetalSonic_State_Defeated;
        metal->velocity.x = 0;
        metal->velocity.y = -0x40000;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_DEFEATED, &metal->metalSonicAnimator, false, 0);

        foreach_active(GigaMetal, part) { destroyEntity(part); }

        for (int32 i = 0; i < 0x40; ++i) {
            int32 x = metal->position.x + RSDK.Rand(-0x600000, 0x400000);
            int32 y = metal->position.y + RSDK.Rand(-0x600000, 0x600000);
            CREATE_ENTITY(GigaMetal, INT_TO_VOID(GIGAMETAL_SHARD), x, y);
        }

        EntityFXFade *fxFade  = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn       = 256;
        fxFade->speedOut      = 64;
        Zone->autoScrollSpeed = 0;
        RSDK.PlaySfx(MetalSonic->sfxExplosion3, false, 255);

        EntityDango *dango = CREATE_ENTITY(Dango, NULL, (ScreenInfo->position.x - 64) << 16, (ScreenInfo->position.y + 200) << 16);
        dango->timer       = 0;
        dango->direction   = FLIP_X;
        dango->state       = Dango_StateTaunt_Setup;

        int32 tileX = ScreenInfo->position.x >> 4;
        int32 tileY = ((ScreenInfo->size.y + ScreenInfo->position.y) >> 4) - 5;

        int32 spawnX = (tileX << 20) + 0x80000;
        for (int32 x = 0; x < 32; ++x) {
            int32 spawnY = (tileY << 20) + 0x80000;

            for (int32 y = 0; y < 6; ++y) {
                uint16 tile = RSDK.GetTile(Zone->fgLayer[1], tileX, tileY);

                if (tile != (uint16)-1) {
                    RSDK.SetTile(Zone->fgLayer[1], tileX, tileY, -1);
                    EntityBreakableWall *block = CREATE_ENTITY(BreakableWall, INT_TO_VOID(BREAKWALL_TILE_FIXED), spawnX, spawnY);
                    block->drawGroup           = Zone->objectDrawGroup[1];
                    block->visible             = true;
                    block->tileInfo            = tile;
                    block->velocity.x          = RSDK.Rand(-0x20000, 0x20000);
                    block->velocity.y          = RSDK.Rand(-0x20000, 0x20000);
                    block->drawFX              = FX_ROTATE | FX_FLIP;
                }

                ++tileY;
                spawnY += 0x100000;
            }

            spawnX += 0x100000;
            tileY -= 6;
            ++tileX;
        }

        RSDK.CopyTileLayer(Zone->fgLayer[0], 0, 30, Zone->fgLayer[0], 0, 174, 256, 2);
        RSDK.CopyTileLayer(Zone->fgLayer[1], 0, 26, Zone->fgLayer[0], 0, 0, 256, 6);
    }
    else {
        GigaMetal->explodeTimer++;
    }
}

void GigaMetal_StateShoulder_Impact(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y += 0x3800;
    self->componentPos.y += self->velocity.y;

    if (self->velocity.y > 0) {
        if (self->componentPos.y >= self->targetPos.y) {
            self->componentPos.y = self->targetPos.y;
            self->velocity.y     = -(self->velocity.y >> 2);
            if (self->velocity.y > -0x800) {
                self->velocity.y = 0;
                self->state      = StateMachine_None;
            }
        }
    }
}

void GigaMetal_StateHead_Impact(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y -= 0x3800;
    self->componentPos.y += self->velocity.y;

    if (self->velocity.y < 0) {
        if (self->componentPos.y <= self->targetPos.y) {
            self->componentPos.y = self->targetPos.y;
            self->velocity.y     = -(self->velocity.y >> 2);

            if (self->velocity.y < 0x4000) {
                self->velocity.y = 0;
                self->state      = StateMachine_None;
            }
        }
    }
}

void GigaMetal_State_Laser(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&self->jointAnimator);

    if (!(Zone->timer & 1)) {
        if (self->laserSize < 9)
            self->laserSize++;
    }

    self->position.x = self->body->position.x;
    self->position.y = self->body->position.y;

    int32 angle = self->angle - self->body->rotation;
    int32 x     = self->position.x + (RSDK.Sin512(angle + 8) << 12);
    int32 y     = self->position.y + (RSDK.Cos512(angle + 8) << 12);
    int32 moveX = (RSDK.Sin512(angle) << 12) & 0xFFFF0000;
    int32 moveY = (RSDK.Cos512(angle) << 12) & 0xFFFF0000;

    for (int32 c = 0; c < self->laserSize; ++c) {
        self->position.x = x;
        self->position.y = y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &GigaMetal->hitboxLaser) && Player_ElementHurt(player, self, SHIELD_LIGHTNING))
                player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
        }

        if (!(Zone->timer & 3)) {
            if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x20000, true))
                CREATE_ENTITY(Explosion, NULL, self->position.x, self->position.y);
        }

        x += moveX;
        y += moveY;
    }
}

void GigaMetal_StateLaser_Finish(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&self->jointAnimator);

    if (self->scale.y <= 8)
        destroyEntity(self);
    else
        self->scale.y -= 8;
}

void GigaMetal_State_Shard(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->angle += self->groundVel;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void GigaMetal_StateHead_PrepareRoar(void)
{
    RSDK_THIS(GigaMetal);

    self->drawFX = FX_ROTATE;

    if (self->rotation <= -32) {
        self->state = GigaMetal_StateHead_Roar;

        RSDK.PlaySfx(GigaMetal->sfxRoar, false, 255);
    }
    else {
        self->rotation -= 2;
    }
}

void GigaMetal_StateHead_Roar(void)
{
    RSDK_THIS(GigaMetal);

    self->rotation = RSDK.Rand(-34, -30);
}

void GigaMetal_StateHead_FinishRoar(void)
{
    RSDK_THIS(GigaMetal);

    if (self->rotation >= 0) {
        self->rotation = 0;
        self->drawFX   = FX_NONE;
        self->state    = StateMachine_None;
    }
    else {
        self->rotation += 2;
    }
}

void GigaMetal_StateHead_PrepareLaser(void)
{
    RSDK_THIS(GigaMetal);

    self->drawFX = FX_ROTATE;

    if (self->rotation <= -64) {
        RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASERCHARGE, &self->armAnimator, false, 0);
        self->state = GigaMetal_StateHead_ChargeLaser;

        RSDK.PlaySfx(GigaMetal->sfxRoar, false, 255);
    }
    else {
        self->rotation -= 2;
    }
}

void GigaMetal_StateHead_ChargeLaser(void)
{
    RSDK_THIS(GigaMetal);

    // Shaking effect
    self->rotation = RSDK.Rand(-66, -62);

    RSDK.ProcessAnimation(&self->armAnimator);

    if (++self->timer == 120) {
        self->timer                                                                                          = 0;
        self->state                                                                                          = GigaMetal_StateHead_FiringLaser;
        CREATE_ENTITY(GigaMetal, INT_TO_VOID(GIGAMETAL_LASEREDGE), self->position.x, self->position.y)->body = self;
        RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);
    }
}

void GigaMetal_StateHead_FiringLaser(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&self->armAnimator);

    if (self->rotation >= 0) {
        if (++self->timer == 2) {
            RSDK.SetSpriteAnimation(-1, 0, &self->armAnimator, false, 0);

            foreach_active(GigaMetal, laser)
            {
                if (laser->state == GigaMetal_State_Laser) {
                    laser->drawFX |= FX_SCALE;
                    laser->alpha = 0xA0;
                    laser->state = GigaMetal_StateLaser_Finish;
                }
            }
        }
        else if (self->timer == 30) {
            self->timer = 0;

            foreach_active(GigaMetal, arm)
            {
                if (arm->state == GigaMetal_StateArm_Idle_Behind) {
                    arm->angle = arm->body->angle;
                    if (arm->mainAnimator.animationID == GIGAMETAL_ARMBACK)
                        arm->angle += 0x80;
                    arm->timer = 48;
                    arm->state = GigaMetal_StateArm_Marching;
                }
            }

            self->rotation = 0;
            self->drawFX   = FX_NONE;
            self->state    = StateMachine_None;
        }
    }
    else {
        self->rotation++;
    }
}

void GigaMetal_StateHead_TargetingPlayer(void)
{
    RSDK_THIS(GigaMetal);

    ++self->timer;
    RSDK.ProcessAnimation(&self->jointAnimator);

    if (self->timer == 1) {
        RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_EYE, &self->jointAnimator, false, 3);
    }
    else if (self->timer == 88) {
        RSDK.SetSpriteAnimation(-1, 0, &self->jointAnimator, false, 0);
        self->targetPos = self->componentPos;
        self->timer     = 0;
        self->state     = GigaMetal_StateHead_ReleasingBombs;
    }

    bool32 playTargetingSfx = false;
    if (self->timer >= 72)
        playTargetingSfx = (self->timer & 7) == 1;
    else
        playTargetingSfx = (self->timer % 20) == 1;

    if (playTargetingSfx)
        RSDK.PlaySfx(GigaMetal->sfxTarget, false, 255);
}

void GigaMetal_StateHead_ReleasingBombs(void)
{
    RSDK_THIS(GigaMetal);

    self->timer++;
    if ((self->timer % 90) == 1) {
        self->velocity.x   = -0x8000;
        EntityMSBomb *bomb = CREATE_ENTITY(MSBomb, NULL, self->position.x + 0x100000, self->position.y + 0xC0000);

        bomb->velocity.x = 0x40000;
        bomb->velocity.y = 0x40000;
        RSDK.PlaySfx(GigaMetal->sfxCannon, false, 255);
    }

    self->componentPos.x += self->velocity.x;
    self->componentPos.y += self->velocity.x;
    self->velocity.x += 0x3800;

    if (self->componentPos.x > self->targetPos.x) {
        self->componentPos.x = self->targetPos.x;
        self->componentPos.y = self->targetPos.y;
        self->velocity.x     = 0;
    }

    if (self->timer == 224) {
        EntityGigaMetal *frontArm = self->frontArm;
        self->timer               = 0;
        frontArm->timer           = 48;
        frontArm->angle           = 0;
        frontArm->state           = GigaMetal_StateArm_Marching;

        EntityGigaMetal *backArm = self->backArm;
        backArm->timer           = 48;
        backArm->angle           = 128;
        backArm->state           = GigaMetal_StateArm_Marching;
        self->state              = 0;
    }
}

void GigaMetal_HandleArmCollisions(void)
{
    RSDK_THIS(GigaMetal);

    if (self->mainAnimator.animationID != GIGAMETAL_ARMBACK) {
        int32 angle = self->rotationAngles[0] >> 6;

        self->position.x = self->body->position.x + self->componentPos.x + (RSDK.Sin1024(angle) << 11);
        self->position.y = self->body->position.y + self->componentPos.y + (RSDK.Cos1024(angle) << 11);

        angle += self->rotationAngles[1] >> 6;
        self->position.x += 0x600 * RSDK.Sin1024(angle);
        self->position.y += 0x600 * RSDK.Cos1024(angle);
        self->position.x += RSDK.Sin1024(angle) << 9;

        angle += 0x100;
        self->position.y += RSDK.Cos1024(angle) << 9;
        self->position.x += 0x1500 * RSDK.Sin1024(angle);
        self->position.y += 0x1500 * RSDK.Cos1024(angle);

        angle += self->rotationAngles[2] >> 6;
        self->position.x += 0x600 * RSDK.Sin1024(angle);
        self->position.y += 0x600 * RSDK.Cos1024(angle);

        foreach_active(Player, player)
        {
            if (!GigaMetal->invincibleTimer) {
                if (Player_CheckCollisionTouch(player, self, &GigaMetal->hitboxHand)
                    && !Player_CheckMightyUnspin(player, 0x400, false, &player->uncurlTimer) && Player_Hurt(player, self)) {
                    player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
                }
            }
        }
    }
}

void GigaMetal_StateArm_Idle_Front(void)
{
    RSDK_THIS(GigaMetal);

    self->rotationAngles[0] = RSDK.Sin256(self->timer) - 0x800;
    self->rotationAngles[1] = RSDK.Sin256(self->timer) - 0x200;
    self->rotationAngles[2] = RSDK.Sin256(self->timer) + 0xC00;
    self->timer             = (self->timer + 2) & 0xFF;

    GigaMetal_HandleArmCollisions();
}

void GigaMetal_StateArm_Idle_Behind(void)
{
    RSDK_THIS(GigaMetal);

    if (self->rotationAngles[0] >= -0x1F00) {
        self->rotationAngles[0] += ((-0x2000 - self->rotationAngles[0]) >> 3);
        self->rotationAngles[1] += (-0x1000 - self->rotationAngles[1]) >> 3;
        self->rotationAngles[2] += ((0x1000 - self->rotationAngles[2]) >> 3);
    }
    else {
        self->rotationAngles[1] = RSDK.Rand(-0x1200, -0xE00);
    }

    GigaMetal_HandleArmCollisions();
}

void GigaMetal_StateArm_Marching(void)
{
    RSDK_THIS(GigaMetal);

    int32 angle1 = 16 * RSDK.Sin256(self->angle);
    int32 angle2 = 4 * RSDK.Sin256(self->angle);
    int32 angle3 = 4 * RSDK.Sin256(self->angle - 0x40);

    if (self->timer > 0) {
        angle1 = self->rotationAngles[0] + ((angle1 - self->rotationAngles[0]) >> 2);
        angle2 = self->rotationAngles[1] + ((angle2 - self->rotationAngles[1]) >> 2);
        angle3 = self->rotationAngles[2] + ((angle3 - self->rotationAngles[2]) >> 2);
        --self->timer;
    }

    self->rotationAngles[0] = angle1;
    self->rotationAngles[1] = angle2;
    self->rotationAngles[2] = angle3;
    self->angle             = (self->angle + 2) & 0xFF;

    GigaMetal_HandleArmCollisions();
}

#if GAME_INCLUDE_EDITOR
void GigaMetal_EditorDraw(void)
{
    RSDK_THIS(GigaMetal);

    switch (self->aniID) {
        case GIGAMETAL_HEAD:
            self->componentPos.x = 0x140000;
            self->componentPos.y = -0x240000;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_HEAD, &self->mainAnimator, true, 0);

            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;

        case GIGAMETAL_SHOULDER:
            self->componentPos.x = -0x240000;
            self->componentPos.y = -0x2C0000;
            self->drawGroup      = Zone->objectDrawGroup[1];
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_SHOULDER, &self->mainAnimator, true, 0);

            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;

        case GIGAMETAL_BODY:
            self->active = ACTIVE_BOUNDS;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_BODY, &self->mainAnimator, true, 0);

            RSDK.DrawSprite(&self->mainAnimator, NULL, false);

            if (showGizmos()) {
                RSDK_DRAWING_OVERLAY(true);

                DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER + 32, -SCREEN_YSIZE + 16, WIDE_SCR_XCENTER + 32, 16, 1 | 2 | 4 | 8, 0x00C0F0);

                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case GIGAMETAL_COVER:
            self->inkEffect = INK_ADD;
            self->alpha     = 0xFF;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_COVER, &self->mainAnimator, true, 0);

            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            break;

        case GIGAMETAL_ARMFRONT:
            self->componentPos.x = -0x1C0000;
            self->componentPos.y = -0x2C0000;
            self->drawGroup      = Zone->objectDrawGroup[1];

            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->jointAnimator, true, 1);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->armAnimator, true, 2);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->handAnimator, true, 3);

            self->body = self;
            GigaMetal_Draw_Arm();
            break;

        case GIGAMETAL_ARMBACK:
            self->componentPos.x = -0xC0000;
            self->componentPos.y = -0x2C0000;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->jointAnimator, true, 1);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->armAnimator, true, 2);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->handAnimator, true, 3);

            self->body = self;
            GigaMetal_Draw_Arm();
            break;

        case GIGAMETAL_LASEREDGE:
            self->drawFX    = FX_ROTATE;
            self->laserSize = -1;
            self->angle     = 0x40;
            self->visible   = true;
            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASERBEAM, &self->jointAnimator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->armAnimator, true, 1);

            self->body = self;
            GigaMetal_Draw_LaserEdge();
            break;

        case GIGAMETAL_SHARD: break;

        default: break;
    }
}

void GigaMetal_EditorLoad(void)
{
    GigaMetal->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/GigaMetal.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(GigaMetal, aniID);
    RSDK_ENUM_VAR("Head", GIGAMETAL_HEAD);
    RSDK_ENUM_VAR("Shoulder", GIGAMETAL_SHOULDER);
    RSDK_ENUM_VAR("Body", GIGAMETAL_BODY);
    RSDK_ENUM_VAR("Cover", GIGAMETAL_COVER);
    RSDK_ENUM_VAR("Eye", GIGAMETAL_EYE);
    RSDK_ENUM_VAR("Arm (Front)", GIGAMETAL_ARMFRONT);
    RSDK_ENUM_VAR("Arm (Back)", GIGAMETAL_ARMBACK);
}
#endif

void GigaMetal_Serialize(void)
{
    RSDK_EDITABLE_VAR(GigaMetal, VAR_UINT8, aniID);
    RSDK_EDITABLE_VAR(GigaMetal, VAR_UINT8, frameID);
}
#endif
