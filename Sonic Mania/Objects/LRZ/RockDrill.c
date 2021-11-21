#include "SonicMania.h"

ObjectRockDrill *RockDrill;

void RockDrill_Update(void)
{
    RSDK_THIS(RockDrill);
    StateMachine_Run(self->state);
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
        RSDK.StopSfx(RockDrill->sfxDrill);
        RockDrill->playingDrillSfx = false;
    }
}

void RockDrill_Draw(void)
{
    RSDK_THIS(RockDrill);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if ((self->invincibilityTimer & 1) != 0) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }
    self->animator.frameID = 1;
    drawPos.x -= 0x260000;
    drawPos.y -= self->field_8C[0];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 2;
    drawPos.y                = self->field_94[0] + self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    drawPos.x += 0xA0000;
    drawPos.y = self->position.y - self->field_8C[1];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 2;
    drawPos.y                = self->field_94[1] + self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    drawPos.x += 0x420000;
    drawPos.y = self->position.y - self->field_8C[0];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 2;
    drawPos.y                = self->field_94[0] + self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 1;
    drawPos.x -= 0xA0000;
    drawPos.y = self->position.y - self->field_8C[1];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 2;
    drawPos.y                = self->field_94[1] + self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, 0, false);

    RSDK.SetPaletteEntry(0, 32, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void RockDrill_Create(void *data)
{
    RSDK_THIS(RockDrill);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->field_A4[0]   = 4;
        self->field_9C[0]   = 4;
        RSDK.SetSpriteAnimation(RockDrill->aniFrames, 0, &self->animator, true, 0);
        self->drawOrder = Zone->drawOrderLow;
        self->state     = RockDrill_State_Unknown1;
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
    RockDrill->sfxHit             = RSDK.GetSfx("Stage/BossHit.wav");
    RockDrill->sfxExplosion       = RSDK.GetSfx("Stage/Explosion2.wav");
    RockDrill->sfxDrill           = RSDK.GetSfx("LRZ/Drill.wav");
}

void RockDrill_CheckPlayerCollisions(void)
{
    RSDK_THIS(RockDrill);
    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
    }
    else {

        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &RockDrill->hitbox1) || Player_CheckBadnikTouch(player, self, &RockDrill->hitbox2)
                || Player_CheckBadnikTouch(player, self, &RockDrill->hitbox3)) {
                if (Player_CheckBossHit(player, self)) {
                    RSDK.PlaySfx(RockDrill->sfxHit, false, 255);
                    self->invincibilityTimer = 30;
                }
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &RockDrill->hitbox4))
                    Player_CheckHit(player, self);
            }
        }
    }
}

void RockDrill_SpawnDebris(int offset)
{
    RSDK_THIS(RockDrill);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x    = RSDK.Rand(0, 6) << 15;
    debris->velocity.y    = RSDK.Rand(-12, -8) << 15;
    debris->gravity       = 0x3800;
    debris->drawOrder     = Zone->drawOrderLow;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
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

    int slot                           = SceneInfo->entitySlot - 1;
    EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
    while (platform->objectID == CollapsingPlatform->objectID) {
        platform->active = ACTIVE_NEVER;
        platform         = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
    }

    self->active = ACTIVE_NORMAL;
    self->state  = RockDrill_State_Unknown2;
    if (self->lockCamera) {
        self->boundsL         = Zone->cameraBoundsL[0];
        self->boundsR         = Zone->cameraBoundsR[0];
        self->boundsT         = Zone->cameraBoundsT[0];
        self->boundsB         = Zone->cameraBoundsB[0];
        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->centerX - 96;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->centerX - 96;
        Zone->cameraBoundsB[0] = (self->position.y >> 16) + 96;
    }
    self->timer = 120;
}

