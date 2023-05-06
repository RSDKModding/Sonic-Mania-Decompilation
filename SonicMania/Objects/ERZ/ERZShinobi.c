// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZShinobi Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectERZShinobi *ERZShinobi;

void ERZShinobi_Update(void)
{
    RSDK_THIS(ERZShinobi);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);

    self->rotStore = (self->rotStore + 8) & 0x1FF;
}

void ERZShinobi_LateUpdate(void) {}

void ERZShinobi_StaticUpdate(void) {}

void ERZShinobi_Draw(void)
{
    RSDK_THIS(ERZShinobi);

    if ((self->invincibilityTimer & 1))
        RSDK.CopyPalette(2, 128, 0, 128, 128);

    self->rotation = self->rotStore;

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

    if ((self->invincibilityTimer & 1))
        RSDK.CopyPalette(1, 128, 0, 128, 128);
}

void ERZShinobi_Create(void *data)
{
    RSDK_THIS(ERZShinobi);

    if (!SceneInfo->inEditor) {
        self->visible         = true;
        self->drawFX          = FX_ROTATE | FX_FLIP;
        self->drawGroup       = Zone->objectDrawGroup[0];
        self->active          = ACTIVE_NORMAL;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x800000;
        self->collisionLayers = Zone->collisionLayers;
        self->tileCollisions  = TILECOLLISION_DOWN;
        self->finRadius       = 0x1600;
        self->state           = ERZShinobi_State_Moving;

        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->armAnimator, true, 1);
        RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 1, &self->finAnimator, true, 0);
    }
}

void ERZShinobi_StageLoad(void)
{
    ERZShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE);

    ERZShinobi->hitbox.left   = -24;
    ERZShinobi->hitbox.top    = -24;
    ERZShinobi->hitbox.right  = 24;
    ERZShinobi->hitbox.bottom = 24;
}

void ERZShinobi_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZShinobi);

    foreach_active(Player, playerLoop)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(playerLoop, self, &ERZShinobi->hitbox) && Player_CheckBossHit(playerLoop, self)) {
            ERZShinobi_Hit();
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

void ERZShinobi_Hit(void)
{
    RSDK_THIS(ERZShinobi);

    self->invincibilityTimer = 48;
    RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
}

void ERZShinobi_HandleTileCollisions(void)
{
    RSDK_THIS(ERZShinobi);

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

void ERZShinobi_State_Moving(void)
{
    RSDK_THIS(ERZShinobi);

    ERZShinobi_HandleTileCollisions();

    if (++self->timer == 60) {
        self->timer = 0;
        if (self->onGround) {
            self->numBounces = 2;
            self->velocity.y = -0x80000;
            self->onGround   = false;
        }
    }

    ERZShinobi_CheckPlayerCollisions();
}

#if GAME_INCLUDE_EDITOR
void ERZShinobi_EditorDraw(void)
{
    RSDK_THIS(ERZShinobi);

    self->finRadius = 0x1600;

    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->bodyAnimator, false, 0);
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 0, &self->armAnimator, false, 1);
    RSDK.SetSpriteAnimation(ERZShinobi->aniFrames, 1, &self->finAnimator, false, 0);

    ERZShinobi_Draw();
}

void ERZShinobi_EditorLoad(void) { ERZShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE); }
#endif

void ERZShinobi_Serialize(void) {}
