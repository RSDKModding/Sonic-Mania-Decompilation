// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TurboSpiker Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTurboSpiker *TurboSpiker;

// back in 3mixed, i was assigned this object
// i was never able to get it properly working
// here it is. :smirk2: -rmg
// hello rmg, this is rdc. there were MULTIPLE easy to find/fix bugs here so don't u dare smirk

void TurboSpiker_Update(void)
{
    RSDK_THIS(TurboSpiker);

    StateMachine_Run(self->state);
}

void TurboSpiker_LateUpdate(void) {}

void TurboSpiker_StaticUpdate(void) {}

void TurboSpiker_Draw(void)
{
    RSDK_THIS(TurboSpiker);

    RSDK.DrawSprite(&self->shellAnimator, NULL, false);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TurboSpiker_Create(void *data)
{
    RSDK_THIS(TurboSpiker);

    self->playedLaunchSfx = false;
    self->visible         = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup     = Zone->objectDrawGroup[0] + 1;
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->startDir      = self->direction;

    if (data) {
        self->active = ACTIVE_NORMAL;
    }
    else {
        self->unused = 16;
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 1, &self->shellAnimator, true, 0);
        self->state = TurboSpiker_State_Init;
    }
}

void TurboSpiker_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HCZ"))
        TurboSpiker->aniFrames = RSDK.LoadSpriteAnimation("HCZ/TurboSpiker.bin", SCOPE_STAGE);

    TurboSpiker->hitboxBadnik.left   = -14;
    TurboSpiker->hitboxBadnik.top    = -14;
    TurboSpiker->hitboxBadnik.right  = 14;
    TurboSpiker->hitboxBadnik.bottom = 14;

    TurboSpiker->hitboxSpike.left   = -5;
    TurboSpiker->hitboxSpike.top    = -31;
    TurboSpiker->hitboxSpike.right  = 9;
    TurboSpiker->hitboxSpike.bottom = 2;

    TurboSpiker->hitboxRange.left   = -128;
    TurboSpiker->hitboxRange.top    = -512;
    TurboSpiker->hitboxRange.right  = 128;
    TurboSpiker->hitboxRange.bottom = 512;

    TurboSpiker->sfxLaunch = RSDK.GetSfx("Stage/Launch.wav");
    TurboSpiker->sfxSplash = RSDK.GetSfx("Stage/Splash.wav");

    DEBUGMODE_ADD_OBJ(TurboSpiker);
}

void TurboSpiker_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void TurboSpiker_DebugSpawn(void)
{
    RSDK_THIS(TurboSpiker);
    CREATE_ENTITY(TurboSpiker, NULL, self->position.x, self->position.y);
}

void TurboSpiker_Hermit_Collide(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &TurboSpiker->hitboxBadnik) && Player_CheckBadnikBreak(player, self, false)) {
            if (self->spike)
                destroyEntity(self->spike);

            destroyEntity(self);
        }
    }
}

void TurboSpiker_Hermit_CheckOffScreen(void)
{
    RSDK_THIS(TurboSpiker);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        if (self->spike)
            destroyEntity(self->spike);

        self->direction = self->startDir;
        TurboSpiker_Create(NULL);
    }
}

void TurboSpiker_HandleMovement(void)
{
    RSDK_THIS(TurboSpiker);

    self->position.x += self->velocity.x;

    if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 1, &self->shellAnimator, true, 0);
        self->timer = 0;
        if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x20000 * (self->direction ? -1 : 1), 0x80000, 8))
            self->state = TurboSpiker_State_Turning;
        else
            self->state = TurboSpiker_State_Falling;
    }
}

