#include "SonicMania.h"

ObjectRexon *Rexon;

void Rexon_Update(void)
{
    RSDK_THIS(Rexon);

    StateMachine_Run(self->state);

    self->positions[Rexon_SegmentCount].x = self->position.x;
    self->positions[Rexon_SegmentCount].y = self->position.y;
    if (self->direction)
        self->positions[Rexon_SegmentCount].x += 0x1C0000;
    else
        self->positions[Rexon_SegmentCount].x -= 0x1C0000;
    self->positions[Rexon_SegmentCount].y += 0x110000;

    for (int i = Rexon_SegmentCount - 1; i >= 0; --i) {
        int y = self->field_B4[i] >> 1;
        if (self->direction == FLIP_X)
            y = (0x80 - y) & 0xFF;
        self->positions[i].x = self->field_A0[i] * RSDK.Cos256(y) + self->positions[i + 1].x;
        self->positions[i].y = self->field_A0[i] * RSDK.Sin256(y) + self->positions[i + 1].y;
    }
}

void Rexon_LateUpdate(void) {}

void Rexon_StaticUpdate(void) {}

void Rexon_Draw(void)
{
    RSDK_THIS(Rexon);

    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->state != Rexon_State_Destroyed && self->state != Rexon_State_Explode) {
        RSDK.DrawSprite(&self->animator3, self->positions, false);
        for (int i = 1; i < Rexon_SegmentCount; ++i) RSDK.DrawSprite(&self->animator2, &self->positions[i], false);
    }
}

void Rexon_Create(void *data)
{
    RSDK_THIS(Rexon);

    self->drawFX |= FX_FLIP;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->startDir      = self->direction;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawOrder     = Zone->drawOrderLow;
    self->type          = voidToInt(data);

    switch (self->type) {
        case 0:
            self->timer = 128;
            if (!self->noMove)
                self->velocity.x = -0x2000;

            for (int i = 0; i < Rexon_SegmentCount; ++i) {
                self->field_A0[i] = 0;
                self->field_B4[i] = 0x170;
                self->field_C8[i] = 0;
            }

            self->segmentID = 0;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &self->animator3, true, 2);
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &self->animator2, true, 0);
            self->state = Rexon_State_Setup;
            break;
        case 1:
            self->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &self->animator1, true, 0);
            self->state = Rexon_State1_Unknown;
            break;
        case 2:
            self->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 4, &self->animator1, true, 0);
            self->state = Rexon_State2_Unknown;
            break;
    }
}

void Rexon_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Rexon.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Rexon.bin", SCOPE_STAGE);

    Rexon->hitbox1.left   = -8;
    Rexon->hitbox1.top    = -8;
    Rexon->hitbox1.right  = 8;
    Rexon->hitbox1.bottom = 8;

    Rexon->hitbox2.left   = -16;
    Rexon->hitbox2.top    = -8;
    Rexon->hitbox2.right  = 16;
    Rexon->hitbox2.bottom = 8;

    Rexon->hitbox3.left   = -160;
    Rexon->hitbox3.top    = -256;
    Rexon->hitbox3.right  = 96;
    Rexon->hitbox3.bottom = 256;

    Rexon->hitbox4.left   = -4;
    Rexon->hitbox4.top    = -4;
    Rexon->hitbox4.right  = 4;
    Rexon->hitbox4.bottom = 4;

    Rexon->sfxShot      = RSDK.GetSFX("Stage/Shot.wav");
    Rexon->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");

    DEBUGMODE_ADD_OBJ(Rexon);
}

void Rexon_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Rexon, NULL, self->position.x, self->position.y);
}

void Rexon_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Rexon_CheckOnScreen(void)
{
    RSDK_THIS(Rexon);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        Rexon_Create(NULL);
    }
}

