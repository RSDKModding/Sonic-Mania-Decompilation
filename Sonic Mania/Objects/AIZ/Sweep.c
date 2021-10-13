#include "SonicMania.h"

ObjectSweep *Sweep;

void Sweep_Update(void)
{
    RSDK_THIS(Sweep);
    StateMachine_Run(entity->state);
    if (entity->state != Sweep_Unknown11) {
        EntityWater *water = (EntityWater *)entity->waterPtr;
        if (water)
            entity->position.y = water->position.y - (water->size.y >> 1) - 0x90000;
        else
            entity->position.y = Water->waterLevel - 0x90000;
    }
}

void Sweep_LateUpdate(void) {}

void Sweep_StaticUpdate(void) {}

void Sweep_Draw(void)
{
    RSDK_THIS(Sweep);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Sweep_Create(void *data)
{
    RSDK_THIS(Sweep);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible = true;
        entity->drawFX |= FX_FLIP;
        entity->drawOrder = Zone->drawOrderLow;
        entity->startPos  = entity->position;
        entity->startDir  = entity->direction;
        entity->timer     = 128;
        entity->hasShot   = false;

        if (data) {
            entity->active        = ACTIVE_NORMAL;
            entity->updateRange.x = 0x200000;
            entity->updateRange.y = 0x200000;
            RSDK.SetSpriteAnimation(Sweep->aniFrames, 4, &entity->animator, true, 0);
            entity->state = Sweep_Unknown11;
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &entity->animator, true, 0);

            foreach_all(Water, water)
            {
                Hitbox waterHitbox;
                Hitbox hitbox;

                waterHitbox.right  = water->size.x >> 17;
                waterHitbox.left   = -(water->size.x >> 17);
                waterHitbox.bottom = water->size.y >> 17;
                waterHitbox.top    = -(water->size.y >> 17);

                hitbox.left   = 1;
                hitbox.top    = 64;
                hitbox.right  = 1;
                hitbox.bottom = 64;
                if (water->type == 1 && RSDK.CheckObjectCollisionTouchBox(water, &waterHitbox, entity, &hitbox)) {
                    entity->waterPtr = (Entity *)water;
                }
            }
            entity->state = Sweep_Unknown5;
        }
    }
}

void Sweep_StageLoad(void)
{
    if (RSDK.CheckStageFolder("CPZ")) {
        Sweep->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Sweep.bin", SCOPE_STAGE);
    }
#if RETRO_USE_PLUS
    else if (RSDK.CheckStageFolder("AIZ")) {
        Sweep->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Sweep.bin", SCOPE_STAGE);
    }
#endif
    Sweep->hitbox1.left   = -10;
    Sweep->hitbox1.top    = -7;
    Sweep->hitbox1.right  = 10;
    Sweep->hitbox1.bottom = 4;
    Sweep->hitbox2.left   = -256;
    Sweep->hitbox2.top    = -16;
    Sweep->hitbox2.right  = 0;
    Sweep->hitbox2.bottom = 16;
    Sweep->hitbox3.left   = -13;
    Sweep->hitbox3.top    = -3;
    Sweep->hitbox3.right  = -8;
    Sweep->hitbox3.bottom = 3;
    Sweep->sfxPon         = RSDK.GetSFX("Stage/Pon.wav");
    DEBUGMODE_ADD_OBJ(Sweep);
}

void Sweep_DebugSpawn(void)
{
    RSDK_THIS(Sweep);
    EntitySweep *sweep = CREATE_ENTITY(Sweep, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
    sweep->direction   = entity->direction;
    sweep->startDir    = entity->direction;
}

void Sweep_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Sweep_CheckOnScreen(void)
{
    RSDK_THIS(Sweep);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Sweep_Create(NULL);
    }
}

void Sweep_HandleInteractions(void)
{
    RSDK_THIS(Sweep);

    foreach_active(Player, player)
    {
        Hitbox hitbox;
        Hitbox *playerHitbox = Player_GetHitbox(player);
        RSDK.GetHitbox(&player->playerAnimator, 0);

        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
        if (shield->objectID == Shield->objectID && shield->state == Shield_State_Insta) {
            hitbox.left   = 2 * playerHitbox->left - (playerHitbox->left >> 1);
            hitbox.top    = 2 * playerHitbox->top - (playerHitbox->top >> 1);
            hitbox.right  = 2 * playerHitbox->right - (playerHitbox->right >> 1);
            hitbox.bottom = 2 * playerHitbox->bottom - (playerHitbox->bottom >> 1);
            playerHitbox  = &hitbox;
        }

        int32 side = RSDK.CheckObjectCollisionBox(entity, &Sweep->hitbox1, player, playerHitbox, false);
        if (side) {
            if (entity->state != Sweep_Unknown10 && ((entity->direction == FLIP_NONE && side == 2) || (entity->direction == FLIP_X && side == 3)))
                Player_CheckHit(player, entity);
            else
                Player_CheckBadnikBreak(entity, player, true);
        }
    }
}

