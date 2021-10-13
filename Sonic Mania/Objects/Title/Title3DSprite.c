#include "SonicMania.h"

ObjectTitle3DSprite *Title3DSprite;

void Title3DSprite_Update(void)
{
    RSDK_THIS(Title3DSprite);
    int32 valA         = (entity->position.y >> 8) * RSDK.Sin1024(TitleBG->angle);
    int32 valB         = (entity->position.y >> 8) * RSDK.Cos1024(TitleBG->angle);
    entity->field_5C = valA + (entity->position.x >> 8) * RSDK.Cos1024(TitleBG->angle);
    entity->field_60 = valB - (entity->position.x >> 8) * RSDK.Sin1024(TitleBG->angle);
    entity->field_60 >>= 10;
    entity->depth3D  = entity->field_60;
    entity->field_5C = -entity->field_5C >> 10;
}

void Title3DSprite_LateUpdate(void) {}

void Title3DSprite_StaticUpdate(void) {}

void Title3DSprite_Draw(void)
{
    RSDK_THIS(Title3DSprite);
    int32 depth = entity->depth3D + Title3DSprite->field_C;
    if (depth && depth >= 256) {
        int32 scale = 0x18000 * Title3DSprite->field_4 / depth;
        if (scale > 0x200)
            scale = 0x200;
        entity->scale.x = scale;
        entity->scale.y = scale;

        Vector2 drawPos;
        drawPos.x = (RSDK_screens->centerX + Title3DSprite->field_4 * entity->field_5C / depth) << 16;
        drawPos.y = (Title3DSprite->field_4 * Title3DSprite->field_8 / depth + 152) << 16;
        RSDK.DrawSprite(&entity->animator, &drawPos, true);
    }
}

void Title3DSprite_Create(void *data)
{
    RSDK_THIS(Title3DSprite);
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Title3DSprite->spriteIndex, 5, &entity->animator, true, entity->frame);
        entity->position.x -= 0x2000000;
        entity->position.y -= 0x2000000;
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = false;
        entity->drawOrder = 2;
        entity->drawFX    = FX_NONE;
    }
}

void Title3DSprite_StageLoad(void)
{
    Title3DSprite->spriteIndex = RSDK.LoadSpriteAnimation("Title/Background.bin", SCOPE_STAGE);
    Title3DSprite->field_4     = 0x90;
    Title3DSprite->field_8     = 0x2800;
    Title3DSprite->field_C     = 0xA000;
}

void Title3DSprite_EditorDraw(void) {}

void Title3DSprite_EditorLoad(void) {}

void Title3DSprite_Serialize(void) { RSDK_EDITABLE_VAR(Title3DSprite, VAR_ENUM, frame); }
