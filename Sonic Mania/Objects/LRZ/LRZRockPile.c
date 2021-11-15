#include "SonicMania.h"

ObjectLRZRockPile *LRZRockPile;

void LRZRockPile_Update(void)
{
    RSDK_THIS(LRZRockPile);
    StateMachine_Run(entity->state);

    if (entity->timer > 0)
        entity->timer--;
}

void LRZRockPile_LateUpdate(void) {}

void LRZRockPile_StaticUpdate(void) {}

void LRZRockPile_Draw(void)
{
    RSDK_THIS(LRZRockPile);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void LRZRockPile_Create(void *data)
{
    RSDK_THIS(LRZRockPile);

    entity->active = ACTIVE_BOUNDS;
    if (entity->planeFilter > 0 && ((uint8)(entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->playerDrawHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;

    entity->startPos      = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    switch (entity->type) {
        case 0:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &entity->animator, true, 0);
            entity->hitbox = LRZRockPile->hitbox1;
            entity->state  = LRZRockPile_State_Type0;
            break;
        case 1:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &entity->animator, true, 0);
            entity->hitbox = LRZRockPile->hitbox2;
            entity->state  = LRZRockPile_State_Type1;
            break;
        case 2:
            RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &entity->animator, true, 0);
            entity->hitbox = LRZRockPile->hitbox3;
            entity->state  = LRZRockPile_State_Type2;
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

    LRZRockPile->sfxLedgeBreak = RSDK.GetSFX("Stage/LedgeBreak.wav");

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

    switch (entity->type) {
        case 0:
            entity->debrisCount = 6;

            entity->debrisInfo[0] = 3;
            entity->debrisInfo[1] = 2;
            entity->debrisInfo[2] = -0x10000;
            entity->debrisInfo[3] = -0x190000;
            entity->debrisInfo[4] = FLIP_NONE;
            entity->debrisInfo[5] = 0;

            entity->debrisInfo[6]  = 3;
            entity->debrisInfo[7]  = 4;
            entity->debrisInfo[8]  = 0;
            entity->debrisInfo[9]  = 0x190000;
            entity->debrisInfo[10] = FLIP_NONE;
            entity->debrisInfo[11] = 0;

            entity->debrisInfo[12] = 2;
            entity->debrisInfo[13] = 0;
            entity->debrisInfo[14] = -0x80000;
            entity->debrisInfo[15] = -0x100000;
            entity->debrisInfo[16] = FLIP_NONE;
            entity->debrisInfo[17] = 0;

            entity->debrisInfo[18] = 2;
            entity->debrisInfo[19] = 2;
            entity->debrisInfo[20] = 0x80000;
            entity->debrisInfo[21] = 0x200000;
            entity->debrisInfo[22] = FLIP_NONE;
            entity->debrisInfo[23] = 0;

            entity->debrisInfo[24] = 3;
            entity->debrisInfo[25] = 0;
            entity->debrisInfo[26] = -0x10000;
            entity->debrisInfo[27] = -0x20000;
            entity->debrisInfo[28] = FLIP_NONE;
            entity->debrisInfo[29] = 0;

            entity->debrisInfo[30] = 3;
            entity->debrisInfo[31] = 2;
            entity->debrisInfo[32] = -0x10000;
            entity->debrisInfo[33] = 0xC0000;
            entity->debrisInfo[34] = FLIP_NONE;
            entity->debrisInfo[35] = 0;
            break;
        case 1:
        case 2:
            entity->debrisCount   = 22;
            entity->debrisInfo[0] = 3;
            entity->debrisInfo[1] = 3;
            entity->debrisInfo[2] = -0x2D0000;
            entity->debrisInfo[3] = 0xB0000;
            entity->debrisInfo[4] = FLIP_X;
            entity->debrisInfo[5] = 9;

            entity->debrisInfo[6]  = 3;
            entity->debrisInfo[7]  = 3;
            entity->debrisInfo[8]  = 0x2D0000;
            entity->debrisInfo[9]  = 0xB0000;
            entity->debrisInfo[10] = FLIP_NONE;
            entity->debrisInfo[11] = 9;

            entity->debrisInfo[12] = 3;
            entity->debrisInfo[13] = 3;
            entity->debrisInfo[14] = -0x200000;
            entity->debrisInfo[15] = -0x100000;
            entity->debrisInfo[16] = FLIP_X;
            entity->debrisInfo[17] = 30;

            entity->debrisInfo[18] = 3;
            entity->debrisInfo[19] = 3;
            entity->debrisInfo[20] = 0x200000;
            entity->debrisInfo[21] = -0x100000;
            entity->debrisInfo[22] = FLIP_NONE;
            entity->debrisInfo[23] = 30;

            entity->debrisInfo[24] = 2;
            entity->debrisInfo[25] = 0;
            entity->debrisInfo[26] = 0;
            entity->debrisInfo[27] = 0x180000;
            entity->debrisInfo[28] = FLIP_XY;
            entity->debrisInfo[29] = 0;

            entity->debrisInfo[30] = 2;
            entity->debrisInfo[31] = 0;
            entity->debrisInfo[32] = -0x370000;
            entity->debrisInfo[33] = 0x10000;
            entity->debrisInfo[34] = 0;
            entity->debrisInfo[35] = 20;

            entity->debrisInfo[36] = 2;
            entity->debrisInfo[37] = 1;
            entity->debrisInfo[38] = -0x300000;
            entity->debrisInfo[39] = 0xF0000;
            entity->debrisInfo[40] = FLIP_NONE;
            entity->debrisInfo[41] = 14;

            entity->debrisInfo[42] = 2;
            entity->debrisInfo[43] = 3;
            entity->debrisInfo[44] = -0x220000;
            entity->debrisInfo[45] = 0x180000;
            entity->debrisInfo[46] = FLIP_X;
            entity->debrisInfo[47] = 6;

            entity->debrisInfo[48] = 1;
            entity->debrisInfo[49] = 5;
            entity->debrisInfo[50] = -0x160000;
            entity->debrisInfo[51] = 0x130000;
            entity->debrisInfo[52] = FLIP_NONE;
            entity->debrisInfo[53] = 12;

            entity->debrisInfo[54] = 2;
            entity->debrisInfo[55] = 0;
            entity->debrisInfo[56] = 3604480;
            entity->debrisInfo[57] = 0x10000;
            entity->debrisInfo[58] = FLIP_X;
            entity->debrisInfo[59] = 20;

            entity->debrisInfo[60] = 2;
            entity->debrisInfo[61] = 1;
            entity->debrisInfo[62] = 0x300000;
            entity->debrisInfo[63] = 0xF0000;
            entity->debrisInfo[64] = FLIP_X;
            entity->debrisInfo[65] = 14;

            entity->debrisInfo[66] = 2;
            entity->debrisInfo[67] = 3;
            entity->debrisInfo[68] = 0x220000;
            entity->debrisInfo[69] = 0x180000;
            entity->debrisInfo[70] = FLIP_NONE;
            entity->debrisInfo[71] = 6;

            entity->debrisInfo[72] = 1;
            entity->debrisInfo[73] = 5;
            entity->debrisInfo[74] = 0x160000;
            entity->debrisInfo[75] = 0x130000;
            entity->debrisInfo[76] = FLIP_X;
            entity->debrisInfo[77] = 12;

            entity->debrisInfo[78] = 2;
            entity->debrisInfo[79] = 2;
            entity->debrisInfo[80] = -0xA0000;
            entity->debrisInfo[81] = 0x140000;
            entity->debrisInfo[82] = FLIP_NONE;
            entity->debrisInfo[83] = 8;

            entity->debrisInfo[84] = 2;
            entity->debrisInfo[85] = 2;
            entity->debrisInfo[86] = 0xA0000;
            entity->debrisInfo[87] = 0x140000;
            entity->debrisInfo[88] = FLIP_X;
            entity->debrisInfo[89] = 7;

            entity->debrisInfo[90] = 3;
            entity->debrisInfo[91] = 0;
            entity->debrisInfo[92] = -0x310000;
            entity->debrisInfo[93] = -0x110000;
            entity->debrisInfo[94] = FLIP_NONE;
            entity->debrisInfo[95] = 26;

            entity->debrisInfo[96]  = 3;
            entity->debrisInfo[97]  = 0;
            entity->debrisInfo[98]  = 0xF0000;
            entity->debrisInfo[99]  = -0x110000;
            entity->debrisInfo[100] = FLIP_NONE;
            entity->debrisInfo[101] = 24;

            entity->debrisInfo[102] = 3;
            entity->debrisInfo[103] = 0;
            entity->debrisInfo[104] = -0xF0000;
            entity->debrisInfo[105] = -0x110000;
            entity->debrisInfo[106] = FLIP_X;
            entity->debrisInfo[107] = 24;

            entity->debrisInfo[108] = 3;
            entity->debrisInfo[109] = 2;
            entity->debrisInfo[110] = 0x310000;
            entity->debrisInfo[111] = -0x110000;
            entity->debrisInfo[112] = FLIP_NONE;
            entity->debrisInfo[113] = 26;

            entity->debrisInfo[114] = 3;
            entity->debrisInfo[115] = 0;
            entity->debrisInfo[116] = -0x210000;
            entity->debrisInfo[117] = 0x20000;
            entity->debrisInfo[118] = FLIP_NONE;
            entity->debrisInfo[119] = 18;

            entity->debrisInfo[120] = 3;
            entity->debrisInfo[121] = 0;
            entity->debrisInfo[122] = 0x210000;
            entity->debrisInfo[123] = 0x20000;
            entity->debrisInfo[124] = FLIP_X;
            entity->debrisInfo[125] = 17;

            entity->debrisInfo[126] = 3;
            entity->debrisInfo[127] = 1;
            entity->debrisInfo[128] = 0;
            entity->debrisInfo[129] = 0x80000;
            entity->debrisInfo[130] = FLIP_NONE;
            entity->debrisInfo[131] = 16;
            break;
    }
}

void LRZRockPile_SpawnRockDebris(int x, int y, int velX, int velY)
{
    RSDK_THIS(LRZRockPile);
    LRZRockPile_SetupDebris();

    int sizeX = (entity->hitbox.right - entity->hitbox.left) << 16;
    int sizeY = (entity->hitbox.bottom - entity->hitbox.top) << 16;

    int destroyDelay = 0;
    if (RSDK.CheckStageFolder("LRZ1")) {
        for (int i = 0; i < entity->debrisCount; ++i) {
            int animationID = entity->debrisInfo[(i * 6) + 0];
            int frameID     = entity->debrisInfo[(i * 6) + 1];
            int offsetX     = entity->debrisInfo[(i * 6) + 2];
            int offsetY     = entity->debrisInfo[(i * 6) + 3];
            int direction   = entity->debrisInfo[(i * 6) + 4];
            int delay       = entity->debrisInfo[(i * 6) + 5];

            if (!offsetX && !offsetY)
                break;

            int v19              = entity->position.x + offsetX;
            int v17              = entity->position.y + offsetY;
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, v19, v17);
            RSDK.SetSpriteAnimation(LRZRockPile->particleFrames, animationID, &debris->animator, true, frameID);
            debris->drawOrder     = entity->drawOrder;
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
        int spawnY = entity->position.y - 0x100000;

        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, entity->position.x, spawnY);
        RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &debris->animator, true, 1);
        debris->drawFX        = FX_ROTATE | FX_FLIP;
        debris->drawOrder     = entity->drawOrder;
        debris->gravity       = 0x3800;
        debris->direction     = entity->direction;
        debris->updateRange.x = 0x800000;
        debris->updateRange.y = 0x800000;
        debris->rotSpeed      = 8 * (velX <= 0) - 4;

        debris->velocity.x = (velX >> 8) * (abs(abs(x - entity->position.x) / (sizeX >> 16)) >> 8);
        if (velX)
            debris->velocity.x += ((RSDK.Rand(0, 16) + 8) * (2 * (velX > 1) - 1)) << 12;

        debris->velocity.y = (velY >> 8) * (((y - spawnY) / (sizeY >> 16)) >> 8);
        if (velY)
            debris->velocity.y += ((RSDK.Rand(0, 16) + 8) * (2 * (velY > 1) - 1)) << 12;

        sizeX >>= 16;
        sizeY >>= 16;
        spawnY = entity->position.y + 0x100000;

        debris = CREATE_ENTITY(Debris, Debris_State_Fall, entity->position.x, spawnY);
        RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &debris->animator, true, 2);
        debris->drawFX        = FX_ROTATE | FX_FLIP;
        debris->drawOrder     = entity->drawOrder;
        debris->gravity       = 0x3800;
        debris->direction     = entity->direction;
        debris->updateRange.x = 0x800000;
        debris->updateRange.y = 0x800000;
        debris->rotSpeed      = 8 * (velX > 0) - 4;
        debris->velocity.x    = (velX >> 8) * ((abs(x - entity->position.x) / sizeX) >> 8);
        if (velX)
            debris->velocity.x += ((RSDK.Rand(0, 16) + 8) * (2 * (velX > 1) - 1)) << 12;
        debris->velocity.y = (velY >> 8) * (((y - spawnY) / sizeY) >> 8);
        if (velY)
            debris->velocity.y += ((RSDK.Rand(0, 16) + 8) * (2 * (velY > 1) - 1)) << 12;
    }
    entity->timer2 = destroyDelay;

    RSDK.PlaySfx(LRZRockPile->sfxLedgeBreak, false, 255);
    if (entity->timer2 > 0) {
        entity->visible = 0;
        entity->state   = LRZRockPile_State_Broken;
    }
    else {
        destroyEntity(entity);
    }
}

