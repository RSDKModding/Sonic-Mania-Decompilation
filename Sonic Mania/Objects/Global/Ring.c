#include "SonicMania.h"

ObjectRing *Ring;

void Ring_Update(void)
{
    RSDK_THIS(Ring);
    StateMachine_Run(entity->state);
}

void Ring_LateUpdate(void) {}

void Ring_StaticUpdate(void) {}

void Ring_Draw(void)
{
    RSDK_THIS(Ring);
    StateMachine_Run(entity->stateDraw);
}

void Ring_Create(void *data)
{
    RSDK_THIS(Ring);
    entity->visible = true;

    int32 layer = Zone->drawOrderLow;
    if (entity->planeFilter > 0 && ((uint8)entity->planeFilter - 1) & 2)
        layer = Zone->drawOrderHigh;

    entity->drawOrder = layer + 1;
    if (entity->type == RING_TYPE_BIG) {
        entity->drawFX |= FX_FLIP;
        entity->ringAmount = 5;
    }

    if (!SceneInfo->inEditor) {
        if (!data) {
            RSDK.SetSpriteAnimation(Ring->aniFrames, entity->type, &entity->animator, true, 0);

            entity->amplitude.x >>= 10;
            entity->amplitude.y >>= 10;
            entity->active = ACTIVE_BOUNDS;
            switch (entity->moveType) {
                case RING_MOVE_NORMAL:
                    entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
                    entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
                    entity->state         = Ring_State_Move;
                    entity->stateDraw     = Ring_Draw_Oscillating;
                    break;

                case RING_MOVE_CIRCLE:
                    entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
                    entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
                    entity->state         = Ring_State_Circular;
                    entity->stateDraw     = Ring_Draw_Oscillating;
                    break;

                case RING_MOVE_TRACK:
                    entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 9;
                    entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 9;
                    entity->state         = Ring_State_Track;
                    entity->stateDraw     = Ring_Draw_Oscillating;
                    break;

                case RING_MOVE_PATH:
                    entity->updateRange.x = 0x400000;
                    entity->updateRange.y = 0x400000;
                    entity->active        = ACTIVE_NEVER;
                    entity->state         = Ring_State_Path;
                    entity->stateDraw     = Ring_Draw_Oscillating;
                    break;

                default:
                case RING_MOVE_FIXED:
                    entity->updateRange.x = 0x400000;
                    entity->updateRange.y = 0x400000;
                    entity->state         = Ring_State_Normal;
                    entity->stateDraw     = Ring_Draw_Normal;
                    break;
            }
        }
        else {
            entity->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Ring->aniFrames, RING_TYPE_NORMAL, &entity->animator, true, 0);
        }
    }
}

void Ring_StageLoad(void)
{
    Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);

    Ring->hitbox.left   = -8;
    Ring->hitbox.top    = -8;
    Ring->hitbox.right  = 8;
    Ring->hitbox.bottom = 8;

    DEBUGMODE_ADD_OBJ(Ring);

    Ring->sfxRing = RSDK.GetSFX("Global/Ring.wav");
}

void Ring_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Ring, NULL, entity->position.x, entity->position.y);
}

