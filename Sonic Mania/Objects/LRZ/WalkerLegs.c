#include "SonicMania.h"

ObjectWalkerLegs *WalkerLegs;

void WalkerLegs_Update(void)
{
    RSDK_THIS(WalkerLegs);

    StateMachine_Run(entity->state);

    ++entity->field_BC[0];
    ++entity->field_BC[1];
    entity->position.x = entity->field_68[1].x;
    entity->position.y = entity->field_68[1].y;

    if (entity->timer > 0)
        entity->timer--;
}

void WalkerLegs_LateUpdate(void) {}

void WalkerLegs_StaticUpdate(void) {}

void WalkerLegs_Draw(void) { WalkerLegs_DrawSprites(); }

void WalkerLegs_Create(void *data)
{
    RSDK_THIS(WalkerLegs);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->field_68[0].x = entity->position.x;
    entity->field_68[0].y = entity->position.y;
    entity->field_68[1]   = entity->position;
    entity->field_68[2]   = entity->position;
    entity->field_68[2].x += (2 * (entity->direction == FLIP_NONE) - 1) << 22;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;
    entity->state         = WalkerLegs_State_Unknown1;
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
    WalkerLegs->sfxWalkerLegs  = RSDK.GetSFX("LRZ/WalkerLegs.wav");
    WalkerLegs->sfxWalkerLegs2 = RSDK.GetSFX("LRZ/WalkerLegs2.wav");
}

void WalkerLegs_DrawSprites(void)
{
    RSDK_THIS(WalkerLegs);
    Vector2 drawPos;

    RSDK.DrawSprite(&WalkerLegs->animator1, &entity->field_68[1], false);
    RSDK.DrawSprite(&WalkerLegs->animator1, &entity->field_68[2], false);
    drawPos = entity->field_68[1];

    int moveX = (entity->field_68[2].x - entity->field_68[1].x) >> 3;
    int moveY = (entity->field_68[2].y - entity->field_68[1].y) >> 3;
    for (int i = 0; i < 9; ++i) {
        RSDK.DrawSprite(&WalkerLegs->animator2, &drawPos, false);
        drawPos.x += moveX;
        drawPos.y += moveY;
    }
}

void WalkerLegs_CheckOnScreen(void)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        if (!RSDK.CheckPosOnScreen(&entity->field_68[0], &entity->updateRange)) {
            entity->position.x = entity->field_68[0].x;
            entity->position.y = entity->field_68[0].y;
            entity->visible    = false;
            entity->state      = WalkerLegs_State_Unknown4;
        }
    }
}

