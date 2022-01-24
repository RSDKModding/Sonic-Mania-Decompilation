// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GigaMetal Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
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

    if (GigaMetal->field_8 <= 0) {
        if (GigaMetal->invincibleTimer & 1) {
            RSDK.CopyPalette(2, 32, 0, 32, 16);
            RSDK.CopyPalette(2, 160, 0, 160, 10);
            RSDK.CopyPalette(2, 240, 0, 240, 7);

            if (self->stateDraw) {
                StateMachine_Run(self->stateDraw);
            }
            else {
                RSDK.DrawSprite(&self->animator, NULL, false);
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
                RSDK.DrawSprite(&self->animator, NULL, false);
            }
        }
    }
    else {
        RSDK.SetLimitedFade(0, 4, 5, GigaMetal->field_8, 0, 256);
        if (self->stateDraw) {
            StateMachine_Run(self->stateDraw);
        }
        else {
            RSDK.DrawSprite(&self->animator, NULL, false);
        }
        RSDK.CopyPalette(4, 1, 0, 1, 255);
    }
}

void GigaMetal_Create(void *data)
{
    RSDK_THIS(GigaMetal);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->drawOrder     = Zone->drawOrderLow - 1;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            int slot              = RSDK.GetEntityID(self);
            if (data)
                self->aniID = voidToInt(data);

            switch (self->aniID) {
                case GIGAMETAL_HEAD:
                    self->body       = RSDK_GET_ENTITY(slot - 1, );
                    self->stateDraw  = GigaMetal_StateDraw0_Unknown;
                    self->field_8C.x = 0x140000;
                    self->field_8C.y = -0x240000;
                    self->frontArm   = RSDK_GET_ENTITY(slot + 3, );
                    self->backArm    = RSDK_GET_ENTITY(slot - 2, );
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_HEAD, &self->animator, true, 0);
                    break;
                case GIGAMETAL_SHOULDER:
                    self->body       = RSDK_GET_ENTITY(slot - 5, );
                    self->frontArm   = RSDK_GET_ENTITY(slot - 1, );
                    self->stateDraw  = GigaMetal_StateDraw1_Unknown;
                    self->field_8C.x = -0x240000;
                    self->field_8C.y = -0x2C0000;
                    self->drawOrder  = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_SHOULDER, &self->animator, true, 0);
                    break;
                case GIGAMETAL_BODY: {
                    self->active   = ACTIVE_BOUNDS;
                    self->head     = RSDK_GET_ENTITY(slot + 1, );
                    self->ruby     = RSDK_GET_ENTITY(slot + 2, );
                    self->cover    = RSDK_GET_ENTITY(slot + 3, );
                    self->frontArm = RSDK_GET_ENTITY(slot + 4, );
                    self->backArm  = RSDK_GET_ENTITY(slot - 1, );
                    self->shoulder = RSDK_GET_ENTITY(slot + 5, );
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_BODY, &self->animator, true, 0);
                    foreach_all(MetalSonic, metal) { self->metalSonic = (Entity *)metal; }
                    self->health = 8;
                    self->state  = GigaMetal_State_SetupBounds;
                    break;
                }
                case GIGAMETAL_COVER: {
                    self->body      = RSDK_GET_ENTITY(slot - 3, );
                    self->stateDraw = GigaMetal_StateDraw3_Unknown;
                    self->inkEffect = INK_ADD;
                    self->alpha     = 255;
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_COVER, &self->animator, true, 0);
                    break;
                }
                case GIGAMETAL_ARMFRONT: {
                    self->body       = RSDK_GET_ENTITY(slot - 4, );
                    self->stateDraw  = GigaMetal_StateDraw6_Unknown;
                    self->field_8C.x = -0x1C0000;
                    self->field_8C.y = -0x2C0000;
                    self->drawOrder  = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator2, true, 1);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator3, true, 2);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator4, true, 3);
                    break;
                }
                case GIGAMETAL_ARMBACK: {
                    self->body       = RSDK_GET_ENTITY(slot + 1, );
                    self->stateDraw  = GigaMetal_StateDraw6_Unknown;
                    self->field_8C.x = -0xC0000;
                    self->field_8C.y = -0x2C0000;
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator2, true, 1);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator3, true, 2);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator4, true, 3);
                    break;
                }
                case GIGAMETAL_LASEREDGE: {
                    self->stateDraw = GigaMetal_StateDraw8_Unknown;
                    self->drawFX    = FX_ROTATE;
                    self->field_10C = -1;
                    self->angle     = 64;
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->scale.x   = 0x200;
                    self->scale.y   = 0x200;
                    self->state     = GigaMetal_State8_Unknown;
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->animator, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASERBEAM, &self->animator2, true, 0);
                    RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->animator3, true, 1);
                    break;
                }
                case GIGAMETAL_SHARD: {
                    self->scale.x    = RSDK.Rand(0x200, 0x400);
                    self->scale.y    = RSDK.Rand(0x200, 0x400);
                    self->field_100  = RSDK.Rand(0, 0x400);
                    self->field_104  = RSDK.Rand(0, 0x400);
                    self->field_108  = RSDK.Rand(0, 0x400);
                    self->active     = ACTIVE_NORMAL;
                    self->visible    = true;
                    self->drawOrder  = Zone->drawOrderHigh;
                    self->velocity.x = RSDK.Rand(-0x10000, 0x10000);
                    self->velocity.y = RSDK.Rand(-0x40000, -0x10000);
                    self->groundVel  = RSDK.Rand(-16, 16);
                    self->state      = GigaMetal_State10_Unknown;
                    self->stateDraw  = GigaMetal_StateDraw10_Unknown;
                    break;
                }
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

    GigaMetal->hitbox1.left   = -12;
    GigaMetal->hitbox1.top    = -12;
    GigaMetal->hitbox1.right  = 12;
    GigaMetal->hitbox1.bottom = 12;

    GigaMetal->hitbox2.left   = -12;
    GigaMetal->hitbox2.top    = -10;
    GigaMetal->hitbox2.right  = 12;
    GigaMetal->hitbox2.bottom = 10;

    GigaMetal->hitbox3.left   = -40;
    GigaMetal->hitbox3.top    = -16;
    GigaMetal->hitbox3.right  = 40;
    GigaMetal->hitbox3.bottom = 40;

    GigaMetal->hitbox4.left   = -56;
    GigaMetal->hitbox4.top    = -124;
    GigaMetal->hitbox4.right  = 16;
    GigaMetal->hitbox4.bottom = -64;

    GigaMetal->sfxRoar   = RSDK.GetSfx("SSZ2/MSRoar.wav");
    GigaMetal->sfxImpact = RSDK.GetSfx("Stage/Impact6.wav");
    GigaMetal->sfxTarget = RSDK.GetSfx("SSZ2/MSTarget.wav");
    GigaMetal->sfxPimpom = RSDK.GetSfx("Stage/PimPom.wav");
    GigaMetal->sfxCannon = RSDK.GetSfx("SSZ2/MSCannon.wav");
    RSDK.CopyPalette(0, 1, 4, 1, 255);
    for (int i = 0; i < 256; ++i) RSDK.SetPaletteEntry(5, i, 0xF00080);
    GigaMetal->invincibleTimer = 0;
    GigaMetal->field_8         = 0;
    if (StarPost->postIDs[0]) {
        foreach_all(Music, music)
        {
            if (music->playOnLoad)
                music->trackID = TRACK_EGGMAN1;
        }
        Music->restartTrackID = TRACK_EGGMAN1;
    }
}

