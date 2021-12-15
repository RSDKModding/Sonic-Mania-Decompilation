// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EggPistonsMKII Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator1, NULL, false);
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
                self->type = voidToInt(data);

            switch (self->type) {
                case EGGPISTON_PISTON:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 0, &self->animator1, true, 0);
                    self->updateRange.x                                  = 0xA00000;
                    self->updateRange.y                                  = 0xEC0000;
                    self->hitbox.left                                    = -32;
                    self->hitbox.top                                     = -96;
                    self->hitbox.right                                   = 32;
                    self->hitbox.bottom                                  = 60;
                    self->state                                          = EggPistonsMKII_StatePiston_Unknown1;
                    self->visible                                        = true;
                    self->drawOrder                                      = Zone->fgLayerHigh - 1;
                    self->pistonID                                       = self->position.y;
                    EggPistonsMKII->pistons[EggPistonsMKII->pistonCount++] = (Entity *)self;
                    break;
                case EGGPISTON_CONTROL:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->animator1, true, 0);
                    self->updateRange.x      = 0x800000;
                    self->updateRange.y      = 0x800000;
                    self->hitbox.left        = -33;
                    self->hitbox.top         = -37;
                    self->hitbox.right       = 33;
                    self->hitbox.bottom      = 6;
                    self->state              = EggPistonsMKII_State_SetupArena;
                    self->visible            = false;
                    self->drawOrder          = Zone->drawOrderLow;
                    EggPistonsMKII->ControllerInfo = (Entity *)self;
                    break;
                case EGGPISTON_EMITTER:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 1, &self->animator1, true, 0);
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;
                    self->state         = EggPistonsMKII_StatePiston_Unknown1;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderLow + 1;
                    break;
                case EGGPISTON_BARRIER:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &self->animator1, true, 0);
                    self->hitbox.left   = -32;
                    self->hitbox.top    = -32;
                    self->hitbox.right  = 32;
                    self->hitbox.bottom = 32;
                    self->state         = EggPistonsMKII_StateOrbGenerator_Idle;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderLow;
                    break;
                case EGGPISTON_PLASMABALL:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 2, &self->animator1, true, 0);
                    self->hitbox.left   = -12;
                    self->hitbox.top    = -12;
                    self->hitbox.right  = 12;
                    self->hitbox.bottom = 12;
                    self->state         = EggPistonsMKII_StateOrb_Unknown1;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderLow;
                    break;
                case EGGPISTON_ALARM:
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->state         = EggPistonsMKII_StateAlarm_Active;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderHigh;
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
    EggPistonsMKII->aniFrames          = RSDK.LoadSpriteAnimation("MMZ/EggPistonsMKII.bin", SCOPE_STAGE);
    EggPistonsMKII->eggmanFrames       = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ1.bin", SCOPE_STAGE);
    EggPistonsMKII->pistonCount        = 0;
    EggPistonsMKII->invincibilityTimer = 0;
    EggPistonsMKII->field_2D           = 0;
    EggPistonsMKII->sfxHit             = RSDK.GetSfx("Stage/BossHit.wav");
    EggPistonsMKII->sfxWall            = RSDK.GetSfx("Stage/Wall.wav");
    EggPistonsMKII->sfxElectrify       = RSDK.GetSfx("Stage/Electrify.wav");
    EggPistonsMKII->sfxExplosion       = RSDK.GetSfx("Stage/Explosion2.wav");
    EggPistonsMKII->sfxAlarm           = RSDK.GetSfx("MMZ/Alarm.wav");
    RSDK.StopSfx(EggPistonsMKII->sfxAlarm);
}

void EggPistonsMKII_CheckPlayerCollisions_Piston(void)
{
    RSDK_THIS(EggPistonsMKII);
    EntityEggPistonsMKII *parent = (EntityEggPistonsMKII *)self->parent;

    foreach_active(Player, player)
    {
        int side = Player_CheckCollisionBox(player, self, &self->hitbox);
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
        if (self->direction)
            parent->position.y += 0xE0000;
        else
            parent->position.y -= 0xA0000;
    }
}

