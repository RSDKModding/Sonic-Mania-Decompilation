#include "SonicMania.h"

ObjectOrbinaut *Orbinaut = NULL;

void Orbinaut_Update(void)
{
    RSDK_THIS(Orbinaut);
    StateMachine_Run(entity->state);
}

void Orbinaut_LateUpdate(void) {}

void Orbinaut_StaticUpdate(void) {}

void Orbinaut_Draw(void)
{
    RSDK_THIS(Orbinaut);
    for (int32 o = 0; o < Orbinaut_MaxOrbs; ++o) {
        if ((1 << o) & entity->activeOrbs) {
            RSDK.DrawSprite(&entity->animatorOrb, &entity->orbPositions[o], false);
        }
    }

    RSDK.DrawSprite(&entity->animatorFace, NULL, false);
}

void Orbinaut_Create(void *data)
{
    RSDK_THIS(Orbinaut);

    entity->visible = true;
    if (entity->planeFilter > 0 && ((entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->activeOrbs    = 1 | 2 | 4 | 8;
    if (data) {
        RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 1, &entity->animatorFace, true, 0);
        entity->state = Orbinaut_State_Orb;
    }
    else {
        RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 0, &entity->animatorFace, true, 0);
        RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 1, &entity->animatorOrb, true, 0);
        entity->state = Orbinaut_State_Setup;
        if (entity->fireOrbs) {
            entity->velocity.x = 0;
        }
        else {
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = -0x4000;
            else
                entity->velocity.x = 0x4000;
        }
    }
}

void Orbinaut_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        Orbinaut->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Orbinaut.bin", SCOPE_STAGE);
    Orbinaut->hitbox1.left   = -8;
    Orbinaut->hitbox1.top    = -8;
    Orbinaut->hitbox1.right  = 8;
    Orbinaut->hitbox1.bottom = 8;
    Orbinaut->hitbox2.left   = -4;
    Orbinaut->hitbox2.top    = -4;
    Orbinaut->hitbox2.right  = 4;
    Orbinaut->hitbox2.bottom = 4;
    DEBUGMODE_ADD_OBJ(Orbinaut);
}

void Orbinaut_DebugSpawn(void)
{
    RSDK_THIS(Orbinaut);
    CREATE_ENTITY(Orbinaut, NULL, entity->position.x, entity->position.y);
}

void Orbinaut_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Orbinaut->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Orbinaut_HandlePlayerInteractions(void)
{
    RSDK_THIS(Orbinaut);

    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;
    for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
        if ((1 << i) & entity->activeOrbs) {
            entity->position.x = entity->orbPositions[i].x;
            entity->position.y = entity->orbPositions[i].y;
            foreach_active(Player, player)
            {
                if (entity->planeFilter <= 0 || player->collisionPlane == (uint8)((entity->planeFilter - 1) & 1)) {
                    if (Player_CheckCollisionTouch(player, entity, &Orbinaut->hitbox2)) {
                        Player_CheckHit(player, entity);
                    }
                }
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Orbinaut->hitbox1) && Player_CheckBadnikBreak(entity, player, false)) {
            int32 angle = entity->angle;
            for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
                if ((1 << i) & entity->activeOrbs) {
                    entity->position.x = entity->orbPositions[i].x;
                    entity->position.y = entity->orbPositions[i].y;
                    EntityOrbinaut *orb = CREATE_ENTITY(Orbinaut, intToVoid(1), entity->orbPositions[i].x, entity->orbPositions[i].y);

                    orb->state = Orbinaut_Unknown10;
                    orb->velocity.x = 0x380 * RSDK.Cos256(angle);
                    orb->velocity.y = 0x380 * RSDK.Sin256(angle);
                }
                angle += (0x100 / Orbinaut_MaxOrbs);
            }
            destroyEntity(entity);
        }
    }
}

void Orbinaut_HandleRotation(void)
{
    RSDK_THIS(Orbinaut);

    int32 angle = entity->angle;
    if (entity->direction)
        entity->angle = (angle - 1) & 0xFF;
    else
        entity->angle = (angle + 1) & 0xFF;

    for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
        if ((1 << i) & entity->activeOrbs) {
            entity->orbPositions[i].x = (RSDK.Cos256(angle) << 12) + entity->position.x;
            entity->orbPositions[i].y = (RSDK.Sin256(angle) << 12) + entity->position.y;
        }
        angle += (0x100 / Orbinaut_MaxOrbs);
    }
}

