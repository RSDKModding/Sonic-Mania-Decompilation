#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectBloominator *Bloominator;

void Bloominator_Update(void)
{
    RSDK_THIS(Bloominator);
    StateMachine_Run(entity->state);
}

void Bloominator_LateUpdate(void) {}

void Bloominator_StaticUpdate(void) {}

void Bloominator_Draw(void)
{
    RSDK_THIS(Bloominator);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Bloominator_Create(void *data)
{
    RSDK_THIS(Bloominator);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    if (data) {
        --entity->drawOrder;
        RSDK.SetSpriteAnimation(Bloominator->spriteIndex, 2, &entity->animator, true, 0);
        entity->state = Bloominator_State_Spikeball;
    }
    else {
        RSDK.SetSpriteAnimation(Bloominator->spriteIndex, 0, &entity->animator, true, 0);
        entity->state = Bloominator_State_Setup;
    }
}

void Bloominator_StageLoad(void)
{
    if (RSDK.CheckStageFolder("AIZ"))
        Bloominator->spriteIndex = RSDK.LoadSpriteAnimation("AIZ/Bloominator.bin", SCOPE_STAGE);
    Bloominator->hitbox.left             = -12;
    Bloominator->hitbox.top              = -20;
    Bloominator->hitbox.right            = 12;
    Bloominator->hitbox.bottom           = 20;
    Bloominator->projectileHitbox.left   = -4;
    Bloominator->projectileHitbox.top    = -4;
    Bloominator->projectileHitbox.right  = 4;
    Bloominator->projectileHitbox.bottom = 4;
    Bloominator->sfxShot                 = RSDK.GetSFX("Stage/Shot.wav");

    DEBUGMODE_ADD_OBJ(Bloominator);
}

void Bloominator_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bloominator->spriteIndex, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Bloominator_DebugSpawn(void)
{
    RSDK_THIS(Bloominator);
    RSDK.CreateEntity(Bloominator->objectID, 0, entity->position.x, entity->position.y);
}

void Bloominator_CheckHit(void)
{
    RSDK_THIS(Bloominator);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Bloominator->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Bloominator_Idle(void)
{
    RSDK_THIS(Bloominator);
    if (entity->activeScreens) {
        if (++entity->timer >= 60) {
            entity->timer = 0;
            RSDK.SetSpriteAnimation(Bloominator->spriteIndex, 1, &entity->animator, true, 0);
            entity->state = Bloominator_State_Firing;
        }
    }
    RSDK.ProcessAnimation(&entity->animator);
    Bloominator_CheckHit();
    if (!RSDK.CheckOnScreen(entity, NULL))
        Bloominator_Create(NULL);
}

void Bloominator_State_Firing(void)
{
    RSDK_THIS(Bloominator);
    EntityBloominator *spikeBall = NULL;

    switch (++entity->timer) {
        case 15:
            spikeBall =
                (EntityBloominator *)RSDK.CreateEntity(Bloominator->objectID, (void *)1, entity->position.x - 0x10000, entity->position.y - 0x160000);
            spikeBall->velocity.x = -0x10000;
            spikeBall->velocity.y = -0x50000;
            RSDK.PlaySfx(Bloominator->sfxShot, 0, 255);
            break;
        case 45:
            spikeBall =
                (EntityBloominator *)RSDK.CreateEntity(Bloominator->objectID, (void *)1, entity->position.x - 0x10000, entity->position.y - 0x160000);
            spikeBall->velocity.x = 0x10000;
            spikeBall->velocity.y = -0x50000;
            RSDK.PlaySfx(Bloominator->sfxShot, 0, 255);
            break;
        case 50:
            entity->timer = -60;
            RSDK.SetSpriteAnimation(Bloominator->spriteIndex, 0, &entity->animator, true, 0);
            entity->state = Bloominator_Idle;
            break;
    }
    RSDK.ProcessAnimation(&entity->animator);
    Bloominator_CheckHit();
    if (!RSDK.CheckOnScreen(entity, NULL))
        Bloominator_Create(NULL);
}

void Bloominator_State_Setup(void)
{
    RSDK_THIS(Bloominator);
    entity->active = ACTIVE_NORMAL;
    entity->timer  = 0;
    entity->state  = Bloominator_Idle;
    Bloominator_Idle();
}

void Bloominator_State_Spikeball(void)
{
    RSDK_THIS(Bloominator);
    if (RSDK.CheckOnScreen(entity, NULL)) {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
        RSDK.ProcessAnimation(&entity->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Bloominator->projectileHitbox)) {
                Player_CheckProjectileHit(player, entity);
            }
        }
    }
    else {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void Bloominator_EditorDraw(void) {}

void Bloominator_EditorLoad(void) {}

void Bloominator_Serialize(void) {}
#endif
