// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Grabber Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGrabber *Grabber;

void Grabber_Update(void)
{
    RSDK_THIS(Grabber);

    StateMachine_Run(self->state);
}

void Grabber_LateUpdate(void) {}

void Grabber_StaticUpdate(void) {}

void Grabber_Draw(void)
{
    RSDK_THIS(Grabber);

    if (SceneInfo->currentDrawGroup == self->drawGroup) {
        RSDK.DrawLine(self->position.x, self->startPos.y - 0x100000, self->position.x, self->position.y, 0x202020, 0x00, INK_NONE, false);
        RSDK.DrawLine(self->position.x - 0x10000, self->startPos.y - 0x100000, self->position.x - 0x10000, self->position.y, 0xE0E0E0, 0x00, INK_NONE,
                      false);

        Vector2 drawPos;
        int32 storeDir  = self->direction;
        drawPos.x       = self->position.x;
        drawPos.y       = self->startPos.y;
        self->direction = 0;
        RSDK.DrawSprite(&self->wheelAnimator, &drawPos, false);

        self->direction = storeDir;
        RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

        if (!SceneInfo->currentScreenID)
            RSDK.AddDrawListRef(Zone->objectDrawGroup[1], SceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&self->clampAnimator, NULL, false);
    }
}

void Grabber_Create(void *data)
{
    RSDK_THIS(Grabber);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->startPos  = self->position;
    self->startDir  = self->direction;
    self->active    = ACTIVE_BOUNDS;

    self->velocity.x    = !self->direction ? -0x4000 : 0x4000;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_FLIP;
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 1, &self->clampAnimator, true, 0);
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 4, &self->wheelAnimator, true, 0);
    self->state = Grabber_State_Init;
}

void Grabber_StageLoad(void)
{
    Grabber->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Grabber.bin", SCOPE_STAGE);

    Grabber->hitboxBadnik.left   = -8;
    Grabber->hitboxBadnik.top    = -8;
    Grabber->hitboxBadnik.right  = 8;
    Grabber->hitboxBadnik.bottom = 8;

    Grabber->hitboxRange.left   = -64;
    Grabber->hitboxRange.top    = 0;
    Grabber->hitboxRange.right  = 64;
    Grabber->hitboxRange.bottom = 128;

    Grabber->hitboxGrab.left   = -8;
    Grabber->hitboxGrab.top    = 0;
    Grabber->hitboxGrab.right  = 8;
    Grabber->hitboxGrab.bottom = 16;

    DEBUGMODE_ADD_OBJ(Grabber);

    Grabber->sfxGrab    = RSDK.GetSfx("Global/Grab.wav");
    Grabber->sfxDestroy = RSDK.GetSfx("Global/Destroy.wav");
}

void Grabber_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Grabber->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Grabber_DebugSpawn(void)
{
    RSDK_THIS(Grabber);
    EntityGrabber *grabber = CREATE_ENTITY(Grabber, NULL, self->position.x, self->position.y);
    grabber->direction     = self->direction;
    grabber->startDir      = self->direction;
    grabber->velocity.x    = !self->direction ? -0x4000 : 0x4000;
}

void Grabber_CheckPlayerCollisions(void)
{
    RSDK_THIS(Grabber);

    foreach_active(Player, player)
    {
        if (player != self->grabbedPlayer) {
            if (self->state == Grabber_State_TryToGrab) {
                if (Player_CheckCollisionTouch(player, self, &Grabber->hitboxGrab)) {
                    RSDK.PlaySfx(Grabber->sfxGrab, false, 255);
                    self->state             = Grabber_State_GrabbedPlayer;
                    self->isPermanent       = true;
                    self->grabbedPlayer     = player;
                    player->velocity.x      = 0;
                    player->velocity.y      = 0;
                    player->groundVel       = 0;
                    player->state           = Player_State_Static;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->onGround        = false;
                    player->direction       = self->direction;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, true, 0);
                    player->animator.speed = 0;
                    player->direction      = self->direction ^ FLIP_X;
                }
            }

            if (player != self->grabbedPlayer && !self->grabDelay && Player_CheckBadnikTouch(player, self, &Grabber->hitboxBadnik)
                && Player_CheckBadnikBreak(player, self, false)) {
                EntityPlayer *playerPtr = self->grabbedPlayer;
                if (playerPtr)
                    playerPtr->state = Player_State_Air;
                destroyEntity(self);
            }
        }
    }
}

