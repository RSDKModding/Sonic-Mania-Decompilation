// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MegaOctus Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMegaOctus *MegaOctus;

void MegaOctus_Update(void)
{
    RSDK_THIS(MegaOctus);

    StateMachine_Run(self->state);
}

void MegaOctus_LateUpdate(void) {}

void MegaOctus_StaticUpdate(void)
{
    foreach_active(MegaOctus, boss)
    {
        if (boss->type == MEGAOCTUS_ARM)
            RSDK.AddDrawListRef(Zone->objectDrawGroup[0], RSDK.GetEntitySlot(boss));
    }
}

void MegaOctus_Draw(void)
{
    RSDK_THIS(MegaOctus);

    StateMachine_Run(self->stateDraw);
}

void MegaOctus_Create(void *data)
{
    RSDK_THIS(MegaOctus);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active = ACTIVE_BOUNDS;
            if (data)
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case MEGAOCTUS_BODY:
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 0, &self->animator, true, 0);
                    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 1, &MegaOctus->eggmanAnimator, true, 0);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 1, &MegaOctus->noseAnimator, true, 5);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 2, &MegaOctus->boltsAnimator, true, 5);
                    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 0, &MegaOctus->hatchBaseAnimator, true, 0);
                    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 1, &MegaOctus->hatchOpenAnimator, true, 0);

                    self->hitbox.left   = -33;
                    self->hitbox.top    = -37;
                    self->hitbox.right  = 33;
                    self->hitbox.bottom = 6;

                    self->origin.x = self->position.x + 0x800000;
                    self->origin.y = self->position.y;

                    self->updateRange.x   = 0x800000;
                    self->updateRange.y   = 0x800000;
                    MegaOctus->bossEntity = self;
                    MegaOctus->turnPos    = 0;
                    self->state           = MegaOctus_State_SetupBounds;
                    self->stateDraw       = MegaOctus_Draw_Body;
                    break;

                case MEGAOCTUS_UNUSED1: break;

                case MEGAOCTUS_HARPOON:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;

                    self->origin        = self->position;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->angle         = 0;
                    self->rotation      = 0;

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &self->animator, true, 1);
                    break;

                case MEGAOCTUS_CANNON:
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->drawGroup     = Zone->objectDrawGroup[0] + 1;

                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;

                    self->targetPos        = self->position.y - 0x700000;
                    self->origin           = self->position;
                    self->lastAttackHeight = 0x100;
                    self->shotCount        = 3;

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &self->animator, true, 1);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 4, &self->altAnimator, true, 3);

                    self->velocity.y = -0x10000;
                    self->state      = MegaOctus_StateCannon_RiseUp;
                    self->stateDraw  = MegaOctus_Draw_Cannon;
                    break;

                case MEGAOCTUS_ORB:
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->drawGroup     = Zone->objectDrawGroup[0] + 1;

                    self->hitbox.left   = -16;
                    self->hitbox.top    = -16;
                    self->hitbox.right  = 16;
                    self->hitbox.bottom = 16;

                    self->targetPos = self->position.y - 0x1000000;
                    self->origin    = self->position;

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &self->animator, true, 1);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 5, &self->altAnimator, true, 0);

                    self->velocity.y = -0x18000;
                    self->state      = MegaOctus_StateOrb_Wait;
                    self->stateDraw  = MegaOctus_Draw_Orb;
                    break;

                case MEGAOCTUS_ARM:
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->updateRange.x = 0x800000;
                    self->drawGroup     = Zone->objectDrawGroup[0] + 1;

                    self->hitbox.left   = -8;
                    self->hitbox.top    = -8;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 8;

                    self->origin        = self->position;
                    self->updateRange.y = 0x800000;
                    self->angle         = 0;
                    self->rotation      = 0;

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &self->animator, true, 1);

                    self->state     = MegaOctus_StateArm_WrapAroundPlatform;
                    self->stateDraw = MegaOctus_Draw_Arm_WrapAroundPlatformBase;
                    break;

                case MEGAOCTUS_LASER:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    self->hitbox.left   = -16;
                    self->hitbox.top    = -1;
                    self->hitbox.right  = 16;
                    self->hitbox.bottom = 1;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 7, &self->animator, true, 0);

                    self->state     = MegaOctus_State_Laser;
                    self->stateDraw = MegaOctus_Draw_Laser;
                    break;

                case MEGAOCTUS_ORBSHOT:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    self->hitbox.left   = -3;
                    self->hitbox.top    = -3;
                    self->hitbox.right  = 3;
                    self->hitbox.bottom = 3;

                    self->updateRange.x = 0x400000;
                    self->updateRange.y = 0x400000;

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 6, &self->animator, true, 0);

                    self->state     = MegaOctus_State_Shot;
                    self->stateDraw = MegaOctus_Draw_OrbShot;
                    break;

                case MEGAOCTUS_UNUSED8: break;

                case MEGAOCTUS_LASERFIRE:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    self->hitbox.left   = -8;
                    self->hitbox.top    = -4;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 0;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 8, &self->animator, true, 0);

                    self->state     = MegaOctus_State_LaserFire;
                    self->stateDraw = MegaOctus_Draw_Laser;
                    break;

                default: break;
            }
        }
        else {
            MegaOctus->bossEntity = NULL;
            destroyEntity(self);
        }
    }
}