void TurboSpiker_State_Init(void)
{
    RSDK_THIS(TurboSpiker);

    self->active = ACTIVE_NORMAL;

    if (self->type == TURBOSPIKER_HIDDEN) {
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 1, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 3, &self->shellAnimator, true, 0);
        self->drawGroup = Zone->fgDrawGroup[0] + 1;
        if (Player_GetNearestPlayer())
            self->direction = Player_GetNearestPlayer()->position.x >= self->position.x;
    }

    EntityTurboSpiker *spike = CREATE_ENTITY(TurboSpiker, INT_TO_VOID(true), self->position.x, self->position.y);
    spike->isPermanent       = true;
    spike->direction         = self->direction;
    spike->drawGroup         = self->drawGroup - 1;
    RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 3, &spike->shellAnimator, true, 0);
    spike->state = TurboSpiker_HandleSpikeCollisions;
    self->spike  = spike;

    if (self->type == TURBOSPIKER_VISIBLE) {
        self->timer      = 64;
        self->state      = TurboSpiker_State_Idle;
        self->velocity.x = 0x10000 * (self->direction ? 1 : -1);
        TurboSpiker_State_Idle();
        self->velocity.y = 0;
    }
    else {
        self->state = TurboSpiker_State_Hidden;
        TurboSpiker_State_Hidden();
        self->velocity.y = 0;
    }
}
void TurboSpiker_State_Idle(void)
{
    RSDK_THIS(TurboSpiker);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &TurboSpiker->hitboxRange)) {
            self->timer = 1;
            self->state = TurboSpiker_State_FireShell;
            TurboSpiker_Hermit_Collide();
            TurboSpiker_Hermit_CheckOffScreen();
            foreach_return;
        }
    }

    TurboSpiker_HandleMovement();

    if (self->spike) {
        self->spike->position.x = self->position.x;
        self->spike->position.y = self->position.y;
    }

    if (self->state == TurboSpiker_State_Idle && !--self->timer) {
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 1, &self->shellAnimator, true, 0);
        self->timer = 0;
        self->state = TurboSpiker_State_Turning;
    }

    RSDK.ProcessAnimation(&self->shellAnimator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOffScreen();
}

void TurboSpiker_State_Hidden(void)
{
    RSDK_THIS(TurboSpiker);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &TurboSpiker->hitboxRange)) {
            CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, self->position.y + 0x80000)->childPtr = INT_TO_VOID(true);
            RSDK.PlaySfx(TurboSpiker->sfxSplash, false, 0xFF);
            RSDK.SetSpriteAnimation(-1, 0, &self->animator, true, 0);
            RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 1, &self->shellAnimator, true, 0);
            self->drawGroup = Zone->objectDrawGroup[0] + 1;
            if (self->spike)
                self->spike->drawGroup = Zone->objectDrawGroup[0];

            EntityTurboSpiker *ember = CREATE_ENTITY(TurboSpiker, INT_TO_VOID(true), self->position.x, self->position.y);
            ember->direction         = self->direction;
            ember->drawGroup         = self->drawGroup + 1;
            RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 6, &ember->shellAnimator, true, 0);
            ember->state = TurboSpiker_State_Ember;
            self->timer  = 60;
            self->state  = TurboSpiker_State_FireShell;
            TurboSpiker_State_FireShell();
            foreach_return;
        }
    }

    RSDK.ProcessAnimation(&self->animator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOffScreen();
}

void TurboSpiker_State_Moving(void)
{
    RSDK_THIS(TurboSpiker);

    TurboSpiker_HandleMovement();

    if (self->spike) {
        self->spike->position.x = self->position.x;
        self->spike->position.y = self->position.y;
    }

    RSDK.ProcessAnimation(&self->shellAnimator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOffScreen();
}

void TurboSpiker_State_Turning(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&self->shellAnimator);

    if (++self->timer == 15) {
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 2, &self->shellAnimator, true, 0);
        self->state = TurboSpiker_State_FinishTurning;
    }

    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOffScreen();
}

void TurboSpiker_State_FinishTurning(void)
{
    RSDK_THIS(TurboSpiker);

    RSDK.ProcessAnimation(&self->shellAnimator);

    if (self->shellAnimator.frameID == self->shellAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 0, &self->shellAnimator, true, 0);
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;

        if (self->spike) {
            self->spike->direction = self->direction;
            self->timer            = 128;
            self->state            = TurboSpiker_State_Idle;
            TurboSpiker_State_Idle();
        }
        else {
            self->state = TurboSpiker_State_Moving;
            TurboSpiker_State_Moving();
        }
    }
    else {
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOffScreen();
    }
}

