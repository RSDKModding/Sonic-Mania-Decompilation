#include "SonicMania.h"

ObjectDragonfly *Dragonfly;

void Dragonfly_Update(void)
{
    RSDK_THIS(Dragonfly);
    StateMachine_Run(entity->state);
}

void Dragonfly_LateUpdate(void) {}

void Dragonfly_StaticUpdate(void) {}

void Dragonfly_Draw(void)
{
    RSDK_THIS(Dragonfly);
    if (entity->animator.animationID == 3) {
        RSDK.DrawSprite(&entity->animator, 0, false);
    }
    else {
        for (int i = 0; i < 6; ++i) {
            entity->bodyAnimator.frameID = i == 0;
            entity->direction         = entity->directions[i];
            RSDK.DrawSprite(&entity->bodyAnimator, &entity->positions[i], false);
        }
        entity->direction = false;
        RSDK.DrawSprite(&entity->animator, 0, false);
        entity->inkEffect = INK_ALPHA;
        RSDK.DrawSprite(&entity->wingAnimator, 0, false);
        entity->inkEffect = INK_NONE;
    }
}

void Dragonfly_Create(void *data)
{
    RSDK_THIS(Dragonfly);
    entity->visible   = 1;
    entity->drawOrder = Zone->drawOrderLow;

    if (RSDK_sceneInfo->inEditor)
        return;
    if (!entity->speed) {
        entity->dist  = 0x40;
        entity->speed = 0x04;
    }
    entity->drawFX |= INK_BLEND;
    if (data) {
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x100000;
        entity->updateRange.y = 0x100000;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 3, &entity->animator, true, 0);
        entity->state = Dragonfly_DrawDead;
    }
    else {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.y = (entity->dist + 0x80) << 16;
        entity->spawnPos.x    = entity->position.x;
        entity->spawnPos.y    = entity->position.y;
        entity->updateRange.x = 0x800000;
        entity->alpha         = 0x80;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 2, &entity->wingAnimator, true, 0);
        RSDK.SetSpriteAnimation(Dragonfly->animID, 3, &entity->bodyAnimator, true, 0);
        entity->state = Dragonfly_Turn;
    }
}

void Dragonfly_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1"))
        Dragonfly->animID = RSDK.LoadSpriteAnimation("PSZ1/Dragonfly.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("PSZ2"))
        Dragonfly->animID = RSDK.LoadSpriteAnimation("PSZ2/Dragonfly.bin", SCOPE_STAGE);

    Dragonfly->hurtbox.left   = -8;
    Dragonfly->hurtbox.top    = -8;
    Dragonfly->hurtbox.right  = 8;
    Dragonfly->hurtbox.bottom = 8;
    Dragonfly->hitbox.left   = -5;
    Dragonfly->hitbox.top    = -3;
    Dragonfly->hitbox.right  = 5;
    Dragonfly->hitbox.bottom = 2;
    DEBUGMODE_ADD_OBJ(Dragonfly);
}

void Dragonfly_EditorDraw(void) { Dragonfly_DebugDraw(); }

void Dragonfly_EditorLoad(void) { Dragonfly_StageLoad(); }

void Dragonfly_Serialize(void)
{
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, dir);
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, dist);
    RSDK_EDITABLE_VAR(Dragonfly, VAR_UINT8, speed);
}

void Dragonfly_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Dragonfly->animID, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Dragonfly_CheckOnScreen(void)
{
    // rdc would probably not shut the fuck up if i didn't add this /hj
    RSDK_THIS(Dragonfly);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->spawnPos, &entity->updateRange)) {
        entity->position.x = entity->spawnPos.x;
        entity->position.y = entity->spawnPos.y;
        Dragonfly_Create(NULL);
    }
}

void Dragonfly_DebugSpawn(void) { RSDK.CreateEntity(Dragonfly->objectID, 0, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y); }

void Dragonfly_Collide(void)
{
    RSDK_THIS(Dragonfly);
    foreach_active(Player, player) {
        if (Player_CheckBadnikHit(player, entity, &Dragonfly->hurtbox)) {
            if (Player_CheckBadnikBreak(entity, player, false)) {
                for (int i = 0; i < 6; ++i) {
                    EntityDragonfly *child = CREATE_ENTITY(Dragonfly, intToVoid(1), entity->positions[i].x, entity->positions[i].y); 
                    child->animator.frameID = i == 0;
                    child->velocity.x        = RSDK.Rand(-4, 4) << 15;
                    child->velocity.y        = RSDK.Rand(-5, 1) << 15;
                }
                RSDK.ResetEntityPtr(entity, 0, 0);
            }
        }
        else {
            Vector2 oldpos = entity->position;
            for (int i = 0; i < 6; ++i) {
                entity->position = entity->positions[i];
                if (Player_CheckCollisionTouch(player, entity, &Dragonfly->hitbox))
                    Player_CheckHit(player, entity);
            }
            entity->position = oldpos;
        }
    }
}

void Dragonfly_Turn(void)
{
    RSDK_THIS(Dragonfly);
    entity->active = ACTIVE_NORMAL;
    if (!entity->dir) {
        entity->angle = 0;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 0, &entity->animator, true, 0);
    }
    else {
        entity->angle = 0x200;
        RSDK.SetSpriteAnimation(Dragonfly->animID, 1, &entity->animator, true, 0);
    }
    entity->state = Dragonfly_DefaultState;
    Dragonfly_DefaultState();
}

void Dragonfly_DefaultState(void)
{
    RSDK_THIS(Dragonfly);
    entity->angle += entity->speed;
    entity->position.x = 0xC00 * RSDK.Cos256(entity->angle + 0x40) + entity->spawnPos.x;
    entity->position.y = (entity->dist << 6) * RSDK.Sin1024(entity->angle) + entity->spawnPos.y;
    int currentAngle   = entity->angle - 90;

    for (int i = 0; i < 6; ++i, currentAngle += 13) {
        entity->directions[i] = ((currentAngle + 0x100) & 0x3FFu) < 0x200 ? 2 : 0;

        entity->positions[i].x = 0xC00 * RSDK.Cos256(currentAngle + 0x40) + entity->spawnPos.x;
        entity->positions[i].y = (entity->dist << 6) * RSDK.Sin1024(currentAngle) + entity->spawnPos.y;
    }

    bool32 dir = ((entity->angle + 0x100) & 0x3FF) < 0x200;
    if (entity->animator.animationID == 1) {
        if (dir)
            RSDK.SetSpriteAnimation(Dragonfly->animID, 0, &entity->animator, true, 0);
    }
    else if (!dir)
        RSDK.SetSpriteAnimation(Dragonfly->animID, 1, &entity->animator, true, 0);

    RSDK.ProcessAnimation(&entity->animator);
    RSDK.ProcessAnimation(&entity->wingAnimator);

    Dragonfly_Collide();
    Dragonfly_CheckOnScreen();
}

void Dragonfly_DrawDead(void)
{
    RSDK_THIS(Dragonfly);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
    entity->visible ^= true;
}
