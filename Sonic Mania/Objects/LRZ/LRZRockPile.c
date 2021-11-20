#include "SonicMania.h"

ObjectLRZRockPile *LRZRockPile;

void LRZRockPile_Update(void)
{
    RSDK_THIS(LRZRockPile);
    StateMachine_Run(self->state);

    if (self->timer > 0)
        self->timer--;
}

void LRZRockPile_LateUpdate(void) {}

void LRZRockPile_StaticUpdate(void) {}

void LRZRockPile_Draw(void)
{
    RSDK_THIS(LRZRockPile);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void LRZRockPile_Create(void *data)
{
    RSDK_THIS(LRZRockPile);

    self->active = ACTIVE_BOUNDS;
    if (self->planeFilter > 0 && ((uint8)(self->planeFilter - 1) & 2))
        self->drawOrder = Zone->playerDrawHigh;
    else
        self->drawOrder = Zone->drawOrderLow;

    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    switch (self->type) {
        case 0:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &self->animator, true, 0);
            self->hitbox = LRZRockPile->hitbox1;
            self->state  = LRZRockPile_State_Type0;
            break;
        case 1:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0);
            self->hitbox = LRZRockPile->hitbox2;
            self->state  = LRZRockPile_State_Type1;
            break;
        case 2:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0);
            self->hitbox = LRZRockPile->hitbox3;
            self->state  = LRZRockPile_State_Type2;
            break;
    }
}

void LRZRockPile_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1")) {
        LRZRockPile->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/LRZRockPile.bin", SCOPE_STAGE);
        LRZRockPile->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
        LRZRockPile->hitbox1.left   = -16;
        LRZRockPile->hitbox1.top    = -40;
        LRZRockPile->hitbox1.right  = 16;
        LRZRockPile->hitbox1.bottom = 40;
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        LRZRockPile->aniFrames      = RSDK.LoadSpriteAnimation("LRZ2/LRZRockPile.bin", SCOPE_STAGE);
        LRZRockPile->hitbox1.left   = -16;
        LRZRockPile->hitbox1.top    = -32;
        LRZRockPile->hitbox1.right  = 16;
        LRZRockPile->hitbox1.bottom = 32;
    }

    LRZRockPile->sfxLedgeBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");

    LRZRockPile->hitbox2.left   = -64;
    LRZRockPile->hitbox2.top    = -32;
    LRZRockPile->hitbox2.right  = 64;
    LRZRockPile->hitbox2.bottom = -16;

    LRZRockPile->hitbox3.left   = -64;
    LRZRockPile->hitbox3.top    = -32;
    LRZRockPile->hitbox3.right  = 64;
    LRZRockPile->hitbox3.bottom = 32;
}