void MegaOctus_StageLoad(void)
{
    MegaOctus->active     = ACTIVE_ALWAYS;
    MegaOctus->bossEntity = NULL;

    MegaOctus->aniFrames    = RSDK.LoadSpriteAnimation("OOZ/MegaOctus.bin", SCOPE_STAGE);
    MegaOctus->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanOOZ.bin", SCOPE_STAGE);
    MegaOctus->hatchFrames  = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE);

    MegaOctus->hitbox.left   = -16;
    MegaOctus->hitbox.top    = 22;
    MegaOctus->hitbox.right  = 16;
    MegaOctus->hitbox.bottom = 30;

    MegaOctus->spawnHarpoon = false;
    MegaOctus->orbHealth[0] = 4;
    MegaOctus->orbHealth[1] = 4;
    MegaOctus->defeated     = false;

    MegaOctus->sfxBossHit     = RSDK.GetSfx("Stage/BossHit.wav");
    MegaOctus->sfxExplosion   = RSDK.GetSfx("Stage/Explosion2.wav");
    MegaOctus->sfxLaser       = RSDK.GetSfx("OOZ/OOZLaser.wav");
    MegaOctus->sfxBullet      = RSDK.GetSfx("OOZ/OOZBullet.wav");
    MegaOctus->sfxHarpoon     = RSDK.GetSfx("OOZ/Harpoon.wav");
    MegaOctus->sfxSurface     = RSDK.GetSfx("OOZ/OOZSurface.wav");
    MegaOctus->sfxLaserSplash = RSDK.GetSfx("OOZ/LaserSplash.wav");
}

void MegaOctus_CheckPlayerCollisions_Body(void)
{
    RSDK_THIS(MegaOctus);

    if (self->invincibilityTimer)
        self->invincibilityTimer--;

    foreach_active(Player, player)
    {
        int32 playerRadius = 0x100000;

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
        if (shield->classID == Shield->classID && shield->state == Shield_State_Insta)
            playerRadius = 0x160000;

        if (RSDK.CheckObjectCollisionTouchCircle(self, 0x300000, player, playerRadius)) {
            int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

            player->velocity.x += 80 * RSDK.Cos256(angle);
            if (self->invincibilityTimer || !Player_CheckBossHit(player, self)) {
                player->velocity.y -= 80 * abs(RSDK.Sin256(angle));
            }
            else {
                MegaOctus_Hit();
            }
        }
    }
}

void MegaOctus_HandleEggmanAnim(void)
{
    RSDK_THIS(MegaOctus);

    switch (MegaOctus->eggmanAnimator.animationID) {
        case 0:
        case 1:
            if (MegaOctus->eggmanAnimator.frameID >= MegaOctus->eggmanAnimator.frameCount - 1) {
                if (self->invincibilityTimer) {
                    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 3, &MegaOctus->eggmanAnimator, true, 0);
                }
                else {
                    bool32 laughing = false;
                    foreach_active(Player, player)
                    {
                        if (player->state == Player_State_Hurt || player->state == Player_State_Death)
                            laughing = true;
                    }

                    if (laughing)
                        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 4, &MegaOctus->eggmanAnimator, true, 0);
                }
            }
            break;

        case 2:
        case 4:
            if (self->invincibilityTimer) {
                RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 3, &MegaOctus->eggmanAnimator, true, 0);
            }
            else {
                bool32 laughing = false;
                foreach_active(Player, player)
                {
                    if (player->state == Player_State_Hurt || player->state == Player_State_Death)
                        laughing = true;
                }

                if (MegaOctus->eggmanAnimator.frameID >= MegaOctus->eggmanAnimator.frameCount - 1) {
                    if (!laughing) {
                        uint8 anim = self->state == MegaOctus_State_CloseHatch ? 2 : 0;
                        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, anim, &MegaOctus->eggmanAnimator, true, 0);
                    }
                    else {
                        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 4, &MegaOctus->eggmanAnimator, true, 8);
                    }
                }
            }
            break;

        case 3:
            if (!self->invincibilityTimer) {
                if (MegaOctus->eggmanAnimator.frameID >= MegaOctus->eggmanAnimator.frameCount - 1) {
                    uint8 anim = self->state == MegaOctus_State_CloseHatch ? 2 : 0;
                    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, anim, &MegaOctus->eggmanAnimator, true, 0);
                }
            }
            break;

        default: break;
    }
}

void MegaOctus_Hit(void)
{
    RSDK_THIS(MegaOctus);

    if (self->health)
        self->health--;

    if (self->health) {
        RSDK.PlaySfx(MegaOctus->sfxBossHit, false, 255);
        self->invincibilityTimer = 30;
        MegaOctus->spawnHarpoon  = true;
    }
    else {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);
        self->invincibilityTimer = 120;
        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 5, &MegaOctus->eggmanAnimator, true, 0);
        MegaOctus->defeated = true;
        self->state         = MegaOctus_State_Destroyed;
    }
}

void MegaOctus_Explode(void)
{
    RSDK_THIS(MegaOctus);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x                    = self->position.x + (RSDK.Rand(-48, 48) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-48, 48) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void MegaOctus_HandleDirectionChange(void)
{
    RSDK_THIS(MegaOctus);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (abs(player1->position.x - self->position.x) < 0x200000) {
        if (self->orbID > 10)
            self->orbID--;
        else if (self->orbID < 10)
            self->orbID++;
    }
    else {
        if (player1->position.x < self->position.x) {
            if (self->orbID)
                self->orbID--;
        }
        else {
            if (self->orbID < 20)
                self->orbID++;
        }
    }

    self->direction                  = self->orbID < 10;
    MegaOctus->noseAnimator.frameID  = self->orbID >> 1;
    MegaOctus->boltsAnimator.frameID = self->orbID >> 1;
    MegaOctus->turnPos               = MegaOctus->turnOffsets[MegaOctus->noseAnimator.frameID];
}

void MegaOctus_State_SetupBounds(void)
{
    RSDK_THIS(MegaOctus);

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = false;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + 448;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 96;
        Zone->deathBoundary[0]      = Zone->cameraBoundsB[0] << 16;
#if MANIA_USE_PLUS
        Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - 384;
#endif

        self->position.y += 0xC00000;
        self->active  = ACTIVE_NORMAL;
        self->visible = true;
        self->state   = MegaOctus_State_SetupArena;
    }
}

