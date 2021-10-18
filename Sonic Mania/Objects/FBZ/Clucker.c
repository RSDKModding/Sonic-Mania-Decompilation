#include "SonicMania.h"

ObjectClucker *Clucker;

void Clucker_Update(void)
{
    RSDK_THIS(Clucker);
    StateMachine_Run(entity->state);
}

void Clucker_LateUpdate(void) {}

void Clucker_StaticUpdate(void) {}

void Clucker_Draw(void)
{
    RSDK_THIS(Clucker);
    Vector2 drawPos;

    if (entity->state == Clucker_State_Projectile) {
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
    else {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        if (entity->direction & FLIP_Y)
            drawPos.y -= entity->yOffset;
        else
            drawPos.y += entity->yOffset;

        if (entity->state != Clucker_State_Unknown5) {
            if (entity->yOffset == -0x180000) {
                if (entity->direction & FLIP_X)
                    drawPos.x -= entity->field_60;
                else
                    drawPos.x += entity->field_60;

                entity->animator.frameID = 2;
                RSDK.DrawSprite(&entity->animator, &drawPos, false);

                if (entity->direction & FLIP_X)
                    drawPos.x += entity->field_60;
                else
                    drawPos.x -= entity->field_60;
            }
            RSDK.GetFrame(Clucker->aniFrames, 0, 1)->height = 9 - (entity->yOffset >> 16);
            entity->animator.frameID                        = 1;
        }
        RSDK.DrawSprite(&entity->animator, &drawPos, false);

        if (!entity->hasChildren) {
            int32 frame                = entity->animator.frameID;
            entity->animator.frameID = 0;
            RSDK.DrawSprite(&entity->animator, NULL, false);
            entity->animator.frameID = frame;
        }
    }
}

void Clucker_Create(void *data)
{
    RSDK_THIS(Clucker);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        EntityPlatform *platform = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, Platform);
        if (platform->objectID == Platform->objectID) {
            if (platform->childCount > 0)
                entity->hasChildren = true;
        }
        else {
            entity->direction *= FLIP_Y;
        }
        entity->visible = true;
        entity->active  = ACTIVE_BOUNDS;

        if (data) {
            entity->drawOrder     = Zone->drawOrderLow;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x400000;
            entity->active        = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Clucker->aniFrames, 1, &entity->animator, true, 0);
            entity->state = Clucker_State_Projectile;
        }
        else {
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            entity->drawOrder     = Zone->drawOrderLow + 1;
            if (!entity->delay)
                entity->delay = 68;
            RSDK.SetSpriteAnimation(Clucker->aniFrames, 0, &entity->animator, true, 0);
            entity->state = Clucker_State_Setup;
        }
    }
}

void Clucker_StageLoad(void)
{
    Clucker->aniFrames           = RSDK.LoadSpriteAnimation("FBZ/Clucker.bin", SCOPE_STAGE);
    Clucker->hitboxBadnik.left   = -16;
    Clucker->hitboxBadnik.top    = -8;
    Clucker->hitboxBadnik.right  = 16;
    Clucker->hitboxBadnik.bottom = 8;
    Clucker->hitboxRange.left    = -128;
    Clucker->hitboxRange.top     = -256;
    Clucker->hitboxRange.right   = 128;
    Clucker->hitboxRange.bottom  = 32;
    Clucker->hitboxSolid.left    = -16;
    Clucker->hitboxSolid.top     = -8;
    Clucker->hitboxSolid.right   = 16;
    Clucker->hitboxSolid.bottom  = 8;
    Clucker->hitbox4.left        = -4;
    Clucker->hitbox4.top         = -4;
    Clucker->hitbox4.right       = 4;
    Clucker->hitbox4.bottom      = 4;
    Clucker->sfxShot             = RSDK.GetSFX("Stage/Shot.wav");

    DEBUGMODE_ADD_OBJ(Clucker);
}

void Clucker_DebugSpawn(void)
{
    RSDK_THIS(Clucker);
    CREATE_ENTITY(Clucker, NULL, entity->position.x, entity->position.y);
}

void Clucker_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Clucker->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Clucker_HandlePlayerInteractions(void)
{
    RSDK_THIS(Clucker);
    int32 posY = entity->position.y;

    foreach_active(Player, player)
    {
        if (!entity->hasChildren)
            Player_CheckCollisionBox(player, entity, &Clucker->hitboxSolid);

        if ((entity->direction & FLIP_Y))
            entity->position.y -= entity->yOffset;
        else
            entity->position.y += entity->yOffset;
        if (Player_CheckBadnikTouch(player, entity, &Clucker->hitboxBadnik)) {
            if (entity->hasChildren) {
                Player_CheckBadnikBreak(entity, player, true);
            }
            else if (Player_CheckBadnikBreak(entity, player, false)) {
                entity->yOffset = -0x7FFFFFFF;
                entity->state   = Clucker_State_Destroyed;
            }
        }
        entity->position.y = posY;
    }
}

