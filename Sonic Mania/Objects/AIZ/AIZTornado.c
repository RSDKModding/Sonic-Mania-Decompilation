#include "SonicMania.h"

ObjectAIZTornado *AIZTornado;

void AIZTornado_Update(void)
{
    RSDK_THIS(AIZTornado);
    entity->prevPos.x = entity->position.x;
    entity->prevPos.y = entity->position.y;
    StateMachine_Run(entity->state);
    entity->animatorTornado.frameID = entity->turnAngle >> 4;
    RSDK.ProcessAnimation(&entity->animatorPropeller);
    RSDK.ProcessAnimation(&entity->animatorPilot);
    RSDK.ProcessAnimation(&entity->animatorFlame);
}

void AIZTornado_LateUpdate(void) {}

void AIZTornado_StaticUpdate(void) {}

void AIZTornado_Draw(void)
{
    RSDK_THIS(AIZTornado);
    RSDK.DrawSprite(&entity->animatorPilot, NULL, false);
    RSDK.DrawSprite(&entity->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&entity->animatorTornado, NULL, false);
    if (entity->showFlame) {
        Vector2 drawPos;
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y += AIZTornado->flameOffsets[entity->animatorTornado.frameID];
        RSDK.DrawSprite(&entity->animatorFlame, &drawPos, false);
    }
}

void AIZTornado_Create(void *data)
{
    RSDK_THIS(AIZTornado);
    if (!SceneInfo->inEditor) {
        entity->visible       = true;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->drawFX        = FX_FLIP;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->movePos.x     = entity->position.x;
        entity->movePos.y     = entity->position.y;
        entity->turnAngle     = 48;
        entity->isStood       = true;
        entity->offsetX       = 0x80000;
        entity->showFlame     = true;
        if (RSDK.CheckStageFolder("AIZ") || RSDK.CheckStageFolder("Credits"))
            entity->active = ACTIVE_NORMAL;
        if (!StarPost->postIDs[0]) {
            entity->active = ACTIVE_NORMAL;
            entity->state  = AIZTornado_State_Move;
        }
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 0, &entity->animatorTornado, true, 0);
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 1, &entity->animatorPropeller, true, 0);
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 2, &entity->animatorFlame, true, 0);
        if (globals->playerID & 1)
            RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 6, &entity->animatorPilot, true, 0);
        else
            RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 3, &entity->animatorPilot, true, 0);
    }
}

void AIZTornado_StageLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ") || RSDK.CheckStageFolder("Credits"))
        AIZTornado->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZTornado.bin", SCOPE_STAGE);
}

void AIZTornado_HandleMovement(void)
{
    RSDK_THIS(AIZTornado);
    entity->movePos.x += AIZTornadoPath->moveVel.x;
    entity->movePos.y += AIZTornadoPath->moveVel.y;
    entity->newPos.x = entity->movePos.x;
    entity->newPos.y = entity->movePos.y;

    if (!entity->disableInteractions) {
        int32 x = entity->movePos.x;
        int32 y = entity->movePos.y;
        if (AIZTornadoPath->moveVel.y) {
            entity->turnAngle = 0;
        }
        else {
            y               = 0xA00 * RSDK.Sin256(2 * Zone->timer) + entity->movePos.y;
            entity->turnAngle = (RSDK.Sin256(2 * Zone->timer) >> 3) + 32;
        }
        entity->newPos.x += (x - entity->movePos.x) >> 1;
        entity->newPos.y += (y - entity->movePos.y) >> 1;
    }
    entity->position.x = entity->newPos.x;
    entity->position.y = entity->newPos.y;
}

void AIZTornado_HandlePlayerCollisions(void)
{
    RSDK_THIS(AIZTornado);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Hitbox *hitbox       = RSDK.GetHitbox(&entity->animatorTornado, 0);
    if (entity->turnAngle >= 32)
        player->drawOrder = entity->drawOrder + 1;
    else
        player->drawOrder = entity->drawOrder;
    int32 x = entity->position.x;
    int32 y = entity->position.y;
    entity->prevPos.x &= 0xFFFF0000;
    entity->prevPos.y &= 0xFFFF0000;
    entity->isStood        = false;
    entity->moveVelocity.x = (x & 0xFFFF0000) - entity->prevPos.x;
    entity->moveVelocity.y = (y & 0xFFFF0000) - entity->prevPos.y;
    entity->position.x     = entity->prevPos.x;
    entity->position.y     = entity->prevPos.y;

    if (Player_CheckCollisionPlatform(player, entity, hitbox)) {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RIDE, &player->playerAnimator, false, 0);
        player->state = Player_State_None;
        player->position.x += entity->moveVelocity.x;
        player->position.y += entity->moveVelocity.y;
        player->flailing = false;
        entity->isStood  = true;
        if (player->velocity.y > 0x10000) {
            entity->collideTimer = 0;
            entity->gravityForce = 0x20000;
        }
    }
    entity->position.x = x;
    entity->position.y = y;
}

void AIZTornado_State_Move(void)
{
    RSDK_THIS(AIZTornado);
    entity->prevPos.x = entity->position.x;
    entity->prevPos.y = entity->position.y;
    AIZTornado_HandleMovement();
    if (!entity->disableInteractions)
        AIZTornado_HandlePlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void AIZTornado_EditorDraw(void)
{
    RSDK_THIS(AIZTornado);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 0, &entity->animatorTornado, true, 0);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 1, &entity->animatorPropeller, true, 0);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 2, &entity->animatorFlame, true, 0);
    RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 3, &entity->animatorPilot, true, 0);

    RSDK.DrawSprite(&entity->animatorPilot, NULL, false);
    RSDK.DrawSprite(&entity->animatorPropeller, NULL, false);
    RSDK.DrawSprite(&entity->animatorTornado, NULL, false);
}

void AIZTornado_EditorLoad(void) { AIZTornado->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZTornado.bin", SCOPE_STAGE); }
#endif

void AIZTornado_Serialize(void) {}