void MegaOctus_State_SetupArena(void)
{
    RSDK_THIS(MegaOctus);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->origin.x) {
        RSDK.GetTileLayer(Zone->fgLayer[0])->drawGroup[0] = 2;
        Zone->playerBoundActiveL[0]                       = true;
        Zone->cameraBoundsL[0]                            = (self->position.x >> 16) - 192;

        Music_TransitionTrack(TRACK_EGGMAN1, 0.0075);

        OOZSetup->useSmogEffect = false;
        self->velocity.y        = -0x40000;
        self->health            = 8;
        self->timer             = 60;

        EntityMegaOctus *arm = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ARM), self->position.x + 0x800000, self->origin.y + 0x400000);
        arm->direction       = self->direction;
        arm->angle           = 128;

        arm            = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ARM), self->position.x - 0x800000, self->origin.y + 0x400000);
        arm->direction = self->direction;
        arm->angle     = 128;

        self->state = MegaOctus_State_None;
    }
}

void MegaOctus_State_None(void)
{
    // :3
}

void MegaOctus_State_EnterMegaOctus(void)
{
    RSDK_THIS(MegaOctus);

    self->position.y += self->velocity.y;
    self->velocity.y += MANIA_USE_PLUS ? 0x3800 : 0x6000;

    if (self->velocity.y >= 0x10000) {
        self->state      = MegaOctus_State_OpenHatchAndLaugh;
        self->velocity.y = 0;
    }

    MegaOctus_CheckPlayerCollisions_Body();
}

void MegaOctus_State_OpenHatchAndLaugh(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&MegaOctus->hatchOpenAnimator);
    RSDK.ProcessAnimation(&MegaOctus->eggmanAnimator);

    MegaOctus_HandleEggmanAnim();
    MegaOctus_HandleDirectionChange();
    MegaOctus_CheckPlayerCollisions_Body();

    if (--self->timer <= 0) {
        self->timer = 60;

        RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 2, &MegaOctus->hatchOpenAnimator, true, 0);
        if (MegaOctus->eggmanAnimator.animationID < 2)
            RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 2, &MegaOctus->eggmanAnimator, true, 0);

        self->state = MegaOctus_State_CloseHatch;
    }
}

void MegaOctus_State_CloseHatch(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&MegaOctus->hatchOpenAnimator);
    RSDK.ProcessAnimation(&MegaOctus->eggmanAnimator);

    MegaOctus_HandleEggmanAnim();

    MegaOctus->eggmanVelocity += 0x4800;
    MegaOctus->eggmanOffset += MegaOctus->eggmanVelocity;

    MegaOctus_HandleDirectionChange();
    MegaOctus_CheckPlayerCollisions_Body();

    if (--self->timer <= 0)
        self->state = MegaOctus_State_DiveIntoOil;
}

void MegaOctus_State_DiveIntoOil(void)
{
    RSDK_THIS(MegaOctus);

    self->position.y += self->velocity.y;
    self->velocity.y += MANIA_USE_PLUS ? 0x3800 : 0x6000;

    if (self->position.y >= self->origin.y + 0xC00000) {
        self->position.y = self->origin.y + 0xC00000;

        self->state = MegaOctus_State_SpawnWeapons;
        self->timer = MegaOctus->spawnHarpoon ? 480 : 240;
    }

    MegaOctus_CheckPlayerCollisions_Body();
}

void MegaOctus_State_SpawnWeapons(void)
{
    RSDK_THIS(MegaOctus);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= self->origin.x)
        self->position.x = self->origin.x - 0x800000;
    else
        self->position.x = self->origin.x + 0x800000;

    if (--self->timer == 240) {
        MegaOctus->spawnHarpoon  = false;
        EntityMegaOctus *harpoon = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_HARPOON), self->position.x, self->origin.y + 0x300000);
        harpoon->direction       = self->direction;
        RSDK.PlaySfx(MegaOctus->sfxHarpoon, false, 255);

        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= self->position.x) {
            harpoon->origin.x -= 0x800000;
            harpoon->state     = MegaOctus_State_HarpoonLeft;
            harpoon->stateDraw = MegaOctus_Draw_HarpoonLeft;
        }
        else {
            harpoon->origin.x += 0x800000;
            harpoon->state     = MegaOctus_State_HarpoonRight;
            harpoon->stateDraw = MegaOctus_Draw_HarpoonRight;
        }
    }
    else if (self->timer <= 0) {
        self->timer                                                                                                     = 480;
        CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_CANNON), self->position.x, self->origin.y + 0x400000)->direction = self->direction;
        self->state = MegaOctus_State_CannonThenSpawnOrbs;
    }

    MegaOctus_CheckPlayerCollisions_Body();
}

void MegaOctus_State_CannonThenSpawnOrbs(void)
{
    RSDK_THIS(MegaOctus);

    MegaOctus_HandleDirectionChange();

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x <= self->origin.x)
        self->position.x = self->origin.x - 0x800000;
    else
        self->position.x = self->origin.x + 0x800000;

    MegaOctus_CheckPlayerCollisions_Body();

    if (--self->timer <= 0) {
        EntityMegaOctus *arm = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ARM), self->position.x + 0x800000, self->origin.y + 0x400000);
        arm->direction       = self->direction;
        arm->angle           = 128;

        arm            = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ARM), self->position.x - 0x800000, self->origin.y + 0x400000);
        arm->direction = self->direction;
        arm->angle     = 128;

        if (MegaOctus->orbHealth[0] > 0) {
            EntityMegaOctus *orb = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ORB), self->position.x + 0x380000, self->origin.y + 0x780000);
            orb->direction       = self->direction;
            orb->health          = MegaOctus->orbHealth[0];
            orb->orbID           = 0;
            orb->velocity.x      = self->position.x > self->origin.x ? 0 : 136;
        }

        if (MegaOctus->orbHealth[1] > 0) {
            EntityMegaOctus *orb = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ORB), self->position.x - 0x380000, self->origin.y + 0x780000);
            orb->direction       = self->direction;
            orb->health          = MegaOctus->orbHealth[1];
            orb->orbID           = 1;
            orb->angle           = 0x100;
            orb->velocity.x      = self->position.x > self->origin.x ? -136 : 0;
        }

        self->timer = 120;
        self->state = MegaOctus_State_None;
    }
}

