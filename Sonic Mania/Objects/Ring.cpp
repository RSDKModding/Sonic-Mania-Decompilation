#include "../SonicMania.hpp"

ObjectRing *Ring;

void Ring_Update()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void Ring_LateUpdate() {}

void Ring_StaticUpdate() {}

void Ring_Draw()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    if (entity->stateDraw)
        entity->stateDraw();
}

void Ring_Create(void *data)
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->visible    = true;

    int layer = 2;
    if (entity->planeFilter > 0 && ((byte)entity->planeFilter - 1) & 2)
        layer = Zone->drawOrderHigh;
    else
        layer = Zone->drawOrderLow;

    entity->drawOrder = layer + 1;
    if (entity->type == 1) {
        entity->drawFX |= FX_FLIP;
        entity->ringAmount = 5;
    }

    if (data) {
        entity->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, 0, &entity->animData, 1, 0);
    }
    else {
        RSDK.SetSpriteAnimation(Ring->spriteIndex, entity->type, &entity->animData, 1, 0);

        entity->amplitude.x >>= 10;
        entity->amplitude.y >>= 10;
        int ax           = entity->amplitude.x;
        int ay           = entity->amplitude.y;
        entity->active = ACTIVE_BOUNDS;
        switch (entity->moveType) {
            case 1:
                if (ax < 0)
                    ax = -ax;
                entity->updateRange.x = (ax + 0x1000) << 10;
                if (ay < 0)
                    ay = -ay;
                entity->state         = Ring_State_Move;
                entity->updateRange.y = (ay + 0x1000) << 10;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
            case 2:
                if (ax < 0)
                    ax = -ax;
                entity->updateRange.x = (ax + 0x1000) << 10;
                if (ay < 0)
                    ay = -ay;
                entity->state         = Ring_State_Circular;
                entity->updateRange.y = (ay + 0x1000) << 10;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
            case 3:
                if (ax < 0)
                    ax = -ax;
                entity->updateRange.x = (ax + 0x2000) << 9;
                if (ay < 0)
                    ay = -ay;
                entity->state         = Ring_State_Path;
                entity->updateRange.y = (ay + 0x2000) << 9;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
            case 4:
                entity->updateRange.x = 0x400000;
                entity->updateRange.y = 0x400000;
                entity->active      = 0;
                entity->state         = Ring_State_Track;
                entity->stateDraw     = Ring_StateDraw_Oscillating;
                break;
            default:
                entity->updateRange.x = 0x400000;
                entity->updateRange.y = 0x400000;
                entity->state         = Ring_State_Normal;
                entity->stateDraw     = Ring_StateDraw_Normal;
                break;
        }
    }
}

