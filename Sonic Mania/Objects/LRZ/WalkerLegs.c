// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WalkerLegs Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectWalkerLegs *WalkerLegs;

void WalkerLegs_Update(void)
{
    RSDK_THIS(WalkerLegs);

    StateMachine_Run(self->state);

    ++self->field_BC[0];
    ++self->field_BC[1];
    self->position.x = self->field_68[1].x;
    self->position.y = self->field_68[1].y;

    if (self->timer > 0)
        self->timer--;
}

void WalkerLegs_LateUpdate(void) {}

void WalkerLegs_StaticUpdate(void) {}

void WalkerLegs_Draw(void) { WalkerLegs_DrawSprites(); }

void WalkerLegs_Create(void *data)
{
    RSDK_THIS(WalkerLegs);

    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderLow;
    self->field_68[0].x = self->position.x;
    self->field_68[0].y = self->position.y;
    self->field_68[1]   = self->position;
    self->field_68[2]   = self->position;
    self->field_68[2].x += (2 * (self->direction == FLIP_NONE) - 1) << 22;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    self->state         = WalkerLegs_State_Unknown1;
}

void WalkerLegs_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ2/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ2/Particles.bin", SCOPE_STAGE);
    }

    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 0, &WalkerLegs->animator1, true, 0);
    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 1, &WalkerLegs->animator2, true, 0);
    WalkerLegs->hitbox.left    = -26;
    WalkerLegs->hitbox.right   = 27;
    WalkerLegs->hitbox.top     = -40;
    WalkerLegs->hitbox.bottom  = -32;
    WalkerLegs->sfxWalkerLegs  = RSDK.GetSfx("LRZ/WalkerLegs.wav");
    WalkerLegs->sfxWalkerLegs2 = RSDK.GetSfx("LRZ/WalkerLegs2.wav");
}

void WalkerLegs_DrawSprites(void)
{
    RSDK_THIS(WalkerLegs);
    Vector2 drawPos;

    RSDK.DrawSprite(&WalkerLegs->animator1, &self->field_68[1], false);
    RSDK.DrawSprite(&WalkerLegs->animator1, &self->field_68[2], false);
    drawPos = self->field_68[1];

    int moveX = (self->field_68[2].x - self->field_68[1].x) >> 3;
    int moveY = (self->field_68[2].y - self->field_68[1].y) >> 3;
    for (int i = 0; i < 9; ++i) {
        RSDK.DrawSprite(&WalkerLegs->animator2, &drawPos, false);
        drawPos.x += moveX;
        drawPos.y += moveY;
    }
}

void WalkerLegs_CheckOnScreen(void)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        if (!RSDK.CheckPosOnScreen(&self->field_68[0], &self->updateRange)) {
            self->position.x = self->field_68[0].x;
            self->position.y = self->field_68[0].y;
            self->visible    = false;
            self->state      = WalkerLegs_State_Unknown4;
        }
    }
}

