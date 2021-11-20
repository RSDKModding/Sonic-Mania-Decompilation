#include "SonicMania.h"

ObjectOctus *Octus;

void Octus_Update(void)
{
    RSDK_THIS(Octus);
    StateMachine_Run(self->state);
}

void Octus_LateUpdate(void) {}

void Octus_StaticUpdate(void) {}

void Octus_Draw(void)
{
    RSDK_THIS(Octus);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Octus_Create(void *data)
{
    RSDK_THIS(Octus);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    self->drawFX |= FX_FLIP;
    self->startPos = self->position;
    self->startDir = self->direction;
    self->timer    = 128;
    self->field_60 = 0;
    self->field_70 = 0;

    if (data) {
        self->inkEffect     = INK_ADD;
        self->alpha         = 192;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x200000;
        self->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 3, &self->animator, true, 0);
        self->state = Octus_State_Shot;
    }
    else {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &self->animator, true, 0);
        self->state = Octus_State_Setup;
        self->alpha = 0xC0;
    }
}

void Octus_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        Octus->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Octus.bin", SCOPE_STAGE);
    Octus->hitbox1.left   = -16;
    Octus->hitbox1.top    = -12;
    Octus->hitbox1.right  = 16;
    Octus->hitbox1.bottom = 12;
    Octus->hitbox3.left   = -128;
    Octus->hitbox3.top    = -512;
    Octus->hitbox3.right  = 128;
    Octus->hitbox3.bottom = 512;
    Octus->hitbox2.left   = -4;
    Octus->hitbox2.top    = -4;
    Octus->hitbox2.right  = 4;
    Octus->hitbox2.bottom = 4;
    Octus->sfxShot        = RSDK.GetSfx("Stage/Shot.wav");
    DEBUGMODE_ADD_OBJ(Octus);
}

void Octus_DebugSpawn(void)
{
    RSDK_THIS(Octus);
    EntityOctus *octus = CREATE_ENTITY(Octus, NULL, self->position.x, self->position.y);
    octus->direction   = self->direction;
    octus->startDir    = self->direction;
}

void Octus_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Octus_CheckPlayerCollisions(void)
{
    RSDK_THIS(Octus);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Octus->hitbox1))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Octus_CheckOnScreen(void)
{
    RSDK_THIS(Octus);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        Octus_Create(NULL);
    }
}

void Octus_State_Setup(void)
{
    RSDK_THIS(Octus);
    self->active = ACTIVE_NORMAL;
    self->state  = Octus_Unknown5;
    Octus_Unknown5();
}

void Octus_Unknown5(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&self->animator);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Octus->hitbox3)) {
            self->timer = 32;
            RSDK.SetSpriteAnimation(Octus->aniFrames, 1, &self->animator, true, 0);
            self->state = Octus_Unknown6;
            foreach_break;
        }
    }

    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown6(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&self->animator);
    if (--self->timer <= 0) {
        self->velocity.y = -0x20000;
        self->state      = Octus_Unknown7;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown7(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1000;
    if (self->velocity.y >= 0) {
        self->timer = 60;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 2, &self->animator, true, 0);
        self->state = Octus_Unknown8;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown8(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&self->animator);

    self->timer--;
    if (self->timer == 51) {
        EntityOctus *shot = CREATE_ENTITY(Octus, intToVoid(true), self->position.x, self->position.y);
        if (self->direction) {
            shot->position.x += 0xE0000;
            shot->velocity.x = 0x20000;
        }
        else {
            shot->position.x -= 0xE0000;
            shot->velocity.x = -0x20000;
        }
        RSDK.PlaySfx(Octus->sfxShot, false, 255);
    }
    else if (self->timer <= 0) {
        self->state = Octus_Unknown9;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown9(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1000;
    if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, 1)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &self->animator, true, 0);
        self->state = Octus_Unknown5;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_State_Shot(void)
{
    RSDK_THIS(Octus);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Octus->hitbox2)) 
                Player_CheckProjectileHit(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Octus_EditorDraw(void) { Octus_Draw(); }

void Octus_EditorLoad(void) { Octus->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Octus.bin", SCOPE_STAGE); }
#endif

void Octus_Serialize(void) { RSDK_EDITABLE_VAR(Octus, VAR_UINT8, direction); }