void GigaMetal_StateDraw3_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x = self->body->position.x + self->field_8C.x;
    self->position.y = self->body->position.y + self->field_8C.y;

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void GigaMetal_StateDraw0_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x = self->body->position.x + self->field_8C.x;
    self->position.y = self->body->position.y + self->field_8C.y;

    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

void GigaMetal_StateDraw1_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x = self->body->position.x + self->field_8C.x;
    self->position.y = self->body->position.y + self->field_8C.y;

    int x = 3 * (self->position.x >> 2) + ((self->frontArm->position.x - 0xC0000) >> 2);
    if (x > self->position.x)
        x = self->position.x;
    self->position.x = x;

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void GigaMetal_StateDraw6_Unknown(void)
{
    RSDK_THIS(GigaMetal);
    Vector2 drawPos;

    self->position.x = self->body->position.x + self->field_8C.x;
    self->position.y = self->body->position.y + self->field_8C.y;

    int angle          = self->field_100 >> 6;
    drawPos.x          = (RSDK.Sin1024(angle) << 11) + self->position.x;
    drawPos.y          = (RSDK.Cos1024(angle) << 11) + self->position.y;
    self->position.x = drawPos.x;
    self->position.y = drawPos.y;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    angle += (self->field_104 >> 6);
    drawPos.x += 0x600 * RSDK.Sin1024(angle);
    drawPos.y += 0x600 * RSDK.Cos1024(angle);

    if (self->animator.animationID == 5) {
        self->drawFX   = FX_ROTATE;
        self->rotation = -(angle >> 1);
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->drawFX = FX_NONE;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }
    else {
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->drawFX   = FX_ROTATE;
        self->rotation = -(angle >> 1);
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->drawFX = FX_NONE;
    }

    drawPos.x += RSDK.Sin1024(angle) << 9;
    drawPos.y += RSDK.Cos1024(angle) << 9;

    angle += 0x100;
    drawPos.x += 0x1500 * RSDK.Sin1024(angle);
    drawPos.y += 0x1500 * RSDK.Cos1024(angle);
    self->drawFX   = FX_ROTATE;
    self->rotation = -((angle + ((self->field_108 >> 6) - 256)) >> 1);
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    self->drawFX = FX_NONE;
}