void RockDrill_State_Unknown2(void)
{
    RSDK_THIS(RockDrill);

    ++RockDrill->shouldPlayDrillSfx;
    RSDK.ProcessAnimation(&self->animator);

    for (int i = 0; i < 2; ++i) {
        if (self->field_9C[i]) {
            self->field_9C[i]--;
        }
        else if (self->field_AC[i]) {
            bool32 flag = self->field_8C[i] == 0x20000;
            self->field_8C[i] -= 0x20000;
            if (flag) {
                self->field_AC[i] = 0;
                self->field_9C[i] = 10;
            }
        }
        else {
            self->field_8C[i] += 0x40000;
            if (self->field_8C[i] == 0x100000)
                self->field_AC[i] = 1;
        }

        if (self->field_A4[i]) {
            self->field_A4[i]--;
        }
        else if (self->field_B4[i]) {
            bool32 flag = self->field_94[i] == 0x40000;
            self->field_94[i] -= 0x40000;
            if (flag) {
                self->field_B4[i] = 0;
                self->field_A4[i] = 5;
            }
        }
        else {
            self->field_94[i] += 0x80000;
            if (self->field_94[i] == 0x100000)
                self->field_B4[i] = 1;
        }
    }

    if (self->field_94[0] == 0x80000) {
        RockDrill_SpawnDebris(-0x260000);
        RockDrill_SpawnDebris(0x260000);
    }
    else {
        if (self->field_94[1] == 0x80000) {
            RockDrill_SpawnDebris(-0x1C0000);
            RockDrill_SpawnDebris(0x1C0000);
        }
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (!camera->shakePos.y)
        camera->shakePos.y = 4;
    self->position.y ^= 0x10000;
    RockDrill_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->active = ACTIVE_BOUNDS;
        self->state  = RockDrill_State_Unknown1;
    }
    if (--self->timer <= 0) {
        foreach_active(LRZRockPile, pile)
        {
            pile->flag   = true;
            pile->active = ACTIVE_NORMAL;
            pile->timer  = 0;
            pile->timer2 = 0;
        }

        if (self->lockCamera) {
            Zone->cameraBoundsL[0] = self->boundsL;
            Zone->cameraBoundsR[0] = self->boundsR;
            Zone->cameraBoundsT[0] = self->boundsT;
            Zone->cameraBoundsB[0] = self->boundsB;
        }
        self->timer  = 30;
        self->state  = RockDrill_State_Unknown3;
        self->active = ACTIVE_NORMAL;
    }
}

void RockDrill_State_Unknown3(void)
{
    RSDK_THIS(RockDrill);

    if (self->timer-- <= 0) {
        self->velocity.y += 0x3800;
        self->position.y += self->velocity.y;
        uint16 tileLow  = RSDK.GetTileInfo(Zone->fgLow, self->position.x >> 20, (self->position.y + 0x200000) >> 20);
        uint16 tileHigh = RSDK.GetTileInfo(Zone->fgHigh, self->position.x >> 20, (self->position.y + 0x200000) >> 20);
        if (RSDK.GetTileBehaviour(tileLow, 0) == 1 || RSDK.GetTileBehaviour(tileHigh, 0) == 1) {
            self->timer      = 0;
            self->velocity.y = 0x8000;
            self->drawOrder  = Zone->drawOrderLow - 1;
            self->state      = RockDrill_State_Unknown4;
        }
        else {
            if (!RSDK.CheckOnScreen(self, &self->updateRange))
                destroyEntity(self);
        }
    }
}

void RockDrill_State_Unknown4(void)
{
    RSDK_THIS(RockDrill);
    self->position.y += self->velocity.y;

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(Drillerdroid->sfxExplosion, false, 255);
        if (!(Zone->timer & 3)) {
            int x                      = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int y                      = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh + 2;
        }
    }
    if (++self->timer > 120) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(0, 2) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x    = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y    = RSDK.Rand(-10, -6) << 15;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderLow;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void RockDrill_EditorDraw(void)
{
    RSDK_THIS(RockDrill);
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->field_A4[0]   = 4;
    self->field_9C[0]   = 4;
    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 0, &self->animator, true, 0);

    RockDrill_Draw();
}

void RockDrill_EditorLoad(void) { RockDrill->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/RockDrill.bin", SCOPE_STAGE); }
#endif

void RockDrill_Serialize(void) { RSDK_EDITABLE_VAR(RockDrill, VAR_BOOL, lockCamera); }
