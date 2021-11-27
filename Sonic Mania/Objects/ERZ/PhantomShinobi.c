#include "SonicMania.h"

ObjectPhantomShinobi *PhantomShinobi;

void PhantomShinobi_Update(void)
{
    RSDK_THIS(PhantomShinobi);
    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;
    StateMachine_Run(self->state);

    self->rotStore += self->field_78;
    self->rotStore &= 0xFFFF;
}

void PhantomShinobi_LateUpdate(void) {}

void PhantomShinobi_StaticUpdate(void) {}

void PhantomShinobi_Draw(void)
{
    RSDK_THIS(PhantomShinobi);

    RSDK.SetActivePalette(4, 0, ScreenInfo[SceneInfo->currentScreenID].height);

    if (self->invincibilityTimer & 1)
        RSDK.CopyPalette(6, 128, 4, 128, 128);
    self->rotation = self->rotStore >> 7;

    for (int32 i = 0; i < 8; ++i) {
        Vector2 drawPos;

        drawPos.x                 = self->spearOffset * RSDK.Sin512(self->rotation) + self->position.x;
        drawPos.y                 = self->position.y - self->spearOffset * RSDK.Cos512(self->rotation);
        self->animator3.frameID = ((self->rotation + 16) >> 5) & 0xF;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        drawPos.x = ((5 * self->spearOffset * RSDK.Sin512(self->rotation)) >> 3) + self->position.x;
        drawPos.y = self->position.y - ((5 * self->spearOffset * RSDK.Cos512(self->rotation)) >> 3);
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->rotation += 64;
    }

    RSDK.DrawSprite(&self->animator1, NULL, false);

    if (self->invincibilityTimer & 1)
        RSDK.CopyPalette(5, 128, 4, 128, 128);

    RSDK.SetActivePalette(0, 0, ScreenInfo[SceneInfo->currentScreenID].height);
}

void PhantomShinobi_Create(void *data)
{
    RSDK_THIS(PhantomShinobi);

    if (!SceneInfo->inEditor) {
        self->visible         = true;
        self->drawFX          = FX_ROTATE | FX_FLIP;
        self->drawOrder       = Zone->drawOrderLow;
        self->active          = ACTIVE_NEVER;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x800000;
        self->collisionLayers = Zone->fgLayers;
        self->startPos.x      = self->position.x;
        self->startPos.y      = self->position.y;
        self->tileCollisions  = true;
        self->spearOffset     = 0x1600;
        PhantomShinobi_ResetStates();
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->animator2, true, 1);
        RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 1, &self->animator3, true, 0);
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

    Soundboard_LoadSFX("TMZ3/ShinobiBlade.wav", true, PhantomShinobi_BladeCheckCB, NULL);
}

void PhantomShinobi_CheckPlayerCollisions(void)
{
    RSDK_THIS(PhantomShinobi);

    foreach_active(Player, playerLoop)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(playerLoop, self, &PhantomShinobi->hitbox)
            && Player_CheckBossHit(playerLoop, self)) {
            self->invincibilityTimer = 48;
            RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
            foreach_return;
        }
    }

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    int32 angle  = self->rotStore;
    foreach_active(Player, player)
    {
        self->position.x = storeX + self->spearOffset * RSDK.Sin512(angle);
        self->position.y = storeY - self->spearOffset * RSDK.Cos512(angle);
        if (RSDK.CheckObjectCollisionTouchCircle(player, 0xC0000, self, 0x80000)) {
            Player_CheckHit(player, self);
        }
        angle += 64;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void PhantomShinobi_HandleTileCollisions(void)
{
    RSDK_THIS(PhantomShinobi);
    if (self->onGround) {
        self->spearOffset += (4096 - self->spearOffset) >> 3;
        if (!self->prevOnGround) {
            if (self->field_7C > 0) {
                self->velocity.y = -0x30000;
                self->field_7C--;
                self->onGround = false;
            }
        }
    }
    else {
        self->velocity.y += 0x3800;
        if (self->velocity.y > 0xC0000)
            self->velocity.y = 0xC0000;
        self->spearOffset += (0x1600 - self->spearOffset) >> 3;
    }

    int32 val               = self->spearOffset / 88;
    self->outerBox.right  = val;
    self->outerBox.bottom = val;
    self->outerBox.left   = -val;
    self->outerBox.top    = -val;
    self->innerBox.left   = 2 - val;
    self->innerBox.right  = val - 2;
    self->innerBox.top    = -val;
    self->innerBox.bottom = val;

    self->prevOnGround = self->onGround;
    RSDK.ProcessTileCollisions(self, &self->outerBox, &self->innerBox);
}

void PhantomShinobi_ResetStates(void)
{
    RSDK_THIS(PhantomShinobi);

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
    self->position.y -= 0x400000;
    self->spearOffset = 0;
    self->field_80    = 0;
    self->state       = PhantomShinobi_State_Unknown1;
    self->direction   = RSDK.Rand(0, 2);
}

void PhantomShinobi_State_Unknown1(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->position.y < self->startPos.y) {
        self->position.y += 0x8000;
    }

    if (++self->timer >= 128) {
        self->spearOffset += 0x100;
        if (self->spearOffset >= 0x1600) {
            self->timer       = 0;
            self->spearOffset = 0x1600;
            self->state       = PhantomShinobi_State_Unknown2;
        }
    }
}

