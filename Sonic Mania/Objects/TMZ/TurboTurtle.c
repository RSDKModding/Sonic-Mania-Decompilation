#include "SonicMania.h"

ObjectTurboTurtle *TurboTurtle;

void TurboTurtle_Update(void)
{
    RSDK_THIS(TurboTurtle);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.animationID == 1) {
        if (!self->animator.frameID && self->animator.animationTimer == 1)
            RSDK.PlaySfx(TurboTurtle->sfxWalk, false, 255);
        if (self->animator.frameID == 5 && self->animator.animationTimer == 1)
            RSDK.PlaySfx(TurboTurtle->sfxWalk2, false, 255);
    }

    self->fanActiveTop   = false;
    self->leftFanActive  = false;
    self->rightFanActive = false;

    StateMachine_Run(self->state);

    TurboTurtle_CheckPlayerCollisions();
    if (self->state != TurboTurtle_State_Setup) {
        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->direction  = self->startDir;
            self->position.x = self->startPos.x;
            self->position.y = self->startPos.y;
            TurboTurtle_Create(NULL);
        }
    }
}

void TurboTurtle_LateUpdate(void) {}

void TurboTurtle_StaticUpdate(void) {}

void TurboTurtle_Draw(void)
{
    RSDK_THIS(TurboTurtle);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TurboTurtle_Create(void *data)
{
    RSDK_THIS(TurboTurtle);

    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    self->updateRange.x =
        abs(0x70000 * (self->stepCount * (2 * (self->initialSide == 0) - 1) + self->startPos.x - self->startPos.x)) + 0x800000;
    if (!self->leftFanDuration)
        self->leftFanDuration = 60;
    if (!self->rightFanDuration)
        self->rightFanDuration = 60;
    if (!self->leftFanLength)
        self->leftFanLength = 128;
    if (!self->rightFanLength)
        self->rightFanLength = 128;
    if (!self->leftFanStrength)
        self->leftFanStrength = 18;
    if (!self->rightFanStrength)
        self->rightFanStrength = 18;

    self->hitbox1.left   = -24;
    self->hitbox1.top    = -19;
    self->hitbox1.right  = 24;
    self->hitbox1.bottom = -19;

    self->hitbox2.left   = -19;
    self->hitbox2.top    = -24;
    self->hitbox2.right  = 19;
    self->hitbox2.bottom = 24;

    self->hitbox3.left   = -20;
    self->hitbox3.top    = -24;
    self->hitbox3.right  = -20;
    self->hitbox3.bottom = 24;
    self->state          = TurboTurtle_State_Setup;
}

void TurboTurtle_StageLoad(void)
{
    TurboTurtle->aniFrames     = RSDK.LoadSpriteAnimation("TMZ1/TurboTurtle.bin", SCOPE_STAGE);
    TurboTurtle->hitbox.left   = -13;
    TurboTurtle->hitbox.top    = -12;
    TurboTurtle->hitbox.right  = 13;
    TurboTurtle->hitbox.bottom = 16;

    DEBUGMODE_ADD_OBJ(TurboTurtle);

    TurboTurtle->sfxWalk  = RSDK.GetSFX("TMZ1/TurtleWalk.wav");
    TurboTurtle->sfxWalk2 = RSDK.GetSFX("TMZ1/TurtleWalk2.wav");

    Soundboard_LoadSFX("HCZ/SmallFan.wav", 37404, TurboTurtle_FanCheckCB, NULL);
}

void TurboTurtle_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityTurboTurtle *turboTurtle = CREATE_ENTITY(TurboTurtle, NULL, self->position.x, self->position.y);
    turboTurtle->direction         = self->direction;
    turboTurtle->startDir          = self->direction;
}

void TurboTurtle_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void TurboTurtle_CheckPlayerCollisions(void)
{
    RSDK_THIS(TurboTurtle);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &TurboTurtle->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void TurboTurtle_SetupState(void)
{
    RSDK_THIS(TurboTurtle);

    int dir = 0;
    if (self->currentSide)
        dir = self->rightFanDir;
    else
        dir = self->leftFanDir;
    self->timer = 0;

    switch (dir) {
        case 0:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 2, &self->animator, true, 0);
            self->state = TurboTurtle_State1_Unknown1;
            break;
        case 1:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 4, &self->animator, true, 0);
            self->direction = FLIP_NONE;
            self->state     = TurboTurtle_State2_Unknown1;
            break;
        case 2:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 4, &self->animator, true, 0);
            self->direction = FLIP_X;
            self->state     = TurboTurtle_State3_Unknown1;
            break;
    }
}

