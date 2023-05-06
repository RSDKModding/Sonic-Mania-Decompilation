// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggPistonsMKII Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectEggPistonsMKII *EggPistonsMKII;

void EggPistonsMKII_Update(void)
{
    RSDK_THIS(EggPistonsMKII);

    StateMachine_Run(self->state);
}

void EggPistonsMKII_LateUpdate(void) {}

void EggPistonsMKII_StaticUpdate(void) {}

void EggPistonsMKII_Draw(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (self->type == EGGPISTON_ALARM) {
        RSDK.FillScreen(0x880000, EggPistonsMKII->alarmTimer, EggPistonsMKII->alarmTimer, EggPistonsMKII->alarmTimer);
    }
    else {
        RSDK.DrawSprite(&self->altAnimator, NULL, false);
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
}

void EggPistonsMKII_Create(void *data)
{
    RSDK_THIS(EggPistonsMKII);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active = ACTIVE_BOUNDS;

            if (data)
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case EGGPISTON_PISTON:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 0, &self->mainAnimator, true, 0);

                    self->updateRange.x = 0xA00000;
                    self->updateRange.y = 0xEC0000;

                    self->hitbox.left   = -32;
                    self->hitbox.top    = -96;
                    self->hitbox.right  = 32;
                    self->hitbox.bottom = 60;

                    self->state     = EggPistonsMKII_StatePiston_Idle;
                    self->visible   = true;
                    self->drawGroup = Zone->fgDrawGroup[1] - 1;
                    self->pistonID  = self->position.y;

                    EggPistonsMKII->pistons[EggPistonsMKII->pistonCount++] = self;
                    break;

                case EGGPISTON_CONTROL:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->mainAnimator, true, 0);

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    self->hitbox.left   = -33;
                    self->hitbox.top    = -37;
                    self->hitbox.right  = 33;
                    self->hitbox.bottom = 6;

                    self->state     = EggPistonsMKII_State_SetupArena;
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    EggPistonsMKII->controller = self;
                    break;

                case EGGPISTON_EMITTER:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 1, &self->mainAnimator, true, 0);

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;

                    self->state     = EggPistonsMKII_StatePiston_Idle;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;
                    break;

                case EGGPISTON_BARRIER:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &self->mainAnimator, true, 0);

                    self->hitbox.left   = -32;
                    self->hitbox.top    = -32;
                    self->hitbox.right  = 32;
                    self->hitbox.bottom = 32;

                    self->state         = EggPistonsMKII_StateOrbGenerator_Idle;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[0];
                    break;

                case EGGPISTON_PLASMABALL:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 2, &self->mainAnimator, true, 0);

                    self->hitbox.left   = -12;
                    self->hitbox.top    = -12;
                    self->hitbox.right  = 12;
                    self->hitbox.bottom = 12;

                    self->state         = EggPistonsMKII_StateOrb_MoveToTargetPos;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[0];
                    break;

                case EGGPISTON_ALARM:
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    self->state     = EggPistonsMKII_StateAlarm_Active;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[1];
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void EggPistonsMKII_StageLoad(void)
{
    EggPistonsMKII->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/EggPistonsMKII.bin", SCOPE_STAGE);
    EggPistonsMKII->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ1.bin", SCOPE_STAGE);

    EggPistonsMKII->pistonCount        = 0;
    EggPistonsMKII->invincibilityTimer = 0;
    EggPistonsMKII->isPhase2           = false;

    EggPistonsMKII->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    EggPistonsMKII->sfxWall      = RSDK.GetSfx("Stage/Wall.wav");
    EggPistonsMKII->sfxElectrify = RSDK.GetSfx("Stage/Electrify.wav");
    EggPistonsMKII->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    EggPistonsMKII->sfxAlarm     = RSDK.GetSfx("MMZ/Alarm.wav");

    RSDK.StopSfx(EggPistonsMKII->sfxAlarm);
}

