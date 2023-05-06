// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LaunchSpring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLaunchSpring *LaunchSpring;

void LaunchSpring_Update(void)
{
    RSDK_THIS(LaunchSpring);

    StateMachine_Run(self->state);
}

void LaunchSpring_LateUpdate(void) {}

void LaunchSpring_StaticUpdate(void) {}

void LaunchSpring_Draw(void)
{
    RSDK_THIS(LaunchSpring);

    RSDK.GetFrame(LaunchSpring->aniFrames, 2, 0)->pivotY = -8 - self->springPivot;
    RSDK.DrawSprite(&self->springAnimator, NULL, false);

    self->mainAnimator.frameID = 0;
    RSDK.DrawSprite(&self->jointAnimator, NULL, false);

    SpriteFrame *frame = RSDK.GetFrame(LaunchSpring->aniFrames, 0, 0);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    frame->pivotY   = self->timer - 55;
    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    frame->pivotY   = -55;
    self->direction = FLIP_NONE;
}

void LaunchSpring_Create(void *data)
{
    RSDK_THIS(LaunchSpring);

    if (!self->rotSpeed)
        self->rotSpeed = 4;

    self->drawFX   = FX_ROTATE | FX_FLIP;
    self->rotation = self->angle << 6;

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2");
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 1, &self->jointAnimator, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->springAnimator, true, 0);
        self->drawGroup = Zone->objectDrawGroup[1];

        switch (self->type) {
            case LAUNCHSPRING_SPINNING:
                self->timer = 47;
                self->state = LaunchSpring_State_Spinning;
                break;

            case LAUNCHSPRING_CANNON:
                self->state                  = LaunchSpring_State_Cannon;
                self->springAnimator.frameID = 1;
                break;

            case LAUNCHSPRING_ROTATE_45DEG:
                self->minAngle               = self->rotation - 0x40;
                self->maxAngle               = self->rotation + 0x40;
                self->state                  = LaunchSpring_State_Rotating;
                self->springAnimator.frameID = 1;
                break;

            case LAUNCHSPRING_ROTATE_90DEG:
                self->minAngle               = self->rotation - 0x80;
                self->maxAngle               = self->rotation + 0x80;
                self->state                  = LaunchSpring_State_Rotating;
                self->springAnimator.frameID = 1;
                break;

            case LAUNCHSPRING_ROTATE_135DEG:
                self->minAngle               = self->rotation - 0xC0;
                self->maxAngle               = self->rotation + 0xC0;
                self->state                  = LaunchSpring_State_Rotating;
                self->springAnimator.frameID = 1;
                break;

            case LAUNCHSPRING_ROTATE_180DEG:
                self->minAngle               = self->rotation - 0x100;
                self->maxAngle               = self->rotation + 0x100;
                self->state                  = LaunchSpring_State_Rotating;
                self->springAnimator.frameID = 1;
                break;

            default: break;
        }

        // What's this doin here...? Isn't this supposed to be StageLoad code?

        LaunchSpring->sfxGrab = RSDK.GetSfx("Global/Grab.wav");

        if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2")) {
            LaunchSpring->isTMZ           = false;
            LaunchSpring->sfxClack        = RSDK.GetSfx("Stage/Clack.wav");
            LaunchSpring->sfxSpeedBooster = RSDK.GetSfx("Stage/SpeedBooster.wav");
        }
        else if (RSDK.CheckSceneFolder("TMZ1") || RSDK.CheckSceneFolder("TMZ2")) {
            LaunchSpring->isTMZ           = true;
            LaunchSpring->sfxSpeedBooster = RSDK.GetSfx("Stage/CannonFire.wav");
        }
    }
}

void LaunchSpring_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/LaunchSpring.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/LaunchSpring.bin", SCOPE_STAGE);

    LaunchSpring->hitbox.top    = -24;
    LaunchSpring->hitbox.left   = -24;
    LaunchSpring->hitbox.right  = 24;
    LaunchSpring->hitbox.bottom = 24;
}

bool32 LaunchSpring_CheckFireworkActive(EntityPlayer *player)
{
    if (!Firework)
        return false;

    int32 playerID        = RSDK.GetEntitySlot(player);
    bool32 fireworkActive = false;

    foreach_active(Firework, firework)
    {
        if ((1 << playerID) & firework->activePlayers) {
            fireworkActive = true;
            foreach_break;
        }
    }

    return fireworkActive;
}

void LaunchSpring_CheckPlayerCollisions(void *nextState)
{
    RSDK_THIS(LaunchSpring);

    foreach_active(Player, player)
    {
        if (!player->sidekick && !LaunchSpring_CheckFireworkActive(player)) {
            if (Player_CheckCollisionTouch(player, self, &LaunchSpring->hitbox)) {
                RSDK.PlaySfx(LaunchSpring->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

                player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
                player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);

                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->onGround        = false;
                player->state           = Player_State_Static;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;

                self->springAnimator.frameID = 1;
                self->playerPtr              = player;
                self->state                  = nextState;
                foreach_break;
            }
        }
    }
}