void WalkerLegs_CheckPlayerCollisions(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *posPtrs[2];
    Vector2 *movePosPtrs[2];
    posPtrs[0]     = &entity->field_68[1];
    posPtrs[1]     = &entity->field_68[2];
    movePosPtrs[0] = &entity->field_80[0];
    movePosPtrs[1] = &entity->field_80[1];
    int storeX     = entity->position.x;
    int storeY     = entity->position.y;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        for (int l = 0; l < 2; ++l) {
            int moveX          = movePosPtrs[l]->x;
            int moveY          = movePosPtrs[l]->y;
            entity->position.x = posPtrs[l]->x;
            entity->position.y = posPtrs[l]->y;

            if ((1 << playerID) & entity->activePlayers[l]) {
                player->position.x += moveX;
                player->position.y += moveY;
            }

            if (Player_CheckCollisionPlatform(player, entity, &WalkerLegs->hitbox) == C_TOP)
                entity->activePlayers[l] |= (1 << playerID);
            else
                entity->activePlayers[l] &= ~(1 << playerID);
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void WalkerLegs_CheckObjectCrush(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *posPtrs[2];
    posPtrs[0]                = &entity->field_68[1];
    posPtrs[1]                = &entity->field_68[2];
    WalkerLegs->hitbox.top    = -WalkerLegs->hitbox.top;
    WalkerLegs->hitbox.bottom = -WalkerLegs->hitbox.bottom;
    int storeX                = entity->position.x;
    int storeY                = entity->position.y;

    foreach_active(Player, player)
    {
        for (int l = 0; l < 2; ++l) {
            entity->position.x = posPtrs[l]->x;
            entity->position.y = posPtrs[l]->y;

            if (Player_CheckCollisionBox(player, entity, &WalkerLegs->hitbox) == C_BOTTOM)
                player->collisionFlagV |= 2;
        }
    }

    if (Rexon) {
        foreach_active(Rexon, rexon)
        {
            if (!rexon->type) {
                for (int l = 0; l < 2; ++l) {
                    entity->position.x = posPtrs[l]->x;
                    entity->position.y = posPtrs[l]->y;

                    if (RSDK.CheckObjectCollisionTouchBox(rexon, &Rexon->hitbox2, entity, &WalkerLegs->hitbox)) {
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
            entity->position.x = posPtrs[l]->x;
            entity->position.y = posPtrs[l]->y;

            if (RSDK.CheckObjectCollisionTouchBox(entity, &WalkerLegs->hitbox, spikes, &spikeHitbox)) {
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
                entity->timer = 8;
            }
        }
    }

    WalkerLegs->hitbox.top    = -WalkerLegs->hitbox.top;
    WalkerLegs->hitbox.bottom = -WalkerLegs->hitbox.bottom;
    entity->position.x        = storeX;
    entity->position.y        = storeY;
}

void WalkerLegs_CheckPlayerStood(void)
{
    RSDK_THIS(WalkerLegs);

    if (!entity->steps || entity->stepCount < entity->steps) {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->sidekick == false && ((1 << playerID) & entity->activePlayers[entity->field_AC != 0])) {
                entity->startAngle = entity->angle;
                entity->active     = ACTIVE_NORMAL;
                entity->state      = WalkerLegs_State_Unknown3;
            }
        }
    }
}

void WalkerLegs_CheckTileCollisions(void)
{
    RSDK_THIS(WalkerLegs);

    entity->field_B4 += 0x2000;
    entity->angle += entity->field_B4;

    int entityAngle = entity->angle;
    int angle       = abs(entity->angle - entity->startAngle) >> 16;
    int x = 0, y = 0;
    if (entity->field_AC) {
        x = entity->field_68[1].x;
        y = entity->field_68[1].y;
    }
    else {
        entityAngle -= 0x1000000;
        x = entity->field_68[2].x;
        y = entity->field_68[2].y;
    }

    int rx = abs(entity->field_68[2].x - entity->field_68[1].x);
    int ry = abs(entity->field_68[2].y - entity->field_68[1].y);

    uint16 radius = MathHelpers_SquareRoot((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16)) - 1;
    if (radius <= 0x40) {
        radius = 0x40;
    }

    int ang = entityAngle & 0x1FFFFFF;

    int newX = radius * (RSDK.Cos512(ang >> 16) << 7);
    int newY = radius * (RSDK.Sin512(ang >> 16) << 7);
    if (entity->direction == FLIP_X)
        newX = -newX;

    entity->position.x = (newX + x) & 0xFFFF0000;
    entity->position.y = (newY + y) & 0xFFFF0000;

    bool32 flag = false;
    if (!entity->field_AC) {
        entity->field_68[1].x = entity->position.x;
        entity->field_68[1].y = entity->position.y;
        if (angle > 128 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
            entity->field_68[1].x = entity->position.x;
            entity->field_68[1].y = entity->position.y;
            entity->field_A0      = entity->position.y + 0x280000;
            flag                  = true;
        }
    }
    else {
        entity->field_68[2].x = entity->position.x;
        entity->field_68[2].y = entity->position.y;
        if (angle > 128 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
            entity->field_68[2].x = entity->position.x;
            entity->field_68[2].y = entity->position.y;
            entity->field_A4      = entity->position.y + 0x280000;
            flag                  = true;
        }
    }

    if (flag) {
        entity->field_B4 = 0;
        entity->field_AC = entity->field_AC == 0;
        Camera_ShakeScreen(0, 0, 5);
        ++entity->stepCount;
        entity->state = WalkerLegs_State_Unknown2;
        entity->flag  = true;
        uint16 tile   = RSDK.GetTileInfo(Zone->fgLow, entity->position.x >> 20, (entity->position.y + 0x280000) >> 20);

        if (tile == 0xFFFF)
            tile = RSDK.GetTileInfo(Zone->fgHigh, entity->position.x >> 20, (entity->position.y + 0x280000) >> 20);

        uint8 behaviour = RSDK.GetTileBehaviour(tile, entity->collisionPlane);
        if (behaviour == 1 || behaviour == 2) {
            RSDK.PlaySfx(WalkerLegs->sfxWalkerLegs2, false, 255);
            WalkerLegs_CreateDebris(entity->field_AC == 0, true);
        }
        else {
            RSDK.PlaySfx(WalkerLegs->sfxWalkerLegs, false, 255);
            WalkerLegs_CreateDebris(entity->field_AC == 0, false);
        }
        entity->field_BC[entity->field_AC == 0] = 0;
    }

    entity->position.x = entity->field_68[1].x;
    entity->position.y = entity->field_68[1].y;
}

void WalkerLegs_CheckStoodLava(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *posPtrs[2];
    posPtrs[0] = &entity->field_68[1];
    posPtrs[1] = &entity->field_68[2];

    for (int l = 0; l < 2; ++l) {
        int x = posPtrs[l]->x;
        int y = posPtrs[l]->y;
        int y2 = posPtrs[1 - l]->y;

        entity->position.x = x;
        entity->position.y = y;
        uint16 tile        = RSDK.GetTileInfo(Zone->fgLow, entity->position.x >> 20, (entity->position.y + 0x280000) >> 20);

        if (tile == 0xFFFF)
            tile = RSDK.GetTileInfo(Zone->fgHigh, entity->position.x >> 20, (entity->position.y + 0x280000) >> 20);
        uint8 behaviour = RSDK.GetTileBehaviour(tile, entity->collisionPlane);
        if ((behaviour == 1 || behaviour == 2) && y - y2 < 0x500000) {
            entity->flag = true;
            y += 0x2800;
            WalkerLegs_CreateSmoke(l == 1);
        }

        posPtrs[l]->x = x;
        posPtrs[l]->y = y;
    }

    entity->position.x = entity->field_68[1].x;
    entity->position.y = entity->field_68[1].y;
}

void WalkerLegs_CreateDebris(bool32 flag1, bool32 flag2)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckStageFolder("LRZ2") || flag2) {
        int x = 0, y = 0;
        if (flag1) {
            x = entity->field_68[2].x;
            y = entity->field_68[2].y;
        }
        else {
            x = entity->field_68[1].x;
            y = entity->field_68[1].y;
        }

        int size  = (WalkerLegs->hitbox.right - WalkerLegs->hitbox.left) >> 1;
        int count = RSDK.Rand(4, 6);
        int move  = (size << 17) / (count - 1);
        int pos   = 0;

        for (int i = 0; i < count; ++i) {
            int spawnX           = (entity->position.x - (size << 16)) + pos + ((RSDK.Rand(0, 12) - 6) << 15);
            int spawnY           = ((RSDK.Rand(0, 8) - 4) << 15) + (y + 0x280000);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, spawnY);
            RSDK.SetSpriteAnimation(WalkerLegs->particleFrames, !flag2, &debris->animator, true, 0);

            debris->drawOrder  = Zone->drawOrderHigh;
            debris->gravity    = 0x3800;
            debris->velocity.x = 0x180 * (abs(spawnX - x) >> 8) / size;
            if (debris->position.x < entity->position.x) {
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

    if (entity->field_BC[flag != 0] >= 5) {
        if (!(entity->field_BC[flag != 0] % 5)) {
            int count = RSDK.Rand(1, 2);
            for (int i = 0; i < count; ++i) {
                int offsetX = 0;
                int spawnX = 0, spawnY = 0;
                if (flag)
                    offsetX = entity->field_68[2].x;
                else
                    offsetX = entity->field_68[1].x;
                spawnX = (RSDK.Rand(WalkerLegs->hitbox.left, WalkerLegs->hitbox.right) << 16) + offsetX;
                if (flag)
                    spawnY = entity->field_A4;
                else
                    spawnY = entity->field_A0;
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_LightningSpark, spawnX, spawnY);
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

    entity->stepCount        = 0;
    entity->field_B4         = 0;
    entity->field_AC         = 0;
    entity->angle            = 0;
    entity->flag             = false;
    entity->visible          = true;
    entity->active           = ACTIVE_BOUNDS;
    entity->activePlayers[0] = 0;
    entity->activePlayers[1] = 0;
    entity->position.x       = entity->field_68[0].x;
    entity->position.y       = entity->field_68[0].y;
    entity->field_68[1].x    = entity->field_68[0].x;
    entity->field_68[1].y    = entity->field_68[0].y;
    entity->field_68[2].x    = entity->field_68[0].x;
    entity->field_68[2].y    = entity->field_68[0].y;
    entity->field_A0         = entity->field_68[1].y;
    entity->field_80[0].x    = 0;
    entity->field_80[0].y    = 0;
    entity->field_80[1].x    = 0;
    entity->field_80[1].y    = 0;
    entity->field_68[2].x += (2 * (entity->direction == FLIP_NONE) - 1) << 22;
    entity->field_A0 -= (WalkerLegs->hitbox.top << 16);
    entity->field_A4 = entity->field_68[2].y - WalkerLegs->hitbox.top << 16;
    entity->state    = WalkerLegs_State_Unknown2;
}

void WalkerLegs_State_Unknown2(void)
{
    RSDK_THIS(WalkerLegs);

    entity->angle &= 0x1FFFFFF;
    entity->field_90[0].x = entity->field_68[1].x;
    entity->field_90[0].y = entity->field_68[1].y;
    entity->field_90[1].x = entity->field_68[2].x;
    entity->field_90[1].y = entity->field_68[2].y;
    WalkerLegs_CheckStoodLava();

    entity->field_80[0].x = entity->field_68[1].x - entity->field_90[0].x;
    entity->field_80[0].y = entity->field_68[1].y - entity->field_90[0].y;
    entity->field_80[1].x = entity->field_68[2].x - entity->field_90[1].x;
    entity->field_80[1].y = entity->field_68[2].y - entity->field_90[1].y;
    WalkerLegs_CheckPlayerCollisions();

    if (entity->flag) {
        int x = 0;
        if (entity->direction)
            x = entity->field_68[1].x - entity->field_68[2].x;
        else
            x = entity->field_68[2].x - entity->field_68[1].x;
        entity->flag  = false;
        entity->angle = RSDK.ATan2(x, entity->field_68[2].y - entity->field_68[1].y) << 17;
    }
    WalkerLegs_CheckPlayerStood();
    WalkerLegs_CheckOnScreen();
}

void WalkerLegs_State_Unknown3(void)
{
    RSDK_THIS(WalkerLegs);

    entity->field_90[0]   = entity->field_68[1];
    entity->field_90[1].x = entity->field_68[2].x;
    entity->field_90[1].y = entity->field_68[2].y;
    entity->flag          = false;
    WalkerLegs_CheckStoodLava();
    WalkerLegs_CheckTileCollisions();
    entity->field_80[0].x = entity->field_68[1].x - entity->field_90[0].x;
    entity->field_80[0].y = entity->field_68[1].y - entity->field_90[0].y;
    entity->field_80[1].x = entity->field_68[2].x - entity->field_90[1].x;
    entity->field_80[1].y = entity->field_68[2].y - entity->field_90[1].y;
    WalkerLegs_CheckPlayerCollisions();
    WalkerLegs_CheckObjectCrush();
    WalkerLegs_CheckOnScreen();
}

void WalkerLegs_State_Unknown4(void)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        entity->state = WalkerLegs_State_Unknown1;
}

#if RETRO_INCLUDE_EDITOR
void WalkerLegs_EditorDraw(void)
{
    RSDK_THIS(WalkerLegs);

    entity->field_68[0].x = entity->position.x;
    entity->field_68[0].y = entity->position.y;
    entity->field_68[1]   = entity->position;
    entity->field_68[2]   = entity->position;
    entity->field_68[2].x += (2 * (entity->direction == FLIP_NONE) - 1) << 22;

    entity->inkEffect = INK_NONE;
    WalkerLegs_DrawSprites();

    int dist = (0x40 * entity->steps) << 16;

    entity->inkEffect = INK_BLEND;
    entity->field_68[1].x += entity->direction ? -dist : dist;
    entity->field_68[2].x += entity->direction ? -dist : dist;
    WalkerLegs_DrawSprites();

    RSDK.DrawLine(entity->position.x, entity->position.y, entity->field_68[1].x, entity->field_68[1].y, 0x00FF00, 0xFF, INK_NONE, false);
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