void EggPistonsMKII_CheckPlayerCollisions_Piston(void)
{
    RSDK_THIS(EggPistonsMKII);

    EntityEggPistonsMKII *parent = self->parent;

    foreach_active(Player, player)
    {
        int32 side = Player_CheckCollisionBox(player, self, &self->hitbox);
        if (side == C_TOP) {
            player->position.y += self->velocity.y;
            player->position.y &= 0xFFFF0000;
        }
        else if (side == C_BOTTOM && player->onGround)
            player->deathType = PLAYER_DEATH_DIE_USESFX;
    }

    self->position.y += self->velocity.y;
    if (parent) {
        parent->position.x = self->position.x;
        parent->position.y = self->position.y;
        parent->position.y += self->direction ? 0xE0000 : -0xA0000;
    }
}

void EggPistonsMKII_CheckPlayerCollisions_EggPiston(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (!EggPistonsMKII->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox)
            && EggPistonsMKII_CheckPlayerAttacking(player)) {
            EggPistonsMKII_Hit();
        }
    }
}

void EggPistonsMKII_CheckPlayerCollisions_Ball(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            Player_ElementHurt(player, self, SHIELD_LIGHTNING);
    }
}

void EggPistonsMKII_CheckPlayerCollisions_Solid(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_BOTTOM) {
            if (player->onGround)
                player->deathType = PLAYER_DEATH_DIE_USESFX;
        }
    }
}

void EggPistonsMKII_Hit(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII->invincibilityTimer = 120;
    if (EggPistonsMKII->health > 0)
        EggPistonsMKII->health--;

    if (EggPistonsMKII->health) {
        RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 1, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 4, &self->altAnimator, true, 0);
        RSDK.PlaySfx(EggPistonsMKII->sfxHit, false, 255);
    }
    else {
        RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 2, &self->mainAnimator, true, 0);
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->mainAnimator.frameID = 1;
        RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
        self->state = EggPistonsMKII_State_Finish;
    }
}

void EggPistonsMKII_Explode(void)
{
    RSDK_THIS(EggPistonsMKII);

    int32 interval = 4 * (EggPistonsMKII->health != 0) + 3;

    if (!(Zone->timer % interval)) {
        RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x = self->position.x + (RSDK.Rand(-24, 24) << 16);
            int32 y = self->position.y + (RSDK.Rand(-48, 48) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

EntityEggPistonsMKII *EggPistonsMKII_GetNextPiston(void)
{
    RSDK_THIS(EggPistonsMKII);

    int32 pistonID               = RSDK.Rand(0, 5);
    EntityEggPistonsMKII *piston = EggPistonsMKII->pistons[pistonID];

    if (piston->state != EggPistonsMKII_StatePiston_Idle)
        pistonID = EggPistonsMKII->pistonID - 1; // force another try

    while (pistonID == EggPistonsMKII->pistonID - 1 || pistonID == EggPistonsMKII->pistonID + 1) {
        pistonID = RSDK.Rand(0, 5);
        piston   = EggPistonsMKII->pistons[pistonID];

        if (piston->state != EggPistonsMKII_StatePiston_Idle)
            pistonID = EggPistonsMKII->pistonID - 1; // force another try
    }

    EggPistonsMKII->pistonID = pistonID;

    self->visible = true;
    piston->timer = 15;
    piston->state = EggPistonsMKII_StatePiston_Shaking;
    return piston;
}

void EggPistonsMKII_SpawnElecBall(void)
{
    EntityEggPistonsMKII *spawner = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, EggPistonsMKII);
    spawner->state                = EggPistonsMKII_StateOrbGenerator_Warning;
    spawner->timer                = 330;
    int32 offset                  = 0;

    for (int32 i = 0; i < 4; ++i) {
        EntityEggPistonsMKII *child = CREATE_ENTITY(EggPistonsMKII, INT_TO_VOID(EGGPISTON_PLASMABALL), spawner->position.x, spawner->position.y);
        child->pistonID             = offset + spawner->position.x + ((RSDK.Rand(0, 48) - 48) << 16);
        child->inkEffect            = INK_ADD;
        child->velocity.x           = (child->pistonID - child->position.x) >> 4;
        child->alpha                = 0;
        offset -= 0x570000;
    }
}

bool32 EggPistonsMKII_CheckPlayerAttacking(EntityPlayer *player)
{
    RSDK_THIS(EggPistonsMKII);

    if (Player_CheckAttacking(player, NULL)) {
        if (player->position.x >= self->position.x) {
            player->velocity.x = 0x30000;
            player->groundVel  = 0x30000;
        }
        else {
            player->velocity.x = -0x30000;
            player->groundVel  = -0x30000;
        }

        if (player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_GLIDE) {
            RSDK.SetSpriteAnimation(Player->knuxFrames, ANI_GLIDE_DROP, &player->animator, true, 0);
            player->state = Player_State_KnuxGlideDrop;
        }

        return true;
    }

    return false;
}

void EggPistonsMKII_State_SetupArena(void)
{
    RSDK_THIS(EggPistonsMKII);

    for (int32 i = 0; i < 6; ++i) {
        EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot + 8 + i, CollapsingPlatform);
        if (platform->classID == CollapsingPlatform->classID)
            platform->active = ACTIVE_NEVER;
    }

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 71;
        Zone->cameraBoundsT[0]      = Zone->cameraBoundsB[0] - ScreenInfo->size.y;

        self->active = ACTIVE_NORMAL;
        self->state  = EggPistonsMKII_State_EnterBoss;
    }
}

