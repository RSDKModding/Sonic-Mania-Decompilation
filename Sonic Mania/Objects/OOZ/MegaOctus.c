#include "SonicMania.h"

ObjectMegaOctus *MegaOctus;

void MegaOctus_Update(void)
{
    RSDK_THIS(MegaOctus);
    StateMachine_Run(entity->state);
}

void MegaOctus_LateUpdate(void) {}

void MegaOctus_StaticUpdate(void)
{
    foreach_active(MegaOctus, boss)
    {
        if (boss->type == MEGAOCTUS_ARM)
            RSDK.AddDrawListRef(Zone->drawOrderLow, RSDK.GetEntityID(boss));
    }
}

void MegaOctus_Draw(void)
{
    RSDK_THIS(MegaOctus);
    StateMachine_Run(entity->stateDraw);
}

void MegaOctus_Create(void *data)
{
    RSDK_THIS(MegaOctus);

    entity->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active = ACTIVE_BOUNDS;
            if (data)
                entity->type = voidToInt(data);
            switch (entity->type) {
                case MEGAOCTUS_BODY:
                    entity->visible   = false;
                    entity->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 1, &MegaOctus->animator3, true, 0);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 1, &MegaOctus->animator1, true, 5);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 2, &MegaOctus->animator2, true, 5);
                    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 0, &MegaOctus->animator4, true, 0);
                    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 1, &MegaOctus->animator5, true, 0);
                    entity->hitbox.left   = -33;
                    entity->hitbox.top    = -37;
                    entity->hitbox.right  = 33;
                    entity->hitbox.bottom = 6;
                    entity->origin.x      = entity->position.x + 0x800000;
                    entity->origin.y      = entity->position.y;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    MegaOctus->bossPtr    = (Entity *)entity;
                    MegaOctus->field_8C   = 0;
                    entity->state         = MegaOctus_State_SetupBounds;
                    entity->stateDraw     = MegaOctus_StateDraw_Body;
                    break;
                case MEGAOCTUS_UNUSED1: break;
                case MEGAOCTUS_HARPOON:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    entity->origin.x      = entity->position.x;
                    entity->origin.y      = entity->position.y;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->angle         = 0;
                    entity->rotation      = 0;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &entity->animator1, true, 1);
                    break;
                case MEGAOCTUS_GUN:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->drawOrder     = Zone->drawOrderLow + 1;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    entity->field_6C      = entity->position.y - 0x700000;
                    entity->origin.x      = entity->position.x;
                    entity->origin.y      = entity->position.y;
                    entity->field_70      = 256;
                    entity->shotCount     = 3;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &entity->animator1, true, 1);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 4, &entity->animator2, true, 3);
                    entity->velocity.y = -0x10000;
                    entity->state      = MegaOctus_StateGun_Unknown1;
                    entity->stateDraw  = MegaOctus_StateDraw_Gun;
                    break;
                case MEGAOCTUS_ORB:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->drawOrder     = Zone->drawOrderLow + 1;
                    entity->hitbox.left   = -16;
                    entity->hitbox.top    = -16;
                    entity->hitbox.right  = 16;
                    entity->hitbox.bottom = 16;
                    entity->field_6C      = entity->position.y - 0x1000000;
                    entity->origin.x      = entity->position.x;
                    entity->origin.y      = entity->position.y;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &entity->animator1, true, 1);
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 5, &entity->animator2, true, 0);
                    entity->velocity.y = -0x18000;
                    entity->state      = MegaOctus_StateOrb_Wait;
                    entity->stateDraw  = MegaOctus_StateDraw_Orb;
                    break;
                case MEGAOCTUS_ARM:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->updateRange.x = 0x800000;
                    entity->drawOrder     = Zone->drawOrderLow + 1;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    entity->origin.x      = entity->position.x;
                    entity->origin.y      = entity->position.y;
                    entity->updateRange.y = 0x800000;
                    entity->angle         = 0;
                    entity->rotation      = 0;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &entity->animator1, true, 1);
                    entity->state     = MegaOctus_StateArm_Unknown1;
                    entity->stateDraw = MegaOctus_StateDraw_Arm_Unknown1;
                    break;
                case MEGAOCTUS_LASER:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->hitbox.left   = -16;
                    entity->hitbox.top    = -1;
                    entity->hitbox.right  = 16;
                    entity->hitbox.bottom = 1;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 7, &entity->animator1, true, 0);
                    entity->state     = MegaOctus_State_Laser;
                    entity->stateDraw = MegaOctus_StateDraw_Laser;
                    break;
                case MEGAOCTUS_ORBSHOT:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->hitbox.left   = -3;
                    entity->hitbox.top    = -3;
                    entity->hitbox.right  = 3;
                    entity->hitbox.bottom = 3;
                    entity->updateRange.x = 0x400000;
                    entity->updateRange.y = 0x400000;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 6, &entity->animator1, true, 0);
                    entity->state     = MegaOctus_State_Shot;
                    entity->stateDraw = MegaOctus_StateDraw_OrbShot;
                    break;
                case MEGAOCTUS_UNUSED8: break;
                case MEGAOCTUS_LASERFIRE:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -4;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 0;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 8, &entity->animator1, true, 0);
                    entity->state     = MegaOctus_State_LaserFire;
                    entity->stateDraw = MegaOctus_StateDraw_Laser;
                    break;
                default: return;
            }
        }
        else {
            MegaOctus->bossPtr = NULL;
            destroyEntity(entity);
        }
    }
}