void PhantomShinobi_State_Unknown2(void)
{
    RSDK_THIS(PhantomShinobi);

    if (++self->timer == 16) {
        self->timer = 0;
        self->state = PhantomShinobi_State_Unknown3;
    }
}

void PhantomShinobi_State_Unknown3(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->direction) {
        if (self->field_78 > -0x600) {
            self->field_78 -= 12;
        }
        else {
            self->velocity.x -= 0x20000;
            self->field_78   = -0x600;
            self->state      = PhantomShinobi_State_Unknown4;
            self->onGround   = false;
            self->field_7C   = 1;
            self->velocity.y = -0x30000;
        }
    }
    else {
        if (self->field_78 >= 0x600) {
            self->field_78   = 0x600;
            self->velocity.x = 0x20000;
            self->state      = PhantomShinobi_State_Unknown4;
            self->onGround   = false;
            self->field_7C   = 1;
            self->velocity.y = -0x30000;
        }
        else {
            self->field_78 += 12;
        }
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown4(void)
{
    RSDK_THIS(PhantomShinobi);

    PhantomShinobi_HandleTileCollisions();
    if (self->onGround && !self->prevOnGround) {
        if (self->direction == FLIP_NONE)
            self->groundVel = 0x80000;
        else
            self->groundVel = -0x80000;
    }
    if (!self->groundVel) {
        self->onGround   = false;
        self->velocity.y = -0x40000;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x38000;
        else
            self->velocity.x = 0x38000;
        self->field_7C = 1;
        self->direction ^= FLIP_X;
        self->state = PhantomShinobi_State_Unknown5;
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown5(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->velocity.y < 0x20000) {
        self->velocity.y += 0x2800;
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }

    if (abs(self->field_78) <= 256) {
        if (!(self->rotStore & 0x1C00)) {
            self->field_78 = 0;
            self->rotStore = 0;
            self->timer    = 0;
            self->state    = PhantomShinobi_State_Unknown6;
        }
    }
    else {
        self->field_78 -= (self->field_78 >> 5);
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown6(void)
{
    RSDK_THIS(PhantomShinobi);

    self->spearOffset -= 128;
    if (++self->timer == 24) {
        self->timer      = 0;
        self->velocity.x = 0x400;
        RSDK.PlaySfx(PhantomShinobi->sfxHit, false, 255);
        self->state = PhantomShinobi_State_Unknown7;
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown7(void)
{
    RSDK_THIS(PhantomShinobi);

    self->velocity.x -= 16;
    if (self->velocity.x < -0xC00)
        self->velocity.x = -0xC00;

    self->spearOffset += self->velocity.x;
    if (self->velocity.x < 0 && self->spearOffset < 0x1600) {
        self->spearOffset = 0x1600;
        if (++self->field_80 == 2) {
            PhantomEgg_SetupScanlineCB();
            self->timer = 0;
            self->state = PhantomShinobi_State_Unknown8;
        }
        else {
            self->state = PhantomShinobi_State_Unknown2;
        }
    }
    PhantomShinobi_CheckPlayerCollisions();
}

void PhantomShinobi_State_Unknown8(void)
{
    RSDK_THIS(PhantomShinobi);

    if (self->spearOffset > 0) {
        self->spearOffset -= (self->spearOffset >> 4);
    }

    if (++self->timer == 60) {
        PhantomShinobi_ResetStates();
        self->active = ACTIVE_NEVER;
    }
}

bool32 PhantomShinobi_BladeCheckCB(void)
{
    foreach_active(PhantomShinobi, shinobi)
    {
        if (shinobi->state == PhantomShinobi_State_Unknown4) {
            foreach_return true;
        }
    }
    return false;
}

#if RETRO_INCLUDE_EDITOR
void PhantomShinobi_EditorDraw(void)
{
    RSDK_THIS(PhantomShinobi);

    self->spearOffset = 0x1600;
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 0, &self->animator2, false, 1);
    RSDK.SetSpriteAnimation(PhantomShinobi->aniFrames, 1, &self->animator3, false, 0);

    PhantomShinobi_Draw();
}

void PhantomShinobi_EditorLoad(void) { PhantomShinobi->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomShinobi.bin", SCOPE_STAGE); }
#endif

void PhantomShinobi_Serialize(void) {}