void TurboSpiker_State_Falling(void)
{
    RSDK_THIS(TurboSpiker);

    RSDK.ProcessAnimation(&self->shellAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->spike) {
        self->spike->position.x = self->position.x;
        self->spike->position.y = self->position.y;
    }

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 0, &self->shellAnimator, true, 0);
        self->state = TurboSpiker_State_Moving;
        TurboSpiker_State_Moving();
    }
    else {
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOffScreen();
    }
}

void TurboSpiker_State_FireShell(void)
{
    RSDK_THIS(TurboSpiker);

    if (!--self->timer) {
        if (Player_GetNearestPlayer())
            self->direction = Player_GetNearestPlayer()->position.x < self->position.x;

        if (self->spike) {
            RSDK.PlaySfx(TurboSpiker->sfxLaunch, false, 0xFF);
            RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 4, &self->spike->animator, true, 0);
            self->spike->direction  = self->direction;
            self->spike->velocity.x = 0x14000 * (self->direction ? -1 : 1);
            self->spike->velocity.y = -0x30000;
            self->spike->state      = TurboSpiker_State_Spike;
            self->spike             = NULL;
        }

        self->velocity.x = 0x20000 * (self->direction ? 1 : -1);
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 0, &self->shellAnimator, true, 0);
        self->state               = TurboSpiker_State_Moving;
        self->shellAnimator.speed = 2;
        TurboSpiker_HandleMovement();
    }

    RSDK.ProcessAnimation(&self->shellAnimator);

    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOffScreen();
}

void TurboSpiker_HandleSpikeCollisions(void)
{
    RSDK_THIS(TurboSpiker);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &TurboSpiker->hitboxSpike)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}

void TurboSpiker_State_Spike(void)
{
    RSDK_THIS(TurboSpiker);

    if (!self->playedLaunchSfx) {
        RSDK.PlaySfx(TurboSpiker->sfxLaunch, false, 0xFF);
        self->playedLaunchSfx = true;
    }

    if (!self->onScreen) {
        RSDK.StopSfx(TurboSpiker->sfxLaunch);
        self->playedLaunchSfx = false;
    }

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    TurboSpiker_HandleSpikeCollisions();

    if (!(Zone->timer & 3)) {
        int32 x                  = self->position.x + (RSDK.Rand(-4, 4) << 16);
        int32 y                  = self->position.y + (RSDK.Rand(-2, 3) << 16);
        EntityTurboSpiker *ember = CREATE_ENTITY(TurboSpiker, INT_TO_VOID(true), x, y);
        ember->direction         = self->direction;
        ember->drawGroup         = self->drawGroup - 1;
        RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 5, &ember->shellAnimator, true, 0);
        ember->state = TurboSpiker_State_Ember;
    }

    RSDK.ProcessAnimation(&self->animator);
    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void TurboSpiker_State_Ember(void)
{
    RSDK_THIS(TurboSpiker);

    RSDK.ProcessAnimation(&self->shellAnimator);
    if (self->shellAnimator.frameID == self->shellAnimator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void TurboSpiker_EditorDraw(void)
{
    RSDK_THIS(TurboSpiker);

    RSDK.SetSpriteAnimation(TurboSpiker->aniFrames, 1, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(self->type == TURBOSPIKER_VISIBLE ? TurboSpiker->aniFrames : -1, 3, &self->shellAnimator, true, 0);

    RSDK.DrawSprite(&self->shellAnimator, NULL, false);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TurboSpiker_EditorLoad(void)
{
    TurboSpiker->aniFrames = RSDK.LoadSpriteAnimation("HCZ/TurboSpiker.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TurboSpiker, type);
    RSDK_ENUM_VAR("Always Visible", TURBOSPIKER_VISIBLE);
    RSDK_ENUM_VAR("Hidden Behind Waterfall", TURBOSPIKER_HIDDEN);

    RSDK_ACTIVE_VAR(TurboSpiker, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void TurboSpiker_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurboSpiker, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TurboSpiker, VAR_UINT8, direction);
}