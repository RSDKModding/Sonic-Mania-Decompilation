#include "SonicMania.h"

ObjectSplats *Splats;

void Splats_Update(void)
{
    RSDK_THIS(Splats);
    StateMachine_Run(self->state);
}

void Splats_LateUpdate(void) {}

void Splats_StaticUpdate(void) {}

void Splats_Draw(void)
{
    RSDK_THIS(Splats);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Splats_Create(void *data)
{
    RSDK_THIS(Splats);

    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->startDir      = self->direction;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (self->bounceCount < 1)
        self->bounceCount = 1;

    if (Splats->state == Splats_Unknown3) {
        self->drawFX |= FX_FLIP;
        self->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->animator1, true, 0);
        self->state = Splats_Unknown2;
    }
    else {
        switch (voidToInt(data)) {
            case 0:
                self->drawOrder = Zone->drawOrderHigh;
                self->delay     = 0;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &self->animator1, true, 0);
                self->state = Splats_Unknown2;
                break;
            case 1:
                self->drawFX |= FX_FLIP;
                self->drawOrder = Zone->drawOrderLow;
                self->active    = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->animator1, true, 0);
                self->animator1.loopIndex  = 1;
                self->state                = Splats_Unknown6;
                self->animator1.frameCount = 2;
                break;
            case 2:
                self->drawFX |= FX_FLIP;
                self->inkEffect |= INK_ALPHA;
                self->alpha     = 256;
                self->drawOrder = Zone->drawOrderHigh;
                self->active    = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Splats->aniFrames, 3, &self->animator1, true, 0);
                self->state = Splats_Unknown10;
                break;
        }
    }
}

void Splats_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_Unknown3;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_Unknown5;
    }
    Splats->hitbox1.left   = -10;
    Splats->hitbox1.top    = -20;
    Splats->hitbox1.right  = 6;
    Splats->hitbox1.bottom = 20;
    Splats->hitbox2.left   = -18;
    Splats->hitbox2.top    = -26;
    Splats->hitbox2.right  = 18;
    Splats->hitbox2.bottom = 17;
    Splats->hitbox3.left   = -18;
    Splats->hitbox3.top    = -146;
    Splats->hitbox3.right  = 18;
    Splats->hitbox3.bottom = -18;

    DEBUGMODE_ADD_OBJ(Splats);
    Splats->sfxSplatsSpawn = RSDK.GetSFX("PSZ/SplatsSpawn.wav");
    Splats->sfxSplatsLand  = RSDK.GetSFX("PSZ/SplatsLand.wav");
}

void Splats_DebugDraw(void)
{
    if (Splats->state == Splats_Unknown3)
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &DebugMode->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Splats->aniFrames, 1, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Splats_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Splats, NULL, self->position.x, self->position.y);
}

void Splats_CheckPlayerCollisions(void)
{
    RSDK_THIS(Splats);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Splats->hitbox1)) {
            if (Splats->state == Splats_Unknown3) {
                Player_CheckBadnikBreak(self, player, true);
            }
            else {
                if (Player_CheckBadnikBreak(self, player, false)) {
                    EntitySplats *parent = (EntitySplats *)self->parent;
                    if (parent) {
                        if (parent->objectID == Splats->objectID)
                            --parent->activeCount;
                    }
                    RSDK.ResetEntityPtr(self, TYPE_BLANK, NULL);
                    foreach_break;
                }
            }
        }
    }
}

void Splats_CheckOnScreen(void)
{
    RSDK_THIS(Splats);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        self->field_68   = 0;
        Splats_Create(NULL);
    }
}

void Splats_Unknown2(void)
{
    RSDK_THIS(Splats);
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;
    self->state      = Splats->state;
    StateMachine_Run(self->state);
}

void Splats_Unknown3(void)
{
    RSDK_THIS(Splats);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > 0 && RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, 0, 0x100000, true)) {
        if (self->bounceCount) {
            if (++self->activeCount >= self->bounceCount) {
                self->activeCount = 0;
                self->direction ^= FLIP_X;
                self->velocity.x = -self->velocity.x;
            }
        }
        self->position.y -= 0x80000;
        self->velocity.y = -0x40000;
    }
    self->animator1.frameID = self->velocity.y < 0;
    Splats_CheckPlayerCollisions();
    Splats_CheckOnScreen();
}

void Splats_Unknown4(void)
{
    RSDK_THIS(Splats);
    self->field_68 = 0;
    self->delay    = 0;
    self->active   = ACTIVE_NORMAL;
    self->state    = Splats_Unknown5;
    Splats_Unknown5();
}

