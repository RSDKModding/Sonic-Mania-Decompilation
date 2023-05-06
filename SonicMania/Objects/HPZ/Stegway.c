// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Stegway Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectStegway *Stegway = NULL;

void Stegway_Update(void)
{
    RSDK_THIS(Stegway);
    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->wheelAnimator);
    if (self->showJet)
        RSDK.ProcessAnimation(&self->jetAnimator);

    StateMachine_Run(self->state);

    Stegway_HandlePlayerInteractions();
    if (self->state != Stegway_State_Init)
        Stegway_CheckOffScreen();
}

void Stegway_LateUpdate(void) {}

void Stegway_StaticUpdate(void) {}

void Stegway_Draw(void)
{
    RSDK_THIS(Stegway);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->wheelAnimator, NULL, false);
    if (self->showJet)
        RSDK.DrawSprite(&self->jetAnimator, NULL, false);
}

void Stegway_Create(void *data)
{
    RSDK_THIS(Stegway);
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Stegway_State_Init;
}

void Stegway_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("HPZ"))
        Stegway->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Stegway.bin", SCOPE_STAGE);

    Stegway->hitboxBadnik.left   = -20;
    Stegway->hitboxBadnik.top    = -14;
    Stegway->hitboxBadnik.right  = 20;
    Stegway->hitboxBadnik.bottom = 14;

    Stegway->hitboxRange.left   = -96;
    Stegway->hitboxRange.top    = -32;
    Stegway->hitboxRange.right  = 0;
    Stegway->hitboxRange.bottom = 14;

    Stegway->sfxRev     = RSDK.GetSfx("Stage/Rev.wav");
    Stegway->sfxRelease = RSDK.GetSfx("Global/Release.wav");

    DEBUGMODE_ADD_OBJ(Stegway);
}

void Stegway_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityStegway *stegway = CREATE_ENTITY(Stegway, NULL, self->position.x, self->position.y);
    stegway->direction     = self->direction;
    stegway->startDir      = self->direction;
}

void Stegway_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Stegway_CheckOffScreen(void)
{
    RSDK_THIS(Stegway);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->direction = self->startDir;
        self->position  = self->startPos;
        Stegway_Create(NULL);
    }
}

void Stegway_HandlePlayerInteractions(void)
{
    RSDK_THIS(Stegway);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Stegway->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Stegway_SetupAnims(char type, bool32 force)
{
    RSDK_THIS(Stegway);
    switch (type) {
        case 0:
            if (force || self->mainAnimator.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &self->mainAnimator, true, 0);
            self->mainAnimator.speed  = 1;
            self->wheelAnimator.speed = 6;
            break;

        case 1:
            if (force || self->mainAnimator.animationID != 1)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 1, &self->mainAnimator, true, 0);
            self->wheelAnimator.speed = 16;
            break;

        case 2:
            if (force || self->mainAnimator.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &self->mainAnimator, true, 0);
            if (self->mainAnimator.frameID == 2)
                self->mainAnimator.frameID = 1;
            self->mainAnimator.speed  = 0;
            self->wheelAnimator.speed = 24;
            break;

        case 3:
            if (force || self->mainAnimator.animationID)
                RSDK.SetSpriteAnimation(Stegway->aniFrames, 0, &self->mainAnimator, true, 0);
            if (self->mainAnimator.frameID == 2)
                self->mainAnimator.frameID = 1;
            self->mainAnimator.speed  = 0;
            self->wheelAnimator.speed = 0;
            break;
        default: break;
    }
}

void Stegway_State_Init(void)
{
    RSDK_THIS(Stegway);

    self->active = ACTIVE_NORMAL;
    if (self->direction == FLIP_NONE)
        self->velocity.x = -0x4000;
    else
        self->velocity.x = 0x4000;
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 2, &self->wheelAnimator, true, 0);
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 3, &self->jetAnimator, true, 0);
    Stegway_SetupAnims(0, true);
    self->state = Stegway_State_Moving;
    Stegway_State_Moving();
}