void MegaOctus_StageLoad(void)
{
    MegaOctus->active       = ACTIVE_ALWAYS;
    MegaOctus->bossPtr      = NULL;
    MegaOctus->aniFrames    = RSDK.LoadSpriteAnimation("OOZ/MegaOctus.bin", SCOPE_STAGE);
    MegaOctus->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanOOZ.bin", SCOPE_STAGE);
    MegaOctus->hatchFrames  = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE);

    MegaOctus->hitbox.left    = -16;
    MegaOctus->hitbox.top     = 22;
    MegaOctus->hitbox.right   = 16;
    MegaOctus->hitbox.bottom  = 30;
    MegaOctus->spawnHarpoon   = false;
    MegaOctus->orbHealth[0]   = 4;
    MegaOctus->orbHealth[1]   = 4;
    MegaOctus->field_D4       = false;
    MegaOctus->sfxBossHit     = RSDK.GetSFX("Stage/BossHit.wav");
    MegaOctus->sfxExplosion   = RSDK.GetSFX("Stage/Explosion2.wav");
    MegaOctus->sfxLaser       = RSDK.GetSFX("OOZ/OOZLaser.wav");
    MegaOctus->sfxBullet      = RSDK.GetSFX("OOZ/OOZBullet.wav");
    MegaOctus->sfxHarpoon     = RSDK.GetSFX("OOZ/Harpoon.wav");
    MegaOctus->sfxSurface     = RSDK.GetSFX("OOZ/OOZSurface.wav");
    MegaOctus->sfxLaserSplash = RSDK.GetSFX("OOZ/LaserSplash.wav");
}

void MegaOctus_CheckPlayerCollisions_Body(void)
{
    RSDK_THIS(MegaOctus);

    if (entity->invincibilityTimer)
        entity->invincibilityTimer--;

    foreach_active(Player, player)
    {
        int32 playerRadius     = 0x100000;
        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
        if (shield->objectID == Shield->objectID && shield->state == Shield_State_Insta)
            playerRadius = 0x160000;

        if (RSDK.CheckObjectCollisionTouchCircle(entity, 0x300000, player, playerRadius)) {
            int32 angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);

            player->velocity.x += 80 * RSDK.Cos256(angle);
            if (entity->invincibilityTimer || !Player_CheckBossHit(player, entity)) {
                if (80 * RSDK.Sin256(angle) >= 0)
                    player->velocity.y -= 80 * RSDK.Sin256(angle);
                else
                    player->velocity.y -= -80 * RSDK.Sin256(angle);
            }
            else {
                if (entity->health)
                    entity->health--;
                if (entity->health) {
                    RSDK.PlaySfx(MegaOctus->sfxBossHit, false, 255);
                    entity->invincibilityTimer = 30;
                    MegaOctus->spawnHarpoon    = 1;
                }
                else {
                    SceneInfo->timeEnabled = false;
                    Player_GiveScore(RSDK.GetEntityByID(SLOT_PLAYER1), 1000);
                    RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);
                    entity->invincibilityTimer = 120;
                    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 5, &MegaOctus->animator3, true, 0);
                    MegaOctus->field_D4 = true;
                    entity->state       = MegaOctus_State_Destroyed;
                }
            }
        }
    }
}

void MegaOctus_HandleEggmanAnim(void)
{
    RSDK_THIS(MegaOctus);

    switch (MegaOctus->animator3.animationID) {
        case 0:
        case 1:
            if (MegaOctus->animator3.frameID >= MegaOctus->animator3.frameCount - 1) {
                if (entity->invincibilityTimer) {
                    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 3, &MegaOctus->animator3, true, 0);
                }
                else {
                    bool32 laugh = false;
                    foreach_active(Player, player)
                    {
                        if (player->state == Player_State_Hit || player->state == Player_State_Die)
                            laugh = true;
                    }

                    if (laugh)
                        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 4, &MegaOctus->animator3, true, 0);
                }
            }
            break;
        case 2:
        case 4:
            if (entity->invincibilityTimer) {
                RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 3, &MegaOctus->animator3, true, 0);
            }
            else {
                bool32 laugh = false;
                foreach_active(Player, player)
                {
                    if (player->state == Player_State_Hit || player->state == Player_State_Die)
                        laugh = true;
                }

                if (MegaOctus->animator3.frameID >= MegaOctus->animator3.frameCount - 1) {
                    if (!laugh) {
                        uint8 anim = entity->state == MegaOctus_State_Unknown4 ? 2 : 0;
                        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, anim, &MegaOctus->animator3, true, 0);
                    }
                    else {
                        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 4, &MegaOctus->animator3, true, 8);
                    }
                }
            }
            break;
        case 3:
            if (!entity->invincibilityTimer) {
                if (MegaOctus->animator3.frameID >= MegaOctus->animator3.frameCount - 1) {
                    uint8 anim = entity->state == MegaOctus_State_Unknown4 ? 2 : 0;
                    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, anim, &MegaOctus->animator3, true, 0);
                }
            }
            break;
        default: break;
    }
}

void MegaOctus_Explode(void)
{
    RSDK_THIS(MegaOctus);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x = RSDK.Rand(-48, 48) << 16;
            int32 y = RSDK.Rand(-48, 48) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), entity->position.x + x, entity->position.y + y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void MegaOctus_HandleDirectionChange(void)
{
    RSDK_THIS(MegaOctus);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (abs(player1->position.x - entity->position.x) < 0x200000) {
        if (entity->orbID > 10) {
            entity->orbID--;
        }
        else if (entity->orbID < 10)
            entity->orbID++;
    }
    else {
        if (player1->position.x < entity->position.x) {
            if (entity->orbID)
                entity->orbID--;
        }
        else if (entity->orbID < 20)
            entity->orbID++;
    }

    entity->direction            = entity->orbID < 10;
    MegaOctus->animator1.frameID = entity->orbID >> 1;
    MegaOctus->animator2.frameID = entity->orbID >> 1;
    MegaOctus->field_8C          = MegaOctus->field_90[MegaOctus->animator1.frameID];
}