void EggPistonsMKII_CheckPlayerCollisions(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (!EggPistonsMKII->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox)
            && EggPistonsMKII_CheckPlayerAttacking(player)) {
            EggPistonsMKII->invincibilityTimer = 120;
            if (EggPistonsMKII->health > 0)
                EggPistonsMKII->health--;

            if (EggPistonsMKII->health) {
                RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 1, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 4, &self->animator2, true, 0);
                RSDK.PlaySfx(EggPistonsMKII->sfxHit, false, 255);
            }
            else {
                RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 2, &self->animator1, true, 0);
                SceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                self->animator1.frameID = 1;
                RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
                self->state = EggPistonsMKII_State_Finish;
            }
        }
    }
}

void EggPistonsMKII_CheckPlayerCollisions_Ball(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
        }
    }
}

void EggPistonsMKII_HandlePlayerCollisions(void)
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

EntityEggPistonsMKII *EggPistonsMKII_GetNextPiston(void)
{
    RSDK_THIS(EggPistonsMKII);

    EntityEggPistonsMKII *piston = NULL;
    int pistonID                 = 0;

    do {
        do {
            pistonID = RSDK.Rand(0, 5);
            piston   = (EntityEggPistonsMKII *)EggPistonsMKII->pistons[pistonID];
        } while (piston->state != EggPistonsMKII_StatePiston_Unknown1);
    } while (pistonID == EggPistonsMKII->pistonID - 1 || pistonID == EggPistonsMKII->pistonID + 1);

    EggPistonsMKII->pistonID = pistonID;

    self->visible = true;
    piston->timer   = 15;
    piston->state   = EggPistonsMKII_StatePiston_Unknown2;
    return piston;
}

void EggPistonsMKII_SpawnElecBall(void)
{
    EntityEggPistonsMKII *spawner = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, EggPistonsMKII);
    spawner->state                = EggPistonsMKII_StateOrbGenerator_Warning;
    spawner->timer                = 330;
    int offset                    = 0;

    for (int i = 0; i < 4; ++i) {
        EntityEggPistonsMKII *child = CREATE_ENTITY(EggPistonsMKII, intToVoid(EGGPISTON_PLASMABALL), spawner->position.x, spawner->position.y);
        child->pistonID             = offset + spawner->position.x + ((RSDK.Rand(0, 48) - 48) << 16);
        child->inkEffect            = INK_ADD;
        child->velocity.x           = (child->pistonID - child->position.x) >> 4;
        child->alpha                = 0;
        offset -= 0x570000;
    }
}

bool32 EggPistonsMKII_CheckPlayerAttacking(void *p)
{
    RSDK_THIS(EggPistonsMKII);
    EntityPlayer *player = (EntityPlayer *)p;

    if (Player_CheckAttacking(player, NULL)) {
        if (player->position.x >= self->position.x) {
            player->velocity.x = 0x30000;
            player->groundVel  = 0x30000;
        }
        else {
            player->velocity.x = -0x30000;
            player->groundVel  = -0x30000;
        }
        if (player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_FLY) {
            RSDK.SetSpriteAnimation(Player->knuxSpriteIndex, ANI_FLYTIRED, &player->animator, true, 0);
            player->state = Player_State_KnuxGlideDrop;
        }
        return true;
    }
    return false;
}

void EggPistonsMKII_State_SetupArena(void)
{
    RSDK_THIS(EggPistonsMKII);

    for (int i = 0; i < 6; ++i) {
        EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot + 8 + i, CollapsingPlatform);
        if (platform->objectID == CollapsingPlatform->objectID)
            platform->active = ACTIVE_NEVER;
    }

    if (++self->timer >= 2) {
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->cameraBoundsB[0]     = (self->position.y >> 16) + 71;
        Zone->cameraBoundsT[0]     = (Zone->cameraBoundsB[0] - ScreenInfo->height);
        self->active              = ACTIVE_NORMAL;
        self->state               = EggPistonsMKII_State_EnterBoss;
    }
}

