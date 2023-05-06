// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZRockPile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->drawGroup = Zone->playerDrawGroup[1];
    else
        self->drawGroup = Zone->objectDrawGroup[0];

    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    switch (self->type) {
        case LRZROCKPILE_WALL:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &self->animator, true, 0);
            self->hitbox = LRZRockPile->hitboxWall;
            self->state  = LRZRockPile_State_Wall;
            break;

        case LRZROCKPILE_FLOOR_TOPSOLID:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0);
            self->hitbox = LRZRockPile->hitboxFloorTopSolid;
            self->state  = LRZRockPile_State_Floor_TopSolid;
            break;

        case LRZROCKPILE_FLOOR_ALLSOLID:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0);
            self->hitbox = LRZRockPile->hitboxFloorAllSolid;
            self->state  = LRZRockPile_State_Floor_AllSolid;
            break;
    }
}

void LRZRockPile_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1")) {
        LRZRockPile->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/LRZRockPile.bin", SCOPE_STAGE);
        LRZRockPile->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);

        LRZRockPile->hitboxWall.left   = -16;
        LRZRockPile->hitboxWall.top    = -40;
        LRZRockPile->hitboxWall.right  = 16;
        LRZRockPile->hitboxWall.bottom = 40;
    }
    else if (RSDK.CheckSceneFolder("LRZ2")) {
        LRZRockPile->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZRockPile.bin", SCOPE_STAGE);

        LRZRockPile->hitboxWall.left   = -16;
        LRZRockPile->hitboxWall.top    = -32;
        LRZRockPile->hitboxWall.right  = 16;
        LRZRockPile->hitboxWall.bottom = 32;
    }

    LRZRockPile->sfxLedgeBreak = RSDK.GetSfx("Stage/LedgeBreak.wav");

    LRZRockPile->hitboxFloorTopSolid.left   = -64;
    LRZRockPile->hitboxFloorTopSolid.top    = -32;
    LRZRockPile->hitboxFloorTopSolid.right  = 64;
    LRZRockPile->hitboxFloorTopSolid.bottom = -16;

    LRZRockPile->hitboxFloorAllSolid.left   = -64;
    LRZRockPile->hitboxFloorAllSolid.top    = -32;
    LRZRockPile->hitboxFloorAllSolid.right  = 64;
    LRZRockPile->hitboxFloorAllSolid.bottom = 32;
}

