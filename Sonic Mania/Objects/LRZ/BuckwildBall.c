#include "SonicMania.h"

ObjectBuckwildBall *BuckwildBall;

void BuckwildBall_Update(void)
{
    RSDK_THIS(BuckwildBall);
    StateMachine_Run(self->state);
    if (self->state != BuckwildBall_State_Unknown2 && self->state != BuckwildBall_State_Unknown5 && self->state != BuckwildBall_State_Setup) {
        if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x1C0000, 2)) {
            if (self->particleDelay-- <= 0) {
                BuckwildBall_SpawnDebris();
                BuckwildBall_HandleTimerSfx();
                self->particleDelay = RSDK.Rand(8, 15);
            }
        }
        BuckwildBall_CheckPlayerCollisions();
        BuckwildBall_HandleRollCrush();
        RSDK.ProcessAnimation(&self->animator);

        if (self->timerSfx > 0)
            self->timerSfx--;
    }
}

void BuckwildBall_LateUpdate(void) {}

void BuckwildBall_StaticUpdate(void) {}

void BuckwildBall_Draw(void)
{
    RSDK_THIS(BuckwildBall);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void BuckwildBall_Create(void *data)
{
    RSDK_THIS(BuckwildBall);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->drawOrder     = Zone->drawOrderLow;
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->field_B8      = -1;
        if (self->mode != 1)
            self->updateRange.x = (self->amplitude + 128) << 16;
        self->state = BuckwildBall_State_Setup;
        if (!self->speed)
            self->speed = 2;
    }
}

void BuckwildBall_StageLoad(void)
{
    BuckwildBall->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/BuckwildBall.bin", SCOPE_STAGE);
    BuckwildBall->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    BuckwildBall->sfxImpact      = RSDK.GetSFX("Stage/Impact2.wav");
    BuckwildBall->sfxSharp       = RSDK.GetSFX("Stage/Sharp.wav");
    BuckwildBall->hitbox.left    = -28;
    BuckwildBall->hitbox.top     = -28;
    BuckwildBall->hitbox.right   = 28;
    BuckwildBall->hitbox.bottom  = 28;
}

void BuckwildBall_HandleTimerSfx(void)
{
    RSDK_THIS(BuckwildBall);

    if (self->timerSfx > 0) {
        LogHelpers_Print("timerSfx = %d", self->timerSfx);
    }
    else {
        RSDK.PlaySfx(BuckwildBall->sfxImpact, false, 255);
        self->timerSfx = 8;
    }
}

void BuckwildBall_CheckOnScreen(void)
{
    RSDK_THIS(BuckwildBall);

    if (!RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange) && !RSDK.CheckPosOnScreen(&self->position, &self->updateRange)) {
        if (self->respawn) {
            self->position.x = self->startPos.x;
            self->position.y = self->startPos.y;
            self->state      = BuckwildBall_State_Setup;
            self->active     = ACTIVE_BOUNDS;
            self->visible    = false;
        }
        else {
            destroyEntity(self);
        }
    }
}

void BuckwildBall_SpawnDebris(void)
{
    RSDK_THIS(BuckwildBall);

    int x = self->position.x;
    int y = self->position.y + 0x1C0000;

    int sizeX  = (BuckwildBall->hitbox.right - BuckwildBall->hitbox.left) >> 1;
    int spawnX = x + ((RSDK.Rand(0, 64) - 32) << 16);
    int spawnY = ((RSDK.Rand(0, 8) - 4) << 15) + y;

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, spawnY);
    RSDK.SetSpriteAnimation(BuckwildBall->particleFrames, 1, &debris->animator, true, 0);
    debris->drawOrder  = Zone->drawOrderHigh;
    debris->gravity    = 0x3800;
    debris->velocity.x = 0x180 * (abs(spawnX - x) >> 8) / sizeX;
    if (debris->position.x < self->position.x) {
        debris->direction  = FLIP_X;
        debris->velocity.x = -debris->velocity.x;
    }
    debris->velocity.y = -0x1000 * RSDK.Rand(32, 54);
}

void BuckwildBall_CheckPlayerCollisions(void)
{
    RSDK_THIS(BuckwildBall);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &BuckwildBall->hitbox)) {
#if RETRO_USE_PLUS
            if (!Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer))
#endif
                Player_CheckHit(player, self);
        }
    }
}

