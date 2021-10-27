#include "SonicMania.h"

ObjectMetalArm *MetalArm;

void MetalArm_Update(void)
{
    RSDK_THIS(MetalArm);
    entity->offset.x = -entity->posUnknown.x;
    entity->offset.y = -entity->posUnknown.y;

    int32 valA = entity->timer;
    if (entity->durationA < entity->timer)
        valA = entity->durationA;
    int32 difA           = ((entity->endAngleA - entity->startAngleA) << 16) / entity->durationA;
    entity->armAngle.x = (entity->startAngleA << 16) + difA * valA;

    int32 valB = entity->timer;
    if (entity->durationB < entity->timer)
        valB = entity->durationB;
    int32 difB           = ((entity->endAngleB - entity->startAngleB) << 16) / entity->durationB;
    entity->armAngle.y = (entity->startAngleB << 16) + difB * valB;

    entity->posUnknown = MetalArm_Unknown2();
    entity->posUnknown.x &= 0xFFFF0000;
    entity->posUnknown.y &= 0xFFFF0000;
    entity->position.x = entity->posUnknown.x;
    entity->position.y = entity->posUnknown.y;
    entity->offset.x += entity->position.x;
    entity->offset.y += entity->position.y;
    MetalArm_Unknown3();
    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
    StateMachine_Run(entity->state);
}

void MetalArm_LateUpdate(void) {}

void MetalArm_StaticUpdate(void) {}

void MetalArm_Draw(void)
{
    RSDK_THIS(MetalArm);
    entity->rotation = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    int32 x = 0x2400 * RSDK.Cos512((entity->armAngle.x >> 16)) + entity->position.x;
    int32 y = 0x2400 * RSDK.Sin512((entity->armAngle.x >> 16)) + entity->position.y;

    Vector2 drawPos;
    drawPos            = MetalArm_Unknown2();
    entity->position.x = x;
    entity->position.y = y;
    entity->position.x &= 0xFFFF0000;
    entity->position.y &= 0xFFFF0000;
    entity->rotation = (entity->armAngle.x + entity->armAngle.y) >> 16;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
    entity->position.x &= 0xFFFF0000;
    entity->position.y &= 0xFFFF0000;
    entity->rotation = (entity->armAngle.x >> 16);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->rotation = 0;
    entity->position = drawPos;
    entity->position.x &= 0xFFFF0000;
    entity->position.y &= 0xFFFF0000;
    RSDK.DrawSprite(&entity->animator4, &drawPos, false);

    entity->position.x = entity->startPos.x;
    entity->position.y = entity->startPos.y;
}

void MetalArm_Create(void *data)
{
    RSDK_THIS(MetalArm);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->updateRange.x = 0x1000000;
    entity->updateRange.y = 0x1000000;
    if (!entity->durationA)
        entity->durationA = 60;
    if (!entity->durationB)
        entity->durationB = 40;
    if (!entity->holdDuration)
        entity->holdDuration = 60;
    entity->hitbox.left   = -56;
    entity->hitbox.top    = -27;
    entity->hitbox.right  = 35;
    entity->hitbox.bottom = -7;
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 2, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(MetalArm->aniFrames, 3, &entity->animator4, true, 0);
    entity->armAngle.x = entity->startAngleA << 16;
    entity->armAngle.y = entity->startAngleB << 16;
    entity->timer      = 0;
    entity->posUnknown = MetalArm_Unknown2();
    entity->state      = MetalArm_Unknown4;
}

void MetalArm_StageLoad(void)
{
    MetalArm->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/MetalArm.bin", SCOPE_STAGE);
    Soundboard_LoadSFX("TMZ1/MetalArm.wav", true, MetalArm_Unknown1, NULL);
}

bool32 MetalArm_Unknown1(void)
{
    int32 count = 0;
    foreach_active(MetalArm, arm)
    {
        if (arm->state == MetalArm_Unknown5 || arm->state == MetalArm_Unknown7)
            ++count;
    }
    return count > 0;
}

Vector2 MetalArm_Unknown2(void)
{
    RSDK_THIS(MetalArm);
    Vector2 result;

    int32 x    = 0x2400 * RSDK.Cos512((entity->armAngle.x >> 16)) + entity->position.x;
    int32 y    = 0x2400 * RSDK.Sin512((entity->armAngle.x >> 16)) + entity->position.y;
    result.x = x + 0x3800 * RSDK.Cos512((entity->armAngle.x + entity->armAngle.y) >> 16);
    result.y = y + 0x3800 * RSDK.Sin512((entity->armAngle.x + entity->armAngle.y) >> 16);
    return result;
}

void MetalArm_Unknown3(void)
{
    RSDK_THIS(MetalArm);

    foreach_active(Player, player)
    {
        int32 id = RSDK.GetEntityID(player);
        if ((1 << id) & entity->activePlayers) {
            player->position.x += entity->offset.x;
            player->position.y += entity->offset.y;
            player->position.y += 0x10000;
        }

        if (!Player_CheckCollisionPlatform(player, entity, &entity->hitbox))
            entity->activePlayers &= ~(1 << id);
        else
            entity->activePlayers |= (1 << id);
    }
}

void MetalArm_Unknown4(void)
{
    RSDK_THIS(MetalArm);
    if ((entity->activePlayers & 1)) {
        entity->timer  = 0;
        entity->timer2 = 0;
        entity->active = ACTIVE_NORMAL;
        entity->state  = MetalArm_Unknown5;
    }
}

void MetalArm_Unknown5(void)
{
    RSDK_THIS(MetalArm);

    int32 duration = entity->durationB;
    if (entity->durationA > entity->durationB)
        duration = entity->durationA;
    if (entity->timer >= duration) {
        entity->timer2 = 0;
        entity->state  = MetalArm_Unknown6;
    }
    else {
        entity->timer++;
    }
}

void MetalArm_Unknown6(void)
{
    RSDK_THIS(MetalArm);
    if (entity->activePlayers & 1) {
        entity->timer2 = 0;
    }
    else {
        if (entity->timer2 >= entity->holdDuration) {
            entity->state = MetalArm_Unknown7;
        }
        else {
            entity->timer2++;
        }
    }
}

void MetalArm_Unknown7(void)
{
    RSDK_THIS(MetalArm);
    if (entity->timer <= 0) {
        entity->state  = MetalArm_Unknown4;
        entity->active = ACTIVE_BOUNDS;
    }
    else {
        entity->timer--;
    }
}

#if RETRO_INCLUDE_EDITOR
void MetalArm_EditorDraw(void) {}

void MetalArm_EditorLoad(void) {}
#endif

void MetalArm_Serialize(void)
{
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, startAngleA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, startAngleB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, endAngleA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, endAngleB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, durationA);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, durationB);
    RSDK_EDITABLE_VAR(MetalArm, VAR_INT32, holdDuration);
}
