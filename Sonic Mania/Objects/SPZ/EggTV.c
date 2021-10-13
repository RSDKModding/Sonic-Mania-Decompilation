#include "SonicMania.h"

ObjectEggTV *EggTV;

void EggTV_Update(void)
{
    RSDK_THIS(EggTV);
    if (entity->lineAlpha < 96)
        entity->lineAlpha += 4;
}

void EggTV_LateUpdate(void) {}

void EggTV_StaticUpdate(void) { RSDK.ProcessAnimation(&EggTV->animator); }

void EggTV_Draw(void)
{
    EggTV_DrawTV();
    if (globals->gameMode != MODE_COMPETITION)
        EggTV_DrawScanlines();
}

void EggTV_Create(void *data)
{
    RSDK_THIS(EggTV);
    
    entity->active = ACTIVE_BOUNDS;
    if (entity->drawOverTV)
        entity->drawOrder = Zone->drawOrderLow;
    else
        entity->drawOrder = Zone->fgLayerLow + 1;

    entity->visible       = true;
    entity->drawFX        = FX_NONE;
    entity->updateRange.x = 0x800000 + entity->size.x;
    entity->updateRange.y = 0x800000 + entity->size.y;

    entity->hitbox.left   = -(entity->size.x >> 17);
    entity->hitbox.top    = -(entity->size.y >> 17);
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.bottom = entity->size.x >> 17;
}

void EggTV_StageLoad(void)
{
    EggTV->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/EggTV.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EggTV->aniFrames, 0, &EggTV->animator, true, 0);
}

void EggTV_DrawScanlines(void)
{
    RSDK_THIS(EggTV);

    int32 y = (((Zone->timer >> 1) & 1) << 16) - (entity->size.y >> 1) + entity->position.y;
    if (entity->size.y >= 0 && (entity->size.y & 0xFFFF0000)) {
        for (int32 i = 0; i < (entity->size.y >> 16); i += 2) {
            RSDK.DrawLine(entity->position.x - (entity->size.x >> 1), y, entity->position.x + (entity->size.x >> 1), y, 0x404060,
                          entity->lineAlpha, INK_BLEND, false);
            y += 0x20000;
        }
    }
}
void EggTV_DrawTV(void)
{
    RSDK_THIS(EggTV);
    entity->alpha     = 255;
    entity->drawFX    = FX_SCALE;
    entity->inkEffect = INK_NONE;
    entity->scale.x   = ((entity->size.x >> 16) << 9) / 96;
    entity->scale.y   = ((entity->size.y >> 16) << 9) / 96;
    RSDK.DrawSprite(&EggTV->animator, NULL, false);
    entity->drawFX = FX_NONE;
}

void EggTV_EditorDraw(void) {}

void EggTV_EditorLoad(void) {}

void EggTV_Serialize(void)
{
    RSDK_EDITABLE_VAR(EggTV, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(EggTV, VAR_BOOL, drawOverTV);
}