void WalkerLegs_CheckPlayerCollisions(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *posPtrs[2];
    Vector2 *movePosPtrs[2];
    posPtrs[0]     = &self->field_68[1];
    posPtrs[1]     = &self->field_68[2];
    movePosPtrs[0] = &self->field_80[0];
    movePosPtrs[1] = &self->field_80[1];
    int storeX     = self->position.x;
    int storeY     = self->position.y;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        for (int l = 0; l < 2; ++l) {
            int moveX          = movePosPtrs[l]->x;
            int moveY          = movePosPtrs[l]->y;
            self->position.x = posPtrs[l]->x;
            self->position.y = posPtrs[l]->y;

            if ((1 << playerID) & self->activePlayers[l]) {
                player->position.x += moveX;
                player->position.y += moveY;
            }

            if (Player_CheckCollisionPlatform(player, self, &WalkerLegs->hitbox) == C_TOP)
                self->activePlayers[l] |= (1 << playerID);
            else
                self->activePlayers[l] &= ~(1 << playerID);
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void WalkerLegs_CheckObjectCrush(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *posPtrs[2];
    posPtrs[0]                = &self->field_68[1];
    posPtrs[1]                = &self->field_68[2];
    WalkerLegs->hitbox.top    = -WalkerLegs->hitbox.top;
    WalkerLegs->hitbox.bottom = -WalkerLegs->hitbox.bottom;
    int storeX                = self->position.x;
    int storeY                = self->position.y;

    foreach_active(Player, player)
    {
        for (int l = 0; l < 2; ++l) {
            self->position.x = posPtrs[l]->x;
            self->position.y = posPtrs[l]->y;

            if (Player_CheckCollisionBox(player, self, &WalkerLegs->hitbox) == C_BOTTOM)
                player->collisionFlagV |= 2;
        }
    }

    if (Rexon) {
        foreach_active(Rexon, rexon)
        {
            if (!rexon->type) {
                for (int l = 0; l < 2; ++l) {
                    self->position.x = posPtrs[l]->x;
                    self->position.y = posPtrs[l]->y;

                    if (RSDK.CheckObjectCollisionTouchBox(rexon, &Rexon->hitbox2, self, &WalkerLegs->hitbox)) {
                        Rexon_Destroy(rexon, true);
                        if (!WalkerLegs->hasAchievement) {
                            API_UnlockAchievement("ACH_LRZ");
                            WalkerLegs->hasAchievement = true;
                        }
                    }
                }
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
        for (int l = 0; l < 2; ++l) {
            self->position.x = posPtrs[l]->x;
            self->position.y = posPtrs[l]->y;

            if (RSDK.CheckObjectCollisionTouchBox(self, &WalkerLegs->hitbox, spikes, &spikeHitbox)) {
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
                self->timer = 8;
            }
        }
    }

    WalkerLegs->hitbox.top    = -WalkerLegs->hitbox.top;
    WalkerLegs->hitbox.bottom = -WalkerLegs->hitbox.bottom;
    self->position.x        = storeX;
    self->position.y        = storeY;
}

void WalkerLegs_CheckPlayerStood(void)
{
    RSDK_THIS(WalkerLegs);

    if (!self->steps || self->stepCount < self->steps) {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->sidekick == false && ((1 << playerID) & self->activePlayers[self->field_AC != 0])) {
                self->startAngle = self->angle;
                self->active     = ACTIVE_NORMAL;
                self->state      = WalkerLegs_State_Unknown3;
            }
        }
    }
}

void WalkerLegs_CheckTileCollisions(void)
{
    RSDK_THIS(WalkerLegs);

    self->field_B4 += 0x2000;
    self->angle += self->field_B4;

    int entityAngle = self->angle;
    int angle       = abs(self->angle - self->startAngle) >> 16;
    int x = 0, y = 0;
    if (self->field_AC) {
        x = self->field_68[1].x;
        y = self->field_68[1].y;
    }
    else {
        entityAngle -= 0x1000000;
        x = self->field_68[2].x;
        y = self->field_68[2].y;
    }

    int rx = abs(self->field_68[2].x - self->field_68[1].x);
    int ry = abs(self->field_68[2].y - self->field_68[1].y);

    uint16 radius = MathHelpers_SquareRoot((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16)) - 1;
    if (radius <= 0x40) {
        radius = 0x40;
    }

    int ang = entityAngle & 0x1FFFFFF;

    int newX = radius * (RSDK.Cos512(ang >> 16) << 7);
    int newY = radius * (RSDK.Sin512(ang >> 16) << 7);
    if (self->direction == FLIP_X)
        newX = -newX;

    self->position.x = (newX + x) & 0xFFFF0000;
    self->position.y = (newY + y) & 0xFFFF0000;

    bool32 flag = false;
    if (!self->field_AC) {
        self->field_68[1].x = self->position.x;
        self->field_68[1].y = self->position.y;
        if (angle > 128 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
            self->field_68[1].x = self->position.x;
            self->field_68[1].y = self->position.y;
            self->field_A0      = self->position.y + 0x280000;
            flag                  = true;
        }
    }
    else {
        self->field_68[2].x = self->position.x;
        self->field_68[2].y = self->position.y;
        if (angle > 128 && RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
            self->field_68[2].x = self->position.x;
            self->field_68[2].y = self->position.y;
            self->field_A4      = self->position.y + 0x280000;
            flag                  = true;
        }
    }

    if (flag) {
        self->field_B4 = 0;
        self->field_AC = self->field_AC == 0;
        Camera_ShakeScreen(0, 0, 5);
        ++self->stepCount;
        self->state = WalkerLegs_State_Unknown2;
        self->flag  = true;
        uint16 tile   = RSDK.GetTileInfo(Zone->fgLow, self->position.x >> 20, (self->position.y + 0x280000) >> 20);

        if (tile == 0xFFFF)
            tile = RSDK.GetTileInfo(Zone->fgHigh, self->position.x >> 20, (self->position.y + 0x280000) >> 20);

        uint8 behaviour = RSDK.GetTileBehaviour(tile, self->collisionPlane);
        if (behaviour == 1 || behaviour == 2) {
            RSDK.PlaySfx(WalkerLegs->sfxWalkerLegs2, false, 255);
            WalkerLegs_CreateDebris(self->field_AC == 0, true);
        }
        else {
            RSDK.PlaySfx(WalkerLegs->sfxWalkerLegs, false, 255);
            WalkerLegs_CreateDebris(self->field_AC == 0, false);
        }
        self->field_BC[self->field_AC == 0] = 0;
    }

    self->position.x = self->field_68[1].x;
    self->position.y = self->field_68[1].y;
}

void WalkerLegs_CheckStoodLava(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *posPtrs[2];
    posPtrs[0] = &self->field_68[1];
    posPtrs[1] = &self->field_68[2];

    for (int l = 0; l < 2; ++l) {
        int x = posPtrs[l]->x;
        int y = posPtrs[l]->y;
        int y2 = posPtrs[1 - l]->y;

        self->position.x = x;
        self->position.y = y;
        uint16 tile        = RSDK.GetTileInfo(Zone->fgLow, self->position.x >> 20, (self->position.y + 0x280000) >> 20);

        if (tile == 0xFFFF)
            tile = RSDK.GetTileInfo(Zone->fgHigh, self->position.x >> 20, (self->position.y + 0x280000) >> 20);
        uint8 behaviour = RSDK.GetTileBehaviour(tile, self->collisionPlane);
        if ((behaviour == 1 || behaviour == 2) && y - y2 < 0x500000) {
            self->flag = true;
            y += 0x2800;
            WalkerLegs_CreateSmoke(l == 1);
        }

        posPtrs[l]->x = x;
        posPtrs[l]->y = y;
    }

    self->position.x = self->field_68[1].x;
    self->position.y = self->field_68[1].y;
}

void WalkerLegs_CreateDebris(bool32 flag1, bool32 flag2)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckStageFolder("LRZ2") || flag2) {
        int x = 0, y = 0;
        if (flag1) {
            x = self->field_68[2].x;
            y = self->field_68[2].y;
        }
        else {
            x = self->field_68[1].x;
            y = self->field_68[1].y;
        }

        int size  = (WalkerLegs->hitbox.right - WalkerLegs->hitbox.left) >> 1;
        int count = RSDK.Rand(4, 6);
        int move  = (size << 17) / (count - 1);
        int pos   = 0;

        for (int i = 0; i < count; ++i) {
            int spawnX           = (self->position.x - (size << 16)) + pos + ((RSDK.Rand(0, 12) - 6) << 15);
            int spawnY           = ((RSDK.Rand(0, 8) - 4) << 15) + (y + 0x280000);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, spawnY);
            RSDK.SetSpriteAnimation(WalkerLegs->particleFrames, !flag2, &debris->animator, true, 0);

            debris->drawOrder  = Zone->drawOrderHigh;
            debris->gravity    = 0x3800;
            debris->velocity.x = 0x180 * (abs(spawnX - x) >> 8) / size;
            if (debris->position.x < self->position.x) {
                debris->direction  = FLIP_X;
                debris->velocity.x = -debris->velocity.x;
            }

            debris->velocity.y = -0x1000 * RSDK.Rand(32, 54);
            if (flag2)
                debris->velocity.y >>= 1;

            pos += move;
        }
    }
}