void Orbinaut_CheckOnScreen(void)
{
    RSDK_THIS(Orbinaut);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Orbinaut_Create(NULL);
    }
}

void Orbinaut_State_Setup(void)
{
    RSDK_THIS(Orbinaut);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Orbinaut_Unknown6;
    Orbinaut_Unknown6();
}

void Orbinaut_Unknown6(void)
{
    RSDK_THIS(Orbinaut);

    entity->position.x += entity->velocity.x;
    Orbinaut_HandleRotation();
    Orbinaut_HandlePlayerInteractions();

    if (entity->fireOrbs) {
        EntityPlayer *playerPtr = NULL;
        int32 distanceX           = 0x7FFFFFFF;
        int32 distanceY           = 0x7FFFFFFF;
        foreach_active(Player, player)
        {
            if (abs(player->position.y - entity->position.y) < distanceY)
                distanceY = abs(player->position.y - entity->position.y);

            if (abs(player->position.y - entity->position.y) < 0x400000) {
                if (!playerPtr) {
                    if (abs(player->position.x - entity->position.x) < distanceX) {
                        distanceX = abs(player->position.x - entity->position.x);
                        playerPtr = player;
                    }
                }
                else {
                    if (abs(player->position.x - entity->position.x) < distanceX) {
                        distanceX = abs(player->position.x - entity->position.x);
                        playerPtr = player;
                    }
                }
            }
        }

        if (!playerPtr)
            playerPtr = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (distanceX <= 0x800000) {
            entity->state             = Orbinaut_Unknown7;
            entity->animatorFace.frameID = 1;
        }

        entity->direction = playerPtr->position.x >= entity->position.x;
    }

    Orbinaut_CheckOnScreen();
}

void Orbinaut_Unknown7(void)
{
    RSDK_THIS(Orbinaut);

    int32 angle = entity->angle;
    Orbinaut_HandleRotation();

    for (int32 i = 0; i < Orbinaut_MaxOrbs; ++i) {
        if (angle == 64) {
            if ((1 << i) & entity->activeOrbs) {
                entity->activeOrbs &= ~(1 << i);
                EntityOrbinaut *sol = CREATE_ENTITY(Orbinaut, intToVoid(true), entity->orbPositions[i].x, entity->orbPositions[i].y);
                if (entity->direction == FLIP_NONE)
                    sol->velocity.x = -0x20000;
                else
                    sol->velocity.x = 0x20000;
            }
            angle += (0x100 / Orbinaut_MaxOrbs);
        }
    }
    Orbinaut_HandlePlayerInteractions();

    if (!entity->activeOrbs) {
        entity->state = Orbinaut_Unknown8;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
    }

    if (entity->animatorFace.animationTimer >= 0x10)
        entity->animatorFace.frameID = 2;
    else
        entity->animatorFace.animationTimer++;

    Orbinaut_CheckOnScreen();
}

void Orbinaut_Unknown8(void)
{
    RSDK_THIS(Orbinaut);
    entity->position.x += entity->velocity.x;
    Orbinaut_HandlePlayerInteractions();
    Orbinaut_CheckOnScreen();
}

void Orbinaut_State_Orb(void)
{
    RSDK_THIS(Orbinaut);
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->position.x += entity->velocity.x;

        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == (uint8)((entity->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, entity, &Sol->hitbox2)) {
                    if (player->shield != SHIELD_FIRE)
                        Player_CheckHit(player, entity);
                }
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Orbinaut_Unknown10(void)
{
    RSDK_THIS(Orbinaut);
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
    }
    else {
        destroyEntity(entity);
    }
}


void Orbinaut_EditorDraw(void) {}

void Orbinaut_EditorLoad(void) {}

void Orbinaut_Serialize(void)
{
    RSDK_EDITABLE_VAR(Orbinaut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Orbinaut, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(Orbinaut, VAR_BOOL, fireOrbs);
}
