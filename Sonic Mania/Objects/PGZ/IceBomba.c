#include "SonicMania.h"

ObjectIceBomba *IceBomba;

void IceBomba_Update(void)
{
    RSDK_THIS(IceBomba);
    StateMachine_Run(entity->state);
}

void IceBomba_LateUpdate(void) {}

void IceBomba_StaticUpdate(void) {}

void IceBomba_Draw(void)
{
    RSDK_THIS(IceBomba);
    Vector2 drawPos = entity->position;
    drawPos.x += 0x40000;
    drawPos.y += 0x1A0000;

    int dirStore      = entity->direction;
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->bombAnimator, &drawPos, false);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    entity->direction = dirStore;
    entity->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&entity->wingAnimator, NULL, false);

    entity->inkEffect = INK_NONE;
}

void IceBomba_Create(void *data)
{
    RSDK_THIS(IceBomba);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    if (!RSDK_sceneInfo->inEditor) {
        entity->dip <<= 7;
        entity->drawFX |= FX_FLIP;
        if (data) {
            entity->active        = ACTIVE_NORMAL;
            entity->updateRange.x = 0x400000;
            entity->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(IceBomba->animID, 2, &entity->animator, true, 0);
            entity->state = IceBomba_Bomb_Handle;
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->alpha         = 0x80;
            entity->spawnDist     = entity->dist;
            entity->updateRange.x = (entity->dist + 0x80) << 16;
            entity->updateRange.y = (entity->dip + 0x8000) << 8;
            entity->spawnPos.x    = entity->position.x;
            entity->spawnPos.y    = entity->position.y;
            RSDK.SetSpriteAnimation(IceBomba->animID, 0, &entity->animator, true, 0);
            RSDK.SetSpriteAnimation(IceBomba->animID, 1, &entity->wingAnimator, true, 0);
            RSDK.SetSpriteAnimation(IceBomba->animID, 2, &entity->bombAnimator, true, 0);
            entity->direction = entity->dir;
            if (entity->dir) {
                entity->animator.frameID     = 4;
                entity->bombAnimator.frameID = 4;
            }
            else {
                entity->animator.frameID     = 0;
                entity->bombAnimator.frameID = 0;
            }
            entity->state = IceBomba_Fly_Create;
        }
    }
}

void IceBomba_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        IceBomba->animID = RSDK.LoadSpriteAnimation("PSZ1/IceBomba.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        IceBomba->animID = RSDK.LoadSpriteAnimation("PSZ2/IceBomba.bin", SCOPE_STAGE);

    IceBomba->flyHitbox.left    = -12;
    IceBomba->flyHitbox.top     = -12;
    IceBomba->flyHitbox.right   = 12;
    IceBomba->flyHitbox.bottom  = 12;
    IceBomba->bombHitbox.left   = -16;
    IceBomba->bombHitbox.top    = -16;
    IceBomba->bombHitbox.right  = 16;
    IceBomba->bombHitbox.bottom = 16;
    IceBomba->checkbox.left     = -12;
    IceBomba->checkbox.top      = 64;
    IceBomba->checkbox.right    = 12;
    IceBomba->checkbox.bottom   = 128;
    IceBomba->explosionSFX = RSDK.GetSFX("Stage/Explosion.wav");
    IceBomba->freezeSFX    = RSDK.GetSFX("PSZ/Freeze.wav");
    DEBUGMODE_ADD_OBJ(IceBomba);
}

void IceBomba_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(IceBomba->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void IceBomba_DebugSpawn(void)
{
    RSDK_THIS(IceBomba);
    CREATE_ENTITY(IceBomba, NULL, entity->position.x, entity->position.y);
}

void IceBomba_Fly_Collide(void)
{
    RSDK_THIS(IceBomba);
    int oldDir        = entity->direction;
    entity->direction = 0;
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &IceBomba->flyHitbox)) {
            if (Player_CheckBadnikBreak(entity, player, false)) {
                if (entity->state != IceBomba_Fly_FlyAway) {
                    EntityIceBomba *bomb = CREATE_ENTITY(IceBomba, intToVoid(1), entity->position.x, entity->position.y);
                    if (entity->direction)
                        bomb->position.x -= 0x40000;
                    else
                        bomb->position.x += 0x40000;
                    bomb->position.y += 0x1A0000;
                    bomb->direction = entity->direction;
                }
                destroyEntity(entity);
            }
        }
        else if (entity->state != IceBomba_Fly_FlyAway) {
            Vector2 prePos = entity->position;
            entity->position.x += 0x40000 * (entity->direction ? -1 : 1);
            entity->position.y += 0x1A0000;
            if (Player_CheckCollisionTouch(player, entity, &IceBomba->bombHitbox)) {
                RSDK.PlaySfx(IceBomba->explosionSFX, 0, 255);
                CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->bombAnimator, true, 0);
                entity->state = IceBomba_Fly_FlyAway;
                Ice_FreezePlayer((Entity*)player);
            }
            entity->position = prePos;
        }
    }
    entity->direction = oldDir;
}