void LaunchSpring_LaunchPlayer(void)
{
    RSDK_THIS(LaunchSpring);

    EntityPlayer *player = self->playerPtr;

    if (player->state == Player_State_Static) {
        RSDK.PlaySfx(LaunchSpring->sfxSpeedBooster, false, 0xFF);

        if (self->strict) {
            self->rotation = (self->rotation + 0x40) & 0x1C0;

            player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
            player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);
        }

        player->applyJumpCap     = false;
        player->jumpAbilityState = 0;
        player->velocity.x       = RSDK.Cos512(self->rotation - 0x80) << 11;
        player->velocity.y       = RSDK.Sin512(self->rotation - 0x80) << 11;
        player->state            = Player_State_Air;
    }
}

void LaunchSpring_State_Spinning(void)
{
    RSDK_THIS(LaunchSpring);

    self->rotation = (self->rotation - self->rotSpeed) & 0x1FF;

    LaunchSpring_CheckPlayerCollisions(LaunchSpring_State_Spinning_GrabbedPlayer);
}

void LaunchSpring_State_Spinning_GrabbedPlayer(void)
{
    RSDK_THIS(LaunchSpring);

    EntityPlayer *player = self->playerPtr;

    player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
    player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);

    if (!--self->timer) {
        RSDK.PlaySfx(LaunchSpring->sfxClack, false, 0xFF);
        self->state = LaunchSpring_State_Spinning_ReadyToFire;
    }
}

void LaunchSpring_State_Spinning_ReadyToFire(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = self->playerPtr;

    if (player->state == Player_State_Static) {
        self->rotation = (self->rotation - self->rotSpeed) & 0x1FF;

        player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
        player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);

        if (player->jumpPress) {
            LaunchSpring_LaunchPlayer();
            self->state = LaunchSpring_State_Spinning_FiredPlayer;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_State_Spinning_FiredPlayer;
    }
}

void LaunchSpring_State_Spinning_FiredPlayer(void)
{
    RSDK_THIS(LaunchSpring);

    RSDK.ProcessAnimation(&self->springAnimator);

    if (self->springAnimator.frameID == self->springAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->springAnimator, true, 0);
        self->state = LaunchSpring_State_Spinning_ReadyUp;
    }
}

void LaunchSpring_State_Spinning_ReadyUp(void)
{
    RSDK_THIS(LaunchSpring);

    self->timer += 2;
    if (self->timer >= 47) {
        self->timer = 47;
        self->state = LaunchSpring_State_Spinning;
    }
}

void LaunchSpring_State_Cannon(void)
{
    RSDK_THIS(LaunchSpring);

    if (self->manual)
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_State_Cannon_Fire_Manual);
    else
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_State_Cannon_Fire_Automatic);
}

void LaunchSpring_State_Cannon_Fire_Automatic(void)
{
    RSDK_THIS(LaunchSpring);

    EntityPlayer *player = self->playerPtr;

    if (player->state == Player_State_Static) {
        player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
        player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);

        if (++self->autoFireTimer == 60) {
            self->autoFireTimer = 0;
            LaunchSpring_LaunchPlayer();
            self->state = LaunchSpring_State_Cannon_FiredPlayer;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_State_Cannon_FiredPlayer;
    }
}

void LaunchSpring_State_Cannon_Fire_Manual(void)
{
    RSDK_THIS(LaunchSpring);

    EntityPlayer *player = self->playerPtr;

    if (player->state == Player_State_Static) {
        player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
        player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);

        if (player->jumpPress) {
            LaunchSpring_LaunchPlayer();
            self->autoFireTimer = 0;
            self->state         = LaunchSpring_State_Cannon_FiredPlayer;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_State_Cannon_FiredPlayer;
    }
}

void LaunchSpring_State_Cannon_FiredPlayer(void)
{
    RSDK_THIS(LaunchSpring);

    if (LaunchSpring->isTMZ) {
        if (self->autoFireTimer >= 30)
            self->state = LaunchSpring_State_Cannon;
        else
            self->autoFireTimer++;
    }
    else {
        RSDK.ProcessAnimation(&self->springAnimator);

        if (self->springAnimator.frameID == self->springAnimator.frameCount - 1) {
            RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->springAnimator, true, 0);
            self->springAnimator.frameID = 1;
            self->state                  = LaunchSpring_State_Cannon;
        }
    }
}

