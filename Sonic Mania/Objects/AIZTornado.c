#include "../SonicMania.h"

ObjectAIZTornado *AIZTornado;

void AIZTornado_Update(void)
{
    RSDK_THIS(AIZTornado);
    entity->prevPos.x = RSDK_sceneInfo->entity->position.x;
    entity->prevPos.y = entity->position.y;
    StateMachine_Run(entity->state);
    entity->animator1.frameID = entity->dword60 >> 4;
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
}

void AIZTornado_LateUpdate(void) {}

void AIZTornado_StaticUpdate(void) {}

void AIZTornado_Draw(void)
{
    RSDK_THIS(AIZTornado);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    if (entity->dword68) {
        Vector2 drawPos;
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y += AIZTornado->array[entity->animator1.frameID];
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
    }
}

void AIZTornado_Create(void *data)
{
    RSDK_THIS(AIZTornado);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->drawFX        = FX_FLIP;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->dword7C       = entity->position.x;
        entity->dword80       = entity->position.y;
        entity->dword60       = 48;
        entity->dword64       = 1;
        entity->dword98       = 0x80000;
        entity->dword68       = 1;
        if (RSDK.CheckStageFolder("AIZ") || RSDK.CheckStageFolder("Credits"))
            entity->active = ACTIVE_NORMAL;
        if (!StarPost->postIDs[0]) {
            entity->active = ACTIVE_NORMAL;
            entity->state  = AIZTornado_Unknown3;
        }
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 2, &entity->animator4, true, 0);
        if (globals->playerID & 1)
            RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 6, &entity->animator3, true, 0);
        else
            RSDK.SetSpriteAnimation(AIZTornado->aniFrames, 3, &entity->animator3, true, 0);
    }
}

void AIZTornado_StageLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ") || RSDK.CheckStageFolder("Credits"))
        AIZTornado->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZTornado.bin", SCOPE_STAGE);
}

void AIZTornado_Unknown1(void)
{
    RSDK_THIS(AIZTornado);
    entity->dword7C += AIZTornadoPath->field_C.x;
    entity->dword80 += AIZTornadoPath->field_C.y;
    entity->field_84 = entity->dword7C;
    entity->field_88 = entity->dword80;

    if (!entity->field_94) {
        int x = entity->dword7C;
        int y = entity->dword80;
        if (AIZTornadoPath->field_C.y) {
            entity->dword60 = 0;
        }
        else {
            y               = 0xA00 * RSDK.Sin256(2 * Zone->timer) + entity->dword80;
            entity->dword60 = (RSDK.Sin256(2 * Zone->timer) >> 3) + 32;
        }
        entity->field_84 += (x - entity->dword7C) >> 1;
        entity->field_88 += (y - entity->dword80) >> 1;
    }
    entity->position.x = entity->field_84;
    entity->position.y = entity->field_88;
}

void AIZTornado_Unknown2(void)
{
    RSDK_THIS(AIZTornado);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    Hitbox *hitbox       = RSDK.GetHitbox(&entity->animator1, 0);
    if (entity->dword60 >= 32)
        player->drawOrder = entity->drawOrder + 1;
    else
        player->drawOrder = entity->drawOrder;
    int x = entity->position.x;
    int y = entity->position.y;
    entity->prevPos.x &= 0xFFFF0000;
    entity->prevPos.y &= 0xFFFF0000;
    entity->dword64        = 0;
    entity->moveVelocity.x = (x & 0xFFFF0000) - entity->prevPos.x;
    entity->moveVelocity.y = (y & 0xFFFF0000) - entity->prevPos.y;
    entity->position.x     = entity->prevPos.x;
    entity->position.y     = entity->prevPos.y;

    if (Player_CheckCollisionPlatform(player, entity, hitbox)) {
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_RIDE, &player->playerAnimator, false, 0);
        player->state = Player_State_None;
        player->position.x += entity->moveVelocity.x;
        player->position.y += entity->moveVelocity.y;
        player->flailing = false;
        entity->dword64  = 1;
        if (player->velocity.y > 0x10000) {
            entity->dword5C  = 0;
            entity->field_70 = 0x20000;
        }
    }
    entity->position.x = x;
    entity->position.y = y;
}

void AIZTornado_Unknown3(void)
{
    RSDK_THIS(AIZTornado);
    entity->prevPos.x = entity->position.x;
    entity->prevPos.y = entity->position.y;
    AIZTornado_Unknown1();
    if (!entity->field_94)
        AIZTornado_Unknown2();
}

void AIZTornado_EditorDraw(void) {}

void AIZTornado_EditorLoad(void) {}

void AIZTornado_Serialize(void) {}