void WalkerLegs_CreateSmoke(bool32 flag)
{
    RSDK_THIS(WalkerLegs);

    if (self->field_BC[flag != 0] >= 5) {
        if (!(self->field_BC[flag != 0] % 5)) {
            int count = RSDK.Rand(1, 2);
            for (int i = 0; i < count; ++i) {
                int offsetX = 0;
                int spawnX = 0, spawnY = 0;
                if (flag)
                    offsetX = self->field_68[2].x;
                else
                    offsetX = self->field_68[1].x;
                spawnX = (RSDK.Rand(WalkerLegs->hitbox.left, WalkerLegs->hitbox.right) << 16) + offsetX;
                if (flag)
                    spawnY = self->field_A4;
                else
                    spawnY = self->field_A0;
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, spawnX, spawnY);
                RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
                debris->velocity.x = 0;
                debris->velocity.y = -0x1000 * RSDK.Rand(0, 4);
                debris->drawOrder  = Zone->drawOrderHigh - 1;
                debris->timer      = 30;
            }
        }
    }
}

void WalkerLegs_State_Unknown1(void)
{
    RSDK_THIS(WalkerLegs);

    self->stepCount        = 0;
    self->field_B4         = 0;
    self->field_AC         = 0;
    self->angle            = 0;
    self->flag             = false;
    self->visible          = true;
    self->active           = ACTIVE_BOUNDS;
    self->activePlayers[0] = 0;
    self->activePlayers[1] = 0;
    self->position.x       = self->field_68[0].x;
    self->position.y       = self->field_68[0].y;
    self->field_68[1].x    = self->field_68[0].x;
    self->field_68[1].y    = self->field_68[0].y;
    self->field_68[2].x    = self->field_68[0].x;
    self->field_68[2].y    = self->field_68[0].y;
    self->field_A0         = self->field_68[1].y;
    self->field_80[0].x    = 0;
    self->field_80[0].y    = 0;
    self->field_80[1].x    = 0;
    self->field_80[1].y    = 0;
    self->field_68[2].x += (2 * (self->direction == FLIP_NONE) - 1) << 22;
    self->field_A0 -= (WalkerLegs->hitbox.top << 16);
    self->field_A4 = self->field_68[2].y - (WalkerLegs->hitbox.top << 16);
    self->state    = WalkerLegs_State_Unknown2;
}