void MegaOctus_State_Destroyed(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&MegaOctus->hatchOpenAnimator);
    RSDK.ProcessAnimation(&MegaOctus->eggmanAnimator);

    if (MegaOctus->eggmanVelocity > 0) {
        MegaOctus->eggmanVelocity += 0x4800;
        MegaOctus->eggmanOffset += MegaOctus->eggmanVelocity;
    }

    MegaOctus_Explode();

    if (--self->invincibilityTimer <= 0) {
        if (!MegaOctus->eggmanVelocity)
            RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 2, &MegaOctus->hatchOpenAnimator, true, 0);

        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->timer = 0;
        self->state = MegaOctus_State_Finish;
    }
}

void MegaOctus_State_Finish(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&MegaOctus->hatchOpenAnimator);
    RSDK.ProcessAnimation(&MegaOctus->eggmanAnimator);

    MegaOctus->eggmanVelocity += 0x4800;
    MegaOctus->eggmanOffset += MegaOctus->eggmanVelocity;

    self->position.y += 0x10000;

    if (!(Zone->timer & 7)) {
        int32 x                    = self->position.x + (RSDK.Rand(-48, 48) << 16);
        int32 y                    = self->position.y + (RSDK.Rand(-48, 48) << 16);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSSPUFF), x, y);
        explosion->drawGroup       = self->drawGroup;
    }

    if (++self->timer > 120) {
        bool32 isFinished = true;
        foreach_active(MegaOctus, boss)
        {
            if (boss->type == MEGAOCTUS_ARM) {
                isFinished = false;
                foreach_break;
            }
        }

        if (isFinished) {
            Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
            destroyEntity(self);
        }
    }
}

void MegaOctus_Draw_Body(void)
{
    RSDK_THIS(MegaOctus);

    int32 turnPos = abs(MegaOctus->turnPos) / 96;
    if (self->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    Vector2 drawPos = self->position;
    drawPos.y -= 0x320000;
    RSDK.DrawSprite(&MegaOctus->hatchOpenAnimator, &drawPos, false);

    Vector2 eggmanPos = drawPos;
    eggmanPos.y += MegaOctus->eggmanOffset;
    RSDK.DrawSprite(&MegaOctus->eggmanAnimator, &eggmanPos, false);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.DrawSprite(&MegaOctus->hatchBaseAnimator, &drawPos, false);

    if (MegaOctus->turnPos < 0) {
        self->drawFX |= FX_SCALE;
        self->scale.y = 0x200;
        self->scale.x = 0x200 - (turnPos << 9 >> 15);
    }
    drawPos = self->position;
    drawPos.x += turnPos - 0x10000 + MegaOctus->turnPos + 16 * (turnPos - 0x10000);
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += turnPos * (MegaOctus->turnPos >> 18);
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x -= turnPos * (MegaOctus->turnPos >> 18);
    self->drawFX &= ~FX_SCALE;
    if (MegaOctus->turnPos > 0) {
        self->drawFX |= FX_SCALE;
        self->scale.y = 0x200;
        self->scale.x = 0x200 - (turnPos << 9 >> 15);
    }

    drawPos.x += 34 * (0x10000 - turnPos);
    self->animator.frameID = 1;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += turnPos * (MegaOctus->turnPos >> 18);
    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x -= turnPos * (MegaOctus->turnPos >> 18);
    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&MegaOctus->boltsAnimator, NULL, false);
    RSDK.DrawSprite(&MegaOctus->noseAnimator, NULL, false);

    self->animator.frameID = 6;
    RSDK.DrawSprite(&self->animator, NULL, false);
    RSDK.SetPaletteEntry(0, 128, 0x0000);
}

void MegaOctus_CheckPlayerCollisions_Harpoon(void)
{
    RSDK_THIS(MegaOctus);

    self->position.x = 0x3400 * RSDK.Sin512(self->angle) + self->origin.x;
    self->position.y = 0x3400 * RSDK.Cos512(self->angle) + self->origin.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}

void MegaOctus_State_HarpoonLeft(void)
{
    RSDK_THIS(MegaOctus);

    self->angle = (self->angle + 3) & 0x1FF;
    self->shotCount += 12;

    MegaOctus_CheckPlayerCollisions_Harpoon();

    if (self->angle == 1)
        destroyEntity(self);
}

void MegaOctus_State_HarpoonRight(void)
{
    RSDK_THIS(MegaOctus);

    self->angle = (self->angle - 3) & 0x1FF;
    self->shotCount -= 12;

    MegaOctus_CheckPlayerCollisions_Harpoon();

    if (self->angle == 2)
        destroyEntity(self);
}

void MegaOctus_Draw_HarpoonLeft(void)
{
    RSDK_THIS(MegaOctus);

    self->animator.frameID = 1;
    int32 angle            = (self->angle - 108) & 0x1FF;
    for (int32 i = 0; i < 9; ++i) {
        self->position.x = 0x3400 * RSDK.Sin512(angle) + self->origin.x;
        self->position.y = 0x3400 * RSDK.Cos512(angle) + self->origin.y;
        RSDK.DrawSprite(&self->animator, NULL, false);
        angle = (angle + 12) & 0x1FF;
    }

    self->drawFX |= FX_ROTATE;
    self->direction        = FLIP_NONE;
    self->rotation         = (-0x100 - self->angle) & 0x1FF;
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->drawFX &= ~FX_ROTATE;
}