void Stegway_State_Moving(void)
{
    RSDK_THIS(Stegway);

    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Stegway->hitboxRange)) {
            self->state = Stegway_State_RevUp;
            Stegway_SetupAnims(3, false);
        }
    }

    bool32 collided = false;
    if (self->direction)
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0xC0000, 0x100000, 8);
    else
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0xC0000, 0x100000, 8);

    if (!collided) {
        self->state = Stegway_State_Turn;
        Stegway_SetupAnims(3, false);
        self->showJet = false;
        self->noFloor = false;
    }
    Stegway_CheckOffScreen();
}

void Stegway_State_Turn(void)
{
    RSDK_THIS(Stegway);

    if (self->timer >= 29) {
        self->timer = 0;
        self->state = Stegway_State_Moving;
        Stegway_SetupAnims(0, false);

        self->direction ^= FLIP_X;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
    }
    else {
        self->timer++;
    }
}

void Stegway_State_RevUp(void)
{
    RSDK_THIS(Stegway);
    if (++self->timer == 8) {
        self->timer = 0;
        self->state = Stegway_State_RevRelease;
        Stegway_SetupAnims(2, false);
        RSDK.PlaySfx(Stegway->sfxRev, false, 255);
    }
}

void Stegway_State_RevRelease(void)
{
    RSDK_THIS(Stegway);
    if (++self->timer == 32) {
        self->timer = 0;
        self->state = Stegway_State_Dash;
        EntityDust *dust =
            CREATE_ENTITY(Dust, self, self->position.x - 0xA0000 * (2 * (self->direction != FLIP_NONE) - 1), self->position.y + 0x100000);
        RSDK.SetSpriteAnimation(Dust->aniFrames, 2, &dust->animator, true, 0);
        dust->state     = Dust_State_DustPuff;
        dust->direction = FLIP_X - self->direction;
        dust->drawGroup = self->drawGroup;
        Stegway_SetupAnims(1, false);
        self->showJet = true;
        self->velocity.x *= 12;
        RSDK.PlaySfx(Stegway->sfxRelease, false, 255);
    }
}

void Stegway_State_Dash(void)
{
    RSDK_THIS(Stegway);

    self->position.x += self->velocity.x;
    int32 dir = 2 * (self->direction != FLIP_NONE) - 1;

    if (!self->noFloor) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;
        if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, dir << 22, 0x100000, 8))
            self->noFloor = true;
        self->position.x = storeX;
        self->position.y = storeY;
    }

    bool32 collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0xC0000 * dir, 0x100000, 8);

    if (self->noFloor) {
        if (self->velocity.x * dir >= 0x4000) {
            self->velocity.x -= 0xA00 * dir;
            if (dir * self->velocity.x < 0x4000) {
                self->velocity.x = dir << 14;
                self->state      = Stegway_State_Moving;
                Stegway_SetupAnims(0, false);
                self->mainAnimator.frameID = 0;
                self->showJet              = false;
                self->noFloor              = false;
            }
        }
    }

    if (!collided) {
        self->state = Stegway_State_Turn;
        Stegway_SetupAnims(3, false);
        self->showJet = false;
        self->noFloor = false;
    }
}

#if GAME_INCLUDE_EDITOR
void Stegway_EditorDraw(void)
{
    RSDK_THIS(Stegway);
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 2, &self->wheelAnimator, true, 0);
    RSDK.SetSpriteAnimation(Stegway->aniFrames, 3, &self->jetAnimator, true, 0);
    Stegway_SetupAnims(0, true);

    Stegway_Draw();
}

void Stegway_EditorLoad(void)
{
    Stegway->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Stegway.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Stegway, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Stegway_Serialize(void) { RSDK_EDITABLE_VAR(Stegway, VAR_UINT8, direction); }