void BuckwildBall_HandleRollCrush(void)
{
    RSDK_THIS(BuckwildBall);

    Hitbox crushHitbox;
    crushHitbox.left   = -8;
    crushHitbox.top    = -8;
    crushHitbox.right  = 8;
    crushHitbox.bottom = 8;

    foreach_active(Iwamodoki, iwamodoki)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, iwamodoki, &crushHitbox)) {
#if RETRO_USE_PLUS
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.RandSeeded(0, 32, &Zone->randSeed) >> 4] + 1), iwamodoki->position.x,
                          iwamodoki->position.y);
#else
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Rand(0, 32) >> 4] + 1), iwamodoki->position.x, iwamodoki->position.y);
#endif
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), iwamodoki->position.x, iwamodoki->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(iwamodoki);
        }
    }

    foreach_active(Fireworm, fireworm)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, fireworm, &crushHitbox)) {
#if RETRO_USE_PLUS
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.RandSeeded(0, 32, &Zone->randSeed) >> 4] + 1), fireworm->position.x,
                          fireworm->position.y);
#else
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Rand(0, 32) >> 4] + 1), fireworm->position.x,
                          fireworm->position.y);
#endif
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), fireworm->position.x, fireworm->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(fireworm);
        }
    }

    foreach_active(Toxomister, toxomister)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, toxomister, &crushHitbox)) {
            if (toxomister->state == Toxomister_State_Unknown1) {
#if RETRO_USE_PLUS
                CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.RandSeeded(0, 32, &Zone->randSeed) >> 4] + 1), toxomister->position.x,
                              toxomister->position.y);
#else
                CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Rand(0, 32) >> 4] + 1), toxomister->position.x, toxomister->position.y);
#endif
                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), toxomister->position.x, toxomister->position.y)->drawOrder = Zone->drawOrderHigh;
                RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
                destroyEntity(toxomister);
            }
            else if (!toxomister->grabbedPlayer) {
                destroyEntity(toxomister);
            }
        }
    }

    Hitbox spikeHitbox;
    spikeHitbox.left   = -16;
    spikeHitbox.top    = -16;
    spikeHitbox.right  = 16;
    spikeHitbox.bottom = 16;

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, spikes, &spikeHitbox)) {
            for (int i = 0; i < 2; ++i) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, Debris_State_Fall, (((2 * (i != 0) - 1) * (spikes->type == 0)) << 19) + spikes->position.x,
                                  (((2 * (i != 0) - 1) * (spikes->type != 0)) << 19) + spikes->position.y);
                RSDK.SetSpriteAnimation(BuckwildBall->particleFrames, 4, &debris->animator, true, spikes->type >> 1);
                debris->drawOrder = Zone->drawOrderHigh;
                debris->direction = spikes->direction;
                debris->drawFX |= FX_ROTATE;
                debris->gravity    = 0x3800;
                debris->rotSpeed   = RSDK.Rand(-32, 32);
                debris->velocity.x = RSDK.Rand(-0x28000, 0x28000);
                debris->velocity.y = -0x1000 * RSDK.Rand(32, 96);
            }
            destroyEntity(spikes);
            RSDK.PlaySfx(BuckwildBall->sfxSharp, false, 255);
            RSDK.PlaySfx(BuckwildBall->sfxImpact, false, 255);
            self->timerSfx = 8;
        }
    }
}

void BuckwildBall_State_Setup(void)
{
    RSDK_THIS(BuckwildBall);

    RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &self->animator, true, 0);

    self->timerSfx = 0;
    if (self->mode) {
        if (!--self->mode) {
            int sizeX                   = self->detectSize.x;
            int sizeY                   = self->detectSize.y;
            self->visible             = false;
            self->state               = BuckwildBall_State_Unknown2;
            self->drawOrder           = Zone->drawOrderLow;
            self->detectHitbox.left   = -(sizeX >> 17);
            self->detectHitbox.top    = -(sizeY >> 17);
            self->detectHitbox.right  = sizeX >> 17;
            self->detectHitbox.bottom = sizeY >> 17;
            self->velocity.x          = 0;
            self->ballPos             = self->startPos;
            self->ballPos.x += self->detectOffset.x;
            self->ballPos.y += self->detectOffset.y;
            self->velocity.y = 0;
        }
    }
    else {
        self->state = BuckwildBall_State_Unknown1;
    }
}

void BuckwildBall_State_Unknown1(void)
{
    RSDK_THIS(BuckwildBall);

    int angle          = ((self->speed & 0xFFFF) * (Zone->timer & 0xFFFF)) & 0x3FF;
    self->position.x = (self->amplitude << 6) * RSDK.Sin1024(angle) + self->startPos.x;
    self->direction  = (angle - 0x100) > 0x200;
}

