#include "SonicMania.h"

ObjectDiveEggman *DiveEggman;

void DiveEggman_Update(void)
{
    RSDK_THIS(DiveEggman);
    StateMachine_Run(entity->state);
}

void DiveEggman_LateUpdate(void) {}

void DiveEggman_StaticUpdate(void) {}

void DiveEggman_Draw(void)
{
    RSDK_THIS(DiveEggman);
    if (entity->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 130, 0xE0E0E0);
    RSDK.DrawSprite(&entity->animator, NULL, false);
    RSDK.SetPaletteEntry(0, 130, 0x001840);
}

void DiveEggman_Create(void *data)
{
    RSDK_THIS(DiveEggman);

    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(entity);
    }
    else {
        entity->drawFX = FX_FLIP;
        if (!SceneInfo->inEditor) {
            entity->visible = true;
            if (data) {
                if (voidToInt(data) == 1) {
                    entity->active    = ACTIVE_NORMAL;
                    entity->drawOrder = Zone->playerDrawLow + 1;
                    entity->timer     = 480;
                    RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 5, &entity->animator, true, 0);
                    entity->state = DiveEggman_State2_Unknown1;
                }
            }
            else {
                entity->active          = ACTIVE_BOUNDS;
                entity->drawOrder       = Zone->playerDrawLow + 2;
                entity->updateRange.x   = 0x800000;
                entity->startY          = entity->position.y;
                entity->updateRange.y   = 0x800000;
                entity->health          = 4;
                DiveEggman->screwMobile = (Entity *)RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, ScrewMobile);
                RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 0, &entity->animator, true, 0);
                entity->state = DiveEggman_State_Unknown1;
            }
        }
    }
}

void DiveEggman_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ")) {
        DiveEggman->diveFrames = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE);
        DiveEggman->aniFrames  = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ1.bin", SCOPE_STAGE);
    }

    DiveEggman->hitbox1.left   = -8;
    DiveEggman->hitbox1.top    = -8;
    DiveEggman->hitbox1.right  = 8;
    DiveEggman->hitbox1.bottom = 8;

    DiveEggman->hitbox2.left   = -8;
    DiveEggman->hitbox2.top    = -8;
    DiveEggman->hitbox2.right  = 8;
    DiveEggman->hitbox2.bottom = 8;

    DiveEggman->hitbox3.left   = -26;
    DiveEggman->hitbox3.top    = -22;
    DiveEggman->hitbox3.right  = 7;
    DiveEggman->hitbox3.bottom = 11;

    DiveEggman->sfxBigFan       = RSDK.GetSFX("HCZ/BigFan.wav");
    DiveEggman->sfxHit          = RSDK.GetSFX("Stage/BossHit.wav");
    DiveEggman->sfxExplosion    = RSDK.GetSFX("Stage/Explosion2.wav");
    DiveEggman->sfxRockemSockem = RSDK.GetSFX("Stage/RockemSockem.wav");
}

void DiveEggman_Explode(void)
{
    RSDK_THIS(DiveEggman);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);

        if (!(Zone->timer & 0xF)) {
            int x                      = entity->position.x + (RSDK.Rand(-19, 20) << 16);
            int y                      = entity->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            if (entity->timer <= 40)
                explosion->drawOrder = entity->drawOrder;
            else
                explosion->drawOrder = entity->drawOrder - 1;
        }
    }
}

void DiveEggman_State_Unknown1(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&entity->animator);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;
    if (screwMobile->state != ScrewMobile_State_Unknown1) {
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &entity->animator, true, 0);
        entity->velocity.x = -0x10000;
        entity->active     = ACTIVE_NORMAL;
        entity->state      = DiveEggman_State_Unknown2;
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void DiveEggman_State_Unknown2(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->position.x += entity->velocity.x;

    if (entity->velocity.y > 0) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y -= 0x400;
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x200000, 0, true)
        || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x200000, 0, true)) {
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }

    if (screwMobile->animator2.animationSpeed >= 0xFF) {
        if (entity->position.x <= screwMobile->position.x) {
            entity->direction  = FLIP_NONE;
            entity->velocity.x = 0x8000;
        }
        else {
            entity->direction  = FLIP_X;
            entity->velocity.x = -0x8000;
        }
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 2, &entity->animator, false, 0);
        entity->state = DiveEggman_State_Unknown3;
    }
    if (--entity->timer <= 0) {
        entity->timer = 180;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 4, &entity->animator, false, 0);
        entity->state = DiveEggman_State_Unknown6;
    }
}

