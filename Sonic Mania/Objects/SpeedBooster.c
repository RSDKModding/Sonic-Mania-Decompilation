#include "../SonicMania.h"

ObjectSpeedBooster *SpeedBooster;

void SpeedBooster_Update(void)
{
    RSDK_THIS(SpeedBooster);
    StateMachine_Run(entity->state);
    for (int i = 0; i < 4; i++) {
        if (entity->playerCooldown[i])
            entity->playerCooldown[i]--;
    }
}

void SpeedBooster_LateUpdate(void) {}

void SpeedBooster_StaticUpdate(void) {}

void SpeedBooster_Draw(void)
{
    RSDK_THIS(SpeedBooster);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void SpeedBooster_Create(void *data)
{
    RSDK_THIS(SpeedBooster);
    entity->visible = true;
    if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2"))
        entity->drawOrder = Zone->playerDrawLow;
    else
        entity->drawOrder = Zone->drawOrderHigh;
    entity->drawPos.x     = entity->position.x;
    entity->drawPos.y     = entity->position.y;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    if (data) {
        entity->active = ACTIVE_NORMAL;
        entity->drawFX = INK_ALPHA;
        RSDK.SetSpriteAnimation(SpeedBooster->animID, 1, &entity->animator, true, 0);
        entity->state = SpeedBooster_MovingState;
    }
    else {
        entity->active = ACTIVE_BOUNDS;
        if (entity->speed == 0)
            entity->speed = RSDK.CheckStageFolder("CPZ") ? 10 : 16;
        entity->groundVel = entity->speed << 16;
        RSDK.SetSpriteAnimation(SpeedBooster->animID, 0, &entity->animator, true, 0);
        entity->state = SpeedBooster_BasicState;
    }
}

void SpeedBooster_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ")) {
        SpeedBooster->animID        = RSDK.LoadSpriteAnimation("CPZ/SpeedBooster.bin", SCOPE_STAGE);
        SpeedBooster->hitbox.left   = -16;
        SpeedBooster->hitbox.top    = -16;
        SpeedBooster->hitbox.right  = 16;
        SpeedBooster->hitbox.bottom = 16;
        SpeedBooster->sfxID         = RSDK.GetSFX("Global/Spring.wav");
        SpeedBooster->defaultState  = SpeedBooster_BasicState;
    }
    else if (RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2")) {
        SpeedBooster->animID =
            RSDK.LoadSpriteAnimation((RSDK.CheckStageFolder("SSZ1") ? "SSZ1/SpeedBooster.bin" : "SSZ2/SpeedBooster.bin"), SCOPE_STAGE);
        SpeedBooster->hitbox.left   = -32;
        SpeedBooster->hitbox.top    = -16;
        SpeedBooster->hitbox.right  = 32;
        SpeedBooster->hitbox.bottom = 0;
        SpeedBooster->sfxID         = RSDK.GetSFX("Stage/SpeedBooster.wav");
        SpeedBooster->defaultState  = SpeedBooster_SSZState;
    }
    int count = DebugMode->itemCount;
    if (count < 256) {
        DebugMode->itemCount++;
        DebugMode->objectIDs[count] = SpeedBooster->objectID;
        DebugMode->spawn[count]     = SpeedBooster_DebugSpawn;
        DebugMode->draw[count]      = SpeedBooster_DebugDraw;
    }
}

void SpeedBooster_EditorDraw(void) {}

void SpeedBooster_EditorLoad(void) {}

void SpeedBooster_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpeedBooster, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SpeedBooster, VAR_UINT8, speed);
}

void SpeedBooster_DebugSpawn(void) { DEBUGMODE_ADD_OBJ(SpeedBooster); }

void SpeedBooster_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SpeedBooster->animID, 0, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, false);
}

void SpeedBooster_MovingState(void)
{
    RSDK_THIS(SpeedBooster);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->rotation += 6;
    entity->velocity.y += 0x3800;
    entity->drawPos.x = entity->position.x;
    entity->drawPos.y = entity->position.y;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void SpeedBooster_BasicState(void)
{
    RSDK_THIS(SpeedBooster);
    RSDK.ProcessAnimation(&entity->animator);
    SpeedBooster_Interact();
}

void SpeedBooster_Interact(void)
{
    RSDK_THIS(SpeedBooster);
    bool32 isSSZ = RSDK.CheckStageFolder("SSZ1") || RSDK.CheckStageFolder("SSZ2");
    foreach_active(Player, player)
    {
        int playerID   = RSDK.GetEntityID(player);
        Hitbox *hitbox = &SpeedBooster->hitbox;
        if (entity->playerCooldown[playerID] || player->playerAnimator.animationID == 18 || !Player_CheckCollisionTouch(player, entity, hitbox) || !player->onGround) {
            entity->playerPos[playerID] = player->position.x;
            continue;
        }
        entity->velocity.x       = 2 * (player->position.x < entity->position.x) - 1;
        entity->state            = SpeedBooster->defaultState;
        entity->animator.frameID = 0;
        RSDK.PlaySFX(SpeedBooster->sfxID, 0, 255);
        entity->active = 2;
        bool32 check   = isSSZ ? (entity->playerPos[playerID] <= entity->position.x) : (!entity->direction);
        if (check) {
            if (player->groundVel < entity->groundVel)
                player->groundVel = entity->groundVel;
            player->direction = 0;
        }
        else {
            if (player->groundVel > -entity->groundVel)
                player->groundVel = -entity->groundVel;
            player->direction = 1;
        }
        entity->playerCooldown[playerID] = 30;
        player->controlLock              = 16;
        player->pushing                  = 0;
        player->tileCollisions           = true;
        if (player->state != Player_State_Roll)
            player->state = Player_State_Ground;
        entity->playerPos[playerID] = player->position.x;
    }
}

void SpeedBooster_SSZState(void)
{
    RSDK_THIS(SpeedBooster);
    entity->velocity.x = 0x55550 * RSDK_sceneInfo->entity->velocity.x;
    entity->drawPos.x  = entity->position.x;
    entity->drawPos.y  = entity->position.y;
    SPAWN_CHILD(SpeedBooster, 1);
    int newVel        = 0x10000;
    child->velocity.y = -0x70000;
    if (entity->velocity.x > 0)
        newVel = -0x10000;
    child->velocity.x = newVel;
    entity->active    = ACTIVE_NORMAL;
    entity->cooldown  = 6;
    entity->drawPos.x -= entity->velocity.x;
    if (entity->cooldown-- == 1) {
        entity->cooldown = 6;
        entity->state    = SpeedBooster_Wait2;
    }
    SpeedBooster_Interact();
    entity->state = SpeedBooster_Wait1;
    SpeedBooster_Interact();
}

void SpeedBooster_Wait1(void) {
    RSDK_THIS(SpeedBooster);
    entity->drawPos.x -= entity->velocity.x;
    if (entity->cooldown-- == 1) {
        entity->cooldown = 6;
        entity->state    = SpeedBooster_Wait2;
    }
    SpeedBooster_Interact();
}

void SpeedBooster_Wait2(void) {
    RSDK_THIS(SpeedBooster);
    entity->drawPos.x += entity->velocity.x;
    if (entity->cooldown-- == 1) {
        entity->active = ACTIVE_BOUNDS;
        entity->state    = SpeedBooster_BasicState;
    }
    SpeedBooster_Interact();
}