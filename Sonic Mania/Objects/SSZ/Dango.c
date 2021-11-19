#include "SonicMania.h"

ObjectDango *Dango;

void Dango_Update(void)
{
    RSDK_THIS(Dango);
    StateMachine_Run(self->state);

    if (self->timer2 > 0)
        self->timer2--;
}

void Dango_LateUpdate(void) {}

void Dango_StaticUpdate(void) {}

void Dango_Draw(void)
{
    RSDK_THIS(Dango);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Dango_Create(void *data)
{
    RSDK_THIS(Dango);
    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    self->drawFX |= FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);
    self->state = Dango_State_Setup;
}

void Dango_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Dango.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Dango.bin", SCOPE_STAGE);

    Dango->hitbox1.top    = -14;
    Dango->hitbox1.left   = -14;
    Dango->hitbox1.right  = 14;
    Dango->hitbox1.bottom = 14;
    Dango->hitbox2.top    = -128;
    Dango->hitbox2.left   = -112;
    Dango->hitbox2.right  = -18;
    Dango->hitbox2.bottom = 128;
    Dango->sfxBumper      = RSDK.GetSFX("Stage/Bumper3.wav");
    DEBUGMODE_ADD_OBJ(Dango);
}

void Dango_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityDango *dango = CREATE_ENTITY(Dango, NULL, self->position.x, self->position.y);
    RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &dango->animator, true, 0);
    dango->state = Dango_State_Unknown5;
    if (dango->direction == FLIP_NONE)
        dango->groundVel = -0x20000;
    else
        dango->groundVel = 0x20000;
}

void Dango_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Dango->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Dango_CheckOnScreen(void)
{
    RSDK_THIS(Dango);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->direction  = self->startDir;
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        Dango_Create(NULL);
    }
}

void Dango_CheckPlayerCollisions(void)
{
    RSDK_THIS(Dango);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Dango->hitbox1)) {
            int anim    = player->animator.animationID;
            bool32 flag = anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH;
#if RETRO_USE_PLUS
            if (player->characterID == ID_MIGHTY)
                flag |= anim == ANI_CROUCH || player->jumpAbilityTimer > 1;
            flag &= self->state == Dango_State_Unknown4 && player->state != Player_State_MightyHammerDrop;
#else
            flag &= self->state == Dango_State_Unknown4;
#endif

            if (flag) {
                if (anim != ANI_JUMP)
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                RSDK.PlaySfx(Dango->sfxBumper, false, 255);

                if (player->velocity.x <= 0) {
                    if (self->groundVel > player->velocity.x) {
                        if (self->groundVel > -0x20000)
                            self->groundVel = -0x20000;
                    }
                }
                else if (self->groundVel < player->velocity.x) {
                    self->groundVel = player->velocity.x;
                    if (self->groundVel < 0x20000)
                        self->groundVel = 0x20000;
                }
                self->velocity.x = self->groundVel;
#if RETRO_USE_PLUS
                if (!(player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1)) {
#endif
                    player->groundVel  = (player->position.x - self->position.x) >> 2;
                    player->velocity.x = player->groundVel;
                    player->velocity.y = (player->position.y - self->position.y) >> 2;
                    player->onGround   = false;
#if RETRO_USE_PLUS
                }
#endif

                if ((self->position.x > player->position.x && self->velocity.x < 0)
                    || (self->position.x < player->position.x && self->velocity.x > 0)) {
                    self->groundVel  = -self->groundVel;
                    self->velocity.x = -self->velocity.x;
                }
            }
            else {
                Player_CheckBadnikBreak(self, player, true);
            }
        }
    }
}

bool32 Dango_HandleTileCollisions(StateMachine(nextState), uint8 anim)
{
    RSDK_THIS(Dango);

    bool32 flag = false;
    int storeX  = self->position.x;
    int storeY  = self->position.y;
    self->position.x += self->groundVel;

    bool32 collided = false;
    if (self->groundVel <= 0)
        collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, Dango->hitbox1.left << 16, 0, 4);
    else
        collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_LWALL, 0, Dango->hitbox1.right << 16, 0, 4);

    if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, 2)) {
        uint16 tile   = RSDK.GetTileInfo(Zone->fgLow, self->position.x >> 16, (self->position.y + 0xD0000) >> 16);
        self->angle = RSDK.GetTileAngle(tile, 0, 0);
        if (self->groundVel <= 0) {
            if (self->angle < 0x80 && self->angle > 0x10)
                flag = true;
            else
                self->velocity.x = (self->groundVel >> 8) * RSDK.Cos256(self->angle);
        }
        else {
            if ((uint32)(self->angle - 0x81) <= 0x6E)
                flag = true;
            else
                self->velocity.x = (self->groundVel >> 8) * RSDK.Cos256(self->angle);
        }
    }
    else {
        flag = true;
    }

    if (collided)
        flag = true;
    if (self->flag)
        flag = false;
    if (flag) {
        self->position.x = storeX;
        self->position.y = storeY;
        RSDK.SetSpriteAnimation(Dango->aniFrames, anim, &self->animator, true, 0);
        self->state = nextState;
        self->flag  = true;
    }
    return flag;
}

