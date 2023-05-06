// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Firework Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFirework *Firework;

void Firework_Update(void)
{
    RSDK_THIS(Firework);

    if (self->mainAnimator.animationID < 3) {
        RSDK.ProcessAnimation(&self->mainAnimator);
    }
    else if (self->timer == 10) {
        if (self->mainAnimator.frameID < self->mainAnimator.frameCount - 1)
            RSDK.ProcessAnimation(&self->mainAnimator);

        if (self->sparkAnimator.frameID < self->sparkAnimator.frameCount - 1)
            RSDK.ProcessAnimation(&self->sparkAnimator);
    }

    StateMachine_Run(self->state);
    Firework_HandlePlayerTimers();
}

void Firework_LateUpdate(void) {}

void Firework_StaticUpdate(void)
{
    bool32 fireworkActive = false;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
        foreach_active(Firework, firework)
        {
            if (firework->rideActive) {
                if (!Firework->playingFlameSfx) {
                    RSDK.PlaySfx(Firework->sfxFlame, true, 255);
                    Firework->playingFlameSfx = true;
                }

                fireworkActive = true;
                foreach_break;
            }
        }
    }

    if (!fireworkActive) {
        if (Firework->playingFlameSfx) {
            RSDK.StopSfx(Firework->sfxFlame);
            Firework->playingFlameSfx = false;
        }
    }
}

void Firework_Draw(void)
{
    RSDK_THIS(Firework);

    self->rotation = ((self->angle >> 1) + 0x80) & 0x1FF;
    if (self->state == Firework_State_Explode) {
        if (self->timer > 0)
            RSDK.FillScreen(0xF0F0F0, self->timer - 128, self->timer - 256, self->timer);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, &self->position, false);
    }

    if (self->mainAnimator.animationID >= 3) {
        self->inkEffect = INK_ADD;
        RSDK.DrawSprite(&self->sparkAnimator, NULL, false);
        self->inkEffect = INK_NONE;
    }
}

void Firework_Create(void *data)
{
    RSDK_THIS(Firework);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!self->innerRadius)
        self->innerRadius = 64;

    if (!self->outerRadius)
        self->outerRadius = 128;

    if (!self->distance)
        self->distance = 512;

    self->startPos = self->position;

    self->state = Firework_State_Init;
}

void Firework_StageLoad(void)
{
    Firework->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Firework.bin", SCOPE_STAGE);

    Firework->hitboxFireworkGrab.left   = -11;
    Firework->hitboxFireworkGrab.top    = -32;
    Firework->hitboxFireworkGrab.right  = 10;
    Firework->hitboxFireworkGrab.bottom = 15;

    Firework->hitboxTileCheck.left   = -10;
    Firework->hitboxTileCheck.top    = -10;
    Firework->hitboxTileCheck.right  = 10;
    Firework->hitboxTileCheck.bottom = 10;

    Firework->sfxRocketJet  = RSDK.GetSfx("Stage/RocketJet.wav");
    Firework->sfxFlame      = RSDK.GetSfx("Stage/Flame2.wav");
    Firework->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    Firework->sfxExplosion  = RSDK.GetSfx("Stage/Explosion.wav");

    Firework->active = ACTIVE_ALWAYS;
}

