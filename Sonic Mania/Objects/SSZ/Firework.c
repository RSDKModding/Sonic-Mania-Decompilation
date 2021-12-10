#include "SonicMania.h"

ObjectFirework *Firework;

void Firework_Update(void)
{
    RSDK_THIS(Firework);

    if (self->animator1.animationID < 3) {
        RSDK.ProcessAnimation(&self->animator1);
    }
    else if (self->timer == 10) {
        if (self->animator1.frameID < self->animator1.frameCount - 1)
            RSDK.ProcessAnimation(&self->animator1);
        if (self->animator2.frameID < self->animator2.frameCount - 1)
            RSDK.ProcessAnimation(&self->animator2);
    }

    StateMachine_Run(self->state);
    Firework_HandlePlayerTimers();
}

void Firework_LateUpdate(void) {}

void Firework_StaticUpdate(void)
{

    bool32 flag = false;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        foreach_active(Firework, firework)
        {
            if (firework->field_A0) {
                if (!Firework->playingFlameSfx) {
                    RSDK.PlaySfx(Firework->sfxFlame, true, 255);
                    Firework->playingFlameSfx = true;
                }
                flag = true;
                foreach_break;
            }
        }
    }

    if (!flag) {
        if (Firework->playingFlameSfx) {
            RSDK.StopSfx(Firework->sfxFlame);
            Firework->playingFlameSfx = false;
        }
    }
}

void Firework_Draw(void)
{
    RSDK_THIS(Firework);

    self->rotation = ((self->angle >> 1) + 128) & 0x1FF;
    if (self->state == Firework_State_Unknown4) {
        if (self->timer > 0)
            RSDK.FillScreen(0xF0F0F0, self->timer - 128, self->timer - 256, self->timer);
    }
    else {
        RSDK.DrawSprite(&self->animator1, &self->position, false);
    }
    if (self->animator1.animationID >= 3) {
        self->inkEffect = INK_ADD;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        self->inkEffect = INK_NONE;
    }
}

void Firework_Create(void *data)
{
    RSDK_THIS(Firework);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->drawOrder     = Zone->drawOrderLow;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (!self->innerRadius)
        self->innerRadius = 64;
    if (!self->outerRadius)
        self->outerRadius = 128;
    if (!self->distance)
        self->distance = 512;
    self->startPos = self->position;
    self->state    = Firework_State_Setup;
}

void Firework_StageLoad(void)
{
    Firework->aniFrames      = RSDK.LoadSpriteAnimation("SSZ2/Firework.bin", SCOPE_STAGE);
    Firework->hitbox1.left   = -11;
    Firework->hitbox1.top    = -32;
    Firework->hitbox1.right  = 10;
    Firework->hitbox1.bottom = 15;
    Firework->hitbox2.left   = -10;
    Firework->hitbox2.top    = -10;
    Firework->hitbox2.right  = 10;
    Firework->hitbox2.bottom = 10;
    Firework->sfxRocketJet   = RSDK.GetSfx("Stage/RocketJet.wav");
    Firework->sfxFlame       = RSDK.GetSfx("Stage/Flame2.wav");
    Firework->sfxExplosion2  = RSDK.GetSfx("Stage/Explosion2.wav");
    Firework->sfxExplosion   = RSDK.GetSfx("Stage/Explosion.wav");
    Firework->active         = ACTIVE_ALWAYS;
}