void TurboTurtle_HandleFans(void)
{
    RSDK_THIS(TurboTurtle);

    int storeDir      = self->direction;
    self->direction = FLIP_NONE;

    int strength = 0;
    if (self->currentSide)
        strength = self->rightFanStrength;
    else
        strength = self->leftFanStrength;

    int length = 0;
    if (self->currentSide)
        length = self->rightFanLength;
    else
        length = self->leftFanLength;
    if (self->fanActiveTop) {
        length += RSDK.Sin256(2 * Zone->timer) >> 5;
        self->hitbox1.top = self->hitbox1.bottom - length;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                    if (!player->onGround) {
                        player->velocity.y -= player->gravityStrength;
                        if (player->velocity.y > 0)
                            player->velocity.y -= (player->velocity.y >> 3);
                    }
                    int thisY  = self->position.y;
                    int bottom = thisY + (self->hitbox1.bottom << 16);
                    int top    = (self->hitbox1.top << 16) + thisY;
                    if (player->position.y > top)
                        top = player->position.y;
                    player->position.y -= ((strength * (((length << 16) - bottom + top != 0 ? (length << 16) - bottom + top : 0) / length)) >> 1);
                }
            }
        }
        TurboTurtle_HandleFanDebris(0, strength, length << 16);
    }

    if (self->leftFanActive) {
        self->hitbox3.left = self->hitbox3.right - length;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox3)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                    int left = (self->hitbox3.left << 16) + self->position.x;
                    if (player->position.x > left)
                        left = player->position.x;
                    int pos = (length << 16) - (self->position.x + (self->hitbox3.right << 16)) + left;
                    player->position.x -= (strength * ((pos & -(pos != 0)) / length)) >> 1;
                }
            }
        }
        TurboTurtle_HandleFanDebris(2, strength, length << 16);
    }

    if (self->rightFanActive) {
        self->hitbox2.right = length + self->hitbox2.left;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox3)) {
                int anim = player->playerAnimator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFTSWING) {
                    int right = (self->hitbox2.right << 16) + self->position.x;
                    if (player->position.x < right)
                        right = player->position.x;
                    int pos = self->position.x + (self->hitbox2.left << 16);
                    player->position.x += (strength * (((length << 16) - right + (pos != 0) ? (length << 16) - right + pos : 0) / length)) >> 1;
                }
            }
        }
        TurboTurtle_HandleFanDebris(1, strength, length << 16);
    }

    self->direction = storeDir;
}

void TurboTurtle_HandleFanDebris(uint8 type, int strength, int length)
{
    RSDK_THIS(TurboTurtle);

    if (!(Zone->timer % 3)) {
        for (int i = 0; i < RSDK.Rand(1, 2); ++i) {
            int timer = 0;
            int anim  = (RSDK.Rand(0, 10) > 7) + 6;
            int frame = RSDK.Rand(0, 3);

            int spawnX = 0, spawnY = 0;
            int velX = 0, velY = 0;
            switch (type) {
                case 0:
                    spawnY = self->position.y + (self->hitbox1.bottom << 16);
                    spawnX = (RSDK.Rand(-13, 13) << 16) + self->position.x;
                    velX   = (spawnX - self->position.x) >> 7;
                    RSDK.Rand(0, 4);
                    velY  = -0x8000 * strength;
                    timer = length / abs(-0x8000 * strength);
                    break;
                case 1:
                    spawnX = self->position.x + (self->hitbox2.left << 16);
                    spawnY = (RSDK.Rand(-13, 13) << 16) + self->position.y;
                    RSDK.Rand(0, 4);
                    velX  = strength << 15;
                    velY  = (spawnY - self->position.y) >> 7;
                    timer = length / abs(strength << 15);
                    break;
                case 2:
                    spawnX = self->position.x - (self->hitbox2.left << 16);
                    spawnY = (RSDK.Rand(-13, 13) << 16) + self->position.y;
                    RSDK.Rand(0, 4);
                    velX  = -0x8000 * strength;
                    velY  = (spawnY - self->position.y) >> 7;
                    timer = length / abs(-0x8000 * strength);
                    break;
                default: break;
            }

            timer += RSDK.Rand(-5, 5);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, spawnX, spawnY);
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, anim, &debris->animator, true, frame);
            debris->velocity.x = velX;
            debris->velocity.y = velY;
            debris->drawOrder  = Zone->drawOrderLow - 1;
            debris->timer      = timer;
        }
    }
}

bool32 TurboTurtle_FanCheckCB(void)
{
    int count = 0;
    foreach_all(TurboTurtle, turtle)
    {
        if (turtle->state == TurboTurtle_State1_Unknown1 || turtle->state == TurboTurtle_State2_Unknown2
            || turtle->state == TurboTurtle_State3_Unknown2)
            ++count;
    }

    return count > 0;
}

