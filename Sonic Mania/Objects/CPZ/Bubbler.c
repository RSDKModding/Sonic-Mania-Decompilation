#include "SonicMania.h"

ObjectBubbler *Bubbler = NULL;

void Bubbler_Update(void)
{
    RSDK_THIS(Bubbler);
    StateMachine_Run(self->state);
}

void Bubbler_LateUpdate(void) {}

void Bubbler_StaticUpdate(void) {}

void Bubbler_Draw(void)
{
    RSDK_THIS(Bubbler);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->startPos.x)
        RSDK.DrawSprite(&self->animator2, NULL, false);
}

void Bubbler_Create(void *data)
{
    RSDK_THIS(Bubbler);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 3, &self->animator1, true, 0);
        self->state = Bubbler_State_Projectile_Unknown1;
    }
    else {
        self->startPos   = self->position;
        self->startDir   = self->direction;
        self->velocity.y = 0;
        if (!self->direction)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
        self->timer  = 0;
        self->timer2 = 32;
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 1, &self->animator2, true, 0);
        self->state = Bubbler_State_Setup;
    }
}

void Bubbler_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        Bubbler->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Bubbler.bin", SCOPE_STAGE);
    Bubbler->hitbox1.left   = -16;
    Bubbler->hitbox1.top    = -12;
    Bubbler->hitbox1.right  = 16;
    Bubbler->hitbox1.bottom = 12;
    Bubbler->hitbox2.left   = -160;
    Bubbler->hitbox2.top    = -12;
    Bubbler->hitbox2.right  = 16;
    Bubbler->hitbox2.bottom = 96;
    Bubbler->hitbox3.left   = -2;
    Bubbler->hitbox3.top    = -2;
    Bubbler->hitbox3.right  = 2;
    Bubbler->hitbox3.bottom = 2;
    DEBUGMODE_ADD_OBJ(Bubbler);
}

void Bubbler_DebugSpawn(void)
{
    RSDK_THIS(Bubbler);
    CREATE_ENTITY(Bubbler, NULL, self->position.x, self->position.y);
}

void Bubbler_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Bubbler_HandleInteractions(void)
{
    RSDK_THIS(Bubbler);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Bubbler->hitbox1))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Bubbler_HandleProjectileInteractions(void)
{
    RSDK_THIS(Bubbler);
    int32 distance = 0x7FFFFFFF;
    foreach_active(Player, player)
    {
        if (abs(player->position.x - self->position.x) < distance) {
            distance          = abs(player->position.x - self->position.x);
            self->direction = player->position.x >= self->position.x;
        }

        if (Player_CheckCollisionTouch(player, self, &Bubbler->hitbox3)) {
            Player_CheckHit(player, self);
        }
    }
}

void Bubbler_CheckOnScreen(void)
{
    RSDK_THIS(Bubbler);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Bubbler_Create(NULL);
    }
}

void Bubbler_State_Setup(void)
{
    RSDK_THIS(Bubbler);
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;
    self->state      = Bubbler_State_Unknown1;
    Bubbler_State_Unknown1();
}

void Bubbler_State_Unknown1(void)
{
    RSDK_THIS(Bubbler);
    self->position.x += self->velocity.x;
    RSDK.ProcessAnimation(&self->animator2);

    if (!--self->timer) {
        self->direction ^= 1;
        self->velocity.x = -self->velocity.x;
        self->timer      = 512;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Bubbler->hitbox2)) {
            self->timer = 16;
            self->state = Bubbler_State_Unknown2;
            if (self->direction == FLIP_NONE)
                self->velocity.x = -0x28000;
            else
                self->velocity.x = 0x28000;
        }
    }

    Bubbler_HandleInteractions();
    Bubbler_CheckOnScreen();
}

void Bubbler_State_Unknown2(void)
{
    RSDK_THIS(Bubbler);

    RSDK.ProcessAnimation(&self->animator2);
    if (--self->timer & 0x8000) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 2, &self->animator2, true, 0);
        self->state = Bubbler_State_Unknown3;
    }
    Bubbler_HandleInteractions();
    Bubbler_CheckOnScreen();
}

void Bubbler_State_Unknown3(void)
{
    RSDK_THIS(Bubbler);
    RSDK.ProcessAnimation(&self->animator2);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (++self->timer2 >= 30) {
        self->timer2 = 0;
        int32 spawnX     = self->position.x + 0x60000;
        if (self->direction)
            spawnX = self->position.x - 0x60000;
        CREATE_ENTITY(Bubbler, intToVoid(true), spawnX, self->position.y + 0xA0000)->active = ACTIVE_NORMAL;
    }
    Bubbler_HandleInteractions();
    Bubbler_CheckOnScreen();
}

void Bubbler_State_Projectile_Unknown1(void)
{
    RSDK_THIS(Bubbler);
    RSDK.ProcessAnimation(&self->animator1);
    self->position.y += 0x10000;
    Bubbler_HandleProjectileInteractions();
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, false)) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 4, &self->animator1, true, 0);
        self->state = Bubbler_State_Projectile_Unknown2;
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Bubbler_State_Projectile_Unknown2(void)
{
    RSDK_THIS(Bubbler);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID >= 7)
        destroyEntity(self);
    else if (self->animator1.frameID < 6)
        Bubbler_HandleProjectileInteractions();
}

#if RETRO_INCLUDE_EDITOR
void Bubbler_EditorDraw(void)
{
    RSDK_THIS(Bubbler);
    self->startPos   = self->position;
    RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Bubbler->aniFrames, 1, &self->animator2, true, 0);

    Bubbler_Draw();
}

void Bubbler_EditorLoad(void) { Bubbler->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Bubbler.bin", SCOPE_STAGE); }
#endif

void Bubbler_Serialize(void) { RSDK_EDITABLE_VAR(Bubbler, VAR_UINT8, direction); }