void EggPistonsMKII_State_EnterBoss(void)
{
    RSDK_THIS(EggPistonsMKII);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    int32 pos = MANIA_USE_PLUS ? (self->position.x - 0xC00000) : self->position.x;
    if (player1->position.x > pos) {
#if MANIA_USE_PLUS
        if (player1->onGround)
            player1->state = Player_State_Ground;
        player1->stateInput = StateMachine_None;
        player1->jumpPress  = false;
        player1->jumpHold   = false;
        player1->left       = false;
        player1->right      = true;
        player1->up         = false;
        player1->down       = false;

        if (player1->groundVel > 0x30000)
            player1->groundVel = 0x30000;

        if (player1->position.x > (self->position.x - 0x80000)) {
#endif
            Zone->playerBoundActiveL[0] = true;
            Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x;
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            EggPistonsMKII->health = 8;

            self->timer = 142;
            for (int32 i = 0; i < 2; ++i) {
                EntityEggPistonsMKII *barrier = RSDK_GET_ENTITY(SceneInfo->entitySlot + 14 + i, EggPistonsMKII);
                barrier->velocity.y           = -0x20000;
                barrier->timer                = 64;
                barrier->state                = EggPistonsMKII_StateBarrier_Solid;
            }

            player1->right = false;
            player1->left  = true;
            self->state    = EggPistonsMKII_State_PistonReveal;
#if MANIA_USE_PLUS
        }
#endif
    }
}

void EggPistonsMKII_State_PistonReveal(void)
{
    RSDK_THIS(EggPistonsMKII);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (!player1->stateInput && player1->groundVel <= 0) {
        player1->groundVel  = 0;
        player1->velocity.x = 0;
        player1->left       = false;
        player1->direction  = FLIP_NONE;
    }

    if (--self->timer <= 0) {
        player1->stateInput = Player_Input_P1;

        for (int32 i = 0; i < 6; ++i) {
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot + 8 + i, CollapsingPlatform);
            if (platform->classID == CollapsingPlatform->classID) {
                platform->active        = ACTIVE_NORMAL;
                platform->collapseDelay = 8;
                platform->stoodPos.x    = self->position.x;
            }
        }

        for (int32 i = 0; i < 2; ++i) {
            EntityEggPistonsMKII *piston = RSDK_GET_ENTITY(SceneInfo->entitySlot + 14 + i, EggPistonsMKII);
            piston->velocity.y           = 0x80000;
            piston->timer                = 8;
            piston->state                = EggPistonsMKII_StateBarrier_Solid;
        }

        self->position.y -= 0x800000;
        self->timer = 120;
        self->state = EggPistonsMKII_State_ClassicMode;
    }
}

