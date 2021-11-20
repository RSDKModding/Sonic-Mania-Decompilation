#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectRhinobot *Rhinobot;

void Rhinobot_Update(void)
{
    RSDK_THIS(Rhinobot);
    StateMachine_Run(self->state);
}

void Rhinobot_LateUpdate(void) {}

void Rhinobot_StaticUpdate(void) {}

void Rhinobot_Draw(void)
{
    RSDK_THIS(Rhinobot);
    if (self->playedHuff) {
        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        if (self->state == Rhinobot_State_HandleGroundMovement) {
            if (!self->moveDir)
                drawPos.x -= 0xD0000;
            else
                drawPos.x += 0xD0000;
        }
        RSDK.DrawSprite(&self->animatorDust, &drawPos, false);
    }
    RSDK.DrawSprite(&self->animatorBot, NULL, false);
}

void Rhinobot_Create(void *data)
{
    RSDK_THIS(Rhinobot);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->animatorBot, true, 0);
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 2, &self->animatorDust, true, 0);
    self->state = Rhinobot_State_Setup;
}

void Rhinobot_StageLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ"))
        Rhinobot->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Rhinobot.bin", SCOPE_STAGE);
    Rhinobot->hitbox.left   = -14;
    Rhinobot->hitbox.top    = -9;
    Rhinobot->hitbox.right  = 23;
    Rhinobot->hitbox.bottom = 15;
    Rhinobot->sfxHuff       = RSDK.GetSfx("Stage/Huff.wav");
    DEBUGMODE_ADD_OBJ(Rhinobot);
}

bool32 Rhinobot_CheckTileCollisions(void)
{
    RSDK_THIS(Rhinobot);
    if (self->moveDir) {
        if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x40000, 0xF0000, 8)) {
            if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0, false))
                return false;
            if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
                return false;
            self->state = Rhinobot_State_Fall;
        }
    }
    else if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x40000, 0xF0000, 8)) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0, false))
            return false;
        if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
            return false;
        self->state = Rhinobot_State_Fall;
    }
    return true;
}

void Rhinobot_CheckHit(void)
{
    RSDK_THIS(Rhinobot);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Rhinobot->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void Rhinobot_CheckOnScreen(void)
{
    RSDK_THIS(Rhinobot);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Rhinobot_Create(NULL);
    }
}

void Rhinobot_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rhinobot_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Rhinobot, NULL, self->position.x, self->position.y);
}

void Rhinobot_StateDelay_StartMove(void)
{
    RSDK_THIS(Rhinobot);
    self->playedHuff = false;
    if (!self->moveDir)
        self->velocity.x = -0x40000;
    else
        self->velocity.x = 0x40000;
    self->state      = Rhinobot_State_Skidding;
    self->stateDelay = Rhinobot_StateDelay_Skidding;
}

void Rhinobot_StateDelay_Skidding(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 1, &self->animatorBot, true, 0);
    self->skidDir ^= FLIP_X;
    self->acceleration = -self->acceleration;
    self->stateDelay   = Rhinobot_StateDelay_SkidFinish;
    self->topSpeed     = -self->topSpeed;
}

void Rhinobot_StateDelay_SkidFinish(void)
{
    RSDK_THIS(Rhinobot);
    self->moveDir ^= FLIP_X;
    self->direction ^= FLIP_X;
    self->playedHuff = false;
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->animatorBot, true, 0);
    self->stateDelay = Rhinobot_StateDelay_Skidding;
}

void Rhinobot_State_Setup(void)
{
    RSDK_THIS(Rhinobot);

    self->active     = ACTIVE_NORMAL;
    self->stateDelay = Rhinobot_StateDelay_Skidding;
    self->moveDir    = self->startDir;
    self->skidDir    = self->startDir;
    if (self->direction == FLIP_NONE) {
        self->acceleration = -0x1000;
        self->topSpeed     = -0x30000;
    }
    else {
        self->acceleration = 0x1000;
        self->topSpeed     = 0x30000;
    }
    self->state = Rhinobot_State_HandleGroundMovement;
    Rhinobot_State_HandleGroundMovement();
}