void Ring_StageLoad()
{
    Ring->spriteIndex   = RSDK.LoadAnimation("Global/Ring.bin", SCOPE_STAGE);
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

void Ring_DebugSpawn() { RSDK.SpawnEntity(Ring->objectID, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y); }
void Ring_DebugDraw()
{
    RSDK.SetSpriteAnimation(Ring->spriteIndex, 0, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
}

void Ring_StateDraw_Normal()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->direction  = entity->animData.frameID > 8;
    return RSDK.DrawSprite(&entity->animData, NULL, 0);
}
void Ring_StateDraw_Oscillating()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->direction  = entity->animData.frameID > 8;
    return RSDK.DrawSprite(&entity->animData, &entity->offset, 0);
}
void Ring_StateDraw_Sparkle()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    if (RSDK_sceneInfo->entity->alpha == 0xE0) {
        entity->animData.frameID += 16;
        entity->inkEffect = INK_ADD;
        RSDK.DrawSprite(&entity->animData, NULL, 0);
        entity->inkEffect = INK_NONE;
        entity->animData.frameID -= 16;
    }
    return RSDK.DrawSprite(&entity->animData, NULL, 0);
}
void Ring_State_Attract()
{
    EntityRing *entity   = (EntityRing *)RSDK_sceneInfo->entity;
    EntityPlayer *player = entity->storedPlayer;
    if (player->shield == SHIELD_LIGHTNING) {
        int startX = entity->position.x;
        int xVel   = entity->velocity.x;
        int newVel = 0;

        if (entity->position.x <= player->position.x) {
            if (xVel >= 0)
                newVel = xVel + 0x3000;
            else
                newVel = xVel + 0xC000;
        }
        else if (xVel <= 0) {
            newVel = xVel - 0x3000;
        }
        else {
            newVel = xVel - 0xC000;
        }
        entity->velocity.x = newVel;
        int yVel           = entity->velocity.y;
        int startY         = entity->position.y;
        if (startY <= player->position.y) {
            if (yVel >= 0)
                newVel = yVel + 0x3000;
            else
                newVel = yVel + 0xC000;
        }
        else if (yVel <= 0) {
            newVel = yVel - 0x3000;
        }
        else {
            newVel = yVel - 0xC000;
        }
        entity->velocity.y = newVel;
        entity->position.x = startX + entity->velocity.x;
        entity->position.y = startY + entity->velocity.y;
    }
    else {
        entity->state                   = Ring_State_Bounce;
        entity->animData.animationSpeed = 0x80;
        entity->alpha                   = 0x100;
        entity->inkEffect               = INK_ALPHA;
    }
    Ring_Collect();
    entity->animData.frameID = Zone->ringFrame;
}
void Ring_State_Big()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x1200;
    entity->position.y += entity->velocity.y;
    entity->angle += entity->animData.animationSpeed >> 6;
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

    int x = entity->scale.x << 11;
    int y = entity->scale.y << 11;
    Ring_CheckObjectCollisions(x, y);
    if (!(entity->angle & 0xF)) {
        EntityRing *sparkle =
            (EntityRing *)RSDK.SpawnEntity(Ring->objectID, 0, entity->position.x + RSDK.Rand(-x, x), entity->position.y + RSDK.Rand(-y, y));
        sparkle->state     = Ring_State_Sparkle;
        sparkle->stateDraw = Ring_StateDraw_Sparkle;
        sparkle->active  = 2;
        sparkle->visible   = 0;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, entity->sparkleType + 2, &sparkle->animData, true, 0);
        int frameCount = sparkle->animData.frameCount;
        if (sparkle->animData.animationID == 2) {
            sparkle->alpha = 0xE0;
            frameCount >>= 1;
        }
        sparkle->maxFrameCount           = frameCount - 1;
        sparkle->animData.animationSpeed = 4;
        entity->sparkleType              = (entity->sparkleType + 1) % 3;
    }

    RSDK.ProcessAnimation(&entity->animData);

    if (!(++entity->timer & 7)) {
        if (entity->animData.animationSpeed > 0x80) {
            entity->animData.animationSpeed -= 8;
        }
    }
    if (entity->timer > 71)
        Ring_Collect();
    if (entity->timer > 0xFF)
        RSDK.DestroyEntity(entity, 0, 0);
}
void Ring_State_Bounce()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->velocity.y += 0x1800;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->moveType) {
        if (entity->velocity.y > 0 && RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, entity->collisionPlane, 0, 0x80000, true)) {
            int yvel = (entity->velocity.y >> 2) - entity->velocity.y;
            if (yvel > -0x10000)
                yvel = -0x10000;
            entity->velocity.y = yvel;
        }
    }
    else {
        Ring_CheckObjectCollisions(0x80000, 0x80000);
    }
    RSDK.ProcessAnimation(&entity->animData);

    if (!(++entity->timer & 7)) {
        if (entity->animData.animationSpeed > 0x40)
            entity->animData.animationSpeed -= 0x10;
    }

    if (entity->timer > 0x3F)
        Ring_Collect();

    if (entity->timer > 0xFF)
        RSDK.DestroyEntity(entity, 0, 0);
    else if (entity->timer >= 0xF0)
        entity->alpha -= 0x10;
}
void Ring_State_Circular()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->offset.x   = entity->amplitude.x * RSDK.Cos1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.x;
    entity->offset.y   = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer + 4 * entity->angle) + entity->position.y;
    Ring_Collect();

    entity->animData.frameID = Zone->ringFrame;
}
void Ring_State_Grow()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->position.x += entity->velocity.x;
    entity->velocity.y += 0x1800;
    entity->position.y += entity->velocity.y;

    RSDK.ProcessAnimation(&entity->animData);
    entity->scale.x += 16;
    entity->scale.y += 16;
    if (++entity->timer > 64)
        RSDK.DestroyEntity(entity, 0, 0);
}
void Ring_State_Move()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->offset.x   = entity->amplitude.x * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.x;
    entity->offset.y   = entity->amplitude.y * RSDK.Sin1024(entity->speed * Zone->timer) + entity->position.y;
    Ring_Collect();

    entity->animData.frameID = Zone->ringFrame;
}
void Ring_State_Normal()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    Ring_Collect();

    entity->animData.frameID = Zone->ringFrame;
}
void Ring_State_Path()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;

    ushort timeVal = Zone->timer << 7;
    if ((timeVal & 0x100) == entity->direction) {
        entity->offset.x = entity->position.x + (timeVal * entity->amplitude.x >> 6) - (entity->amplitude.x << 15);
        entity->offset.y = entity->position.y + (timeVal * entity->amplitude.y >> 6) - (entity->amplitude.y << 15);
    }
    else {
        entity->offset.x = entity->position.x + (entity->amplitude.x << 15) - (timeVal * entity->amplitude.x >> 6);
        entity->offset.y = entity->position.y + (entity->amplitude.y << 15) - (timeVal * entity->amplitude.y >> 6);
    }
    Ring_Collect();

    entity->animData.frameID = Zone->ringFrame;
}
void Ring_State_Sparkle()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->timer <= 0) {
        entity->visible = true;
        RSDK.ProcessAnimation(&entity->animData);

        if (entity->animData.frameID >= entity->maxFrameCount)
            RSDK.DestroyEntity(entity, 0, 0);
    }
    else {
        entity->visible = 0;
        entity->timer--;
    }
}
void Ring_State_Track()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
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

    entity->animData.frameID = Zone->ringFrame;
}
void Ring_CheckObjectCollisions(int offsetX, int offsetY)
{
    int flags          = 0;
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    //int xVel           = entity->velocity.x;
    //int yVel           = entity->velocity.y;

    if (Platform) {
        EntityPlatform *colEnt = NULL;
        for (colEnt = NULL; RSDK.GetActiveObjects(Platform->objectID, (Entity **)&colEnt); flags |= 1 << Ring_CheckPlatformCollisions(colEnt)) {
            ;
        }
    }

    if (Crate) {
        EntityCrate *colEnt = NULL;
        for (colEnt = NULL; RSDK.GetActiveObjects(Crate->objectID, (Entity **)&colEnt);
             flags |= 1 << Ring_CheckPlatformCollisions((EntityPlatform *)colEnt)) {
            ;
        }
    }

    if (Bridge) {
        // TODO
    }

    if (Spikes) {
        //EntitySpikes *colEnt = NULL;
        // for (colEnt = 0; RSDK.GetActiveObjects(Spikes->objectID, (Entity **)&colEnt) == 1;
        //     flags |= 1 << RSDK.CheckObjectCollisionBox(colEnt, colEnt->hitbox, entity, &Ring->hitbox, 1)) {
        //    ;
        //}
    }

    if (Ice) {
        // TODO
    }

    if (BigSqueeze) {
        // TODO
    }

    if (SpikeCorridor) {
        // TODO
    }
}
byte Ring_CheckPlatformCollisions(EntityPlatform *platform)
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    int boxResult      = 0;
    if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        switch (platform->collisionType) {
            case 0x00:
                boxResult = RSDK.CheckObjectCollisionPlatform(platform, RSDK.GetHitbox(&platform->animData, 0), entity, &Ring->hitbox, 1);
                break;
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
            case 0xF: boxResult = RSDK.CheckObjectCollisionBox(platform, RSDK.GetHitbox(&platform->animData, 1), entity, &Ring->hitbox, 1); break;
            case 2u:
                boxResult = 0;
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
        if (boxResult == 2 || boxResult == 3)
            entity->velocity.x -= platform->velocity.x;
    }
    return boxResult;
}
void Ring_Collect()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;

    int x = entity->position.x;
    int y = entity->position.y;
    if (entity->stateDraw == Ring_StateDraw_Oscillating) {
        entity->position.x = entity->offset.x;
        entity->position.y = entity->offset.y;
    }
    EntityPlayer *player = NULL;
    if (!RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
        entity->position.x = x;
        entity->position.y = y;
        return;
    }
    while (true) {
        if (Player_CheckCollisionTouch(player, entity, &Ring->hitbox))
            break;
        if (entity->state != Ring_State_Attract && player->shield == SHIELD_LIGHTNING
            && RSDK.CheckObjectCollisionTouchCircle(entity, 0x500000, player, 0x10000)) {
            entity->offset.x     = 0;
            entity->state        = Ring_State_Attract;
            entity->stateDraw    = Ring_StateDraw_Normal;
            entity->active     = ACTIVE_NORMAL;
            entity->storedPlayer = player;
            return RSDK.NextForEachLoop();
        }

        if (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player) != 1) {
            entity->position.x = x;
            entity->position.y = y;
            return;
        }
    }

    if (entity->planeFilter > 0 && player->collisionPlane != (((byte)entity->planeFilter - 1) & 1)) {
        if (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player) != 1) {
            entity->position.x = x;
            entity->position.y = y;
            return;
        }
    }
    if (player->sidekick)
        player = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);

    int ringAmount = 1;
    if (entity->type == 1) {
        player->ringExtraLife += 100 * (entity->ringAmount / 100);
        ringAmount = entity->ringAmount;
    }
    Player_GiveRings(ringAmount, player, 1);
    int max = 0x100000;
    if (entity->type != 1)
        max = 0x80000;

    int cnt = 4 * (entity->type == 1) + 4;
    int min = -max;
    for (int i = 0; i < cnt; ++i) {
        EntityRing *sparkle =
            (EntityRing *)RSDK.SpawnEntity(Ring->objectID, 0, entity->position.x + RSDK.Rand(min, max), entity->position.y + RSDK.Rand(min, max));
        sparkle->state     = Ring_State_Sparkle;
        sparkle->stateDraw = Ring_StateDraw_Sparkle;
        sparkle->active  = 2;
        sparkle->visible   = 0;
        if (entity->drawOrder == 1)
            sparkle->drawOrder = 1;
        else
            sparkle->drawOrder = Zone->drawOrderHigh;
        RSDK.SetSpriteAnimation(Ring->spriteIndex, i % 3 + 2, &sparkle->animData, true, 0);
        int frameCount = sparkle->animData.frameCount;
        if (sparkle->animData.animationID == 2) {
            sparkle->alpha = 0xE0;
            frameCount >>= 1;
        }
        sparkle->maxFrameCount           = frameCount - 1;
        sparkle->animData.animationSpeed = RSDK.Rand(6, 8);
        sparkle->timer                   = 2 * i++;
    }
    RSDK.DestroyEntity(entity, 0, 0);
    entity->active = -1;
    return RSDK.NextForEachLoop();
}
void Ring_FakeLoseRings(Entity *entity, int ringCount, byte drawOrder)
{
    int ringCount2 = ringCount - 16;
    int ringCount3 = ringCount2 - 16;
    int radius     = 0x800;
    int scale      = 0x100;
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

    int angleVal = 0xC0 - 8 * (ringCount2 & 0xFFFFFFFE);
    int angle    = angleVal + 8;
    if (!(ringCount2 & 1))
        angle = angleVal - 8;

    if (ringCount2 > 0) {
        do {
            EntityRing *ringGrow = (EntityRing *)RSDK.SpawnEntity(Ring->objectID, entity, entity->position.x, entity->position.y);
            ringGrow->velocity.x = radius * RSDK.Cos256(angle);
            ringGrow->velocity.y = radius * RSDK.Sin256(angle);
            RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ringGrow->animData, true, 0);
            angle += 16;
            ringGrow->animData.animationSpeed = 0x200;
            ringGrow->scale.x                 = scale;
            ringGrow->scale.y                 = scale;
            ringGrow->drawFX                  = 5;
            ringGrow->drawOrder               = drawOrder;
            ringGrow->state                   = Ring_State_Grow;
            ringGrow->stateDraw               = Ring_StateDraw_Normal;
        } while (!--ringCount2);
    }

    angleVal = 0xC0 - 8 * (ringCount & 0xFFFFFFFE);
    angle    = angleVal + 8;
    if (!(ringCount & 1))
        angle = angleVal - 8;

    if (ringCount > 0) {
        do {
            EntityRing *ringGrow = (EntityRing *)RSDK.SpawnEntity(Ring->objectID, entity, entity->position.x, entity->position.y);
            ringGrow->velocity.x = (radius + 0x200) * RSDK.Cos256(angle);
            ringGrow->velocity.y = (radius + 0x200) * RSDK.Sin256(angle);
            RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ringGrow->animData, true, 0);
            angle += 16;
            ringGrow->animData.animationSpeed = 0x200;
            ringGrow->scale.x                 = scale + 0x40;
            ringGrow->scale.y                 = scale + 0x40;
            ringGrow->drawFX                  = 5;
            ringGrow->drawOrder               = drawOrder;
            ringGrow->state                   = Ring_State_Grow;
            ringGrow->stateDraw               = Ring_StateDraw_Normal;
        } while (!--ringCount);
    }

    angleVal = 0xC0 - 8 * (ringCount3 & 0xFFFFFFFE);
    angle    = angleVal + 8;
    if (!(ringCount3 & 1))
        angle = angleVal - 8;

    if (ringCount3 > 0) {
        do {
            EntityRing *ringGrow = (EntityRing *)RSDK.SpawnEntity(Ring->objectID, entity, entity->position.x, entity->position.y);
            ringGrow->velocity.x = (radius + 0x400) * RSDK.Cos256(angle);
            ringGrow->velocity.y = (radius + 0x400) * RSDK.Sin256(angle);
            RSDK.SetSpriteAnimation(Ring->spriteIndex, 1, &ringGrow->animData, true, 0);
            angle += 16;
            ringGrow->animData.animationSpeed = 0x200;
            ringGrow->scale.x                 = scale + 0x80;
            ringGrow->scale.y                 = scale + 0x80;
            ringGrow->drawFX                  = 5;
            ringGrow->drawOrder               = drawOrder;
            ringGrow->state                   = Ring_State_Grow;
            ringGrow->stateDraw               = Ring_StateDraw_Normal;
        } while (!--ringCount3);
    }
}

void Ring_EditorDraw() {}

void Ring_EditorLoad() {}

void Ring_Serialize()
{
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, moveType);
    RSDK_EDITABLE_VAR(Ring, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(Ring, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Ring, VAR_INT32, angle);
}