void Splats_Unknown5(void)
{
    RSDK_THIS(Splats);
    if (!self->field_68) {
        if (!self->activeScreens)
            return;
        self->field_68 = true;
    }
    RSDK.ProcessAnimation(&self->animator2);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        Player_CheckCollisionBox(player, self, &Splats->hitbox2);
        flag |= Player_CheckCollisionTouch(player, self, &Splats->hitbox3);
        flag |= (player->animator.animationID == ANI_SPINDASH);
    }

    if (--self->delay <= 0) {
        if (flag || (self->activeCount >= self->numActive)) {
            self->delay++;
        }
        else {
            RSDK.SetSpriteAnimation(Splats->aniFrames, 2, &self->animator2, true, 0);
            self->delay = self->minDelay;
            RSDK.PlaySfx(Splats->sfxSplatsSpawn, false, 255);
            EntitySplats *child = CREATE_ENTITY(Splats, intToVoid(1), self->position.x, self->position.y - 0x60000);
            child->parent       = (Entity *)self;
            child->bounceCount  = self->bounceCount;
            child->direction    = self->direction;
            child->velocity.y   = -0x60000;
            if (self->direction == FLIP_NONE)
                child->velocity.x = -0x10000;
            else
                child->velocity.x = 0x10000;
            ++self->activeCount;
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL)) {
        self->state  = Splats_Unknown4;
        self->active = ACTIVE_NORMAL;
    }
}

void Splats_Unknown6(void)
{
    RSDK_THIS(Splats);
    RSDK.ProcessAnimation(&self->animator1);
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > -0x40000)
        self->position.x += self->velocity.x;

    if (self->velocity.y > 0) {
        self->animator1.loopIndex  = 4;
        self->animator1.frameCount = 5;
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, 0, 0x120000, true)) {
            RSDK.PlaySfx(Splats->sfxSplatsLand, false, 255);
            EntitySplats *splats = CREATE_ENTITY(Splats, intToVoid(2), self->position.x, self->position.y);
            splats->direction    = self->direction;
            self->delay        = 4;
            self->state        = Splats_Unknown8;
        }
    }
    Splats_CheckPlayerCollisions();
}

void Splats_Unknown7(void)
{
    RSDK_THIS(Splats);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->velocity.y > 0) {
        self->animator1.loopIndex  = 4;
        self->animator1.frameCount = 5;
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, 0, 0, 0, 0x120000, true)) {
            RSDK.PlaySfx(Splats->sfxSplatsLand, 0, 0xFF);
            if (self->bounceCount) {
                if (++self->activeCount < self->bounceCount) {
                    self->delay = 4;
                    self->state = Splats_Unknown8;
                }
                else {
                    RSDK.SetSpriteAnimation(Splats->aniFrames, 4, &self->animator1, true, 0);
                    self->state       = Splats_Unknown9;
                    self->activeCount = 0;
                }
            }

            EntitySplats *splats = CREATE_ENTITY(Splats, intToVoid(2), self->position.x, self->position.y);
            splats->direction    = self->direction;
        }
    }

    if (RSDK.CheckOnScreen(self, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = (EntitySplats *)self->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
        destroyEntity(self);
    }
}

void Splats_Unknown8(void)
{
    RSDK_THIS(Splats);
    if (--self->delay <= 0) {
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->animator1, true, 0);
        self->animator1.loopIndex  = 1;
        self->animator1.frameCount = 2;
        self->position.y -= 0x80000;
        self->velocity.y = -0x40000;
        self->state      = Splats_Unknown7;
    }

    if (RSDK.CheckOnScreen(self, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = (EntitySplats *)self->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
        destroyEntity(self);
    }
}

void Splats_Unknown9(void)
{
    RSDK_THIS(Splats);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Splats->aniFrames, 0, &self->animator1, true, 0);
        self->position.y -= 0x80000;
        self->animator1.frameCount = 2;
        self->animator1.loopIndex  = 1;
        self->direction ^= FLIP_X;
        self->velocity.y = -0x40000;
        self->velocity.x = -self->velocity.x;
        self->state      = Splats_Unknown7;
    }

    if (RSDK.CheckOnScreen(self, NULL)) {
        Splats_CheckPlayerCollisions();
    }
    else {
        EntitySplats *parent = (EntitySplats *)self->parent;
        if (parent) {
            if (parent->objectID == Splats->objectID)
                --parent->activeCount;
        }
        destroyEntity(self);
    }
}

void Splats_Unknown10(void)
{
    RSDK_THIS(Splats);
    RSDK.ProcessAnimation(&self->animator1);

    if (self->delay >= 30) {
        self->alpha -= 2;
        if (!self->alpha)
            destroyEntity(self);
    }
    else {
        self->delay++;
    }
}

#if RETRO_INCLUDE_EDITOR
void Splats_EditorDraw(void) { Splats_Draw(); }

void Splats_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_Unknown3;
    }
    else if (RSDK.CheckStageFolder("PSZ1")) {
        Splats->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Splats.bin", SCOPE_STAGE);
        Splats->state     = Splats_Unknown5;
    }
}
#endif

void Splats_Serialize(void)
{
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, bounceCount);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT8, numActive);
    RSDK_EDITABLE_VAR(Splats, VAR_UINT16, minDelay);
}
