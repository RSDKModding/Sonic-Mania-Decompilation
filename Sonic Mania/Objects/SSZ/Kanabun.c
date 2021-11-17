#include "SonicMania.h"

ObjectKanabun *Kanabun;

void Kanabun_Update(void)
{
    RSDK_THIS(Kanabun);
    StateMachine_Run(entity->state);
}

void Kanabun_LateUpdate(void) {}

void Kanabun_StaticUpdate(void) {}

void Kanabun_Draw(void)
{
    RSDK_THIS(Kanabun);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Kanabun_Create(void *data)
{
    RSDK_THIS(Kanabun);

    if (!entity->angleVel)
        entity->angleVel = 1;
    if (!entity->hVel) {
        entity->hDist    = 1;
        entity->bobDist  = 2;
        entity->hVel     = 0x4000;
        entity->angleVel = 2;
    }

    entity->drawFX = FX_FLIP | FX_ROTATE | FX_SCALE;
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->startPos      = entity->position;
        entity->startDir      = entity->direction;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Kanabun->aniFrames, 0, &entity->animator, true, 0);
        entity->state = Kanabun_State_Setup;
    }
}

void Kanabun_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Kanabun->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Kanabun.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Kanabun->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Kanabun.bin", SCOPE_STAGE);

    Kanabun->hitbox.left   = -6;
    Kanabun->hitbox.top    = -6;
    Kanabun->hitbox.right  = 6;
    Kanabun->hitbox.bottom = 6;
    DEBUGMODE_ADD_OBJ(Kanabun);
}

void Kanabun_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Kanabun, NULL, entity->position.x, entity->position.y);
}

void Kanabun_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Kanabun->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Kanabun_CheckPlayerCollisions(void)
{
    RSDK_THIS(Kanabun);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Kanabun->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Kanabun_CheckOnScreen(void)
{
    RSDK_THIS(Kanabun);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Kanabun_Create(NULL);
    }
}

void Kanabun_HandleMovement(void)
{
    RSDK_THIS(Kanabun);

    entity->position.x += entity->velocity.x;
    entity->position.y = ((entity->bobDist * RSDK.Sin512(entity->angle)) << 8) + entity->startPos.y;
    entity->angle += entity->angleVel;

    entity->scale.x = (abs(RSDK.Sin512(((entity->angle >> 1) + 0x80) & 0x1FF)) >> 1) + 0x100;
    entity->scale.y = entity->scale.x;
    if (((uint32)(entity->angle - 0x80) & 0x1FF) >= 0x100)
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
}

void Kanabun_State_Setup(void)
{
    RSDK_THIS(Kanabun);

    entity->velocity.x = entity->hVel;
    if (entity->direction == FLIP_NONE)
        entity->velocity.x = -entity->velocity.x;
    entity->active = ACTIVE_NORMAL;

    entity->state = Kanabun_State_Unknown1;
    Kanabun_State_Unknown1();
}

void Kanabun_State_Unknown1(void)
{
    RSDK_THIS(Kanabun);

    Kanabun_HandleMovement();
    if (entity->position.y >= entity->startPos.y) {
        if (entity->groundVel == 1)
            entity->groundVel = 0;
    }
    else if (!entity->groundVel) {
        entity->groundVel = 1;
    }

    int vel = entity->hVel * entity->hDist * (0x100 / entity->angleVel);
    if ((entity->direction == FLIP_NONE && entity->position.x <= (entity->startPos.x - vel))
        || (entity->direction == FLIP_X && entity->position.x >= (vel + entity->startPos.x))) {
        RSDK.SetSpriteAnimation(Kanabun->aniFrames, 1, &entity->animator, true, 0);
        entity->state = Kanabun_State_Unknown2;
    }
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->drawOrder == Zone->drawOrderHigh)
        Kanabun_CheckPlayerCollisions();
    Kanabun_CheckOnScreen();
}

void Kanabun_State_Unknown2(void)
{
    RSDK_THIS(Kanabun);
    Kanabun_HandleMovement();
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Kanabun->aniFrames, 0, &entity->animator, true, 0);
        entity->velocity.x = -entity->velocity.x;
        entity->groundVel  = 0;
        entity->direction ^= FLIP_X;
        entity->state = Kanabun_State_Unknown1;
    }
    else {
        if (entity->drawOrder == Zone->drawOrderHigh)
            Kanabun_CheckPlayerCollisions();
        Kanabun_CheckOnScreen();
    }
}

#if RETRO_INCLUDE_EDITOR
void Kanabun_EditorDraw(void)
{
    RSDK_THIS(Kanabun);
    RSDK.SetSpriteAnimation(Kanabun->aniFrames, 0, &entity->animator, false, 0);
    entity->scale.x = 0x200;
    entity->scale.y = 0x200;

    Kanabun_Draw();
}

void Kanabun_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        Kanabun->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Kanabun.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("SSZ2"))
        Kanabun->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Kanabun.bin", SCOPE_STAGE);
}
#endif

void Kanabun_Serialize(void)
{
    RSDK_EDITABLE_VAR(Kanabun, VAR_ENUM, hVel);
    RSDK_EDITABLE_VAR(Kanabun, VAR_UINT8, hDist);
    RSDK_EDITABLE_VAR(Kanabun, VAR_UINT8, bobDist);
    RSDK_EDITABLE_VAR(Kanabun, VAR_UINT8, angleVel);
    RSDK_EDITABLE_VAR(Kanabun, VAR_UINT8, direction);
}
