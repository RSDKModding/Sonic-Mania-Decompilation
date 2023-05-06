// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MSZSpotlight Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    RSDK.DrawSprite(&self->animatorSpotlight, NULL, false);
}

void MSZSpotlight_Create(void *data)
{
    RSDK_THIS(MSZSpotlight);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[1];
        self->startPos  = self->position;
        self->inkEffect = INK_ADD;
        self->angle     = RSDK.Rand(0, 256);

        switch (self->color) {
            case MSZSPOTLIGHT_RED: self->angleOffset = -4; break;
            case MSZSPOTLIGHT_GREEN: self->angleOffset = 4; break;
            case MSZSPOTLIGHT_BLUE: self->angleOffset = 2; break;
        }

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(MSZSpotlight->aniFrames, 9, &self->animatorSpotlight, true, self->color);
    }
}

void MSZSpotlight_StageLoad(void)
{
    MSZSpotlight->aniFrames = RSDK.LoadSpriteAnimation("MSZ/HeavyMystic.bin", SCOPE_STAGE);

    // Yeah, this is actually here in the original
    // This (& the one in FBZSetup) is how I found out about what these funcs do LOL
    RSDK_ACTIVE_VAR(MSZSpotlight, color);
    RSDK_ENUM_VAR("Red", MSZSPOTLIGHT_RED);
    RSDK_ENUM_VAR("Green", MSZSPOTLIGHT_GREEN);
    RSDK_ENUM_VAR("Blue", MSZSPOTLIGHT_BLUE);
}

void MSZSpotlight_State_Appear(void)
{
    RSDK_THIS(MSZSpotlight);

    self->angle      = (self->angle & 0xFF) + self->angleOffset;
    self->position.x = (RSDK.Cos256(self->angle) << 13) + self->startPos.x;
    self->position.y = (RSDK.Sin256(self->angle) << 13) + self->startPos.y;

    if (self->alpha >= 0x100)
        self->state = MSZSpotlight_State_Circling;
    else
        self->alpha += 8;
}

void MSZSpotlight_State_Circling(void)
{
    RSDK_THIS(MSZSpotlight);

    self->angle      = (self->angle & 0xFF) + self->angleOffset;
    self->position.x = (RSDK.Cos256(self->angle) << 13) + self->startPos.x;
    self->position.y = (RSDK.Sin256(self->angle) << 13) + self->startPos.y;

    if (++self->timer == 120) {
        self->timer = 0;
        self->state = MSZSpotlight_State_Idle;
    }
}

void MSZSpotlight_State_Idle(void)
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
        self->velocity.y = (self->startPos.y - self->position.y) >> 4;
        self->state      = MSZSpotlight_State_MoveToBox;
    }
}

void MSZSpotlight_State_MoveToBox(void)
{
    RSDK_THIS(MSZSpotlight);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 rx = (self->startPos.x - self->position.x) >> 16;
    int32 ry = (self->startPos.y - self->position.y) >> 16;

    if (rx * rx + ry * ry < 16) {
        foreach_active(HeavyMystic, mystic)
        {
            if (mystic->type == MYSTIC_BOX && mystic->state != HeavyMystic_StateBox_CloseDoors) {
                mystic->state = HeavyMystic_StateBox_CloseDoors;
                RSDK.PlaySfx(HeavyMystic->sfxClack, false, 255);
            }
        }

        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->drawFX     = FX_SCALE;
        self->scale.x    = 0x200;
        self->scale.y    = 0x200;
        self->state      = MSZSpotlight_State_Disappear;
    }
}

void MSZSpotlight_State_Disappear(void)
{
    RSDK_THIS(MSZSpotlight);

    self->scale.x += 0x20;
    self->scale.y += 0x20;

    if (self->scale.x < 0x300)
        self->alpha = 0x100;

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 0x10;
}

#if GAME_INCLUDE_EDITOR
void MSZSpotlight_EditorDraw(void)
{
    RSDK_THIS(MSZSpotlight);

    RSDK.SetSpriteAnimation(MSZSpotlight->aniFrames, 9, &self->animatorSpotlight, true, self->color);
    RSDK.DrawSprite(&self->animatorSpotlight, NULL, false);
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
