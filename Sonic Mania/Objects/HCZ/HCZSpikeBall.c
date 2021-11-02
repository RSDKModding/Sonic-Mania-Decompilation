#include "SonicMania.h"

ObjectHCZSpikeBall *HCZSpikeBall;

void HCZSpikeBall_Update(void)
{
    HCZSpikeBall_Unknown4();
    HCZSpikeBall_HandlePlayerInteractions();
}

void HCZSpikeBall_LateUpdate(void) {}

void HCZSpikeBall_StaticUpdate(void) {}

void HCZSpikeBall_Draw(void)
{
    RSDK_THIS(HCZSpikeBall);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void HCZSpikeBall_Create(void *data)
{
    RSDK_THIS(HCZSpikeBall);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->field_5C.x    = entity->position.x;
    entity->field_5C.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    entity->hitbox.left   = -10;
    entity->hitbox.top    = -10;
    entity->hitbox.right  = 10;
    entity->hitbox.bottom = 10;
    HCZSpikeBall_HandleConveyorSetup();
    RSDK.SetSpriteAnimation(HCZSpikeBall->aniFrames, 0, &entity->animator, true, 1);
}

void HCZSpikeBall_StageLoad(void) { HCZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE); }

void HCZSpikeBall_HandleConveyorSetup(void)
{
    RSDK_THIS(HCZSpikeBall);
    if (!RSDK_sceneInfo->inEditor) {
        foreach_all(HangConveyor, conveyor)
        {
            if (MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &conveyor->hitbox3, entity->position.x,
                                          entity->position.y)) {
                entity->conveyor    = conveyor;
                entity->updateRange = conveyor->updateRange;
                foreach_break;
            }
        }

        if (entity->conveyor)
            HCZSpikeBall_Unknown2();
    }
}

void HCZSpikeBall_Unknown2(void)
{
    RSDK_THIS(HCZSpikeBall);

    EntityHangConveyor *conveyor = entity->conveyor;
    if (conveyor) {
        Hitbox hitbox1, hitbox2, hitbox3, hitbox4;

        int32 len = 0;
        if (conveyor->length != -3)
            len = (conveyor->length + 3) << 20;

        hitbox1.top    = -26;
        hitbox1.bottom = -16;
        hitbox1.right  = len >> 17;
        hitbox1.left   = -hitbox1.right;

        hitbox2.left   = conveyor->hitbox3.left;
        hitbox2.top    = conveyor->hitbox3.top;
        hitbox2.right  = -hitbox1.right;
        hitbox2.bottom = conveyor->hitbox3.bottom;

        hitbox3.left   = -(len >> 17);
        hitbox3.top    = 16;
        hitbox3.right  = len >> 17;
        hitbox3.bottom = 26;

        hitbox4.left   = len >> 17;
        hitbox4.top    = hitbox2.top;
        hitbox4.right  = conveyor->hitbox3.right;
        hitbox4.bottom = hitbox2.bottom;

        if (MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox1, entity->position.x,
                                      entity->position.y)) {
            int32 dist = conveyor->position.x - (len >> 1);
            if (conveyor->direction == FLIP_NONE)
                dist = conveyor->position.x + (len >> 1);
            entity->field_68 = abs(entity->position.x - (entity->position.x - dist)) / 0x15555;
        }
        else if (MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox2, entity->position.x,
                                           entity->position.y)) {
            int32 off   = len / 0x15555;
            int32 angle = 384;
            if (conveyor->direction) {
                off   = 2 * (len / 0x15555) + 51;
                angle = 128;
            }

            entity->field_68 =
                (abs(angle - 2 * RSDK.ATan2(entity->position.x - conveyor->position.x - (len >> 1), entity->position.y - conveyor->position.y)) / 5)
                + off;
        }
        else {
            if (!MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox3, entity->position.x,
                                           entity->position.y)) {

                if (!MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox4, entity->position.x,
                                               entity->position.y)) {
                    entity->field_68 = 0;
                }
                else {
                    int32 dist  = len / 0x15555;
                    int32 angle = 384;
                    if (!conveyor->direction) {
                        dist  = 2 * (len / 0x15555) + 51;
                        angle = 128;
                    }

                    entity->field_68 =
                        (abs(angle
                             - 2 * RSDK.ATan2(entity->position.x - (len >> 1) + conveyor->position.x, entity->position.y - conveyor->position.y))
                         / 5)
                        + dist;
                }
            }
            else {
                int32 dist = (len >> 1) + conveyor->position.x;
                if (conveyor->direction == FLIP_NONE)
                    dist = conveyor->position.x - (len >> 1);
                entity->field_68 = abs((entity->position.x - dist)) / 0x15555 + (len / 0x15555) + 51;
            }
        }
    }
}

