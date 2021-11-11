#include "SonicMania.h"

ObjectLavaFall *LavaFall;

void LavaFall_Update(void)
{
    RSDK_THIS(LavaFall);
    StateMachine_Run(entity->state);
}

void LavaFall_LateUpdate(void) {}

void LavaFall_StaticUpdate(void)
{
    if (LavaFall->shouldPlayLavaSfx) {
        if (!LavaFall->playingLavaSfx) {
            RSDK.PlaySfx(LavaFall->sfxLava, 81870, 255);
            LavaFall->playingLavaSfx = true;
        }
        LavaFall->shouldPlayLavaSfx = 0;
    }
    else if (LavaFall->playingLavaSfx) {
        RSDK.StopSFX(LavaFall->sfxLava);
        LavaFall->playingLavaSfx = false;
    }
}

void LavaFall_Draw(void)
{
    RSDK_THIS(LavaFall);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void LavaFall_Create(void *data)
{
    RSDK_THIS(LavaFall);

    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x1000000;
    if (data) {
        entity->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(LavaFall->aniFrames, 0, &entity->animator, true, 0);
        entity->state = LavaFall_State_Lava;
    }
    else {
        entity->state = LavaFall_State_Idle;
    }
}

void LavaFall_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        LavaFall->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaFall.bin", SCOPE_STAGE);
    LavaFall->hitbox.left       = -32;
    LavaFall->hitbox.top        = -32;
    LavaFall->hitbox.right      = 32;
    LavaFall->hitbox.bottom     = 32;
    LavaFall->active            = ACTIVE_ALWAYS;
    LavaFall->shouldPlayLavaSfx = 0;
    LavaFall->playingLavaSfx    = false;
    LavaFall->sfxLava           = RSDK.GetSFX("Stage/Lava.wav");
}

void LavaFall_State_Idle(void)
{
    RSDK_THIS(LavaFall);

    if (!((Zone->timer + entity->intervalOffset) % entity->interval)) {
        entity->timer  = entity->duration;
        entity->active = ACTIVE_NORMAL;
        entity->state  = LavaFall_State_LavaFall;
    }
}

void LavaFall_State_LavaFall(void)
{
    RSDK_THIS(LavaFall);

    if (--entity->timer <= 0) {
        if (entity->interval) {
            entity->active = ACTIVE_BOUNDS;
            entity->state  = LavaFall_State_Idle;
        }
        else {
            destroyEntity(entity);
        }
    }

    ++LavaFall->shouldPlayLavaSfx;
    if (++entity->animator.animationTimer == 8) {
        entity->animator.animationTimer = 0;
        CREATE_ENTITY(LavaFall, intToVoid(1), entity->position.x, entity->position.y);
    }
}

void LavaFall_State_Lava(void)
{
    RSDK_THIS(LavaFall);

    entity->position.y += 0x70000;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &LavaFall->hitbox)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }

    if (entity->activeScreens)
        ++LavaFall->shouldPlayLavaSfx;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void LavaFall_EditorDraw(void)
{
    RSDK_THIS(LavaFall);
    RSDK.SetSpriteAnimation(LavaFall->aniFrames, 0, &entity->animator, true, 0);

    LavaFall_Draw();
}

void LavaFall_EditorLoad(void) { LavaFall->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaFall.bin", SCOPE_STAGE); }
#endif

void LavaFall_Serialize(void)
{
    RSDK_EDITABLE_VAR(LavaFall, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(LavaFall, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(LavaFall, VAR_UINT16, duration);
}
