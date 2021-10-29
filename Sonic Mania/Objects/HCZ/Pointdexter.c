#include "SonicMania.h"

ObjectPointdexter *Pointdexter;

void Pointdexter_Update(void)
{
    RSDK_THIS(Pointdexter);
    StateMachine_Run(entity->state);
}

void Pointdexter_LateUpdate(void) {}

void Pointdexter_StaticUpdate(void) {}

void Pointdexter_Draw(void)
{
    RSDK_THIS(Pointdexter);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Pointdexter_Create(void *data)
{
    RSDK_THIS(Pointdexter);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Pointdexter->aniFrames, 0, &entity->animator, true, 0);
    entity->state = Pointdexter_State_Setup;
}

void Pointdexter_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        Pointdexter->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Pointdexter.bin", SCOPE_STAGE);
    Pointdexter->hitbox.left   = -16;
    Pointdexter->hitbox.top    = -6;
    Pointdexter->hitbox.right  = 16;
    Pointdexter->hitbox.bottom = 6;
    DEBUGMODE_ADD_OBJ(Pointdexter);
}

void Pointdexter_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Pointdexter, NULL, entity->position.x, entity->position.y);
}

void Pointdexter_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Pointdexter->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Pointdexter_CheckOnScreen(void)
{
    RSDK_THIS(Pointdexter);

    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Pointdexter_Create(NULL);
    }
}

void Pointdexter_CheckPlayerCollisions(void)
{
    RSDK_THIS(Pointdexter);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Pointdexter->hitbox)) {
            if (entity->animator.frameID != 2 || player->invincibleTimer || player->blinkTimer
#if RETRO_USE_PLUS
                || player->state == Player_State_MightyHammerDrop
#endif
            ) {
                Player_CheckBadnikBreak(entity, player, true);
            }
            else if (!Player_CheckMightyUnspin(0x200, player, 2, &player->uncurlTimer)) {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void Pointdexter_State_Setup(void)
{
    RSDK_THIS(Pointdexter);
    if (entity->position.y >= Water->waterLevel) {
        entity->active = ACTIVE_NORMAL;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
        entity->timer = 0;
        entity->state = Pointdexter_State_Swimming;
        Pointdexter_State_Swimming();
    }
    else {
        destroyEntity(entity);
    }
}

void Pointdexter_State_Swimming(void)
{
    RSDK_THIS(Pointdexter);

    entity->position.x += entity->velocity.x;
    if (!--entity->timer) {
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
        entity->timer      = 0;
    }
    entity->position.y += 32 * RSDK.Sin256(entity->angle);
    entity->angle = (entity->angle + 2) & 0xFF;
    RSDK.ProcessAnimation(&entity->animator);
    Pointdexter_CheckPlayerCollisions();
    Pointdexter_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void Pointdexter_EditorDraw(void) { Pointdexter_Draw(); }

void Pointdexter_EditorLoad(void) { Pointdexter->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Pointdexter.bin", SCOPE_STAGE); }
#endif

void Pointdexter_Serialize(void) { RSDK_EDITABLE_VAR(Pointdexter, VAR_UINT8, direction); }