void Clucker_CheckOnScreen(void)
{
    RSDK_THIS(Clucker);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->direction = entity->direction >> 1;
        Clucker_Create(NULL);
    }
}

void Clucker_State_Setup(void)
{
    RSDK_THIS(Clucker);

    entity->active = ACTIVE_NORMAL;
    if (!entity->hasChildren) {
        entity->yOffset  = 0x80000;
        entity->field_60 = 0x80000;
        entity->state    = Clucker_State_Unknown1;
        Clucker_State_Unknown1();
    }
    else {
        entity->yOffset  = -0x180000;
        entity->field_60 = 0;
        entity->state    = Clucker_State_Unknown4;
        Clucker_State_Unknown4();
    }
}

void Clucker_State_Unknown1(void)
{
    RSDK_THIS(Clucker);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Clucker->hitboxRange)) {
            if (player->position.x >= entity->position.x)
                entity->direction |= FLIP_X;
            else
                entity->direction &= ~FLIP_X;
            entity->state = Clucker_State_Unknown2;
        }
    }
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown2(void)
{
    RSDK_THIS(Clucker);
    entity->yOffset -= 0x20000;
    if (entity->yOffset == -0x180000)
        entity->state = Clucker_State_Unknown3;
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown3(void)
{
    RSDK_THIS(Clucker);

    entity->field_60 -= 0x10000;
    if (!entity->field_60) {
        entity->timer = 14;
        entity->state = Clucker_State_Unknown4;
    }
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown4(void)
{
    RSDK_THIS(Clucker);

    --entity->timer;
    if (entity->timer == 4) {
        entity->field_60 += 0x20000;
    }
    else if (!entity->timer) {
        entity->field_60 -= 0x20000;
        int32 spawnY = entity->position.y;
        if (entity->direction & FLIP_Y)
            spawnY += 0xD0000;
        else
            spawnY -= 0xD0000;

        EntityClucker *projectile = CREATE_ENTITY(Clucker, intToVoid(true), entity->position.x, spawnY);
        if (entity->direction & FLIP_X) {
            projectile->position.x += 0x100000;
            projectile->velocity.x = 0x20000;
        }
        else {
            projectile->position.x -= 0x100000;
            projectile->velocity.x = -0x20000;
        }
        projectile->direction = entity->direction;
        entity->timer         = entity->delay;
        RSDK.PlaySfx(Clucker->sfxShot, false, 255);
    }

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        if (player->position.x >= entity->position.x) {
            if (!(entity->direction & 1)) {
                RSDK.SetSpriteAnimation(Clucker->aniFrames, 2, &entity->animator, true, 1);
                entity->state = Clucker_State_Unknown5;
                entity->timer = 68;
            }
        }
        else if ((entity->direction & 1)) {
            RSDK.SetSpriteAnimation(Clucker->aniFrames, 2, &entity->animator, true, 1);
            entity->state = Clucker_State_Unknown5;
            entity->timer = 68;
        }
    }
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Unknown5(void)
{
    RSDK_THIS(Clucker);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        entity->direction ^= 1;
        RSDK.SetSpriteAnimation(Clucker->aniFrames, 0, &entity->animator, true, 1);
        entity->state = Clucker_State_Unknown4;
    }
    Clucker_HandlePlayerInteractions();
    Clucker_CheckOnScreen();
}

void Clucker_State_Destroyed(void)
{
    RSDK_THIS(Clucker);

    EntityPlatform *platform = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, Platform);
    int32 offsetX              = 0;
    int32 offsetY              = 0;
    if (platform->objectID == Platform->objectID) {
        offsetX = platform->collisionOffset.x;
        offsetY = platform->collisionOffset.y;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &Clucker->hitboxSolid)) {
            player->position.x += offsetX;
            player->position.y += offsetY;
        }
    }
}

void Clucker_State_Projectile(void)
{
    RSDK_THIS(Clucker);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.ProcessAnimation(&entity->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Clucker->hitbox4))
                Player_CheckProjectileHit(player, entity);
        }

        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, false))
            destroyEntity(entity);
    }
    else {
        destroyEntity(entity);
    }
}

void Clucker_EditorDraw(void) {}

void Clucker_EditorLoad(void) {}

void Clucker_Serialize(void)
{
    RSDK_EDITABLE_VAR(Clucker, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Clucker, VAR_UINT16, delay);
}