void Sweep_CheckShoot(void)
{
    RSDK_THIS(Sweep);

    if (!entity->hasShot && entity->timer <= 16) {

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Sweep->hitbox2)) {
                RSDK.SetSpriteAnimation(Sweep->aniFrames, 3, &entity->animator, true, 0);
                EntitySweep *projectile = CREATE_ENTITY(Sweep, intToVoid(true), entity->position.x, entity->position.y);
                if (!entity->direction)
                    projectile->velocity.x = -0x30000;
                else
                    projectile->velocity.x = 0x30000;
                projectile->direction = entity->direction;
                projectile->active    = ACTIVE_NORMAL;
                RSDK.PlaySfx(Sweep->sfxPon, 0, 255);
                entity->hasShot    = true;
                entity->stateStore = entity->state;
                entity->state      = Sweep_Unknown9;
                foreach_break;
            }
        }
    }
}

void Sweep_Unknown5(void)
{
    RSDK_THIS(Sweep);

    entity->active  = ACTIVE_NORMAL;
    entity->timer   = 32;
    entity->hasShot = false;
    entity->state   = Sweep_Unknown6;
    Sweep_Unknown6();
}

void Sweep_Unknown6(void)
{
    RSDK_THIS(Sweep);

    entity->timer--;
    if (entity->timer) {
        Sweep_CheckShoot();
    }
    else {
        entity->hasShot = false;
        entity->timer   = 64;
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &entity->animator, true, 1);
        entity->state = Sweep_Unknown7;
    }
    Sweep_HandleInteractions();
    Sweep_CheckOnScreen();
}

void Sweep_Unknown7(void)
{
    RSDK_THIS(Sweep);

    RSDK.ProcessAnimation(&entity->animator);
    entity->timer--;
    if (entity->timer <= 0) {
        entity->timer   = 32;
        entity->hasShot = false;
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 1, &entity->animator, true, 0);
        entity->state = Sweep_Unknown8;
    }
    else {
        if (entity->direction && entity->velocity.x < 0x20000) {
            entity->velocity.x += 0x3800;
        }
        else if (!entity->direction && entity->velocity.x > -0x20000) {
            entity->velocity.x -= 0x3800;
        }
        entity->position.x += entity->velocity.x;
    }

    Sweep_HandleInteractions();
    Sweep_CheckOnScreen();
}

void Sweep_Unknown8(void)
{
    RSDK_THIS(Sweep);

    RSDK.ProcessAnimation(&entity->animator);
    entity->timer--;
    if (entity->timer <= 0) {
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 2, &entity->animator, true, 0);
        entity->state = Sweep_Unknown10;
    }
    else if (!entity->velocity.x) {
        Sweep_CheckShoot();
    }
    else {
        if (entity->direction) {
            if (entity->velocity.x > 0)
                entity->velocity.x -= 0x3800;
            else
                entity->velocity.x = 0;
        }
        else {
            if (entity->velocity.x > 0)
                entity->velocity.x += 0x3800;
            else
                entity->velocity.x = 0;
        }
        entity->position.x += entity->velocity.x;
    }

    Sweep_HandleInteractions();
    Sweep_CheckOnScreen();
}

void Sweep_Unknown9(void)
{
    RSDK_THIS(Sweep);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 1, &entity->animator, true, 4);
        entity->state = entity->stateStore;
        StateMachine_Run(entity->state);
    }
    else {
        Sweep_HandleInteractions();
        Sweep_CheckOnScreen();
    }
}

void Sweep_Unknown10(void)
{
    RSDK_THIS(Sweep);

    entity->position.x += entity->velocity.x;
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Sweep->aniFrames, 0, &entity->animator, true, 0);
        entity->timer = 32;
        entity->direction ^= 1;
        entity->state = Sweep_Unknown6;
        Sweep_Unknown6();
    }
    else {
        Sweep_HandleInteractions();
        Sweep_CheckOnScreen();
    }
}

void Sweep_Unknown11(void)
{
    RSDK_THIS(Sweep);
    entity->position.x += entity->velocity.x;
    if (RSDK.CheckOnScreen(entity, 0)) {
        RSDK.ProcessAnimation(&entity->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Sweep->hitbox3)) {
                Player_CheckProjectileHit(player, entity);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Sweep_EditorDraw(void) {}

void Sweep_EditorLoad(void) {}

void Sweep_Serialize(void) {}
