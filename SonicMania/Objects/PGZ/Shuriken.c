// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Shuriken Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectShuriken *Shuriken;

void Shuriken_Update(void)
{
    RSDK_THIS(Shuriken);

    if (self->state) {
        StateMachine_Run(self->state);

        if (self->animator.animationID == 1 || self->animator.animationID == 2 || self->animator.animationID == 4)
            RSDK.ProcessAnimation(&self->animator);
    }
}

void Shuriken_LateUpdate(void) {}

void Shuriken_StaticUpdate(void)
{
    EntityShuriken *activeGroups[255];
    memset(activeGroups, 0, sizeof(activeGroups));

    foreach_active(Shuriken, shuriken)
    {
        if (shuriken->timerGroup) {
            if (!activeGroups[shuriken->timerGroup - 1]) {
                activeGroups[shuriken->timerGroup - 1] = shuriken;
                ++Shuriken->timerGroups[shuriken->timerGroup - 1];
            }
        }
    }

    for (int32 i = 0; i < 0xFF; ++i) {
        if (activeGroups[i] == NULL && Shuriken->timerGroups[i])
            Shuriken->timerGroups[i] = 0;
    }
}

void Shuriken_Draw(void)
{
    RSDK_THIS(Shuriken);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Shuriken_Create(void *data)
{
    RSDK_THIS(Shuriken);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Shuriken_State_Init;
}

void Shuriken_StageLoad(void)
{
    Shuriken->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Shuriken.bin", SCOPE_STAGE);

    Shuriken->hitboxShooter.top    = -8;
    Shuriken->hitboxShooter.left   = -16;
    Shuriken->hitboxShooter.right  = 16;
    Shuriken->hitboxShooter.bottom = 8;

    Shuriken->hitboxRange.top    = -256;
    Shuriken->hitboxRange.left   = -64;
    Shuriken->hitboxRange.right  = 64;
    Shuriken->hitboxRange.bottom = 256;

    Shuriken->hitboxShuriken.top    = -3;
    Shuriken->hitboxShuriken.left   = -16;
    Shuriken->hitboxShuriken.right  = 16;
    Shuriken->hitboxShuriken.bottom = 5;

    Shuriken->hitboxPlatform.top    = 0;
    Shuriken->hitboxPlatform.left   = -16;
    Shuriken->hitboxPlatform.right  = 16;
    Shuriken->hitboxPlatform.bottom = 8;

    Shuriken->sfxArrowLaunch = RSDK.GetSfx("PSZ/ArrowLaunch.wav");
    Shuriken->sfxArrowHit    = RSDK.GetSfx("PSZ/ArrowHit.wav");
}

void Shuriken_CheckPlayerCollisions(void)
{
    RSDK_THIS(Shuriken);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Shuriken->hitboxShuriken)) {
#if MANIA_USE_PLUS
            if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                self->interaction = false;
                self->state       = Shuriken_State_ShurikenDebris;
            }
            else {
#endif
                Player_Hurt(player, self);
#if MANIA_USE_PLUS
            }
#endif
        }
    }
}

void Shuriken_HandleSolidCollisions(void)
{
    RSDK_THIS(Shuriken);

    self->activePlayers = 0;

    foreach_active(Player, player)
    {
        bool32 wasOnGround = self->onGround;
        if (Player_CheckCollisionPlatform(player, self, &Shuriken->hitboxPlatform)) {
            if (!wasOnGround && !player->sidekick && !self->dropTimer)
                self->dropTimer = 1;

            self->activePlayers |= 1 << RSDK.GetEntitySlot(player);
            player->position.y &= 0xFFFF0000;
        }
    }
}

void Shuriken_State_Init(void)
{
    RSDK_THIS(Shuriken);

    self->timer  = 0;
    self->active = ACTIVE_BOUNDS;
    RSDK.SetSpriteAnimation(Shuriken->aniFrames, 0, &self->animator, true, 0);
    self->drawGroup = Zone->objectDrawGroup[1];

    self->state = Shuriken_State_AwaitActivate;
    Shuriken_State_AwaitActivate();
}

void Shuriken_State_AwaitActivate(void)
{
    RSDK_THIS(Shuriken);

    if (self->timerMode) {
        int32 group = self->timerGroup - 1;
        int32 timer = self->timerGroup ? Shuriken->timerGroups[group] : Zone->timer;

        if (!((timer + self->intervalOffset) % self->interval)) {
            RSDK.SetSpriteAnimation(Shuriken->aniFrames, 2, &self->animator, false, 0);
            self->state = Shuriken_State_FireShuriken;
            self->timer = 0;
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (!player->sidekick && Player_CheckCollisionTouch(player, self, &Shuriken->hitboxRange)) {
                self->active = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Shuriken->aniFrames, 1, &self->animator, false, 0);
                self->state = Shuriken_State_CheckPlayerInRange;
            }
        }
    }
}

void Shuriken_State_CheckPlayerInRange(void)
{
    RSDK_THIS(Shuriken);

    bool32 inRange = false;
    foreach_active(Player, player)
    {
        if (!player->sidekick && Player_CheckCollisionTouch(player, self, &Shuriken->hitboxRange)) {
            inRange = true;
            foreach_break;
        }
    }

    if (!inRange) {
        self->timer = self->delay;
        self->state = Shuriken_State_ShootDelay;
    }
}

