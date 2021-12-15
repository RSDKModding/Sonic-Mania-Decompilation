// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Vultron Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectVultron *Vultron;

void Vultron_Update(void)
{
    RSDK_THIS(Vultron);
    StateMachine_Run(self->state);
}

void Vultron_LateUpdate(void) {}

void Vultron_StaticUpdate(void) {}

void Vultron_Draw(void)
{
    RSDK_THIS(Vultron);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
}

void Vultron_Create(void *data)
{
    RSDK_THIS(Vultron);
    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawOrder = Zone->drawOrderLow;
        self->drawFX    = FX_FLIP;
        if (data)
            self->type = voidToInt(data);
        self->active = ACTIVE_BOUNDS;

        switch (self->type) {
            case 0:
                self->updateRange.y = 0x800000;
                self->updateRange.x = (self->dist + 16) << 19;
                self->startPos      = self->position;
                self->startDir      = self->direction;
                RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &self->animator1, true, 0);
                self->state = Vultron_State_Setup;
                break;
            case 1:
                self->updateRange.x  = 0x800000;
                self->updateRange.y  = 0x800000;
                self->drawFX         = FX_ROTATE;
                self->rotation       = 192 * (SceneInfo->createSlot & 1) + 320;
                self->hitbox.left  = -12;
                self->hitbox.top  = -8;
                self->hitbox.right = 12;
                self->hitbox.bottom = 8;
                RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &self->animator2, true, 0);
                self->state = Vultron_State2_Unknown;
                break;
        }
    }
}

void Vultron_StageLoad(void)
{
    Vultron->sfxVultron = RSDK.GetSfx("MSZ/Vultron.wav");
    if (RSDK.CheckStageFolder("MSZ"))
        Vultron->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Vultron.bin", SCOPE_STAGE);

    Vultron->hitbox.left  = 0;
    Vultron->hitbox.top  = -64;
    Vultron->hitbox.right = 256;
    Vultron->hitbox.bottom = 128;
    DEBUGMODE_ADD_OBJ(Vultron);
}

void Vultron_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityVultron *vultron = CREATE_ENTITY(Vultron, NULL, self->position.x, self->position.y);
    vultron->direction     = self->direction;
    vultron->startDir      = self->direction;
    vultron->dist          = 64;
    vultron->updateRange.x = 0x2800000;
}

void Vultron_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Vultron_CheckPlayerCollisions(void)
{
    RSDK_THIS(Vultron);
    Hitbox *hitbox = RSDK.GetHitbox(&self->animator1, 0);
    int left       = (hitbox->left << 16) + (((hitbox->right - hitbox->left) << 15) & 0xFFFF0000);
    int top        = (hitbox->top << 16) + (((hitbox->bottom - hitbox->top) << 15) & 0xFFFF0000);
    if (self->direction == FLIP_X)
        left = -left;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, hitbox)) {
            self->position.x += left;
            self->position.y += top;
            if (!Player_CheckBadnikBreak(self, player, true)) {
                self->position.x -= left;
                self->position.y -= top;
            }
        }
    }
}

void Vultron_CheckOnScreen(void)
{
    RSDK_THIS(Vultron);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        Vultron_Create(NULL);
    }
}

void Vultron_State_Setup(void)
{
    RSDK_THIS(Vultron);
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = 0;
    self->state      = Vultron_State_Unknown1;
    Vultron_State_Unknown1();
}

