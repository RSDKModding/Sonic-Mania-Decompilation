// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Shutterbug Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectShutterbug *Shutterbug;

// cleanup note: look in Unused/Wisp

void Shutterbug_Update(void)
{
    RSDK_THIS(Shutterbug);
    StateMachine_Run(self->state);
}

void Shutterbug_LateUpdate(void) {}

void Shutterbug_StaticUpdate(void) {}

void Shutterbug_Draw(void)
{
    RSDK_THIS(Shutterbug);
    RSDK.DrawSprite(&self->animator, NULL, false);
    if (self->flickerTimer & 2) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0x100;
        RSDK.DrawSprite(&self->overlayAnimator, NULL, false);
        self->inkEffect = INK_NONE;
    }
}

void Shutterbug_Create(void *data)
{
    RSDK_THIS(Shutterbug);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderHigh;

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (!self->range.x) {
            self->range.x = 0x7FFF0000;
            self->range.y = 0x7FFF0000;
        }
        self->drawFX |= FX_FLIP;
        self->hitboxRange.left   = (-self->range.x) >> 16;
        self->hitboxRange.top    = (-self->range.y) >> 16;
        self->hitboxRange.bottom = self->range.y >> 16;
        self->hitboxRange.right  = self->range.x >> 16;
        self->direction          = FLIP_X;
        self->moveDir            = FLIP_X;
        if (!self->snaps)
            self->numSnaps = 0xFF;
        else
            self->numSnaps = self->snaps;
        self->focusTarget = NULL;
        self->alpha       = 0xC0;
        self->snapTimer   = 16;

        RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 0, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 1, &self->overlayAnimator, true, 0);
        self->state = Shutterbug_State_Setup;
    }
}

void Shutterbug_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Shutterbug.bin", SCOPE_STAGE);
    else
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Shutterbug.bin", SCOPE_STAGE);

    Shutterbug->hitboxBadnik.left   = -15;
    Shutterbug->hitboxBadnik.top    = -12;
    Shutterbug->hitboxBadnik.right  = 15;
    Shutterbug->hitboxBadnik.bottom = 12;

    Shutterbug->hitboxAchievement.left   = -160;
    Shutterbug->hitboxAchievement.top    = -160;
    Shutterbug->hitboxAchievement.right  = 160;
    Shutterbug->hitboxAchievement.bottom = 160;

    Shutterbug->pictureCount = 0;

    DEBUGMODE_ADD_OBJ(Shutterbug);

    Shutterbug->snapSfx = RSDK.GetSfx("SPZ/ShBugSnap.wav");
}

void Shutterbug_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Shutterbug, NULL, self->position.x, self->position.y);
}

void Shutterbug_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Shutterbug_CheckOnScreen(void)
{
    RSDK_THIS(Shutterbug);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->spawnPos, &self->updateRange)) {
        self->position.x = self->spawnPos.x;
        self->position.y = self->spawnPos.y;
        Shutterbug_Create(NULL);
    }
}

void Shutterbug_State_Setup(void)
{
    RSDK_THIS(Shutterbug);

    self->active      = ACTIVE_NORMAL;
    self->velocity.x  = 0;
    self->velocity.y  = 0;
    self->offset.x    = 0;
    self->focusTarget = NULL;
    self->turnTimer   = 0;
    self->state       = Shutterbug_State_FlyAround;
    Shutterbug_State_FlyAround();
}

void Shutterbug_State_FlyAround(void)
{
    RSDK_THIS(Shutterbug);

    if (self->snapTimer == 20 && self->focusTarget) {
        self->flickerTimer = 8;
        RSDK.PlaySfx(Shutterbug->snapSfx, false, 255);
        Shutterbug_TakePicture();
    }

    if (!self->snapTimer--) {
        if (self->focusTarget)
            --self->numSnaps;

        if (self->numSnaps < 0) {
            if (self->focusTarget && self->position.x >= self->focusTarget->position.x)
                self->velocity.x = 0x20000;
            else
                self->velocity.x = -0x20000;
            self->velocity.y = -0x20000;
            self->state      = Shutterbug_State_FlyAway;
        }
        else {
            self->offset.x = 0;
            self->offset.y = 0;
            if (self->focusTarget)
                self->velocity.y = -0x10000;
            self->snapTimer = 96;
            self->state     = Shutterbug_State_ShakeFly;
        }
    }

    Shutterbug_HandleBodyAnim();
    Shutterbug_CheckFocus();
    Shutterbug_CheckOnScreen();
}