void Rexon_CheckPlayerCollisions(void)
{
    RSDK_THIS(Rexon);

    if (self->state != Rexon_State_Explode) {
        int storeX = self->position.x;
        int storeY = self->position.y;

        foreach_active(Player, player)
        {
            Player_CheckCollisionBox(player, self, &Rexon->hitbox2);
            if (self->state != Rexon_State_Destroyed) {
                self->position.x = self->positions[0].x;
                self->position.y = self->positions[0].y;
                if (Player_CheckBadnikTouch(player, self, &Rexon->hitbox1)) {
                    if (Player_CheckBadnikBreak(self, player, false))
                        Rexon_Destroy(self, false);
                }
                else {
                    for (int i = 1; i < Rexon_SegmentCount; ++i) {
                        self->position.x = self->positions[i].x;
                        self->position.y = self->positions[i].y;
                        if (Player_CheckCollisionTouch(player, self, &Rexon->hitbox1))
                            Player_CheckHit(player, self);
                    }
                }
                self->position.x = storeX;
                self->position.y = storeY;
            }
        }
    }
}

void Rexon_Destroy(EntityRexon *rexon, bool32 crushed)
{
    if (rexon->state != Rexon_State_Destroyed && rexon->state != Rexon_State_Explode) {
        int x             = rexon->position.x;
        int y             = rexon->position.y;
        rexon->position.x = rexon->positions[0].x;
        rexon->position.y = rexon->positions[0].y;
#if RETRO_USE_PLUS
        CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Random(0, 32, &Zone->randKey) >> 4] + 1), rexon->position.x, rexon->position.y);
#else
        CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Rand(0, 32) >> 4] + 1), rexon->position.x, rexon->position.y);
#endif
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), rexon->position.x, rexon->position.y)->drawOrder = Zone->drawOrderHigh;
        RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
        rexon->position.x = x;
        rexon->position.y = y;

        int32 velocities[] = { -0x8000, 0x10000, -0x10000, 0x8000 };
        for (int i = 1; i < Rexon_SegmentCount; ++i) {
            EntityRexon *debris = CREATE_ENTITY(Rexon, intToVoid(1), rexon->positions[i].x, rexon->positions[i].y);
            debris->direction   = rexon->direction;
            debris->velocity.x  = velocities[(i - 1) & 3];
        }
    }
    if (crushed) {
        rexon->active       = ACTIVE_NORMAL;
        rexon->destroyDelay = 60;
        rexon->state        = Rexon_State_Explode;
    }
    else {
        rexon->state = Rexon_State_Destroyed;
    }
}

void Rexon_State_Setup(void)
{
    RSDK_THIS(Rexon);

    self->active = ACTIVE_NORMAL;
    self->state  = Rexon_State_Unknown1;
    Rexon_State_Unknown1();
}