void Vultron_State_Unknown1(void)
{
    RSDK_THIS(Vultron);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Vultron->hitbox)) {
            self->velocity.y = -0xD800;
            if (self->direction == FLIP_NONE)
                self->velocity.x = 0x8000;
            else
                self->velocity.x = -0x8000;
            self->storeY = self->position.y;
            RSDK.PlaySfx(Vultron->sfxVultron, false, 255);
            self->state = Vultron_State_Unknown2;
        }
    }

    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown2(void)
{
    RSDK_THIS(Vultron);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == 5) {
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &self->animator2, true, 0);
        self->state = Vultron_State_Unknown3;
        self->velocity.x *= 8;
    }
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown3(void)
{
    RSDK_THIS(Vultron);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1800;

    if (self->velocity.y <= 0) {
        self->distance   = self->dist;
        self->velocity.y = 0;
        self->state      = Vultron_State_Unknown4;
    }
    RSDK.ProcessAnimation(&self->animator2);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown4(void)
{
    RSDK_THIS(Vultron);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (!--self->distance) {
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        self->state = Vultron_State_Unknown5;
    }
    RSDK.ProcessAnimation(&self->animator2);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown5(void)
{
    RSDK_THIS(Vultron);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (self->direction)
        self->velocity.x += 0x1800;
    else
        self->velocity.y -= 0x1800;

    self->velocity.y -= 0x4000;
    if (self->velocity.y < -0x38000) {
        self->velocity.y = -0x38000;
        self->state      = Vultron_State_Unknown6;
    }
    RSDK.ProcessAnimation(&self->animator1);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State_Unknown6(void)
{
    RSDK_THIS(Vultron);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (self->direction) {
        self->velocity.x += 0x1800;
        if (self->velocity.x > 0)
            self->velocity.x = 0;
    }
    else {
        self->velocity.x -= 0x1800;
        if (self->velocity.x < 0)
            self->velocity.x = 0;
    }
    self->velocity.y += 0x1800;
    if (self->velocity.y > -0xC800) {
        self->direction  = self->direction ^ 1;
        self->position.y = self->storeY;
        if (self->direction == FLIP_NONE)
            self->velocity.x = 0x8000;
        else
            self->velocity.x = -0x8000;
        self->velocity.y = -0xC800;
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &self->animator1, true, 0);
        self->state = Vultron_State_Unknown2;
    }
    RSDK.ProcessAnimation(&self->animator1);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOnScreen();
}

void Vultron_State2_Unknown(void)
{
    RSDK_THIS(Vultron);
    EntityPlayer *playerPtr = Player_GetNearestPlayerX();
    RSDK.ProcessAnimation(&self->animator2);
    int angle = RSDK.ATan2((playerPtr->position.x - self->position.x) >> 16, (playerPtr->position.y - self->position.y) >> 16);

    int rot = 2 * angle - self->rotation;

    if (abs(2 * angle - self->rotation) >= abs(rot - 0x200)) {
        if (abs(rot - 0x200) < abs(rot + 0x200)) {
            self->rotation += ((rot - 0x200) >> 5);
        }
        else {
            self->rotation += ((rot + 0x200) >> 5);
        }
    }
    else {
        if (abs(2 * angle - self->rotation) < abs(rot + 0x200)) {
            self->rotation += (rot >> 5);
        }
        else {
            self->rotation += ((rot + 0x200) >> 5);
        }
    }

    self->rotation &= 0x1FF;
    self->position.x += RSDK.Cos512(self->rotation) << 9;
    self->position.y += RSDK.Sin512(self->rotation) << 9;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &self->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

#if RETRO_INCLUDE_EDITOR
void Vultron_EditorDraw(void)
{
    RSDK_THIS(Vultron);
    switch (self->type) {
        case 0:
            self->drawFX = FX_NONE;
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &self->animator1, false, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 2, &self->animator2, true, 0);
            break;
        case 1:
            self->drawFX   = FX_ROTATE;
            self->rotation = 192 * (SceneInfo->createSlot & 1) + 320;
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &self->animator1, false, 0);
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &self->animator2, false, 0);
            break;
    }

    Vultron_Draw();
}

void Vultron_EditorLoad(void) { Vultron->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Vultron.bin", SCOPE_STAGE); }
#endif

void Vultron_Serialize(void)
{
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT16, dist);
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT8, direction);
}