void DiveEggman_State_Unknown3(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->position.x += entity->velocity.x;

    if (entity->position.y >= Water->waterLevel + 0x180000)
        entity->position.y -= 0x2000;

    if (screwMobile->animator2.animationSpeed >= 0x100) {
        if (abs(entity->position.x - screwMobile->position.x) < 0x100000) {
            int dist      = entity->position.x - screwMobile->position.x;
            int y         = MathHelpers_SquareRoot((0x100 - (dist >> 16) * (dist >> 16))) << 16;
            entity->angle = RSDK.ATan2(dist, y);
            if (entity->angle < 0x80)
                entity->drawOrder = Zone->playerDrawLow + 2;
            else
                entity->drawOrder = Zone->hudDrawOrder - 1;
            RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 3, &entity->animator, false, 0);
            entity->state = DiveEggman_State_Unknown4;
        }
    }
    else {
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x10000;
        else
            entity->velocity.x = 0x10000;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &entity->animator, false, 0);
        entity->state = DiveEggman_State_Unknown2;
    }
}

void DiveEggman_State_Unknown4(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->position.y -= 0x10000;
    entity->position.x = (RSDK.Cos256(entity->angle) << 12) + screwMobile->position.x;
    if ((entity->angle & 0xFF) < 0x80)
        entity->drawOrder = Zone->playerDrawLow + 2;
    else
        entity->drawOrder = Zone->hudDrawOrder - 1;
    entity->angle += 4;

    if (screwMobile->animator2.animationSpeed >= 0x100) {
        if (entity->position.y < screwMobile->position.y + 0x280000) {
            RSDK.PlaySfx(DiveEggman->sfxHit, false, 255);
            entity->invincibilityTimer = 30;
            if (entity->health) {
                entity->health--;
            }

            if (!entity->health) {
                entity->timer               = 120;
                entity->drawOrder           = Zone->hudDrawOrder - 1;
                entity->state               = DiveEggman_State_Destroyed;
                SceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
            }
            screwMobile->whirlPoolTimer = 60;
        }
    }
    else {
        if (entity->position.y < Water->waterLevel - (screwMobile->field_74 << 16)) {
            entity->velocity.y = -0x10000;
            entity->state      = DiveEggman_State_Unknown5;
            entity->velocity.x = RSDK.Cos256(entity->angle) << 8;
        }
    }
}

void DiveEggman_State_Unknown5(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (entity->position.y >= Water->waterLevel) {
        entity->velocity.y >>= 2;
        CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), entity->position.x, Water->waterLevel);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x10000;
        else
            entity->velocity.x = 0x10000;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &entity->animator, false, 0);
        entity->state = DiveEggman_State_Unknown2;
    }
}

void DiveEggman_State_Unknown6(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    if (entity->animator.frameID >= entity->animator.frameCount - 1) {
        EntityDiveEggman *child = CREATE_ENTITY(DiveEggman, intToVoid(1), entity->position.x, entity->position.y + 0x20000);
        if (entity->direction)
            child->position.x += 0x1A0000;
        else
            child->position.x -= 0x1A0000;
        RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 1, &entity->animator, false, 0);
        entity->state = DiveEggman_State_Unknown2;
    }
}

void DiveEggman_State_Destroyed(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&entity->animator);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    DiveEggman_Explode();

    if (--entity->timer <= 0) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        RSDK.SetSpriteAnimation(DiveEggman->aniFrames, 0, &entity->animator, true, 0);
        entity->velocity.y = -0x20000;
        entity->timer      = 0x2000;
        entity->state      = DiveEggman_State_Finish;
    }
}

void DiveEggman_State_Finish(void)
{
    RSDK_THIS(DiveEggman);

    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;
    entity->position.x += entity->velocity.x;

    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->timer;
    if (entity->timer == 0x2000 && entity->position.y >= Water->waterLevel) {
        entity->velocity.y >>= 2;
        entity->timer = 0x1000;
        CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), entity->position.x, Water->waterLevel);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
    }

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        Zone->screenBoundsT1[0] = 0;
        screwMobile->state      = ScrewMobile_State_BossFinished;
        destroyEntity(entity);
    }
}

bool32 DiveEggman_Unknown10(void)
{
    RSDK_THIS(DiveEggman);

    RSDK.ProcessAnimation(&entity->animator);
    if (!--entity->timer) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(DiveEggman->sfxExplosion, false, 255);
        EntityWater *water = CREATE_ENTITY(Water, intToVoid(WATER_BUBBLE), entity->position.x, entity->position.y);
        water->velocity.y  = -0x8800;
        water->angle       = 2 * RSDK.Rand(0, 256);
        water->field_68    = water->position.x;
        water->childPtr    = 0;
        RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, true, 0);
        destroyEntity(entity);
        return false;
    }
    return true;
}