void MegaOctus_State_SetupBounds(void)
{
    RSDK_THIS(MegaOctus);
    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 448;
        Zone->playerBoundActiveB[0] = false;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 96;
        Zone->deathBoundary[0]      = Zone->screenBoundsB1[0] << 16;
#if RETRO_USE_PLUS
        Zone->screenBoundsT1[0] = Zone->screenBoundsB1[0] - 384;
#endif
        entity->position.y += 0xC00000;
        entity->active  = ACTIVE_NORMAL;
        entity->visible = true;
        entity->state   = MegaOctus_State_SetupArena;
    }
}

void MegaOctus_State_SetupArena(void)
{
    RSDK_THIS(MegaOctus);
    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->origin.x) {
        RSDK.GetSceneLayer(Zone->fgLow)->drawLayer[0] = 2;
        Zone->playerBoundActiveL[0]                   = true;
        Zone->screenBoundsL1[0]                       = (entity->position.x >> 16) - 192;
        Music_TransitionTrack(TRACK_EGGMAN1, 0.0075);
        OOZSetup->flags      = false;
        entity->velocity.y   = -0x40000;
        entity->health       = 8;
        entity->timer        = 60;
        EntityMegaOctus *arm = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ARM), entity->position.x + 0x800000, entity->origin.y + 0x400000);
        arm->direction       = entity->direction;
        arm->angle           = 128;

        arm            = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ARM), entity->position.x - 0x800000, entity->origin.y + 0x400000);
        arm->direction = entity->direction;
        arm->angle     = 128;

        entity->state = MegaOctus_State_Unknown1;
    }
}

void MegaOctus_State_Unknown1(void) {}

void MegaOctus_State_Unknown2(void)
{
    RSDK_THIS(MegaOctus);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += RETRO_USE_PLUS ? 0x3800 : 0x6000;
    if (entity->velocity.y >= 0x10000) {
        entity->state      = MegaOctus_State_Unknown3;
        entity->velocity.y = 0;
    }
    MegaOctus_CheckPlayerCollisions_Body();
}

void MegaOctus_State_Unknown3(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&MegaOctus->animator5);
    RSDK.ProcessAnimation(&MegaOctus->animator3);
    MegaOctus_HandleEggmanAnim();
    MegaOctus_HandleDirectionChange();
    MegaOctus_CheckPlayerCollisions_Body();

    if (--entity->timer <= 0) {
        entity->timer = 60;
        RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 2, &MegaOctus->animator5, true, 0);
        if (MegaOctus->animator3.animationID < 2)
            RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 2, &MegaOctus->animator3, true, 0);
        entity->state = MegaOctus_State_Unknown4;
    }
}

void MegaOctus_State_Unknown4(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&MegaOctus->animator5);
    RSDK.ProcessAnimation(&MegaOctus->animator3);
    MegaOctus_HandleEggmanAnim();
    MegaOctus->field_84 += 0x4800;
    MegaOctus->field_80 += MegaOctus->field_84;
    MegaOctus_HandleDirectionChange();
    MegaOctus_CheckPlayerCollisions_Body();

    if (--entity->timer <= 0)
        entity->state = MegaOctus_State_Unknown5;
}

void MegaOctus_State_Unknown5(void)
{
    RSDK_THIS(MegaOctus);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += RETRO_USE_PLUS ? 0x3800 : 0x6000;

    if (entity->position.y >= entity->origin.y + 0xC00000) {
        entity->position.y = entity->origin.y + 0xC00000;
        entity->state      = MegaOctus_State_SpawnWeapons;
        if (MegaOctus->spawnHarpoon)
            entity->timer = 480;
        else
            entity->timer = 240;
    }
    MegaOctus_CheckPlayerCollisions_Body();
}

void MegaOctus_State_SpawnWeapons(void)
{
    RSDK_THIS(MegaOctus);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= entity->origin.x)
        entity->position.x = entity->origin.x - 0x800000;
    else
        entity->position.x = entity->origin.x + 0x800000;

    --entity->timer;
    if (entity->timer == 240) {
        MegaOctus->spawnHarpoon  = false;
        EntityMegaOctus *harpoon = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_HARPOON), entity->position.x, entity->origin.y + 0x300000);
        harpoon->direction       = entity->direction;
        RSDK.PlaySfx(MegaOctus->sfxHarpoon, false, 255);
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= entity->position.x) {
            harpoon->origin.x -= 0x800000;
            harpoon->state     = MegaOctus_State_HarpoonLeft;
            harpoon->stateDraw = MegaOctus_StateDraw_HarpoonLeft;
        }
        else {
            harpoon->origin.x += 0x800000;
            harpoon->state     = MegaOctus_State_HarpoonRight;
            harpoon->stateDraw = MegaOctus_StateDraw_HarpoonRight;
        }
    }
    else if (entity->timer <= 0) {
        entity->timer                                                                                                  = 480;
        CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_GUN), entity->position.x, entity->origin.y + 0x400000)->direction = entity->direction;
        entity->state                                                                                                  = MegaOctus_State_SpawnOrbs;
    }
    MegaOctus_CheckPlayerCollisions_Body();
}