void EggPistonsMKII_State_EnterBoss(void)
{
    RSDK_THIS(EggPistonsMKII);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]     = ScreenInfo->position.x;

    int pos = RETRO_USE_PLUS ? (self->position.x - 0xC00000) : self->position.x;
    if (player1->position.x > pos) {
#if RETRO_USE_PLUS
        if (player1->onGround)
            player1->state = Player_State_Ground;
        player1->stateInput = 0;
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
            Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);

            EggPistonsMKII->health = 8;
            self->timer          = 142;
            for (int i = 0; i < 2; ++i) {
                EntityEggPistonsMKII *barrier = RSDK_GET_ENTITY(SceneInfo->entitySlot + 14 + i, EggPistonsMKII);
                barrier->velocity.y           = -0x20000;
                barrier->timer                = 64;
                barrier->state                = EggPistonsMKII_State_Unknown1;
            }

            player1->right = false;
            player1->left  = true;
            self->state  = EggPistonsMKII_State_PistonReveal;
#if RETRO_USE_PLUS
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
        player1->stateInput = Player_ProcessP1Input;

        for (int i = 0; i < 6; ++i) {
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(SceneInfo->entitySlot + 8 + i, CollapsingPlatform);
            if (platform->objectID == CollapsingPlatform->objectID) {
                platform->active        = ACTIVE_NORMAL;
                platform->collapseDelay = 8;
                platform->playerPos.x   = self->position.x;
            }
        }

        for (int i = 0; i < 2; ++i) {
            EntityEggPistonsMKII *piston = RSDK_GET_ENTITY(SceneInfo->entitySlot + 14 + i, EggPistonsMKII);
            piston->velocity.y           = 0x80000;
            piston->timer                = 8;
            piston->state                = EggPistonsMKII_State_Unknown1;
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
            EggPistonsMKII_GetNextPiston()->parent = (Entity *)self;
            RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
            self->timer = 165;
            RSDK.PlaySfx(EggPistonsMKII->sfxWall, false, 255);
        }
        self->pistonID ^= 1;
    }

    if (EggPistonsMKII->invincibilityTimer)
        EggPistonsMKII->invincibilityTimer--;
    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= self->position.x;
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    if (self->visible)
        EggPistonsMKII_CheckPlayerCollisions();
}

void EggPistonsMKII_State_StartPinchMode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (--self->timer <= 0) {
        CREATE_ENTITY(EggPistonsMKII, intToVoid(EGGPISTON_ALARM), self->position.x, self->position.y);

        EntityEggPistonsMKII *orbSpawner = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, EggPistonsMKII);
        RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), orbSpawner->position.x, orbSpawner->position.y)->drawOrder = Zone->drawOrderHigh;
        EggPistonsMKII->field_2D                                                                                        = 1;
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
            piston->parent = (Entity *)self;
            RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        }
        self->pistonID = (self->pistonID + 1) & 3;
        RSDK.PlaySfx(EggPistonsMKII->sfxWall, false, 255);
        self->timer = 64;
    }

    if (EggPistonsMKII->invincibilityTimer)
        EggPistonsMKII->invincibilityTimer--;
    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= self->position.x;
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    if (self->visible)
        EggPistonsMKII_CheckPlayerCollisions();

    if (!EggPistonsMKII->health)
        self->state = EggPistonsMKII_State_Destroyed;
}

void EggPistonsMKII_State_Destroyed(void)
{
    RSDK_THIS(EggPistonsMKII);

    int id = 0;
    for (int i = 0; i < 5; ++i) {
        EntityEggPistonsMKII *piston = (EntityEggPistonsMKII *)EggPistonsMKII->pistons[i];

        if (piston->state == EggPistonsMKII_StatePiston_Unknown1) {
            ++id;
        }
    }

    if (++id >= 5) {
        EggPistonsMKII->field_2D = 0;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        RSDK.StopSfx(EggPistonsMKII->sfxAlarm);
        self->state = EggPistonsMKII_State_Finish;
    }
}

