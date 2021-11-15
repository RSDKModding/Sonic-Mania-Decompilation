#include "SonicMania.h"

ObjectMSZSpotlight *MSZSpotlight;

void MSZSpotlight_Update(void)
{
    RSDK_THIS(MSZSpotlight);
    StateMachine_Run(entity->state);
}

void MSZSpotlight_LateUpdate(void) {}

void MSZSpotlight_StaticUpdate(void) {}

void MSZSpotlight_Draw(void)
{
    RSDK_THIS(MSZSpotlight);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MSZSpotlight_Create(void *data)
{
    RSDK_THIS(MSZSpotlight);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible    = true;
        entity->drawOrder  = Zone->drawOrderHigh;
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
        entity->inkEffect  = INK_ADD;
        entity->angle      = RSDK.Rand(0, 256);

        switch (entity->color) {
            case MSZSPOTLIGHT_RED: entity->moveFlag = -4; break;
            case MSZSPOTLIGHT_GREEN: entity->moveFlag = 4; break;
            case MSZSPOTLIGHT_BLUE: entity->moveFlag = 2; break;
        }

        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(MSZSpotlight->spriteIndex, 9, &entity->animator, true, entity->color);
    }
}

void MSZSpotlight_StageLoad(void)
{
    MSZSpotlight->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MSZSpotlight, color);
    RSDK.AddVarEnumValue("Red");
    RSDK.AddVarEnumValue("Green");
    RSDK.AddVarEnumValue("Blue");
}

void MSZSpotlight_Unknown1(void)
{
    RSDK_THIS(MSZSpotlight);
    entity->angle      = ((entity->angle & 0xFF) + entity->moveFlag);
    entity->position.x = (RSDK.Cos256(entity->angle) << 13) + entity->startPos.x;
    entity->position.y = (RSDK.Sin256(entity->angle) << 13) + entity->startPos.y;

    if (entity->alpha >= 0x100)
        entity->state = MSZSpotlight_Unknown2;
    else
        entity->alpha += 8;
}

void MSZSpotlight_Unknown2(void)
{
    RSDK_THIS(MSZSpotlight);

    entity->angle      = ((entity->angle & 0xFF) + entity->moveFlag);
    entity->position.x = (RSDK.Cos256(entity->angle) << 13) + entity->startPos.x;
    entity->position.y = (RSDK.Sin256(entity->angle) << 13) + entity->startPos.y;

    if (++entity->timer == 120) {
        entity->timer = 0;
        entity->state = MSZSpotlight_Unknown3;
    }
}

void MSZSpotlight_Unknown3(void)
{
    RSDK_THIS(MSZSpotlight);
    if (++entity->timer == 30) {
        entity->timer = 0;
        foreach_active(HeavyMystic, mystic)
        {
            if (mystic->type == MYSTIC_BOX) {
                entity->startPos.x = mystic->position.x;
                entity->startPos.y = mystic->position.y;
            }
        }
        entity->velocity.x = (entity->startPos.x - entity->position.x) >> 4;
        entity->velocity.y = ((entity->startPos.y - entity->position.y) >> 4);
        entity->state      = MSZSpotlight_Unknown4;
    }
}

void MSZSpotlight_Unknown4(void)
{
    RSDK_THIS(MSZSpotlight);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    int rx = (entity->startPos.x - entity->position.x) >> 16;
    int ry = (entity->startPos.y - entity->position.y) >> 16;
    if (rx * rx + ry * ry < 16) {
        foreach_active(HeavyMystic, mystic)
        {
            if (mystic->type == MYSTIC_BOX && mystic->state != HeavyMystic_State2_Unknown4) {
                mystic->state = HeavyMystic_State2_Unknown4;
                RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
            }
        }
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->drawFX     = FX_SCALE;
        entity->scale.x    = 0x200;
        entity->scale.y    = 0x200;
        entity->state      = MSZSpotlight_Unknown5;
    }
}

void MSZSpotlight_Unknown5(void)
{
    RSDK_THIS(MSZSpotlight);
    entity->scale.x += 32;
    entity->scale.y += 32;
    if (entity->scale.x < 0x300)
        entity->alpha = 0x100;

    if (entity->alpha <= 0)
        destroyEntity(entity);
    else
        entity->alpha -= 16;
}

#if RETRO_INCLUDE_EDITOR
void MSZSpotlight_EditorDraw(void)
{
    RSDK_THIS(MSZSpotlight);
    RSDK.SetSpriteAnimation(MSZSpotlight->spriteIndex, 9, &entity->animator, true, entity->color);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MSZSpotlight_EditorLoad(void)
{
    MSZSpotlight->spriteIndex = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MSZSpotlight, color);
    RSDK_ENUM_VAR("Red", MSZSPOTLIGHT_RED);
    RSDK_ENUM_VAR("Green", MSZSPOTLIGHT_GREEN);
    RSDK_ENUM_VAR("Blue", MSZSPOTLIGHT_BLUE);
}
#endif

void MSZSpotlight_Serialize(void) { RSDK_EDITABLE_VAR(MSZSpotlight, VAR_UINT8, color); }