void MegaOctus_State_SpawnOrbs(void)
{
    RSDK_THIS(MegaOctus);
    MegaOctus_HandleDirectionChange();
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x <= entity->origin.x)
        entity->position.x = entity->origin.x - 0x800000;
    else
        entity->position.x = entity->origin.x + 0x800000;

    MegaOctus_CheckPlayerCollisions_Body();

    if (--entity->timer <= 0) {
        EntityMegaOctus *arm = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ARM), entity->position.x + 0x800000, entity->origin.y + 0x400000);
        arm->direction       = entity->direction;
        arm->angle           = 128;

        arm            = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ARM), entity->position.x - 0x800000, entity->origin.y + 0x400000);
        arm->direction = entity->direction;
        arm->angle     = 128;

        if (MegaOctus->orbHealth[0] > 0) {
            EntityMegaOctus *orb = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ORB), entity->position.x + 0x380000, entity->origin.y + 0x780000);
            orb->direction       = entity->direction;
            orb->health          = MegaOctus->orbHealth[0];
            orb->orbID           = 0;
            if (entity->position.x > entity->origin.x)
                orb->velocity.x = 0;
            else
                orb->velocity.x = 136;
        }
        if (MegaOctus->orbHealth[1] > 0) {
            EntityMegaOctus *orb = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ORB), entity->position.x - 0x380000, entity->origin.y + 0x780000);
            orb->direction       = entity->direction;
            orb->health          = MegaOctus->orbHealth[1];
            orb->orbID           = 1;
            orb->angle           = 256;
            if (entity->position.x > entity->origin.x)
                orb->velocity.x = -136;
            else
                orb->velocity.x = 0;
        }
        entity->timer = 120;
        entity->state = MegaOctus_State_Unknown1;
    }
}

void MegaOctus_State_Destroyed(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.ProcessAnimation(&MegaOctus->animator5);
    RSDK.ProcessAnimation(&MegaOctus->animator3);

    if (MegaOctus->field_84 > 0) {
        MegaOctus->field_84 += 0x4800;
        MegaOctus->field_80 += MegaOctus->field_84;
    }

    MegaOctus_Explode();
    if (--entity->invincibilityTimer <= 0) {
        if (!MegaOctus->field_84)
            RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 2, &MegaOctus->animator5, true, 0);
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->timer = 0;
        entity->state = MegaOctus_State_Finish;
    }
}

void MegaOctus_State_Finish(void)
{
    RSDK_THIS(MegaOctus);
    RSDK.ProcessAnimation(&MegaOctus->animator5);
    RSDK.ProcessAnimation(&MegaOctus->animator3);
    MegaOctus->field_84 += 0x4800;
    MegaOctus->field_80 += MegaOctus->field_84;
    entity->position.y += 0x10000;

    if (!(Zone->timer & 7)) {
        int32 x                      = entity->position.x + (RSDK.Rand(-48, 48) << 16);
        int32 y                      = entity->position.y + (RSDK.Rand(-48, 48) << 16);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(3), x, y);
        explosion->drawOrder       = entity->drawOrder;
    }

    if (++entity->timer > 120) {
        bool32 flag = true;
        foreach_active(MegaOctus, boss)
        {
            if (boss->type == MEGAOCTUS_ARM) {
                flag = false;
                foreach_break;
            }
        }

        if (flag) {
            Zone->screenBoundsR1[0] += 424;
            destroyEntity(entity);
        }
    }
}

void MegaOctus_StateDraw_Body(void)
{
    RSDK_THIS(MegaOctus);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    int32 val = abs(MegaOctus->field_8C) / 96;
    if (entity->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }
    drawPos.x = entity->position.x;
    drawPos.y -= 0x320000;
    RSDK.DrawSprite(&MegaOctus->animator5, &drawPos, false);

    Vector2 pos;
    pos   = drawPos;
    pos.y = drawPos.y + MegaOctus->field_80;
    RSDK.DrawSprite(&MegaOctus->animator3, &pos, false);

    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, 0, false);
    RSDK.DrawSprite(&MegaOctus->animator4, &drawPos, false);

    if (MegaOctus->field_8C < 0) {
        entity->drawFX |= FX_SCALE;
        entity->scale.y = 0x200;
        entity->scale.x = 0x200 - (val << 9 >> 15);
    }
    drawPos = entity->position;
    drawPos.x += val - 0x10000 + MegaOctus->field_8C + 16 * (val - 0x10000);
    entity->animator1.frameID = 1;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x += val * (MegaOctus->field_8C >> 18);
    entity->animator1.frameID = 2;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x -= val * (MegaOctus->field_8C >> 18);
    entity->drawFX &= ~FX_SCALE;
    if (MegaOctus->field_8C > 0) {
        entity->drawFX |= FX_SCALE;
        entity->scale.y = 0x200;
        entity->scale.x = 0x200 - (val << 9 >> 15);
    }

    drawPos.x += 34 * (0x10000 - val);
    entity->animator1.frameID = 1;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x += val * (MegaOctus->field_8C >> 18);
    entity->animator1.frameID = 2;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x -= val * (MegaOctus->field_8C >> 18);
    entity->drawFX = FX_FLIP;
    RSDK.DrawSprite(&MegaOctus->animator2, NULL, false);
    RSDK.DrawSprite(&MegaOctus->animator1, NULL, false);

    entity->animator1.frameID = 6;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.SetPaletteEntry(0, 128, 0x0000);
}

void MegaOctus_CheckPlayerCollisions_Unknown(void)
{
    RSDK_THIS(MegaOctus);
    entity->position.x = 0x3400 * RSDK.Sin512(entity->angle) + entity->origin.x;
    entity->position.y = 0x3400 * RSDK.Cos512(entity->angle) + entity->origin.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, entity);
        }
    }
}

void MegaOctus_State_HarpoonLeft(void)
{
    RSDK_THIS(MegaOctus);

    entity->angle = (entity->angle + 3) & 0x1FF;
    entity->shotCount += 12;
    MegaOctus_CheckPlayerCollisions_Unknown();

    if (entity->angle == 1)
        destroyEntity(entity);
}

