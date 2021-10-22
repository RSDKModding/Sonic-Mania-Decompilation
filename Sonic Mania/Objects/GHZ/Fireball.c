#include "SonicMania.h"

ObjectFireball *Fireball = NULL;

void Fireball_Update(void)
{
    RSDK_THIS(Fireball);
    StateMachine_Run(entity->state);
}

void Fireball_LateUpdate(void) {}

void Fireball_StaticUpdate(void) {}

void Fireball_Draw(void)
{
    RSDK_THIS(Fireball);
    StateMachine_Run(entity->stateDraw);
}

void Fireball_Create(void *data)
{
    RSDK_THIS(Fireball);
    entity->drawFX |= FX_ROTATE | FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawOrder     = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Fireball->aniFrames, 0, &entity->animator, true, 0);
    entity->interval *= 15;
    if (data) {
        entity->state     = (Type_StateMachine)data;
        entity->active    = ACTIVE_NORMAL;
        entity->stateDraw = Fireball_StateDraw_Normal;
    }
    else {

        switch (entity->type) {
            default: entity->groundVel <<= 7;
            case 0: entity->state = Fireball_State_Unknown1; break;
            case 1:
                entity->groundVel <<= 7;
                entity->state = Fireball_State_Unknown2;
                break;
            case 2:
                entity->groundVel <<= 7;
                entity->state = Fireball_State_Unknown3;
                break;
        }
    }
}

void Fireball_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Fireball->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE);
    Fireball->hitbox.left   = -6;
    Fireball->hitbox.top    = -6;
    Fireball->hitbox.right  = 6;
    Fireball->hitbox.bottom = 6;
    Fireball->sfxFireball   = RSDK.GetSFX("Stage/Fireball.wav");
}

void Fireball_HandlePlayerInteractions(void)
{
    RSDK_THIS(Fireball);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Fireball->hitbox)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }
}

void Fireball_State_Unknown1(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_Unknown4, entity->position.x, entity->position.y);
        fireball->angle          = entity->rotation;
        fireball->rotation       = entity->rotation;
        fireball->groundVel      = -entity->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(256 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(256 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_Unknown2(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_Unknown5, entity->position.x, entity->position.y);
        fireball->angle          = entity->rotation;
        fireball->rotation       = entity->rotation;
        fireball->groundVel      = -entity->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(256 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(256 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_Unknown3(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_Unknown6, entity->position.x, entity->position.y);
        fireball->angle          = entity->rotation;
        fireball->rotation       = entity->rotation;
        fireball->groundVel      = -entity->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(256 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(256 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_Unknown4(void)
{
    RSDK_THIS(Fireball);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->groundVel += 24;
    entity->velocity.x = entity->groundVel * RSDK.Sin512(256 - entity->angle);
    entity->velocity.y = entity->groundVel * RSDK.Cos512(256 - entity->angle);
    if (entity->groundVel > 0)
        entity->rotation = entity->angle + 256;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
    RSDK.ProcessAnimation(&entity->animator);
    Fireball_HandlePlayerInteractions();
}

void Fireball_State_Unknown5(void)
{
    RSDK_THIS(Fireball);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(512 - RSDK_sceneInfo->entity->rotation) << 10,
                                 RSDK.Cos512(512 - entity->rotation) << 10, true)) {
        entity->state = Fireball_State_Unknown7;
        RSDK.SetSpriteAnimation(Fireball->aniFrames, 1, &entity->animator, true, 0);
    }
    else {
        if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
            RSDK.ProcessAnimation(&entity->animator);
            Fireball_HandlePlayerInteractions();
        }
        else {
            destroyEntity(entity);
        }
    }
}

void Fireball_State_Unknown6(void)
{
    RSDK_THIS(Fireball);
    entity->rotation = 2 * RSDK.ATan2((entity->velocity.x >> 16), (entity->velocity.y >> 16)) + 384;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(512 - RSDK_sceneInfo->entity->rotation) << 10,
                                 RSDK.Cos512(512 - entity->rotation) << 10, true)) {
        entity->state = Fireball_State_Unknown7;
        RSDK.SetSpriteAnimation(Fireball->aniFrames, 1, &entity->animator, true, 0);
    }
    else {
        if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
            RSDK.ProcessAnimation(&entity->animator);
            Fireball_HandlePlayerInteractions();
        }
        else {
            destroyEntity(entity);
        }
    }
}

void Fireball_State_Unknown7(void)
{
    RSDK_THIS(Fireball);
    RSDK.ProcessAnimation(&entity->animator);
    Fireball_HandlePlayerInteractions();
    if (entity->animator.frameID == 2)
        destroyEntity(entity);
}

void Fireball_StateDraw_Normal(void)
{
    RSDK_THIS(Fireball);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Fireball_EditorDraw(void)
{
    RSDK_THIS(Fireball);
    StateMachine_Run(entity->stateDraw);
}

void Fireball_EditorLoad(void) { Fireball->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE); }

void Fireball_Serialize(void)
{
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Fireball, VAR_INT32, rotation);
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(Fireball, VAR_ENUM, groundVel);
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, intervalOffset);
}