void LRZRockPile_State_Type0(void)
{
    RSDK_THIS(LRZRockPile);

    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1)) {
            if ((!entity->onlyKnux || player->characterID == ID_KNUCKLES)
#if RETRO_USE_PLUS
                || (!entity->onlyMighty || player->characterID == ID_MIGHTY)
#endif
            ) {
                bool32 flag = abs(player->groundVel) >= 0x48000 && player->onGround && player->playerAnimator.animationID == ANI_JUMP;
                if (player->shield == SHIELD_FIRE) {
                    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                    flag |= shield->animator.animationID == 2;
                }

                flag |= player->characterID == ID_KNUCKLES;
                if (player->characterID == ID_SONIC && player->superState == SUPERSTATE_SUPER)
                    flag |= true;

                if ((flag && !player->sidekick) && !entity->unbreakable) {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                        if (player->position.x > entity->position.x)
                            LRZRockPile_SpawnRockDebris(player->position.x, player->position.y, -0x40000, -0x20000);
                        else
                            LRZRockPile_SpawnRockDebris(player->position.x, player->position.y, 0x40000, -0x20000);
                    }
                    continue;
                }
            }
            Player_CheckCollisionBox(player, entity, &entity->hitbox);
        }
    }
}

void LRZRockPile_State_Type1(void)
{
    RSDK_THIS(LRZRockPile);
    if (entity->flag && entity->timer <= 0) {
        LRZRockPile_SpawnRockDebris(0, 0, 0, 0);
    }
    else {
        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox)
#if RETRO_USE_PLUS
                    && (!entity->onlyMighty || player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
#endif
                    && !player->sidekick && !entity->unbreakable && player->onGround) {
                    if (entity->delay != 0xFFFF) {
                        if (!entity->flag) {
                            entity->flag  = true;
                            entity->timer = entity->delay;
                        }
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop)
                            entity->timer = 1;
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

    if (entity->flag && entity->timer <= 0) {
        LRZRockPile_SpawnRockDebris(0, 0, 0, 0);
    }
    else {
        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == C_TOP
#if RETRO_USE_PLUS
                    && (!entity->onlyMighty || player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
#endif
                    && !player->sidekick && !entity->unbreakable && player->onGround) {
                    if (entity->delay != 0xFFFF) {
                        if (!entity->flag) {
                            entity->flag  = true;
                            entity->timer = entity->delay;
                        }
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop)
                            entity->timer = 1;
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

    if (entity->timer2 <= 0) {
        destroyEntity(entity);
    }
    else {
        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1))
                Player_CheckCollisionBox(player, entity, &entity->hitbox);
        }
    }

    --entity->timer2;
}

#if RETRO_INCLUDE_EDITOR
void LRZRockPile_EditorDraw(void)
{
    RSDK_THIS(LRZRockPile);

    switch (entity->type) {
        case 0: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 0, &entity->animator, true, 0); break;
        case 1: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &entity->animator, true, 0); break;
        case 2: RSDK.SetSpriteAnimation(LRZRockPile->aniFrames, 1, &entity->animator, true, 0); break;
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
