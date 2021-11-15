#include "SonicMania.h"

ObjectLRZFireball *LRZFireball;

void LRZFireball_Update(void)
{
    RSDK_THIS(LRZFireball);
    StateMachine_Run(entity->state);
}

void LRZFireball_LateUpdate(void) {}

void LRZFireball_StaticUpdate(void) {}

void LRZFireball_Draw(void)
{
    RSDK_THIS(LRZFireball);
    StateMachine_Run(entity->stateDraw);
}

void LRZFireball_Create(void *data)
{
    RSDK_THIS(LRZFireball);

    entity->drawFX |= FX_FLIP | FX_ROTATE;
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    if (data) {
        entity->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 3, &entity->animator, true, 0);
        entity->state     = data;
        entity->active    = ACTIVE_NORMAL;
        entity->stateDraw = LRZFireball_StateDraw_Visible;
    }
    else {
        entity->drawOrder = Zone->drawOrderLow + 1;

        switch (entity->type) {
            case 0: entity->state = LRZFireball_State_Type0; break;
            case 1:
                RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &entity->animator, true, 0);
                entity->state     = LRZFireball_State_Type1;
                entity->stateDraw = LRZFireball_StateDraw_Visible;
                break;
            case 2:
                RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &entity->animator, true, 0);
                entity->state     = LRZFireball_State_Type2;
                entity->stateDraw = LRZFireball_StateDraw_Visible;
                break;
        }

        if (!RSDK_sceneInfo->inEditor)
            entity->groundVel <<= 7;
    }
}

void LRZFireball_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        LRZFireball->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LRZFireball.bin", SCOPE_STAGE);
    LRZFireball->hitbox.left   = -6;
    LRZFireball->hitbox.top    = -6;
    LRZFireball->hitbox.right  = 6;
    LRZFireball->hitbox.bottom = 6;
    LRZFireball->sfxFireball   = RSDK.GetSFX("Stage/Fireball.wav");
}

void LRZFireball_CheckPlayerCollisions(void)
{
    RSDK_THIS(LRZFireball);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &LRZFireball->hitbox)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }
}

void LRZFireball_CheckOnScreen(void)
{
    RSDK_THIS(LRZFireball);

    entity->alpha -= 0x20;
    if (entity->alpha <= 0 || (entity->alpha && RSDK.CheckOnScreen(entity, &entity->updateRange))) {
        RSDK.ProcessAnimation(&entity->animator);
        LRZFireball_CheckPlayerCollisions();
    }
    else {
        destroyEntity(entity);
    }
}

void LRZFireball_CheckTileCollisions(void)
{
    RSDK_THIS(LRZFireball);

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(0x200 - entity->rotation) << 11,
                                 RSDK.Cos512(0x200 - entity->rotation) << 11, false)
        && !entity->alpha) {
        entity->inkEffect = INK_ALPHA;
        entity->alpha     = 0x100;
    }
}

void LRZFireball_State_Type0(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        EntityLRZFireball *child = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type0, entity->position.x, entity->position.y);

        child->angle      = entity->rotation;
        child->rotation   = entity->rotation;
        child->groundVel  = -entity->groundVel;
        child->velocity.x = child->groundVel * RSDK.Sin512(0x100 - child->angle);
        child->velocity.y = child->groundVel * RSDK.Cos512(0x100 - child->angle);
        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
    }
}

void LRZFireball_State_Type1(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        EntityLRZFireball *child = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type1, entity->position.x, entity->position.y);

        child->angle      = entity->rotation;
        child->rotation   = entity->rotation;
        child->groundVel  = -entity->groundVel;
        child->velocity.x = child->groundVel * RSDK.Sin512(0x100 - child->angle);
        child->velocity.y = child->groundVel * RSDK.Cos512(0x100 - child->angle);
        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
    }
}

void LRZFireball_State_Type2(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        EntityLRZFireball *child = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type2, entity->position.x, entity->position.y);

        child->angle      = entity->rotation;
        child->rotation   = entity->rotation;
        child->groundVel  = -entity->groundVel;
        child->velocity.x = child->groundVel * RSDK.Sin512(0x100 - child->angle);
        child->velocity.y = child->groundVel * RSDK.Cos512(0x100 - child->angle);
        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
    }
}

void LRZFireball_StateChild_Type0(void)
{
    RSDK_THIS(LRZFireball);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->groundVel += 0x18;
    entity->velocity.x = entity->groundVel * RSDK.Sin512(0x100 - entity->angle);
    entity->velocity.y = entity->groundVel * RSDK.Cos512(0x100 - entity->angle);
    if (entity->groundVel > 0)
        entity->rotation = entity->angle + 0x100;

    LRZFireball_CheckOnScreen();
}

void LRZFireball_StateChild_Type1(void)
{
    RSDK_THIS(LRZFireball);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    LRZFireball_CheckTileCollisions();
    LRZFireball_CheckOnScreen();
}

void LRZFireball_StateChild_Type2(void)
{
    RSDK_THIS(LRZFireball);

    entity->rotation = 2 * RSDK.ATan2(entity->velocity.x >> 16, entity->velocity.y >> 16) + 0x180;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    LRZFireball_CheckTileCollisions();
    LRZFireball_CheckOnScreen();
}

void LRZFireball_StateDraw_Visible(void)
{
    RSDK_THIS(LRZFireball);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void LRZFireball_EditorDraw(void)
{
    RSDK_THIS(LRZFireball);

    switch (entity->type) {
        case 0: RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 3, &entity->animator, true, 0); break;
        case 1:
            RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &entity->animator, true, 0);
            entity->stateDraw = LRZFireball_StateDraw_Visible;
            break;
        case 2:
            RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &entity->animator, true, 0);
            entity->stateDraw = LRZFireball_StateDraw_Visible;
            break;
    }

    LRZFireball_StateDraw_Visible();

    // TODO: transparent fireball strength indicator
}

void LRZFireball_EditorLoad(void) { LRZFireball->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LRZFireball.bin", SCOPE_STAGE); }
#endif

void LRZFireball_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_INT32, rotation);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_ENUM, groundVel);
}
