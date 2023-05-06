// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomShinobi Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPhantomShinobi *PhantomShinobi;

void PhantomShinobi_Update(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);

    self->rotStore += self->rotSpeed;
    self->rotStore &= 0xFFFF;
}

void PhantomShinobi_LateUpdate(void) {}

void PhantomShinobi_StaticUpdate(void) {}

void PhantomShinobi_Draw(void)
{
    RSDK_THIS(PhantomShinobi);

    RSDK.SetActivePalette(4, 0, ScreenInfo[SceneInfo->currentScreenID].size.y);

    if (self->invincibilityTimer & 1)
        RSDK.CopyPalette(6, 128, 4, 128, 128);

    self->rotation = self->rotStore >> 7;

    for (int32 i = 0; i < 8; ++i) {
        Vector2 drawPos;

        drawPos.x                 = self->finRadius * RSDK.Sin512(self->rotation) + self->position.x;
        drawPos.y                 = self->position.y - self->finRadius * RSDK.Cos512(self->rotation);
        self->finAnimator.frameID = ((self->rotation + 16) >> 5) & 0xF;
        RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

        drawPos.x = ((5 * self->finRadius * RSDK.Sin512(self->rotation)) >> 3) + self->position.x;
        drawPos.y = self->position.y - ((5 * self->finRadius * RSDK.Cos512(self->rotation)) >> 3);
        RSDK.DrawSprite(&self->armAnimator, &drawPos, false);

        self->rotation += 64;
    }

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    if (self->invincibilityTimer & 1)
        RSDK.CopyPalette(5, 128, 4, 128, 128);

    RSDK.SetActivePalette(0, 0, ScreenInfo[SceneInfo->currentScreenID].size.y);
}

void PhantomShinobi_Create(void *data)
{
    RSDK_THIS(PhantomShinobi);

    if (!SceneInfo->inEditor) {
        self->visible         = true;
        self->drawFX          = FX_ROTATE | FX_FLIP;
        self->drawGroup       = Zone->objectDrawGroup[0];
        self->active          = ACTIVE_NEVER;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x800000;
        self->collisionLayers = Zone->collisionLayers;
        self->startPos        = self->position;
        self->tileCollisions  = TILECOLLISION_DOWN;
        self->finRadius       = 0x1600;

        PhantomShinobi_ResetStates();

        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->armAnimator, true, 1);
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 1, &self->finAnimator, true, 0);
    }
}

void PhantomShinobi_StageLoad(void)
{
    PhantomShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE);

    PhantomShinobi->hitbox.left   = -24;
    PhantomShinobi->hitbox.top    = -24;
    PhantomShinobi->hitbox.right  = 24;
    PhantomShinobi->hitbox.bottom = 24;

    PhantomShinobi->sfxBlade = RSDK.GetSfx("TMZ3/ShinobiBlade.wav");
    PhantomShinobi->sfxHit   = RSDK.GetSfx("TMZ3/ShinobiHit.wav");

    Soundboard_LoadSfx("TMZ3/ShinobiBlade.wav", true, PhantomShinobi_SfxCheck_ShinobiBlade, StateMachine_None);
}

void PhantomShinobi_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomShinobi);

    foreach_active(Player, playerLoop)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(playerLoop, self, &PhantomShinobi->hitbox)
            && Player_CheckBossHit(playerLoop, self)) {
            PhantomShinobi_Hit();
            foreach_return;
        }
    }

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    int32 angle  = self->rotStore;

    foreach_active(Player, player)
    {
        for (int32 f = 0; f < 8; ++f) {
            self->position.x = storeX + self->finRadius * RSDK.Sin512(angle);
            self->position.y = storeY - self->finRadius * RSDK.Cos512(angle);
            if (RSDK.CheckObjectCollisionTouchCircle(player, 0xC0000, self, 0x80000)) {
                Player_Hurt(player, self);
            }

            angle += 0x40;
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void PhantomShinobi_Hit(void)
{
    RSDK_THIS(PhantomShinobi);

    self->invincibilityTimer = 48;
    RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
}

void PhantomShinobi_HandleTileCollisions(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->onGround) {
        self->finRadius += (0x1000 - self->finRadius) >> 3;
        if (!self->prevOnGround) {
            if (self->numBounces > 0) {
                self->velocity.y = -0x30000;
                self->numBounces--;
                self->onGround = false;
            }
        }
    }
    else {
        self->velocity.y += 0x3800;
        if (self->velocity.y > 0xC0000)
            self->velocity.y = 0xC0000;

        self->finRadius += (0x1600 - self->finRadius) >> 3;
    }

    int32 size = self->finRadius / 88;

    self->outerBox.left   = -size;
    self->outerBox.top    = -size;
    self->outerBox.right  = size;
    self->outerBox.bottom = size;

    self->innerBox.left   = 2 - size;
    self->innerBox.top    = -size;
    self->innerBox.right  = size - 2;
    self->innerBox.bottom = size;

    self->prevOnGround = self->onGround;
    RSDK.ProcessObjectMovement(self, &self->outerBox, &self->innerBox);
}

void PhantomShinobi_ResetStates(void)
{
    RSDK_THIS(PhantomShinobi);

    self->position = self->startPos;
    self->position.y -= 0x400000;
    self->finRadius   = 0;
    self->attackCount = 0;

    self->state     = PhantomShinobi_State_EnterShinobi;
    self->direction = RSDK.Rand(FLIP_NONE, FLIP_X + 1);
}

void PhantomShinobi_State_EnterShinobi(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->position.y < self->startPos.y) {
        self->position.y += 0x8000;
    }

    if (++self->timer >= 128) {
        self->finRadius += 0x100;
        if (self->finRadius >= 0x1600) {
            self->timer     = 0;
            self->finRadius = 0x1600;
            self->state     = PhantomShinobi_State_AttackDelay;
        }
    }
}