void Ring_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Ring->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Ring_Collect(void)
{
    RSDK_THIS(Ring);

    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;

    if (entity->drawPos.x)
        entity->position = entity->drawPos;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Ring->hitbox)) {
            if (!entity->planeFilter || player->collisionPlane == (((uint8)entity->planeFilter - 1) & 1)) {
                if (player->sidekick)
                    player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

                int32 ringAmount = 1;
                if (entity->type == RING_TYPE_BIG) {
                    player->ringExtraLife += 100 * (entity->ringAmount / 100);
                    ringAmount = entity->ringAmount;
                }
                Player_GiveRings(ringAmount, player, true);
                int32 max = 0x100000;
                if (entity->type != RING_TYPE_BIG)
                    max = 0x80000;

                int32 cnt = 4 * (entity->type == RING_TYPE_BIG) + 4;
                int32 min = -max;
                for (int32 i = 0; i < cnt; ++i) {
                    EntityRing *sparkle =
                        CREATE_ENTITY(Ring, NULL, entity->position.x + RSDK.Rand(min, max), entity->position.y + RSDK.Rand(min, max));
                    sparkle->state     = Ring_State_Sparkle;
                    sparkle->stateDraw = Ring_Draw_Sparkle;
                    sparkle->active    = 2;
                    sparkle->visible   = false;
                    if (entity->drawOrder == 1)
                        sparkle->drawOrder = 1;
                    else
                        sparkle->drawOrder = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(Ring->aniFrames, RING_TYPE_SPARKLE1 + (i % 3), &sparkle->animator, true, 0);
                    int32 frameCount = sparkle->animator.frameCount;
                    if (sparkle->animator.animationID == 2) {
                        sparkle->alpha = 0xE0;
                        frameCount >>= 1;
                    }
                    sparkle->maxFrameCount           = frameCount - 1;
                    sparkle->animator.animationSpeed = RSDK.Rand(6, 8);
                    sparkle->timer                   = 2 * i++;
                }
                destroyEntity(entity);
                entity->active = ACTIVE_NEVER2;
                foreach_return;
            }
        }
        else if (entity->state != Ring_State_Attract && player->shield == SHIELD_LIGHTNING
                 && RSDK.CheckObjectCollisionTouchCircle(entity, 0x500000, player, 0x10000)) {
            entity->drawPos.x    = 0;
            entity->state        = Ring_State_Attract;
            entity->stateDraw    = Ring_Draw_Normal;
            entity->active       = ACTIVE_NORMAL;
            entity->storedPlayer = player;
            foreach_return;
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}
void Ring_LoseRings(EntityPlayer *player, int32 rings, uint8 cPlane)
{
    RSDK_THIS(Player);
    int32 ringCntA = rings;
    int32 ringCntB = rings - 16;
    int32 ringCntC = rings - 32;

    ringCntA = clampVal(ringCntA, 0, 16);
    ringCntB = clampVal(ringCntB, 0, 16);
    ringCntC = clampVal(ringCntC, 0, 16);

    int32 angleStart = 0xC0 - 8 * (ringCntB & -2);
    int32 angle      = angleStart + 8;
    if (!(ringCntB & 1))
        angle = angleStart - 8;

    for (int32 i = 0; i < ringCntB; ++i) {
        EntityRing *ring              = CREATE_ENTITY(Ring, player, player->position.x, player->position.y);
        ring->velocity.x              = RSDK.Cos256(angle) << 9;
        ring->velocity.y              = RSDK.Sin256(angle) << 9;
        ring->animator.animationSpeed = 0x200;
        ring->collisionPlane          = cPlane;
        ring->inkEffect               = INK_ALPHA;
        ring->alpha                   = 0x100;
        ring->isPermanent             = true;
        ring->state                   = Ring_State_Bounce;
        ring->stateDraw               = Ring_Draw_Normal;
        ring->drawOrder               = entity->drawOrder;
        ring->moveType                = RING_MOVE_NORMAL;
        angle += 16;
    }

    angleStart = 0xC0 - 8 * (ringCntA & -2);
    angle      = angleStart + 8;
    if (!(ringCntA & 1))
        angle = angleStart - 8;

    for (int32 i = 0; i < ringCntA; ++i) {
        EntityRing *ring              = CREATE_ENTITY(Ring, player, player->position.x, player->position.y);
        ring->velocity.x              = RSDK.Cos256(angle) << 10;
        ring->velocity.y              = RSDK.Sin256(angle) << 10;
        ring->animator.animationSpeed = 512;
        ring->collisionPlane          = cPlane;
        ring->inkEffect               = INK_ALPHA;
        ring->alpha                   = 256;
        ring->isPermanent             = true;
        ring->state                   = Ring_State_Bounce;
        ring->stateDraw               = Ring_Draw_Normal;
        ring->drawOrder               = entity->drawOrder;
        angle += 16;
    }

    angleStart = 0xC0 - 8 * (ringCntC & -2);
    angle      = angleStart + 8;
    if (!(ringCntC & 1))
        angle = angleStart - 8;

    for (int32 i = 0; i < ringCntC; ++i) {
        EntityRing *ring = CREATE_ENTITY(Ring, player, player->position.x, player->position.y);
        ring->velocity.x = RSDK.Cos256(angle) << 11;
        ring->velocity.y = RSDK.Sin256(angle) << 11;
        RSDK.SetSpriteAnimation(Ring->aniFrames, RING_TYPE_BIG, &ring->animator, true, 0);
        ring->scale.x                 = 0x100;
        ring->scale.y                 = 0x100;
        ring->animator.animationSpeed = 0x200;
        ring->drawFX                  = FX_FLIP | FX_ROTATE | FX_SCALE;
        ring->state                   = Ring_State_Grow;
        ring->stateDraw               = Ring_Draw_Normal;
        angle += 16;
    }
}
void Ring_LoseHyperRings(EntityPlayer *player, int32 rings, uint8 cPlane)
{
    int32 ringCnt = rings >> 2;
    if (ringCnt <= 8) {
        if (ringCnt < 1)
            ringCnt = 1;
    }
    else {
        ringCnt = 8;
    }
    int32 ringValue = rings / ringCnt;

    int32 startAngle = 0xC0 - 0x10 * (ringCnt & 0xFFFFFFFE);
    int32 angle      = startAngle + 0x10;
    if (!(ringCnt & 1))
        angle = startAngle - 0x10;

    for (int32 i = 0; i < ringCnt; ++i) {
        EntityRing *ring = CREATE_ENTITY(Ring, player, player->position.x, player->position.y);
        RSDK.SetSpriteAnimation(Ring->aniFrames, RING_TYPE_BIG, &ring->animator, true, 0);
        ring->type                    = RING_TYPE_BIG;
        ring->velocity.x              = 0x300 * RSDK.Cos256(angle);
        ring->isPermanent             = true;
        ring->drawFX                  = FX_FLIP | FX_SCALE;
        ring->alpha                   = 0x100;
        ring->velocity.y              = 0x300 * RSDK.Sin256(angle);
        ring->animator.animationSpeed = 0x180;
        ring->collisionPlane          = cPlane;
        ring->angle                   = i << 6;
        ring->ringAmount              = ringValue;
        ring->state                   = Ring_State_Big;
        ring->stateDraw               = Ring_Draw_Normal;
        angle += 32;
    }
}
void Ring_FakeLoseRings(Vector2 *position, int32 ringCount, uint8 drawOrder)
{
    int32 ringCount2 = ringCount - 16;
    int32 ringCount3 = ringCount2 - 16;
    int32 radius     = 0x800;
    int32 scale      = 0x100;
    if (ringCount > 16) {
        radius    = 0x600;
        ringCount = 16;
        scale     = 0xC0;
    }

    if (ringCount2 <= 16) {
        if (ringCount2 < 0)
            ringCount2 = 0;
    }
    else {
        radius -= 0x200;
        ringCount2 = 16;
        scale -= 0x40;
    }

    ringCount3 = clampVal(ringCount3, 0, 16);

    int32 angleVal = 0xC0 - 8 * (ringCount2 & 0xFFFFFFFE);
    int32 angle    = angleVal + 8;
    if (!(ringCount2 & 1))
        angle = angleVal - 8;

    for (int32 r = 0; r < ringCount2; ++r) {
        EntityRing *ringGrow = CREATE_ENTITY(Ring, position, position->x, position->y);
        ringGrow->velocity.x = radius * RSDK.Cos256(angle);
        ringGrow->velocity.y = radius * RSDK.Sin256(angle);
        RSDK.SetSpriteAnimation(Ring->aniFrames, RING_TYPE_BIG, &ringGrow->animator, true, 0);
        ringGrow->animator.animationSpeed = 0x200;
        ringGrow->scale.x                 = scale;
        ringGrow->scale.y                 = scale;
        ringGrow->drawFX                  = FX_FLIP | FX_SCALE;
        ringGrow->drawOrder               = drawOrder;
        ringGrow->state                   = Ring_State_Grow;
        ringGrow->stateDraw               = Ring_Draw_Normal;
        angle += 16;
    }

    angleVal = 0xC0 - 8 * (ringCount & 0xFFFFFFFE);
    angle    = angleVal + 8;
    if (!(ringCount & 1))
        angle = angleVal - 8;

    for (int32 r = 0; r < ringCount; ++r) {
        EntityRing *ringGrow = CREATE_ENTITY(Ring, position, position->x, position->y);
        ringGrow->velocity.x = (radius + 0x200) * RSDK.Cos256(angle);
        ringGrow->velocity.y = (radius + 0x200) * RSDK.Sin256(angle);
        RSDK.SetSpriteAnimation(Ring->aniFrames, RING_TYPE_BIG, &ringGrow->animator, true, 0);
        ringGrow->animator.animationSpeed = 0x200;
        ringGrow->scale.x                 = scale + 0x40;
        ringGrow->scale.y                 = scale + 0x40;
        ringGrow->drawFX                  = FX_FLIP | FX_SCALE;
        ringGrow->drawOrder               = drawOrder;
        ringGrow->state                   = Ring_State_Grow;
        ringGrow->stateDraw               = Ring_Draw_Normal;
        angle += 16;
    }

    angleVal = 0xC0 - 8 * (ringCount3 & 0xFFFFFFFE);
    angle    = angleVal + 8;
    if (!(ringCount3 & 1))
        angle = angleVal - 8;

    for (int32 r = 0; r < ringCount3; ++r) {
        EntityRing *ringGrow = CREATE_ENTITY(Ring, position, position->x, position->y);
        ringGrow->velocity.x = (radius + 0x400) * RSDK.Cos256(angle);
        ringGrow->velocity.y = (radius + 0x400) * RSDK.Sin256(angle);
        RSDK.SetSpriteAnimation(Ring->aniFrames, RING_TYPE_BIG, &ringGrow->animator, true, 0);
        ringGrow->animator.animationSpeed = 0x200;
        ringGrow->scale.x                 = scale + 0x80;
        ringGrow->scale.y                 = scale + 0x80;
        ringGrow->drawFX                  = FX_FLIP | FX_SCALE;
        ringGrow->drawOrder               = drawOrder;
        ringGrow->state                   = Ring_State_Grow;
        ringGrow->stateDraw               = Ring_Draw_Normal;
        angle += 16;
    }
}