void BuckwildBall_State_Unknown2(void)
{
    RSDK_THIS(BuckwildBall);

    self->position.x = self->ballPos.x;
    self->position.y = self->ballPos.y;

    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &self->detectHitbox)) {
                self->visible = true;
                self->active  = ACTIVE_NORMAL;
                self->state   = BuckwildBall_State_Unknown3;
            }
        }
    }

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void BuckwildBall_State_Unknown3(void)
{
    RSDK_THIS(BuckwildBall);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x1C0000, true)) {
        if (!RSDK.GetEntityCount(Drillerdroid->objectID, true) || (Drillerdroid->field_5C[self->field_B8] == 1)) {
            self->velocity.y = 0;
            self->state      = BuckwildBall_State_Unknown4;
            self->velocity.x = abs(self->speed << 15) * (2 * (self->direction != FLIP_NONE) - 1);
        }
        else if (Drillerdroid->field_5C[self->field_B8]) {
            self->state                   = BuckwildBall_State_Unknown5;
            self->velocity.x              = 0;
            self->velocity.y              = -0x40000;
            self->animator.animationSpeed = 0;
            self->drawOrder               = Zone->drawOrderHigh;
        }
        else {
            int slot                                                                        = RSDK.GetEntityID(Drillerdroid->boss);
            RSDK_GET_ENTITY(slot + 6 + self->field_B8, CollapsingPlatform)->collapseDelay = 1;
            Drillerdroid->field_5C[self->field_B8]                                        = -1;
            self->velocity.y                                                              = -0x40000;
        }

        BuckwildBall_HandleTimerSfx();
        Camera_ShakeScreen(0, 0, 5);
    }

    if (self->field_B8 > -1) {
        BuckwildBall_CheckOnScreen();
    }
}

void BuckwildBall_State_Unknown4(void)
{
    RSDK_THIS(BuckwildBall);

    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    self->velocity.x += abs(self->speed << 10) * (2 * !(self->direction == FLIP_NONE) - 1);
    self->position.x += self->velocity.x;
    if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x1C0000, 2)) {
        self->velocity.y = 0;
        if (abs(self->velocity.x) > 0x20000 && RSDK.Rand(0, 100) > 0x50) {
            BuckwildBall_SpawnDebris();
            BuckwildBall_HandleTimerSfx();
            self->velocity.y = -0x18000;
        }
    }

    bool32 collided = false;
    if (self->direction == FLIP_X) {
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x1C0000, 0, true);
    }
    else if (self->direction == FLIP_NONE) {
        collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x1C0000, 0, true);
    }

    if (collided) {
        self->state = BuckwildBall_State_Unknown5;
        BuckwildBall_HandleTimerSfx();
        Camera_ShakeScreen(0, 0, 5);
        self->velocity.x              = 0;
        self->velocity.y              = -0x40000;
        self->animator.animationSpeed = 0;
        self->drawOrder               = Zone->drawOrderHigh;
    }

    if (self->field_B8 > -1) {
        BuckwildBall_CheckOnScreen();
    }
}

void BuckwildBall_State_Unknown5(void)
{
    RSDK_THIS(BuckwildBall);
    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->visible = Zone->timer % 4 < 2;

    BuckwildBall_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void BuckwildBall_EditorDraw(void)
{
    RSDK_THIS(BuckwildBall);

    RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &self->animator, true, 0);

    int sizeX                   = self->detectSize.x;
    int sizeY                   = self->detectSize.y;
    self->detectHitbox.left   = -(sizeX >> 17);
    self->detectHitbox.top    = -(sizeY >> 17);
    self->detectHitbox.right  = sizeX >> 17;
    self->detectHitbox.bottom = sizeY >> 17;
    self->ballPos             = self->position;
    self->ballPos.x += self->detectOffset.x;
    self->ballPos.y += self->detectOffset.y;

    BuckwildBall_Draw();

    DrawHelpers_DrawHitboxOutline(0xFF0000, FLIP_NONE, self->ballPos.x, self->ballPos.y, &self->detectHitbox);
}

void BuckwildBall_EditorLoad(void)
{
    BuckwildBall->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/BuckwildBall.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BuckwildBall, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void BuckwildBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT32, amplitude);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT8, mode);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_VECTOR2, detectOffset);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_VECTOR2, detectSize);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_BOOL, respawn);
}
