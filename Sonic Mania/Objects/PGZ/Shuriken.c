#include "SonicMania.h"

ObjectShuriken *Shuriken;

void Shuriken_Update(void)
{
    RSDK_THIS(Shuriken);
    if (self->state) {
        StateMachine_Run(self->state);
        if (self->animator.animationID == 1 || self->animator.animationID == 2 || self->animator.animationID == 4)
            RSDK.ProcessAnimation(&self->animator);
    }
}

void Shuriken_LateUpdate(void) {}

void Shuriken_StaticUpdate(void)
{
    EntityShuriken *groups[255];
    memset(groups, 0, sizeof(groups));

    foreach_active(Shuriken, shuriken)
    {
        if (shuriken->timerGroup) {
            if (!groups[shuriken->timerGroup - 1]) {
                groups[shuriken->timerGroup - 1] = shuriken;
                ++Shuriken->timerGroups[shuriken->timerGroup - 1];
            }
        }
    }

    for (int32 i = 0; i < 0xFF; ++i) {
        if (groups[i] == NULL && Shuriken->timerGroups[i])
            Shuriken->timerGroups[i] = 0;
    }
}

void Shuriken_Draw(void)
{
    RSDK_THIS(Shuriken);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Shuriken_Create(void *data)
{
    RSDK_THIS(Shuriken);
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = Shuriken_State_Setup;
}

void Shuriken_StageLoad(void)
{
    Shuriken->aniFrames      = RSDK.LoadSpriteAnimation("PSZ2/Shuriken.bin", SCOPE_STAGE);
    Shuriken->hitbox1.top    = -8;
    Shuriken->hitbox1.left   = -16;
    Shuriken->hitbox1.right  = 16;
    Shuriken->hitbox1.bottom = 8;
    Shuriken->hitbox2.top    = -256;
    Shuriken->hitbox2.left   = -64;
    Shuriken->hitbox2.right  = 64;
    Shuriken->hitbox2.bottom = 256;
    Shuriken->hitbox3.top    = -3;
    Shuriken->hitbox3.left   = -16;
    Shuriken->hitbox3.right  = 16;
    Shuriken->hitbox3.bottom = 5;
    Shuriken->hitbox4.top    = 0;
    Shuriken->hitbox4.left   = -16;
    Shuriken->hitbox4.right  = 16;
    Shuriken->hitbox4.bottom = 8;
    Shuriken->sfxArrowLaunch = RSDK.GetSFX("PSZ/ArrowLaunch.wav");
    Shuriken->sfxArrowHit    = RSDK.GetSFX("PSZ/ArrowHit.wav");
}

void Shuriken_CheckPlayerCollisions(void)
{
    RSDK_THIS(Shuriken);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Shuriken->hitbox3)) {
#if RETRO_USE_PLUS
            if (Player_CheckMightyShellHit(player, self, -0x400, -0x600)) {
                self->interaction = false;
                self->state       = Shuriken_Unknown14;
            }
            else {
#endif
                Player_CheckHit(player, self);
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}

void Shuriken_HandleSolidCollisions(void)
{
    RSDK_THIS(Shuriken);
    self->activePlayers = 0;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, &Shuriken->hitbox4)) {
            if (!player->onGround && !player->sidekick && !self->dropTimer)
                self->dropTimer = 1;
            self->activePlayers |= 1 << RSDK.GetEntityID(player);
            player->position.y &= 0xFFFF0000;
        }
    }
}

void Shuriken_State_Setup(void)
{
    RSDK_THIS(Shuriken);
    self->timer  = 0;
    self->active = ACTIVE_BOUNDS;
    RSDK.SetSpriteAnimation(Shuriken->aniFrames, 0, &self->animator, true, 0);
    self->drawOrder = Zone->drawOrderHigh;
    self->state     = Shuriken_Unknown4;
    Shuriken_Unknown4();
}

void Shuriken_Unknown4(void)
{
    RSDK_THIS(Shuriken);
    if (self->timerMode) {
        int32 timer = 0;
        if (self->timerGroup)
            timer = Shuriken->timerGroups[(self->timerGroup - 1)];
        else
            timer = Zone->timer;

        if (!((timer + self->intervalOffset) % self->interval)) {
            RSDK.SetSpriteAnimation(Shuriken->aniFrames, 2, &self->animator, false, 0);
            self->state = Shuriken_Unknown7;
            self->timer = 0;
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (!player->sidekick && Player_CheckCollisionTouch(player, self, &Shuriken->hitbox2)) {
                self->active = ACTIVE_NORMAL;
                RSDK.SetSpriteAnimation(Shuriken->aniFrames, 1, &self->animator, false, 0);
                self->state = Shuriken_Unknown5;
            }
        }
    }
}

void Shuriken_Unknown5(void)
{
    RSDK_THIS(Shuriken);

    bool32 flag = false;
    foreach_active(Player, player)
    {
        if (!player->sidekick && Player_CheckCollisionTouch(player, self, &Shuriken->hitbox2)) {
            flag = true;
            foreach_break;
        }
    }

    if (!flag) {
        self->timer = self->delay;
        self->state = Shuriken_Unknown6;
    }
}