void MegaOctus_Draw_HarpoonRight(void)
{
    RSDK_THIS(MegaOctus);

    self->animator.frameID = 1;
    int32 angle            = (self->angle + 108) & 0x1FF;
    for (int32 i = 0; i < 9; ++i) {
        self->position.x = 0x3400 * RSDK.Sin512(angle) + self->origin.x;
        self->position.y = 0x3400 * RSDK.Cos512(angle) + self->origin.y;
        RSDK.DrawSprite(&self->animator, NULL, false);
        angle = (angle - 12) & 0x1FF;
    }

    self->drawFX |= FX_ROTATE;
    self->direction        = FLIP_X;
    self->rotation         = (-0x100 - self->angle) & 0x1FF;
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->drawFX &= ~FX_ROTATE;
}

void MegaOctus_CheckPlayerCollisions_Cannon(void)
{
    RSDK_THIS(MegaOctus);

    Vector2 storePos = self->position;

    foreach_active(Player, player)
    {
        self->position.x = (RSDK.Cos512(self->angle) << 10) + storePos.x;
        self->position.y = (RSDK.Sin512(self->angle) << 8) + storePos.y;

        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            self->position = storePos;

#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }

    self->position = storePos;
}

void MegaOctus_StateCannon_RiseUp(void)
{
    RSDK_THIS(MegaOctus);

    self->position.y += self->velocity.y;
    self->angle = (self->angle + 6) & 0x1FF;

    RSDK.ProcessAnimation(&self->altAnimator);

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;
    if (self->position.y <= self->targetPos) {
        self->shotCount        = MANIA_USE_PLUS ? 2 : 3;
        self->timer            = 128;
        self->lastAttackHeight = 0x100;
        self->state            = MegaOctus_StateCannon_Idle;
    }

    MegaOctus_CheckPlayerCollisions_Cannon();
}

void MegaOctus_StateCannon_Idle(void)
{
    RSDK_THIS(MegaOctus);

    self->angle = (self->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&self->altAnimator);

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;

    if (--self->timer <= 0) {
        if (self->shotCount <= 0) {
            self->velocity.y = 0x10000;
            self->state      = MegaOctus_StateCannon_SinkDown;
        }
        else {
            int32 attackHeight = self->lastAttackHeight;
            while (attackHeight == self->lastAttackHeight) attackHeight = RSDK.Rand(0, 4);

            self->lastAttackHeight = attackHeight;
            self->targetPos        = MegaOctus->cannonHeights[attackHeight] + self->origin.y;
            self->state            = MegaOctus_StateCannon_FireLaser;
            self->velocity.y       = self->targetPos < self->position.y ? -0x8000 : 0x8000;
        }
    }

    MegaOctus_CheckPlayerCollisions_Cannon();
}

void MegaOctus_StateCannon_FireLaser(void)
{
    RSDK_THIS(MegaOctus);

    self->position.y += self->velocity.y;
    self->angle = (self->angle + 6) & 0x1FF;

    RSDK.ProcessAnimation(&self->altAnimator);

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;

    if ((self->velocity.y < 0 && self->position.y <= self->targetPos) || (self->velocity.y >= 0 && self->position.y >= self->targetPos)) {
        self->position.y = self->targetPos;
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 4, &self->altAnimator, true, 0);
        RSDK.PlaySfx(MegaOctus->sfxLaser, false, 255);

        --self->shotCount;
        self->timer            = 40;
        int32 x                = (RSDK.Cos512(self->angle) << 10) + self->position.x;
        int32 y                = (RSDK.Sin512(self->angle) << 9) + self->position.y;
        EntityMegaOctus *child = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_LASER), x, y);
        child->direction       = self->direction;
        child->velocity.x      = self->direction ? 0x40000 : -0x40000;
        child->position.x += child->velocity.x;
        child->parent = (Entity *)self;
        child->timer  = 11;
        self->state   = MegaOctus_StateCannon_Idle;
    }
    MegaOctus_CheckPlayerCollisions_Cannon();
}

void MegaOctus_StateCannon_SinkDown(void)
{
    RSDK_THIS(MegaOctus);

    self->angle = (self->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&self->altAnimator);

    self->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= self->position.x;

    self->position.y += self->velocity.y;

    MegaOctus_CheckPlayerCollisions_Cannon();

    if (self->position.y > self->origin.y)
        destroyEntity(self);
}
void MegaOctus_Draw_Cannon(void)
{
    RSDK_THIS(MegaOctus);

    int32 angle = self->angle;
    int32 y     = self->position.y + 0x780000;

    Vector2 drawPos;
    for (int32 i = 0; i < 8; ++i) {
        drawPos.x = (RSDK.Cos512(angle) << 10) + self->position.x;
        drawPos.y = (RSDK.Sin512(angle) << 8) + y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        angle = (angle + 64) & 0x1FF;
        y -= 0xF0000;
    }

    drawPos.x = (RSDK.Cos512(angle) << 10) + self->position.x;
    drawPos.y = (RSDK.Sin512(angle) << 8) + y;
    RSDK.DrawSprite(&self->altAnimator, &drawPos, false);
}