void Shutterbug_State_ShakeFly()
{
    RSDK_THIS(Shutterbug);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->focusTarget) {
        self->offset.x = self->focusTarget->position.x;
        self->offset.y = self->focusTarget->position.y;
    }
    else if (!self->offset.x) {
        self->offset.x = 0;
        self->offset.y = 0;

        while (abs(self->offset.x) < 4) self->offset.x = RSDK.Rand(-16, 17);
        while (abs(self->offset.y) < 4) self->offset.y = RSDK.Rand(-16, 17);
        self->offset.x = self->position.x + (self->offset.x << 17);
        self->offset.y = self->position.y + (self->offset.y << 17);
    }

    if (self->offset.x >= self->position.x) {
        self->velocity.x += 0x1000;
        if (self->velocity.x > 0x20000)
            self->velocity.x = 0x20000;
        self->moveDir = FLIP_NONE;
    }
    else {
        self->velocity.x -= 0x1000;
        if (self->velocity.x < -0x20000)
            self->velocity.x = -0x20000;
        self->moveDir = FLIP_X;
    }

    if (self->offset.y >= self->position.y) {
        self->velocity.y += 0x1000;
        if (self->velocity.y > 0x20000)
            self->velocity.y = 0x20000;
    }
    else {
        self->velocity.y -= 0x1000;
        if (self->velocity.y < -0x20000)
            self->velocity.y = -0x20000;
    }

    if (!self->snapTimer--) {
        self->state      = Shutterbug_State_FlyAround;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->snapTimer  = RSDK.Rand(0, 32) + 20;
    }

    if (!self->passThrough) {
        if (abs(self->velocity.x) <= abs(self->velocity.y)) {
            if (!Shutterbug_BounceY())
                Shutterbug_BounceX();
        }
        else if (!Shutterbug_BounceX()) {
            Shutterbug_BounceY();
        }
    }

    Shutterbug_HandleBodyAnim();
    Shutterbug_CheckFocus();
    Shutterbug_CheckOnScreen();
}

void Shutterbug_State_FlyAway(void)
{
    RSDK_THIS(Shutterbug);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    Shutterbug_HandleBodyAnim();
    Shutterbug_CheckFocus();
    Shutterbug_CheckOnScreen();
}

int Shutterbug_BounceX(void)
{
    RSDK_THIS(Shutterbug);

    // hi rmg here you may be wondering why the hell CMODE_FLOOR is used
    // oh lemme tell ya buddy i have no fuckin idea

    int32 x         = (self->velocity.x <= 0) ? -0x90000 : 0x90000;
    bool32 collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, x, 0x80000, false)
                      || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, x, -0x80000, false);
    if (collided)
        self->velocity.x = -self->velocity.x;
    return collided;
}
int Shutterbug_BounceY(void)
{
    RSDK_THIS(Shutterbug);

    // makes sense here though!
    int32 y         = (self->velocity.y <= 0) ? -0x90000 : 0x90000;
    bool32 collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x80000, y, false)
                      || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x80000, y, false);
    if (collided)
        self->velocity.y = -self->velocity.y;
    return collided;
}

void Shutterbug_CheckFocus(void)
{
    RSDK_THIS(Shutterbug);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
            if (!self->focusTarget) {
                self->focusTarget = player;
            }
            else {
                // pick closest
                if (abs(self->position.x - player->position.x) < abs(self->position.x - self->focusTarget->position.x))
                    self->focusTarget = player;
            }

            if (Player_CheckBadnikTouch(player, self, &Shutterbug->hitboxBadnik))
                Player_CheckBadnikBreak(self, player, true);
        }
    }
}

void Shutterbug_TakePicture(void)
{
    RSDK_THIS(Shutterbug);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Shutterbug->hitboxAchievement) && ++Shutterbug->pictureCount == 10)
            API_UnlockAchievement("ACH_SPZ");
    }
}

void Shutterbug_HandleBodyAnim(void)
{
    RSDK_THIS(Shutterbug);

    ++self->animator.timer;
    self->animator.timer &= 3;

    if (!self->flickerTimer || self->turnTimer) {
        if (self->direction == self->moveDir) {
            if (self->turnTimer)
                self->turnTimer--;
        }
        else if (++self->turnTimer >= 9)
            self->direction = self->moveDir;
        self->animator.frameID = (self->animator.timer >> 1) + (self->turnTimer & ~1);
    }
    else {
        self->animator.frameID = (self->animator.timer >> 1) + (((28 - self->flickerTimer--) >> 1) & ~1);
    }
}

#if RETRO_INCLUDE_EDITOR
void Shutterbug_EditorDraw(void)
{
    RSDK_THIS(Shutterbug);
    RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 0, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(Shutterbug->aniFrames, 1, &self->overlayAnimator, true, 0);
    self->drawFX = FX_FLIP;

    Shutterbug_Draw();
}

void Shutterbug_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Shutterbug.bin", SCOPE_STAGE);
    else
        Shutterbug->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Shutterbug.bin", SCOPE_STAGE);
}
#endif

void Shutterbug_Serialize(void)
{
    RSDK_EDITABLE_VAR(Shutterbug, VAR_UINT8, snaps);
    RSDK_EDITABLE_VAR(Shutterbug, VAR_BOOL, passThrough);
    RSDK_EDITABLE_VAR(Shutterbug, VAR_VECTOR2, range);
}