void Rhinobot_State_HandleGroundMovement(void)
{
    RSDK_THIS(Rhinobot);
    EntityPlayer *player = (EntityPlayer *)Player_GetNearestPlayer();

    bool32 flag = false;
    if (abs(player->position.y - self->position.y) <= 0x200000) {
        if (abs(player->position.x - self->position.x) <= 0x600000) {
            if (self->skidDir == self->moveDir && (player->position.x > self->position.x) == self->skidDir) {
                self->state      = Rhinobot_State_Idle;
                self->velocity.x = 0;
                self->timer      = 32;
                self->stateDelay = Rhinobot_StateDelay_StartMove;
                self->playedHuff = true;
                if (self->activeScreens)
                    RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
                flag = true;
            }
        }
    }

    if (!flag) {
        if (Rhinobot_CheckTileCollisions()) {
            if (self->state == Rhinobot_State_HandleGroundMovement) {
                self->velocity.x += self->acceleration;
                self->position.x += self->velocity.x;

                if (!self->velocity.x || (self->skidDir && self->velocity.x >= self->topSpeed)
                    || (!self->skidDir && self->velocity.x <= self->topSpeed)) {
                    self->stateDelay();
                }
            }
        }
        else {
            self->velocity.x = 0;
            self->skidDir    = self->moveDir ^ 1;
            self->moveDir ^= FLIP_X;
            self->direction ^= FLIP_X;
            self->playedHuff = false;
            RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->animatorBot, true, 0);
            self->stateDelay = Rhinobot_StateDelay_Skidding;
        }

        if (((self->moveDir && !self->skidDir && self->velocity.x < 0x28000)
             || (!self->moveDir && self->skidDir && self->velocity.x > -0x28000))
            && !self->playedHuff) {
            self->playedHuff = true;
            if (self->activeScreens)
                RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
        }
    }

    RSDK.ProcessAnimation(&self->animatorBot);
    RSDK.ProcessAnimation(&self->animatorDust);
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_State_Skidding(void)
{
    RSDK_THIS(Rhinobot);
    self->position.x += self->velocity.x;
    if (!Rhinobot_CheckTileCollisions()) {
        self->velocity.x = 0;
        self->skidDir    = self->moveDir ^ FLIP_X;
        self->moveDir ^= FLIP_X;
        self->direction ^= FLIP_X;
        self->playedHuff = false;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->animatorBot, true, 0);
        self->stateDelay = Rhinobot_StateDelay_Skidding;
        self->state      = Rhinobot_State_HandleGroundMovement;
    }
    RSDK.ProcessAnimation(&self->animatorBot);
    RSDK.ProcessAnimation(&self->animatorDust);
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_State_Idle(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.ProcessAnimation(&self->animatorDust);
    if (--self->timer <= 0)
        self->stateDelay();
    Rhinobot_CheckHit();
    Rhinobot_CheckOnScreen();
}

void Rhinobot_State_Fall(void)
{
    RSDK_THIS(Rhinobot);
    RSDK.ProcessAnimation(&self->animatorDust);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->animatorBot, true, 0);
        self->playedHuff = true;
        if (self->activeScreens)
            RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
        self->state = Rhinobot_State_HandleGroundMovement;
        Rhinobot_State_HandleGroundMovement();
    }
    else {
        Rhinobot_CheckHit();
        Rhinobot_CheckOnScreen();
    }
}

#if RETRO_INCLUDE_EDITOR
void Rhinobot_EditorDraw(void) { Rhinobot_Draw(); }

void Rhinobot_EditorLoad(void) { Rhinobot->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Rhinobot.bin", SCOPE_STAGE); }
#endif

void Rhinobot_Serialize(void) {}
#endif
