// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Blaster Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBlaster *Blaster;

void Blaster_Update(void)
{
    RSDK_THIS(Blaster);

    StateMachine_Run(self->state);
}

void Blaster_LateUpdate(void) {}

void Blaster_StaticUpdate(void) {}

void Blaster_Draw(void)
{
    RSDK_THIS(Blaster);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Blaster_Create(void *data)
{
    RSDK_THIS(Blaster);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (data) {
        if (data == INT_TO_VOID(BLASTER_SHOT)) {
            self->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 4, &self->animator, true, 0);
            self->state = Blaster_State_BeginShot;
        }
        else if (data == INT_TO_VOID(BLASTER_SHELL)) {
            self->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 5, &self->animator, true, 0);
            self->state = Blaster_State_Shell;
        }
    }
    else {
        self->startPos = self->position;
        self->startDir = self->direction;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 1, &self->animator, true, 0);
        self->state = Blaster_State_Init;
    }
}

void Blaster_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        Blaster->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Blaster.bin", SCOPE_STAGE);

    Blaster->hitboxBadnik.left   = -16;
    Blaster->hitboxBadnik.top    = -8;
    Blaster->hitboxBadnik.right  = 24;
    Blaster->hitboxBadnik.bottom = 16;

    Blaster->hitboxRange.left   = -128;
    Blaster->hitboxRange.top    = -128;
    Blaster->hitboxRange.right  = 0;
    Blaster->hitboxRange.bottom = 0;

    Blaster->hitboxProjectile.left   = -4;
    Blaster->hitboxProjectile.top    = -4;
    Blaster->hitboxProjectile.right  = 4;
    Blaster->hitboxProjectile.bottom = 4;

    DEBUGMODE_ADD_OBJ(Blaster);
}

void Blaster_DebugSpawn(void)
{
    RSDK_THIS(Blaster);

    CREATE_ENTITY(Blaster, NULL, self->position.x, self->position.y);
}

void Blaster_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Blaster_HandlePlayerInteractions(void)
{
    RSDK_THIS(Blaster);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Blaster->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);

        if (self->state != Blaster_State_AttackPlayer && self->state != Blaster_State_Fall && self->animator.animationID != 3) {
            if (Player_CheckCollisionTouch(player, self, &Blaster->hitboxRange)) {
                self->attackTimer = 0;
                RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &self->animator, true, 0);
                self->state = Blaster_State_AttackPlayer;
            }
        }
    }
}

void Blaster_CheckOffScreen(void)
{
    RSDK_THIS(Blaster);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Blaster_Create(NULL);
    }
}

void Blaster_State_Init(void)
{
    RSDK_THIS(Blaster);
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = self->direction == FLIP_NONE ? -0x8000 : 0x8000;
    self->velocity.y = 0;
    self->timer      = 256;

    self->state = Blaster_State_Move;
    Blaster_State_Move();
}

void Blaster_State_Move(void)
{
    RSDK_THIS(Blaster);

    self->position.x += self->velocity.x;
    RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, 8);

    if (!--self->timer) {
        self->state = Blaster_State_HandleTurn;
        self->timer = 30;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &self->animator, true, 0);
    }
    else if (self->velocity.x < 0 && !RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0xE0000, 0x140000, false)) {
        self->state = Blaster_State_HandleTurn;
        self->timer = 30;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &self->animator, true, 0);
    }
    else if (self->velocity.x > 0 && !RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0xE0000, 0x140000, false)) {
        self->state = Blaster_State_HandleTurn;
        self->timer = 30;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &self->animator, true, 0);
    }

    RSDK.ProcessAnimation(&self->animator);

    Blaster_HandlePlayerInteractions();
    Blaster_CheckOffScreen();
}