void EggPistonsMKII_State_Finish(void) {}

void EggPistonsMKII_StatePiston_Unknown1(void) { EggPistonsMKII_CheckPlayerCollisions_Piston(); }

void EggPistonsMKII_StatePiston_Unknown2(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->position.y = (RSDK.Rand(-2, 3) << 16) + self->pistonID;
    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--self->timer) {
        self->position.y = self->pistonID;
        if (self->direction == FLIP_NONE)
            self->velocity.y = -0x8000;
        else
            self->velocity.y = 0x8000;

        self->timer = 32;
        self->state = EggPistonsMKII_StatePiston_Unknown3;
    }

    if (!EggPistonsMKII->health) {
        self->position.y = self->pistonID;
        self->state      = EggPistonsMKII_StatePiston_Explode;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = self->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = self->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Unknown3(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();
    if (!--self->timer) {
        self->timer = 48;
        if (self->direction == FLIP_NONE)
            self->velocity.y = -0x30000;
        else
            self->velocity.y = 0x30000;
        self->state = EggPistonsMKII_StatePiston_Unknown4;
    }

    if (!EggPistonsMKII->health) {
        if (self->direction == FLIP_NONE)
            self->velocity.y = 0x10000;
        else
            self->velocity.y = -0x10000;
        self->timer >>= 1;
        self->state = EggPistonsMKII_StatePiston_Explode;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = self->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = self->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Unknown4(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--self->timer) {
        self->timer = 80;
        if (self->direction == FLIP_NONE)
            self->velocity.y = 0x20000;
        else
            self->velocity.y = -0x20000;
        self->state = EggPistonsMKII_StatePiston_Unknown5;
    }

    if (!EggPistonsMKII->health) {
        self->state = EggPistonsMKII_StatePiston_Explode;
        if (self->direction == FLIP_NONE)
            self->velocity.y = 0x10000;
        else
            self->velocity.y = -0x10000;
        self->timer >>= 1;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = self->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = self->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Unknown5(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--self->timer) {
        self->velocity.y = 0;
        self->parent     = NULL;
        self->state      = EggPistonsMKII_StatePiston_Unknown1;
    }

    if (!EggPistonsMKII->health || (EggPistonsMKII->field_2D == 0 && EggPistonsMKII->health == 4)) {
        self->state = EggPistonsMKII_StatePiston_Explode;
        if (self->direction == FLIP_NONE)
            self->velocity.y = 0x10000;
        else
            self->velocity.y = -0x10000;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = self->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = self->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Explode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if ((self->direction || self->position.y < self->pistonID) && (self->direction != FLIP_Y || self->position.y > self->pistonID)) {
        EggPistonsMKII_CheckPlayerCollisions_Piston();
        if (!EggPistonsMKII->health || (((EggPistonsMKII->field_2D == 0) & (EggPistonsMKII->health == 4)) != 0)) {
            int interval = 4 * (EggPistonsMKII->health != 0) + 3;
            if (!(Zone->timer % interval)) {
                RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
                if (Zone->timer & 4) {
                    int x = self->position.x + (RSDK.Rand(-24, 24) << 16);
                    int y = self->position.y + (RSDK.Rand(-48, 48) << 16);
                    CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
                }
            }
        }
    }
    else {
        self->position.y = self->pistonID;
        self->velocity.y = 0;
        self->parent     = NULL;
        self->state      = EggPistonsMKII_StatePiston_Unknown1;
    }
}

void EggPistonsMKII_StateOrbGenerator_Idle(void) {}

void EggPistonsMKII_StateOrbGenerator_Warning(void)
{
    RSDK_THIS(EggPistonsMKII);

    RSDK.ProcessAnimation(&self->animator1);

    if ((self->timer & 7) == 4)
        RSDK.PlaySfx(EggPistonsMKII->sfxElectrify, false, 255);

    if (!--self->timer) {
        self->animator1.frameID = 0;
        self->state             = EggPistonsMKII_StateOrbGenerator_Idle;
    }
}

void EggPistonsMKII_StateOrb_Unknown1(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (self->alpha < 0xC0)
        self->alpha += 0x10;
    self->position.x += self->velocity.x;
    if (self->position.x <= self->pistonID) {
        self->timer = 180;
        self->state = EggPistonsMKII_StateOrb_Unknown2;
    }
    RSDK.ProcessAnimation(&self->animator1);
}

void EggPistonsMKII_StateOrb_Unknown2(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (!--self->timer) {
        self->alpha      = 128;
        self->velocity.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x - self->position.x) >> 7;
        self->velocity.y = 0x14000;
        self->timer      = 133;
        RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 3, &self->animator2, true, 0);
        self->state = EggPistonsMKII_StateOrb_Unknown3;
        self->animator1.speed *= 2;
    }
    RSDK.ProcessAnimation(&self->animator1);
}

void EggPistonsMKII_StateOrb_Unknown3(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->velocity.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x - self->position.x) >> 7;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (!--self->timer)
        destroyEntity(self);
    RSDK.ProcessAnimation(&self->animator1);
    EggPistonsMKII_CheckPlayerCollisions_Ball();
}

void EggPistonsMKII_StateAlarm_Active(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII->alarmTimer = (abs(RSDK.Sin256(2 * EggPistonsMKII->alarmAngle)) >> 1) + 32;
    ++EggPistonsMKII->alarmAngle;

    if (!EggPistonsMKII->health && !EggPistonsMKII->field_2D && EggPistonsMKII->alarmTimer == 32)
        self->state = EggPistonsMKII_StateAlarm_Destroyed;
}

void EggPistonsMKII_StateAlarm_Destroyed(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (!--EggPistonsMKII->alarmTimer) {

        for (int i = 0; i < 2; ++i) {
            int slot                      = RSDK.GetEntityID(EggPistonsMKII->ControllerInfo);
            EntityEggPistonsMKII *barrier = RSDK_GET_ENTITY(slot + 14 + i, EggPistonsMKII);

            for (int d = 0; d < 4; ++d) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, barrier->position.x, barrier->position.y);
                RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &debris->animator, true, i + 1);
                debris->gravity       = 0x3800;
                debris->velocity.x    = ((i & 1) << 17 >> (i >> 1)) - 0x10000;
                debris->velocity.y    = 0x18000 * ((i >> 1) - 2);
                debris->drawOrder     = Zone->drawOrderHigh;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }

            destroyEntity(barrier);
        }

        Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
        Zone->playerBoundActiveR[0] = false;
        destroyEntity(self);
    }
}