void GigaMetal_StateDraw8_Unknown(void)
{
    RSDK_THIS(GigaMetal);
    Vector2 drawPos;

    self->position.x = self->body->position.x;
    self->position.y = self->body->position.y;

    int angle        = self->angle - self->body->rotation;
    self->rotation = self->angle + self->body->rotation;
    drawPos.x        = (RSDK.Sin512(angle + 8) << 12) + self->position.x;
    drawPos.y        = (RSDK.Cos512(angle + 8) << 12) + self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
    int moveX = (RSDK.Sin512(angle) << 12) & 0xFFFF0000;
    int moveY = ((RSDK.Cos512(angle)) << 12) & 0xFFFF0000;

    for (int i = 0; i < self->field_10C; ++i) {
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        drawPos.x += moveX;
        drawPos.y += moveY;
    }
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
}

void GigaMetal_StateDraw10_Unknown(void)
{
    RSDK_THIS(GigaMetal);
    Vector2 vertices[3];

    int x = ScreenInfo[SceneInfo->currentScreenID].position.x << 16;
    int y = ScreenInfo[SceneInfo->currentScreenID].position.y << 16;

    int angle     = self->angle + self->field_100;
    vertices[0].x = self->scale.x * RSDK.Sin1024(angle) - x + self->position.x;
    vertices[0].y = self->scale.y * RSDK.Cos1024(angle) + self->position.y - y;

    angle         = self->angle + self->field_104;
    vertices[1].x = self->scale.x * RSDK.Sin1024(angle) - x + self->position.x;
    vertices[1].y = self->scale.y * RSDK.Cos1024(angle) + self->position.y - y;

    angle         = self->angle + self->field_108;
    vertices[2].x = self->scale.x * RSDK.Sin1024(angle) - x + self->position.x;
    vertices[2].y = self->scale.y * RSDK.Cos1024(angle) + self->position.y - y;
    RSDK.DrawQuad(vertices, 3, 240, 0, 128, 256, INK_ADD);
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
        if (Player_CheckCollisionTouch(player, self, &GigaMetal->hitbox4) && player->velocity.x < 0 && Player_CheckBossHit(player, self)) {
            player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
            RSDK.PlaySfx(GigaMetal->sfxPimpom, false, 255);
        }
    }

    foreach_active(Player, playerPtr)
    {
        if (!GigaMetal->invincibleTimer && Player_CheckBadnikTouch(playerPtr, self, &GigaMetal->hitbox3) && Player_CheckBossHit(playerPtr, self)) {
            playerPtr->velocity.x += Zone->autoScrollSpeed;
            GigaMetal_Hit();
        }
    }
}

void GigaMetal_Hit(void)
{
    RSDK_THIS(GigaMetal);

    if (--self->health <= 0) {
        EntityGigaMetal *head     = (EntityGigaMetal *)self->head;
        EntityGigaMetal *frontArm = (EntityGigaMetal *)self->frontArm;
        EntityGigaMetal *backArm  = (EntityGigaMetal *)self->backArm;
        head->state               = StateMachine_None;
        frontArm->state           = StateMachine_None;
        backArm->state            = StateMachine_None;

        self->timer               = 0;
        self->state               = GigaMetal_State_Destroyed;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
    }
    else {
        GigaMetal->invincibleTimer = 40;
        RSDK.PlaySfx(MetalSonic->sfxHit, false, 255);

        EntityGigaMetal *cover = (EntityGigaMetal *)self->cover;
        ++cover->animator.frameID;
    }
}

void GigaMetal_State_SetupBounds(void)
{
    RSDK_THIS(GigaMetal);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.y < self->position.y && !player1->collisionPlane) {
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX + 32;
        Zone->cameraBoundsR[0]     = (self->position.x >> 16) + ScreenInfo->centerX + 32;
        Zone->cameraBoundsB[0]     = (self->position.y >> 16) + 16;
        Zone->deathBoundary[0]      = (self->position.y >> 16) + 16;

        EntityMetalSonic *metal = (EntityMetalSonic *)self->metalSonic;
        if (metal) {
            if (!metal->activeScreens) {
                metal->position.x = self->position.x - 0xC00000;
                metal->position.y = self->position.y + 0x400000;
            }
            metal->angle        = 0;
            metal->targetPos.x = self->position.x - 0x400000;
            metal->targetPos.y = self->position.y - 0x500000;
            metal->timer        = 0;
            metal->visible      = true;
            metal->active       = ACTIVE_NORMAL;
            metal->state        = MetalSonic_State_WaitForRuby;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &metal->animator, false, 0);
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &metal->animator2, false, 0);
        }
        self->position.x -= 0x500000;
        self->position.y -= 0x380000;
        self->active = ACTIVE_NORMAL;
        self->state  = GigaMetal_State_Unknown2;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
    }
}