void Shuriken_State_ShootDelay(void)
{
    RSDK_THIS(Shuriken);

    if (self->timer) {
        self->timer--;
    }
    else {
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 2, &self->animator, false, 0);
        self->state = Shuriken_State_FireShuriken;
        Shuriken_State_FireShuriken();
    }
}

void Shuriken_State_FireShuriken(void)
{
    RSDK_THIS(Shuriken);

    if (self->timer >= 15) {
        RSDK.PlaySfx(Shuriken->sfxArrowLaunch, false, 255);
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 3, &self->animator, false, 0);
        self->state = Shuriken_State_ShurikenFired;

        EntityShuriken *shuriken = CREATE_ENTITY(Shuriken, self, self->position.x, self->position.y);
        shuriken->state          = Shuriken_State_InitShuriken;
        shuriken->direction      = self->direction;
        shuriken->active         = ACTIVE_NORMAL;
        shuriken->velocity.x     = self->direction == FLIP_NONE ? 0x40000 : -0x40000;
    }
    else {
        self->timer++;
    }
}

void Shuriken_State_ShurikenFired(void)
{
    RSDK_THIS(Shuriken);

    bool32 collided = false;
    foreach_active(Shuriken, shuriken)
    {
        if (shuriken->state == Shuriken_State_ShurikenThrown) {
            if (RSDK.CheckObjectCollisionTouchBox(self, &Shuriken->hitboxShooter, shuriken, &Shuriken->hitboxShuriken)) {
                collided = true;
                foreach_break;
            }
        }
    }

    if (!collided) {
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 4, &self->animator, false, 0);
        self->state = Shuriken_State_Deactivate;
        self->timer = 0;
    }
}

void Shuriken_State_Deactivate(void)
{
    RSDK_THIS(Shuriken);

    if (self->timer >= 10)
        self->state = Shuriken_State_Init;
    else
        self->timer++;
}

void Shuriken_State_InitShuriken(void)
{
    RSDK_THIS(Shuriken);

    self->timer     = 0;
    self->dropTimer = 0;
    self->active    = ACTIVE_NORMAL;
    self->drawGroup = Zone->objectDrawGroup[1] - 1;
    RSDK.SetSpriteAnimation(Shuriken->aniFrames, 5, &self->animator, true, 0);

    self->state = Shuriken_State_ShurikenThrown;
    Shuriken_State_ShurikenThrown();
}

void Shuriken_State_ShurikenThrown(void)
{
    RSDK_THIS(Shuriken);

    RSDK.ProcessAnimation(&self->animator);

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);

    Shuriken_CheckPlayerCollisions();

    self->position.x += self->velocity.x;

    bool32 collided = false;
    if (self->direction)
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_RWALL, 0, -0xE0000, 0, 2);
    else
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_LWALL, 0, 0xE0000, 0, 2);

    if (collided) {
        foreach_active(Shuriken, shuriken)
        {
            if (shuriken != self) {
                if (RSDK.CheckObjectCollisionTouchBox(self, &Shuriken->hitboxShuriken, shuriken, &Shuriken->hitboxShuriken)) {
                    shuriken->velocity.y = 0;
                    shuriken->state      = Shuriken_State_ShurikenFall;
                }
            }
        }

        self->velocity.x = 0;
        RSDK.PlaySfx(Shuriken->sfxArrowHit, false, 255);
        self->state = Shuriken_State_ShurikenStuck;
    }
}

void Shuriken_State_ShurikenStuck(void)
{
    RSDK_THIS(Shuriken);

    if (self->timer < 300)
        self->timer++;

    if (self->dropTimer > 0 && self->dropTimer < 60)
        self->dropTimer++;

    Shuriken_HandleSolidCollisions();

    bool32 collided = false;
    if (self->direction)
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_RWALL, 0, -0xE0000, 0, 2);
    else
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_LWALL, 0, 0xE0000, 0, 2);

    if (!collided || self->timer >= 300 || self->dropTimer >= 60) {
        self->updateRange.x = 0x4000000;
        self->updateRange.y = 0x4000000;
        self->velocity.y    = 0;
        self->state         = Shuriken_State_ShurikenFall;
    }
}

void Shuriken_State_ShurikenFall(void)
{
    RSDK_THIS(Shuriken);

    if (self->velocity.y >= 0x40000)
        self->velocity.y = 0x40000;
    else
        self->velocity.y += 0x4000;

    self->position.y += self->velocity.y;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if MANIA_USE_PLUS
void Shuriken_State_ShurikenDebris(void)
{
    RSDK_THIS(Shuriken);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}
#endif

#if GAME_INCLUDE_EDITOR
void Shuriken_EditorDraw(void)
{
    RSDK_THIS(Shuriken);

    RSDK.SetSpriteAnimation(Shuriken->aniFrames, 0, &self->animator, true, 0);
    self->drawGroup = Zone->objectDrawGroup[1];

    Shuriken_Draw();
}

void Shuriken_EditorLoad(void)
{
    Shuriken->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Shuriken.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Shuriken, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Shuriken_Serialize(void)
{
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, delay);
    RSDK_EDITABLE_VAR(Shuriken, VAR_BOOL, timerMode);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT8, timerGroup);
}