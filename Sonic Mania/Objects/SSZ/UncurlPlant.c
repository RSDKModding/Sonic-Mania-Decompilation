#include "SonicMania.h"

ObjectUncurlPlant *UncurlPlant;

void UncurlPlant_Update(void)
{
    RSDK_THIS(UncurlPlant);
    if (entity->field_120 != 1) {
        if (entity->field_124 > 0)
            entity->field_124 -= 16;
    }
    else {
        if (entity->field_12C <= 0) {
            for (int i = 0; i < entity->field_128; ++i) {
                entity->field_100[i] += (UncurlPlant->array3[i] - entity->field_100[i]) >> 2;
            }

            for (int i = entity->field_128; i < 8; ++i) {
                entity->field_100[i] += (UncurlPlant->array2[i] - entity->field_100[i]) >> 2;
            }
        }
        else {
            for (int i = 0; i < entity->field_128; ++i) {
                entity->field_100[i] = UncurlPlant->array3[i];
                entity->segments[i]  = UncurlPlant->array2[i];
            }
        }
        if (entity->field_12C > 1) {
            entity->field_124 = 256;
        }
        else if (entity->field_124 < 256) {
            entity->field_124 += 32;
        }
    }

    entity->segments[0] = UncurlPlant->array1[0] + ((entity->field_124 * (entity->field_100[0] - UncurlPlant->array1[0])) >> 8);
    entity->segments[1] = UncurlPlant->array1[1] + ((entity->field_124 * (entity->field_100[1] - UncurlPlant->array1[1])) >> 8);
    entity->segments[2] = UncurlPlant->array1[2] + ((entity->field_124 * (entity->field_100[2] - UncurlPlant->array1[2])) >> 8);
    entity->segments[3] = UncurlPlant->array1[3] + ((entity->field_124 * (entity->field_100[3] - UncurlPlant->array1[3])) >> 8);
    entity->segments[4] = UncurlPlant->array1[4] + ((entity->field_124 * (entity->field_100[4] - UncurlPlant->array1[4])) >> 8);
    entity->segments[5] = UncurlPlant->array1[5] + ((entity->field_124 * (entity->field_100[5] - UncurlPlant->array1[5])) >> 8);
    entity->segments[6] = UncurlPlant->array1[6] + ((entity->field_124 * (entity->field_100[6] - UncurlPlant->array1[6])) >> 8);
    entity->segments[7] = UncurlPlant->array1[7] + ((entity->field_124 * (entity->field_100[7] - UncurlPlant->array1[7])) >> 8);

    int valStore      = entity->field_128;
    entity->field_120 = 0;
    entity->field_128 = -1;
    entity->field_12C = 0;
    UncurlPlant_CalculateDrawPositions();
    foreach_active(Player, player)
    {
        int angle = 0;
        int val   = 0;
        for (int i = 0; i < 8; ++i) {
            angle += entity->segments[i];
            if (angle >= 144)
                break;

            entity->position.x = entity->positions[i].x;
            entity->position.y = entity->positions[i].y;
            if (Player_CheckCollisionPlatform(player, entity, &UncurlPlant->hitbox) == 1) {
                player->position.y += 0x40000;
                entity->field_120 = 1;
                if (val > entity->field_128) {
                    if (abs(player->velocity.x) < 0xC0000) {
                        if (abs(player->velocity.x) >= 0x40000)
                            entity->field_12C = 1;
                    }
                    else {
                        entity->field_12C = 2;
                    }
                    entity->field_128 = val;
                }
            }
            ++val;
        }
    }

    entity->field_128++;
    if (entity->field_120 == 2) {
        if (valStore > entity->field_128) {
            entity->field_128 = valStore - 1;
        }
        else if (valStore < entity->field_128) {
            entity->field_128 = valStore + 1;
        }
    }
    entity->position.x = entity->drawPositions[0].x;
    entity->position.y = entity->drawPositions[0].y;
}

void UncurlPlant_LateUpdate(void) {}

void UncurlPlant_StaticUpdate(void) {}

void UncurlPlant_Draw(void)
{
    RSDK_THIS(UncurlPlant);
    for (int i = 0; i < 8; ++i) {
        RSDK.DrawSprite(&entity->animator, &entity->drawPositions[i], false);
        RSDK.DrawSprite(&entity->animators[i], &entity->drawPositions[i], false);
    }
}

void UncurlPlant_Create(void *data)
{
    RSDK_THIS(UncurlPlant);
    if (RSDK_sceneInfo->inEditor) {
        entity->segments[0] = 0x00;
        entity->segments[1] = 0x10;
        entity->segments[2] = 0x40;
        entity->segments[3] = 0x60;
        entity->segments[4] = 0x70;
        entity->segments[5] = 0x90;
        entity->segments[6] = 0xA0;
        entity->segments[7] = 0xC0;
    }
    else {
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh - 2;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        UncurlPlant_CalculatePositions();
        RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &entity->animator, true, 0);
        for (int i = 0; i < 8; ++i) {
            RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &entity->animators[i], true, RSDK.Rand(1, 8));
            entity->segments[i] = UncurlPlant->array1[i];
        }
    }
}

void UncurlPlant_StageLoad(void)
{
    UncurlPlant->aniFrames     = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);
    UncurlPlant->hitbox.left   = -8;
    UncurlPlant->hitbox.top    = -11;
    UncurlPlant->hitbox.right  = 8;
    UncurlPlant->hitbox.bottom = 8;
}

void UncurlPlant_CalculateDrawPositions(void)
{
    RSDK_THIS(UncurlPlant);

    int angle                  = entity->segments[0];
    entity->drawPositions[0].x = entity->position.x;
    entity->drawPositions[0].y = entity->position.y;
    if (entity->direction == FLIP_NONE) {
        for (int i = 1; i < 8; ++i) {
            angle += entity->segments[i];
            entity->drawPositions[i].x = entity->drawPositions[i - 1].x + (RSDK.Cos1024(angle) << 10);
            entity->drawPositions[i].y = entity->drawPositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
    else {
        for (int i = 1; i < 8; ++i) {
            angle += entity->segments[i];
            entity->drawPositions[i].x = entity->drawPositions[i - 1].x - (RSDK.Cos1024(angle) << 10);
            entity->drawPositions[i].y = entity->drawPositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
}

void UncurlPlant_CalculatePositions(void)
{
    RSDK_THIS(UncurlPlant);

    int angle              = UncurlPlant->array3[0];
    entity->positions[0].x = entity->position.x;
    entity->positions[0].y = entity->position.y;
    if (entity->direction == FLIP_NONE) {
        for (int i = 1; i < 8; ++i) {
            angle += UncurlPlant->array3[i];
            entity->positions[i].x = entity->positions[i - 1].x + (RSDK.Cos1024(angle) << 10);
            entity->positions[i].y = entity->positions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
    else {
        for (int i = 1; i < 8; ++i) {
            angle += UncurlPlant->array3[i];
            entity->positions[i].x = entity->positions[i - 1].x - (RSDK.Cos1024(angle) << 10);
            entity->positions[i].y = entity->positions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
}

void UncurlPlant_EditorDraw(void) {}

void UncurlPlant_EditorLoad(void) {}

void UncurlPlant_Serialize(void) { RSDK_EDITABLE_VAR(UncurlPlant, VAR_UINT8, direction); }