void GigaMetal_State_Unknown2(void)
{
    RSDK_THIS(GigaMetal);

    EntityMetalSonic *metal = (EntityMetalSonic *)self->metalSonic;
    if (metal->active == ACTIVE_NEVER) {
        EntityGigaMetal *head = (EntityGigaMetal *)self->head;
        head->visible         = true;
        head->active          = ACTIVE_NORMAL;

        EntityPhantomRuby *ruby = (EntityPhantomRuby *)self->ruby;
        ruby->startPos.x        = self->position.x + 0x200000;
        ruby->startPos.y        = self->position.y + 0x100000;
        ruby->drawOrder         = self->drawOrder;
        ruby->position.x        = ruby->startPos.x;
        ruby->position.y        = ruby->startPos.y;

        EntityGigaMetal *cover = (EntityGigaMetal *)self->cover;
        cover->visible         = true;
        cover->active          = ACTIVE_NORMAL;

        EntityGigaMetal *shoulder = (EntityGigaMetal *)self->shoulder;
        shoulder->visible         = true;
        shoulder->active          = ACTIVE_NORMAL;

        EntityGigaMetal *frontArm = (EntityGigaMetal *)self->frontArm;
        frontArm->visible         = true;
        frontArm->active          = ACTIVE_NORMAL;
        frontArm->state           = GigaMetal_StateArm_Unknown1;

        EntityGigaMetal *backArm = (EntityGigaMetal *)self->backArm;
        backArm->visible         = true;
        backArm->active          = ACTIVE_NORMAL;
        backArm->state           = GigaMetal_StateArm_Unknown1;

        self->visible = true;
        self->state   = GigaMetal_State_Unknown3;
        RSDK.CopyTileLayer(Zone->fgHigh, 0, 34, Zone->fgHigh, 0, 26, 256, 6);

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

void GigaMetal_State_Unknown3(void)
{
    RSDK_THIS(GigaMetal);

    if (++self->timer == 120) {
        EntityGigaMetal *head = (EntityGigaMetal *)self->head;
        head->state           = GigaMetal_StateHead_Unknown1;

        EntityGigaMetal *frontArm = (EntityGigaMetal *)self->frontArm;
        frontArm->state           = GigaMetal_StateArm_Unknown2;

        EntityGigaMetal *backArm = (EntityGigaMetal *)self->backArm;
        backArm->state           = GigaMetal_StateArm_Unknown2;

        self->timer            = 0;
        self->state            = GigaMetal_State_Unknown4;
    }
}

void GigaMetal_State_Unknown4(void)
{
    RSDK_THIS(GigaMetal);

    ++self->timer;
    if (!(self->timer & 7))
        Camera_ShakeScreen(0, 0, 4);
    if (self->timer == 120) {
        EntityGigaMetal *head = (EntityGigaMetal *)self->head;
        head->state           = GigaMetal_StateHead_Unknown7;

        EntityGigaMetal *frontArm = (EntityGigaMetal *)self->frontArm;
        frontArm->timer           = 48;
        frontArm->state           = GigaMetal_StateArm_Unknown3;

        EntityGigaMetal *backArm = (EntityGigaMetal *)self->backArm;
        backArm->timer           = 48;
        backArm->angle           = 128;
        backArm->state           = GigaMetal_StateArm_Unknown3;

        self->field_94.x = self->position.x;
        self->field_94.y = self->position.y;
        self->field_94.y += 0x80000;
        self->timer         = 320;
        self->velocity.x    = 0x20000;
        self->state         = GigaMetal_State_Unknown5;
        Zone->autoScrollSpeed = 0x20000;
    }
}

void GigaMetal_State_Unknown5(void)
{
    RSDK_THIS(GigaMetal);

    self->position.x += self->velocity.x;

    if ((RSDK.Sin256(self->angle) & 0x200000) != 0)
        self->position.y = self->field_94.y - -(RSDK.Sin256(self->angle) << 10);
    else
        self->position.y = self->field_94.y - (RSDK.Sin256(self->angle) << 10);
    self->angle = (self->angle + 2) & 0xFF;

    EntityPhantomRuby *ruby = (EntityPhantomRuby *)self->ruby;
    ruby->position.x        = self->position.x + 0x200000;
    ruby->startPos.y        = self->position.y + 0x100000;

    EntityGigaMetal *head = (EntityGigaMetal *)self->head;
    if (!(self->angle & 0x7F)) {
        if (!head->state) {
            head->field_94.x = head->field_8C.x;
            head->field_94.y = head->field_8C.y;
            head->velocity.y = 0x10000;
            head->state      = GigaMetal_StateHead_Unknown3;
            RSDK.PlaySfx(GigaMetal->sfxImpact, false, 255);
        }

        EntityGigaMetal *shoulder = (EntityGigaMetal *)self->shoulder;
        shoulder->field_94.x      = shoulder->field_8C.x;
        shoulder->field_94.y      = shoulder->field_8C.y;
        shoulder->velocity.y      = -0x12000;
        shoulder->state           = GigaMetal_StateShoulder_Unknown1;
        Camera_ShakeScreen(0, 0, 4);
    }

    if (!head->state || head->state == GigaMetal_StateHead_Unknown3)
        --self->timer;
    if (self->timer <= 0) {
        EntityGigaMetal *frontArm = (EntityGigaMetal *)self->frontArm;
        EntityGigaMetal *backArm  = (EntityGigaMetal *)self->backArm;

        self->timer = RSDK.Rand(240, 480);
        if (RSDK.Rand(0, 1024) <= 512) {
            if ((self->field_88 & 0xFFFFFF00) != 0)
                self->field_88 = 0;
            else
                self->field_88++;

            if ((self->field_88 & 0xFF) > 2)
                self->field_88 = 256;
        }
        else {
            if ((self->field_88 & 0xFFFFFF00) == 256)
                ++self->field_88;
            else
                self->field_88 = 256;

            if ((self->field_88 & 0xFF) > 2)
                self->field_88 = 0;
        }

        if ((self->field_88 & 0xFFFFFF00) == 256)
            head->state = GigaMetal_StateHead_Unknown8;
        else
            head->state = GigaMetal_StateHead_Unknown4;

        frontArm->state = GigaMetal_StateArm_Unknown2;
        backArm->state  = GigaMetal_StateArm_Unknown2;
    }
    GigaMetal_HandleCameraMovement();
    GigaMetal_CheckPlayerCollisions();

    if ((Zone->timer & 7) == 2) {
        int endX = ((ScreenInfo->width + ScreenInfo->position.x) >> 4) + 1;
        int endY = (ScreenInfo->height + ScreenInfo->position.y) >> 4;

        RSDK.CopyTileLayer(Zone->fgHigh, endX, endY - 5, Zone->fgHigh, endX, endY - 5 + 8, 1, 6);
        if (endX > 224)
            RSDK.CopyTileLayer(Zone->fgHigh, endX - 224, endY - 5, Zone->fgHigh, endX - 224, endY - 5 + 8, 1, 6);

        int tileX  = (ScreenInfo->position.x >> 4) + 8;
        int spawnY = (endY << 20) + 0x80000;

        for (int delay = 4; delay < 40; delay += 6) {
            uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, tileX, endY);

            if (tile != (uint16)-1) {
                EntityBreakableWall *block = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_DYNAMIC), (tileX << 20) + 0x80000, spawnY);
                block->drawOrder           = Zone->drawOrderHigh;
                block->layerID             = Zone->fgHigh;
                block->tileInfo            = tile;
                block->tilePos.x           = tileX;
                block->tilePos.y           = endY;
                block->timer               = delay;
                block->active              = ACTIVE_NORMAL;
                if (tileX > 224) {
                    block                = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_DYNAMIC), (tileX << 20) - 0xDF80000, spawnY);
                    block->drawOrder     = Zone->drawOrderHigh;
                    block->layerID       = Zone->fgHigh;
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

void GigaMetal_State_Destroyed(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y += 0x1000;
    self->position.y += self->velocity.y;
    if (self->position.y >= self->field_94.y) {
        EntityGigaMetal *head = (EntityGigaMetal *)self->head;
        self->velocity.y    = -0xC000;
        head->velocity.y      = 0x18000;
        head->field_94        = head->field_8C;
        head->state           = GigaMetal_StateHead_Unknown3;

        EntityGigaMetal *shoulder = (EntityGigaMetal *)self->shoulder;
        shoulder->velocity.y      = -0x18000;
        shoulder->field_94        = shoulder->field_8C;
        shoulder->state           = GigaMetal_StateShoulder_Unknown1;
        Camera_ShakeScreen(0, 0, 4);
    }

    EntityPhantomRuby *ruby = (EntityPhantomRuby *)self->ruby;
    ruby->position.x        = self->position.x + 0x200000;
    ruby->startPos.y        = self->position.y + 0x100000;

    if (!(Zone->timer & 7)) {
        int x = self->position.x + RSDK.Rand(-0x600000, 0x600000);
        int y = self->position.y + RSDK.Rand(-0x600000, 0x600000);
        CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
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

    if (GigaMetal->field_8 >= 240) {
        GigaMetal->field_8      = 0;
        EntityMetalSonic *metal = (EntityMetalSonic *)self->metalSonic;

        metal->position.x = self->position.x - 0x100000;
        metal->position.y = self->position.y - 0x200000;
        metal->active     = ACTIVE_NORMAL;
        metal->state           = MetalSonic_State_Defeated;
        metal->velocity.x = 0;
        metal->velocity.y = -0x40000;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 10, &metal->animator, false, 0);

        foreach_active(GigaMetal, part) { destroyEntity(part); }

        for (int i = 0; i < 0x40; ++i) {
            int x = metal->position.x + RSDK.Rand(-0x600000, 0x400000);
            int y = metal->position.y + RSDK.Rand(-0x600000, 0x600000);
            CREATE_ENTITY(GigaMetal, intToVoid(GIGAMETAL_SHARD), x, y);
        }

        EntityFXFade *fxFade  = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn       = 256;
        fxFade->speedOut      = 64;
        Zone->autoScrollSpeed = 0;
        RSDK.PlaySfx(MetalSonic->sfxExplosion3, false, 255);

        EntityDango *dango = CREATE_ENTITY(Dango, NULL, (ScreenInfo->position.x - 64) << 16, (ScreenInfo->position.y + 200) << 16);
        dango->timer       = 0;
        dango->direction   = FLIP_X;
        dango->state       = Dango_StateTaunt_Unknown1;

        int32 tileX = ScreenInfo->position.x >> 4;
        int32 tileY = ((ScreenInfo->height + ScreenInfo->position.y) >> 4) - 5;

        int32 spawnX = (tileX << 20) + 0x80000;
        for (int x = 0; x < 32; ++x) {
            int32 spawnY = (tileY << 20) + 0x80000;
            for (int32 y = 0; y < 6; ++y) {
                uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, tileX, tileY);
                if (tile != (uint16)-1) {
                    RSDK.SetTileInfo(Zone->fgHigh, tileX, tileY, -1);
                    EntityBreakableWall *block = CREATE_ENTITY(BreakableWall, intToVoid(BREAKWALL_TILE_FIXED), spawnX, spawnY);
                    block->drawOrder           = Zone->drawOrderHigh;
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

        RSDK.CopyTileLayer(Zone->fgLow, 0, 30, Zone->fgLow, 0, 174, 256, 2);
        RSDK.CopyTileLayer(Zone->fgHigh, 0, 26, Zone->fgLow, 0, 0, 256, 6);
    }
    else {
        GigaMetal->field_8++;
    }
}

void GigaMetal_StateShoulder_Unknown1(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y += 0x3800;
    self->field_8C.y += self->velocity.y;
    if (self->velocity.y > 0) {
        if (self->field_8C.y >= self->field_94.y) {
            self->field_8C.y = self->field_94.y;
            self->velocity.y = -(self->velocity.y >> 2);
            if (self->velocity.y > -0x800) {
                self->velocity.y = 0;
                self->state      = StateMachine_None;
            }
        }
    }
}

void GigaMetal_StateHead_Unknown3(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y -= 0x3800;
    self->field_8C.y += self->velocity.y;
    if (self->velocity.y < 0) {
        if (self->field_8C.y <= self->field_94.y) {
            self->field_8C.y = self->field_94.y;
            self->velocity.y = -(self->velocity.y >> 2);
            if (self->velocity.y < 0x4000) {
                self->velocity.y = 0;
                self->state      = StateMachine_None;
            }
        }
    }
}

void GigaMetal_State8_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&self->animator2);
    if (!(Zone->timer & 1)) {
        if (self->field_10C < 9)
            self->field_10C++;
    }

    self->position.x = self->body->position.x;
    self->position.y = self->body->position.y;
    int angle          = self->angle - self->body->rotation;
    int x              = (RSDK.Sin512(angle + 8) << 12) + self->position.x;
    int y              = (RSDK.Cos512(angle + 8) << 12) + self->position.y;
    int moveX          = (RSDK.Sin512(angle) << 12) & 0xFFFF0000;
    int moveY          = (RSDK.Cos512(angle) << 12) & 0xFFFF0000;

    for (int c = 0; c < self->field_10C; ++c) {
        self->position.x = x;
        self->position.y = y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &GigaMetal->hitbox1) && Player_CheckElementalHit(player, self, SHIELD_LIGHTNING)) {
                player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
            }
        }

        if (!(Zone->timer & 3)) {
            if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x20000, true))
                CREATE_ENTITY(Explosion, NULL, self->position.x, self->position.y);
        }

        x += moveX;
        y += moveY;
    }
}

