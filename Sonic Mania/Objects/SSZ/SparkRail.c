#include "SonicMania.h"

ObjectSparkRail *SparkRail;

void SparkRail_Update(void)
{
    RSDK_THIS(SparkRail);
    int32 posY = entity->position.y + (entity->size.y >> 1);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && player->onGround) {
            if (abs(player->groundVel) > 0x80000) {
                RSDK.PlaySfx(SparkRail->sfxPon, false, 255);
                EntityDebris *spark = CREATE_ENTITY(Debris, Debris_State_Move, entity->position.x, entity->position.y);
                RSDK.SetSpriteAnimation(SparkRail->aniFrames, 0, &spark->animator, true, 0);
                spark->drawFX     = FX_FLIP;
                spark->direction  = player->direction != FLIP_NONE;
                spark->drawOrder  = Zone->drawOrderLow;
                spark->position.x = player->position.x;
                spark->position.y = posY;
                spark->timer      = 30;
            }
        }
    }
}

void SparkRail_LateUpdate(void) {}

void SparkRail_StaticUpdate(void) {}

void SparkRail_Draw(void) {}

void SparkRail_Create(void *data)
{
    RSDK_THIS(SparkRail);
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->visible       = false;
    entity->hitbox.left   = -(entity->size.x >> 17);
    entity->hitbox.top    = -(entity->size.y >> 17);
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.bottom = entity->size.y >> 17;
    if (!entity->size.x)
        entity->size.x = 0x800000;
    if (!entity->size.y)
        entity->size.y = 0x200000;
}

void SparkRail_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        SparkRail->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Spark.bin", SCOPE_STAGE);
    else
        SparkRail->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/Spark.bin", SCOPE_STAGE);
    SparkRail->sfxPon = RSDK.GetSFX("Stage/Pon.wav");
}

#if RETRO_INCLUDE_EDITOR
void SparkRail_EditorDraw(void) {}

void SparkRail_EditorLoad(void) {}
#endif

void SparkRail_Serialize(void) { RSDK_EDITABLE_VAR(SparkRail, VAR_VECTOR2, size); }
