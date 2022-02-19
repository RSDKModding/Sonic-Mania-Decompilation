// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Buggernaut Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&self->animator2, NULL, false);

    self->inkEffect = INK_NONE;
}

void Buggernaut_Create(void *data)
{
    RSDK_THIS(Buggernaut);

    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow + 1;
    if (!SceneInfo->inEditor) {
        self->drawFX |= FX_FLIP;
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->direction     = FLIP_NONE;
        self->alpha         = 128;
        self->timer         = 16;
        self->timer2        = -1;
        RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 2, &self->animator2, true, 0);
        self->state = Buggernaut_State_Setup;
    }
}

void Buggernaut_StageLoad(void)
{
    Buggernaut->aniFrames           = RSDK.LoadSpriteAnimation("HCZ/Buggernaut.bin", SCOPE_STAGE);
    Buggernaut->hitboxBadnik.left   = -8;
    Buggernaut->hitboxBadnik.top    = -8;
    Buggernaut->hitboxBadnik.right  = 8;
    Buggernaut->hitboxBadnik.bottom = 8;
    Buggernaut->hitbox2.left        = -64;
    Buggernaut->hitbox2.top         = -64;
    Buggernaut->hitbox2.right       = 64;
    Buggernaut->hitbox2.bottom      = 64;

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
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Buggernaut_CheckOffScreen(void)
{
    RSDK_THIS(Buggernaut);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        if (self->animator1.animationID) {
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
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x90000, -0x80000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x90000, 0x80000, false);
    }
    else {
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x90000, -0x80000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x90000, 0x80000, false);
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
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x80000, -0x90000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x80000, 0x90000, false);
    }
    else {
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x80000, 0x90000, false);
        collided |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x80000, 0x90000, false);
    }

    if (collided)
        self->velocity.y = -self->velocity.y;
    return collided;
}

void Buggernaut_State_Setup(void)
{
    RSDK_THIS(Buggernaut);

    self->active     = ACTIVE_NORMAL;
    int y              = self->position.y;
    int x              = self->position.x;
    self->velocity.x = 0;
    self->velocity.y = 0;

    EntityBuggernaut *child = CREATE_ENTITY(Buggernaut, intToVoid(true), x, y);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 1, &child->animator1, true, 0);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 3, &child->animator2, true, 0);
    child->active      = ACTIVE_NORMAL;
    child->drawOrder   = self->drawOrder - 1;
    child->parent      = (Entity *)self;
    child->passThrough = self->passThrough;
    child->isPermanent = true;
    child->state       = Buggernaut_State2_Unknown;
    self->state      = Buggernaut_State_Unknown1;
    Buggernaut_State_Unknown1();
}

void Buggernaut_State_Unknown1(void)
{
    RSDK_THIS(Buggernaut);
    self->timer--;
    if (--self->timer <= 0) {
        if (--self->timer2) {
            self->velocity.y = -0x10000;
            self->timer      = 96;
            self->state      = Buggernaut_State_Unknown2;
        }
        else {
            self->velocity.x = RSDK.Rand(-2, 3) << 16;
            self->velocity.y = RSDK.Rand(-2, 3) << 16;

            if (self->velocity.y == 0 || self->velocity.x == 0)
                self->velocity.x = -0x20000;
            self->velocity.y = -0x20000;
            self->state      = Buggernaut_State_Unknown3;
        }
    }
    RSDK.ProcessAnimation(&self->animator2);
    Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOffScreen();
}

void Buggernaut_State_Unknown2(void)
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
        self->state      = Buggernaut_State_Unknown1;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->direction  = FLIP_NONE;
    }
    RSDK.ProcessAnimation(&self->animator2);
    Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOffScreen();
}

void Buggernaut_State_Unknown3(void)
{
    RSDK_THIS(Buggernaut);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    RSDK.ProcessAnimation(&self->animator2);
    if (!self->animator1.animationID)
        Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOffScreen();
}

void Buggernaut_State2_Unknown(void)
{
    RSDK_THIS(Buggernaut);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    EntityBuggernaut *buggernaut = (EntityBuggernaut *)self->parent;
    if (!buggernaut || buggernaut->objectID != Buggernaut->objectID) {
        self->parent = NULL;
        foreach_active(Buggernaut, buggernaut)
        {
            if (buggernaut->animator1.animationID == 0
                && RSDK.CheckObjectCollisionTouchBox(buggernaut, &Buggernaut->hitboxBadnik, self, &Buggernaut->hitbox2)) {
                self->parent = (Entity *)buggernaut;
                foreach_break;
            }
        }

        if (!self->parent) {
            self->velocity.x = RSDK.Rand(-2, 3) << 16;
            self->velocity.y = RSDK.Rand(-2, 3) << 16;

            if (!self->velocity.y || !self->velocity.x)
                self->velocity.x = -0x20000;
            self->velocity.y = -0x20000;
            self->state      = Buggernaut_State_Unknown3;
        }
    }

    if (self->parent) {
        if (buggernaut->position.x >= self->position.x) {
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

        if (buggernaut->position.y >= self->position.y) {
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

    RSDK.ProcessAnimation(&self->animator2);
    if (buggernaut->state == Buggernaut_State_Setup)
        destroyEntity(self);
}

void Buggernaut_EditorDraw(void)
{
    RSDK_THIS(Buggernaut);
    self->drawFX = FX_FLIP;
    self->alpha  = 0x80;
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 2, &self->animator2, true, 0);

    Buggernaut_Draw();
}

void Buggernaut_EditorLoad(void) { Buggernaut->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Buggernaut.bin", SCOPE_STAGE); }

void Buggernaut_Serialize(void)
{
    RSDK_EDITABLE_VAR(Buggernaut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Buggernaut, VAR_BOOL, passThrough);
}
