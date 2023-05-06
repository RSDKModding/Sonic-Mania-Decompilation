// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Buggernaut Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

// NOTE:
// This object was prolly based on Unused/Wisp
// it looks like it has the same structure, and even has "buzzCount", even though its set to -1

#include "Game.h"

ObjectBuggernaut *Buggernaut;

void Buggernaut_Update(void)
{
    RSDK_THIS(Buggernaut);

    StateMachine_Run(self->state);
}

void Buggernaut_LateUpdate(void) {}

void Buggernaut_StaticUpdate(void) {}

void Buggernaut_Draw(void)
{
    RSDK_THIS(Buggernaut);

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);

    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->wingAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void Buggernaut_Create(void *data)
{
    RSDK_THIS(Buggernaut);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0] + 1;

    if (!SceneInfo->inEditor) {
        self->drawFX |= FX_FLIP;
        self->startPos      = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->direction     = FLIP_NONE;
        self->alpha         = 128;
        self->timer         = 16;
        self->buzzCount     = -1;

        RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 2, &self->wingAnimator, true, 0);

        self->state = Buggernaut_State_Init;
    }
}

void Buggernaut_StageLoad(void)
{
    Buggernaut->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Buggernaut.bin", SCOPE_STAGE);

    Buggernaut->hitboxBadnik.left   = -8;
    Buggernaut->hitboxBadnik.top    = -8;
    Buggernaut->hitboxBadnik.right  = 8;
    Buggernaut->hitboxBadnik.bottom = 8;

    Buggernaut->hitboxParentRange.left   = -64;
    Buggernaut->hitboxParentRange.top    = -64;
    Buggernaut->hitboxParentRange.right  = 64;
    Buggernaut->hitboxParentRange.bottom = 64;

    DEBUGMODE_ADD_OBJ(Buggernaut);
}

void Buggernaut_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Buggernaut, NULL, self->position.x, self->position.y);
}

void Buggernaut_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Buggernaut_CheckPlayerCollisions(void)
{
    RSDK_THIS(Buggernaut);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Buggernaut->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Buggernaut_CheckOffScreen(void)
{
    RSDK_THIS(Buggernaut);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        if (self->bodyAnimator.animationID) {
            destroyEntity(self);
        }
        else {
            self->position = self->startPos;
            Buggernaut_Create(NULL);
        }
    }
}

bool32 Buggernaut_HandleTileCollisionsX(void)
{
    RSDK_THIS(Buggernaut);

    bool32 collided = false;
    if (self->velocity.x <= 0) {
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x90000, -0x80000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x90000, 0x80000, false);
    }
    else {
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x90000, -0x80000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x90000, 0x80000, false);
    }

    if (collided)
        self->velocity.x = -self->velocity.x;

    return collided;
}

bool32 Buggernaut_HandleTileCollisionsY(void)
{
    RSDK_THIS(Buggernaut);

    bool32 collided = false;
    if (self->velocity.y <= 0) {
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x80000, -0x90000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x80000, 0x90000, false);
    }
    else {
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x80000, 0x90000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x80000, 0x90000, false);
    }

    if (collided)
        self->velocity.y = -self->velocity.y;

    return collided;
}

void Buggernaut_State_Init(void)
{
    RSDK_THIS(Buggernaut);

    self->active     = ACTIVE_NORMAL;
    int32 x          = self->position.x;
    int32 y          = self->position.y;
    self->velocity.x = 0;
    self->velocity.y = 0;

    EntityBuggernaut *child = CREATE_ENTITY(Buggernaut, INT_TO_VOID(true), x, y);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 1, &child->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 3, &child->wingAnimator, true, 0);
    child->active      = ACTIVE_NORMAL;
    child->drawGroup   = self->drawGroup - 1;
    child->parent      = self;
    child->passThrough = self->passThrough;
    child->isPermanent = true;
    child->state       = Buggernaut_State_Child;

    self->state = Buggernaut_State_Idle;
    Buggernaut_State_Idle();
}

void Buggernaut_State_Idle(void)
{
    RSDK_THIS(Buggernaut);

    if (--self->timer <= 0) {
        if (--self->buzzCount) {
            self->velocity.y = -0x10000;
            self->timer      = 96;
            self->state      = Buggernaut_State_FlyTowardTarget;
        }
        else {
            self->velocity.x = RSDK.Rand(-2, 3) << 16;
            self->velocity.y = RSDK.Rand(-2, 3) << 16;

            if (!self->velocity.x || !self->velocity.y)
                self->velocity.x = -0x20000;

            self->velocity.y = -0x20000;
            self->state      = Buggernaut_State_FlyAway;
        }
    }

    RSDK.ProcessAnimation(&self->wingAnimator);

    Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOffScreen();
}

