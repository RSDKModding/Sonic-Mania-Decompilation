#include "../SonicMania.h"

ObjectUFO_Dust *UFO_Dust;

void UFO_Dust_Update(void) {}

void UFO_Dust_LateUpdate(void)
{
    RSDK_THIS(UFO_Dust);

    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
    else {
        int x = entity->position.x >> 8;
        int y = entity->height >> 8;
        int z = entity->position.y >> 8;

        Matrix *mat = &UFO_Camera->matWorld;

        entity->worldX = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
        entity->worldY = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
        entity->depth  = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
    }
}

void UFO_Dust_StaticUpdate(void) {}

void UFO_Dust_Draw(void)
{
    RSDK_THIS(UFO_Dust);

    if (entity->depth >= 1024) {
        Vector2 drawPos;
        drawPos.x = (RSDK_screens->centerX + (entity->worldX << 8) / entity->depth) << 16;
        drawPos.y = (RSDK_screens->centerY - (entity->worldY << 8) / entity->depth) << 16;

        entity->scale.x = 0x1000000 / entity->depth;
        entity->scale.y = 0x1000000 / entity->depth;
        RSDK.DrawSprite(&entity->data, &drawPos, true);
    }
}

void UFO_Dust_Create(void *data)
{
    RSDK_THIS(UFO_Dust);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawFX        = FX_SCALE | FX_FLIP;
        entity->drawOrder     = 4;
        entity->active        = ACTIVE_RBOUNDS;
        entity->updateRange.x = 0x400;
        entity->updateRange.y = 0x400;
        entity->position.x += RSDK.Rand(-0x80000, 0x80000);
        entity->position.y += RSDK.Rand(-0x80000, 0x80000);
        entity->height = RSDK.Rand(0x40000, 0x100000);
        RSDK.SetSpriteAnimation(UFO_Dust->spriteIndex, 0, &entity->data, true, 0);
    }
}

void UFO_Dust_StageLoad(void) { UFO_Dust->spriteIndex = RSDK.LoadSpriteAnimation("SpecialUFO/Dust.bin", SCOPE_STAGE); }

void UFO_Dust_EditorDraw(void) {}

void UFO_Dust_EditorLoad(void) {}

void UFO_Dust_Serialize(void) { RSDK_EDITABLE_VAR(UFO_Dust, VAR_ENUM, height); }
