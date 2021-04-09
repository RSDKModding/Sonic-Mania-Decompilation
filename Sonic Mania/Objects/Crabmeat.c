#include "../SonicMania.h"

ObjectCrabmeat *Crabmeat;

void Crabmeat_Update(void)
{
    RSDK_THIS(Crabmeat);
    StateMachine_Run(entity->state);
}

void Crabmeat_LateUpdate(void) {}

void Crabmeat_StaticUpdate(void) {}

void Crabmeat_Draw(void)
{
    RSDK_THIS(Crabmeat);
    RSDK.DrawSprite(&entity->data, 0, 0);
}

void Crabmeat_Create(void *data)
{
    RSDK_THIS(Crabmeat);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    if (data) {
        entity->inkEffect     = INK_ADD;
        entity->alpha         = 0xC0;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x400000;
        entity->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Crabmeat->spriteIndex, 3, &entity->data, true, 0);
        entity->state = Crabmeat_State_Projectile;
    }
    else {
        entity->drawFX |= FX_FLIP;
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->startDir      = entity->direction;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Crabmeat->spriteIndex, 0, &entity->data, true, 0);
        entity->state = Crabmeat_Unknown1;
    }
}

void Crabmeat_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Crabmeat->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Crabmeat.bin", SCOPE_STAGE);
    Crabmeat->hitbox.left             = -14;
    Crabmeat->hitbox.top              = -14;
    Crabmeat->hitbox.right            = 14;
    Crabmeat->hitbox.bottom           = 14;
    Crabmeat->projectileHitbox.left   = -6;
    Crabmeat->projectileHitbox.top    = -6;
    Crabmeat->projectileHitbox.right  = 6;
    Crabmeat->projectileHitbox.bottom = 6;

    DEBUGMODE_ADD_OBJ(Crabmeat);
}

void Crabmeat_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Crabmeat->spriteIndex, 0, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
}
void Crabmeat_DebugSpawn(void)
{
    RSDK_THIS(Crabmeat);
    RSDK.CreateEntity(Crabmeat->objectID, 0, entity->position.x, entity->position.y);
}

void Crabmeat_State_Main(void)
{
    RSDK_THIS(Crabmeat);

    entity->position.x += entity->velocity.x;
    if (entity->timer >= 128
        || (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, ((entity->velocity.x >> 31) & 0xFFE40000) + 0xE0000, 0xF0000, 8))) {
        entity->timer = 0;
        if (entity->dword60 == 0)
            RSDK.SetSpriteAnimation(Crabmeat->spriteIndex, 0, &entity->data, true, 0);
        else
            RSDK.SetSpriteAnimation(Crabmeat->spriteIndex, 2, &entity->data, true, 0);
        entity->state = Crabmeat_State_Shoot;
    }
    else {
        entity->timer++;
    }
    RSDK.ProcessAnimation(&entity->data);
    Crabmeat_CheckPlayerCollisions();
    Crabmeat_CheckOnScreen();
}

void Crabmeat_State_Projectile(void)
{
    RSDK_THIS(Crabmeat);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (!RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        RSDK.ProcessAnimation(&entity->data);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Crabmeat->projectileHitbox))
                Player_CheckProjectileHit(player, entity);
        }
    }
}

void Crabmeat_State_Shoot(void)
{
    RSDK_THIS(Crabmeat);

    if (entity->timer >= 60) {
        switch (entity->dword60) {
            default:
                if (entity->data.frameID != entity->data.frameCount - 1)
                    break;
            case 0:
                entity->dword60 = 1;
                RSDK.SetSpriteAnimation(Crabmeat->spriteIndex, 1, &entity->data, true, 0);
                entity->direction ^= 1u;
                entity->velocity.x = -entity->velocity.x;
                entity->timer      = 0;
                entity->state      = Crabmeat_State_Main;
                break;
            case 1:
                entity->dword60 = 2;

                EntityCrabmeat *projectile =
                    (EntityCrabmeat *)RSDK.CreateEntity(Crabmeat->objectID, (void *)1, entity->position.x - 0x100000, entity->position.y);
                projectile->velocity.x = -0x10000;
                projectile->velocity.y = -0x40000;

                projectile = (EntityCrabmeat *)RSDK.CreateEntity(Crabmeat->objectID, (void *)1, entity->position.x + 0x100000, entity->position.y);
                projectile->velocity.x = 0x10000;
                projectile->velocity.y = -0x40000;
                break;
        }
    }
    else {
        entity->timer++;
    }

    RSDK.ProcessAnimation(&entity->data);
    Crabmeat_CheckPlayerCollisions();
    Crabmeat_CheckOnScreen();
}

void Crabmeat_Unknown1(void)
{
    RSDK_THIS(Crabmeat);

    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = -0x8000;
    RSDK.SetSpriteAnimation(Crabmeat->spriteIndex, 1, &entity->data, true, 0);
    entity->state = Crabmeat_State_Main;
    Crabmeat_State_Main();
}

void Crabmeat_CheckOnScreen(void)
{
    RSDK_THIS(Crabmeat);
    if (entity->tileCollisions && !RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        entity->timer      = 0;
        Crabmeat_Create(NULL);
    }
}

void Crabmeat_CheckPlayerCollisions(void)
{
    RSDK_THIS(Crabmeat);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Crabmeat->hitbox))
            Player_CheckBadnikBreak((Entity*)entity, player, true);
    }
}

void Crabmeat_EditorDraw(void) {}

void Crabmeat_EditorLoad(void) {}

void Crabmeat_Serialize(void) {}