void Shuriken_Unknown6(void)
{
    RSDK_THIS(Shuriken);
    if (self->timer) {
        self->timer--;
    }
    else {
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 2, &self->animator, false, 0);
        self->state = Shuriken_Unknown7;
        Shuriken_Unknown7();
    }
}

void Shuriken_Unknown7(void)
{
    RSDK_THIS(Shuriken);
    if (self->timer >= 15) {
        RSDK.PlaySfx(Shuriken->sfxArrowLaunch, false, 255);
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 3, &self->animator, false, 0);
        self->state            = Shuriken_Unknown8;
        EntityShuriken *shuriken = CREATE_ENTITY(Shuriken, self, self->position.x, self->position.y);
        shuriken->state          = Shuriken_Unknown10;
        shuriken->direction      = self->direction;
        shuriken->active         = ACTIVE_NORMAL;
        if (self->direction == FLIP_NONE)
            shuriken->velocity.x = 0x40000;
        else
            shuriken->velocity.x = -0x40000;
    }
    else {
        self->timer++;
    }
}

void Shuriken_Unknown8(void)
{
    RSDK_THIS(Shuriken);

    bool32 flag = false;
    foreach_active(Shuriken, shuriken)
    {
        if (shuriken->state == Shuriken_Unknown11) {
            if (RSDK.CheckObjectCollisionTouchBox(self, &Shuriken->hitbox1, shuriken, &Shuriken->hitbox3)) {
                flag = true;
                foreach_break;
            }
        }
    }

    if (!flag) {
        RSDK.SetSpriteAnimation(Shuriken->aniFrames, 4, &self->animator, false, 0);
        self->state = Shuriken_Unknown9;
        self->timer = 0;
    }
}

void Shuriken_Unknown9(void)
{
    RSDK_THIS(Shuriken);
    if (self->timer >= 10)
        self->state = Shuriken_State_Setup;
    else
        self->timer++;
}

void Shuriken_Unknown10(void)
{
    RSDK_THIS(Shuriken);
    self->timer     = 0;
    self->dropTimer = 0;
    self->active    = ACTIVE_NORMAL;
    self->drawOrder = Zone->drawOrderHigh - 1;
    RSDK.SetSpriteAnimation(Shuriken->aniFrames, 5, &self->animator, true, 0);
    self->state = Shuriken_Unknown11;
    Shuriken_Unknown11();
}

void Shuriken_Unknown11(void)
{
    RSDK_THIS(Shuriken);
    RSDK.ProcessAnimation(&self->animator);
    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
    Shuriken_CheckPlayerCollisions();
    self->position.x += self->velocity.x;

    bool32 flag = false;
    if (self->direction)
        flag = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, -0xE0000, 0, 2);
    else
        flag = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_LWALL, 0, 0xE0000, 0, 2);
    if (flag) {
        foreach_active(Shuriken, shuriken)
        {
            if (shuriken != self) {
                if (RSDK.CheckObjectCollisionTouchBox(self, &Shuriken->hitbox3, shuriken, &Shuriken->hitbox3)) {
                    shuriken->velocity.y = 0;
                    shuriken->state      = Shuriken_Unknown13;
                }
            }
        }
        self->velocity.x = 0;
        RSDK.PlaySfx(Shuriken->sfxArrowHit, false, 255);
        self->state = Shuriken_Unknown12;
    }
}

void Shuriken_Unknown12(void)
{
    RSDK_THIS(Shuriken);
    if (self->timer < 300)
        self->timer++;

    if (self->dropTimer > 0 && self->dropTimer < 60)
        self->dropTimer++;
    Shuriken_HandleSolidCollisions();

    bool32 flag = false;
    if (self->direction)
        flag = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, -0xE0000, 0, 2);
    else
        flag = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_LWALL, 0, 0xE0000, 0, 2);
    if (!flag || self->timer >= 300 || self->dropTimer >= 60) {
        self->updateRange.x = 0x4000000;
        self->updateRange.y = 0x4000000;
        self->velocity.y    = 0;
        self->state         = Shuriken_Unknown13;
    }
}

void Shuriken_Unknown13(void)
{
    RSDK_THIS(Shuriken);
    if (self->velocity.y >= 0x40000)
        self->velocity.y = 0x40000;
    else
        self->velocity.y += 0x4000;
    self->position.y += self->velocity.y;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Shuriken_Unknown14(void)
{
    RSDK_THIS(Shuriken);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->visible ^= 1;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void Shuriken_EditorDraw(void) {}

void Shuriken_EditorLoad(void) {}
#endif

void Shuriken_Serialize(void)
{
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, delay);
    RSDK_EDITABLE_VAR(Shuriken, VAR_BOOL, timerMode);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(Shuriken, VAR_UINT8, timerGroup);
}
