// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSZSpotlight Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMSZSpotlight *MSZSpotlight;

void MSZSpotlight_Update(void)
{
    RSDK_THIS(MSZSpotlight);
    StateMachine_Run(self->state);
}

void MSZSpotlight_LateUpdate(void) {}

void MSZSpotlight_StaticUpdate(void) {}

void MSZSpotlight_Draw(void)
{
    RSDK_THIS(MSZSpotlight);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MSZSpotlight_Create(void *data)
{
    RSDK_THIS(MSZSpotlight);
    if (!SceneInfo->inEditor) {
        self->visible    = true;
        self->drawOrder  = Zone->drawOrderHigh;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->inkEffect  = INK_ADD;
        self->angle      = RSDK.Rand(0, 256);

        switch (self->color) {
            case MSZSPOTLIGHT_RED: self->moveFlag = -4; break;
            case MSZSPOTLIGHT_GREEN: self->moveFlag = 4; break;
            case MSZSPOTLIGHT_BLUE: self->moveFlag = 2; break;
        }

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(MSZSpotlight->aniFrames, 9, &self->animator, true, self->color);
    }
}

void MSZSpotlight_StageLoad(void)
{
    MSZSpotlight->aniFrames = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MSZSpotlight, color);
    RSDK.AddVarEnumValue("Red");
    RSDK.AddVarEnumValue("Green");
    RSDK.AddVarEnumValue("Blue");
}

void MSZSpotlight_Unknown1(void)
{
    RSDK_THIS(MSZSpotlight);
    self->angle      = ((self->angle & 0xFF) + self->moveFlag);
    self->position.x = (RSDK.Cos256(self->angle) << 13) + self->startPos.x;
    self->position.y = (RSDK.Sin256(self->angle) << 13) + self->startPos.y;

    if (self->alpha >= 0x100)
        self->state = MSZSpotlight_Unknown2;
    else
        self->alpha += 8;
}

void MSZSpotlight_Unknown2(void)
{
    RSDK_THIS(MSZSpotlight);

    self->angle      = ((self->angle & 0xFF) + self->moveFlag);
    self->position.x = (RSDK.Cos256(self->angle) << 13) + self->startPos.x;
    self->position.y = (RSDK.Sin256(self->angle) << 13) + self->startPos.y;

    if (++self->timer == 120) {
        self->timer = 0;
        self->state = MSZSpotlight_Unknown3;
    }
}

void MSZSpotlight_Unknown3(void)
{
    RSDK_THIS(MSZSpotlight);
    if (++self->timer == 30) {
        self->timer = 0;
        foreach_active(HeavyMystic, mystic)
        {
            if (mystic->type == MYSTIC_BOX) {
                self->startPos.x = mystic->position.x;
                self->startPos.y = mystic->position.y;
            }
        }
        self->velocity.x = (self->startPos.x - self->position.x) >> 4;
        self->velocity.y = ((self->startPos.y - self->position.y) >> 4);
        self->state      = MSZSpotlight_Unknown4;
    }
}

void MSZSpotlight_Unknown4(void)
{
    RSDK_THIS(MSZSpotlight);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    int rx = (self->startPos.x - self->position.x) >> 16;
    int ry = (self->startPos.y - self->position.y) >> 16;
    if (rx * rx + ry * ry < 16) {
        foreach_active(HeavyMystic, mystic)
        {
            if (mystic->type == MYSTIC_BOX && mystic->state != HeavyMystic_State2_Unknown4) {
                mystic->state = HeavyMystic_State2_Unknown4;
                RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
            }
        }
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->drawFX     = FX_SCALE;
        self->scale.x    = 0x200;
        self->scale.y    = 0x200;
        self->state      = MSZSpotlight_Unknown5;
    }
}

void MSZSpotlight_Unknown5(void)
{
    RSDK_THIS(MSZSpotlight);
    self->scale.x += 32;
    self->scale.y += 32;
    if (self->scale.x < 0x300)
        self->alpha = 0x100;

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 16;
}

#if RETRO_INCLUDE_EDITOR
void MSZSpotlight_EditorDraw(void)
{
    RSDK_THIS(MSZSpotlight);
    RSDK.SetSpriteAnimation(MSZSpotlight->aniFrames, 9, &self->animator, true, self->color);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MSZSpotlight_EditorLoad(void)
{
    MSZSpotlight->aniFrames = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MSZSpotlight, color);
    RSDK_ENUM_VAR("Red", MSZSPOTLIGHT_RED);
    RSDK_ENUM_VAR("Green", MSZSPOTLIGHT_GREEN);
    RSDK_ENUM_VAR("Blue", MSZSPOTLIGHT_BLUE);
}
#endif

void MSZSpotlight_Serialize(void) { RSDK_EDITABLE_VAR(MSZSpotlight, VAR_UINT8, color); }