void Firework_CheckPlayerCollisions(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (!((1 << playerID) & self->activePlayers)) {
            if (player->state != Player_State_Static && !self->playerTimers[playerID]
                && Player_CheckCollisionTouch(player, self, &Firework->hitboxFireworkGrab)) {
                player->tileCollisions = TILECOLLISION_NONE;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_PULLEY_HOLD, &player->animator, false, 0);

                player->state = Player_State_Static;
                self->activePlayers |= 1 << playerID;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);

                player->direction = player->position.x > self->position.x;
                if (player->sidekick == false)
                    self->activated = true;
            }
        }

        if ((1 << playerID) & self->activePlayers) {
            if (player->state == Player_State_Static) {
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = self->angle >> 1;

                if (player->direction == FLIP_NONE)
                    player->rotation = ((self->angle >> 1) + 0xC0) % 511;
                else
                    player->rotation = ((self->angle >> 1) + 0x444) % 511;

                player->position.x = self->position.x;
                player->position.y = self->position.y;

                int32 power = player->direction == FLIP_NONE ? 16 : -16;

                player->position.x += power * (RSDK.Sin1024(self->angle) << 6);
                player->position.y -= power * (RSDK.Cos1024(self->angle) << 6);
                player->position.x += -0x100 * RSDK.Cos1024(self->angle);
                player->position.y -= -0x100 * RSDK.Sin1024(self->angle);
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Firework_HandlePlayerTimers(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (self->playerTimers[playerID])
            self->playerTimers[playerID]--;
    }
}

void Firework_RemovePlayers(bool32 crashed)
{
    foreach_active(Player, player)
    {
        RSDK.GetEntitySlot(player); // so true tbh
        Firework_HandlePlayerRemoval(player, crashed);
    }
}

void Firework_HandlePlayerRemoval(EntityPlayer *player, bool32 crashed)
{
    RSDK_THIS(Firework);

    int32 playerID = RSDK.GetEntitySlot(player);

    if ((1 << playerID) & self->activePlayers) {
        player->state          = Player_State_Air;
        player->onGround       = false;
        player->tileCollisions = TILECOLLISION_DOWN;
        player->velocity       = self->velocity;

        self->playerTimers[playerID] = 30;
        if (crashed) {
            bool32 isSidekick = player->sidekick;
            player->sidekick  = true;
            Player_Hit(player);
            player->sidekick = isSidekick;
        }
        else {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        }

        self->activePlayers &= ~(1 << playerID);
    }
}

void Firework_HandlePlayerJump(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        if ((1 << RSDK.GetEntitySlot(player)) & self->activePlayers && player->jumpPress) {
            int32 playerID = RSDK.GetEntitySlot(player);

            if ((1 << playerID) & self->activePlayers) {
                player->state                = Player_State_Air;
                player->onGround             = false;
                player->tileCollisions       = TILECOLLISION_DOWN;
                player->velocity.x           = self->velocity.x;
                player->velocity.y           = self->velocity.y;
                self->playerTimers[playerID] = 30;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Firework_HandlePlayerControl(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        if (!player->sidekick && (1 << RSDK.GetEntitySlot(player)) & self->activePlayers) {
            if (player->left)
                --self->angle;
            else if (player->right)
                ++self->angle;
        }
    }
}

void Firework_HandleMoveDir(void)
{
    RSDK_THIS(Firework);

    if (self->moveVelocity < 0x80000)
        self->moveVelocity += 0x2000;

    if (self->moveVelocity > 0x80000)
        self->moveVelocity = 0x80000;

    self->velocity.x = (self->moveVelocity >> 10) * RSDK.Cos1024(self->angle);
    self->velocity.y = (self->moveVelocity >> 10) * RSDK.Sin1024(self->angle);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 rx = abs(self->lastPosition.x - self->position.x) >> 16;
    int32 ry = abs(self->lastPosition.y - self->position.y) >> 16;
    self->ridePos += MathHelpers_SquareRoot(rx * rx + ry * ry);

    self->lastPosition.x = self->position.x;
    self->lastPosition.y = self->position.y;
}

void Firework_HandleDebrisSpawn(void)
{
    RSDK_THIS(Firework);

    if (!(self->timer % 5)) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x, self->position.y);

        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->position.x -= 0x600 * RSDK.Cos1024(self->angle);
        debris->position.y -= 0x600 * RSDK.Sin1024(self->angle);
        debris->drawGroup = Zone->objectDrawGroup[0];
        debris->timer     = 52;
    }
}

void Firework_HandleTileCollisions(void)
{
    RSDK_THIS(Firework);

    bool32 collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_LWALL, 0, Firework->hitboxTileCheck.right << 13, 0, 4);
    collided |= RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, Firework->hitboxTileCheck.top << 13, 4);
    collided |= RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_RWALL, 0, Firework->hitboxTileCheck.left << 13, 0, 4);

    if (collided) {
        Camera_ShakeScreen(0, 0, 4);
        Firework_HandleRideEnd(true);
    }
}

