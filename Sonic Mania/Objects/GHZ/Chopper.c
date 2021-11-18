#include "SonicMania.h"

ObjectChopper *Chopper;

void Chopper_Update(void)
{
    RSDK_THIS(Chopper);
    StateMachine_Run(self->state);
}

void Chopper_LateUpdate(void) {}

void Chopper_StaticUpdate(void) {}

void Chopper_Draw(void)
{
    RSDK_THIS(Chopper);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Chopper_Create(void *data)
{
    RSDK_THIS(Chopper);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x1200000;
    self->drawFX        = FX_FLIP;
    self->state         = Chopper_Unknown3;
}

void Chopper_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Chopper->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Chopper.bin", SCOPE_STAGE);
    Chopper->hitbox.left    = -10;
    Chopper->hitbox.top     = -20;
    Chopper->hitbox.right   = 6;
    Chopper->hitbox.bottom  = 20;
    Chopper->hitbox2.left   = -20;
    Chopper->hitbox2.top    = -6;
    Chopper->hitbox2.right  = 20;
    Chopper->hitbox2.bottom = 10;
    Chopper->hitbox3.left   = -160;
    Chopper->hitbox3.top    = -32;
    Chopper->hitbox3.right  = 16;
    Chopper->hitbox3.bottom = 32;
    Chopper->hitbox4.left   = -20;
    Chopper->hitbox4.top    = -24;
    Chopper->hitbox4.right  = 20;
    Chopper->hitbox4.bottom = -16;

    DEBUGMODE_ADD_OBJ(Chopper);
}
void Chopper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Chopper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Chopper_DebugSpawn(void)
{
    RSDK_THIS(Chopper);
    RSDK.CreateEntity(Chopper->objectID, 0, self->position.x, self->position.y);
}

void Chopper_CheckOnScreen(void)
{
    RSDK_THIS(Chopper);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        Chopper_Create(NULL);
    }
}

void Chopper_Unknown3(void)
{
    RSDK_THIS(Chopper);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;
    if (self->type == 0) {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 0, &self->animator, true, 0);
        self->state = Chopper_Unknown4;
        Chopper_Unknown4();
    }
    else {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, true, 0);
        self->state = Chopper_Unknown5;
        self->timer = 512;

        if (!self->direction)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
        Chopper_Unknown5();
    }
}

void Chopper_Unknown4(void)
{
    RSDK_THIS(Chopper);
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;
    if (self->velocity.y >= -0x38000) {
        if (self->velocity.y <= 0x38000) {
            self->animator.animationSpeed = 2;
        }
        else {
            self->animator.frameID        = 0;
            self->animator.animationSpeed = 0;
        }
    }
    else {
        self->animator.animationSpeed = 1;
    }

    if (self->position.y > self->startPos.y) {
        self->position.y = self->startPos.y;
        self->velocity.y = -0x70000;
    }
    RSDK.ProcessAnimation(&self->animator);
    Chopper_CheckPlayerCollisions();
    Chopper_CheckOnScreen();
}

void Chopper_Unknown5(void)
{
    RSDK_THIS(Chopper);
    self->position.x += self->velocity.x;

    bool32 flag = false;
    if (self->direction) {
        flag = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0xF0000, true);
    }
    else {
        flag = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0xF0000, true);
    }

    if (--self->timer <= 0 || flag) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
        self->timer      = 512;
    }

    if (self->charge) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Chopper->hitbox3)) {
                self->state = Chopper_Unknown6;
                RSDK.SetSpriteAnimation(Chopper->aniFrames, 2, &self->animator, false, 0);
                self->timer = 16;
                if (!self->direction)
                    self->velocity.x = -0x20000;
                else
                    self->velocity.x = 0x20000;
                if (abs(player->position.y - self->position.y) > 0x100000) {
                    if (player->position.y < self->position.y)
                        self->velocity.y = -0x8000;
                    else
                        self->velocity.y = 0x8000;
                }
            }
        }
    }
    RSDK.ProcessAnimation(&self->animator);
    Chopper_CheckPlayerCollisions2();
    Chopper_CheckOnScreen();
}

void Chopper_Unknown6(void)
{
    RSDK_THIS(Chopper);

    if (!self->timer)
        self->state = Chopper_Unknown7;
    else
        --self->timer;
    RSDK.ProcessAnimation(&self->animator);
    Chopper_CheckPlayerCollisions2();
    Chopper_CheckOnScreen();
}

void Chopper_Unknown7(void)
{
    RSDK_THIS(Chopper);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    bool32 flag = false;
    if (self->direction) {
        flag = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0xF0000, true);
    }
    else {
        flag = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0xF0000, true);
    }

    if (flag) {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, false, 0);
        self->timer      = 512;
        self->velocity.y = 0;
        if (self->direction == FLIP_X)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
        self->direction ^= FLIP_X;
        self->state = Chopper_Unknown5;
        flag          = false;
    }
    else if (self->velocity.y >= 0) {
        if (self->velocity.y > 0)
            flag = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true);
    }
    else {
        flag = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x100000, true);
        foreach_active(Water, water)
        {
            if (water->type == 1)
                flag |= !RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Chopper->hitbox4);
        }
    }

    if (flag) {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, false, 0);
        self->timer      = 512;
        self->velocity.y = 0;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
        self->state = Chopper_Unknown5;
    }

    RSDK.ProcessAnimation(&self->animator);
    Chopper_CheckPlayerCollisions2();
    Chopper_CheckOnScreen();
}

void Chopper_CheckPlayerCollisions(void)
{
    RSDK_THIS(Chopper);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Chopper->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Chopper_CheckPlayerCollisions2(void)
{
    RSDK_THIS(Chopper);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Chopper->hitbox2))
            Player_CheckBadnikBreak(self, player, true);
    }
}

#if RETRO_INCLUDE_EDITOR
void Chopper_EditorDraw(void)
{
    RSDK_THIS(Chopper);
    if (self->type == 0)
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 0, &self->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, true, 0);

    Chopper_Draw();
}

void Chopper_EditorLoad(void) { Chopper->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Chopper.bin", SCOPE_STAGE); }
#endif

void Chopper_Serialize(void)
{
    RSDK_EDITABLE_VAR(Chopper, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Chopper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Chopper, VAR_BOOL, charge);
}