void LaunchSpring_State_Rotating(void)
{
    RSDK_THIS(LaunchSpring);

    if (self->rotDir) {
        self->rotation -= self->rotSpeed;

        if (self->rotation <= self->minAngle) {
            self->rotDir   = 0;
            self->rotation = self->minAngle;
        }
    }
    else {
        self->rotation += self->rotSpeed;

        if (self->rotation >= self->maxAngle) {
            self->rotDir   = 1;
            self->rotation = self->maxAngle;
        }
    }

    if (self->manual)
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_State_Rotate_Fire_Manual);
    else
        LaunchSpring_CheckPlayerCollisions(LaunchSpring_State_Rotate_Fire_Automatic);
}

void LaunchSpring_State_Rotate_Fire_Automatic(void)
{
    RSDK_THIS(LaunchSpring);

    EntityPlayer *player = self->playerPtr;

    if (player->state == Player_State_Static) {
        if (self->strict)
            self->rotation = (self->rotation + 0x20) & 0xC0;

        player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
        player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);

        if (++self->autoFireTimer == 60) {
            self->autoFireTimer = 0;
            LaunchSpring_LaunchPlayer();
            self->state = LaunchSpring_State_Rotating_FiredPlayer;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_State_Rotating_FiredPlayer;
    }
}

void LaunchSpring_State_Rotate_Fire_Manual(void)
{
    RSDK_THIS(LaunchSpring);
    EntityPlayer *player = self->playerPtr;

    if (player->state == Player_State_Static) {
        if (self->rotDir) {
            self->rotation -= self->rotSpeed;

            if (self->rotation <= self->minAngle) {
                self->rotation = self->minAngle;
                self->rotDir   = FLIP_NONE;
            }
        }
        else {
            self->rotation += self->rotSpeed;

            if (self->rotation >= self->maxAngle) {
                self->rotation = self->maxAngle;
                self->rotDir   = FLIP_X;
            }
        }

        player->position.x = self->position.x + (RSDK.Cos512(self->rotation - 0x80) << 12);
        player->position.y = self->position.y + (RSDK.Sin512(self->rotation - 0x80) << 12);

        if (player->jumpPress) {
            LaunchSpring_LaunchPlayer();
            self->state = LaunchSpring_State_Rotating_FiredPlayer;
        }
    }
    else {
        self->playerPtr = NULL;
        self->state     = LaunchSpring_State_Rotating_FiredPlayer;
    }
}

void LaunchSpring_State_Rotating_FiredPlayer(void)
{
    RSDK_THIS(LaunchSpring);

    RSDK.ProcessAnimation(&self->springAnimator);

    if (self->springAnimator.frameID == self->springAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->springAnimator, true, 0);
        self->springAnimator.frameID = 1;
        self->state                  = LaunchSpring_State_Rotating;
    }
}

#if GAME_INCLUDE_EDITOR
void LaunchSpring_EditorDraw(void)
{
    RSDK_THIS(LaunchSpring);

    if (RSDK.CheckSceneFolder("SSZ1") || RSDK.CheckSceneFolder("SSZ2")) {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 1, &self->jointAnimator, true, 0);
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 2, &self->springAnimator, true, 0);

        self->rotation = self->angle << 6;

        LaunchSpring_Draw();
    }
    else {
        RSDK.SetSpriteAnimation(LaunchSpring->aniFrames, 0, &self->mainAnimator, true, 4);

        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
}

void LaunchSpring_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/LaunchSpring.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SSZ2"))
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/LaunchSpring.bin", SCOPE_STAGE);
    else
        LaunchSpring->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LaunchSpring, type);
    RSDK_ENUM_VAR("Spinning", LAUNCHSPRING_SPINNING);
    RSDK_ENUM_VAR("Cannon", LAUNCHSPRING_CANNON);
    RSDK_ENUM_VAR("Rotating (45 Degrees)", LAUNCHSPRING_ROTATE_45DEG);
    RSDK_ENUM_VAR("Rotating (90 Degrees)", LAUNCHSPRING_ROTATE_90DEG);
    RSDK_ENUM_VAR("Rotating (135 Degrees)", LAUNCHSPRING_ROTATE_135DEG);
    RSDK_ENUM_VAR("Rotating (180 Degrees)", LAUNCHSPRING_ROTATE_180DEG);

    RSDK_ACTIVE_VAR(LaunchSpring, rotDir);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);

    RSDK_ACTIVE_VAR(LaunchSpring, angle);
    RSDK_ENUM_VAR("Up", 0);
    RSDK_ENUM_VAR("Left", 1);
    RSDK_ENUM_VAR("Down", 2);
    RSDK_ENUM_VAR("Right", 3);
}
#endif

void LaunchSpring_Serialize(void)
{
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, rotDir);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_UINT8, rotSpeed);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_BOOL, manual);
    RSDK_EDITABLE_VAR(LaunchSpring, VAR_BOOL, strict);
}