void MegaOctus_CheckPlayerCollisions_Orb(void)
{
    RSDK_THIS(MegaOctus);

    Vector2 storePos = self->position;

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            self->position.x = (RSDK.Cos512(self->angle) << 10) + storePos.x;
            self->position.y = (RSDK.Sin512(self->angle) << 8) + storePos.y;
            if (Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
                self->position = storePos;

                --self->health;
                --MegaOctus->orbHealth[self->orbID];

                if (self->health) {
                    RSDK.PlaySfx(MegaOctus->sfxBossHit, false, 255);
                    self->invincibilityTimer = 30;
                }
                else {
                    RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);
                    self->invincibilityTimer = 60;
                    self->state              = MegaOctus_StateOrb_Destroyed;
                }
            }
        }
    }

    self->position = storePos;
}

void MegaOctus_StateOrb_Wait(void)
{
    RSDK_THIS(MegaOctus);

    self->position.y += self->velocity.y;

    self->angle = (self->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&self->altAnimator);

    if (MegaOctus->defeated) {
        self->invincibilityTimer = 60;
        self->state              = MegaOctus_StateOrb_Destroyed;
    }

    if (self->position.y <= self->targetPos) {
        self->timer = 60;
        self->state = MegaOctus_StateOrb_FireShot;
    }

    MegaOctus_CheckPlayerCollisions_Orb();
}

void MegaOctus_StateOrb_FireShot(void)
{
    RSDK_THIS(MegaOctus);

    self->angle = (self->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&self->altAnimator);

    if (MegaOctus->defeated) {
        self->invincibilityTimer = 60;
        self->state              = MegaOctus_StateOrb_Destroyed;
    }

#if MANIA_USE_PLUS
    if (self->shotCount > 0)
        self->position.x += self->velocity.x * RSDK.Sin1024(++self->orbMoveAngle);
#endif

    if (--self->timer <= 0) {
        ++self->shotCount;

        int32 x = (RSDK.Cos512(self->angle) << 10) + self->position.x;
        int32 y = (RSDK.Sin512(self->angle) << 9) + self->position.y;

        int32 angle = 0;
#if MANIA_USE_PLUS
        if (MegaOctus->bossEntity->position.x <= x)
            angle = -3 * self->shotCount;
        else
            angle = 3 * self->shotCount;
        angle *= 4;

        EntityMegaOctus *shot = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ORBSHOT), x, y);
        shot->velocity.x      = 0x300 * RSDK.Sin256(angle);
        shot->velocity.y      = 0x300 * RSDK.Cos256(angle);
#else
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        angle                 = RSDK.ATan2(player1->position.x - x, player1->position.y - y);

        EntityMegaOctus *shot = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ORBSHOT), x, y);
        shot->velocity.x      = 0x300 * RSDK.Cos256(angle);
        shot->velocity.y      = 0x300 * RSDK.Sin256(angle);
#endif

        RSDK.PlaySfx(MegaOctus->sfxBullet, false, 255);
        if (self->shotCount >= 4) {
            self->velocity.y = 0x10000;
            self->state      = MegaOctus_StateOrb_Idle;
        }
        else {
            self->timer = 60;
        }
    }

    MegaOctus_CheckPlayerCollisions_Orb();
}

void MegaOctus_StateOrb_Idle(void)
{
    RSDK_THIS(MegaOctus);

    self->angle = (self->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&self->altAnimator);

    if (MegaOctus->defeated) {
        self->invincibilityTimer = 60;
        self->state              = MegaOctus_StateOrb_Destroyed;
    }

    self->position.y += self->velocity.y;
    MegaOctus_CheckPlayerCollisions_Orb();

    if (self->position.y > self->origin.y)
        destroyEntity(self);
}

void MegaOctus_StateOrb_Destroyed(void)
{
    RSDK_THIS(MegaOctus);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x                    = self->position.x + (RSDK.Cos512(self->angle) << 10) + (RSDK.Rand(-16, 16) << 16);
            int32 y                    = self->position.y + (RSDK.Sin512(self->angle) << 9) + (RSDK.Rand(-16, 16) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + 2), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }

    if (--self->invincibilityTimer <= 0) {
        int32 angle = self->angle;
        int32 y     = self->position.y + 0xF80000;

        EntityDebris *debris = NULL;
        for (int32 i = 0; i < 16; ++i) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
            RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &debris->animator, true, 1);
            debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[0] + 1;
            debris->updateRange.x   = 0x400000;
            debris->updateRange.y   = 0x400000;

            angle = (angle + 0x40) & 0x1FF;
            y -= 0xF0000;
        }

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 0);
        debris->velocity.x      = -0x20000;
        debris->velocity.y      = -0x40000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0] + 1;
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 1);
        debris->velocity.x      = 0x20000;
        debris->velocity.y      = -0x40000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0] + 1;
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 2);
        debris->velocity.x      = -0x10000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0] + 1;
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 3);
        debris->velocity.x      = 0x10000;
        debris->velocity.y      = -0x20000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0] + 1;
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        destroyEntity(self);
    }
}

void MegaOctus_Draw_Orb(void)
{
    RSDK_THIS(MegaOctus);

    int32 angle = self->angle;
    int32 y     = self->position.y + 0xF80000;

    if (self->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    Vector2 drawPos;
    for (int32 i = 0; i < 16; ++i) {
        drawPos.x = (RSDK.Cos512(angle) << 10) + self->position.x;
        drawPos.y = (RSDK.Sin512(angle) << 8) + y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);

        angle = (angle + 0x40) & 0x1FF;
        y -= 0xF0000;
    }

    drawPos.x = (RSDK.Cos512(angle) << 10) + self->position.x;
    drawPos.y = (RSDK.Sin512(angle) << 8) + y;
    RSDK.DrawSprite(&self->altAnimator, &drawPos, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void MegaOctus_StateArm_WrapAroundPlatform(void)
{
    RSDK_THIS(MegaOctus);

    self->angle += 4;

    self->position.y -= 0xC000;
    self->shotCount += 0xC000;
    self->position.x = 0x1400 * RSDK.Cos256(self->angle) + self->origin.x;

    if (self->shotCount >= 0x4B0000) {
        if (!self->targetPos) {
            // Create another arm to grab the other platform
            self->targetPos      = 1;
            EntityMegaOctus *arm = CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_ARM), self->position.x, self->position.y + 0x40000);
            arm->direction       = self->direction;
            arm->state           = MegaOctus_StateArm_GrabPlatform;
            arm->stateDraw       = MegaOctus_Draw_Arm_WrapAroundPlatformTop;
        }

        if (self->shotCount >= 0x71A000) {
            self->timer = 30;
            self->state = MegaOctus_StateArm_GrabbedPlatform;
        }
    }
}