void MegaOctus_State_HarpoonRight(void)
{
    RSDK_THIS(MegaOctus);

    entity->angle = (entity->angle - 3) & 0x1FF;
    entity->shotCount -= 12;
    MegaOctus_CheckPlayerCollisions_Unknown();

    if (entity->angle == 2)
        destroyEntity(entity);
}

void MegaOctus_StateDraw_HarpoonLeft(void)
{
    RSDK_THIS(MegaOctus);

    entity->animator1.frameID = 1;
    int32 angle                 = (entity->angle - 108) & 0x1FF;
    for (int32 i = 0; i < 9; ++i) {
        entity->position.x = 0x3400 * RSDK.Sin512(angle) + entity->origin.x;
        entity->position.y = 0x3400 * RSDK.Cos512(angle) + entity->origin.y;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        angle = (angle + 12) & 0x1FF;
    }

    entity->drawFX |= FX_ROTATE;
    entity->direction         = FLIP_NONE;
    entity->rotation          = (-256 - entity->angle) & 0x1FF;
    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->drawFX &= ~FX_ROTATE;
}

void MegaOctus_StateDraw_HarpoonRight(void)
{
    RSDK_THIS(MegaOctus);

    entity->animator1.frameID = 1;
    int32 angle                 = (entity->angle + 108) & 0x1FF;
    for (int32 i = 0; i < 9; ++i) {
        entity->position.x = 0x3400 * RSDK.Sin512(angle) + entity->origin.x;
        entity->position.y = 0x3400 * RSDK.Cos512(angle) + entity->origin.y;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        angle = (angle - 12) & 0x1FF;
    }

    entity->drawFX |= FX_ROTATE;
    entity->direction         = FLIP_X;
    entity->rotation          = (-256 - entity->angle) & 0x1FF;
    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->drawFX &= ~FX_ROTATE;
}

void MegaOctus_CheckPlayerCollisions_Gun(void)
{
    RSDK_THIS(MegaOctus);

    Vector2 pos;
    pos.x = (RSDK.Cos512(entity->angle) << 10) + entity->position.x;
    pos.y = (RSDK.Sin512(entity->angle) << 8) + entity->position.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, &pos, &entity->hitbox)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(768, player, 2, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, entity);
        }
    }
}

void MegaOctus_StateGun_Unknown1(void)
{
    RSDK_THIS(MegaOctus);
    entity->position.y += entity->velocity.y;
    entity->angle = (entity->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;
    if (entity->position.y <= entity->field_6C) {
        entity->shotCount = RETRO_USE_PLUS ? 2 : 3;
        entity->timer     = 128;
        entity->field_70  = 256;
        entity->state     = MegaOctus_StateGun_Unknown2;
    }
    MegaOctus_CheckPlayerCollisions_Gun();
}

void MegaOctus_StateGun_Unknown2(void)
{
    RSDK_THIS(MegaOctus);
    entity->angle = (entity->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;
    if (--entity->timer <= 0) {
        if (entity->shotCount <= 0) {
            entity->velocity.y = 0x10000;
            entity->state      = MegaOctus_StateGun_Unknown4;
        }
        else {
            int32 id = 0;
            while (id == entity->field_70) id = RSDK.Rand(0, 4);

            entity->field_70 = id;
            entity->field_6C = MegaOctus->field_BC[id] + entity->origin.y;
            entity->state    = MegaOctus_StateGun_FireLaser;
            if (entity->field_6C < entity->position.y)
                entity->velocity.y = -0x8000;
            else
                entity->velocity.y = 0x8000;
        }
    }
    MegaOctus_CheckPlayerCollisions_Gun();
}

void MegaOctus_StateGun_FireLaser(void)
{
    RSDK_THIS(MegaOctus);
    entity->position.y += entity->velocity.y;
    entity->angle = (entity->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;

    if ((entity->velocity.y >= 0 && entity->position.y < entity->field_6C) || (entity->velocity.y < 0 && entity->position.y > entity->field_6C)) {
        // TODO: cleanup
    }
    else {
        entity->position.y = entity->field_6C;
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 4, &entity->animator2, true, 0);
        RSDK.PlaySfx(MegaOctus->sfxLaser, false, 255);

        --entity->shotCount;
        entity->timer          = 40;
        int32 x                  = (RSDK.Cos512(entity->angle) << 10) + entity->position.x;
        int32 y                  = (RSDK.Sin512(entity->angle) << 9) + entity->position.y;
        EntityMegaOctus *child = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_LASER), x, y);
        child->direction       = entity->direction;
        if (entity->direction)
            child->velocity.x = 0x40000;
        else
            child->velocity.x = -0x40000;
        child->position.x += child->velocity.x;
        child->parent = (Entity *)entity;
        child->timer  = 11;
        entity->state = MegaOctus_StateGun_Unknown2;
    }
    MegaOctus_CheckPlayerCollisions_Gun();
}

void MegaOctus_StateGun_Unknown4(void)
{
    RSDK_THIS(MegaOctus);
    entity->angle = (entity->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x >= entity->position.x;
    entity->position.y += entity->velocity.y;
    MegaOctus_CheckPlayerCollisions_Gun();

    if (entity->position.y > entity->origin.y)
        destroyEntity(entity);
}
void MegaOctus_StateDraw_Gun(void)
{
    RSDK_THIS(MegaOctus);
    Vector2 drawPos;

    int32 angle = entity->angle;
    int32 y     = entity->position.y + 0x780000;

    for (int32 i = 0; i < 8; ++i) {
        drawPos.x = (RSDK.Cos512(angle) << 10) + entity->position.x;
        drawPos.y = (RSDK.Sin512(angle) << 8) + y;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        angle = (angle + 64) & 0x1FF;
        y -= 0xF0000;
    }

    drawPos.x = (RSDK.Cos512(angle) << 10) + entity->position.x;
    drawPos.y = (RSDK.Sin512(angle) << 8) + y;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void MegaOctus_CheckPlayerCollisions_Orb(void)
{
    RSDK_THIS(MegaOctus);

    Vector2 pos;
    pos.x = (RSDK.Cos512(entity->angle) << 10) + entity->position.x;
    pos.y = (RSDK.Sin512(entity->angle) << 8) + entity->position.y;

    if (entity->invincibilityTimer) {
        entity->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, &pos, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
                --entity->health;
                --MegaOctus->orbHealth[entity->orbID];
                if (entity->health) {
                    RSDK.PlaySfx(MegaOctus->sfxBossHit, false, 255);
                    entity->invincibilityTimer = 30;
                }
                else {
                    RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);
                    entity->invincibilityTimer = 60;
                    entity->state              = MegaOctus_StateOrb_Destroyed;
                }
            }
        }
    }
}