uint8 Ring_CheckPlatformCollisions(EntityPlatform *platform)
{
    RSDK_THIS(Ring);

    int32 side = C_NONE;
    if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        switch (platform->collision) {
            case PLATFORM_C_0:
                side = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animator, 0), entity, &Ring->hitbox, true);
                break;
            case PLATFORM_C_1:
            case PLATFORM_C_5:
            case PLATFORM_C_6:
            case PLATFORM_C_7:
            case PLATFORM_C_8:
            case PLATFORM_C_9:
            case PLATFORM_C_10:
            case PLATFORM_C_11:
            case PLATFORM_C_12:
            case PLATFORM_C_13:
            case PLATFORM_C_14:
            case PLATFORM_C_15:
                side = RSDK.CheckObjectCollisionBox(platform, RSDK.GetHitbox(&platform->animator, 1), entity, &Ring->hitbox, true);
                break;
            case PLATFORM_C_2:
                side = C_NONE;
                if (RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, entity, &Ring->hitbox) && entity->collisionLayers & Zone->moveID) {
                    TileLayer *moveLayer  = RSDK.GetSceneLayer(Zone->moveLayer);
                    moveLayer->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                    moveLayer->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                }
                break;
            default: break;
        }
        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
        if (side == C_LEFT || side == C_RIGHT)
            entity->velocity.x -= platform->velocity.x;
    }
    return side;
}

