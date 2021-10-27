#include "SonicMania.h"

ObjectCactula *Cactula;

void Cactula_Update(void)
{
    RSDK_THIS(Cactula);
    StateMachine_Run(entity->state);
}

void Cactula_LateUpdate(void) {}

void Cactula_StaticUpdate(void) {}

void Cactula_Draw(void)
{
    RSDK_THIS(Cactula);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y += entity->offsetY;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Cactula_Create(void *data)
{
    RSDK_THIS(Cactula);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow + 1;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->offsetY       = 0x80000;
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &entity->animator2, true, 1);
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 1, &entity->animator3, true, 0);
    entity->state = Cactula_State_Unknown1;
}

void Cactula_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        Cactula->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Cactula.bin", SCOPE_STAGE);

    Cactula->hitbox.left   = -12;
    Cactula->hitbox.top    = -12;
    Cactula->hitbox.right  = 12;
    Cactula->hitbox.bottom = 12;
    DEBUGMODE_ADD_OBJ(Cactula);
    Cactula->sfxCactDrop = RSDK.GetSFX("MSZ/CactDrop.wav");
    Soundboard_LoadSFX("MSZ/CactChopper.wav", true, Cactula_CheckCB, NULL);
}

void Cactula_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Cactula, NULL, entity->position.x, entity->position.y);
}

void Cactula_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Cactula->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Cactula_CheckPlayerCollisions(void)
{
    RSDK_THIS(Cactula);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Cactula->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

bool32 Cactula_CheckCB(void)
{
    int count = 0;
    foreach_active(Cactula, cactula)
    {
        if (cactula->state == Cactula_State_Unknown2 || cactula->state == Cactula_State_Unknown3)
            ++count;
    }
    return count > 0;
}

void Cactula_State_Unknown1(void)
{
    RSDK_THIS(Cactula);

    EntityPlayer *player = Player_GetNearestPlayerX();

    if (abs(entity->position.x - player->position.x) < 0x800000) {
        entity->velocity.y = -0x20000;
        entity->state      = Cactula_State_Unknown2;
    }
    Cactula_CheckPlayerCollisions();
}

void Cactula_State_Unknown2(void)
{
    RSDK_THIS(Cactula);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->offsetY += entity->velocity.y;
    entity->velocity.y += 0x4000;
    if (entity->offsetY >= 0 && entity->velocity.y >= 0) {
        entity->offsetY    = 0;
        entity->velocity.y = -0xA000;
        entity->state      = Cactula_State_Unknown3;
    }
    Cactula_CheckPlayerCollisions();
}

void Cactula_State_Unknown3(void)
{
    RSDK_THIS(Cactula);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->velocity.y += 0x100;
    entity->position.y += entity->velocity.y;

    EntityPlayer *player = Player_GetNearestPlayerX();
    if (!RSDK.CheckOnScreen(entity, NULL) && entity->position.y > player->position.y) {
        destroyEntity(entity);
    }
    else {
        if (!entity->flag) {
            if (abs(entity->position.x - player->position.x) < 0x100000) {
                RSDK.PlaySfx(Cactula->sfxCactDrop, false, 255);
                EntityProjectile *projectile = CREATE_ENTITY(Projectile, Projectile_State_MoveGravity, entity->position.x, entity->position.y);
                projectile->gravityStrength  = 0x3800;
                projectile->drawOrder        = Zone->drawOrderLow;
                projectile->hitbox.left      = -6;
                projectile->hitbox.top       = -6;
                projectile->hitbox.right     = 6;
                projectile->hitbox.bottom    = 6;
                projectile->type             = 7;
                projectile->hurtDelay        = 16;
                RSDK.SetSpriteAnimation(Cactula->aniFrames, 2, &projectile->animator, true, 0);
                entity->flag = true;
            }
        }

        if (RSDK.GetTileInfo(Zone->fgHigh, entity->position.x >> 20, entity->position.y >> 20) == 0xFFFF)
            Cactula_CheckPlayerCollisions();
    }
}

#if RETRO_INCLUDE_EDITOR
void Cactula_EditorDraw(void) { Cactula_Draw(); }

void Cactula_EditorLoad(void) { Cactula->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Cactula.bin", SCOPE_STAGE); }
#endif

void Cactula_Serialize(void) {}