void DiveEggman_State2_Unknown1(void)
{
    RSDK_THIS(DiveEggman);

    if (entity->velocity.x) {
        if (entity->velocity.x <= 0)
            entity->velocity.x += 0x2000;
        else
            entity->velocity.x -= 0x2000;
    }

    if (entity->velocity.y > 0) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y -= 0x400;
    }

    if (DiveEggman_Unknown10()) {
        EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;
        if (screwMobile->animator2.animationSpeed >= 0xFF) {
            entity->state = DiveEggman_State2_Unknown2;
            if (entity->position.x > screwMobile->position.x)
                entity->velocity.x = -0x10000;
            else
                entity->velocity.x = 0x10000;
        }
    }
}

void DiveEggman_State2_Unknown2(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    if (DiveEggman_Unknown10()) {
        entity->position.x += entity->velocity.x;

        if (entity->position.y >= Water->waterLevel + 0x100000)
            entity->position.y -= 0x2000;

        if (screwMobile->animator2.animationSpeed >= 0x100) {
            if (abs(entity->position.x - screwMobile->position.x) < 0x100000) {
                int dist      = entity->position.x - screwMobile->position.x;
                int y         = MathHelpers_SquareRoot((0x100 - (dist >> 16) * (dist >> 16))) << 16;
                entity->angle = RSDK.ATan2(dist, y);
                if (entity->angle < 0x80)
                    entity->drawOrder = Zone->playerDrawLow + 2;
                else
                    entity->drawOrder = Zone->hudDrawOrder - 1;
                entity->state = DiveEggman_State2_Unknown3;
            }
        }
        else {
            entity->state = DiveEggman_State2_Unknown1;
        }
    }
}

void DiveEggman_State2_Unknown3(void)
{
    RSDK_THIS(DiveEggman);
    EntityScrewMobile *screwMobile = (EntityScrewMobile *)DiveEggman->screwMobile;

    if (DiveEggman_Unknown10()) {
        entity->position.y -= 0x10000;
        entity->position.x = (RSDK.Cos256(entity->angle) << 12) + screwMobile->position.x;
        if ((entity->angle & 0xFF) < 0x80)
            entity->drawOrder = Zone->playerDrawLow + 2;
        else
            entity->drawOrder = Zone->hudDrawOrder - 1;
        entity->angle += 4;
        if (screwMobile->animator2.animationSpeed >= 0x100) {
            if (entity->position.y < screwMobile->position.y + 0x180000) {
                entity->timer = 1;
                --screwMobile->field_7C;
                screwMobile->invincibilityTimer = 30;
                screwMobile->whirlPoolTimer     = 30;
                EntityPlayer *player1           = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

                if (!player1->blinkTimer && !player1->invincibleTimer) {
                    if (player1->shield) {
                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player1), Shield);
                        if (shield->objectID == Shield->objectID)
                            destroyEntity(shield);
                        player1->shield     = SHIELD_NONE;
                        player1->blinkTimer = 120;
                        RSDK.PlaySfx(Player->sfxHurt, false, 255);
                    }
                    else {
                        if (player1->rings <= 0) {
                            player1->hurtFlag = 1;
                            RSDK.PlaySfx(Player->sfxHurt, false, 255);
                            ScrewMobile->shouldPlayFanSfx = 0;
                        }
                        else {
                            player1->blinkTimer = 120;
                            if (!player1->hyperRing)
                                Player_LoseRings(player1, player1->rings, player1->collisionPlane);
                            else
                                Player_LoseHyperRings(player1, player1->rings, player1->collisionPlane);
                            player1->hyperRing     = false;
                            player1->rings         = 0;
                            player1->ringExtraLife = 100;
                            RSDK.PlaySfx(Player->sfxLoseRings, false, 255);
                        }
                    }
                    DiveEggman_Unknown10();
                }
            }
        }
        else {
            if (entity->position.y < Water->waterLevel - (screwMobile->field_74 << 16)) {
                entity->velocity.y = -0x10000;
                entity->state      = DiveEggman_State2_Unknown4;
                entity->velocity.x = (RSDK.Cos256(entity->angle) << 8);
            }
        }
    }
}

void DiveEggman_State2_Unknown4(void)
{
    RSDK_THIS(DiveEggman);

    if (DiveEggman_Unknown10()) {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x2000;
        if (entity->position.y >= Water->waterLevel) {
            entity->velocity.y >>= 2;
            CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), entity->position.x, Water->waterLevel);
            RSDK.PlaySfx(Water->sfxSplash, false, 255);
            entity->state = DiveEggman_State2_Unknown1;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void DiveEggman_EditorDraw(void)
{
    RSDK_THIS(DiveEggman);
    RSDK.SetSpriteAnimation(DiveEggman->diveFrames, 0, &entity->animator, true, 0);

    DiveEggman_Draw();
}

void DiveEggman_EditorLoad(void) { DiveEggman->diveFrames = RSDK.LoadSpriteAnimation("HCZ/DiveEggman.bin", SCOPE_STAGE); }
#endif

void DiveEggman_Serialize(void) {}
