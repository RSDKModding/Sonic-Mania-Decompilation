#include "SonicMania.h"

ObjectOctus *Octus;

void Octus_Update(void)
{
    RSDK_THIS(Octus);
    StateMachine_Run(entity->state);
}

void Octus_LateUpdate(void) {}

void Octus_StaticUpdate(void) {}

void Octus_Draw(void)
{
    RSDK_THIS(Octus);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Octus_Create(void *data)
{
    RSDK_THIS(Octus);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->startPos = entity->position;
    entity->startDir = entity->direction;
    entity->timer    = 128;
    entity->field_60 = 0;
    entity->field_70 = 0;

    if (data) {
        entity->inkEffect     = INK_ADD;
        entity->alpha         = 192;
        entity->active        = ACTIVE_NORMAL;
        entity->updateRange.x = 0x200000;
        entity->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 3, &entity->animator, true, 0);
        entity->state = Octus_State_Shot;
    }
    else {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &entity->animator, true, 0);
        entity->state = Octus_State_Setup;
        entity->alpha = 192;
    }
}

void Octus_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") == 1 || RSDK.CheckStageFolder("OOZ2") == 1)
        Octus->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Octus.bin", SCOPE_STAGE);
    Octus->hitbox1.left   = -16;
    Octus->hitbox1.top    = -12;
    Octus->hitbox1.right  = 16;
    Octus->hitbox1.bottom = 12;
    Octus->hitbox3.left   = -128;
    Octus->hitbox3.top    = -512;
    Octus->hitbox3.right  = 128;
    Octus->hitbox3.bottom = 512;
    Octus->hitbox2.left   = -4;
    Octus->hitbox2.top    = -4;
    Octus->hitbox2.right  = 4;
    Octus->hitbox2.bottom = 4;
    Octus->sfxShot        = RSDK.GetSFX("Stage/Shot.wav");
    DEBUGMODE_ADD_OBJ(Octus);
}

void Octus_DebugSpawn(void)
{
    RSDK_THIS(Octus);
    EntityOctus *octus = CREATE_ENTITY(Octus, NULL, entity->position.x, entity->position.y);
    octus->direction   = entity->direction;
    octus->startDir    = entity->direction;
}

void Octus_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Octus_CheckPlayerCollisions(void)
{
    RSDK_THIS(Octus);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Octus->hitbox1))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Octus_CheckOnScreen(void)
{
    RSDK_THIS(Octus);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Octus_Create(NULL);
    }
}

void Octus_State_Setup(void)
{
    RSDK_THIS(Octus);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Octus_Unknown5;
    Octus_Unknown5();
}

void Octus_Unknown5(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&entity->animator);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Octus->hitbox3)) {
            entity->timer = 32;
            RSDK.SetSpriteAnimation(Octus->aniFrames, 1, &entity->animator, true, 0);
            entity->state = Octus_Unknown6;
            foreach_break;
        }
    }

    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown6(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&entity->animator);
    if (--entity->timer <= 0) {
        entity->velocity.y = -0x20000;
        entity->state      = Octus_Unknown7;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown7(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&entity->animator);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1000;
    if (entity->velocity.y >= 0) {
        entity->timer = 60;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 2, &entity->animator, true, 0);
        entity->state = Octus_Unknown8;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown8(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&entity->animator);

    entity->timer--;
    if (entity->timer == 51) {
        EntityOctus *shot = CREATE_ENTITY(Octus, intToVoid(true), entity->position.x, entity->position.y);
        if (entity->direction) {
            shot->position.x += 0xE0000;
            shot->velocity.x = 0x20000;
        }
        else {
            shot->position.x -= 0xE0000;
            shot->velocity.x = -0x20000;
        }
        RSDK.PlaySfx(Octus->sfxShot, false, 255);
    }
    else if (entity->timer <= 0) {
        entity->state = Octus_Unknown9;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_Unknown9(void)
{
    RSDK_THIS(Octus);
    RSDK.ProcessAnimation(&entity->animator);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1000;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xD0000, 1)) {
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &entity->animator, true, 0);
        entity->state = Octus_Unknown5;
    }
    Octus_CheckPlayerCollisions();
    Octus_CheckOnScreen();
}

void Octus_State_Shot(void)
{
    RSDK_THIS(Octus);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.ProcessAnimation(&entity->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Octus->hitbox2)) {
                Player_CheckProjectileHit(player, entity);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void Octus_EditorDraw(void) {}

void Octus_EditorLoad(void) {}
#endif

void Octus_Serialize(void) { RSDK_EDITABLE_VAR(Octus, VAR_UINT8, direction); }