void MegaOctus_StateOrb_Wait(void)
{
    RSDK_THIS(MegaOctus);
    entity->position.y += entity->velocity.y;

    entity->angle = (entity->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator2);
    if (MegaOctus->field_D4) {
        entity->invincibilityTimer = 60;
        entity->state              = MegaOctus_StateOrb_Destroyed;
    }
    if (entity->position.y <= entity->field_6C) {
        entity->timer = 60;
        entity->state = MegaOctus_StateOrb_FireShot;
    }
    MegaOctus_CheckPlayerCollisions_Orb();
}

void MegaOctus_StateOrb_FireShot(void)
{
    RSDK_THIS(MegaOctus);
    entity->angle = (entity->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator2);

    if (MegaOctus->field_D4) {
        entity->invincibilityTimer = 60;
        entity->state              = MegaOctus_StateOrb_Destroyed;
    }
#if RETRO_USE_PLUS
    if (entity->shotCount > 0)
        entity->position.x += entity->velocity.x * RSDK.Sin1024(++entity->field_68);
#endif
    if (--entity->timer <= 0) {
        ++entity->shotCount;

        int32 x = (RSDK.Cos512(entity->angle) << 10) + entity->position.x;
        int32 y = (RSDK.Sin512(entity->angle) << 9) + entity->position.y;

        int32 angle = 0;
#if RETRO_USE_PLUS
        if (MegaOctus->bossPtr->position.x <= x)
            angle = -3 * entity->shotCount;
        else
            angle = 3 * entity->shotCount;
        angle *= 4;

        EntityMegaOctus *shot = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ORBSHOT), x, y);
        shot->velocity.x      = 0x300 * RSDK.Sin256(angle);
        shot->velocity.y      = 0x300 * RSDK.Cos256(angle);
#else
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        angle                 = RSDK.ATan2(player1->position.x - x, player1->position.y - y);

        EntityMegaOctus *shot = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ORBSHOT), x, y);
        shot->velocity.x      = 0x300 * RSDK.Cos256(angle);
        shot->velocity.y      = 0x300 * RSDK.Sin256(angle);
#endif

        RSDK.PlaySfx(MegaOctus->sfxBullet, false, 255);
        if (entity->shotCount >= 4) {
            entity->velocity.y = 0x10000;
            entity->state      = MegaOctus_StateOrb_Idle;
        }
        else {
            entity->timer = 60;
        }
    }
    MegaOctus_CheckPlayerCollisions_Orb();
}

void MegaOctus_StateOrb_Idle(void)
{
    RSDK_THIS(MegaOctus);

    entity->angle = (entity->angle + 6) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator2);
    if (MegaOctus->field_D4) {
        entity->invincibilityTimer = 60;
        entity->state              = MegaOctus_StateOrb_Destroyed;
    }
    entity->position.y += entity->velocity.y;
    MegaOctus_CheckPlayerCollisions_Orb();

    if (entity->position.y > entity->origin.y)
        destroyEntity(entity);
}

void MegaOctus_StateOrb_Destroyed(void)
{
    RSDK_THIS(MegaOctus);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MegaOctus->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x                      = entity->position.x + (RSDK.Cos512(entity->angle) << 10) + (RSDK.Rand(-16, 16) << 16);
            int32 y                      = entity->position.y + (RSDK.Sin512(entity->angle) << 9) + (RSDK.Rand(-16, 16) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh + 2;
        }
    }

    if (--entity->invincibilityTimer <= 0) {
        int32 angle = entity->angle;
        int32 y     = entity->position.y + 0xF80000;

        EntityDebris *debris = NULL;
        for (int32 i = 0; i < 16; ++i) {
            debris =
                CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
            RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 3, &debris->animator, true, 1);
            debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
            debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderLow + 1;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;
            angle                 = (angle + 64) & 0x1FF;
            y -= 0xF0000;
        }

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 0);
        debris->velocity.x    = -0x20000;
        debris->velocity.y    = -0x40000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow + 1;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 1);
        debris->velocity.x    = 0x20000;
        debris->velocity.y    = -0x40000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow + 1;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 2);
        debris->velocity.x    = -0x10000;
        debris->velocity.y    = -0x20000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow + 1;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + (RSDK.Cos512(angle) << 10), y + (RSDK.Sin512(angle) << 8));
        RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 9, &debris->animator, true, 3);
        debris->velocity.x    = 0x10000;
        debris->velocity.y    = -0x20000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow + 1;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        destroyEntity(entity);
    }
}

