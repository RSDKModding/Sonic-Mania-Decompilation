#include "SonicMania.h"

ObjectScarab *Scarab;

void Scarab_Update(void)
{
    RSDK_THIS(Scarab);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->moveOffset.x = -entity->position.x;
    entity->moveOffset.y = -entity->position.y;

    StateMachine_Run(entity->state);

    entity->moveOffset.x += entity->position.x;
    entity->moveOffset.y += entity->position.y;
    Scarab_CheckPlayerCollisions();
    Scarab_HandleChildMove();
    Scarab_HandlePlayerGrab();
    if (entity->state != Scarab_State_Setup) {
        if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
            int32 x                = -entity->position.x;
            int32 y                = -entity->position.y;
            entity->direction    = entity->startDir;
            entity->position     = entity->startPos;
            entity->moveOffset.x = entity->position.x + x;
            entity->moveOffset.y = entity->position.y + y;
            Scarab_HandleChildMove();
            Scarab_Create(NULL);
        }
    }
}

void Scarab_LateUpdate(void) {}

void Scarab_StaticUpdate(void)
{
    foreach_active(Scarab, scarab) { RSDK.AddDrawListRef(scarab->drawOrderHigh, RSDK.GetEntityID(scarab)); }
}

void Scarab_Draw(void)
{
    RSDK_THIS(Scarab);
    if (RSDK_sceneInfo->currentDrawGroup == entity->drawOrderHigh) {
        RSDK.DrawSprite(&entity->animator3, NULL, false);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void Scarab_Create(void *data)
{
    RSDK_THIS(Scarab);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;

    if (!entity->planeFilter) {
        entity->drawOrderLow  = Zone->drawOrderLow;
        entity->drawOrderHigh = Zone->playerDrawHigh;
    }
    if ((uint8)(entity->planeFilter - 1) & 2) {
        entity->drawOrderLow  = Zone->drawOrderHigh;
        entity->drawOrderHigh = Zone->playerDrawHigh;
    }
    else {
        entity->drawOrderLow  = Zone->drawOrderLow;
        entity->drawOrderHigh = Zone->playerDrawLow;
    }

    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!entity->amplitude)
        entity->amplitude = 100;
    entity->startPos = entity->position;
    entity->startDir = entity->direction;
    entity->state    = Scarab_State_Setup;
}

void Scarab_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        Scarab->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Scarab.bin", SCOPE_STAGE);
    Scarab->hitboxBadnik.left   = -16;
    Scarab->hitboxBadnik.top    = -14;
    Scarab->hitboxBadnik.right  = 14;
    Scarab->hitboxBadnik.bottom = 6;
    Scarab->hitboxGrab.left     = -48;
    Scarab->hitboxGrab.top      = -14;
    Scarab->hitboxGrab.right    = -17;
    Scarab->hitboxGrab.bottom   = 6;
    Scarab->active              = ACTIVE_ALWAYS;
    DEBUGMODE_ADD_OBJ(Scarab);
}

void Scarab_DebugSpawn(void)
{
    RSDK_THIS(Scarab);
    EntityScarab *scarab = CREATE_ENTITY(Scarab, NULL, entity->position.x, entity->position.y);
    scarab->direction    = entity->direction;
    scarab->startDir     = entity->direction;
}

void Scarab_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 4, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Scarab_CheckPlayerCollisions(void)
{
    RSDK_THIS(Scarab);

    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == ((uint8)(entity->planeFilter - 1) & 1)) {
            int32 playerID = RSDK.GetEntityID(player);
            if (Player_CheckBadnikTouch(player, entity, &Scarab->hitboxBadnik)) {
                Scarab_HandlePlayerRelease();
                Player_CheckBadnikBreak(entity, player, true);
            }

            if (!entity->childCount) {
                if (Player_CheckCollisionTouch(player, entity, &Scarab->hitboxGrab)) {
                    if (!((1 << playerID) & entity->grabbedPlayers) && !entity->playerTimers[playerID]) {
                        entity->grabbedPlayers |= (1 << playerID);
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                        player->groundVel       = 0;
                        player->onGround        = false;
                        player->tileCollisions  = false;
                        player->state           = Player_State_None;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        entity->isPermanent     = true;
                    }
                }
            }
        }
    }
}

void Scarab_HandleChildMove(void)
{
    RSDK_THIS(Scarab);

    int32 slot = RSDK_sceneInfo->entitySlot + 1;
    for (int32 c = 0; c < entity->childCount; ++c) {
        EntityItemBox *child = RSDK_GET_ENTITY(slot + c, ItemBox);
        child->position.x += entity->moveOffset.x;
        child->position.y += entity->moveOffset.y;
        if (child->objectID == ItemBox->objectID)
            child->moveOffset = entity->moveOffset;
    }
}