void WalkerLegs_State_Unknown2(void)
{
    RSDK_THIS(WalkerLegs);

    self->angle &= 0x1FFFFFF;
    self->field_90[0].x = self->field_68[1].x;
    self->field_90[0].y = self->field_68[1].y;
    self->field_90[1].x = self->field_68[2].x;
    self->field_90[1].y = self->field_68[2].y;
    WalkerLegs_CheckStoodLava();

    self->field_80[0].x = self->field_68[1].x - self->field_90[0].x;
    self->field_80[0].y = self->field_68[1].y - self->field_90[0].y;
    self->field_80[1].x = self->field_68[2].x - self->field_90[1].x;
    self->field_80[1].y = self->field_68[2].y - self->field_90[1].y;
    WalkerLegs_CheckPlayerCollisions();

    if (self->flag) {
        int x = 0;
        if (self->direction)
            x = self->field_68[1].x - self->field_68[2].x;
        else
            x = self->field_68[2].x - self->field_68[1].x;
        self->flag  = false;
        self->angle = RSDK.ATan2(x, self->field_68[2].y - self->field_68[1].y) << 17;
    }
    WalkerLegs_CheckPlayerStood();
    WalkerLegs_CheckOnScreen();
}

void WalkerLegs_State_Unknown3(void)
{
    RSDK_THIS(WalkerLegs);

    self->field_90[0]   = self->field_68[1];
    self->field_90[1].x = self->field_68[2].x;
    self->field_90[1].y = self->field_68[2].y;
    self->flag          = false;
    WalkerLegs_CheckStoodLava();
    WalkerLegs_CheckTileCollisions();
    self->field_80[0].x = self->field_68[1].x - self->field_90[0].x;
    self->field_80[0].y = self->field_68[1].y - self->field_90[0].y;
    self->field_80[1].x = self->field_68[2].x - self->field_90[1].x;
    self->field_80[1].y = self->field_68[2].y - self->field_90[1].y;
    WalkerLegs_CheckPlayerCollisions();
    WalkerLegs_CheckObjectCrush();
    WalkerLegs_CheckOnScreen();
}

void WalkerLegs_State_Unknown4(void)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        self->state = WalkerLegs_State_Unknown1;
}

#if RETRO_INCLUDE_EDITOR
void WalkerLegs_EditorDraw(void)
{
    RSDK_THIS(WalkerLegs);

    self->field_68[0].x = self->position.x;
    self->field_68[0].y = self->position.y;
    self->field_68[1]   = self->position;
    self->field_68[2]   = self->position;
    self->field_68[2].x += (2 * (self->direction == FLIP_NONE) - 1) << 22;

    self->inkEffect = INK_NONE;
    WalkerLegs_DrawSprites();

    //Draw Distance
    if (showGizmos()) {
        int dist = (0x40 * self->steps) << 16;

        self->inkEffect = INK_BLEND;
        self->field_68[1].x += self->direction ? -dist : dist;
        self->field_68[2].x += self->direction ? -dist : dist;
        WalkerLegs_DrawSprites();

        RSDK.DrawLine(self->position.x, self->position.y, self->field_68[1].x, self->field_68[1].y, 0x00FF00, 0xFF, INK_NONE, false);
    }
}

void WalkerLegs_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ2/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ2/Particles.bin", SCOPE_STAGE);
    }

    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 0, &WalkerLegs->animator1, true, 0);
    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 1, &WalkerLegs->animator2, true, 0);

    RSDK_ACTIVE_VAR(WalkerLegs, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void WalkerLegs_Serialize(void)
{
    RSDK_EDITABLE_VAR(WalkerLegs, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(WalkerLegs, VAR_UINT16, steps);
}