void EggPistonsMKII_State_ClassicMode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (--self->timer <= 0) {
        if (self->pistonID) {
            if (EggPistonsMKII->health > 4) {
                EggPistonsMKII_SpawnElecBall();
                RSDK.PlaySfx(EggPistonsMKII->sfxElectrify, false, 255);
                self->timer = 345;
            }
            else {
                self->timer = 120;
                self->state = EggPistonsMKII_State_StartPinchMode;
            }
        }
        else {
            EggPistonsMKII_GetNextPiston();
            EggPistonsMKII_GetNextPiston()->parent = self;

            RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->altAnimator, true, 0);

            self->timer = 165;
            RSDK.PlaySfx(EggPistonsMKII->sfxWall, false, 255);
        }
        self->pistonID ^= 1;
    }

    if (EggPistonsMKII->invincibilityTimer)
        EggPistonsMKII->invincibilityTimer--;

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= self->position.x;

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->altAnimator);

    if (self->visible)
        EggPistonsMKII_CheckPlayerCollisions_EggPiston();
}

void EggPistonsMKII_State_StartPinchMode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (--self->timer <= 0) {
        CREATE_ENTITY(EggPistonsMKII, INT_TO_VOID(EGGPISTON_ALARM), self->position.x, self->position.y);

        EntityEggPistonsMKII *orbSpawner = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, EggPistonsMKII);
        RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), orbSpawner->position.x, orbSpawner->position.y)->drawGroup = Zone->objectDrawGroup[1];

        EggPistonsMKII->isPhase2 = true;
        destroyEntity(orbSpawner);

        self->timer    = 120;
        self->pistonID = 1;
        RSDK.PlaySfx(EggPistonsMKII->sfxAlarm, true, 255);
        self->state = EggPistonsMKII_State_PinchMode;
    }
}

void EggPistonsMKII_State_PinchMode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (--self->timer <= 0) {
        EntityEggPistonsMKII *piston = EggPistonsMKII_GetNextPiston();
        if (!self->pistonID) {
            piston->parent = self;
            RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->altAnimator, true, 0);
        }

        self->pistonID = (self->pistonID + 1) & 3;

        RSDK.PlaySfx(EggPistonsMKII->sfxWall, false, 255);
        self->timer = 64;
    }

    if (EggPistonsMKII->invincibilityTimer)
        EggPistonsMKII->invincibilityTimer--;

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= self->position.x;
    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->altAnimator);

    if (self->visible)
        EggPistonsMKII_CheckPlayerCollisions_EggPiston();

    if (!EggPistonsMKII->health)
        self->state = EggPistonsMKII_State_Destroyed;
}

void EggPistonsMKII_State_Destroyed(void)
{
    RSDK_THIS(EggPistonsMKII);

    int32 id = 0;
    for (int32 i = 0; i < 5; ++i) {
        EntityEggPistonsMKII *piston = EggPistonsMKII->pistons[i];

        if (piston->state == EggPistonsMKII_StatePiston_Idle)
            ++id;
    }

    if (++id >= 5) {
        EggPistonsMKII->isPhase2 = false;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        RSDK.StopSfx(EggPistonsMKII->sfxAlarm);
        self->state = EggPistonsMKII_State_Finish;
    }
}

void EggPistonsMKII_State_Finish(void) {}

void EggPistonsMKII_StatePiston_Idle(void) { EggPistonsMKII_CheckPlayerCollisions_Piston(); }

void EggPistonsMKII_StatePiston_Shaking(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->position.y = (RSDK.Rand(-2, 3) << 16) + self->pistonID;
    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--self->timer) {
        self->position.y = self->pistonID;
        self->velocity.y = self->direction == FLIP_NONE ? -0x8000 : 0x8000;

        self->timer = 32;
        self->state = EggPistonsMKII_StatePiston_BeginCrushing;
    }

    if (!EggPistonsMKII->health) {
        self->position.y = self->pistonID;
        self->state      = EggPistonsMKII_StatePiston_Explode;

        EggPistonsMKII_Explode();
    }
}