void MegaOctus_StateDraw_Orb(void)
{
    RSDK_THIS(MegaOctus);
    Vector2 drawPos;

    int32 angle = entity->angle;
    int32 y     = entity->position.y + 0xF80000;

    if (entity->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128u, 0xE0E0E0);

    for (int32 i = 0; i < 16; ++i) {
        drawPos.x = (RSDK.Cos512(angle) << 10) + entity->position.x;
        drawPos.y = (RSDK.Sin512(angle) << 8) + y;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        angle = (angle + 64) & 0x1FF;
        y -= 0xF0000;
    }

    drawPos.x = (RSDK.Cos512(angle) << 10) + entity->position.x;
    drawPos.y = (RSDK.Sin512(angle) << 8) + y;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void MegaOctus_StateArm_Unknown1(void)
{
    RSDK_THIS(MegaOctus);
    entity->angle += 4;
    entity->position.y -= 0xC000;
    entity->shotCount += 0xC000;
    entity->position.x = 0x1400 * RSDK.Cos256(entity->angle) + entity->origin.x;
    if (entity->shotCount >= 0x4B0000) {
        if (!entity->field_6C) {
            entity->field_6C     = 1;
            EntityMegaOctus *arm = CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_ARM), entity->position.x, entity->position.y + 0x40000);
            arm->direction       = entity->direction;
            arm->state           = MegaOctus_StateArm_Unknown5;
            arm->stateDraw       = MegaOctus_StateDraw_Arm_Unknown2;
        }
        if (entity->shotCount >= 0x71A000) {
            entity->timer = 30;
            entity->state = MegaOctus_StateArm_Unknown2;
        }
    }
}

void MegaOctus_StateArm_Unknown5(void)
{
    RSDK_THIS(MegaOctus);
    entity->angle -= 4;
    entity->position.x += 0xC000;
    entity->shotCount += 0xC000;

    entity->position.y = (RSDK.Sin256(entity->angle) << 12) + entity->origin.y;
    if (entity->shotCount >= 0x270000) {
        foreach_active(TilePlatform, platform)
        {
            if (RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox, platform, &platform->hitbox)) {
                entity->parent = (Entity *)platform;
#if RETRO_USE_PLUS
                entity->tilePlatY = platform->position.y;
#endif
                RSDK.CopyTileLayer(Zone->fgLow, (platform->position.x >> 20) - 4, (platform->position.y >> 20) - 2, Zone->moveLayer, 10, 1, 8, 5);
            }
        }

        int32 slot = RSDK.GetEntityID(MegaOctus->bossPtr) + 4;

        EntityCollapsingPlatform *collapsingPlatform = RSDK_GET_ENTITY(slot, CollapsingPlatform);
        collapsingPlatform->collapseDelay            = 24;
        collapsingPlatform->playerPos.x              = entity->position.x;

        collapsingPlatform                = RSDK_GET_ENTITY(slot + 1, CollapsingPlatform);
        collapsingPlatform->collapseDelay = 24;
        collapsingPlatform->playerPos.x   = entity->position.x;

        entity->timer = 30;
        entity->state = MegaOctus_StateArm_Unknown2;
    }
}

void MegaOctus_StateArm_Unknown2(void)
{
    RSDK_THIS(MegaOctus);
    if (--entity->timer <= 0) {
#if !RETRO_USE_PLUS
        foreach_active(Player, player)
        {
            if (abs(player->position.x - player->position.x) < 0x400000 && player->groundedStore) {
                player->state      = Player_State_Air;
                player->velocity.y = -0x10000;
                player->onGround   = false;
            }
        }
#endif

        entity->state = MegaOctus_StateArm_Unknown3;
    }
}

void MegaOctus_StateArm_Unknown3(void)
{
    RSDK_THIS(MegaOctus);
    EntityTilePlatform *parent = (EntityTilePlatform *)entity->parent;

    entity->origin.y += 0x60000;
    entity->position.y += 0x60000;
    if (parent) {
#if RETRO_USE_PLUS
        parent->state      = MegaOctus_TilePlatformState_Unknown;
        parent->velocity.y = 0x60000;
        if (parent->drawPos.y - entity->tilePlatY > 0x480000)
            parent->stateCollide = Platform_CollisionState_None;
#else
        parent->drawPos.y += 0x60000;
        parent->centerPos.y += 0x60000;
#endif
    }

    if (++entity->timer >= 32) {
        EntityMegaOctus *boss = (EntityMegaOctus *)MegaOctus->bossPtr;
        boss->velocity.y      = RETRO_USE_PLUS ? -0x8F400 : -0xB8000;
        boss->state           = MegaOctus_State_Unknown2;

        MegaOctus->field_80 = 0;
        MegaOctus->field_84 = 0;
        RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 1, &MegaOctus->animator3, true, 0);
        RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 1, &MegaOctus->animator5, true, 0);
        RSDK.PlaySfx(MegaOctus->sfxSurface, false, 255);

        EntityMegaOctus *parent = (EntityMegaOctus *)entity->parent;
        if (parent) {
            entity->timer = 384;
            entity->state = MegaOctus_StateArm_Unknown4;
#if RETRO_USE_PLUS
            parent->state      = StateMachine_None;
            parent->position.y = entity->tilePlatY + (entity->timer << 15);
#endif
        }
        else {
            destroyEntity(entity);
        }
    }
}

void MegaOctus_StateArm_Unknown4(void)
{
    RSDK_THIS(MegaOctus);
    EntityTilePlatform *parent = (EntityTilePlatform *)entity->parent;

#if RETRO_USE_PLUS
    if (parent) {
        parent->state         = MegaOctus_TilePlatformState_Unknown;
        parent->collapseDelay = 4;
        parent->velocity.y    = -0x8000;

        if (parent->drawPos.y > entity->tilePlatY)
            --entity->timer;
        else
            entity->timer = 0;

        if (parent->drawPos.y - entity->tilePlatY < 0x480000)
            parent->stateCollide = Platform_CollisionState_Tiles;
    }
#else
    parent->drawPos.y -= 0x8000;
    parent->centerPos.y -= 0x8000;
    --entity->timer;
#endif

    if (entity->timer <= 0) {
#if RETRO_USE_PLUS
        if (parent) {
            parent->state      = StateMachine_None;
            parent->drawPos.y  = entity->tilePlatY;
            parent->velocity.y = 0;
        }
#endif
        RSDK.CopyTileLayer(Zone->fgLow, (parent->position.x >> 20) - 4, (parent->position.y >> 20) - 2, Zone->moveLayer, 1, 1, 8, 5);
        destroyEntity(entity);
    }
}