void Buggernaut_State_FlyTowardTarget(void)
{
    RSDK_THIS(Buggernaut);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        if (player->position.x >= self->position.x) {
            self->velocity.x += 0x1000;

            if (self->velocity.x > 0x20000)
                self->velocity.x = 0x20000;

            self->direction = FLIP_X;
        }
        else {
            self->velocity.x -= 0x1000;

            if (self->velocity.x < -0x20000)
                self->velocity.x = -0x20000;

            self->direction = FLIP_NONE;
        }

        if (player->position.y >= self->position.y) {
            self->velocity.y += 0x1000;

            if (self->velocity.y > 0x20000)
                self->velocity.y = 0x20000;
        }
        else {
            self->velocity.y -= 0x1000;

            if (self->velocity.y < -0x20000)
                self->velocity.y = -0x20000;
        }
    }

    if (self->velocity.y > 0 && self->position.y > Water->waterLevel - 0x80000)
        self->velocity.y = -self->velocity.y;

    if (!self->passThrough) {
        if (abs(self->velocity.x) <= abs(self->velocity.y)) {
            if (!Buggernaut_HandleTileCollisionsY())
                Buggernaut_HandleTileCollisionsX();
        }
        else if (!Buggernaut_HandleTileCollisionsX()) {
            Buggernaut_HandleTileCollisionsY();
        }
    }

    if (!--self->timer) {
        self->timer      = RSDK.Rand(0, 32);
        self->state      = Buggernaut_State_Idle;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->direction  = FLIP_NONE;
    }

    RSDK.ProcessAnimation(&self->wingAnimator);

    Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOffScreen();
}

void Buggernaut_State_FlyAway(void)
{
    RSDK_THIS(Buggernaut);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->wingAnimator);

    if (!self->bodyAnimator.animationID)
        Buggernaut_CheckPlayerCollisions();

    Buggernaut_CheckOffScreen();
}

void Buggernaut_State_Child(void)
{
    RSDK_THIS(Buggernaut);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    EntityBuggernaut *parent = self->parent;
    if (!parent || parent->classID != Buggernaut->classID) {
        self->parent = NULL;
        foreach_active(Buggernaut, buggernaut)
        {
            if (buggernaut->bodyAnimator.animationID == 0
                && RSDK.CheckObjectCollisionTouchBox(buggernaut, &Buggernaut->hitboxBadnik, self, &Buggernaut->hitboxParentRange)) {
                self->parent = buggernaut;
                foreach_break;
            }
        }

        if (!self->parent) {
            self->velocity.x = RSDK.Rand(-2, 3) << 16;
            self->velocity.y = RSDK.Rand(-2, 3) << 16;

            if (!self->velocity.y || !self->velocity.x)
                self->velocity.x = -0x20000;
            self->velocity.y = -0x20000;
            self->state      = Buggernaut_State_FlyAway;
        }
    }

    if (self->parent) {
        if (parent->position.x >= self->position.x) {
            self->velocity.x += 0x2000;

            if (self->velocity.x > 0x20000)
                self->velocity.x = 0x20000;

            self->direction = FLIP_X;
        }
        else {
            self->velocity.x -= 0x2000;

            if (self->velocity.x < -0x20000)
                self->velocity.x = -0x20000;

            self->direction = FLIP_NONE;
        }

        if (parent->position.y >= self->position.y) {
            self->velocity.y += 0x2000;

            if (self->velocity.y > 0x20000)
                self->velocity.y = 0x20000;
        }
        else {
            self->velocity.y -= 0x2000;

            if (self->velocity.y < -0x20000)
                self->velocity.y = -0x20000;
        }

        if (self->velocity.y > 0 && self->position.y > Water->waterLevel - 0x80000)
            self->velocity.y = -self->velocity.y;

        if (!self->passThrough) {
            if (abs(self->velocity.x) <= abs(self->velocity.y)) {
                if (!Buggernaut_HandleTileCollisionsY())
                    Buggernaut_HandleTileCollisionsX();
            }
            else if (!Buggernaut_HandleTileCollisionsX()) {
                Buggernaut_HandleTileCollisionsY();
            }
        }
    }

    RSDK.ProcessAnimation(&self->wingAnimator);

    if (parent->state == Buggernaut_State_Init)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Buggernaut_EditorDraw(void)
{
    RSDK_THIS(Buggernaut);

    self->drawFX = FX_FLIP;
    self->alpha  = 0x80;
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 2, &self->wingAnimator, true, 0);

    Buggernaut_Draw();
}

void Buggernaut_EditorLoad(void)
{
    Buggernaut->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Buggernaut.bin", SCOPE_STAGE);

    // despite being editable, this variable is set to FLIP_NONE on create, thereby making the editable var's value unused
    RSDK_ACTIVE_VAR(Buggernaut, direction);
    RSDK_ENUM_VAR("(Unused)", FLIP_NONE);
    // RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    // RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Buggernaut_Serialize(void)
{
    RSDK_EDITABLE_VAR(Buggernaut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Buggernaut, VAR_BOOL, passThrough);
}
