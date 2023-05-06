// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Vultron Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    RSDK.DrawSprite(&self->flameAnimator, NULL, false);
}

void Vultron_Create(void *data)
{
    RSDK_THIS(Vultron);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[0];
        self->drawFX    = FX_FLIP;

        if (data)
            self->type = VOID_TO_INT(data);

        self->active = ACTIVE_BOUNDS;

        switch (self->type) {
            case VULTRON_DIVE:
                self->updateRange.x = (self->dist + 16) << 19;
                self->updateRange.y = 0x800000;
                self->startPos      = self->position;
                self->startDir      = self->direction;

                RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &self->bodyAnimator, true, 0);
                self->state = Vultron_State_Init;
                break;

            case VULTRON_TARGET:
                self->updateRange.x = 0x800000;
                self->updateRange.y = 0x800000;
                self->drawFX        = FX_ROTATE;
                self->rotation      = 0xC0 * (SceneInfo->createSlot & 1) + 0x140;

                self->hitboxBadnik.left   = -12;
                self->hitboxBadnik.top    = -8;
                self->hitboxBadnik.right  = 12;
                self->hitboxBadnik.bottom = 8;

                RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &self->bodyAnimator, true, 0);
                RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &self->flameAnimator, true, 0);
                self->state = Vultron_State_Targeting;
                break;
        }
    }
}

void Vultron_StageLoad(void)
{
    Vultron->sfxVultron = RSDK.GetSfx("MSZ/Vultron.wav");

    if (RSDK.CheckSceneFolder("MSZ"))
        Vultron->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Vultron.bin", SCOPE_STAGE);

    Vultron->hitboxRange.left   = 0;
    Vultron->hitboxRange.top    = -64;
    Vultron->hitboxRange.right  = 256;
    Vultron->hitboxRange.bottom = 128;

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

    Hitbox *hitbox = RSDK.GetHitbox(&self->bodyAnimator, 0);
    int32 left     = (hitbox->left << 16) + (((hitbox->right - hitbox->left) << 15) & 0xFFFF0000);
    int32 top      = (hitbox->top << 16) + (((hitbox->bottom - hitbox->top) << 15) & 0xFFFF0000);
    if (self->direction == FLIP_X)
        left = -left;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, hitbox)) {
            self->position.x += left;
            self->position.y += top;

            if (!Player_CheckBadnikBreak(player, self, true)) {
                self->position.x -= left;
                self->position.y -= top;
            }
        }
    }
}

void Vultron_CheckOffScreen(void)
{
    RSDK_THIS(Vultron);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        RSDK.SetSpriteAnimation(-1, 0, &self->flameAnimator, true, 0);

        Vultron_Create(NULL);
    }
}

void Vultron_State_Init(void)
{
    RSDK_THIS(Vultron);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = 0;

    self->state = Vultron_State_CheckPlayerInRange;
    Vultron_State_CheckPlayerInRange();
}

void Vultron_State_CheckPlayerInRange(void)
{
    RSDK_THIS(Vultron);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Vultron->hitboxRange)) {
            self->velocity.x = self->direction == FLIP_NONE ? 0x8000 : -0x8000;
            self->velocity.y = -0xD800;
            self->storeY     = self->position.y;
            RSDK.PlaySfx(Vultron->sfxVultron, false, 255);
            self->state = Vultron_State_Hop;
        }
    }

    Vultron_CheckPlayerCollisions();
    Vultron_CheckOffScreen();
}

void Vultron_State_Hop(void)
{
    RSDK_THIS(Vultron);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    RSDK.ProcessAnimation(&self->bodyAnimator);

    if (self->bodyAnimator.frameID == 5) {
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &self->flameAnimator, true, 0);
        self->state = Vultron_State_Dive;
        self->velocity.x *= 8;
    }

    Vultron_CheckPlayerCollisions();
    Vultron_CheckOffScreen();
}