void MegaOctus_StateArm_GrabPlatform(void)
{
    RSDK_THIS(MegaOctus);

    self->position.x += 0xC000;
    self->shotCount += 0xC000;

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, -4, 12);

    if (self->shotCount >= 0x270000) {
        foreach_active(TilePlatform, platform)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox, platform, &platform->hitbox)) {
                self->parent = (Entity *)platform;
#if MANIA_USE_PLUS
                self->tilePlatY = platform->position.y;
#endif
                RSDK.CopyTileLayer(Zone->fgLayer[0], (platform->position.x >> 20) - 4, (platform->position.y >> 20) - 2, Zone->moveLayer, 10, 1, 8,
                                   5);
            }
        }

        int32 slot = RSDK.GetEntitySlot(MegaOctus->bossEntity) + 4;

        EntityCollapsingPlatform *collapsingPlatform = RSDK_GET_ENTITY(slot, CollapsingPlatform);
        collapsingPlatform->collapseDelay            = 24;
        collapsingPlatform->stoodPos.x               = self->position.x;

        collapsingPlatform                = RSDK_GET_ENTITY(slot + 1, CollapsingPlatform);
        collapsingPlatform->collapseDelay = 24;
        collapsingPlatform->stoodPos.x    = self->position.x;

        self->timer = 30;
        self->state = MegaOctus_StateArm_GrabbedPlatform;
    }
}

void MegaOctus_StateArm_GrabbedPlatform(void)
{
    RSDK_THIS(MegaOctus);

    if (--self->timer <= 0) {
#if !MANIA_USE_PLUS
        foreach_active(Player, player)
        {
            if (abs(player->position.x - player->position.x) < 0x400000 && player->groundedStore) {
                player->state      = Player_State_Air;
                player->velocity.y = -0x10000;
                player->onGround   = false;
            }
        }
#endif

        self->state = MegaOctus_StateArm_PullPlatformDown;
    }
}

void MegaOctus_StateArm_PullPlatformDown(void)
{
    RSDK_THIS(MegaOctus);

    EntityTilePlatform *parent = (EntityTilePlatform *)self->parent;

    self->origin.y += 0x60000;
    self->position.y += 0x60000;

    if (parent) {
#if MANIA_USE_PLUS
        parent->state      = MegaOctus_TilePlatformState_RiseOuttaOil;
        parent->velocity.y = 0x60000;
        if (parent->drawPos.y - self->tilePlatY > 0x480000)
            parent->stateCollide = Platform_Collision_None;
#else
        parent->drawPos.y += 0x60000;
        parent->centerPos.y += 0x60000;
#endif
    }

    if (++self->timer >= 32) {
        EntityMegaOctus *boss = MegaOctus->bossEntity;
        boss->velocity.y      = MANIA_USE_PLUS ? -0x8F400 : -0xB8000;
        boss->state           = MegaOctus_State_EnterMegaOctus;

        MegaOctus->eggmanOffset   = 0;
        MegaOctus->eggmanVelocity = 0;
        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 1, &MegaOctus->eggmanAnimator, true, 0);
        RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 1, &MegaOctus->hatchOpenAnimator, true, 0);
        RSDK.PlaySfx(MegaOctus->sfxSurface, false, 255);

        if (parent) {
            self->timer = 384;
            self->state = MegaOctus_StateArm_RisePlatformUp;
#if MANIA_USE_PLUS
            parent->state      = StateMachine_None;
            parent->position.y = self->tilePlatY + (self->timer << 15);
#endif
        }
        else {
            destroyEntity(self);
        }
    }
}

void MegaOctus_StateArm_RisePlatformUp(void)
{
    RSDK_THIS(MegaOctus);

    EntityTilePlatform *parent = (EntityTilePlatform *)self->parent;

#if MANIA_USE_PLUS
    if (parent) {
        parent->state      = MegaOctus_TilePlatformState_RiseOuttaOil;
        parent->timer      = 4;
        parent->velocity.y = -0x8000;

        if (parent->drawPos.y > self->tilePlatY)
            --self->timer;
        else
            self->timer = 0;

        if (parent->drawPos.y - self->tilePlatY < 0x480000)
            parent->stateCollide = Platform_Collision_Tiles;
    }
#else
    parent->drawPos.y -= 0x8000;
    parent->centerPos.y -= 0x8000;
    --self->timer;
#endif

    if (self->timer <= 0) {
#if MANIA_USE_PLUS
        if (parent) {
            parent->state      = StateMachine_None;
            parent->drawPos.y  = self->tilePlatY;
            parent->velocity.y = 0;
        }
#endif

        RSDK.CopyTileLayer(Zone->fgLayer[0], (parent->position.x >> 20) - 4, (parent->position.y >> 20) - 2, Zone->moveLayer, 1, 1, 8, 5);
        destroyEntity(self);
    }
}