void EggPistonsMKII_State_Explode(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_HandlePlayerCollisions();
    if (!EggPistonsMKII->health) {
        if (!(Zone->timer % 3)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = self->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = self->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_State_Unknown1(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->position.y += self->velocity.y;
    if (--self->timer <= 0) {
        self->state = EggPistonsMKII_State_Explode;
        if (self->velocity.y == 0x80000) {
            RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->shakePos.y = 4;
            self->velocity.y                                = 0;
        }
        else if (self->velocity.y == 0x20000) {
            self->velocity.y = -0x20000;
            self->timer      = 32;
            self->state      = EggPistonsMKII_State_Unknown1;
        }
    }
    EggPistonsMKII_HandlePlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void EggPistonsMKII_EditorDraw(void)
{
    RSDK_THIS(EggPistonsMKII);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    switch (self->type) {
        case EGGPISTON_PISTON: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 0, &self->animator1, true, 0); break;
        case EGGPISTON_CONTROL: RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &self->animator1, true, 0); break;
        case EGGPISTON_EMITTER: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 1, &self->animator1, true, 0); break;
        case EGGPISTON_BARRIER: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &self->animator1, true, 0); break;
        case EGGPISTON_PLASMABALL: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 2, &self->animator1, true, 0); break;
        case EGGPISTON_ALARM: RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator1, true, 0); break;
        default: break;
    }

    RSDK.DrawSprite(&self->animator1, NULL, false);
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
