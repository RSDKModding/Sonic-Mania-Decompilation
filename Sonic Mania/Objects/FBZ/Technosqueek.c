#include "SonicMania.h"

ObjectTechnosqueek *Technosqueek;

void Technosqueek_Update(void)
{
    RSDK_THIS(Technosqueek);
    StateMachine_Run(self->state);
}

void Technosqueek_LateUpdate(void) {}

void Technosqueek_StaticUpdate(void) {}

void Technosqueek_Draw(void)
{
    RSDK_THIS(Technosqueek);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->type) {
        if (self->direction & FLIP_Y)
            drawPos.y = self->position.y - self->field_6C;
        else
            drawPos.y = self->field_6C + self->position.y;
    }
    else {
        if (self->direction & FLIP_X)
            drawPos.x = self->position.x - self->field_6C;
        else
            drawPos.x = self->field_6C + self->position.x;
    }
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Technosqueek_Create(void *data)
{
    RSDK_THIS(Technosqueek);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    self->drawFX |= FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->startDir      = self->direction;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (!self->type) {
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 2, &self->animator2, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 3, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 5, &self->animator2, true, 0);
        }
        self->state = Technosqueek_State_Unknown1;
        self->distance *= 2;
    }
}

void Technosqueek_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        Technosqueek->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Technosqueek.bin", SCOPE_STAGE);
    Technosqueek->hitbox.left   = -4;
    Technosqueek->hitbox.top    = -4;
    Technosqueek->hitbox.right  = 4;
    Technosqueek->hitbox.bottom = 4;
    DEBUGMODE_ADD_OBJ(Technosqueek);
}

void Technosqueek_DebugSpawn(void)
{
    RSDK_THIS(Technosqueek);
    CREATE_ENTITY(Technosqueek, NULL, self->position.x, self->position.y);
}

void Technosqueek_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Technosqueek_HandlePlayerInteractions(void)
{
    RSDK_THIS(Technosqueek);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Technosqueek->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Technosqueek_CheckOnScreen(void)
{
    RSDK_THIS(Technosqueek);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->distance >>= 1;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        Technosqueek_Create(NULL);
    }
}

void Technosqueek_State_Unknown1(void)
{
    RSDK_THIS(Technosqueek);

    self->active = ACTIVE_NORMAL;
    bool32 flag    = false;
    if (self->type)
        flag = !(self->direction & FLIP_Y);
    else
        flag = !(self->direction & FLIP_X);
    if (flag) {
        self->field_64  = 0x2000;
        self->field_68  = self->speed << 14;
        self->groundVel = -self->field_68;
    }
    else {
        self->groundVel = self->speed << 14;
        self->field_64  = -0x2000;
        self->field_68  = -self->groundVel;
    }
    self->field_60 = self->distance >> 1;
    self->flag     = true;
    self->field_6C = 0;
    if (self->type) {
        self->state = Technosqueek_State_Unknown4;
        Technosqueek_State_Unknown4();
    }
    else {
        self->state = Technosqueek_State_Unknown2;
        Technosqueek_State_Unknown2();
    }
}

void Technosqueek_State_Unknown2(void)
{
    RSDK_THIS(Technosqueek);

    self->position.x += self->groundVel;
    if (self->field_60) {
        self->field_60--;
    }
    else {
        self->groundVel += self->field_64;
        if (self->groundVel == self->field_68) {
            self->field_60 = self->distance;
            self->field_68 = -self->field_68;
            self->field_64 = -self->field_64;
            self->flag     = true;
        }
    }

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->flag) {
        if (abs(self->groundVel) <= 0x10000 && self->animator1.animationID != 1)
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 1, &self->animator1, true, 0);
        if (self->animator1.frameID == 1) {
            self->field_6C = -0x80000;
        }
        else if (self->animator1.frameID == 2) {
            self->field_6C = -0x120000;
        }
    }
    else {
        if (self->field_6C < 0) {
            self->field_6C += abs(self->groundVel - abs(self->field_64));
        }
        else {
            self->field_6C = 0;
        }
    }
    if (!self->groundVel)
        self->state = Technosqueek_State_Unknown3;
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown3(void)
{
    RSDK_THIS(Technosqueek);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        self->flag = false;
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 0, &self->animator1, true, 0);
        self->state = Technosqueek_State_Unknown2;
        self->direction ^= 1u;
    }
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown4(void)
{
    RSDK_THIS(Technosqueek);

    self->position.y += self->groundVel;
    if (self->field_60) {
        self->field_60--;
    }
    else {
        self->groundVel += self->field_64;
        if (self->groundVel == self->field_68) {
            self->field_60 = self->distance;
            self->field_68 = -self->field_68;
            self->field_64 = -self->field_64;
            self->flag     = true;
        }
    }

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->flag) {
        if (abs(self->groundVel) <= 0x10000 && self->animator1.animationID != 4)
            RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 4, &self->animator1, true, 0);
        if (self->animator1.frameID == 1) {
            self->field_6C = -0x80000;
        }
        else if (self->animator1.frameID == 2) {
            self->field_6C = -0x120000;
        }
    }
    else {
        if (self->field_6C < 0) {
            self->field_6C += abs(self->groundVel - abs(self->field_64));
        }
        else {
            self->field_6C = 0;
        }
    }
    if (!self->groundVel)
        self->state = Technosqueek_State_Unknown5;
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown5(void)
{
    RSDK_THIS(Technosqueek);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        self->flag = false;
        RSDK.SetSpriteAnimation(Technosqueek->aniFrames, 3, &self->animator1, true, 0);
        self->state = Technosqueek_State_Unknown4;
        self->direction ^= FLIP_Y;
    }
    Technosqueek_HandlePlayerInteractions();
    Technosqueek_CheckOnScreen();
}

void Technosqueek_State_Unknown6(void)
{
    RSDK_THIS(Technosqueek);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->startDir   = self->direction;
        self->velocity.y = 0;
        self->speed      = 16;
        self->state      = Technosqueek_State_Unknown1;
    }
    RSDK.ProcessAnimation(&self->animator2);
    Technosqueek_HandlePlayerInteractions();
}

#if RETRO_INCLUDE_EDITOR
void Technosqueek_EditorDraw(void) {}

void Technosqueek_EditorLoad(void) {}
#endif

void Technosqueek_Serialize(void)
{
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(Technosqueek, VAR_UINT16, distance);
}