void MegaOctus_StateDraw_Arm_Unknown1(void)
{
    RSDK_THIS(MegaOctus);
    int32 pos = 0xA0000;

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    uint8 angle = entity->angle;
    for (int32 i = entity->shotCount; i > 0; i -= 0x6000) {
        pos += 0x6000;
        if (pos >= 0x60000) {
            if (i < 0x400000) {
                if ((SceneInfo->currentDrawGroup == Zone->drawOrderLow + 1 && angle < 0x80)
                    || (SceneInfo->currentDrawGroup == Zone->drawOrderLow && angle >= 0x80))
                    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
            }
            angle -= 32;
            drawPos.y += 0x60000;
            drawPos.x = 0x1400 * RSDK.Cos256(angle) + entity->origin.x;
            pos -= 0x60000;
        }
    }
}

void MegaOctus_StateDraw_Arm_Unknown2(void)
{
    RSDK_THIS(MegaOctus);
    int32 pos = 0xA0000;

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    int32 angle = entity->angle;

    uint8 angle2 = angle - 64;
    int32 count   = (entity->shotCount - 1) / 0x6000 + 1;
    for (int32 i = 0; i < count; ++i) {
        pos += 0x6000;
        if (pos >= 0x60000) {
            if ((SceneInfo->currentDrawGroup == Zone->drawOrderLow + 1 && angle2 < 0x80)
                || (SceneInfo->currentDrawGroup == Zone->drawOrderLow && angle2 >= 0x80))
                RSDK.DrawSprite(&entity->animator1, &drawPos, false);
            angle2 += 32;
            drawPos.x -= 0x60000;
            angle += 32;
            drawPos.y = (RSDK.Sin256(angle) << 12) + entity->origin.y;
            pos -= 0x60000;
        }
    }
}

void MegaOctus_State_Laser(void)
{
    RSDK_THIS(MegaOctus);
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;

    if (entity->timer > 0) {
        entity->timer--;
        entity->position.y = (RSDK.Sin512(entity->parent->angle) << 9) + entity->parent->position.y;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
            Player_CheckHit(player, entity);
    }

    if (entity->activeScreens == 1) {
        if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, 8)) {
            if (entity->shotCount != entity->position.x >> 20) {
                entity->shotCount = entity->position.x >> 20;
                RSDK.PlaySfx(MegaOctus->sfxLaserSplash, false, 255);
                CREATE_ENTITY(MegaOctus, intToVoid(MEGAOCTUS_LASERFIRE), (entity->position.x & 0xFFF00000) + 0x80000, entity->position.y);
            }
        }
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void MegaOctus_State_LaserFire(void)
{
    RSDK_THIS(MegaOctus);
    RSDK.ProcessAnimation(&entity->animator1);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
            Player_CheckHit(player, entity);
    }

    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void MegaOctus_StateDraw_Laser(void)
{
    RSDK_THIS(MegaOctus);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void MegaOctus_State_Shot(void)
{
    RSDK_THIS(MegaOctus);
    if (MegaOctus->field_D4 || !RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        destroyEntity(entity);
    }
    else {
        RSDK.ProcessAnimation(&entity->animator1);
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox))
                Player_CheckProjectileHit(player, entity);
        }
    }
}

void MegaOctus_StateDraw_OrbShot(void)
{
    RSDK_THIS(MegaOctus);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

#if RETRO_USE_PLUS
void MegaOctus_TilePlatformState_Unknown(void)
{
    EntityTilePlatform *entity = RSDK_GET_ENTITY(SceneInfo->entitySlot, TilePlatform);

    if (entity->velocity.y <= 0x10000) {
        foreach_active(Player, player)
        {
            if ((1 << player->playerID) & entity->stoodPlayers) {
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
            if ((1 << player->playerID) & entity->stoodPlayers) {
                if (player->state != OOZSetup_PlayerState_OilPool)
                    player->velocity.y = entity->velocity.y >> 1;
                player->onGround = false;
                entity->stoodPlayers &= ~(1 << player->playerID);
            }
        }
    }

    entity->drawPos.y += entity->velocity.y;
}
#endif

#if RETRO_INCLUDE_EDITOR
void MegaOctus_EditorDraw(void)
{
    RSDK_THIS(MegaOctus);

    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(MegaOctus->eggmanFrames, 1, &MegaOctus->animator3, false, 0);
    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 1, &MegaOctus->animator1, false, 5);
    RSDK.SetSpriteAnimation(MegaOctus->aniFrames, 2, &MegaOctus->animator2, false, 5);
    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 0, &MegaOctus->animator4, false, 0);
    RSDK.SetSpriteAnimation(MegaOctus->hatchFrames, 1, &MegaOctus->animator5, false, 0);

    MegaOctus_StateDraw_Body();
}

void MegaOctus_EditorLoad(void)
{
    MegaOctus->aniFrames    = RSDK.LoadSpriteAnimation("OOZ/MegaOctus.bin", SCOPE_STAGE);
    MegaOctus->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanOOZ.bin", SCOPE_STAGE);
    MegaOctus->hatchFrames  = RSDK.LoadSpriteAnimation("OOZ/Hatch.bin", SCOPE_STAGE);
}
#endif

void MegaOctus_Serialize(void) {}