void EggPistonsMKII_StatePiston_BeginCrushing(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();
    if (!--self->timer) {
        self->timer      = 48;
        self->velocity.y = self->direction == FLIP_NONE ? -0x30000 : 0x30000;
        self->state      = EggPistonsMKII_StatePiston_CrushExtend;
    }

    if (!EggPistonsMKII->health) {
        self->velocity.y = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
        self->timer >>= 1;
        self->state = EggPistonsMKII_StatePiston_Explode;

        EggPistonsMKII_Explode();
    }
}

void EggPistonsMKII_StatePiston_CrushExtend(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--self->timer) {
        self->timer      = 80;
        self->velocity.y = self->direction == FLIP_NONE ? 0x20000 : -0x20000;
        self->state      = EggPistonsMKII_StatePiston_Retract;
    }

    if (!EggPistonsMKII->health) {
        self->state      = EggPistonsMKII_StatePiston_Explode;
        self->velocity.y = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
        self->timer >>= 1;

        EggPistonsMKII_Explode();
    }
}

void EggPistonsMKII_StatePiston_Retract(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--self->timer) {
        self->velocity.y = 0;
        self->parent     = NULL;
        self->state      = EggPistonsMKII_StatePiston_Idle;
    }

    if (!EggPistonsMKII->health || (!EggPistonsMKII->isPhase2 && EggPistonsMKII->health == 4)) {
        self->state      = EggPistonsMKII_StatePiston_Explode;
        self->velocity.y = self->direction == FLIP_NONE ? 0x10000 : -0x10000;

        EggPistonsMKII_Explode();
    }
}

void EggPistonsMKII_StatePiston_Explode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if ((self->direction || self->position.y < self->pistonID) && (self->direction != FLIP_Y || self->position.y > self->pistonID)) {
        EggPistonsMKII_CheckPlayerCollisions_Piston();

        if (!EggPistonsMKII->health || (!EggPistonsMKII->isPhase2 && EggPistonsMKII->health == 4)) {
            EggPistonsMKII_Explode();
        }
    }
    else {
        self->position.y = self->pistonID;
        self->velocity.y = 0;
        self->parent     = NULL;
        self->state      = EggPistonsMKII_StatePiston_Idle;
    }
}

void EggPistonsMKII_StateOrbGenerator_Idle(void) {}

void EggPistonsMKII_StateOrbGenerator_Warning(void)
{
    RSDK_THIS(EggPistonsMKII);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if ((self->timer & 7) == 4)
        RSDK.PlaySfx(EggPistonsMKII->sfxElectrify, false, 255);

    if (!--self->timer) {
        self->mainAnimator.frameID = 0;
        self->state                = EggPistonsMKII_StateOrbGenerator_Idle;
    }
}

void EggPistonsMKII_StateOrb_MoveToTargetPos(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (self->alpha < 0xC0)
        self->alpha += 0x10;

    self->position.x += self->velocity.x;

    if (self->position.x <= self->pistonID) {
        self->timer = 180;
        self->state = EggPistonsMKII_StateOrb_Charging;
    }

    RSDK.ProcessAnimation(&self->mainAnimator);
}

void EggPistonsMKII_StateOrb_Charging(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (!--self->timer) {
        self->alpha      = 0x80;
        self->velocity.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x - self->position.x) >> 7;
        self->velocity.y = 0x14000;
        self->timer      = 133;

        RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 3, &self->altAnimator, true, 0);
        self->state = EggPistonsMKII_StateOrb_Attacking;
        self->mainAnimator.speed *= 2;
    }

    RSDK.ProcessAnimation(&self->mainAnimator);
}

void EggPistonsMKII_StateOrb_Attacking(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->velocity.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x - self->position.x) >> 7;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!--self->timer)
        destroyEntity(self);

    RSDK.ProcessAnimation(&self->mainAnimator);

    EggPistonsMKII_CheckPlayerCollisions_Ball();
}