void Blaster_State_HandleTurn(void)
{
    RSDK_THIS(Blaster);

    RSDK.ProcessAnimation(&self->animator);
    Blaster_HandlePlayerInteractions();
    Blaster_CheckOffScreen();

    switch (--self->timer) {
        case 15: RSDK.SetSpriteAnimation(Blaster->aniFrames, 3, &self->animator, true, 0); break;

        case 7:
            self->direction ^= FLIP_X;
            self->velocity.x = -self->velocity.x;
            break;

        case 0:
            self->state = Blaster_State_Move;
            self->timer = 512;
            Blaster_State_Move();
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 1, &self->animator, true, 0);
            break;
    }
}

void Blaster_State_AttackPlayer(void)
{
    RSDK_THIS(Blaster);

    switch (++self->attackTimer) {
        case 18: {
            EntityBlaster *shot = NULL;
            if (self->direction) {
                shot             = CREATE_ENTITY(Blaster, INT_TO_VOID(BLASTER_SHOT), self->position.x + 0x170000, self->position.y - 0x150000);
                shot->velocity.x = 0x20000;
            }
            else {
                shot             = CREATE_ENTITY(Blaster, INT_TO_VOID(BLASTER_SHOT), self->position.x - 0x170000, self->position.y - 0x150000);
                shot->velocity.x = -0x20000;
            }
            shot->velocity.y = -0x48000;
            break;
        }

        case 20: RSDK.SetSpriteAnimation(Blaster->aniFrames, 2, &self->animator, true, 0); break;

        case 24: {
            EntityBlaster *shell = NULL;
            if (self->direction) {
                shell             = CREATE_ENTITY(Blaster, INT_TO_VOID(BLASTER_SHELL), self->position.x + 0x60000, self->position.y - 0x30000);
                shell->velocity.x = -0x10000;
            }
            else {
                shell             = CREATE_ENTITY(Blaster, INT_TO_VOID(BLASTER_SHELL), self->position.x - 0x60000, self->position.y - 0x30000);
                shell->velocity.x = 0x10000;
            }
            shell->velocity.y = -0x20000;
            break;
        }

        case 60:
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 1, &self->animator, true, 0);
            self->state = Blaster_State_Move;
            self->timer = 512;
            break;

        default: break;
    }

    RSDK.ProcessAnimation(&self->animator);

    Blaster_HandlePlayerInteractions();
    Blaster_CheckOffScreen();
}

void Blaster_State_MagnetAttract(void)
{
    RSDK_THIS(Blaster);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -0xC0000, 8))
        self->velocity.y = 0;

    Blaster_HandlePlayerInteractions();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        Blaster_CheckOffScreen();
    }

    // Always set us to released state, the electro magnet should reset the state before the next frame
    self->state = Blaster_State_MagnetReleased;
}

void Blaster_State_MagnetReleased(void)
{
    RSDK_THIS(Blaster);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    Blaster_HandlePlayerInteractions();
    Blaster_CheckOffScreen();

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        self->velocity.y = 0;
        self->state      = Blaster_State_Move;
    }
    else {
        self->state = Blaster_State_MagnetReleased;
    }
}

void Blaster_State_BeginShot(void)
{
    RSDK_THIS(Blaster);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == 2)
        self->state = Blaster_State_Shot;
}

void Blaster_State_Shot(void)
{
    RSDK_THIS(Blaster);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        RSDK.ProcessAnimation(&self->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Blaster->hitboxProjectile)) {
                Player_ProjectileHurt(player, self);
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void Blaster_State_Fall(void)
{
    RSDK_THIS(Blaster);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->direction  = self->direction;
        self->velocity.y = 0;
        self->state      = Blaster_State_Init;
    }

    Blaster_HandlePlayerInteractions();
}

void Blaster_State_Shell(void)
{
    RSDK_THIS(Blaster);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    RSDK.ProcessAnimation(&self->animator);

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Blaster_EditorDraw(void)
{
    RSDK_THIS(Blaster);

    RSDK.SetSpriteAnimation(Blaster->aniFrames, 1, &self->animator, true, 0);

    Blaster_Draw();
}

void Blaster_EditorLoad(void)
{
    Blaster->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Blaster.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Blaster, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Blaster_Serialize(void) { RSDK_EDITABLE_VAR(Blaster, VAR_UINT8, direction); }