void LRZRockPile_SetupDebris(void)
{
    RSDK_THIS(LRZRockPile);

    switch (self->type) {
        case LRZROCKPILE_WALL:
            self->debrisCount = 6;

            self->rockDebrisInfo[0] = 3;
            self->rockDebrisInfo[1] = 2;
            self->rockDebrisInfo[2] = -0x10000;
            self->rockDebrisInfo[3] = -0x190000;
            self->rockDebrisInfo[4] = FLIP_NONE;
            self->rockDebrisInfo[5] = 0;

            self->rockDebrisInfo[6]  = 3;
            self->rockDebrisInfo[7]  = 4;
            self->rockDebrisInfo[8]  = 0;
            self->rockDebrisInfo[9]  = 0x190000;
            self->rockDebrisInfo[10] = FLIP_NONE;
            self->rockDebrisInfo[11] = 0;

            self->rockDebrisInfo[12] = 2;
            self->rockDebrisInfo[13] = 0;
            self->rockDebrisInfo[14] = -0x80000;
            self->rockDebrisInfo[15] = -0x100000;
            self->rockDebrisInfo[16] = FLIP_NONE;
            self->rockDebrisInfo[17] = 0;

            self->rockDebrisInfo[18] = 2;
            self->rockDebrisInfo[19] = 2;
            self->rockDebrisInfo[20] = 0x80000;
            self->rockDebrisInfo[21] = 0x200000;
            self->rockDebrisInfo[22] = FLIP_NONE;
            self->rockDebrisInfo[23] = 0;

            self->rockDebrisInfo[24] = 3;
            self->rockDebrisInfo[25] = 0;
            self->rockDebrisInfo[26] = -0x10000;
            self->rockDebrisInfo[27] = -0x20000;
            self->rockDebrisInfo[28] = FLIP_NONE;
            self->rockDebrisInfo[29] = 0;

            self->rockDebrisInfo[30] = 3;
            self->rockDebrisInfo[31] = 2;
            self->rockDebrisInfo[32] = -0x10000;
            self->rockDebrisInfo[33] = 0xC0000;
            self->rockDebrisInfo[34] = FLIP_NONE;
            self->rockDebrisInfo[35] = 0;
            break;

        case LRZROCKPILE_FLOOR_TOPSOLID:
        case LRZROCKPILE_FLOOR_ALLSOLID:
            self->debrisCount       = 22;
            self->rockDebrisInfo[0] = 3;
            self->rockDebrisInfo[1] = 3;
            self->rockDebrisInfo[2] = -0x2D0000;
            self->rockDebrisInfo[3] = 0xB0000;
            self->rockDebrisInfo[4] = FLIP_X;
            self->rockDebrisInfo[5] = 9;

            self->rockDebrisInfo[6]  = 3;
            self->rockDebrisInfo[7]  = 3;
            self->rockDebrisInfo[8]  = 0x2D0000;
            self->rockDebrisInfo[9]  = 0xB0000;
            self->rockDebrisInfo[10] = FLIP_NONE;
            self->rockDebrisInfo[11] = 9;

            self->rockDebrisInfo[12] = 3;
            self->rockDebrisInfo[13] = 3;
            self->rockDebrisInfo[14] = -0x200000;
            self->rockDebrisInfo[15] = -0x100000;
            self->rockDebrisInfo[16] = FLIP_X;
            self->rockDebrisInfo[17] = 30;

            self->rockDebrisInfo[18] = 3;
            self->rockDebrisInfo[19] = 3;
            self->rockDebrisInfo[20] = 0x200000;
            self->rockDebrisInfo[21] = -0x100000;
            self->rockDebrisInfo[22] = FLIP_NONE;
            self->rockDebrisInfo[23] = 30;

            self->rockDebrisInfo[24] = 2;
            self->rockDebrisInfo[25] = 0;
            self->rockDebrisInfo[26] = 0;
            self->rockDebrisInfo[27] = 0x180000;
            self->rockDebrisInfo[28] = FLIP_XY;
            self->rockDebrisInfo[29] = 0;

            self->rockDebrisInfo[30] = 2;
            self->rockDebrisInfo[31] = 0;
            self->rockDebrisInfo[32] = -0x370000;
            self->rockDebrisInfo[33] = 0x10000;
            self->rockDebrisInfo[34] = 0;
            self->rockDebrisInfo[35] = 20;

            self->rockDebrisInfo[36] = 2;
            self->rockDebrisInfo[37] = 1;
            self->rockDebrisInfo[38] = -0x300000;
            self->rockDebrisInfo[39] = 0xF0000;
            self->rockDebrisInfo[40] = FLIP_NONE;
            self->rockDebrisInfo[41] = 14;

            self->rockDebrisInfo[42] = 2;
            self->rockDebrisInfo[43] = 3;
            self->rockDebrisInfo[44] = -0x220000;
            self->rockDebrisInfo[45] = 0x180000;
            self->rockDebrisInfo[46] = FLIP_X;
            self->rockDebrisInfo[47] = 6;

            self->rockDebrisInfo[48] = 1;
            self->rockDebrisInfo[49] = 5;
            self->rockDebrisInfo[50] = -0x160000;
            self->rockDebrisInfo[51] = 0x130000;
            self->rockDebrisInfo[52] = FLIP_NONE;
            self->rockDebrisInfo[53] = 12;

            self->rockDebrisInfo[54] = 2;
            self->rockDebrisInfo[55] = 0;
            self->rockDebrisInfo[56] = 3604480;
            self->rockDebrisInfo[57] = 0x10000;
            self->rockDebrisInfo[58] = FLIP_X;
            self->rockDebrisInfo[59] = 20;

            self->rockDebrisInfo[60] = 2;
            self->rockDebrisInfo[61] = 1;
            self->rockDebrisInfo[62] = 0x300000;
            self->rockDebrisInfo[63] = 0xF0000;
            self->rockDebrisInfo[64] = FLIP_X;
            self->rockDebrisInfo[65] = 14;

            self->rockDebrisInfo[66] = 2;
            self->rockDebrisInfo[67] = 3;
            self->rockDebrisInfo[68] = 0x220000;
            self->rockDebrisInfo[69] = 0x180000;
            self->rockDebrisInfo[70] = FLIP_NONE;
            self->rockDebrisInfo[71] = 6;

            self->rockDebrisInfo[72] = 1;
            self->rockDebrisInfo[73] = 5;
            self->rockDebrisInfo[74] = 0x160000;
            self->rockDebrisInfo[75] = 0x130000;
            self->rockDebrisInfo[76] = FLIP_X;
            self->rockDebrisInfo[77] = 12;

            self->rockDebrisInfo[78] = 2;
            self->rockDebrisInfo[79] = 2;
            self->rockDebrisInfo[80] = -0xA0000;
            self->rockDebrisInfo[81] = 0x140000;
            self->rockDebrisInfo[82] = FLIP_NONE;
            self->rockDebrisInfo[83] = 8;

            self->rockDebrisInfo[84] = 2;
            self->rockDebrisInfo[85] = 2;
            self->rockDebrisInfo[86] = 0xA0000;
            self->rockDebrisInfo[87] = 0x140000;
            self->rockDebrisInfo[88] = FLIP_X;
            self->rockDebrisInfo[89] = 7;

            self->rockDebrisInfo[90] = 3;
            self->rockDebrisInfo[91] = 0;
            self->rockDebrisInfo[92] = -0x310000;
            self->rockDebrisInfo[93] = -0x110000;
            self->rockDebrisInfo[94] = FLIP_NONE;
            self->rockDebrisInfo[95] = 26;

            self->rockDebrisInfo[96]  = 3;
            self->rockDebrisInfo[97]  = 0;
            self->rockDebrisInfo[98]  = 0xF0000;
            self->rockDebrisInfo[99]  = -0x110000;
            self->rockDebrisInfo[100] = FLIP_NONE;
            self->rockDebrisInfo[101] = 24;

            self->rockDebrisInfo[102] = 3;
            self->rockDebrisInfo[103] = 0;
            self->rockDebrisInfo[104] = -0xF0000;
            self->rockDebrisInfo[105] = -0x110000;
            self->rockDebrisInfo[106] = FLIP_X;
            self->rockDebrisInfo[107] = 24;

            self->rockDebrisInfo[108] = 3;
            self->rockDebrisInfo[109] = 2;
            self->rockDebrisInfo[110] = 0x310000;
            self->rockDebrisInfo[111] = -0x110000;
            self->rockDebrisInfo[112] = FLIP_NONE;
            self->rockDebrisInfo[113] = 26;

            self->rockDebrisInfo[114] = 3;
            self->rockDebrisInfo[115] = 0;
            self->rockDebrisInfo[116] = -0x210000;
            self->rockDebrisInfo[117] = 0x20000;
            self->rockDebrisInfo[118] = FLIP_NONE;
            self->rockDebrisInfo[119] = 18;

            self->rockDebrisInfo[120] = 3;
            self->rockDebrisInfo[121] = 0;
            self->rockDebrisInfo[122] = 0x210000;
            self->rockDebrisInfo[123] = 0x20000;
            self->rockDebrisInfo[124] = FLIP_X;
            self->rockDebrisInfo[125] = 17;

            self->rockDebrisInfo[126] = 3;
            self->rockDebrisInfo[127] = 1;
            self->rockDebrisInfo[128] = 0;
            self->rockDebrisInfo[129] = 0x80000;
            self->rockDebrisInfo[130] = FLIP_NONE;
            self->rockDebrisInfo[131] = 16;
            break;
    }
}