void Vultron_State_Dive(void)
{
    RSDK_THIS(Vultron);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1800;

    if (self->velocity.y <= 0) {
        self->distRemain = self->dist;
        self->velocity.y = 0;
        self->state      = Vultron_State_Flying;
    }

    RSDK.ProcessAnimation(&self->flameAnimator);

    Vultron_CheckPlayerCollisions();
    Vultron_CheckOffScreen();
}

void Vultron_State_Flying(void)
{
    RSDK_THIS(Vultron);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!--self->distRemain) {
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->flameAnimator, true, 0);
        self->state = Vultron_State_Rise;
    }

    RSDK.ProcessAnimation(&self->flameAnimator);
    Vultron_CheckPlayerCollisions();
    Vultron_CheckOffScreen();
}

void Vultron_State_Rise(void)
{
    RSDK_THIS(Vultron);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->velocity.x += self->direction ? 0x1800 : -0x1800;
    self->velocity.y -= 0x4000;

    if (self->velocity.y < -0x38000) {
        self->velocity.y = -0x38000;
        self->state      = Vultron_State_PrepareDive;
    }

    RSDK.ProcessAnimation(&self->bodyAnimator);

    Vultron_CheckPlayerCollisions();
    Vultron_CheckOffScreen();
}

void Vultron_State_PrepareDive(void)
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
        self->direction ^= FLIP_X;
        self->position.y = self->storeY;
        self->velocity.x = self->direction == FLIP_NONE ? 0x8000 : -0x8000;
        self->velocity.y = -0xC800;
        RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &self->bodyAnimator, true, 0);
        self->state = Vultron_State_Hop;
    }

    RSDK.ProcessAnimation(&self->bodyAnimator);

    Vultron_CheckPlayerCollisions();
    Vultron_CheckOffScreen();
}

void Vultron_State_Targeting(void)
{
    RSDK_THIS(Vultron);

    EntityPlayer *targetPlayer = Player_GetNearestPlayerX();

    RSDK.ProcessAnimation(&self->flameAnimator);

    int32 angle = RSDK.ATan2((targetPlayer->position.x - self->position.x) >> 16, (targetPlayer->position.y - self->position.y) >> 16);
    int32 rot   = (angle << 1) - self->rotation;

    if (abs(2 * angle - self->rotation) >= abs(rot - 0x200)) {
        if (abs(rot - 0x200) < abs(rot + 0x200))
            self->rotation += ((rot - 0x200) >> 5);
        else
            self->rotation += ((rot + 0x200) >> 5);
    }
    else {
        if (abs(2 * angle - self->rotation) < abs(rot + 0x200))
            self->rotation += (rot >> 5);
        else
            self->rotation += ((rot + 0x200) >> 5);
    }

    self->rotation &= 0x1FF;
    self->position.x += RSDK.Cos512(self->rotation) << 9;
    self->position.y += RSDK.Sin512(self->rotation) << 9;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &self->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

#if GAME_INCLUDE_EDITOR
void Vultron_EditorDraw(void)
{
    RSDK_THIS(Vultron);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    switch (self->type) {
        case VULTRON_DIVE:
            self->drawFX = FX_NONE;
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 0, &self->bodyAnimator, false, 0);
            RSDK.SetSpriteAnimation(-1, 2, &self->flameAnimator, true, 0);
            break;

        case VULTRON_TARGET:
            self->drawFX = FX_ROTATE;
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 1, &self->bodyAnimator, false, 0);
            RSDK.SetSpriteAnimation(Vultron->aniFrames, 2, &self->flameAnimator, false, 0);
            break;
    }

    Vultron_Draw();
}

void Vultron_EditorLoad(void)
{
    Vultron->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Vultron.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Vultron, type);
    RSDK_ENUM_VAR("Dive", VULTRON_DIVE);
    RSDK_ENUM_VAR("Target Player", VULTRON_TARGET);

    RSDK_ACTIVE_VAR(Vultron, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Vultron_Serialize(void)
{
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT16, dist);
    RSDK_EDITABLE_VAR(Vultron, VAR_UINT8, direction);
}