void GigaMetal_StateHead_Unknown10(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&self->animator2);

    if (self->scale.y <= 8)
        destroyEntity(self);
    else
        self->scale.y -= 8;
}

void GigaMetal_State10_Unknown(void)
{
    RSDK_THIS(GigaMetal);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->angle += self->groundVel;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void GigaMetal_StateHead_Unknown1(void)
{
    RSDK_THIS(GigaMetal);

    self->drawFX = FX_ROTATE;
    if (self->rotation <= -32) {
        self->state = GigaMetal_StateHead_Unknown2;
        RSDK.PlaySfx(GigaMetal->sfxRoar, false, 255);
    }
    else {
        self->rotation -= 2;
    }
}

void GigaMetal_StateHead_Unknown2(void)
{
    RSDK_THIS(GigaMetal);

    self->rotation = RSDK.Rand(-34, -30);
}

void GigaMetal_StateHead_Unknown7(void)
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

void GigaMetal_StateHead_Unknown4(void)
{
    RSDK_THIS(GigaMetal);

    self->drawFX = FX_ROTATE;
    if (self->rotation <= -64) {
        RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASERCHARGE, &self->animator3, false, 0);
        self->state = GigaMetal_StateHead_Unknown5;
        RSDK.PlaySfx(GigaMetal->sfxRoar, false, 255);
    }
    else {
        self->rotation -= 2;
    }
}

