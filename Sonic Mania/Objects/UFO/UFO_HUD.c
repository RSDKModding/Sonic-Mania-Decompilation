#include "SonicMania.h"

ObjectUFO_HUD *UFO_HUD;

void UFO_HUD_Update(void)
{
    RSDK_THIS(UFO_HUD);
#if RETRO_USE_PLUS && RETRO_GAMEVER != VER_107
    if (ControllerInfo->keyY.press)
        UFO_HUD_LevelUpMach();
#endif

    if (self->scale.x > 512) {
        self->scale.x -= 0x10;
        self->scale.y -= 0x10;
        if (self->scale.x <= 512) {
            int32 cnt = 32 * UFO_Setup->machPoints / UFO_Setup->machQuotas[UFO_Setup->machLevel];
            RSDK.CopyPalette(self->palID, 96, 0, 96, cnt);
            RSDK.CopyPalette(1, cnt + 96, 0, cnt + 96, (uint8)(32 - cnt));
        }
        else {
            RSDK.SetLimitedFade(0, 1, 4, self->scale.x - 0x200, 96, 127);
        }
    }

    if (self->timer > 0)
        self->timer--;

    if (UFO_Setup->rings <= 0 || UFO_Setup->rings >= 10)
        self->showRingCount = true;
    else
        self->showRingCount = (UFO_Setup->timer >> 3) & 1;
}

void UFO_HUD_LateUpdate(void) {}

void UFO_HUD_StaticUpdate(void) {}

void UFO_HUD_Draw(void)
{
    RSDK_THIS(UFO_HUD);

    Vector2 drawPos;
    drawPos.y = 0x240000;
    drawPos.x = ScreenInfo->centerX << 16;
    if (self->scale.x > 0x200)
        self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->animator1, &drawPos, true);

    for (int32 i = 0; i <= UFO_Setup->machLevel; ++i) {
        RSDK.DrawSprite(&self->animator4, &drawPos, true);
        drawPos.x += 0xA0000;
    }

    drawPos.x = ScreenInfo->centerX << 16;
    if (!(self->timer & 4) && self->timer) {
        self->animator3.frameID = 3;
        RSDK.DrawSprite(&self->animator3, &drawPos, true);
    }
    else if (!self->timer) {
        self->animator3.frameID = UFO_Setup->machLevel;
        RSDK.DrawSprite(&self->animator3, &drawPos, true);
    }

    self->drawFX = FX_NONE;
    if (self->showRingCount) {
        drawPos.x += 0x200000;
        drawPos.y = 0x250000;
        UFO_HUD_DrawNumbers(&drawPos, UFO_Setup->rings);
    }
}

void UFO_HUD_Create(void *data)
{
    RSDK_THIS(UFO_HUD);
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_NORMAL;
        self->visible       = true;
        self->drawOrder     = 12;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->palID         = 2;
        RSDK.SetSpriteAnimation(UFO_HUD->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(UFO_HUD->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(UFO_HUD->aniFrames, 2, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(UFO_HUD->aniFrames, 3, &self->animator4, true, 0);
    }
}

void UFO_HUD_StageLoad(void)
{
    UFO_HUD->aniFrames = RSDK.LoadSpriteAnimation("SpecialUFO/HUD.bin", SCOPE_STAGE);
    RSDK.ResetEntitySlot(SLOT_UFO_HUD, UFO_HUD->objectID, NULL);
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
        RSDK.PlaySfx(UFO_Sphere->sfxMachSpeed, 0, 255);
    }

    if (hud->scale.x == 512) {
        int32 cnt = 32 * UFO_Setup->machPoints / UFO_Setup->machQuotas[UFO_Setup->machLevel];
        RSDK.CopyPalette(hud->palID, 96, 0, 96, cnt);
        RSDK.CopyPalette(1, cnt + 96, 0, cnt + 96, (uint8)(32 - cnt));
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
    RSDK.PlaySfx(UFO_Sphere->sfxMachSpeed, false, 255);
}

void UFO_HUD_DrawNumbers(Vector2 *drawPos, int32 value)
{
    RSDK_THIS(UFO_HUD);

    int32 mult = 1;
    for (int32 i = 0; i < 3; ++i) {
        self->animator2.frameID = value / mult % 10;
        RSDK.DrawSprite(&self->animator2, drawPos, true);
        drawPos->x -= 0x100000;
        mult *= 10;
    }
}

#if RETRO_INCLUDE_EDITOR
void UFO_HUD_EditorDraw(void) {}

void UFO_HUD_EditorLoad(void) {}
#endif

void UFO_HUD_Serialize(void) {}