void LRZRockPile_SetupDebris(void)
{
    RSDK_THIS(LRZRockPile);

    switch (self->type) {
        case 0:
            self->debrisCount = 6;

            self->debrisInfo[0] = 3;
            self->debrisInfo[1] = 2;
            self->debrisInfo[2] = -0x10000;
            self->debrisInfo[3] = -0x190000;
            self->debrisInfo[4] = FLIP_NONE;
            self->debrisInfo[5] = 0;

            self->debrisInfo[6]  = 3;
            self->debrisInfo[7]  = 4;
            self->debrisInfo[8]  = 0;
            self->debrisInfo[9]  = 0x190000;
            self->debrisInfo[10] = FLIP_NONE;
            self->debrisInfo[11] = 0;

            self->debrisInfo[12] = 2;
            self->debrisInfo[13] = 0;
            self->debrisInfo[14] = -0x80000;
            self->debrisInfo[15] = -0x100000;
            self->debrisInfo[16] = FLIP_NONE;
            self->debrisInfo[17] = 0;

            self->debrisInfo[18] = 2;
            self->debrisInfo[19] = 2;
            self->debrisInfo[20] = 0x80000;
            self->debrisInfo[21] = 0x200000;
            self->debrisInfo[22] = FLIP_NONE;
            self->debrisInfo[23] = 0;

            self->debrisInfo[24] = 3;
            self->debrisInfo[25] = 0;
            self->debrisInfo[26] = -0x10000;
            self->debrisInfo[27] = -0x20000;
            self->debrisInfo[28] = FLIP_NONE;
            self->debrisInfo[29] = 0;

            self->debrisInfo[30] = 3;
            self->debrisInfo[31] = 2;
            self->debrisInfo[32] = -0x10000;
            self->debrisInfo[33] = 0xC0000;
            self->debrisInfo[34] = FLIP_NONE;
            self->debrisInfo[35] = 0;
            break;
        case 1:
        case 2:
            self->debrisCount   = 22;
            self->debrisInfo[0] = 3;
            self->debrisInfo[1] = 3;
            self->debrisInfo[2] = -0x2D0000;
            self->debrisInfo[3] = 0xB0000;
            self->debrisInfo[4] = FLIP_X;
            self->debrisInfo[5] = 9;

            self->debrisInfo[6]  = 3;
            self->debrisInfo[7]  = 3;
            self->debrisInfo[8]  = 0x2D0000;
            self->debrisInfo[9]  = 0xB0000;
            self->debrisInfo[10] = FLIP_NONE;
            self->debrisInfo[11] = 9;

            self->debrisInfo[12] = 3;
            self->debrisInfo[13] = 3;
            self->debrisInfo[14] = -0x200000;
            self->debrisInfo[15] = -0x100000;
            self->debrisInfo[16] = FLIP_X;
            self->debrisInfo[17] = 30;

            self->debrisInfo[18] = 3;
            self->debrisInfo[19] = 3;
            self->debrisInfo[20] = 0x200000;
            self->debrisInfo[21] = -0x100000;
            self->debrisInfo[22] = FLIP_NONE;
            self->debrisInfo[23] = 30;

            self->debrisInfo[24] = 2;
            self->debrisInfo[25] = 0;
            self->debrisInfo[26] = 0;
            self->debrisInfo[27] = 0x180000;
            self->debrisInfo[28] = FLIP_XY;
            self->debrisInfo[29] = 0;

            self->debrisInfo[30] = 2;
            self->debrisInfo[31] = 0;
            self->debrisInfo[32] = -0x370000;
            self->debrisInfo[33] = 0x10000;
            self->debrisInfo[34] = 0;
            self->debrisInfo[35] = 20;

            self->debrisInfo[36] = 2;
            self->debrisInfo[37] = 1;
            self->debrisInfo[38] = -0x300000;
            self->debrisInfo[39] = 0xF0000;
            self->debrisInfo[40] = FLIP_NONE;
            self->debrisInfo[41] = 14;

            self->debrisInfo[42] = 2;
            self->debrisInfo[43] = 3;
            self->debrisInfo[44] = -0x220000;
            self->debrisInfo[45] = 0x180000;
            self->debrisInfo[46] = FLIP_X;
            self->debrisInfo[47] = 6;

            self->debrisInfo[48] = 1;
            self->debrisInfo[49] = 5;
            self->debrisInfo[50] = -0x160000;
            self->debrisInfo[51] = 0x130000;
            self->debrisInfo[52] = FLIP_NONE;
            self->debrisInfo[53] = 12;

            self->debrisInfo[54] = 2;
            self->debrisInfo[55] = 0;
            self->debrisInfo[56] = 3604480;
            self->debrisInfo[57] = 0x10000;
            self->debrisInfo[58] = FLIP_X;
            self->debrisInfo[59] = 20;

            self->debrisInfo[60] = 2;
            self->debrisInfo[61] = 1;
            self->debrisInfo[62] = 0x300000;
            self->debrisInfo[63] = 0xF0000;
            self->debrisInfo[64] = FLIP_X;
            self->debrisInfo[65] = 14;

            self->debrisInfo[66] = 2;
            self->debrisInfo[67] = 3;
            self->debrisInfo[68] = 0x220000;
            self->debrisInfo[69] = 0x180000;
            self->debrisInfo[70] = FLIP_NONE;
            self->debrisInfo[71] = 6;

            self->debrisInfo[72] = 1;
            self->debrisInfo[73] = 5;
            self->debrisInfo[74] = 0x160000;
            self->debrisInfo[75] = 0x130000;
            self->debrisInfo[76] = FLIP_X;
            self->debrisInfo[77] = 12;

            self->debrisInfo[78] = 2;
            self->debrisInfo[79] = 2;
            self->debrisInfo[80] = -0xA0000;
            self->debrisInfo[81] = 0x140000;
            self->debrisInfo[82] = FLIP_NONE;
            self->debrisInfo[83] = 8;

            self->debrisInfo[84] = 2;
            self->debrisInfo[85] = 2;
            self->debrisInfo[86] = 0xA0000;
            self->debrisInfo[87] = 0x140000;
            self->debrisInfo[88] = FLIP_X;
            self->debrisInfo[89] = 7;

            self->debrisInfo[90] = 3;
            self->debrisInfo[91] = 0;
            self->debrisInfo[92] = -0x310000;
            self->debrisInfo[93] = -0x110000;
            self->debrisInfo[94] = FLIP_NONE;
            self->debrisInfo[95] = 26;

            self->debrisInfo[96]  = 3;
            self->debrisInfo[97]  = 0;
            self->debrisInfo[98]  = 0xF0000;
            self->debrisInfo[99]  = -0x110000;
            self->debrisInfo[100] = FLIP_NONE;
            self->debrisInfo[101] = 24;

            self->debrisInfo[102] = 3;
            self->debrisInfo[103] = 0;
            self->debrisInfo[104] = -0xF0000;
            self->debrisInfo[105] = -0x110000;
            self->debrisInfo[106] = FLIP_X;
            self->debrisInfo[107] = 24;

            self->debrisInfo[108] = 3;
            self->debrisInfo[109] = 2;
            self->debrisInfo[110] = 0x310000;
            self->debrisInfo[111] = -0x110000;
            self->debrisInfo[112] = FLIP_NONE;
            self->debrisInfo[113] = 26;

            self->debrisInfo[114] = 3;
            self->debrisInfo[115] = 0;
            self->debrisInfo[116] = -0x210000;
            self->debrisInfo[117] = 0x20000;
            self->debrisInfo[118] = FLIP_NONE;
            self->debrisInfo[119] = 18;

            self->debrisInfo[120] = 3;
            self->debrisInfo[121] = 0;
            self->debrisInfo[122] = 0x210000;
            self->debrisInfo[123] = 0x20000;
            self->debrisInfo[124] = FLIP_X;
            self->debrisInfo[125] = 17;

            self->debrisInfo[126] = 3;
            self->debrisInfo[127] = 1;
            self->debrisInfo[128] = 0;
            self->debrisInfo[129] = 0x80000;
            self->debrisInfo[130] = FLIP_NONE;
            self->debrisInfo[131] = 16;
            break;
    }
}

