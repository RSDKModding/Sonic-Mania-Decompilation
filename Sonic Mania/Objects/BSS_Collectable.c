#include "../SonicMania.h"

ObjectBSS_Collectable *BSS_Collectable;

void BSS_Collectable_Update(void) {}

void BSS_Collectable_LateUpdate(void) {}

void BSS_Collectable_StaticUpdate(void)
{
    RSDK.ProcessAnimation(&BSS_Collectable->sphereData[BSS_RING]);
    RSDK.ProcessAnimation(&BSS_Collectable->sphereData[BSS_RING_SPARKLE]);
    RSDK.ProcessAnimation(&BSS_Collectable->sphereData[BSS_MEDAL_SILVER]);
    RSDK.ProcessAnimation(&BSS_Collectable->sphereData[BSS_MEDAL_GOLD]);
}

void BSS_Collectable_Draw(void)
{
    RSDK_THIS(BSS_Collectable);
    Vector2 drawPos;

    switch (entity->type) {
        case BSS_RING:
            entity->drawFX    = FX_SCALE | FX_SCALE;
            entity->scale.x   = BSS_Collectable->scaleXTable[entity->frameID];
            entity->scale.y   = BSS_Collectable->scaleYTable[entity->frameID];
            entity->direction = BSS_Collectable->sphereData[entity->type].frameID > 8;
            drawPos.x         = entity->position.x;
            drawPos.y         = entity->position.y;
            drawPos.y -= BSS_Collectable->screenYValues[entity->frameID];
            RSDK.DrawSprite(&BSS_Collectable->sphereData[entity->type], &drawPos, true);
            entity->drawFX = FX_NONE;
            return;
        case BSS_RING_SPARKLE: RSDK.DrawSprite(&BSS_Collectable->sphereData[entity->type], NULL, true); break;
        case BSS_EMERALD_CHAOS:
        case BSS_EMERALD_SUPER:
            BSS_Collectable->sphereData[entity->type].frameID = entity->frameID >> 1;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[entity->type], NULL, true);
            break;
        case BSS_MEDAL_SILVER:
        case BSS_MEDAL_GOLD:
            entity->drawFX  = FX_SCALE;
            entity->scale.x = BSS_Collectable->scaleTable2[entity->frameID];
            entity->scale.y = BSS_Collectable->scaleTable2[entity->frameID];
            drawPos.x       = entity->position.x;
            drawPos.y       = entity->position.y;
            drawPos.y -= BSS_Collectable->screenYValues[entity->frameID];
            RSDK.DrawSprite(&BSS_Collectable->sphereData[entity->type], &drawPos, true);
            entity->drawFX = FX_NONE;
            break;
        case 129:
            BSS_Collectable->sphereData[BSS_SPHERE_GREEN].frameID = entity->frameID;
            entity->alpha                          = 0x80;
            entity->inkEffect                      = INK_ALPHA;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[BSS_SPHERE_GREEN], NULL, true);
            entity->inkEffect = INK_NONE;
            break;
        case 130:
            BSS_Collectable->sphereData[BSS_SPHERE_BLUE].frameID = entity->frameID;
            entity->alpha                          = 0x80;
            entity->inkEffect                      = INK_ALPHA;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[BSS_SPHERE_BLUE], NULL, true);
            entity->inkEffect = INK_NONE;
            break;
        case 134:
            BSS_Collectable->sphereData[BSS_SPHERE_PINK].frameID = entity->frameID;
            entity->alpha                          = 0x80;
            entity->inkEffect                      = INK_ALPHA;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[BSS_SPHERE_PINK], NULL, true);
            entity->inkEffect = INK_NONE;
            break;
        default:
            BSS_Collectable->sphereData[entity->type].frameID = entity->frameID;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[entity->type], NULL, true);
            break;
    }
}

void BSS_Collectable_Create(void *data)
{
    RSDK_THIS(BSS_Collectable);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->visible       = true;
        entity->drawOrder     = 3;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;

        for (int i = 0; i < 8; ++i) {
            RSDK.SetSpriteAnimation(BSS_Collectable->spriteIndex, i, &BSS_Collectable->sphereData[i + 1], true, 0);
        }
        // RSDK.SetSpriteAnimation(BSS_Collectable->spriteIndex, 0, (AnimationData *)&BSS_Collectable[2].scaleXTable[21], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->ringSpriteIndex, 0, &BSS_Collectable->sphereData[7], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->ringSpriteIndex, 1, &BSS_Collectable->sphereData[15], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->spriteIndex, 6, &BSS_Collectable->sphereData[16], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->spriteIndex, 7, &BSS_Collectable->sphereData[17], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->spriteIndex, 8, &BSS_Collectable->sphereData[18], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->spriteIndex, 9, &BSS_Collectable->sphereData[19], true, 0);
    }
}

void BSS_Collectable_StageLoad(void)
{
    BSS_Collectable->spriteIndex     = RSDK.LoadSpriteAnimation("SpecialBS/StageObjects.bin", SCOPE_STAGE);
    BSS_Collectable->ringSpriteIndex = RSDK.LoadSpriteAnimation("SpecialBS/Ring.bin", SCOPE_STAGE);

    if (!BSS_Collectable->loadedTables) {
        BSS_Collectable->loadedTables = true;

        int id = 0x20;
        for (int i = 0; i < 0x20; ++i) {
            BSS_Collectable->scaleXTable[i] *= 14;
            BSS_Collectable->scaleYTable[i] *= 14;
            BSS_Collectable->scaleTable2[i] *= 16;
            BSS_Collectable->screenYValues[i] = id * (BSS_Collectable->scaleYTable[i] << 6);
            BSS_Collectable->screenYValues[i] = id * (BSS_Collectable->scaleTable2[i] << 6);

            int scale                       = i * (BSS_Collectable->scaleYTable[i] - BSS_Collectable->scaleXTable[i]);
            int sx                          = BSS_Collectable->scaleXTable[i];
            BSS_Collectable->scaleYTable[i] = sx + (scale >> 5);

            --id;
        }
    }
}

void BSS_Collectable_EditorDraw(void) {}

void BSS_Collectable_EditorLoad(void) {}

void BSS_Collectable_Serialize(void) {}