void PhantomShinobi_State_AttackDelay(void)
{
    RSDK_THIS(PhantomShinobi);

    if (++self->timer == 16) {
        self->timer = 0;
        self->state = PhantomShinobi_State_SetupAttack;
    }
}

void PhantomShinobi_State_SetupAttack(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->direction) {
        if (self->rotSpeed <= -0x600) {
            self->velocity.x -= 0x20000;
            self->rotSpeed   = -0x600;
            self->state      = PhantomShinobi_State_Moving;
            self->onGround   = false;
            self->numBounces = 1;
            self->velocity.y = -0x30000;
        }
        else {
            self->rotSpeed -= 12;
        }
    }
    else {
        if (self->rotSpeed >= 0x600) {
            self->rotSpeed   = 0x600;
            self->velocity.x = 0x20000;
            self->state      = PhantomShinobi_State_Moving;
            self->onGround   = false;
            self->numBounces = 1;
            self->velocity.y = -0x30000;
        }
        else {
            self->rotSpeed += 12;
        }
    }

    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Moving(void)
{
    RSDK_THIS(PhantomShinobi);

    PhantomShinobi_HandleTileCollisions();

    if (self->onGround && !self->prevOnGround) {
        self->groundVel = self->direction == FLIP_NONE ? 0x80000 : -0x80000;
    }

    if (!self->groundVel) {
        self->onGround   = false;
        self->velocity.x = self->direction == FLIP_NONE ? -0x38000 : 0x38000;
        self->velocity.y = -0x40000;

        self->numBounces = 1;
        self->direction ^= FLIP_X;
        self->state = PhantomShinobi_State_PrepareFinAttack;
    }

    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_PrepareFinAttack(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->velocity.y < 0x20000) {
        self->velocity.y += 0x2800;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }

    if (abs(self->rotSpeed) <= 256) {
        if (!(self->rotStore & 0x1C00)) {
            self->rotSpeed = 0;
            self->rotStore = 0;
            self->timer    = 0;
            self->state    = PhantomShinobi_State_ExtendFins;
        }
    }
    else {
        self->rotSpeed -= (self->rotSpeed >> 5);
    }

    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_ExtendFins(void)
{
    RSDK_THIS(PhantomShinobi);

    self->finRadius -= 128;
    if (++self->timer == 24) {
        self->timer      = 0;
        self->velocity.x = 0x400;

        RSDK.PlaySfx(PhantomShinobi->sfxHit, false, 255);
        self->state = PhantomShinobi_State_RetractFins;
    }

    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_RetractFins(void)
{
    RSDK_THIS(PhantomShinobi);

    self->velocity.x -= 16;
    if (self->velocity.x < -0xC00)
        self->velocity.x = -0xC00;

    self->finRadius += self->velocity.x;

    if (self->velocity.x < 0 && self->finRadius < 0x1600) {
        self->finRadius = 0x1600;

        if (++self->attackCount == 2) {
            PhantomEgg_SetupWarpFX();
            self->timer = 0;
            self->state = PhantomShinobi_State_FinishedAttack;
        }
        else {
            self->state = PhantomShinobi_State_AttackDelay;
        }
    }

    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_FinishedAttack(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->finRadius > 0)
        self->finRadius -= (self->finRadius >> 4);

    if (++self->timer == 60) {
        PhantomShinobi_ResetStates();
        self->active = ACTIVE_NEVER;
    }
}

bool32 PhantomShinobi_SfxCheck_ShinobiBlade(void)
{
    foreach_active(PhantomShinobi, shinobi)
    {
        if (shinobi->state == PhantomShinobi_State_Moving) {
            foreach_return true;
        }
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void PhantomShinobi_EditorDraw(void)
{
    RSDK_THIS(PhantomShinobi);

    self->finRadius = 0x1600;

    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->bodyAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->armAnimator, false, 1);
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 1, &self->finAnimator, false, 0);

    PhantomShinobi_Draw();
}

void PhantomShinobi_EditorLoad(void) { PhantomShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE); }
#endif

void PhantomShinobi_Serialize(void) {}
