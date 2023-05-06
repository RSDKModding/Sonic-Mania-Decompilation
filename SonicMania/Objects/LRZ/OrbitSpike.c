// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: OrbitSpike Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectOrbitSpike *OrbitSpike;

void OrbitSpike_Update(void)
{
    RSDK_THIS(OrbitSpike);

    uint8 angle = 2 * ((self->offset & 0xFF) + (Zone->timer & 0xFF));

    if (self->amplitude.x) {
        if (angle >= 0x80) {
            self->drawGroup = Zone->objectDrawGroup[0];
            self->scale.x   = 386 + (abs(angle - 0xC0) << 1);
            self->scale.y   = 386 + (abs(angle - 0xC0) << 1);
        }
        else {
            self->drawGroup = Zone->objectDrawGroup[1];
            self->scale.x   = (319 - abs(angle - 0x40)) << 1;
            self->scale.y   = (319 - abs(angle - 0x40)) << 1;
        }
    }
    else {
        int32 newAngle = (angle + 0x40) & 0xFF;
        if (newAngle >= 0x80) {
            self->drawGroup = Zone->objectDrawGroup[0];
            self->scale.x   = 386 + (abs(newAngle - 0xC0) << 1);
            self->scale.y   = 386 + (abs(newAngle - 0xC0) << 1);
        }
        else {
            self->drawGroup = Zone->objectDrawGroup[1];
            self->scale.x   = (319 - abs(newAngle - 0x40)) << 1;
            self->scale.y   = (319 - abs(newAngle - 0x40)) << 1;
        }
    }

    self->position.x = (self->amplitude.x >> 8) * RSDK.Cos256(angle) + self->center.x;
    self->position.y = (self->amplitude.y >> 8) * RSDK.Sin256(angle) + self->center.y;

    if (self->drawGroup == Zone->objectDrawGroup[1]) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &OrbitSpike->hitbox)) {
#if MANIA_USE_PLUS
                if (!Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer))
#endif
                    Player_Hurt(player, self);
            }
        }
    }
}

void OrbitSpike_LateUpdate(void) {}

void OrbitSpike_StaticUpdate(void) {}

void OrbitSpike_Draw(void)
{
    RSDK_THIS(OrbitSpike);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void OrbitSpike_Create(void *data)
{
    RSDK_THIS(OrbitSpike);

    if (!SceneInfo->inEditor) {
        self->center        = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawFX        = FX_SCALE;

        RSDK.SetSpriteAnimation(OrbitSpike->aniFrames, 0, &self->animator, true, 0);
        self->drawGroup = Zone->objectDrawGroup[1];
    }
}

void OrbitSpike_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        OrbitSpike->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/OrbitSpike.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2"))
        OrbitSpike->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/OrbitSpike.bin", SCOPE_STAGE);

    OrbitSpike->hitbox.left   = -12;
    OrbitSpike->hitbox.top    = -12;
    OrbitSpike->hitbox.right  = 12;
    OrbitSpike->hitbox.bottom = 12;
}

#if GAME_INCLUDE_EDITOR
void OrbitSpike_EditorDraw(void)
{
    RSDK_THIS(OrbitSpike);

    self->center        = self->position;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawFX        = FX_SCALE;
    RSDK.SetSpriteAnimation(OrbitSpike->aniFrames, 0, &self->animator, true, 0);

    self->inkEffect = INK_NONE;
    self->scale.x   = 0x200;
    self->scale.y   = 0x200;

    OrbitSpike_Draw();

    if (showGizmos()) {
        // Offset render
        uint8 angle = 2 * (self->offset & 0xFF);
        if (self->amplitude.x) {
            if (angle >= 0x80) {
                self->scale.x = 386 + (abs(angle - 0xC0) << 1);
                self->scale.y = 386 + (abs(angle - 0xC0) << 1);
            }
            else {
                self->scale.x = (319 - abs(angle - 0x40)) << 1;
                self->scale.y = (319 - abs(angle - 0x40)) << 1;
            }
        }
        else {
            int32 newAngle = (angle + 0x40) & 0xFF;
            if (newAngle >= 0x80) {
                self->scale.x = 386 + (abs(newAngle - 0xC0) << 1);
                self->scale.y = 386 + (abs(newAngle - 0xC0) << 1);
            }
            else {
                self->scale.x = (319 - abs(newAngle - 0x40)) << 1;
                self->scale.y = (319 - abs(newAngle - 0x40)) << 1;
            }
        }

        self->position.x = (self->amplitude.x >> 8) * RSDK.Cos256(angle) + self->center.x;
        self->position.y = (self->amplitude.y >> 8) * RSDK.Sin256(angle) + self->center.y;

        self->inkEffect = INK_ALPHA;
        self->alpha     = 0x40;
        OrbitSpike_Draw();

        self->position = self->center;
    }
}

void OrbitSpike_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        OrbitSpike->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/OrbitSpike.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2"))
        OrbitSpike->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/OrbitSpike.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(OrbitSpike, direction);
    RSDK_ENUM_VAR("(Unused)", 0);
}
#endif

void OrbitSpike_Serialize(void)
{
    RSDK_EDITABLE_VAR(OrbitSpike, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(OrbitSpike, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(OrbitSpike, VAR_UINT8, offset);
}