void MegaOctus_Draw_Arm_WrapAroundPlatformBase(void)
{
    RSDK_THIS(MegaOctus);

    int32 pos       = 0xA0000;
    Vector2 drawPos = self->position;
    uint8 angle     = self->angle;

    for (int32 i = self->shotCount; i > 0; i -= 0x6000) {
        pos += 0x6000;
        if (pos >= 0x60000) {
            if (i < 0x400000) {
                if ((SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0] + 1 && angle < 0x80)
                    || (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0] && angle >= 0x80))
                    RSDK.DrawSprite(&self->animator, &drawPos, false);
            }
            angle -= 0x20;
            drawPos.y += 0x60000;
            drawPos.x = 0x1400 * RSDK.Cos256(angle) + self->origin.x;
            pos -= 0x60000;
        }
    }
}

void MegaOctus_Draw_Arm_WrapAroundPlatformTop(void)
{
    RSDK_THIS(MegaOctus);

    int32 pos       = 0xA0000;
    Vector2 drawPos = self->position;
    int32 count     = (self->shotCount - 1) / 0x6000 + 1;

    int32 angle      = self->angle;
    uint8 checkAngle = self->angle - 0x40;
    for (int32 i = 0; i < count; ++i) {
        pos += 0x6000;
        if (pos >= 0x60000) {
            if ((SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0] + 1 && checkAngle < 0x80)
                || (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0] && checkAngle >= 0x80))
                RSDK.DrawSprite(&self->animator, &drawPos, false);
            checkAngle += 0x20;
            angle += 0x20;
            drawPos.x -= 0x60000;
            drawPos.y = (RSDK.Sin256(angle) << 12) + self->origin.y;
            pos -= 0x60000;
        }
    }
}

void MegaOctus_State_Laser(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;

    if (self->timer > 0) {
        self->timer--;
        self->position.y = (RSDK.Sin512(self->parent->angle) << 9) + self->parent->position.y;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            Player_Hurt(player, self);
    }

    if (self->onScreen == 1) {
        if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0, 8)) {
            if (self->shotCount != self->position.x >> 20) {
                self->shotCount = self->position.x >> 20;
                RSDK.PlaySfx(MegaOctus->sfxLaserSplash, false, 255);
                CREATE_ENTITY(MegaOctus, INT_TO_VOID(MEGAOCTUS_LASERFIRE), (self->position.x & 0xFFF00000) + 0x80000, self->position.y);
            }
        }
    }

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void MegaOctus_State_LaserFire(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&self->animator);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            Player_Hurt(player, self);
    }

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void MegaOctus_Draw_Laser(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MegaOctus_State_Shot(void)
{
    RSDK_THIS(MegaOctus);

    if (MegaOctus->defeated || !RSDK.CheckOnScreen(self, &self->updateRange)) {
        destroyEntity(self);
    }
    else {
        RSDK.ProcessAnimation(&self->animator);

        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                Player_ProjectileHurt(player, self);
        }
    }
}

void MegaOctus_Draw_OrbShot(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if MANIA_USE_PLUS
void MegaOctus_TilePlatformState_RiseOuttaOil(void)
{
    EntityTilePlatform *self = RSDK_GET_ENTITY(SceneInfo->entitySlot, TilePlatform);

    if (self->velocity.y <= 0x10000) {
        foreach_active(Player, player)
        {
            if ((1 << player->playerID) & self->stoodPlayers) {
                if (player->state != OOZSetup_PlayerState_OilPool) {
                    player->state = Player_State_Ground;
                    OOZSetup->activePlayers &= ~(1 << player->playerID);
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if ((1 << player->playerID) & self->stoodPlayers) {
                if (player->state != OOZSetup_PlayerState_OilPool)
                    player->velocity.y = self->velocity.y >> 1;

                player->onGround = false;
                self->stoodPlayers &= ~(1 << player->playerID);
            }
        }
    }

    self->drawPos.y += self->velocity.y;
}
#endif

#if GAME_INCLUDE_EDITOR
void MegaOctus_EditorDraw(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 0, &self->animator, false, 0);
    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 1, &MegaOctus->eggmanAnimator, false, 0);
    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 1, &MegaOctus->noseAnimator, false, 5);
    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 2, &MegaOctus->boltsAnimator, false, 5);
    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 0, &MegaOctus->hatchBaseAnimator, false, 0);
    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 1, &MegaOctus->hatchOpenAnimator, false, 0);

    MegaOctus_Draw_Body();

    if (showGizmos()) {
        int32 slot = RSDK.GetEntitySlot(self);

        RSDK_DRAWING_OVERLAY(true);

#if MANIA_USE_PLUS
        DrawHelpers_DrawArenaBounds(-192, -384, 448, 96, 1 | 2 | 4 | 8, 0x00C0F0);
#else
        DrawHelpers_DrawArenaBounds(-192, -384, 448, 96, 1 | 0 | 4 | 8, 0x00C0F0);
#endif

        // Tile Platforms
        for (int32 p = 1; p < 4; ++p) {
            EntityTilePlatform *platform = RSDK_GET_ENTITY(slot + p, TilePlatform);
            if (!platform)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, platform->position.x, platform->position.y, 0xFFFF00, INK_NONE, 0xFF);
        }

        // Collapsing Platforms
        for (int32 p = 4; p < 6; ++p) {
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(slot + p, CollapsingPlatform);
            if (!platform)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, platform->position.x, platform->position.y, 0xFFFF00, INK_NONE, 0xFF);
        }
        RSDK_DRAWING_OVERLAY(false);
    }
}

void MegaOctus_EditorLoad(void)
{
    MegaOctus->aniFrames    = RSDK.LoadSpriteAnimation("OOZ/MegaOctus.bin", SCOPE_STAGE);
    MegaOctus->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanOOZ.bin", SCOPE_STAGE);
    MegaOctus->hatchFrames  = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE);
}
#endif

void MegaOctus_Serialize(void) {}