void Firework_HandleRideEnd(bool32 crashed)
{
    RSDK_THIS(Firework);

    self->rideActive = false;

    if (crashed) {
        // Watch out! you're gonna crash! ah!
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
        RSDK.PlaySfx(Firework->sfxExplosion2, false, 255);

        Firework_RemovePlayers(true);
        self->state = Firework_State_Explode;
    }
    else {
        int32 angle = 0;

        for (int32 i = 0; i < 8; ++i) {
            int32 x = (RSDK.Cos1024(angle) << 6) * self->innerRadius + self->position.x;
            int32 y = (RSDK.Sin1024(angle) << 6) * self->innerRadius + self->position.y;

            EntityFirework *firework = CREATE_ENTITY(Firework, NULL, x, y);
            firework->velocity.x     = 0x30 * RSDK.Cos1024(angle);
            firework->velocity.y     = 0x30 * RSDK.Sin1024(angle);
            firework->state          = Firework_State_SparkSetup;
            firework->angle          = angle;
            firework->sparkID        = 0;

            x                    = (RSDK.Cos1024(angle) << 6) * self->outerRadius + self->position.x;
            y                    = (RSDK.Sin1024(angle) << 6) * self->outerRadius + self->position.y;
            firework             = CREATE_ENTITY(Firework, NULL, x, y);
            firework->velocity.x = 0x20 * RSDK.Cos1024(angle);
            firework->velocity.y = 0x20 * RSDK.Sin1024(angle);
            firework->angle      = angle;
            firework->state      = Firework_State_SparkSetup;
            firework->sparkID    = 1;

            angle += 0x80;
        }

        self->timer     = 512;
        self->drawGroup = Zone->hudDrawGroup;
        RSDK.PlaySfx(Firework->sfxExplosion, false, 255);

        Firework_RemovePlayers(false);
        self->state = Firework_State_Explode;
    }
}

void Firework_CheckOffScreen(void)
{
    RSDK_THIS(Firework);

    if (self->state == Firework_State_Explode) {
        if (self->timer <= 0) {
            if (!RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
                self->position  = self->startPos;
                self->state     = Firework_State_ResetOffScreen;
                self->visible   = false;
                self->direction = FLIP_NONE;
            }
        }
    }
    else {
        if (!RSDK.CheckOnScreen(self, NULL)) {
            if (!RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
                self->position  = self->startPos;
                self->state     = Firework_State_ResetOffScreen;
                self->visible   = false;
                self->direction = FLIP_NONE;
            }
        }
    }
}

void Firework_HandleSparkAnimations(int32 sparkID, int16 angle, Animator *fireworkAnimator, Animator *sparkAnimator, uint8 *direction)
{
    uint16 ang = angle & 0x3FF;
    int32 anim = 4 * (sparkID != 0) + 3;

    if (ang <= 0x40 || ang > 0x3C0) {
        if (direction)
            *direction = FLIP_NONE;

        if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim, fireworkAnimator, true, 0);
    }
    else if (ang <= 0xC0) {
        if (direction)
            *direction = FLIP_Y;

        if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, fireworkAnimator, true, 0);
    }
    else if (ang <= 0x140) {
        if (direction)
            *direction = FLIP_Y;

        if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 1, fireworkAnimator, true, 0);
    }
    else if (ang <= 0x1C0) {
        if (direction)
            *direction = FLIP_XY;

        if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, fireworkAnimator, true, 0);
    }
    else if (ang <= 0x240) {
        if (direction)
            *direction = FLIP_X;

        if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim, fireworkAnimator, true, 0);
    }
    else if (ang <= 0x2C0) {
        if (direction)
            *direction = FLIP_X;

        if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, fireworkAnimator, true, 0);
    }
    else if (ang > 0x340) {
        if (direction)
            *direction = FLIP_NONE;

        if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, fireworkAnimator, true, 0);
    }
    else {
        if (direction)
            *direction = FLIP_NONE;
        else if (fireworkAnimator)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 1, fireworkAnimator, true, 0);
    }

    if (sparkAnimator)
        RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 3, sparkAnimator, true, 0);
}