void Rexon_State_Unknown1(void)
{
    RSDK_THIS(Rexon);

    if (self->noMove) {
        self->direction = Player_GetNearestPlayer()->position.x >= self->position.x;
    }
    else {
        self->position.x += self->velocity.x;
        if (--self->timer <= 0) {
            self->direction ^= FLIP_X;
            self->timer      = 128;
            self->velocity.x = -self->velocity.x;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Rexon->hitbox3)) {
            self->state     = Rexon_State_Unknown2;
            self->direction = self->direction = player->position.x >= self->position.x;
            foreach_break;
        }
    }

    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Unknown2(void)
{
    RSDK_THIS(Rexon);

    self->field_A0[self->segmentID] += 640;
    if (self->field_A0[self->segmentID] >= 0xF00) {
        self->field_A0[self->segmentID] = 0xF00;
        ++self->segmentID;
    }

    for (int i = self->segmentID - 1; i >= 0; --i) {
        self->field_B4[i] += 2;
    }

    if (self->segmentID == Rexon_SegmentCount)
        self->state = Rexon_State_Unknown3;
    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Unknown3(void)
{
    RSDK_THIS(Rexon);

    RSDK.ProcessAnimation(&self->animator3);

    for (int i = 0; i < Rexon_SegmentCount; ++i) {
        if (self->field_C8[i]) {
            if (--self->field_B4[i] <= 352)
                self->field_C8[i] = false;
        }
        else if (++self->field_B4[i] >= 416) {
            self->field_C8[i] = true;
        }
    }

    if (self->field_B4[0] == 0x160) {
        RSDK.PlaySfx(Rexon->sfxShot, false, 255);
        EntityRexon *shot = CREATE_ENTITY(Rexon, intToVoid(2), self->positions[0].x, self->positions[0].y);
        if (self->direction) {
            shot->position.x += 0xE0000;
            shot->velocity.x = 0x10000;
        }
        else {
            shot->position.x -= 0xE0000;
            shot->velocity.x = -0x10000;
        }
        shot->position.y += 0x60000;
        shot->velocity.y = 0x8000;
    }
    else if (self->field_B4[0] == 0x170) {
        if (self->field_C8[0] == 1)
            RSDK.SetSpriteAnimation(Rexon->aniFrames, 0, &self->animator3, true, 0);
    }
    else if (self->field_B4[0] == 0x180 && !self->field_C8[0]) {
        RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &self->animator3, true, 0);
    }
    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Destroyed(void)
{
    Rexon_CheckPlayerCollisions();
    Rexon_CheckOnScreen();
}

void Rexon_State_Explode(void)
{
    RSDK_THIS(Rexon);

    self->position.y += 0x4000;
    self->velocity.y = 0x4000;

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(Rexon->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x                      = self->position.x + (RSDK.Rand(Rexon->hitbox2.left, Rexon->hitbox2.right) << 16);
            int y                      = self->position.y + (RSDK.Rand(Rexon->hitbox2.top, Rexon->hitbox2.bottom) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }
    if (--self->destroyDelay <= 0)
        destroyEntity(self);
}

void Rexon_State1_Unknown(void)
{
    RSDK_THIS(Rexon);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Rexon_State2_Unknown(void)
{
    RSDK_THIS(Rexon);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Rexon->hitbox4)) {
            Player_CheckProjectileHit(player, self);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, Rexon->hitbox4.bottom << 13, 4)
        || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, Rexon->hitbox4.left << 13, 0, 4)
        || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, Rexon->hitbox4.top << 13, 4)
        || RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, Rexon->hitbox4.right << 13, 0, 4) || !RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Rexon_EditorDraw(void)
{
    RSDK_THIS(Rexon);

    for (int i = 0; i < Rexon_SegmentCount; ++i) {
        self->field_A0[i] = 0xF00;
        self->field_B4[i] = 0x170;
    }

    self->segmentID = 0;
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 3, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 1, &self->animator3, true, 2);
    RSDK.SetSpriteAnimation(Rexon->aniFrames, 2, &self->animator2, true, 0);

    self->positions[Rexon_SegmentCount].x = self->position.x;
    self->positions[Rexon_SegmentCount].y = self->position.y;
    if (self->direction)
        self->positions[Rexon_SegmentCount].x += 0x1C0000;
    else
        self->positions[Rexon_SegmentCount].x -= 0x1C0000;
    self->positions[Rexon_SegmentCount].y += 0x110000;

    for (int i = Rexon_SegmentCount - 1; i >= 0; --i) {
        int y = self->field_B4[i] >> 1;
        if (self->direction == FLIP_X)
            y = (0x80 - y) & 0xFF;
        self->positions[i].x = self->field_A0[i] * RSDK.Cos256(y) + self->positions[i + 1].x;
        self->positions[i].y = self->field_A0[i] * RSDK.Sin256(y) + self->positions[i + 1].y;
    }

    Rexon_Draw();
}

void Rexon_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Rexon.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Rexon->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Rexon.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Rexon, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Rexon_Serialize(void)
{
    RSDK_EDITABLE_VAR(Rexon, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Rexon, VAR_BOOL, noMove);
}
