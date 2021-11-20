#include "SonicMania.h"

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

    switch (self->type) {
        case BSS_RING:
            self->drawFX    = FX_FLIP | FX_SCALE;
            self->scale.x   = BSS_Collectable->ringScaleTableX[self->frameID];
            self->scale.y   = BSS_Collectable->ringScaleTableY[self->frameID];
            self->direction = BSS_Collectable->sphereData[self->type].frameID > 8;
            drawPos.x         = self->position.x;
            drawPos.y         = self->position.y;
            drawPos.y -= BSS_Collectable->screenYValues[self->frameID];
            RSDK.DrawSprite(&BSS_Collectable->sphereData[self->type], &drawPos, true);
            self->drawFX = FX_NONE;
            return;
        case BSS_RING_SPARKLE: RSDK.DrawSprite(&BSS_Collectable->sphereData[self->type], NULL, true); break;
        case BSS_EMERALD_CHAOS:
        case BSS_EMERALD_SUPER:
            BSS_Collectable->sphereData[self->type].frameID = self->frameID >> 1;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[self->type], NULL, true);
            break;
        case BSS_MEDAL_SILVER:
        case BSS_MEDAL_GOLD:
            self->drawFX  = FX_SCALE;
            self->scale.x = BSS_Collectable->medalScaleTable[self->frameID];
            self->scale.y = BSS_Collectable->medalScaleTable[self->frameID];
            drawPos.x       = self->position.x;
            drawPos.y       = self->position.y;
            drawPos.y -= BSS_Collectable->screenYValues[self->frameID];
            RSDK.DrawSprite(&BSS_Collectable->sphereData[self->type], &drawPos, true);
            self->drawFX = FX_NONE;
            break;
        case BSS_SPHERE_GREEN_STOOD:
            BSS_Collectable->sphereData[BSS_SPHERE_GREEN].frameID = self->frameID;
            self->alpha                                         = 0x80;
            self->inkEffect                                     = INK_ALPHA;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[BSS_SPHERE_GREEN], NULL, true);
            self->inkEffect = INK_NONE;
            break;
        case BSS_BLUE_STOOD:
            BSS_Collectable->sphereData[BSS_SPHERE_BLUE].frameID = self->frameID;
            self->alpha                                        = 0x80;
            self->inkEffect                                    = INK_ALPHA;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[BSS_SPHERE_BLUE], NULL, true);
            self->inkEffect = INK_NONE;
            break;
        case BSS_SPHERE_PINK_STOOD:
            BSS_Collectable->sphereData[BSS_SPHERE_PINK].frameID = self->frameID;
            self->alpha                                        = 0x80;
            self->inkEffect                                    = INK_ALPHA;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[BSS_SPHERE_PINK], NULL, true);
            self->inkEffect = INK_NONE;
            break;
        default:
            BSS_Collectable->sphereData[self->type].frameID = self->frameID;
            RSDK.DrawSprite(&BSS_Collectable->sphereData[self->type], NULL, true);
            break;
    }
}

void BSS_Collectable_Create(void *data)
{
    RSDK_THIS(BSS_Collectable);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->drawOrder     = 3;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        for (int32 i = 0; i < 8; ++i)
            RSDK.SetSpriteAnimation(BSS_Collectable->aniFrames, i, &BSS_Collectable->sphereData[i + 1], true, 0);
        // RSDK.SetSpriteAnimation(BSS_Collectable->textSpriteIndex, 0, (Animator *)&BSS_Collectable[2].ringScaleTableX[21], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->ringSpriteIndex, 0, &BSS_Collectable->sphereData[7], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->ringSpriteIndex, 1, &BSS_Collectable->sphereData[15], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->aniFrames, 6, &BSS_Collectable->sphereData[16], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->aniFrames, 7, &BSS_Collectable->sphereData[17], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->aniFrames, 8, &BSS_Collectable->sphereData[18], true, 0);
        RSDK.SetSpriteAnimation(BSS_Collectable->aniFrames, 9, &BSS_Collectable->sphereData[19], true, 0);
    }
}

void BSS_Collectable_StageLoad(void)
{
    BSS_Collectable->aniFrames     = RSDK.LoadSpriteAnimation("SpecialBS/StageObjects.bin", SCOPE_STAGE);
    BSS_Collectable->ringSpriteIndex = RSDK.LoadSpriteAnimation("SpecialBS/Ring.bin", SCOPE_STAGE);

    if (!BSS_Collectable->loadedTables) {
        BSS_Collectable->loadedTables = true;

        int32 id = 0x20;
        for (int32 i = 0; i < 0x20; ++i) {
            BSS_Collectable->ringScaleTableX[i] *= 14;
            BSS_Collectable->ringScaleTableY[i] *= 14;
            BSS_Collectable->medalScaleTable[i] *= 16;
            BSS_Collectable->screenYValues[i]    = id * (BSS_Collectable->ringScaleTableY[i] << 6);
            BSS_Collectable->medalScreenYVals[i] = id * (BSS_Collectable->medalScaleTable[i] << 6);

            int32 scale                           = i * (BSS_Collectable->ringScaleTableY[i] - BSS_Collectable->ringScaleTableX[i]);
            int32 sx                              = BSS_Collectable->ringScaleTableX[i];
            BSS_Collectable->ringScaleTableY[i] = sx + (scale >> 5);

            --id;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void BSS_Collectable_EditorDraw(void) {}

void BSS_Collectable_EditorLoad(void) {}
#endif

void BSS_Collectable_Serialize(void) {}