void Firework_State_Init(void)
{
    RSDK_THIS(Firework);

    self->position      = self->startPos;
    self->lastPosition  = self->startPos;
    self->visible       = true;
    self->activePlayers = 0;
    self->rotation      = 0;
    self->angle         = 0x300;
    self->timer         = 0;
    self->activated     = false;
    self->moveVelocity  = 0;
    self->ridePos       = 0;
    self->drawGroup     = Zone->objectDrawGroup[0];

    RSDK.SetSpriteAnimation(Firework->aniFrames, 0, &self->mainAnimator, true, 0);
    self->state = Firework_State_AwaitPlayerRide;
}

void Firework_State_AwaitPlayerRide(void)
{
    RSDK_THIS(Firework);

    Firework_CheckPlayerCollisions();

    if (self->activated) {
        RSDK.PlaySfx(Firework->sfxRocketJet, false, 255);
        RSDK.SetSpriteAnimation(Firework->aniFrames, 1, &self->mainAnimator, true, 0);
        self->timer = 30;
        self->state = Firework_State_PlayerGrabDelay;
    }
}

void Firework_State_PlayerGrabDelay(void)
{
    RSDK_THIS(Firework);

    if (self->timer <= 0) {
        self->rideActive = true;
        RSDK.SetSpriteAnimation(Firework->aniFrames, 2, &self->mainAnimator, true, 0);

        self->lastPosition = self->position;
        self->timer        = 0;
        self->state        = Firework_State_Riding;
    }
    else {
        self->timer--;
    }

    Firework_CheckPlayerCollisions();
    Firework_HandlePlayerJump();
}

void Firework_State_Riding(void)
{
    RSDK_THIS(Firework);

    Firework_HandleMoveDir();
    Firework_HandleDebrisSpawn();
    Firework_CheckPlayerCollisions();
    Firework_HandlePlayerJump();
    Firework_HandlePlayerControl();

    if (self->ridePos > self->distance)
        Firework_HandleRideEnd(false);

    Firework_HandleTileCollisions();
    Firework_CheckOffScreen();

    ++self->timer;
}

void Firework_State_Explode(void)
{
    RSDK_THIS(Firework);

    if (self->timer <= 0) {
        self->activePlayers = 0;
        Firework_CheckOffScreen();
        self->visible = false;
    }
    else {
        self->timer -= 0x40;
    }
}

void Firework_State_ResetOffScreen(void)
{
    RSDK_THIS(Firework);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->state = Firework_State_Init;
        Firework_Create(NULL);
    }
}

void Firework_State_SparkSetup(void)
{
    RSDK_THIS(Firework);

    self->visible  = true;
    self->rotation = 0;
    self->timer    = 0;
    self->state    = Firework_State_Spark;
}

void Firework_State_Spark(void)
{
    RSDK_THIS(Firework);

    if (self->timer >= 10) {
        if (self->timer == 10) {
            self->position.x += self->velocity.x;
            self->position.y += self->velocity.y;

            if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
                destroyEntity(self);
        }
    }
    else {
        Firework_HandleSparkAnimations(self->sparkID, self->angle, &self->mainAnimator, &self->sparkAnimator, &self->direction);
        ++self->timer;
    }
}

#if GAME_INCLUDE_EDITOR
void Firework_EditorDraw(void)
{
    RSDK_THIS(Firework);

    RSDK.SetSpriteAnimation(Firework->aniFrames, 0, &self->mainAnimator, false, 0);

    Firework_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x, self->position.y - (self->distance << 16), 0xFFFF00, INK_NONE,
                              0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Firework_EditorLoad(void) { Firework->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Firework.bin", SCOPE_STAGE); }
#endif

void Firework_Serialize(void)
{
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, distance);
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, innerRadius);
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, outerRadius);
}