void Firework_CheckPlayerCollisions(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (!((1 << playerID) & self->activePlayers)) {

            if (player->state != Player_State_None && !self->playerTimers[playerID]
                && Player_CheckCollisionTouch(player, self, &Firework->hitbox1)) {
                player->tileCollisions = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_PULLEYHOLD, &player->animator, false, 0);
                player->state = Player_State_None;
                self->activePlayers |= 1 << playerID;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                player->direction = player->position.x > self->position.x;
                if (player->sidekick == false)
                    self->activated = true;
            }
        }

        if ((1 << playerID) & self->activePlayers) {
            if (player->state == Player_State_None) {
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = self->angle >> 1;

                if (player->direction == FLIP_NONE)
                    player->rotation = ((self->angle >> 1) + 0xC0) % 511;
                else
                    player->rotation = ((self->angle >> 1) + 0x444) % 511;

                player->position.x = self->position.x;
                player->position.y = self->position.y;

                int power = -16;
                if (player->direction == FLIP_NONE)
                    power = 16;

                player->position.x += power * (RSDK.Sin1024(self->angle) << 6);
                player->position.y -= power * (RSDK.Cos1024(self->angle) << 6);
                player->position.x += -0x100 * RSDK.Cos1024(self->angle);
                player->position.y -= -0x100 * RSDK.Sin1024(self->angle);
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Firework_HandlePlayerTimers(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (self->playerTimers[playerID])
            self->playerTimers[playerID]--;
    }
}

void Firework_RemovePlayers(bool32 hurt)
{
    foreach_active(Player, player)
    {
        RSDK.GetEntityID(player);
        Firework_HandlePlayerRemoval(player, hurt);
    }
}

void Firework_HandlePlayerRemoval(void *p, bool32 hurt)
{
    RSDK_THIS(Firework);
    EntityPlayer *player = (EntityPlayer *)p;

    int playerID = RSDK.GetEntityID(player);
    if ((1 << playerID) & self->activePlayers) {
        player->state                  = Player_State_Air;
        player->onGround               = false;
        player->tileCollisions         = true;
        player->velocity               = self->velocity;
        self->playerTimers[playerID] = 30;
        if (hurt) {
            bool32 isSidekick = player->sidekick;
            player->sidekick  = true;
            Player_Hit(player);
            player->sidekick = isSidekick;
        }
        else {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        }
        self->activePlayers &= ~(1 << playerID);
    }
}

void Firework_HandlePlayerJump(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        if ((1 << RSDK.GetEntityID(player)) & self->activePlayers && player->jumpPress) {
            int playerID = RSDK.GetEntityID(player);

            if ((1 << playerID) & self->activePlayers) {
                player->state                  = Player_State_Air;
                player->onGround               = false;
                player->tileCollisions         = true;
                player->velocity.x             = self->velocity.x;
                player->velocity.y             = self->velocity.y;
                self->playerTimers[playerID] = 30;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                self->activePlayers &= ~(1 << playerID);
            }
        }
    }
}

void Firework_HandlePlayerControl(void)
{
    RSDK_THIS(Firework);

    foreach_active(Player, player)
    {
        if (!player->sidekick && (1 << RSDK.GetEntityID(player)) & self->activePlayers) {
            if (player->left) {
                --self->angle;
            }
            else if (player->right) {
                ++self->angle;
            }
        }
    }
}

void Firework_HandleMoveDir(void)
{
    RSDK_THIS(Firework);

    if (self->field_88 < 0x80000)
        self->field_88 += 0x2000;
    if (self->field_88 > 0x80000)
        self->field_88 = 0x80000;

    self->velocity.x = (self->field_88 >> 10) * RSDK.Cos1024(self->angle);
    self->velocity.y = (self->field_88 >> 10) * RSDK.Sin1024(self->angle);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int rx = abs(self->field_98.x - self->position.x) >> 16;
    int ry = abs(self->field_98.y - self->position.y) >> 16;
    self->field_8C += MathHelpers_SquareRoot(rx * rx + ry * ry);
    self->field_98.x = self->position.x;
    self->field_98.y = self->position.y;
}

void Firework_HandleDebrisSpawn(void)
{
    RSDK_THIS(Firework);

    if (!(self->timer % 5)) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->position.x -= 0x600 * RSDK.Cos1024(self->angle);
        debris->position.y -= 0x600 * RSDK.Sin1024(self->angle);
        debris->drawOrder = Zone->drawOrderLow;
        debris->timer     = 52;
    }
}