void Dango_State_Setup(void)
{
    RSDK_THIS(Dango);

    self->active = ACTIVE_NORMAL;
    if (self->direction == FLIP_NONE)
        self->groundVel = -0x6000;
    else
        self->groundVel = 0x6000;
    self->velocity.y = 0;
    self->state      = Dango_State_Unknown1;
    Dango_State_Unknown1();
}

void Dango_State_Unknown1(void)
{
    RSDK_THIS(Dango);

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, false)) {
        RSDK.ProcessAnimation(&self->animator);
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
        if (!Dango_HandleTileCollisions(Dango_State_Unknown6, 5) && self->timer2 <= 0) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, self, &Dango->hitbox2)) {
                    RSDK.SetSpriteAnimation(Dango->aniFrames, 3, &self->animator, true, 0);
                    self->state = Dango_State_Unknown3;
                }
            }
        }
    }
    else {
        self->state = Dango_State_Unknown2;
        Dango_State_Unknown2();
    }
}

void Dango_State_Unknown6(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);
        self->flag       = false;
        self->groundVel  = -self->groundVel;
        self->velocity.x = -self->velocity.x;
        self->state      = Dango_State_Unknown1;
        self->direction ^= FLIP_X;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown2(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->groundVel;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, true)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);
        self->state = Dango_State_Unknown1;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown3(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &self->animator, true, 0);
        self->state = Dango_State_Unknown4;
        if (self->direction == FLIP_NONE)
            self->groundVel = -0x20000;
        else
            self->groundVel = 0x20000;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown4(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, false)) {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
        Dango_HandleTileCollisions(Dango_State_Unknown7, 4);
    }
    else {
        self->state = Dango_State_Unknown5;
        Dango_State_Unknown5();
    }
}

void Dango_State_Unknown5(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->groundVel;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, true)) {
        self->velocity.y = 0;
        self->state      = Dango_State_Unknown4;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

void Dango_State_Unknown7(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 1, &self->animator, true, 0);
        self->flag   = false;
        self->timer2 = 30;
        if (self->direction == FLIP_NONE)
            self->groundVel = -0x6000;
        else
            self->groundVel = 0x6000;
        self->state = Dango_State_Unknown1;
    }
    else {
        Dango_CheckPlayerCollisions();
        Dango_CheckOnScreen();
    }
}

#if RETRO_USE_PLUS
void Dango_StateTaunt_Unknown1(void)
{
    RSDK_THIS(Dango);

    if (++self->timer == 180) {
        self->timer     = 0;
        self->groundVel = 0x60000;
        self->state     = Dango_StateTaunt_Unknown2;
        RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &self->animator, true, 0);
    }
}

void Dango_StateTaunt_Unknown2(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->groundVel;
    RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xE0000, 14);

    foreach_active(PhantomRuby, ruby)
    {
        if (abs(self->position.x - ruby->position.x) < 0x120000) {
            RSDK.PlaySfx(Dango->sfxBumper, false, 255);
            self->state    = Dango_StateTaunt_Unknown3;
            ruby->state      = PhantomRuby_State_FallOffScreen;
            ruby->velocity.x = self->groundVel;
            ruby->velocity.y = -0x80000;
            RSDK.SetSpriteAnimation(Dango->aniFrames, 4, &self->animator, true, 0);
        }
    }
}

void Dango_StateTaunt_Unknown3(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    self->groundVel -= self->groundVel >> 2;
    self->position.x += self->groundVel;
    if (self->groundVel < 0x8000) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 6, &self->animator, true, 0);
        self->state = Dango_StateTaunt_Unknown4;
    }
}

void Dango_StateTaunt_Unknown4(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        ++self->timer;
    if (self->timer == 32) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 5, &self->animator, true, 0);
        self->timer = 0;
        self->state = Dango_StateTaunt_Unknown5;
    }
}

void Dango_StateTaunt_Unknown5(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Dango->aniFrames, 3, &self->animator, true, 0);
        self->direction = FLIP_NONE;
        self->state     = Dango_StateTaunt_Unknown6;
    }
}

void Dango_StateTaunt_Unknown6(void)
{
    RSDK_THIS(Dango);

    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.animationID == 3) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(Dango->aniFrames, 2, &self->animator, true, 0);
    }
    else {
        self->position.x -= 0x40000;
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}
#endif

#if RETRO_INCLUDE_EDITOR
void Dango_EditorDraw(void) { Dango_Draw(); }

void Dango_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Dango.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Dango->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Dango.bin", SCOPE_STAGE);
}
#endif

void Dango_Serialize(void) {}