void LRZRockPile_SpawnRockDebris(int32 x, int32 y, int32 velX, int32 velY)
{
    RSDK_THIS(LRZRockPile);

    LRZRockPile_SetupDebris();

    int32 sizeX = (self->hitbox.right - self->hitbox.left) << 16;
    int32 sizeY = (self->hitbox.bottom - self->hitbox.top) << 16;

    int32 destroyDelay = 0;
    if (RSDK.CheckSceneFolder("LRZ1")) {
        for (int32 i = 0; i < self->debrisCount; ++i) {
            int32 animationID = self->rockDebrisInfo[(i * 6) + 0];
            int32 frameID     = self->rockDebrisInfo[(i * 6) + 1];
            int32 offsetX     = self->rockDebrisInfo[(i * 6) + 2];
            int32 offsetY     = self->rockDebrisInfo[(i * 6) + 3];
            int32 direction   = self->rockDebrisInfo[(i * 6) + 4];
            int32 delay       = self->rockDebrisInfo[(i * 6) + 5];

            if (!offsetX && !offsetY)
                break;

            int32 spawnX       = self->position.x + offsetX;
            int32 spawnY       = self->position.y + offsetY;
            EntityDebris *rock = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, spawnY);

            RSDK.SetSpriteAnimation(LRZRockPile->particleFrames, animationID, &rock->animator, true, frameID);
            rock->drawGroup       = self->drawGroup;
            rock->delay           = delay;
            rock->direction       = direction;
            rock->gravityStrength = 0x3800;
            rock->updateRange.x   = 0x800000;
            rock->updateRange.y   = 0x800000;

            rock->velocity.x = (velX >> 8) * ((abs(x - spawnX) / (sizeX >> 16)) >> 8);
            if (velX)
                rock->velocity.x += ((RSDK.Rand(0, 16) + 8) * (2 * (velX > 1) - 1)) << 12;

            rock->velocity.y = (velY >> 8) * (((y - spawnY) / (sizeY >> 16)) >> 8);
            if (velY)
                rock->velocity.y += ((RSDK.Rand(0, 16) + 8) * (2 * (velY > 1) - 1)) << 12;

            destroyDelay = MAX(destroyDelay, delay);
        }
    }
    else if (RSDK.CheckSceneFolder("LRZ2")) {
        int32 spawnY = self->position.y - 0x100000;

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, self->position.x, spawnY);
        RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &debris->animator, true, 1);
        debris->drawFX          = FX_ROTATE | FX_FLIP;
        debris->drawGroup       = self->drawGroup;
        debris->gravityStrength = 0x3800;
        debris->direction       = self->direction;
        debris->updateRange.x   = 0x800000;
        debris->updateRange.y   = 0x800000;
        debris->rotSpeed        = 8 * (velX <= 0) - 4;

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
        debris->drawFX          = FX_ROTATE | FX_FLIP;
        debris->drawGroup       = self->drawGroup;
        debris->gravityStrength = 0x3800;
        debris->direction       = self->direction;
        debris->updateRange.x   = 0x800000;
        debris->updateRange.y   = 0x800000;
        debris->rotSpeed        = 8 * (velX > 0) - 4;
        debris->velocity.x      = (velX >> 8) * ((abs(x - self->position.x) / sizeX) >> 8);

        if (velX)
            debris->velocity.x += ((RSDK.Rand(0, 16) + 8) * (2 * (velX > 1) - 1)) << 12;

        debris->velocity.y = (velY >> 8) * (((y - spawnY) / sizeY) >> 8);
        if (velY)
            debris->velocity.y += ((RSDK.Rand(0, 16) + 8) * (2 * (velY > 1) - 1)) << 12;
    }

    self->destroyTimer = destroyDelay;
    RSDK.PlaySfx(LRZRockPile->sfxLedgeBreak, false, 255);

    if (self->destroyTimer > 0) {
        self->visible = false;
        self->state   = LRZRockPile_State_Broken;
    }
    else {
        destroyEntity(self);
    }
}