void EggPistonsMKII_StateAlarm_Active(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII->alarmTimer = (abs(RSDK.Sin256(2 * EggPistonsMKII->alarmAngle)) >> 1) + 32;
    ++EggPistonsMKII->alarmAngle;

    if (!EggPistonsMKII->health && !EggPistonsMKII->isPhase2 && EggPistonsMKII->alarmTimer == 32)
        self->state = EggPistonsMKII_StateAlarm_Destroyed;
}

void EggPistonsMKII_StateAlarm_Destroyed(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (!--EggPistonsMKII->alarmTimer) {
        for (int32 i = 0; i < 2; ++i) {
            int32 slot                    = RSDK.GetEntitySlot(EggPistonsMKII->controller);
            EntityEggPistonsMKII *barrier = RSDK_GET_ENTITY(slot + 14 + i, EggPistonsMKII);

            for (int32 d = 0; d < 4; ++d) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, barrier->position.x, barrier->position.y);
                RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &debris->animator, true, i + 1);
                debris->gravityStrength = 0x3800;
                debris->velocity.x      = ((i & 1) << 17 >> (i >> 1)) - 0x10000;
                debris->velocity.y      = 0x18000 * ((i >> 1) - 2);
                debris->drawGroup       = Zone->objectDrawGroup[1];
                debris->updateRange.x   = 0x400000;
                debris->updateRange.y   = 0x400000;
            }

            destroyEntity(barrier);
        }

        Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
        Zone->playerBoundActiveR[0] = false;

        destroyEntity(self);
    }
}

void EggPistonsMKII_StateBarrier_Solid(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->position.y += self->velocity.y;
    if (--self->timer <= 0) {
        self->state = EggPistonsMKII_StateBarrier_Explode;

        if (self->velocity.y == 0x80000) {
            RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->shakePos.y = 4;
            self->velocity.y                                  = 0;
        }
        else if (self->velocity.y == 0x20000) {
            self->velocity.y = -0x20000;
            self->timer      = 32;
            self->state      = EggPistonsMKII_StateBarrier_Solid;
        }
    }

    EggPistonsMKII_CheckPlayerCollisions_Solid();
}

void EggPistonsMKII_StateBarrier_Explode(void)
{
    EggPistonsMKII_CheckPlayerCollisions_Solid();

    if (!EggPistonsMKII->health)
        EggPistonsMKII_Explode();
}

#if GAME_INCLUDE_EDITOR
void EggPistonsMKII_EditorDraw(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    switch (self->type) {
        case EGGPISTON_PISTON: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 0, &self->mainAnimator, true, 0); break;
        case EGGPISTON_CONTROL: RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->mainAnimator, true, 0); break;
        case EGGPISTON_EMITTER: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 1, &self->mainAnimator, true, 0); break;
        case EGGPISTON_BARRIER: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &self->mainAnimator, true, 0); break;
        case EGGPISTON_PLASMABALL: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 2, &self->mainAnimator, true, 0); break;
        case EGGPISTON_ALARM: RSDK.SetSpriteAnimation(-1, 0, &self->mainAnimator, true, 0); break;
        default: break;
    }

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    if (self->type == EGGPISTON_CONTROL && showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 71, 1 | 2 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void EggPistonsMKII_EditorLoad(void)
{
    EggPistonsMKII->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/EggPistonsMKII.bin", SCOPE_STAGE);
    EggPistonsMKII->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ1.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(EggPistonsMKII, type);
    RSDK_ENUM_VAR("Piston", EGGPISTON_PISTON);
    RSDK_ENUM_VAR("Control", EGGPISTON_CONTROL);
    RSDK_ENUM_VAR("Emitter", EGGPISTON_EMITTER);
    RSDK_ENUM_VAR("Barrier", EGGPISTON_BARRIER);
}
#endif

void EggPistonsMKII_Serialize(void)
{
    RSDK_EDITABLE_VAR(EggPistonsMKII, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(EggPistonsMKII, VAR_UINT8, type);
}