void IceBomba_Fly_Create(void)
{
    RSDK_THIS(IceBomba);
    entity->active     = ACTIVE_NORMAL;
    entity->angle      = 0;
    entity->direction  = entity->dir;
    entity->unused_62  = entity->dir;
    entity->velocity.x = 0x8000 * (entity->dir ? 1 : -1);
    entity->state      = IceBomba_Fly_Idle;
    IceBomba_Fly_Idle();
}

void IceBomba_CheckOnScreen(void)
{
    RSDK_THIS(IceBomba);
    if (!RSDK.CheckOnScreen(entity, 0) && !RSDK.CheckPosOnScreen(&entity->spawnPos, &entity->updateRange)) {
        entity->dip >>= 7;
        entity->position.x = entity->spawnPos.x;
        entity->position.y = entity->spawnPos.y;
        IceBomba_Create(NULL);
    }
}

void IceBomba_Fly_Idle(void)
{
    RSDK_THIS(IceBomba);
    RSDK.ProcessAnimation(&entity->wingAnimator);
    entity->angle += 4;
    entity->position.x += entity->velocity.x;
    entity->position.y = entity->dip * RSDK.Sin1024(entity->angle) + entity->spawnPos.y;
    if (--entity->spawnDist <= 0) {
        RSDK.SetSpriteAnimation(0xFFFF, 1, &entity->wingAnimator, true, 0);
        entity->state = IceBomba_Fly_Turn;
    }
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &IceBomba->checkbox)) {
            EntityIceBomba *bomb = CREATE_ENTITY(IceBomba, intToVoid(1), entity->position.x, entity->position.y);
            bomb->position.x += 0x40000 * (entity->direction ? -1 : 1);
            bomb->position.y += 0x1A0000;
            bomb->direction = entity->direction;
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->bombAnimator, true, 0);
            entity->state = IceBomba_Fly_FlyAway;
            foreach_break;
        }
    }
    IceBomba_Fly_Collide();
    IceBomba_CheckOnScreen();
}

void IceBomba_Fly_Turn(void)
{
    RSDK_THIS(IceBomba);
    entity->angle += 4;
    entity->position.x += entity->velocity.x;
    entity->position.y = entity->dip * RSDK.Sin1024(entity->angle) + entity->spawnPos.y;
    entity->velocity.x += 0x1000 * (entity->direction ? -1 : 1);
    int animTimer = ++entity->animator.animationTimer;
    if (entity->direction) {
        if (animTimer >= 3) {
            --entity->bombAnimator.frameID;
            --entity->animator.frameID;
            entity->animator.animationTimer = 0;
            if (entity->animator.frameID <= 0) {
                entity->direction ^= 1;
                entity->spawnDist = 2 * entity->dist;
                entity->velocity.x = 0x8000 * (entity->direction ? 1 : -1);
                RSDK.SetSpriteAnimation(IceBomba->animID, 1, &entity->wingAnimator, true, 0);
                entity->state = IceBomba_Fly_Idle;
            }
        }
    }
    else if (animTimer >= 3) {
        ++entity->bombAnimator.frameID;
        ++entity->animator.frameID;
        entity->animator.animationTimer = 0;
        if (entity->animator.frameID >= 4) {
            entity->direction  ^= 1; //ida says its a direct =. this does not make any sense
            entity->spawnDist  = 2 * entity->dist;
            entity->velocity.x = 0x8000;
            RSDK.SetSpriteAnimation(IceBomba->animID, 1, &entity->wingAnimator, true, 0);
            entity->state = IceBomba_Fly_Idle;
        }
    }
    IceBomba_Fly_Collide();
    IceBomba_CheckOnScreen();
}

void IceBomba_Fly_FlyAway(void)
{
    RSDK_THIS(IceBomba);
    RSDK.ProcessAnimation(&entity->wingAnimator);
    if (entity->direction) {
        if (entity->animator.frameID < 4) {
            ++entity->animator.frameID;
            ++entity->bombAnimator.frameID;
        }
    }
    else {
        if (entity->animator.frameID > 0) {
            --entity->animator.frameID;
            --entity->bombAnimator.frameID;
        }
    }
    entity->position.x += entity->velocity.x;
    entity->position.y -= 0x20000;
    IceBomba_Fly_Collide();
    IceBomba_CheckOnScreen();
}

void IceBomba_Bomb_Handle(void)
{
    RSDK_THIS(IceBomba);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(entity, 0)) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            RSDK.PlaySfx(IceBomba->explosionSFX, 0, 255);
            CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            CREATE_ENTITY(Ice, intToVoid(2), entity->position.x, entity->position.y + 0x100000);
            RSDK.PlaySfx(IceBomba->freezeSFX, 0, 255);
            destroyEntity(entity);
        }
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &IceBomba->bombHitbox)) {
                RSDK.PlaySfx(IceBomba->explosionSFX, 0, 255);
                CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
                Ice_FreezePlayer((Entity*)player);
                destroyEntity(entity);
                foreach_return;
            }
        }
    }
    else
        destroyEntity(entity);
}

void IceBomba_EditorDraw(void) { IceBomba_DebugDraw(); }

void IceBomba_EditorLoad(void) { IceBomba_StageLoad(); }

void IceBomba_Serialize(void)
{
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT8, dir);
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT16, dist);
    RSDK_EDITABLE_VAR(IceBomba, VAR_UINT16, dip);
}