void LRZRockPile_State_Wall(void)
{
    RSDK_THIS(LRZRockPile);

    foreach_active(Player, player)
    {
        if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
            if ((!self->onlyKnux || player->characterID == ID_KNUCKLES)
#if MANIA_USE_PLUS
                && (!self->onlyMighty || player->characterID == ID_MIGHTY)
#endif
            ) {
                bool32 canBreak = abs(player->groundVel) >= 0x48000 && player->onGround && player->animator.animationID == ANI_JUMP;
                if (player->shield == SHIELD_FIRE) {
                    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
                    canBreak |= shield->shieldAnimator.animationID == SHIELDANI_FIREATTACK;
                }

                canBreak |= player->characterID == ID_KNUCKLES;
                if (player->characterID == ID_SONIC && player->superState == SUPERSTATE_SUPER)
                    canBreak |= true;

                if ((canBreak && !player->sidekick) && !self->unbreakable) {
                    if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                        if (player->position.x > self->position.x)
                            LRZRockPile_SpawnRockDebris(player->position.x, player->position.y, -0x40000, -0x20000);
                        else
                            LRZRockPile_SpawnRockDebris(player->position.x, player->position.y, 0x40000, -0x20000);
                    }
                    continue; // rock's broken now, we don't want to check any solid collisions
                }
            }

            Player_CheckCollisionBox(player, self, &self->hitbox);
        }
    }
}

