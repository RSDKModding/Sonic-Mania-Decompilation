#include "SonicMania.h"

ObjectBall *Ball = NULL;

void Ball_Update(void)
{
    RSDK_THIS(Ball);
    StateMachine_Run(entity->state);
}

void Ball_LateUpdate(void) {}

void Ball_StaticUpdate(void) {}

void Ball_Draw(void)
{
    RSDK_THIS(Ball);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Ball_Create(void *data)
{
    RSDK_THIS(Ball);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Ball->aniFrames, 2, &entity->animator, true, 0);
        entity->state = Ball_State2_Unknown1;
    }
    else {
        entity->startPos = entity->position;
        if (!entity->type) {
            RSDK.SetSpriteAnimation(Ball->aniFrames, 0, &entity->animator, true, 0);
            entity->state = Ball_State_Setup;
        }
        else {
            entity->state = Ball_State4_Unknown1;
        }
    }
}

void Ball_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ"))
        Ball->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Ball.bin", SCOPE_STAGE);
    Ball->hitbox1.left   = -10;
    Ball->hitbox1.top    = -10;
    Ball->hitbox1.right  = 10;
    Ball->hitbox1.bottom = 10;
    Ball->hitbox2.left   = -128;
    Ball->hitbox2.top    = -128;
    Ball->hitbox2.right  = 128;
    Ball->hitbox2.bottom = 128;
    Ball->sfxSplash      = RSDK.GetSFX("Stage/Splash2.wav");
    DEBUGMODE_ADD_OBJ(Ball);
}

void Ball_DebugSpawn(void)
{
    RSDK_THIS(Ball);
    CREATE_ENTITY(Ball, NULL, entity->position.x, entity->position.y);
}

void Ball_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Ball->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Ball_HandleInteractions(void)
{
    RSDK_THIS(Ball);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Ball->hitbox1)) {
            Player_CheckHit(player, entity);
            CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfx_Destroy, false, 255);
            entity->velocity.y = 0;
            RSDK.SetSpriteAnimation(Ball->aniFrames, 1, &entity->animator, true, 0);
            entity->state = Ball_State_Unknown3;
        }
    }
}

void Ball_CheckOnScreen(void)
{
    RSDK_THIS(Ball);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position   = entity->startPos;
        entity->direction  = FLIP_NONE;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        Ball_Create(NULL);
    }
}

void Ball_SpawnChildren(void)
{
    RSDK_THIS(Ball);
    RSDK.PlaySfx(Ball->sfxSplash, false, 255);
    for (int i = 0; i < 5; ++i) {
        EntityBall *ball = CREATE_ENTITY(Ball, intToVoid(true), entity->position.x, entity->position.y);
        ball->drawOrder  = Zone->drawOrderHigh;
        ball->velocity.x = RSDK.Rand(-0x100, 0x100) << 10;
        if (ball->velocity.x < 0)
            ball->velocity.x += 0x20000;
        ball->velocity.x -= 0x10000;
        if (i > 0)
            ball->velocity.y = RSDK.Rand(-0x400, 0x400) << 8;
        ball->velocity.y -= entity->velocity.y >> 1;
    }
    destroyEntity(entity);
}

void Ball_State_Setup(void)
{
    RSDK_THIS(Ball);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Ball_State_Unknown1;
    Ball_State_Unknown1();
}

void Ball_State_Unknown1(void)
{
    RSDK_THIS(Ball);
    if (entity->direction == FLIP_X) {
        entity->velocity.y += 0x800;
        if (entity->velocity.y >= 0xC000)
            entity->direction = FLIP_NONE;
    }
    else {
        entity->velocity.y -= 0x800;
        if (entity->velocity.y <= -0xC000)
            entity->direction = FLIP_X;
    }
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->animator);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Ball->hitbox2)) {
            entity->playerPtr = (Entity *)player;
            entity->state     = Ball_State_Unknown2;
        }
    }
    Ball_HandleInteractions();
    Ball_CheckOnScreen();
}