void Firework_HandleTileCollisions(void)
{
    RSDK_THIS(Firework);

    bool32 collided = RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_LWALL, 0, Firework->hitbox2.right << 13, 0, 4);
    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_ROOF, 0, 0, Firework->hitbox2.top << 13, 4);
    collided |= RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_RWALL, 0, Firework->hitbox2.left << 13, 0, 4);

    if (collided) {
        Camera_ShakeScreen(0, 0, 4);
        Firework_HandleRideEnd(true);
    }
}

void Firework_HandleRideEnd(bool32 hurt)
{
    RSDK_THIS(Firework);

    self->field_A0 = false;
    if (hurt) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Firework->sfxExplosion2, false, 255);
        Firework_RemovePlayers(true);
        self->state = Firework_State_Unknown4;
    }
    else {
        int angle = 0;

        for (int i = 0; i < 8; ++i) {
            int x                    = (RSDK.Cos1024(angle) << 6) * self->innerRadius + self->position.x;
            int y                    = (RSDK.Sin1024(angle) << 6) * self->innerRadius + self->position.y;
            EntityFirework *firework = CREATE_ENTITY(Firework, NULL, x, y);
            firework->velocity.x     = 48 * RSDK.Cos1024(angle);
            firework->velocity.y     = 48 * RSDK.Sin1024(angle);
            firework->state          = Firework_State_Unknown5;
            firework->angle          = angle;
            firework->field_7C       = 0;

            x                    = (RSDK.Cos1024(angle) << 6) * self->outerRadius + self->position.x;
            y                    = (RSDK.Sin1024(angle) << 6) * self->outerRadius + self->position.y;
            firework             = CREATE_ENTITY(Firework, NULL, x, y);
            firework->velocity.x = 32 * RSDK.Cos1024(angle);
            firework->velocity.y = 32 * RSDK.Sin1024(angle);
            firework->angle      = angle;
            firework->state      = Firework_State_Unknown5;
            firework->field_7C   = 1;
            angle += 0x80;
        }

        self->timer     = 512;
        self->drawOrder = Zone->hudDrawOrder;
        RSDK.PlaySfx(Firework->sfxExplosion, false, 255);
        Firework_RemovePlayers(false);
        self->state = Firework_State_Unknown4;
    }
}

void Firework_CheckOnScreen(void)
{
    RSDK_THIS(Firework);

    if (self->state == Firework_State_Unknown4) {
        if (self->timer <= 0) {
            if (!RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
                self->position.x = self->startPos.x;
                self->position.y = self->startPos.y;
                self->state      = Firework_State_ResetOnScreen;
                self->visible    = false;
                self->direction  = FLIP_NONE;
            }
        }
    }
    else {
        if (!RSDK.CheckOnScreen(self, NULL)) {
            if (!RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
                self->position.x = self->startPos.x;
                self->position.y = self->startPos.y;
                self->state      = Firework_State_ResetOnScreen;
                self->visible    = false;
                self->direction  = FLIP_NONE;
            }
        }
    }
}

void Firework_HandleAnimations(int a1, int16 angle, Animator *animator1, Animator *animator2, uint8 *direction)
{
    uint16 ang = angle & 0x3FF;
    int anim   = 4 * (a1 != 0) + 3;

    if (ang - 65 > 0x37F) {
        if (direction)
            *direction = FLIP_NONE;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim, animator1, true, 0);
    }
    else if (ang <= 0xC0) {
        if (direction)
            *direction = FLIP_Y;

        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (ang <= 0x140) {
        if (direction)
            *direction = FLIP_Y;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 1, animator1, true, 0);
    }
    else if (ang <= 0x1C0) {
        if (direction)
            *direction = FLIP_XY;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (ang <= 0x240) {
        if (direction)
            *direction = FLIP_X;

        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim, animator1, true, 0);
    }
    else if (ang <= 0x2C0) {
        if (direction)
            *direction = FLIP_X;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (ang > 0x340) {
        if (direction)
            *direction = FLIP_NONE;
        if (animator1)
            RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 2, animator1, true, 0);
    }
    else if (direction)
        *direction = FLIP_NONE;
    else if (animator1)
        RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 1, animator1, true, 0);

    if (animator2)
        RSDK.SetSpriteAnimation(Firework->aniFrames, anim + 3, animator2, true, 0);
}