void LRZRockPile_State_Floor_TopSolid(void)
{
    RSDK_THIS(LRZRockPile);

    if (self->canCollapse && self->timer <= 0) {
        LRZRockPile_SpawnRockDebris(0, 0, 0, 0);
    }
    else {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionPlatform(player, self, &self->hitbox)
#if MANIA_USE_PLUS
                    && (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop))
#endif
                    && !player->sidekick && !self->unbreakable && player->onGround) {
                    if (self->delay != (uint16)-1) {
                        if (!self->canCollapse) {
                            self->canCollapse = true;
                            self->timer       = self->delay;
                        }
#if MANIA_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop)
                            self->timer = 1;
#endif
                    }
                }
            }
        }
    }
}

void LRZRockPile_State_Floor_AllSolid(void)
{
    RSDK_THIS(LRZRockPile);

    if (self->canCollapse && self->timer <= 0) {
        LRZRockPile_SpawnRockDebris(0, 0, 0, 0);
    }
    else {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP
#if MANIA_USE_PLUS
                    && (!self->onlyMighty || (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop))
#endif
                    && !player->sidekick && !self->unbreakable && player->onGround) {
                    if (self->delay != (uint16)-1) {
                        if (!self->canCollapse) {
                            self->canCollapse = true;
                            self->timer       = self->delay;
                        }
#if MANIA_USE_PLUS
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

    if (self->destroyTimer <= 0) {
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == ((uint8)(self->planeFilter - 1) & 1))
                Player_CheckCollisionBox(player, self, &self->hitbox);
        }
    }

    --self->destroyTimer;
}

#if GAME_INCLUDE_EDITOR
void LRZRockPile_EditorDraw(void)
{
    RSDK_THIS(LRZRockPile);

    switch (self->type) {
        case LRZROCKPILE_WALL: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &self->animator, true, 0); break;
        case LRZROCKPILE_FLOOR_TOPSOLID: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0); break;
        case LRZROCKPILE_FLOOR_ALLSOLID: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &self->animator, true, 0); break;
    }

    LRZRockPile_Draw();
}

void LRZRockPile_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1")) {
        LRZRockPile->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/LRZRockPile.bin", SCOPE_STAGE);
        LRZRockPile->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("LRZ2")) {
        LRZRockPile->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/LRZRockPile.bin", SCOPE_STAGE);
    }

    RSDK_ACTIVE_VAR(LRZRockPile, type);
    RSDK_ENUM_VAR("Wall", LRZROCKPILE_WALL);
    RSDK_ENUM_VAR("Floor (Top Solid)", LRZROCKPILE_FLOOR_TOPSOLID);
    RSDK_ENUM_VAR("Floor (All Solid)", LRZROCKPILE_FLOOR_ALLSOLID);

    RSDK_ACTIVE_VAR(LRZRockPile, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);

    // No idea what "debugParticles" is, assuming it was an editor-only thing to preview what the rock particles were like???
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
#if MANIA_USE_PLUS
    RSDK_EDITABLE_VAR(LRZRockPile, VAR_BOOL, onlyMighty);
#endif
}