void TurboTurtle_State_Setup(void)
{
    RSDK_THIS(TurboTurtle);

    self->active = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &self->animator, true, 0);
    self->direction   = self->startDir;
    self->position    = self->startPos;
    self->currentSide = self->initialSide;
    TurboTurtle_SetupState();
}

void TurboTurtle_State_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);
    int offsets[] = { 0, 0, 0, 0, 1, 1, 0, 0 };

    self->position.x += (2 * (self->currentSide == 1) - 1) * (offsets[self->animator.frameID] << 16);
    if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 2)) {
        self->stepTimer = 0;
        TurboTurtle_SetupState();
    }
    else if (self->animator.frameID == self->animator.frameCount - 1) {
        if (self->animator.animationTimer == self->animator.frameDelay) {
            if (self->stepTimer < self->stepCount) {
                self->stepTimer++;
            }
            else {
                self->stepTimer = 0;
                TurboTurtle_SetupState();
            }
        }
    }
}

void TurboTurtle_State_Unknown2(void)
{
    RSDK_THIS(TurboTurtle);

    if (self->animator.animationID == 5) {
        if (self->animator.frameID == self->animator.frameCount - 1) {
            uint8 dir           = self->currentSide != 0;
            self->currentSide = !self->currentSide;
            self->direction   = dir;
            self->stepTimer   = 0;
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 1, &self->animator, true, 0);
            self->state = TurboTurtle_State_Unknown1;
        }
    }
}

void TurboTurtle_State1_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);

    int duration = 0;
    if (self->currentSide)
        duration = self->rightFanDuration;
    else
        duration = self->leftFanDuration;
    if (self->stationary || self->timer < duration) {
        self->fanActiveTop = true;
        TurboTurtle_HandleFans();
        if (!self->stationary)
            ++self->timer;
    }
    else {
        uint8 dir           = self->currentSide != 0;
        self->currentSide = !self->currentSide;
        self->direction   = dir;
        self->stepTimer   = 0;
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 1, &self->animator, true, 0);
        self->state = TurboTurtle_State_Unknown1;
    }
}

void TurboTurtle_State2_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);

    if (self->animator.animationID == 4) {
        if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 3, &self->animator, true, 0);
            self->state = TurboTurtle_State2_Unknown2;
        }
    }
}

void TurboTurtle_State2_Unknown2(void)
{
    RSDK_THIS(TurboTurtle);

    int duration = 0;
    if (self->currentSide)
        duration = self->rightFanDuration;
    else
        duration = self->leftFanDuration;
    if (self->stationary || self->timer < duration) {
        self->rightFanActive = true;
        TurboTurtle_HandleFans();
        if (!self->stationary)
            ++self->timer;
    }
    else {
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 5, &self->animator, true, 0);
        self->state = TurboTurtle_State_Unknown2;
    }
}

void TurboTurtle_State3_Unknown1(void)
{
    RSDK_THIS(TurboTurtle);

    if (self->animator.animationID == 4) {
        if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 3, &self->animator, true, 0);
            self->state = TurboTurtle_State3_Unknown2;
        }
    }
}

void TurboTurtle_State3_Unknown2(void)
{
    RSDK_THIS(TurboTurtle);

    int duration = 0;
    if (self->currentSide)
        duration = self->rightFanDuration;
    else
        duration = self->leftFanDuration;
    if (self->stationary || self->timer < duration) {
        self->leftFanActive = true;
        TurboTurtle_HandleFans();
        if (!self->stationary)
            ++self->timer;
    }
    else {
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 5, &self->animator, true, 0);
        self->state = TurboTurtle_State_Unknown2;
    }
}

#if RETRO_INCLUDE_EDITOR
void TurboTurtle_EditorDraw(void)
{
    RSDK_THIS(TurboTurtle);

    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->startDir      = self->direction;

    self->updateRange.x =
        abs(0x70000 * (self->stepCount * (2 * (self->initialSide == 0) - 1) + self->startPos.x - self->startPos.x)) + 0x800000;

    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &self->animator, true, 0);
    self->direction   = self->startDir;

    TurboTurtle_Draw();
}

void TurboTurtle_EditorLoad(void) { TurboTurtle->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TurboTurtle.bin", SCOPE_STAGE); }
#endif

void TurboTurtle_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT16, stepCount);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, initialSide);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_BOOL, stationary);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, leftFanDir);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanDuration);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanStrength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanLength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, rightFanDir);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanDuration);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanStrength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanLength);
}