void GigaMetal_StateHead_Unknown5(void)
{
    RSDK_THIS(GigaMetal);

    self->rotation = RSDK.Rand(-66, -62);
    RSDK.ProcessAnimation(&self->animator3);
    if (++self->timer == 120) {
        self->timer                                                                                          = 0;
        self->state                                                                                          = GigaMetal_StateHead_Unknown6;
        CREATE_ENTITY(GigaMetal, intToVoid(GIGAMETAL_LASEREDGE), self->position.x, self->position.y)->body   = (Entity *)self;
        RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);
    }
}

void GigaMetal_StateHead_Unknown6(void)
{
    RSDK_THIS(GigaMetal);

    RSDK.ProcessAnimation(&self->animator3);

    if (self->rotation >= 0) {
        if (++self->timer == 2) {
            RSDK.SetSpriteAnimation(-1, 0, &self->animator3, false, 0);

            foreach_active(GigaMetal, arm)
            {
                if (arm->state == GigaMetal_State8_Unknown) {
                    arm->drawFX |= FX_SCALE;
                    arm->alpha = 0xA0;
                    arm->state = GigaMetal_StateHead_Unknown10;
                }
            }
        }
        else if (self->timer == 30) {
            self->timer = 0;
            foreach_active(GigaMetal, arm)
            {
                if (arm->state == GigaMetal_StateArm_Unknown2) {
                    arm->angle = arm->body->angle;
                    if (arm->animator.animationID == 6)
                        arm->angle += 128;
                    arm->timer = 48;
                    arm->state = GigaMetal_StateArm_Unknown3;
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

void GigaMetal_StateHead_Unknown8(void)
{
    RSDK_THIS(GigaMetal);

    ++self->timer;
    RSDK.ProcessAnimation(&self->animator2);

    if (self->timer == 1) {
        RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_EYE, &self->animator2, false, 3);
    }
    else if (self->timer == 88) {
        RSDK.SetSpriteAnimation(-1, 0, &self->animator2, false, 0);
        self->field_94 = self->field_8C;
        self->timer    = 0;
        self->state    = GigaMetal_StateHead_Unknown9;
    }

    bool32 flag = false;
    if (self->timer >= 72)
        flag = (self->timer & 7) == 1;
    else
        flag = (self->timer % 20) == 1;

    if (flag)
        RSDK.PlaySfx(GigaMetal->sfxTarget, false, 255);
}

void GigaMetal_StateHead_Unknown9(void)
{
    RSDK_THIS(GigaMetal);

    self->timer++;
    if ((self->timer % 90) == 1) {
        self->velocity.x = -0x8000;
        EntityMSBomb *bomb = CREATE_ENTITY(MSBomb, NULL, self->position.x + 0x100000, self->position.y + 0xC0000);
        bomb->velocity.x   = 0x40000;
        bomb->velocity.y   = 0x40000;
        RSDK.PlaySfx(GigaMetal->sfxCannon, false, 255);
    }

    self->field_8C.x += self->velocity.x;
    self->field_8C.y += self->velocity.x;
    self->velocity.x += 0x3800;
    if (self->field_8C.x > self->field_94.x) {
        self->field_8C.x = self->field_94.x;
        self->field_8C.y = self->field_94.y;
        self->velocity.x = 0;
    }

    if (self->timer == 224) {
        EntityGigaMetal *frontArm = (EntityGigaMetal *)self->frontArm;
        self->timer             = 0;
        frontArm->timer           = 48;
        frontArm->angle           = 0;
        frontArm->state           = GigaMetal_StateArm_Unknown3;

        EntityGigaMetal *backArm = (EntityGigaMetal *)self->backArm;
        backArm->timer           = 48;
        backArm->angle           = 128;
        backArm->state           = GigaMetal_StateArm_Unknown3;
        self->state            = 0;
    }
}

void GigaMetal_HandleArmCollisions(void)
{
    RSDK_THIS(GigaMetal);

    if (self->animator.animationID != 6) {
        int angle          = self->field_100 >> 6;
        self->position.x = self->body->position.x;
        self->position.y = self->body->position.y;
        self->position.x += self->field_8C.x;
        self->position.y += self->field_8C.y;
        self->position.x += RSDK.Sin1024(angle) << 11;
        self->position.y += RSDK.Cos1024(angle) << 11;

        angle += self->field_104 >> 6;
        self->position.x += 0x600 * RSDK.Sin1024(angle);
        self->position.y += 0x600 * RSDK.Cos1024(angle);
        self->position.x += RSDK.Sin1024(angle) << 9;

        angle += 256;
        self->position.y += RSDK.Cos1024(angle) << 9;
        self->position.x += 0x1500 * RSDK.Sin1024(angle);
        self->position.y += 0x1500 * RSDK.Cos1024(angle);

        angle += self->field_108 >> 6;
        self->position.x += 0x600 * RSDK.Sin1024(angle);
        self->position.y += 0x600 * RSDK.Cos1024(angle);

        foreach_active(Player, player)
        {
            if (!GigaMetal->invincibleTimer) {
                if (Player_CheckCollisionTouch(player, self, &GigaMetal->hitbox2)
                    && !Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer) && Player_CheckHit(player, self)) {
                    player->velocity.x = abs(player->velocity.x) + Zone->autoScrollSpeed;
                }
            }
        }
    }
}

void GigaMetal_StateArm_Unknown1(void)
{
    RSDK_THIS(GigaMetal);

    self->field_100 = RSDK.Sin256(self->timer) - 0x800;
    self->field_104 = RSDK.Sin256(self->timer) - 0x200;
    self->field_108 = RSDK.Sin256(self->timer) + 0xC00;
    self->timer     = (self->timer + 2) & 0xFF;
    GigaMetal_HandleArmCollisions();
}

void GigaMetal_StateArm_Unknown2(void)
{
    RSDK_THIS(GigaMetal);

    if (self->field_100 >= -0x1F00) {
        self->field_100 += ((-0x2000 - self->field_100) >> 3);
        self->field_104 += (-0x1000 - self->field_104) >> 3;
        self->field_108 += ((0x1000 - self->field_108) >> 3);
    }
    else {
        self->field_104 = RSDK.Rand(-0x1200, -0xE00);
    }
    GigaMetal_HandleArmCollisions();
}

void GigaMetal_StateArm_Unknown3(void)
{
    RSDK_THIS(GigaMetal);

    int val1 = 16 * RSDK.Sin256(self->angle);
    int val2 = 4 * RSDK.Sin256(self->angle);
    int val3 = 4 * RSDK.Sin256(self->angle - 64);
    if (self->timer > 0) {
        val2 = self->field_104 + ((val2 - self->field_104) >> 2);
        val1 = self->field_100 + ((val1 - self->field_100) >> 2);
        val3 = self->field_108 + ((val3 - self->field_108) >> 2);
        --self->timer;
    }

    self->field_100 = val1;
    self->field_104 = val2;
    self->field_108 = val3;
    self->angle     = (self->angle + 2) & 0xFF;
    GigaMetal_HandleArmCollisions();
}

#if RETRO_INCLUDE_EDITOR
void GigaMetal_EditorDraw(void)
{
    RSDK_THIS(GigaMetal);

    switch (self->aniID) {
        case GIGAMETAL_HEAD:
            self->stateDraw  = GigaMetal_StateDraw0_Unknown;
            self->field_8C.x = 0x140000;
            self->field_8C.y = -0x240000;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_HEAD, &self->animator, true, 0);

            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case GIGAMETAL_SHOULDER:
            self->stateDraw  = GigaMetal_StateDraw1_Unknown;
            self->field_8C.x = -0x240000;
            self->field_8C.y = -0x2C0000;
            self->drawOrder  = Zone->drawOrderHigh;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_SHOULDER, &self->animator, true, 0);

            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case GIGAMETAL_BODY:
            self->active   = ACTIVE_BOUNDS;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_BODY, &self->animator, true, 0);
            self->health = 8;
            self->state  = GigaMetal_State_SetupBounds;

            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case GIGAMETAL_COVER:
            self->inkEffect = INK_ADD;
            self->alpha     = 255;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_COVER, &self->animator, true, 0);

            RSDK.DrawSprite(&self->animator, NULL, false);
            break;
        case GIGAMETAL_ARMFRONT:
            self->stateDraw  = GigaMetal_StateDraw6_Unknown;
            self->field_8C.x = -0x1C0000;
            self->field_8C.y = -0x2C0000;
            self->drawOrder  = Zone->drawOrderHigh;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator2, true, 1);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator3, true, 2);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMFRONT, &self->animator4, true, 3);

            self->body = (Entity *)self;
            StateMachine_Run(self->stateDraw);
            break;
        case GIGAMETAL_ARMBACK:
            self->stateDraw  = GigaMetal_StateDraw6_Unknown;
            self->field_8C.x = -0xC0000;
            self->field_8C.y = -0x2C0000;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator2, true, 1);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator3, true, 2);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_ARMBACK, &self->animator4, true, 3);

            self->body = (Entity *)self;
            StateMachine_Run(self->stateDraw);
            break;
        case GIGAMETAL_LASEREDGE:
            self->stateDraw = GigaMetal_StateDraw8_Unknown;
            self->drawFX    = FX_ROTATE;
            self->field_10C = -1;
            self->angle     = 64;
            self->visible   = true;
            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASERBEAM, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(GigaMetal->aniFrames, GIGAMETAL_LASEREDGE, &self->animator3, true, 1);

            self->body = (Entity *)self;
            StateMachine_Run(self->stateDraw);
            break;
        case GIGAMETAL_SHARD:
            break;
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
