// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Bubbler Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBubbler *Bubbler = NULL;

// NOTE: this object is basically a port of the Bubbler & Bubbler's mother debug objects in S2 '13, though it's unused
// It's not in any stage lists, and the sprites for it aren't aligned correctly, though they are at different positions from S2 '13s

void Bubbler_Update(void)
{
    RSDK_THIS(Bubbler);

    StateMachine_Run(self->state);
}

void Bubbler_LateUpdate(void) {}

void Bubbler_StaticUpdate(void) {}

void Bubbler_Draw(void)
{
    RSDK_THIS(Bubbler);

    RSDK.DrawSprite(&self->bodyHitbox, NULL, false);
    if (self->startPos.x)
        RSDK.DrawSprite(&self->flameAnimator, NULL, false);
}

void Bubbler_Create(void *data)
{
    RSDK_THIS(Bubbler);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (data) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 3, &self->bodyHitbox, true, 0);
        self->state = Bubbler_StateProjectile_Seed;
    }
    else {
        self->startPos   = self->position;
        self->startDir   = self->direction;
        self->velocity.x = !self->direction ? -0x4000 : 0x4000;
        self->velocity.y = 0;
        self->timer      = 0;
        self->spawnTimer = 32;
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &self->bodyHitbox, true, 0);
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 1, &self->flameAnimator, true, 0);
        self->state = Bubbler_State_Init;
    }
}

void Bubbler_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("CPZ"))
        Bubbler->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Bubbler.bin", SCOPE_STAGE);

    Bubbler->hitboxBadnik.left   = -16;
    Bubbler->hitboxBadnik.top    = -12;
    Bubbler->hitboxBadnik.right  = 16;
    Bubbler->hitboxBadnik.bottom = 12;

    Bubbler->hitboxRange.left   = -160;
    Bubbler->hitboxRange.top    = -12;
    Bubbler->hitboxRange.right  = 16;
    Bubbler->hitboxRange.bottom = 96;

    Bubbler->hitboxProjectile.left   = -2;
    Bubbler->hitboxProjectile.top    = -2;
    Bubbler->hitboxProjectile.right  = 2;
    Bubbler->hitboxProjectile.bottom = 2;

    DEBUGMODE_ADD_OBJ(Bubbler);
}

void Bubbler_DebugSpawn(void)
{
    RSDK_THIS(Bubbler);

    CREATE_ENTITY(Bubbler, NULL, self->position.x, self->position.y);
}

void Bubbler_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Bubbler_HandleInteractions(void)
{
    RSDK_THIS(Bubbler);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Bubbler->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Bubbler_HandleProjectileInteractions(void)
{
    RSDK_THIS(Bubbler);

    int32 distance = 0x7FFFFFFF;
    foreach_active(Player, player)
    {
        if (abs(player->position.x - self->position.x) < distance) {
            distance        = abs(player->position.x - self->position.x);
            self->direction = player->position.x >= self->position.x;
        }

        if (Player_CheckCollisionTouch(player, self, &Bubbler->hitboxProjectile))
            Player_Hurt(player, self);
    }
}

void Bubbler_CheckOffScreen(void)
{
    RSDK_THIS(Bubbler);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Bubbler_Create(NULL);
    }
}

void Bubbler_State_Init(void)
{
    RSDK_THIS(Bubbler);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;
    self->state      = Bubbler_State_MotherPatrol;
    Bubbler_State_MotherPatrol();
}

void Bubbler_State_MotherPatrol(void)
{
    RSDK_THIS(Bubbler);

    self->position.x += self->velocity.x;
    RSDK.ProcessAnimation(&self->flameAnimator);

    if (!--self->timer) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
        self->timer      = 0x200;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Bubbler->hitboxRange)) {
            self->timer = 16;
            self->state = Bubbler_State_FoundPlayer;
            if (self->direction == FLIP_NONE)
                self->velocity.x = -0x28000;
            else
                self->velocity.x = 0x28000;
        }
    }

    Bubbler_HandleInteractions();
    Bubbler_CheckOffScreen();
}

void Bubbler_State_FoundPlayer(void)
{
    RSDK_THIS(Bubbler);

    RSDK.ProcessAnimation(&self->flameAnimator);

    if (--self->timer < 0) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 2, &self->flameAnimator, true, 0);
        self->state = Bubbler_State_AttackPlayer;
    }

    Bubbler_HandleInteractions();
    Bubbler_CheckOffScreen();
}

void Bubbler_State_AttackPlayer(void)
{
    RSDK_THIS(Bubbler);

    RSDK.ProcessAnimation(&self->flameAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (++self->spawnTimer >= 30) {
        self->spawnTimer = 0;
        int32 spawnX     = self->position.x + 0x60000;
        if (self->direction)
            spawnX = self->position.x - 0x60000;
        CREATE_ENTITY(Bubbler, INT_TO_VOID(true), spawnX, self->position.y + 0xA0000)->active = ACTIVE_NORMAL;
    }

    Bubbler_HandleInteractions();
    Bubbler_CheckOffScreen();
}

void Bubbler_StateProjectile_Seed(void)
{
    RSDK_THIS(Bubbler);

    RSDK.ProcessAnimation(&self->bodyHitbox);
    self->position.y += 0x10000;

    Bubbler_HandleProjectileInteractions();

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0, false)) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 4, &self->bodyHitbox, true, 0);
        self->state = Bubbler_StateProjectile_Bubbler;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Bubbler_StateProjectile_Bubbler(void)
{
    RSDK_THIS(Bubbler);

    RSDK.ProcessAnimation(&self->bodyHitbox);

    if (self->bodyHitbox.frameID >= 7)
        destroyEntity(self);
    else if (self->bodyHitbox.frameID < 6)
        Bubbler_HandleProjectileInteractions();
}

#if GAME_INCLUDE_EDITOR
void Bubbler_EditorDraw(void)
{
    RSDK_THIS(Bubbler);

    self->startPos = self->position;
    RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &self->bodyHitbox, true, 0);
    RSDK.SetSpriteAnimation(Bubbler->aniFrames, 1, &self->flameAnimator, true, 0);

    Bubbler_Draw();
}

void Bubbler_EditorLoad(void)
{
    Bubbler->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Bubbler.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Bubbler, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Bubbler_Serialize(void) { RSDK_EDITABLE_VAR(Bubbler, VAR_UINT8, direction); }
