#include "SonicMania.h"

ObjectBubbler *Bubbler = NULL;

void Bubbler_Update(void)
{
    RSDK_THIS(Bubbler);
    StateMachine_Run(entity->state);
}

void Bubbler_LateUpdate(void) {}

void Bubbler_StaticUpdate(void) {}

void Bubbler_Draw(void)
{
    RSDK_THIS(Bubbler);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->startPos.x)
        RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void Bubbler_Create(void *data)
{
    RSDK_THIS(Bubbler);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 3, &entity->animator1, true, 0);
        entity->state = Bubbler_State_Projectile_Unknown1;
    }
    else {
        entity->startPos   = entity->position;
        entity->startDir   = entity->direction;
        entity->velocity.y = 0;
        if (!entity->direction)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
        entity->timer  = 0;
        entity->timer2 = 32;
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 1, &entity->animator2, true, 0);
        entity->state = Bubbler_State_Setup;
    }
}

void Bubbler_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        Bubbler->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Bubbler.bin", SCOPE_STAGE);
    Bubbler->hitbox1.left   = -16;
    Bubbler->hitbox1.top    = -12;
    Bubbler->hitbox1.right  = 16;
    Bubbler->hitbox1.bottom = 12;
    Bubbler->hitbox2.left   = -160;
    Bubbler->hitbox2.top    = -12;
    Bubbler->hitbox2.right  = 16;
    Bubbler->hitbox2.bottom = 96;
    Bubbler->hitbox3.left   = -2;
    Bubbler->hitbox3.top    = -2;
    Bubbler->hitbox3.right  = 2;
    Bubbler->hitbox3.bottom = 2;
    DEBUGMODE_ADD_OBJ(Bubbler);
}

void Bubbler_DebugSpawn(void)
{
    RSDK_THIS(Bubbler);
    CREATE_ENTITY(Bubbler, NULL, entity->position.x, entity->position.y);
}

void Bubbler_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bubbler->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Bubbler_HandleInteractions(void)
{
    RSDK_THIS(Bubbler);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Bubbler->hitbox1))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Bubbler_HandleProjectileInteractions(void)
{
    RSDK_THIS(Bubbler);
    int32 distance = 0x7FFFFFFF;
    foreach_active(Player, player)
    {
        if (abs(player->position.x - entity->position.x) < distance) {
            distance          = abs(player->position.x - entity->position.x);
            entity->direction = player->position.x >= entity->position.x;
        }

        if (Player_CheckCollisionTouch(player, entity, &Bubbler->hitbox3)) {
            Player_CheckHit(player, entity);
        }
    }
}

void Bubbler_CheckOnScreen(void)
{
    RSDK_THIS(Bubbler);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Bubbler_Create(NULL);
    }
}

void Bubbler_State_Setup(void)
{
    RSDK_THIS(Bubbler);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = -0x10000;
    entity->state      = Bubbler_State_Unknown1;
    Bubbler_State_Unknown1();
}

void Bubbler_State_Unknown1(void)
{
    RSDK_THIS(Bubbler);
    entity->position.x += entity->velocity.x;
    RSDK.ProcessAnimation(&entity->animator2);

    if (!--entity->timer) {
        entity->direction ^= 1;
        entity->velocity.x = -entity->velocity.x;
        entity->timer      = 512;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Bubbler->hitbox2)) {
            entity->timer = 16;
            entity->state = Bubbler_State_Unknown2;
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = -0x28000;
            else
                entity->velocity.x = 0x28000;
        }
    }

    Bubbler_HandleInteractions();
    Bubbler_CheckOnScreen();
}

void Bubbler_State_Unknown2(void)
{
    RSDK_THIS(Bubbler);

    RSDK.ProcessAnimation(&entity->animator2);
    if (--entity->timer & 0x8000) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 2, &entity->animator2, true, 0);
        entity->state = Bubbler_State_Unknown3;
    }
    Bubbler_HandleInteractions();
    Bubbler_CheckOnScreen();
}

void Bubbler_State_Unknown3(void)
{
    RSDK_THIS(Bubbler);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (++entity->timer2 >= 30) {
        entity->timer2 = 0;
        int32 spawnX     = entity->position.x + 0x60000;
        if (entity->direction)
            spawnX = entity->position.x - 0x60000;
        CREATE_ENTITY(Bubbler, intToVoid(true), spawnX, entity->position.y + 0xA0000)->active = ACTIVE_NORMAL;
    }
    Bubbler_HandleInteractions();
    Bubbler_CheckOnScreen();
}

void Bubbler_State_Projectile_Unknown1(void)
{
    RSDK_THIS(Bubbler);
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.y += 0x10000;
    Bubbler_HandleProjectileInteractions();
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, false)) {
        RSDK.SetSpriteAnimation(Bubbler->aniFrames, 4, &entity->animator1, true, 0);
        entity->state = Bubbler_State_Projectile_Unknown2;
    }

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void Bubbler_State_Projectile_Unknown2(void)
{
    RSDK_THIS(Bubbler);
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID >= 7)
        destroyEntity(entity);
    else if (entity->animator1.frameID < 6)
        Bubbler_HandleProjectileInteractions();
}

#if RETRO_INCLUDE_EDITOR
void Bubbler_EditorDraw(void) {}

void Bubbler_EditorLoad(void) {}
#endif

void Bubbler_Serialize(void) { RSDK_EDITABLE_VAR(Bubbler, VAR_UINT8, direction); }
