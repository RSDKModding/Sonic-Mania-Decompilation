#include "SonicMania.h"

ObjectRockDrill *RockDrill;

void RockDrill_Update(void)
{
    RSDK_THIS(RockDrill);
    StateMachine_Run(entity->state);
}

void RockDrill_LateUpdate(void) {}

void RockDrill_StaticUpdate(void)
{
    if (RockDrill->shouldPlayDrillSfx) {
        if (!RockDrill->playingDrillSfx) {
            RSDK.PlaySfx(RockDrill->sfxDrill, 43643, 255);
            RockDrill->playingDrillSfx = true;
        }
        RockDrill->shouldPlayDrillSfx = 0;
    }
    else if (RockDrill->playingDrillSfx) {
        RSDK.StopSFX(RockDrill->sfxDrill);
        RockDrill->playingDrillSfx = false;
    }
}

void RockDrill_Draw(void)
{
    RSDK_THIS(RockDrill);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if ((entity->invincibilityTimer & 1) != 0) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }
    entity->animator.frameID = 1;
    drawPos.x -= 0x260000;
    drawPos.y -= entity->field_8C[0];
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 2;
    drawPos.y                = entity->field_94[0] + entity->position.y;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 1;
    drawPos.x += 0xA0000;
    drawPos.y = entity->position.y - entity->field_8C[1];
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 2;
    drawPos.y                = entity->field_94[1] + entity->position.y;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 1;
    drawPos.x += 0x420000;
    drawPos.y = entity->position.y - entity->field_8C[0];
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 2;
    drawPos.y                = entity->field_94[0] + entity->position.y;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 1;
    drawPos.x -= 0xA0000;
    drawPos.y = entity->position.y - entity->field_8C[1];
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 2;
    drawPos.y                = entity->field_94[1] + entity->position.y;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, 0, false);

    RSDK.SetPaletteEntry(0, 32, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void RockDrill_Create(void *data)
{
    RSDK_THIS(RockDrill);

    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->field_A4[0]   = 4;
        entity->field_9C[0]   = 4;
        RSDK.SetSpriteAnimation(RockDrill->aniFrames, 0, &entity->animator, true, 0);
        entity->drawOrder = Zone->drawOrderLow;
        entity->state     = RockDrill_State_Unknown1;
    }
}

void RockDrill_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        RockDrill->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/RockDrill.bin", SCOPE_STAGE);

    RockDrill->hitbox1.left   = -47;
    RockDrill->hitbox1.top    = -16;
    RockDrill->hitbox1.right  = 47;
    RockDrill->hitbox1.bottom = 16;

    RockDrill->hitbox2.left   = -47;
    RockDrill->hitbox2.top    = -56;
    RockDrill->hitbox2.right  = -16;
    RockDrill->hitbox2.bottom = -16;

    RockDrill->hitbox3.left   = 16;
    RockDrill->hitbox3.top    = -56;
    RockDrill->hitbox3.right  = 47;
    RockDrill->hitbox3.bottom = -16;

    RockDrill->hitbox4.left   = -47;
    RockDrill->hitbox4.top    = 16;
    RockDrill->hitbox4.right  = -47;
    RockDrill->hitbox4.bottom = 37;

    RockDrill->active             = ACTIVE_ALWAYS;
    RockDrill->shouldPlayDrillSfx = 0;
    RockDrill->playingDrillSfx    = false;
    RockDrill->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    RockDrill->sfxExplosion       = RSDK.GetSFX("Stage/Explosion2.wav");
    RockDrill->sfxDrill           = RSDK.GetSFX("LRZ/Drill.wav");
}

void RockDrill_CheckPlayerCollisions(void)
{
    RSDK_THIS(RockDrill);
    if (entity->invincibilityTimer) {
        entity->invincibilityTimer--;
    }
    else {

        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, entity, &RockDrill->hitbox1) || Player_CheckBadnikTouch(player, entity, &RockDrill->hitbox2)
                || Player_CheckBadnikTouch(player, entity, &RockDrill->hitbox3)) {
                if (Player_CheckBossHit(player, entity)) {
                    RSDK.PlaySfx(RockDrill->sfxHit, false, 255);
                    entity->invincibilityTimer = 30;
                }
            }
            else {
                if (Player_CheckCollisionTouch(player, entity, &RockDrill->hitbox4))
                    Player_CheckHit(player, entity);
            }
        }
    }
}

void RockDrill_SpawnDebris(int offset)
{
    RSDK_THIS(RockDrill);

    EntityDebris *debris =
        CREATE_ENTITY(Debris, Debris_State_Fall, offset + RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y + 0x400000);
    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(0, 6) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + entity->position.x, entity->position.y + 0x400000);
    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(-6, 0) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;
}

