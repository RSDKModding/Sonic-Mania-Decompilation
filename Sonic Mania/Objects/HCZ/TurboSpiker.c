// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TurboSpiker Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.DrawSprite(&self->spikeAnimator, NULL, false);
}

void TurboSpiker_Create(void *data)
{
    RSDK_THIS(TurboSpiker);
    self->launchPlayed = false;
    self->visible      = true;
    self->drawFX |= FX_FLIP;
    self->drawOrder     = Zone->drawOrderLow + 1;
    self->spawnPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->spawnDir      = self->direction;
    if (data) {
        self->active = ACTIVE_NORMAL;
    }
    else {
        self->unused_64 = 0x10;
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &self->animator, true, 0);
        self->state = TurboSpiker_Hermit_Create;
    }
}

void TurboSpiker_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        TurboSpiker->animID = RSDK.LoadSpriteAnimation("HCZ/TurboSpiker.bin", SCOPE_STAGE);
    TurboSpiker->hermitHitbox.left   = -14;
    TurboSpiker->hermitHitbox.top    = -14;
    TurboSpiker->hermitHitbox.right  = 14;
    TurboSpiker->hermitHitbox.bottom = 14;
    TurboSpiker->spikeHitbox.left    = -5;
    TurboSpiker->spikeHitbox.top     = -31;
    TurboSpiker->spikeHitbox.right   = 9;
    TurboSpiker->spikeHitbox.bottom  = 2;
    TurboSpiker->checkbox.left       = -0x80;
    TurboSpiker->checkbox.top        = -0x200;
    TurboSpiker->checkbox.right      = 0x80;
    TurboSpiker->checkbox.bottom     = 0x200;
    TurboSpiker->launchSFX           = RSDK.GetSfx("Stage/Launch.wav");
    TurboSpiker->splashSFX           = RSDK.GetSfx("Stage/Splash.wav");
    DEBUGMODE_ADD_OBJ(TurboSpiker);
}

void TurboSpiker_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &DebugMode->animator, true, 0);
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
        if (Player_CheckBadnikTouch(player, self, &TurboSpiker->hermitHitbox) && Player_CheckBadnikBreak(self, player, false)) {
            if (self->spike)
                destroyEntity(self->spike);
            destroyEntity(self);
        }
    }
}

void TurboSpiker_Hermit_CheckOnScreen(void)
{
    RSDK_THIS(TurboSpiker);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->spawnPos, &self->updateRange)) {
        self->position.x = self->spawnPos.x;
        self->position.y = self->spawnPos.y;
        if (self->spike)
            destroyEntity(self->spike);
        self->direction = self->spawnDir;
        TurboSpiker_Create(NULL);
    }
}

void TurboSpiker_Hermit_NextState(void)
{
    RSDK_THIS(TurboSpiker);
    self->position.x += self->velocity.x;
    if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &self->animator, true, 0);
        self->timer = 0;
        if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x20000 * (self->direction ? -1 : 1), 0x80000, 8))
            self->state = TurboSpiker_Hermit_Turn;
        else
            self->state = TurboSpiker_Hermit_Move;
    }
}

void TurboSpiker_Hermit_Create(void)
{
    RSDK_THIS(TurboSpiker);
    self->active = ACTIVE_NORMAL;
    if (self->type == 1) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &self->spikeAnimator, true, 0);
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 3, &self->animator, true, 0);
        self->drawOrder = Zone->fgLayerLow + 1;
        if (Player_GetNearestPlayer())
            self->direction = Player_GetNearestPlayer()->position.x >= self->position.x;
    }
    EntityTurboSpiker *spike = CREATE_ENTITY(TurboSpiker, intToVoid(1), self->position.x, self->position.y);
    spike->isPermanent       = true;
    spike->direction         = self->direction;
    spike->drawOrder         = self->drawOrder - 1;
    RSDK.SetSpriteAnimation(TurboSpiker->animID, 3, &spike->animator, true, 0);
    spike->state  = TurboSpiker_Spike_Collide;
    self->spike = (Entity*)spike;
    if (!self->type) {
        self->timer      = 64;
        self->state      = TurboSpiker_Hermit_Idle;
        self->velocity.x = 0x10000 * (self->direction ? 1 : -1);
        TurboSpiker_Hermit_Idle();
        self->velocity.y = 0;
    }
    else {
        self->state = TurboSpiker_Hermit_IdleWater;
        TurboSpiker_Hermit_IdleWater();
        self->velocity.y = 0;
    }
}
void TurboSpiker_Hermit_Idle(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &TurboSpiker->checkbox)) {
            self->timer = 1;
            self->state = TurboSpiker_Hermit_Fire;
            TurboSpiker_Hermit_Collide();
            TurboSpiker_Hermit_CheckOnScreen();
            foreach_return;
        }
    }

    TurboSpiker_Hermit_NextState();
    Entity *spike = self->spike;
    if (spike) {
        spike->position.x = self->position.x;
        spike->position.y = self->position.y;
    }
    if (self->state == TurboSpiker_Hermit_Idle && !--self->timer) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &self->animator, true, 0);
        self->timer = 0;
        self->state = TurboSpiker_Hermit_Turn;
    }
    RSDK.ProcessAnimation(&self->animator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_IdleWater(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &TurboSpiker->checkbox)) {
            CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), self->position.x, self->position.y + 0x80000)->childPtr = intToVoid(1);
            RSDK.PlaySfx(TurboSpiker->splashSFX, false, 0xFF);
            RSDK.SetSpriteAnimation(-1, 0, &self->spikeAnimator, true, 0);
            RSDK.SetSpriteAnimation(TurboSpiker->animID, 1, &self->animator, true, 0);
            self->drawOrder = Zone->drawOrderLow + 1;
            if (self->spike)
                self->spike->drawOrder = Zone->drawOrderLow;
            EntityTurboSpiker *ember = CREATE_ENTITY(TurboSpiker, intToVoid(1), self->position.x, self->position.y);
            ember->direction         = self->direction;
            ember->drawOrder         = self->drawOrder + 1;
            RSDK.SetSpriteAnimation(TurboSpiker->animID, 6, &ember->animator, true, 0);
            ember->state  = TurboSpiker_Ember_Animate;
            self->timer = 60;
            self->state = TurboSpiker_Hermit_Fire;
            TurboSpiker_Hermit_Fire();
            foreach_return;
        }
    }
    RSDK.ProcessAnimation(&self->spikeAnimator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_Handle(void)
{
    RSDK_THIS(TurboSpiker);
    TurboSpiker_Hermit_NextState();
    if (self->spike) {
        self->spike->position.x = self->position.x;
        self->spike->position.y = self->position.y;
    }
    RSDK.ProcessAnimation(&self->animator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_Turn(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&self->animator);
    if (++self->timer == 15) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 2, &self->animator, true, 0);
        self->state = TurboSpiker_Hermit_AfterTurn;
    }
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Hermit_AfterTurn(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &self->animator, true, 0);
        self->direction ^= 1;
        self->velocity.x = -self->velocity.x;
        if (self->spike) {
            self->spike->direction = self->direction;
            self->timer            = 128;
            self->state            = TurboSpiker_Hermit_Idle;
            TurboSpiker_Hermit_Idle();
        }
        else {
            self->state = TurboSpiker_Hermit_Handle;
            TurboSpiker_Hermit_Handle();
        }
    }
    else {
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOnScreen();
    }
}