void Firework_State_Setup(void)
{
    RSDK_THIS(Firework);

    self->position.x    = self->startPos.x;
    self->position.y    = self->startPos.y;
    self->field_98.x    = self->startPos.x;
    self->field_98.y    = self->startPos.y;
    self->visible       = true;
    self->activePlayers = 0;
    self->rotation      = 0;
    self->angle         = 768;
    self->timer         = 0;
    self->activated     = false;
    self->field_88      = 0;
    self->field_8C      = 0;
    self->drawOrder     = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Firework->aniFrames, 0, &self->animator1, true, 0);
    self->state = Firework_State_Unknown1;
}

void Firework_State_Unknown1(void)
{
    RSDK_THIS(Firework);

    Firework_CheckPlayerCollisions();
    if (self->activated) {
        RSDK.PlaySfx(Firework->sfxRocketJet, false, 255);
        RSDK.SetSpriteAnimation(Firework->aniFrames, 1, &self->animator1, true, 0);
        self->timer = 30;
        self->state = Firework_State_Unknown2;
    }
}

void Firework_State_Unknown2(void)
{
    RSDK_THIS(Firework);

    if (self->timer <= 0) {
        self->field_A0 = true;
        RSDK.SetSpriteAnimation(Firework->aniFrames, 2, &self->animator1, true, 0);
        self->field_98.x = self->position.x;
        self->field_98.y = self->position.y;
        self->timer      = 0;
        self->state      = Firework_State_Unknown3;
    }
    else {
        self->timer--;
    }
    Firework_CheckPlayerCollisions();
    Firework_HandlePlayerJump();
}

void Firework_State_Unknown3(void)
{
    RSDK_THIS(Firework);

    Firework_HandleMoveDir();
    Firework_HandleDebrisSpawn();
    Firework_CheckPlayerCollisions();
    Firework_HandlePlayerJump();
    Firework_HandlePlayerControl();
    if (self->field_8C > self->distance)
        Firework_HandleRideEnd(false);
    Firework_HandleTileCollisions();
    Firework_CheckOnScreen();
    ++self->timer;
}

void Firework_State_Unknown4(void)
{
    RSDK_THIS(Firework);

    if (self->timer <= 0) {
        self->activePlayers = 0;
        Firework_CheckOnScreen();
        self->visible = false;
    }
    else {
        self->timer -= 0x40;
    }
}

void Firework_State_ResetOnScreen(void)
{
    RSDK_THIS(Firework);

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->state = Firework_State_Setup;
        Firework_Create(NULL);
    }
}

void Firework_State_Unknown5(void)
{
    RSDK_THIS(Firework);

    self->visible  = true;
    self->rotation = 0;
    self->timer    = 0;
    self->state    = Firework_State_Unknown6;
}

void Firework_State_Unknown6(void)
{
    RSDK_THIS(Firework);

    if (self->timer >= 10) {
        if (self->timer == 10) {
            self->position.x += self->velocity.x;
            self->position.y += self->velocity.y;
            if (self->animator1.frameID == self->animator1.frameCount - 1)
                destroyEntity(self);
        }
    }
    else {
        Firework_HandleAnimations(self->field_7C, self->angle, &self->animator1, &self->animator2, &self->direction);
        ++self->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void Firework_EditorDraw(void)
{
    RSDK_THIS(Firework);
    RSDK.SetSpriteAnimation(Firework->aniFrames, 0, &self->animator1, false, 0);

    Firework_Draw();
}

void Firework_EditorLoad(void) { Firework->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Firework.bin", SCOPE_STAGE); }
#endif

void Firework_Serialize(void)
{
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, distance);
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, innerRadius);
    RSDK_EDITABLE_VAR(Firework, VAR_INT32, outerRadius);
}
