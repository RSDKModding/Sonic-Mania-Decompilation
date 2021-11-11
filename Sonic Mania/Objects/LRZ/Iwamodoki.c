#include "SonicMania.h"

ObjectIwamodoki *Iwamodoki;

void Iwamodoki_Update(void)
{
    RSDK_THIS(Iwamodoki);

    if (LRZConvItem && entity->lrzConvPhys) {
        entity->preMovePos.x = entity->position.x;
        entity->preMovePos.y = entity->position.y;
        entity->moveOffset   = LRZConvItem_Unknown2(entity);
    }
    else {
        entity->preMovePos.x = entity->position.x;
        entity->preMovePos.y = entity->position.y;
        entity->moveOffset.x = 0;
        entity->moveOffset.y = 0;
    }
    entity->postMovePos.x = entity->position.x;
    entity->postMovePos.y = entity->position.y;

    StateMachine_Run(entity->state);
}

void Iwamodoki_LateUpdate(void) {}

void Iwamodoki_StaticUpdate(void) {}

void Iwamodoki_Draw(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Iwamodoki_Create(void *data)
{
    RSDK_THIS(Iwamodoki);

    entity->drawFX |= FX_FLIP;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->visible       = true;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawOrder     = Zone->drawOrderLow;

    if (data) {
        entity->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, voidToInt(data) + 2, &entity->animator, true, 0);
        entity->state = Iwamodoki_State_Debris;
    }
    else {
        RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 0, &entity->animator, true, 0);
        entity->state = Iwamodoki_State_Setup;
    }
}

void Iwamodoki_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Iwamodoki.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Iwamodoki.bin", SCOPE_STAGE);

    Iwamodoki->hitbox1.left   = -12;
    Iwamodoki->hitbox1.top    = -11;
    Iwamodoki->hitbox1.right  = 12;
    Iwamodoki->hitbox1.bottom = 11;

    Iwamodoki->hitbox2.left   = -64;
    Iwamodoki->hitbox2.top    = -64;
    Iwamodoki->hitbox2.right  = 64;
    Iwamodoki->hitbox2.bottom = 64;

    Iwamodoki->hitbox3.left   = -4;
    Iwamodoki->hitbox3.top    = -4;
    Iwamodoki->hitbox3.right  = 4;
    Iwamodoki->hitbox3.bottom = 4;

    DEBUGMODE_ADD_OBJ(Iwamodoki);
}

void Iwamodoki_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Iwamodoki, NULL, entity->position.x, entity->position.y);
}

void Iwamodoki_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Iwamodoki_HandlePlayerCollisions(void)
{
    RSDK_THIS(Iwamodoki);

    entity->position.x = entity->preMovePos.x;
    entity->position.y = entity->preMovePos.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &Iwamodoki->hitbox1) == C_TOP) {
            player->position.x += entity->moveOffset.x;
            player->position.y += entity->moveOffset.y;
        }
    }

    entity->position.x = entity->postMovePos.x;
    entity->position.y = entity->postMovePos.y;
}

void Iwamodoki_CheckOnScreen(void)
{
    RSDK_THIS(Iwamodoki);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Iwamodoki_Create(NULL);
    }
}

void Iwamodoki_State_Setup(void)
{
    RSDK_THIS(Iwamodoki);

    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = -0x10000;
    entity->state      = Iwamodoki_State_Unknown1;
    Iwamodoki_State_Unknown1();
}

void Iwamodoki_State_Unknown1(void)
{
    RSDK_THIS(Iwamodoki);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Iwamodoki->hitbox2)) {
            entity->state = Iwamodoki_State_Unknown2;
        }
    }

    Iwamodoki_HandlePlayerCollisions();
    Iwamodoki_CheckOnScreen();
}

void Iwamodoki_State_Unknown2(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == 6) {
        RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 1, &entity->animator, true, 0);
        entity->timer2 = 15;
        entity->timer1 = 15;
        entity->state  = Iwamodoki_State_Unknown3;
    }
    Iwamodoki_HandlePlayerCollisions();
    Iwamodoki_CheckOnScreen();
}

void Iwamodoki_State_Unknown3(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&entity->animator);

    if (!--entity->timer1) {
        if (!--entity->timer2) {
            entity->timer1 = 32;
            RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 2, &entity->animator, true, 0);
            entity->state = Iwamodoki_State_Unknown4;
        }
        else {
            RSDK.SetSpriteAnimation(Iwamodoki->aniFrames, 1, &entity->animator, true, 0);
            entity->timer1 = entity->timer2;
        }
    }
    Iwamodoki_HandlePlayerCollisions();
    Iwamodoki_CheckOnScreen();
}

void Iwamodoki_State_Unknown4(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&entity->animator);
    if (!--entity->timer1) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
        if (entity->activeScreens == 1)
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);

        EntityIwamodoki *debris = CREATE_ENTITY(Iwamodoki, intToVoid(1), entity->position.x, entity->position.y);
        debris->velocity.x      = -0x20000;
        debris->velocity.y      = -0x40000;

        debris             = CREATE_ENTITY(Iwamodoki, intToVoid(1), entity->position.x, entity->position.y);
        debris->velocity.x = 0x20000;
        debris->velocity.y = -0x40000;

        debris             = CREATE_ENTITY(Iwamodoki, intToVoid(2), entity->position.x, entity->position.y);
        debris->velocity.x = -0x40000;
        debris->velocity.y = -0x20000;

        debris             = CREATE_ENTITY(Iwamodoki, intToVoid(2), entity->position.x, entity->position.y);
        debris->velocity.x = 0x40000;
        debris->velocity.y = -0x20000;

        destroyEntity(entity);
    }
    else {
        Iwamodoki_HandlePlayerCollisions();
        Iwamodoki_CheckOnScreen();
    }
}

void Iwamodoki_State_Debris(void)
{
    RSDK_THIS(Iwamodoki);

    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Iwamodoki->hitbox3)) {
            Player_CheckProjectileHit(player, entity);
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Iwamodoki_EditorDraw(void) { Iwamodoki_Draw(); }

void Iwamodoki_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Iwamodoki.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("LRZ2") || RSDK.CheckStageFolder("LRZ3"))
        Iwamodoki->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Iwamodoki.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Iwamodoki, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void Iwamodoki_Serialize(void)
{
    RSDK_EDITABLE_VAR(Iwamodoki, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Iwamodoki, VAR_BOOL, lrzConvPhys);
}