void Grabber_CheckOffScreen(void)
{
    RSDK_THIS(Grabber);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position      = self->startPos;
        self->struggleDelay = 0;
        self->struggleTimer = 0;
        self->struggleFlags = 0;
        self->direction     = self->startDir;
        self->grabbedPlayer = NULL;
        self->turnTimer     = 0;
        self->timer         = 0;
        Grabber_Create(NULL);
    }
}

void Grabber_HandleExplode(void)
{
    RSDK_THIS(Grabber);

    if (!--self->turnTimer) {
        self->bodyAnimator.frameID ^= 1;
        self->turnTimer = self->timer;

        if (!--self->timer) {
            EntityPlayer *player = self->grabbedPlayer;
            if (player && player->state == Player_State_Static) {
                Player_Hurt(player, self);
                if (player->state != Player_State_Hurt && Player_CheckValidState(player))
                    player->state = Player_State_Air;
                self->grabbedPlayer = NULL;
            }

            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1];
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(self);
        }
    }
}

void Grabber_State_Init(void)
{
    RSDK_THIS(Grabber);

    self->active = ACTIVE_NORMAL;
    self->state  = Grabber_State_CheckForGrab;
    Grabber_State_CheckForGrab();
}

void Grabber_State_CheckForGrab(void)
{
    RSDK_THIS(Grabber);

    RSDK.ProcessAnimation(&self->wheelAnimator);

    if (++self->turnTimer >= 256) {
        self->turnTimer = 0;
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 3, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->clampAnimator, true, 0);
        self->state = Grabber_State_HandleTurn;
    }
    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Grabber->hitboxRange)) {
            self->timer = 16;
            self->state = Grabber_State_GrabDelay;
        }
    }

    Grabber_CheckPlayerCollisions();
    Grabber_CheckOffScreen();
}

void Grabber_State_GrabDelay(void)
{
    RSDK_THIS(Grabber);

    if (--self->timer < 0) {
        self->timer = 32;
        self->state = Grabber_State_TryToGrab;
    }

    Grabber_CheckPlayerCollisions();
    Grabber_CheckOffScreen();
}

void Grabber_State_HandleTurn(void)
{
    RSDK_THIS(Grabber);

    RSDK.ProcessAnimation(&self->bodyAnimator);

    if (self->bodyAnimator.frameID == self->bodyAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 1, &self->clampAnimator, true, 0);
        if (self->direction == FLIP_X)
            self->position.x -= 0x10000;
        else
            self->position.x += 0x10000;

        self->state      = Grabber_State_CheckForGrab;
        self->velocity.x = -self->velocity.x;
        self->direction ^= FLIP_X;
        Grabber_State_CheckForGrab();
    }
    else {
        Grabber_CheckPlayerCollisions();
        Grabber_CheckOffScreen();
    }
}

void Grabber_State_TryToGrab(void)
{
    RSDK_THIS(Grabber);

    if (--self->timer < 0)
        self->state = Grabber_State_RiseUp;

    self->position.y += 0x20000;

    Grabber_CheckPlayerCollisions();
    Grabber_CheckOffScreen();
}

void Grabber_State_RiseUp(void)
{
    RSDK_THIS(Grabber);

    if (++self->timer >= 32) {
        self->timer = 0;
        self->state = Grabber_State_CheckForGrab;
    }

    self->position.y -= 0x20000;
    Grabber_CheckPlayerCollisions();
    Grabber_CheckOffScreen();
}