void HCZSpikeBall_HandlePlayerInteractions(void)
{
    RSDK_THIS(HCZSpikeBall);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            Player_CheckHit(player, entity);
        }
    }
}

void HCZSpikeBall_Unknown4(void)
{
    RSDK_THIS(HCZSpikeBall);

    if (entity->conveyor) {
        int32 len = 0;
        if (entity->conveyor->length != -3)
            len = (entity->conveyor->length + 3) << 20;

        int32 timer   = (entity->field_68 + Zone->timer) % ((2 * len + 0x8A3AE6) / 0x15555);
        int32 conveyX = entity->conveyor->position.x;
        int32 conveyY = entity->conveyor->position.y;
        if (timer < len / 0x15555) {
            if (entity->conveyor->direction == FLIP_NONE)
                entity->position.x = (len >> 1) + conveyX;
            else
                entity->position.x = conveyX - (len >> 1);
            entity->position.y = conveyY - 0x160000;
            entity->position.x += 0x15555 * timer * (2 * (entity->conveyor->direction != FLIP_NONE) - 1);
        }
        else if (timer < len / 0x15555 + 51) {
            int32 val = timer - (len / 0x15555);
            if (entity->conveyor->direction == FLIP_NONE)
                entity->position.x = conveyX - (len >> 1);
            else
                entity->position.x = (len >> 1) + conveyX;

            int32 mult = 5;
            if (entity->conveyor->direction == FLIP_NONE)
                mult = -5;

            entity->position.y = conveyY;
            entity->position.x += 0xB00 * RSDK.Cos512(val * mult + 384);
            entity->position.y += 0xB00 * RSDK.Sin512(val * mult + 384);
        }
        else if (timer >= 2 * (len / 0x15555) + 51) {
            entity->position.y = -51 - 2 * (len / 0x15555) + timer;
            if (entity->conveyor->direction == FLIP_NONE)
                entity->position.x = conveyX + (len >> 1);
            else
                entity->position.x = conveyX - (len >> 1);

            int32 mult = 5;
            if (entity->conveyor->direction == FLIP_NONE)
                mult = -5;

            entity->position.x += 0xB00 * RSDK.Cos512((-51 - 2 * (len / 0x15555) + timer) * mult + 128);
            entity->position.y += 0xB00 * RSDK.Sin512((-51 - 2 * (len / 0x15555) + timer) * mult + 128);
        }
        else {
            entity->position.y = conveyY + 0x160000;
            if (entity->conveyor->direction == FLIP_NONE)
                entity->position.x = (conveyX - (len >> 1));
            else
                entity->position.x = (len >> 1) + conveyX;
            entity->position.x += 0x15555 * (-51 - (len / 0x15555) + timer) * (2 * (entity->conveyor->direction == FLIP_NONE) - 1);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void HCZSpikeBall_EditorDraw(void) { HCZSpikeBall_Draw(); }

void HCZSpikeBall_EditorLoad(void) { HCZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE); }
#endif

void HCZSpikeBall_Serialize(void) {}
