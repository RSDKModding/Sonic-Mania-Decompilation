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

    if (data) {
        entity->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, 0, &entity->animator, true, 0);
    }
    else {
        RSDK.SetSpriteAnimation(Ring->spriteIndex, entity->type, &entity->animator, true, 0);

        entity->amplitude.x >>= 10;
        entity->amplitude.y >>= 10;
        entity->active = ACTIVE_BOUNDS;
        switch (entity->moveType) {
            default:
                entity->updateRange.x = 0x400000;
                entity->updateRange.y = 0x400000;
                entity->state         = Ring_State_Normal;
                entity->stateDraw     = Ring_StateDraw_Normal;
                break;
            case RING_MOVE_NORMAL:
                entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
                entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
                entity->state         = Ring_State_Move;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
            case RING_MOVE_CIRCLE:
                entity->updateRange.x = (abs(entity->amplitude.x) + 0x1000) << 10;
                entity->updateRange.y = (abs(entity->amplitude.y) + 0x1000) << 10;
                entity->state         = Ring_State_Circular;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
            case RING_MOVE_PATH:
                entity->updateRange.x = (abs(entity->amplitude.x) + 0x2000) << 9;
                entity->updateRange.y = (abs(entity->amplitude.y) + 0x2000) << 9;
                entity->state         = Ring_State_Path;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
            case RING_MOVE_TRACK:
                entity->updateRange.x = 0x400000;
                entity->updateRange.y = 0x400000;
                entity->active        = ACTIVE_NEVER;
                entity->state         = Ring_State_Track;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
        }
    }
}

void Ring_StageLoad(void)
{
    Ring->spriteIndex   = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);
    Ring->hitbox.left   = -8;
    Ring->hitbox.top    = -8;
    Ring->hitbox.right  = 8;
    Ring->hitbox.bottom = 8;
    if (DebugMode->itemCount < 0x100) {
        DebugMode->objectIDs[DebugMode->itemCount] = Ring->objectID;
        DebugMode->spawn[DebugMode->itemCount]     = Ring_DebugSpawn;
        DebugMode->draw[DebugMode->itemCount++]    = Ring_DebugDraw;
    }
    Ring->sfx_Ring = RSDK.GetSFX("Global/Ring.wav");
}

void Ring_DebugSpawn(void)
{
    RSDK_THIS(Ring);
    CREATE_ENTITY(Ring, NULL, entity->position.x, entity->position.y);
}

void Ring_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Ring->spriteIndex, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Ring_StateDraw_Normal(void)
{
    RSDK_THIS(Ring);
    entity->direction = entity->animator.frameID > 8;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}
