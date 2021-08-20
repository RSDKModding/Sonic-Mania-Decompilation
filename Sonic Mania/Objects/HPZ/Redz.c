#include "SonicMania.h"

ObjectRedz *Redz = NULL;

void Redz_Update(void)
{
    RSDK_THIS(Redz);
    RSDK.ProcessAnimation(&entity->animator);

    StateMachine_Run(entity->state);

    if (entity->state != Redz_State_Setup && entity->state != Redz_State_Flame && entity->state != Redz_State_FlameSetup) {
        Redz_HandlePlayerInteractions();
        if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
            entity->direction  = entity->startDir;
            entity->position.x = entity->startPos.x;
            entity->position.y = entity->startPos.y;
            Redz_Create(NULL);
        }
    }
}

void Redz_LateUpdate(void) {}

void Redz_StaticUpdate(void) {}

void Redz_Draw(void)
{
    RSDK_THIS(Redz);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Redz_Create(void *data)
{
    RSDK_THIS(Redz);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos      = entity->position;
    entity->startDir      = entity->direction;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = Redz_State_Setup;
}

void Redz_StageLoad(void)
{
    //if (RSDK.CheckStageFolder("HPZ"))
        Redz->aniFrames = RSDK.LoadSpriteAnimation("HPZ/Redz.bin", SCOPE_STAGE);
    Redz->hitboxBadnik.left   = -16;
    Redz->hitboxBadnik.top    = -16;
    Redz->hitboxBadnik.right  = 16;
    Redz->hitboxBadnik.bottom = 16;
    Redz->hitbox2.left        = -64;
    Redz->hitbox2.top         = -96;
    Redz->hitbox2.right       = 0;
    Redz->hitbox2.bottom      = -8;
    Redz->hitboxFlame.left    = -7;
    Redz->hitboxFlame.top     = -7;
    Redz->hitboxFlame.right   = 7;
    Redz->hitboxFlame.bottom  = -7;
    Redz->hitboxRange.left    = 0;
    Redz->hitboxRange.top     = 0;
    Redz->hitboxRange.right   = 0;
    Redz->hitboxRange.bottom  = 0;
    Redz->sfxFlame            = RSDK.GetSFX("Stage/Flame.wav");
    DEBUGMODE_ADD_OBJ(Redz);
}

void Redz_DebugSpawn(void)
{
    RSDK_THIS(Redz);
    EntityRedz *redz = CREATE_ENTITY(Redz, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
    redz->direction  = entity->direction;
    redz->startDir   = entity->direction;
}

void Redz_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Redz_HandlePlayerInteractions(void)
{
    RSDK_THIS(Redz);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Redz->hitboxBadnik))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Redz_State_Setup(void)
{
    RSDK_THIS(Redz);

    entity->active = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &entity->animator, true, 0);
    entity->animator.frameID        = 0;
    entity->animator.animationSpeed = 1;
    entity->state                   = Redz_State_Unknown;
    entity->velocity.x              = (2 * (entity->direction != FLIP_NONE) - 1) << 15;
    Redz_State_Unknown();
}

void Redz_State_Unknown(void)
{
    RSDK_THIS(Redz);

    entity->position.x += entity->velocity.x;
    if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, (2 * (entity->direction != FLIP_NONE) - 1) << 19, 0x100000, 8)) {
        entity->state = Redz_State_Unknown2;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &entity->animator, true, 0);
        entity->animator.frameID        = 0;
        entity->animator.animationSpeed = 0;
    }

    if (entity->timer <= 0) {
        foreach_active(Player, player)
        {
            if (RSDK.CheckObjectCollisionTouchBox(player, &Redz->hitboxRange, entity, &Redz->hitbox2)) {
                entity->state = Redz_State_Unknown3;
                RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &entity->animator, true, 0);
                entity->animator.frameID        = 0;
                entity->timer                   = 60;
                entity->animator.animationSpeed = 0;
            }
        }
    }
    else {
        entity->timer--;
    }
}

void Redz_State_Unknown2(void)
{
    RSDK_THIS(Redz);

    if (entity->timer2 >= 59) {
        entity->state  = Redz_State_Unknown;
        entity->timer2 = 0;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &entity->animator, true, 0);
        entity->animator.frameID        = 0;
        entity->animator.animationSpeed = 1;
        entity->direction               = entity->direction == FLIP_NONE;
        entity->velocity.x              = -entity->velocity.x;
    }
    else {
        entity->timer2++;
    }
}

void Redz_State_Unknown3(void)
{
    RSDK_THIS(Redz);
    if (entity->timer2 >= 30) {
        entity->state  = Redz_State_Unknown4;
        entity->timer2 = 0;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 1, &entity->animator, true, 0);
        RSDK.PlaySFX(Redz->sfxFlame, false, 255);
    }
    else {
        entity->timer2++;
    }
}

void Redz_State_Unknown4(void)
{
    RSDK_THIS(Redz);
    if (entity->timer2 >= 90) {
        entity->timer2 = 0;
        RSDK.SetSpriteAnimation(Redz->aniFrames, 0, &entity->animator, true, 0);
        entity->animator.frameID        = 0;
        entity->animator.animationSpeed = 1;
        entity->state                   = Redz_State_Unknown;
    }

    if (entity->timer2 & 3) {
        ++entity->timer2;
    }
    else {
        EntityRedz *flame = CREATE_ENTITY(Redz, entity, entity->position.x, entity->position.y);
        flame->state      = Redz_State_FlameSetup;
        flame->position.y -= 0x40000;
        flame->position.x += (2 * (entity->direction != FLIP_NONE) - 1) << 19;
        flame->velocity.x =
            (2 * (entity->direction != FLIP_NONE) - 1) * (RSDK.Cos512(((RSDK.Sin512(8 * (entity->timer2 & 0x3F)) >> 5) - 48) & 0x1FF) << 8);
        flame->velocity.y = RSDK.Sin512(((RSDK.Sin512(8 * (entity->timer2 & 0x3F)) >> 5) - 48) & 0x1FF) << 8;
        ++entity->timer2;
    }
}

void Redz_State_FlameSetup(void)
{
    RSDK_THIS(Redz);

    RSDK_sceneInfo->entity->active = ACTIVE_NORMAL;
    RSDK.SetSpriteAnimation(Redz->aniFrames, 2, &entity->animator, true, 0);
    entity->animator.frameID        = 0;
    entity->state                   = Redz_State_Flame;
    entity->animator.animationSpeed = 1;
    Redz_State_Flame();
}

void Redz_State_Flame(void)
{
    RSDK_THIS(Redz);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Redz->hitboxFlame)) {
            if (player->shield != SHIELD_FIRE)
                Player_CheckHit(player, entity);
        }
    }

    if (++entity->timer2 > 20)
        destroyEntity(entity);
}

void Redz_EditorDraw(void) {}

void Redz_EditorLoad(void) {}

void Redz_Serialize(void) { RSDK_EDITABLE_VAR(Redz, VAR_UINT8, direction); }
