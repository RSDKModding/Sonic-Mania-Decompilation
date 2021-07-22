#include "SonicMania.h"

ObjectChopper *Chopper;

void Chopper_Update(void)
{
    RSDK_THIS(Chopper);
    StateMachine_Run(entity->state);
}

void Chopper_LateUpdate(void) {}

void Chopper_StaticUpdate(void) {}

void Chopper_Draw(void)
{
    RSDK_THIS(Chopper);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Chopper_Create(void *data)
{
    RSDK_THIS(Chopper);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x1200000;
    entity->drawFX        = FX_FLIP;
    entity->state         = Chopper_Unknown3;
}

void Chopper_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Chopper->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Chopper.bin", SCOPE_STAGE);
    Chopper->hitbox.left    = -10;
    Chopper->hitbox.top     = -20;
    Chopper->hitbox.right   = 6;
    Chopper->hitbox.bottom  = 20;
    Chopper->hitbox2.left   = -20;
    Chopper->hitbox2.top    = -6;
    Chopper->hitbox2.right  = 20;
    Chopper->hitbox2.bottom = 10;
    Chopper->hitbox3.left   = -160;
    Chopper->hitbox3.top    = -32;
    Chopper->hitbox3.right  = 16;
    Chopper->hitbox3.bottom = 32;
    Chopper->hitbox4.left   = -20;
    Chopper->hitbox4.top    = -24;
    Chopper->hitbox4.right  = 20;
    Chopper->hitbox4.bottom = -16;

    DEBUGMODE_ADD_OBJ(Chopper);
}
void Chopper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Chopper->spriteIndex, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
}

void Chopper_DebugSpawn(void)
{
    RSDK_THIS(Chopper);
    RSDK.CreateEntity(Chopper->objectID, 0, entity->position.x, entity->position.y);
}

void Chopper_CheckOnScreen(void)
{
    RSDK_THIS(Chopper);
    if (!RSDK.CheckOnScreen(entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Chopper_Create(NULL);
    }
}

void Chopper_Unknown3(void)
{
    RSDK_THIS(Chopper);

    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = -0x10000;
    if (entity->type == 0) {
        RSDK.SetSpriteAnimation(Chopper->spriteIndex, 0, &entity->animator, true, 0);
        entity->state = Chopper_Unknown4;
        Chopper_Unknown4();
    }
    else {
        RSDK.SetSpriteAnimation(Chopper->spriteIndex, 1, &entity->animator, true, 0);
        entity->state = Chopper_Unknown5;
        entity->timer = 512;

        if (!entity->direction)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
        Chopper_Unknown5();
    }
}

void Chopper_Unknown4(void)
{
    RSDK_THIS(Chopper);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;
    if (entity->velocity.y >= -0x38000) {
        if (entity->velocity.y <= 0x38000) {
            entity->animator.animationSpeed = 2;
        }
        else {
            entity->animator.frameID        = 0;
            entity->animator.animationSpeed = 0;
        }
    }
    else {
        entity->animator.animationSpeed = 1;
    }

    if (entity->position.y > entity->startPos.y) {
        entity->position.y = entity->startPos.y;
        entity->velocity.y = -0x70000;
    }
    RSDK.ProcessAnimation(&entity->animator);
    Chopper_CheckPlayerCollisions();
    Chopper_CheckOnScreen();
}

void Chopper_Unknown5(void)
{
    RSDK_THIS(Chopper);
    entity->position.x += entity->velocity.x;

    bool32 flag = false;
    if (entity->direction) {
        flag = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0xF0000, true);
    }
    else {
        flag = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0xF0000, true);
    }

    if (--entity->timer <= 0 || flag) {
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
        entity->timer      = 512;
    }

    if (entity->charge) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Chopper->hitbox3)) {
                entity->state = Chopper_Unknown6;
                RSDK.SetSpriteAnimation(Chopper->spriteIndex, 2, &entity->animator, 0, 0);
                entity->timer = 16;
                if (!entity->direction)
                    entity->velocity.x = -0x20000;
                else
                    entity->velocity.x = 0x20000;
                if (abs(player->position.y - entity->position.y) > 0x100000) {
                    if (player->position.y < entity->position.y)
                        entity->velocity.y = -0x8000;
                    else
                        entity->velocity.y = 0x8000;
                }
            }
        }
    }
    RSDK.ProcessAnimation(&entity->animator);
    Chopper_CheckPlayerCollisions2();
    Chopper_CheckOnScreen();
}

void Chopper_Unknown6(void)
{
    RSDK_THIS(Chopper);

    if (!entity->timer)
        entity->state = Chopper_Unknown7;
    RSDK.ProcessAnimation(&entity->animator);
    Chopper_CheckPlayerCollisions2();
    Chopper_CheckOnScreen();
}

void Chopper_Unknown7(void)
{
    RSDK_THIS(Chopper);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    bool32 flag = false;
    if (entity->direction) {
        flag = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x100000, 0xF0000, true);
    }
    else {
        flag = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, -0xF0000, true);
        flag |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x100000, 0xF0000, true);
    }

    if (flag) {
        RSDK.SetSpriteAnimation(Chopper->spriteIndex, 1, &entity->animator, false, 0);
        entity->timer      = 512;
        entity->velocity.y = 0;
        if (entity->direction == FLIP_X)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
        entity->direction ^= FLIP_X;
        entity->state = Chopper_Unknown5;
        flag          = false;
    }
    else if (entity->velocity.y >= 0) {
        if (entity->velocity.y > 0)
            flag = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true);
    }
    else {
        flag = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x100000, true);
        foreach_active(Water, water)
        {
            if (water->type == 1)
                flag |= !RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, entity, &Chopper->hitbox4);
        }
    }

    if (flag) {
        RSDK.SetSpriteAnimation(Chopper->spriteIndex, 1, &entity->animator, false, 0);
        entity->timer      = 512;
        entity->velocity.y = 0;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
        entity->state = Chopper_Unknown5;
    }

    RSDK.ProcessAnimation(&entity->animator);
    Chopper_CheckPlayerCollisions2();
    Chopper_CheckOnScreen();
}

void Chopper_CheckPlayerCollisions(void)
{
    RSDK_THIS(Chopper);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Chopper->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Chopper_CheckPlayerCollisions2(void)
{
    RSDK_THIS(Chopper);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Chopper->hitbox2))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Chopper_EditorDraw(void) {}

void Chopper_EditorLoad(void) {}

void Chopper_Serialize(void)
{
    RSDK_EDITABLE_VAR(Chopper, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Chopper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Chopper, VAR_BOOL, charge);
}