void RockDrill_State_Unknown1(void)
{
    RSDK_THIS(RockDrill);

    int slot                           = RSDK_sceneInfo->entitySlot - 1;
    EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
    while (platform->objectID == CollapsingPlatform->objectID) {
        platform->active = ACTIVE_NEVER;
        platform         = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
    }

    entity->active = ACTIVE_NORMAL;
    entity->state  = RockDrill_State_Unknown2;
    if (entity->lockCamera) {
        entity->boundsL         = Zone->screenBoundsL1[0];
        entity->boundsR         = Zone->screenBoundsR1[0];
        entity->boundsT         = Zone->screenBoundsT1[0];
        entity->boundsB         = Zone->screenBoundsB1[0];
        Zone->screenBoundsL1[0] = (entity->position.x >> 16) - RSDK_screens->centerX - 96;
        Zone->screenBoundsR1[0] = (entity->position.x >> 16) + RSDK_screens->centerX - 96;
        Zone->screenBoundsB1[0] = (entity->position.y >> 16) + 96;
    }
    entity->timer = 120;
}

void RockDrill_State_Unknown2(void)
{
    RSDK_THIS(RockDrill);

    ++RockDrill->shouldPlayDrillSfx;
    RSDK.ProcessAnimation(&entity->animator);

    for (int i = 0; i < 2; ++i) {
        if (entity->field_9C[i]) {
            entity->field_9C[i]--;
        }
        else if (entity->field_AC[i]) {
            bool32 flag = entity->field_8C[i] == 0x20000;
            entity->field_8C[i] -= 0x20000;
            if (flag) {
                entity->field_AC[i] = 0;
                entity->field_9C[i] = 10;
            }
        }
        else {
            entity->field_8C[i] += 0x40000;
            if (entity->field_8C[i] == 0x100000)
                entity->field_AC[i] = 1;
        }

        if (entity->field_A4[i]) {
            entity->field_A4[i]--;
        }
        else if (entity->field_B4[i]) {
            bool32 flag = entity->field_94[i] == 0x40000;
            entity->field_94[i] -= 0x40000;
            if (flag) {
                entity->field_B4[i] = 0;
                entity->field_A4[i] = 5;
            }
        }
        else {
            entity->field_94[i] += 0x80000;
            if (entity->field_94[i] == 0x100000)
                entity->field_B4[i] = 1;
        }
    }

    if (entity->field_94[0] == 0x80000) {
        RockDrill_SpawnDebris(-0x260000);
        RockDrill_SpawnDebris(0x260000);
    }
    else {
        if (entity->field_94[1] == 0x80000) {
            RockDrill_SpawnDebris(-0x1C0000);
            RockDrill_SpawnDebris(0x1C0000);
        }
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (!camera->shakePos.y)
        camera->shakePos.y = 4;
    entity->position.y ^= 0x10000;
    RockDrill_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = RockDrill_State_Unknown1;
    }
    if (--entity->timer <= 0) {
        foreach_active(LRZRockPile, pile)
        {
            pile->flag   = true;
            pile->active = ACTIVE_NORMAL;
            pile->timer  = 0;
            pile->timer2 = 0;
        }

        if (entity->lockCamera) {
            Zone->screenBoundsL1[0] = entity->boundsL;
            Zone->screenBoundsR1[0] = entity->boundsR;
            Zone->screenBoundsT1[0] = entity->boundsT;
            Zone->screenBoundsB1[0] = entity->boundsB;
        }
        entity->timer  = 30;
        entity->state  = RockDrill_State_Unknown3;
        entity->active = ACTIVE_NORMAL;
    }
}

void RockDrill_State_Unknown3(void)
{
    RSDK_THIS(RockDrill);

    if (entity->timer-- <= 0) {
        entity->velocity.y += 0x3800;
        entity->position.y += entity->velocity.y;
        uint16 tileLow  = RSDK.GetTileInfo(Zone->fgLow, entity->position.x >> 20, (entity->position.y + 0x200000) >> 20);
        uint16 tileHigh = RSDK.GetTileInfo(Zone->fgHigh, entity->position.x >> 20, (entity->position.y + 0x200000) >> 20);
        if (RSDK.GetTileBehaviour(tileLow, 0) == 1 || RSDK.GetTileBehaviour(tileHigh, 0) == 1) {
            entity->timer      = 0;
            entity->velocity.y = 0x8000;
            entity->drawOrder  = Zone->drawOrderLow - 1;
            entity->state      = RockDrill_State_Unknown4;
        }
        else {
            if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
                destroyEntity(entity);
        }
    }
}

void RockDrill_State_Unknown4(void)
{
    RSDK_THIS(RockDrill);
    entity->position.y += entity->velocity.y;

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(Drillerdroid->sfxExplosion, false, 255);
        if (!(Zone->timer & 3)) {
            int x                      = entity->position.x + (RSDK.Rand(-19, 20) << 16);
            int y                      = entity->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh + 2;
        }
    }
    if (++entity->timer > 120) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x - 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x300000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x + 0x230000, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void RockDrill_EditorDraw(void)
{
    RSDK_THIS(RockDrill);
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->field_A4[0]   = 4;
    entity->field_9C[0]   = 4;
    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 0, &entity->animator, true, 0);

    RockDrill_Draw();
}

void RockDrill_EditorLoad(void) { RockDrill->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/RockDrill.bin", SCOPE_STAGE); }
#endif

void RockDrill_Serialize(void) { RSDK_EDITABLE_VAR(RockDrill, VAR_BOOL, lockCamera); }