void Ring_CheckObjectCollisions(int32 drawPosX, int32 drawPosY)
{
    int32 flags = 0;
    RSDK_THIS(Ring);
    int32 xVel = entity->velocity.x;
    int32 yVel = entity->velocity.y;

    if (Platform) {
        foreach_active(Platform, colEnt) { flags |= 1 << Ring_CheckPlatformCollisions(colEnt); }
    }

    if (Crate) {
        foreach_active(Crate, colEnt) { flags |= 1 << Ring_CheckPlatformCollisions((EntityPlatform *)colEnt); }
    }

    if (Bridge) {
        foreach_active(Bridge, bridge)
        {
            if (entity->position.x > bridge->startPos && entity->position.x < bridge->endPos && entity->velocity.y >= 0) {
                Hitbox bridgeHitbox;
                bridgeHitbox.left  = -0x400;
                bridgeHitbox.right = 0x400;

                int32 divisor = 0;
                int32 ang     = 0;
                if (entity->position.x - bridge->startPos <= divisor) {
                    divisor = bridge->stoodPos;
                    ang     = (entity->position.x - bridge->startPos) << 7;
                }
                else {
                    divisor = bridge->endPos - divisor - bridge->startPos;
                    ang     = (bridge->endPos - entity->position.x) << 7;
                }

                int32 hitY = (bridge->field_6C * RSDK.Sin512(ang / divisor) >> 9) - 0x80000;
                if (entity->velocity.y >= 0x8000) {
                    bridgeHitbox.bottom = (hitY >> 16);
                    bridgeHitbox.top    = (hitY >> 16) - 8;
                }
                else {
                    bridgeHitbox.top    = (hitY >> 16);
                    bridgeHitbox.bottom = (hitY >> 16) + 8;
                }
                if (RSDK.CheckObjectCollisionTouchBox(bridge, &bridgeHitbox, entity, &Ring->hitbox)) {
                    entity->position.y = hitY + bridge->position.y - (Ring->hitbox.bottom << 16);
                    flags |= 2;
                }
            }
        }
    }

    if (Spikes) {
        foreach_active(Spikes, colEnt) { flags |= 1 << RSDK.CheckObjectCollisionBox(colEnt, &colEnt->hitbox, entity, &Ring->hitbox, true); }
    }

    if (Ice) {
        foreach_active(Ice, ice) { flags |= 1 << RSDK.CheckObjectCollisionBox(ice, &ice->hitbox1, entity, &Ring->hitbox, true); }
    }

    if (BigSqueeze) {
        foreach_active(BigSqueeze, boss)
        {
            if (entity->position.x < BigSqueeze->value4[2] + 0x200000)
                flags |= 8;
            if (entity->position.x > BigSqueeze->value4[3] - 0x200000)
                flags |= 4;
            if (entity->position.y > BigSqueeze->value7 - 0x80000)
                flags |= 2;
        }
    }

    if (SpikeCorridor) {
        foreach_active(SpikeCorridor, corridor)
        {
            if (corridor->parent) {
                flags |= (1 << RSDK.CheckObjectCollisionBox(corridor, &corridor->hitboxes[0], entity, &Ring->hitbox, true));
                flags |= (1 << RSDK.CheckObjectCollisionBox(corridor, &corridor->hitboxes[1], entity, &Ring->hitbox, true));
                flags |= (1 << RSDK.CheckObjectCollisionBox(corridor, &corridor->hitboxes[2], entity, &Ring->hitbox, true));
                flags |= (1 << RSDK.CheckObjectCollisionBox(corridor, &corridor->hitboxes[3], entity, &Ring->hitbox, true));
            }
        }
    }

    if (xVel <= 0) {
        if (!(flags & 8) && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, entity->collisionPlane, -drawPosX, 0, true)) {
            entity->velocity.x = -xVel;
        }
    }
    else if (!(flags & 4) && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, entity->collisionPlane, drawPosX, 0, true)) {
        entity->velocity.x = -xVel;
    }

    if (yVel <= 0) {
        if (flags & 0x10 || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, entity->collisionPlane, 0, -drawPosY, true)) {
            entity->velocity.y = -yVel;
        }
    }
    else if (flags & 2 || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, entity->collisionPlane, 0, drawPosY, true)) {
        entity->velocity.y = (yVel >> 2) - yVel;
        if (entity->velocity.y > -0x10000)
            entity->velocity.y = -0x10000;
    }
}