void LRZRockPile_SpawnRockDebris(int x, int y, int velX, int velY)
{
    RSDK_THIS(LRZRockPile);
    LRZRockPile_SetupDebris();

    int sizeX = (self->hitbox.right - self->hitbox.left) << 16;
    int sizeY = (self->hitbox.bottom - self->hitbox.top) << 16;

    int destroyDelay = 0;
    if (RSDK.CheckStageFolder("LRZ1")) {
        for (int i = 0; i < self->debrisCount; ++i) {
            int animationID = self->debrisInfo[(i * 6) + 0];
            int frameID     = self->debrisInfo[(i * 6) + 1];
            int offsetX     = self->debrisInfo[(i * 6) + 2];
            int offsetY     = self->debrisInfo[(i * 6) + 3];
            int direction   = self->debrisInfo[(i * 6) + 4];
            int delay       = self->debrisInfo[(i * 6) + 5];

            if (!offsetX && !offsetY)
                break;

            int v19              = self->position.x + offsetX;
            int v17              = self->position.y + offsetY;
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, v19, v17);
            RSDK.SetSpriteAnimation(LRZRockPile->particleFrames, animationID, &debris->animator, true, frameID);
            debris->drawOrder     = self->drawOrder;
            debris->delay         = delay;
            debris->direction     = direction;
            debris->gravity       = 0x3800;
            debris->updateRange.x = 0x800000;
            debris->updateRange.y = 0x800000;

            debris->velocity.x = (velX >> 8) * ((abs(x - v19) / (sizeX >> 16)) >> 8);
            if (velX)
                debris->velocity.x += ((RSDK.Rand(0, 16) + 8) * (2 * (velX > 1) - 1)) << 12;
            debris->velocity.y = (velY >> 8) * (((y - v17) / (sizeY >> 16)) >> 8);
            if (velY)
                debris->velocity.y += ((RSDK.Rand(0, 16) + 8) * (2 * (velY > 1) - 1)) << 12;

            if (delay > destroyDelay)
                destroyDelay = delay;
        }
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        int spawnY = self->position.y - 0x100000;

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x, spawnY);
        RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &debris->animator, true, 1);
        debris->drawFX        = FX_ROTATE | FX_FLIP;
        debris->drawOrder     = self->drawOrder;
        debris->gravity       = 0x3800;
        debris->direction     = self->direction;
        debris->updateRange.x = 0x800000;
        debris->updateRange.y = 0x800000;
        debris->rotSpeed      = 8 * (velX <= 0) - 4;

        debris->velocity.x = (velX >> 8) * (abs(abs(x - self->position.x) / (sizeX >> 16)) >> 8);
        if (velX)
            debris->velocity.x += ((RSDK.Rand(0, 16) + 8) * (2 * (velX > 1) - 1)) << 12;

        debris->velocity.y = (velY >> 8) * (((y - spawnY) / (sizeY >> 16)) >> 8);
        if (velY)
            debris->velocity.y += ((RSDK.Rand(0, 16) + 8) * (2 * (velY > 1) - 1)) << 12;

        sizeX >>= 16;
        sizeY >>= 16;
        spawnY = self->position.y + 0x100000;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x, spawnY);
        RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &debris->animator, true, 2);
        debris->drawFX        = FX_ROTATE | FX_FLIP;
        debris->drawOrder     = self->drawOrder;
        debris->gravity       = 0x3800;
        debris->direction     = self->direction;
        debris->updateRange.x = 0x800000;
        debris->updateRange.y = 0x800000;
        debris->rotSpeed      = 8 * (velX > 0) - 4;
        debris->velocity.x    = (velX >> 8) * ((abs(x - self->position.x) / sizeX) >> 8);
        if (velX)
            debris->velocity.x += ((RSDK.Rand(0, 16) + 8) * (2 * (velX > 1) - 1)) << 12;
        debris->velocity.y = (velY >> 8) * (((y - spawnY) / sizeY) >> 8);
        if (velY)
            debris->velocity.y += ((RSDK.Rand(0, 16) + 8) * (2 * (velY > 1) - 1)) << 12;
    }
    self->timer2 = destroyDelay;

    RSDK.PlaySfx(LRZRockPile->sfxLedgeBreak, false, 255);
    if (self->timer2 > 0) {
        self->visible = 0;
        self->state   = LRZRockPile_State_Broken;
    }
    else {
        destroyEntity(self);
    }
}