void Scarab_HandlePlayerGrab(void)
{
    RSDK_THIS(Scarab);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if ((1 << playerID) & entity->grabbedPlayers) {
            if (entity->playerTimers[playerID] < 60 && player->interaction && player->state == Player_State_None) {
                player->position.x = entity->position.x + ((2 * (entity->direction != FLIP_NONE) - 1) << 21);
                player->position.y = entity->position.y - 0xA0000;
                if (entity->state == Scarab_State_Move)
                    player->velocity.x = 0x6000 * entity->field_DB;
                else
                    player->velocity.x = 0;
                player->velocity.y = 0;
                ++entity->playerTimers[playerID];
            }
            else {
                entity->grabbedPlayers &= ~(1 << playerID);
                entity->playerTimers[playerID] = -10;
                if (player->state != Player_State_FlyIn && player->state != Player_State_JumpIn) {
                    player->tileCollisions = true;
                    player->state          = Player_State_Air;
                    player->velocity.x     = 0xA0000 * (2 * (entity->direction != FLIP_NONE) - 1);
                    player->velocity.y     = 0;
                }
                entity->isPermanent = false;
            }
        }
        else {
            if (entity->playerTimers[playerID] < 0)
                entity->playerTimers[playerID]++;
        }
    }
}

void Scarab_HandlePlayerRelease(void)
{
    RSDK_THIS(Scarab);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if ((1 << playerID) & entity->grabbedPlayers) {
            entity->grabbedPlayers &= ~(1 << playerID);
            entity->playerTimers[playerID] = -10;
            if (player->state != Player_State_FlyIn && player->state != Player_State_JumpIn) {
                player->tileCollisions = true;
                player->state          = Player_State_Air;
                player->velocity.x     = 0xA0000 * (2 * (entity->direction != FLIP_NONE) - 1);
                player->velocity.y     = 0;
            }
            entity->isPermanent = false;
        }
    }
}

void Scarab_State_Setup(void)
{
    RSDK_THIS(Scarab);

    entity->active   = ACTIVE_NORMAL;
    entity->state    = Scarab_State_Move;
    entity->timer2   = 0;
    entity->timer    = 0;
    entity->field_DB = 2 * (entity->direction != FLIP_NONE) - 1;
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 1, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 5, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 3, &entity->animator3, true, 0);
    Scarab_State_Move();
}

void Scarab_State_Move(void)
{
    RSDK_THIS(Scarab);
    if (entity->animator3.frameID == entity->animator3.frameCount - 1)
        ++entity->timer2;
    if (entity->timer2 == 9) {
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 4, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 2, &entity->animator3, true, 0);
        entity->timer = 45;
        entity->state = Scarab_State_Wait;
    }

    int32 mult      = entity->field_DB;
    int32 amplitude = entity->amplitude << 16;
    int32 x         = entity->position.x + 0x6000 * mult;
    int32 y         = entity->position.y;

    entity->position.x = x;
    if (abs(x - entity->startPos.x) < amplitude && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x60000, 2)) {
        entity->position.x = x;
        //entity->position.y = y;
    }
    else {
        entity->position.y = y;
        entity->position.x = entity->startPos.x + amplitude * mult;
        entity->field_DB   = -entity->field_DB;
    }
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x60000, 4);
}

void Scarab_State_Wait(void)
{
    RSDK_THIS(Scarab);

    if (entity->timer <= 0) {
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 1, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 5, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Scarab->aniFrames, 3, &entity->animator3, true, 0);
        entity->timer2 = 0;
        entity->state  = Scarab_State_Move;
    }
    else {
        entity->timer--;
    }
}

void Scarab_EditorDraw(void)
{
    RSDK_THIS(Scarab);
    RSDK.SetSpriteAnimation(Scarab->aniFrames, 4, &entity->animator1, true, 0);
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 0, &entity->animator2, true, 0);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    RSDK.SetSpriteAnimation(Scarab->aniFrames, 2, &entity->animator3, true, 0);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void Scarab_EditorLoad(void)
{
    Scarab->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Scarab.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Scarab, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
}

void Scarab_Serialize(void)
{
    RSDK_EDITABLE_VAR(Scarab, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Scarab, VAR_INT32, amplitude);
    RSDK_EDITABLE_VAR(Scarab, VAR_UINT8, childCount);
    RSDK_EDITABLE_VAR(Scarab, VAR_ENUM, planeFilter);
}
