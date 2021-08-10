#include "SonicMania.h"

ObjectUFO_HUD *UFO_HUD;

void UFO_HUD_Update(void)
{
    RSDK_THIS(UFO_HUD);
#if RETRO_USE_PLUS && RETRO_GAMEVER != VER_107
    if (RSDK_controller->keyY.press)
        UFO_HUD_LevelUpMach();
#endif

    if (entity->scale.x > 512) {
        entity->scale.x -= 0x10;
        entity->scale.y -= 0x10;
        if (entity->scale.x <= 512) {
            int cnt = 32 * UFO_Setup->machPoints / UFO_Setup->machQuotas[UFO_Setup->machLevel];
            RSDK.CopyPalette(entity->palID, 96, 0, 96, cnt);
            RSDK.CopyPalette(1, cnt + 96, 0, cnt + 96, (byte)(32 - cnt));
        }
        else {
            RSDK.SetLimitedFade(0, 1, 4, entity->scale.x - 0x200, 96, 127);
        }
    }

    if (entity->timer > 0)
        entity->timer--;

    if (UFO_Setup->rings <= 0 || UFO_Setup->rings >= 10)
        entity->field_58 = 1;
    else
        entity->field_58 = (UFO_Setup->timer >> 3) & 1;
}

void UFO_HUD_LateUpdate(void) {}

void UFO_HUD_StaticUpdate(void) {}

void UFO_HUD_Draw(void)
{
    RSDK_THIS(UFO_HUD);

    Vector2 drawPos;
    drawPos.y = 0x240000;
    drawPos.x = RSDK_screens->centerX << 16;
    if (entity->scale.x > 0x200)
        entity->drawFX = FX_SCALE;
    RSDK.DrawSprite(&entity->data1, &drawPos, true);

    for (int i = 0; i <= UFO_Setup->machLevel; ++i) {
        RSDK.DrawSprite(&entity->data4, &drawPos, true);
        drawPos.x += 0xA0000;
    }

    drawPos.x = RSDK_screens->centerX << 16;
    if (!(entity->timer & 4) && entity->timer) {
        entity->data3.frameID = 3;
        RSDK.DrawSprite(&entity->data3, &drawPos, true);
    }
    else if (!entity->timer) {
        entity->data3.frameID = UFO_Setup->machLevel;
        RSDK.DrawSprite(&entity->data3, &drawPos, true);
    }

    entity->drawFX = FX_NONE;
    if (entity->field_58 == 1) {
        drawPos.x += 0x200000;
        drawPos.y = 0x250000;
        UFO_HUD_DrawNumbers(&drawPos, UFO_Setup->rings);
    }
}

void UFO_HUD_Create(void *data)
{
    RSDK_THIS(UFO_HUD);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_NORMAL;
        entity->visible       = true;
        entity->drawOrder     = 12;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->palID         = 2;
        RSDK.SetSpriteAnimation(UFO_HUD->spriteIndex, 0, &entity->data1, true, 0);
        RSDK.SetSpriteAnimation(UFO_HUD->spriteIndex, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(UFO_HUD->spriteIndex, 2, &entity->data3, true, 0);
        RSDK.SetSpriteAnimation(UFO_HUD->spriteIndex, 3, &entity->data4, true, 0);
    }
}

void UFO_HUD_StageLoad(void)
{
    UFO_HUD->spriteIndex = RSDK.LoadSpriteAnimation("SpecialUFO/HUD.bin", SCOPE_STAGE);
    RSDK.ResetEntitySlot(SLOT_UFO_HUD, UFO_HUD->objectID, 0);
}

void UFO_HUD_CheckLevelUp(void)
{
    EntityUFO_HUD *hud = RSDK_GET_ENTITY(SLOT_UFO_HUD, UFO_HUD);
    if (UFO_Setup->machPoints >= UFO_Setup->machQuotas[UFO_Setup->machLevel]) {
        UFO_Setup->machPoints -= UFO_Setup->machQuotas[UFO_Setup->machLevel];
        if (UFO_Setup->machLevel >= 2) {
            ++UFO_Setup->scoreBonus;
            hud->timer = 60;
        }
        else {
            UFO_Setup->machLevel++;
            if (UFO_Setup->machLevel == 2)
                hud->palID = 3;
        }
        UFO_Player_ChangeMachState();
        hud->scale.x = 768;
        hud->scale.y = 768;
        RSDK.PlaySFX(UFO_Sphere->sfx_MachSpeed, 0, 255);
    }

    if (hud->scale.x == 512) {
        int cnt = 32 * UFO_Setup->machPoints / UFO_Setup->machQuotas[UFO_Setup->machLevel];
        RSDK.CopyPalette(hud->palID, 96, 0, 96, cnt);
        RSDK.CopyPalette(1, cnt + 96, 0, cnt + 96, (byte)(32 - cnt));
    }
}

void UFO_HUD_LevelUpMach(void)
{
    EntityUFO_HUD *hud    = RSDK_GET_ENTITY(SLOT_UFO_HUD, UFO_HUD);
    UFO_Setup->machPoints = 0;

    if (UFO_Setup->machLevel >= 2) {
        ++UFO_Setup->scoreBonus;
        hud->timer = 60;
    }
    else {
        UFO_Setup->machLevel++;
        if (UFO_Setup->machLevel == 2)
            hud->palID = 3;
    }
    UFO_Player_ChangeMachState();
    hud->scale.x = 0x300;
    hud->scale.y = 0x300;
    RSDK.PlaySFX(UFO_Sphere->sfx_MachSpeed, 0, 255);
}

void UFO_HUD_DrawNumbers(Vector2 *drawPos, int value)
{
    RSDK_THIS(UFO_HUD);

    int mult = 1;
    for (int i = 0; i < 3; ++i) {
        entity->animator2.frameID = value / mult % 10;
        RSDK.DrawSprite(&entity->animator2, drawPos, true);
        drawPos->x -= 0x100000;
        mult *= 10;
    }
}

void UFO_HUD_EditorDraw(void) {}

void UFO_HUD_EditorLoad(void) {}

void UFO_HUD_Serialize(void) {}