void LRZRockPile_State_Type0(void)
{
    RSDK_THIS(LRZRockPile);

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
            if ((!self->onlyKnux || player->characterID == ID_KNUCKLES)
#if RETRO_USE_PLUS
                || (!self->onlyMighty || player->characterID == ID_MIGHTY)
#endif
            ) {
                bool32 flag = abs(player->groundVel) >= 0x48000 && player->onGround && player->animator.animationID == ANI_JUMP;
                if (player->shield == SHIELD_FIRE) {
                    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                    flag |= shield->animator.animationID == 2;
                }

                flag |= player->characterID == ID_KNUCKLES;
                if (player->characterID == ID_SONIC && player->superState == SUPERSTATE_SUPER)
                    flag |= true;

                if ((flag && !player->sidekick) && !self->unbreakable) {
                    if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                        if (player->position.x > self->position.x)
                            LRZRockPile_SpawnRockDebris(player->position.x, player->position.y, -0x40000, -0x20000);
                        else
                            LRZRockPile_SpawnRockDebris(player->position.x, player->position.y, 0x40000, -0x20000);
                    }
                    continue;
                }
            }
            Player_CheckCollisionBox(player, self, &self->hitbox);
        }
    }
}

void LRZRockPile_State_Type1(void)
{
    RSDK_THIS(LRZRockPile);
    if (self->flag && self->timer <= 0) {
        LRZRockPile_SpawnRockDebris(0, 0, 0, 0);
    }
    else {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionPlatform(player, self, &self->hitbox)
#if RETRO_USE_PLUS
                    && (!self->onlyMighty || player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
#endif
                    && !player->sidekick && !self->unbreakable && player->onGround) {
                    if (self->delay != 0xFFFF) {
                        if (!self->flag) {
                            self->flag  = true;
                            self->timer = self->delay;
                        }
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop)
                            self->timer = 1;
#endif
                    }
                }
            }
        }
    }
}

void LRZRockPile_State_Type2(void)
{
    RSDK_THIS(LRZRockPile);

    if (self->flag && self->timer <= 0) {
        LRZRockPile_SpawnRockDebris(0, 0, 0, 0);
    }
    else {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP
#if RETRO_USE_PLUS
                    && (!self->onlyMighty || player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
#endif
                    && !player->sidekick && !self->unbreakable && player->onGround) {
                    if (self->delay != 0xFFFF) {
                        if (!self->flag) {
                            self->flag  = true;
                            self->timer = self->delay;
                        }
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop)
                            self->timer = 1;
#endif
                    }
                }
            }
        }
    }
}

void LRZRockPile_State_Broken(void)
{
    RSDK_THIS(LRZRockPile);

    if (self->timer2 <= 0) {
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
                Player_CheckCollisionBox(player, self, &self->hitbox);
        }
    }

    --self->timer2;
}

#if RETRO_INCLUDE_EDITOR
void LRZRockPile_EditorDraw(void)
{
    RSDK_THIS(LRZRockPile);

    switch (self->type) {
        case 0: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &self->animator, true, 0); break;
        case 1: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0); break;
        case 2: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0); break;
    }

    LRZRockPile_Draw();
}

void LRZRockPile_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1")) {
        LRZRockPile->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/LRZRockPile.bin", SCOPE_STAGE);
        LRZRockPile->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("LRZ2")) {
        LRZRockPile->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZRockPile.bin", SCOPE_STAGE);
    }

    RSDK_ACTIVE_VAR(LRZRockPile, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}
#endif

void LRZRockPile_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_UINT16, delay);
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_BOOL, debugParticles);
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_BOOL, unbreakable);
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_BOOL, onlyKnux);
#if RETRO_USE_PLUS
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_BOOL, onlyMighty);
#endif
}