void Ring_State_Normal(void)
{
    RSDK_THIS(Ring);
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Move(void)
{
    RSDK_THIS(Ring);
    entity->drawPos.x = entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.x;
    entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.y;
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Circular(void)
{
    RSDK_THIS(Ring);
    entity->drawPos.x = entity->amplitude.x * RSDK.Cos1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.x;
    entity->drawPos.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.y;
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Path(void)
{
    RSDK_THIS(Ring);
    entity->drawPos.x += entity->velocity.x;
    entity->drawPos.y += entity->velocity.y;
    Entity *node = RSDK.GetEntityByID(entity->speed);

    if (entity->velocity.x <= 0) {
        if (entity->drawPos.x < node->position.x) {
            entity->drawPos.x = node->position.x;
        }
    }
    else if (entity->drawPos.x > node->position.x) {
        entity->drawPos.x = node->position.x;
    }

    if (entity->velocity.y <= 0) {
        if (entity->drawPos.y < node->position.y) {
            entity->drawPos.y = node->position.y;
        }
    }
    else if (entity->drawPos.y > node->position.y) {
        entity->drawPos.y = node->position.y;
    }
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Track(void)
{
    RSDK_THIS(Ring);

    uint16 timeVal = Zone->timer << 7;
    if ((timeVal & 0x100) == entity->direction) {
        entity->drawPos.x = entity->position.x + (timeVal * entity->amplitude.x >> 6) - (entity->amplitude.x << 15);
        entity->drawPos.y = entity->position.y + (timeVal * entity->amplitude.y >> 6) - (entity->amplitude.y << 15);
    }
    else {
        entity->drawPos.x = entity->position.x + (entity->amplitude.x << 15) - (timeVal * entity->amplitude.x >> 6);
        entity->drawPos.y = entity->position.y + (entity->amplitude.y << 15) - (timeVal * entity->amplitude.y >> 6);
    }
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Bounce(void)
{
    RSDK_THIS(Ring);
    entity->velocity.y += 0x1800;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->moveType != RING_MOVE_FIXED) {
        if (entity->velocity.y > 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, entity->collisionPlane, 0, 0x80000, true)) {
            int32 yvel = (entity->velocity.y >> 2) - entity->velocity.y;
            if (yvel > -0x10000)
                yvel = -0x10000;
            entity->velocity.y = yvel;
        }
    }
    else {
        Ring_CheckObjectCollisions(0x80000, 0x80000);
    }
    RSDK.ProcessAnimation(&entity->animator);

    if (!(++entity->timer & 7)) {
        if (entity->animator.animationSpeed > 0x40)
            entity->animator.animationSpeed -= 0x10;
    }

    if (entity->timer > 0x3F)
        Ring_Collect();

    if (entity->timer > 0xFF)
        destroyEntity(entity);
    else if (entity->timer >= 0xF0)
        entity->alpha -= 0x10;
}
void Ring_State_Grow(void)
{
    RSDK_THIS(Ring);
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x1800;
    entity->position.y += entity->velocity.y;

    RSDK.ProcessAnimation(&entity->animator);
    entity->scale.x += 16;
    entity->scale.y += 16;
    if (++entity->timer > 64)
        destroyEntity(entity);
}
void Ring_State_Big(void)
{
    RSDK_THIS(Ring);
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x1200;
    entity->position.y += entity->velocity.y;
    entity->angle += entity->animator.animationSpeed >> 6;
    if (entity->timer <= 0xF0) {
        entity->scale.x = (-RSDK.Sin256(entity->angle) >> 1) + 0x180;
        entity->scale.y = (RSDK.Sin256(entity->angle) >> 1) + 0x180;
    }
    else {
        if (entity->scale.x > entity->scale.y) {
            entity->scale.x += 8;
            entity->scale.y -= (entity->scale.y >> 3);
        }
        else {
            entity->scale.y += 8;
            entity->scale.x -= (entity->scale.x >> 3);
        }
    }

    int32 x = entity->scale.x << 11;
    int32 y = entity->scale.y << 11;
    Ring_CheckObjectCollisions(x, y);
    if (!(entity->angle & 0xF)) {
        EntityRing *sparkle = CREATE_ENTITY(Ring, NULL, entity->position.x + RSDK.Rand(-x, x), entity->position.y + RSDK.Rand(-y, y));
        sparkle->state      = Ring_State_Sparkle;
        sparkle->stateDraw  = Ring_Draw_Sparkle;
        sparkle->active     = 2;
        sparkle->visible    = 0;
        RSDK.SetSpriteAnimation(Ring->aniFrames, entity->sparkleType + 2, &sparkle->animator, true, 0);
        int32 frameCount = sparkle->animator.frameCount;
        if (sparkle->animator.animationID == 2) {
            sparkle->alpha = 0xE0;
            frameCount >>= 1;
        }
        sparkle->maxFrameCount           = frameCount - 1;
        sparkle->animator.animationSpeed = 4;
        entity->sparkleType              = (entity->sparkleType + 1) % 3;
    }

    RSDK.ProcessAnimation(&entity->animator);

    if (!(++entity->timer & 7)) {
        if (entity->animator.animationSpeed > 0x80) {
            entity->animator.animationSpeed -= 8;
        }
    }
    if (entity->timer > 71)
        Ring_Collect();
    if (entity->timer > 0xFF)
        destroyEntity(entity);
}
void Ring_State_Attract(void)
{
    RSDK_THIS(Ring);
    EntityPlayer *player = entity->storedPlayer;
    if (player->shield == SHIELD_LIGHTNING) {
        int32 startX = entity->position.x;
        int32 startY = entity->position.y;

        if (entity->position.x <= player->position.x) {
            if (entity->velocity.x >= 0)
                entity->velocity.x += 0x3000;
            else
                entity->velocity.x += 0xC000;
        }
        else {
            if (entity->velocity.x <= 0)
                entity->velocity.x -= 0x3000;
            else
                entity->velocity.x -= 0xC000;
        }

        if (startY <= player->position.y) {
            if (entity->velocity.y >= 0)
                entity->velocity.y += 0x3000;
            else
                entity->velocity.y += 0xC000;
        }
        else {
            if (entity->velocity.y <= 0)
                entity->velocity.y -= 0x3000;
            else
                entity->velocity.y -= 0xC000;
        }
        entity->position.x = startX + entity->velocity.x;
        entity->position.y = startY + entity->velocity.y;
    }
    else {
        entity->state                   = Ring_State_Bounce;
        entity->animator.animationSpeed = 0x80;
        entity->alpha                   = 0x100;
        entity->inkEffect               = INK_ALPHA;
    }
    Ring_Collect();
    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Sparkle(void)
{
    RSDK_THIS(Ring);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->timer <= 0) {
        entity->visible = true;
        RSDK.ProcessAnimation(&entity->animator);

        if (entity->animator.frameID >= entity->maxFrameCount)
            destroyEntity(entity);
    }
    else {
        entity->visible = false;
        entity->timer--;
    }
}

void Ring_Draw_Normal(void)
{
    RSDK_THIS(Ring);
    entity->direction = entity->animator.frameID > 8;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}
void Ring_Draw_Oscillating(void)
{
    RSDK_THIS(Ring);
    entity->direction = entity->animator.frameID > 8;
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}
void Ring_Draw_Sparkle(void)
{
    RSDK_THIS(Ring);
    if (entity->alpha == 0xE0) {
        entity->animator.frameID += 16;
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->animator, NULL, false);

        entity->inkEffect = INK_NONE;
        entity->animator.frameID -= 16;
    }
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void Ring_EditorDraw(void)
{
    RSDK_THIS(Ring);
    RSDK.SetSpriteAnimation(Ring->aniFrames, entity->type, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (showGizmos()) {
        Vector2 pos;
        Vector2 amplitude;
        amplitude.x = entity->amplitude.x >> 10;
        amplitude.y = entity->amplitude.y >> 10;

        switch (entity->moveType) {
            case RING_MOVE_NORMAL:
                entity->updateRange.x = (abs(amplitude.x) + 0x1000) << 10;
                entity->updateRange.y = (abs(amplitude.y) + 0x1000) << 10;

                //draw distance previews
                entity->inkEffect     = INK_BLEND;
                entity->drawPos.x     = amplitude.x * RSDK.Sin1024(0x100) + entity->position.x;
                entity->drawPos.y     = amplitude.y * RSDK.Sin1024(0x100) + entity->position.y;
                pos           = entity->drawPos;
                Ring_Draw_Oscillating();

                entity->drawPos.x = amplitude.x * RSDK.Sin1024(0x300) + entity->position.x;
                entity->drawPos.y = amplitude.y * RSDK.Sin1024(0x300) + entity->position.y;
                Ring_Draw_Oscillating();

                RSDK.DrawLine(pos.x, pos.y, entity->drawPos.x, entity->drawPos.y, 0x00FF00, 0, INK_NONE, false);
                break;

            case RING_MOVE_CIRCLE:
                entity->updateRange.x = (abs(amplitude.x) + 0x1000) << 10;
                entity->updateRange.y = (abs(amplitude.y) + 0x1000) << 10;

                // draw distance preview
                entity->inkEffect = INK_BLEND;
                entity->drawPos.x = amplitude.x * RSDK.Cos1024(4 * entity->angle) + entity->position.x;
                entity->drawPos.y = amplitude.y * RSDK.Sin1024(4 * entity->angle) + entity->position.y;

                Ring_Draw_Oscillating();
                break;

            case RING_MOVE_TRACK:
                entity->updateRange.x = (abs(amplitude.x) + 0x2000) << 9;
                entity->updateRange.y = (abs(amplitude.y) + 0x2000) << 9;
                entity->state         = Ring_State_Track;

                // draw distance preview
                entity->inkEffect = INK_BLEND;

                // draw distance previews
                entity->inkEffect = INK_BLEND;
                entity->drawPos.x = entity->position.x + (0x100 * amplitude.x >> 6) - (amplitude.x << 15);
                entity->drawPos.y = entity->position.y + (0x100 * amplitude.y >> 6) - (amplitude.y << 15);
                pos       = entity->drawPos;
                Ring_Draw_Oscillating();

                entity->drawPos.x = entity->position.x + (amplitude.x << 15) - (0x000 * amplitude.x >> 6);
                entity->drawPos.y = entity->position.y + (amplitude.y << 15) - (0x000 * amplitude.y >> 6);
                Ring_Draw_Oscillating();

                RSDK.DrawLine(pos.x, pos.y, entity->drawPos.x, entity->drawPos.y, 0x00FF00, 0, INK_NONE, false);
                break;

            case RING_MOVE_PATH:
                // idk, maybe custom code?
                break;

            default:
            case RING_MOVE_FIXED:
                break;
        }

        entity->inkEffect = INK_NONE;
    }

}

void Ring_EditorLoad(void)
{
    Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Ring, type);
    RSDK_ENUM_VAR("Normal", RING_TYPE_NORMAL);
    RSDK_ENUM_VAR("Big", RING_TYPE_BIG);

    RSDK_ACTIVE_VAR(Ring, moveType);
    RSDK_ENUM_VAR("Fixed", RING_MOVE_FIXED);
    RSDK_ENUM_VAR("Normal", RING_MOVE_NORMAL);
    RSDK_ENUM_VAR("Circle", RING_MOVE_CIRCLE);
    RSDK_ENUM_VAR("Track", RING_MOVE_TRACK);
    RSDK_ENUM_VAR("Path", RING_MOVE_PATH);

    RSDK_ACTIVE_VAR(Ring, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}
#endif

void Ring_Serialize(void)
{
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, moveType);
    RSDK_EDITABLE_VAR(Ring, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Ring, VAR_INT32, angle);
}