void Ring_StateDraw_Oscillating(void)
{
    RSDK_THIS(Ring);
    entity->direction = entity->animator.frameID > 8;
    RSDK.DrawSprite(&entity->animator, &entity->offset, false);
}
void Ring_StateDraw_Sparkle(void)
{
    RSDK_THIS(Ring);
    if (RSDK_sceneInfo->entity->alpha == 0xE0) {
        entity->animator.frameID += 16;
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->animator, NULL, false);

        entity->inkEffect = INK_NONE;
        entity->animator.frameID -= 16;
    }
    RSDK.DrawSprite(&entity->animator, NULL, false);
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
        sparkle->stateDraw  = Ring_StateDraw_Sparkle;
        sparkle->active     = 2;
        sparkle->visible    = 0;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, entity->sparkleType + 2, &sparkle->animator, true, 0);
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
void Ring_State_Bounce(void)
{
    RSDK_THIS(Ring);
    entity->velocity.y += 0x1800;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->moveType != RING_MOVE_NONE) {
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
void Ring_State_Circular(void)
{
    RSDK_THIS(Ring);
    entity->offset.x = entity->amplitude.x * RSDK.Cos1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.x;
    entity->offset.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.y;
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
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
void Ring_State_Move(void)
{
    RSDK_THIS(Ring);
    entity->offset.x = entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.x;
    entity->offset.y = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.y;
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Normal(void)
{
    RSDK_THIS(Ring);
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_State_Path(void)
{
    RSDK_THIS(Ring);

    uint16 timeVal = Zone->timer << 7;
    if ((timeVal & 0x100) == entity->direction) {
        entity->offset.x = entity->position.x + (timeVal * entity->amplitude.x >> 6) - (entity->amplitude.x << 15);
        entity->offset.y = entity->position.y + (timeVal * entity->amplitude.y >> 6) - (entity->amplitude.y << 15);
    }
    else {
        entity->offset.x = entity->position.x + (entity->amplitude.x << 15) - (timeVal * entity->amplitude.x >> 6);
        entity->offset.y = entity->position.y + (entity->amplitude.y << 15) - (timeVal * entity->amplitude.y >> 6);
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
void Ring_State_Track(void)
{
    RSDK_THIS(Ring);
    entity->offset.x += entity->velocity.x;
    entity->offset.y += entity->velocity.y;
    Entity *track = RSDK.GetEntityByID(entity->speed);

    if (entity->velocity.x <= 0) {
        if (entity->offset.x < track->position.x) {
            entity->offset.x = track->position.x;
        }
    }
    else if (entity->offset.x > track->position.x) {
        entity->offset.x = track->position.x;
    }

    if (entity->velocity.y <= 0) {
        if (entity->offset.y < track->position.y) {
            entity->offset.y = track->position.y;
        }
    }
    else if (entity->offset.y > track->position.y) {
        entity->offset.y = track->position.y;
    }
    Ring_Collect();

    entity->animator.frameID = Zone->ringFrame;
}
void Ring_CheckObjectCollisions(int32 offsetX, int32 offsetY)
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
        if (!(flags & 8) && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, entity->collisionPlane, -offsetX, 0, true)) {
            entity->velocity.x = -xVel;
        }
    }
    else if (!(flags & 4) && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, entity->collisionPlane, offsetX, 0, true)) {
        entity->velocity.x = -xVel;
    }

    if (yVel <= 0) {
        if (flags & 0x10 || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, entity->collisionPlane, 0, -offsetY, true)) {
            entity->velocity.y = -yVel;
        }
    }
    else if (flags & 2 || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, entity->collisionPlane, 0, offsetY, true)) {
        entity->velocity.y = (yVel >> 2) - yVel;
        if (entity->velocity.y > -0x10000)
            entity->velocity.y = -0x10000;
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
            case 0x0: side = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animator, 0), entity, &Ring->hitbox, true); break;
            case 0x1:
            case 0x5:
            case 0x6:
            case 0x7:
            case 0x8:
            case 0x9:
            case 0xA:
            case 0xB:
            case 0xC:
            case 0xD:
            case 0xE:
            case 0xF: side = RSDK.CheckObjectCollisionBox(platform, RSDK.GetHitbox(&platform->animator, 1), entity, &Ring->hitbox, true); break;
            case 0x2:
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
void Ring_Collect(void)
{
    RSDK_THIS(Ring);

    int32 x = entity->position.x;
    int32 y = entity->position.y;

    if (entity->offset.x)
        entity->position = entity->offset;

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
                    sparkle->stateDraw = Ring_StateDraw_Sparkle;
                    sparkle->active    = 2;
                    sparkle->visible   = 0;
                    if (entity->drawOrder == 1)
                        sparkle->drawOrder = 1;
                    else
                        sparkle->drawOrder = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(Ring->spriteIndex, i % 3 + 2, &sparkle->animator, true, 0);
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
            entity->offset.x     = 0;
            entity->state        = Ring_State_Attract;
            entity->stateDraw    = Ring_StateDraw_Normal;
            entity->active       = ACTIVE_NORMAL;
            entity->storedPlayer = player;
            foreach_return;
        }
    }
    entity->position.x = x;
    entity->position.y = y;
}
void Ring_FakeLoseRings(Entity *entity, int32 ringCount, uint8 drawOrder)
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

    if (ringCount3 <= 16) {
        if (ringCount3 < 0)
            ringCount3 = 0;
    }
    else {
        ringCount3 = 16;
    }

    int32 angleVal = 0xC0 - 8 * (ringCount2 & 0xFFFFFFFE);
    int32 angle    = angleVal + 8;
    if (!(ringCount2 & 1))
        angle = angleVal - 8;

    for (int32 r = 0; r < ringCount2; ++r) {
        EntityRing *ringGrow = CREATE_ENTITY(Ring, entity, entity->position.x, entity->position.y);
        ringGrow->velocity.x = radius * RSDK.Cos256(angle);
        ringGrow->velocity.y = radius * RSDK.Sin256(angle);
        RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ringGrow->animator, true, 0);
        angle += 16;
        ringGrow->animator.animationSpeed = 0x200;
        ringGrow->scale.x                 = scale;
        ringGrow->scale.y                 = scale;
        ringGrow->drawFX                  = 5;
        ringGrow->drawOrder               = drawOrder;
        ringGrow->state                   = Ring_State_Grow;
        ringGrow->stateDraw               = Ring_StateDraw_Normal;
    }

    angleVal = 0xC0 - 8 * (ringCount & 0xFFFFFFFE);
    angle    = angleVal + 8;
    if (!(ringCount & 1))
        angle = angleVal - 8;

    for (int32 r = 0; r < ringCount; ++r) {
        EntityRing *ringGrow = CREATE_ENTITY(Ring, entity, entity->position.x, entity->position.y);
        ringGrow->velocity.x = (radius + 0x200) * RSDK.Cos256(angle);
        ringGrow->velocity.y = (radius + 0x200) * RSDK.Sin256(angle);
        RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ringGrow->animator, true, 0);
        angle += 16;
        ringGrow->animator.animationSpeed = 0x200;
        ringGrow->scale.x                 = scale + 0x40;
        ringGrow->scale.y                 = scale + 0x40;
        ringGrow->drawFX                  = 5;
        ringGrow->drawOrder               = drawOrder;
        ringGrow->state                   = Ring_State_Grow;
        ringGrow->stateDraw               = Ring_StateDraw_Normal;
    }

    angleVal = 0xC0 - 8 * (ringCount3 & 0xFFFFFFFE);
    angle    = angleVal + 8;
    if (!(ringCount3 & 1))
        angle = angleVal - 8;

    for (int32 r = 0; r < ringCount3; ++r) {
        EntityRing *ringGrow = CREATE_ENTITY(Ring, entity, entity->position.x, entity->position.y);
        ringGrow->velocity.x = (radius + 0x400) * RSDK.Cos256(angle);
        ringGrow->velocity.y = (radius + 0x400) * RSDK.Sin256(angle);
        RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ringGrow->animator, true, 0);
        angle += 16;
        ringGrow->animator.animationSpeed = 0x200;
        ringGrow->scale.x                 = scale + 0x80;
        ringGrow->scale.y                 = scale + 0x80;
        ringGrow->drawFX                  = 5;
        ringGrow->drawOrder               = drawOrder;
        ringGrow->state                   = Ring_State_Grow;
        ringGrow->stateDraw               = Ring_StateDraw_Normal;
    }
}

#if RETRO_INCLUDE_EDITOR
void Ring_EditorDraw(void)
{
    RSDK_THIS(Ring);
    RSDK.SetSpriteAnimation(Ring->spriteIndex, entity->type, &entity->animator, true, 0);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Ring_EditorLoad(void)
{
    Ring->spriteIndex = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Ring, type);
    RSDK_ENUM_VAR(RING_TYPE_NORMAL);
    RSDK_ENUM_VAR(RING_TYPE_BIG);

    RSDK_ACTIVE_VAR(Ring, moveType);
    RSDK_ENUM_VAR(RING_MOVE_NONE);
    RSDK_ENUM_VAR(RING_MOVE_NORMAL);
    RSDK_ENUM_VAR(RING_MOVE_CIRCLE);
    RSDK_ENUM_VAR(RING_MOVE_PATH);
    RSDK_ENUM_VAR(RING_MOVE_TRACK);

    RSDK_ACTIVE_VAR(Ring, planeFilter);
    RSDK_ENUM_VAR(PLANEFILTER_NONE);
    RSDK_ENUM_VAR(PLANEFILTER_A);
    RSDK_ENUM_VAR(PLANEFILTER_B);
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