void Grabber_State_GrabbedPlayer(void)
{
    RSDK_THIS(Grabber);

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->clampAnimator);

    if (++self->timer < 32) {
        self->position.y -= 0x20000;

        EntityPlayer *player = self->grabbedPlayer;
        if (player) {
            player->animator.speed = 0;
            player->position.x     = self->position.x;
            player->position.y     = self->position.y + 0x100000;
            player->velocity.x     = 0;
            player->velocity.y     = 0;
            player->direction      = self->direction ^ FLIP_X;
        }
        Grabber_CheckPlayerCollisions();
    }
    else if (self->bodyAnimator.frameID != self->bodyAnimator.frameCount - 1) {
        Grabber_CheckPlayerCollisions();
    }
    else {
        self->turnTimer = 1;
        self->timer     = 16;
        RSDK.SetSpriteAnimation(Grabber->aniFrames, 2, &self->bodyAnimator, true, 0);
        self->state = Grabber_State_Struggle;
        Grabber_CheckPlayerCollisions();
    }

    Grabber_CheckOffScreen();
}

void Grabber_State_Struggle(void)
{
    RSDK_THIS(Grabber);

    EntityPlayer *player = self->grabbedPlayer;
    if (player) {
        player->animator.speed = 0;
        if (self->struggleFlags) {
            player->velocity.x = 0;
            player->velocity.y = 0;
            if (self->struggleDelay) {
                self->struggleDelay--;

                uint8 struggleFlags = 0;
                if (player->left)
                    struggleFlags = 1;

                if (player->right)
                    struggleFlags |= 2;

                if (struggleFlags) {
                    if (struggleFlags != 3 && struggleFlags != self->struggleFlags) {
                        self->struggleFlags = struggleFlags;
                        if (++self->struggleTimer >= 4) {
                            player->state       = Player_State_Air;
                            self->grabbedPlayer = 0;
                            self->grabDelay     = 16;
                            self->state         = Grabber_State_PlayerEscaped;
                        }
                    }
                }
            }
            else {
                self->struggleTimer = 0;
                self->struggleFlags = 0;
            }
        }
        else {
            if (player->left) {
                self->struggleFlags = 1;
                self->struggleDelay = 64;
            }
            else if (player->right) {
                self->struggleFlags = 2;
                self->struggleDelay = 64;
            }
        }

        player->position.x = self->position.x;
        player->position.y = self->position.y + 0x100000;
        player->direction  = self->direction ^ FLIP_X;
    }

    Grabber_CheckPlayerCollisions();
    Grabber_HandleExplode();
    Grabber_CheckOffScreen();
}

void Grabber_State_PlayerEscaped(void)
{
    RSDK_THIS(Grabber);

    if (self->grabDelay)
        self->grabDelay--;

    Grabber_CheckPlayerCollisions();
    Grabber_HandleExplode();
    Grabber_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Grabber_EditorDraw(void)
{
    RSDK_THIS(Grabber);

    self->startPos = self->position;

    RSDK.DrawLine(self->position.x, self->startPos.y - 0x100000, self->position.x, self->position.y, 0x202020, 0, INK_NONE, false);
    RSDK.DrawLine(self->position.x - 0x10000, self->startPos.y - 0x100000, self->position.x - 0x10000, self->position.y, 0xE0E0E0, 0, INK_NONE,
                  false);

    Vector2 drawPos;
    int32 dir       = self->direction;
    drawPos.x       = self->position.x;
    drawPos.y       = self->startPos.y;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->wheelAnimator, &drawPos, false);

    self->direction = dir;
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    RSDK.DrawSprite(&self->clampAnimator, NULL, false);
}

void Grabber_EditorLoad(void)
{
    Grabber->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Grabber.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Grabber, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Grabber_Serialize(void) { RSDK_EDITABLE_VAR(Grabber, VAR_UINT8, direction); }
