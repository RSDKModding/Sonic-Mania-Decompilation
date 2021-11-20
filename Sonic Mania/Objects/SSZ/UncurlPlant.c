#include "SonicMania.h"

ObjectUncurlPlant *UncurlPlant;

void UncurlPlant_Update(void)
{
    RSDK_THIS(UncurlPlant);
    if (self->field_120 != 1) {
        if (self->field_124 > 0)
            self->field_124 -= 16;
    }
    else {
        if (self->field_12C <= 0) {
            for (int32 i = 0; i < self->field_128; ++i) {
                self->field_100[i] += (UncurlPlant->array3[i] - self->field_100[i]) >> 2;
            }

            for (int32 i = self->field_128; i < 8; ++i) {
                self->field_100[i] += (UncurlPlant->array2[i] - self->field_100[i]) >> 2;
            }
        }
        else {
            for (int32 i = 0; i < self->field_128; ++i) {
                self->field_100[i] = UncurlPlant->array3[i];
                self->segments[i]  = UncurlPlant->array2[i];
            }
        }
        if (self->field_12C > 1) {
            self->field_124 = 256;
        }
        else if (self->field_124 < 256) {
            self->field_124 += 32;
        }
    }

    self->segments[0] = UncurlPlant->array1[0] + ((self->field_124 * (self->field_100[0] - UncurlPlant->array1[0])) >> 8);
    self->segments[1] = UncurlPlant->array1[1] + ((self->field_124 * (self->field_100[1] - UncurlPlant->array1[1])) >> 8);
    self->segments[2] = UncurlPlant->array1[2] + ((self->field_124 * (self->field_100[2] - UncurlPlant->array1[2])) >> 8);
    self->segments[3] = UncurlPlant->array1[3] + ((self->field_124 * (self->field_100[3] - UncurlPlant->array1[3])) >> 8);
    self->segments[4] = UncurlPlant->array1[4] + ((self->field_124 * (self->field_100[4] - UncurlPlant->array1[4])) >> 8);
    self->segments[5] = UncurlPlant->array1[5] + ((self->field_124 * (self->field_100[5] - UncurlPlant->array1[5])) >> 8);
    self->segments[6] = UncurlPlant->array1[6] + ((self->field_124 * (self->field_100[6] - UncurlPlant->array1[6])) >> 8);
    self->segments[7] = UncurlPlant->array1[7] + ((self->field_124 * (self->field_100[7] - UncurlPlant->array1[7])) >> 8);

    int32 valStore      = self->field_128;
    self->field_120 = 0;
    self->field_128 = -1;
    self->field_12C = 0;
    UncurlPlant_CalculateDrawPositions();
    foreach_active(Player, player)
    {
        int32 angle = 0;
        int32 val   = 0;
        for (int32 i = 0; i < 8; ++i) {
            angle += self->segments[i];
            if (angle >= 144)
                break;

            self->position.x = self->positions[i].x;
            self->position.y = self->positions[i].y;
            if (Player_CheckCollisionPlatform(player, self, &UncurlPlant->hitbox) == 1) {
                player->position.y += 0x40000;
                self->field_120 = 1;
                if (val > self->field_128) {
                    if (abs(player->velocity.x) < 0xC0000) {
                        if (abs(player->velocity.x) >= 0x40000)
                            self->field_12C = 1;
                    }
                    else {
                        self->field_12C = 2;
                    }
                    self->field_128 = val;
                }
            }
            ++val;
        }
    }

    self->field_128++;
    if (self->field_120 == 2) {
        if (valStore > self->field_128) {
            self->field_128 = valStore - 1;
        }
        else if (valStore < self->field_128) {
            self->field_128 = valStore + 1;
        }
    }
    self->position.x = self->drawPositions[0].x;
    self->position.y = self->drawPositions[0].y;
}

void UncurlPlant_LateUpdate(void) {}

void UncurlPlant_StaticUpdate(void) {}

void UncurlPlant_Draw(void)
{
    RSDK_THIS(UncurlPlant);
    for (int32 i = 0; i < 8; ++i) {
        RSDK.DrawSprite(&self->animator, &self->drawPositions[i], false);
        RSDK.DrawSprite(&self->animators[i], &self->drawPositions[i], false);
    }
}

void UncurlPlant_Create(void *data)
{
    RSDK_THIS(UncurlPlant);
    if (SceneInfo->inEditor) {
        self->segments[0] = 0x00;
        self->segments[1] = 0x10;
        self->segments[2] = 0x40;
        self->segments[3] = 0x60;
        self->segments[4] = 0x70;
        self->segments[5] = 0x90;
        self->segments[6] = 0xA0;
        self->segments[7] = 0xC0;
    }
    else {
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderHigh - 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        UncurlPlant_CalculatePositions();
        RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &self->animator, true, 0);
        for (int32 i = 0; i < 8; ++i) {
            RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &self->animators[i], true, RSDK.Rand(1, 8));
            self->segments[i] = UncurlPlant->array1[i];
        }
    }
}

void UncurlPlant_StageLoad(void)
{
    UncurlPlant->aniFrames     = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE);
    UncurlPlant->hitbox.left   = -8;
    UncurlPlant->hitbox.top    = -12;
    UncurlPlant->hitbox.right  = 8;
    UncurlPlant->hitbox.bottom = 8;
}

void UncurlPlant_CalculateDrawPositions(void)
{
    RSDK_THIS(UncurlPlant);

    int32 angle                  = self->segments[0];
    self->drawPositions[0].x = self->position.x;
    self->drawPositions[0].y = self->position.y;
    if (self->direction == FLIP_NONE) {
        for (int32 i = 1; i < 8; ++i) {
            angle += self->segments[i];
            self->drawPositions[i].x = self->drawPositions[i - 1].x + (RSDK.Cos1024(angle) << 10);
            self->drawPositions[i].y = self->drawPositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
    else {
        for (int32 i = 1; i < 8; ++i) {
            angle += self->segments[i];
            self->drawPositions[i].x = self->drawPositions[i - 1].x - (RSDK.Cos1024(angle) << 10);
            self->drawPositions[i].y = self->drawPositions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
}

void UncurlPlant_CalculatePositions(void)
{
    RSDK_THIS(UncurlPlant);

    int32 angle              = UncurlPlant->array3[0];
    self->positions[0].x = self->position.x;
    self->positions[0].y = self->position.y;
    if (self->direction == FLIP_NONE) {
        for (int32 i = 1; i < 8; ++i) {
            angle += UncurlPlant->array3[i];
            self->positions[i].x = self->positions[i - 1].x + (RSDK.Cos1024(angle) << 10);
            self->positions[i].y = self->positions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
    else {
        for (int32 i = 1; i < 8; ++i) {
            angle += UncurlPlant->array3[i];
            self->positions[i].x = self->positions[i - 1].x - (RSDK.Cos1024(angle) << 10);
            self->positions[i].y = self->positions[i - 1].y - (RSDK.Sin1024(angle) << 10);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void UncurlPlant_EditorDraw(void)
{
    RSDK_THIS(UncurlPlant);
    RSDK.SetSpriteAnimation(UncurlPlant->aniFrames, 1, &self->animator, false, 0);
    UncurlPlant_CalculateDrawPositions();

    for (int32 i = 0; i < 8; ++i) {
        RSDK.DrawSprite(&self->animator, &self->drawPositions[i], false);
    }
}

void UncurlPlant_EditorLoad(void) { UncurlPlant->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/Plants.bin", SCOPE_STAGE); }
#endif

void UncurlPlant_Serialize(void) { RSDK_EDITABLE_VAR(UncurlPlant, VAR_UINT8, direction); }