void TurboSpiker_Hermit_Move(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->spike) {
        self->spike->position.x = self->position.x;
        self->spike->position.y = self->position.y;
    }
    if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &self->animator, true, 0);
        self->state = TurboSpiker_Hermit_Handle;
        TurboSpiker_Hermit_Handle();
    }
    else {
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOnScreen();
    }
}

void TurboSpiker_Hermit_Fire(void)
{
    RSDK_THIS(TurboSpiker);
    if (!--self->timer) {
        if (Player_GetNearestPlayer())
            self->direction = Player_GetNearestPlayer()->position.x < self->position.x;
        EntityTurboSpiker *spike = (EntityTurboSpiker *)self->spike;
        if (spike) {
            RSDK.PlaySfx(TurboSpiker->launchSFX, 0, 255);
            RSDK.SetSpriteAnimation(TurboSpiker->animID, 4, &spike->spikeAnimator, true, 0);
            spike->direction  = self->direction;
            spike->velocity.x = 0x14000 * (self->direction ? -1 : 1);
            spike->velocity.y = -0x30000;
            spike->state      = TurboSpiker_Spike_Fly;
            self->spike     = NULL;
        }
        self->velocity.x = 0x20000 * (self->direction ? 1 : -1);
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 0, &self->animator, true, 0);
        self->state                   = TurboSpiker_Hermit_Handle;
        self->animator.speed = 2;
        TurboSpiker_Hermit_NextState();
        RSDK.ProcessAnimation(&self->animator);
        TurboSpiker_Hermit_Collide();
        TurboSpiker_Hermit_CheckOnScreen();
    }
    RSDK.ProcessAnimation(&self->animator);
    TurboSpiker_Hermit_Collide();
    TurboSpiker_Hermit_CheckOnScreen();
}

void TurboSpiker_Spike_Collide(void)
{
    RSDK_THIS(TurboSpiker);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &TurboSpiker->spikeHitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x400, player, 2, &player->uncurlTimer))
#endif
            Player_CheckHit(player, self);
        }
    }
}

void TurboSpiker_Spike_Fly(void)
{
    RSDK_THIS(TurboSpiker);
    if (!self->launchPlayed) {
        RSDK.PlaySfx(TurboSpiker->launchSFX, 0, 255);
        self->launchPlayed = true;
    }
    if (!self->activeScreens) {
        RSDK.StopSfx(TurboSpiker->launchSFX);
        self->launchPlayed = false;
    }
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    TurboSpiker_Spike_Collide();

    if (!(Zone->timer & 3)) {
        EntityTurboSpiker *ember =
            CREATE_ENTITY(TurboSpiker, intToVoid(1), (RSDK.Rand(-4, 4) << 16) + self->position.x, (RSDK.Rand(-2, 3) << 16) + self->position.y);
        ember->direction         = self->direction;
        ember->drawOrder         = self->drawOrder - 1;
        RSDK.SetSpriteAnimation(TurboSpiker->animID, 5, &ember->animator, true, 0);
        ember->state = TurboSpiker_Ember_Animate;
    }
    RSDK.ProcessAnimation(&self->spikeAnimator);
    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void TurboSpiker_Ember_Animate(void)
{
    RSDK_THIS(TurboSpiker);
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void TurboSpiker_EditorDraw(void) { TurboSpiker_DebugDraw(); }

void TurboSpiker_EditorLoad(void) { TurboSpiker_StageLoad(); }

void TurboSpiker_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurboSpiker, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TurboSpiker, VAR_UINT8, direction);
}