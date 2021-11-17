#include "SonicMania.h"

ObjectAIZEggRobo *AIZEggRobo;

void AIZEggRobo_Update(void)
{
    RSDK_THIS(AIZEggRobo);
    if (entity->oscillate)
        entity->position.y += RSDK.Sin256(4 * (entity->oscillateOffset + Zone->timer)) << 7;
    if (entity->movePos.x != entity->position.x) {
        int32 distance = entity->position.x - entity->movePos.x;
        if (distance < 0)
            entity->direction = FLIP_X;
        else if (distance > 0)
            entity->direction = FLIP_NONE;
    }
    entity->movePos = entity->position;
    RSDK.ProcessAnimation(&entity->animatorBody);
    RSDK.ProcessAnimation(&entity->animatorLegs);
    RSDK.ProcessAnimation(&entity->animatorArm);
}

void AIZEggRobo_LateUpdate(void) {}

void AIZEggRobo_StaticUpdate(void) {}

void AIZEggRobo_Draw(void)
{
    RSDK_THIS(AIZEggRobo);
    RSDK.DrawSprite(&entity->animatorLegs, NULL, false);
    RSDK.DrawSprite(&entity->animatorBody, NULL, false);
    RSDK.DrawSprite(&entity->animatorArm, NULL, false);
}

void AIZEggRobo_Create(void *data)
{
    RSDK_THIS(AIZEggRobo);
    entity->active          = ACTIVE_BOUNDS;
    entity->drawOrder       = Zone->drawOrderLow;
    entity->rotation        = entity->angle;
    entity->startPos        = entity->position;
    entity->movePos.x       = entity->position.x;
    entity->movePos.y       = entity->position.y;
    entity->visible         = true;
    entity->drawFX          = FX_ROTATE | FX_FLIP;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x800000;
    entity->oscillateOffset = RSDK.Rand(0, 256);
    RSDK.SetSpriteAnimation(AIZEggRobo->aniFrames, 0, &entity->animatorBody, true, 0);
    RSDK.SetSpriteAnimation(AIZEggRobo->aniFrames, 1, &entity->animatorArm, true, 0);
    RSDK.SetSpriteAnimation(AIZEggRobo->aniFrames, 2, &entity->animatorLegs, true, 0);
}

void AIZEggRobo_StageLoad(void) { AIZEggRobo->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZEggRobo.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void AIZEggRobo_EditorDraw(void) { AIZEggRobo_Draw(); }

void AIZEggRobo_EditorLoad(void)
{
    AIZEggRobo->aniFrames = RSDK.LoadSpriteAnimation("AIZ/AIZEggRobo.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(AIZEggRobo, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void AIZEggRobo_Serialize(void)
{
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_BOOL, oscillate);
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(AIZEggRobo, VAR_BOOL, forKnux);
}
