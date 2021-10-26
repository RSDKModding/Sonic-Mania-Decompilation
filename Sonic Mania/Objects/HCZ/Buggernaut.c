#include "SonicMania.h"

ObjectBuggernaut *Buggernaut;

void Buggernaut_Update(void)
{
    RSDK_THIS(Buggernaut);
    StateMachine_Run(entity->state);
}

void Buggernaut_LateUpdate(void) {}

void Buggernaut_StaticUpdate(void) {}

void Buggernaut_Draw(void)
{
    RSDK_THIS(Buggernaut);
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_ALPHA;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->inkEffect = INK_NONE;
}

void Buggernaut_Create(void *data)
{
    RSDK_THIS(Buggernaut);

    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow + 1;
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawFX |= FX_FLIP;
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->direction     = FLIP_NONE;
        entity->alpha         = 128;
        entity->timer         = 16;
        entity->timer2        = -1;
        RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 2, &entity->animator2, true, 0);
        entity->state = Buggernaut_State_Setup;
    }
}

void Buggernaut_StageLoad(void)
{
    Buggernaut->aniFrames           = RSDK.LoadSpriteAnimation("HCZ/Buggernaut.bin", SCOPE_STAGE);
    Buggernaut->hitboxBadnik.left   = -8;
    Buggernaut->hitboxBadnik.top    = -8;
    Buggernaut->hitboxBadnik.right  = 8;
    Buggernaut->hitboxBadnik.bottom = 8;
    Buggernaut->hitbox2.left        = -64;
    Buggernaut->hitbox2.top         = -64;
    Buggernaut->hitbox2.right       = 64;
    Buggernaut->hitbox2.bottom      = 64;

    DEBUGMODE_ADD_OBJ(Buggernaut);
}

void Buggernaut_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Buggernaut, NULL, entity->position.x, entity->position.y);
}

void Buggernaut_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Buggernaut_CheckPlayerCollisions(void)
{
    RSDK_THIS(Buggernaut);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Buggernaut->hitboxBadnik))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Buggernaut_CheckOnScreen(void)
{
    RSDK_THIS(Buggernaut);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        if (entity->animator1.animationID) {
            destroyEntity(entity);
        }
        else {
            entity->position = entity->startPos;
            Buggernaut_Create(NULL);
        }
    }
}

bool32 Buggernaut_HandleTileCollisionsX(void)
{
    RSDK_THIS(Buggernaut);

    bool32 collided = false;
    if (entity->velocity.x <= 0) {
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x90000, -0x80000, false);
        collided |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x90000, 0x80000, false);
    }
    else {
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x90000, -0x80000, false);
        collided |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x90000, 0x80000, false);
    }

    if (collided)
        entity->velocity.x = -entity->velocity.x;
    return collided;
}

bool32 Buggernaut_HandleTileCollisionsY(void)
{
    RSDK_THIS(Buggernaut);

    bool32 collided = false;
    if (entity->velocity.y <= 0) {
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x80000, -0x90000, false);
        collided |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x80000, 0x90000, false);
    }
    else {
        collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x80000, 0x90000, false);
        collided |= RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x80000, 0x90000, false);
    }

    if (collided)
        entity->velocity.y = -entity->velocity.y;
    return collided;
}

void Buggernaut_State_Setup(void)
{
    RSDK_THIS(Buggernaut);

    entity->active     = ACTIVE_NORMAL;
    int y              = entity->position.y;
    int x              = entity->position.x;
    entity->velocity.x = 0;
    entity->velocity.y = 0;

    EntityBuggernaut *child = CREATE_ENTITY(Buggernaut, intToVoid(true), x, y);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 1, &child->animator1, true, 0);
    RSDK.SetSpriteAnimation(Buggernaut->aniFrames, 3, &child->animator2, true, 0);
    child->active      = ACTIVE_NORMAL;
    child->drawOrder   = entity->drawOrder - 1;
    child->parent      = (Entity *)entity;
    child->passThrough = entity->passThrough;
    child->isPermanent = true;
    child->state       = Buggernaut_State2_Unknown;
    entity->state      = Buggernaut_State_Unknown1;
    Buggernaut_State_Unknown1();
}

void Buggernaut_State_Unknown1(void)
{
    RSDK_THIS(Buggernaut);
    entity->timer--;
    if (--entity->timer <= 0) {
        if (--entity->timer2) {
            entity->velocity.y = -0x10000;
            entity->timer      = 96;
            entity->state      = Buggernaut_State_Unknown2;
        }
        else {
            entity->velocity.x = RSDK.Rand(-2, 3) << 16;
            entity->velocity.y = RSDK.Rand(-2, 3) << 16;

            if (entity->velocity.y == 0 || entity->velocity.x == 0)
                entity->velocity.x = -0x20000;
            entity->velocity.y = -0x20000;
            entity->state      = Buggernaut_State_Unknown3;
        }
    }
    RSDK.ProcessAnimation(&entity->animator2);
    Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOnScreen();
}