void Ball_State_Unknown2(void)
{
    RSDK_THIS(Ball);
    Entity *playerPtr = entity->playerPtr;
    if (entity->position.x <= playerPtr->position.x) {
        entity->velocity.x += 0x1000;
        if (entity->velocity.x > 0x20000)
            entity->velocity.x = 0x20000;
    }
    else {
        entity->velocity.x -= 0x1000;
        if (entity->velocity.x < -0x20000)
            entity->velocity.x = -0x20000;
    }
    if (entity->position.y <= playerPtr->position.y - 0x400000) {
        entity->velocity.y += 0x1000;
        if (entity->velocity.y > 0x20000)
            entity->velocity.y = 0x20000;
    }
    else {
        entity->velocity.y -= 0x1000;
        if (entity->velocity.y < -0x20000)
            entity->velocity.y = -0x20000;
    }
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (abs(entity->position.x - playerPtr->position.x) < 0x100000) {
        if (abs(0x500000 + entity->position.y - playerPtr->position.y) < 0x100000 && RSDK.CheckOnScreen(entity, &entity->updateRange)) {
            RSDK.CreateEntity(Explosion->objectID, intToVoid(1), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfx_Destroy, false, 255);
            entity->velocity.y = 0;
            RSDK.SetSpriteAnimation(Ball->aniFrames, 1, &entity->animator, true, 0);
            entity->state = Ball_State_Unknown3;
        }
    }
    RSDK.ProcessAnimation(&entity->animator);
    Ball_HandleInteractions();
    Ball_CheckOnScreen();
}

void Ball_State_Unknown3(void)
{
    RSDK_THIS(Ball);
    RSDK.ProcessAnimation(&entity->animator);
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xB0000, false)) {
        Ball_SpawnChildren();
    }
    else {
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Ball->hitbox1)) {
                if (player->shield != SHIELD_BUBBLE)
                    Player_CheckHit(player, entity);
                Ball_SpawnChildren();
            }
        }

        if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
}

void Ball_State2_Unknown1(void)
{
    RSDK_THIS(Ball);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void Ball_State3_Unknown1(void)
{
    RSDK_THIS(Ball);
    RSDK.ProcessAnimation(&entity->animator);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->velocity.x < 0)
        entity->velocity.x += 0x400;
    if (entity->velocity.x > 0)
        entity->velocity.x -= 0x400;

    if (entity->velocity.y < 0)
        entity->velocity.y += 0x400;
    if (entity->velocity.y > 0)
        entity->velocity.y -= 0x400;

    if (!entity->velocity.x && !entity->velocity.y) {
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
        entity->state      = Ball_State_Unknown1;
    }
    Ball_HandleInteractions();
}

void Ball_State4_Unknown1(void)
{
    RSDK_THIS(Ball);
    EntityBall *child = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, Ball);
    if (child->objectID != Ball->objectID) {
        RSDK.ResetEntityPtr(child, Ball->objectID, NULL);
        child->active     = ACTIVE_NORMAL;
        child->position.x = entity->position.x;
        child->position.y = entity->position.y;
        child->state      = Ball_State3_Unknown1;
        switch (entity->type) {
            case 1: child->velocity.x = -0x20000; break;
            case 2:
                child->velocity.y = -0x20000;
                child->state      = Ball_State3_Unknown1;
                break;
            case 3:
                child->velocity.x = 0x20000;
                child->state      = Ball_State3_Unknown1;
                break;
            case 4:
                child->velocity.y = 0x20000;
                child->state      = Ball_State3_Unknown1;
                break;
            default: break;
        }
    }
}

void Ball_EditorDraw(void) {}

void Ball_EditorLoad(void) {}

void Ball_Serialize(void) { RSDK_EDITABLE_VAR(Ball, VAR_UINT8, type); }