void Buggernaut_State_Unknown2(void)
{
    RSDK_THIS(Buggernaut);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    EntityPlayer *player = Player_GetNearestPlayer();
    if (player) {
        if (player->position.x >= entity->position.x) {
            entity->velocity.x += 0x1000;
            if (entity->velocity.x > 0x20000)
                entity->velocity.x = 0x20000;
            entity->direction = FLIP_X;
        }
        else {
            entity->velocity.x -= 0x1000;
            if (entity->velocity.x < -0x20000)
                entity->velocity.x = -0x20000;
            entity->direction = FLIP_NONE;
        }
        if (player->position.y >= entity->position.y) {
            entity->velocity.y += 0x1000;
            if (entity->velocity.y > 0x20000)
                entity->velocity.y = 0x20000;
        }
        else {
            entity->velocity.y -= 0x1000;
            if (entity->velocity.y < -0x20000)
                entity->velocity.y = -0x20000;
        }
    }

    if (entity->velocity.y > 0 && entity->position.y > Water->waterLevel - 0x80000)
        entity->velocity.y = -entity->velocity.y;
    if (!entity->passThrough) {
        if (abs(entity->velocity.x) <= abs(entity->velocity.y)) {
            if (!Buggernaut_HandleTileCollisionsY())
                Buggernaut_HandleTileCollisionsX();
        }
        else if (!Buggernaut_HandleTileCollisionsX()) {
            Buggernaut_HandleTileCollisionsY();
        }
    }

    if (!--entity->timer) {
        entity->timer      = RSDK.Rand(0, 32);
        entity->state      = Buggernaut_State_Unknown1;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->direction  = FLIP_NONE;
    }
    RSDK.ProcessAnimation(&entity->animator2);
    Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOnScreen();
}

void Buggernaut_State_Unknown3(void)
{
    RSDK_THIS(Buggernaut);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->animator2);
    if (!entity->animator1.animationID)
        Buggernaut_CheckPlayerCollisions();
    Buggernaut_CheckOnScreen();
}

void Buggernaut_State2_Unknown(void)
{
    RSDK_THIS(Buggernaut);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    EntityBuggernaut *buggernaut = (EntityBuggernaut *)entity->parent;
    if (!buggernaut || buggernaut->objectID != Buggernaut->objectID) {
        entity->parent = NULL;
        foreach_active(Buggernaut, buggernaut)
        {
            if (buggernaut->animator1.animationID == 0
                && RSDK.CheckObjectCollisionTouchBox(buggernaut, &Buggernaut->hitboxBadnik, entity, &Buggernaut->hitbox2)) {
                entity->parent = (Entity *)buggernaut;
                foreach_break;
            }
        }

        if (!entity->parent) {
            entity->velocity.x = RSDK.Rand(-2, 3) << 16;
            entity->velocity.y = RSDK.Rand(-2, 3) << 16;

            if (!entity->velocity.y || !entity->velocity.x)
                entity->velocity.x = -0x20000;
            entity->velocity.y = -0x20000;
            entity->state      = Buggernaut_State_Unknown3;
        }
    }

    if (entity->parent) {
        if (buggernaut->position.x >= entity->position.x) {
            entity->velocity.x += 0x2000;
            if (entity->velocity.x > 0x20000)
                entity->velocity.x = 0x20000;
            entity->direction = FLIP_X;
        }
        else {
            entity->velocity.x -= 0x2000;
            if (entity->velocity.x < -0x20000)
                entity->velocity.x = -0x20000;
            entity->direction = FLIP_NONE;
        }

        if (buggernaut->position.y >= entity->position.y) {
            entity->velocity.y += 0x2000;
            if (entity->velocity.y > 0x20000)
                entity->velocity.y = 0x20000;
        }
        else {
            entity->velocity.y -= 0x2000;
            if (entity->velocity.y < -0x20000)
                entity->velocity.y = -0x20000;
        }

        if (entity->velocity.y > 0 && entity->position.y > Water->waterLevel - 0x80000)
            entity->velocity.y = -entity->velocity.y;

        if (!entity->passThrough) {
            if (abs(entity->velocity.x) <= abs(entity->velocity.y)) {
                if (!Buggernaut_HandleTileCollisionsY())
                    Buggernaut_HandleTileCollisionsX();
            }
            else if (!Buggernaut_HandleTileCollisionsX()) {
                Buggernaut_HandleTileCollisionsY();
            }
        }
    }

    RSDK.ProcessAnimation(&entity->animator2);
    if (buggernaut->state == Buggernaut_State_Setup)
        destroyEntity(entity);
}

void Buggernaut_EditorDraw(void)
{
    RSDK_THIS(Buggernaut);
    entity->drawFX = FX_FLIP;
    Buggernaut_Draw();
}

void Buggernaut_EditorLoad(void) { Buggernaut->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Buggernaut.bin", SCOPE_STAGE); }

void Buggernaut_Serialize(void)
{
    RSDK_EDITABLE_VAR(Buggernaut, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Buggernaut, VAR_BOOL, passThrough);
}
